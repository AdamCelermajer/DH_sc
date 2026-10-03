#include "world.hpp"
#include "navigation_state.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <cstring>
std::vector<std::uint8_t> read(const char* file){std::ifstream f(file,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
void require(bool condition,const char* error){if(!condition)throw std::runtime_error(error);}
std::uint64_t digest(const dh2::navigation::Graph& g){
 std::uint64_t value=0xcbf29ce484222325ull;
 const auto append=[&](const void* p,std::size_t size){for(std::size_t i=0;i<size;i+=4){std::uint32_t word;std::memcpy(&word,static_cast<const unsigned char*>(p)+i,4);if((word&0x7f800000)==0x7f800000&&(word&0x7fffff))word=0x7fc00000;
  for(unsigned j=0;j<4;++j){value^=(word>>(8*j))&255;value*=0x100000001b3ull;}}};
 append(&g.node_count,16);append(g.nodes,g.node_count*sizeof(dh2::navigation::Node));append(g.edges,g.edge_count*sizeof(dh2::navigation::Edge));append(g.invalid,g.invalid_count*sizeof(dh2::navigation::InvalidNode));append(g.validation,g.validation_count*4);append(&g.root,16);return value;
}
int main(int argc,char** argv){
 if(argc!=4)return 2;
 try{
  auto bytes=read(argv[1]),descriptor=read(argv[2]);dh2::resources::BresView view{};require(dh2_bres_open(&view,bytes.data(),bytes.size())==dh2::resources::BresError::ok,"Invalid BRES");
  dh2::world::Level level;std::string error;require(dh2::world::load(view,descriptor.data(),descriptor.size(),level,error),error.c_str());auto& floors=*level.native_floor;
  std::ofstream out(argv[3],std::ios::binary);require(bool(out),"Cannot write floor input reference");auto data=[&](const void* p,unsigned n){if(n)out.write(static_cast<const char*>(p),n);};auto word=[&](unsigned v){data(&v,4);};
  word(0x31494c46);word(floors.records.size());unsigned parts=0,octants=0;
  for(const auto& owned:floors.records){const auto& record=*owned;word(record.room);word(record.geometry);word(record.name.size());data(record.name.data(),record.name.size());
   const float position[]{record.clone.values[12],record.clone.values[13],record.clone.values[14]},rotation[]{0,0,0,1},scale[]{1,1,1};data(position,12);data(rotation,16);data(scale,12);data(&record.local,24);data(&record.flags,8);
   dh2::assets::Mesh mesh{};require(dh2_mesh_open(&mesh,&view,record.geometry)==dh2::assets::Error::ok,"Floor mesh rejected");word(mesh.primitives);
   for(unsigned i=0;i<mesh.primitives;++i){dh2::assets::Primitive primitive{};dh2::assets::Attribute position_stream{};require(dh2_mesh_primitive(&mesh,i,&primitive)==dh2::assets::Error::ok,"Floor primitive rejected");require(dh2_mesh_attribute(&mesh,primitive.attributes[0],&position_stream)==dh2::assets::Error::ok,"Floor position rejected");require(primitive.index_width==2,"Expected 16-bit indices");
    constexpr unsigned widths[]{1,1,2,2,4,4,4};unsigned vertex_bytes=position_stream.vertices?(position_stream.vertices-1)*position_stream.stride+position_stream.components*widths[position_stream.type]:0;
    word(position_stream.vertices);word(primitive.index_count);word(position_stream.type);word(position_stream.components);word(position_stream.stride);word(primitive.engine_type);word(vertex_bytes);word(primitive.index_count*2);data(position_stream.data,vertex_bytes);data(primitive.indices,primitive.index_count*2);++parts;
   }
   data(&record.clone,68);data(&record.world,24);data(&record.bounds,24);word(record.triangles.size());data(record.triangles.data(),record.triangles.size()*36);data(record.retained.data(),record.retained.size()*36);octants+=record.tree.node_count;
  }
  require(bool(out),"Failed writing floor input reference");
  const auto linked_state=dh2::navigation::link_state(floors.graph,floors.floor_graphs.data(),floors.floor_graphs.size(),floors.sewing);
  std::cout<<"{\"floor_records\":"<<floors.records.size()<<",\"mesh_parts\":"<<parts<<",\"triangle_count\":"<<level.floor.size()<<",\"octree_nodes\":"<<octants<<",\"graph_nodes\":"<<floors.graph.node_count<<",\"graph_edges\":"<<floors.graph.edge_count<<",\"graph_invalid_archive\":"<<floors.graph.invalid_count<<",\"graph_validation\":"<<floors.graph.validation_count<<",\"graph_state_fnv1a64\":\""<<std::hex<<digest(floors.graph)<<"\",\"linked_state_fnv1a64\":\""<<dh2::navigation::link_state_digest(linked_state)<<std::dec<<"\",\"neighbour_floor_relations\":"<<floors.sewing.link_count<<",\"floor_sewing_completed\":"<<(floors.sewn?"true":"false")<<",\"source_floor_overrides\":0,\"collision_used_by_height\":true,\"route_search_used_by_movement\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
