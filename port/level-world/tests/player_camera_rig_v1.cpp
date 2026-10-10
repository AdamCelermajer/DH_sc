#include "../player_camera_rig_v1.hpp"
#include <algorithm>
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
void require(bool condition,const char* message) {
    if(!condition)throw std::runtime_error(message);
}
bool close(float a,float b,float epsilon=0.001f) { return std::abs(a-b)<=epsilon; }
bool same_pose(const dh2::player_camera_rig_v1::Pose& a,
               const dh2::player_camera_rig_v1::Pose& b) {
    for(unsigned i=0;i<16;++i)
        if(!close(a.camera[i],b.camera[i])||!close(a.target[i],b.target[i])||
           !close(a.up_vector[i],b.up_vector[i]))return false;
    for(unsigned i=0;i<3;++i)
        if(!close(a.target_parent_z_axis[i],b.target_parent_z_axis[i]))return false;
    return true;
}
}

int main(int argc,char** argv) {
    try {
        if(argc!=3&&argc!=5){std::cerr<<"usage: player_camera_rig_v1_audit camera_scene.bdae camera_idle.bdae [camera_shake.bdae camera_crit.bdae]\n";return 2;}
        const auto camera=read(argv[1]),idle=read(argv[2]);
        dh2::player_camera_rig_v1::Rig rig;std::string error;
        require(rig.load({camera.data(),camera.size(),idle.data(),idle.size()},error),error.c_str());
        dh2::player_camera_rig_v1::Assets named_assets{
            camera.data(),camera.size(),idle.data(),idle.size()};
        named_assets.camera_name="PlayerCamera_Default";
        dh2::player_camera_rig_v1::Rig named_rig;
        if(!named_rig.load(named_assets,error))throw std::runtime_error("named rig load: "+error);
        dh2::player_camera_rig_v1::Pose named_pose{},default_pose{};
        require(named_rig.sample(named_rig.animation_start(),&named_pose,error),error.c_str());
        require(rig.sample(rig.animation_start(),&default_pose,error),error.c_str());
        require(same_pose(named_pose,default_pose),
                "exact authored Camera name changed the Root_Camera-node-relative pose");
        named_assets.camera_name="PlayerCamera_Missing";
        dh2::player_camera_rig_v1::Rig missing_named_rig;
        if(missing_named_rig.load(named_assets,error)||error.empty())
            throw std::runtime_error("missing exact authored Camera name fell back to a different camera");
        const auto& projection=rig.projection();
        require(close(projection.source_fov_value,45.0f),"Authored camera FOV changed");
        require(close(projection.aspect_ratio,1.5f),"Authored camera aspect changed");
        require(close(projection.near_clip,600.0f),"Authored camera near plane changed");
        require(close(projection.far_clip,3800.0f),"Authored camera far plane changed");
        require(rig.animation_start()==0&&rig.animation_end()==33,"Unexpected authored idle range");
        require(std::isfinite(rig.source_default_target_distance())&&
                rig.source_default_target_distance()>0.0f,
                "Source CameraLevel rest-scene target-to-parent distance is unavailable");
        require(rig.track_count()==6&&!rig.skipped_tracks()&&!rig.unbound_tracks(),
                "Camera idle tracks are not fully supported and bound");
        dh2::player_camera_rig_v1::Pose first{},middle{};
        require(rig.sample(rig.animation_start(),&first,error),error.c_str());
        const float idle_camera_distance=std::hypot(
            std::hypot(first.camera[12]-first.target[12],
                       first.camera[13]-first.target[13]),
            first.camera[14]-first.target[14]);
        require(close(rig.source_default_target_distance(),4.1e-5f,1.0e-8f)&&
                idle_camera_distance>1000.0f,
                "CameraLevel zoom distance must use rest-scene targetCam-to-parent, not idle eye-to-target");
        require(rig.sample(rig.animation_start()+(rig.animation_end()-rig.animation_start())/2,&middle,error),error.c_str());
        for(float value:first.camera)require(std::isfinite(value),"Nonfinite camera matrix");
        for(float value:first.target)require(std::isfinite(value),"Nonfinite target matrix");
        for(float value:first.up_vector)require(std::isfinite(value),"Nonfinite up-vector matrix");
        float parent_axis_length=0.0f,parent_vs_node_axis_delta=0.0f;
        for(unsigned i=0;i<3;++i){
            require(std::isfinite(first.target_parent_z_axis[i]),"Nonfinite target-parent Z axis");
            parent_axis_length+=first.target_parent_z_axis[i]*first.target_parent_z_axis[i];
            parent_vs_node_axis_delta+=std::abs(first.target_parent_z_axis[i]-first.target[8+i]);
        }
        require(parent_axis_length>0.0f,"Target parent basis did not expose local-Z direction");
        float maximum_parent_vs_node_axis_delta=parent_vs_node_axis_delta;
        for(const auto sample_time:{rig.animation_start(),
                                    rig.animation_start()+(rig.animation_end()-rig.animation_start())/2,
                                    rig.animation_end()}){
            dh2::player_camera_rig_v1::Pose sampled{};
            require(rig.sample(sample_time,&sampled,error),error.c_str());
            float delta=0.0f;
            for(unsigned i=0;i<3;++i)delta+=std::abs(
                sampled.target_parent_z_axis[i]-sampled.target[8+i]);
            maximum_parent_vs_node_axis_delta=std::max(maximum_parent_vs_node_axis_delta,delta);
        }
        require(std::abs(first.camera[12])+std::abs(first.camera[13])+std::abs(first.camera[14])>1.0f,
                "Camera pose did not expose the authored camera transform");
        float forward[3]{first.target[12]-first.camera[12],
                         first.target[13]-first.camera[13],
                         first.target[14]-first.camera[14]};
        float up_from_eye[3]{first.up_vector[12]-first.camera[12],
                             first.up_vector[13]-first.camera[13],
                             first.up_vector[14]-first.camera[14]};
        float forward_length=0.0f,up_length=0.0f,dot=0.0f;
        for(unsigned i=0;i<3;++i){
            forward_length+=forward[i]*forward[i];up_length+=up_from_eye[i]*up_from_eye[i];
            dot+=forward[i]*up_from_eye[i];
        }
        require(forward_length>0.0f&&up_length>0.0f&&
                std::abs(dot/std::sqrt(forward_length*up_length))<0.01f,
                "Authored up-vector node is not an eye-relative camera up direction");
        float pose_delta=0.0f;
        for(unsigned i=0;i<16;++i)pose_delta+=std::abs(first.camera[i]-middle.camera[i])
            +std::abs(first.target[i]-middle.target[i])+std::abs(first.up_vector[i]-middle.up_vector[i]);
        dh2::player_camera_rig_v1::Playback playback;
        require(playback.start(rig,error),error.c_str());
        dh2::player_camera_rig_v1::Pose playback_pose{},terminal_pose{},expected_terminal{};
        require(playback.advance(rig,0,&playback_pose,error),error.c_str());
        require(playback.current_time_ms()==rig.animation_start()&&!playback.completed(),
                "Camera idle timeline did not start at the authored clip start");
        require(playback.advance(rig,16,&playback_pose,error),error.c_str());
        require(playback.current_time_ms()==rig.animation_start()+16&&same_pose(playback_pose,middle),
                "Camera idle timeline did not follow the supplied game-frame delta");
        require(playback.advance(rig,17,&playback_pose,error),error.c_str());
        require(playback.current_time_ms()==rig.animation_end()&&!playback.completed(),
                "Camera idle timeline did not retain the exact terminal boundary");
        require(playback.advance(rig,1,&terminal_pose,error),error.c_str());
        require(playback.current_time_ms()==rig.animation_end()&&playback.completed(),
                "Non-looping CameraLevel idle did not complete at its authored end");
        require(rig.sample(rig.animation_end(),&expected_terminal,error),error.c_str());
        require(same_pose(terminal_pose,expected_terminal),
                "Camera idle completion did not retain the authored terminal pose");
        require(playback.advance(rig,250,&terminal_pose,error),error.c_str());
        require(playback.current_time_ms()==rig.animation_end()&&playback.completed()&&
                same_pose(terminal_pose,expected_terminal),
                "Completed non-looping camera idle wrapped back to the beginning");
        if(argc==5){
            const std::vector<std::uint8_t> invalid_clip(8,0);
            require(!rig.load_animation(invalid_clip.data(),invalid_clip.size(),error)&&
                        rig.animation_start()==0&&rig.animation_end()==33&&
                        rig.track_count()==6,
                    "Rejected clip damaged the active camera animation");
            for(int i=3;i<5;++i){
                const auto clip=read(argv[i]);
                if(!rig.load_animation(clip.data(),clip.size(),error))
                    throw std::runtime_error(std::string(argv[i])+": "+error);
                require(rig.track_count()>0&&!rig.unbound_tracks()&&
                            rig.animation_end()>rig.animation_start(),
                        "Selected camera effect did not bind as a complete camera clip");
                dh2::player_camera_rig_v1::Pose effect_start{},effect_middle{};
                require(rig.sample(rig.animation_start(),&effect_start,error),error.c_str());
                require(rig.sample(rig.animation_start()+
                            (rig.animation_end()-rig.animation_start())/2,&effect_middle,error),error.c_str());
                for(const auto* sampled:{&effect_start,&effect_middle}){
                    float delta=0.0f;
                    for(unsigned element=0;element<3;++element)delta+=std::abs(
                        sampled->target_parent_z_axis[element]-sampled->target[8+element]);
                    maximum_parent_vs_node_axis_delta=std::max(maximum_parent_vs_node_axis_delta,delta);
                }
                float effect_delta=0.0f;
                for(unsigned element=0;element<16;++element)
                    effect_delta+=std::abs(effect_start.camera[element]-effect_middle.camera[element])+
                                  std::abs(effect_start.target[element]-effect_middle.target[element])+
                                  std::abs(effect_start.up_vector[element]-effect_middle.up_vector[element]);
                require(effect_delta>1e-4f,
                        "Selected camera effect does not change the authored camera pose");
                require(playback.start(rig,error),error.c_str());
                require(playback.advance(rig,0,&playback_pose,error),error.c_str());
                require(playback.current_time_ms()==rig.animation_start()&&!playback.completed(),
                        "One camera timeline did not restart at the selected effect clip start");
                require(playback.advance(rig,static_cast<std::uint32_t>(
                            rig.animation_end()-rig.animation_start()+1),&playback_pose,error),error.c_str());
                require(playback.current_time_ms()==rig.animation_end()&&playback.completed(),
                        "Selected camera effect did not complete at its source terminal time");
            }
        }
        std::cout<<"camera rig loaded; tracks="<<rig.track_count()<<" range="
                 <<rig.animation_start()<<".."<<rig.animation_end()<<"; eye0="
                 <<first.camera[12]<<","<<first.camera[13]<<","<<first.camera[14]
                 <<" target0="<<first.target[12]<<","<<first.target[13]<<","<<first.target[14]
                 <<" targetParentZ0="<<first.target_parent_z_axis[0]<<","<<first.target_parent_z_axis[1]
                 <<","<<first.target_parent_z_axis[2]<<" targetOwnZDelta="<<parent_vs_node_axis_delta
                 <<" maxParentVsNodeAxisDelta="<<maximum_parent_vs_node_axis_delta
                 <<" up0="<<first.up_vector[12]<<","<<first.up_vector[13]<<","<<first.up_vector[14]
                 <<"; eyeMid="<<middle.camera[12]<<","<<middle.camera[13]<<","<<middle.camera[14]
                 <<" poseDelta="<<pose_delta<<" sourceDistance="<<rig.source_default_target_distance()<<" idleEyeDistance="<<idle_camera_distance<<"; timeline=game-delta,once,terminal="
                 <<playback.current_time_ms()<<"\n";
        return 0;
    } catch(const std::exception& e) { std::cerr<<e.what()<<"\n";return 1; }
}
