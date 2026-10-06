#pragma once
#include "menu_stack_v1.hpp"
#include <map>
#include <functional>
#include <string>
#include <optional>
namespace dh2::ui {
// Sole source global/manager scalar owner for the authored MenuManager. These
// BSS globals initialize zero in the original ELF; no menu-active policy is
// inferred from Android overlays. Original kernels own subsequent writes.
class AuthoredMenuApplicationFieldsV3 {
 MenuStackGlobalsV1 globals_{};
 std::uint8_t rollover_{},igm_{};
 std::optional<std::uint8_t> application_ec_;
 std::uintptr_t manager60_{};
 std::uint8_t manager_multitouch110_{}; //MenuManagerC1 431d18/C2 431aac
 std::map<std::uintptr_t,bool> listeners_;
public:
 // use_native_drm is a function-result projection, not one of the captured
 // BSS fields. Require its actual platform producer before publishing graph.
 bool globals(const std::function<bool(bool&,std::string&)>& native_drm,
  MenuStackGlobalsV1*&,std::string&);
 std::uint8_t& rollover()noexcept{return rollover_;}
 std::uint8_t& igm_opened()noexcept{return igm_;}
 std::optional<std::uint8_t>& application_ec()noexcept{return application_ec_;}
 std::uintptr_t& manager60()noexcept{return manager60_;}
 std::uint8_t& manager_multitouch110_v4()noexcept{return manager_multitouch110_;}
 const std::map<std::uintptr_t,bool>& listeners()const noexcept{return listeners_;}
 // RegisterListener431750 inserts or changes existing bool totrue. Source
 // UnRegisterListener42e110 delivers Debug first, then changes existing bool
 // tofalse; it neither inserts a missing receiver nor removes a map node.
 bool register_listener(std::uintptr_t,std::string&);
 bool unregister_listener(std::uintptr_t,
  const std::function<bool(std::string&)>& actual_debug,std::string&);
};
}
