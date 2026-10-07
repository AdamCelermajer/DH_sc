#include "source_campaign_object_loading_deferred_v106.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "model_renderer.hpp"
#include <tuple>
#include <utility>
namespace model_renderer {namespace {
struct DeferredObjectLoadingV106 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::world::CanonicalObjectManagerV1> manager;
 //This is only the reached service packet, never another SourceLoadingV43
 //owner, scheduler, candidate, manager or preparation graph.
 dh2::loader::SourceLoadingInputsV43 reached;
 bool attempted{},composed{},failed{},binding{};std::string failure;
 bool reject(std::string& e){failed=true;if(failure.empty())failure=e.empty()?"Required actual reached Stage10/17 provider":e;e=failure;return false;}
 bool ensure(std::string& e){
  if(failed){e=failure;return false;}
  auto source=world.lock();auto actual_manager=manager.lock();SourceCampaignCandidateBorrowV55 actual;
  if(!source||!source->owner||!actual_manager||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world.get()!=source->owner.get()||actual.actual_world.owner_before(source->owner)||source->owner.owner_before(actual.actual_world)||actual.objects.get()!=actual_manager.get()||actual.objects.owner_before(actual_manager)||actual_manager.owner_before(actual.objects)){if(e.empty())e="Changed SAME retained Stage10/17 World/manager";return reject(e);}
  if(composed)return true;
  if(binding||attempted){e="Actual Stage10/17 provider composition reentered or failed prefix retained";return reject(e);}
  attempted=true;binding=true;struct Guard{bool& flag;~Guard(){flag=false;}} guard{binding};
  if(!source->source_object_loading_v95){e="Required actual V62 Stage10/17 provider at reached loading operation";return reject(e);}
  reached.manager=actual_manager;
  if(!source->source_object_loading_v95(actual,reached,e)){reached.manager.reset();return reject(e);}
  //Underlying source owner borrows the SAME manager weakly. Preserve its
  //independent service/resource pins, never keep a containing alias here.
  reached.manager.reset();composed=true;e.clear();return true;
 }
};
}
bool bind_deferred_campaign_object_loading_v106(const std::shared_ptr<SourceWorldBorrowV61>& world,
 dh2::loader::SourceLoadingInputsV43& inputs,std::string& e){
 auto& services=inputs.object_services;
 if(!world||!world->owner||!inputs.manager||services.load_module||services.init_post||services.make_handle||services.resolve||services.test_enable_condition||services.type_name||services.is_updatable||services.room_init_object_list||services.membership_fields||services.append_list||services.clear_list||inputs.external.stage_body[17]){e="Required sole empty actual Stage10/17 input transport before SourceLoading.create";return false;}
 auto owner=std::make_shared<DeferredObjectLoadingV106>();owner->world=world;owner->manager=inputs.manager;
#define SOURCE_OBJECT_LEAF_V106(name) services.name=[owner](auto&&... args)->bool {auto arguments=std::forward_as_tuple(args...);auto& error=std::get<sizeof...(args)-1>(arguments);if(!owner->ensure(error))return false;auto call=owner->reached.object_services.name;if(!call){error="Missing composed source Stage10 leaf: " #name;return owner->reject(error);}return call(std::forward<decltype(args)>(args)...);}
 SOURCE_OBJECT_LEAF_V106(make_handle);SOURCE_OBJECT_LEAF_V106(resolve);
 SOURCE_OBJECT_LEAF_V106(test_enable_condition);SOURCE_OBJECT_LEAF_V106(type_name);
 SOURCE_OBJECT_LEAF_V106(is_updatable);SOURCE_OBJECT_LEAF_V106(room_init_object_list);
 SOURCE_OBJECT_LEAF_V106(membership_fields);SOURCE_OBJECT_LEAF_V106(append_list);SOURCE_OBJECT_LEAF_V106(clear_list);
#undef SOURCE_OBJECT_LEAF_V106
 inputs.external.stage_body[17]=[owner](std::string& e){if(!owner->ensure(e))return dh2::loader::LifecycleStepV36::failed;auto body=owner->reached.external.stage_body[17];if(!body){e="Missing composed source Stage17 InitFinal body";owner->reject(e);return dh2::loader::LifecycleStepV36::failed;}return body(e);};
 inputs.resource_pins.push_back(owner);e.clear();return true;
}
}
