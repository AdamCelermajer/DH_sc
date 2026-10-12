#include "floors.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>
namespace dh2::floors {
namespace {
void require(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
unsigned query(void* owner,unsigned id,const float* point){
 auto& world=*static_cast<World*>(owner);if(id>=world.selectors.size())return 0;collision::Result result{};
 return dh2_selector_floor(&result,&world.selectors[id],point)==1;
}
}
bool append(const resources::BresView& view,const scene::Scene& scene,const scene::Instance& instance,unsigned room,World& out,std::string& error){
 error.clear();try{
  require(out.records.size()<512,"Too many native floors");require(instance.node_index<scene.graph.size(),"Invalid floor node");const auto& node=scene.graph[instance.node_index];
  require(node.parent>=0&&unsigned(node.parent)<scene.graph.size(),"Floor has no room parent");
  require(node.user_properties.find("floortypes")==std::string::npos,"Floor property decoding still requires reconstruction");
  auto record=std::make_unique<Record>();record->name=node.name;record->room=room;record->geometry=instance.geometry;record->flags={0,1};
  // constructNode creates a CSceneNode with the authored TRS, then a
  // BaseMeshSceneNode child with identity local TRS. _LoadNavMesh reads
  // properties/name from that CSceneNode, moves the child to its cached
  // absolute position, and copies the child's local rotation/scale.
  const float position[]{node.world[12],node.world[13],node.world[14]},rotation[]{0,0,0,1},scale[]{1,1,1};
  require(dh2_floor_clone_matrix(&record->clone,position,rotation,scale)==0,"Floor clone transform rejected");
  assets::Mesh mesh{};require(dh2_mesh_open(&mesh,&view,instance.geometry)==assets::Error::ok,"Floor mesh rejected");require(mesh.primitives<=10000,"Too many floor mesh buffers");
  std::vector<floor_source::Part> parts;unsigned count=0;
  for(unsigned i=0;i<mesh.primitives;++i){assets::Primitive primitive{};require(dh2_mesh_primitive(&mesh,i,&primitive)==assets::Error::ok,"Floor primitive rejected");floor_source::Part part{};part.primitive_type=primitive.engine_type;
   if(part.primitive_type!=6){parts.push_back(part);continue;}
   require(dh2_mesh_attribute(&mesh,primitive.attributes[0],&part.position)==assets::Error::ok,"Floor position rejected");
   require(primitive.index_width==2,"Original floor selector requires 16-bit indices");part.indices=primitive.indices;part.draw_count=primitive.index_count;
   if(part.position.components>=2&&part.position.components<=4){require(part.draw_count%3==0&&part.draw_count/3<=100000-count,"Invalid floor triangle budget");count+=part.draw_count/3;}
   parts.push_back(part);
  }
  require(count>0,"Floor has no supported triangles");record->triangles.resize(count);unsigned written=0;
  require(dh2_floor_mesh_triangles(record->triangles.data(),count,&written,parts.data(),parts.size(),&record->clone,1)==0&&written==count,"Floor mesh extraction rejected");
  std::copy(mesh.minimum,mesh.minimum+3,record->local.minimum);std::copy(mesh.maximum,mesh.maximum+3,record->local.maximum);
  require(dh2_floor_transform_bounds(&record->world,&record->local,&record->clone)==0&&dh2_floor_source_bounds(&record->bounds,&record->world)==0,"Floor bounds rejected");
  record->octants.resize(count*8+1);record->indices.resize(count*8+1);record->scratch.resize(count);record->selected.resize(count);record->selected_ids.resize(count);
  record->tree={record->octants.data(),record->indices.data(),record->scratch.data(),record->triangles.data(),count,0,0,unsigned(record->octants.size()),unsigned(record->indices.size()),count,15,0,0};
  require(dh2_octree_build(&record->tree,record->triangles.data(),count,15)==0,"Floor octree storage exhausted");record->workspace={record->selected_ids.data(),record->selected.data(),count,0};
  out.records.push_back(std::move(record));return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool build_graph(World& world,std::string& error){
 error.clear();try{
  require(!world.records.empty()&&world.selectors.empty(),"Invalid native floor build state");unsigned total=0;
  for(const auto& record:world.records){require(record->triangles.size()<=100000-total,"Native graph triangle budget exceeded");total+=record->triangles.size();world.selectors.push_back({{&record->tree,&record->clone,1,0},record->bounds,&record->workspace});}
  world.nodes.resize(total*6);world.edges.resize(total*6);world.invalid.resize(total*3);world.validation.resize(total*3);world.validation_floors.resize(total*3);world.floor_graphs.resize(world.records.size());
  world.graph={world.nodes.data(),world.edges.data(),world.invalid.data(),world.validation.data(),0,0,0,0,unsigned(world.nodes.size()),unsigned(world.edges.size()),unsigned(world.invalid.size()),unsigned(world.validation.size()),0,0,0,0,query,&world};
  for(unsigned id=0;id<world.records.size();++id){auto& record=*world.records[id];require(dh2_nav_begin_floor(&world.graph,id)==0,"Native graph floor rejected");
   auto& floor=world.floor_graphs[id];floor={id,record.flags.object,0,world.graph.node_count,world.graph.invalid_count,0,{},{}};
   std::copy(record.bounds.minimum,record.bounds.minimum+3,floor.minimum);std::copy(record.bounds.maximum,record.bounds.maximum+3,floor.maximum);const unsigned first_validation=world.graph.validation_count;
   for(const auto& triangle:record.triangles){navigation::Triangle value;std::memcpy(&value,&triangle,36);require(dh2_nav_triangle(&world.graph,&value,record.flags.object)==0,"Native floor graph rejected");}
   floor.root=world.graph.root;floor.invalid_count=world.graph.invalid_count-floor.invalid_first;
   std::fill(world.validation_floors.begin()+first_validation,world.validation_floors.begin()+world.graph.validation_count,id);
   record.retained.resize(record.triangles.size());require(dh2_floor_raise_triangles(record.retained.data(),record.triangles.data(),record.triangles.size())==0,"Retained floor geometry rejected");
  }
  return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool post_load(World& world,std::string& error){
 error.clear();try{
  require(!world.sewn&&world.floor_graphs.size()==world.records.size()&&!world.records.empty(),"Invalid floor post-load state");
  unsigned room_count=0,boundary_capacity=1;for(unsigned i=0;i<world.records.size();++i){require(world.records[i]->room<512,"Native floor room limit exceeded");room_count=std::max(room_count,world.records[i]->room+1);boundary_capacity=std::max(boundary_capacity,world.floor_graphs[i].invalid_count);}
  std::vector<std::vector<unsigned>> groups(room_count);std::vector<octree::Box> bounds(room_count);
  for(unsigned i=0;i<world.records.size();++i){const auto room=world.records[i]->room;const auto& floor=world.floor_graphs[i];auto& box=bounds[room];
   if(groups[room].empty()){std::copy(floor.minimum,floor.minimum+3,box.minimum);std::copy(floor.maximum,floor.maximum+3,box.maximum);}
   else for(unsigned k=0;k<3;++k){box.minimum[k]=std::min(box.minimum[k],floor.minimum[k]);box.maximum[k]=std::max(box.maximum[k],floor.maximum[k]);}
   groups[room].push_back(i);
  }
  world.first_boundary.resize(boundary_capacity);world.second_boundary.resize(boundary_capacity);world.links.resize(world.records.size()*world.records.size());
  world.sewing={world.first_boundary.data(),world.second_boundary.data(),boundary_capacity,0,world.validation_floors.data(),world.links.data(),0,unsigned(world.links.size())};
  auto sew=[&](unsigned ai,unsigned bi){auto& a=world.floor_graphs[ai];auto& b=world.floor_graphs[bi];if((a.flags|b.flags)&0x04000000)return;
   auto eligible=[&](const navigation::FloorGraph& from,const navigation::FloorGraph& to){unsigned count=0;
    for(unsigned i=0;i<from.invalid_count;++i){const auto& v=world.invalid[from.invalid_first+i];bool accepted=true;
     for(unsigned k=0;k<3;++k){volatile float high=v.position[k]+1,low=v.position[k]-1;accepted&=to.minimum[k]<=high&&to.maximum[k]>=low;}count+=accepted;
    }return count;
   };
   const std::uint64_t na=eligible(a,b),nb=eligible(b,a),pairs=na*nb,internal=na+(na?nb:0);
   const auto edge_capacity=std::uint64_t(world.graph.edge_count)+4*internal+8*pairs,validation_capacity=std::uint64_t(world.graph.validation_count)+2*internal+4*pairs;
   require(edge_capacity<=1000000&&validation_capacity<=1000000,"Native sewing storage budget exceeded");
   if(edge_capacity>world.edges.size())world.edges.resize(edge_capacity);
   if(validation_capacity>world.validation.size()){world.validation.resize(validation_capacity);world.validation_floors.resize(validation_capacity);}
   world.graph.edges=world.edges.data();world.graph.edge_capacity=world.edges.size();world.graph.validation=world.validation.data();world.graph.validation_capacity=world.validation.size();world.sewing.validation_floors=world.validation_floors.data();
   require(dh2_nav_link(&world.graph,&a,&b,&world.sewing)==0,"Native floor sewing rejected");
  };
  for(unsigned room=0;room<room_count;++room){
   if(groups[room].empty())continue;
   for(unsigned other=room+1;other<room_count;++other)if(!groups[other].empty()&&dh2_nav_bounds_overlap(bounds[room].minimum,bounds[room].maximum,bounds[other].minimum,bounds[other].maximum,50)){
    for(auto ai:groups[room])for(auto bi:groups[other]){const auto& a=world.floor_graphs[ai];const auto& b=world.floor_graphs[bi];if(dh2_nav_bounds_overlap(a.minimum,a.maximum,b.minimum,b.maximum,50))sew(ai,bi);}
   }
   const auto& floors=groups[room];for(unsigned i=0;i<floors.size();++i){auto& a=world.floor_graphs[floors[i]];if(a.flags&0x04000000)continue;
    for(unsigned j=i+1;j<floors.size();++j){const auto& b=world.floor_graphs[floors[j]];if(dh2_nav_bounds_overlap(a.minimum,a.maximum,b.minimum,b.maximum,0))sew(floors[i],floors[j]);}
    // Original PFFloor::PostLoad clears the invalid-vector and a second work
    // vector. Retain raw invalid records as an audit archive, close active range.
    a.invalid_count=0;
   }
  }
  world.search_edges.resize(world.graph.edge_count);world.search_offsets.assign(world.graph.node_count+1,0);
  for(unsigned i=0;i<world.graph.edge_count;++i){const auto& edge=world.graph.edges[i];require(edge.from&&edge.from<=world.graph.node_count&&edge.to&&edge.to<=world.graph.node_count,"Invalid sewn graph edge");world.search_edges[i]=i+1;++world.search_offsets[edge.from];}
  for(unsigned i=1;i<world.search_offsets.size();++i)world.search_offsets[i]+=world.search_offsets[i-1];
  std::sort(world.search_edges.begin(),world.search_edges.end(),[&](unsigned a,unsigned b){const auto& x=world.graph.edges[a-1];const auto& y=world.graph.edges[b-1];return x.from!=y.from?x.from<y.from:x.to<y.to;});
  world.search_nodes.resize(world.graph.node_count);world.search_heap.resize(world.graph.edge_count+1);
  world.collision_room_floors=std::move(groups);world.collision_rooms.resize(room_count);world.collision_floors.resize(world.records.size());world.route_traits.resize(world.records.size());
  bool first_room=true;for(unsigned i=0;i<room_count;++i){const auto& ids=world.collision_room_floors[i];world.collision_rooms[i]={bounds[i],ids.data(),unsigned(ids.size()),0};
   if(ids.empty())continue;
   if(first_room){world.collision_world.bounds=bounds[i];first_room=false;}
   else for(unsigned k=0;k<3;++k){world.collision_world.bounds.minimum[k]=std::min(world.collision_world.bounds.minimum[k],bounds[i].minimum[k]);world.collision_world.bounds.maximum[k]=std::max(world.collision_world.bounds.maximum[k],bounds[i].maximum[k]);}
  }
  for(unsigned i=0;i<world.records.size();++i){const auto flags=world.records[i]->flags;world.route_traits[i]={flags.floor,flags.object};world.collision_floors[i]={&world.selectors[i],world.route_traits[i]};}
  world.collision_world.rooms=world.collision_rooms.data();world.collision_world.floors=world.collision_floors.data();world.collision_world.room_count=room_count;world.collision_world.floor_count=world.records.size();
  world.route_graph={&world.graph,world.search_edges.data(),world.search_offsets.data()};world.failed_routes.resize(4096);world.route_internal_path.resize(world.graph.node_count);
  world.route_world={&world.route_graph,world.floor_graphs.data(),world.route_traits.data(),unsigned(world.records.size()),0,world.failed_routes.data(),0,unsigned(world.failed_routes.size())};
  world.route_search={world.search_nodes.data(),world.search_heap.data(),unsigned(world.search_nodes.size()),unsigned(world.search_heap.size())};world.route_workspace={&world.route_search,world.route_internal_path.data(),unsigned(world.route_internal_path.size()),0};
  world.sewn=true;return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
int search_nodes(World& world,unsigned start,unsigned limit,const navigation::SearchTest& test,navigation::SearchResult& result){
 if(!world.sewn)return 1;
 const navigation::SearchGraph graph{&world.graph,world.search_edges.data(),world.search_offsets.data()};
 const navigation::SearchRequest request{&graph,&test,start,limit,1,0};
 navigation::SearchWorkspace workspace{world.search_nodes.data(),world.search_heap.data(),unsigned(world.search_nodes.size()),unsigned(world.search_heap.size())};
 return dh2_nav_search(&result,&request,&workspace);
}
int route(World& world,const float* source,const float* target,unsigned limit,navigation::RouteObject* object,navigation::RouteResult& result){
 if(!world.sewn||!source||!target)return 1;
 navigation::RouteRequest request{&world.route_world,&world.collision_world,object,{},{},limit,1,1,0};std::copy(source,source+3,request.source);std::copy(target,target+3,request.target);
 return dh2_nav_route(&result,&request,&world.route_workspace);
}
int find_path(World& world,navigation::PathObject& object,const float* target,unsigned limit,navigation::RouteResult& result){
 if(!world.sewn||!target)return 1;
 navigation::FindRequest request{&world.route_world,&world.collision_world,&object,&result,&world.route_workspace,{},limit,1,0};std::copy(target,target+3,request.target);
 return dh2_nav_find_path(&request);
}
void clear_route_cache(World& world){world.route_world.failed_count=0;}
bool height(const World& world,const float* point,float& result){
 if(!point)return false;
 for(unsigned i=0;i<3;++i)if(!std::isfinite(point[i]))return false;
 double closest=INFINITY;bool found=false;
 for(const auto& floor:world.selectors){collision::Result hit{};if(dh2_selector_floor(&hit,&floor,point)!=1)continue;const double distance=std::abs(double(hit.point[2])-point[2]);
  if(distance<closest){closest=distance;result=hit.point[2];found=true;}}
 return found;
}
}
