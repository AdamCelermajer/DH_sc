#include "character_menu_campaign_save_v50.hpp"
#include <cstring>
namespace dh2::character {
CharacterMenuCampaignSaveV50::CharacterMenuCampaignSaveV50(
 std::shared_ptr<data::PlayerSaveLoadOwnerV1> authority,
 std::shared_ptr<level::CampaignSaveProfileV45> profile,CharacterMenuCampaignSaveServicesV50 services):
 authority_(std::move(authority)),profile_(std::move(profile)),services_(std::move(services)){}
bool CharacterMenuCampaignSaveV50::coherent(std::string& e)const{
 if(!authority_||!profile_||!profile_->ready()||
    authority_->profile().identity!=reinterpret_cast<std::uintptr_t>(profile_.get())||
    authority_->profile().owner.get()!=profile_.get()){
  e="Required published SAME Character Save+8 campaign profile/lifetime";return false;
 }return true;
}
bool CharacterMenuCampaignSaveV50::bind(std::string& e){
 if(attempted_){e="Character menu campaign writer registration cannot replay";return false;}attempted_=true;
 if(!coherent(e))return false;
 if(!services_.tables||!services_.characters||!services_.skills||!services_.power_names||
    !services_.level_names||!services_.map_names||!services_.current_difficulty||!services_.actor||
    !services_.remaining.owner||!services_.remaining.invoke){
  e="Required actual Character/Skill/Power/LVLS/Map/difficulty/actor/online save providers";return false;
 }
 auto self=shared_from_this();named_=std::make_shared<data::PlayerSaveNamedWriterV1>(authority_,services_.skills);
 metadata_=std::make_shared<level::PlayerSaveMetadataWriterV45>(authority_,
  level::PlayerSaveMetadataServicesV45{services_.tables,services_.characters,services_.current_difficulty});
 for(const char* tag:{"PNAM","PLVL","SKIL","FAES","CFEE"})if(!profile_->register_named_writer(tag,named_,e))return false;
 std::weak_ptr<CharacterMenuCampaignSaveV50> weak=self;
 for(const char* tag:{"PCLS","PDFL","LNAM","LEPT","LUSP","GEAR","PROP","LVLS","QEST","FTVL"}){
  const std::string name=tag;
  if(!profile_->register_writer(tag,self,[weak,name](auto& stream,auto& error){
   auto s=weak.lock();if(!s){error="Expired SAME Character menu campaign writer";return false;}
   return s->write(name.c_str(),stream,error);
  },e))return false;
 }
 ready_=true;e.clear();return true;
}
bool CharacterMenuCampaignSaveV50::write(const char* tag,level::SavegameStreamV2& stream,std::string& e){
 if(!ready_||!coherent(e))return false;
 if(!std::strcmp(tag,"PCLS")||!std::strcmp(tag,"PDFL")||!std::strcmp(tag,"LNAM")||
    !std::strcmp(tag,"LEPT")||!std::strcmp(tag,"LUSP"))return metadata_->write(tag,stream,e);
 auto& save=authority_->save();
 if(!std::strcmp(tag,"LVLS"))return level::player_save_level_states_v45(
  {services_.tables,services_.level_names,services_.map_names,&save.level_states_v45(),&save.map_states_v45()},stream,e);
 if(!std::strcmp(tag,"FTVL"))return level::player_save_fast_travel_v45(&save.fast_travel_v45(),stream,e);
 if(!std::strcmp(tag,"QEST")){
  const auto& a=save.regular_quests_v45();const auto& b=save.volatile_quests_v45();
  return level::player_save_quests_v45(authority_->save_mode(),
   {services_.tables,&a.source_quests_v45(),&a.progress(),services_.quest_save_data},
   {services_.tables,&b.source_quests_v45(),&b.progress(),services_.quest_save_data},stream,e);
 }
 CharacterMenuSaveBorrowV50 actor;
 if(!services_.actor(actor,e))return false;
 if(!actor.receiver||!actor.inventory||!actor.properties||
    actor.inventory->character()!=save.character()||!actor.properties->saved||!actor.properties->resolved){
  e="Required fresh SAME Character/Gear/property source section receiver";return false;
 }
 if(!std::strcmp(tag,"GEAR"))return level::player_save_inventory_writer_v45(*actor.inventory,*services_.power_names,stream,e);
 if(!std::strcmp(tag,"PROP"))return level::player_save_properties_writer_v45(*actor.properties,save.source_property_tail194_v45(),stream,e);
 e="Unowned Character campaign section "+std::string(tag);return false;
}
data::PlayerSaveWriteServicesV1 CharacterMenuCampaignSaveV50::write_services(){
 std::weak_ptr<CharacterMenuCampaignSaveV50> weak=shared_from_this();
 return profile_->write_services([weak](const auto& q,auto& out,auto& e){
  auto s=weak.lock();if(!s||!s->ready_){e="Required retained bound Character menu campaign writer";return false;}
  if(!s->coherent(e)||q.authority!=s->authority_.get()||q.save!=&s->authority_->save()){
   if(e.empty())e="Character menu SG_Save replaced its sole authority";return false;
  }
  return s->services_.remaining.invoke(q,out,e);
 });
}
bool CharacterMenuCampaignSaveV50::load_property_section(const data::PlayerSaveLoadRequestV1& q,
 data::PlayerSaveLoadResponseV1&,std::string& e){
 if(!ready_||!coherent(e))return false;
 if(q.operation!=data::PlayerSaveLoadOpV1::load_section||!q.section||std::strcmp(q.section,"PROP")||
    q.save!=&authority_->save()||q.profile.identity!=reinterpret_cast<std::uintptr_t>(profile_.get())||!q.reader_enabled){
  e="Required exact SAME campaign PROP reader request";return false;
 }
 CharacterMenuSaveBorrowV50 actor;if(!services_.actor(actor,e))return false;
 if(!actor.receiver||!actor.properties||!actor.inventory||actor.inventory->character()!=q.save->character()){
  e="Required SAME live Character PROP reload receiver";return false;
 }
 const auto cache=profile_->cache();const auto bytes=cache.payload("PROP");
 if(!bytes.data)return true; // exact absent named section does not invoke reader
 std::size_t consumed{};return level::player_save_properties_reader_v45(*q.save,*actor.properties,bytes,consumed,e);
}
}
