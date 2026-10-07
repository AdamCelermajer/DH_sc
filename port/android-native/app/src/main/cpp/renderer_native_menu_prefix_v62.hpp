#pragma once
#include "menu_manager_prefix_v62.hpp"
#include "menu_postmovie_v62.hpp"
#include "world_map_profile_table_v59.hpp"
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace dh2::ui {class MenuStackOwnerV1;}
namespace dh2::ui {struct CapturedMenuLeaseV101;}
namespace model_renderer {
bool prepare_native_menu_process_v104(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 std::function<bool(const char*,std::int32_t&,std::string&)>,std::string&);
bool publish_native_menu_process_movie_v104(std::uint32_t,std::string&);
bool borrow_native_process_menu_directory_v104(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 std::shared_ptr<dh2::ui::MenuStackOwnerV1>&,std::string&);
bool capture_native_process_menu_v104(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const char*,dh2::ui::CapturedMenuLeaseV101&,std::string&);
//Final adapter detach AFTER genuine GS Unload/Level D1 and quiescent gate.
//Keeps process App/directory/roster/resources and never fabricates D0 receipts.
bool retire_native_menu_campaign_v104(const std::shared_ptr<void>& actual_world,std::string&);
bool create_native_process_menu_singleton_v104(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const char*,std::uintptr_t&,std::string&);
bool prepare_actual_gameplay_debug_v67(const std::shared_ptr<void>&,std::string&);
// SAME actual source/demonstration receiver only; the renderer implementing
// this function lends the existing14a8 projection with its real actor lease.
bool borrow_menu_character_ooi_type_v62(const std::shared_ptr<void>& actual_world,std::uintptr_t,std::shared_ptr<void>&,
 const std::int32_t*&,std::string&);
bool bind_native_menu_prefix_v62(std::shared_ptr<void> actual_world,
 dh2::ui::MenuManagerUpdateServicesV58&,std::string&);
bool bind_native_menu_postmovie_v62(std::shared_ptr<void>,
 dh2::ui::MenuManagerUnloadServicesV58&,dh2::ui::MenuManagerUpdateServicesV58&,std::string&);
//Transport lease contains weak World/actual retained receiver references;
//never the owning World itself. Stable HudManagerActor/Player projections
//belong to the canonical receiver, because InfoHUD retains cached target.
using MenuHudWorldOperationV62=std::function<int(const dh2::ui::HudManagerRequest&,dh2::ui::HudManagerResponse&,std::string&)>;
bool source_store_menu_cut_screen_v62(std::uint8_t,std::string&);
bool bind_native_menu_hud_world_v62(std::shared_ptr<void> world,std::shared_ptr<void> transport,
 MenuHudWorldOperationV62,const dh2::ui::HudControlsServicesV62&,std::string&);
dh2::ui::AuthoredMenuFieldsV1* native_map_menu_fields_v62(std::uintptr_t)noexcept;
bool native_map_menu_update_v62(std::uintptr_t,std::string&);
bool source_native_map_icon_v62(std::shared_ptr<void> world,std::uint32_t type,dh2::ui::MenuMapIconV62,std::string&);
bool source_native_loadmenu3_info_v62(std::shared_ptr<void>,std::uintptr_t,std::string&);
bool source_native_loadmenu3_controls_v62(std::shared_ptr<void>,std::uintptr_t,std::string&);
bool source_native_loadmenu3_v62(std::shared_ptr<void>,std::string&);
bool source_native_loadmenu3_singletons_v67(std::shared_ptr<void>,std::string&);
dh2::ui::AuthoredMenuFieldsV1* native_derived_menu_fields_v67(std::uintptr_t)noexcept;
bool native_derived_menu_update_v67(std::uintptr_t,std::string&);
 bool native_worldmap_visible_v68(std::shared_ptr<void>,bool&,std::string&);
// Unload/IsVisible field providers reuse this process-static unowned MenuBase
// if its constructor registered a positive actual DebugHUD movie receiver.
dh2::ui::AuthoredMenuFieldsV1* native_debug_menu_fields_v62(std::uintptr_t)noexcept;
}
