#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <string>

namespace dh2::item_drop_collision_bounds_v1 {
struct Bounds {
 float minimum[3]{},maximum[3]{};
 bool initialized=false;
};

// Convert an authored absolute node matrix into the ItemAudioVisual root's
// local frame. BDAE roots commonly sit inside translated authoring groups;
// ItemObject collision bounds are relative to the selected visual root.
inline bool relative_to_root(const float root[16],const float node[16],
 float relative[16],std::string& error) {
 for(unsigned i=0;i<16;++i)if(!std::isfinite(root[i])||!std::isfinite(node[i])){
  error="Non-finite source item node matrix";return false;
 }
 if(std::fabs(root[3])>1e-5f||std::fabs(root[7])>1e-5f||
    std::fabs(root[11])>1e-5f||std::fabs(root[15]-1.f)>1e-5f||
    std::fabs(node[3])>1e-5f||std::fabs(node[7])>1e-5f||
    std::fabs(node[11])>1e-5f||std::fabs(node[15]-1.f)>1e-5f){
  error="Non-affine source item node matrix";return false;
 }
 const double a00=root[0],a01=root[4],a02=root[8];
 const double a10=root[1],a11=root[5],a12=root[9];
 const double a20=root[2],a21=root[6],a22=root[10];
 const double det=a00*(a11*a22-a12*a21)-a01*(a10*a22-a12*a20)+
                  a02*(a10*a21-a11*a20);
 if(!std::isfinite(det)||std::fabs(det)<1e-12){
  error="Singular ItemAudioVisual root transform";return false;
 }
 const double inv[3][3]={
  {(a11*a22-a12*a21)/det,(a02*a21-a01*a22)/det,(a01*a12-a02*a11)/det},
  {(a12*a20-a10*a22)/det,(a00*a22-a02*a20)/det,(a02*a10-a00*a12)/det},
  {(a10*a21-a11*a20)/det,(a01*a20-a00*a21)/det,(a00*a11-a01*a10)/det}};
 for(unsigned col=0;col<3;++col)for(unsigned row=0;row<3;++row){
  double value=0.;for(unsigned k=0;k<3;++k)value+=inv[row][k]*node[k*4+col];
  relative[col*4+row]=static_cast<float>(value);
 }
 for(unsigned row=0;row<3;++row){
  double value=0.;for(unsigned k=0;k<3;++k)
   value+=inv[row][k]*(double(node[12+k])-root[12+k]);
  relative[12+row]=static_cast<float>(value);
 }
 relative[3]=relative[7]=relative[11]=0.f;relative[15]=1.f;
 for(unsigned i=0;i<16;++i)if(!std::isfinite(relative[i])){
  error="Source item relative transform overflow";return false;
 }
 error.clear();return true;
}

// CSceneNode::computeBoundingBox transforms each authored mesh box by that
// child node's relative matrix, then unions the resulting world-space boxes.
inline bool include_mesh_box(Bounds& result,const float minimum[3],
 const float maximum[3],const float matrix[16],std::string& error) {
 for(unsigned axis=0;axis<3;++axis)
  if(!std::isfinite(minimum[axis])||!std::isfinite(maximum[axis])||
     minimum[axis]>maximum[axis]){
   error="Invalid source item mesh AABB";return false;
  }
 float low[3]={std::numeric_limits<float>::infinity(),
               std::numeric_limits<float>::infinity(),
               std::numeric_limits<float>::infinity()};
 float high[3]={-low[0],-low[1],-low[2]};
 for(unsigned i=0;i<16;++i)
  if(!std::isfinite(matrix[i])){error="Non-finite source item node matrix";return false;}
 for(unsigned corner=0;corner<8;++corner){
  const float p[3]={corner&1?maximum[0]:minimum[0],
                    corner&2?maximum[1]:minimum[1],
                    corner&4?maximum[2]:minimum[2]};
  const float transformed[3]={
   matrix[0]*p[0]+matrix[4]*p[1]+matrix[8]*p[2]+matrix[12],
   matrix[1]*p[0]+matrix[5]*p[1]+matrix[9]*p[2]+matrix[13],
   matrix[2]*p[0]+matrix[6]*p[1]+matrix[10]*p[2]+matrix[14]};
  for(unsigned axis=0;axis<3;++axis){
   if(!std::isfinite(transformed[axis])){error="Source item AABB transform overflow";return false;}
   low[axis]=std::min(low[axis],transformed[axis]);
   high[axis]=std::max(high[axis],transformed[axis]);
  }
 }
 if(!result.initialized){
  std::copy(low,low+3,result.minimum);std::copy(high,high+3,result.maximum);
  result.initialized=true;
 }else for(unsigned axis=0;axis<3;++axis){
  result.minimum[axis]=std::min(result.minimum[axis],low[axis]);
  result.maximum[axis]=std::max(result.maximum[axis],high[axis]);
 }
 error.clear();return true;
}

// PhysicalObject's ItemObject path uses a circle from the maximum x/y AABB
// width. ItemObject::SetRelativeAABB multiplies x/y by 1.5 when no _colbox_
// node exists; source dimensions are converted from game units by 0.01.
inline bool sensor_radius(const Bounds& bounds,bool colbox_found,
 float scale_x,float scale_y,float& radius,std::string& error) {
 if(!bounds.initialized||!std::isfinite(scale_x)||!std::isfinite(scale_y)){
  error="Source item collision bounds are unavailable";return false;
 }
 const float width=(bounds.maximum[0]-bounds.minimum[0])*std::fabs(scale_x);
 const float height=(bounds.maximum[1]-bounds.minimum[1])*std::fabs(scale_y);
 const float padding=colbox_found?1.f:1.5f;
 radius=std::max(width,height)*.005f*padding;
 if(!std::isfinite(radius)||radius<=0.f||radius>10.f){
  error="Source item bounds produced an invalid sensor radius";return false;
 }
 error.clear();return true;
}

// GameObject::GetInteractionRadius returns half the smaller horizontal
// AABB dimension in source game units. ItemObject::SetRelativeAABB has
// already applied its source 1.5 padding when no _colbox_ is present. These
// coordinates are intentionally not converted to physics meters here.
inline bool interaction_radius(const Bounds& bounds,bool colbox_found,
 float scale_x,float scale_y,float& radius,std::string& error) {
 if(!bounds.initialized||!std::isfinite(scale_x)||!std::isfinite(scale_y)){
  error="Source item interaction bounds are unavailable";return false;
 }
 const float padding=colbox_found?1.f:1.5f;
 const float width=(bounds.maximum[0]-bounds.minimum[0])*std::fabs(scale_x)*padding;
 const float height=(bounds.maximum[1]-bounds.minimum[1])*std::fabs(scale_y)*padding;
 radius=std::min(width,height)*.5f;
 if(!std::isfinite(radius)||radius<0.f){
  error="Source item bounds produced an invalid interaction radius";return false;
 }
 error.clear();return true;
}
} // namespace dh2::item_drop_collision_bounds_v1
