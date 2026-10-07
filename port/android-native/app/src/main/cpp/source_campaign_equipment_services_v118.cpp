#include "source_campaign_equipment_services_v118.hpp"
#include "source_process_trophies_v100.hpp"
#include "renderer_native_gslevel_v27.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <character_menu_mutations_v4.hpp>
#include <source_item_resources_v88.hpp>
#include <item_presentation_v5.hpp>
#include <stdexcept>
#include <cstring>
namespace model_renderer {namespace {
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
using Inventory=dh2::data::FreshInventoryOwnedV4;
using Request=dh2::data::OwnedInventoryRequestV4;
using Response=dh2::data::OwnedInventoryResponseV4;
using Operation=dh2::data::OwnedInventoryOperationV4;
bool required(const char* leaf,std::string& e){if(e.empty())e=std::string("Required actual canonical equipment ")+leaf;return false;}
struct EquipmentServicesV118 {
 std::weak_ptr<Record> actor;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::shared_ptr<dh2::character::SourceItemResourcesV88> items;
 bool presentation_retired{};
 bool current(Inventory& inventory,std::shared_ptr<Record>& r,
  std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& e)const{
  r=actor.lock();app=application.lock();
  if(presentation_retired||!r||!app||!r->actor||!r->actor->object||!items||!items->ready()||
     inventory.properties()!=r->properties||inventory.table().identifiers!=items->loot().items().identifiers)
   return required("SAME retained Inventory/Character/table owner",e);
  //Menu Drop uses the original NULL-character temporary with these SAME
  //tables/properties. Positive Character inventories must be actual37c.
  if(inventory.character()&&(inventory.character()!=r->actor->object->identity||r->inventory37c!=&inventory))
   return required("actual moved Character inventory37c",e);
  return true;
 }
 bool trophy_services(const std::shared_ptr<Record>& r,
  const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
  dh2::character::CharacterMenuMutationServicesV4& out,
  dh2::android_ui::ProcessTrophyBorrowV100& captured,std::string& e){
  if(!r->actor||!r->actor->object||!app->source_player_manager_v59())return required("same PM/IsPlayer source receivers",e);
  out.owner=items;
  out.is_player=[r](std::uintptr_t id,bool& value,std::string& e){
   if(id!=r->actor->object->identity)return required("SetGold IsPlayer receiver",e);return r->is_player(value,e);
  };
  const auto pm=app->source_player_manager_v59();
  out.is_local_player=[pm](std::uintptr_t id,bool& value,std::string& e){return pm->source_is_local_player_v61(id,value,e);};
  out.achievement=[app,&captured](std::uintptr_t,const char* name,std::string& e){
   //No trophy method is called until the original threshold/predicate reaches
   //it. Keep the first actual manager lease for subsequent source rereads.
   if(!captured.owner&&!dh2::android_ui::borrow_source_process_trophies_v100(app,captured,e))return false;
   if(!captured.native||!captured.manager||!name)return required("positive captured TrophyManager",e);
   if(captured.native->unlock_named(name)){e=captured.manager->error();return required("original named UnlockTrophy",e);}
   e.clear();return true;
  };
  return true;
 }
 static bool invoke(void* raw,Inventory& inventory,const Request& q,Response& out,std::string& e){
  auto& self=*static_cast<EquipmentServicesV118*>(raw);std::shared_ptr<Record> r;
  std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> app;
  if(!self.current(inventory,r,app,e))return false;
  switch(q.operation){
   case Operation::gold_notifications:{
    //The actual SetGold store already happened in the authoritative owner.
    //NULL-character is a proved source return, never zeroed player gold.
    if(!inventory.character()){e.clear();return true;}
    dh2::character::CharacterMenuMutationServicesV4 services;dh2::android_ui::ProcessTrophyBorrowV100 trophy;
    if(!self.trophy_services(r,app,services,trophy,e))return false;
    return dh2::character::character_menu_gold_notifications_v4(inventory,services,e);
   }
   case Operation::full_notifications:{
    //3ff7a8 follows actual insertion/IsInventoryFull. It has no HUD lookup;
    //the only positive presentation is the process full_inventory trophy.
    if(!inventory.character())return required("Inventory full-notification NULL Character dereference",e);
    if(q.index>=inventory.items().size()||!inventory.items()[q.index]||
       inventory.items()[q.index]->item.get()!=q.item)return required("actual inserted full-inventory Item",e);
    dh2::character::CharacterMenuMutationServicesV4 services;dh2::android_ui::ProcessTrophyBorrowV100 trophy;
    if(!self.trophy_services(r,app,services,trophy,e))return false;
    bool player{},local{};if(!services.is_player(inventory.character(),player,e))return false;
    if(!player){e.clear();return true;}
    if(!services.is_local_player(inventory.character(),local,e))return false;
    if(!local){e.clear();return true;}
    return services.achievement(inventory.character(),"full_inventory",e);
   }
   case Operation::add_power:
    return q.item&&self.items->presentation()?
     self.items->presentation()->add_power(*q.item,q.argument,static_cast<std::int32_t>(q.index),self.items->text(),e):required("actual ItemPower append owner",e);
   case Operation::inventory_full:{
    bool full{};const dh2::data::OwnedInventoryServicesV4 services{&self,invoke,observe};
    if(!inventory.inventory_full(full,services,e))return false;out.value=full;e.clear();return true;
   }
   case Operation::destroy_item:
    return q.item&&self.items->presentation()?self.items->presentation()->forget(*q.item,e):required("actual Item destruction presentation",e);
   case Operation::debug_load:
    if(!r->services.debug||!r->services.debug_files||dh2_character_debug_load(r->services.debug,r->services.debug_files)!=1)return required("inventory Debug.Load",e);
    e.clear();return true;
   case Operation::debug_query:{
    std::uint32_t value{};
    if(!q.name||!r->services.debug||!r->services.debug_files||dh2_character_debug_get(&value,r->services.debug,q.name,r->services.debug_files)!=1)return required("inventory Debug.GetSwitch",e);
    std::memcpy(&out.value,&value,4);e.clear();return true;
   }
   case Operation::update_name:return q.item&&dh2::data::item_update_name_v5(*q.item,self.items->text(),e);
   case Operation::update_stats:return q.item&&dh2::data::item_update_stats_v5(*q.item,self.items->text(),e);
   case Operation::update_requirements:return q.item&&dh2::data::item_update_requirements_v5(*q.item,self.items->text(),e);
   default:e="Required actual equipment continuation at source "+std::to_string(q.source_caller);return false;
  }
 }
 static void observe(void* raw,Inventory& inventory,const Request& q){
  if(q.operation!=Operation::destroy_item)return;
  auto& self=*static_cast<EquipmentServicesV118*>(raw);std::shared_ptr<Record> r;
  std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> app;std::string e;
  if(!self.current(inventory,r,app,e)||!q.item||!self.items->presentation()||!self.items->presentation()->forget(*q.item,e))
   throw std::runtime_error(e.empty()?"Actual inventory item destruction presentation failed":e);
 }
 bool release(Record& actual,std::string& e){
  auto record=actor.lock();if(!record||record.get()!=&actual||!items||!items->presentation())return required("SAME Gear power-alias retirement",e);
  if(presentation_retired){e.clear();return true;}
  auto* inventory=record->inventory37c;
  if(!inventory||inventory->character()!=record->actor->object->identity||inventory->properties()!=record->properties)
   return required("actual Inventory before power-description teardown",e);
  for(const auto& slot:inventory->items())if(slot&&slot->item&&!items->presentation()->forget(*slot->item,e))return false;
  presentation_retired=true;e.clear();return true;
 }
};
}
bool make_source_campaign_equipment_services_v118(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& record,
 const std::shared_ptr<dh2::character::SourceItemResourcesV88>& items,
 dh2::data::OwnedInventoryServicesV4& out,std::shared_ptr<void>& owner,std::string& e){
 if(!app||!record||!record->actor||!record->actor->object||!items||!items->ready()||out.invoke||owner)
  return required("once-enrolled SAME Gear required-services owner",e);
 auto services=std::make_shared<EquipmentServicesV118>();services->actor=record;services->application=app;services->items=items;
 out={services.get(),EquipmentServicesV118::invoke,EquipmentServicesV118::observe};owner=std::move(services);e.clear();return true;
}
bool release_source_campaign_equipment_presentation_v118(const std::shared_ptr<void>& provider,
 dh2::world::CanonicalCharacterCandidateRecordV60& record,std::string& e){
 if(!provider)return required("actual required-service retention",e);
 return std::static_pointer_cast<EquipmentServicesV118>(provider)->release(record,e);
}
}
