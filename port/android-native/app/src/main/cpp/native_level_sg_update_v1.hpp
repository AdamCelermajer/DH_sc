#pragma once

#include "../../../../../level-world/source_level_owner_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::native::level_sg_update_v1 {

// Projection of the existing Level::Update(false) frame. The caller supplies
// identities borrowed from the active Level/PlayerManager; this adapter owns
// neither a frame clock nor a Character/Save/ScriptManager.
struct FrameV1 {
    std::uintptr_t level_identity = 0;
    bool active_gameplay_update = false;
    std::int32_t pending_script_id = 0; // Level + 0x148; meaningful only when known.
    bool pending_script_id_known = false;
    std::uintptr_t local_character_identity = 0;
};

struct ServicesV1 {
    void* context = nullptr;
    // Called only for a non-sentinel pending script. It reads the existing
    // Application::IsCurrentlyInGameView state.
    bool (*is_currently_in_game_view)(void*, bool&, std::string&) = nullptr;
    // Mirrors Level::Update's ScriptManager::StartScript(id, -1, false).
    bool (*start_level_script)(void*, std::uintptr_t level_identity,
                               std::int32_t script_id, std::int32_t argument,
                               bool flag, std::string&) = nullptr;
    // Mirrors Character::SG_Update(false) on the local Character. The provider
    // must borrow that Character's PlayerSavegame and its one canonical QEST
    // owner; this call occurs before ScriptManager::ExecuteAllScripts().
    bool (*character_sg_update)(void*, std::uintptr_t level_identity,
                                std::uintptr_t character_identity,
                                bool checkpoint, std::string&) = nullptr;
    // The same Level::Update then executes its one ScriptManager after the
    // Character path (or after that path is skipped for a missing player).
    // This is a callback seam, not another clock or scheduler.
    bool (*execute_all_scripts)(void*, std::uintptr_t level_identity,
                                std::string&) = nullptr;
};

enum class OutcomeV1 : std::uint8_t {
    inactive_level_frame,
    no_local_character,
    character_updated
};

struct ResultV1 {
    OutcomeV1 outcome = OutcomeV1::inactive_level_frame;
    bool pending_script_started = false;
    bool character_update_called = false;
    bool script_manager_executed = false;
    std::uintptr_t level_identity = 0;
    std::uintptr_t character_identity = 0;
};

// Project the existing SourceLevelOwner snapshot into this dispatch contract.
// The pending field is copied together with its provenance bit; an unknown
// borrowed Level is never coerced to the constructor sentinel.
inline bool frame_from_source_level_v1(
        const source_level_owner_v1::Snapshot& source,
        bool active_gameplay_update, FrameV1& out, std::string& error) {
    FrameV1 value;
    value.active_gameplay_update = active_gameplay_update;
    value.level_identity = source.source_level;
    value.pending_script_id = source.pending_script_id;
    value.pending_script_id_known = source.pending_script_id_known;
    value.local_character_identity = source.player_character;
    if (active_gameplay_update &&
        (source.phase != source_level_owner_v1::Phase::active ||
         !source.source_level)) {
        error = "Level frame projection requires the active SourceLevelOwner identity";
        return false;
    }
    out = value;
    error.clear();
    return true;
}

// Source Level::Update order: for a non--1 Level+0x148 value, query the
// current-game-view state and optionally StartScript(id,-1,false); then call
// local Character::SG_Update(false) when one exists, followed by the same
// ScriptManager::ExecuteAllScripts. The active flag is projected only after
// the existing Level/game-state pause gates; this adapter owns no loop/clock.
inline bool dispatch_v1(const FrameV1& frame, const ServicesV1& services,
                        ResultV1& out, std::string& error) {
    if (!frame.active_gameplay_update) {
        ResultV1 value;
        value.outcome = OutcomeV1::inactive_level_frame;
        value.level_identity = frame.level_identity;
        out = value;
        error.clear();
        return true;
    }
    if (!frame.level_identity) {
        error = "Level::Update SG_Update adapter requires the active Level identity";
        return false;
    }
    ResultV1 value;
    value.level_identity = frame.level_identity;
    value.character_identity = frame.local_character_identity;
    if (!frame.pending_script_id_known) {
        error = "Level::Update requires a source-owned Level+0x148 value";
        return false;
    }
    if (frame.pending_script_id != -1) {
        if (!services.is_currently_in_game_view) {
            error = "Level::Update pending-script branch requires the current game-view provider";
            return false;
        }
        bool in_game_view = false;
        if (!services.is_currently_in_game_view(services.context, in_game_view, error)) {
            if (error.empty()) error = "Application::IsCurrentlyInGameView provider failed";
            return false;
        }
        if (in_game_view) {
            if (!services.start_level_script) {
                error = "Level::Update pending-script branch requires the existing ScriptManager provider";
                return false;
            }
            if (!services.start_level_script(services.context, frame.level_identity,
                                             frame.pending_script_id, -1, false, error)) {
                if (error.empty()) error = "Level::Update pending ScriptManager::StartScript failed";
                return false;
            }
            value.pending_script_started = true;
        }
    }

    // A missing Character suppresses only SG_Update, not the later shared
    // ScriptManager frame. This preserves the source Level::Update tail.
    if (frame.local_character_identity) {
        if (!services.character_sg_update) {
            error = "Level::Update requires the local Character::SG_Update(false) provider";
            return false;
        }
        if (!services.character_sg_update(services.context, frame.level_identity,
                                          frame.local_character_identity, false, error)) {
            if (error.empty()) error = "Character::SG_Update(false) provider failed";
            return false;
        }
        value.outcome = OutcomeV1::character_updated;
        value.character_update_called = true;
    } else {
        value.outcome = OutcomeV1::no_local_character;
    }
    if (!services.execute_all_scripts) {
        error = "Level::Update requires its active ScriptManager ExecuteAllScripts provider";
        return false;
    }
    if (!services.execute_all_scripts(services.context, frame.level_identity, error)) {
        if (error.empty()) error = "Level::Update ScriptManager execution failed";
        return false;
    }
    value.script_manager_executed = true;
    out = value;
    error.clear();
    return true;
}

} // namespace dh2::native::level_sg_update_v1
