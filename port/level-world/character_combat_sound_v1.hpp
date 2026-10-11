#pragma once

#include <array>
#include <cstdint>

namespace dh2::character_combat_sound_v1 {

// The original CharSounds row stores four list counts and four arrays of SDD
// Sounds IDs, followed by the material flags consumed by F_ApplyCombatSound.
struct SoundList {
    std::array<std::uint16_t, 3> ids{};
    std::uint8_t count = 0;
};

struct SoundProfile {
    const char* name = nullptr;
    SoundList death;
    SoundList hurt;
    SoundList impact_vs_flesh;
    SoundList impact_vs_metal;
    bool is_flesh = false;
    bool is_metallic = false;
};

struct Request {
    const SoundProfile* attacker = nullptr;
    const SoundProfile* defender = nullptr;
    std::uintptr_t defender_identity = 0;
    std::int32_t damage = 0;
    bool defender_dead = false;
    bool minimal_randoms = false;
};

struct Services {
    void* context = nullptr;
    // Must advance the shared source ordinary Random stream exactly once and
    // return an index in [0, bound). No kernel-owned or substitute RNG exists.
    bool (*random_index)(void*, std::int32_t bound, std::int32_t& index) = nullptr;
    // The host resolves defender_identity to its source target position and
    // submits the original Play3D request. A false return never changes the
    // source branch order or consumes additional random values.
    bool (*play_3d)(void*, std::uint16_t source_sound_id,
                    std::uintptr_t defender_identity) = nullptr;
};

enum class Status : std::uint8_t {
    complete,
    minimal_randoms_suppressed,
    invalid_input,
    missing_random_service,
    random_service_failed,
    missing_playback_service,
    playback_rejected
};

struct Result {
    Status status = Status::invalid_input;
    std::uint8_t random_draws = 0;
    std::uint8_t playback_attempts = 0;
};

inline bool valid(const SoundList& list) noexcept {
    return list.count <= list.ids.size();
}

// Source: Character::F_ApplyCombatSound(AttackResult, Character*, Character*)
// at ELF 0x003afee0. It tests MP_MinimalRandoms before branch-specific work;
// dead defenders may play death and then impact, while living positive-damage
// defenders may play hurt and then a material impact.
inline Result apply(const Request& request, const Services& services) noexcept {
    Result result{};
    if (request.minimal_randoms) {
        result.status = Status::minimal_randoms_suppressed;
        return result;
    }
    if (!request.attacker || !request.defender || !request.defender_identity) {
        result.status = Status::invalid_input;
        return result;
    }

    bool playback_rejected = false;
    bool service_missing = false;
    auto emit = [&](const SoundList& list) {
        if (!valid(list)) {
            result.status = Status::invalid_input;
            return false;
        }
        if (!list.count) return true;
        if (!services.random_index) {
            result.status = Status::missing_random_service;
            return false;
        }
        std::int32_t index = -1;
        if (!services.random_index(services.context, list.count, index)) {
            result.status = Status::random_service_failed;
            return false;
        }
        ++result.random_draws;
        if (index < 0 || index >= list.count) {
            result.status = Status::random_service_failed;
            return false;
        }
        ++result.playback_attempts;
        if (!services.play_3d) {
            service_missing = true;
            return true;
        }
        if (!services.play_3d(services.context, list.ids[std::size_t(index)],
                              request.defender_identity))
            playback_rejected = true;
        return true;
    };

    if (request.defender_dead && !emit(request.defender->death)) return result;
    if (request.damage <= 0) {
        result.status = service_missing ? Status::missing_playback_service
                      : playback_rejected ? Status::playback_rejected
                                          : Status::complete;
        return result;
    }
    if (!request.defender_dead && !emit(request.defender->hurt)) return result;

    if (request.defender->is_flesh) {
        if (!valid(request.attacker->impact_vs_flesh) ||
            !emit(request.attacker->impact_vs_flesh)) return result;
    } else if (request.defender->is_metallic) {
        if (!valid(request.attacker->impact_vs_metal) ||
            !emit(request.attacker->impact_vs_metal)) return result;
    }

    result.status = service_missing ? Status::missing_playback_service
                  : playback_rejected ? Status::playback_rejected
                                      : Status::complete;
    return result;
}

} // namespace dh2::character_combat_sound_v1
