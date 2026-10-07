#include "../loot_source_fields_v47.hpp"
#include <cassert>
// Source-only regression, deliberately not executed under the RAM hold.
// This tests the exact C1 -> Character14e8 -> SetCharacter association seam;
// it does not claim whole PlayerManager AddCharacter or player InitPost.
int main(){
 using namespace dh2;
 constexpr std::uintptr_t actor_id=0x6001;
 character::LootPlayerFieldAssociationV47 fields(actor_id);
 std::string error;
 auto save=std::make_shared<data::PlayerSavegameV1>();
 assert(save->character()==0&&*fields.save_slot14e8()==0);
 assert(fields.source_fresh_save_store_then_character_v60(save,error));
 assert(*fields.save_slot14e8()==reinterpret_cast<std::uintptr_t>(save.get()));
 assert(save->character()==actor_id);
 const auto slot=*fields.save_slot14e8();
 auto replacement=std::make_shared<data::PlayerSavegameV1>();
 assert(!fields.source_fresh_save_store_then_character_v60(replacement,error));
 assert(*fields.save_slot14e8()==slot&&replacement->character()==0);
 assert(!fields.source_fresh_save_store_then_character_v60(save,error));
 assert(*fields.save_slot14e8()==slot&&save->character()==actor_id);
 character::LootPlayerFieldAssociationV47 other(actor_id+1);
 assert(!other.source_fresh_save_store_then_character_v60(save,error));
 assert(*other.save_slot14e8()==0&&save->character()==actor_id);
 assert(!other.source_fresh_save_store_then_character_v60({},error));
 assert(*other.save_slot14e8()==0);
}
