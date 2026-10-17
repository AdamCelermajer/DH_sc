#pragma once

#include "native_quest_owner.hpp"
#include "player_savegame_v1.hpp"
#include "../../../../../level-world/source_level_owner_v1.hpp"

namespace dh2::native::level_quest_active_v1 {

using QuestOwner = dh2::native::quests::Owner;
using Save = dh2::data::PlayerSavegameV1;
using SourceLevel = dh2::source_level_owner_v1::Snapshot;
using ScriptRunning = bool (*)(void*, const std::string&, bool&, std::string&);
using StartScript = bool (*)(void*, const std::string&, std::int32_t, bool,
                             std::string&);

struct ScriptServices {
    void* context = nullptr;
    ScriptRunning is_running = nullptr;
    StartScript start = nullptr;
};

// Port coverage for Character::SG_Update(false): update the existing selected
// offline QEST log's active Objectives at the proven pre-physics Level boundary.
// CompileQuests/Synchronization and checkpoint SG_Update remain separate owners.
inline bool update_active_log(const SourceLevel& level,
                              std::uintptr_t expected_level,
                              std::uintptr_t expected_character,
                              const Save& save, QuestOwner& quests,
                              bool online, std::int32_t difficulty,
                              std::int32_t application_time,
                              const ScriptServices& scripts,
                              std::vector<dh2::native::quests::QuestUpdateActiveResultV1>& out,
                              std::string& error) {
    if (level.phase != dh2::source_level_owner_v1::Phase::active ||
        level.source_level_state != 38 || !level.source_level ||
        level.source_level != expected_level || !expected_character ||
        level.player_character != expected_character || !level.quest_owner ||
        level.quest_owner != static_cast<const void*>(&quests) ||
        !level.canonical_player_savegame ||
        level.canonical_player_savegame != reinterpret_cast<std::uintptr_t>(&save) ||
        save.character() != expected_character || !quests.owns_save(&save)) {
        error = "Quest frame update requires the active Level and its same Character/Save/QEST owners";
        return false;
    }
    if (online || difficulty < 0 || difficulty > 2) {
        error = "Quest frame update supports only the selected offline Save difficulty";
        return false;
    }
    const dh2::native::quests::QuestUpdateActiveServicesV1 services{
        scripts.context, scripts.is_running, application_time, true, scripts.start};
    // Offline PlayerSavegame::SG_GetQuestSG selects this same Save's +0xb8 log.
    return quests.update_active_log(0, static_cast<std::uint32_t>(difficulty),
                                    services, out, error);
}

} // namespace dh2::native::level_quest_active_v1
