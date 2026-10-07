#pragma once
#include <gslevel_lifecycle_v2.hpp>
#include <cstring>
namespace dh2::loader {
struct GSApplicationBorrowV44 {
 std::shared_ptr<void> actual_owner;
 const std::uint8_t* byte_ec{};
 std::uint32_t* original_debug_word_global{}; // GOT2dfc, source3866b4.
 std::uint8_t* original_debug_flag_global{}; // GOT2fd0, source3866bc/3867bc.
};
template<class Level> struct GSLevelReadBorrowV44 {
 std::shared_ptr<Level> actual_owner;
 std::uintptr_t identity{};
 const std::uint32_t* state130{};
 const std::uint8_t* byte145{};
};
struct GSMenuBorrowV44 {std::shared_ptr<void> actual_owner;std::uintptr_t identity{};};
template<class Level> struct GSLevelUpdateServicesV44 {
 std::function<bool(const std::shared_ptr<Level>&,GSLevelReadBorrowV44<Level>&,std::string&)> borrow_level;
 // Exact Level.Load3ef218 only: same source state130=0. No whole Init/readiness.
 std::function<bool(const std::shared_ptr<Level>&,std::string&)> level_load;
 // Actual Level.Update(false), routed to real source loading/frame authority.
 std::function<bool(const std::shared_ptr<Level>&,bool,std::string&)> level_update;
 std::shared_ptr<void> actual_debug_owner;
 std::function<bool(std::string&)> debug_load;
 std::function<bool(const char*,bool&,std::string&)> debug_get_switch;
 std::function<bool(const char*,bool,std::string&)> debug_set_switch;
 std::function<bool(GSMenuBorrowV44&,std::string&)> menu_instance;
 std::function<bool(const GSMenuBorrowV44&,bool,std::string&)> menu_update;
};
enum class GSLevelUpdateResultV44 {advanced,application_gate,failed};
// Exact whole GS.Update386630 envelope. One actual GS fields reference; no
// current-Level slot, GameSM state selection, alternate loading or ready flag.
template<class Level> class GSLevelUpdateV44 {
 std::shared_ptr<void> actual_gs_owner_;GSLevelFieldsV2<Level>& fields_;
 GSApplicationBorrowV44 application_;GSLevelUpdateServicesV44<Level> services_;
 bool busy_{},failed_{};std::string error_,required_;
 bool reject(const char* what){failed_=true;required_=what;if(error_.empty())error_=std::string("Required actual GS.Update provider: ")+what;return false;}
 template<class F,class... Args> bool call(const char* name,const F& fn,Args&&... args){
  if(!fn||!fn(std::forward<Args>(args)...,error_)||failed_)return reject(name);return true;
 }
 bool borrow(const std::shared_ptr<Level>& level,GSLevelReadBorrowV44<Level>& out){
  if(!level)return reject("nonnull actual GS.field34 Level");
  if(!call("SAME C1 state130/byte145 borrow",services_.borrow_level,level,out))return false;
  if(!out.actual_owner||out.actual_owner.get()!=level.get()||out.actual_owner.owner_before(level)||level.owner_before(out.actual_owner)||!out.identity||!out.state130||!out.byte145)return reject("SAME retained Level fields");return true;
 }
 static std::int32_t signed_word(std::uint32_t v){std::int32_t result;std::memcpy(&result,&v,4);return result;}
 bool body(){
  switch(fields_.loading38){
  case 1:fields_.loading38=2;break;
  case 2:{auto level=fields_.level34;if(!level)return reject("nonnull Level.Load receiver");if(!call("Level.Load",services_.level_load,level))return false;fields_.loading38=3;break;}
  case 3:{auto level=fields_.level34;if(!level)return reject("nonnull Level.Update receiver");if(!call("Level.Update(false)",services_.level_update,level,false))return false;
   GSLevelReadBorrowV44<Level> actual;if(!borrow(fields_.level34,actual))return false;if(*actual.state130==38)fields_.loading38=4;break;}
  case 4:{
   if(!services_.actual_debug_owner||!application_.original_debug_word_global||!application_.original_debug_flag_global)return reject("actual Debug/global word/byte owners");
   *application_.original_debug_word_global=0;*application_.original_debug_flag_global=0;
   if(!call("DebugSwitches.load",services_.debug_load))return false;
   bool enabled{};if(!call("DebugSwitches.GetSwitch",services_.debug_get_switch,"Lua_DumpCalls",enabled))return false;
   if(enabled){*application_.original_debug_flag_global=1;if(!call("DebugSwitches.load after flag1",services_.debug_load)||!call("DebugSwitches.SetSwitch(false)",services_.debug_set_switch,"Lua_DumpCalls",false))return false;}
   auto level=fields_.level34;if(!level)return reject("nonnull Level.Update receiver");if(!call("Level.Update(false)",services_.level_update,level,false))return false;break;}
  default:break;
  }
  // Original rereads GS.field34 after every reached callback. Same source
  // Level145 skips menu; signed state130 in2..26 skips menu, outside updates.
  const auto level=fields_.level34;
  if(level){GSLevelReadBorrowV44<Level> actual;if(!borrow(level,actual))return false;
   if(*actual.byte145)return true;const auto state=signed_word(*actual.state130);if(state>1&&state<=26)return true;
  }
  GSMenuBorrowV44 menu;if(!call("MenuManager.GetInstance",services_.menu_instance,menu))return false;
  if(!menu.actual_owner||!menu.identity)return reject("actual MenuManager instance");
  return call("MenuManager.Update(true)",services_.menu_update,menu,true);
 }
public:
 GSLevelUpdateV44(std::shared_ptr<void> actual_gs_owner,GSLevelFieldsV2<Level>& actual_fields,GSApplicationBorrowV44 application,GSLevelUpdateServicesV44<Level> services):actual_gs_owner_(std::move(actual_gs_owner)),fields_(actual_fields),application_(std::move(application)),services_(std::move(services)){}
 GSLevelUpdateV44(const GSLevelUpdateV44&)=delete;
 const std::string& error()const noexcept{return error_;}
 const std::string& required_service()const noexcept{return required_;}
 GSLevelUpdateResultV44 tick(){
  if(failed_)return GSLevelUpdateResultV44::failed;if(busy_){reject("nonreentrant owning-runtime access");return GSLevelUpdateResultV44::failed;}
  if(!actual_gs_owner_||!application_.actual_owner||!application_.byte_ec){reject("SAME GS/Application byteec lease");return GSLevelUpdateResultV44::failed;}
  if(*application_.byte_ec)return GSLevelUpdateResultV44::application_gate;
  struct Guard{bool& b;Guard(bool& v):b(v){b=true;}~Guard(){b=false;}} guard(busy_);
  try{if(!body()||failed_)return GSLevelUpdateResultV44::failed;return GSLevelUpdateResultV44::advanced;}
  catch(const std::exception& e){error_=e.what();reject("source provider exception");return GSLevelUpdateResultV44::failed;}
  catch(...){reject("unknown source provider exception");return GSLevelUpdateResultV44::failed;}
 }
};
}
