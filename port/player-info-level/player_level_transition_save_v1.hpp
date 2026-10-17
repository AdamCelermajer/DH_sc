#pragma once

#include "../game-data/player_savegame_v1.hpp"

namespace dh2::player_level_transition_save_v1 {

struct Services {
    void* context{};
    bool (*character_active)(void*, std::uintptr_t, bool&, std::string&){};
    bool (*character_level)(void*, std::uintptr_t, std::int32_t&, std::string&){};
    bool (*current_level_entry_point)(void*, std::uintptr_t expected_level,
                                      std::int32_t&, std::string&){};
    bool (*active_difficulty)(void*, std::int32_t&, std::string&){};
    bool (*unix_time_seconds)(void*, std::uint32_t&, std::string&){};
    // Must persist through this same Save's canonical registered writer.
    bool (*save_gameplay)(void*, data::PlayerSavegameV1&, std::string&){};
};

enum class Phase : std::uint8_t {
    idle, active_check, unblock, character_level, save_date,
    entry_point, persist, restore_block, complete
};

// One source Level::SG_SavePlayer(Character, force) call (ELF 0x3efa54). The supplied
// Character and Level are identities borrowed from the caller; the Save is the
// existing canonical PlayerSavegame owner. This does not enumerate PlayerInfo,
// own a Save/Level, write files itself, or implement LevelSavegame::QuickSave.
class Owner {
    data::PlayerSavegameV1& save_;
    std::uintptr_t character_{};
    std::uintptr_t level_{};
    Services services_{};
    Phase phase_{Phase::idle};
    bool active_{};
public:
    Owner(data::PlayerSavegameV1&, std::uintptr_t character,
          std::uintptr_t level, Services);
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;

    bool save_player(bool force_unblock, std::string& error);
    Phase reached_phase() const noexcept { return phase_; }
};

} // namespace dh2::player_level_transition_save_v1
