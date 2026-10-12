#pragma once
#include "swf_event_dispatch.hpp"
#include <cstddef>
#include <cstdint>
namespace dh2::ui {
struct SwfCursor16 {float x,y,rotation;std::int32_t buttons;};
struct SwfCursorSlot64 {
 SwfCursor16 cursor;
 std::uintptr_t focus,hover,graphic,pending,pressed;
 std::uint8_t enabled,padding[7];
};
// Strong fields belong to the graph receiver. Weak root/context identities
// are borrowed from a retained exact movie lease, not an outer reloadable movie.
struct SwfInputState288 {
 SwfCursorSlot64 slots[4];
 std::uintptr_t root,context,native_receiver;
 std::uint32_t flags,reserved;
};
// Live, side-effect-free source field projection. The provider refreshes this
// projection on each request; it must not dispatch AS/events for direct reads.
struct SwfInputCharacter32 {
 const char* name;
 std::uint32_t is_sprite;
 std::uint8_t visible,mouse9c,sprite_ea,padding;
 std::uintptr_t reserved[2];
};
enum class SwfInputOperation:std::uint32_t {
 retain=1,drop=2,character=3,world_matrix=4,local_position=5,
 screen_to_logical=6,publish_raw_cursor=7,notify_mouse_state=8,
 root_movie=9,topmost=10,set_matrix=11,collect_buttons=12,
 play_animation=13,can_handle_event=14,native_event=15,as_method=16,
 play_state=17,advance=18
};
struct SwfInputRequest64 {
 SwfInputOperation operation;
 std::uint32_t index;
 std::uintptr_t character;
 const char* name;
 SwfEvent48* event;
 float values[6];
 std::int32_t integer;
 std::uint32_t reserved;
};
// character request integer:1 name;2 class/spriteEA;4 source mouse9c.
// Providers must not require a mouse9c backend for a name-only source read.
struct SwfInputResponse56 {
 const SwfInputCharacter32* character;
 const std::uintptr_t* characters;
 std::uintptr_t identity;
 float values[6];
 std::int32_t count,result;
};
struct SwfInputServices16 {
 void* context;
 // 1 delivered, 0 required endpoint unavailable. Direct field projections
 // are side-effect-free; actual source method services may synchronously
 // mutate this state/reenter. All borrowed buffers survive the source caller.
 int(*invoke)(void*,SwfInputState288*,const SwfInputRequest64*,SwfInputResponse56*);
};
static_assert(sizeof(SwfCursor16)==16);
static_assert(sizeof(SwfCursorSlot64)==64);
static_assert(sizeof(SwfInputState288)==288);
static_assert(sizeof(SwfInputCharacter32)==32);
static_assert(sizeof(SwfInputRequest64)==64);
static_assert(sizeof(SwfInputResponse56)==56);
static_assert(sizeof(SwfInputServices16)==16);
}
// 0 success, -1 malformed caller/index, -2 reached required service absent.
// Native rejection precedes mutation; service failure preserves the delivered
// source prefix and releases local strong references. Slot assignment follows
// original drop-old -> store-new -> retain-new, including reentry.
extern "C" {
int dh2_ui_swf_set_focus(dh2::ui::SwfInputState288*,std::uintptr_t,std::uint32_t,std::uint32_t*,const dh2::ui::SwfInputServices16*);
int dh2_ui_swf_reset_focus(dh2::ui::SwfInputState288*,std::uint32_t,std::uint32_t*,const dh2::ui::SwfInputServices16*);
int dh2_ui_swf_update_input(dh2::ui::SwfInputState288*,std::int32_t,std::uint32_t,std::uint32_t*,const dh2::ui::SwfInputServices16*);
int dh2_ui_swf_update_cursor(dh2::ui::SwfInputState288*,const dh2::ui::SwfCursor16*,std::uint32_t,std::uint32_t*,const dh2::ui::SwfInputServices16*);
// RenderFX::Update animation/pending-click tail. Advance is the required
// source root service, never stock root advance (which adds a mouse prefix).
int dh2_ui_swf_update_pending(dh2::ui::SwfInputState288*,std::int32_t,std::uint32_t advance_flag,std::uint32_t*,const dh2::ui::SwfInputServices16*);
}
