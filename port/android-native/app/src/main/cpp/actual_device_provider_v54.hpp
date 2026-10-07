#pragma once
#include "resource_budget_v37.hpp"
#include "swf_menu_device_v1.hpp"
#include "hud_startup_callbacks.hpp"
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {
// A witness for the actual current context, never a capability/cache owner.
struct CurrentGlesContextV54 {
 std::uintptr_t display{},context{},thread{};
 std::int32_t client_version{};
 bool opengl_es_api{};
};
class ActualDeviceProviderV54 {
 std::weak_ptr<resources::ContextResourceBudgetV37> owner_;
 CurrentGlesContextV54 bound_{};std::uint64_t generation_{};
public:
 using Current=std::function<bool(CurrentGlesContextV54&,std::string&)>;
 using Flags=std::function<bool(ui::MenuDeviceFactsV1&,std::string&)>;
 void context_lost() noexcept {owner_.reset();bound_={};generation_=0;}
 bool bind_current(const std::shared_ptr<resources::ContextResourceBudgetV37>& owner,const Current& current,std::string& error){
  context_lost();if(!owner||!current){error="Required actual GL Device owner";return false;}
  const auto before=owner->context_state_v41();CurrentGlesContextV54 c;
  if(!current(c,error))return false;
  const auto after=owner->context_state_v41();
  if(!before.ready||!after.ready||!before.generation||before.generation!=after.generation||!c.context||!c.display||!c.thread||!c.opengl_es_api||c.client_version<2){error="Required live selected GLES2 renderer context";return false;}
  owner_=owner;bound_=c;generation_=after.generation;error.clear();return true;
 }
 bool renderer_type(const std::shared_ptr<resources::ContextResourceBudgetV37>& actual,const Current& current,std::uint32_t& out,std::string& error)const{
  const auto owner=owner_.lock();if(!owner||owner!=actual||!current){error="Required same retained GL Device owner";return false;}
  const auto before=owner->context_state_v41();CurrentGlesContextV54 c;
  if(!current(c,error))return false;
  const auto after=owner->context_state_v41();
  if(!before.ready||!after.ready||before.generation!=generation_||after.generation!=generation_||c.context!=bound_.context||c.display!=bound_.display||c.thread!=bound_.thread||!c.opengl_es_api||c.client_version<2){error="Required current generation/thread GLES2 Device owner";return false;}
  // Original concrete COpenGLES2Driver vtable slot5c ->5aefa4 returns8.
  // Valid only for main's selected GLES2 backend bound above; no flags/caps copy.
  out=8;error.clear();return true;
 }
 bool high_performance(const std::shared_ptr<resources::ContextResourceBudgetV37>& actual,const Current& current,const Flags& flags,bool& out,std::string& error)const{
  if(!flags){error="Required actual live Device flag owner";return false;}
  ui::MenuDeviceFactsV1 f;if(!flags(f,error))return false;
  // Original source exclusions short circuit BEFORE reading the renderer.
  std::uint32_t type=0;
  if(!f.sharp&&!f.htc&&!f.multiplayer_mode&&!renderer_type(actual,current,type,error))return false;
  const ui::HudDevicePipeline16 p{{f.sharp,f.htc,f.multiplayer_mode},type};
  const int result=dh2_hud_device_pipeline(&p);
  if(result<0){error="Actual source Device pipeline rejected";return false;}
  out=result!=0;error.clear();return true;
 }
};
}
