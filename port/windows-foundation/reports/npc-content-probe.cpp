#include "../../scene-materials/scene.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
int main(int argc,char**argv) {
 for(int a=1;a<argc;++a){
  std::ifstream f(argv[a],std::ios::binary|std::ios::ate);auto n=f.tellg();if(n<=0)return 1;
  std::vector<unsigned char>b(static_cast<std::size_t>(n));f.seekg(0);f.read(reinterpret_cast<char*>(b.data()),n);
  dh2::resources::BresView v{};if(dh2_bres_open(&v,b.data(),b.size())!=dh2::resources::BresError::ok)return 2;
  dh2::scene::Scene s;std::string e;if(!dh2::scene::load(v,s,e)){std::cerr<<e;return 3;}
  std::cout<<"FILE "<<argv[a]<<"\n";
  for(const auto&i:s.instances){
   std::cout<<"INSTANCE "<<i.node<<" controller="<<i.controller<<" materials=";for(auto m:i.materials)std::cout<<m<<",";std::cout<<"\n";
   dh2::assets::Mesh m{};if(dh2_mesh_open(&m,&v,i.geometry)!=dh2::assets::Error::ok)return 4;
   for(unsigned p=0;p<m.primitives;++p){dh2::assets::Primitive q{};dh2_mesh_primitive(&m,p,&q);
    std::cout<<"PRIMITIVE "<<p<<" symbol="<<q.material<<" bound=";
    if(p<i.materials.size()){const auto&t=s.materials.at(i.materials[p]);std::cout<<t.id<<":"<<t.diffuse;}
    std::cout<<"\n";
    for(unsigned sem=0;sem<18;++sem){if(q.attributes[sem]<0)continue;dh2::assets::Attribute at{};
     if(dh2_mesh_attribute(&m,q.attributes[sem],&at)!=dh2::assets::Error::ok)return 5;
     float lo[4]{INFINITY,INFINITY,INFINITY,INFINITY},hi[4]{-INFINITY,-INFINITY,-INFINITY,-INFINITY};
     for(unsigned k=0;k<m.vertices;++k){float z[4]{};if(at.components>4)break;if(!dh2_attribute_read(&at,k,z))return 6;for(unsigned c=0;c<at.components;++c){lo[c]=std::min(lo[c],z[c]);hi[c]=std::max(hi[c],z[c]);}}
     std::cout<<"SEM "<<sem<<" attr="<<q.attributes[sem]<<" type="<<at.type<<" components="<<at.components<<" range=";
     for(unsigned c=0;c<at.components&&c<4;++c)std::cout<<lo[c]<<".."<<hi[c]<<",";std::cout<<"\n";
    }
   }
  }
 }
}
