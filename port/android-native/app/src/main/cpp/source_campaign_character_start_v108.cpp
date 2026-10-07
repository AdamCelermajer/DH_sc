#include "source_campaign_character_start_v108.hpp"
#include "source_campaign_character_eligibility_v106.hpp"
#include "source_campaign_character_controller_v107.hpp"
#include "source_campaign_character_unload_v105.hpp"
#include "source_campaign_death_rewards_v84.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_services_owner_v5.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <character_loading_queue_v105.hpp>
#include <character_update_queued.hpp>
#include <climits>
#include <cstring>
#include <map>
namespace model_renderer {namespace {
using namespace dh2::character;
struct StartV108 {
 SourceCampaignCandidateBorrowV55 candidate;SourceCampaignCharacterBorrowV62 actor;
 std::map<dh2::player::PlayerInfoFieldsV1*,UpdateStartupPlayer24> player_loans;
 std::string debug_string;
 bool refresh(UpdateStartupOwner64& out,std::string& e){
  auto& r=*actor.character;if(!r.actor||!r.actor->object||!r.properties||!r.actor->controller||!r.design.ai())return false;
  ScriptSessionView view{};const bool active=r.player_script_owner_v62?r.player_script_owner_v62->session().owner().active(view):r.actor->session&&r.actor->session->owner().active(view);
  out.active_ai=active?view.identity:0;out.controller=r.actor->controller->identity();out.resolved_hp=r.properties->resolved[36];
  std::int32_t ai{};if(dh2_character_target_ai_id(&ai,r.properties->resolved.data(),r.design.ai()->rows.size()))return false;
  const auto row=dh2::data::ai_props(*r.design.ai(),ai);if(!row){e="Required actual queued GetCharAIId row";return false;}out.delayed_load=static_cast<std::uint8_t>(row->delayed_load);
  r.deferred_receiver_v108={r.actor->object->identity,out.controller};return true;
 }
 static int invoke(void* raw,UpdateStartupOwner64* owner,const UpdateStartupRequest40* q,UpdateStartupResponse16* out){
  auto& t=*static_cast<StartV108*>(raw);auto& r=*t.actor.character;auto& e=r.error;if(!owner||!q||!out)return -1;
  bool player{};unsigned value{};const auto pm=t.candidate.application->source_player_manager_v59();
  switch(q->operation){
   case update_debug_load:if(!r.services.debug||!r.services.debug_files||dh2_character_debug_load(r.services.debug,r.services.debug_files)!=1)return -1;break;
   case update_debug_construct:if(!q->name||!t.debug_string.empty())return -1;t.debug_string=q->name;break;
   case update_debug_destroy:if(!q->name||t.debug_string!=q->name)return -1;t.debug_string.clear();break;
   case update_debug_query:if(!q->name||t.debug_string!=q->name||dh2_character_debug_get(&value,r.services.debug,t.debug_string.c_str(),r.services.debug_files)!=1)return -1;out->word=value;break;
   case update_get_player:{dh2::player::PlayerInfoFieldsV1* info{};if(!pm||!pm->manager()||!pm->manager()->get_player(static_cast<int>(q->argument),q->argument2!=0,info,e)||!info)return -1;
    //Only Character660 is read by reached KillPlayerOne. Unported bot
    //selector is rejected at its separate operation before unused fields.
    auto& loan=t.player_loans[info];loan={reinterpret_cast<std::uintptr_t>(info),info->character660,INT32_MIN,255,{0,0,0}};out->identity=reinterpret_cast<std::uintptr_t>(&loan);break;}
   case update_set_property:{std::int32_t bits;std::memcpy(&bits,&q->argument2,4);if(dh2_property_set(&r.view,static_cast<int>(q->argument),bits))return -1;break;}
   case update_is_dead:if(!r.life)return -1;out->word=r.life->dead;break;
   case update_cmd_kill:if(!source_campaign_cmd_kill_v84(r.services.world,q->subject,r.actor->object->identity,0,q->argument,nullptr,e))return -1;break;
   case update_is_player:if(!r.is_player(player,e))return -1;out->word=player;break;
   case update_get_state:{std::int32_t state;if(!r.actor->machine||dh2_character_native_fsm_get_integer(&state,&r.actor->machine->native_fsm(),0)!=1)return -1;out->word=static_cast<unsigned>(state);break;}
   case update_get_current_level:{dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e)||!current){if(e.empty())e="Required original nonNULL CurrentLevel row3c in queued update";return -1;}out->word=static_cast<unsigned>(current.level()->constructor_fields_v3().row3c);break;}
   case update_load_and_init:{const auto code=r.source_load_n_init_v106(q->argument,e);if(code<0)return -1;out->word=static_cast<unsigned>(code);break;}
   case update_is_monster:case update_is_miniboss:case update_is_boss:{std::int32_t ai{};auto rows=r.design.ai();if(!rows||dh2_character_target_ai_id(&ai,r.properties->resolved.data(),rows->rows.size()))return -1;auto row=dh2::data::ai_props(*rows,ai);if(!row)return -1;out->word=q->operation==update_is_monster?row->type==4:q->operation==update_is_miniboss?(row->flags>>1)&1u:(row->flags>>2)&1u;break;}
   case update_online:{auto source=t.candidate.application->get_online_loading_v55();if(!source)return -1;out->word=source->byte5();break;}
   case update_real_time:if(!borrow_application_time_v68(out->word,e))return -1;break;
   case update_set_potions:e="Required source positive debug SG_SetPotions/Inventory quantity mutation";return -1;
   case update_player_for_character:e="Required actual positive isABot PlayerInfo bot selector";return -1;
   default:return -1;
  }
  return t.refresh(*owner,e)?0:-1;
 }
 static int deferred(void* raw,CharacterDeferredQueue*,const DeferredQueueRequest32* q){
  auto& t=*static_cast<StartV108*>(raw);auto& e=t.actor.character->error;if(!q)return -1;
  if(q->operation==deferred_queue_unload_ai)return source_campaign_character_unload_ai_v108(t.candidate.actual_world,q->owner,q->final!=0,e)?0:-1;
  SourceCampaignCharacterBorrowV62 current;if(!borrow_source_campaign_character_v62(t.candidate.actual_world,q->owner,current,e)||!current.character->actor->controller||current.character->actor->controller->identity()!=q->controller)return -1;
  return source_campaign_cmd_kill_v84(t.candidate.actual_world,q->controller,q->owner,0,q->final,nullptr,e)?0:-1;
 }
};
}
bool source_campaign_character_start_v108(const std::shared_ptr<void>& world,std::uintptr_t id,bool& accepted,std::string& e){
 StartV108 scope;if(!borrow_source_campaign_candidate_v55(scope.candidate,e)||scope.candidate.actual_world!=world||!borrow_source_campaign_character_v62(world,id,scope.actor,e))return false;
 accepted=false;
 return source_campaign_character_with_eligibility_v108(world,id,[&](CanUpdateOwner40& eligible,const CanUpdateServices24& eligibility,std::string& error){
  UpdateStartupOwner64 owner{};owner.eligibility=&eligible;owner.eligibility_services=&eligibility;owner.application_updates=&scope.candidate.objects->source_updates5c_v102();
  if(!scope.refresh(owner,error))return false;
  UpdateStartupServices24 services{&scope,StartV108::invoke,(1u<<19)-1u,0};
  DeferredQueueServices24 deferred{&scope,StartV108::deferred,3,0};auto queue=character_loading_queue_v105();
  if(!queue){error="Required actual concurrent-AI queue C1 owner";return false;}
  const UpdateQueuedBinding24 binding{queue.get(),&scope.actor.character->deferred_receiver_v108,&deferred};unsigned stage{};
  if(dh2_character_update_queued(&owner,&services,&binding,&stage)){error=scope.actor.character->error.empty()?"Actual Character.Update queued prefix failed":scope.actor.character->error;return false;}
  if(stage==update_ineligible)return true;
  //CanUpdate's captured Root200 store must be visible before controller.
  //The outer scoped loan commits on return, so controller remains outside it.
  accepted=true;return true;
 },e)&&(!accepted||(source_campaign_character_controller_update_v107(world,id,e)&&[&]{
  UpdateStartupOwner64 owner{};CanUpdateOwner40 identity{};identity.identity=id;owner.eligibility=&identity;
  if(!scope.refresh(owner,e))return false;UpdateStartupServices24 services{&scope,StartV108::invoke,(1u<<19)-1u,0};unsigned stage{};
  if(dh2_character_update_after_controller(&owner,&services,&stage)){e=scope.actor.character->error.empty()?"Actual post-controller bot prefix failed":scope.actor.character->error;return false;}return stage==update_timers_ready;
 }()));
}
}
