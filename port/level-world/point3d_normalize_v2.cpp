#include "point3d_normalize_v2.hpp"
#include <cmath>
extern "C" float* dh2_point3d_normalize_v2(float* p){
 if(!p)return nullptr;
 volatile float xx=p[0]*p[0],yy=p[1]*p[1];volatile float xy=xx+yy;volatile float zz=p[2]*p[2];volatile float sum=xy+zz;
 const float length=::sqrtf(sum);
 p[0]=p[0]/length;p[1]=p[1]/length;p[2]=p[2]/length;return p;
}
