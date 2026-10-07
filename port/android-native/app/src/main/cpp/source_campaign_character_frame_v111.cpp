#include "source_campaign_character_frame_v111.hpp"
#include "source_campaign_character_gameobject_v111.hpp"
#include "source_campaign_character_interest_v111.hpp"
#include "source_campaign_character_start_v108.hpp"
#include "source_campaign_character_zoning_v108.hpp"
#include "source_campaign_object_update_bindings_v105.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_light_environment_v113.hpp"
#include "source_campaign_class_release_v106.hpp"
#include "source_campaign_object_update_actor_v104.hpp"
#include "source_campaign_fx_v77.hpp"
#include "source_campaign_anchor_v75.hpp"
#include "source_campaign_character_clean_v107.hpp"
#include "source_campaign_combat_v115.hpp"
#include <module_room_zone_connection_v91.hpp>
#include <character_ai_groups_v87.hpp>
#include <canonical_point3d_globals_v1.hpp>
#include <design_settings.hpp>
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_services_owner_v5.hpp>
#include <script_manager_owner_v52.hpp>
#include <navigation_motion.hpp>
#include <floors.hpp>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <set>
namespace model_renderer {namespace {
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
bool spot(Record& r,const SourceCampaignCandidateBorrowV55& scope,std::string& e){
 auto& f=*r.actor->source_frame_fields_v106();auto fx=r.target_fx_manager_v70;
 if(!f.spot_fx14cc&&!f.spot_enabled14c8)return true; //whole3a47b0 NULL/byte0 return
 if(!fx||!r.target_fx_pin_v70){e="Required SAME VisualFXManager for actual SpotTarget";return false;}
 if(!f.spot_fx14cc){if(!fx->grab_marker_v28(f.spot_set14ca,0,f.spot_fx14cc,e))return false;}
 if(!f.spot_fx14cc)return true;
 if(!f.spot_enabled14c8){if(!fx->drop(f.spot_fx14cc,e))return false;if(!f.spot_fx14cc)return true;}
 dh2::navigation::PositionResult result{};
 if(dh2_nav_validate_position(&result,&scope.floors->collision_world,nullptr,f.spot14b0,nullptr)){e="Actual SpotTarget PFWorld ValidatePosition failed";return false;}
 if(!result.valid){std::copy_n(f.previous_spot14bc,3,f.spot14b0);return true;}
 std::copy_n(f.spot14b0,3,f.previous_spot14bc);
 return fx->marker_rotation_v70(f.spot_fx14cc,f.spot14b0,e)&&fx->marker_sync_v83(f.spot_fx14cc,false,e);
}
bool suffix(Record& r,const SourceCampaignCandidateBorrowV55& scope,std::string& e){
 auto a=r.actor;auto fields=a->source_frame_fields_v106();if(!fields){e="Required source Character frame suffix fields";return false;}
 if(fields->state_fx_delay14fc>0.f)fields->state_fx_delay14fc=fields->state_fx_delay14fc-static_cast<float>(scope.application->source_loading_v55().dt8c);
 bool player;if(!r.is_player(player,e))return false;
 if(player){if(!source_campaign_character_interest_v111(scope.actual_world,a->object->identity,e)||!spot(r,scope,e))return false;}
 else{int aiId;auto ai=r.design.ai();if(!ai||dh2_character_target_ai_id(&aiId,r.view.resolved,ai->rows.size()))return false;
  const auto row=dh2::data::ai_props(*ai,aiId);if(!row)return false;
  if(row->type==7){
   const auto enabled=a->source_bool_field(0x8a);if(!enabled){e="Required actual Merchant enabled8a";return false;}
   if(*enabled){const auto talk=a->source_bool_field(0x2fa);if(!talk){e="Required actual Merchant TalkToNPC2fa";return false;}
    auto& self=a->source_self_fx1484();
    if((!*talk&&!self)||(*talk&&self)){
     if(!borrow_source_campaign_fx_v77(scope.actual_world,r.target_fx_pin_v70,r.target_fx_manager_v70,e)||!r.target_fx_manager_v70)return false;
     if(*talk){if(!r.target_fx_manager_v70->drop(self,e))return false;}
     else{if(!r.target_fx_manager_v70->grab_marker_v28(139,a->object->identity,self,e))return false;
      if(self&&!r.target_fx_manager_v70->marker_loop_v76(self,true,e))return false;}
    }
   }
  }
  if(a->kill_fields_v42().suppress_quest14e4){
   //3ac900 checks GetOnline5 first. Offline takes the ordinary suffix;
   //it never reads14e5 or changes the remote-update118 byte.
   auto online=scope.application->get_online_loading_v55();if(!online){e="Required actual GetOnline5 for Character suffix";return false;}
   if(online->byte5()){e="Required online Character14e5/remote118 suffix";return false;}
  }}
 if(!(scope.application->source_loading_v55().frame74&7u)){
  //The normal source kind0 branch requires no global effect IDs: identical
  //kind returns before Drop/Grab. Positive conditions require original cache.
  if(!r.life){e="Required actual Character IsDead";return false;}
  if(!r.life->dead){int state;if(dh2_character_native_fsm_get_integer(&state,&a->machine->native_fsm(),0)!=1)return false;
   const auto flags=a->machine->state().flags;
   //SM_IsStunned(true)/SM_IsScared(true) read SAME FSM2c (Character528),
   //whose logical source backing is attack_gate. This differs from testing ID9/8.
   const auto mask=a->machine->state().attack_gate;
   const bool stun=(mask&2u)&&(flags&0x800u);
   const bool scare=(mask&4u)&&(flags&0x400u);
   const float walk=static_cast<float>(r.view.resolved[46])/256.f;
   const int desired=fields->state_fx_kind1490>4?0:stun?4:fields->state_fx_kind1490>3?0:scare?3:fields->state_fx_kind1490>1?0:walk<1.f?1:0;
   std::int32_t set=-1;int selected=desired;
   if(selected){
    //GOT32c8 is Arrays.DesignSettingsTable.members (9a6498), not an
    //independent cached ID owner. V176 strips only the wire vtable word.
    std::shared_ptr<SourceWorldBorrowV61> source;
    if(!borrow_source_campaign_condition_world_v70(scope,source,e)||!source->settings)return false;
    auto settings=source->settings->borrow();const unsigned field=selected==4?31:selected==3?28:30;
    auto word=settings.word(0,field);if(!word){e="Required actual DesignSettings StateFX member";return false;}
    std::memcpy(&set,word,4);if(set==-1)selected=0;
   }
   if(selected!=fields->state_fx_kind1490){fields->state_fx_kind1490=selected;
    std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* effects{};
    if(!borrow_source_campaign_fx_v77(scope.actual_world,pin,effects,e)||!effects)return false;
    r.target_fx_pin_v70=std::move(pin);r.target_fx_manager_v70=effects;
    auto& statefx=a->source_state_fx148c();
    if(set==-1){if(!effects->drop(statefx,e))return false;}
    else{
     if(statefx&&!effects->drop(statefx,e))return false;
     if(!effects->grab_marker_v28(set,0,statefx,e))return false;
     if(statefx){
      if(!effects->marker_anchor_v28(statefx,a->object->identity,true,e)||
         !effects->marker_rotation_v70(statefx,dh2::world::canonical_vec3_origin_v1().data(),e)||
         !effects->marker_sync_v83(statefx,false,e)||!effects->marker_visible_v28(statefx,true,e)||
         !effects->marker_loop_v76(statefx,true,e))return false;
     }
    }
   }
  }
 }
 bool local;if(!r.services.is_local_player||!r.services.is_local_player(a->object->identity,local,e))return false;
 if(!r.life->dead&&local&&r.target_marker_v70){if(!r.target_marker_v70->update(a->object->target.last_target,a->source_ooi14a4,e))return false;}
 else if(r.target_marker_v70){if(!r.target_marker_v70->update(0,0,e))return false;}
 auto script=scope.application->source_script_manager_v52();if(!script){e="Required actual ScriptManager.IsCutSceneRunning";return false;}
 bool cutscene{};const auto count=script->contexts().size();for(std::size_t i=0;i<count;++i){bool running;if(!script->is_script_running_v96(i,running,e))return false;if(running){cutscene=true;break;}}
 if(cutscene)return true;
 std::shared_ptr<SourceWorldBorrowV61> source;
 if(fields->displayed_gold1500!=-1||(fields->displayed_damage1504!=-1&&local)){
  if(!borrow_source_campaign_condition_world_v70(scope,source,e)||!source->character_reward_text_owner_v114||!source->character_reward_text_v114){e="Required actual HUD buffered Character Gold/XP receiver";return false;}}
 if(fields->displayed_gold1500!=-1){const auto captured=fields->displayed_gold1500;
  if(!source->character_reward_text_v114(a->object->identity,false,captured,e))return false;
  fields->displayed_gold1500=-1; //3ac174/178 AFTER actual Gold delivery.
 }
 if(fields->displayed_damage1504!=-1&&local){const auto raw=fields->displayed_damage1504;
  //Original ASR #8, including negative source words; no FP rounding.
  const auto value=raw>=0?raw/256:-1-static_cast<std::int32_t>((~static_cast<std::uint32_t>(raw))>>8);
  if(!source->character_reward_text_v114(a->object->identity,true,value,e))return false;
  fields->displayed_damage1504=-1; //3ac1ec/1f0 AFTER actual XP delivery.
 }
 return true;
}
}
bool bind_source_campaign_character_reward_text_v114(const std::shared_ptr<void>& world,std::shared_ptr<void> owner,SourceCharacterRewardTextCallbackV114 callback,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> source;
 if(!owner||!callback||!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!borrow_source_campaign_condition_world_v70(candidate,source,e)){if(e.empty())e="Required SAME actual Character reward presentation enrollment";return false;}
 if(source->character_reward_text_owner_v114||source->character_reward_text_v114){e="Repeated actual Character reward presentation enrollment";return false;}
 source->character_reward_text_owner_v114=std::move(owner);
 source->character_reward_text_v114=[callback=std::move(callback)](auto id,bool xp,auto value,auto& e){return callback(id,xp?SourceCharacterRewardTextV114::xp:SourceCharacterRewardTextV114::gold,value,e);};
 return true;
}
bool source_campaign_character_frame_v111(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCandidateBorrowV55 scope;SourceCampaignCharacterBorrowV62 loan;
 if(!borrow_source_campaign_candidate_v55(scope,e)||scope.actual_world!=world||!borrow_source_campaign_character_v62(world,id,loan,e))return false;
 auto r=loan.character;bool accepted;if(!source_campaign_character_start_v108(world,id,accepted,e))return false;if(!accepted)return true;
 auto a=r->actor;auto fields=a?a->source_frame_fields_v106():nullptr;if(!fields||!a->machine){e="Required retained Character timer/FSM frame";return false;}
 const auto dt=scope.application->source_loading_v55().dt8c;
 if(fields->timer14e0){if(!r->timer_util_v107||reinterpret_cast<std::uintptr_t>(r->timer_util_v107.get())!=fields->timer14e0){e="Required SAME optional Character.TimerUtil14e0";return false;}
  dh2::character::source_character_timer_util_update_v106(*r->timer_util_v107,dt);}
 auto script=scope.application->source_script_manager_v52();if(!script){e="Required source CharTimers ScriptManager30";return false;}
 if(!source_campaign_character_timers_update_v102(world,id,dt,script->fields().byte30,e)||
    !source_campaign_character_ai_update_v108(world,id,e)||!source_campaign_character_fsm_update_v101(world,id,e)||
    !source_campaign_character_animator_update_v102(world,id,e)||!source_campaign_character_gameobject_v111(world,id,e))return false;
 return suffix(*r,scope,e);
}
bool source_campaign_character_force_position_v111(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){SourceCampaignCharacterBorrowV62 actual;
 if(!borrow_source_campaign_character_v62(world,id,actual,e)||!actual.character->actor)return false;
 const auto slot=actual.character->actor->source_visual();if(!slot)return true;
 auto visual=actual.character->visual?actual.character->visual->visual():nullptr;if(!visual||reinterpret_cast<std::uintptr_t>(visual.get())!=slot){e="Required SAME positive Character VisualObject";return false;}
 visual->source_force_update_position_v96();return true;
}
bool source_campaign_character_target_position_v114(const std::shared_ptr<void>& world,std::uintptr_t id,std::array<float,3>& out,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,e)||!actual.character->actor)return false;
 dh2::world::GameObjectInitializationFieldsV62 fields;if(!actual.character->actor->inherited_initialization_fields_v62(actual.character,fields,e))return false;
 const auto* node=fields.pointer(0x180);const auto* enabled=fields.byte(0x80);if(!node||!enabled){e="Required actual GetTargetPosition180/80 fields";return false;}
 const auto* position=fields.vector3(*node&&*enabled?0x184:0x160);if(!position){e="Required SAME GetTargetPosition cache184/position160";return false;}
 std::copy_n(position,3,out.begin());e.clear();return true;
}
bool source_campaign_character_master_v111(const std::shared_ptr<void>& world,std::uintptr_t id,std::uintptr_t& out,std::string& e){SourceCampaignCharacterBorrowV62 actual;
 if(!borrow_source_campaign_character_v62(world,id,actual,e))return false;auto fields=actual.character->actor->source_ai_pointers_v105();if(!fields){e="Required actual CharAI50 C1";return false;}out=fields->master50;return true;}
