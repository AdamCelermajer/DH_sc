#include "death_checkpoint_load_v1.hpp"

#include <exception>

namespace dh::foundation::persistence::death_flow {
namespace {
bool invoke(bool (*provider)(void*, std::string&), void* context,
            const char* missing, std::string& error) {
    if (!provider) {
        error = missing;
        return false;
    }
    if (!provider(context, error)) {
        if (error.empty()) error = missing;
        return false;
    }
    error.clear();
    return true;
}

bool set_loading(const DeathCheckpointLoadServicesV1& services, bool value,
                 std::string& error) {
    if (!services.set_level_checkpoint_loading) {
        error = "Level::LoadCheckpoint requires its same-Level loading-flag provider";
        return false;
    }
    if (!services.set_level_checkpoint_loading(services.context, value, error)) {
        if (error.empty()) error = "Level checkpoint loading-flag provider failed";
        return false;
    }
    error.clear();
    return true;
}
}

bool begin_death_checkpoint_load_v1(bool level_type_38,
                                    bool level_savegame_present,
                                    DeathCheckpointLoadTicketV1& ticket,
                                    std::string& error) {
    ticket = {};
    if (!level_type_38) {
        error = "Death checkpoint load requires the active source gameplay Level type38";
        return false;
    }
    ticket.level_savegame_present = level_savegame_present;
    ticket.valid = true;
    error.clear();
    return true;
}

bool execute_death_checkpoint_load_v1(
    DeathCheckpointLoadTicketV1& ticket,
    const DeathCheckpointLoadServicesV1& services,
    DeathCheckpointLoadReceiptV1& receipt,
    std::string& error) {
    receipt = {};
    if (!ticket.valid || ticket.consumed) {
        error = "Death checkpoint load ticket is absent or already consumed";
        return false;
    }
    ticket.consumed = true;

    if (!set_loading(services, true, error)) {
        receipt.phase = DeathCheckpointLoadPhaseV1::failed;
        receipt.failure_phase = DeathCheckpointLoadPhaseV1::level_loading_entered;
        return false;
    }
    receipt.phase = DeathCheckpointLoadPhaseV1::level_loading_entered;
    bool failed = false;
    auto fail_at = [&](DeathCheckpointLoadPhaseV1 phase) {
        receipt.phase = DeathCheckpointLoadPhaseV1::failed;
        receipt.failure_phase = phase;
        failed = true;
    };

    try {
        if (ticket.level_savegame_present) {
            if (!invoke(services.load_level_checkpoint, services.context,
                        "LevelSavegame::LoadCheckPoint requires the retained source checkpoint cache/object loader",
                        error)) {
                fail_at(DeathCheckpointLoadPhaseV1::level_checkpoint_loaded);
            } else {
                receipt.phase = DeathCheckpointLoadPhaseV1::level_checkpoint_loaded;
                if (!invoke(services.reinitialize_game_events, services.context,
                            "GameEventManager::ReInit requires the same Level event owner",
                            error)) {
                    fail_at(DeathCheckpointLoadPhaseV1::game_events_reinitialized);
                } else {
                    receipt.phase = DeathCheckpointLoadPhaseV1::game_events_reinitialized;
                }
            }
        }

        if (!failed) {
            if (!services.resolve_local_character) {
                error = "PlayerManager::GetLocalPlayer requires the same PlayerManager/Character resolver";
                fail_at(DeathCheckpointLoadPhaseV1::local_character_resolved);
            } else if (!services.resolve_local_character(
                           services.context, receipt.local_character_present, error)) {
                if (error.empty()) error = "Local Character resolution failed";
                fail_at(DeathCheckpointLoadPhaseV1::local_character_resolved);
            } else {
                receipt.phase = DeathCheckpointLoadPhaseV1::local_character_resolved;
            }
        }
        if (!failed && receipt.local_character_present) {
            const auto load_qest = services.player_checkpoint_savegame_receiver_present
                ? services.load_checkpoint_qest
                : services.load_qest_fallback;
            const char* missing_qest = services.player_checkpoint_savegame_receiver_present
                ? "Character::SG_LoadCheckpoint requires its checkpoint PlayerSavegame cache and QEST-only load(16)"
                : "Character::SG_LoadCheckpoint fallback requires PlayerSavegame::SG_Load(16)";
            if (!invoke(load_qest, services.context, missing_qest, error)) {
                fail_at(DeathCheckpointLoadPhaseV1::player_qest_loaded);
            } else {
                receipt.phase = DeathCheckpointLoadPhaseV1::player_qest_loaded;
                if (!invoke(services.update_character_quests, services.context,
                            "Character::SG_Update(true) requires the same PlayerSavegame quest updater",
                            error)) {
                    fail_at(DeathCheckpointLoadPhaseV1::player_quests_updated);
                } else {
                    receipt.phase = DeathCheckpointLoadPhaseV1::player_quests_updated;
                }
            }
        }
    } catch (const std::exception& ex) {
        error = ex.what();
        fail_at(receipt.phase);
    } catch (...) {
        error = "Death checkpoint provider threw after a non-transactional source prefix";
        fail_at(receipt.phase);
    }

    const std::string source_error = error;
    std::string clear_error;
    if (!set_loading(services, false, clear_error)) {
        receipt.loading_flag_cleared = false;
        if (error.empty()) error = clear_error;
        else error += "; additionally failed to clear Level loading flag: " + clear_error;
        if (!failed) fail_at(DeathCheckpointLoadPhaseV1::level_loading_cleared);
        return false;
    }
    receipt.loading_flag_cleared = true;
    if (failed) {
        error = source_error.empty() ? "Death checkpoint provider failed" : source_error;
        return false;
    }
    receipt.phase = DeathCheckpointLoadPhaseV1::complete;
    receipt.complete = true;
    error.clear();
    return true;
}

} // namespace dh::foundation::persistence::death_flow
