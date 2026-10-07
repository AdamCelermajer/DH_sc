#pragma once
#include "swf_cursor_input.hpp"
#include <memory>
#include <string>
namespace gameswf {struct character;}
namespace dh2::ui {
// The actual RenderFX constructor's four controllers, also when no pointer
// input adapter has been connected yet. Facade and connected V1/V2 adapters
// share THIS storage; there is no shadow four-slot snapshot/registry.
class SwfControllerStorageV91 {
 struct Impl;std::unique_ptr<Impl> p_;
public:
 SwfControllerStorageV91();~SwfControllerStorageV91();
 SwfControllerStorageV91(const SwfControllerStorageV91&)=delete;
 SwfControllerStorageV91& operator=(const SwfControllerStorageV91&)=delete;
 SwfInputState288& fields()noexcept;
 void retain(gameswf::character*);
 void drop(gameswf::character*);
 // Original7a84c4 -> smart_ptr.set_ref0: drop-old THEN store-zero,
 // focus/hover/graphic/pending/pressed, no AS/events/cursor/enabled writes.
 bool reset_controller(std::uint32_t,std::string&);
 // Report invariant failures while the owner is still retained; prefix stays committed.
 bool release_all(std::string&);
};
}
