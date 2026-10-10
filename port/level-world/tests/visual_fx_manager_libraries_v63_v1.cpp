#include "../visual_fx_manager_libraries_v63.hpp"
#include "../application_services_owner_v5.hpp"
#include "../../level-loader/lifecycle_v36.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2;
namespace {
unsigned checks;
void check(bool value,const char* why){++checks;if(!value)throw std::runtime_error(why);}
using Bytes=std::vector<std::uint8_t>;
void word(Bytes& out,std::uint32_t value){for(unsigned i=0;i<4;++i)out.push_back(std::uint8_t(value>>(8*i)));}
void text(Bytes& out,const char* value){std::string s=value;word(out,std::uint32_t(s.size()));out.insert(out.end(),s.begin(),s.end());}
void names(Bytes& out,std::initializer_list<const char*> values){word(out,std::uint32_t(values.size()));for(auto v:values)text(out,v);}
data::EffectsTables effects(){
 Bytes records,n,layout,keys,paths;for(unsigned i=0;i<3;++i){word(records,0);word(n,0);}
 names(layout,{"File","ForceCancel","Loop","OrientOnce","OrientWithAnchor","PlayTime","PoolSize","Redir","ScaleWithAnchor","SelfIllum","Speed","SubObject"});
 names(layout,{"ForceCache","LoopAFX","Steps","Type"});
 names(layout,{"BloodDeathEffect","BloodEffect","FootprintEffect","SwooshEffect","TriggerFloorFX"});
 names(layout,{"Effect","Floortype","RunSound","WalkSound"});names(keys,{"actual_fx"});names(paths,{"data/actual_fx.bdae"});
 data::EffectsTables result;std::string e;
 check(result.load({records.data(),records.size()},{n.data(),n.size()},{layout.data(),layout.size()},{keys.data(),keys.size()},{paths.data(),paths.size()},e),e.c_str());
 return result;
}
void stage_case(unsigned nested_operation,bool enabled){
 auto app=std::make_shared<application::ApplicationServicesOwnerV5>();auto tables=effects();
 fx::VisualFxManagerLibrariesV63* actual{};std::vector<unsigned> calls;std::string first_failure;
 fx::VisualFxLibraryDebugV63 debug;debug.application=app;
 debug.invoke=[&](std::uint32_t operation,const char* name,std::uint32_t& value,std::string& e){
  calls.push_back(operation);
  check(operation==fx::debug_load||operation==fx::debug_module,"Only original PreCache Debug leaves");
  check(operation==fx::debug_load?!name:(name&&std::string(name)=="AnimatedFX"),"Exact source Debug arguments");
  if(operation==nested_operation){
   std::string inner;check(!actual->precache_libraries(inner),"Nested same-owner mutation rejected");first_failure=inner;
   e="provider swallowed nested failure"; // Deliberately report success anyway.
  }else e.clear();
  value=enabled?1:0;return true;
 };
 fx::VisualFxManagerLibrariesV63 owner(tables.borrow(),std::move(debug));actual=&owner;
 auto& pending=owner.pending_queue_v63();pending.ids[0]=0;pending.count=1;
 std::uint32_t phase=30,progress=78,counter=7,current=5;unsigned publications{};
 auto level=std::make_shared<int>(0);loader::LifecycleServicesV36 services;
 services.stage_body[30]=[&](std::string& e){return owner.precache_libraries(e)?loader::LifecycleStepV36::complete:loader::LifecycleStepV36::failed;};
 services.publish_progress=[&](auto p,auto v,std::string& e){check(p==31&&v==81,"Actual dispatcher source tail");++publications;e.clear();return true;};
 loader::LifecycleV36 lifecycle({&progress,&phase,&counter,&current},level,{},std::move(services));
 const auto status=lifecycle.tick();
 check(pending.count==1&&pending.ids[0]==0,"PreCache preserves same registration queue");
 if(nested_operation){
  check(status==loader::LifecycleStatusV36::failed,"Swallowed nested failure reaches dispatcher");
  check(phase==30&&progress==78&&counter==7&&publications==0,"Failure cannot publish byte/state/progress suffix");
  check(owner.precache_byte4()==0,"No byte4 publication after nested failure");
  check(owner.error()==first_failure&&lifecycle.diagnostics().error==first_failure,"First failure survives provider success/error overwrite");
  check(calls==(nested_operation==fx::debug_load?std::vector<unsigned>{fx::debug_load}:std::vector<unsigned>{fx::debug_load,fx::debug_module}),"Stop before next provider leaf");
  std::string replay;const auto reached=calls;check(!owner.precache_libraries(replay)&&replay==first_failure&&calls==reached,"Sticky failure refuses direct replay");
  check(lifecycle.tick()==status&&calls==reached,"Dispatcher refuses replay");
 }else{
  check(status==loader::LifecycleStatusV36::loading&&phase==31&&progress==81&&counter==5&&publications==1,"Whole dispatcher/leaf success");
  check(owner.precache_byte4()==unsigned(enabled),"Original conditional byte4 store");
  check(calls==std::vector<unsigned>{fx::debug_load,fx::debug_module},"Original Debug leaf order");
 }
}
}
int main(){try{
 stage_case(0,false);stage_case(0,true);stage_case(fx::debug_load,true);stage_case(fx::debug_module,true);
 std::cout<<"PASS actual V63 PreCache leaf and Lifecycle stage30; swallowed nested failures, suffix/replay guards, source arguments and queue checks="<<checks<<'\n';return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
