#pragma once
#include <functional>
#include <memory>
#include <string>
#include <cstdint>
namespace dh2::ui {
// Exact source gameswf::s_render_handler query (GOT99844c/BSS9fc994).
// Exposes no vendor renderer type to Android clients; actual NULL is a result.
bool swf_actual_renderer_borrow_v91(std::uintptr_t&,std::string&)noexcept;
struct SwfMenuUnloadServicesV91 {
 std::shared_ptr<void> owner;
 // Actual RenderFX renderer singleton/GOT owner: true null is allowed,
 // unavailable getter is not inferred as renderer-null.
 std::function<bool(std::uintptr_t&,std::string&)> renderer;
 std::function<bool(std::uintptr_t,std::string&)> renderer_virtual_a4;
 // Same MenuStackOwner render storage, not a new list or Hide/Pop.
 std::function<bool(std::string&)> clear_catalog_104;
 std::function<bool(std::string&)> clear_active_states_114;
 // Publish actual source low24 flags=ffffff and context=null into the
 // existing RenderFX projection; upper byte is preserved by its owner.
 std::function<bool(std::string&)> clear_render_context_and_flags;
};
}
