#pragma once
// Software mixer for the original gameplay sound effects. Pure C++17 (no
// Android, SDL or JNI types) so the exact playback semantics are exercised by a
// host test and shared by any output adapter.
//
// Vox semantics reproduced (libDungeonHunter2.so):
//   * Priority banks: vox::PriorityBankManager::_CanAddEmitter 0x863578 and
//     AddEmitter 0x86ea54 -- per-bank maxplaybacks with four behaviours
//     (steal oldest / steal lowest priority / steal low prio or oldest at equal
//     priority / refuse). Higher priority numbers win; a stolen emitter is
//     stopped immediately (EmitterObj::Stop(0.0)).
//   * Sounds.xml loop flag is the one handed to VoxEngine::Play (0x8621c0).
//   * Group gain: VoxEngine::SetGroupGain 0x86b9b8 ramps over 0.05 s.
//   * Pause/resume use the 0.2 s fades of the authored NativePauseAllSounds
//     path; Stop(fade) fades linearly and releases the voice at its end.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace dh2::gameplay_sfx {

struct Pcm {
    std::vector<std::int16_t> samples; // interleaved
    std::uint32_t frames = 0;
    std::uint8_t channels = 0;
    std::uint32_t rate = 0;
};

namespace detail {
inline std::uint32_t le32(const std::uint8_t* p) noexcept {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
           (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}
inline std::uint16_t le16(const std::uint8_t* p) noexcept {
    return static_cast<std::uint16_t>(p[0] | (std::uint16_t(p[1]) << 8));
}
} // namespace detail

// RIFF/WAVE, format tag 1 (PCM), 16-bit, one or two channels. The cache's SFX
// WAVs are all of this form; anything else is rejected rather than guessed.
inline bool decode_wav(const std::uint8_t* data, std::size_t size, Pcm& out,
                       std::string& error) {
    if (!data || size < 12 || std::memcmp(data, "RIFF", 4) != 0 ||
        std::memcmp(data + 8, "WAVE", 4) != 0) {
        error = "not a RIFF/WAVE file";
        return false;
    }
    bool have_format = false;
    std::uint16_t channels = 0, bits = 0;
    std::uint32_t rate = 0;
    for (std::size_t at = 12; at + 8 <= size;) {
        const std::uint32_t chunk = detail::le32(data + at + 4);
        const std::size_t body = at + 8;
        const std::size_t available = size - body;
        if (std::memcmp(data + at, "fmt ", 4) == 0) {
            if (chunk < 16 || available < 16) {
                error = "fmt chunk is truncated";
                return false;
            }
            if (detail::le16(data + body) != 1) {
                error = "WAVE is not integer PCM";
                return false;
            }
            channels = detail::le16(data + body + 2);
            rate = detail::le32(data + body + 4);
            bits = detail::le16(data + body + 14);
            have_format = true;
        } else if (std::memcmp(data + at, "data", 4) == 0) {
            if (!have_format) {
                error = "data chunk precedes fmt";
                return false;
            }
            if (bits != 16 || (channels != 1 && channels != 2) || rate < 8000 ||
                rate > 192000) {
                error = "unsupported WAVE sample format";
                return false;
            }
            const std::size_t bytes = std::min<std::size_t>(chunk, available);
            const std::size_t frames = bytes / (2u * channels);
            if (!frames) {
                error = "WAVE has no sample frames";
                return false;
            }
            out.samples.resize(frames * channels);
            for (std::size_t i = 0; i < out.samples.size(); ++i)
                out.samples[i] = static_cast<std::int16_t>(detail::le16(data + body + 2 * i));
            out.frames = static_cast<std::uint32_t>(frames);
            out.channels = static_cast<std::uint8_t>(channels);
            out.rate = rate;
            error.clear();
            return true;
        }
        at = body + chunk + (chunk & 1u);
        if (at < body) break;
    }
    error = "WAVE has no data chunk";
    return false;
}

struct BankRule {
    std::int32_t threshold = std::numeric_limits<std::int32_t>::min() + 1;
    std::int32_t max_playbacks = std::numeric_limits<std::int32_t>::max();
    std::uint8_t behaviour = 3; // 0 steal oldest, 1 lowest priority, 2 low/old, 3 refuse
};

struct VoiceRequest {
    std::uintptr_t owner = 0;
    std::uint16_t uid = 0;
    std::shared_ptr<const Pcm> pcm;
    bool loop = false;
    std::int32_t fade_in_ms = 0;
    float left = 1.0f;
    float right = 1.0f;
    std::uint8_t bank = 0;
    std::int32_t priority = 0;
};

enum class PlayResult : std::uint8_t { started, refused_by_bank, invalid, voice_limit };

class Mixer {
public:
    static constexpr std::size_t kMaxVoices = 48;
    static constexpr std::size_t kBlockFrames = 256;

    Mixer(std::uint32_t output_rate, std::vector<BankRule> banks)
        : rate_(output_rate ? output_rate : 48000), banks_(std::move(banks)) {
        master_ = master_target_ = 1.0f;
    }

