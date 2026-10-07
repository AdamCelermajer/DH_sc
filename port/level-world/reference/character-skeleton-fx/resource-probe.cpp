#include "../../../engine-animation/animation.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
int main(int argc,char** argv){try{
 if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);if(!f)throw std::runtime_error("resource file");std::vector<std::uint8_t> raw{std::istreambuf_iterator<char>(f),{}};dh2::resources::BresView b{};std::string error;dh2::scene::Scene scene;
 if(dh2_bres_open(&b,raw.data(),raw.size())!=dh2::resources::BresError::ok||!dh2::scene::load(b,scene,error))throw std::runtime_error(error);
 dh2::animation::Player player;if(!player.load(raw.data(),raw.size(),scene,error,dh2::animation::MissingTargets::ignore))throw std::runtime_error(error);
 std::cout<<"{\"validation\":\"PASS\",\"bytes\":"<<raw.size()<<",\"nodes\":"<<scene.graph.size()<<",\"instances\":"<<scene.instances.size()<<",\"ignored_instances\":"<<scene.ignored_instances<<",\"animation_tracks\":"<<player.track_count()<<",\"skipped_tracks\":"<<player.skipped<<",\"unbound_tracks\":"<<player.unbound<<",\"start\":"<<player.start<<",\"end\":"<<player.end<<",\"materials\":[";bool first=true;
 for(const auto& m:scene.materials){if(!first)std::cout<<',';first=false;std::cout<<"{\"id\":\""<<m.id<<"\",\"diffuse\":\""<<m.diffuse<<"\",\"alpha_map\":\""<<m.alpha_map<<"\",\"additive\":"<<(m.additive?"true":"false")<<"}";}
 std::cout<<"],\"raw_animations\":[";first=true;
 for(std::uint32_t i=0;i<dh2_bres_library_count(&b,dh2::resources::Library::animation);++i){dh2::assets::Animation a{};if(dh2_animation_open(&a,&b,i,0)!=dh2::assets::Error::ok)throw std::runtime_error("animation accessor");if(!first)std::cout<<',';first=false;std::cout<<"{\"target\":\""<<dh2_animation_target(&a)<<"\",\"channels\":"<<dh2_animation_channels(&a)<<",\"samplers\":"<<dh2_animation_samplers(&a)<<",\"start\":"<<a.segment_start<<",\"end\":"<<a.segment_end<<",\"types\":[";for(std::uint32_t j=0;j<dh2_animation_channels(&a);++j){if(j)std::cout<<',';std::cout<<dh2_animation_type(&a,j);}std::cout<<"]}";}
 std::cout<<"],\"source_factory_executed\":false,\"GPU_executed\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
