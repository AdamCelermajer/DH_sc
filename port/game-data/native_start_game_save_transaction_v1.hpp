#pragma once

#include "native_start_game_plan_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::data {

// Borrowed callbacks for the one selected PlayerInfo +680 metadata Save.
// identity is captured from that Save owner; every callback must reject any
// identity other than its own. This coordinator owns neither a Save nor a
// profile and cannot silently substitute a temporary Save.
struct NativeStartGameMetadataOwnerV1 {
    void* context = nullptr;
    std::uintptr_t save_identity = 0;
    bool (*read)(void*, std::uintptr_t, NativeStartGameSaveViewV1&, std::string&) = nullptr;
    bool (*save_numeric_request)(void*, std::uintptr_t, std::int32_t&,
                                 std::string&) = nullptr;
    bool (*clear_spawn_point_and_save)(void*, std::uintptr_t, std::size_t,
                                       std::int32_t&, std::string&) = nullptr;
};

enum class NativeStartGameSavePhaseV1 : std::uint8_t {
    empty,
    planned,
    spawn_flag_saved,
};

// Transaction token records which borrowed metadata Save produced the plan.
// The caller may run other source-backed preparation (seed selection, asset
// load) between begin and save_spawn_flag_before_load.
struct NativeStartGameSaveTransactionV1 {
    std::uintptr_t save_identity = 0;
    NativeStartGamePlanV1 plan;
    NativeStartGameSavePhaseV1 phase = NativeStartGameSavePhaseV1::empty;
};

bool begin_native_start_game_save_transaction_v1(
    const NativeStartGameMetadataOwnerV1&, const LevelTables&,
    const NativeStartGameRequestV1&, NativeStartGameSaveTransactionV1&,
    std::int32_t& current_difficulty, std::string& error);

bool save_spawn_flag_before_native_load_v1(
    const NativeStartGameMetadataOwnerV1&,
    NativeStartGameSaveTransactionV1&, std::int32_t& current_difficulty,
    std::string& error);

}  // namespace dh2::data
