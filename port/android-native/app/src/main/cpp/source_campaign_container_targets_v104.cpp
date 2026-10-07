#include "source_campaign_container_targets_v104.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "source_campaign_items_v88.hpp"
#include "source_campaign_object_update_actor_v104.hpp"
#include "source_campaign_events_v75.hpp"
#include "source_campaign_character_interaction_v114.hpp"
#include "source_process_trophies_v100.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <application_services_owner_v5.hpp>
#include <source_assertion_process_v76.hpp>
namespace model_renderer {
bool bind_source_openable_interaction_v116(const std::shared_ptr<void>& world,
 std::function<bool(std::shared_ptr<void>&,dh2::world::CanonicalGameObjectBaseOwnerV1*&,std::string&)> base,
 dh2::world::OpenableContainerInteractionServicesV2& out,std::string& e){
 using BaseLoan=decltype(base);
 struct State {std::weak_ptr<void> world;BaseLoan base;SourceCampaignLevelEventsBorrowV88 level;dh2::android_ui::ProcessTrophyBorrowV100 trophy;};
 if(!world||!base){e="Required SAME Openable constructor/World loan";return false;}
 auto s=std::make_shared<State>();s->world=world;s->base=std::move(base);out={};out.owner=s;
 auto scope=[s](std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e){auto world=s->world.lock();SourceCampaignCandidateBorrowV55 c;if(!world||!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world)return false;return borrow_source_campaign_condition_world_v70(c,w,e);};
 out.current_level=[s](auto& id,auto& e){auto world=s->world.lock();if(!world||!borrow_source_campaign_level_events_v88(world,s->level,e))return false;id=s->level.identity;return true;};
 out.assert_missing_level=[](auto& e){auto assertion=dh2::world::SourceAssertionProcessV76::borrow();return assertion&&assertion->report("OpenableContainer.cpp",249,"level",e);};
 out.local_player_hosting=[scope](bool& value,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(w,e))return false;auto online=w->application->get_online_loading_v55();if(!online)return false;if(online->byte5()){e="Required original online Matching/CNetPlayerInfo IsHost";return false;}value=true;return true;};
 out.room64=[s](auto& value,auto& e){std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* actual{};if(!s->base(pin,actual,e)||!actual)return false;value=actual->room64();return true;};
 out.constant=[scope](const char* group,const char* key,auto& value,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(w,e)||!w->design)return false;auto d=w->design->borrow();auto* lookup=d.design();if(!lookup||!lookup->lookup||lookup->lookup(lookup->context,0,group,key,&value)){e="Required actual Openable quest constant";return false;}return true;};
 out.raise_async=[s](std::uintptr_t level,const dh2::world::OpenableContainerQuestEventV2& q,auto& e){if(!s->level||s->level.identity!=level){e="Lost captured Openable Level event receiver";return false;}auto captured=std::move(s->level);s->level={};std::uint8_t a=q.byte18,b=q.byte19;std::int32_t quantity=q.source_index;auto actor=q.actor;dh2::loader::GameEventQuestBorrowV75 fields;fields.receiver=captured.level;fields.character8=actor;fields.character8_cell=&actor;fields.id18=q.data_id;fields.id18_cell=&q.data_id;fields.pending_network10=&a;fields.from_network11=&b;fields.quantity14=&quantity;dh2::loader::ScopedGameQuestEventV75 event(reinterpret_cast<std::uintptr_t>(&q),&q.objective_type,std::move(fields));return raise_source_campaign_level_event_v88(captured,event.borrow(),e);};
 out.handle_as_character=[s](auto id,auto& value,auto& e){auto world=s->world.lock();return world&&source_campaign_object_as_character_v114(world,id,value,e);};
 out.is_player=[s](auto id,bool& value,auto& e){auto world=s->world.lock();SourceCampaignCharacterBorrowV62 b;return world&&borrow_source_campaign_character_v62(world,id,b,e)&&b.character&&b.character->is_player(value,e);};
 out.is_local_player=[scope](auto id,bool& value,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;return scope(w,e)&&w->player_manager&&w->player_manager->source_is_local_player_v61(id,value,e);};
 out.props_add_int=[s](auto id,std::int32_t index,std::int32_t value,auto& e){auto world=s->world.lock();SourceCampaignCharacterBorrowV62 b;if(!world||!borrow_source_campaign_character_v62(world,id,b,e)||!b.character)return false;return dh2_property_add(&b.character->view,index,static_cast<std::int32_t>(std::uint32_t(value)<<8))==0;};
 out.props_get_int=[s,scope](auto id,std::int32_t index,bool saved,std::int32_t& value,auto& e){auto world=s->world.lock();SourceCampaignCharacterBorrowV62 b;std::shared_ptr<SourceWorldBorrowV61> w;if(saved||!world||!borrow_source_campaign_character_v62(world,id,b,e)||!b.character||!scope(w,e))return false;if(!dh2::android_ui::borrow_source_process_trophies_v100(w->application,s->trophy,e))return false;std::int32_t raw{};if(dh2_property_resolve(&b.character->view,index,&raw))return false;value=raw>>8;return true;};
 out.trophy_name_index=[s](const char* name,auto& index,auto& e){if(!name||!s->trophy.manager){e="Required captured Openable Trophy singleton";return false;}index=s->trophy.manager->catalog().index(name);return true;};
 out.unlock_trophy=[s](auto index,auto& e){if(!s->trophy.manager)return false;if(s->trophy.manager->unlock(index)){e=s->trophy.manager->error();return false;}return true;};e.clear();return true;
}
bool bind_source_container_drop_v104(const std::shared_ptr<void>& world,std::uintptr_t container,
 std::function<bool(std::int32_t,std::uintptr_t,std::int32_t,bool,std::string&)>& actual_leaf,std::string& e){
 if(!world||!container){e="Required SAME constructed Container/World before DropLoot leaf binding";return false;}
 const std::weak_ptr<void> weak=world;
 actual_leaf=[weak,container](std::int32_t table,std::uintptr_t opener,std::int32_t fixed_powers,bool flag,std::string& e){auto current=weak.lock();if(!current){e="Released Container DropLoot World";return false;}return source_campaign_container_drop_v104(current,container,table,opener,fixed_powers,flag,e);};
 e.clear();return true;
}
namespace {
bool bind_suffix(const std::shared_ptr<void>& actual_world,dh2::world::CanonicalContainerTargetServicesV104 services,
 dh2::world::CanonicalClassReceiverV1& out,std::string& e){
 if(!actual_world||!services.provider||!out.init_post||!out.object.identity){e="Required actual canonical Container factory/InitPost before target suffix binding";return false;}
 const std::weak_ptr<void> weak=actual_world;auto before=out.init_post;
 out.init_post=[weak,before,services=std::move(services)](std::string& e){
  if(!before(e))return false;auto world=weak.lock();SourceCampaignCandidateBorrowV55 c;
  if(!world||!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world)return false;
  return enroll_source_campaign_container_target_v104(c,services,e);
 };e.clear();return true;
}
}
class SourceCampaignContainerTargetsV104 final {
 std::weak_ptr<SourceWorldBorrowV61> world_;
 std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> targets_;
 std::map<std::uintptr_t,std::unique_ptr<dh2::world::CanonicalContainerTargetV104>> registrations_;
 bool scope(std::shared_ptr<SourceWorldBorrowV61>& w,SourceCampaignCandidateBorrowV55& c,std::string& e){
  w=world_.lock();if(!w||!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=w->owner||!w->canonical_world||c.objects!=w->canonical_world->manager_lease){if(e.empty())e="Released SAME Container native target scope";return false;}return true;
 }
public:
 SourceCampaignContainerTargetsV104(std::shared_ptr<SourceWorldBorrowV61> w,std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> targets):world_(w),targets_(std::move(targets)){}
 bool enroll(dh2::world::CanonicalContainerTargetServicesV104 services,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!scope(w,c,e)||!services.base||!services.base(pin,base,e)||!pin||!base)return false;
  const auto id=base->identity();if(registrations_.count(id)){e="Container target native suffix already reached";return false;}
  auto item=std::make_unique<dh2::world::CanonicalContainerTargetV104>(*c.objects,*targets_,std::move(services));
  auto* actual=item.get();registrations_.emplace(id,std::move(item));return actual->register_after_init_post(e);
 }
 bool retire(std::uintptr_t id,std::string& e){
  auto i=registrations_.find(id);if(i==registrations_.end()){e.clear();return true;}
  if(!i->second->retire_after_unpublication(e))return false;registrations_.erase(i);e.clear();return true;
 }
 bool release(std::string& e){
  for(auto i=registrations_.begin();i!=registrations_.end();){if(!i->second->retire_after_unpublication(e))return false;i=registrations_.erase(i);}e.clear();return true;
 }
 bool interact(std::uintptr_t id,std::uintptr_t character,bool& handled,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCandidateBorrowV55 c;if(!scope(w,c,e))return false;const auto found=registrations_.find(id);handled=found!=registrations_.end();if(!handled){e.clear();return true;}return found->second->source_interact_v114(character,e);}
};
bool source_campaign_item_container_interact_v114(const std::shared_ptr<void>& world,std::uintptr_t target,std::uintptr_t character,bool& handled,std::string& e){
 handled=false;SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;dh2::world::ObjectUpdateActorV102 actor;
 if(!target||!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e)||!borrow_source_campaign_object_update_actor_v104(c,target,actor,e)||!actor.object.type_f4)return false;
 if(*actor.object.type_f4==3){handled=true;if(!w->prepared_items_v88){e="Actual SAME Item.Interact pool required";return false;}return w->prepared_items_v88->interact(target,character,e);}
 if(w->container_targets_v104)return w->container_targets_v104->interact(target,character,handled,e);
 //Unrecognized selected class is delegated to its proven virtual98 body;
 //this result never stands for a successful GameObject/Character interaction.
 e.clear();return true;
}
bool enroll_source_campaign_container_target_v104(const SourceCampaignCandidateBorrowV55& c,dh2::world::CanonicalContainerTargetServicesV104 s,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow_source_campaign_condition_world_v70(c,w,e))return false;
 if(!w->container_targets_v104){std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> targets;
  if(!borrow_source_campaign_character_targets_v88(c.actual_world,targets,e)||!targets)return false;
  w->container_targets_v104=std::make_shared<SourceCampaignContainerTargetsV104>(w,std::move(targets));}
 return w->container_targets_v104->enroll(std::move(s),e);
}
bool retire_source_campaign_container_target_v104(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e))return false;
 if(w->container_targets_v104)return w->container_targets_v104->retire(id,e);e.clear();return true;
}
bool release_source_campaign_container_targets_v104(const SourceCampaignCandidateBorrowV55& c,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow_source_campaign_condition_world_v70(c,w,e))return false;
 if(w->container_targets_v104&&!w->container_targets_v104->release(e))return false;w->container_targets_v104.reset();e.clear();return true;
}
bool bind_source_openable_target_suffix_v104(const std::shared_ptr<void>& world,std::shared_ptr<void> provider,
 const std::shared_ptr<dh2::world::CanonicalOpenableGraphV21>& actual,dh2::world::CanonicalClassReceiverV1& out,std::string& e){
 if(!actual||out.object.identity!=actual->receiver().base().identity()){e="Openable target suffix requires SAME actual factory graph";return false;}
 std::weak_ptr<dh2::world::CanonicalOpenableGraphV21> weak=actual;dh2::world::CanonicalContainerTargetServicesV104 s;s.provider=std::move(provider);s.selected_interaction_type=0;
 s.base=[weak](auto& pin,auto*& base,auto& e){auto graph=weak.lock();if(!graph){e="Released source Openable receiver";return false;}pin=graph;base=&graph->receiver().base();return true;};
 s.visual=[weak](auto& out,auto& e){auto graph=weak.lock();if(!graph){e="Released actual Openable visual owner";return false;}out=graph->visual();return true;};
 s.state394=[weak](auto*& state,auto& pin,auto& e){auto graph=weak.lock();if(!graph){e="Released actual Openable state394";return false;}pin=graph;state=&graph->receiver().fields().state394;return true;};
 s.interact=[weak](std::uintptr_t character,std::string& e){auto graph=weak.lock();if(!graph){e="Released actual Openable.Interact receiver";return false;}return graph->interact(character,e);};
 return bind_suffix(world,std::move(s),out,e);
}
bool bind_source_destructible_target_suffix_v104(const std::shared_ptr<void>& world,std::shared_ptr<void> provider,
 const std::shared_ptr<dh2::world::CanonicalDestructibleContainerV16>& actual,dh2::world::CanonicalClassReceiverV1& out,std::string& e){
 if(!actual||out.object.identity!=actual->base().identity()){e="Destructible target suffix requires SAME actual factory receiver";return false;}
 std::weak_ptr<dh2::world::CanonicalDestructibleContainerV16> weak=actual;std::weak_ptr<void> containing=world;
 dh2::world::CanonicalContainerTargetServicesV104 s;s.provider=std::move(provider);s.selected_interaction_type=8;
 s.base=[weak](auto& pin,auto*& base,auto& e){auto r=weak.lock();if(!r){e="Released source Destructible receiver";return false;}pin=r;base=&r->base();return true;};
 s.visual=[weak,containing](auto& out,auto& e){auto r=weak.lock();auto world=containing.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!r||!world||!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->gameobject_graph_v68)return false;
  auto* field=r->base().pointer(0x2d8);if(!field){e="Unproduced Destructible visual2d8";return false;}if(!*field){out.reset();return true;}return w->gameobject_graph_v68->borrow_visual(*field,out,e);};
 s.state394=[weak](auto*& state,auto& pin,auto& e){auto r=weak.lock();if(!r){e="Released actual Destructible state394";return false;}pin=r;state=r->source_state394_v104();return true;};
 s.interact=[weak](std::uintptr_t character,std::string& e){auto actual=weak.lock();if(!actual){e="Released actual Destructible.Interact receiver";return false;}return actual->interact(character,e);};
 return bind_suffix(world,std::move(s),out,e);
}
}
