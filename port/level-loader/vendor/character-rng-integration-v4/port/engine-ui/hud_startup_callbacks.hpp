#pragma once
#include <cstdint>
namespace dh2::ui {
struct HudStartupState48 {
    std::uintptr_t application,savegame,sound,result;
    std::uint32_t sharp_devices,htc_devices,no_igp,reserved;
};
enum class HudStartupOperation:std::uint32_t {
    load_settings=1,update_saved_values=2,get_saved_option=3,
    set_initial_volume=4,get_language=5,set_language=6,
    is_high_performance=7,set_result_bool=8
};
struct HudStartupRequest40 {
    HudStartupOperation operation;std::int32_t argument;
    std::uintptr_t subject;const char* name;
    float values[3];std::uint32_t reserved;
};
struct HudStartupResponse16 {std::int32_t value;std::uint32_t reserved;std::uintptr_t identity;};
struct HudStartupServices16 {
    void* context;
    int (*invoke)(void*,HudStartupState48*,const HudStartupRequest40*,HudStartupResponse16*);
};
struct HudDevicePipeline16 {std::uint32_t exclusions[3],capabilities;};
static_assert(sizeof(void*)==8&&sizeof(HudStartupState48)==48);
static_assert(sizeof(HudStartupRequest40)==40&&sizeof(HudStartupResponse16)==16&&sizeof(HudStartupServices16)==16&&sizeof(HudDevicePipeline16)==16);
}
// Original wrappers ignore script args. Caller retains the genuine Application,
// Savegame/Audio/device and AS result services; no successful backend is invented.
// 0 delivered,-1 malformed entry,-2 required provider/response failure. Earlier
// synchronous effects remain when a later service fails. Providers may mutate
// live state/reenter but must preserve this state/services/receiver lifetime.
extern "C" int dh2_hud_load_settings(dh2::ui::HudStartupState48*,const dh2::ui::HudStartupServices16*) noexcept;
extern "C" int dh2_hud_is_multiplayer_enabled(dh2::ui::HudStartupState48*,const dh2::ui::HudStartupServices16*) noexcept;
// Pure source Device::IsProgrammablePipeline predicate; exclusions are exact
// source byte facts in source read order, capabilities is the genuine virtual
// +0x5c result. 0/1 predicate,-1 malformed. This supplies no hardware producer.
extern "C" int dh2_hud_device_pipeline(const dh2::ui::HudDevicePipeline16*) noexcept;
