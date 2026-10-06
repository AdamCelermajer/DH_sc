#include "../../scene-materials/scene.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <stdexcept>
int main(int argc,char**argv){try{if(argc!=2)return 2;const std::string path=argv[1];for(const auto*name:{"skybox_swamp.bdae","skybox_blood.bdae","skybox_under.bdae","skybox_darktemple.bdae","skybox_wind.bdae"}){
 std::ifstream f(path+"/"+name,std::ios::binary);if(!f)throw std::runtime_error("Actual cache file missing");std::vector<std::uint8_t>b{std::istreambuf_iterator<char>(f),{}};dh2::resources::BresView v;std::string error;if(dh2_bres_open(&v,b.data(),b.size())!=dh2::resources::BresError::ok)throw std::runtime_error("Actual skybox BRES open failed");dh2::scene::Scene s;if(!dh2::scene::load(v,s,error))throw std::runtime_error(error);
 std::cout<<name<<" nodes="<<s.graph.size()<<" instances="<<s.instances.size()<<" materials="<<s.materials.size()<<" ignored="<<s.ignored_instances<<'\n';for(const auto&i:s.instances){dh2::assets::Mesh mesh;if(dh2_mesh_open(&mesh,&v,i.geometry)!=dh2::assets::Error::ok)throw std::runtime_error("Actual skybox mesh open failed");std::cout<<" mesh node="<<i.node<<" controller="<<i.controller<<" vertices="<<mesh.vertices<<" primitives="<<mesh.primitives<<" bbox="<<mesh.minimum[0]<<','<<mesh.minimum[1]<<','<<mesh.minimum[2]<<".."<<mesh.maximum[0]<<','<<mesh.maximum[1]<<','<<mesh.maximum[2]<<'\n';}
 for(const auto&m:s.materials)std::cout<<" material="<<m.id<<" effect="<<m.effect_file<<" uri="<<m.effect_uri<<" technique="<<m.gles2_technique<<" diffuse="<<m.diffuse<<" alpha="<<m.alpha_map<<'\n';
 }std::cout<<"Actual five skybox resource/scene/mesh metadata PASS\n";}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
