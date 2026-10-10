#include "quest_reward_give_xp_bridge_v1.hpp"

namespace dh2::quest_reward_give_xp_bridge_v1 {

std::int32_t give_xp(
    void* raw,
    data::quest_reward_execution_v1::Record&,
    data::quest_reward_execution_v1::CharacterRef& reward_character,
    std::int32_t amount_fixed,
    std::uint8_t source_flag,
    bool* granted) {
    if (!raw || !granted) return 1;
    auto& context = *static_cast<Context*>(raw);
    *granted = false;
    context.last_source_return = 0;
    ++context.calls;

    const auto* view = context.character;
    if (!context.xp_owner || !view || !context.expected_character ||
        source_flag != 1 || reward_character.identity != context.expected_character ||
        !reward_character.reward_xp_1504 || view->identity != context.expected_character ||
        !view->properties || !view->savegame ||
        view->savegame->character() != context.expected_character) {
        context.error = "Reward_XP::Give requires its source a3=1 and the same live Character/Save/PropertyView owner";
        return 1;
    }

    std::uint32_t source_return = 0;
    const auto status = character_distribute_give_xp_dispatch_v1::give_xp(
        context.xp_owner, context.character, amount_fixed, source_flag,
        &source_return, context.error);
    if (status) {
        if (context.error.empty())
            context.error = "Character::_GiveXP did not complete through its selected runtime";
        return 1;
    }

    // Reward_XP::Give always returns 1 after the _GiveXP call. The original
    // _GiveXP bool only gates storing Reward+0x1504, so preserve it separately.
    context.last_source_return = source_return;
    *granted = source_return != 0;
    context.error.clear();
    return 0;
}

} // namespace dh2::quest_reward_give_xp_bridge_v1
