#pragma once
#include <cstdint>
namespace dh2::ui {
// Native MenuBase registration projection. It borrows real native identity
// and renderer; provider callbacks may update renderer synchronously.
struct MenuManagerPushEntry24V1 {std::uintptr_t identity,renderer;const char* name;};
struct MenuManagerPushState32V1 {
 MenuManagerPushEntry24V1* const* entries;std::uint32_t count,reserved;
 std::uintptr_t manager,multi_manager;
};
enum class MenuManagerPushOperationV1:std::uint32_t {
 is_valid=1,is_in_stack=2,reset_touch=3,process_touch_events=4,multi_push=5,
 debug_load=6,debug_get=7,hud_root=8,register_listener=9
};
struct MenuManagerPushRequest32V1 {
 MenuManagerPushOperationV1 operation;std::uint32_t reserved;
 MenuManagerPushEntry24V1* menu;const char* text;std::uintptr_t receiver;
};
struct MenuManagerPushResponse16V1 {std::uint32_t value,reserved;std::uintptr_t identity;};
struct MenuManagerPushServices16V1 {
 void* context;
 // Return 1 for actual synchronous delivery; 0 rejects a required backend.
 // Must retain entries/receivers and must not throw across the C ABI.
 int (*invoke)(void*,MenuManagerPushState32V1*,const MenuManagerPushRequest32V1*,MenuManagerPushResponse16V1*);
};
static_assert(sizeof(MenuManagerPushEntry24V1)==24&&sizeof(MenuManagerPushState32V1)==32&&sizeof(MenuManagerPushRequest32V1)==32&&sizeof(MenuManagerPushResponse16V1)==16&&sizeof(MenuManagerPushServices16V1)==16);
}
// Source 0x42d1f0 and 0x4317e8. Lookup preserves registration order and exact
// case; a real name miss or null entry is a no-op. Push retains source call
// order and rereads renderer after synchronous deliveries. It requires the
// real MultiMenuManager implementation; it does not fabricate a generic stack.
// 0 delivered, -1 malformed host binding, -2 missing/rejected service.
extern "C" dh2::ui::MenuManagerPushEntry24V1* dh2_menu_manager_find_v1(const dh2::ui::MenuManagerPushState32V1*,const char*) noexcept;
extern "C" int dh2_menu_manager_push_v1(dh2::ui::MenuManagerPushState32V1*,dh2::ui::MenuManagerPushEntry24V1*,const dh2::ui::MenuManagerPushServices16V1*) noexcept;
