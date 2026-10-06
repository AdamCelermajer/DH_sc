#include "character_loot_pickup_fx_v32.hpp"
namespace dh2::character {
bool CharacterLootPickupFxV32::route(const LootInteractRequestV8& q,LootInteractResponseV8& out,bool& handled,std::string& e){
 handled=false;using O=LootInteractOperationV8;
 if(q.operation==O::loot_fx){handled=true;
  if(!q.key||std::string(q.key)!="loot_orb_fx"){e="Required original Item pickup FX literal";return false;}
  if(!tables_){e="Required actual Effects names for pickup static lookup";return false;}
  if(!lookup_initialized_){const auto& names=tables_.set_names();cached_set_=-1;
   for(std::size_t i=0;i<names.size();++i)if(names[i]==q.key){cached_set_=static_cast<std::int32_t>(i);break;}
   lookup_initialized_=true;
  }
  CharacterLootLiveBorrowV22 same;if(!actors_.drop_actor(q.character,same,e))return false;
  if(!same.position160){e="Required SAME picker raw Character position160";return false;}
  // Actual ELF symbol495d14 is the NULL-rotation PlayAnimFXSet overload,
  // not a distinct IrrFX factory. It passes position, NULL anchor/set parent.
  return fx_.play_set(cached_set_,same.position160,nullptr,0,nullptr,e);
 }
 if(q.operation==O::despawn){handled=true;
  if(!items_.factory().find(q.object)){e="Required SAME live145 Item receiver for source Despawn";return false;}
  if(!items_.pool().manager().despawn(q.object,e))return false;out={};return true;
 }
 return true;
}
}
