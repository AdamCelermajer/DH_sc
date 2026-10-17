#include "character_distribute_give_xp_dispatch_v1.hpp"

namespace dh2::character_distribute_give_xp_dispatch_v1 {

std::int32_t give_xp(
    void* context,
    character_distribute_xp_v1::CharacterView* character,
    std::int32_t amount_fixed,
    std::uint32_t update_player_stat,
    std::uint32_t* source_return,
    std::string& error) {
    if (!context || !character || !character->identity ||
        !character->properties || !character->savegame ||
        !source_return || !character->savegame->character() ||
        character->savegame->character() != character->identity ||
        !static_cast<const Owner*>(context)->constants) {
        error = "DistributeXP _GiveXP adapter requires the same live Character, PropertyView, Save, and PyData owners";
        return 1;
    }

    const auto& owner = *static_cast<const Owner*>(context);
    character_give_xp_v1::Bindings bindings{
        character->identity,
        amount_fixed,
        character->properties,
        character->savegame,
        owner.constants,
        update_player_stat != 0};
    character_give_xp_v1::Runtime runtime(bindings, owner.backend);
    character_give_xp_v1::Result result{};
    const auto status = runtime.give_xp(&result, error);
    if (status != character_give_xp_v1::Status::complete) {
        if (error.empty())
            error = "Source Character::_GiveXP progression did not complete";
        return 1;
    }

    *source_return = result.source_return;
    error.clear();
    return 0;
}

} // namespace dh2::character_distribute_give_xp_dispatch_v1
