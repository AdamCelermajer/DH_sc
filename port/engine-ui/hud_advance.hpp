#pragma once
#include <cstdint>
#include <cstddef>
namespace dh2::ui {
struct HudWeakProxy8 {std::uint32_t references;std::uint8_t alive,reserved[3];};
struct HudAdvanceNode32 {std::uintptr_t identity;HudAdvanceNode32* parent;HudWeakProxy8* proxy;std::uint8_t needs_advance,reserved[7];};
struct HudAdvanceServices16 {void* context;int(*destroy_proxy)(void*,HudWeakProxy8*);};
static_assert(sizeof(HudWeakProxy8)==8&&sizeof(HudAdvanceNode32)==32&&sizeof(HudAdvanceServices16)==16);
}
extern "C" int dh2_ui_hud_notify_v1(dh2::ui::HudAdvanceNode32*,const dh2::ui::HudAdvanceServices16*) noexcept;
