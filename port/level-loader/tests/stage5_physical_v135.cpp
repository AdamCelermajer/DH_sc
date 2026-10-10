#include "../stage_loader_v46_physical.hpp"
#include <character_design_services.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::loader;
namespace {
unsigned checks{};
void check(bool value,const char* reason){++checks;if(!value)throw std::runtime_error(reason);}
struct Fields {std::uint32_t progress{10},phase{5},counter{},current{};};
struct Fixture {
 std::shared_ptr<Fields> fields=std::make_shared<Fields>();
 std::shared_ptr<dh2::physical::NativeWorld> world=std::make_shared<dh2::physical::NativeWorld>();
 std::shared_ptr<dh2::character::DebugSwitches> debug{dh2_character_debug_create(),dh2_character_debug_destroy};
 dh2::character::DebugFileServices24 files{this,open,close};
 EarlyLoadingDebugV46 services;
 std::vector<std::string> trace;
 int fail_at{-1};bool throw_failure{},mutate_phase{},existing_backend{};unsigned opens{};
 static int open(void* raw,const char*,std::uintptr_t* handle){++static_cast<Fixture*>(raw)->opens;*handle=0;return 0;}
 static int close(void*,std::uintptr_t){throw std::runtime_error("Unexpected missing-file close");}
 bool event(const char* name,std::string& error){
  trace.emplace_back(name);
  if(static_cast<int>(trace.size())-1!=fail_at)return true;
  error="injected Stage5 Debug failure";
  if(throw_failure)throw std::runtime_error(error);
  return false;
 }
 explicit Fixture(bool has_backend=true):existing_backend(has_backend){
  check(bool(debug),"Actual Debug C1 failed");
  if(existing_backend){const float bounds[4]{-10,-10,10,10};world->load(bounds);}
  services.actual_owner=debug;
  services.get_instance_and_switch=[this](const char* key,bool& value,std::string& error){
   const bool physical=std::string(key)=="isTracingPhysicalWorld";
   check(physical||std::string(key)=="isTracingLevel_Loading","Wrong Stage5 Debug key");
   check(physical?!world->backend():bool(world->backend())==existing_backend,"Debug query reached on wrong side of clear/construction");
   if(!event(key,error))return false;
   std::uint32_t result{};
   check(dh2_character_debug_get(&result,debug.get(),key,&files)==1,"Actual Debug query failed");
   value=result!=0;return true;
  };
  services.load=[this](std::string& error){
   check(!world->backend(),"Physical Debug load ran before clear or after construction");
   if(!event("DebugSwitches.load",error))return false;
   check(dh2_character_debug_load(debug.get(),&files)==1,"Actual Debug load failed");
   if(mutate_phase)fields->phase=7;
   return true;
  };
 }
 LifecycleBorrowV36 borrow(){return {fields,reinterpret_cast<std::uintptr_t>(fields.get()),{&fields->progress,&fields->phase,&fields->counter,&fields->current}};}
 std::unique_ptr<LifecycleV36> dispatcher(){
  LifecycleServicesV36 dispatch;dispatch.stage_body[5]=stage5_physical_body_v46(borrow(),world,services);
  dispatch.publish_progress=[this](std::int32_t phase,std::int32_t progress,std::string&){
   check(phase==6&&progress==15&&fields->phase==6&&fields->progress==15,"Stage5 dispatch/tail order changed");
   check(world->backend()&&world->backend()->GetBodyCount()==1,"Stage5 did not construct genuine empty Box2D");
   trace.emplace_back("onProgress(6,15)");return true;
  };
  return std::make_unique<LifecycleV36>(borrow().fields,fields,std::vector<std::shared_ptr<const void>>{},std::move(dispatch));
 }
 void physical_key_inserted(){
  std::uint32_t loaded{},count{};check(dh2_character_debug_snapshot(debug.get(),&loaded,&count)==1,"Debug snapshot failed");
  bool found{};for(std::uint32_t i=0;i<count;++i){const char* name{};std::uint32_t value{};
   check(dh2_character_debug_entry(debug.get(),i,&name,&value)==1,"Debug entry failed");
   if(std::string(name)=="isTracingPhysicalWorld"){found=true;check(value==0,"Unexpected trace value");}}
  check(loaded&&found&&opens==1,"Stage5 omitted shared Debug map insertion");
 }
};
}
int main(){try{
 for(bool existing:{false,true}){Fixture f(existing);auto dispatcher=f.dispatcher();check(dispatcher->tick()==LifecycleStatusV36::loading,"Stage5 success failed");
  check(f.trace==std::vector<std::string>({"isTracingLevel_Loading","DebugSwitches.load","isTracingPhysicalWorld","onProgress(6,15)"}),"Stage5 source callback order differs");f.physical_key_inserted();}
 for(int failed_leaf=0;failed_leaf<3;++failed_leaf)for(bool throwing:{false,true}){
  Fixture f;f.fail_at=failed_leaf;f.throw_failure=throwing;auto dispatcher=f.dispatcher();
  check(dispatcher->tick()==LifecycleStatusV36::failed,"Stage5 provider failure escaped");
  check(f.trace.size()==static_cast<std::size_t>(failed_leaf+1),"Stage5 ran callbacks after failed leaf");
  check(bool(f.world->backend())==(failed_leaf==0),"Stage5 failed-prefix backend differs");
  check(f.fields->phase==5&&f.fields->progress==10,"Failed Stage5 advanced dispatcher/progress");
  const auto failure=dispatcher->diagnostics().error;check(failure=="injected Stage5 Debug failure","First Stage5 failure lost");
  const auto calls=f.trace.size();check(dispatcher->tick()==LifecycleStatusV36::failed&&dispatcher->diagnostics().error==failure&&f.trace.size()==calls,"Failed Stage5 replayed clear/Debug");
 }
 {Fixture f;f.services.load={};auto dispatcher=f.dispatcher();check(dispatcher->tick()==LifecycleStatusV36::failed&&!f.world->backend(),"Missing physical Debug load did not preserve clear prefix");
  check(f.trace==std::vector<std::string>({"isTracingLevel_Loading"})&&f.fields->phase==5&&f.fields->progress==10,"Missing physical Debug load ran suffix");}
 {Fixture f;f.mutate_phase=true;auto dispatcher=f.dispatcher();check(dispatcher->tick()==LifecycleStatusV36::failed&&!f.world->backend(),"Mutated Stage5 phase constructed backend");
  check(f.trace==std::vector<std::string>({"isTracingLevel_Loading","DebugSwitches.load"}),"Mutated Stage5 phase reached query/construction");}
 {Fixture f;std::function<LifecycleStepV36(std::string&)> body;const auto load=f.services.load;
  f.services.load=[&](std::string& error){if(!load(error))return false;check(body(error)==LifecycleStepV36::failed,"Reentered Stage5 escaped");error.clear();return true;};
  body=stage5_physical_body_v46(f.borrow(),f.world,f.services);std::string error;
  check(body(error)==LifecycleStepV36::failed&&!f.world->backend(),"Reentered Stage5 constructed backend");
  check(error=="Stage5 body already attempted; refusing physical reset replay","Reentered Stage5 first failure lost");
  check(f.trace==std::vector<std::string>({"isTracingLevel_Loading","DebugSwitches.load"}),"Reentered Stage5 reached suffix");}
 std::cout<<"Stage5 actual physics callback-order/failure regression: PASS ("<<checks<<" checks)\n";
 return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
