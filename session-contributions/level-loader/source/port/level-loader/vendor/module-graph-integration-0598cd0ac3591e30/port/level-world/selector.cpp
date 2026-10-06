#include "selector.hpp"
#include <cmath>
#include <cstring>
#include <limits>
namespace {
using namespace dh2::selector;
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
Matrix identity(){Matrix m{};m.values[0]=m.values[5]=m.values[10]=m.values[15]=1;m.identity=1;return m;}
void product(Matrix& a,const Matrix& b){
 if(b.identity)return;
 if(a.identity){a=b;return;}
 const Matrix original=a;
 for(unsigned col=0;col<4;++col)for(unsigned row=0;row<4;++row){
  a.values[col*4+row]=add(add(add(mul(original.values[row],b.values[col*4]),mul(original.values[4+row],b.values[col*4+1])),mul(original.values[8+row],b.values[col*4+2])),mul(original.values[12+row],b.values[col*4+3]));
 }
 a.identity=0;
}
bool inverse(Matrix& matrix){
 if(matrix.identity)return true;
 const auto& m=matrix.values;
 // The twelve 2x2 minors and cofactor association follow getInverse, including
 // its float determinant epsilon and reciprocal-multiply output rounding.
 const float t0=sub(mul(m[10],m[15]),mul(m[11],m[14]));
 const float t1=sub(mul(m[15],m[6]),mul(m[14],m[7]));
 const float t2=sub(mul(m[11],m[6]),mul(m[10],m[7]));
 const float t3=sub(mul(m[15],m[2]),mul(m[14],m[3]));
 const float t4=sub(mul(m[11],m[2]),mul(m[10],m[3]));
 const float t5=sub(mul(m[7],m[2]),mul(m[6],m[3]));
 const float t6=sub(mul(m[8],m[13]),mul(m[9],m[12]));
 const float t7=sub(mul(m[13],m[4]),mul(m[12],m[5]));
 const float t8=sub(mul(m[9],m[4]),mul(m[8],m[5]));
 const float t9=sub(mul(m[13],m[0]),mul(m[12],m[1]));
 const float t10=sub(mul(m[9],m[0]),mul(m[8],m[1]));
 const float t11=sub(mul(m[5],m[0]),mul(m[4],m[1]));
 const float determinant=add(sub(add(add(sub(mul(t0,t11),mul(t1,t10)),mul(t2,t9)),mul(t3,t8)),mul(t4,t7)),mul(t5,t6));
 if(std::fabs(determinant)<=0x1.0c6f7ap-20f)return false;
 Matrix result{};auto& r=result.values;
 r[0]=add(sub(mul(t0,m[5]),mul(t1,m[9])),mul(t2,m[13]));
 r[1]=sub(sub(mul(t3,m[9]),mul(t0,m[1])),mul(t4,m[13]));
 r[2]=add(sub(mul(t1,m[1]),mul(t3,m[5])),mul(t5,m[13]));
 r[3]=sub(sub(mul(t4,m[5]),mul(t2,m[1])),mul(t5,m[9]));
 r[4]=sub(sub(mul(t1,m[8]),mul(t0,m[4])),mul(t2,m[12]));
 r[5]=add(sub(mul(t0,m[0]),mul(t3,m[8])),mul(t4,m[12]));
 r[6]=sub(sub(mul(t3,m[4]),mul(t1,m[0])),mul(t5,m[12]));
 r[7]=add(sub(mul(t2,m[0]),mul(t4,m[4])),mul(t5,m[8]));
 r[8]=add(sub(mul(t6,m[7]),mul(t7,m[11])),mul(t8,m[15]));
 r[9]=sub(sub(mul(t9,m[11]),mul(t6,m[3])),mul(t10,m[15]));
 r[10]=add(sub(mul(t7,m[3]),mul(t9,m[7])),mul(t11,m[15]));
 r[11]=sub(sub(mul(t10,m[7]),mul(t8,m[3])),mul(t11,m[11]));
 r[12]=sub(sub(mul(t7,m[10]),mul(t6,m[6])),mul(t8,m[14]));
 r[13]=add(sub(mul(t6,m[2]),mul(t9,m[10])),mul(t10,m[14]));
 r[14]=sub(sub(mul(t9,m[6]),mul(t7,m[2])),mul(t11,m[14]));
 r[15]=add(sub(mul(t8,m[2]),mul(t10,m[6])),mul(t11,m[10]));
 const float reciprocal=div(1,determinant);for(float& value:r)value=mul(value,reciprocal);
 matrix=result;return true;
}
void transform(float* out,const Matrix& m,const float* in){
 float result[3];for(unsigned row=0;row<3;++row)result[row]=add(add(add(mul(in[0],m.values[row]),mul(in[1],m.values[4+row])),mul(in[2],m.values[8+row])),m.values[12+row]);
 std::memcpy(out,result,12);
}
bool valid(const Selector* selector,const Workspace* workspace){
 return selector&&workspace&&selector->tree&&selector->geometry_transformed<=1&&!selector->reserved&&!workspace->reserved&&
  workspace->capacity>=selector->tree->triangle_count&&workspace->capacity<=100000&&
  (!workspace->capacity||(workspace->indices&&workspace->triangles))&&(!selector->node_transform||selector->node_transform->identity<=1);
}
}
extern "C" int dh2_selector_inverse(dh2::selector::Matrix* matrix){
 if(!matrix||matrix->identity>1)return -1;
 return inverse(*matrix);
}
extern "C" std::uint32_t dh2_selector_triangles(dh2::selector::Workspace* workspace,const dh2::selector::Selector* selector,const dh2::octree::Box* query,const dh2::selector::Matrix* extra){
 constexpr auto invalid=std::numeric_limits<std::uint32_t>::max();
 if(!valid(selector,workspace)||!query||(extra&&extra->identity>1))return invalid;
 Matrix output=extra?*extra:identity();dh2::octree::Box local=*query;
 if(selector->node_transform&&!selector->geometry_transformed){
  product(output,*selector->node_transform);
  Matrix invert=*selector->node_transform;inverse(invert); // A singular inverse leaves the copied matrix intact, as makeInverse does.
  if(!invert.identity){
   transform(local.minimum,invert,query->minimum);transform(local.maximum,invert,query->maximum);
   for(unsigned i=0;i<3;++i)if(local.minimum[i]>local.maximum[i]){const float value=local.minimum[i];local.minimum[i]=local.maximum[i];local.maximum[i]=value;}
  }
 }
 const auto count=dh2_octree_box(selector->tree,&local,workspace->indices,workspace->capacity);if(count==invalid)return invalid;
 for(unsigned i=0;i<count;++i){
  auto& triangle=workspace->triangles[i];triangle=selector->tree->triangles[workspace->indices[i]];
  if(!output.identity)for(auto& point:triangle.points)transform(point,output,point);
 }
 return count;
}
extern "C" int dh2_selector_raycast(dh2::collision::Result* result,const dh2::selector::Selector* selector,dh2::selector::Workspace* workspace,const dh2::collision::Ray* ray){
 if(!result||!ray||!valid(selector,workspace))return -1;
 dh2::octree::Box box;
 for(unsigned i=0;i<3;++i){box.minimum[i]=ray->end[i]<ray->start[i]?ray->end[i]:ray->start[i];box.maximum[i]=ray->start[i]<ray->end[i]?ray->end[i]:ray->start[i];}
 const auto count=dh2_selector_triangles(workspace,selector,&box,nullptr);
 if(count==std::numeric_limits<std::uint32_t>::max())return -1;
 return dh2_collision_raycast(result,workspace->triangles,count,ray);
}
extern "C" int dh2_selector_floor(dh2::collision::Result* result,const dh2::selector::Floor* floor,const float* point){
 if(!result||!floor||!point||!valid(&floor->selector,floor->workspace))return -1;
 for(unsigned i=0;i<3;++i)if(!(floor->bounds.minimum[i]<=point[i]&&point[i]<=floor->bounds.maximum[i])){result->hit=0;result->index=std::numeric_limits<std::uint32_t>::max();return 0;}
 dh2::collision::Ray ray;std::memcpy(ray.start,point,12);std::memcpy(ray.end,point,12);ray.start[2]=add(point[2],1000);ray.end[2]=sub(point[2],1000);
 return dh2_selector_raycast(result,&floor->selector,floor->workspace,&ray);
}
extern "C" std::uint32_t dh2_selector_floor_query(void* user,std::uint32_t floor,const float* point){
 const auto* set=static_cast<const dh2::selector::FloorSet*>(user);if(!set||!set->floors||set->reserved||floor>=set->count)return 0;
 dh2::collision::Result result{};return dh2_selector_floor(&result,set->floors+floor,point)>0;
}
