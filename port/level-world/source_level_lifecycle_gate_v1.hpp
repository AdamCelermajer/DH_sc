#pragma once

#include <cstdint>

namespace dh2::source_level_lifecycle_gate_v1 {

// Borrowed identities and fields read from the authoritative source chain:
// Application::GetCurrentLevel -> GSLevel::s_level, owned by one StateMachine
// GSLevel whose +0x34 is the same Level. Source refs: getter 0x32f594, ctor
// 0x386190, Update 0x386630, Dtor 0x3860bc, LevelSavegame ctor 0x462934.
// No object is created or dereferenced.
enum class GsLevelPhase : std::uint32_t {
    unknown = 0, constructed = 1, load_requested = 2, updating = 3, active = 4
};

struct Snapshot {
    std::uintptr_t gslevel_owner{};
    std::uintptr_t gslevel_level_34{};
    std::uintptr_t application_current_level{};
    std::uintptr_t level_savegame_ec{}; // Level +0xec, nullable by source
    std::uintptr_t savegame_parent_level_08{}; // LevelSavegame +0x08
    GsLevelPhase gslevel_phase{GsLevelPhase::unknown}; // GSLevel +0x38
    std::int32_t level_state_130{};
    std::uint8_t transition_flag_144{};
    std::uint32_t teardown_started{};
};

enum class Status : std::uint32_t {
    ready, missing_owner, missing_level, unpublished_level, owner_mismatch,
    loading, not_loaded, tearing_down, savegame_parent_mismatch
};

struct Descriptor {
    Status status{Status::missing_owner};
    std::uintptr_t gslevel_owner{};
    std::uintptr_t level{};
    std::uintptr_t level_savegame{};
    std::int32_t level_state{};
    std::uint8_t transition_flag{};
    std::uint32_t quick_save_available{};
};

// Validation only: a runtime provider must obtain these values from its one
// live source owner. The selected Irrlicht world/actor projections cannot
// satisfy this descriptor by sharing a map ID or visual scene.
Descriptor inspect(const Snapshot&) noexcept;

} // namespace dh2::source_level_lifecycle_gate_v1
