// New-only staged connection. Root must install actual profile during original
// PlayerSaveLoad create_profile; never replace a published positive profile.
// Headers campaign_save_profile_v45.hpp/player_save_metadata_writer_v45.hpp/
// player_save_inventory_writer_v45.hpp. Include after current renderer owners.
struct RendererCampaignSaveProvidersV45 {
 std::shared_ptr<void> storage;
 std::shared_ptr<dh2::level::CampaignSaveProfileV45> profile;
 std::function<bool(std::int32_t&,std::string&)> current_difficulty;
 const std::vector<std::string>* power_names{};
 const std::vector<std::string>* level_names{};const std::vector<std::string>* map_names{};
 // Exact positive Quest::_saveQuestData condition/objective/list continuation.
 std::function<bool(std::uintptr_t,dh2::level::SavegameStreamV2&,std::string&)> quest_save_data;
 dh2::data::PlayerSaveWriteServicesV1 online_quests;
};
class RendererCampaignSaveBindingsV45 {
 PlayerSkillsRuntime& t_;RendererCampaignSaveProvidersV45 services_;
 dh2::character::CharacterGameDesign::Borrow design_;
 std::shared_ptr<dh2::data::PlayerSaveNamedWriterV1> named_;
 std::shared_ptr<dh2::level::PlayerSaveMetadataWriterV45> metadata_;
 std::shared_ptr<int> callback_lifetime_=std::make_shared<int>(1);
 bool bound_{};
public:
 RendererCampaignSaveBindingsV45(PlayerSkillsRuntime& t,RendererCampaignSaveProvidersV45 s):t_(t),services_(std::move(s)){}
 bool bind(std::string& e){
  if(bound_||!services_.storage||!services_.profile||!services_.profile->ready()||!services_.current_difficulty||
     !services_.power_names||!services_.level_names||!services_.map_names||!t_.world||!t_.world->design||
     !t_.player_save_load||!t_.player||!t_.world->skills_owner||!player_equipment||!player_equipment->inventory()){
   e="Required actual campaign profile/difficulty/194/PowerNames/LVLS-QEST-FTVL/Save/Gear providers";return false;
  }
  if(t_.player_save_load->profile().identity!=reinterpret_cast<std::uintptr_t>(services_.profile.get())||
     &t_.player_save_load->save()!=t_.player->native_savegame()){
   e="Required original published SAME PlayerSave profile+8 before campaign writer binding";return false;
  }
  auto authority=std::shared_ptr<dh2::data::PlayerSaveLoadOwnerV1>(services_.storage,t_.player_save_load.get());
  design_=t_.world->design->borrow();
  named_=std::make_shared<dh2::data::PlayerSaveNamedWriterV1>(authority,t_.world->skills_owner->borrow());
  metadata_=std::make_shared<dh2::level::PlayerSaveMetadataWriterV45>(authority,
   dh2::level::PlayerSaveMetadataServicesV45{services_.storage,design_.characters(),services_.current_difficulty});
  for(const char* tag:{"PNAM","PLVL","SKIL","FAES","CFEE"})if(!services_.profile->register_named_writer(tag,named_,e))return false;
  std::weak_ptr<dh2::level::PlayerSaveMetadataWriterV45> weak=metadata_;
  for(const char* tag:{"PCLS","PDFL","LNAM","LEPT","LUSP"}){
   const std::string name=tag;if(!services_.profile->register_writer(tag,metadata_,[weak,name](auto& stream,auto& error){auto m=weak.lock();if(!m){error="Expired real campaign metadata writer";return false;}return m->write(name.c_str(),stream,error);},e))return false;
  }
  if(!services_.profile->register_writer("GEAR",callback_lifetime_,[this](auto& stream,auto& error){
   if(!player_equipment||!player_equipment->inventory()||player_equipment->inventory()->character()!=t_.fsm.character){error="Required SAME live campaign inventory";return false;}
   return dh2::level::player_save_inventory_writer_v45(*player_equipment->inventory(),*services_.power_names,stream,error);
  },e))return false;
  if(!services_.profile->register_writer("PROP",callback_lifetime_,[this](auto& stream,auto& error){
   if(!t_.player||!t_.player_save_load){error="Required same retained PROP Save/Character";return false;}
   const auto* flag=t_.player_save_load->save().source_property_tail194_v45();
   return dh2::level::player_save_properties_writer_v45(t_.player->session().property_view(),flag,stream,error);
  },e))return false;
  for(const char* tag:{"LVLS","QEST","FTVL"}){const std::string name=tag;
   if(!services_.profile->register_writer(tag,callback_lifetime_,[this,name](auto& stream,auto& error){
    auto& save=t_.player_save_load->save();
    if(name=="LVLS")return dh2::level::player_save_level_states_v45({services_.storage,services_.level_names,services_.map_names,&save.level_states_v45(),&save.map_states_v45()},stream,error);
    if(name=="FTVL")return dh2::level::player_save_fast_travel_v45(&save.fast_travel_v45(),stream,error);
    const auto& regular=save.regular_quests_v45();const auto& volatile_quests=save.volatile_quests_v45();
    dh2::level::QuestSaveCollectionBorrowV45 r{services_.storage,&regular.source_quests_v45(),&regular.progress(),services_.quest_save_data};
    dh2::level::QuestSaveCollectionBorrowV45 v{services_.storage,&volatile_quests.source_quests_v45(),&volatile_quests.progress(),services_.quest_save_data};
    return dh2::level::player_save_quests_v45(t_.player_save_load->save_mode(),r,v,stream,error);
   },e))return false;
  }
  bound_=true;return true;
 }
 dh2::data::PlayerSaveWriteServicesV1 write_services(){
  std::weak_ptr<int> alive=callback_lifetime_;
  return services_.profile->write_services([this,alive](const auto& q,auto& out,auto& e){
   auto guard=alive.lock();if(!guard){e="Expired actual renderer campaign Save binding";return false;}
   if(!bound_||!t_.player_save_load||q.authority!=t_.player_save_load.get()){e="Required bound SAME campaign source Save request";return false;}
   if(q.operation==dh2::data::PlayerSaveWriteOpV1::online){
    std::uintptr_t identity{};std::int32_t online{};
    if(!EquipmentPlatform::query(equipment_platform.get(),dh2::player::EquipmentWorldQueryV1::online,0,identity,online,e))return false;out.flag=online!=0;return true;
   }
   if(!services_.online_quests.owner||!services_.online_quests.invoke){e="Required source campaign network/quest/checkpoint selector "+std::to_string(unsigned(q.operation));return false;}
   return services_.online_quests.invoke(q,out,e);
  });
 }
 // Caller releases this before its Save authority. Profile callbacks remain
 // guarded by weak/real provider leases, never a Profile→Save→Profile cycle.
};
