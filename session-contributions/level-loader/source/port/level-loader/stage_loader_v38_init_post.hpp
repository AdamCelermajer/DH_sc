#pragma once
#include "lifecycle_v36.hpp"
#include <memory>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>
namespace dh2::loader {
template<class Manager> struct CanonicalInitPostServicesV38 {
 using Actor=typename Manager::SourceActorBorrowV38;
 using Handle=std::decay_t<decltype(*std::declval<Actor>().shared_handle)>;
 // The real Module receiver's LoadModule: chosen MGP then chosen MVP,
 // through CanonicalModuleFilesV1. Must finish before generic InitPost.
 std::function<LifecycleStepV36(std::uintptr_t,std::string&)> load_module;
 std::function<bool(const Actor*,Handle&,std::string&)> make_handle;
 std::function<bool(Handle&,bool,const Actor*&,std::string&)> resolve;
 std::function<bool(const Actor&,std::string&)> init_post;
 std::function<bool(const Actor&,bool,std::string&)> test_enable_condition;
 std::function<bool(const Actor&,std::string&,std::string&)> type_name;
 std::function<bool(const Actor&,bool&,std::string&)> is_updatable;
 std::function<bool(const Actor&,std::string&)> room_init_object_list;
 std::function<bool(const Actor&,std::uintptr_t&,std::uint8_t&,std::uintptr_t&,std::uint8_t&,std::string&)> membership_fields;
 // Actual SAME manager source lists. No copied output vectors in scheduler.
 std::function<bool(std::uint32_t,const Actor&,std::string&)> append_list;
 std::function<bool(std::uint32_t,std::string&)> clear_list;
};
// Original ObjectManager.InitPost34552c algorithm over the real manager map,
// Module list and phase7c. Scalar cursors are per scheduler as an explicit
// nonreentrancy repair to original function-static cursors. No map/list copies.
template<class Manager> class CanonicalInitPostV38 {
 using Services=CanonicalInitPostServicesV38<Manager>;
 using Actor=typename Services::Actor;using Handle=typename Services::Handle;
 std::shared_ptr<void> manager_pin_;Manager& manager_;Services services_;
 std::size_t module_cursor_{};std::int32_t key_{};bool end_{true},failed_{},finished_{},busy_{};
 std::string error_;
 LifecycleStepV36 fail(const char* missing){failed_=true;if(error_.empty())error_=std::string("Required actual InitPost provider: ")+missing;return LifecycleStepV36::failed;}
 template<class F,class... A> bool call(const F& fn,A&&... args){return fn&&fn(std::forward<A>(args)...,error_)&&!failed_;}
 void reset_cursor(){const Actor* ignored{};end_=!manager_.source_ordered_begin_v38(key_,ignored);}
 bool advance_cursor(){const Actor* ignored{};end_=!manager_.source_ordered_next_v38(key_,key_,ignored);return true;}
public:
 CanonicalInitPostV38(std::shared_ptr<void> manager_pin,Manager& manager,Services services):manager_pin_(std::move(manager_pin)),manager_(manager),services_(std::move(services)){if(manager_.source_init_phase7c_v38()!=0)fail("fresh actual phase7c/cursor provenance");}
 CanonicalInitPostV38(const CanonicalInitPostV38&)=delete;
 const std::string& error()const noexcept{return error_;}
 LifecycleStepV36 step(){
  if(failed_)return LifecycleStepV36::failed;if(finished_)return LifecycleStepV36::complete;
  if(!manager_pin_)return fail("same-manager lease");if(busy_)return fail("nonreentrant owning-runtime access");
  struct Guard{bool& b;Guard(bool& v):b(v){b=true;}~Guard(){b=false;}} guard(busy_);
  auto& phase=manager_.source_init_phase7c_v38();
  if(phase==0){module_cursor_=0;reset_cursor();phase=1;}
  if(module_cursor_==manager_.modules().size()){
   if(phase==1){phase=2;reset_cursor();++phase;}
  }else if(phase==1){
   if(!services_.load_module)return fail("Module.LoadModule");
   const auto result=services_.load_module(manager_.modules()[module_cursor_],error_);
   if(failed_)return LifecycleStepV36::failed;
   if(result==LifecycleStepV36::pending)return result;
   if(result!=LifecycleStepV36::complete)return fail("Module.LoadModule");
   if(failed_)return LifecycleStepV36::failed;++module_cursor_;
  }
  if(end_){
   ++phase;if(phase!=4){finished_=true;return LifecycleStepV36::complete;}
   reset_cursor();for(auto offset:{0x2cu,0x44u,0x34u})if(!call(services_.clear_list,offset))return fail("clear actual lists2c/44/34");
   return LifecycleStepV36::pending;
  }
  const Actor* object{};if(!manager_.source_ordered_entry_v38(key_,object))return fail("current actual registry node erased");
  if(phase==3){
   Handle handle{};const Actor* resolved{};
   if(!call(services_.make_handle,object,handle))return fail("ObjectHandle constructor");
   if(!call(services_.resolve,handle,false,resolved))return fail("ObjectHandle.GetObject(false)");
   if(resolved){
    if(!call(services_.resolve,handle,true,resolved)||!resolved)return fail("asserted ObjectHandle before InitPost");
    if(!call(services_.init_post,*resolved))return fail("virtual InitPost");
    if(!call(services_.resolve,handle,true,resolved)||!resolved)return fail("asserted ObjectHandle after InitPost");
    if(!call(services_.test_enable_condition,*resolved,false))return fail("TestEnableCondition(false)");
   }
  }else if(phase==4&&object){
   std::string name;if(!call(services_.type_name,*object,name))return fail("actual class type name");
   for(char& ch:name)if(ch>='A'&&ch<='Z')ch=char(ch+'a'-'A');
   if(name=="roomzone"){
    if(!call(services_.append_list,0x24,*object))return fail("append actual room list24");
    if(!call(services_.room_init_object_list,*object))return fail("RoomZone.InitObjectList");
   }else {
    bool updatable{};if(!call(services_.is_updatable,*object,updatable))return fail("virtual IsUpdatable");
    if(updatable&&(!call(services_.append_list,0x2c,*object)))return fail("append actual update list2c");
   }
   std::uintptr_t a8{},cc{};std::uint8_t ac{},d0{};
   if(!call(services_.membership_fields,*object,a8,ac,cc,d0))return fail("actual membership fields a8/ac/cc/d0");
   if((!ac&&a8)||(!d0&&cc))if(!call(services_.append_list,0x44,*object))return fail("append actual membership list44");
  }
  if(failed_)return LifecycleStepV36::failed;advance_cursor();return LifecycleStepV36::pending;
 }
};
}
