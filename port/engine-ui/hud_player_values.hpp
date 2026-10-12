#pragma once
#include <cstdint>
#include <cstddef>
namespace dh2::ui {
// Cached resolved CharacterProperties words, not a recomputation service.
struct HudValuesState24 { const std::int32_t* resolved; std::uint32_t count, reserved; std::uintptr_t render_fx; };
enum class HudValueOperation : std::uint32_t { resolve_clip=1, is_sprite=2, goto_frame=3, set_play_state=4, divide_zero=5 };
enum class HudValueClip : std::uint32_t { hp=0, mp=1, xp=2, distress=3, hurt=4 };
struct HudValueRequest32 { HudValueOperation operation; HudValueClip index; std::int32_t value, other; std::uintptr_t render_fx, clip; };
struct HudValueResponse16 { std::uintptr_t clip; std::int32_t value; std::uint32_t reserved; };
struct HudValueServices16 { void* context; int (*invoke)(void*,HudValuesState24*,const HudValueRequest32*,HudValueResponse16*); };
// Service result 1 means delivered, 0 means required provider failure. Clip
// lookups may deliver null; is_sprite returns the source virtual type-2 result.
// goto_frame and set_play_state are mandatory backend operations, not mocks.
// divide_zero supplies the unresolved original integer-runtime handler result.
// No caller output buffer: callback frames are the source-observable output.
const char* hud_value_clip_path(HudValueClip) noexcept;
static_assert(sizeof(HudValuesState24)==24 && sizeof(HudValueRequest32)==32 && sizeof(HudValueResponse16)==16 && sizeof(HudValueServices16)==16);
}
extern "C" {
// Bounded FastUpdate 41e0f4..41e208: excludes potion text prefix and skill tail.
// 0 success, -1 malformed (before callbacks), -2 required delivery failed.
int dh2_ui_hud_player_values(dh2::ui::HudValuesState24*,const dh2::ui::HudValueServices16*) noexcept;
// Exact RenderFX::GotoFrame null/type/virtual order, with caller-owned backend.
int dh2_ui_hud_goto_frame(dh2::ui::HudValuesState24*,dh2::ui::HudValueClip,std::uintptr_t render_fx,std::uintptr_t clip,std::int32_t frame,std::uint32_t play,const dh2::ui::HudValueServices16*) noexcept;
}
