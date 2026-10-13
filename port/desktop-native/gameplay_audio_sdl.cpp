#include "gameplay_audio_sdl.hpp"

#include "android/asset_manager.h"
#include "../android-native/app/src/main/cpp/player_gameplay_audio.hpp"
#include "../engine-audio/native-format/ima_adpcm_step_v1.hpp"

#include <SDL.h>

#include <algorithm>
#include <array>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <mutex>
#include <string_view>
#include <utility>
#include <vector>

namespace dh2::desktop_native {
namespace {
constexpr int kRate = 32000;
constexpr int kChannels = 2;
constexpr int kBlockBytes = 1024;
constexpr int kBlockFrames = 1017;
constexpr int kMusicFadeFrames = kRate * 2;
constexpr std::uint32_t kQ30One = 0x40000000u;

std::uint16_t u16(const std::uint8_t* p) noexcept {
    return static_cast<std::uint16_t>(p[0] | (std::uint16_t(p[1]) << 8));
}
std::uint32_t u32(const std::uint8_t* p) noexcept {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
           (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}

bool read_asset(AAssetManager* manager, const std::string& path,
                std::vector<std::uint8_t>& bytes, std::string& error) {
    std::unique_ptr<AAsset, decltype(&AAsset_close)> asset(
        AAssetManager_open(manager, path.c_str(), AASSET_MODE_BUFFER), &AAsset_close);
    if (!asset) { error = "audio asset is unavailable: " + path; return false; }
    const auto length = AAsset_getLength64(asset.get());
    if (length <= 0 || static_cast<std::uint64_t>(length) >
            std::numeric_limits<std::size_t>::max()) {
        error = "audio asset has an invalid size: " + path; return false;
    }
    bytes.resize(static_cast<std::size_t>(length));
    std::size_t offset = 0;
    while (offset < bytes.size()) {
        const int count = AAsset_read(asset.get(), bytes.data() + offset,
                                      bytes.size() - offset);
        if (count <= 0) { error = "audio asset read was truncated: " + path; return false; }
        offset += static_cast<std::size_t>(count);
    }
    error.clear();
    return true;
}

struct MusicFiles {
    std::uint16_t uid;
    const char* ambient;
    const char* combat; // nullptr for single-track ambient-only rows
    std::uint32_t payload_bytes, sample_frames;
};
// Exported WAVs in original-media/gameplay/music, named by *-music-provenance.json.
// UIDs 485, 486 and 487 have no provenance row; they keep the cathedral crypt WAVs.
constexpr std::array<MusicFiles, 27> kMusicFiles{{
    {467, "swamp_ambient_music.wav", "swamp_combat_music.wav", 6138880, 6096650},
    {468, "swamp_witch_ambient_music.wav", "swamp_witch_combat_music.wav", 6138880, 6096650},
    {470, "water_temple_cave_ambient_music.wav", "water_temple_cave_combat_music.wav", 6138880, 6096650},
    {471, "darkwood_ambient_music.wav", "darkwood_combat_music.wav", 5846016, 5805166},
    {472, "root_den_ambient_music.wav", "root_den_combat_music.wav", 5846016, 5805166},
    {474, "earth_temple_ambient_music.wav", "earth_temple_combat_music.wav", 5846016, 5805166},
    {475, "infected_village_ambient_music.wav", "infected_village_combat_music.wav", 5846016, 5805166},
    {477, "gothicus_no_ambiance_ambient_music.wav", "gothicus_no_ambiance_combat_music.wav", 4757504, 4724364},
    {484, "abbey_safezone_ambient_music.wav", "abbey_safezone_combat_music.wav", 5140480, 5105117},
    {485, "crypt_ambient_music.wav", "crypt_combat_music.wav", 5140480, 5105117},
    {486, "crypt_ambient_music.wav", "crypt_combat_music.wav", 5140480, 5105117},
    {487, "crypt_ambient_music.wav", "crypt_combat_music.wav", 5140480, 5105117},
    {488, "red_desert_ambient_music.wav", "red_desert_combat_music.wav", 4472832, 4441600},
    {489, "red_desert_cave_ambient_music.wav", "red_desert_cave_combat_music.wav", 4472832, 4441600},
    {492, "lighthouse_ambient_music.wav", "lighthouse_combat_music.wav", 4472832, 4441600},
    {493, "fire_temple_ambient_music.wav", "fire_temple_combat_music.wav", 4472832, 4441600},
    {494, "icy_cliff_ambient_music.wav", "icy_cliff_combat_music.wav", 5231616, 5195283},
    {496, "icy_cavern_ambient_music.wav", "icy_cavern_combat_music.wav", 5231616, 5195283},
    {501, "wind_temple_ambient_music.wav", "wind_temple_combat_music.wav", 5231616, 5195283},
    {502, "underworld_ambient_music.wav", "underworld_combat_music.wav", 4198400, 4169150},
    {503, "machine_rooms_ambient_music.wav", "machine_rooms_combat_music.wav", 4198400, 4169150},
    {504, "dark_temple_ambient_music.wav", "dark_temple_combat_music.wav", 4198400, 4169150},
    {505, "swamp_merchant_camp_music.wav", nullptr, 3591168, 3565714},
    {506, "blood_sands_music.wav", nullptr, 3591168, 3565714},
    {507, "kings_grave_music.wav", nullptr, 3591168, 3565716},
    {508, "gated_castle_village_music.wav", nullptr, 3697664, 3671470},
    {509, "northerland_music.wav", nullptr, 3697664, 3671470},
}};

bool parse_music_resource(std::uint16_t uid, std::string_view state,
                          std::string& path, std::uint32_t& payload_bytes,
                          std::uint32_t& sample_frames) {
    const auto* source = player_gameplay_audio::source_music_resource(uid);
    if (!source || (source->has_vxn_states
            ? (state != "ambient" && state != "combat")
            : state != "ambient")) return false;
    const auto row = std::find_if(kMusicFiles.begin(), kMusicFiles.end(),
        [uid](const MusicFiles& entry) { return entry.uid == uid; });
    if (row == kMusicFiles.end()) return false;
    const char* asset = state == "ambient" ? row->ambient : row->combat;
    if (!asset) return false;
    path = std::string("original-media/gameplay/music/") + asset;
    payload_bytes = row->payload_bytes; sample_frames = row->sample_frames;
    return true;
}

struct MusicAsset {
    std::uint16_t uid = 0;
    std::string state, path;
    std::uint32_t frames = 0;
    std::vector<std::uint8_t> encoded;
};

bool decode_music_wav(std::uint16_t uid, std::string_view state,
                      AAssetManager* assets, std::shared_ptr<MusicAsset>& output,
                      std::string& error) {
    std::string path;
    std::uint32_t expected_payload = 0, expected_frames = 0;
    if (!parse_music_resource(uid, state, path, expected_payload, expected_frames)) {
        error = "unsupported source music UID/state"; return false;
    }
    std::vector<std::uint8_t> wav;
    if (!read_asset(assets, path, wav, error)) return false;
    // The RIFF size of single-track source exports is not trusted (it comes from the original
    // container); fmt/fact/data sizes and chunk bounds are validated below.
    if (wav.size() != std::size_t(60) + expected_payload || wav.size() < 60 ||
        std::memcmp(wav.data(), "RIFF", 4) != 0 ||
        std::memcmp(wav.data() + 8, "WAVE", 4) != 0 ||
        std::memcmp(wav.data() + 12, "fmt ", 4) != 0 ||
        u32(wav.data() + 16) != 20 ||
        std::memcmp(wav.data() + 40, "fact", 4) != 0 ||
        u32(wav.data() + 44) != 4 ||
        std::memcmp(wav.data() + 52, "data", 4) != 0 ||
        u32(wav.data() + 56) != expected_payload) {
        error = "source music WAV container is malformed: " + path; return false;
    }
    bool format_ok = false, fact_ok = false, data_ok = false;
    std::uint32_t fact_frames = 0;
    std::vector<std::uint8_t> encoded;
    for (std::size_t at = 12; at + 8 <= wav.size();) {
        const auto chunk_size = u32(wav.data() + at + 4);
        const std::size_t body = at + 8;
        if (chunk_size > wav.size() - body) {
            error = "source music WAV chunk exceeds asset: " + path; return false;
        }
        if (std::memcmp(wav.data() + at, "fmt ", 4) == 0) {
            if (chunk_size != 20 || u16(wav.data() + body) != 17 ||
                u16(wav.data() + body + 2) != kChannels ||
                u32(wav.data() + body + 4) != kRate ||
                u16(wav.data() + body + 12) != kBlockBytes ||
                u16(wav.data() + body + 14) != 4 ||
                u16(wav.data() + body + 16) != 2 ||
                u16(wav.data() + body + 18) != kBlockFrames) {
                error = "source music IMA format differs from the retained decoder: " + path;
                return false;
            }
            format_ok = true;
        } else if (std::memcmp(wav.data() + at, "fact", 4) == 0) {
            if (chunk_size != 4) { error = "source music fact chunk is malformed"; return false; }
            fact_frames = u32(wav.data() + body); fact_ok = true;
        } else if (std::memcmp(wav.data() + at, "data", 4) == 0) {
            encoded.assign(wav.begin() + static_cast<std::ptrdiff_t>(body),
                           wav.begin() + static_cast<std::ptrdiff_t>(body + chunk_size));
            data_ok = true;
        }
        at = body + chunk_size + (chunk_size & 1u);
    }
    if (!format_ok || !fact_ok || !data_ok || encoded.size() != expected_payload ||
        fact_frames != expected_frames || encoded.size() % kBlockBytes != 0 ||
        fact_frames == 0 || fact_frames > (encoded.size() / kBlockBytes) * kBlockFrames) {
        error = "source music WAV facts differ from the retained Android resource: " + path;
        return false;
    }
    auto resource = std::make_shared<MusicAsset>();
    resource->uid = uid; resource->state.assign(state); resource->path = std::move(path);
    resource->frames = fact_frames; resource->encoded = std::move(encoded);
    output = std::move(resource); error.clear(); return true;
}

bool decode_ima_block(const std::uint8_t* input, std::int16_t* output,
                      std::size_t capacity_frames) noexcept {
    if (!input || !output || capacity_frames < kBlockFrames) return false;
    using dh2::engine_audio::native_format::ImaAdpcmStateV1;
    using dh2::engine_audio::native_format::ima_adpcm_decode_nibble_v1;
    std::array<ImaAdpcmStateV1, kChannels> channels{};
    for (int c = 0; c < kChannels; ++c) {
        const int at = c * 4;
        channels[c].predictor = static_cast<std::int16_t>(u16(input + at));
        channels[c].step_index = input[at + 2];
        if (channels[c].step_index > 88) return false;
        output[c] = channels[c].predictor;
    }
    std::array<std::int16_t, 16> group{};
    std::size_t frames = 1;
    for (int block_group = 0; block_group < 127; ++block_group) {
        for (int c = 0; c < kChannels; ++c) {
            const int at = 8 + block_group * 8 + c * 4;
            for (int byte = 0; byte < 4; ++byte) {
                const auto packed = input[at + byte];
                for (int half = 0; half < 2; ++half) {
                    const auto nibble = static_cast<std::uint8_t>(
                        half == 0 ? (packed & 0x0f) : (packed >> 4));
                    group[c * 8 + byte * 2 + half] =
                        ima_adpcm_decode_nibble_v1(&channels[c], nibble);
                }
            }
        }
        for (int i = 0; i < 8; ++i, ++frames) {
            output[frames * 2] = group[i];
            output[frames * 2 + 1] = group[8 + i];
        }
    }
    return frames == kBlockFrames;
}

struct MusicVoice {
    std::shared_ptr<MusicAsset> asset;
    std::uint64_t cursor = 0;
    std::uint32_t gain_q30 = 0, fade_remaining = 0, fade_target_q30 = 0;
    std::int32_t step_q30 = 0;
    bool paused = false, pause_when_silent = false;
    std::uint64_t decoded_block = std::numeric_limits<std::uint64_t>::max();
    std::array<std::int16_t, kBlockFrames * kChannels> decoded{};

    std::int16_t next(int channel) noexcept {
        if (!asset || !asset->frames) return 0;
        if (cursor >= asset->frames) cursor = 0;
        const auto block = cursor / kBlockFrames;
        if (decoded_block != block) {
            const auto offset = block * kBlockBytes;
            if (offset + kBlockBytes > asset->encoded.size() ||
                !decode_ima_block(asset->encoded.data() + offset, decoded.data(), kBlockFrames)) {
                asset.reset(); return 0;
            }
            decoded_block = block;
        }
        const auto at = static_cast<std::size_t>(cursor % kBlockFrames) * kChannels;
        const auto sample = decoded[at + static_cast<std::size_t>(channel)];
        if (channel == kChannels - 1) ++cursor;
        return sample;
    }
};

struct EffectVoice {
    std::uintptr_t owner = 0;
    std::uint16_t sound_id = 0;
    std::vector<std::int16_t> samples;
    std::size_t cursor = 0;
    bool looping = false;
    bool paused = false, pause_when_silent = false;
    float gain = 1.f, target_gain = 1.f, gain_step = 0.f;
    std::uint32_t fade_remaining = 0;
    float left = 1.f, right = 1.f;
};

bool decode_effect_wav(AAssetManager* assets, const std::string& path,
                       std::vector<std::int16_t>& pcm, std::string& error) {
    std::vector<std::uint8_t> file;
    if (!read_asset(assets, path, file, error)) return false;
    if (file.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        error = "WAV asset exceeds SDL's in-memory decoder limit"; return false;
    }
    SDL_AudioSpec source{}; Uint8* loaded = nullptr; Uint32 loaded_length = 0;
    SDL_RWops* rw = SDL_RWFromConstMem(file.data(), static_cast<int>(file.size()));
    if (!rw || !SDL_LoadWAV_RW(rw, 1, &source, &loaded, &loaded_length)) {
        error = std::string("SDL could not decode retained WAV: ") + SDL_GetError();
        return false;
    }
    SDL_AudioCVT cvt{};
    const int conversion = SDL_BuildAudioCVT(&cvt, source.format, source.channels,
        source.freq, AUDIO_S16SYS, kChannels, kRate);
    if (conversion < 0) {
        SDL_FreeWAV(loaded); error = std::string("SDL audio conversion setup failed: ") + SDL_GetError();
        return false;
    }
    if (conversion == 0) {
        if (loaded_length % (sizeof(std::int16_t) * kChannels)) {
            SDL_FreeWAV(loaded); error = "decoded WAV has a partial stereo frame"; return false;
        }
        pcm.resize(loaded_length / sizeof(std::int16_t));
        std::memcpy(pcm.data(), loaded, loaded_length);
    } else {
        if (loaded_length > static_cast<Uint32>(std::numeric_limits<int>::max()) ||
            cvt.len_mult <= 0 || loaded_length >
                static_cast<Uint32>(std::numeric_limits<int>::max() / cvt.len_mult)) {
            SDL_FreeWAV(loaded); error = "WAV conversion size overflow"; return false;
        }
        cvt.len = static_cast<int>(loaded_length);
        cvt.buf = static_cast<Uint8*>(SDL_malloc(static_cast<std::size_t>(cvt.len) * cvt.len_mult));
        if (!cvt.buf) { SDL_FreeWAV(loaded); error = "WAV conversion allocation failed"; return false; }
        std::memcpy(cvt.buf, loaded, loaded_length); SDL_FreeWAV(loaded);
        if (SDL_ConvertAudio(&cvt) != 0 || cvt.len_cvt < 0 ||
            cvt.len_cvt % (sizeof(std::int16_t) * kChannels)) {
            SDL_free(cvt.buf); error = std::string("WAV conversion failed: ") + SDL_GetError();
            return false;
        }
        pcm.resize(static_cast<std::size_t>(cvt.len_cvt) / sizeof(std::int16_t));
        std::memcpy(pcm.data(), cvt.buf, static_cast<std::size_t>(cvt.len_cvt));
        SDL_free(cvt.buf);
    }
    if (pcm.empty() || pcm.size() % kChannels) {
        error = "retained WAV decoded to no complete stereo frames"; return false;
    }
    error.clear(); return true;
}

bool parse_integer(std::string_view text, std::int64_t& value) noexcept {
    const auto result = std::from_chars(text.data(), text.data() + text.size(), value);
    return result.ec == std::errc{} && result.ptr == text.data() + text.size();
}

bool retained_effect_filename(std::uint16_t sound_id, std::string_view filename) noexcept {
    const auto* match = player_gameplay_audio::find_uid(sound_id);
    return match && filename == match->filename;
}

bool split_command(const std::string& text, std::array<std::string_view, 10>& fields,
                   std::size_t& count) noexcept {
    count = 0; std::size_t begin = 0;
    while (begin <= text.size() && count < fields.size()) {
        const auto end = text.find(',', begin);
        fields[count++] = std::string_view(text).substr(begin,
            end == std::string::npos ? text.size() - begin : end - begin);
        if (end == std::string::npos) return true;
        begin = end + 1;
    }
    return false;
}

std::int16_t saturate(std::int64_t sample) noexcept {
    return static_cast<std::int16_t>(std::clamp<std::int64_t>(sample, -32768, 32767));
}
} // namespace

struct GameplayAudioSdl::Impl {
    SDL_AudioDeviceID device = 0;
    SDL_AudioSpec obtained{};
    std::mutex mutex;
    std::vector<EffectVoice> effects;
    std::array<MusicVoice, 3> music{}; // Android owner slots: dying, old, current.
    float music_volume = 1.f, effect_volume = 1.f;
    bool native_audio_paused = false, native_music_paused = false;

