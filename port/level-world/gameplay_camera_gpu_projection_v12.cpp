#include "gameplay_camera_gpu_projection_v12.hpp"
#include <cstring>
#include <initializer_list>
namespace {void invert(float& f){std::uint32_t bits;std::memcpy(&bits,&f,4);bits^=0x80000000u;std::memcpy(&f,&bits,4);}}
extern "C" void dh2_camera_gpu_projection_v12(dh2::camera::MatrixV8* matrix,const dh2::camera::GpuProjectionFieldsV12* driver){
 auto* m=matrix->values;matrix->identity_flag=0;
 // Whole CCommonGLDriverBase::fixUpProjectionMatrix6dd9d0.
 // Original fcmpeq(11,0) chooses orthographic conversion; NaN follows the
 // perspective branch just like the original softfloat comparison.
 if(m[11]==0.f){const float twice=m[14]+m[14];m[14]=twice-1.f;m[10]=m[10]+m[10];}
 else{const float twice=m[10]+m[10];m[10]=twice-1.f;m[14]=m[14]+m[14];}
 if(driver->flip_y)for(unsigned i:{1u,5u,9u,13u})invert(m[i]);
 // Whole IVideoDriver::fixUpProjectionMatrixOrientation5a8f88.
 if(driver->render_target_count>1)return;
 const auto orientation=driver->orientation;if(!orientation)return;
 if(orientation==1||orientation==3)for(unsigned column=0;column<4;++column){auto i=column*4;const auto first=m[i];m[i]=m[i+1];m[i+1]=first;}
 if(orientation==2||orientation==3)for(unsigned i:{1u,5u,9u,13u})invert(m[i]);
 if(orientation==1||orientation==2)for(unsigned i:{0u,4u,8u,12u})invert(m[i]);
}
