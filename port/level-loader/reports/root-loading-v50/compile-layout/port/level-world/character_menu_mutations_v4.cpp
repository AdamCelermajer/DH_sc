#include "character_menu_mutations_v4.hpp"
namespace dh2::character {
namespace {
struct Frame {
 std::shared_ptr<void> original_owner;
 CharacterMenuMutationServicesV4 services;
 std::shared_ptr<data::PlayerSaveLoadOwnerV1> saved;
 data::OwnedInventoryServicesV4 inventory;
 std::unique_ptr<ui::CharacterMenuItemActionsV1> items;
 std::unique_ptr<ui::CharacterMenuSaveActionsV1> save;
};
bool required(const char* name,std::string& e){if(e.empty())e=std::string("Required source character-menu mutation ")+name;return false;}
bool inventory_effect(void* raw,data::FreshInventoryOwnedV4& inventory,
 const data::OwnedInventoryRequestV4& q,data::OwnedInventoryResponseV4& out,std::string& e){
 auto& f=*static_cast<Frame*>(raw);
 if(q.operation!=data::OwnedInventoryOperationV4::gold_notifications){
  return f.inventory.invoke?f.inventory.invoke(f.inventory.context,inventory,q,out,e):required("inventory effect",e);
 }
 // Whole positive SetGold3fdfd8 notification tail AFTER the actual gold store.
 // Source reads gold afresh after each trophy callback; do not snapshot it.
 return character_menu_gold_notifications_v4(inventory,f.services,e);
}
void inventory_observe(void* raw,data::FreshInventoryOwnedV4& inventory,const data::OwnedInventoryRequestV4& q){
 auto& f=*static_cast<Frame*>(raw);if(f.inventory.observe_storage)f.inventory.observe_storage(f.inventory.context,inventory,q);
}
}
bool character_menu_gold_notifications_v4(data::FreshInventoryOwnedV4& inventory,
 const CharacterMenuMutationServicesV4& s,std::string& e){
 const auto actor=inventory.character();if(!actor)return true;
 bool player{};if(!s.is_player||!s.is_player(actor,player,e))return required("SetGold IsPlayer",e);
 if(!player)return true;bool local{};
 if(!s.is_local_player||!s.is_local_player(actor,local,e))return required("SetGold IsLocalPlayer",e);
 if(!local)return true;
 constexpr std::int32_t thresholds[]{9999,99999,999999};
 constexpr const char* names[]{"gear_10kgold","gear_100kgold","gear_1mgold"};
 for(unsigned i=0;i<3;++i){if(inventory.gold()<=thresholds[i])return true;
  if(!s.achievement||!s.achievement(actor,names[i],e))return required("SetGold trophy continuation",e);}
 return true;
}
bool bind_character_menu_mutations_v4(ui::CharacterMenuActionsOwnerV1& actions,
 ui::CharacterMenuQueriesGraphV1& queries,skills::CharacterPlayerSkillsV6& skills,
 std::shared_ptr<data::PlayerSaveLoadOwnerV1> saved,
 CharacterMenuMutationServicesV4 services,std::string& e){
 const auto& actual=actions.bindings();
 if(!queries.owner||queries.actions!=&actions||!services.owner||!saved||
   actual.skills.identity()!=&skills||actual.save!=&saved->save()||
   skills.native_savegame()!=actual.save||!actual.equipment||
   actual.equipment->properties()!=skills.session().properties())return required("same Skill/Save/profile/Gear graph",e);
 auto frame=std::make_shared<Frame>();frame->original_owner=queries.owner;
 frame->services=std::move(services);frame->saved=std::move(saved);
 ui::CharacterMenuItemActionsGraphV1 items;items.owner=frame->services.owner;
 if(!actual.equipment->bind_menu_item_actions_v4(items,e))return false;
 const auto character=items.inventory->character();auto* f=frame.get();
 frame->inventory=items.inventory_services;items.inventory_services={f,inventory_effect,inventory_observe};
 items.transmute_multiplier=[f](auto& value,auto& error){return f->services.constant?
  f->services.constant("CharacterDesign","TransmuteMultiplier",value,error):required("TransmuteMultiplier",error);};
 items.is_local_player=[f,character](bool& value,auto& error){return f->services.is_local_player?
  f->services.is_local_player(character,value,error):required("IsLocalPlayer",error);};
 items.is_player=[f,character](bool& value,auto& error){return f->services.is_player?
  f->services.is_player(character,value,error):required("IsPlayer",error);};
 items.online=frame->services.online;items.drop_packet=frame->services.drop_packet;
 items.drop_world=frame->services.drop_world;
 items.achievement=[f,character](const char* key,auto& error){return f->services.achievement?
  f->services.achievement(character,key,error):required("achievement",error);};
 frame->items=std::make_unique<ui::CharacterMenuItemActionsV1>(std::move(items));
 ui::CharacterMenuSaveActionsGraphV1 save;save.owner=frame->services.owner;save.actions=&actions;
 save.constant=frame->services.constant;save.is_player=frame->services.is_player;
 save.achievement=frame->services.achievement;
 save.save=[f,&skills,expected_character=character](data::PlayerSavegameV1& actual_save,const auto& authority,
   std::uintptr_t character,data::PropertyView& properties,std::string& error){
  if(&actual_save!=&f->saved->save()||skills.native_savegame()!=&actual_save||authority.identity()!=&skills||
    character!=expected_character||properties.saved!=skills.session().property_view().saved||
    properties.resolved!=skills.session().property_view().resolved)return required("SG_Save same retained backing",error);
  // Original Character SG_Save3bc4a8 directly calls Save SG_Save464b2c.
  // That owner returns on NULL profile+8/disabled+c BEFORE any services.
  // Slot -1 alone is never treated as a successful no-write branch.
  data::PlayerSaveWriteOwnerV1 writer(f->saved,f->services.save);
  return writer.save(error);
 };
 frame->save=std::make_unique<ui::CharacterMenuSaveActionsV1>(std::move(save));
 queries.owner=frame;queries.item_actions=frame->items.get();queries.save_actions=frame->save.get();
 return true;
}
}
