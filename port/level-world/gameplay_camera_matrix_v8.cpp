#include "gameplay_camera_matrix_v8.hpp"
#include <cmath>
namespace {
void normalize(float p[3]){const float a=p[0]*p[0],b=p[1]*p[1],c=p[2]*p[2];const float ab=a+b;const float square=ab+c;if(square!=0.f){const float inverse=1.f/std::sqrt(square);for(unsigned n=0;n<3;++n)p[n]=p[n]*inverse;}}
float dot(const float a[3],const float b[3]){const float x=a[0]*b[0],y=a[1]*b[1],z=a[2]*b[2];const float xy=x+y;return xy+z;}
}
extern "C" void dh2_camera_look_at_v8(dh2::camera::MatrixV8* out,const float eye[3],const float target[3],const float up[3]){
 float forward[3]{target[0]-eye[0],target[1]-eye[1],target[2]-eye[2]};normalize(forward);
 float side[3]{(-up[1])*forward[2]+up[2]*forward[1],(-up[2])*forward[0]+up[0]*forward[2],(-up[0])*forward[1]+up[1]*forward[0]};normalize(side);
 float vertical[3]{(-forward[1])*side[2]+forward[2]*side[1],(-forward[2])*side[0]+forward[0]*side[2],(-forward[0])*side[1]+forward[1]*side[0]};
 *out={};for(unsigned c=0;c<3;++c){out->values[c*4]=side[c];out->values[c*4+1]=vertical[c];out->values[c*4+2]=forward[c];}
 out->values[12]=-dot(side,eye);out->values[13]=-dot(vertical,eye);out->values[14]=-dot(forward,eye);out->values[15]=1.f;
}
extern "C" void dh2_camera_perspective_v8(dh2::camera::MatrixV8* out,const float input[4]){
 const double half=static_cast<double>(input[0])*0.5;const double y=1.0/std::tan(half);const double x=y/static_cast<double>(input[1]);
 *out={};out->values[0]=static_cast<float>(x);out->values[5]=static_cast<float>(y);
 const float range=input[3]-input[2];out->values[10]=input[3]/range;out->values[11]=1.f;
 const float numerator=(-input[2])*input[3];out->values[14]=numerator/range;
}