bool source_campaign_character_set_master_v111(const std::shared_ptr<void>& world,std::uintptr_t id,std::uintptr_t master,std::string& e){SourceCampaignCharacterBorrowV62 actual;
 if(!borrow_source_campaign_character_v62(world,id,actual,e))return false;auto r=actual.character;auto fields=r->actor->source_ai_pointers_v105();if(!fields){e="Required actual CharAI50 C1";return false;}
 fields->master50=master;if(!master)return true; //store precedes all source positive queries
 auto ai=r->design.ai();int aiId;if(!ai||dh2_character_target_ai_id(&aiId,r->view.resolved,ai->rows.size()))return false;
 auto row=dh2::data::ai_props(*ai,aiId);auto targets=r->services.world_targets;if(!row||!targets)return false;
 auto query=targets->targets().search_services();dh2::target_search::Response16 dead{};const dh2::target_search::Request24 request{dh2::target_search::is_dead,0,fields->master50,0};
 if(query.invoke(query.context,&request,&dead))return false;fields->master_alive54=dead.word^1u;
 dh2::character::skills::WorldTargetActorBorrowV1 own,other;
 if(targets->actor(fields->master50,&other)||targets->actor(id,&own)||!own.target_node||!other.target_node)return false;
 const auto p=dh2::character::skills::dh2_world_target_position_v1(own.position,own.cached_target_position,*own.target_node,own.target_enabled?*own.target_enabled:0);
 const auto q=dh2::character::skills::dh2_world_target_position_v1(other.position,other.cached_target_position,*other.target_node,other.target_enabled?*other.target_enabled:0);
 if(!p||!q)return false;fields->master_sight55=0;
 volatile float x=q[0]-p[0],y=q[1]-p[1],z=q[2]-p[2],radius=row->view_radius*row->view_radius;
 volatile float xx=x*x,yy=y*y,zz=z*z,sum=xx+yy,total=sum+zz;
 if(radius>total)fields->master_sight55=1;return true;
}
namespace {
struct CharacterFrameAuthorityV111 {
 std::weak_ptr<void> world;
 std::map<std::uintptr_t,dh2::world::CanonicalObjectBorrowV1> orphans;
 std::set<std::uintptr_t> closed;
 bool scope(SourceCampaignCandidateBorrowV55& out,std::string& e){auto actual=world.lock();
  if(!actual||!borrow_source_campaign_candidate_v55(out,e)||out.actual_world!=actual){e="Retired native ObjectManager frame/lifecycle provider";return false;}return true;}
};
}
bool compose_source_campaign_character_frame_leaves_v111(const SourceCampaignCandidateBorrowV55& candidate,SourceObjectUpdateLeavesV105& out,std::string& e){
 if(!candidate.actual_world||!candidate.objects||!candidate.application||out.owner){e="Required once-produced actual Character frame authority";return false;}
 auto state=std::make_shared<CharacterFrameAuthorityV111>();state->world=candidate.actual_world;out.owner=state;out.lifecycle.owner=state;
 out.character_frame=[state](std::uintptr_t id,std::string& e){auto world=state->world.lock();if(!world){e="Retired Character frame World";return false;}return source_campaign_character_frame_v111(world,id,e);};
 out.lifecycle.require_quiescent=[state](auto& manager,std::string& e){SourceCampaignCandidateBorrowV55 scope;
  if(!state->scope(scope,e)||scope.objects.get()!=&manager||!scope.physical_world){e="Required SAME manager/physical lifecycle scope";return false;}
  if(!scope.physical_world->cleanup_delivery_idle_v106()){e="Native manager removal overlaps actual physical callback delivery";return false;}
  return require_source_campaign_class_delivery_v106(scope.actual_world,e);};
 out.lifecycle.game_object=[state](const auto& object,bool& game,auto& out,std::string& e){SourceCampaignCandidateBorrowV55 scope;
  if(!state->scope(scope,e)||!object.as_character)return false;std::uintptr_t character{};if(!object.as_character(object.context,character,e))return false;
  if(character){SourceCampaignCharacterBorrowV62 r;if(!borrow_source_campaign_character_v62(scope.actual_world,character,r,e))return false;
   dh2::world::GameObjectInitializationFieldsV62 fields;if(!r.character->actor->inherited_initialization_fields_v62(r.character,fields,e))return false;
   out={r.character,character,fields.pointer(0x2f4),fields.byte(0x2f8),fields.byte(0x2fc)};
  }else{std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
   if(!borrow_source_campaign_object_base_v77(scope,object.identity,pin,base,e))return false;
   out={pin,object.identity,base->pointer(0x2f4),base->byte(0x2f8),base->byte(0x2fc)};
  }
  if(!out.owner||!out.room_zone2f4||!out.no_room2f8||!out.orphan2fc){e="Required actual selected GameObject lifecycle fields";return false;}
  game=true;return true;};
 out.lifecycle.room_remove=[state](const auto& object,std::uintptr_t room,std::string& e){SourceCampaignCandidateBorrowV55 scope;std::shared_ptr<SourceWorldBorrowV61> source;
  if(!state->scope(scope,e)||!borrow_source_campaign_condition_world_v70(scope,source,e)||!source->module_room_zones_v91)return false;
  std::shared_ptr<dh2::world::CanonicalRoomZoneV3> receiver;
  if(!source->module_room_zones_v91->borrow_room(room,receiver,e)||!receiver){e="Required actual Room.DelObject receiver";return false;}
  receiver->source_remove_object_v104(object.identity);return true;};
 out.lifecycle.flush_target_list=[state](const auto& object,std::string& e){SourceCampaignCandidateBorrowV55 scope;SourceCampaignCharacterBorrowV62 loan;
  if(!state->scope(scope,e)||!borrow_source_campaign_character_v62(scope.actual_world,object.identity,loan,e))return false;
  auto target=loan.character->actor->source_target_list304_v111();if(!target){e="Required actual embedded Character TargetList304 constructor";return false;}return target->flush_backup_results_v111(e);};
 out.lifecycle.object_delete=[state](const auto& object,std::string& e){SourceCampaignCandidateBorrowV55 scope;SourceCampaignCharacterBorrowV62 loan;
  if(!state->scope(scope,e)||!borrow_source_campaign_character_v62(scope.actual_world,object.identity,loan,e))return false;loan.character->actor->source_delete_v111();return true;};
 out.lifecycle.ai_update_pointers=[state](const auto& object,std::string& e){SourceCampaignCandidateBorrowV55 scope;return state->scope(scope,e)&&source_campaign_character_update_pointers_v105(scope,object.identity,e);};
 out.lifecycle.ai_remove_from_group=[state](const auto& object,std::string& e){SourceCampaignCandidateBorrowV55 scope;SourceCampaignCharacterBorrowV62 loan;
  return state->scope(scope,e)&&borrow_source_campaign_character_v62(scope.actual_world,object.identity,loan,e)&&dh2::character::source_character_remove_from_group_v87(*loan.character,e);};
 out.lifecycle.online_byte5=[state](bool& out,std::string& e){SourceCampaignCandidateBorrowV55 scope;if(!state->scope(scope,e))return false;
  auto online=scope.application->get_online_loading_v55();if(!online){e="Required actual lifecycle GetOnline5";return false;}out=online->byte5()!=0;return true;};
 out.lifecycle.character_virtual28=[state](const auto& object,bool& out,std::string& e){SourceCampaignCandidateBorrowV55 scope;SourceCampaignCharacterBorrowV62 loan;
  return state->scope(scope,e)&&borrow_source_campaign_character_v62(scope.actual_world,object.identity,loan,e)&&loan.character->is_player(out,e);};
 out.lifecycle.network_id108=[state](const auto& object,std::uint16_t& out,std::string& e){SourceCampaignCandidateBorrowV55 scope;dh2::world::ObjectUpdateActorV102 actor;
  if(!state->scope(scope,e)||!borrow_source_campaign_object_update_actor_v104(scope,object.identity,actor,e)||!actor.integer)return false;
  auto id=actor.integer(0x108);if(!id){e="Required same ObjectBase network halfword108";return false;}out=static_cast<std::uint16_t>(*id);return true;};
 out.lifecycle.class_d0=[state](const auto& object,std::string& e){SourceCampaignCandidateBorrowV55 scope;SourceCampaignCharacterBorrowV62 loan;
  if(!state->scope(scope,e)||!borrow_source_campaign_character_v62(scope.actual_world,object.identity,loan,e))return false;
  if(state->closed.count(object.identity)){e="Repeated actual Character D0 delivery";return false;}
  auto r=loan.character;if(!source_campaign_character_clean_v107(scope.actual_world,object.identity,e))return false;
  auto attached=r->actor->position_fields_v7().attached2e0;
  if(attached){if(!source_campaign_anchor_delete_v75(scope.actual_world,attached,e))return false;r->actor->position_fields_v7().attached2e0=0;}
  auto target=r->actor->source_target_list304_v111();if(!target||!target->destroy(e))return false;
  if(!r->close_after_unpublication(e))return false;
  if(!finish_source_campaign_batch_object_retirement_v113(scope.actual_world,object.identity,e))return false;
  state->closed.insert(object.identity);return true;};
 out.lifecycle.orphan_admit=[state](const auto& object,std::string& e){
  if(!object.lease||!state->orphans.emplace(object.identity,object).second){e="Invalid or duplicate actual Character orphan admission";return false;}return true;};
 out.lifecycle.native_receiver=[state](std::uintptr_t id,auto& out,std::string& e){auto found=state->orphans.find(id);if(found==state->orphans.end()){e="Unknown actual retained Character orphan";return false;}out=found->second;return true;};
 out.lifecycle.retire_after_unpublication=[state](auto& manager,std::string& e){SourceCampaignCandidateBorrowV55 scope;
  if(!state->scope(scope,e)||scope.objects.get()!=&manager)return false;
  for(auto id:state->closed){const dh2::world::CanonicalObjectBorrowV1* actor{};std::int32_t key{};bool found=manager.source_ordered_begin_v38(key,actor);
   while(found){if(actor&&actor->identity==id){e="Source Character map still published after D0";return false;}found=manager.source_ordered_next_v38(key,key,actor);}
   std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> targets;if(!borrow_source_campaign_character_targets_v88(scope.actual_world,targets,e))return false;
   //The native directory loan is removed only after source manager publication
   //is gone, so source GetHandle reloads during Remove see original storage.
   if(!source_campaign_forget_combat_actor_v115(scope.actual_world,id,e))return false;
   if(targets->remove(id)){e="Missing actual Character target registration at retirement";return false;}
   if(!retire_source_campaign_closed_character_v111(scope.actual_world,id,e))return false;state->orphans.erase(id);}
  state->closed.clear();return true;};
 std::shared_ptr<SourceWorldBorrowV61> source;if(!borrow_source_campaign_condition_world_v70(candidate,source,e))return false;
 if(!bind_source_campaign_journal_lifecycle_v104(source,out.lifecycle,[weak_source=std::weak_ptr<SourceWorldBorrowV61>(source)](auto& out,std::string& e){auto w=weak_source.lock();return w&&borrow_source_campaign_light_quiescence_v113(w->owner,out,e);},e))return false;
 return true;
}
}
