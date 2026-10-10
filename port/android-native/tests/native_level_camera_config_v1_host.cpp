#include "../app/src/main/cpp/native_camera_crypt_frame_v1.hpp"
#include "../app/src/main/cpp/native_level_camera_config_v1.hpp"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
std::string read(const char* path) {
    std::ifstream file(path,std::ios::binary);
    return {std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
}
bool near(float a,float b,float epsilon=1e-5f) {
    return std::fabs(a-b)<=epsilon;
}
}

int main(int argc,char** argv) {
    if(argc!=4){std::fprintf(stderr,"usage: native_level_camera_config_v1_host swamp.mlx crypt.rule.xml crypt.mlx\n");return 2;}
    const auto swamp_xml=read(argv[1]),crypt_xml=read(argv[2]);
    const auto crypt_mlx=read(argv[3]);
    dh2::native::level_camera_config_v1::ClipPlanes swamp{},crypt{};
    if(!near(dh2::native::level_camera_config_v1::kVerticalFovRadians,
             0.42963001132011414f,1e-8f)){
        std::fprintf(stderr,"FAIL: Level::_LoadCamera vertical FOV must match CameraBase::SetData\n");return 1;
    }
    if(!dh2::native::level_camera_config_v1::parse_clip_planes(swamp_xml,&swamp)||
       !near(swamp.near_clip,900.0f)||!near(swamp.far_clip,4200.0f)){
        std::fprintf(stderr,"FAIL: SWAMP LevelConfig clip planes must come from 001_swamp.mlx\n");return 1;
    }
    if(!dh2::native::level_camera_config_v1::parse_clip_planes(crypt_xml,&crypt)||
       !near(crypt.near_clip,900.0f)||!near(crypt.far_clip,5000.0f)){
        std::fprintf(stderr,"FAIL: Crypt rule clip planes must come from 007_crypt_01.rule.xml\n");return 1;
    }
    using dh2::native::level_camera_config_v1::CameraRoute;
    CameraRoute swamp_route{},crypt_route{},explicit_route{};
    if(!dh2::native::level_camera_config_v1::parse_camera_route(swamp_xml,&swamp_route)||
       !dh2::native::level_camera_config_v1::parse_camera_route(crypt_mlx,&crypt_route)||
       swamp_route.camera_file!="CameraTests.bdae"||
       crypt_route.camera_file!="CameraTests.bdae"||
       swamp_route.camera_name!="PlayerCamera_Default"||
       crypt_route.camera_name!="PlayerCamera_Default"||
       swamp_route.animset!="Default"||crypt_route.animset!="Default"){
        std::fprintf(stderr,"FAIL: empty source camera fields did not resolve to CameraTests.bdae / PlayerCamera_Default / Default\n");return 1;
    }
    if(!dh2::native::level_camera_config_v1::parse_camera_route(
       "<Level camera_file=\"data/3d/camera/custom.bdae\" camera_animset=\"Boss\" />",
       &explicit_route)||explicit_route.camera_file!="data/3d/camera/custom.bdae"||
       explicit_route.animset!="Boss"){
        std::fprintf(stderr,"FAIL: explicit LevelConfig camera resource/animset was not preserved\n");return 1;
    }
    bool combat_music=false;
    if(!dh2::native::level_camera_config_v1::parse_combat_music_enabled(
          "<Level><GameObject gametype=\"LevelConfig\"/>",&combat_music)||!combat_music||
       !dh2::native::level_camera_config_v1::parse_combat_music_enabled(
          "<Level combat_music_enabled=\"0\"/>",&combat_music)||combat_music||
       !dh2::native::level_camera_config_v1::parse_combat_music_enabled(
          "<Level combat_music_enabled=\"1\"/>",&combat_music)||!combat_music||
       dh2::native::level_camera_config_v1::parse_combat_music_enabled(
          "<Level combat_music_enabled=\"yes\"/>",&combat_music)){
        std::fprintf(stderr,"FAIL: LevelConfig combat music source default/boolean rejected\n");return 1;
    }

    using namespace dh2::native::crypt_camera_frame_v1;
    const Vec3 target{1090.75f,-212.202f,255.0001f};
    const Vec3 eye_offset{1380.0f,-1180.46f,2551.55f};
    const Vec3 up_direction{-404.008f,345.604f,378.41f};
    Frame swamp_frame{},crypt_frame{};
    if(!build_player_frame(target,2400,1080,&swamp_frame,kVerticalFovRadians,
                           swamp.near_clip,swamp.far_clip,eye_offset,up_direction)||
       !build_player_frame(target,2400,1080,&crypt_frame,kVerticalFovRadians,
                           crypt.near_clip,crypt.far_clip,eye_offset,up_direction)||
       !near(swamp_frame.aspect,2400.0f/1080.0f)||
       !near(swamp_frame.input_yaw,std::atan2(eye_offset[1],eye_offset[0]))||
       !near(swamp_frame.input_pitch,std::atan2(eye_offset[2],std::hypot(eye_offset[0],eye_offset[1])))){
        std::fprintf(stderr,"FAIL: shared rig frame rejected source route or modern viewport\n");return 1;
    }
    bool projection_differs=false;
    for(std::size_t i=0;i<swamp_frame.view_projection.size();++i)
        projection_differs|=!near(swamp_frame.view_projection[i],crypt_frame.view_projection[i],1e-7f);
    if(!projection_differs){std::fprintf(stderr,"FAIL: source level clip-plane override did not affect projection\n");return 1;}
    Frame invalid{};
    if(build_player_frame(target,2400,1080,&invalid,kVerticalFovRadians,
                          900.0f,900.0f,eye_offset,up_direction)){
        std::fprintf(stderr,"FAIL: invalid near/far order accepted\n");return 1;
    }
    std::puts("PASS: per-level camera clip planes, LevelConfig camera resource/animset route, shared player-rig projection");
    return 0;
}
