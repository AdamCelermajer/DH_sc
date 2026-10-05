#pragma once
#include <cstdint>
#include <string>
namespace gameswf {struct character;}
namespace dh2::ui {
// Presentation successor; original fixed HealthBars enemy source remains intact.
struct EnemyHudPresentationV2 {
 std::uintptr_t identity{};
 std::int32_t raw_hp{},raw_max_hp{};
 bool dead{},on_screen{};
 float screen_pixels[2]{}; // actual surface pixels; caller projects target head
};
struct EnemyHudPresentationServicesV2 {
 void* context{};
 bool(*target)(void*,std::uintptr_t,EnemyHudPresentationV2&,std::string&){};
};
// Desired anchor is native root twips. Uses real clip bounds bottom-centre and
// parent inverse world matrix, preserving clip's authored linear transform.
bool enemy_hud_anchor_v2(gameswf::character*,const float root_twips[2],std::string&,const float visible_root_twips[4]=nullptr);
int enemy_hud_hp_frame_v2(std::int32_t raw_hp,std::int32_t raw_max_hp);
}
