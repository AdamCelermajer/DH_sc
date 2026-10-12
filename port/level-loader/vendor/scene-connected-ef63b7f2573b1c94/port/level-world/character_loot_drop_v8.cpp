#include "character_loot_drop_v8.hpp"
namespace dh2::character::skills {
bool CharacterLootDropV8::character(std::uintptr_t id,WorldLootActorBorrowV8& out,std::string& e){
 out={};if(!id)return true;target_providers::Handle16 handle{};std::uintptr_t live{};
 if(world_.get_handle(id,&handle)||world_.resolve(&handle,&live,false)){e=world_.error();if(e.empty())e="Required same-World loot handle resolution";return false;}
 if(!live)return true;
 if(!services_.actor){e="Required actual same-World loot actor borrow";return false;}
 if(!services_.actor(services_.context,live,out,e))return false;
 if(out.identity!=live){e="World loot actor borrow identity mismatch";return false;}
 if(out.type_index!=0)out={};
 return true;
}
bool CharacterLootDropV8::drop_table(std::int32_t table,std::uintptr_t source,std::uintptr_t killer,std::int32_t index,std::string& e){
 e.clear();if(running_||pending_){e="Unsupported reentry or unconsumed previous world loot prefix";return false;}running_=true;struct Reset{bool& b;~Reset(){b=false;}}reset{running_};
 WorldLootActorBorrowV8 credited,victim;
 if(!character(killer,credited,e)||!character(source,victim,e))return false;
 bool eligible{};
 if(victim.identity){if(!victim.ai){e="Required actual source Character GetCharAI for loot gate";return false;}eligible=(victim.ai->flags&4)!=0;if(!eligible)eligible=(victim.ai->flags&2)!=0;}
 if(!eligible){if(!credited.identity)return true;if(!services_.is_player){e="Required actual source killer IsPlayer virtual";return false;}if(!services_.is_player(services_.context,credited.identity,eligible,e))return false;if(!eligible)eligible=victim.identity==credited.identity;}
 if(!eligible)return true;
 if(!tables_){e="Required actual loot tables for temporary inventory";return false;}
 pending_=std::make_unique<data::LootTemporaryInventoryV8>(tables_);
 // GetInventoryToDrop resolves the original killer argument AGAIN. Preserve
 // this fresh handle/query boundary; it can change after eligibility queries.
 WorldLootActorBorrowV8 bonus;if(!character(killer,bonus,e))return false;
 std::int32_t value_bonus{},power_bonus{};
 if(bonus.identity){if(!bonus.properties||dh2_property_resolve(bonus.properties,195,&value_bonus)||dh2_property_resolve(bonus.properties,196,&power_bonus)){e="Required same live killer properties195/196";return false;}}
 if(!services_.create){e="Required original temporary inventory AddLoot";return false;}
 if(!services_.create(services_.context,*pending_,table,value_bonus,power_bonus,index,e))return false;
 if(!services_.drop){e="Required original ItemManager DropAndAwardLoot world producer";return false;}
 if(!services_.drop(services_.context,*pending_,source,killer,index,e))return false;
 if(!pending_->items().empty()){e="World loot drop reported success without spawning all actual inventory items";return false;}
 pending_.reset();return true;
}
}
