#include "character_profile_bootstrap_v59.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::character {
CharacterProfileBootstrapV59::CharacterProfileBootstrapV59(CharacterProfileBootstrapInputsV59 in):input_(std::move(in)){}
bool CharacterProfileBootstrapV59::coherent(std::string& e)const{
 if(!input_.save||!input_.load||&input_.load->save()!=input_.save.get()||
    !input_.character||input_.save->character()!=input_.character||!input_.actual_source_cells_lease||
    !input_.source_save14e8||*input_.source_save14e8!=reinterpret_cast<std::uintptr_t>(input_.save.get())||
    !source_slot_v122()||*source_slot_v122()!=input_.selected_slot||
    !input_.profile||!input_.profile->ready()){
  e="Required SAME constructed Character/Save14e8/PlayerInfo664/profile bootstrap receivers";return false;
 }
 if(input_.source_direct_save_slot_v122&&(input_.source_player_info_slot664||input_.source_direct_save_slot_v122!=input_.save->source_slot_field_v122())){e="Menu profile slot must borrow SAME Save slot without a fabricated PlayerInfo";return false;}
 return true;
}
bool CharacterProfileBootstrapV59::prepare(std::string& e){
 if(attempted_){e="Selected-profile bootstrap cannot replay its retained source prefix";return false;}attempted_=true;
 if(!coherent(e))return false;
 if(input_.selected_slot<0||input_.selected_slot>=4||!input_.selected_file_lease||
    (!input_.selected_file_bytes.data&&input_.selected_file_bytes.size)||!input_.source_set_slot){
  e="Required actual selected-slot file receipt and source SetSlot producer";return false;
 }
 {const auto cache=input_.profile->cache();
  if(!cache||cache.bytes().size()!=input_.selected_file_bytes.size||
     !std::equal(cache.bytes().begin(),cache.bytes().end(),input_.selected_file_bytes.data)){
   e="Constructed campaign cache differs from the retained selected profile read";return false;
  }
 }
 if(input_.prior_slot_store.save){
  if(input_.prior_slot_store.save!=input_.save.get()||input_.prior_slot_store.value!=*source_slot_v122()||
     input_.save->slot()!=input_.prior_slot_store.value){e="Stale actual pre-profile SetSlot execution receipt";return false;}
 }else if(!input_.source_set_slot(*source_slot_v122(),e))return false;
 if(!coherent(e))return false;
 if(input_.save->slot()!=input_.selected_slot){e="Source SetSlot did not publish SAME selected Save slot";return false;}
 const auto& previous=input_.load->profile();
 if(previous.identity&&previous.identity!=reinterpret_cast<std::uintptr_t>(input_.profile.get())){
  e="Cannot replace a different live Save+8 profile during bootstrap";return false;
 }
 if(!input_.load->publish_profile(input_.profile->receiver(),e))return false;
 if(!input_.reads.tables||!input_.reads.quests||input_.reads.quests->save()!=input_.save){e="Required actual immutable reader tables and SAME regular/volatile Quest owner";return false;}
 std::weak_ptr<CharacterProfileBootstrapV59> weak=shared_from_this();
 input_.reads.remaining={input_.reads.tables,[weak](const auto& q,auto& out,auto& e){auto self=weak.lock();
  if(!self){e="Expired actual selected-profile bootstrap";return false;}return self->remaining(q,out,e);
 }};
 reader_=std::make_shared<CharacterMenuProfileLoadV51>(input_.save,input_.profile,input_.reads);
 if(!input_.load->bind_services_v50(reader_->load_services(),e)||!input_.load->load(1,e))return false;
 if(!input_.defer_mask2_to_initpost_v62&&!input_.load->load(2,e))return false;
 prepared_=true;e.clear();return true;
}
bool CharacterProfileBootstrapV59::configure_equipment(player::PlayerEquipmentRenderInputsV1& in,std::string& e){
 if(!prepared_||!coherent(e)||in.character!=input_.character||in.saved_gear_v50||in.source_profile_load_v59){
  if(e.empty())e="Required single SAME selected-profile pre-Grant equipment delivery";return false;
 }
 std::weak_ptr<CharacterProfileBootstrapV59> weak=shared_from_this();
 in.source_profile_load_v59=[weak](auto& equipment,auto& e){auto self=weak.lock();
  if(!self){e="Expired actual profile before initial equipment grant";return false;}
  return self->before_initial_grant(equipment,e);
 };e.clear();return true;
}
bool CharacterProfileBootstrapV59::before_initial_grant(player::PlayerEquipmentRenderOwnerV1& equipment,std::string& e){
 if(!prepared_||load4_attempted_||initializing_||!coherent(e)||!equipment.inventory()||
    equipment.inventory()->character()!=input_.character||!equipment.property_view()){
  if(e.empty())e="Required once-only SAME fresh Gear source Load4 boundary";return false;
 }
 load4_attempted_=true;equipment_=&equipment;initializing_=true;
 struct Scope{bool& value;~Scope(){value=false;}}scope{initializing_};
 if(!input_.load->load(4,e))return false;
 const auto cache=input_.profile->cache();
 if(cache.section("GEAR")&&!gear_delivered_){e="Present source GEAR was not delivered by ordered Load4";return false;}
 load4_complete_=true;e.clear();return true;
}
bool CharacterProfileBootstrapV59::remaining(const data::PlayerSaveLoadRequestV1& q,
 data::PlayerSaveLoadResponseV1& out,std::string& e){
 if(!coherent(e)||q.save!=input_.save.get())return false;
 if(q.operation==data::PlayerSaveLoadOpV1::load_section&&q.section&&
    (!std::strcmp(q.section,"PROP")||!std::strcmp(q.section,"GEAR"))){
  if(q.profile.identity!=reinterpret_cast<std::uintptr_t>(input_.profile.get())||!q.reader_enabled||
     !initializing_||!equipment_||!equipment_->inventory()||equipment_->inventory()->character()!=input_.character){
   e="Required exact pre-Grant SAME profile/property/GEAR named reader";return false;
  }
  const auto cache=input_.profile->cache();if(!cache.section(q.section)){e.clear();return true;}
  const auto bytes=cache.payload(q.section);std::size_t used{};
  if(!std::strcmp(q.section,"PROP"))return level::player_save_properties_reader_v45(*input_.save,*equipment_->property_view(),bytes,used,e);
  if(gear_delivered_){e="Source GEAR cannot append twice in selected-profile bootstrap";return false;}
  data::InventoryLoadReceiptV1 receipt;
  if(!equipment_->load_saved_section_v59(bytes,receipt,e))return false;
  gear_delivered_=true;return true;
 }
 if(!input_.remaining.owner||!input_.remaining.invoke){
  e="Required actual campaign online/volatile source operation "+std::to_string(unsigned(q.operation));return false;
 }
 return input_.remaining.invoke(q,out,e);
}
bool CharacterProfileBootstrapV59::finish(player::PlayerEquipmentRenderOwnerV1& equipment,
 CharacterMenuCampaignSaveServicesV50 services,std::string& e){
 if(finish_attempted_||!load4_complete_||initializing_||equipment_!=&equipment||!equipment.ready()||!coherent(e)){
  if(e.empty())e="Required completed SAME initial equipment/profile prefix before menu writer binding";return false;
 }
 finish_attempted_=true;
 if(!services.quest_save_data){std::weak_ptr<CharacterMenuQuestsV51> weak=input_.reads.quests;
  services.quest_save_data=[weak](auto id,auto& stream,auto& e){auto quests=weak.lock();
   if(!quests){e="Expired SAME actual quest writer";return false;}return quests->save_quest(id,stream,e);
  };
 }
 writer_=std::make_shared<CharacterMenuCampaignSaveV50>(input_.load,input_.profile,std::move(services));
 if(!writer_->bind(e))return false;
 // Future menu Load20 must reach the SAME ready writer's PROP reader. Initial
 // GEAR cannot repeat because the guarded restore window has ended.
 input_.reads.campaign=writer_;
 reader_=std::make_shared<CharacterMenuProfileLoadV51>(input_.save,input_.profile,input_.reads);
 if(!input_.load->bind_services_v50(reader_->load_services(),e))return false;
 finished_=true;e.clear();return true;
}
}
