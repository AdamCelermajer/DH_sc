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
 bool publish_source_c1_v104(MenuStackGlobalsV1&,std::string&);
 bool publish_base_render_v104(std::uintptr_t,std::string&);
 // Source MenuFX.RegisterState may append after initial PostLoad. These
 // successors preserve live stack occurrences and republish directory views;
 // the historical initial-registration guards remain unchanged.
 bool register_render_live_v27(std::uintptr_t,std::uint32_t,MenuStackCharacterV1*,MenuStackCharacterV1*,std::string&);
 bool register_menu_live_v27(std::uintptr_t,std::uintptr_t,const std::string&,MenuStackCharacterV1*,std::uint32_t,std::uint32_t,std::string&);
  MenuStackV1* view()noexcept;
  MenuStackRenderV1* render(std::uintptr_t)noexcept;
 MenuStackMenuV1* registered_menu_v27(std::uintptr_t)noexcept;
 std::uint32_t registry_count_v58()const noexcept;
 MenuStackMenuV1* registry_at_v58(std::uint32_t)noexcept;
 // Exact directory erase after original clear-render/deleting callback. This
 // does not Hide/Pop, destroy a movie, or infer MenuBase ownership from storage.
 bool erase_registry_v58(std::uint32_t,std::uintptr_t,std::string&);
 // Source MenuFX::Unload count clears, without registry/receiver deletion.
 bool clear_catalog_104_v91(std::uintptr_t actual_render,std::string&);
 bool clear_active_states_114_v91(std::uintptr_t actual_render,std::string&);
 bool clear_render_fields_v91(std::uintptr_t actual_render,std::string&);
 // MultiMenu array.remove, without MenuBase events or state-array changes.
 bool remove_active_render_v93(std::uint32_t index,std::uintptr_t expected,std::string&);
 // Retire adapter projections after the source resource has unloaded and its
 // primary field is cleared. Other live resources/receivers are untouched.
 bool retire_render_resource_v93(std::uintptr_t,std::string&);
 bool publish_hud_render_v93(std::uintptr_t,std::string&);
 MenuStackMenuV1* menu(const char*)noexcept;
 //Pins existing adapter storage for a previously captured source receiver.
 //Directory removal/renderer retirement still execute their real mutations.
 bool capture_storage_v101(MenuStackMenuV1*,std::shared_ptr<void>& menu_storage,
                           std::shared_ptr<void>& render_storage,std::string&);
 //Loans the real render allocation even when it has no active/catalog menu.
 bool capture_render_storage_v114(MenuStackRenderV1*,std::shared_ptr<void>&,std::string&);
 bool resize_render_states_v114(MenuStackRenderV1&,std::uint32_t,std::string&);
 int push(const char*,const MenuStackServicesV1&);
 int pop(const char*,const MenuStackServicesV1&);
 int pop_current(bool all,const MenuStackServicesV1&);
 int pop_above(const char*,const MenuStackServicesV1&);
 int pop_named_direct(const char*,const MenuStackServicesV1&);
 int hide_all(const MenuStackServicesV1&);
};
}
