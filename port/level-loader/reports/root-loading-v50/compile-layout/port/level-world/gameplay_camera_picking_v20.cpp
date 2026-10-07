#include "gameplay_camera_picking_v20.hpp"
#include <cmath>
#include <cstring>
namespace {
float dot(const float*a,const float*b){return (a[0]*b[0]+a[1]*b[1])+a[2]*b[2];}
float source_length(const float*a){return static_cast<float>(std::sqrt(static_cast<double>(dot(a,a))));}
}
extern "C" void dh2_camera_matrix_product_v20(float*out,const float*a,const float*b){
 float tmp[16];for(unsigned c=0;c<4;++c)for(unsigned r=0;r<4;++r)tmp[c*4+r]=((a[r]*b[c*4]+a[4+r]*b[c*4+1])+a[8+r]*b[c*4+2])+a[12+r]*b[c*4+3];std::memcpy(out,tmp,sizeof tmp);
}
extern "C" void dh2_camera_screen_coord_v20(float*out,const float*p,const float*projection,const float*view){
 float m[16];dh2_camera_matrix_product_v20(m,projection,view);float q[4];for(unsigned i=0;i<4;++i)q[i]=((p[0]*m[i]+p[1]*m[4+i])+p[2]*m[8+i])+1.f*m[12+i];const float reciprocal=1.f/q[3];out[0]=reciprocal*q[0];out[1]=reciprocal*q[1];
}
extern "C" void dh2_camera_screen_pixel_v20(std::int32_t*out,const float*p,const std::int32_t*size){
 for(unsigned i=0;i<2;++i){const float a=(p[i]+1.f)*.5f;const float v=a*static_cast<float>(size[i]);
 // Original aeabi_f2iz saturates. Avoid modern C++ undefined conversion on
 // nonfinite/out-of-range inputs while preserving the ARM conversion result.
 out[i]=std::isnan(v)?0:v>=2147483648.f?INT32_MAX:v<=-2147483648.f?INT32_MIN:static_cast<std::int32_t>(v);}
}
extern "C" void dh2_camera_project_pixel_v20(std::int32_t*out,const float*p,const float*projection,const float*view,const std::int32_t*size){
 float m[16];dh2_camera_matrix_product_v20(m,projection,view);float q[4];for(unsigned i=0;i<4;++i)q[i]=((p[0]*m[i]+p[1]*m[4+i])+p[2]*m[8+i])+1.f*m[12+i];
 if(q[3]<0.f){out[0]=out[1]=-10000;return;}const float reciprocal=q[3]!=0.f?1.f/q[3]:1.f;
 const std::int32_t hw=size[0]/2,hh=size[1]/2;
 const float x=(static_cast<float>(hw)*q[0])*reciprocal+.5f;
 const float y=static_cast<float>(hh)*(reciprocal*q[1])+.5f;
 const float fx=std::floor(x),fy=std::floor(y);
 const std::int32_t ix=std::isnan(fx)?0:fx>=2147483648.f?INT32_MAX:fx<=-2147483648.f?INT32_MIN:static_cast<std::int32_t>(fx);
 const std::int32_t iy=std::isnan(fy)?0:fy>=2147483648.f?INT32_MAX:fy<=-2147483648.f?INT32_MIN:static_cast<std::int32_t>(fy);
 out[0]=static_cast<std::int32_t>(static_cast<std::uint32_t>(ix)+static_cast<std::uint32_t>(hw));out[1]=static_cast<std::int32_t>(static_cast<std::uint32_t>(hh)-static_cast<std::uint32_t>(iy));
}
extern "C" void dh2_camera_frustum_planes_v20(float*out,const float*m){
 for(unsigned c=0;c<4;++c){out[2*4+c]=m[c*4+3]+m[c*4];out[3*4+c]=m[c*4+3]-m[c*4];out[5*4+c]=m[c*4+3]-m[c*4+1];out[4*4+c]=m[c*4+3]+m[c*4+1];out[c]=m[c*4+3]-m[c*4+2];out[4+c]=m[c*4+2];}
 for(unsigned i=0;i<6;++i){float*p=out+i*4;const float square=dot(p,p);const float inverse=-(1.f/std::sqrt(square));for(unsigned j=0;j<4;++j)p[j]=p[j]*inverse;}
}
extern "C" int dh2_camera_plane_line_v20(const float*p,const float*start,const float*direction,float*out){const float denominator=dot(p,direction);if(denominator==0.f)return 0;const float t=-(dot(p,start)+p[3])/denominator;for(unsigned i=0;i<3;++i)out[i]=start[i]+t*direction[i];return 1;}
extern "C" int dh2_camera_limited_plane_v20(const float*p,const float*a,const float*b,float*out){
 float direction[3];for(unsigned i=0;i<3;++i)direction[i]=b[i]-a[i];if(!dh2_camera_plane_line_v20(p,a,direction,out))return 0;
 const float length=dot(direction,direction);float distance[3];for(unsigned i=0;i<3;++i)distance[i]=out[i]-a[i];if(!(length>=dot(distance,distance)))return 0;for(unsigned i=0;i<3;++i)distance[i]=out[i]-b[i];return length>=dot(distance,distance);
}
extern "C" int dh2_camera_planes_v20(const float*a,const float*b,float*point,float*direction){
 const float length_a=source_length(a),product=dot(b,a),length_b=source_length(b);const float determinant=length_a*length_b-product*product;
 if(std::fabs(static_cast<double>(determinant))<1e-8)return 0;
 const double inverse=1.0/static_cast<double>(determinant);
 direction[0]=(-a[1])*b[2]+a[2]*b[1];direction[1]=(-a[2])*b[0]+a[0]*b[2];direction[2]=(-a[0])*b[1]+a[1]*b[0];
 const float c1=static_cast<float>(static_cast<double>((-a[3])*length_b+b[3]*product)*inverse);
 const float c2=static_cast<float>(static_cast<double>((-b[3])*length_a+a[3]*product)*inverse);
 for(unsigned i=0;i<3;++i)point[i]=c1*a[i]+c2*b[i];return 1;
}
extern "C" int dh2_camera_three_planes_v20(const float*a,const float*b,const float*c,float*out){float point[3]{},direction[3]{};if(!dh2_camera_planes_v20(a,b,point,direction))return 0;return dh2_camera_plane_line_v20(c,point,direction,out);}
extern "C" void dh2_camera_ray_v20(float*out,const float*position,const float*planes,const std::int32_t*pixel,const std::int32_t*size,int orthographic){
 float left_up[3]{},right_up[3]{},left_down[3]{};
 dh2_camera_three_planes_v20(planes,planes+20,planes+8,left_up);
 dh2_camera_three_planes_v20(planes,planes+20,planes+12,right_up);
 dh2_camera_three_planes_v20(planes,planes+16,planes+8,left_down);
 const float dx=static_cast<float>(pixel[0])/static_cast<float>(size[0]),dy=static_cast<float>(pixel[1])/static_cast<float>(size[1]);
 for(unsigned i=0;i<3;++i){const float horizontal=right_up[i]-left_up[i],vertical=left_down[i]-left_up[i];out[i]=orthographic?((dx-.5f)*horizontal+position[i])+(dy-.5f)*vertical:position[i];out[i+3]=(dx*horizontal+left_up[i])+dy*vertical;}
}
namespace dh2::camera {
bool GameplayCameraPickingV20::bind(CameraPickingViewV20 v,std::string&e){if(v.viewport_width<=0||v.viewport_height<=0){e="Required actual nonzero camera driver viewport";return false;}view_=std::move(v);frustum_.position=view_.camera.eye;float m[16];dh2_camera_matrix_product_v20(m,view_.camera.projection.values,view_.camera.view.values);dh2_camera_frustum_planes_v20(frustum_.planes[0].data(),m);ready_=true;return true;}
bool GameplayCameraPickingV20::screen_coord(const PointV2&p,std::array<float,2>&out,std::string&e)const{if(!ready_){e="Required actual camera driver matrices";return false;}dh2_camera_screen_coord_v20(out.data(),p.data(),view_.camera.projection.values,view_.camera.view.values);return true;}
bool GameplayCameraPickingV20::screen_pixels(const PointV2&p,std::array<std::int32_t,2>&out,std::string&e)const{if(!ready_){e="Required actual camera driver borrow";return false;}if(!view_.scene_manager_present||!view_.camera_present){out={-1000,-1000};return true;}const std::int32_t size[2]{view_.viewport_width,view_.viewport_height};dh2_camera_project_pixel_v20(out.data(),p.data(),view_.camera.projection.values,view_.camera.view.values,size);return true;}
bool GameplayCameraPickingV20::ray(const std::array<std::int32_t,2>&p,CameraRayV20&out,std::string&e)const{if(!ready_){e="Required actual camera/scene driver borrow";return false;}out={};if(!view_.scene_manager_present||!view_.camera_present)return true;const std::int32_t size[2]{view_.viewport_width,view_.viewport_height};float values[6];dh2_camera_ray_v20(values,frustum_.position.data(),frustum_.planes[0].data(),p.data(),size,view_.orthographic);for(unsigned i=0;i<3;++i){out.start[i]=values[i];out.end[i]=values[i+3];}return true;}
bool GameplayCameraPickingV20::world_coord(const std::array<float,2>&p,float height,PointV2&out,bool&intersects,std::string&e)const{if(!ready_||view_.render_width<=0||view_.render_height<=0){e="Required actual camera active-render-target dimensions";return false;}std::int32_t size[2]{view_.render_width,view_.render_height};std::array<std::int32_t,2> pixel;dh2_camera_screen_pixel_v20(pixel.data(),p.data(),size);CameraRayV20 r;if(!ray(pixel,r,e))return false;const float plane[4]{0,0,1,-(height+0.f)};intersects=dh2_camera_limited_plane_v20(plane,r.start.data(),r.end.data(),out.data())!=0;return true;}
}
