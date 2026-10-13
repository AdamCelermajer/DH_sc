#pragma once
// Consumer of the shared gameplay audio queue (player_gameplay_audio.hpp).
// It turns the queue's `dh2fx,...` wire commands into Mixer voices: Sounds.xml
// UID -> staged WAV (decoded once, cached), Sounds.xml bank/priority/loop,
// positional Q14 gains, fades, StopSound/StopAllSounds and the authored
// pause/resume/volume controls. Platform-free so a host test drives it with
// in-memory assets; gameplay_sfx_android.cpp supplies AAssetManager + OpenSL ES.
#include "gameplay_sfx_mixer_v1.hpp"
#include "player_gameplay_audio.hpp"

#include <algorithm>
#include <array>
#include <charconv>
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace dh2::gameplay_sfx {

enum class Outcome : std::uint8_t {
    started,         // a voice was admitted
    refused_by_bank, // vox priority bank refused the emitter
    stopped,         // stop / stop-all applied
    control,         // pause / resume / volume applied
    ignored,         // valid command with nothing to do (e.g. pause-music)
    invalid,         // malformed or inconsistent with the retained catalog
    load_failed,     // asset missing or not decodable
    voice_limit,
};

struct Result {
    Outcome outcome = Outcome::invalid;
    std::uint16_t uid = 0;
    std::string detail;
};

class Engine {
public:
    // Reads one packaged asset ("original-media/gameplay/<file>").
    using AssetReader = std::function<bool(const std::string& path,
                                           std::vector<std::uint8_t>& bytes,
                                           std::string& error)>;

    Engine(std::uint32_t output_rate, AssetReader reader,
           std::size_t cache_budget_bytes = 48u * 1024u * 1024u)
        : mixer_(output_rate, banks()), reader_(std::move(reader)),
          budget_(cache_budget_bytes) {}

    Mixer& mixer() noexcept { return mixer_; }

    // One serialized queue command. Safe to call from any single control
    // thread; render() may run concurrently on the audio thread.
    Result command(std::string_view text) {
        std::array<std::string_view, 10> fields{};
        std::size_t count = 0;
        if (!split(text, fields, count)) return fail("too many fields");
        if (count < 2 || fields[0] != "dh2fx") return fail("not a dh2fx command");
        if (fields[1] == "play" && (count == 7 || count == 10)) return play(fields, count);
        if (fields[1] == "menu" && count == 3) return menu(fields[2]);
        if (fields[1] == "stop" && count == 5) return stop(fields);
        if (fields[1] == "stopAll" && count == 3) {
            std::int64_t fade = 0;
            if (!number(fields[2], fade) || fade < 0 || fade > 60000)
                return fail("stopAll fade out of range");
            mixer_.stop_all(static_cast<std::int32_t>(fade));
            return {Outcome::stopped, 0, {}};
        }
        return fail("unknown dh2fx command");
    }

    // Authored menu audio controls (OriginalUiSession::consume_menu_audio).
    Result control(std::string_view text) {
        constexpr std::int32_t kAllEmitterFadeMs = 200; // NativePause/ResumeAllSounds
        if (text == "pause-all") {
            mixer_.pause_all(kAllEmitterFadeMs);
            return {Outcome::control, 0, {}};
        }
        if (text == "resume-all" || text == "resume") {
            mixer_.resume_all(kAllEmitterFadeMs);
            return {Outcome::control, 0, {}};
        }
        if (text == "pause-music" || text == "title") return {Outcome::ignored, 0, {}};
        const auto first = text.find(',');
        const auto second = first == std::string_view::npos ? first : text.find(',', first + 1);
        std::int64_t music = 0, fx = 0;
        if (first == std::string_view::npos || second == std::string_view::npos ||
            text.substr(0, first) != "volume" ||
            text.find(',', second + 1) != std::string_view::npos ||
            !number(text.substr(first + 1, second - first - 1), music) ||
            !number(text.substr(second + 1), fx))
            return fail("unsupported audio control");
        // VoxSoundManager::SetSoundVolume(1, VolumeFX): SFX group mask gain =
        // option / 100, clamped to [0,1].
        mixer_.set_master_volume(static_cast<float>(std::clamp<std::int64_t>(fx, 0, 100)) / 100.0f);
        return {Outcome::control, 0, {}};
    }

    void render(std::int16_t* out, std::size_t frames) { mixer_.render(out, frames); }

    std::size_t cached_bytes() const noexcept { return cached_bytes_; }
    std::size_t cached_files() const noexcept { return cache_.size(); }

private:
    struct CacheEntry {
        std::shared_ptr<const Pcm> pcm;
        std::uint64_t last_use = 0;
        std::size_t bytes = 0;
    };

    static std::vector<BankRule> banks() {
        std::vector<BankRule> rules;
        for (const auto& row : player_gameplay_audio::sfx_banks) {
            BankRule rule;
            rule.max_playbacks = row.max_playbacks;
            rule.behaviour = row.behaviour;
            rules.push_back(rule);
        }
        return rules;
    }
    static bool number(std::string_view text, std::int64_t& value) noexcept {
        const auto r = std::from_chars(text.data(), text.data() + text.size(), value);
        return r.ec == std::errc{} && r.ptr == text.data() + text.size();
    }
    static bool split(std::string_view text, std::array<std::string_view, 10>& fields,
                      std::size_t& count) noexcept {
        count = 0;
        std::size_t begin = 0;
        while (begin <= text.size() && count < fields.size()) {
            const auto end = text.find(',', begin);
            fields[count++] = text.substr(begin, end == std::string_view::npos
                                                     ? text.size() - begin : end - begin);
            if (end == std::string_view::npos) return true;
            begin = end + 1;
        }
        return false;
    }
    static Result fail(const char* why) { return {Outcome::invalid, 0, why}; }

    std::shared_ptr<const Pcm> load(const std::string& path, std::string& error) {
        const auto it = cache_.find(path);
        if (it != cache_.end()) {
            it->second.last_use = ++clock_;
            return it->second.pcm;
        }
        std::vector<std::uint8_t> bytes;
        if (!reader_ || !reader_(path, bytes, error)) {
            if (error.empty()) error = "asset unavailable: " + path;
            return {};
        }
        auto pcm = std::make_shared<Pcm>();
        if (!decode_wav(bytes.data(), bytes.size(), *pcm, error)) {
            error += ": " + path;
            return {};
        }
        CacheEntry entry;
        entry.bytes = pcm->samples.size() * sizeof(std::int16_t);
        entry.pcm = pcm;
        entry.last_use = ++clock_;
        cached_bytes_ += entry.bytes;
        cache_.emplace(path, std::move(entry));
        evict(path);
        return pcm;
    }
    // Drop least-recently-used decoded samples no live voice references.
    void evict(const std::string& keep) {
        while (cached_bytes_ > budget_) {
            auto victim = cache_.end();
            for (auto it = cache_.begin(); it != cache_.end(); ++it) {
                if (it->first == keep || it->second.pcm.use_count() > 1) continue;
                if (victim == cache_.end() || it->second.last_use < victim->second.last_use)
                    victim = it;
            }
            if (victim == cache_.end()) return;
            cached_bytes_ -= victim->second.bytes;
            cache_.erase(victim);
        }
    }

    Result play(const std::array<std::string_view, 10>& f, std::size_t count) {
        std::int64_t owner = 0, uid = 0, fade = 0;
        if (!number(f[2], owner) || !number(f[3], uid) || !number(f[4], fade) ||
            (f[5] != "0" && f[5] != "1") || owner < 0 ||
            static_cast<std::uint64_t>(owner) > std::numeric_limits<std::uintptr_t>::max() ||
            uid < 0 || uid > 65535 || fade < std::numeric_limits<std::int32_t>::min() ||
            fade > std::numeric_limits<std::int32_t>::max())
            return fail("malformed play command");
        const auto* entry = player_gameplay_audio::find_uid(static_cast<std::uint16_t>(uid));
        if (!entry) return fail("UID is not in the retained catalog");
        if (f[6] != entry->filename) return fail("filename differs from the retained UID");
        std::int64_t left = 16384, right = 16384;
        const bool spatial = count == 10;
        if (spatial && (f[7] != "1" || !number(f[8], left) || !number(f[9], right) ||
                        left < 0 || left > 16384 || right < 0 || right > 16384))
            return fail("malformed spatial mix");
        const auto id = static_cast<std::uint16_t>(uid);
        const auto* def = player_gameplay_audio::find_sfx_def(id);
        std::string error;
        auto pcm = load(std::string("original-media/gameplay/") + entry->filename, error);
        if (!pcm) return {Outcome::load_failed, id, error};
        VoiceRequest request;
        request.owner = static_cast<std::uintptr_t>(owner);
        request.uid = id;
        request.pcm = std::move(pcm);
        // Vox::Play receives the Sounds.xml loop flag, not the caller's.
        request.loop = def ? def->loop : f[5] == "1";
        // Play3D starts every emitter with a 0.05 s fade-in (0x36a9b4).
        request.fade_in_ms = static_cast<std::int32_t>(std::clamp<std::int64_t>(
            fade, spatial ? 50 : 0, 60000));
        request.left = static_cast<float>(left) / 16384.0f;
        request.right = static_cast<float>(right) / 16384.0f;
        request.bank = def ? def->bank : 0;
        request.priority = def ? def->priority : 0;
        return finish(mixer_.play(request), id);
    }

    // Front-menu effects are the five sfx_menu_* files staged at original-media/:
    // Sounds.xml bank 6 (INT, steal oldest), priority 2, group sfx_int.
    Result menu(std::string_view file) {
        static constexpr std::array<std::string_view, 5> allowed{{
            "sfx_menu_back.wav", "sfx_menu_confirm.wav", "sfx_menu_select.wav",
            "sfx_menu_spending.wav", "sfx_menu_tab.wav"}};
        if (std::find(allowed.begin(), allowed.end(), file) == allowed.end())
            return fail("unsupported menu effect");
        std::string error;
        auto pcm = load("original-media/" + std::string(file), error);
        if (!pcm) return {Outcome::load_failed, 0, error};
        VoiceRequest request;
        request.pcm = std::move(pcm);
        request.bank = 6;
        request.priority = 2;
        return finish(mixer_.play(request), 0);
    }

    Result stop(const std::array<std::string_view, 10>& f) {
        std::int64_t owner = 0, uid = 0, fade = 0;
        if (!number(f[2], owner) || !number(f[3], uid) || !number(f[4], fade) || owner < 0 ||
            static_cast<std::uint64_t>(owner) > std::numeric_limits<std::uintptr_t>::max() ||
            uid < 0 || uid > 65535 || fade < std::numeric_limits<std::int32_t>::min() ||
            fade > std::numeric_limits<std::int32_t>::max())
            return fail("malformed stop command");
        mixer_.stop(static_cast<std::uintptr_t>(owner), static_cast<std::uint16_t>(uid),
                    static_cast<std::int32_t>(fade));
        return {Outcome::stopped, static_cast<std::uint16_t>(uid), {}};
    }

    static Result finish(PlayResult result, std::uint16_t uid) {
        switch (result) {
        case PlayResult::started: return {Outcome::started, uid, {}};
        case PlayResult::refused_by_bank: return {Outcome::refused_by_bank, uid, {}};
        case PlayResult::voice_limit: return {Outcome::voice_limit, uid, {}};
        case PlayResult::invalid: break;
        }
        return {Outcome::invalid, uid, "mixer rejected the voice"};
    }

    Mixer mixer_;
    AssetReader reader_;
    std::size_t budget_;
    std::unordered_map<std::string, CacheEntry> cache_;
    std::size_t cached_bytes_ = 0;
    std::uint64_t clock_ = 0;
};

} // namespace dh2::gameplay_sfx
