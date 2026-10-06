#pragma once
#include <cstdint>
namespace dh2::ui {
// USE_NATIVE_DRM_GAME9f640e is one byte in SHT_NOBITS. Multi.PushMenu,
// GSInit.Update and Level._LoadProcess read it directly. No offline/device
// policy is substituted. This projection owns the actual initial BSS byte;
// no recovered runtime writer is exposed by this bounded owner.
class AuthoredMenuNativeDrmV4 {
 std::uint8_t use_native_drm_game_{};
public:
 bool source_global()const noexcept{return use_native_drm_game_!=0;}
};
}
