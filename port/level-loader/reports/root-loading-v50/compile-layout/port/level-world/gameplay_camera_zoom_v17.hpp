#pragma once
#include "gameplay_camera_factory_v16.hpp"
#include "event_manager_owner_v12.hpp"
#include <map>
namespace dh2::camera {
struct CameraTouchV17 {std::int16_t x{},y{};std::int32_t finger{};bool pressed{};};
struct CameraMouseV17 {std::int32_t type{},subtype{},x{},y{};float wheel{};};
struct CameraZoomServicesV17 {
 CameraFactoryBackendV16 backend;
 std::function<bool(const events::EventBorrowV12&,CameraTouchV17&,std::string&)> touch;
 std::function<bool(bool&,std::string&)> map_byte1e4;
 std::function<bool(std::int32_t,std::int32_t,bool&,std::string&)> map_point_inside;
};
class GameplayCameraZoomV17:public std::enable_shared_from_this<GameplayCameraZoomV17> {
 struct TouchData {std::int16_t start_x{},start_y{},current_x{},current_y{};};
 CameraZoomServicesV17 services_;
 std::weak_ptr<events::EventManagerOwnerV12> application_events14_;
 std::weak_ptr<GameplayCameraRuntimeV11> camera20_;
 std::uintptr_t source_camera_identity20_{};
 std::map<std::int32_t,TouchData> touches8_;
 std::uint8_t enabled24_{},drag34_{};float inverse_viewport28_{};
 std::int32_t drag_x2c_{},drag_y30_{};PointV2 snapshot38_{};
 bool attach4_{},attach5_{},initialized_{};
 bool viewport_inverse(std::string&);
 bool current(std::shared_ptr<GameplayCameraRuntimeV11>&,std::string&)const;
 static bool receive(void*,const events::EventBorrowV12&,events::EventManagerOwnerV12&,std::int32_t&,std::string&);
public:
 explicit GameplayCameraZoomV17(CameraZoomServicesV17 s):services_(std::move(s)){}
 // Actual Application+14 heap manager, not Level's EventManager base.
 bool initialize(std::shared_ptr<events::EventManagerOwnerV12>,std::string&);
 bool set_camera(std::shared_ptr<GameplayCameraRuntimeV11>,std::string&);
 bool reset_zoom(std::string&);
 void reset_finger_map()noexcept{touches8_.clear();}
 bool on_event(const events::EventBorrowV12&,std::int32_t& consumed,std::string&);
 bool on_mouse(const CameraMouseV17&,std::int32_t& consumed,std::string&);
 bool close(std::string&);
 std::uint8_t& source_enabled24()noexcept{return enabled24_;}
 std::uintptr_t source_camera_identity20()const noexcept{return source_camera_identity20_;}
 float source_inverse_viewport28()const noexcept{return inverse_viewport28_;}
 std::size_t finger_count()const noexcept{return touches8_.size();}
};
}
