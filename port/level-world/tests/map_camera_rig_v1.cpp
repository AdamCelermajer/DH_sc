#include "../player_camera_rig_v1.hpp"
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::vector<std::uint8_t> read(const char* path) {
    std::ifstream file(path,std::ios::binary);
    if(!file)throw std::runtime_error(std::string("Cannot open ")+path);
    return {std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
}
void require(bool ok,const char* message) {
    if(!ok)throw std::runtime_error(message);
}
}

int main(int argc,char** argv) {
    try {
        if(argc!=3)return 2;
        const auto camera=read(argv[1]),idle=read(argv[2]);
        dh2::player_camera_rig_v1::Rig rig;std::string error;
        const dh2::player_camera_rig_v1::Assets assets{
            camera.data(),camera.size(),idle.data(),idle.size(),
            "Player_minimap-node",nullptr,true};
        if(!rig.load(assets,error))
            throw std::runtime_error(error);
        require(rig.track_count()>0&&!rig.skipped_tracks()&&!rig.unbound_tracks(),
                "Minimap idle animation did not bind completely to the map camera scene");
        require(rig.animation_end()>rig.animation_start(),
                "Minimap idle animation has no source time range");
        dh2::player_camera_rig_v1::Pose start{},end{};
        require(rig.sample(rig.animation_start(),&start,error),error.c_str());
        require(rig.sample(rig.animation_end(),&end,error),error.c_str());
        for(float value:start.camera)require(std::isfinite(value),"Nonfinite minimap camera start pose");
        for(float value:start.target)require(std::isfinite(value),"Nonfinite minimap target start pose");
        for(float value:start.up_vector)require(std::isfinite(value),"Nonfinite minimap up-vector start pose");
        dh2::player_camera_rig_v1::Playback playback;
        require(playback.start(rig,error),error.c_str());
        require(playback.advance(rig,0,&start,error),error.c_str());
        require(playback.current_time_ms()==rig.animation_start(),
                "Minimap camera timeline did not begin at the authored clip start");
        std::cout<<"minimap camera rig loaded; tracks="<<rig.track_count()<<" range="
                 <<rig.animation_start()<<".."<<rig.animation_end()<<" aspect="
                 <<rig.projection().aspect_ratio<<" fov="<<rig.projection().source_fov_value<<"\n";
        return 0;
    } catch(const std::exception& e) {
        std::cerr<<e.what()<<"\n";return 1;
    }
}