    static void fade_music(MusicVoice& voice, std::uint32_t target,
                           std::uint32_t frames, bool pause_at_zero) noexcept {
        if (!voice.asset) return;
        voice.paused = false;
        voice.fade_target_q30 = target;
        voice.fade_remaining = frames;
        voice.step_q30 = frames ? static_cast<std::int32_t>(
            (std::int64_t(target) - voice.gain_q30) / frames) : 0;
        voice.pause_when_silent = pause_at_zero;
        if (!frames) {
            voice.gain_q30 = target;
            voice.step_q30 = 0;
            if (pause_at_zero && !target) voice.paused = true;
        }
    }

    void pause_effects(std::uint32_t frames) noexcept {
        for (auto& voice : effects) {
            if (voice.paused) continue;
            if (voice.pause_when_silent && voice.fade_remaining <= frames) continue;
            voice.target_gain = 0.f;
            voice.fade_remaining = frames;
            voice.gain_step = frames ? -voice.gain / static_cast<float>(frames) : 0.f;
            voice.pause_when_silent = true;
            if (!frames) { voice.gain = 0.f; voice.paused = true; }
        }
    }

    void resume_effects(std::uint32_t frames) noexcept {
        for (auto& voice : effects) {
            if (!voice.paused && !voice.pause_when_silent) continue;
            voice.paused = false;
            voice.pause_when_silent = false;
            voice.target_gain = 1.f;
            voice.fade_remaining = frames;
            voice.gain_step = frames ? (1.f - voice.gain) / static_cast<float>(frames) : 0.f;
            if (!frames) voice.gain = 1.f;
        }
    }

