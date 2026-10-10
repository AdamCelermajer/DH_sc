#include "../actor_lighting.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace f=dh::foundation;
namespace {
void check(bool value,const char* message) { if (!value) throw std::runtime_error(message); }
void near(float a,float b) { check(std::abs(a-b)<1.0e-6f,"Lighting value mismatch"); }
}
int main(int argc,char** argv) {
    try {
        std::string error;
        std::ifstream input(argc>1 ? argv[1] : "port/windows-foundation/assets/original-cache/data/3d/light/swamp.lightset_xml",std::ios::binary);
        check(bool(input),"Actual Swamp lightset unavailable");
        const std::string xml((std::istreambuf_iterator<char>(input)),{});
        std::vector<f::AuthoredPointLight> lights;
        check(f::parseAuthoredPointLights(xml,lights,error),error.c_str());
        check(lights.size()==1 && lights[0].name=="_prim_PlayerLight" && lights[0].attachedTo=="Player","Authored module source mismatch");
        check(lights[0].automatic && lights[0].attachedOffset[2]==200,"Source attachment changed");
        const auto light=f::normalizeSourceLight(lights[0]);
        near(light.attenuation[0],.265625f); near(light.attenuation[1],0); near(light.attenuation[2],.132813f/1000000.0f);
        check(light.ambient[0]==1 && light.diffuse[1]==1 && light.specular[2]==1,"Source color normalization");
        const auto previous=lights.size();
        check(!f::parseAuthoredPointLights("<Module><GameObject type='Light' gametype='LightPoint' name='a' attenuation='1,2,3 extra'/></Module>",lights,error),"Malformed vector accepted");
        check(lights.size()==previous,"Failed parser overwrote source data");
        check(!f::parseAuthoredPointLights("<Module><GameObject type='Light' gametype='Unknown' name='a'/></Module>",lights,error),"Unknown light silently substituted");
        f::SourceLightSets bank;
        std::vector<f::SourceLightBinding> plan;
        check(f::planSourceLightBindings(bank,f::SourceLightSetId::Monster,{true,true,true,true,true},{true,true,true,true},plan,error),error.c_str());
        check(plan.empty(),"Null MonsterLight synthesized from PlayerLight");
        bank.sets[3][1]=light; bank.sets[3][3]=light; bank.sets[3][4]=light; bank.dummyOff[0]=light;
        check(f::planSourceLightBindings(bank,f::SourceLightSetId::Monster,{false,true,false,true,true},{true,true,true,true},plan,error),error.c_str());
        check(plan.size()==4 && plan[0].shaderSlot==0 && plan[0].sourceSlot==1 && !plan[0].dummy
            && plan[1].sourceSlot==3 && plan[2].dummy && plan[2].sourceSlot==0
            && plan[3].dummy && !plan[3].light,"Source enabled compaction/dummy order/fifth-slot exclusion");
        check(f::planSourceLightBindings(bank,f::SourceLightSetId::Monster,{false,true,false,true,true},{false,true,true,true},plan,error),error.c_str());
        check(plan.empty(),"Missing light0 parameter incorrectly advanced shader slots");
        check(!f::planSourceLightBindings(bank,static_cast<f::SourceLightSetId>(4),{},{},plan,error),"Invalid set accepted");
        check(f::classifySourceVertexLighting("ProfileCOMMON_emul_VS.glsl","")==f::SourceVertexLighting::CommonUnlit,"Default branch classified as lit");
        check(f::classifySourceVertexLighting("ProfileCOMMON_emul_VS.glsl","// #define LIGHTING\n # define LIGHTING\n")==f::SourceVertexLighting::CommonLit,"Lighting macro detection");
        check(f::classifySourceVertexLighting("Unknown.glsl","")==f::SourceVertexLighting::Unsupported,"Unknown shader guessed");
        check(f::classifySourceVertexLighting("ProfileCOMMON_emul_VS.glsl","#ifdef DEVICE\n#define LIGHTING\n#endif")==f::SourceVertexLighting::Unsupported,"Conditional driver environment guessed");
        check(f::classifySourceVertexLighting("ProfileCOMMON_emul_VS.glsl","#define LIGHTING\n#undef LIGHTING")==f::SourceVertexLighting::CommonUnlit,"Unconditional undef ignored");
        f::SourceLight reference; reference.position={0,0,10,1}; reference.attenuation={1,0,0};
        reference.ambient={.2f,.2f,.2f,1}; reference.diffuse={.4f,.4f,.4f,1}; reference.specular={0,0,0,1};
        f::SourceCommonMaterialLighting material; material.ambient={1,1,1,1}; material.diffuse={1,1,1,1};
        std::array<float,4> color;
        check(f::evaluateSourceCommonVertexLight(reference,material,{0,0,0},{0,0,1},color,error),error.c_str());
        near(color[0],.6f); near(color[1],.6f); near(color[2],.6f); near(color[3],1);
        check(f::evaluateSourceCommonVertexLight(reference,material,{0,0,0},{0,0,2},color,error),error.c_str());
        near(color[0],1); // Original transformedNormal remains unnormalized.
        reference.attenuation={1,0,.01f};
        check(f::evaluateSourceCommonVertexLight(reference,material,{0,0,0},{0,0,1},color,error),error.c_str());
        near(color[0],.3f); // COMMON attenuates ambient, unlike Diffuse_L1.
        check(f::evaluateSourceCommonVertexLight(reference,material,{0,0,0},{0,0,-1},color,error),error.c_str());
        near(color[0],.1f);
        reference.attenuation={0,0,0}; color={17,23,29,31};
        check(!f::evaluateSourceCommonVertexLight(reference,material,{0,0,0},{0,0,1},color,error),"Zero attenuation denominator fabricated");
        check(color[0]==17,"Failed lighting evaluation mutated result");
        std::cout<<"Source light module, unit normalization, slot binding and COMMON shader reference passed\n";
        return 0;
    } catch (const std::exception& error) { std::cerr<<error.what()<<'\n'; return 1; }
}
