#pragma once

#include "level_transition_v1.hpp"
#include "../game-data/player_save_load_owner_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::ui {

enum class NativeDestinationRuntimeKindV1 : std::uint8_t {
    unsupported,
    swamp_static,
    crypt_generated
};

// A non-mutating destination resolution over the one active PlayerSave
// loader/profile and current LevelList. Every unique LevelList row with a
// filename is retained; current runtime adapter strings are populated only
// for SWAMP and generated Crypt. Other rows remain represented with
// kind=unsupported for the destination builder. No Level, Save, profile, or
// ObjectManager is created here.
struct NativeLevelTransitionPlanV1 {
    LevelTransitionLoadV1 load{};
    std::int32_t level_row{-1};
    std::string level_name;
    NativeDestinationRuntimeKindV1 runtime_kind{};
    std::string descriptor;
    std::string bres;
    std::string spawns;
    std::string objects;
    std::uint32_t saved_seed{};
    bool used_saved_seed{};
};

// Resolve the destination named by NativeGoToZone against the current source
// LevelList and the active Character's already-loaded Save/profile transport.
// `player_slot` and `difficulty` must come from the live PlayerManager and
// Character getters. `dont_use_player_seed` and `real_time_seed` are supplied
// by their existing providers; this function performs no profile read/write.
bool build_native_level_transition_plan_v1(
    const data::LevelTables&,
    const data::PlayerSaveLoadOwnerV1& active_player_loader,
    std::int32_t player_slot,
    const std::string& destination_name,
    std::int32_t normalized_entry_point,
    std::int32_t difficulty,
    bool dont_use_player_seed,
    std::uint32_t real_time_seed,
    NativeLevelTransitionPlanV1& output,
    std::string& error);

} // namespace dh2::ui
