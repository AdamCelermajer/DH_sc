#include "script_manager_bind_v63.hpp"
#include <cassert>
#include <iostream>
#include <vector>
using namespace dh2::loader;
namespace v63_transport_fixture {
// Explicit binding/lifetime fixtures only. No actual Main engine leaf runs.
struct App {unsigned* destructed;std::uint64_t cache_like_member{};explicit App(unsigned& n):destructed(&n){}~App(){++*destructed;}};
struct Constants{};struct Menus{};struct Menu{};struct FX{};struct Objects{};struct Handle{};struct Object{};struct Animator{};struct Character{};struct Cache{};
struct Types {using Application=App;using Constants=v63_transport_fixture::Constants;using MenuManager=Menus;using Menu=v63_transport_fixture::Menu;using FXManager=FX;using ObjectManager=Objects;using ObjectHandle=Handle;using Object=v63_transport_fixture::Object;using Character=v63_transport_fixture::Character;using Animator=v63_transport_fixture::Animator;};
}
int main(){using namespace v63_transport_fixture;std::string error;unsigned destroyed=0,old_factory_calls=0,execute_calls=0,cache_opens=0,sentinel_opens=0;
 auto app=std::make_shared<App>(destroyed);std::weak_ptr<App> weak_app=app;
 auto cache_owner=std::make_shared<Cache>();std::weak_ptr<Cache> weak_cache=cache_owner;
 ScriptManagerServicesV52 cache;cache.owner=cache_owner;cache.open_file=[weak_cache,&cache_opens](const std::string&,ScriptFileV52& out,std::string& e){auto actual=weak_cache.lock();if(!actual){e="cache fixture expired";return false;}++cache_opens;out={};return true;};
 ScriptInitLeavesV62<Types> init;init.application=app;
 // Deliberately failing Execute fixture proves typed args/result transport.
 // It reports no successful actual engine behavior.
 ActualScriptExecuteLeafV63<Types> execute=[weak_app,&execute_calls](App& actual,const CheckedCommandBorrowV59& same,bool skip,std::int32_t context,std::string& e){auto pin=weak_app.lock();assert(pin&&pin.get()==&actual&&skip&&context==123);assert(same.actual_receiver&&same.actual_data&&same.actual_data->complete()&&*same.kind8==1&&*same.data_c==same.actual_data->identity());++execute_calls;e="explicit Main Execute transport fixture failure";return false;};
 // Sentinel output must remain exactly retained after failed composition.
 auto sentinel_owner=std::make_shared<int>(42);ScriptManagerServicesV52 rejected;rejected.owner=sentinel_owner;rejected.open_file=[&sentinel_opens](const std::string&,ScriptFileV52& out,std::string&){++sentinel_opens;out={};return true;};
 auto unchanged=[&](){assert(rejected.owner.get()==sentinel_owner.get()&&!rejected.command_factory&&bool(rejected.open_file));ScriptFileV52 f;std::string e;assert(rejected.open_file("sentinel",f,e));};
 {auto existing=cache;existing.command_factory=[&old_factory_calls](std::int32_t,ScriptCommandBorrowV52&,std::string&){++old_factory_calls;return false;};assert(!bind_script_manager_services_v63<Types>(existing,init,execute,rejected,error));assert(error.find("existing genuine command_factory")!=std::string::npos&&old_factory_calls==0&&cache_opens==0&&execute_calls==0);unchanged();}
 {auto direct=cache;direct.owner=app;assert(!bind_script_manager_services_v63<Types>(direct,init,execute,rejected,error));assert(error.find("must not own or alias")!=std::string::npos);unchanged();}
 {auto alias=cache;alias.owner=std::shared_ptr<void>(app,&app->cache_like_member);assert(alias.owner.get()!=static_cast<void*>(app.get()));assert(!bind_script_manager_services_v63<Types>(alias,init,execute,rejected,error));assert(error.find("must not own or alias")!=std::string::npos);unchanged();}
 {assert(!bind_script_manager_services_v63<Types>(cache,init,ActualScriptExecuteLeafV63<Types>{},rejected,error));assert(error.find("Main command Execute")!=std::string::npos);unchanged();}
 ScriptManagerServicesV52 composed;assert(bind_script_manager_services_v63<Types>(cache,init,execute,composed,error));assert(composed.owner.get()==cache_owner.get()&&bool(composed.command_factory)&&cache_opens==0&&execute_calls==0&&old_factory_calls==0);
 // Construct actual V59 receiver/Data for source base455628 BX LR. The tiny
 // serialized Data is synthetic kind1 input; it is not a new decode campaign.
 ScriptCommandBorrowV52 borrow;assert(composed.command_factory(1,borrow,error));auto command=std::static_pointer_cast<CanonicalScriptCommandV59>(borrow.actual_owner);assert(command&&borrow.identity==command->identity());
 auto bytes=std::make_shared<std::vector<std::uint8_t>>(std::initializer_list<std::uint8_t>{1,0,0,0});auto data=ScriptCommandDataV52::original_storage_factory(1,error);assert(data);ScriptReaderV52 reader({bytes,bytes->data(),bytes->size(),true});assert(data->read(reader,error)&&reader.finished());*borrow.kind8=1;*borrow.retained_data=data;*borrow.data_c=data->identity();assert(borrow.init(error)&&command->initialized());
 assert(!command->execute(true,123,error)&&error=="explicit Main Execute transport fixture failure"&&execute_calls==1);
 // Retained composed factory/command/cache must not keep actual App alive.
 app.reset();assert(weak_app.expired()&&destroyed==1&&!weak_cache.expired());assert(!command->execute(true,123,error)&&execute_calls==1);
 {assert(!bind_script_manager_services_v63<Types>(cache,init,execute,rejected,error));assert(error.find("Application authority")!=std::string::npos);unchanged();}
 borrow.retained_data->reset();*borrow.data_c=0;assert(borrow.release_storage(error));borrow={};command.reset();
 cache_owner.reset();cache={};assert(!weak_cache.expired());composed={};assert(weak_cache.expired());assert(sentinel_opens==5&&cache_opens==0&&old_factory_calls==0&&execute_calls==1&&destroyed==1);
 std::cout<<"{\"validation\":\"PASS\",\"existing_factory_rejected\":true,\"direct_and_alias_App_owner_rejected\":true,\"failed_output_preserved\":true,\"weak_App_not_retained\":true,\"Execute_fixture_calls\":1,\"actual_Main_engine_effects_verified\":false,\"whole_level_ready\":false}\n";
}
