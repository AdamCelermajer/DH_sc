#pragma once
#include "world.hpp"
#include "data.hpp"
#include "animation.hpp"
#include "skinning.hpp"
namespace dh2::objects {
struct Record {
 unsigned kind=0,room=0;
 std::string name,character,model;
 world::Point position{},rotation_degrees{},scale{};
 std::array<float,16> placement{};
 // DACT v2 retains an authored auto_spawn=0 / AI Limbus gate. This is
 // descriptor metadata; loaded actors must remain hidden until a source spawn
 // request changes their state. It is not a script/trigger execution claim.
 bool gated_spawn=false;
};
struct Vertex {float p[3]{},uv[2]{},color[4]{1,1,1,1};};
struct Primitive {
 unsigned node=0,material=0;
 std::vector<Vertex> vertices;
 std::vector<std::uint16_t> indices;
 skinning::Skin skin;
 std::vector<world::Point> rest_positions;
};
struct Resource {
 scene::Scene scene;
 scene::Scene rest_scene;
 animation::Player animation;
 std::vector<Primitive> primitives;
 unsigned removed_helpers=0,triangles=0;
 // Set only by the exact source Character no-geometry loader. Scene and
 // animation data remain validated; there are simply no drawable primitives.
 bool intentional_empty_visual=false;
};
// Development descriptor preserves authored placements and original table
// links. v1 covers automatic direct actors; v2 can also retain resolved direct
// Limbus actors. Conditional/template factories remain outside this format.
bool load_records(const std::uint8_t*,std::size_t,unsigned rooms,const data::CharacterTable&,const data::Dictionary&,std::vector<Record>&,std::string&);
// Historical baseline audit fixture selection. Android resolves CharAnim
// states through game-data/animation_tables.hpp instead. Empty means decor.
std::string idle_clip(const std::string& model);
bool load_resource(const std::uint8_t* model,std::size_t model_size,const std::uint8_t* clip,std::size_t clip_size,Resource&,std::string&);
// Source Character whose authored BRES has a valid scene graph but no geometry.
// It still loads/validates the scene and animation; this does not accept broken
// meshes or empty decor resources.
bool load_intentional_empty_character_visual(const std::uint8_t* model,
 std::size_t model_size,const std::uint8_t* clip,std::size_t clip_size,
 Resource&,std::string&);
// v69 original controller IDs selected from immutable modular item resources.
// Requires each requested controller exactly once; no equipment grant/store.
bool load_modular_resource(const std::uint8_t* model,std::size_t model_size,
 const std::vector<std::string>& controllers,const std::uint8_t* clip,
 std::size_t clip_size,Resource&,std::string&);
bool sample(Resource&,std::int32_t milliseconds,std::string&);
bool sample(Resource&,const animation::Player&,std::int32_t milliseconds,std::string&);
}
