#pragma once
#include "../engine-resources/resources.hpp"
#include <array>
#include <string>
namespace dh2::scene {
using RenderStateSource76V3=std::array<std::uint8_t,76>;
using RenderPassState32V3=std::array<std::uint8_t,32>;
struct BloodRenderPassV3 {
 RenderStateSource76V3 source{};RenderPassState32V3 pass{};
 std::string vertex_file,fragment_file,vertex_defines,fragment_defines;
 std::uint32_t blend_src{},blend_dst{},blend_equation{},depth_function{},cull_face{},front_face{};
 bool blend{},depth{},depth_write{},cull{},stencil{},sample_coverage{},polygon_offset{};
 // Driver-level color mask/scissor are outside this source renderpass and
 // remain borrowed global state. No default viewport/scissor is synthesized.
};
bool blood_render_pass_v3(const resources::BresView&,const char* actual_material,
 const char* actual_technique,BloodRenderPassV3&,std::string&);
}
extern "C" int dh2_render_pass_convert_v3(dh2::scene::RenderPassState32V3*,const dh2::scene::RenderStateSource76V3*);
