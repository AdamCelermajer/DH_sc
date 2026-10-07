#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::camera {
struct CameraAnchorActorV75 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 const float* position160{};
 const std::uint32_t* heading_active1b5{}; //Actual native HeadingState.active backing.
 const float* heading1b8{};
};
struct CameraAnchorServicesV75 {
 std::shared_ptr<void> provider;
 std::function<bool(std::uintptr_t,CameraAnchorActorV75&,std::string&)> actor;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> handle_character;
 std::function<bool(std::uintptr_t,bool&,std::string&)> moving_false,attacking;
 std::function<bool(std::uintptr_t,std::array<float,3>&,std::string&)> look_at;
 std::function<bool(std::uint32_t&,std::string&)> application_dt;
 std::function<bool(std::int32_t,const char*,std::string&)> assertion;
};
struct CameraAnchorFieldsV75 {
 std::int32_t type4{};std::uintptr_t actor8{};
 std::array<float,3> position_c{};
 std::uint8_t enabled18{1};
 float max_distance1c{},distance_per_sec20{},threshold24{};
 std::uintptr_t character28{};
 std::array<float,3> previous2c{};
 std::uint8_t alternate38{};std::int32_t state3c{5};
 std::array<float,3> hold_position40{};
 std::uint8_t holding4c{};std::int32_t remaining50{};
 float forward_distance54{};std::array<float,3> last_direction58{};
};
//One actual source Anchor receiver. C1/reset/update do not create a camera,
//sample an animation, own Character fields or advance an independent clock.
class CameraAnchorOwnerV75 final {
 CameraAnchorFieldsV75 fields_;CameraAnchorServicesV75 services_;
 bool forward_{},attempted_{},constructed_{},busy_{},failed_{};std::string failure_;
 bool fail(const char*,std::string&);bool actor(CameraAnchorActorV75&,std::string&);
 bool forward_update(std::string&);
public:
 explicit CameraAnchorOwnerV75(CameraAnchorServicesV75);
 bool construct(std::uintptr_t actor,bool forward,float maximum,float speed,float threshold,std::string&);
 bool reset(std::string&);bool update(std::string&);
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 const CameraAnchorFieldsV75& fields()const noexcept{return fields_;}
 bool constructed()const noexcept{return constructed_;}
};
}
