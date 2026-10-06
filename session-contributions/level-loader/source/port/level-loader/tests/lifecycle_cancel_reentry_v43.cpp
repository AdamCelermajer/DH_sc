#include "lifecycle_v36.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::loader;
// Safety-adapter fixture only: exercises cancellation ordering, no Level C1
// or game cleanup provider is claimed by these scalar inputs.
struct Fields {std::uint32_t progress{},state{},counter{},current{};};
int main(){
 auto actual=std::make_shared<Fields>();std::weak_ptr<Fields> weak_level=actual;
 auto resource=std::make_shared<int>(7);std::weak_ptr<int> weak_resource=resource;
 auto* fields=actual.get();LifecycleV36* runtime{};bool nested=true;unsigned calls=0;
 LifecycleServicesV36 services;
 services.cancel_and_unload=[&](std::string&){
  ++calls;assert(!weak_level.expired()&&!weak_resource.expired());
  if(nested){nested=false;assert(runtime->tick()==LifecycleStatusV36::failed);}
  return LifecycleStepV36::complete;
 };
 LifecycleV36 source({&fields->progress,&fields->state,&fields->counter,&fields->current},actual,{resource},std::move(services));runtime=&source;
 actual.reset();resource.reset();source.request_cancel();
 assert(source.tick()==LifecycleStatusV36::failed);
 assert(calls==1&&!weak_level.expired()&&!weak_resource.expired());
 assert(source.diagnostics().required_service=="runtime-thread/nonreentrancy");
 assert(source.diagnostics().owned_pins==2);
 source.request_cancel();assert(source.tick()==LifecycleStatusV36::cancelled);
 assert(calls==2&&weak_level.expired()&&weak_resource.expired());
 assert(source.diagnostics().owned_pins==0&&source.tick()==LifecycleStatusV36::cancelled);
 std::cout<<"LIFECYCLE_CANCEL_REENTRY_V43 PASS nested_failure_preserved=1 failed_cleanup_pins_retained=1 successful_cleanup_releases_pins=1 terminal_tick_safe=1 component_fixture_only=1\n";
}
