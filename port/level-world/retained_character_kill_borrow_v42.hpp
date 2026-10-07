#pragma once
#include "retained_character_actor_v1.hpp"
#include "character_kill_live_v21.hpp"
namespace dh2::character {
// Same retained NPC receiver projection for CharacterKillProductionV23::add.
// No constructor replay, alternate life, numeric registry remapping or copy of
// source metadata; negative preflight leaves the output unchanged.
bool retained_character_kill_borrow_v42(RetainedCharacterActorV1&,
 std::shared_ptr<void> actual_receiver,data::AggroTable& actual_outgoing,
 CharacterKillLiveBorrowV21&,std::string&);
}
