#pragma once
#include "projectile_methods_v112.hpp"
namespace dh2::world {
//Laser/collision leaves extend the SAME common Projectile runtime bag. Main
//owns actual skill, collision/AI, physics, FX, node/material implementations.
//No second table, scene, clock, manager or class receiver is constructed.
struct LaserProjectileServicesV112 {
 //Actual ObjectBase.GetHandle -> ObjectHandle::Character conversion. A
 //genuine non-Character returns identity0; owner pins a positive Character.
 std::function<bool(const ProjectileObjectV112&,std::uintptr_t&,std::shared_ptr<void>&,std::string&)> character_from_handle;
 std::function<bool(std::uintptr_t,const ProjectileObjectV112&,bool&,std::string&)> character_ai_is_enemy;
 //Actual peer position160 loan; Character collision FX does not use GetTargetPosition.
 std::function<bool(const ProjectileObjectV112&,ProjectilePointBorrowV112&,std::string&)> object_position160;
 //Actual VisualFXManager.PlayAnimFXSet(uid,point,NULL,NULL) leaf.
 std::function<bool(std::int32_t,const ProjectilePointV112&,std::uintptr_t,std::uintptr_t,std::string&)> play_anim_fx_set;
 //SAME actual scene node virtuala4 and VisualObject methods. Node loans are
 //resolved by the existing owned visual/resource directory, never base alias.
 std::function<bool(std::uintptr_t,const ProjectilePointV112&,std::string&)> node_set_position;
 std::function<bool(std::uintptr_t,bool,std::string&)> visual_set_visible;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> visual_set_parent;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::shared_ptr<void>&,std::string&)> visual_root8;
 std::function<bool(std::uintptr_t,const ProjectilePointV112&,std::string&)> visual_set_position;
 std::function<bool(std::uintptr_t,std::uint32_t,std::string&)> root_virtual14;
 //Actual embedded PFObject1c8.SetFlying/SetSwimming; no route simulation.
 std::function<bool(CanonicalProjectileV99&,bool,std::string&)> set_flying,set_swimming;
};
//Return transport bool; original OnCollision integer is separately preserved.
bool projectile_on_collision_v112(CanonicalProjectileV99&,std::uintptr_t peer,const std::array<float,2>&,
 ProjectileMethodServicesV112&,LaserProjectileServicesV112&,std::int32_t& source_result,std::string&);
bool laser_projectile_on_collision_v112(CanonicalProjectileV99&,std::uintptr_t peer,const std::array<float,2>&,
 ProjectileMethodServicesV112&,LaserProjectileServicesV112&,std::int32_t& source_result,std::string&);
bool laser_projectile_set_info_v112(CanonicalProjectileV99&,std::int32_t row,std::uintptr_t source,std::uintptr_t target,
 std::uintptr_t check_callback,std::uintptr_t hit_callback,std::uintptr_t userdata,bool from_node,
 ProjectileMethodServicesV112&,LaserProjectileServicesV112&,std::string&);
bool laser_projectile_set_info_angle_v112(CanonicalProjectileV99&,std::int32_t row,std::uintptr_t source,std::uintptr_t target,
 std::uintptr_t check_callback,std::uintptr_t hit_callback,std::uintptr_t userdata,float ignored_angle,
 ProjectileMethodServicesV112&,LaserProjectileServicesV112&,std::string&);
bool laser_projectile_update_v112(CanonicalProjectileV99&,ProjectileMethodServicesV112&,LaserProjectileServicesV112&,std::string&);
bool laser_projectile_get_speed_v112(CanonicalProjectileV99&,float&,std::string&);
}
