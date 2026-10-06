#include "octree.hpp"
#include <cmath>
#include <cstring>
#include <limits>
namespace {
using namespace dh2::octree;
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
bool contained(const dh2::collision::Triangle& t,const Box& box){
 for(const auto& p:t.points)for(unsigned axis=0;axis<3;++axis)if(!(p[axis]>=box.minimum[axis]&&p[axis]<=box.maximum[axis]))return false;
 return true;
}
bool overlaps(const dh2::collision::Triangle& t,const Box& box){
 for(unsigned axis=0;axis<3;++axis)if((t.points[0][axis]<box.minimum[axis]&&t.points[1][axis]<box.minimum[axis]&&t.points[2][axis]<box.minimum[axis])||
 (box.maximum[axis]<t.points[0][axis]&&t.points[1][axis]>box.maximum[axis]&&t.points[2][axis]>box.maximum[axis]))return false;
 return true;
}
bool intersect(const Box& a,const Box& b){
 for(unsigned i=0;i<3;++i)if(!(a.minimum[i]<=b.maximum[i]&&a.maximum[i]>=b.minimum[i]))return false;
 return true;
}
bool construct(Tree& t,unsigned id,unsigned depth){
 if(depth>128)return false;
 auto& n=t.nodes[id-1];
 for(unsigned i=0;i<3;++i)n.box.maximum[i]=n.box.minimum[i]=t.triangles[t.indices[n.first]].points[0][i];
 for(unsigned j=0;j<n.count;++j)for(const auto& p:t.triangles[t.indices[n.first+j]].points)for(unsigned axis=0;axis<3;++axis){
  if(p[axis]>n.box.maximum[axis])n.box.maximum[axis]=p[axis];
  if(p[axis]<n.box.minimum[axis])n.box.minimum[axis]=p[axis];
 }
 float center[3],delta[3];bool point=true;
 constexpr float epsilon=0x1.0c6f7ap-20f;
 for(unsigned i=0;i<3;++i){center[i]=mul(add(n.box.minimum[i],n.box.maximum[i]),.5f);delta[i]=sub(center[i],n.box.maximum[i]);point&=n.box.maximum[i]<=add(n.box.minimum[i],epsilon)&&n.box.maximum[i]>=sub(n.box.minimum[i],epsilon);}
 if(point||n.count<=t.leaf_limit)return true;
 // getEdges uses center +/- (center-max), not the stored min/max values.
 for(unsigned octant=0;octant<8;++octant){
  float corner[3];corner[0]=(octant&4)?sub(center[0],delta[0]):add(center[0],delta[0]);corner[1]=(octant&1)?sub(center[1],delta[1]):add(center[1],delta[1]);corner[2]=(octant&2)?sub(center[2],delta[2]):add(center[2],delta[2]);
  Box cell;for(unsigned axis=0;axis<3;++axis){cell.minimum[axis]=center[axis];cell.maximum[axis]=center[axis];if(center[axis]<corner[axis])cell.maximum[axis]=corner[axis];if(corner[axis]<center[axis])cell.minimum[axis]=corner[axis];}
  const unsigned first=t.index_count;unsigned remaining=0;
  for(unsigned j=0;j<n.count;++j){const auto index=t.indices[n.first+j];
   if(contained(t.triangles[index],cell)){if(t.index_count==t.index_capacity)return false;t.indices[t.index_count++]=index;}
   else t.scratch[remaining++]=index;
  }
  const unsigned selected=t.index_count-first;n.count=remaining;if(remaining)std::memcpy(t.indices+n.first,t.scratch,remaining*4);
  if(selected){
   if(t.node_count==t.node_capacity)return false;
   const unsigned child=++t.node_count;t.nodes[child-1]={};t.nodes[child-1].first=first;t.nodes[child-1].count=selected;n.children[octant]=child;
   if(!construct(t,child,depth+1))return false;
  }
 }
 return true;
}
void select(const Tree& tree,unsigned id,const Box& box,std::uint32_t* out,unsigned capacity,unsigned& count){
 const auto& n=tree.nodes[id-1];if(!intersect(n.box,box))return;
 for(unsigned i=0;i<n.count;++i)if(overlaps(tree.triangles[tree.indices[n.first+i]],box)){
  if(count==capacity)return;
  out[count++]=tree.indices[n.first+i];
 }
 if(count==capacity)return;
 for(unsigned child:n.children)if(child)select(tree,child,box,out,capacity,count);
}
}
extern "C" int dh2_octree_build(dh2::octree::Tree* tree,const dh2::collision::Triangle* triangles,std::uint32_t count,std::uint32_t leaf){
 if(!tree||!tree->nodes||!tree->indices||!tree->scratch||(!triangles&&count)||count>100000||leaf>0x7fffffff||tree->reserved)return 1;
 for(unsigned j=0;j<count;++j)for(const auto& p:triangles[j].points)for(float value:p)if(!std::isfinite(value)||std::fabs(value)>10000000)return 1;
 tree->root=tree->node_count=tree->index_count=0;tree->triangles=triangles;tree->triangle_count=count;tree->leaf_limit=leaf;
 if(!count)return 0;
 if(tree->node_capacity<1||tree->index_capacity<count||tree->scratch_capacity<count)return 2;
 for(unsigned i=0;i<count;++i)tree->indices[i]=i;
 tree->node_count=tree->root=1;tree->index_count=count;tree->nodes[0]={};tree->nodes[0].count=count;
 if(!construct(*tree,1,0)){tree->root=tree->node_count=tree->index_count=0;return 2;}
 return 0;
}
extern "C" std::uint32_t dh2_octree_box(const dh2::octree::Tree* tree,const dh2::octree::Box* box,std::uint32_t* out,std::uint32_t capacity){
 if(!tree||!box||(!out&&capacity)||!tree->nodes||!tree->indices||(!tree->triangles&&tree->triangle_count)||tree->reserved||tree->root>tree->node_count||tree->node_count>tree->node_capacity||tree->index_count>tree->index_capacity)return std::numeric_limits<std::uint32_t>::max();
 unsigned count=0;if(tree->root)select(*tree,tree->root,*box,out,capacity,count);return count;
}
