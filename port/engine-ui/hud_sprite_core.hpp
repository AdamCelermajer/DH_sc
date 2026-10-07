#pragma once
#include <cstdint>
#include <string>
namespace gameswf {struct character;struct sprite_instance;}
namespace dh2::ui {
// Borrowed only while the retaining movie's core Scope is active. Caller must
// supply the verified hash of the actual loaded HUD bytes, not a file-name hint.
struct HudSpriteCoreBindingV1 {gameswf::sprite_instance* sprite{};const void* definition{};const void* root{};std::int32_t id{},frames{};std::uint32_t version{};};
struct HudSpriteCoreServices {
 void* context{};
 // Required source weak-parent needs-advance notification. Success must mark
 // the actual retained scheduling owner; an unimplemented no-op is failure.
 bool (*notify_advance)(void*,gameswf::sprite_instance*,std::string&){};
 // Required genuine engine sound-handler lookup; null is a delivered source
 // absence. Identity is borrowed through synchronous pause delivery.
 bool (*sound_handler)(void*,std::uintptr_t&,std::string&){};
 bool (*pause_sound)(void*,std::uintptr_t,std::int32_t,bool,std::string&){};
};
bool bind_hud_sprite_v1(gameswf::character*,const char* verified_hud_sha256,HudSpriteCoreBindingV1&,std::string&);
// Actual upstream frame-tag/reverse operations, source scheduling kernel.
// Narrow five original dqhud clip domain; unexpected pending/action/stream
// paths reject explicitly. No generic AS timeline or original tag parity claim.
int hud_core_goto_v1(const HudSpriteCoreBindingV1&,std::int32_t,const HudSpriteCoreServices&,std::string&);
int hud_core_play_v1(const HudSpriteCoreBindingV1&,std::int32_t,const HudSpriteCoreServices&,std::string&);
}
