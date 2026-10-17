#pragma once

#include "../game-data/level_tables.hpp"

#include <cstdint>
#include <string>

namespace dh2::ui {

// Borrowed snapshot of the live source Level fields consumed by NativeGoToZone.
// This is not a second Level owner; the provider must read the active world.
struct LevelTransitionCurrentV1 {
    bool present{};
    std::uint8_t transition_flag{}; // Level +0x144
    std::int32_t state{};           // Level +0x130
};

struct LevelTransitionLoadV1 {
    std::string file;
    std::int32_t entry_point{};
    std::int32_t player_slot{};
    bool resume{true};
    bool pending{true};
    std::int32_t difficulty{};
    bool remote_trigger{};
    std::uint32_t seed{};
    std::uint32_t synchronized_seed{};
};

struct LevelTransitionServicesV1 {
    void* context{};
    bool (*eabi_double_to_int)(void*, double, std::int32_t&, std::string&){};
    bool (*current_level)(void*, LevelTransitionCurrentV1&, std::string&){};
    bool (*quick_save)(void*, bool force, std::string&){};
    bool (*save_all_players)(void*, bool force, std::string&){};
    bool (*local_player_slot)(void*, std::int32_t&, std::string&){};
    bool (*local_player_difficulty)(void*, std::int32_t&, std::string&){};
    bool (*display_fast_travel)(void*, bool source_flag,
                                const std::string& level_name,
                                const std::string& repeated_level_name,
                                std::int32_t entry_point, std::string&){};
    bool (*load_level)(void*, const LevelTransitionLoadV1&, std::string&){};
};

enum class LevelTransitionPhaseV1 : std::uint8_t {
    idle, current_level, quick_save, save_players, player_slot,
    resolve_destination, player_difficulty, display_fast_travel, load_level, complete
};

// NativeGoToZone's source-ordered synchronous transaction (ELF 0x4422b8).
// The owner borrows the one decoded catalogue and existing current-Level,
// PlayerManager, Save, menu, and Application::LoadLevel providers. It creates
// no Save, catalogue, VM, Level or world manager. Source-side effects already
// delivered remain delivered when a later service fails; callers must not
// retry such a partial transaction automatically.
class LevelTransitionOwnerV1 {
    const dh2::data::LevelTables& tables_;
    LevelTransitionServicesV1 services_;
    LevelTransitionPhaseV1 phase_{LevelTransitionPhaseV1::idle};
    bool active_{};
public:
    LevelTransitionOwnerV1(const dh2::data::LevelTables&, LevelTransitionServicesV1);
    LevelTransitionOwnerV1(const LevelTransitionOwnerV1&) = delete;
    LevelTransitionOwnerV1& operator=(const LevelTransitionOwnerV1&) = delete;

    // Input is the post-to_string NativeGoToZone level name and raw AS number.
    // Conversion is deferred until after the original current-Level gate.
    bool transition(const std::string& destination_name,
                    double raw_entry_point, std::string& error);
    LevelTransitionPhaseV1 reached_phase() const noexcept { return phase_; }
};

} // namespace dh2::ui
