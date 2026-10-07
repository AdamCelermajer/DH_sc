#include "character_menu_reload_v1.hpp"
namespace {
using namespace dh2::ui;
bool aligned(const void* p,std::size_t alignment){return p&&reinterpret_cast<std::uintptr_t>(p)%alignment==0;}
bool overlap(const void* a,const void* b){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<16:x-y<16;}
}
extern "C" int dh2_character_menu_reload_v1(dh2::ui::MenuReloadResult16V1* out,
 std::uintptr_t actor,const dh2::ui::MenuReloadServices16V1* services){
 using namespace dh2::ui;
 if(!aligned(out,alignof(MenuReloadResult16V1))||!actor||!aligned(services,alignof(MenuReloadServices16V1))||!services->invoke||overlap(out,services))return -1;
 *out={};MenuReloadResponse16V1 response{};
 auto send=[&](MenuReloadServiceV1 service,std::uint32_t argument=0,std::uintptr_t subject=0,const char* path=nullptr,const char* callback=nullptr){
  out->phase=service+1;++out->calls;response={};
  const MenuReloadRequest32V1 request{service,argument,subject?subject:actor,path,callback};
  return services->invoke(services->context,&request,&response)==0&&response.reserved==0;
 };
 if(!send(reload_remove_buffs_v1)||!send(reload_saved_skills_v1,0x20)||
    !send(reload_skill_instances_v1)||!send(reload_update_skills_v1)||
    !send(reload_recalculate_v1,1)||!send(reload_check_items_v1)||
    !send(reload_saved_level_v1))return -2;
 if(response.value>11){
  // Each failed class comparison rereads the saved class, including mutations
  // made synchronously by the prior source getter. Do not cache one value.
  const int classes[]{263,325,290};for(const auto wanted:classes){
   if(!send(reload_saved_class_v1))return -2;
   if(response.value==wanted){out->specialization=1;break;}
  }
 }
 if(!send(reload_menu_fx_v1)||!response.identity)return -2;
 const auto menu_fx=response.identity;
 if(!send(reload_spec_prompt_v1,out->specialization,menu_fx,
          "_root.menu_CharacterMenu","IsSpecTime"))return -2;
 out->phase=11;return 1;
}