    void pause_music_voices(std::uint32_t frames, bool current_only) noexcept {
        const std::size_t first = current_only ? 2 : 0;
        for (std::size_t i = first; i < music.size(); ++i) {
            auto& voice = music[i];
            if (!voice.asset) continue;
            if (voice.pause_when_silent && voice.fade_remaining <= frames) continue;
            fade_music(voice, 0, frames, true);
        }
    }

    void resume_music_voices(std::uint32_t frames) noexcept {
        for (auto& voice : music) {
            if (!voice.asset || (!voice.paused && !voice.pause_when_silent)) continue;
            voice.pause_when_silent = false;
            fade_music(voice, kQ30One, frames, false);
        }
    }

    static void callback(void* raw, Uint8* stream, int length) noexcept {
        auto* self = static_cast<Impl*>(raw);
        if (!stream || length <= 0) return;
        std::memset(stream, 0, static_cast<std::size_t>(length));
        if (!self || self->obtained.format != AUDIO_S16SYS ||
            self->obtained.channels != kChannels) return;
        auto* out = reinterpret_cast<std::int16_t*>(stream);
        const std::size_t frames = static_cast<std::size_t>(length) /
                                   (sizeof(std::int16_t) * kChannels);
        std::lock_guard<std::mutex> lock(self->mutex);
        for (std::size_t frame = 0; frame < frames; ++frame) {
            std::int64_t left = 0, right = 0;
            {
                for (auto& voice : self->music) {
                    if (voice.asset && !voice.paused) {
                        const auto high_gain = static_cast<std::int32_t>(voice.gain_q30 >> 15);
                        left += static_cast<std::int64_t>(
                            (std::int32_t(voice.next(0)) * high_gain) >> 15) * self->music_volume;
                        right += static_cast<std::int64_t>(
                            (std::int32_t(voice.next(1)) * high_gain) >> 15) * self->music_volume;
                    }
                    if (voice.fade_remaining && --voice.fade_remaining == 0) {
                        voice.gain_q30 = voice.fade_target_q30; voice.step_q30 = 0;
                        if (!voice.gain_q30) {
                            if (voice.pause_when_silent) voice.paused = true;
                            else voice.asset.reset();
                        }
                    } else if (voice.fade_remaining) {
                        voice.gain_q30 = static_cast<std::uint32_t>(
                            std::int64_t(voice.gain_q30) + voice.step_q30);
                    }
                }
            }
            for (auto it = self->effects.begin(); it != self->effects.end();) {
                if (it->paused) {
                    if (it->fade_remaining && --it->fade_remaining == 0) {
                        it->gain = it->target_gain; it->gain_step = 0.f;
                        if (it->target_gain == 0.f && !it->pause_when_silent) {
                            it = self->effects.erase(it); continue;
                        }
                    } else if (it->fade_remaining) it->gain += it->gain_step;
                    ++it; continue;
                }
                if (it->samples.empty() || it->cursor + 1 >= it->samples.size()) {
                    if (it->looping && !it->samples.empty()) it->cursor = 0;
                    else { it = self->effects.erase(it); continue; }
                }
                left += static_cast<std::int64_t>(it->samples[it->cursor]) *
                        it->gain * it->left * self->effect_volume;
                right += static_cast<std::int64_t>(it->samples[it->cursor + 1]) *
                         it->gain * it->right * self->effect_volume;
                it->cursor += 2;
                if (it->fade_remaining && --it->fade_remaining == 0) {
                    it->gain = it->target_gain; it->gain_step = 0.f;
                    if (it->target_gain == 0.f) {
                        if (it->pause_when_silent) { it->paused = true; ++it; continue; }
                        it = self->effects.erase(it); continue;
                    }
                } else if (it->fade_remaining) it->gain += it->gain_step;
                ++it;
            }
            out[frame * 2] = saturate(left);
            out[frame * 2 + 1] = saturate(right);
        }
    }

