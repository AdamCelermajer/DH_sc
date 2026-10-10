#include "../../scene-materials/scene.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
int main(int argc,char**argv){for(int file=1;file<argc;++file){
 std::ifstream f(argv[file],std::ios::binary);std::vector<unsigned char>b{std::istreambuf_iterator<char>(f),{}};dh2::resources::BresView v{};
 if(dh2_bres_open(&v,b.data(),b.size())!=dh2::resources::BresError::ok)return 2;dh2::scene::Scene s;std::string error;if(!dh2::scene::load(v,s,error)){std::cerr<<error;return 3;}
 std::cout<<"FILE "<<argv[file]<<" lights="<<s.lights_v113.size()<<" materialCount="<<s.materials.size()<<'\n';
 for(const auto&m:s.materials){std::cout<<"MATERIAL "<<m.id<<" diffuse="<<m.diffuse<<" alpha="<<m.alpha_map<<" effectFile="<<m.effect_file<<" effectURI="<<m.effect_uri<<" technique="<<m.gles2_technique<<" alphaRef="<<m.alpha_ref<<" additive="<<m.additive<<" backface="<<m.backface<<" color=";for(auto c:m.color)std::cout<<c<<',';std::cout<<" textureMatrix=";for(auto x:m.texture_matrix)std::cout<<x<<',';std::cout<<'\n';}
 for(const auto&i:s.instances){std::cout<<"INSTANCE "<<i.node<<" controller="<<i.controller<<" materials=";for(auto m:i.materials)std::cout<<s.materials.at(m).id<<',';std::cout<<'\n';dh2::assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&v,i.geometry)!=dh2::assets::Error::ok)return 4;
  for(unsigned p=0;p<mesh.primitives;++p){dh2::assets::Primitive primitive{};if(dh2_mesh_primitive(&mesh,p,&primitive)!=dh2::assets::Error::ok)return 5;std::cout<<"PRIMITIVE "<<p<<" symbol="<<primitive.material<<" bound="<<s.materials.at(i.materials.at(p)).id<<'\n';
   for(unsigned semantic:{1u,2u,4u}){if(primitive.attributes[semantic]<0){std::cout<<"STREAM "<<semantic<<" absent\n";continue;}dh2::assets::Attribute a{};if(dh2_mesh_attribute(&mesh,primitive.attributes[semantic],&a)!=dh2::assets::Error::ok||a.components>4)return 6;float lo[4]{INFINITY,INFINITY,INFINITY,INFINITY},hi[4]{-INFINITY,-INFINITY,-INFINITY,-INFINITY};for(unsigned n=0;n<mesh.vertices;++n){float value[4]{};if(!dh2_attribute_read(&a,n,value))return 7;for(unsigned k=0;k<a.components;++k){lo[k]=std::min(lo[k],value[k]);hi[k]=std::max(hi[k],value[k]);}}std::cout<<"STREAM "<<semantic<<" type="<<a.type<<" components="<<a.components<<" vertices="<<mesh.vertices<<" range=";for(unsigned k=0;k<a.components;++k)std::cout<<lo[k]<<".."<<hi[k]<<',';std::cout<<'\n';}
  }
 }
 for(const auto&light:s.lights_v113){std::cout<<"LIGHT "<<light.id<<" type="<<light.type<<" intensity="<<light.intensity<<" node="<<s.graph.at(light.node_index).id<<" color=";for(auto c:light.color)std::cout<<c<<',';std::cout<<" parameters=";for(unsigned i=0;i<light.parameter_count;++i)std::cout<<light.parameters[i]<<',';std::cout<<'\n';}
}return 0;}
