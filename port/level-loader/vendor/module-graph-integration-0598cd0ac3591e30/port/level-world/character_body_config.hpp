#pragma once
#include <cstddef>
#include <cstdint>
namespace dh2::physical {
struct CharacterBodyInput {
 void* owner;void* new_physical;void* previous_physical;
 std::uint32_t character_type,is_player,special_owner_byte,collision_group_override,disable_physical,reserved;
 float absolute_bounds[4],position[2]; // xmin,ymin,xmax,ymax; game units
};
struct CharacterBodyDefinition {
 void* user_data;
 float mass,local_center[2],inertia,position[2],angle,linear_damping,angular_damping;
 std::uint32_t allow_sleep,is_sleeping,fixed_rotation,bullet,reserved;
};
struct CharacterShapeDefinition {
 void* user_data;
 std::uint32_t kind,sensor; // circle=0, polygon=1
 float friction,restitution,density,local_position[2],radius,vertices[8];
 std::uint32_t vertex_count;std::int32_t group_index;std::uint32_t category_bits,mask_bits;
};
enum CharacterBodyService : std::uint32_t {
 allocate_physical=1,create_body=2,create_shape=3,mass_from_shapes=4,
 pin_zero_mass=5,destroy_physical=6,assign_physical=7,update_pf_object=8
};
struct CharacterBodyConfig {
 CharacterBodyDefinition body;CharacterShapeDefinition shape;
 float radius;std::uint32_t enabled,po_character,pinned;
 std::uint32_t request_count,requests[8],reserved;
};
static_assert(sizeof(void*)==8);
static_assert(sizeof(CharacterBodyInput)==72&&offsetof(CharacterBodyInput,absolute_bounds)==48);
static_assert(sizeof(CharacterBodyDefinition)==64&&offsetof(CharacterBodyDefinition,allow_sleep)==44);
static_assert(sizeof(CharacterShapeDefinition)==88&&offsetof(CharacterShapeDefinition,vertex_count)==72);
static_assert(sizeof(CharacterBodyConfig)==208&&offsetof(CharacterBodyConfig,requests)==172);
}
extern "C" {
// Produces original creation definitions and service order. Does not allocate,
// create collision shapes, derive mass/local center, destroy or attach objects.
// Pin means SetMass({0,current local center,0}); the center must be obtained from
// the shape/mass backend. 0 completed, -1 malformed native input (no mutation).
int dh2_character_body_config(dh2::physical::CharacterBodyConfig*,const dh2::physical::CharacterBodyInput*);
}