    bool add_effect(AAssetManager* assets, std::uintptr_t owner, std::uint16_t sound_id,
                    std::string_view requested_filename,
                    bool looping, std::int32_t fade_ms, std::uint16_t left_q14,
                    std::uint16_t right_q14, std::string& error) {
        const auto* match = player_gameplay_audio::find_uid(sound_id);
        if (!match) {
            error = "gameplay effect UID is not in the retained source catalog"; return false;
        }
        if (requested_filename != match->filename) {
            error = "gameplay effect filename does not match the retained source UID"; return false;
        }
        std::vector<std::int16_t> pcm;
        if (!decode_effect_wav(assets, "original-media/gameplay/" +
                std::string(match->filename), pcm, error)) return false;
        return add_effect_pcm(owner, sound_id, std::move(pcm), looping, fade_ms,
                              left_q14, right_q14, error);
    }

    bool add_menu_effect(AAssetManager* assets, std::string_view filename,
                         std::string& error) {
        static constexpr std::array<std::string_view, 5> allowed{{
            "sfx_menu_back.wav", "sfx_menu_confirm.wav", "sfx_menu_select.wav",
            "sfx_menu_spending.wav", "sfx_menu_tab.wav"}};
        if (std::find(allowed.begin(), allowed.end(), filename) == allowed.end()) {
            error = "unsupported retained menu effect filename"; return false;
        }
        std::vector<std::int16_t> pcm;
        if (!decode_effect_wav(assets, "original-media/" + std::string(filename), pcm, error))
            return false;
        return add_effect_pcm(0, 0xffff, std::move(pcm), false, 0, 16384, 16384, error);
    }

