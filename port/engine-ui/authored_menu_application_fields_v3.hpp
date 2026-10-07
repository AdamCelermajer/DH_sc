#pragma once
#include "menu_stack_v1.hpp"
#include <map>
#include <functional>
#include <string>
#include <optional>
#include <memory>
#include <vector>
namespace dh2::ui {
// Sole source global/manager scalar owner for the authored MenuManager. These
// BSS globals initialize zero in the original ELF; no menu-active policy is
// inferred from Android overlays. Original kernels own subsequent writes.
class AuthoredMenuApplicationFieldsV3 {
 MenuStackGlobalsV1 globals_{};
 std::uint8_t rollover_{},igm_{};
 mutable std::optional<std::uint8_t> application_ec_;
 std::shared_ptr<void> application_owner_v58_;
 std::uint8_t* application_ec_v58_{};
 std::uintptr_t manager60_{};
 std::uint8_t manager_multitouch110_{}; //MenuManagerC1 431d18/C2 431aac
 std::int32_t current_event_consumed_ac_{};std::uintptr_t current_event_b4_{};
 std::map<std::uintptr_t,bool> listeners_;
 //MenuManager C1 431c38 initializes its OWN legacy vector64/68/6c empty.
 //This is distinct from MultiMenuManager's active RenderFX/state arrays.
 std::vector<MenuStackMenuV1*> source_menu_stack64_v119_;
 std::uint8_t listener_cleanup88_{}; // MenuManager C1 431cc8 stores0.
public:
 // use_native_drm is a function-result projection, not one of the captured
 // BSS fields. Require its actual platform producer before publishing graph.
 bool globals(const std::function<bool(bool&,std::string&)>& native_drm,
  MenuStackGlobalsV1*&,std::string&);
 std::uint8_t& rollover()noexcept{return rollover_;}
 std::uint8_t& igm_opened()noexcept{return igm_;}
 const std::optional<std::uint8_t>& application_ec()const noexcept{
  if(application_ec_v58_)application_ec_=*application_ec_v58_;
  return application_ec_;
 }
 // Native GS.Update and authored Show/Hide must read/write the SAME actual
 // Application+ec byte. Binding preserves its current value; it is not a
 // menu-active initializer. The retained lease covers every subsequent call.
 bool bind_application_ec_v58(std::shared_ptr<void>,std::uint8_t*,std::string&);
 void store_application_ec_v58(std::uint8_t)noexcept;
 std::uintptr_t& manager60()noexcept{return manager60_;}
 std::uint8_t& manager_multitouch110_v4()noexcept{return manager_multitouch110_;}
 std::int32_t& source_event_consumed_ac_v120()noexcept{return current_event_consumed_ac_;}
 std::uintptr_t& source_current_event_b4_v120()noexcept{return current_event_b4_;}
 void source_consume_event_v120()noexcept{current_event_consumed_ac_=1;} //whole42cc34
 const std::map<std::uintptr_t,bool>& listeners()const noexcept{return listeners_;}
 std::vector<MenuStackMenuV1*>& source_menu_stack64_v119()noexcept{return source_menu_stack64_v119_;}
 std::uint8_t listener_cleanup88_v62()const noexcept{return listener_cleanup88_;}
 // Only the tail of genuine MenuManager.Reset42d7c8 produces this1. A
 // caller must finish its actual render/menu reset prefix before this store.
 void source_reset_listener_tail_v62()noexcept{listener_cleanup88_=1;}
 // Update42ec90/42edec clears ALL source nodes when88 is set, then zeros88.
 // UnRegisterListener does not produce88 and remains unchanged.
 void source_update_listener_cleanup_v62()noexcept{if(listener_cleanup88_){listeners_.clear();listener_cleanup88_=0;}}
 // RegisterListener431750 inserts or changes existing bool totrue. Source
 // UnRegisterListener42e110 delivers Debug first, then changes existing bool
 // tofalse; it neither inserts a missing receiver nor removes a map node.
 bool register_listener(std::uintptr_t,std::string&);
 bool unregister_listener(std::uintptr_t,
  const std::function<bool(std::string&)>& actual_debug,std::string&);
};
}
