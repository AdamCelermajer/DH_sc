#include "level_transition_v1.hpp"

#include <algorithm>

namespace dh2::ui {
namespace {
constexpr std::int32_t kQuickSaveState = 38;

bool missing(const char* name, std::string& error) {
    error = std::string("NativeGoToZone provider unavailable: ") + name;
    return false;
}
}

LevelTransitionOwnerV1::LevelTransitionOwnerV1(
    const dh2::data::LevelTables& tables, LevelTransitionServicesV1 services)
    : tables_(tables), services_(services) {}

bool LevelTransitionOwnerV1::transition(const std::string& destination_name,
    double raw_entry_point, std::string& error) {
    error.clear();
    if (active_) {
        error = "NativeGoToZone transition is already active";
        return false;
    }
    active_ = true;
    struct Reset { bool& active; ~Reset() { active = false; } } reset{active_};
    phase_ = LevelTransitionPhaseV1::current_level;

    LevelTransitionCurrentV1 current;
    if (!services_.current_level) return missing("Application::GetCurrentLevel", error);
    if (!services_.current_level(services_.context, current, error)) return false;
    // NativeGoToZone proceeds only for a null Level or when Level+0x144 is set.
    if (current.present && current.transition_flag == 0) {
        phase_ = LevelTransitionPhaseV1::complete;
        return true;
    }

    if (!services_.eabi_double_to_int)
        return missing("__aeabi_d2iz", error);
    std::int32_t entry = 0;
    if (!services_.eabi_double_to_int(services_.context, raw_entry_point, entry, error)) return false;
    if (entry == -1) entry = 0;

    std::int32_t slot = 0;
    if (current.present) {
        if (current.state == kQuickSaveState) {
            phase_ = LevelTransitionPhaseV1::quick_save;
            if (!services_.quick_save) return missing("Level::QuickSave", error);
            if (!services_.quick_save(services_.context, false, error)) return false;
        }
        phase_ = LevelTransitionPhaseV1::save_players;
        if (!services_.save_all_players) return missing("Level::SG_SaveAllPlayer", error);
        if (!services_.save_all_players(services_.context, false, error)) return false;

        phase_ = LevelTransitionPhaseV1::player_slot;
        if (!services_.local_player_slot) return missing("PlayerManager::GetLocalPlayer/Character::SG_GetSlot", error);
        if (!services_.local_player_slot(services_.context, slot, error)) return false;
    }

    // The source body tests std::string begin != end after the save/slot prefix.
    // Empty names therefore retain that prefix's effects and skip the LevelList,
    // DisplayFastTravel and LoadLevel continuation.
    if (destination_name.empty()) {
        phase_ = LevelTransitionPhaseV1::complete;
        return true;
    }

    phase_ = LevelTransitionPhaseV1::resolve_destination;
    const auto it = std::find_if(tables_.levels.begin(), tables_.levels.end(),
        [&](const auto& row) { return row.name == destination_name; });
    if (it == tables_.levels.end() || it->level_file.empty()) {
        error = "NativeGoToZone destination is absent from the current LevelList";
        return false;
    }

    phase_ = LevelTransitionPhaseV1::player_difficulty;
    std::int32_t difficulty = 0;
    if (!services_.local_player_difficulty)
        return missing("PlayerManager::GetLocalPlayer/Character::SG_GetGameDifficulty", error);
    if (!services_.local_player_difficulty(services_.context, difficulty, error)) return false;

    phase_ = LevelTransitionPhaseV1::display_fast_travel;
    if (!services_.display_fast_travel) return missing("MenuManager::DisplayFastTravel AS callback", error);
    // Source InvokeASCallback passes four values: false, destination name,
    // destination name, and the normalized entry point.
    if (!services_.display_fast_travel(services_.context, false,
            destination_name, destination_name, entry, error)) return false;

    phase_ = LevelTransitionPhaseV1::load_level;
    if (!services_.load_level) return missing("Application::LoadLevel", error);
    LevelTransitionLoadV1 request;
    request.file = it->level_file;
    request.entry_point = entry;
    request.player_slot = slot;
    request.difficulty = difficulty;
    if (!services_.load_level(services_.context, request, error)) return false;

    phase_ = LevelTransitionPhaseV1::complete;
    return true;
}

} // namespace dh2::ui
