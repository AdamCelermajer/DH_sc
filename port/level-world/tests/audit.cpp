#include "world.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <cmath>
#include <limits>
void rectangle(dh2::world::Level& level,float x0,float y0,float x1,float y1,float base=0,float slope=0){
 dh2::world::Point a{x0,y0,base+slope*x0},b{x1,y0,base+slope*x1},c{x1,y1,base+slope*x1},d{x0,y1,base+slope*x0};
 level.floor.push_back({a,b,c,0});level.floor.push_back({a,c,d,0});
}
bool movement_cases(){
 using namespace dh2::world;Level slope;rectangle(slope,0,0,200,200,0,.5f);Point p{50,50,25};float h;
 if(!move(slope,p,40,30,5)||std::abs(p[2]-45)>.001f||!supported(slope,p,5,h))return false;
 Level gap;rectangle(gap,0,0,100,100);rectangle(gap,120,0,220,100);p={50,50,0};
 if(!move(gap,p,150,0,5)||p[0]>95||!supported(gap,p,5,h))return false;
 Level floors;rectangle(floors,0,0,100,100);rectangle(floors,0,0,100,100,500);p={50,50,490};
 if(!supported(floors,p,5,h)||h!=500)return false;p={50,50,10};if(!supported(floors,p,5,h)||h!=0)return false;
 p={50,50,250};if(supported(floors,p,5,h))return false;
 Level wall;rectangle(wall,0,0,100,100);p={90,50,0};if(!move(wall,p,50,20,5)||p[0]>95||std::abs(p[1]-70)>.001f)return false;
 const auto previous=p;if(move(wall,p,std::numeric_limits<float>::quiet_NaN(),0,5)||p!=previous)return false;
 if(move(wall,p,0,INFINITY,5)||p!=previous||move(wall,p,10,0,-1)||p!=previous)return false;
 return true;
}
std::vector<std::uint8_t> read(const char* name){std::ifstream f(name,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){if(argc!=3)return 2;auto bytes=read(argv[1]),descriptor=read(argv[2]);dh2::resources::BresView view{};if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)return 3;
 if(!movement_cases()){std::cerr<<"Movement policy regression\n";return 12;}
 dh2::world::Level level;std::string error;if(!dh2::world::load(view,descriptor.data(),descriptor.size(),level,error)){std::cerr<<error<<'\n';return 4;}
 unsigned triangles=0;for(const auto& i:level.scene.instances){dh2::assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&view,i.geometry)!=dh2::assets::Error::ok)return 5;
  for(unsigned j=0;j<mesh.primitives;++j){dh2::assets::Primitive p{};dh2_mesh_primitive(&mesh,j,&p);if(p.collada_type||p.index_count%3||j>=i.materials.size()||level.scene.materials[i.materials[j]].id!=p.material)return 6;triangles+=p.index_count/3;}}
 auto point=level.spawn;float h;if(!dh2::world::supported(level,point,36,h)){std::cerr<<"Unsupported spawn radius\n";return 7;}
 unsigned moved=0;for(unsigned i=0;i<360;++i){if(dh2::world::move(level,point,0,7,36))++moved;if(!dh2::world::supported(level,point,36,h))return 8;}
 if(!moved)return 9;
 auto stairs=level.spawn;unsigned stair_steps=0;for(unsigned i=0;i<220;++i){if(dh2::world::move(level,stairs,0,-7,36))++stair_steps;if(!dh2::world::supported(level,stairs,36,h))return 13;}
 if(!stair_steps||stairs[2]>level.spawn[2]-100)return 14;
 auto rejected=point;if(dh2::world::move(level,rejected,INFINITY,0,36)||rejected!=point)return 10;
 std::mt19937 rng(20261002);for(unsigned i=0;i<2000;++i){auto mutated=descriptor;mutated[rng()%mutated.size()]^=1u<<(rng()%8);dh2::world::Level candidate;
  if(!dh2::world::load(view,mutated.data(),mutated.size(),candidate,error)&&(!candidate.floor.empty()||!candidate.scene.graph.empty()))return 11;}
 std::cout<<"{\"rooms\":"<<level.rooms<<",\"nodes\":"<<level.scene.nodes<<",\"visual_instances\":"<<level.scene.instances.size()<<",\"triangles\":"<<triangles<<",\"navigation_triangles\":"<<level.floor.size()<<",\"spawn\":["<<level.spawn[0]<<","<<level.spawn[1]<<","<<level.spawn[2]<<"],\"accepted_steps\":"<<moved<<",\"end_position\":["<<point[0]<<","<<point[1]<<","<<point[2]<<"],\"stairs_steps\":"<<stair_steps<<",\"stairs_end_position\":["<<stairs[0]<<","<<stairs[1]<<","<<stairs[2]<<"],\"synthetic_movement_checks\":9,\"mutated_descriptors\":2000}\n";
 return 0;
}
