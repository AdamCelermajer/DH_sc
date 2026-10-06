#include "../shader_reflection_v4.hpp"
#include "shader_reflection_v4_gold.hpp"
#include <cstdlib>
#include <iostream>
using namespace dh2::scene;
int main(){
 unsigned checks=0;
 for(const auto& g:gold){ShaderUniformReflectionV4 u;u.name=g.name;u.gl_type=0x1406;u.count=1;u.location=17;shader_uniform_reflect_v4(u);
  unsigned sem=g.semantic==255?0:g.semantic,sub=g.semantic==255?0:g.sub;
  if(u.semantic!=sem||u.sub_id!=sub){std::cerr<<g.name<<" mismatch "<<u.semantic<<','<<unsigned(u.sub_id)<<" expected "<<sem<<','<<sub<<'\n';return 1;}
  if(u.count!=1||u.location!=17||u.type!=5)return 2;++checks;
 }
 std::vector<ShaderUniformReflectionV4> uniforms;
 for(const char* name:{"Sampler0","WorldViewProjectionMatrix","DiffuseColor","CameraPosition","TextureMatrix0"}){ShaderUniformReflectionV4 u;u.name=name;u.gl_type=0x1406;shader_uniform_reflect_v4(u);uniforms.push_back(u);}
 if(shader_uniform_partition_v4(uniforms)!=2)return 3;
 const char* expected[]={"WorldViewProjectionMatrix","CameraPosition","Sampler0","DiffuseColor","TextureMatrix0"};
 for(unsigned i=0;i<5;++i){if(uniforms[i].name!=expected[i])return 4;++checks;}
 ShaderUniformReflectionV4 unknown;unknown.name="unrecognized";unknown.gl_type=0x8b5e;shader_uniform_reflect_v4(unknown);
 if(unknown.semantic!=2||unknown.sub_id!=2||unknown.type!=12)return 5;++checks;
 std::cout<<"PASS "<<checks<<" actual original reflection gold and stable partition checks\n";
}
