#include "renderer_campaign_conditions_v75.hpp"
#include <utility>
namespace model_renderer {
namespace {
template<class A,class B>bool same_condition_owner_v75(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){
 return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
bool actual_condition_bag_v75(const dh2::world::ConditionDataInitServicesV3& bag,
 const std::shared_ptr<dh2::world::NativeConditionRuntimeV69>& runtime,std::string& e){
 if(!runtime||!bag.owner||bag.owner.get()!=runtime.get()||!same_condition_owner_v75(bag.owner,runtime)||
    !bag.conditions||!bag.construct_condition||!bag.initialize_condition||!bag.destroy_condition){
  e="Required SAME returned Main native ConditionList arena/services";return false;
 }return true;
}
}
bool lend_campaign_condition_bag_v75(const SourceCampaignCandidateBorrowV55& actual,
 dh2::world::ConditionDataInitServicesV3& out,std::shared_ptr<dh2::world::NativeConditionRuntimeV69>& runtime,std::string& e){
 dh2::world::ConditionDataInitServicesV3 next;std::shared_ptr<dh2::world::NativeConditionRuntimeV69> owner;
 if(!borrow_source_campaign_conditions_v70(actual,next,owner,e)||!actual_condition_bag_v75(next,owner,e))return false;
 if(same_condition_owner_v75(next.owner,actual.actual_world)||same_condition_owner_v75(next.owner,actual.level)||
    same_condition_owner_v75(next.owner,actual.objects)||same_condition_owner_v75(next.owner,actual.application)){
  e="ConditionList arena must be independent of containing campaign owner";return false;
 }
 out=std::move(next);runtime=std::move(owner);e.clear();return true;
}
bool bind_campaign_condition_base_v75(std::uintptr_t identity,dh2::world::CanonicalBaseBorrowV68 borrow,
 const dh2::world::ConditionDataInitServicesV3& bag,const std::shared_ptr<dh2::world::NativeConditionRuntimeV69>& runtime,
 std::shared_ptr<dh2::world::ConditionDataBindingV72>& out,std::string& e){
 if(!actual_condition_bag_v75(bag,runtime,e))return false;
 return dh2::world::ConditionDataBindingV72::create(identity,std::move(borrow),bag,out,e);
}
bool bind_campaign_item_conditions_v75(const SourceCampaignCandidateBorrowV55& actual,
 const std::shared_ptr<dh2::character::RetainedWorldItemObjectV1>& item,dh2::character::WorldItemGraphServicesV3& services,std::string& e){
 if(!item||!actual.actual_world||services.condition_binding_v75){e="Required once-only SAME Item condition binding";return false;}
 dh2::world::ConditionDataInitServicesV3 bag;std::shared_ptr<dh2::world::NativeConditionRuntimeV69> runtime;
 if(!lend_campaign_condition_bag_v75(actual,bag,runtime,e))return false;
 const auto identity=item->base().identity();const std::weak_ptr<void> world=actual.actual_world;
 const std::weak_ptr<dh2::character::RetainedWorldItemObjectV1> weak=item;
 dh2::world::CanonicalBaseBorrowV68 borrow=[weak,world,identity](auto& pin,auto*& base,std::string& e){
  auto scope=world.lock();auto item=weak.lock();if(!scope||!item||item->base().identity()!=identity){
   e="Required retained SAME actual Item/World condition destruction scope";return false;
  }pin=item;base=&item->base();return true;
 };
 std::shared_ptr<dh2::world::ConditionDataBindingV72> binding;
 if(!bind_campaign_condition_base_v75(identity,std::move(borrow),bag,runtime,binding,e))return false;
 services.initialization.condition_init=binding->initialization();
 services.condition_binding_v75=std::move(binding);e.clear();return true;
}
}
