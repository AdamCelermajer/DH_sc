#include "level_batching_source_v96.hpp"
#include <iostream>
#include <stdexcept>

using namespace dh2;
using namespace dh2::loader;
namespace {
void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
struct Object {
 target_providers::Handle16 handle;
 std::uint32_t type{20};std::int32_t room{};
 std::uint8_t underlay{};
 std::string name,archetype;
 bool gameobject{true},faerie{};
 static bool set_name(void* p,const char* value,std::string&){static_cast<Object*>(p)->name=value;return true;}
 static bool set_archetype(void* p,const char* value,std::string&){static_cast<Object*>(p)->archetype=value;return true;}
 static bool as_character(void*,std::uintptr_t& out,std::string&){out=0;return true;}
 world::CanonicalObjectBorrowV1 borrow(const std::shared_ptr<Object>& owner){
  return {reinterpret_cast<std::uintptr_t>(this),owner,&handle,&type,nullptr,&room,this,set_name,set_archetype,as_character};
 }
 BatchObjectBorrowV96 batch(const std::shared_ptr<Object>& owner){
  if(!gameobject)return {};
  return {owner,reinterpret_cast<std::uintptr_t>(this),&underlay,&name,&archetype};
 }
};
struct Fixture {
 world::CanonicalObjectManagerV1 manager{{}};
 std::shared_ptr<int> provider=std::make_shared<int>(0);
 std::vector<std::shared_ptr<Object>> objects;
 BatchNativeServicesV96 native;
 std::size_t selections{},appends{},traces{},faerie_calls{};
 bool current{true};
 Fixture(){
  native.provider=provider;
  native.validate_current=[this](std::string& e){if(!current)e="Retired test scope";return current;};
  native.as_gameobject=[this](const auto& delivered,auto& out,std::string& e){
   for(const auto& object:objects)if(delivered.identity==reinterpret_cast<std::uintptr_t>(object.get())){
    check(!delivered.lease.owner_before(object)&&!object.owner_before(delivered.lease),"Different delivered receiver owner");
    out=object->batch(object);e.clear();return true;
   }
   e="Unknown delivered receiver";return false;
  };
  native.trace=[this](const char* key,std::string&){check(std::string(key)=="isTracingBatchingCompiler","Different original trace key");++traces;return true;};
  native.is_faerie=[this](std::uintptr_t id,bool& out,std::string&){
   ++faerie_calls;for(const auto& object:objects)if(id==reinterpret_cast<std::uintptr_t>(object.get())){out=object->faerie;return true;}return false;
  };
 }
 std::shared_ptr<Object> add(const char* type,const char* name,bool underlay=false,bool faerie=false,bool gameobject=true){
  auto object=std::make_shared<Object>();object->underlay=underlay;object->faerie=faerie;object->gameobject=gameobject;
  std::string e;target_providers::Handle16 handle;
  check(manager.add(object->borrow(object),name,type,0,false,handle,e),e);
  objects.push_back(object);return object;
 }
 BatchListServicesV96 list(){
  BatchListServicesV96 s;s.validate_current=native.validate_current;
  s.native=[this](const BatchNativeServicesV96*& out,std::string&){++selections;out=&native;return true;};return s;
 }
};
}
int main(){try{
 //Actual manager C1 contains its reserved NULL map entry. Neither selection
 //nor compiler is available: the original empty Stage22 must still finish.
 {Fixture f;auto s=f.list();s.native={};std::string e;
  check(original_batch_list_v96(f.manager,s,e),e);check(e.empty()&&!f.selections&&!f.appends,"Empty list reached selection/append");}
 //A fully filtered list likewise has no append/compiler callback at all.
 {Fixture f;f.add("Module","underlay",true);f.add("Player","player");f.add("Character","xPlayerx");
  f.add("Character","faerie",false,true);f.add("TriggerObject","unknown");f.add("Block","block");f.add("Module","not-gameobject",false,false,false);
  auto s=f.list();std::string e;check(original_batch_list_v96(f.manager,s,e),e);
  check(f.selections==7&&f.traces==5&&f.faerie_calls==1&&!f.appends,"Filtered list changed source call order");}
 //Only the first positive append requires compiler158; do not inspect later
 //objects or report successful completion when that compiler is missing.
 {Fixture f;f.add("Player","excluded");f.add("Module","eligible");f.add("Decor","later");
  auto s=f.list();std::string e;check(!original_batch_list_v96(f.manager,s,e),"Eligible list accepted missing compiler");
  check(e.find("BatchNodeCompiler158")!=std::string::npos&&f.selections==2,"Missing compiler was not checked at reached append");}
 //The same production loop appends to the actual native compiler in source
 //manager order. Module/Decor inclusion precedes Player-name exclusion.
 {Fixture f;auto first=f.add("Module","PlayerModule");f.add("Character","excludedPlayer");
  auto second=f.add("Decor","PlayerDecor");auto third=f.add("Door","door");
  BatchNodeCompilerSourceV96 compiler(f.native);auto s=f.list();
  s.append=[&](const auto& object,std::string& e){++f.appends;return compiler.append_object(object,e);};
  std::string e;check(original_batch_list_v96(f.manager,s,e),e);
  check(compiler.source_objects10()==std::vector<std::uintptr_t>{reinterpret_cast<std::uintptr_t>(first.get()),reinterpret_cast<std::uintptr_t>(second.get()),reinterpret_cast<std::uintptr_t>(third.get())},"Positive list changed compiler vector order");
  check(f.appends==3,"Positive append count changed");}
 //A reached failure retains the actual compiler prefix and stops traversal.
 {Fixture f;auto first=f.add("Module","first");f.add("Decor","second");f.add("Door","later");
  BatchNodeCompilerSourceV96 compiler(f.native);auto s=f.list();
  s.append=[&](const auto& object,std::string& e){if(++f.appends==2){e="Reached append failure";return false;}return compiler.append_object(object,e);};
  std::string e;check(!original_batch_list_v96(f.manager,s,e)&&e=="Reached append failure","Reached failure was masked");
  check(f.selections==2&&compiler.source_objects10()==std::vector<std::uintptr_t>{reinterpret_cast<std::uintptr_t>(first.get())},"Failed append lost/replayed compiler prefix");}
 std::cout<<"Stage22 native batch list: 5 cases passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
