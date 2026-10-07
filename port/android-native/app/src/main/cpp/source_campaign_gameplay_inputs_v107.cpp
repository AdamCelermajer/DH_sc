#include "source_campaign_gameplay_inputs_v107.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_object_update_bindings_v105.hpp"
#include "source_campaign_spawn_groups_v108.hpp"
#include "source_campaign_quests_v76.hpp"
#include "model_renderer.hpp"
#include <level_gameplay_update_v66.hpp>
namespace model_renderer {namespace {
template<class A,class B>bool same(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){
 return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);
}
template<class A,class B>bool aliases(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){
 return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
struct EarlyGameplayInputsV107:std::enable_shared_from_this<EarlyGameplayInputsV107> {
 std::weak_ptr<SourceWorldBorrowV61> world;
 bool busy{},attempted{};std::string failure;
 bool reject(std::string& e,const char* message){if(failure.empty())failure=e.empty()?message:e;e=failure;return false;}
 bool compose(const SourceCampaignCandidateBorrowV55& requested,dh2::loader::LevelGameplayServicesV66& out,std::string& e){
  if(!failure.empty()){e=failure;return false;}
  if(busy||attempted)return reject(e,"Actual early gameplay services are composed once, without reentry");
  auto source=world.lock();SourceCampaignCandidateBorrowV55 actual;
  if(!source||!source->canonical_world||!borrow_source_campaign_candidate_v55(actual,e)||
     !same(requested.actual_world,source->owner)||!same(requested.actual_world,actual.actual_world)||
     !same(requested.application,actual.application)||!same(requested.level,actual.level)||
     !same(requested.objects,actual.objects)||!same(requested.objects,source->canonical_world->manager_lease))
   return reject(e,"Required SAME published candidate/App/manager before source warm-start");
  if(source->gameplay_services_v66||out.provider||out.update_objects||out.inc_ai_queue)
   return reject(e,"Actual gameplay packet already has an update authority");
  busy=true;struct Guard{bool& busy;~Guard(){busy=false;}} guard{busy};attempted=true;
  SourceObjectUpdateLeavesV105 leaves;
  if(!source->object_update_inputs_v107||!source->object_update_inputs_v107(actual,leaves,e)||!leaves.owner)
   return reject(e,"Required genuine native Character/frame/lifecycle leaves before warm-start32");
  if(aliases(leaves.owner,actual.actual_world)||aliases(leaves.owner,actual.level)||aliases(leaves.owner,actual.objects))
   return reject(e,"Object frame authority must not retain its containing World/Level/manager");
  dh2::loader::LevelGameplayServicesV66 packet;
  packet.provider=shared_from_this();packet.actual_application=actual.application;packet.actual_object_manager=actual.objects;
  if(!compose_source_campaign_object_update_v105(actual,std::move(leaves),packet,e))
   return reject(e,"Actual ObjectManager.Update composition failed");
  //CharAI::HandleGroups3d24f8 is the shipping literal BXLR. It does not
  //iterate cohorts or alter AI state; the real group source owners are separate.
  packet.handle_ai_groups=[](std::string& e){e.clear();return true;};
  dh2::world::SpawnGroupServicesV108 spawn_groups;
  if(!compose_source_campaign_spawn_groups_v108(actual,spawn_groups,e))return reject(e,"Actual process spawn-group transport unavailable");
  packet.update_spawn_groups=[spawn_groups=std::move(spawn_groups)](double dt,std::string& e){return dh2::world::process_spawn_group_manager_v108()->update(dt,spawn_groups,e);};
  const std::weak_ptr<SourceWorldBorrowV61> weak_source=source;
  packet.save_update=[weak_source](auto id,auto force,auto& e){auto w=weak_source.lock();if(!w){e="Released actual Character.SG_Update campaign";return false;}return source_campaign_character_sg_update_v108(w->owner,id,force,e);};
  //The central V66 binder adds the actual App8c physics, Camera128, script,
  //event, FX, Scene and audio services. Save positive producers
  //must still be supplied by their actual native owners, never stubbed here.
  out=std::move(packet);e.clear();return true;
 }
};
}
bool install_source_campaign_gameplay_inputs_v107(const std::shared_ptr<SourceWorldBorrowV61>& world,std::string& e){
 if(!world||!world->owner||!world->application||!world->canonical_world||
    !world->canonical_world->manager_lease||world->gameplay_inputs_v96||world->gameplay_services_v66){
  e="Required once-only early gameplay enrollment on SAME actual World";return false;
 }
 auto provider=std::make_shared<EarlyGameplayInputsV107>();provider->world=world;
 world->gameplay_inputs_v96=[provider](const auto& actual,auto& out,std::string& e){return provider->compose(actual,out,e);};
 e.clear();return true;
}
bool bind_source_campaign_early_gameplay_v107(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> source;
 if(!borrow_source_campaign_condition_world_v70(candidate,source,e)||!source||!source->gameplay_inputs_v96)return false;
 if(source->gameplay_services_v66){const auto& bound=*source->gameplay_services_v66;
  if(!same(bound.actual_application,candidate.application)||!same(bound.actual_object_manager,candidate.objects)||
     !bound.provider||!bound.update_objects||!bound.inc_ai_queue){e="Existing gameplay packet addresses another source authority";return false;}
  e.clear();return true;
 }
 dh2::loader::LevelGameplayServicesV66 packet;
 return source->gameplay_inputs_v96(candidate,packet,e)&&bind_source_campaign_gameplay_services_v66(candidate,std::move(packet),e);
}
}
