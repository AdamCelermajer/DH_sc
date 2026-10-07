#pragma once
#include <menu_stack_v1.hpp>
#include <functional>
#include <memory>
#include <string>
namespace dh2::ui {
struct MenuFXPopAllServicesV114 {
 // Pins the SAME captured render, source movie, and projection storage.
 std::shared_ptr<void> owner;
 std::function<bool(std::string&)> current;
 // Actual top receiver virtual10. Capture its existing storage before delivery.
 std::function<bool(MenuStackMenuV1&,std::string&)> hide_virtual10;
 // Only reached if a nested Hide makes source count exceed its capacity.
 // Resize the SAME states array and publish its pointer/capacity; no new stack.
 std::function<bool(MenuStackRenderV1&,std::uint32_t,std::string&)> resize_states;
};
// MenuFX::PopAll 0x7abb20, RenderFX::SetContext 0x7a7ee8.
// Caller supplies actual RenderFX.root3c->character10 as render.root.
bool menu_fx_pop_all_v114(MenuStackRenderV1&,const MenuFXPopAllServicesV114&,std::string&);
}
