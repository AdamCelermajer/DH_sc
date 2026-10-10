#include "../game_event_runtime_v75.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2;
namespace {
unsigned checks;
void check(bool value,const char* why){++checks;if(!value)throw std::runtime_error(why);}
struct Tables {
 std::uint32_t count{2};std::vector<loader::GameEventRowV50> rows;
 const loader::GameEventRowV50* members{};const char* names_storage[2]{"event0","event1"};const char* const* names{names_storage};
 Tables(){
  loader::GameEventObjectiveRowV50 automatic;automatic.type4=6;automatic.fieldc=0;
  loader::GameEventObjectiveRowV50 interact;interact.type4=2;interact.fieldc=-1;interact.field20=17;interact.field28=4;
  rows.resize(2);rows[0].objectives14.push_back(automatic);rows[1].objectives14.push_back(interact);members=rows.data();
 }
};
struct Fixture {
 std::shared_ptr<Tables> tables=std::make_shared<Tables>();
 std::shared_ptr<loader::GameEventManagerV50> storage=std::make_shared<loader::GameEventManagerV50>();
 std::shared_ptr<int> provider=std::make_shared<int>(0),level=std::make_shared<int>(0);
 events::EventManagerOwnerV12 dispatcher{123};std::int32_t row=9;unsigned starts{};bool reject{},nested{},throwing{};
 std::shared_ptr<loader::GameEventRuntimeV75> runtime;
 Fixture(){
  loader::GameEventTablesBorrowV50 borrow{tables,&tables->count,&tables->members,&tables->names,2,2};
  auto result=loader::GameEventLoadStatusV50::pending;unsigned steps{};
  while(result==loader::GameEventLoadStatusV50::pending&&steps++<100)result=storage->load_step(borrow);
  check(result==loader::GameEventLoadStatusV50::complete,"Actual V50 storage producer completes");
  auto* later=storage->by_id(1);later->fields().state0=2;
  auto& objective=later->objectives()[0]->fields();objective.words20_2c[0]=11;objective.byte8=1;objective.completed14=1;
  loader::GameEventRuntimeServicesV75 services;services.provider=provider;
  services.current_level=[this](auto& pin,auto*& events,auto*& source_row,std::string& e){pin=level;events=&dispatcher;source_row=&row;e.clear();return true;};
  services.common_script_count=[](auto& count,std::string& e){count=7;e.clear();return true;};
  services.constant=[](const char* group,const char* name,auto& value,std::string& e){check(std::string(group)=="v2EventState","Actual state constant group");value=std::string(name)=="Inactive"?0:std::string(name)=="Active"?1:2;e.clear();return true;};
  services.start_script=[this](auto id,auto module,bool received,std::string& e){
   ++starts;check(id==7&&module==-1&&!received,"Actual completion script arguments");
   auto* first=storage->by_id(0);auto* later=storage->by_id(1);const auto& before=later->objectives()[0]->fields();
   check(first->fields().state0==0&&first->objectives()[0]->fields().completed14==1,"Current event reset/compile prefix already reached");
   check(later->fields().state0==2&&before.words20_2c[0]==11&&before.completed14==1&&before.byte8==1,"Event0 observer must see event1 prior state and quantity");
   if(nested){std::string inner;check(!runtime->reinit(inner),"Nested ReInit rejected under whole operation guard");e.clear();return true;}
   if(throwing)throw std::runtime_error("completion observer threw");
   if(reject){e="completion observer rejected";return false;}e.clear();return true;
  };
  runtime=std::make_shared<loader::GameEventRuntimeV75>(storage,std::move(services));
 }
 void prior_suffix(){
  auto* later=storage->by_id(1);const auto& f=later->objectives()[0]->fields();
  check(later->fields().state0==2&&f.words20_2c[0]==11&&f.completed14==1&&f.byte8==1,"Failure preserves not-yet-reset later event");
 }
};
void success(){
 Fixture f;std::string e;const bool completed=f.runtime->reinit(e);
 check(completed,e.empty()?"Whole source-order ReInit completes":e.c_str());
 check(e.empty()&&f.runtime->compiled()&&!f.runtime->failed(),"Whole source-order ReInit readiness");
 check(f.starts==1,"Automatic completion start delivered once");auto* later=f.storage->by_id(1);const auto& objective=later->objectives()[0]->fields();
 check(later->fields().state0==0&&objective.words20_2c[0]==0&&objective.completed14==0&&objective.byte8==1,"Later event eventually reset and compiled");
 check(f.storage->by_id(0)->fields().registered18==1&&later->fields().registered18==1&&objective.byte1c==1,"Register suffix finishes on same actual storage");
 check(f.runtime->update(e)&&later->fields().state0==1,"Successful ReInit establishes actual Update readiness");
}
void failed(bool nested,bool throwing){
 Fixture f;f.reject=!nested&&!throwing;f.nested=nested;f.throwing=throwing;std::string e;
 check(!f.runtime->reinit(e)&&f.runtime->failed()&&!f.runtime->compiled(),"ReInit provider/reentry failure latches");
 check(f.starts==1&&f.storage->by_id(0)->fields().registered18==0,"Failure precedes first registration suffix");f.prior_suffix();
 const auto first=e;check(!f.runtime->reinit(e)&&e==first&&f.starts==1,"Failed ReInit does not replay reached callback");f.prior_suffix();
}
}
int main(){try{
 success();failed(false,false);failed(true,false);failed(false,true);
 std::cout<<"PASS actual V50/V75 per-event reset-compile order, earlier observer/later prior fields, registration/readiness and failure/reentry/throw prefixes checks="<<checks<<'\n';return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