    bool add_effect_pcm(std::uintptr_t owner, std::uint16_t sound_id,
                        std::vector<std::int16_t> pcm, bool looping,
                        std::int32_t fade_ms, std::uint16_t left_q14,
                        std::uint16_t right_q14, std::string& error) {
        if (pcm.empty() || pcm.size() % kChannels) {
            error = "retained effect has no complete stereo frames"; return false;
        }
        EffectVoice voice; voice.owner = owner; voice.sound_id = sound_id;
        voice.samples = std::move(pcm); voice.looping = looping;
        voice.left = float(left_q14) / 16384.f; voice.right = float(right_q14) / 16384.f;
        if (fade_ms > 0) {
            const auto frames = static_cast<std::uint64_t>(fade_ms) * kRate / 1000;
            voice.gain = 0.f; voice.target_gain = 1.f;
            voice.fade_remaining = static_cast<std::uint32_t>(std::clamp<std::uint64_t>(
                frames, 1, std::numeric_limits<std::uint32_t>::max()));
            voice.gain_step = (voice.target_gain - voice.gain) / voice.fade_remaining;
        }
        std::lock_guard<std::mutex> lock(mutex);
        if (effects.size() >= player_gameplay_audio::queue_limit) {
            error = "desktop active gameplay effect capacity reached"; return false;
        }
        effects.push_back(std::move(voice)); error.clear(); return true;
    }

