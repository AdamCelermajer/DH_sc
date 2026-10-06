#include "navigation.hpp"
#include <cmath>
#include <cstring>
namespace {
using namespace dh2::navigation;
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
float length(const float* a){return std::sqrt(add(add(mul(a[0],a[0]),mul(a[1],a[1])),mul(a[2],a[2])));}
void normalize(float* a){const float n=length(a);for(unsigned i=0;i<3;++i)a[i]=div(a[i],n);}
void midpoint(float* p,const float* a,const float* b){for(unsigned i=0;i<3;++i)p[i]=mul(add(a[i],b[i]),.5f);}
// Original CompPos comparator: epsilon applies to X and Y only. Preserve the
// red-black tree and insertion order: approximate equivalence is not transitive.
bool less(const float* a,const float* b){
 constexpr float epsilon=0x1.a36e2ep-14f; // original 0x38d1b717
 for(unsigned i=0;i<2;++i)if(!(std::fabs(sub(a[i],b[i]))<epsilon))return a[i]<b[i];
 return a[2]<b[2];
}
Node& node(Graph& g,unsigned id){return g.nodes[id-1];}
bool red(Graph& g,unsigned id){return id&&node(g,id).red;}
unsigned find(Graph& g,const float* p){
 unsigned current=g.root,candidate=0;
 while(current){const auto& n=node(g,current);if(less(n.position,p))current=n.right;else{candidate=current;current=n.left;}}
 return candidate&&!less(p,node(g,candidate).position)?candidate:0;
}
void rotate_left(Graph& g,unsigned x){
 const unsigned y=node(g,x).right;auto& a=node(g,x);auto& b=node(g,y);
 a.right=b.left;if(b.left)node(g,b.left).parent=x;b.parent=a.parent;
 if(!a.parent)g.root=y;else if(x==node(g,a.parent).left)node(g,a.parent).left=y;else node(g,a.parent).right=y;
 b.left=x;a.parent=y;
}
void rotate_right(Graph& g,unsigned x){
 const unsigned y=node(g,x).left;auto& a=node(g,x);auto& b=node(g,y);
 a.left=b.right;if(b.right)node(g,b.right).parent=x;b.parent=a.parent;
 if(!a.parent)g.root=y;else if(x==node(g,a.parent).right)node(g,a.parent).right=y;else node(g,a.parent).left=y;
 b.right=x;a.parent=y;
}
void insert(Graph& g,unsigned id){
 unsigned parent=0,current=g.root;bool left=true;
 while(current){parent=current;left=less(node(g,id).position,node(g,current).position);current=left?node(g,current).left:node(g,current).right;}
 auto& n=node(g,id);n.parent=parent;n.red=1;
 if(!parent)g.root=id;else if(left)node(g,parent).left=id;else node(g,parent).right=id;
 while(id!=g.root&&red(g,node(g,id).parent)){
  unsigned p=node(g,id).parent,grand=node(g,p).parent;
  if(p==node(g,grand).left){
   const unsigned uncle=node(g,grand).right;
   if(red(g,uncle)){node(g,p).red=0;node(g,uncle).red=0;node(g,grand).red=1;id=grand;}
   else{if(id==node(g,p).right){id=p;rotate_left(g,id);p=node(g,id).parent;grand=node(g,p).parent;}node(g,p).red=0;node(g,grand).red=1;rotate_right(g,grand);}
  }else{
   const unsigned uncle=node(g,grand).left;
   if(red(g,uncle)){node(g,p).red=0;node(g,uncle).red=0;node(g,grand).red=1;id=grand;}
   else{if(id==node(g,p).left){id=p;rotate_right(g,id);p=node(g,id).parent;grand=node(g,p).parent;}node(g,p).red=0;node(g,grand).red=1;rotate_left(g,grand);}
  }
 }
 node(g,g.root).red=0;
}
unsigned create_node(Graph& g,const float* a,const float* b,const float* normal,bool force=false){
 float p[3];midpoint(p,a,b);if(const auto found=find(g,p))return found;
 float direction[3];for(unsigned i=0;i<3;++i)direction[i]=sub(b[i],a[i]);
 float side[]{sub(mul(direction[1],normal[2]),mul(direction[2],normal[1])),
               sub(mul(direction[2],normal[0]),mul(direction[0],normal[2])),
               sub(mul(direction[0],normal[1]),mul(direction[1],normal[0]))};
 if(!force){normalize(side);float probe[3];for(unsigned i=0;i<3;++i)probe[i]=add(p[i],side[i]);
  if(!g.query(g.user,g.floor,probe))return 0;
  for(unsigned i=0;i<3;++i)probe[i]=sub(p[i],side[i]);
  if(!g.query(g.user,g.floor,probe))return 0;}
 const unsigned id=++g.node_count;auto& n=node(g,id);n={};n.id=id;n.floor=g.floor;std::memcpy(n.position,p,12);
 n.length=n.width=length(direction);normalize(direction);std::memcpy(n.direction,direction,12);insert(g,id);return id;
}
unsigned create_edge(Graph& g,unsigned from,unsigned to,std::uint32_t flags){
 if((flags&0x02000000)||!from||!to)return 0;
 unsigned id=0;for(unsigned i=0;i<g.edge_count;++i)if(g.edges[i].from==from&&g.edges[i].to==to){id=i+1;break;}
 if(!id){id=++g.edge_count;g.edges[id-1]={from,to,0,0,0};}
 auto& edge=g.edges[id-1];const auto& a=node(g,from);const auto& b=node(g,to);
 float delta[3];for(unsigned i=0;i<3;++i)delta[i]=sub(b.position[i],a.position[i]);
 edge.distance=edge.weight=length(delta);edge.clearance=b.width<a.width?b.width:a.width;return id;
}
bool valid(const Graph* g){return g&&g->nodes&&g->edges&&g->invalid&&g->validation&&g->query&&g->reserved==0&&
 g->node_count<=g->node_capacity&&g->edge_count<=g->edge_capacity&&g->invalid_count<=g->invalid_capacity&&g->validation_count<=g->validation_capacity&&
 g->floor_start<=g->node_count&&(!g->root||(g->root>g->floor_start&&g->root<=g->node_count));}
}
extern "C" int dh2_nav_begin_floor(dh2::navigation::Graph* g,std::uint32_t floor){
 if(!valid(g))return 1;
 g->floor=floor;g->root=0;g->floor_start=g->node_count;return 0;
}
extern "C" int dh2_nav_triangle(dh2::navigation::Graph* g,const dh2::navigation::Triangle* t,std::uint32_t flags){
 if(!valid(g)||!t)return 1;
 for(const auto& p:t->points)for(float x:p)if(!std::isfinite(x))return 1;
 if(flags&0x01000000)return 0;
 if(g->node_capacity-g->node_count<3||g->edge_capacity-g->edge_count<6||g->invalid_capacity-g->invalid_count<3||g->validation_capacity-g->validation_count<3)return 2;
 const float* a=t->points[0];const float* b=t->points[1];const float* c=t->points[2];
 float u[3],v[3];for(unsigned i=0;i<3;++i){u[i]=sub(b[i],a[i]);v[i]=sub(c[i],a[i]);}
 // Original triangle normal uses cross(c-a,b-a), with negation/add ordering.
 float normal[]{add(mul(v[2],-u[1]),mul(u[2],v[1])),add(mul(v[0],-u[2]),mul(u[0],v[2])),add(mul(v[1],-u[0]),mul(u[1],v[0]))};
 const unsigned ids[]{create_node(*g,a,b,normal),create_node(*g,a,c,normal),create_node(*g,b,c,normal)};
 const unsigned pairs[][2]{{0,1},{0,2},{1,2}};
 for(const auto& pair:pairs){create_edge(*g,ids[pair[0]],ids[pair[1]],flags);g->validation[g->validation_count++]=create_edge(*g,ids[pair[1]],ids[pair[0]],flags);}
 const float* sources[]{a,a,b};const float* destinations[]{b,c,c};
 for(unsigned i=0;i<3;++i)if(!ids[i]){
  auto& out=g->invalid[g->invalid_count++];midpoint(out.position,sources[i],destinations[i]);std::memcpy(out.source,sources[i],12);std::memcpy(out.destination,destinations[i],12);std::memcpy(out.normal,normal,12);
  unsigned j=0;for(unsigned k=0;k<3;++k)if(k!=i)out.neighbours[j++]=ids[k];
 }
 return 0;
}

