#pragma once
#include "authored_character_panel_v2.hpp"
#include "authored_menu_application_fields_v3.hpp"
#include "authored_menu_touchscreen_v3.hpp"
#include "authored_menu_deadzones_v3.hpp"
#include <map>
namespace dh2::ui {
// One platform per actual native MenuManager; it borrows Application/TouchScreen
// fields and retains their native owner. No movie/root identity is synthesized.
struct AuthoredCharacterPanelPlatformServicesV4 {
 std::shared_ptr<void> owner;
 AuthoredMenuApplicationFieldsV3* fields{};
 AuthoredMenuTouchScreenV3* touch{};
 std::function<bool(bool&,std::string&)> native_drm;
 std::function<bool(void*,bool(*)(void*,SwfAsGraph&,std::string&),std::string&)> scoped_graph;
 std::function<bool(const char*,std::int32_t&,std::string&)> debug;
 // Projection and bounds execute in the supplied owning Scope. Storage leases
 // remain valid until the callback returns; child order is display-list order.
 std::function<bool(SwfAsGraph&,const std::string&,AuthoredMenuCharacterBorrowV3*&,std::string&)> deadzone_root;
 std::function<bool(SwfAsGraph&,AuthoredMenuCharacterBorrowV3&,AuthoredMenuDeadZoneV3&,std::string&)> absolute_bounds;
 AuthoredMenuLifecycleServicesV1 required_lifecycle;
 std::function<bool(MenuStackV1&,MenuStackRequestV1&,std::string&)> required_stack;
 std::function<bool(std::int16_t,std::int16_t,std::int32_t,std::string&)> release_touch;
 std::function<bool(AuthoredMenuTouchScreenV3&,std::string&)> process_touch;
};
class AuthoredCharacterPanelPlatformV4 {
 struct DeadZones {std::uint8_t registered{};std::vector<AuthoredMenuDeadZoneV3> rectangles;};
 AuthoredCharacterPanelPlatformServicesV4 services_;
 std::map<std::uintptr_t,DeadZones> deadzones_;
 std::string failure_;
 bool bound_{};
 bool lifecycle(AuthoredMenuFieldsV1&,const AuthoredMenuRequestV1&,std::int32_t&,std::string&);
 static int stack_callback(void*,MenuStackV1*,MenuStackRequestV1*);
public:
 explicit AuthoredCharacterPanelPlatformV4(AuthoredCharacterPanelPlatformServicesV4);
 // Platform must outlive panel and remain at a stable address until teardown.
 bool bind(AuthoredCharacterPanelServicesV2&,std::string&);
 const std::vector<AuthoredMenuDeadZoneV3>* deadzones(std::uintptr_t)const noexcept;
 const std::string& failure()const noexcept{return failure_;}
};
}
