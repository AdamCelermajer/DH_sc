#include "world.hpp"
#include "navigation_avoidance.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
using namespace dh2::navigation;
namespace {
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("Truncated avoidance gold");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
 std::vector<unsigned char> block(unsigned n){if(n>10000000)throw std::runtime_error("Avoidance gold budget");std::vector<unsigned char> v(n);read(v.data(),n);return v;}
};
void require(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
unsigned word(const std::vector<unsigned char>& raw,unsigned at){require(at+4<=raw.size(),"Missing avoidance word");unsigned v;std::memcpy(&v,raw.data()+at,4);return v;}
bool equivalent(const void* aa,const void* bb,unsigned n){const auto* a=static_cast<const unsigned char*>(aa);const auto* b=static_cast<const unsigned char*>(bb);for(unsigned at=0;at<n;at+=4){unsigned x,y;std::memcpy(&x,a+at,4);std::memcpy(&y,b+at,4);if(x==y)continue;float xf,yf;std::memcpy(&xf,&x,4);std::memcpy(&yf,&y,4);if(!(std::isnan(xf)&&std::isnan(yf)))return false;}return true;}
void append(std::vector<unsigned char>& bytes,const void* p,unsigned n){const auto* v=static_cast<const unsigned char*>(p);if(n)bytes.insert(bytes.end(),v,v+n);}
std::vector<unsigned char> snapshot(const ObstacleRegistry& r){
 std::vector<unsigned> keys;if(r.floor_count)keys.assign(r.floors,r.floors+r.floor_count);std::sort(keys.begin(),keys.end(),[](unsigned a,unsigned b){return a==~0u?b!=~0u:b!=~0u&&a<b;});std::vector<unsigned char> bytes;append(bytes,&r.floor_count,4);append(bytes,&r.count,4);append(bytes,keys.data(),keys.size()*4);for(auto floor:keys)for(unsigned i=0;i<r.count;++i)if(r.entries[i].floor==floor)append(bytes,&r.entries[i],16);return bytes;
}
void restore(ObstacleRegistry& r,const std::vector<unsigned char>& raw){r.floor_count=word(raw,0);r.count=word(raw,4);require(r.floor_count<=r.floor_capacity&&r.count<=r.capacity&&raw.size()==8+r.floor_count*4+r.count*16,"Invalid registry gold");if(r.floor_count)std::memcpy(r.floors,raw.data()+8,r.floor_count*4);if(r.count)std::memcpy(r.entries,raw.data()+8+r.floor_count*4,r.count*16);}
}
int main(int argc,char** argv){
 if(argc!=4)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x31445641,"Invalid avoidance gold");const unsigned cases=r.word(),actor_count=r.word(),n=r.word(),e=r.word(),count=r.word();require(cases&&cases<=4096&&actor_count==8&&n==335&&e==838&&count==8,"Invalid avoidance gold budgets");const auto nodes=r.block(n*56),edges=r.block(e*20);std::vector<FloorGraph> floor_graphs(count);r.read(floor_graphs.data(),count*48);require(r.word()==count,"Missing avoidance geometry");
  Reader a(argv[2]),b(argv[3]);dh2::resources::BresView view{};require(dh2_bres_open(&view,a.bytes.data(),a.bytes.size())==dh2::resources::BresError::ok,"Invalid authored BRES");dh2::world::Level level;std::string error;require(dh2::world::load(view,b.bytes.data(),b.bytes.size(),level,error),error.c_str());auto& world=*level.native_floor;
  require(world.graph.node_count==n&&world.graph.edge_count==e&&!std::memcmp(world.graph.nodes,nodes.data(),nodes.size())&&!std::memcmp(world.graph.edges,edges.data(),edges.size())&&!std::memcmp(world.floor_graphs.data(),floor_graphs.data(),count*48),"Native asset-loader graph differs from avoidance gold");
  for(unsigned i=0;i<count;++i){const unsigned triangles=r.word();const auto expected=r.block(triangles*36);dh2::octree::Box box;r.read(&box,24);require(world.records[i]->triangles.size()==triangles&&!std::memcmp(world.records[i]->triangles.data(),expected.data(),expected.size())&&!std::memcmp(&world.records[i]->bounds,&box,24),"Native geometry differs from avoidance gold");}
  AvoidanceActor actors[8];std::uint64_t keys[8];for(unsigned i=0;i<8;++i)keys[i]=0x100000001ull+i;ObstacleEntry entries[512];unsigned buckets[16];ObstacleRegistry registry{entries,0,512,buckets,0,16};AvoidanceScene scene{&registry,actors,keys,8,0};ObstacleForce forces[512];ForceBuffer buffer{forces,0,512};unsigned totals[3]{},force_records=0,contributions=0,adjusted=0,turn_limited=0,gated=0,map_keys=0;
  for(unsigned ci=0;ci<cases;++ci){
   const unsigned op=r.word(),i=r.word(),target=r.word(),use_buffer=r.word(),prefix_size=r.word(),before_size=r.word(),result_size=r.word(),records_size=r.word(),after_size=r.word();require(op<3&&i<8&&target<8&&use_buffer<=1&&prefix_size<=512*24&&prefix_size%24==0&&records_size<=512*24&&records_size%24==0&&before_size<=10000&&after_size<=10000&&result_size==(op==0?16:op==1?44:4),"Invalid avoidance operation");float direction[3];r.read(direction,12);r.read(actors,sizeof(actors));const auto prefix=r.block(prefix_size),before=r.block(before_size),expected=r.block(result_size),expected_records=r.block(records_size),after=r.block(after_size);restore(registry,before);buffer.count=prefix_size/24;if(prefix_size)std::memcpy(forces,prefix.data(),prefix_size);const AvoidanceRequest request{&scene,keys[i],use_buffer?&buffer:nullptr};unsigned char result[44]{};int status=0;
   if(op==0){ForceResult out;status=dh2_nav_obstacle_force(&out,&request);std::memcpy(result,&out,16);contributions+=out.count;
   }else if(op==1){AvoidanceResult out;status=dh2_nav_avoid_obstacles(&out,direction,&request);std::memcpy(result,&out,32);std::memcpy(result+32,direction,12);contributions+=out.force.count;adjusted+=out.adjusted;turn_limited+=out.turn_limited;gated+=!out.evaluated;
   }else{const unsigned out=dh2_nav_can_collide(&actors[i].physical,&actors[target].physical);std::memcpy(result,&out,4);}
   require(status==0,"Native avoidance rejected original gold");const unsigned actual_records=use_buffer&&op<2?buffer.count*24:0;if(!equivalent(result,expected.data(),result_size)||actual_records!=records_size||(records_size&&!equivalent(forces,expected_records.data(),records_size))||snapshot(registry)!=after){std::cerr<<"Avoidance mismatch case "<<ci<<" operation "<<op<<'\n';return 4;}++totals[op];force_records+=records_size/24;map_keys+=registry.floor_count;
  }
  require(r.at==r.bytes.size(),"Trailing avoidance gold");unsigned rejections=0;
  float center[3];for(unsigned k=0;k<3;++k)center[k]=float((double(world.records[0]->triangles.front().points[0][k])+world.records[0]->triangles.front().points[1][k]+world.records[0]->triangles.front().points[2][k])/3.);
  for(unsigned i=0;i<8;++i){actors[i]={};dh2_nav_object_defaults(&actors[i].object);auto& o=actors[i].object;o.motion.object_flags=14;o.motion.room=o.motion.floor=0;o.radius=36;o.obstacle_weight=1;o.obstacle_extent=36;for(unsigned k=0;k<3;++k){o.motion.position[k]=center[k]+(i&&k==0?30+i:0);actors[i].target[k]=center[k]+(k==0?1000:0);}entries[i]={0,0,keys[i]};}
  registry={entries,8,512,buckets,1,16};buckets[0]=0;buffer={forces,0,0};const auto saved_registry=snapshot(registry);ForceResult force{{13,17,19},23};const auto saved_force=force;AvoidanceResult avoidance{{{13,17,19},23},29,31,37,41};const auto saved_avoidance=avoidance;float direction[3]{100,0,0},saved_direction[3]{100,0,0};AvoidanceRequest request{&scene,keys[0],&buffer};
  require(dh2_nav_obstacle_force(&force,&request)==2&&!std::memcmp(&force,&saved_force,16)&&snapshot(registry)==saved_registry&&buffer.count==0,"Force capacity rejection mutated state");++rejections;
  require(dh2_nav_avoid_obstacles(&avoidance,direction,&request)==2&&!std::memcmp(&avoidance,&saved_avoidance,32)&&!std::memcmp(direction,saved_direction,12)&&snapshot(registry)==saved_registry&&buffer.count==0,"Steering capacity rejection mutated state");++rejections;
  buffer.capacity=512;entries[0].object=0x100000100ull;const auto unknown=snapshot(registry);require(dh2_nav_obstacle_force(&force,&request)==1&&!std::memcmp(&force,&saved_force,16)&&snapshot(registry)==unknown&&buffer.count==0,"Unknown object rejection mutated state");++rejections;entries[0].object=keys[0];
  keys[1]=keys[0];require(dh2_nav_obstacle_force(&force,&request)==1&&!std::memcmp(&force,&saved_force,16)&&snapshot(registry)==saved_registry&&buffer.count==0,"Duplicate key rejection mutated state");++rejections;keys[1]=0x100000002ull;
  actors[1].physical.primary.present=2;require(dh2_nav_can_collide(&actors[0].physical,&actors[1].physical)==-1&&actors[1].physical.primary.present==2,"Malformed contact was accepted");require(dh2_nav_obstacle_force(&force,&request)==1&&!std::memcmp(&force,&saved_force,16)&&buffer.count==0,"Malformed scene contact mutated force");++rejections;actors[1].physical.primary.present=0;
  direction[0]=std::numeric_limits<float>::quiet_NaN();float invalid[3];std::memcpy(invalid,direction,12);require(dh2_nav_avoid_obstacles(&avoidance,direction,&request)==1&&!std::memcmp(&avoidance,&saved_avoidance,32)&&!std::memcmp(direction,invalid,12)&&snapshot(registry)==saved_registry&&buffer.count==0,"Nonfinite direction rejection mutated state");++rejections;std::memcpy(direction,saved_direction,12);
  buffer.count=513;require(dh2_nav_obstacle_force(&force,&request)==1&&!std::memcmp(&force,&saved_force,16)&&buffer.count==513&&snapshot(registry)==saved_registry,"Malformed buffer count mutated state");++rejections;buffer.count=0;
  registry.count=registry.floor_count=registry.floor_capacity=0;const auto empty=snapshot(registry);require(dh2_nav_obstacle_force(&force,&request)==2&&!std::memcmp(&force,&saved_force,16)&&snapshot(registry)==empty&&buffer.count==0,"Map capacity rejection mutated state");++rejections;
  actors[0].object.motion.floor=~0u;require(dh2_nav_avoid_obstacles(&avoidance,direction,&request)==0&&!avoidance.evaluated&&!std::memcmp(direction,saved_direction,12)&&snapshot(registry)==empty,"Early gate touched empty registry");
  std::cout<<"{\"cases\":"<<cases<<",\"operation_counts\":["<<totals[0]<<','<<totals[1]<<','<<totals[2]<<"],\"force_records\":"<<force_records<<",\"contributions\":"<<contributions<<",\"adjusted\":"<<adjusted<<",\"turn_limited\":"<<turn_limited<<",\"gated\":"<<gated<<",\"registry_keys_compared\":"<<map_keys<<",\"atomic_rejection_checks\":"<<rejections<<",\"empty_registry_early_gate_checks\":1,\"authored_graph_nodes\":"<<n<<",\"authored_graph_edges\":"<<e<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
