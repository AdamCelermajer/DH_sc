#include "scene.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <cmath>
#include <limits>
#include <random>
int main(int argc,char** argv){
  if(argc!=2)return 2;
  std::ifstream f(argv[1],std::ios::binary);std::vector<std::uint8_t> b((std::istreambuf_iterator<char>(f)),{});
  dh2::resources::BresView v{};
  if(dh2_bres_open(&v,b.data(),b.size())!=dh2::resources::BresError::ok)return 3;
  dh2::scene::Scene scene;std::string error;
  if(!dh2::scene::load(v,scene,error)){std::cerr<<error<<'\n';return 4;}
  const auto loaded=scene;
  if(!dh2::scene::update_world(scene,error))return 17;
  for(unsigned i=0;i<scene.instances.size();++i)if(scene.instances[i].world!=loaded.instances[i].world)return 18;
  unsigned draws=0,triangles=0,vertices=0;float low[3]{INFINITY,INFINITY,INFINITY},high[3]{-INFINITY,-INFINITY,-INFINITY};
  for(const auto& i:scene.instances){
    dh2::assets::Mesh m{};if(dh2_mesh_open(&m,&v,i.geometry)!=dh2::assets::Error::ok)return 5;
    if(m.primitives!=i.materials.size())return 6;
    for(unsigned j=0;j<m.primitives;++j){
      dh2::assets::Primitive p{};dh2_mesh_primitive(&m,j,&p);
      if(p.collada_type||p.index_count%3||scene.materials[i.materials[j]].id!=p.material)return 7;
      dh2::assets::Attribute a{};if(dh2_mesh_attribute(&m,p.attributes[0],&a)!=dh2::assets::Error::ok)return 8;
      ++draws;triangles+=p.index_count/3;vertices+=m.vertices;
      for(unsigned k=0;k<m.vertices;++k){float x[4]{};dh2_attribute_read(&a,k,x);
        for(unsigned row=0;row<3;++row){float t=i.world[12+row];for(unsigned c=0;c<3;++c)t+=i.world[4*c+row]*x[c];
          if(!std::isfinite(t))return 9;low[row]=std::min(low[row],t);high[row]=std::max(high[row],t);}
      }
    }
  }
  std::cout<<"{\"nodes\":"<<scene.nodes<<",\"instances\":"<<scene.instances.size()<<",\"draws\":"<<draws<<",\"triangles\":"<<triangles<<",\"vertices_uploaded\":"<<vertices<<",\"ignored_instances\":"<<scene.ignored_instances<<",\"bounds\":[";
  for(unsigned j=0;j<6;++j)std::cout<<(j?",":"")<<(j<3?low[j]:high[j-3]);
  std::cout<<"],\"materials\":[";
  for(unsigned j=0;j<scene.materials.size();++j){auto& m=scene.materials[j];std::cout<<(j?",":"")<<"{\"id\":\""<<m.id<<"\",\"diffuse\":\""<<m.diffuse<<"\",\"alpha_map\":\""<<m.alpha_map<<"\"}";}
  std::cout<<"]}\n";
  // Truncated views and a cycle in the graph must fail atomically.
  auto short_view=v;short_view.size=64;dh2::scene::Scene rejected;
  if(dh2::scene::load(short_view,rejected,error)||!rejected.instances.empty())return 10;
  auto word=[&](unsigned p){return unsigned(b[p])|(unsigned(b[p+1])<<8)|(unsigned(b[p+2])<<16)|(unsigned(b[p+3])<<24);};
  auto original=b;const auto vs=word(v.root_offset+156),node=word(vs+12);
  auto put=[&](unsigned p,unsigned x){for(unsigned j=0;j<4;++j)b[p+j]=(x>>(j*8))&255;};
  put(node+56,1);put(node+60,node);
  if(dh2::scene::load(v,rejected,error)||!rejected.instances.empty())return 11;
  std::mt19937 rng(22026);
  for(unsigned i=0;i<5000;++i){std::copy(original.begin(),original.end(),b.begin());
    const unsigned p=rng()%(b.size()-4);put(p,rng());
    if(!dh2::scene::load(v,rejected,error)&&!rejected.instances.empty())return 12;
  }
  std::cerr<<"Safety: truncation, cycle and 5000 mutated images checked\n";
  return 0;
}
