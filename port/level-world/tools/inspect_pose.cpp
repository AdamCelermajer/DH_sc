#include "objects.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
std::vector<std::uint8_t> read(const char* p){std::ifstream f(p,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){if(argc!=3)return 2;auto model=read(argv[1]),clip=read(argv[2]);dh2::resources::BresView view{};dh2_bres_open(&view,model.data(),model.size());dh2::scene::Scene scene;std::string error;if(!dh2::scene::load(view,scene,error)){std::cerr<<error;return 3;}auto rest=scene;dh2::animation::Player player;if(!player.load(clip.data(),clip.size(),scene,error,dh2::animation::MissingTargets::ignore)||!player.sample(scene,100,error)){std::cerr<<error;return 4;}
 for(unsigned i=0;i<scene.graph.size();++i){const auto& r=rest.graph[i];const auto& p=scene.graph[i];std::cout<<i<<' '<<p.id<<" parent "<<p.parent<<" rest_t ";for(float f:r.translation)std::cout<<f<<' ';std::cout<<"rest_q ";for(float f:r.quaternion)std::cout<<f<<' ';std::cout<<"rest_s ";for(float f:r.scale)std::cout<<f<<' ';std::cout<<"pose_t ";for(float f:p.translation)std::cout<<f<<' ';std::cout<<"pose_q ";for(float f:p.quaternion)std::cout<<f<<' ';std::cout<<"pose_s ";for(float f:p.scale)std::cout<<f<<' ';std::cout<<'\n';}
 for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::controller);++i){dh2::skinning::Skin skin;if(!dh2::skinning::load(view,i,rest,skin,error)){std::cerr<<error;return 5;}std::cout<<"skin "<<skin.id<<" bind_shape ";for(float f:skin.bind_shape)std::cout<<f<<' ';std::cout<<'\n';std::vector<dh2::skinning::Matrix> palette;dh2::skinning::palette(skin,rest,palette,error);for(unsigned j=0;j<palette.size();++j){std::cout<<skin.nodes[j]<<" rest_palette ";for(float f:palette[j])std::cout<<f<<' ';std::cout<<'\n';}}
}
