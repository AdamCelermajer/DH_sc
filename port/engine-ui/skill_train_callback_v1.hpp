#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace dh2::ui {

struct SkillTrainCallbackArgsV1 {
    std::int32_t skill_index = 0;
    std::int32_t player_index = 0;
    bool test_only = false;
};

struct SkillTrainCallbackResultV1 {
    std::uintptr_t character = 0;
    std::int32_t skill_index = 0;
    bool player_found = false;
    bool training_called = false;
    bool has_result = false;
    bool test_only = false;
    std::uint32_t source_return = 0;
    std::int32_t points_left = 0;
};

struct SkillSaveCallbackResultV1 {
    std::uintptr_t character = 0;
    std::int32_t player_index = 0;
    bool player_found = false;
    bool save_called = false;
    bool post_save_called = false;
};

// NativeSaveGame (0x43dc08) converts arg0 only when argc is exactly one;
// every other arity selects local PlayerInfo 0. A missing Player is a source
// no-op. The save provider must be the active Character's canonical profile
// transport; this helper creates no Save or profile owner.
template<class ToInteger, class ResolvePlayer, class Save, class PostSave>
bool apply_skill_save_callback_v1(std::size_t argc, ToInteger&& to_integer,
        ResolvePlayer&& resolve_player, Save&& save, PostSave&& post_save,
        SkillSaveCallbackResultV1& result, std::string& error) {
    result = {};
    error.clear();
    if (argc == 1 && !to_integer(0, result.player_index, error)) return false;
    std::uintptr_t character = 0;
    if (!resolve_player(result.player_index, character, error)) return false;
    if (!character) { error.clear(); return true; }
    result.character = character;
    result.player_found = true;
    if (!save(character, error)) return false;
    result.save_called = true;
    if (!post_save(character, error)) return false;
    result.post_save_called = true;
    error.clear();
    return true;
}

// NativeSaveGame @ 0x43dc08 runs after Character::SG_Save. It awards
// epic_maxskill for each nonzero skill at MaxSkillLevelDVeryHard, then gates
// epic_all_skills_unlocked and epic_spent_all_skill_points on Character's
// IsPlayer virtual. Trophy unlocks are passed to the existing TrophyManager
// owner in this source order.
template<class Unlock>
bool apply_skill_save_trophy_effects_v1(
        const std::vector<std::int32_t>& skill_levels, bool is_player,
        std::int32_t max_skill_level, std::int32_t remaining_skill_points,
        std::int32_t character_level, std::int32_t max_character_level,
        Unlock&& unlock, std::string& error) {
    error.clear();
    bool all_skills_unlocked = true;
    for (const auto level : skill_levels) {
        if (level == 0) {
            all_skills_unlocked = false;
            continue;
        }
        if (level == max_skill_level &&
            !unlock("epic_maxskill", error)) return false;
    }
    if (!is_player) return true;
    if (all_skills_unlocked &&
        !unlock("epic_all_skills_unlocked", error)) return false;
    if (remaining_skill_points == 0 &&
        character_level == max_character_level &&
        !unlock("epic_spent_all_skill_points", error)) return false;
    error.clear();
    return true;
}

// The source callback converts both indices with as_value::to_number() and
// only reads the optional test flag when argc is exactly three.
template<class ToInteger, class ToBoolean>
bool decode_skill_train_callback_v1(std::size_t argc, ToInteger&& to_integer,
        ToBoolean&& to_boolean, SkillTrainCallbackArgsV1& out,
        std::string& error) {
    out = {};
    error.clear();
    if (argc != 2 && argc != 3) {
        error = "NativeSkillsTrainSkill expects two indices and an optional test flag";
        return false;
    }
    if (!to_integer(0, out.skill_index, error) ||
        !to_integer(1, out.player_index, error)) return false;
    out.test_only = argc == 3 ? to_boolean(2) : false;
    return true;
}

// Source NativeSkillsTrainSkill @ 0x43d548 resolves the Player after reading
// [skillIndex, playerIndex, optionalTestOnly]. A negative index still returns
// current points (or false for testOnly), while a valid index delegates to the
// one canonical Character::IncSkill/Save/V4/Player-runtime provider.
template<class ToInteger, class ToBoolean, class ResolvePlayer,
         class ReadPoints, class Train>
bool apply_skill_train_callback_v1(std::size_t argc, ToInteger&& to_integer,
        ToBoolean&& to_boolean, ResolvePlayer&& resolve_player,
        ReadPoints&& read_points, Train&& train,
        SkillTrainCallbackResultV1& result, std::string& error) {
    result = {};
    SkillTrainCallbackArgsV1 call{};
    if (!decode_skill_train_callback_v1(argc,
            std::forward<ToInteger>(to_integer),
            std::forward<ToBoolean>(to_boolean), call, error)) return false;
    std::uintptr_t character = 0;
    if (!resolve_player(call.player_index, character, error)) return false;
    if (!character) { error.clear(); return true; }
    result.character = character;
    result.skill_index = call.skill_index;
    result.player_found = true;
    result.test_only = call.test_only;
    if (call.skill_index < 0) {
        if (!read_points(character, result.points_left, error)) return false;
        result.has_result = true;
        error.clear();
        return true;
    }
    if (!train(character, call.skill_index, call.test_only,
               result.source_return, result.points_left, error)) return false;
    result.training_called = true;
    result.has_result = true;
    error.clear();
    return true;
}

} // namespace dh2::ui
