#include "module_floor_graph_v3.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
namespace dh2::world {
namespace {
void require(bool ok,const char* text){if(!ok)throw std::runtime_error(text);}
unsigned query(void* owner,unsigned id,const float* point){
 auto& w=*static_cast<floors::World*>(owner);
 if(id>=w.selectors.size())return 0;
 collision::Result hit{};return dh2_selector_floor(&hit,&w.selectors[id],point)==1;
}
}
bool module_floor_graph_append_v3(floors::World& w,unsigned id,std::string& error){
 error.clear();try{
  require(!w.sewn&&id<w.records.size()&&id==w.floor_graphs.size()&&id==w.selectors.size(),
   "PF floor graph requires ordered SAME unpublished graph floor");
  auto& r=*w.records[id];require(!r.triangles.empty(),"PF graph requires loaded actual floor triangles");
  std::size_t total=0;for(unsigned i=0;i<=id;++i){total+=w.records[i]->triangles.size();require(total<=100000,"PF graph triangle budget exceeded");}
  // Modern vectors replace source growing arrays. Refresh every pointer after
  // allocation; Graph counters/root retain their original values and IDs.
  w.nodes.resize(total*6);w.edges.resize(total*6);w.invalid.resize(total*3);
  w.validation.resize(total*3);w.validation_floors.resize(total*3);
  auto& g=w.graph;g.nodes=w.nodes.data();g.edges=w.edges.data();g.invalid=w.invalid.data();g.validation=w.validation.data();
  g.node_capacity=w.nodes.size();g.edge_capacity=w.edges.size();g.invalid_capacity=w.invalid.size();g.validation_capacity=w.validation.size();
  g.query=query;g.user=&w;
  w.selectors.push_back({{&r.tree,&r.clone,1,0},r.bounds,&r.workspace});
  require(dh2_nav_begin_floor(&g,id)==0,"PF begin_floor rejected");
  w.floor_graphs.push_back({id,r.flags.object,0,g.node_count,g.invalid_count,0,{},{}});
  auto& f=w.floor_graphs.back();std::copy_n(r.bounds.minimum,3,f.minimum);std::copy_n(r.bounds.maximum,3,f.maximum);
  const unsigned validation_first=g.validation_count;
  for(const auto& triangle:r.triangles){navigation::Triangle t{};std::memcpy(&t,&triangle,sizeof t);
   require(dh2_nav_triangle(&g,&t,r.flags.object)==0,"PF source triangle insertion rejected");}
  f.root=g.root;f.invalid_count=g.invalid_count-f.invalid_first;
  std::fill(w.validation_floors.begin()+validation_first,w.validation_floors.begin()+g.validation_count,id);
  r.retained.resize(r.triangles.size());
  require(dh2_floor_raise_triangles(r.retained.data(),r.triangles.data(),r.triangles.size())==0,"PF retained triangle raise rejected");
  return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
}
