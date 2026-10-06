#include "world_item_drop_inventory_v9.hpp"
#include "world_loot_item_runtime_v1.hpp"
#include "character_menu_mutations_v4.hpp"
namespace dh2::character {
void bind_menu_drop_world_v9(CharacterMenuMutationServicesV4& menu,std::shared_ptr<void> world,
 WorldLootItemRuntimeV1& actual,WorldItemDropInventoryServicesV9 services){
 menu.drop_world=[world=std::move(world),&actual,services](data::FreshInventoryOwnedV4& inventory,
  std::uintptr_t source,std::uintptr_t scatter,std::uintptr_t owner,std::string& e){
  LootInventorySourceV9 borrow;if(!fresh_inventory_loot_source_v9(inventory,world,borrow,e))return false;
  return actual.drop_inventory_source_v9(borrow,source,scatter,owner,services,e);
 };
}
bool WorldLootItemRuntimeV1::drop_inventory_source_v9(LootInventorySourceV9& inventory,
 std::uintptr_t source,std::uintptr_t scatter,std::uintptr_t owner,
 const WorldItemDropInventoryServicesV9& services,std::string& e){
 e.clear();if(!source){e="Required source DropInventory non-NULL object assertion";return false;}
 if(!inventory.owner||!inventory.count||!inventory.item){e="Required same original DropInventory source";return false;}
 if(!inventory.count())return true;
 const float* actual_position{};if(!position(this,source,actual_position,e)||!actual_position)return false;
 const LootItemSourceServicesV9 transfer{this,[](void* raw,const LootItemSourceRequestV9& q,std::string& error){
  auto& runtime=*static_cast<WorldLootItemRuntimeV1*>(raw);auto* object=q.source.object?runtime.receiver(q.source.object->identity):nullptr;
  if(!object||!q.inventory){error="Required same pooled ItemObject source InitAgain";return false;}
  return object->init_again_source_v9(*q.inventory,q.source.index,q.source.character,error);
 }};
 while(inventory.count()){
  float destination[3]{};if(!random_position(this,source,scatter,destination,e))return false;
  LootItemObjectBorrowV8 spawned;
  if(!manager_.spawn_source_v9(inventory,0,source,actual_position,destination,owner,spawned,transfer,e))return false;
  // Original unconditionally continues into source IsCharacter even if Spawn
  // returned NULL; a positive Character then dereferences the result to lock.
  bool character{};if(!services.is_character||!services.is_character(services.context,source,character,e)){if(e.empty())e="Required source DropInventory IsCharacter virtual24";return false;}
  if(character){
   std::int32_t index{};if(!services.friendly_index||!services.friendly_index(services.context,source,index,e)){if(e.empty())e="Required source PlayerInfo friendly678";return false;}
   auto* item=receiver(spawned.identity);if(!item){e="Required non-NULL spawned source ItemObject lock receiver";return false;}
   item->fields().lock3b8=5000;item->fields().player_id3c0=static_cast<std::int16_t>(index);
  }
 }
 return true;
}
}
