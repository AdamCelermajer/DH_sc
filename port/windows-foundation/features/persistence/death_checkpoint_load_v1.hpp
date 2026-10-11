#pragma once

#include <cstdint>
#include <string>

namespace dh::foundation::persistence::death_flow {

enum class DeathCheckpointLoadPhaseV1 : std::uint8_t {
    none,
    level_loading_entered,
    level_checkpoint_loaded,
    game_events_reinitialized,
    local_character_resolved,
    player_qest_loaded,
    player_quests_updated,
    level_loading_cleared,
    complete,
    failed
};

struct DeathCheckpointLoadTicketV1 {
    bool level_savegame_present = false;
    bool valid = false;
    bool consumed = false;
};

struct DeathCheckpointLoadReceiptV1 {
    DeathCheckpointLoadPhaseV1 phase = DeathCheckpointLoadPhaseV1::none;
    DeathCheckpointLoadPhaseV1 failure_phase = DeathCheckpointLoadPhaseV1::none;
    bool local_character_present = false;
    bool loading_flag_cleared = false;
    bool complete = false;
};

struct DeathCheckpointLoadServicesV1 {
    void* context = nullptr;
    // Character::SG_LoadCheckpoint uses its checkpoint Savegame receiver when
    // present, but falls back to PlayerSavegame::SG_Load(16) when absent.
    bool player_checkpoint_savegame_receiver_present = false;
    // Level byte+244. true brackets the source operation; the wrapper also
    // clears it after a provider error, while retaining reached mutations.
    bool (*set_level_checkpoint_loading)(void*, bool, std::string&) = nullptr;
    // Must use the SAME LevelSavegame cache/Level/ObjectManager: select the
    // source checkpoint filename, refresh its cache, load INFO then OBJS,
    // and restore the current live filename/cache as LevelSavegame does.
    // OBJS must dispatch PlayerCharacter_0 to the SAME Character::Deserialize;
    // its IsPlayer branch calls SetVisible(true), not PlayerSavegame::SG_Load.
    bool (*load_level_checkpoint)(void*, std::string&) = nullptr;
    // Source GameEventManager::ReInit, after LevelSavegame::LoadCheckPoint.
    bool (*reinitialize_game_events)(void*, std::string&) = nullptr;
    // GetLocalPlayer(0, 1) and read its Character pointer; false is a valid
    // no-character result, while callback failure is a provider error.
    bool (*resolve_local_character)(void*, bool& present, std::string&) = nullptr;
    // The checkpoint receiver path selects the checkpoint player-save cache,
    // loads 16, then restores the regular profile filename.
    bool (*load_checkpoint_qest)(void*, std::string&) = nullptr;
    // Character::SG_LoadCheckpoint's source fallback when that receiver is
    // absent: PlayerSavegame::SG_Load(16) from its current save receiver.
    bool (*load_qest_fallback)(void*, std::string&) = nullptr;
    // Same Character::SG_Update(true) / PlayerSavegame::SG_Update(true).
    bool (*update_character_quests)(void*, std::string&) = nullptr;
};

bool begin_death_checkpoint_load_v1(bool level_type_38,
                                    bool level_savegame_present,
                                    DeathCheckpointLoadTicketV1&,
                                    std::string& error);

// Mirrors Level::LoadCheckpoint's ordered, non-transactional effects. A
// failed modern provider stops the later prefix and attempts to clear the
// Level loading flag; already reached world/player mutations are retained.
bool execute_death_checkpoint_load_v1(DeathCheckpointLoadTicketV1&,
                                     const DeathCheckpointLoadServicesV1&,
                                     DeathCheckpointLoadReceiptV1&,
                                     std::string& error);

} // namespace dh::foundation::persistence::death_flow
