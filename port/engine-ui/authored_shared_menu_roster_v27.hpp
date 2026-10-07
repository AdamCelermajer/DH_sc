#pragma once
#include "authored_character_panel_v2.hpp"
#include "authored_menu_application_fields_v3.hpp"
#include "authored_menu_character_projection_v4.hpp"
#include "authored_menu_drag_v68.hpp"
#include "menu_native_event_v1.hpp"
namespace dh2::ui {
struct MenuFSCommandServicesV114;
struct AuthoredSharedMenuServicesV27 {
 std::shared_ptr<void> owner;
 AuthoredMenuApplicationFieldsV3* fields{};
 AuthoredMenuLocalizationServicesV1 localization;
 std::function<bool(const char*,std::int32_t&,std::string&)> debug;
 // Actual manager/touch/render services not implemented by MenuBase itself.
 std::function<bool(MenuStackV1&,MenuStackRequestV1&,std::string&)> remaining;
 AuthoredMenuDragV68::Dimensions screen_dimensions;
 std::function<bool(AuthoredMenuFieldsV1&,const AuthoredMenuRequestV1&,std::int32_t&,std::string&)> process_lifecycle{};
};
// PostLoad MenuBase receivers discovered from actual already-loaded movies.
// Owns receiver fields/projections only; shares ONE MenuManager directory and
// pins each same CPU movie. Player/Save/actor initialization is not required.
class AuthoredSharedMenuRosterV27 {
 struct Receiver;
 struct FSLifetimeV114;
 std::shared_ptr<FSLifetimeV114> fs_lifetime_v114_;
 std::shared_ptr<MenuFSCommandServicesV114> fs_services_v114_;
 std::shared_ptr<MenuStackOwnerV1> stack_;
 AuthoredSharedMenuServicesV27 services_;
 std::vector<std::shared_ptr<Receiver>> receivers_;
 std::string failure_;
 Receiver* receiver(std::uintptr_t)noexcept;
 bool lifecycle(Receiver&,const AuthoredMenuRequestV1&,std::int32_t&,std::string&);
 static int dispatch(void*,MenuStackV1*,MenuStackRequestV1*);
public:
 AuthoredSharedMenuRosterV27(std::shared_ptr<MenuStackOwnerV1>,AuthoredSharedMenuServicesV27);
 ~AuthoredSharedMenuRosterV27();
 bool post_load(SwfMovie&,std::shared_ptr<void> same_movie_lease,std::uint32_t source_flags,std::string&);
 bool route(MenuStackV1&,MenuStackRequestV1&,bool& handled,std::string&);
 MenuStackServicesV1 stack_services()noexcept{return {this,dispatch};}
 bool bind_fs_command_services_v114(MenuFSCommandServicesV114,std::string&);
 bool my_fs_command_v114(std::uintptr_t,const char*,const char*,bool& handled,std::string&);
 bool on_fs_command_v114(std::uintptr_t,const char*,const char*,std::string&);
 bool render_fs_command_v114(std::uintptr_t,const char*,const char*,std::string&);
 bool manager_fs_command_v114(const char*,const char*,std::string&);
 bool invoke(std::uintptr_t actual_menu,const char* method,std::string&);
 bool check(std::uintptr_t actual_menu,std::string&);
 AuthoredMenuFieldsV1* receiver_fields_v59(std::uintptr_t)noexcept;
 bool capture_receiver_v104(std::uintptr_t,std::shared_ptr<void>&,
  std::function<bool(std::uintptr_t,bool&,std::string&)>&,std::string&);
 const std::string* receiver_path_v93(std::uintptr_t)noexcept;
 bool update_receiver_v59(std::uintptr_t,std::string&);
 bool delete_receiver_v59(std::uintptr_t,std::string&);
 bool register_drag_and_drops_v68(std::uintptr_t,std::string&);
 bool hide_receiver_v68(std::uintptr_t,std::string&);
 bool native_event_v68(std::uintptr_t,SwfEvent48&,std::string&);
 void bind_screen_dimensions_v68(AuthoredMenuDragV68::Dimensions dimensions){services_.screen_dimensions=std::move(dimensions);}
 const std::string& error()const noexcept{return failure_;}
};
}
