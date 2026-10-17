#pragma once

#include "character_distribute_xp_v1.hpp"
#include "character_give_xp_v1.hpp"

namespace dh2::character_distribute_give_xp_dispatch_v1 {

// Borrows the canonical PyData view and the existing Character/PlayerManager
// service set. One transient _GiveXP Runtime is created for each reached
// DistributeXP callback; PropertyView, Save, PlayerManager, and callbacks stay
// owned by their existing gameplay owners.
struct Owner {
    const dh2_pycst_view* constants{};
    character_give_xp_v1::Backend backend{};
};

// Direct adapter for character_distribute_xp_v1::Services::give_xp. A missing
// runtime input or a failed source operation does not fabricate a source bool.
std::int32_t give_xp(
    void* context,
    character_distribute_xp_v1::CharacterView* character,
    std::int32_t amount_fixed,
    std::uint32_t update_player_stat,
    std::uint32_t* source_return,
    std::string& error);

} // namespace dh2::character_distribute_give_xp_dispatch_v1
