#pragma once
#include <cstdint>
#include <functional>
#include <exception>
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
 enum class DtorContinuationV114 {entry,unload,menu_lookup,close_menu,destroy_level,clear_level,clear_global,complete};
 DtorContinuationV114 dtor_continuation_v114_{DtorContinuationV114::entry};
 std::uintptr_t dtor_menu_v114_{};
 std::shared_ptr<Level> dtor_level_v114_; //SAME failed in-flight native receiver pin.
 std::string dtor_reentry_v114_,dtor_first_failure_v114_;
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
 //Native dependency enrollment after the actual C1 creates its receiver graph.
 //It replaces no source owner/current slot and never replays constructor/Dtor.
 bool bind_release_services_v88(
  std::function<bool(const std::shared_ptr<Level>&,std::string&)> unload,
  std::function<bool(const std::shared_ptr<Level>&,std::string&)> destroy,
  std::string& error){
  if(busy_||dtor_attempted_||stage_!=GSLevelLifecycleStageV2::constructed||
     !unload||!destroy||services_.unload_level||services_.destroy_level){
   error="Require SAME constructed GS and once-only unbound original release providers";return false;
  }
  services_.unload_level=std::move(unload);services_.destroy_level=std::move(destroy);error.clear();return true;
 }
 //Native failure recovery may bind actual dependencies after completed child
 //C1 publication but failed outer onProgress. This never changes source stage.
 bool bind_failed_constructor_release_v115(const std::shared_ptr<Level>& child,
  std::function<bool(const std::shared_ptr<Level>&,std::string&)> unload,
  std::function<bool(const std::shared_ptr<Level>&,std::string&)> destroy,std::string& e){
  if(busy_||dtor_attempted_||!ctor_attempted_||stage_!=GSLevelLifecycleStageV2::failed||
     !child||state_.level34!=child||current_!=child||
     child.owner_before(state_.level34)||state_.level34.owner_before(child)||
     child.owner_before(current_)||current_.owner_before(child)||
     !unload||!destroy||services_.unload_level||services_.destroy_level){
   e="Require actual published child C1 and untouched failed GS release providers";return false;
  }
  services_.unload_level=std::move(unload);services_.destroy_level=std::move(destroy);e.clear();return true;
 }
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
  //The SAME original destructor owns its continuation. A failed engine leaf
  //may resume its retained prefix; no completed leaf/slot clear is replayed.
  if(busy_){if(dtor_reentry_v114_.empty())dtor_reentry_v114_="GSLevel destructor reentered";error_=dtor_reentry_v114_;return false;}
  if(dtor_continuation_v114_==DtorContinuationV114::complete){error_.clear();return true;}
  dtor_attempted_=true;Guard guard(busy_);error_.clear();dtor_reentry_v114_.clear();
  auto deliver=[&](GSLevelLifecycleStageV2 reached,const auto& fn,auto&&... args){
   stage_=reached;bool done=false;
   try{if(fn)done=fn(std::forward<decltype(args)>(args)...,error_);}
   catch(const std::exception& ex){error_=ex.what();}
   catch(...){error_="Actual GSLevel destructor provider threw";}
   if(!done){if(error_.empty())error_="Required actual GSLevel destructor provider";if(dtor_first_failure_v114_.empty())dtor_first_failure_v114_=error_;error_=dtor_first_failure_v114_;failed_at_=reached;stage_=GSLevelLifecycleStageV2::failed;}
   return done;
  };
  auto after=[&]{if(dtor_reentry_v114_.empty())return true;if(dtor_first_failure_v114_.empty())dtor_first_failure_v114_=dtor_reentry_v114_;error_=dtor_first_failure_v114_;failed_at_=stage_;stage_=GSLevelLifecycleStageV2::failed;return false;};
  using D=DtorContinuationV114;
  if(dtor_continuation_v114_==D::entry)dtor_continuation_v114_=state_.level34?D::unload:D::clear_global;
  if(dtor_continuation_v114_==D::unload){
   if(!dtor_level_v114_)dtor_level_v114_=state_.level34;
   if(!deliver(GSLevelLifecycleStageV2::unload,services_.unload_level,dtor_level_v114_))return false;
   dtor_level_v114_.reset();dtor_continuation_v114_=D::menu_lookup;if(!after())return false;
  }
  if(dtor_continuation_v114_==D::menu_lookup){
   std::uintptr_t menu{};
   if(!deliver(GSLevelLifecycleStageV2::menu_lookup,services_.get_menu,"menu_HUD_0",menu))return false;
   dtor_menu_v114_=menu;dtor_continuation_v114_=menu?D::close_menu:D::destroy_level;if(!after())return false;
  }
  if(dtor_continuation_v114_==D::close_menu){
   if(!deliver(GSLevelLifecycleStageV2::close_menu,services_.menu_virtual10,dtor_menu_v114_))return false;
   dtor_continuation_v114_=D::destroy_level;if(!after())return false;
  }
  if(dtor_continuation_v114_==D::destroy_level){
   if(dtor_level_v114_||state_.level34){if(!dtor_level_v114_)dtor_level_v114_=state_.level34;
    if(!deliver(GSLevelLifecycleStageV2::destroy,services_.destroy_level,dtor_level_v114_))return false;
    dtor_level_v114_.reset();dtor_continuation_v114_=D::clear_level;if(!after())return false;
   }else dtor_continuation_v114_=D::clear_global;
  }
  if(dtor_continuation_v114_==D::clear_level){state_.level34.reset();dtor_continuation_v114_=D::clear_global;}
  //Original clears s_level even if field34 is NULL or a callback changed it.
  if(dtor_continuation_v114_==D::clear_global){current_.reset();dtor_continuation_v114_=D::complete;}
  stage_=GSLevelLifecycleStageV2::destroyed;error_.clear();return true;
 }
 GSLevelLifecycleStageV2 stage()const noexcept{return stage_;}
 GSLevelLifecycleStageV2 failed_at()const noexcept{return failed_at_;}
 const std::string& error()const noexcept{return error_;}
};
}
