#pragma once
#include "world.hpp"
#include "data.hpp"
#include "animation.hpp"
#include "skinning.hpp"
#include "../engine-skinning/skin_pose_cache_v32.hpp"
namespace dh2::objects {
struct Record {
 unsigned kind=0,room=0;
 std::string name,character,model;
 world::Point position{},rotation_degrees{},scale{};
 std::array<float,16> placement{};
};
struct Vertex {float p[3]{},uv[2]{},color[4]{1,1,1,1};};
struct Primitive {
 unsigned node=0,material=0;
 std::vector<Vertex> vertices;
 std::vector<std::uint16_t> indices;
 skinning::Skin skin;
 std::vector<world::Point> rest_positions;
 // Source resource streams remain immutable after load. If a development
 // caller replaces/mutates Skin/rest in place, reset this derived cache first.
 skinning::SkinPoseCacheV32 pose_cache_v32;
};
struct Resource {
 scene::Scene scene;
 scene::Scene rest_scene;
 animation::Player animation;
 animation::PoseSampleWorkspaceV32 sample_workspace_v32;
 std::vector<Primitive> primitives;
 unsigned removed_helpers=0,triangles=0;
};
// Development descriptor preserves authored placements and original table
// links. Conditional/template factories remain outside this format.
bool load_records(const std::uint8_t*,std::size_t,unsigned rooms,const data::CharacterTable&,const data::Dictionary&,std::vector<Record>&,std::string&);
// Historical baseline audit fixture selection. Android resolves CharAnim
// states through game-data/animation_tables.hpp instead. Empty means decor.
std::string idle_clip(const std::string& model);
bool load_resource(const std::uint8_t* model,std::size_t model_size,const std::uint8_t* clip,std::size_t clip_size,Resource&,std::string&);
// Explicit original controller IDs selected by modular visual item names.
// Reject missing/duplicate modules instead of showing every armor variant.
bool load_modular_resource(const std::uint8_t* model,std::size_t model_size,
 const std::vector<std::string>& controllers,const std::uint8_t* clip,
 std::size_t clip_size,Resource&,std::string&);
bool sample(Resource&,std::int32_t milliseconds,std::string&);
bool sample(Resource&,const animation::Player&,std::int32_t milliseconds,std::string&);
}
