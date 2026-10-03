#include "world.hpp"
#include "navigation_motion.hpp"
#include <limits>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2::navigation;
namespace {
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("Truncated world-route reference");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
 std::vector<unsigned char> block(unsigned count){if(count>10000000)throw std::runtime_error("World-route reference budget exceeded");std::vector<unsigned char> result(count);read(result.data(),count);return result;}
};
void require(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
}
int main(int argc,char** argv){
 if(argc!=4)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x31544f4d,"Invalid world-route reference");const unsigned cases=r.word(),n=r.word(),e=r.word(),count=r.word();require(cases&&cases<=4096&&n==335&&e==838&&count==8,"Invalid authored route budgets");
  const auto nodes=r.block(n*56),edges=r.block(e*20);std::vector<FloorGraph> floors(count);r.read(floors.data(),count*48);require(r.word()==count,"Missing world geometry");
  Reader a(argv[2]),b(argv[3]);dh2::resources::BresView view{};require(dh2_bres_open(&view,a.bytes.data(),a.bytes.size())==dh2::resources::BresError::ok,"Invalid authored BRES");dh2::world::Level level;std::string error;require(dh2::world::load(view,b.bytes.data(),b.bytes.size(),level,error),error.c_str());auto& world=*level.native_floor;
  require(world.graph.node_count==n&&world.graph.edge_count==e&&!std::memcmp(world.graph.nodes,nodes.data(),nodes.size())&&!std::memcmp(world.graph.edges,edges.data(),edges.size())&&!std::memcmp(world.floor_graphs.data(),floors.data(),count*48),"Native asset-loader graph differs from original gold");
  for(unsigned i=0;i<count;++i){const unsigned triangles=r.word();const auto expected=r.block(triangles*36);dh2::octree::Box box;r.read(&box,24);require(world.records[i]->triangles.size()==triangles&&!std::memcmp(world.records[i]->triangles.data(),expected.data(),expected.size())&&!std::memcmp(&world.records[i]->bounds,&box,24),"Native authored geometry differs from route gold");require(world.collision_rooms[i].count==1&&world.collision_rooms[i].floors[0]==i&&!std::memcmp(&world.collision_rooms[i].bounds,&box,24),"Native room geometry differs from caller gold");}

  unsigned totals[4]{},query_records=0,rejections=0;
  for(unsigned ci=0;ci<cases;++ci){
   const unsigned op=r.word(),input_size=r.word(),output_size=r.word(),queries=r.word();require(op<4&&input_size<=1024&&output_size<=1024&&queries<=32,"Invalid motion operation");const auto input=r.block(input_size),expected=r.block(output_size);r.block(queries*16);query_records+=queries;unsigned char actual[76]{};
   if(op<3){require(input_size>=64,"Missing motion traits");std::vector<FloorTraits> traits(count);std::memcpy(traits.data(),input.data(),64);for(unsigned i=0;i<count;++i){world.route_traits[i]=traits[i];world.collision_floors[i].traits=traits[i];}}
   const auto get=[&](unsigned at){unsigned v;require(at+4<=input.size(),"Missing motion word");std::memcpy(&v,input.data()+at,4);return v;};
   if(op==0){require(input_size==88&&output_size==32,"Invalid height record");const unsigned kind=get(64),index=get(68),special=get(72);require(kind<3&&index<count&&special<=1,"Invalid height arguments");float point[3];std::memcpy(point,input.data()+76,12);HeightHit result{13.,{-17.,19.,-23.},0,0,0,0};int status=kind==0?dh2_nav_floor_height(&result,&world.selectors[index],point):kind==1?dh2_nav_room_height(&result,&world.collision_world,index,point,special):dh2_nav_world_height(&result,&world.collision_world,point,special);require(status>=0,"Height rejected reference");std::memcpy(actual,&result,32);
   }else if(op==1){require(input_size==128&&output_size==76,"Invalid position record");float point[3];MotionObject object;MotionPolicy policy;std::memcpy(point,input.data()+64,12);std::memcpy(&object,input.data()+76,40);std::memcpy(&policy,input.data()+116,8);const unsigned actor=get(124);require(actor<=1,"Invalid actor flag");PositionResult result{};require(dh2_nav_validate_position(&result,&world.collision_world,actor?&object:nullptr,point,&policy)==0,"Position rejected reference");std::memcpy(actual,&result,24);std::memcpy(actual+24,point,12);std::memcpy(actual+36,&object,40);
    if(!rejections){const auto saved_result=result;const auto saved_object=object;float saved_point[3];std::memcpy(saved_point,point,12);const auto unchanged=[&](){return !std::memcmp(&result,&saved_result,24)&&!std::memcmp(&object,&saved_object,40)&&!std::memcmp(point,saved_point,12);};
     const auto saved_policy=policy;policy.ignore_height_delta=2;require(dh2_nav_validate_position(&result,&world.collision_world,&object,point,&policy)==1&&unchanged(),"Policy rejection mutated position");policy=saved_policy;++rejections;
     const auto floor=object.floor;object.floor=count;require(dh2_nav_validate_position(&result,&world.collision_world,&object,point,&policy)==1&&!std::memcmp(point,saved_point,12)&&!std::memcmp(&result,&saved_result,24),"Cached-floor rejection mutated position");object.floor=floor;require(unchanged(),"Cached-floor restoration differs");++rejections;
     world.collision_rooms[0].reserved=1;require(dh2_nav_validate_position(&result,&world.collision_world,&object,point,&policy)==1&&unchanged(),"Room rejection mutated position");world.collision_rooms[0].reserved=0;++rejections;
     point[0]=std::numeric_limits<float>::quiet_NaN();const float invalid=point[0];require(dh2_nav_validate_position(&result,&world.collision_world,&object,point,&policy)==1&&!std::memcmp(&point[0],&invalid,4)&&!std::memcmp(&result,&saved_result,24)&&!std::memcmp(&object,&saved_object,40),"Nonfinite position rejection mutated state");std::memcpy(point,saved_point,12);++rejections;
    }
   }else if(op==2){require(input_size==96&&output_size==16,"Invalid direction record");float direction[3],position[3],radius;std::memcpy(direction,input.data()+64,12);std::memcpy(position,input.data()+76,12);std::memcpy(&radius,input.data()+92,4);DirectionRequest request{&world.collision_world,position,radius,get(88),0};unsigned result;require(dh2_nav_validate_direction(&result,direction,&request)==0,"Direction rejected reference");std::memcpy(actual,&result,4);std::memcpy(actual+4,direction,12);
    if(rejections==4){const auto saved=result;float saved_direction[3];std::memcpy(saved_direction,direction,12);request.reserved=1;require(dh2_nav_validate_direction(&result,direction,&request)==1&&result==saved&&!std::memcmp(direction,saved_direction,12),"Direction rejection mutated output");++rejections;}
   }else{require(input_size==32&&output_size==12,"Invalid segment record");float lines[8],output[2]{13.,-17.};std::memcpy(lines,input.data(),32);const unsigned result=dh2_nav_segment_intersect(output,lines,lines+4);std::memcpy(actual,&result,4);std::memcpy(actual+4,output,8);
    if(rejections==5){const float saved[2]{output[0],output[1]};require(dh2_nav_segment_intersect(output,nullptr,lines)==-1&&!std::memcmp(output,saved,8),"Segment rejection mutated output");++rejections;}
   }
   if(std::memcmp(actual,expected.data(),output_size)){std::cerr<<"Motion mismatch case "<<ci<<" operation "<<op<<'\n';return 4;}++totals[op];
  }
  require(r.at==r.bytes.size(),"Trailing motion gold");std::cout<<"{\"cases\":"<<cases<<",\"height_queries\":"<<totals[0]<<",\"position_requests\":"<<totals[1]<<",\"direction_requests\":"<<totals[2]<<",\"segment_requests\":"<<totals[3]<<",\"referenced_floor_queries\":"<<query_records<<",\"atomic_rejection_checks\":"<<rejections<<",\"authored_graph_nodes\":"<<n<<",\"authored_graph_edges\":"<<e<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