    PlayResult play(const VoiceRequest& request) {
        if (!request.pcm || request.pcm->frames == 0 || request.pcm->rate == 0 ||
            (request.pcm->channels != 1 && request.pcm->channels != 2))
            return PlayResult::invalid;
        std::lock_guard<std::mutex> lock(mutex_);
        if (!admit_locked(request.bank, request.priority)) return PlayResult::refused_by_bank;
        if (voices_.size() >= kMaxVoices) return PlayResult::voice_limit;
        Voice voice;
        voice.owner = request.owner;
        voice.uid = request.uid;
        voice.pcm = request.pcm;
        voice.loop = request.loop;
        voice.bank = request.bank;
        voice.priority = request.priority;
        voice.left = std::clamp(request.left, 0.0f, 1.0f);
        voice.right = std::clamp(request.right, 0.0f, 1.0f);
        voice.step_q32 = (std::uint64_t(request.pcm->rate) << 32) / rate_;
        if (request.fade_in_ms > 0) {
            voice.gain = 0.0f;
            ramp_locked(voice, 1.0f, ms_to_frames(request.fade_in_ms));
        }
        voices_.push_back(std::move(voice));
        return PlayResult::started;
    }

    // Stop(uid, fade): every live voice of this owner/UID. Returns the count.
    std::size_t stop(std::uintptr_t owner, std::uint16_t uid, std::int32_t fade_ms) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::size_t stopped = 0;
        for (auto& voice : voices_) {
            if (voice.released || voice.owner != owner || voice.uid != uid) continue;
            release_locked(voice, fade_ms);
            ++stopped;
        }
        return stopped;
    }

    // VoxSoundManager::StopAllSounds: every emitter fades out, then is released.
    void stop_all(std::int32_t fade_ms) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& voice : voices_)
            if (!voice.released) release_locked(voice, fade_ms);
    }

    // NativePauseAllSounds / resume: existing voices fade to silence and stop
    // advancing; resuming ramps back to full gain. New voices are unaffected.
    void pause_all(std::int32_t fade_ms) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& voice : voices_) {
            if (voice.released || voice.paused || voice.pause_when_silent) continue;
            voice.pause_when_silent = true;
            const auto frames = ms_to_frames(fade_ms);
            if (frames == 0) {
                voice.gain = 0.0f;
                voice.gain_frames = 0;
                voice.paused = true;
            } else {
                ramp_locked(voice, 0.0f, frames);
            }
        }
    }
    void resume_all(std::int32_t fade_ms) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& voice : voices_) {
            if (voice.released || (!voice.paused && !voice.pause_when_silent)) continue;
            voice.paused = false;
            voice.pause_when_silent = false;
            ramp_locked(voice, 1.0f, ms_to_frames(fade_ms));
        }
    }

    // FX volume option (0..1). SetGroupGain ramps over 0.05 s.
    void set_master_volume(float gain) {
        std::lock_guard<std::mutex> lock(mutex_);
        master_target_ = std::clamp(std::isfinite(gain) ? gain : 0.0f, 0.0f, 1.0f);
        master_frames_ = ms_to_frames(50);
        master_step_ = master_frames_ ? (master_target_ - master_) / float(master_frames_) : 0.0f;
        if (!master_frames_) master_ = master_target_;
    }

    // Interleaved stereo int16. Overwrites `out` completely.
    void render(std::int16_t* out, std::size_t frames) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::size_t done = 0;
        while (done < frames) {
            const std::size_t block = std::min(kBlockFrames, frames - done);
            std::fill(acc_.begin(), acc_.begin() + block * 2, 0.0f);
            for (auto it = voices_.begin(); it != voices_.end();) {
                if (it->paused) {
                    ++it;
                    continue;
                }
                if (!mix_voice(*it, block)) it = voices_.erase(it);
                else ++it;
            }
            for (std::size_t i = 0; i < block; ++i) {
                if (master_frames_) {
                    master_ += master_step_;
                    if (--master_frames_ == 0) master_ = master_target_;
                }
                for (std::size_t c = 0; c < 2; ++c) {
                    const float v = acc_[i * 2 + c] * master_;
                    out[(done + i) * 2 + c] = static_cast<std::int16_t>(
                        std::clamp(v, -32768.0f, 32767.0f));
                }
            }
            done += block;
        }
    }

    std::size_t active_voices() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return voices_.size();
    }
    std::size_t bank_voices(std::uint8_t bank) const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::size_t count = 0;
        for (const auto& voice : voices_)
            if (!voice.released && voice.bank == bank) ++count;
        return count;
    }
    std::uint32_t output_rate() const noexcept { return rate_; }

