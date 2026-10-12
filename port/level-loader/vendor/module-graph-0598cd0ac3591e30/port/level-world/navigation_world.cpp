#include "navigation_world.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
namespace {
using namespace dh2::navigation;
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
bool inside(const dh2::octree::Box& b,const float* p){for(unsigned k=0;k<3;++k)if(!(b.minimum[k]<=p[k]&&p[k]<=b.maximum[k]))return false;return true;}
bool geometry_valid(const CollisionWorld* w){
 if(!w||(w->room_count&&!w->rooms)||(w->floor_count&&!w->floors))return false;
 for(unsigned i=0;i<w->floor_count;++i)if(!w->floors[i].selector)return false;
 for(unsigned i=0;i<w->room_count;++i){const auto& room=w->rooms[i];if(room.reserved||(room.count&&!room.floors))return false;for(unsigned j=0;j<room.count;++j)if(room.floors[j]>=w->floor_count)return false;}
 return true;
}
bool less(const float* a,const float* b){constexpr float epsilon=0x1.a36e2ep-14f;for(unsigned k=0;k<2;++k)if(!(std::fabs(sub(a[k],b[k]))<epsilon))return a[k]<b[k];return a[2]<b[2];}
unsigned lookup(const Graph& g,unsigned root,const float* p){
 unsigned current=root,candidate=0,steps=0;
 while(current){if(current>g.node_count||++steps>g.node_count)return 0;const auto& node=g.nodes[current-1];if(less(node.position,p))current=node.right;else{candidate=current;current=node.left;}}
 return candidate&&!less(p,g.nodes[candidate-1].position)?candidate:0;
}
void triangle_nodes(unsigned* ids,const Graph& g,const FloorGraph& f,const dh2::collision::Triangle& triangle){
 constexpr unsigned pairs[3][2]{{0,1},{0,2},{1,2}};
 for(unsigned i=0;i<3;++i){float point[3];for(unsigned k=0;k<3;++k)point[k]=mul(add(triangle.points[pairs[i][0]][k],triangle.points[pairs[i][1]][k]),.5f);ids[i]=lookup(g,f.root,point);}
}
float distance_squared(const Node& n,const float* p){float d[3];for(unsigned k=0;k<3;++k)d[k]=sub(n.position[k],p[k]);return add(add(mul(d[0],d[0]),mul(d[1],d[1])),mul(d[2],d[2]));}
unsigned nearest(const Graph& g,const unsigned* ids,const float* p){
 unsigned result=0;float best=std::numeric_limits<float>::max();
 for(unsigned i=0;i<3;++i)if(ids[i]){const float d=distance_squared(g.nodes[ids[i]-1],p);if(best>d){result=ids[i];best=d;}}
 return result;
}
struct Predicates {const RouteWorld* world;const RouteObject* object;unsigned target;};
unsigned goal(void* owner,unsigned id){return id==static_cast<Predicates*>(owner)->target;}
unsigned edge_valid(void* owner,unsigned id){const auto& p=*static_cast<Predicates*>(owner);return !p.object||p.world->graph->graph->edges[id-1].clearance>=p.object->radius;}
unsigned node_valid(void* owner,unsigned id){const auto& p=*static_cast<Predicates*>(owner);const auto& f=p.world->traits[p.world->graph->graph->nodes[id-1].floor];if(!(f.object&1))return 0;return !p.object||!f.type||(f.type&p.object->flags)==f.type;}
}
extern "C" int dh2_nav_world_collision(WorldHit* out,const CollisionWorld* w,const float* point,unsigned special){
 if(!out||out->reserved||!point||special>1||!geometry_valid(w))return -1;
 out->hit=out->collision.hit=0;out->room=out->floor=out->collision.index=~0u;
 if(!inside(w->bounds,point))return 0;
 for(unsigned room_id=0;room_id<w->room_count;++room_id){const auto& room=w->rooms[room_id];if(!inside(room.bounds,point))continue;
  for(unsigned i=0;i<room.count;++i){const unsigned id=room.floors[i];const auto& floor=w->floors[id];if(!special&&(floor.traits.type&0x03000000))continue;
   auto hit=out->collision;const int status=dh2_selector_floor(&hit,floor.selector,point);if(status<0)return -1;
   if(status==1){out->hit=1;out->room=room_id;out->floor=id;out->collision=hit;return 1;}
  }
 }
 return 0;
}
extern "C" int dh2_nav_route(RouteResult* result,const RouteRequest* request,RouteWorkspace* workspace){
 if(!result||!request||!workspace||request->reserved||workspace->reserved||result->search.reserved||request->pathfinding_enabled>1||request->output_requested>1||!request->world||!workspace->search)return 1;
 auto& w=*request->world;if(w.reserved||!w.graph||!w.graph->graph||!w.floors||!w.traits||!geometry_valid(request->geometry)||request->geometry->floor_count!=w.floor_count||w.failed_count>w.failed_capacity||(w.failed_capacity&&!w.failed))return 1;
 const auto& graph=*w.graph->graph;auto& search=result->search;auto& scratch=*workspace->search;
 if((graph.node_count&&!graph.nodes)||(graph.edge_count&&!graph.edges)||search.path_count>search.path_capacity||(search.path_capacity&&!search.path)||(workspace->internal_capacity&&!workspace->internal_path))return 1;
 for(unsigned i=0;i<search.path_count;++i)if(search.path[i]>graph.edge_count)return 1;
 for(unsigned i=0;i<graph.node_count;++i)if(graph.nodes[i].id!=i+1||graph.nodes[i].floor>=w.floor_count||graph.nodes[i].left>graph.node_count||graph.nodes[i].right>graph.node_count)return 1;
 for(unsigned i=0;i<w.floor_count;++i)if(w.floors[i].root>graph.node_count)return 1;
 for(unsigned i=0;i<3;++i)if(!std::isfinite(request->source[i])||!std::isfinite(request->target[i]))return 1;
 if(request->object&&(!std::isfinite(request->object->radius)||request->object->radius<0))return 1;
 const std::uint64_t capacity=std::uint64_t(search.path_count)+std::max(graph.node_count,1u);
 if(scratch.node_capacity<graph.node_count||std::uint64_t(scratch.heap_capacity)<std::uint64_t(graph.edge_count)+1||workspace->internal_capacity<graph.node_count||
  (request->output_requested&&capacity>search.path_capacity)||w.failed_count==w.failed_capacity)return 2;
 result->found=result->source=result->target=result->kind=0;search.found=search.expanded=search.edges_examined=search.candidate_relaxations=search.non_goal_enqueues=0;
 if(!request->pathfinding_enabled)return 0;
 WorldHit source{},target{};if(dh2_nav_world_collision(&source,request->geometry,request->source,0)!=1||dh2_nav_world_collision(&target,request->geometry,request->target,0)!=1)return 0;
 const auto direct=[&](){result->found=1;result->kind=1;if(request->object&&request->output_requested){std::memcpy(request->object->direct_source,request->source,12);std::memcpy(request->object->direct_target,request->target,12);search.path[search.path_count++]=0;}};
 bool same=true;for(unsigned i=0;i<3;++i)for(unsigned k=0;k<3;++k)same&=source.collision.triangle.points[i][k]==target.collision.triangle.points[i][k];
 if(same){direct();return 0;}
 unsigned source_nodes[3],target_nodes[3];triangle_nodes(source_nodes,graph,w.floors[source.floor],source.collision.triangle);triangle_nodes(target_nodes,graph,w.floors[target.floor],target.collision.triangle);
 result->source=nearest(graph,source_nodes,request->source);result->target=nearest(graph,target_nodes,request->target);
 if(!result->source||!result->target)return 0;
 if(result->source==result->target){direct();return 0;}
 for(unsigned i=w.failed_count;i>0;--i){const auto& f=w.failed[i-1];if(f.source==result->source&&f.target==result->target&&f.limit==request->limit)return 0;}
 Predicates p{&w,request->object,result->target};const SearchTest test{goal,edge_valid,node_valid,&p};const SearchRequest search_request{w.graph,&test,result->source,request->limit,request->output_requested,0};
 auto output=search;if(!request->output_requested){output.path=workspace->internal_path;output.path_capacity=workspace->internal_capacity;output.path_count=0;}
 const int status=dh2_nav_search(&output,&search_request,&scratch);if(status)return status;
 search.found=output.found;search.expanded=output.expanded;search.edges_examined=output.edges_examined;search.candidate_relaxations=output.candidate_relaxations;search.non_goal_enqueues=output.non_goal_enqueues;
 result->found=output.found;if(request->output_requested)search.path_count=output.path_count;
 if(!result->found){w.failed[w.failed_count++]={result->source,result->target,request->limit};if(request->output_requested)search.path_count=0;return 0;}
 result->kind=2;
 if(request->output_requested&&search.path_count>1){const unsigned last=search.path[search.path_count-1];const unsigned from=last?graph.edges[last-1].from:0;for(auto id:target_nodes)if(id==from){--search.path_count;break;}}
 return 0;
}
