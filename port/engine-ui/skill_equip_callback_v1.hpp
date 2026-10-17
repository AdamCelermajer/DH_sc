#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

namespace dh2::ui {

struct SkillEquipCallbackArgsV1 {
    std::int32_t slot_index = 0;
    std::int32_t skill_index = 0;
    std::int32_t player_index = 0;
};

// NativeEquipSkill @ 0x43d63c converts [slot, skill, player] with
// as_value::to_number(), then resolves the Player before asking whether the
// skill can be equipped and writing that slot into the same Save owner.
template<class ToInteger>
bool decode_skill_equip_callback_v1(std::size_t argc, ToInteger&& to_integer,
        SkillEquipCallbackArgsV1& out, std::string& error) {
    out = {};
    error.clear();
    if (argc != 3) {
        error = "NativeEquipSkill expects [slotIndex, skillIndex, playerIndex]";
        return false;
    }
    return to_integer(0, out.slot_index, error) &&
           to_integer(1, out.skill_index, error) &&
           to_integer(2, out.player_index, error);
}

// Source order: resolve NativeGetPlayerChar before testing IsSkillEquippable;
// a missing character and a negative/unavailable skill both finish without a
// Save write. The final provider must mutate the existing canonical Save/V4/
// Player UpdateSkills owners; this adapter creates no gameplay state.
template<class ToInteger, class ResolvePlayer, class EquipSkill>
bool apply_skill_equip_callback_v1(std::size_t argc, ToInteger&& to_integer,
        ResolvePlayer&& resolve_player, EquipSkill&& equip_skill,
        std::string& error) {
    SkillEquipCallbackArgsV1 call{};
    if (!decode_skill_equip_callback_v1(argc,
            std::forward<ToInteger>(to_integer), call, error)) return false;
    std::uintptr_t character = 0;
    if (!resolve_player(call.player_index, character, error)) return false;
    if (!character || call.skill_index < 0) {
        error.clear();
        return true;
    }
    return equip_skill(character, call.slot_index, call.skill_index, error);
}

} // namespace dh2::ui
