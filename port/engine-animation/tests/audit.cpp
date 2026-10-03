#include "animation.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <cmath>
#include <algorithm>
#include <random>
int main(int argc,char** argv){
    if(argc!=2)return 2;
    std::ifstream f(argv[1],std::ios::binary);std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(f)),{});
    dh2::resources::BresView view{};if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)return 3;
    dh2::scene::Scene scene;std::string error;
    if(!dh2::scene::load(view,scene,error)){std::cerr<<error;return 4;}
    dh2::animation::Player player;
    if(!player.load(bytes.data(),bytes.size(),scene,error)){std::cerr<<error;return 5;}
    if(player.track_count()!=2||player.skipped||player.start!=-166||player.end!=1866)return 6;
    const auto before=scene;bool changed=false;
    for(int ms=player.start;ms<=player.end;++ms){
        if(!player.sample(scene,ms,error)){std::cerr<<ms<<' '<<error;return 7;}
        for(unsigned i=0;i<scene.instances.size();++i){
            for(unsigned j=0;j<16;++j){if(!std::isfinite(scene.instances[i].world[j]))return 8;
                if(scene.instances[i].world[j]!=before.instances[i].world[j])changed=true;}
        }
    }
    if(!changed)return 9;
    if(!player.sample(scene,100,error))return 10;
    const auto bone=std::find_if(scene.graph.begin(),scene.graph.end(),[](const auto& n){return n.id=="_bone_-node";});
    if(bone==scene.graph.end()||std::abs(bone->scale[0]-1.024299979f)>1e-7)return 11;
    // Moving the player must retain ownership of all borrowed key views.
    auto moved=std::move(player);if(!moved.sample(scene,500,error))return 12;
    auto wrong=scene;wrong.graph[0].id="wrong";const auto rejected=wrong;
    if(moved.sample(wrong,500,error)||wrong.graph[0].id!=rejected.graph[0].id)return 13;
    if(moved.load(bytes.data(),64,scene,error)||moved.track_count())return 14;
    std::mt19937 rng(221026);auto mutated=bytes;
    for(unsigned i=0;i<5000;++i){std::copy(bytes.begin(),bytes.end(),mutated.begin());
        auto p=rng()%(mutated.size()-4);auto value=rng();for(unsigned j=0;j<4;++j)mutated[p+j]=(value>>(8*j))&255;
        if(moved.load(mutated.data(),mutated.size(),before,error)){
            auto probe=before;if(!moved.sample(probe,500,error)&&error.empty())return 15;
        }else if(moved.track_count())return 16;
    }
    std::cout<<"{\"tracks\":2,\"milliseconds_sampled\":2033,\"world_transforms_changed\":true,\"scale_at_100ms\":1.024299979,\"move_ownership_checked\":true,\"invalid_image_rejected\":true,\"mutated_inputs\":5000}\n";
}
