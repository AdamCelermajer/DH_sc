// Reuse the original-derived replay without linking production sources into
// this executable. Runtime symbol ownership must resolve to the actual DSOs.
#define main dh2_component_audit_main
#include "component_applicator.cpp"
#undef main
#include "../animation_blend.hpp"
#include <dlfcn.h>
#include <string>

namespace {
std::string library(const void* function,const char* expected){
 Dl_info info{};check(dladdr(function,&info)!=0&&info.dli_fname,"Cannot resolve loaded component dependency");
 std::string path=info.dli_fname;check(path.find(expected)!=std::string::npos,"Component audit resolved to wrong library");return path;
}
}
int main(int argc,char**argv){try{
 const auto animation=library(reinterpret_cast<const void*>(&dh2_animation_component_apply),"libdh2_engine_animation.so");
 check(library(reinterpret_cast<const void*>(&dh2_animation_component_apply_blended),"libdh2_engine_animation.so")==animation,"Apply helpers do not share actual animation DSO");
 check(library(reinterpret_cast<const void*>(&dh2_animation_component_blend),"libdh2_engine_animation.so")==animation,"Contribution helper does not share actual animation DSO");
 check(library(reinterpret_cast<const void*>(&dh2::animation::apply_component),"libdh2_engine_animation.so")==animation,"Graph bridge does not share actual animation DSO");
 check(library(reinterpret_cast<const void*>(&dh2::animation::apply_blended_component),"libdh2_engine_animation.so")==animation,"Blended graph bridge does not share actual animation DSO");
 check(library(reinterpret_cast<const void*>(&dh2_animation_blend_vector3),"libdh2_engine_animation.so")==animation,"Weighted kernel does not share actual animation DSO");
 const auto scene=library(reinterpret_cast<const void*>(&dh2::scene::update_world),"libdh2_scene_materials.so");
 const int status=dh2_component_audit_main(argc,argv);if(status)return status;
 std::cout<<"{\"actual_dso_symbols_checked\":7,\"animation_library\":\""<<animation<<"\",\"scene_library\":\""<<scene<<"\"}\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
