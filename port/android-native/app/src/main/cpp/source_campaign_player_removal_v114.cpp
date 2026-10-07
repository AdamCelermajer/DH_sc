#include "source_campaign_player_removal_v114.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_conditions_v70.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_anchor_v75.hpp"
#include "source_campaign_death_rewards_v84.hpp"
#include "source_campaign_fx_v77.hpp"
#include "model_renderer.hpp"
#include "native_source_main_menu_v114.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <character_aggro_clear_all_v2.hpp>
#include <source_character_aggro_v84.hpp>
#include <level_fog_source_v103.hpp>
#include <gameplay_camera_application_v23.hpp>
#include <cstdio>
#include <utility>
namespace model_renderer {namespace {
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
std::uint32_t deleted_player_character_counter_v114{}; // original PM static BSS0
bool required(std::string& e,const char* leaf){if(e.empty())e=std::string("Required actual local removal ")+leaf;return false;}
bool visible(Record& r,std::string& e){
 dh2::world::GameObjectInitializationServicesV1 s;
 if(!r.services.inherited||!r.services.inherited(r,s,e)||!s.set_visible)return required(e,"GameObject.SetVisible");
 return s.set_visible(false,e);
}
bool drop(Record& r,const std::shared_ptr<void>& w,std::uintptr_t& field,std::string& e){
 if(!field)return true;
 if(!r.target_fx_manager_v70&&(!borrow_source_campaign_fx_v77(w,r.target_fx_pin_v70,r.target_fx_manager_v70,e)||!r.target_fx_manager_v70))return false;
 return r.target_fx_manager_v70->drop(field,e);
}
struct Disabled {
 unsigned phase{};std::unique_ptr<Disabled> nested;
 bool run(Record& r,const std::shared_ptr<void>& w,std::string& e){
  auto& a=*r.actor;dh2::world::GameObjectInitializationFieldsV62 f;
  if(!a.inherited_initialization_fields_v62(r.shared_from_this(),f,e))return false;
  while(phase<7){switch(phase){
   case 0:{auto enabled=f.byte(0x8a);if(!enabled)return required(e,"source enabled8a");
    if(*enabled){*enabled=0;nested=std::make_unique<Disabled>();}phase=1;break;}
   // ObjectBase.Disabled virtual3c SetEnable(false) reenters Character.Disabled
   // after its byte8a store. The same byte prevents infinite native recursion.
   case 1:if(nested&&!nested->run(r,w,e))return false;phase=2;break;
   case 2:if(!visible(r,e))return false;phase=3;break;
   case 3:a.runtime.object.motion.object_flags&=~8u;phase=4;break; // PFObject+4 = GO1cc
   case 4:if(a.position_fields_v7().physical2dc){
    if(!r.physical_owner_v62||reinterpret_cast<std::uintptr_t>(&r.physical_owner_v62->native())!=a.position_fields_v7().physical2dc)return required(e,"same physical2dc disableFilter");
    if(!r.physical_owner_v62->disable_filter()){e=r.physical_owner_v62->error();return false;}}
    phase=5;break;
   case 5:{auto idle=f.byte(0x373);if(!idle)return required(e,"idle sound373");*idle=1;phase=6;break;}
   case 6:if(!drop(r,w,a.source_self_fx1484(),e))return false;phase=7;break;
  }}return true;
 }
};
struct AggroScope {
 std::shared_ptr<void> world;
 static bool actor(void* p,std::uintptr_t id,dh2::character::AggroClearActorBorrowV2& out,std::string& e){
  auto& s=*static_cast<AggroScope*>(p);SourceCampaignCharacterBorrowV62 b;
  if(!borrow_source_campaign_character_v62(s.world,id,b,e)||!b.character||!b.character->actor||!b.character->actor->object)return false;
  auto& a=*b.character->actor;auto* maps=a.source_aggro_v84();if(!maps)return required(e,"same CharAI maps");
  out={id,maps->outgoing(),maps->incoming(),&a.object->binding};return true;
 }
 static bool deaggro(void* p,std::uintptr_t receiver,std::uintptr_t other,const dh2_script_callback_scope* scope,std::string& e){
  return source_campaign_character_aggro_event_v84(static_cast<AggroScope*>(p)->world,
   dh2::data::aggro_notify_target_cleared,other,receiver,scope,e);
 }
};
bool clear_aggro(const std::shared_ptr<void>& w,std::uintptr_t id,bool incoming,std::string& e){
 AggroScope scope{w};dh2::character::AggroClearActorBorrowV2 actual;
 if(!AggroScope::actor(&scope,id,actual,e))return false;
 dh2::character::AggroClearAllResultV2 result;
 const dh2::character::AggroClearAllServicesV2 s{&scope,AggroScope::actor,AggroScope::deaggro};
 return incoming?dh2::character::character_aggro_clear_all_toward_me_v2(result,actual,false,s,e):
  dh2::character::character_aggro_clear_all_v2(result,actual,s,e);
}
}
struct SourcePlayerCharacterRemovalV114::Impl {
 unsigned phase{};std::uintptr_t subject{};dh2::player::PlayerInfoFieldsV1* info{};
 std::int32_t internal{};bool dummy{},busy{};std::weak_ptr<void> world;
 Disabled disabled;std::string renamed;std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;
 std::uintptr_t next_target{};bool camera_prefix_done{},camera_update_done{},next_init_done{};
 bool run(const std::shared_ptr<void>& w,const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
  const dh2::player::PlayerManagerRequestV1& q,std::string& e){
  if(busy)return required(e,"non-reentrant same removal");busy=true;struct Busy{bool& b;~Busy(){b=false;}} guard{busy};
  auto pm=app?app->source_player_manager_v59():nullptr;
  if(!w||!pm||!pm->belongs_to_application(app)||!pm->manager()||!q.player||!q.character_count6c4)return required(e,"same App/PM request");
  auto* manager=pm->manager();
  if(!phase){subject=q.player->character660;world=w;if(!subject){phase=22;return true;}}
  if(world.lock()!=w)return required(e,"same old World");
  if(phase==22)return true;
  SourceCampaignCharacterBorrowV62 b;if(!borrow_source_campaign_character_v62(w,subject,b,e)||!b.character||!b.character->actor)return false;
  auto r=b.character;auto& a=*r->actor;
  while(phase<22){switch(phase){
   case 0:{auto deleted=a.source_bool_field(0x81);if(!deleted)return required(e,"actual deleted81");if(*deleted){phase=22;return true;}
    if(!manager->get_by_character(subject,true,info,e)||!info)return false;internal=info->internal670;dummy=internal==-1;phase=1;break;}
   case 1:{const auto value=*q.character_count6c4;if(value<=0)return required(e,"original m_numCreatedCharacters assertion");
    *q.character_count6c4=value-1;phase=2;break;}
   case 2:{const float zero[3]{};if(!r->position||!r->position->set_position(zero,true,e))return false;phase=3;break;}
   case 3:if(!disabled.run(*r,w,e))return false;phase=4;break;
   case 4:if(!visible(*r,e))return false;phase=5;break;
   case 5:if(!source_campaign_character_init_camera_v75(w,subject,e))return false;phase=6;break;
   case 6:if(!drop(*r,w,a.source_highlight14a0(),e))return false;a.source_highlight14a0()=0;phase=7;break;
   case 7:a.source_ooi14a4=0;phase=8;break;
   case 8:if(!a.object||dh2_character_ai_set_target(&a.object->target,0,0,&a.object->binding.services))return required(e,"AI_SetTarget(NULL,false)");phase=9;break;
   case 9:if(dh2_character_ai_sync_last_target(&a.object->target))return required(e,"AI_SyncLastTarget");phase=10;break;
   case 10:if(!clear_aggro(w,subject,false,e))return false;phase=11;break;
   case 11:if(!clear_aggro(w,subject,true,e))return false;phase=12;break;
   case 12:a.source_object_base_delete_v62();phase=13;break;
   case 13:{char value[512];const auto n=std::snprintf(value,sizeof(value),"%s_Deleted_%d",a.source_name().c_str(),static_cast<std::int32_t>(deleted_player_character_counter_v114));
    if(n<0||n>=static_cast<int>(sizeof(value)))return required(e,"original sprintf name domain");renamed=value;++deleted_player_character_counter_v114;phase=14;break;}
   case 14:{SourceCampaignCandidateBorrowV55 c;if(!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=w||!c.objects)return false;
    if(!c.objects->source_rename_v114(subject,renamed.c_str(),[&](auto& error){return a.source_set_name_v114(renamed.c_str(),error);},e))return false;phase=15;break;}
   case 15:{dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!borrow_current_native_level_v27(level,e))return false;
    if(dummy){phase=21;break;}if(!level||!info->local66c){phase=18;break;}
    if(*q.character_count6c4>0){dh2::player::PlayerInfoFieldsV1* next{};
     if(!manager->get_local_player(0,true,next,e)||!next)return false;next_target=next->character660;
     if(!next_target){if(!manager->get_player(0,false,next,e)||!next)return false;next_target=next->character660;}
     if(next_target&&level.level()->constructor_fields_v3().field128){
      SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> native;
      if(!borrow_source_campaign_candidate_runtime_v61(c,e)||!borrow_source_campaign_condition_world_v70(c,native,e))return false;
      auto session=native->camera_application?native->camera_application->world():nullptr;
      camera=session&&session->camera?session->camera->level():nullptr;
      if(!camera||reinterpret_cast<std::uintptr_t>(camera.get())!=level.level()->constructor_fields_v3().field128)return required(e,"same camera128");
     }
    }phase=16;break;}
   case 16:if(camera){if(!camera_prefix_done){if(!camera->set_target(next_target,0,e))return false;camera_prefix_done=true;}
    if(!camera_update_done){if(!camera->update(e))return false;camera_update_done=true;}}
    if(next_target&&*q.character_count6c4==1&&!next_init_done){if(!source_campaign_character_init_camera_v75(w,next_target,e))return false;next_init_done=true;}
    camera.reset();phase=17;break;
   case 17:dh2::world::source_level_update_light_set_v103(true,4);phase=18;break;
   case 18:{dh2::player::PlayerInfoFieldsV1* current{};if(!manager->get_by_internal(internal,false,current,e)||current!=info)return required(e,"same removal PlayerInfo");
    auto f=manager->source_frame_fields_v68();if(!f)return false;if(!f->byte6c9)info->character660=0;info->unknown684=0;phase=19;break;}
   case 19:if(!pm->network()||!pm->network()->source_remove_character_local_tail_v114(*info,e))return false;phase=21;break;
   case 21:if(!source_main_menu_refresh_hud_v114(w,e))return false;phase=22;break;
   default:return required(e,"valid retained removal phase");
  }}e.clear();return true;
 }
};
SourcePlayerCharacterRemovalV114::SourcePlayerCharacterRemovalV114():p_(std::make_unique<Impl>()){}
SourcePlayerCharacterRemovalV114::~SourcePlayerCharacterRemovalV114()=default;
bool SourcePlayerCharacterRemovalV114::execute(const std::shared_ptr<void>& w,const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& a,
 const dh2::player::PlayerManagerRequestV1& q,std::string& e){return p_->run(w,a,q,e);}
}