private:
    struct Voice {
        std::uintptr_t owner = 0;
        std::uint16_t uid = 0;
        std::shared_ptr<const Pcm> pcm;
        bool loop = false;
        std::uint8_t bank = 0;
        std::int32_t priority = 0;
        std::uint64_t cursor = 0;
        std::uint64_t step_q32 = 0;
        float left = 1.0f, right = 1.0f;
        float gain = 1.0f, gain_step = 0.0f, gain_target = 1.0f;
        std::uint32_t gain_frames = 0;
        bool released = false;        // stopping: leaves the bank, ends at silence
        bool pause_when_silent = false;
        bool paused = false;
    };

    std::uint32_t ms_to_frames(std::int32_t ms) const noexcept {
        if (ms <= 0) return 0;
        return static_cast<std::uint32_t>(std::min<std::uint64_t>(
            std::uint64_t(ms) * rate_ / 1000u, std::numeric_limits<std::uint32_t>::max()));
    }
    static void ramp_locked(Voice& voice, float target, std::uint32_t frames) {
        voice.gain_target = target;
        if (frames == 0) {
            voice.gain = target;
            voice.gain_frames = 0;
            voice.gain_step = 0.0f;
        } else {
            voice.gain_frames = frames;
            voice.gain_step = (target - voice.gain) / float(frames);
        }
    }
    void release_locked(Voice& voice, std::int32_t fade_ms) {
        voice.released = true;
        voice.pause_when_silent = false;
        voice.paused = false;
        // A zero fade still ramps over ~2 ms so an immediate stop cannot click.
        const auto frames = fade_ms > 0 ? ms_to_frames(fade_ms)
                                        : std::max<std::uint32_t>(ms_to_frames(2), 1);
        ramp_locked(voice, 0.0f, frames);
    }

    // PriorityBankManager::_CanAddEmitter + AddEmitter. Voices are kept in
    // insertion order, which is the bank vector's age order.
    bool admit_locked(std::uint8_t bank, std::int32_t priority) {
        if (bank >= banks_.size()) return true; // unmanaged
        const BankRule& rule = banks_[bank];
        if (rule.threshold > priority) return false;
        std::size_t count = 0;
        Voice* first_lowest = nullptr;
        Voice* oldest = nullptr;
        for (auto& voice : voices_) {
            if (voice.released || voice.bank != bank) continue;
            ++count;
            if (!oldest) oldest = &voice;
            if (!first_lowest || voice.priority < first_lowest->priority) first_lowest = &voice;
        }
        if (rule.max_playbacks > static_cast<std::int64_t>(count)) return true;
        Voice* victim = nullptr;
        switch (rule.behaviour) {
        case 0:
            victim = oldest;
            break;
        case 1:
            if (first_lowest && priority > first_lowest->priority) victim = first_lowest;
            break;
        case 2:
            if (first_lowest && priority >= first_lowest->priority) victim = first_lowest;
            break;
        default:
            break;
        }
        if (!victim) return false;
        victim->released = true;
        victim->pause_when_silent = false;
        victim->paused = false;
        ramp_locked(*victim, 0.0f, std::max<std::uint32_t>(ms_to_frames(2), 1));
        return true;
    }

    // Mixes `block` frames of one voice into acc_. Returns false when the
    // voice has ended and must be erased.
    bool mix_voice(Voice& voice, std::size_t block) {
        const Pcm& pcm = *voice.pcm;
        const std::size_t channels = pcm.channels;
        const std::uint64_t end = std::uint64_t(pcm.frames) << 32;
        for (std::size_t i = 0; i < block; ++i) {
            float gain = voice.gain;
            if (voice.gain_frames) {
                voice.gain += voice.gain_step;
                if (--voice.gain_frames == 0) {
                    voice.gain = voice.gain_target;
                    if (voice.gain_target <= 0.0f) {
                        if (voice.released) return false;
                        if (voice.pause_when_silent) {
                            voice.paused = true;
                            voice.pause_when_silent = false;
                            return true;
                        }
                    }
                }
            }
            const std::size_t index = static_cast<std::size_t>(voice.cursor >> 32);
            const float frac = float(voice.cursor & 0xffffffffu) * (1.0f / 4294967296.0f);
            std::size_t next = index + 1;
            bool has_next = true;
            if (next >= pcm.frames) {
                if (voice.loop) next -= pcm.frames;
                else has_next = false;
            }
            const std::int16_t* a = &pcm.samples[index * channels];
            const float l0 = float(a[0]);
            const float r0 = channels == 2 ? float(a[1]) : l0;
            float l1 = 0.0f, r1 = 0.0f;
            if (has_next) {
                const std::int16_t* b = &pcm.samples[next * channels];
                l1 = float(b[0]);
                r1 = channels == 2 ? float(b[1]) : l1;
            }
            const float l = l0 + (l1 - l0) * frac;
            const float r = r0 + (r1 - r0) * frac;
            acc_[i * 2] += l * gain * voice.left;
            acc_[i * 2 + 1] += r * gain * voice.right;
            voice.cursor += voice.step_q32;
            if (voice.cursor >= end) {
                if (!voice.loop) return false;
                voice.cursor -= end;
                if (voice.cursor >= end) voice.cursor %= end;
            }
        }
        return true;
    }

    mutable std::mutex mutex_;
    std::uint32_t rate_;
    std::vector<BankRule> banks_;
    std::vector<Voice> voices_;
    std::array<float, kBlockFrames * 2> acc_{};
    float master_ = 1.0f, master_target_ = 1.0f, master_step_ = 0.0f;
    std::uint32_t master_frames_ = 0;
};

} // namespace dh2::gameplay_sfx
