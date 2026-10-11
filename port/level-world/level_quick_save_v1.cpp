#include "level_quick_save_v1.hpp"

namespace dh2::level_quick_save_v1 {
namespace {
Status fail(Result& result, Result* output, std::string& error,
            Status status, const char* fallback) {
    result.status = status;
    if (error.empty()) error = fallback;
    *output = result;
    return status;
}
}

Status run(const State* state, std::uint32_t force, const Services* services,
           Result* output, std::string& error) {
    if (!state || !services || !output || force > 1) {
        error = "QuickSave requires a source state, services, result, and bool force";
        return Status::invalid_argument;
    }
    Result result{};
    error.clear();
    const auto finish = [&](Status status) {
        result.status = status;
        *output = result;
        return status;
    };

    // Original Level::QuickSave checks LevelSavegame, Level state 38, local
    // Character, Character virtual+0x34 == 0, then offline or local-host with
    // PlayerManager byte +0x719 == 0. No provider is called before this gate.
    const bool host_can_save = state->local_player_is_host &&
                               !state->player_manager_gate_719;
    if (!state->level_savegame || state->level_state != 38 ||
        !state->local_character || state->character_quicksave_predicate ||
        (state->online && !host_can_save))
        return finish(Status::skipped);

    if (!services->copy_checkpoint_position)
        return fail(result, output, error, Status::service_unavailable,
                    "Character checkpoint-position writer unavailable");
    ++result.service_calls;
    try {
        if (services->copy_checkpoint_position(services->context,
                state->local_character, state->character_word_168,
                state->character_word_160, state->character_word_164, error))
            return fail(result, output, error, Status::service_failed,
                        "Character checkpoint-position writer failed");
    } catch (...) {
        return fail(result, output, error, Status::service_failed,
                    "Character checkpoint-position writer threw");
    }
    result.position_copied = 1;

    // QuickSave(false) leaves +0x39 untouched. Only the forced branch writes
    // zero before Save and restores the captured byte afterward.
    if (force) {
        if (!services->write_savegame_gate_39)
            return fail(result, output, error, Status::service_unavailable,
                        "LevelSavegame gate-byte writer unavailable");
        ++result.service_calls;
        try {
            if (services->write_savegame_gate_39(services->context,
                    state->level_savegame, 0, error))
                return fail(result, output, error, Status::service_failed,
                            "LevelSavegame gate-byte write failed");
        } catch (...) {
            return fail(result, output, error, Status::service_failed,
                        "LevelSavegame gate-byte writer threw");
        }
    }

    if (!services->save_level_savegame) {
        // The original C++ Save is a void call. A provider outage is new to
        // this adapter, so restore the borrowed field before returning.
        std::string restore_error;
        if (force) {
            ++result.service_calls;
            try {
                if (!services->write_savegame_gate_39(services->context,
                        state->level_savegame, state->level_savegame_gate_39,
                        restore_error))
                    result.original_gate_restored = 1;
            } catch (...) {}
        }
        if (error.empty()) error = "LevelSavegame::Save provider unavailable";
        result.status = Status::service_unavailable;
        *output = result;
        return result.status;
    }

    ++result.service_calls;
    try {
        if (services->save_level_savegame(services->context,
                state->level_savegame, error)) {
            if (force) {
                std::string restore_error;
                ++result.service_calls;
                try {
                    if (!services->write_savegame_gate_39(services->context,
                            state->level_savegame, state->level_savegame_gate_39,
                            restore_error))
                        result.original_gate_restored = 1;
                } catch (...) {}
            }
            return fail(result, output, error, Status::service_failed,
                        "LevelSavegame::Save provider failed");
        }
    } catch (...) {
        if (force) {
            std::string restore_error;
            ++result.service_calls;
            try {
                if (!services->write_savegame_gate_39(services->context,
                        state->level_savegame, state->level_savegame_gate_39,
                        restore_error))
                    result.original_gate_restored = 1;
            } catch (...) {}
        }
        return fail(result, output, error, Status::service_failed,
                    "LevelSavegame::Save provider threw");
    }
    result.save_called = 1;

    if (force) {
        ++result.service_calls;
        try {
            if (services->write_savegame_gate_39(services->context,
                    state->level_savegame, state->level_savegame_gate_39, error))
                return fail(result, output, error, Status::service_failed,
                            "LevelSavegame original gate-byte restore failed");
        } catch (...) {
            return fail(result, output, error, Status::service_failed,
                        "LevelSavegame original gate-byte restore threw");
        }
        result.original_gate_restored = 1;
    } else {
        result.original_gate_restored = 1; // no source write was made
    }
    return finish(Status::saved);
}

} // namespace dh2::level_quick_save_v1