    bool menu_audio_command(std::string_view command, std::string& error) {
        if (command == "resume") {
            std::lock_guard<std::mutex> lock(mutex);
            native_audio_paused = false; native_music_paused = false;
            resume_effects(kRate / 5); resume_music_voices(kRate / 5);
            error.clear(); return true;
        }
        if (command == "pause-all" || command == "resume-all" ||
            command == "pause-music") {
            std::lock_guard<std::mutex> lock(mutex);
            constexpr std::uint32_t kAllEmitterFadeFrames = kRate / 5;
            constexpr std::uint32_t kSingleEmitterFadeFrames = kRate / 20;
            if (command == "pause-all") {
                native_audio_paused = true;
                native_music_paused = false;
                pause_effects(kAllEmitterFadeFrames);
                pause_music_voices(kAllEmitterFadeFrames, false);
            }
            else if (command == "pause-music") {
                if (!music[2].asset) { error.clear(); return true; }
                native_music_paused = true;
                pause_music_voices(kSingleEmitterFadeFrames, true);
            }
            else {
                native_audio_paused = false; native_music_paused = false;
                resume_effects(kAllEmitterFadeFrames);
                resume_music_voices(kAllEmitterFadeFrames);
            }
            error.clear(); return true;
        }
        const auto first_comma = command.find(',');
        const auto second_comma = first_comma == std::string_view::npos
            ? std::string_view::npos : command.find(',', first_comma + 1);
        std::int64_t music = 0, effects_gain = 0;
        if (first_comma == std::string_view::npos ||
            second_comma == std::string_view::npos ||
            command.find(',', second_comma + 1) != std::string_view::npos ||
            command.substr(0, first_comma) != "volume" ||
            !parse_integer(command.substr(first_comma + 1,
                second_comma - first_comma - 1), music) ||
            !parse_integer(command.substr(second_comma + 1), effects_gain)) {
            error = "unsupported or malformed menu audio control"; return false;
        }
        const auto bounded_music = std::clamp<std::int64_t>(music, 0, 100);
        const auto bounded_effects = std::clamp<std::int64_t>(effects_gain, 0, 100);
        std::lock_guard<std::mutex> lock(mutex);
        music_volume = static_cast<float>(bounded_music) / 100.f;
        effect_volume = static_cast<float>(bounded_effects) / 100.f;
        error.clear(); return true;
    }

