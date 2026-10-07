#include "selected_world_camera_config_v20.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include <cstdio>
#include <cstdlib>
#include "camera_config_crypt_mlx_v20.inc"
using namespace dh2::world;
unsigned checks{};void check(bool v){++checks;if(!v){std::fprintf(stderr,"FAIL %u\n",checks);std::exit(1);}}
bool assertion(void*,std::string&){return true;}bool diagnostic(void* p,const char* name,std::string&){check(std::string(name)=="NEED TO CHECK Venkat Vikram ********** ********** ************");++*static_cast<unsigned*>(p);return true;}
int main(){std::string e;unsigned diagnostics{},debugs{},publications{};auto world=std::make_shared<int>(1);CanonicalPropertyMapV1 map({&diagnostics,&canonical_vec3_origin_v1(),assertion,diagnostic});CanonicalObjectManagerV1 manager({});std::shared_ptr<CanonicalLevelConfigV1> same_level_config;
 SelectedWorldCameraServicesV20 s;s.world=world;s.debug_switch=[&](const char* key,bool& out,std::string&){check(std::string(key)=="isTracingLevel");++debugs;out=false;return true;};s.register_object=[&](auto o,const auto& name,const auto& archetype,std::string& error){dh2::target_providers::Handle16 h{};return manager.add(o,name.c_str(),archetype.c_str(),-1,false,h,error);};s.publish=[&](auto config,std::string&){check(manager.object(config->canonical(config).shared_handle->key)->identity==config->identity());same_level_config=config;++publications;return true;};
 SelectedWorldCameraConfigV20 owner;std::vector<std::uint8_t> bytes(actual_crypt_mlx,actual_crypt_mlx+sizeof actual_crypt_mlx);check(owner.load("data/scene/x07_crypt_backup.mlx",bytes,map,manager,s,e));dh2::camera::CameraLevelConfigV19 out;check(owner.camera(out,e));check(out.camera.file=="data/3D/camera/CameraTests.bdae"&&out.camera.node=="PlayerCamera_Default"&&out.camera.animation_set=="Default");check(out.camera.near_plane==900&&out.camera.far_plane==5000&&out.skybox234.empty());check(owner.config()==same_level_config&&debugs==4&&publications==1&&diagnostics==1);check(!owner.load("again",bytes,map,manager,s,e));
 CanonicalObjectManagerV1 failed_manager({});SelectedWorldCameraConfigV20 failed;auto missing=s;missing.publish={};check(!failed.load("data/scene/x07_crypt_backup.mlx",bytes,map,failed_manager,missing,e));check(!failed.camera(out,e)&&failed_manager.source_count50()==0);
 std::printf("Actual selected Crypt LevelConfig camera extraction PASS %u checks; canonical ctor/map/parser/InitPost; declared debug/publication observations\n",checks);
}
