#pragma once
#include <cstdint>
namespace dh2::ui {
// Direct root::advance(float,bool) fields; movie/player services borrow live
// source identities. Reentry mutates this same storage before source rereads.
struct SwfRootFrame32 {float remainder,frame_time,gc_remaining;std::uint8_t loaded,pad[3];std::uintptr_t movie,player;};
enum class SwfFrameOp:std::uint32_t {engine_mutex=1,listeners_advance=2,random=3,flash_vars=4,init_actions=5,construct=6,movie_advance=7,event_load=8,mark_garbage=9,listeners_alive=10,movie_alive=11,clear_garbage=12};
struct SwfFrameRequest24 {SwfFrameOp operation;float delta;std::uintptr_t receiver;std::uint32_t value,reserved;};
struct SwfFrameServices16 {void*context;int(*invoke)(void*,SwfRootFrame32*,const SwfFrameRequest24*);};
static_assert(sizeof(SwfRootFrame32)==32&&sizeof(SwfFrameRequest24)==24&&sizeof(SwfFrameServices16)==16);
struct SwfFrameActions16 {std::uintptr_t*values;std::uint32_t count,capacity;};
struct SwfSpriteFrame64 {std::uintptr_t sprite,definition;std::int32_t current_frame,play_state;std::uint8_t loaded,visible,need,enter;std::uint32_t reserved;SwfFrameActions16 goto_actions,scratch;};
enum class SwfSpriteOp:std::uint32_t {construct=1,event=2,drag=3,execute_actions=4,frame_count=5,wrap_display_list=6,frame_tags=7,do_actions=8,children_advance=9,warning=10};
struct SwfSpriteRequest32 {SwfSpriteOp operation;float delta;std::int32_t frame,value;const std::uintptr_t*actions;std::uint32_t count,reserved;};
struct SwfSpriteServices16 {void*context;int(*invoke)(void*,SwfSpriteFrame64*,const SwfSpriteRequest32*,std::int32_t*);};
static_assert(sizeof(SwfSpriteFrame64)==64&&sizeof(SwfSpriteRequest32)==32&&sizeof(SwfSpriteServices16)==16);
}
extern "C" {
// 0 success; -1 malformed/unsafe infinite catch-up before effects; -2 reached
// missing provider. Finite positive frame_time is the native caller contract.
int dh2_ui_swf_root_frame(dh2::ui::SwfRootFrame32*,float,std::uint32_t,const dh2::ui::SwfFrameServices16*) noexcept;
// Exact sprite::advance field/order kernel. Source allocator is bounded to
// distinct caller-owned scratch/goto spans with capacity<=4096. No AS shortcut.
int dh2_ui_swf_sprite_frame(dh2::ui::SwfSpriteFrame64*,float,const dh2::ui::SwfSpriteServices16*) noexcept;
}
