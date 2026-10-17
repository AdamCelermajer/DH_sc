#pragma once

#include <cstdint>
#include <cstring>

namespace dh2::native_aggro_music_v1 {

// Android's selected music adapter stands in for the VoxSoundManager bytes
// used by AISPlayer::OnAggro/OnDeAggro. The original constructor initializes
// +0x31 (ambient transition allowed) to true and +0x32 (safe-zone music) false.
struct VoxState {
    bool ambient_transition_allowed = true;
    bool safe_zone_music = false;
};

enum class StateChange : std::uint8_t { none, ambient, combat };

struct Result {
    StateChange state = StateChange::none;
    // AISPlayer::OnDeAggro also replays the current level music when the
    // VoxSoundManager safe-zone byte is set. The Android VXN adapter currently
    // owns only Crypt's UID 486 state segments, so callers can preserve this
    // distinction without inventing a level-music command.
    bool replay_level_music = false;
};

// The source ARM build uses signed 32-bit integer arithmetic. Express the
// wrap explicitly so host compilers do not invoke C++ signed-overflow UB.
inline std::int32_t add_i32(std::int32_t lhs, std::int32_t rhs) noexcept {
    std::uint32_t lhs_bits = 0;
    std::uint32_t rhs_bits = 0;
    std::memcpy(&lhs_bits, &lhs, sizeof(lhs_bits));
    std::memcpy(&rhs_bits, &rhs, sizeof(rhs_bits));
    const std::uint32_t sum = lhs_bits + rhs_bits;
    std::int32_t result = 0;
    std::memcpy(&result, &sum, sizeof(result));
    return result;
}

inline std::int32_t subtract_i32(std::int32_t lhs, std::int32_t rhs) noexcept {
    std::uint32_t lhs_bits = 0;
    std::uint32_t rhs_bits = 0;
    std::memcpy(&lhs_bits, &lhs, sizeof(lhs_bits));
    std::memcpy(&rhs_bits, &rhs, sizeof(rhs_bits));
    const std::uint32_t difference = lhs_bits - rhs_bits;
    std::int32_t result = 0;
    std::memcpy(&result, &difference, sizeof(result));
    return result;
}

// Source: AISPlayer::OnAggro (ELF 0x3ddb70).
// AggroCount (+0xd0) always increments. AggroForMusic (+0xd4) changes only
// for offline/local players, then combat music is selected only when the
// source LevelConfig flag is set, the Vox +0x31 gate is true, and the
// DesignSettings CombatMusicTrigger threshold is reached.
inline Result on_aggro(std::int32_t& aggro_count,
                       std::int32_t& aggro_for_music,
                       VoxState& vox,
                       std::int32_t aggressor_combat_music,
                       bool local_or_offline_player,
                       bool level_combat_music_enabled,
                       bool design_trigger_available,
                       std::int32_t combat_music_trigger) noexcept {
    aggro_count = add_i32(aggro_count, 1);
    if (!local_or_offline_player) return {};

    aggro_for_music = add_i32(aggro_for_music, aggressor_combat_music);
    if (level_combat_music_enabled && design_trigger_available &&
        vox.ambient_transition_allowed &&
        aggro_for_music >= combat_music_trigger) {
        vox.ambient_transition_allowed = false;
        return {StateChange::combat, false};
    }
    return {};
}

// Source: AISPlayer::OnDeAggro (ELF 0x3dde48).
// AggroCount always decrements. The local/offline branch subtracts the
// departing character's CharAI +0x14 value and switches back to ambient only
// when the music-weighted count reaches exactly zero.
inline Result on_deaggro(std::int32_t& aggro_count,
                         std::int32_t& aggro_for_music,
                         VoxState& vox,
                         std::int32_t aggressor_combat_music,
                         bool local_or_offline_player) noexcept {
    aggro_count = add_i32(aggro_count, -1);
    if (!local_or_offline_player) return {};

    aggro_for_music = subtract_i32(aggro_for_music, aggressor_combat_music);
    if (!vox.ambient_transition_allowed && aggro_for_music == 0) {
        vox.ambient_transition_allowed = true;
        return {StateChange::ambient, vox.safe_zone_music};
    }
    return {};
}

inline const char* state_name(StateChange state) noexcept {
    switch (state) {
    case StateChange::ambient: return "ambient";
    case StateChange::combat: return "combat";
    default: return nullptr;
    }
}

} // namespace dh2::native_aggro_music_v1
