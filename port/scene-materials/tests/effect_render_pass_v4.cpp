#include "effect_render_pass_v4.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::scene;
int main(int argc,char** argv){
 assert(argc==2);unsigned count=0;
 for(const char* name:{"spell_dh2_hurt_fire.bdae","bloodsplat.bdae","bloodsplat_hero.bdae"}){
  std::ifstream file(std::string(argv[1])+"/"+name,std::ios::binary);assert(file);std::vector<std::uint8_t> bytes(std::istreambuf_iterator<char>(file),{});
  dh2::resources::BresView image{};assert(dh2_bres_open(&image,bytes.data(),bytes.size())==dh2::resources::BresError::ok);
  const bool fire=count==0;EffectRenderPassV4 pass;std::string error;
  assert(effect_render_pass_v4(image,fire?"_1_-_Defaultjh":"fx_particles_alpha","default",pass,error));
  assert(pass.vertex_file=="ProfileCOMMON_emul_VS.glsl"&&pass.fragment_file=="ProfileCOMMON_emul_FS.glsl");
  assert(pass.vertex_defines=="#define TEXTURED\n");
  assert(pass.fragment_defines==(fire?"#define TEXTURED\n#define ADDITIVEBLEND\n ":"#define TEXTURED\n"));
  assert(pass.blend&&pass.depth&&!pass.depth_write&&!pass.stencil&&!pass.sample_coverage&&!pass.polygon_offset);
  assert(pass.blend_equation==0x8006);
  assert(pass.blend_src==(fire?1u:0x302u)&&pass.blend_dst==(fire?1u:0x303u));
  RenderPassState32V3 expected{};assert(!dh2_render_pass_convert_v3(&expected,&pass.source)&&expected==pass.pass);
  const auto old=pass;assert(!effect_render_pass_v4(image,"missing","default",pass,error)&&pass.pass==old.pass&&pass.fragment_defines==old.fragment_defines);
  assert(!effect_render_pass_v4(image,fire?"_1_-_Defaultjh":"fx_particles_alpha","missing",pass,error)&&pass.pass==old.pass);
  ++count;
 }
 std::cout<<"Actual fire and blood material/technique/render-state PASS resources="<<count<<"; GPU pixels remain live integration\n";
}
