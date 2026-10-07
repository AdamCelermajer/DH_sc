#pragma once
#include "gameplay_camera_runtime_v11.hpp"
namespace dh2::camera {
struct CameraRayV20 {PointV2 start{},end{};};
struct CameraFrustumV20 {PointV2 position{};std::array<std::array<float,4>,6> planes{};};
struct CameraPickingViewV20 {
 CameraViewV11 camera;
 std::int32_t viewport_width{},viewport_height{};
 // Source active render target+c/+10 sizes used by normalized GetWorldCoord;
 // viewport rect+14..20 differences above are separately used by ray/pixels.
 std::int32_t render_width{},render_height{};
 bool scene_manager_present{},camera_present{},orthographic{};
};
// Source camera/driver matrices, before GPU-only orientation/depth conversion.
class GameplayCameraPickingV20 {
 CameraPickingViewV20 view_;
 CameraFrustumV20 frustum_;
 bool ready_{};
public:
 bool bind(CameraPickingViewV20,std::string&);
 bool screen_coord(const PointV2&,std::array<float,2>&,std::string&)const;
 bool screen_pixels(const PointV2&,std::array<std::int32_t,2>&,std::string&)const;
 bool ray(const std::array<std::int32_t,2>&,CameraRayV20&,std::string&)const;
 bool world_coord(const std::array<float,2>&,float,PointV2&,bool& intersects,std::string&)const;
};
}
extern "C" {
void dh2_camera_matrix_product_v20(float*,const float*,const float*);
void dh2_camera_screen_coord_v20(float*,const float*,const float*,const float*);
void dh2_camera_screen_pixel_v20(std::int32_t*,const float*,const std::int32_t*);
void dh2_camera_project_pixel_v20(std::int32_t*,const float*,const float*,const float*,const std::int32_t*);
void dh2_camera_frustum_planes_v20(float*,const float*);
int dh2_camera_plane_line_v20(const float*,const float*,const float*,float*);
int dh2_camera_limited_plane_v20(const float*,const float*,const float*,float*);
int dh2_camera_planes_v20(const float*,const float*,float*,float*);
int dh2_camera_three_planes_v20(const float*,const float*,const float*,float*);
void dh2_camera_ray_v20(float*,const float*,const float*,const std::int32_t*,const std::int32_t*,int);
}
