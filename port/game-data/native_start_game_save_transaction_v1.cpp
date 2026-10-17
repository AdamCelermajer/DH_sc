#include "native_start_game_save_transaction_v1.hpp"

#include <utility>

namespace dh2::data {

bool begin_native_start_game_save_transaction_v1(
    const NativeStartGameMetadataOwnerV1& owner, const LevelTables& levels,
    const NativeStartGameRequestV1& request,
    NativeStartGameSaveTransactionV1& output,
    std::int32_t& current_difficulty, std::string& error) {
    error.clear();
    if (!owner.context || !owner.save_identity || !owner.read ||
        !owner.save_numeric_request || !owner.clear_spawn_point_and_save) {
        error = "NativeStartGame selected metadata Save owner is incomplete";
        return false;
    }

    NativeStartGameSaveViewV1 save;
    if (!owner.read(owner.context, owner.save_identity, save, error)) {
        if (error.empty()) error = "NativeStartGame selected metadata Save read failed";
        return false;
    }
    if (save.slot < 0) {
        error = "NativeStartGame selected metadata Save has no slot";
        return false;
    }

    // NativeStartGame updates CurrentDiff and optionally SG_Saves the selected
    // temporary Save before it reads the difficulty-indexed LevelList/LUSP
    // fields. SG_Save's section writers may mutate ordinary Save fields, so a
    // numeric request must re-read through this same owner before resolving
    // the load plan. Without a numeric request, the first read is that source
    // snapshot and no second read is needed.
    NativeStartGameRequestV1 post_save_request = request;
    if (request.has_numeric_difficulty) {
        if (save.unlocked_difficulty < 0 || save.unlocked_difficulty >= 3 ||
            request.current_difficulty < 0 || request.current_difficulty >= 3 ||
            request.requested_difficulty < 0 || request.requested_difficulty >= 3) {
            error = "NativeStartGame difficulty index outside the three source rows";
            return false;
        }
        if (save.unlocked_difficulty >= request.requested_difficulty)
            current_difficulty = request.requested_difficulty;
        else
            current_difficulty = request.current_difficulty;
        post_save_request.current_difficulty = current_difficulty;
        if (!owner.save_numeric_request(owner.context, owner.save_identity,
                                        current_difficulty, error)) {
            if (error.empty()) error = "NativeStartGame numeric SG_Save failed";
            return false;
        }
        NativeStartGameSaveViewV1 after_save;
        if (!owner.read(owner.context, owner.save_identity, after_save, error)) {
            if (error.empty()) error = "NativeStartGame post-SG_Save read failed";
            return false;
        }
        if (after_save.slot != save.slot) {
            error = "NativeStartGame selected Save slot changed during numeric SG_Save";
            return false;
        }
        save = after_save;
    } else {
        current_difficulty = request.current_difficulty;
    }

    NativeStartGameSaveTransactionV1 candidate;
    if (!resolve_native_start_game_plan_v1(levels, save, post_save_request,
                                           candidate.plan, error)) {
        return false;
    }
    candidate.plan.difficulty_before_request = request.current_difficulty;
    if (!candidate.plan.should_launch) {
        error = "NativeStartGame selected metadata Save did not produce a launch";
        return false;
    }
    candidate.save_identity = owner.save_identity;
    candidate.phase = NativeStartGameSavePhaseV1::planned;

    output = std::move(candidate);
    error.clear();
    return true;
}

bool save_spawn_flag_before_native_load_v1(
    const NativeStartGameMetadataOwnerV1& owner,
    NativeStartGameSaveTransactionV1& transaction,
    std::int32_t& current_difficulty, std::string& error) {
    error.clear();
    if (transaction.phase != NativeStartGameSavePhaseV1::planned ||
        !transaction.plan.should_launch || !transaction.save_identity ||
        transaction.save_identity != owner.save_identity || !owner.context ||
        !owner.clear_spawn_point_and_save) {
        error = "NativeStartGame spawn-save owner or phase mismatch";
        return false;
    }
    if (!transaction.plan.clear_saved_spawn_flag_before_load) {
        error = "NativeStartGame plan omitted its source spawn-flag clear";
        return false;
    }
    const auto row = transaction.plan.saved_spawn_flag_row_to_clear;
    if (row < 0 || row >= 3) {
        error = "NativeStartGame selected spawn-flag row is invalid";
        return false;
    }
    if (!owner.clear_spawn_point_and_save(owner.context,
            transaction.save_identity, std::size_t(row),
            current_difficulty, error)) {
        if (error.empty()) error = "NativeStartGame spawn-flag SG_Save failed";
        return false;
    }
    transaction.phase = NativeStartGameSavePhaseV1::spawn_flag_saved;
    error.clear();
    return true;
}

}  // namespace dh2::data
