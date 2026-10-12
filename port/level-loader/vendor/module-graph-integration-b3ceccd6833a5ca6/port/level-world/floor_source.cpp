#include "floor_source.hpp"
#include "../scene-materials/scene.hpp"
#include <cstring>
namespace {
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
bool contains(const char* tags,unsigned length,const char* value,unsigned n){
 for(unsigned i=0;i+n<=length;++i)if(std::memcmp(tags+i,value,n)==0)return true;
 return false;
}
unsigned index(const dh2::floor_source::Part& part,unsigned i){
 if(!part.indices)return i;
 const auto* p=static_cast<const unsigned char*>(part.indices)+i*2;return p[0]|(unsigned(p[1])<<8);
}
}
extern "C" int dh2_floor_clone_matrix(dh2::selector::Matrix* output,const float* position,const float* quaternion,const float* scale){
 if(!output||!position||!quaternion||!scale)return 1;
 dh2::selector::Matrix result{};dh2_node_matrix(result.values,position,quaternion,scale);result.identity=0;*output=result;return 0;
}
extern "C" int dh2_floor_raise_triangles(dh2::collision::Triangle* output,const dh2::collision::Triangle* source,std::uint32_t count){
 if((!output||!source)&&count)return 1;
 if(count>100000)return 1;
 for(unsigned i=0;i<count;++i){auto triangle=source[i];for(auto& point:triangle.points)point[2]=add(point[2],1);output[i]=triangle;}
 return 0;
}
extern "C" int dh2_floor_mesh_triangles(dh2::collision::Triangle* output,std::uint32_t capacity,std::uint32_t* output_count,const dh2::floor_source::Part* parts,std::uint32_t count,const dh2::selector::Matrix* node,std::uint32_t bake){
 if(!output_count||(!parts&&count)||(!output&&capacity)||count>10000||bake>1||(node&&node->identity>1))return 1;
 unsigned total=0;
 for(unsigned p=0;p<count;++p){const auto& part=parts[p];const auto& position=part.position;
  if(part.primitive_type!=6||position.components<2||position.components>4)continue;
  if(!position.data||position.type>6||part.draw_count%3||part.draw_count>300000||(!part.indices&&part.draw_count>position.vertices))return 1;
  float value[4];for(unsigned i=0;i<part.draw_count;++i)if(!dh2_attribute_read(&position,index(part,i),value))return 1;
  if(part.draw_count/3>100000-total)return 1;
  total+=part.draw_count/3;
 }
 if(total>capacity)return 2;
 unsigned written=0;
 for(unsigned p=0;p<count;++p){const auto& part=parts[p];if(part.primitive_type!=6||part.position.components<2||part.position.components>4)continue;
  for(unsigned i=0;i<part.draw_count;i+=3){auto& triangle=output[written++];
   for(unsigned corner=0;corner<3;++corner){float value[4]{};dh2_attribute_read(&part.position,index(part,i+2-corner),value);auto& out=triangle.points[corner];
    if(bake&&node){for(unsigned r=0;r<3;++r)out[r]=add(add(add(mul(value[0],node->values[r]),mul(value[1],node->values[r+4])),mul(value[2],node->values[r+8])),node->values[r+12]);}
    else std::memcpy(out,value,12);
   }
  }
 }
 *output_count=written;return 0;
}
extern "C" int dh2_floor_source_flags(dh2::floor_source::Flags* flags,const char* tags,std::uint32_t length){
 if(!flags||(!tags&&length)||length>4096)return 1;
 if(length){const auto* end=static_cast<const char*>(std::memchr(tags,0,length));if(end)length=end-tags;}
 if(contains(tags,length,"void",4))flags->floor|=0x01000000;
 if(contains(tags,length,"wall",4))flags->floor|=0x02000000;
 if(contains(tags,length,"hole",4))flags->floor|=1;
 if(contains(tags,length,"water",5))flags->floor|=2;
 if(flags->floor&0x03000000)flags->object|=0x07000000;
 return 0;
}
extern "C" int dh2_floor_source_bounds(dh2::octree::Box* output,const dh2::octree::Box* world){
 if(!output||!world)return 1;
 *output=*world;output->maximum[2]=add(world->maximum[2],1000);output->minimum[2]=sub(world->minimum[2],1000);return 0;
}
extern "C" int dh2_floor_transform_bounds(dh2::octree::Box* output,const dh2::octree::Box* local,const dh2::selector::Matrix* matrix){
 if(!output||!local||!matrix||matrix->identity>1)return 1;
 dh2::octree::Box result;
 for(unsigned row=0;row<3;++row){float low=matrix->values[row+12],high=low;
  for(unsigned col=0;col<3;++col){const float a=mul(matrix->values[col*4+row],local->minimum[col]),b=mul(matrix->values[col*4+row],local->maximum[col]);
   if(a<b){low=add(a,low);high=add(b,high);}else{low=add(b,low);high=add(a,high);}
  }
  result.minimum[row]=low;result.maximum[row]=high;
 }
 *output=result;return 0;
}
