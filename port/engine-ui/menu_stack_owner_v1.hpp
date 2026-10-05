#pragma once
#include "menu_stack_v1.hpp"
#include <memory>
#include <string>
namespace dh2::ui {
// Owns projection records, copied names and all stack storage. Movie/character
// identities and required services remain borrowed for the complete call.
class MenuStackOwnerV1 {
 struct Impl;std::unique_ptr<Impl> p_;
public:
 explicit MenuStackOwnerV1(std::uint32_t maximum_occurrences);
 ~MenuStackOwnerV1();MenuStackOwnerV1(const MenuStackOwnerV1&)=delete;
 MenuStackOwnerV1& operator=(const MenuStackOwnerV1&)=delete;
 MenuStackOwnerV1(MenuStackOwnerV1&&)=delete;MenuStackOwnerV1&operator=(MenuStackOwnerV1&&)=delete;
 bool add_render(std::uintptr_t,std::uint32_t flags,MenuStackCharacterV1* root,MenuStackCharacterV1* controller_focus,std::string&);
 bool add_menu(std::uintptr_t,std::uintptr_t render,const std::string&name,MenuStackCharacterV1*,std::uint32_t valid_menu,std::uint32_t visible,std::string&);
 bool seal(std::uintptr_t hud_root,std::uintptr_t base_render,MenuStackGlobalsV1&,std::string&);
 MenuStackV1* view()noexcept;
 MenuStackMenuV1* menu(const char*)noexcept;
 int push(const char*,const MenuStackServicesV1&);
 int pop(const char*,const MenuStackServicesV1&);
 int pop_current(bool all,const MenuStackServicesV1&);
 int pop_above(const char*,const MenuStackServicesV1&);
 int pop_named_direct(const char*,const MenuStackServicesV1&);
 int hide_all(const MenuStackServicesV1&);
};
}
