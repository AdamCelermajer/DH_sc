#include "../../level-world/source_fx_version_overrides_v1.hpp"
#include <cstdlib>
#include <iostream>
using dh2::fx::source_fx_effective_speed_v1;
static void check(bool v,const char* m){if(!v){std::cerr<<m<<'\n';std::exit(1);}}
int main(){
    check(source_fx_effective_speed_v1("data/3D/interface/cutscene_dh2_witch_intro_scene_01_portal.bdae",0.f)==1.0f,"witch portal zero speed plays at 1.0");
    check(source_fx_effective_speed_v1("DATA/3D/INTERFACE/CUTSCENE_DH2_WITCH_INTRO_SCENE_01_PORTAL.BDAE",0.f)==1.0f,"case-insensitive");
    check(source_fx_effective_speed_v1("data/3D/interface/cutscene_dh2_witch_intro_scene_01_portal.bdae",2.f)==2.0f,"nonzero table speed is kept");
    check(source_fx_effective_speed_v1("data/3D/interface/skill_prince_mage_earthmastery.bdae",0.f)==0.f,"other zero speeds keep the table value");
    check(source_fx_effective_speed_v1("",0.f)==0.f,"empty uri");
    std::cout<<"source_fx_version_overrides ok\n";return 0;
}
