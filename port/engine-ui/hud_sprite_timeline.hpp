#pragma once
#include <cstdint>
#include <cstddef>
namespace dh2::ui {
inline constexpr std::uint32_t hud_sprite_timeline_version=1;
struct HudActionBuffer16 { std::uintptr_t* values; std::uint32_t count,capacity; };
struct HudSpriteState64 {
 std::int32_t current_frame,play_state,stream_sound_id;std::uint32_t reserved;
 std::uintptr_t clip,definition;HudActionBuffer16 pending,goto_actions;
};
enum class HudSpriteOperation : std::uint32_t { frame_count=1,reverse_tags=2,forward_tags=3,notify_advance=4,sound_handler=5,pause_sound=6 };
struct HudSpriteRequest32 { HudSpriteOperation operation;std::uint32_t reserved;std::int32_t frame,state_only;std::uintptr_t clip,sound; };
struct HudSpriteResponse16 {std::uintptr_t identity;std::int32_t value;std::uint32_t reserved;};
struct HudSpriteServices16 {void* context;int (*invoke)(void*,HudSpriteState64*,const HudSpriteRequest32*,HudSpriteResponse16*);};
// Buffers borrow source action identities through the movie lifetime. Distinct
// storage; counts <= capacity <=4096. Providers may synchronously mutate live
// fields, but must preserve valid/coherent storage and object lifetime.
static_assert(sizeof(HudSpriteState64)==64&&sizeof(HudSpriteRequest32)==32&&sizeof(HudSpriteResponse16)==16&&sizeof(HudSpriteServices16)==16);
}
extern "C" {
// Source goto_frame scheduling, fixed-capacity domain. 1 changed,0 bad/equal
// (STOP committed),-1 malformed before callbacks,-2 required delivery/capacity.
// Frame-count service must deliver source count0..65536. No tags are fabricated.
int dh2_ui_hud_sprite_goto_v1(dh2::ui::HudSpriteState64*,std::int32_t,const dh2::ui::HudSpriteServices16*) noexcept;
// Source set_play_state sound/old-state/store/notify order. 0 success; -1/-2.
int dh2_ui_hud_sprite_play_v1(dh2::ui::HudSpriteState64*,std::int32_t,const dh2::ui::HudSpriteServices16*) noexcept;
}
