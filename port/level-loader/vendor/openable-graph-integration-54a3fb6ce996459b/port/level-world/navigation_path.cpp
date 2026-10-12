#include "navigation_path.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
namespace {
using namespace dh2::navigation;
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
constexpr unsigned copied=~0u;
bool valid(const PathObject* p){
 if(!p||p->reserved||p->count>p->capacity||(p->capacity&&!p->segments)||p->owned>1||p->owned>p->count)return false;
 for(unsigned i=0;i<p->count;++i){const auto& s=p->segments[i];if((s.edge==copied)!=(i<p->owned))return false;if((!s.edge||s.edge==copied)&&(s.from||s.to))return false;}
 return true;
}
bool graph_valid(const PathObject& p,const Graph* g){
 if(!g||(g->node_count&&!g->nodes)||(g->edge_count&&!g->edges))return false;
 for(unsigned i=0;i<p.count;++i){const auto& s=p.segments[i];if(s.edge&&s.edge!=copied){if(s.edge>g->edge_count)return false;const auto& e=g->edges[s.edge-1];if(!e.from||!e.to||e.from>g->node_count||e.to>g->node_count||s.from!=e.from||s.to!=e.to)return false;}}
 return true;
}
void smooth(PathObject& p,const Graph& g){
 if(p.owned)return;
 auto& first=p.segments[0];const unsigned to=first.to;
 first.edge=copied;first.from=first.to=0;first.weight=1;first.distance=first.clearance=0;p.owned=1;
 if(!to)return;
 const auto& n=g.nodes[to-1];const float half=mul(sub(n.width,p.route.radius),.5f);
 const float dx=mul(n.direction[0],half),dy=mul(n.direction[1],half);
 const float a[]{add(n.position[0],dx),add(n.position[1],dy)};
 const float b[]{sub(n.position[0],dx),sub(n.position[1],dy)};
 LineIntersection hit{};dh2_nav_line_intersection(&hit,a,b,first.source,p.target);
 if(hit.kind==3||hit.kind==5){first.target[0]=hit.point[0];first.target[1]=hit.point[1];}
 else if(hit.kind==4){const float* end=hit.first<0?a:b;first.target[0]=end[0];first.target[1]=end[1];}
}
void calc(PathObject& p){const auto& s=p.segments[0];p.waypoint[0]=sub(s.target[0],s.source[0]);p.waypoint[1]=sub(s.target[1],s.source[1]);p.waypoint[2]=0;}
unsigned past(const PathObject& p){const auto& s=p.segments[0];const float x=mul(sub(p.position[0],s.source[0]),p.waypoint[0]);const float y=mul(sub(p.position[1],s.source[1]),p.waypoint[1]);return add(add(x,y),mul(p.waypoint[2],0))>=0;}
void drop(PathObject& p){if(p.count){p.count=p.owned=0;std::memcpy(p.target,p.position,12);}}
PathSegment segment(const Graph& g,unsigned id,const RouteObject& object){
 PathSegment s{};s.edge=id;
 if(id){const auto& e=g.edges[id-1];s.from=e.from;s.to=e.to;s.weight=e.weight;s.distance=e.distance;s.clearance=e.clearance;std::memcpy(s.source,g.nodes[e.from-1].position,12);std::memcpy(s.target,g.nodes[e.to-1].position,12);}
 else{s.weight=1;std::memcpy(s.source,object.direct_source,12);std::memcpy(s.target,object.direct_target,12);}return s;
}
}
extern "C" int dh2_nav_line_intersection(LineIntersection* out,const float* a,const float* b,const float* c,const float* d){
 if(!out||out->reserved||!a||!b||!c||!d)return 1;
 const float rx=sub(b[0],a[0]),ry=sub(b[1],a[1]),sx=sub(d[0],c[0]),sy=sub(d[1],c[1]);
 const float den=sub(mul(rx,sy),mul(ry,sx)),qy=sub(a[1],c[1]),qx=sub(a[0],c[0]);
 const float first=sub(mul(qy,sx),mul(qx,sy)),second=sub(mul(qy,rx),mul(qx,ry));
 constexpr float eps=0x1.a36e2ep-14f;
 if(den<eps&&den>-eps){out->kind=first<eps&&first>-eps&&std::fabs(sub(first,second))<eps?1:0;return 0;}
 const float inverse=div(1.f,den);out->first=mul(first,inverse);out->second=mul(second,inverse);
 out->point[0]=add(mul(rx,out->first),a[0]);out->point[1]=add(mul(ry,out->first),a[1]);
 const bool on_first=out->first>=0&&out->first<=1,on_second=out->second>=0&&out->second<=1;
 out->kind=on_first?(on_second?5:3):(on_second?4:2);return 0;
}
extern "C" int dh2_nav_smooth_path(PathObject* p,const Graph* g){if(!valid(p)||!p->count||!graph_valid(*p,g))return 1;smooth(*p,*g);return 0;}
extern "C" int dh2_nav_calc_waypoint(PathObject* p){if(!valid(p)||!p->count)return 1;calc(*p);return 0;}
extern "C" int dh2_nav_past_waypoint(unsigned* out,const PathObject* p){if(!out||!valid(p)||!p->count)return 1;*out=past(*p);return 0;}
extern "C" int dh2_nav_drop_path(PathObject* p){if(!valid(p))return 1;drop(*p);return 0;}
extern "C" int dh2_nav_path_length(float* out,const PathObject* p){
 if(!out||!valid(p))return 1;
 float sum=0;
 for(unsigned i=0;i<p->count;++i){const auto& s=p->segments[i];const float x=sub(s.source[0],s.target[0]),y=sub(s.source[1],s.target[1]),z=sub(s.source[2],s.target[2]);sum=add(sum,add(add(mul(x,x),mul(y,y)),mul(z,z)));}*out=sum;return 0;
}
extern "C" int dh2_nav_move_path(MoveResult* out,PathObject* p,const Graph* g){
 if(!out||out->reserved||!valid(p)||!graph_valid(*p,g))return 1;
 out->active=out->past=0;
 if(!p->count){std::memcpy(out->target,p->position,12);return 0;}
 std::memcpy(out->target,p->segments[0].target,12);out->active=1;out->past=past(*p);
 if(!out->past)return 0;
 if(p->owned)--p->owned;
 --p->count;
 if(p->count){std::memmove(p->segments,p->segments+1,p->count*sizeof(PathSegment));smooth(*p,*g);calc(*p);std::memcpy(out->target,p->segments[0].target,12);}
 else{out->active=0;std::memcpy(out->target,p->target,12);}return 0;
}
static int find_path_source_v1(const FindRequest* r,const FindSourceServicesV1* source){
 if(!r||r->reserved||!valid(r->object)||!r->world||!r->world->graph||!r->world->graph->graph||!r->result||!r->workspace||r->pathfinding_enabled>1)return 1;
 auto& p=*r->object;auto& result=*r->result;const auto& graph=*r->world->graph->graph;
 if(!graph_valid(p,&graph)||result.search.reserved||result.search.path_count>result.search.path_capacity||(result.search.path_capacity&&!result.search.path))return 1;
 if(!std::isfinite(p.route.radius)||p.route.radius<0)return 1;
 for(unsigned i=0;i<3;++i)if(!std::isfinite(p.position[i])||!std::isfinite(r->target[i]))return 1;
 const std::uint64_t required=std::uint64_t(graph.node_count)+1;
 if(p.capacity<required||result.search.path_capacity<required)return 2;
 // Route preflight must precede DropPath. Validate it on empty output using
 // the disabled gate: this checks ownership/scratch without querying floors
 // or mutating the failed cache and leaves the real result untouched.
 auto checked=result;checked.search.path_count=0;
 RouteRequest request{r->world,r->geometry,&p.route,{},{},r->limit,0,1,0};
 std::memcpy(request.source,p.position,12);std::memcpy(request.target,r->target,12);
 const int preflight=dh2_nav_route(&checked,&request,r->workspace);if(preflight)return preflight;
 drop(p);std::memcpy(p.target,r->target,12);result.search.path_count=0;request.pathfinding_enabled=r->pathfinding_enabled;
 if(source){std::uint32_t enabled{};if(!source->search_enabled||source->search_enabled(source->context,&enabled)||enabled>1)return 3;request.pathfinding_enabled=enabled;}
 const int status=dh2_nav_route(&result,&request,r->workspace);if(status)return status;
 for(unsigned i=0;i<result.search.path_count;++i)p.segments[p.count++]=segment(graph,result.search.path[i],p.route);
 if(result.found){smooth(p,graph);calc(p);}return 0;
}
extern "C" int dh2_nav_find_path(const FindRequest* r){return find_path_source_v1(r,nullptr);}
extern "C" int dh2_nav_find_path_source_v1(const FindRequest* r,const FindSourceServicesV1* source){if(!source||!source->search_enabled)return 1;return find_path_source_v1(r,source);}
