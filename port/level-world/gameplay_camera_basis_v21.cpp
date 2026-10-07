#include "gameplay_camera_basis_v21.hpp"
#include <cmath>
#include <cstring>
namespace {
float threshold(){std::uint32_t b=0x38d1b717;float f;std::memcpy(&f,&b,4);return f;}
float square(const float*p){return (p[0]*p[0]+p[1]*p[1])+p[2]*p[2];}
}
extern "C" void dh2_camera_basis_v21(float*look,float*up,const float*m){for(unsigned i=0;i<3;++i){look[i]=m[i+4];up[i]=m[i+8];}}
extern "C" int dh2_camera_center_offset_v21(float*out,const float*in){
 out[0]=in[0];out[1]=in[1];out[2]=0.f;
 if(std::fabs(square(out))<threshold())return 1;
 const float height=in[3]-in[4];float negup[3]{-in[7],-in[8],-in[9]};
 const float numerator=(in[0]*negup[0]+in[1]*negup[1])+in[2]*negup[2];
 const float length1=std::sqrt(square(in)),length2=std::sqrt(square(negup));
 const float cosine=numerator/(length1*length2);const float angle=std::fabs(std::acos(cosine));
 const float base=std::tan(angle),upper=std::tan(in[5]*.5f+angle),lower=std::tan(in[6]*-.5f+angle);
 const float first=(base-lower)*height;const float length=std::sqrt(square(out));for(unsigned i=0;i<3;++i)out[i]=out[i]/length;
 const float second=(upper-base)*height;const float scale=(first+second)*.5f-first;
 for(unsigned i=0;i<3;++i)out[i]=out[i]*scale;return 1;
}
namespace dh2::camera {
bool source_camera_basis_v21(const CameraBasisServicesV21&s,PointV2&look,PointV2&up,std::string&e){if(!s.owner){e="Required actual CameraBase owner";return false;}if(!s.camera_present){if(!s.source_vec3_zero){e="Required source initialized Vec3ZERO";return false;}look=up=*s.source_vec3_zero;return true;}if(!s.absolute_camera_matrix){e="Required actual camera absolute matrix";return false;}MatrixV8 m;if(!s.absolute_camera_matrix(m,e))return false;dh2_camera_basis_v21(look.data(),up.data(),m.values);return true;}
bool source_camera_center_offset_v21(const CameraBasisServicesV21&s,float height,PointV2&out,bool&available,std::string&e){available=false;if(!s.owner){e="Required actual CameraBase owner";return false;}if(!s.camera_present)return true;if(!s.absolute_camera_matrix){e="Required source camera absolute transformation";return false;}MatrixV8 m;if(!s.absolute_camera_matrix(m,e))return false;out={m.values[4],m.values[5],0.f};if(std::fabs(square(out.data()))<threshold()){available=true;return true;}if(!s.absolute_camera_position||!s.camera_fov||!s.source_vec3_up){e="Required actual camera position/repeated FOV/source Vec3UP";return false;}PointV2 eye;float first,second;if(!s.absolute_camera_position(eye,e)||!s.camera_fov(first,e)||!s.camera_fov(second,e))return false;const float input[10]{m.values[4],m.values[5],m.values[6],eye[2],height,first,second,(*s.source_vec3_up)[0],(*s.source_vec3_up)[1],(*s.source_vec3_up)[2]};available=dh2_camera_center_offset_v21(out.data(),input)!=0;return true;}
}
