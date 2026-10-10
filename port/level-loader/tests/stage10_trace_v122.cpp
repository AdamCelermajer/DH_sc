#include "../stage_loader_v38_stage10.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
using namespace dh2::loader;
namespace {
struct Actor {std::uintptr_t identity{};std::uint32_t* shared_handle{};};
struct Manager {
 using SourceActorBorrowV38=Actor;
 std::uint32_t phase{},counter_reads{};std::vector<std::uintptr_t> module_list;
 std::uint32_t& source_init_phase7c_v38(){return phase;}
 const auto& modules()const{return module_list;}
 std::uint32_t source_map_size1c_v38(){++counter_reads;return 0;}
 bool source_ordered_begin_v38(std::int32_t&,const Actor*& a){a=nullptr;return false;}
 bool source_ordered_next_v38(std::int32_t,std::int32_t&,const Actor*& a){a=nullptr;return false;}
 bool source_ordered_entry_v38(std::int32_t,const Actor*&){return false;}
};
struct Fixture {
 std::shared_ptr<void> level=std::make_shared<int>(1),manager_pin=std::make_shared<int>(2);
 Manager manager;std::uint32_t progress{26},state{10},counter{17},current{42};
 std::vector<std::uint32_t> cleared;
 LifecycleBorrowV36 borrow(){return {level,1,{&progress,&state,&counter,&current}};}
 CanonicalInitPostServicesV38<Manager> services(){
  CanonicalInitPostServicesV38<Manager> s;
  s.clear_list=[this](std::uint32_t offset,std::string&){cleared.push_back(offset);return true;};return s;
 }
 void untouched()const{assert(manager.phase==0&&manager.counter_reads==0&&current==42&&counter==17&&cleared.empty());}
};
}
int main(){
 {
  Fixture f;unsigned traces{};
  Stage10BodyV38<Manager> body(f.borrow(),f.manager_pin,f.manager,f.services(),[&](std::string&){++traces;f.untouched();return true;});
  assert(body.step()==LifecycleStepV36::pending&&traces==1&&f.current==0&&f.manager.phase==4);
  assert((f.cleared==std::vector<std::uint32_t>{0x2c,0x44,0x34}));
  assert(body.step()==LifecycleStepV36::complete&&traces==1&&f.manager.counter_reads==1&&f.manager.phase==5);
  assert(body.step()==LifecycleStepV36::complete&&traces==1&&f.manager.counter_reads==1&&f.state==10);
 }
 {
  Fixture f;unsigned traces{};
  Stage10BodyV38<Manager> body(f.borrow(),f.manager_pin,f.manager,f.services(),[&](std::string& e){++traces;f.untouched();e="actual trace failure";return false;});
  assert(body.step()==LifecycleStepV36::failed&&body.error()=="actual trace failure");f.untouched();
  assert(body.step()==LifecycleStepV36::failed&&traces==1);f.untouched();
 }
 {
  Fixture f;Stage10BodyV38<Manager> body(f.borrow(),f.manager_pin,f.manager,f.services(),{});
  assert(body.step()==LifecycleStepV36::failed&&!body.error().empty());f.untouched();
 }
 {
  Fixture f;unsigned traces{};
  Stage10BodyV38<Manager> body(f.borrow(),f.manager_pin,f.manager,f.services(),[&](std::string&)->bool{++traces;throw std::runtime_error("trace exception");});
  assert(body.step()==LifecycleStepV36::failed&&body.error()=="trace exception");f.untouched();
  assert(body.step()==LifecycleStepV36::failed&&traces==1);
 }
 {
  Fixture f;Stage10BodyV38<Manager>* active{};unsigned traces{};
  Stage10BodyV38<Manager> body(f.borrow(),f.manager_pin,f.manager,f.services(),[&](std::string& e){++traces;assert(active->step()==LifecycleStepV36::failed);e.clear();return true;});active=&body;
  assert(body.step()==LifecycleStepV36::failed&&body.error()=="Stage10 reentered; reached prefix retained");f.untouched();
  assert(body.step()==LifecycleStepV36::failed&&traces==1);
 }
 {
  Fixture f;Stage10BodyV38<Manager> body(f.borrow(),f.manager_pin,f.manager,f.services(),[&](std::string&){f.state=11;return true;});
  assert(body.step()==LifecycleStepV36::failed&&f.current==42&&f.manager.phase==0&&f.manager.counter_reads==0);
 }
 std::cout<<"PASS Stage10 trace before first mutation, once only, failures/exceptions/reentry retained\n";
}
