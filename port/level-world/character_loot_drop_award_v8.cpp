#include "character_loot_drop_award_v8.hpp"
namespace dh2::character {
bool CharacterLootDropAwardV8::drop(data::LootTemporaryInventoryV8& inventory,std::uintptr_t source,std::uintptr_t killer,std::string& e){
 e.clear();if(running_){e="Unsupported destructive DropAndAwardLoot reentry";return false;}running_=true;struct Reset{bool& b;~Reset(){b=false;}}reset{running_};
 // Source empty inventory skips all source/player/RNG/resource queries.
 if(inventory.items().empty())return true;
 if(!source||!services_.position){e="Required actual source object position160";return false;}
 const float* position{};if(!services_.position(services_.context,source,position,e))return false;if(!position){e="Required same-World position backing";return false;}
 float destination[3]{};while(!inventory.items().empty()){
  if(!services_.random_drop_position){e="Required source same-Random drop-position kernel";return false;}
  if(!services_.random_drop_position(services_.context,source,killer,destination,e))return false;
  auto* original=inventory.items().front()->item.get();if(!original){e="Required actual source GetItem(0)";return false;}
  if(!services_.local_player_character){e="Required actual PlayerManager local-player +660 field";return false;}
  std::uintptr_t character{};if(!services_.local_player_character(services_.context,0,true,character,e))return false;
  LootItemObjectBorrowV8 spawned;if(!manager_.spawn(inventory,0,source,position,destination,character,spawned,e))return false;
  if(!spawned.identity){e="Required original non-NULL ItemManager Spawn receiver before automatic-pickup tail";return false;}
  if(!services_.item){e="Required actual spawned ItemObject internal inventory GetItem(0)";return false;}
  LootItemPickupBorrowV8 pickup;if(!services_.item(services_.context,spawned.identity,pickup,e))return false;
  if(!pickup.item)continue; // original GetItem(0) NULL returns without constants
  if(pickup.item!=original||!pickup.metadata||!pickup.pickup_override58){e="Required same actual spawned item/metadata/source pickup58 backing";return false;}
  auto type=std::int32_t(*pickup.pickup_override58);if(type==-1)type=pickup.metadata->record.words[3];
  if(!services_.constant){e="Required original PickUpType Automatic constant";return false;}
  std::int32_t automatic{};if(!services_.constant(services_.context,"PickUpType","Automatic",automatic,e))return false;
  if(type==automatic&&killer){if(!services_.interact){e="Required whole source ItemObject Interact automatic-pickup receiver";return false;}if(!services_.interact(services_.context,spawned.identity,killer,e))return false;}
 }return true;
}
}
