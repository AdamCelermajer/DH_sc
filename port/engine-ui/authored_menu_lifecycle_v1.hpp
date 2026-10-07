#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <functional>
namespace dh2::ui {
// Actual MenuBase constructor fields: every scalar below starts at zero.
// RegisterMenu sets valid7c only after actual MenuFX::RegisterState delivery.
struct AuthoredMenuFieldsV1 {
 std::uintptr_t identity{},render{},drag5c{};
 std::string name;
 std::uint8_t visible74{},localized75{},valid7c{},owned7d{};
 std::uint32_t counter78{};
};
enum class AuthoredMenuOperationV1 {
 debug_load,debug_query,localize,set_visible,invoke_as,
 store_rollover_event_enabled,store_application_ec,store_igm_opened,
 clear_loading_screen,option_custom_level_running,register_deadzones,
 reset_drag_positions,clear_manager_60,unregister_listener,
 get_saved_language,set_saved_language,save_settings
};
struct AuthoredMenuRequestV1 {
 AuthoredMenuOperationV1 operation;
 const char* text{};
 std::int32_t value{};
};
struct AuthoredMenuLifecycleServicesV1 {
 std::shared_ptr<void> owner;
 // Required synchronous original service. It must borrow the real movie and
 // application owners; received values can return GetLanguage's actual result.
 std::function<bool(AuthoredMenuFieldsV1&,const AuthoredMenuRequestV1&,
                    std::int32_t&,std::string&)> invoke;
};
bool authored_menu_show_v1(AuthoredMenuFieldsV1&,const AuthoredMenuLifecycleServicesV1&,std::string&);
bool authored_menu_hide_v1(AuthoredMenuFieldsV1&,const AuthoredMenuLifecycleServicesV1&,std::string&);
}
