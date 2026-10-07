#include "../canonical_spawn_owner_v1.hpp"
#include "../canonical_gameobject_base_owner_v1.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
// Factory/lifecycle transports are explicit fixtures. Manager, base fields,
// source Item declarations/defaults and Spawn ordering are production owners.
struct Receiver {
 dh2::actor::RuntimeState runtime{};
 CanonicalGameObjectBaseOwnerV1 base;
 Receiver(std::uintptr_t id,std::shared_ptr<void> pin):base(id,3,pin,runtime){}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base.canonical(lease);}
 CanonicalPropertyActorV1 properties(){return base.properties();}
};
struct Fixture {
 std::shared_ptr<void> pin=std::make_shared<int>(7);
 CanonicalPropertyMapV1 map{{nullptr,&canonical_vec3_origin_v1(),nullptr}};
 std::vector<std::string> calls;
 std::map<std::uintptr_t,CanonicalClassReceiverV1> records;
 std::uintptr_t next{100};int fail{-1},sequence{};bool deferred{},accept{true},allocation_null{};
 CanonicalObjectManagerV1 manager;
 Fixture():manager({this,nullptr,nullptr,nullptr,destroy,network,published}){}
 bool reached(const char* name,std::string& error){calls.push_back(name);if(sequence++==fail){error=std::string("fixture failure: ")+name;return false;}return true;}
 static bool destroy(void* p,CanonicalObjectBorrowV1& a,std::string& e){auto& t=*static_cast<Fixture*>(p);t.records.erase(a.identity);return t.reached("duplicate-destroy",e);}
 static bool network(void* p,CanonicalObjectBorrowV1&,std::string& e){return static_cast<Fixture*>(p)->reached("network",e);}
 static bool published(void* p,std::int32_t,const CanonicalObjectBorrowV1&,std::uintptr_t,std::string& e){return static_cast<Fixture*>(p)->reached("published",e);}
 static bool construct(void* p,const CanonicalFactoryEntryV1& entry,CanonicalClassReceiverV1& out,std::string& e){
  auto& t=*static_cast<Fixture*>(p);assert(std::string(entry.name)=="Item");
  if(!t.reached("construct",e))return false;if(t.allocation_null)return true;
  auto r=std::make_shared<Receiver>(t.next++,t.pin);out=canonical_class_receiver_v1(r);
  out.init_post=[&t](std::string& error){return t.reached("init-post",error);};
  t.records.emplace(out.object.identity,out);return true;
 }
 static bool debug(void* p,const char*,std::string& e){return static_cast<Fixture*>(p)->reached("unknown-debug",e);}
 static bool resolve(void* p,dh2::target_providers::Handle16& h,bool fresh,const CanonicalObjectBorrowV1*& out,std::string& e){
  auto& t=*static_cast<Fixture*>(p);if(!t.reached(fresh?"resolve-true":"resolve-false",e))return false;
  out=t.manager.object(h.key);if(out)h.cached=out->identity;return true;
 }
 static bool condition(void* p,const CanonicalObjectBorrowV1&,bool value,std::string& e){assert(value);return static_cast<Fixture*>(p)->reached("condition",e);}
 static bool updatable(void* p,const CanonicalObjectBorrowV1&,bool& value,std::string& e){auto& t=*static_cast<Fixture*>(p);value=t.accept;return t.reached("virtual38",e);}
 static bool pending(void* p,const CanonicalObjectBorrowV1& a,std::string& e){auto& t=*static_cast<Fixture*>(p);return t.reached("pending",e)&&t.manager.append_pending(a,e);}
 static bool receiver(void* p,const CanonicalObjectBorrowV1& object,const CanonicalClassReceiverV1*& out,std::string&){auto& t=*static_cast<Fixture*>(p);auto f=t.records.find(object.identity);out=f==t.records.end()?nullptr:&f->second;return out!=nullptr;}
 CanonicalSpawnServicesV1 services(){return {this,construct,debug,resolve,condition,updatable,pending,receiver};}
};
int main(){std::size_t prefixes{};
 for(bool deferred:{false,true}){
  Fixture t;std::string error;CanonicalSpawnAttemptV1 a(t.manager,t.map,t.services());
  assert(a.spawn("Item","LootA",deferred,true,error));assert(a.phase()==CanonicalSpawnPhaseV1::complete);
  const auto* object=t.manager.object(a.handle().key);assert(object&&object->identity==a.constructed_receiver().object.identity);
  auto actual=std::static_pointer_cast<Receiver>(object->lease);assert(*actual->base.byte(0x80)==1);
  assert(t.calls.back()=="pending");auto init=std::find(t.calls.begin(),t.calls.end(),"init-post");
  assert(t.manager.pending()==std::vector<std::uintptr_t>{object->identity});
  assert((init==t.calls.end())==deferred);if(!deferred){assert(init[1]=="resolve-true"&&init[2]=="condition");}
  const auto count=t.calls.size();assert(!a.spawn("Item","LootA",deferred,true,error)&&t.calls.size()==count);
  for(int failure=0;failure<int(count);++failure){Fixture x;x.fail=failure;CanonicalSpawnAttemptV1 b(x.manager,x.map,x.services());assert(!b.spawn("Item","LootA",deferred,true,error));assert(x.calls.size()==std::size_t(failure+1));assert(b.phase()!=CanonicalSpawnPhaseV1::complete);++prefixes;}
  CanonicalSpawnAttemptV1 duplicate(t.manager,t.map,t.services());assert(duplicate.spawn("Item","LootA",deferred,true,error));
  assert(duplicate.handle().key==a.handle().key&&t.manager.source_count50()==1&&t.records.size()==1);
  assert(t.manager.pending()==std::vector<std::uintptr_t>({object->identity,object->identity}));
 }
 {Fixture t;std::string e;t.accept=false;CanonicalSpawnAttemptV1 a(t.manager,t.map,t.services());assert(a.spawn("Item","Loot",false,false,e));assert(t.calls.back()=="virtual38");}
 {Fixture t;std::string e;t.allocation_null=true;CanonicalSpawnAttemptV1 a(t.manager,t.map,t.services());assert(a.spawn("Item","Loot",false,true,e));assert(t.manager.source_count50()==0&&t.calls.back()=="unknown-debug");}
 {Fixture t;std::string e;CanonicalSpawnAttemptV1 a(t.manager,t.map,t.services());assert(a.spawn("Unknown","Loot",false,true,e));assert(t.calls==std::vector<std::string>{"unknown-debug"});}
 std::cout<<"Canonical Spawn PASS: actual shared manager/Item defaults, deferred ordering, duplicate old receiver, "<<prefixes<<" failure prefixes; lifecycle/factory transports explicit fixtures\n";
}
