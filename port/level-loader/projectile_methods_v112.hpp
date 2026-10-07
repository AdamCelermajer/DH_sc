#pragma once
#include "canonical_projectile_v99.hpp"
#include <functional>
namespace dh2::world {
using ProjectilePointV112=std::array<float,3>;
struct ProjectileTableRowV112 {
 std::shared_ptr<void> owner; //SAME immutable process Arrays row, never a decode.
 std::function<bool(std::uint32_t,std::uint32_t&,std::string&)> word;
 std::function<bool(std::uint32_t,std::uint8_t&,std::string&)> byte;
};
struct ProjectileObjectV112 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 const std::uintptr_t* visual2d8{};const float* rotation16c{};
};
struct ProjectilePointBorrowV112 {std::shared_ptr<void> owner;const float* xyz{};};
struct ProjectileApplicationV112 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uint32_t&,std::string&)> get_dt; //SAME cached App; call twice when original does.
};
struct ProjectilePhysicalPrefixV112 {
 std::shared_ptr<void> backing,receiver;std::uintptr_t identity{};
 bool constructed{},assigned{};
};
struct ProjectileFloorV112 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};const std::uint32_t* flags24{};
};
struct ProjectileRoomV112 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};const std::uint32_t* flags24{};
};
//ONLY native primitives/resources belonging to Main's existing backend.
//Callbacks weakly borrow containing World/receiver; independent owner pins
//engine services. No pool/class/table/clock/physics/FX/skill system is created.
struct ProjectileMethodServicesV112 {
 std::shared_ptr<void> owner;
 std::function<bool(CanonicalProjectileV99&,std::string&)> current;
 std::function<bool(std::uint32_t&,std::string&)> table_count;
 std::function<bool(std::int32_t,ProjectileTableRowV112&,std::string&)> table_row;
 std::function<bool(std::uintptr_t,ProjectileObjectV112&,std::string&)> object;
 std::function<bool(const ProjectileObjectV112&,ProjectilePointV112&,std::string&)> look_at;
 std::function<bool(const ProjectileObjectV112&,ProjectilePointBorrowV112&,std::string&)> target_position;
 std::function<bool(const ProjectileObjectV112&,std::int32_t&,std::string&)> object_virtual34;
 std::function<bool(std::uintptr_t,const char*,std::uintptr_t&,std::string&)> node_from_name;
 std::function<bool(std::uintptr_t,ProjectilePointBorrowV112&,std::string&)> node_absolute_position;
 std::function<bool(CanonicalProjectileV99&,std::int32_t,std::string&)> set_visual_index; //SAME AnimatedFxLibrary row8 ->SetVisualObject(file,NULL,false).
 std::function<bool(CanonicalProjectileV99&,std::string&)> set_visual_null;
 std::function<bool(const std::shared_ptr<ProjectilePhysicalPrefixV112>&,std::string&)> retain_physical_prefix,retire_assigned_physical_prefix;
 //Actual PhysicalObjectC2: false,true,true,true, short0,u16 20,u16 51f,int0.
 std::function<bool(CanonicalProjectileV99&,ProjectilePhysicalPrefixV112&,std::string&)> construct_physical;
 std::function<bool(CanonicalProjectileV99&,ProjectilePhysicalPrefixV112&,bool,std::string&)> set_physical;
 std::function<bool(CanonicalProjectileV99&,const ProjectilePointV112&,bool,std::string&)> set_position;
 std::function<bool(CanonicalProjectileV99&,const ProjectilePointV112&,std::string&)> set_rotation,set_destination;
 std::function<bool(std::uintptr_t,unsigned,unsigned,unsigned,unsigned,unsigned,std::string&)> animator_virtual1c;
 std::function<bool(std::uintptr_t,unsigned,std::int32_t&,std::string&)> animator_virtual18;
 std::function<bool(std::uintptr_t,std::string&)> visual_sync,visual_sync_rotation;
 std::function<bool(CanonicalProjectileV99&,std::string&)> gameobject_update;
 std::function<bool(CanonicalProjectileV99&,bool,std::string&)> pf_flying,pf_swimming; //actual SAME PFObject5241f4/524218.
 std::function<bool(ProjectileApplicationV112&,std::string&)> application;
 //Real callback int(*)(Projectile*,void*), invoked using actual native token.
 std::function<bool(std::uintptr_t,CanonicalProjectileV99&,std::uintptr_t,std::int32_t&,std::string&)> callback;
 std::function<bool(CanonicalProjectileV99&,std::uintptr_t,const std::array<float,2>&,std::string&)> impact_object;
 std::function<bool(CanonicalProjectileV99&,std::int32_t,const ProjectilePointV112&,std::string&)> impact_kind;
 std::function<bool(CanonicalProjectileV99&,std::uintptr_t,bool,std::string&)> despawn;
 //Floor/Room queries loan SAME current PF graph/native refs. Main executes
 //navigation primitives; caller preserves original bool-result handling.
 std::function<bool(CanonicalProjectileV99&,const ProjectilePointV112&,bool,bool&,float&,ProjectileFloorV112&,std::string&)> world_floor_height;
 std::function<bool(CanonicalProjectileV99&,const ProjectileFloorV112&,bool&,std::string&)> can_path_on;
 std::function<bool(CanonicalProjectileV99&,const ProjectilePointV112&,ProjectileRoomV112&,std::string&)> world_room_at;
 std::function<bool(CanonicalProjectileV99&,ProjectileRoomV112&,ProjectileFloorV112&,std::string&)> pf_room_floor;
 std::function<bool(const ProjectileFloorV112&,const ProjectilePointV112&,float&,std::string&)> floor_height;
 std::function<bool(const ProjectileRoomV112&,const ProjectilePointV112&,float&,ProjectileFloorV112&,std::string&)> room_floor_height;
 //Actual original assertion handling. Invalid index/NULL source continuation
 //is outside safe native domain; helpers never invent an assertion mode.
 std::function<bool(unsigned,std::string&)> invalid_argument;
 //float SetInfo uses actual virtualc8 dispatch (Laser override when present).
 std::function<bool(CanonicalProjectileV99&,std::int32_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,bool,std::string&)> virtual_set_info_bool;
};
bool projectile_set_info_v112(CanonicalProjectileV99&,std::int32_t row,std::uintptr_t source,std::uintptr_t target,
 std::uintptr_t check_callback,std::uintptr_t hit_callback,std::uintptr_t userdata,bool from_node,ProjectileMethodServicesV112&,std::string&);
bool projectile_set_info_angle_v112(CanonicalProjectileV99&,std::int32_t row,std::uintptr_t source,std::uintptr_t target,
 std::uintptr_t check_callback,std::uintptr_t hit_callback,std::uintptr_t userdata,float angle,ProjectileMethodServicesV112&,std::string&);
bool projectile_update_v112(CanonicalProjectileV99&,ProjectileMethodServicesV112&,std::string&);
bool projectile_on_expire_v112(CanonicalProjectileV99&,std::int32_t impact_kind,ProjectileMethodServicesV112&,std::string&);
//Shared field helpers borrow the SAME receiver cells; no shadow projectile data.
bool projectile_read_float_v112(CanonicalProjectileV99&,unsigned,float&,std::string&);
bool projectile_write_float_v112(CanonicalProjectileV99&,unsigned,float,std::string&);
bool projectile_read_point_v112(CanonicalProjectileV99&,unsigned,ProjectilePointV112&,std::string&);
bool projectile_write_point_v112(CanonicalProjectileV99&,unsigned,const ProjectilePointV112&,std::string&);
}
