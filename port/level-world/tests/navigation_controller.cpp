#include "navigation_controller.hpp"
#include "world.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>
using namespace dh2::navigation;
namespace {
void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* file){std::ifstream f(file,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){require(at<=bytes.size()&&n<=bytes.size()-at,"Truncated controller gold");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned value;read(&value,4);return value;}
 std::vector<unsigned char> block(unsigned n){require(n<=10000000,"Controller gold budget");std::vector<unsigned char> b(n);read(b.data(),n);return b;}
};
std::uint32_t bits(float f){std::uint32_t u;std::memcpy(&u,&f,4);return u;}
unsigned ordered(float f){const auto u=bits(f);return u&0x80000000u?~u:u|0x80000000u;}
bool same_words(const void* left,const void* right,std::size_t size){
 const auto* a=static_cast<const unsigned char*>(left);const auto* b=static_cast<const unsigned char*>(right);
 for(std::size_t at=0;at<size;at+=4){std::uint32_t x,y;std::memcpy(&x,a+at,4);std::memcpy(&y,b+at,4);if(x==y)continue;float xf,yf;std::memcpy(&xf,&x,4);std::memcpy(&yf,&y,4);if(!std::isnan(xf)||!std::isnan(yf))return false;}return true;
}
std::vector<unsigned char> path_bytes(const PathObject& p){std::vector<unsigned char> b(76+p.count*48);std::memcpy(b.data(),&p,68);std::memcpy(b.data()+68,&p.count,4);std::memcpy(b.data()+72,&p.owned,4);if(p.count)std::memcpy(b.data()+76,p.segments,p.count*48);return b;}
void restore_registry(ObstacleRegistry& r,const std::vector<unsigned char>& bytes){
 require(bytes.size()>=8,"Registry header");unsigned floors,count;std::memcpy(&floors,bytes.data(),4);std::memcpy(&count,bytes.data()+4,4);require(floors<=r.floor_capacity&&count<=r.capacity&&bytes.size()==8+floors*4+count*16,"Registry budget");r.floor_count=floors;r.count=count;if(floors)std::memcpy(r.floors,bytes.data()+8,floors*4);if(count)std::memcpy(r.entries,bytes.data()+8+floors*4,count*16);
}
std::vector<unsigned char> registry_bytes(const ObstacleRegistry& r){
 std::vector<unsigned> floors(r.floors,r.floors+r.floor_count);std::sort(floors.begin(),floors.end());std::vector<unsigned char> b(8+floors.size()*4+r.count*16);const unsigned f=floors.size();std::memcpy(b.data(),&f,4);std::memcpy(b.data()+4,&r.count,4);if(f)std::memcpy(b.data()+8,floors.data(),f*4);std::size_t at=8+f*4;for(auto floor:floors)for(unsigned i=0;i<r.count;++i)if(r.entries[i].floor==floor){std::memcpy(b.data()+at,&r.entries[i],16);at+=16;}require(at==b.size(),"Unlisted registry member");return b;
}
}
int main(int argc,char** argv){
 if(argc!=4)return 2;
 try{
  Reader gold(argv[1]);require(gold.word()==0x31445055,"Invalid controller gold");const unsigned cases=gold.word(),n=gold.word(),e=gold.word(),f=gold.word();require(cases&&cases<10000&&n==335&&e==838&&f==8,"Controller graph budgets");const auto nodes=gold.block(n*56),edges=gold.block(e*20),floors=gold.block(f*48);
  require(gold.word()==8,"Controller geometry count");for(unsigned i=0;i<8;++i){const auto triangles=gold.word();gold.block(triangles*36);gold.block(24);}
  Reader bres(argv[2]),descriptor(argv[3]);dh2::resources::BresView view{};require(dh2_bres_open(&view,bres.bytes.data(),bres.bytes.size())==dh2::resources::BresError::ok,"Invalid controller BRES");dh2::world::Level level;std::string error;require(dh2::world::load(view,descriptor.bytes.data(),descriptor.bytes.size(),level,error),error.c_str());auto& native=*level.native_floor;require(native.graph.node_count==n&&native.graph.edge_count==e,"Authored controller graph counts");require(!std::memcmp(native.nodes.data(),nodes.data(),nodes.size())&&!std::memcmp(native.edges.data(),edges.data(),edges.size()),"Controller graph differs from original");
  std::vector<PathSegment> segments(1024),scratch_segments(1024);AvoidanceActor actors[8]{},scratch_actors[8]{};std::uint64_t keys[8];for(unsigned i=0;i<8;++i)keys[i]=0x100000001ull+i;ObstacleEntry entries[512]{};unsigned floor_keys[16]{},scratch_floors[16]{};ObstacleRegistry registry{entries,0,512,floor_keys,0,16};AvoidanceScene scene{&registry,actors,keys,8,0};ControllerWorkspace workspace{scratch_segments.data(),1024,0,scratch_actors,8,0,scratch_floors,16,0};unsigned queries=0,updates=0,rejections=0,variants=0,max_ulp=0,physical=0,boundaries=0,moves=0;
  for(unsigned ci=0;ci<cases;++ci){
   const unsigned op=gold.word(),before_path_size=gold.word(),after_path_size=gold.word(),before_registry_size=gold.word(),after_registry_size=gold.word(),query_count=gold.word();require(op<=1,"Controller operation");ControllerPolicy policy;PathController state,expected_state;gold.read(&policy,16);gold.read(&state,56);gold.read(actors,sizeof(actors));const auto before_path=gold.block(before_path_size),before_registry=gold.block(before_registry_size);gold.read(&expected_state,56);NavigationObject expected_object;gold.read(&expected_object,64);const auto after_path=gold.block(after_path_size),after_registry=gold.block(after_registry_size);ControllerResult expected_result{};gold.read(&expected_result,80);gold.block(query_count*16);queries+=query_count;
   PathObject path{};require(before_path.size()>=76,"Controller path header");std::memcpy(&path,before_path.data(),68);std::memcpy(&path.count,before_path.data()+68,4);std::memcpy(&path.owned,before_path.data()+72,4);require(path.count<=1024&&before_path.size()==76+path.count*48,"Controller path budget");path.segments=segments.data();path.capacity=1024;if(path.count)std::memcpy(path.segments,before_path.data()+76,path.count*48);restore_registry(registry,before_registry);ControllerResult result{};ControllerRequest request{&state,&path,&actors[0].object,&native.collision_world,&native.graph,&scene,&policy,&workspace,keys[0]};
   if(op){require(dh2_nav_update_path(&result,&request)==0,"Native controller rejected original input");++updates;}else{const int answer=dh2_nav_is_at_destination(&state,&path);require(answer>=0,"Native destination rejected original input");result.at_destination=answer;}
   auto actual_state=state;actual_state.heading.angle=expected_state.heading.angle;require(same_words(&actual_state,&expected_state,56),"Controller state arithmetic mismatch");
   if(bits(state.heading.angle)!=bits(expected_state.heading.angle)){require(state.heading.angle!=0&&expected_state.heading.angle!=0,"Controller heading signed zero mismatch");const auto a=ordered(state.heading.angle),b=ordered(expected_state.heading.angle);const unsigned ulp=a>b?a-b:b-a;require(ulp<=2,"Controller heading libm difference exceeds two ULP");++variants;max_ulp=std::max(max_ulp,ulp);}
   require(same_words(&actors[0].object,&expected_object,64),"Controller navigation object mismatch");const auto actual_path=path_bytes(path);require(actual_path.size()==after_path.size()&&same_words(actual_path.data(),after_path.data(),after_path.size()),"Controller waypoint ownership mismatch");require(registry_bytes(registry)==after_registry,"Controller registry mismatch");require(same_words(&result,&expected_result,80),"Controller decision/steering mismatch");physical+=result.physical_stop_requested;boundaries+=result.boundary_checked;moves+=op&&policy.update_path&&before_path_size>76;
   if(!rejections&&op){
    const auto saved_state=state;const auto saved_object=actors[0].object;const auto saved_path=path_bytes(path);const auto saved_registry=registry_bytes(registry);const auto saved_result=result;
    auto reject=[&](int status){require(dh2_nav_update_path(&result,&request)==status&&!std::memcmp(&state,&saved_state,56)&&!std::memcmp(&actors[0].object,&saved_object,64)&&path_bytes(path)==saved_path&&registry_bytes(registry)==saved_registry&&!std::memcmp(&result,&saved_result,80),"Controller rejection mutated caller state");++rejections;};
    policy.update_path=2;reject(1);policy.update_path=1;workspace.reserved0=1;reject(1);workspace.reserved0=0;request.controller=nullptr;reject(1);request.controller=&state;result.reserved=1;require(dh2_nav_update_path(&result,&request)==1,"Controller result reserved gate");result=saved_result;++rejections;
    workspace.actor_capacity=0;reject(2);workspace.actor_capacity=8;workspace.floor_capacity=0;reject(2);workspace.floor_capacity=16;request.key=0;reject(1);request.key=keys[0];request.geometry=nullptr;reject(1);request.geometry=&native.collision_world;
   }
  }
  require(gold.at==gold.bytes.size(),"Trailing controller gold");std::cout<<"{\"comparisons\":"<<cases<<",\"updates\":"<<updates<<",\"destination_queries\":"<<cases-updates<<",\"move_calls\":"<<moves<<",\"boundary_checks\":"<<boundaries<<",\"physical_stop_requests\":"<<physical<<",\"floor_queries_compared_by_arm64_oracle\":"<<queries<<",\"host_libm_angle_variants\":"<<variants<<",\"maximum_host_angle_ulp\":"<<max_ulp<<",\"atomic_rejection_checks\":"<<rejections<<",\"physical_backend_reconstructed\":false,\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
