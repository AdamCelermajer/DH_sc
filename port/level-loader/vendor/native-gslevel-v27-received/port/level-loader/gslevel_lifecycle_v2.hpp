#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <utility>

namespace dh2::loader {
// GSLevel::Ctor386190 / Dtor3860bc. The template lets production borrow the
// existing CanonicalLevelContextV1 and its existing s_level slot directly.
// It does not introduce another Application-current-Level global.
struct GSLevelArgumentsV2 {
 std::string name18;
 std::int32_t level1c{};
 std::uint32_t word20{},word24{},word28{};
 std::uint8_t byte2c{},byte2d{};
 std::int32_t word30{},word40{};
};
template<class Level> struct GSLevelFieldsV2 {
 GSLevelArgumentsV2 arguments;
 std::shared_ptr<Level> level34;
 std::uint32_t loading38{};
 std::uint8_t active3c{};
};
template<class Level> struct GSLevelServicesV2 {
 // Allocation is a native storage lease. construct must return this SAME
 // allocation; it may not replace it with a second candidate/Level authority.
 std::function<bool(std::string&)> flush_animation_sets;
 std::function<bool(std::shared_ptr<void>&,std::string&)> allocate_level;
 std::function<bool(const std::shared_ptr<void>&,const GSLevelArgumentsV2&,
                    std::shared_ptr<Level>&,std::string&)> construct_level;
 std::function<bool(const char*,std::uintptr_t&,std::string&)> get_menu;
 std::function<bool(std::uint8_t&,std::string&)> online_byte5;
 std::function<bool(std::int32_t&,std::string&)> online_state34;
 std::function<bool(std::uintptr_t,bool&,std::string&)> overlay_contains_menu;
 std::function<bool(std::uintptr_t,std::string&)> push_menu;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> menu_render_fx;
 std::function<bool(std::uintptr_t,std::string&)> check_menu_weak_proxy;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> menu_character;
 std::function<bool(std::uintptr_t,std::uintptr_t,const char*,std::string&)> invoke_as_no_arguments;
 std::function<bool(const std::shared_ptr<Level>&,std::string&)> unload_level;
 std::function<bool(std::uintptr_t,std::string&)> menu_virtual10;
 std::function<bool(const std::shared_ptr<Level>&,std::string&)> destroy_level;
};

enum class GSLevelLifecycleStageV2 {
 idle,flush,allocate,construct,published,menu_lookup,online,overlay,push,
 render_fx,weak_proxy,character,callback,constructed,unload,close_menu,
 destroy,unpublished,destroyed,failed
};

// One retained GSLevel instance. All operations run on its owning runtime
// thread. Required provider failures preserve the completed source prefix;
// repeated calls never replay allocation, constructors, menu pushes or unload.
// A completed outer Ctor does NOT imply Level::Init or gameplay readiness.
template<class Level> class GSLevelLifecycleV2 {
 GSLevelFieldsV2<Level>& state_;
 std::shared_ptr<Level>& current_; // SAME actual GSLevel::s_level storage.
 GSLevelServicesV2<Level> services_;
 GSLevelLifecycleStageV2 stage_{GSLevelLifecycleStageV2::idle};
 GSLevelLifecycleStageV2 failed_at_{GSLevelLifecycleStageV2::idle};
 bool ctor_attempted_{},dtor_attempted_{},busy_{};
 std::string error_;
 struct Guard { bool& busy;explicit Guard(bool& b):busy(b){busy=true;}~Guard(){busy=false;} };
 template<class F,class... Args> bool call(GSLevelLifecycleStageV2 stage,
                                         const F& fn,Args&&... args){
  stage_=stage;
  if(fn&&fn(std::forward<Args>(args)...,error_)){error_.clear();return true;}
  if(error_.empty())error_="Required actual GSLevel lifecycle provider at stage "+std::to_string(unsigned(stage));
  failed_at_=stage;stage_=GSLevelLifecycleStageV2::failed;return false;
 }
public:
 GSLevelLifecycleV2(GSLevelFieldsV2<Level>& state,std::shared_ptr<Level>& actual_global,
                   GSLevelServicesV2<Level> services):
  state_(state),current_(actual_global),services_(std::move(services)){}
 GSLevelLifecycleV2(const GSLevelLifecycleV2&)=delete;
 GSLevelLifecycleV2& operator=(const GSLevelLifecycleV2&)=delete;
 bool construct(){
  if(busy_||ctor_attempted_){error_="GSLevel constructor already attempted or reentered";return false;}
  ctor_attempted_=true;Guard guard(busy_);error_.clear();
  if(!call(GSLevelLifecycleStageV2::flush,services_.flush_animation_sets))return false;
  // Source reads name18 before allocation; the other argument fields after it.
  const std::string selected_name=state_.arguments.name18;
  std::shared_ptr<void> allocation;
  if(!call(GSLevelLifecycleStageV2::allocate,services_.allocate_level,allocation))return false;
  if(!allocation){error_="Required actual allocated Level storage";failed_at_=stage_;stage_=GSLevelLifecycleStageV2::failed;return false;}
  auto arguments=state_.arguments;arguments.name18=selected_name;
  std::shared_ptr<Level> level;
  if(!call(GSLevelLifecycleStageV2::construct,services_.construct_level,allocation,arguments,level))return false;
  if(!level||level.get()!=allocation.get()||
     level.owner_before(allocation)||allocation.owner_before(level)){
   error_="Level constructor returned a different allocation";failed_at_=stage_;stage_=GSLevelLifecycleStageV2::failed;return false;
  }
  state_.loading38=1;state_.level34=level;current_=level;state_.active3c=1;
  stage_=GSLevelLifecycleStageV2::published;
  std::uintptr_t menu{};
  if(!call(GSLevelLifecycleStageV2::menu_lookup,services_.get_menu,"menu_Loading",menu))return false;
  std::uint8_t online{};
  if(!call(GSLevelLifecycleStageV2::online,services_.online_byte5,online))return false;
  if(online){
   std::int32_t mode{};
   if(!call(GSLevelLifecycleStageV2::online,services_.online_state34,mode))return false;
   if(mode==3){
    bool contained{};
    if(!call(GSLevelLifecycleStageV2::overlay,services_.overlay_contains_menu,menu,contained))return false;
    if(contained){stage_=GSLevelLifecycleStageV2::constructed;return true;}
   }
  }
  if(menu){
   if(!call(GSLevelLifecycleStageV2::push,services_.push_menu,menu))return false;
   std::uintptr_t render_fx{},character{};
   if(!call(GSLevelLifecycleStageV2::render_fx,services_.menu_render_fx,menu,render_fx))return false;
   if(!call(GSLevelLifecycleStageV2::weak_proxy,services_.check_menu_weak_proxy,menu))return false;
   if(!call(GSLevelLifecycleStageV2::character,services_.menu_character,menu,character))return false;
   if(!call(GSLevelLifecycleStageV2::callback,services_.invoke_as_no_arguments,render_fx,character,"onProgress"))return false;
  }
  stage_=GSLevelLifecycleStageV2::constructed;return true;
 }
 bool destroy(){
  if(busy_||dtor_attempted_){error_="GSLevel destructor already attempted or reentered";return false;}
  dtor_attempted_=true;Guard guard(busy_);error_.clear();
  if(state_.level34){
   // Pins protect synchronous callbacks, but each source reread still uses
   // the current SAME field34, including mutations by Unload/menu close.
   auto level=state_.level34;
   if(!call(GSLevelLifecycleStageV2::unload,services_.unload_level,level))return false;
   std::uintptr_t menu{};
   if(!call(GSLevelLifecycleStageV2::menu_lookup,services_.get_menu,"menu_HUD_0",menu))return false;
   if(menu&&!call(GSLevelLifecycleStageV2::close_menu,services_.menu_virtual10,menu))return false;
   if(state_.level34){
    level=state_.level34;
    if(!call(GSLevelLifecycleStageV2::destroy,services_.destroy_level,level))return false;
    state_.level34.reset();
   }
  }
  // Original Dtor clears s_level even when field34 is null or callbacks
  // changed the global to another receiver. Do not add identity guards.
  current_.reset();stage_=GSLevelLifecycleStageV2::destroyed;return true;
 }
 GSLevelLifecycleStageV2 stage()const noexcept{return stage_;}
 GSLevelLifecycleStageV2 failed_at()const noexcept{return failed_at_;}
 const std::string& error()const noexcept{return error_;}
};
}