    void stop_effect(std::uintptr_t owner, std::uint16_t sound_id, std::int32_t fade_ms) {
        std::lock_guard<std::mutex> lock(mutex);
        for (auto it = effects.begin(); it != effects.end();) {
            if (it->owner != owner || it->sound_id != sound_id) { ++it; continue; }
            if (fade_ms <= 0) { it = effects.erase(it); continue; }
            it->pause_when_silent = false;
            const auto frames = static_cast<std::uint64_t>(fade_ms) * kRate / 1000;
            it->target_gain = 0.f;
            it->fade_remaining = static_cast<std::uint32_t>(std::clamp<std::uint64_t>(
                frames, 1, std::numeric_limits<std::uint32_t>::max()));
            it->gain_step = (it->target_gain - it->gain) / it->fade_remaining;
            ++it;
        }
    }

    void stop_all_sounds(std::int32_t fade_ms) {
        std::lock_guard<std::mutex> lock(mutex);
        if (fade_ms <= 0) { effects.clear(); music = {}; return; }
        const auto frames64 = static_cast<std::uint64_t>(fade_ms) * kRate / 1000;
        const auto frames = static_cast<std::uint32_t>(std::clamp<std::uint64_t>(
            frames64, 1, std::numeric_limits<std::uint32_t>::max()));
        for (auto& voice : effects) {
            voice.target_gain = 0.f; voice.pause_when_silent = false;
            voice.fade_remaining = frames;
            voice.gain_step = -voice.gain / static_cast<float>(frames);
        }
        for (auto& voice : music) {
            if (!voice.asset) continue;
            voice.fade_target_q30 = 0; voice.pause_when_silent = false;
            voice.fade_remaining = frames;
            voice.step_q30 = static_cast<std::int32_t>(
                -static_cast<std::int64_t>(voice.gain_q30) / frames);
        }
    }

    void select_music(std::shared_ptr<MusicAsset> selected) {
        std::lock_guard<std::mutex> lock(mutex);
        auto& current = music[2];
        if (current.asset && current.asset->state == selected->state &&
            current.asset->path == selected->path) return;
        if (!current.asset && !music[1].asset && !music[0].asset) {
            current.asset = std::move(selected); current.gain_q30 = 0;
            current.fade_target_q30 = kQ30One; current.fade_remaining = kMusicFadeFrames;
            current.step_q30 = static_cast<std::int32_t>(kQ30One / kMusicFadeFrames);
            return;
        }
        music[0] = MusicVoice{};
        music[0] = std::move(music[1]); music[1] = std::move(current);
        current = MusicVoice{}; current.asset = std::move(selected);
        current.fade_target_q30 = kQ30One; current.fade_remaining = kMusicFadeFrames;
        current.step_q30 = kQ30One / kMusicFadeFrames;
        for (std::size_t slot = 0; slot < 2; ++slot) {
            auto& voice = music[slot];
            if (!voice.asset) continue;
            voice.fade_target_q30 = 0; voice.fade_remaining = kMusicFadeFrames;
            voice.step_q30 = static_cast<std::int32_t>(
                (std::int64_t(0) - voice.gain_q30) / kMusicFadeFrames);
        }
    }
};

GameplayAudioSdl::GameplayAudioSdl() : impl_(std::make_unique<Impl>()) {}
GameplayAudioSdl::~GameplayAudioSdl() { close(); }

bool GameplayAudioSdl::open(std::string& error) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    if (impl_->device) { error.clear(); return true; }
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        error = std::string("SDL audio subsystem unavailable: ") + SDL_GetError(); return false;
    }
    SDL_AudioSpec wanted{}; wanted.freq = kRate; wanted.format = AUDIO_S16SYS;
    wanted.channels = kChannels; wanted.samples = 1024; wanted.callback = Impl::callback;
    wanted.userdata = impl_.get();
    impl_->device = SDL_OpenAudioDevice(nullptr, 0, &wanted, &impl_->obtained, 0);
    if (!impl_->device || impl_->obtained.freq != kRate ||
        impl_->obtained.format != AUDIO_S16SYS || impl_->obtained.channels != kChannels) {
        error = std::string("SDL stereo 32 kHz PCM16 device unavailable: ") + SDL_GetError();
        if (impl_->device) SDL_CloseAudioDevice(impl_->device);
        impl_->device = 0; SDL_QuitSubSystem(SDL_INIT_AUDIO); return false;
    }
    SDL_PauseAudioDevice(impl_->device, 0); error.clear(); return true;
}

