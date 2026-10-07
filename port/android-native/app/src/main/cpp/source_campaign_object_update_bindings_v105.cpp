#include "source_campaign_object_update_bindings_v105.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_character_unload_v105.hpp"
#include "source_campaign_noncharacter_virtual_v105.hpp"
#include "source_campaign_ai_queue_v105.hpp"
#include "source_campaign_character_clean_v107.hpp"
#include "source_campaign_items_v88.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "renderer_campaign_conditions_v75.hpp"
#include "model_renderer.hpp"
#include <application_services_owner_v5.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <canonical_character_candidate_v60.hpp>
#include <level_gameplay_update_v66.hpp>
#include <object_enable_condition_v2.hpp>
namespace model_renderer {
namespace {
struct UpdateBindingV105 {
 std::weak_ptr<void> world;SourceObjectUpdateLeavesV105 leaves;
 std::shared_ptr<dh2::world::NativeConditionRuntimeV69> conditions;
 bool scope(SourceCampaignCandidateBorrowV55& actual,std::string& e){
  auto owner=world.lock();if(!owner||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=owner){e="Released or replaced source ObjectManager.Update World";return false;}return true;
 }
 bool condition(const dh2::world::ObjectUpdateActorV102& a,bool disable,bool mark,std::string& e){
  SourceCampaignCandidateBorrowV55 current;if(!scope(current,e))return false;
  struct Call {UpdateBindingV105& owner;SourceCampaignCandidateBorrowV55& scope;const dh2::world::ObjectUpdateActorV102& actor;};Call call{*this,current,a};
  dh2::world::ObjectEnableConditionBorrowV2 f;
  f.enabled8a=a.byte(0x8a);f.minimum_difficulty_ec=a.integer?a.integer(0xec):nullptr;f.disable_f1=a.byte(0xf1);
  f.condition_a8=a.pointer(disable?0xcc:0xa8);f.tested_ac=a.byte(disable?0xd0:0xac);
  dh2::world::ObjectEnableConditionServicesV2 s{&call,
   [](void* raw,bool& character,bool& save,std::uint8_t& ready,std::string& e){auto& c=*static_cast<Call*>(raw);auto pm=c.scope.application->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* info{};
    if(!pm||!pm->get_local_player(0,true,info,e)||!info)return false;character=info->character660!=0;save=false;ready=0;if(!character)return true;
    SourceCampaignCharacterBorrowV62 player;if(!borrow_source_campaign_character_v62(c.scope.actual_world,info->character660,player,e))return false;
    save=bool(player.character->save);if(save)ready=player.character->save->source_quest_sync_ready14_v3();return true;},
   [](void*,bool& present,std::int32_t& difficulty,std::string& e){dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!borrow_current_native_level_v27(level,e))return false;present=bool(level);if(present)difficulty=level.level()->constructor_fields_v3().mode118;return true;},
   [](void* raw,std::uintptr_t id,bool& value,std::string& e){auto& c=*static_cast<Call*>(raw);return c.owner.conditions&&c.owner.conditions->evaluate(id,value,e);},
   [](void* raw,bool enabled,std::string& e){auto& c=*static_cast<Call*>(raw);std::uintptr_t character{};auto& a=c.actor;
    if(!a.object.as_character||!a.object.as_character(a.object.context,character,e))return false;
    if(character)return source_campaign_character_enabled_event_v102(c.scope.actual_world,character,enabled,e);
    if(!c.owner.leaves.noncharacter_virtual){e="Required selected ObjectBase Enabled/Disabled receiver";return false;}
    return c.owner.leaves.noncharacter_virtual(a.object.identity,enabled?0x44:0x48,e);}};
  bool enabled{};return disable?dh2::world::object_test_disable_condition_v103(f,s,mark,enabled,e):dh2::world::object_test_enable_condition_v2(f,s,mark,enabled,e);
 }
};
}
bool compose_source_campaign_object_update_v105(const SourceCampaignCandidateBorrowV55& candidate,SourceObjectUpdateLeavesV105 leaves,dh2::loader::LevelGameplayServicesV66& out,std::string& e){
 if(!candidate.actual_world||!candidate.objects||!candidate.application||!leaves.owner||out.update_objects){e="Required once-bound SAME source manager frame composition";return false;}
 auto state=std::make_shared<UpdateBindingV105>();state->world=candidate.actual_world;state->leaves=std::move(leaves);
 if(!state->leaves.lifecycle.native_storage){const auto manager=std::weak_ptr<dh2::world::CanonicalObjectManagerV1>(candidate.objects);
  state->leaves.lifecycle.native_storage=[manager](auto& addressed,auto& out,std::string& e){auto actual=manager.lock();
   if(!actual||actual.get()!=&addressed){e="Native lifecycle storage belongs to another source manager";return false;}
   return actual->borrow_native_storage_v108(actual,out,e);
  };
 }
 std::shared_ptr<SourceWorldBorrowV61> source;
 if(!borrow_source_campaign_condition_world_v70(candidate,source,e)||source->source_object_lifecycle_v108||!state->leaves.lifecycle.owner){
  if(e.empty())e="Required once-published actual ObjectManager lifecycle authority";return false;
 }
 source->source_object_lifecycle_v108=std::make_shared<dh2::world::CanonicalObjectLifecycleV1>(state->leaves.lifecycle);
 std::weak_ptr<UpdateBindingV105> weak=state;
 if(!state->leaves.character_clean)state->leaves.character_clean=[weak](const auto& actor,std::string& e){
  auto state=weak.lock();auto world=state?state->world.lock():nullptr;
  if(!world){e="Released SAME Character.Clean World";return false;}
  return source_campaign_character_clean_v107(world,actor.identity,e);
 };
 if(!state->leaves.noncharacter_virtual)state->leaves.noncharacter_virtual=[weak](std::uintptr_t id,std::uint32_t selector,std::string& e){auto state=weak.lock();SourceCampaignCandidateBorrowV55 scope;return state&&state->scope(scope,e)&&source_campaign_noncharacter_virtual_v105(scope,id,selector,e);};
 if(!state->leaves.noncharacter_remote54)state->leaves.noncharacter_remote54=[weak](const auto& actor,bool& value,std::string& e){auto state=weak.lock();SourceCampaignCandidateBorrowV55 scope;return state&&state->scope(scope,e)&&source_campaign_noncharacter_remote_v105(scope,actor.object.identity,value,e);};
 if(!state->leaves.room_frame)state->leaves.room_frame=[weak](std::uintptr_t id,std::string& e){auto state=weak.lock();SourceCampaignCandidateBorrowV55 scope;return state&&state->scope(scope,e)&&source_campaign_noncharacter_virtual_v105(scope,id,0x2c,e);};
 dh2::world::ConditionDataInitServicesV3 arena;if(!lend_campaign_condition_bag_v75(candidate,arena,state->conditions,e))return false;
 dh2::world::ObjectUpdateServicesV102 s;s.owner=state;
 std::weak_ptr<dh2::world::CanonicalObjectManagerV1> manager=candidate.objects;
 if(!state->leaves.matching_deferred)state->leaves.matching_deferred=[weak,manager](const auto& actor,bool& deferred,std::string& e){
  auto state=weak.lock();auto actual=manager.lock();SourceCampaignCandidateBorrowV55 scope;
  if(!state||!actual||!state->scope(scope,e))return false;
  const auto pm=scope.application->source_player_manager_v59();
  if(!pm||!pm->network()){e="Required SAME PM/network Matching.Get receiver";return false;}
  std::shared_ptr<dh2::player::MatchingLocalIdentityOwnerV4> matching;std::vector<std::int32_t> members;
  if(!pm->network()->source_matching_members_v107(matching,members,e))return false;
  deferred=actual->source_is_online_deferred_v107(actor.object.identity,members);return true;
 };
 if(!state->leaves.fake_remove)state->leaves.fake_remove=[weak,manager](const auto& actor,std::string& e){
  auto state=weak.lock();auto actual=manager.lock();
  if(!state||!actual||!actor.object.shared_handle){e="Released SAME FakeRemove source receiver";return false;}
  return actual->fake_remove_source_v106(*actor.object.shared_handle,state->leaves.lifecycle,
   [state](const auto& object,std::uint8_t*& byte,std::string& e){SourceCampaignCandidateBorrowV55 scope;dh2::world::ObjectUpdateActorV102 receiver;
    if(!state->scope(scope,e)||!borrow_source_campaign_object_update_actor_v104(scope,object.identity,receiver,e))return false;
    byte=receiver.byte?receiver.byte(0x81):nullptr;return byte!=nullptr;},state->leaves.character_clean,e);
 };
 s.storage=[manager](auto& out,std::string& e){return dh2::world::borrow_object_update_storage_v102(manager.lock(),out,e);};
 s.actor=[state](std::uintptr_t id,auto& out,std::string& e){SourceCampaignCandidateBorrowV55 current;return state->scope(current,e)&&borrow_source_campaign_object_update_actor_v104(current,id,out,e);};
 s.online=[state](bool& online,std::string& e){SourceCampaignCandidateBorrowV55 current;if(!state->scope(current,e))return false;auto source=current.application->get_online_loading_v55();if(!source){e="Required SAME GetOnline receiver";return false;}online=source->byte5()!=0;return true;};
 s.level=[](std::shared_ptr<void>& pin,const std::uint8_t*& byte144,const std::uint8_t*& byte198,std::string& e){dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;if(!current){pin.reset();byte144=byte198=nullptr;return true;}pin=current.level();auto& f=current.level()->constructor_fields_v3();byte144=&f.byte144;byte198=&f.byte198;return true;};
 s.rooms=[state,manager](std::string& e){auto actual=manager.lock();if(!actual){e="Released SAME manager room list";return false;}return actual->source_update_rooms_v104(state->leaves.room_frame,e);};
 s.is_character=[](const auto& a,bool& value,std::string& e){std::uintptr_t id{};if(!a.object.as_character||!a.object.as_character(a.object.context,id,e))return false;value=id!=0;return true;};
 s.character_virtual28=[state](const auto& a,bool& player,std::string& e){SourceCampaignCandidateBorrowV55 current;SourceCampaignCharacterBorrowV62 actor;return state->scope(current,e)&&borrow_source_campaign_character_v62(current.actual_world,a.object.identity,actor,e)&&actor.character->is_player(player,e);};
 s.remotely_updated=[state](const auto& a,bool& value,std::string& e){std::uintptr_t character{};if(!a.object.as_character||!a.object.as_character(a.object.context,character,e))return false;
  // Derived nonCharacter policies (including Trigger.local_only3bc) must
  // come from its actual selected vptr, never a guessed GO_ID ordinal.
  if(!character){if(!state->leaves.noncharacter_remote54){e="Required selected nonCharacter IsRemotelyUpdated virtual54";return false;}return state->leaves.noncharacter_remote54(a,value,e);}
  auto id=a.integer?a.integer(0x110):nullptr;auto byte=a.byte?a.byte(0x118):nullptr;if(!id||(*id==-1&&!byte)){e="Required source IsRemotelyUpdated110/118";return false;}value=*id!=-1||*byte;return true;};
 s.zone_entered=[state](std::uintptr_t id,std::string& e){SourceCampaignCandidateBorrowV55 current;dh2::world::ObjectUpdateActorV102 a;if(!state->scope(current,e)||!borrow_source_campaign_object_update_actor_v104(current,id,a,e))return false;std::uintptr_t character{};if(!a.object.as_character||!a.object.as_character(a.object.context,character,e))return false;if(character)return source_campaign_character_zone_entered_v102(current.actual_world,id,e);if(!state->leaves.noncharacter_virtual){e="Required selected GameObject.ZoneEntered receiver";return false;}return state->leaves.noncharacter_virtual(id,0x38c710,e);};
 s.update_ai=[state](const auto& a,std::string& e){SourceCampaignCandidateBorrowV55 current;return state->scope(current,e)&&source_campaign_character_update_pointers_v105(current,a.object.identity,e);};
 s.unload_script=[state](const auto& a,std::string& e){auto world=state->world.lock();return world&&source_campaign_character_unload_script_v105(world,a.object.identity,false,e);};
 s.update=[state](const auto& a,std::string& e){auto world=state->world.lock();if(!world||!a.object.type_f4){e="Required actual selected virtual2c receiver";return false;}std::uintptr_t character{};if(!a.object.as_character||!a.object.as_character(a.object.context,character,e))return false;
  if(character){if(!state->leaves.character_frame){e="Required whole same Character.Update3abe98";return false;}return state->leaves.character_frame(character,e);}
  if(*a.object.type_f4==3)return source_campaign_item_update_v104(world,a.object.identity,e);
  if(!state->leaves.noncharacter_virtual){e="Required derived nonCharacter.Update virtual2c";return false;}return state->leaves.noncharacter_virtual(a.object.identity,0x2c,e);};
 s.test_enable=[state](const auto& a,bool mark,std::string& e){return state->condition(a,false,mark,e);};s.test_disable=[state](const auto& a,bool mark,std::string& e){return state->condition(a,true,mark,e);};
 s.deferred=[state](const auto& a,bool& value,std::string& e){if(!state->leaves.matching_deferred){e="Required CMatching virtual88 users and SAME manager tree108 IsOnlineDeferred";return false;}return state->leaves.matching_deferred(a,value,e);};
 s.remove=[state,manager](const auto& a,bool fake,std::string& e){if(fake){if(!state->leaves.fake_remove){e="Required whole source FakeRemove349240";return false;}return state->leaves.fake_remove(a,e);}auto m=manager.lock();if(!m||!a.object.shared_handle)return false;return m->remove_source_v1(*a.object.shared_handle,state->leaves.lifecycle,e);};
 s.resolve_handle_false=[state](const auto& a,std::uintptr_t& id,std::string& e){SourceCampaignCandidateBorrowV55 current;return state->scope(current,e)&&source_campaign_resolve_update_handle_v104(current,a,id,e);};
 s.resolve_character=[state](const auto& a,std::uintptr_t& id,std::string& e){SourceCampaignCandidateBorrowV55 current;std::uintptr_t resolved{};if(!state->scope(current,e)||!source_campaign_resolve_update_handle_v104(current,a,resolved,e))return false;if(!resolved){id=0;return true;}dh2::world::ObjectUpdateActorV102 fresh;if(!borrow_source_campaign_object_update_actor_v104(current,resolved,fresh,e))return false;return fresh.object.as_character&&fresh.object.as_character(fresh.object.context,id,e);};
 s.reset_debug_switches=[state](std::string& e){SourceCampaignCandidateBorrowV55 current;return state->scope(current,e)&&source_campaign_object_update_debug_v104(current,e);};
 out.update_objects=[manager,s=std::move(s)](float dt,std::string& e){auto actual=manager.lock();if(!actual){e="Released actual source manager frame";return false;}return dh2::world::canonical_object_update_v102(*actual,dt,s,e);};
 if(!out.inc_ai_queue){const std::weak_ptr<void> world=candidate.actual_world;out.inc_ai_queue=[world](std::string& e){auto current=world.lock();if(!current){e="Released SAME CharAI queue World";return false;}return source_campaign_ai_inc_queue_v105(current,e);};}
 return true;
}
bool source_campaign_object_test_condition_v105(const SourceCampaignCandidateBorrowV55& candidate,std::uintptr_t id,bool disable,bool mark,std::string& e){
 UpdateBindingV105 binding;binding.world=candidate.actual_world;
 dh2::world::ConditionDataInitServicesV3 arena;if(!lend_campaign_condition_bag_v75(candidate,arena,binding.conditions,e))return false;
 binding.leaves.noncharacter_virtual=[&candidate](std::uintptr_t id,std::uint32_t selector,std::string& e){return source_campaign_noncharacter_virtual_v105(candidate,id,selector,e);};
 dh2::world::ObjectUpdateActorV102 actor;return borrow_source_campaign_object_update_actor_v104(candidate,id,actor,e)&&binding.condition(actor,disable,mark,e);
}
}
