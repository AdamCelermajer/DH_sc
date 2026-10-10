#include "original_camera_config.hpp"
#include "asset_catalog.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool condition,const std::string& error){if(!condition)throw std::runtime_error(error);}
int main(int argc,char**argv){try{
    check(argc==2,"Expected original shared asset root");AssetCatalog assets(argv[1]);std::string error;
    OriginalGameplayCamera camera,baseline,fresh;
    for(auto* value:{&camera,&baseline,&fresh})check(value->load(assets,"data/scene/001_swamp.mlx",{},error)&&value->reset({0,0,0},error),error);
    for(auto* value:{&camera,&baseline})check(value->update({1000,0,0},16,error),error);
    check(camera.update_anchor({500,600,700},0,false,error)&&baseline.update_anchor({0,0,0},0,false,error),error);
    const auto direct=camera.pose().target,origin=baseline.pose().target;
    check(std::abs(direct.x-origin.x-500)<.001&&std::abs(direct.y-origin.y-600)<.001&&std::abs(direct.z-origin.z-700)<.001,"Transition anchor was damped or reset source camera channels");
    check(fresh.update_anchor({500,600,700},16,false,error),error);
    check(camera.update({500,600,700},1,error)&&fresh.update({500,600,700},1,error),error);
    check(std::abs(camera.pose().target.x-fresh.pose().target.x)>.00001,"Transition erased prior damping velocity");
    const auto before=camera.pose().target;
    check(!camera.update_anchor({NAN,0,0},1,false,error)&&camera.pose().target.x==before.x,"Invalid anchor changed camera");
    std::cout<<"Original camera transition publication passed: actual camera assets, direct anchor and preserved damping velocity\n";
    return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