extern "C" int dh2_nav_bounds_overlap(const float* amin,const float* amax,const float* bmin,const float* bmax,float margin){
 if(!amin||!amax||!bmin||!bmax||!std::isfinite(margin)||margin<0)return 0;
 for(unsigned i=0;i<3;++i){
  if(!std::isfinite(amin[i])||!std::isfinite(amax[i])||!std::isfinite(bmin[i])||!std::isfinite(bmax[i]))return 0;
  if(!(amin[i]<=add(bmax[i],margin))||!(amax[i]>=sub(bmin[i],margin)))return 0;
 }
 return 1;
}
extern "C" int dh2_nav_link(Graph* g,FloorGraph* a,FloorGraph* b,LinkWorkspace* w){
 if(!valid(g)||!a||!b||a==b||a->floor==b->floor||!w||w->reserved||!w->first||!w->second||!w->validation_floors||!w->links||w->link_count>w->link_capacity)return 1;
 auto check=[&](const FloorGraph& f){
  if((f.invalid_count&&(f.flags&0x01000000))||f.start>g->node_count||f.invalid_first>g->invalid_count||f.invalid_count>g->invalid_count-f.invalid_first||
   (f.root&&(f.root<=f.start||f.root>g->node_count||node(*g,f.root).floor!=f.floor)))return false;
  for(unsigned i=0;i<3;++i)if(!std::isfinite(f.minimum[i])||!std::isfinite(f.maximum[i])||f.minimum[i]>f.maximum[i])return false;
  for(unsigned i=0;i<f.invalid_count;++i){const auto& v=g->invalid[f.invalid_first+i];
   for(float x:v.position)if(!std::isfinite(x))return false;
   for(float x:v.source)if(!std::isfinite(x))return false;
   for(float x:v.destination)if(!std::isfinite(x))return false;
   for(auto id:v.neighbours)if(id>g->node_count||(id&&node(*g,id).floor!=f.floor))return false;
  }
  return true;
 };
 if(!check(*a)||!check(*b))return 1;
 if((a->flags|b->flags)&0x04000000)return 0;
 auto eligible=[](const InvalidNode& v,const FloorGraph& other){
  for(unsigned i=0;i<3;++i)if(!(other.minimum[i]<=add(v.position[i],1))||!(other.maximum[i]>=sub(v.position[i],1)))return false;
  return true;
 };
 unsigned na=0,nb=0;
 for(unsigned i=0;i<a->invalid_count;++i)na+=eligible(g->invalid[a->invalid_first+i],*b);
 for(unsigned i=0;i<b->invalid_count;++i)nb+=eligible(g->invalid[b->invalid_first+i],*a);
 // Worst-case reservation is a bounded modern allocation policy. It preserves
 // atomic rejection without executing original reallocators mid-operation.
 const std::uint64_t pairs=std::uint64_t(na)*nb,internal=std::uint64_t(na)+(na?nb:0);
 if(na>w->capacity||nb>w->capacity||std::uint64_t(na)+nb>g->node_capacity-g->node_count||
  4*internal+8*pairs>g->edge_capacity-g->edge_count||2*internal+4*pairs>g->validation_capacity-g->validation_count||
  (pairs&&w->link_capacity-w->link_count<2))return 2;
 auto select=[&](FloorGraph& f,const FloorGraph& other,BoundaryNode* out){
  g->floor=f.floor;g->root=f.root;g->floor_start=f.start;unsigned n=0;
  for(unsigned i=0;i<f.invalid_count;++i){const unsigned id=f.invalid_first+i;const auto& v=g->invalid[id];if(!eligible(v,other))continue;
   out[n++]={id,(f.flags&0x01000000)?0:create_node(*g,v.source,v.destination,v.normal,true)};
  }
  f.root=g->root;return n;
 };
 select(*a,*b,w->first);select(*b,*a,w->second);
 auto link=[&](const FloorGraph& owner,unsigned from,unsigned to){return create_edge(*g,from,to,owner.flags);};
 auto validate=[&](const FloorGraph& owner,unsigned edge){w->validation_floors[g->validation_count]=owner.floor;g->validation[g->validation_count++]=edge;};
 auto internal_links=[&](const FloorGraph& owner,const BoundaryNode& boundary){
  const auto& v=g->invalid[boundary.invalid];for(auto neighbour:v.neighbours){link(owner,boundary.node,neighbour);validate(owner,link(owner,neighbour,boundary.node));}
 };
 auto floor_link=[&](unsigned from,unsigned to){
  for(unsigned i=0;i<w->link_count;++i)if(w->links[i].from==from&&w->links[i].to==to)return;
  w->links[w->link_count++]={from,to};
 };
 for(unsigned i=0;i<na;++i){const auto& va=w->first[i];internal_links(*a,va);
  for(unsigned j=0;j<nb;++j){const auto& vb=w->second[j];if(!i)internal_links(*b,vb);
   if(!va.node||!vb.node)return 1; // malformed disabled-floor inputs cannot arise from CreateNodes
   const auto& an=node(*g,va.node);const auto& bn=node(*g,vb.node);float delta[3];for(unsigned k=0;k<3;++k)delta[k]=sub(an.position[k],bn.position[k]);
   const float width=add(an.width,bn.width),distance=add(add(mul(delta[0],delta[0]),mul(delta[1],delta[1])),mul(delta[2],delta[2]));
   if(mul(width,width)>distance&&std::fabs(delta[2])<100){
    const auto& av=g->invalid[va.invalid];const auto& bv=g->invalid[vb.invalid];
    for(auto neighbour:bv.neighbours){link(*a,va.node,neighbour);validate(*b,link(*b,neighbour,va.node));}
    for(auto neighbour:av.neighbours){link(*b,vb.node,neighbour);validate(*a,link(*a,neighbour,vb.node));}
   }
   // Original neighbour-floor sets are updated even if the distance test fails.
   floor_link(a->floor,b->floor);floor_link(b->floor,a->floor);
  }
 }
 return 0;
}