void GameplayAudioSdl::pump(AAssetManager* assets) {
    if (!impl_ || !assets) return;
    std::string serialized;
    while (player_gameplay_audio::consume(serialized)) {
        // Keep source gameplay callbacks non-blocking if the host has no
        // usable audio device. Consume and discard only this presentation cue.
        if (!impl_->device) continue;
        std::array<std::string_view, 10> fields{}; std::size_t count = 0;
        if (!split_command(serialized, fields, count)) continue;
        std::int64_t values[6]{};
        if (count == 3 && fields[0] == "dh2music") {
            if (!parse_integer(fields[1], values[0]) || values[0] < 0 || values[0] > 65535) continue;
            std::shared_ptr<MusicAsset> resource; std::string error;
            if (!decode_music_wav(static_cast<std::uint16_t>(values[0]), fields[2],
                                  assets, resource, error)) {
                SDL_Log("DH2 gameplay music ignored: %s", error.c_str()); continue;
            }
            impl_->select_music(std::move(resource));
        } else if ((count == 7 || count == 10) && fields[0] == "dh2fx" && fields[1] == "play") {
            bool valid = true;
            for (int i = 0; i < 3; ++i) valid = valid && parse_integer(fields[i + 2], values[i]);
            if (!valid || (fields[5] != "0" && fields[5] != "1") || values[0] < 0 ||
                static_cast<std::uint64_t>(values[0]) > std::numeric_limits<std::uintptr_t>::max() ||
                values[1] < 0 || values[1] > 65535 || values[2] < std::numeric_limits<std::int32_t>::min() ||
                values[2] > std::numeric_limits<std::int32_t>::max() ||
                !retained_effect_filename(static_cast<std::uint16_t>(values[1]), fields[6])) continue;
            std::uint16_t left = 16384, right = 16384;
            if (count == 10) {
                if (fields[7] != "1" || !parse_integer(fields[8], values[3]) ||
                    !parse_integer(fields[9], values[4]) || values[3] < 0 || values[3] > 16384 ||
                    values[4] < 0 || values[4] > 16384) continue;
                left = static_cast<std::uint16_t>(values[3]); right = static_cast<std::uint16_t>(values[4]);
            }
            std::string error;
            if (!impl_->add_effect(assets, static_cast<std::uintptr_t>(values[0]),
                    static_cast<std::uint16_t>(values[1]), fields[6], fields[5] == "1",
                    static_cast<std::int32_t>(values[2]), left, right, error))
                SDL_Log("DH2 gameplay effect ignored: %s", error.c_str());
        } else if (count == 5 && fields[0] == "dh2fx" && fields[1] == "stop") {
            if (!parse_integer(fields[2], values[0]) || !parse_integer(fields[3], values[1]) ||
                !parse_integer(fields[4], values[2]) || values[0] < 0 ||
                static_cast<std::uint64_t>(values[0]) > std::numeric_limits<std::uintptr_t>::max() || values[1] < 0 ||
                values[1] > 65535 || values[2] < std::numeric_limits<std::int32_t>::min() ||
                values[2] > std::numeric_limits<std::int32_t>::max()) continue;
            impl_->stop_effect(static_cast<std::uintptr_t>(values[0]),
                static_cast<std::uint16_t>(values[1]), static_cast<std::int32_t>(values[2]));
        } else if (count == 3 && fields[0] == "dh2fx" && fields[1] == "stopAll") {
            if (!parse_integer(fields[2], values[0]) || values[0] < 0 ||
                values[0] > 60000) continue;
            impl_->stop_all_sounds(static_cast<std::int32_t>(values[0]));
        }
    }
}

bool GameplayAudioSdl::play_menu_sound(AAssetManager* assets,
                                      std::string_view filename,
                                      std::string& error) {
    if (!impl_ || !impl_->device) {
        error = "SDL audio output is unavailable for the requested menu cue";
        return false;
    }
    return impl_->add_menu_effect(assets, filename, error);
}

bool GameplayAudioSdl::apply_menu_audio_command(std::string_view command,
                                                std::string& error) {
    if (!impl_) { error = "SDL gameplay audio owner is unavailable"; return false; }
    return impl_->menu_audio_command(command, error);
}

void GameplayAudioSdl::close() noexcept {
    if (!impl_) return;
    if (impl_->device) {
        SDL_PauseAudioDevice(impl_->device, 1);
        SDL_CloseAudioDevice(impl_->device); impl_->device = 0;
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
    std::lock_guard<std::mutex> lock(impl_->mutex);
    impl_->effects.clear(); impl_->music = {};
}

bool GameplayAudioSdl::available() const noexcept { return impl_ && impl_->device; }

} // namespace dh2::desktop_native
