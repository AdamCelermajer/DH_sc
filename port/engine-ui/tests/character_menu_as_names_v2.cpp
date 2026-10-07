// Actual retained GameSWF conversion; no synthetic std::to_string/coercion.
// Startup, localization and GPU providers are explicitly borrowed fixtures.
#define main historical_movie_fixture_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "swf_movie.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_menu_as_bridge_v1.hpp"
#include "gameswf/gameswf_function.h"
#include <stdexcept>
#include <cstring>
namespace {
void check(bool yes,const std::string& error){if(!yes)throw std::runtime_error(error);}
unsigned gets=0;
void getter(const gameswf::fn_call& fn){++gets;fn.result->set_string("menu_InventoryMain");}
void setter(const gameswf::fn_call& fn){fn.result->set_undefined();}
struct Host:Test {
 std::shared_ptr<int> owner=std::make_shared<int>(1);
 std::unique_ptr<CharacterMenuAsBridgeV1> bridge;
 unsigned conversions=0,debug_conversions=0,ignored=0,failures=0;
 std::string last;
 Host(){bridge=std::make_unique<CharacterMenuAsBridgeV1>(owner,[this](const char* name,auto& call,auto& error){
  if(!std::strcmp(name,"MenuIgnore")){++ignored;return true;}
  if(!std::strcmp(name,"MenuDebugName")){
   check(call.arguments.size()==1&&bool(call.debug_text),"Genuine deferred AS debug text provider required");
   if(!call.debug_text(call.arguments[0],last,error))return false;
   ++debug_conversions;return true;
  }
  check(call.arguments.size()==1&&bool(call.text),"Genuine deferred AS text provider required");
  if(!call.text(call.arguments[0],last,error))return false;
  ++conversions;
  if(!std::strcmp(name,"MenuReject")){++failures;error="Required menu lookup rejected";return false;}
  check(!std::strcmp(name,"MenuName"),"Unexpected typed callback");return true;
 });}
 static bool native(void* context,const char* name,const gameswf::fn_call& fn,std::string& error){
  auto& h=*static_cast<Host*>(context);
  if(!std::strcmp(name,"NativeLoadSettings"))return true;
  if(!std::strcmp(name,"NativeIsMultiplayerEnabled")){fn.result->set_bool(false);return true;}
  return h.bridge->dispatch(name,fn,error);
 }
 static bool run(void* context,SwfAsGraph& graph,std::string& error){
  auto& h=*static_cast<Host*>(context);SwfAsValue root;check(graph.root_value(root,error),error);
  gameswf::as_object* object=nullptr;check(graph.borrow_object(root,object,error),error);
  auto* env=object->get_environment();const auto size=env->get_stack_size();
  gameswf::as_value result(77);
  auto call=[&](const char* name,int count){return h.bridge->dispatch(name,gameswf::fn_call(&result,object,env,count,env->get_top_index()),error);};
  auto value=[&](const gameswf::as_value& argument,const char* expected){
   env->push(argument);check(call("MenuName",1)&&h.last==expected&&result.to_number()==77,error.empty()?std::string("Source name/result mismatch: expected ")+expected+", received "+h.last:error);env->set_stack_size(size);
  };
  value(gameswf::as_value("menu_InventoryMain"),"menu_InventoryMain");
  value(gameswf::as_value("\xd7\x97\xd7\xa8\xd7\x91"),"\xd7\x97\xd7\xa8\xd7\x91");
  value(gameswf::as_value(2.75),"2.75");value(gameswf::as_value(true),"true");
  value(gameswf::as_value(),"undefined");gameswf::as_value null;null.set_null();value(null,"null");
  gameswf::gc_ptr<gameswf::as_object> named=new gameswf::as_object(object->get_player());
  gameswf::as_value object_argument;object_argument.set_as_object(named.get_ptr());
  check(object_argument.to_object()==named.get_ptr(),"Actual object argument identity lost");
  value(object_argument,"[object Object]");
  for(const auto* argument:{&null,&object_argument}){
   const std::string expected=argument->to_xstring();
   check(expected!="null"&&expected!="[object Object]","Actual pointer debug conversion collapsed to ordinary text");
   env->push(*argument);check(call("MenuDebugName",1)&&h.last==expected&&result.to_number()==77,"Actual source debug conversion/result mismatch");
   env->set_stack_size(size);
  }
  object->builtin_member("MenuBoundName",gameswf::as_value(gameswf::as_value(getter),gameswf::as_value(setter)));
  env->push(gameswf::as_value());check(object->get_member("MenuBoundName",&env->bottom(env->get_top_index())),"Bound property unavailable");
  check(gets==0&&call("MenuIgnore",1)&&gets==0&&result.to_number()==77,"Ignored name coerced during projection");
  check(call("MenuName",1)&&gets==1&&h.last=="menu_InventoryMain"&&result.to_number()==77,"Reached property name/result mismatch");
  check(!call("MenuReject",1)&&gets==2&&h.last=="menu_InventoryMain"&&error=="Required menu lookup rejected"&&result.to_number()==77,"Required rejection lost conversion prefix");
  check(call("MenuDebugName",1)&&gets==3&&h.last=="menu_InventoryMain"&&result.to_number()==77,"Debug conversion must reach original property getter");
  env->set_stack_size(size);error.clear();gets=0;
  check(object->get_member("MenuBoundName",&result),"Bound result unavailable");
  check(call("MenuIgnore",0)&&gets==0&&result.is_property(),"Unchanged bound result coerced");result.set_undefined();
  return true;
 }
};
}
int main(int argc,char** argv){try{
 check(argc==2,"Original SWF directory required");unsigned conversions,debug_conversions,ignored,failures;std::weak_ptr<int> lease;
 {auto host=std::make_shared<Host>();host->base=argv[1];lease=host->owner;
  auto s=host->services();s.native_owner=host;s.native_action=Host::native;
  s.native_actions={"NativeLoadSettings","NativeIsMultiplayerEnabled","MenuName","MenuDebugName","MenuIgnore","MenuReject"};
  SwfMovie movie;host->movie=&movie;std::string error;
  check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",s,error),error);
  check(movie.action_script(host.get(),Host::run,error),error);
  conversions=host->conversions;debug_conversions=host->debug_conversions;ignored=host->ignored;failures=host->failures;
  host->owner.reset();check(!lease.expired(),"Native graph lease lost");
 }
 check(lease.expired(),"Native graph ownership cycle");
 std::cout<<"{\"validation\":\"PASS\",\"actual_AS_text_conversions\":"<<conversions<<",\"actual_AS_debug_text_conversions\":"<<debug_conversions<<",\"ignored_names_and_bound_results\":"<<ignored<<",\"required_failure_prefixes\":"<<failures<<",\"actual_object_virtual_string_conversion\":true,\"no_graph_cycle\":true,\"authored_character_menu_navigation_exercised\":false,\"startup_localization_GPU_are_fixtures\":true}\n";
 return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
