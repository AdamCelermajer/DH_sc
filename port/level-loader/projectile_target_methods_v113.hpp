#pragma once
#include "canonical_projectile_v99.hpp"
#include <array>
namespace dh2::world {
//Pure native member methods. Main resolves a genuine GameObject identity before
//calling SetTarget; NULL is valid. No Handle lookup/C1/callback/pose update.
bool projectile_set_target_v113(CanonicalProjectileV99&,std::uintptr_t actual_target,std::string&);
bool projectile_get_target_v113(CanonicalProjectileV99&,std::uintptr_t&,std::string&);
bool projectile_get_source_v113(CanonicalProjectileV99&,std::uintptr_t&,std::string&);
//Laser's distinct actual source3dc: never silently replace inherited source380.
bool laser_get_source_v113(CanonicalProjectileV99&,std::uintptr_t&,std::string&);
bool projectile_get_speed_v113(CanonicalProjectileV99&,float&,std::string&);
bool projectile_get_heading_angle_v113(CanonicalProjectileV99&,float&,std::string&);
//Live word-backed XYZ loans over SAME scalar fields, no new vector storage.
//388..390 =source target-position captured by SetInfo;394..39c =previous
//target-position captured by Update. Neither is a normalized direction.
struct ProjectilePositionWordsV113 {
 const std::uint32_t *x{},*y{},*z{};
 bool read(std::array<float,3>&,std::string&)const;
};
bool projectile_source_position_words_v113(CanonicalProjectileV99&,ProjectilePositionWordsV113&,std::string&);
bool projectile_previous_position_words_v113(CanonicalProjectileV99&,ProjectilePositionWordsV113&,std::string&);
}