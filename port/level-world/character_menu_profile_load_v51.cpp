#include "character_menu_profile_load_v51.hpp"
#include <cstring>
namespace dh2::character {
bool profile_saved_gear_v51(const std::shared_ptr<level::CampaignSaveProfileV45>& profile,
 bool& present,data::Bytes& bytes,std::shared_ptr<const void>& lease,std::string& e){
 present=false;bytes={};lease.reset();
 if(!profile||!profile->ready()){e="Required actual constructed SAME selected profile for GEAR bootstrap";return false;}
 auto snapshot=std::make_shared<data::PlayerProfileIndexV1::Borrow>(profile->cache());
 if(!*snapshot){e.clear();return true;} // genuine source C1 has no cached file
 present=snapshot->section("GEAR")!=nullptr;
 if(present){bytes=snapshot->payload("GEAR");lease=std::move(snapshot);}
 e.clear();return true;
}
CharacterMenuProfileLoadV51::CharacterMenuProfileLoadV51(std::shared_ptr<data::PlayerSavegameV1> save,
 std::shared_ptr<level::CampaignSaveProfileV45> profile,CharacterMenuProfileLoadServicesV51 services):
 save_(std::move(save)),profile_(std::move(profile)),services_(std::move(services)){}
bool CharacterMenuProfileLoadV51::invoke(const data::PlayerSaveLoadRequestV1& q,
 data::PlayerSaveLoadResponseV1& out,std::string& e){
 if(!save_||q.save!=save_.get()||(profile_&&!profile_->ready())||!services_.tables){
  e="Required SAME retained selected Save/profile/tables";return false;
 }
 using O=data::PlayerSaveLoadOpV1;
 auto remaining=[&](){if(!services_.remaining.owner||!services_.remaining.invoke){
  e="Required selected-profile source operation "+std::to_string(unsigned(q.operation))+(q.section?std::string(" section ")+q.section:std::string{});return false;
 }return services_.remaining.invoke(q,out,e);};
 switch(q.operation){
 case O::init_levels:
  if(!services_.level_defaults28||!services_.map_defaults8){e="Required actual Level+28/MapLoc+8 default arrays";return false;}
  return save_->initialize_level_states_v45(*services_.level_defaults28,*services_.map_defaults8,e);
 case O::init_skills:{const std::vector<std::int32_t>* rows{};
  if(!services_.character_skill_list||!services_.character_skill_list(save_->character(),rows,e)||!rows){if(e.empty())e="Required SAME Character.GetSkillsList initialized owner";return false;}
  return save_->initialize_skills(*rows,e);
 }
 case O::init_faeries:save_->initialize_faeries();return true;
 case O::init_quests:
  if(!services_.quests){e="Required SAME actual Quest persistence initialization owner";return false;}
  return services_.quests->initialize(q.argument,e);
 case O::load_section:break;
 default:return remaining();
 }
 if(!profile_||q.profile.identity!=reinterpret_cast<std::uintptr_t>(profile_.get())||!q.section){e="Selected named reader replaced the actual profile+8";return false;}
 const auto cache=profile_->cache();const auto* section=cache.section(q.section);
 if(!section||!q.reader_enabled)return true; // source absent/null-reader branch
 const auto bytes=cache.payload(q.section);std::size_t consumed{};
 if(!std::strcmp(q.section,"PNAM"))return save_->load_name(bytes,consumed,e);
 if(!std::strcmp(q.section,"PLVL"))return save_->load_level(bytes,consumed,e);
 if(!std::strcmp(q.section,"PCLS")){
  if(!services_.characters){e="Required actual CharacterTable class-name reader";return false;}
  return save_->load_class(bytes,services_.characters->names,consumed,e);
 }
 if(!std::strcmp(q.section,"PDFL")){
  return save_->load_difficulty(bytes,this,[](void* p,std::int32_t value,std::string& error){
   auto& s=*static_cast<CharacterMenuProfileLoadV51*>(p);
   if(!s.services_.store_current_difficulty){error="Required actual CurrentDifficulty global store";return false;}
   return s.services_.store_current_difficulty(value,error);
  },consumed,e);
 }
 if(!std::strcmp(q.section,"LNAM"))return save_->load_location(bytes,consumed,e);
 if(!std::strcmp(q.section,"LEPT"))return save_->load_entry_points(bytes,consumed,e);
 if(!std::strcmp(q.section,"LUSP"))return save_->load_spawn_points(bytes,consumed,e);
 if(!std::strcmp(q.section,"SKIL"))return save_->load_skills(bytes,services_.skills,consumed,e)>=0;
 if(!std::strcmp(q.section,"FAES")){bool mismatch{};return save_->load_faeries(bytes,consumed,mismatch,e);}
 if(!std::strcmp(q.section,"CFEE"))return save_->load_current_faery(bytes,consumed,e);
 if(!std::strcmp(q.section,"QEST")){
  if(!services_.quests){e="Required SAME regular/volatile actual Quest reader";return false;}
  return services_.quests->load(bytes,consumed,e);
 }
 if(!std::strcmp(q.section,"FTVL"))return save_->load_fast_travel_v45(bytes,consumed,e);
 if(!std::strcmp(q.section,"LVLS")){
  if(!services_.level_names||!services_.map_names){e="Required actual Level/MapLoc name readers";return false;}
  return save_->load_level_states_v45(bytes,*services_.level_names,*services_.map_names,consumed,e);
 }
 if(!std::strcmp(q.section,"PROP")){
  // Before InitialGrant the SAME inventory/property receiver exists, but its
  // positive campaign writer is not registered yet. V59 supplies the actual
  // PROP reader endpoint there, rather than manufacturing writer readiness.
  if(!services_.campaign)return remaining();
  return services_.campaign->load_property_section(q,out,e);
 }
 return remaining(); // actual Quest/GEAR/unknown registered reader, never skip
}
data::PlayerSaveLoadServicesV1 CharacterMenuProfileLoadV51::load_services(){
 std::weak_ptr<CharacterMenuProfileLoadV51> weak=shared_from_this();return {services_.tables,[weak](const auto& q,auto& out,auto& e){
  auto s=weak.lock();if(!s){e="Expired actual selected profile reader";return false;}return s->invoke(q,out,e);
 }};
}
}
