#pragma once
#include "../level-loader/native_gslevel_runtime_v27.hpp"
#include <gslevel_update_v44.hpp>
#include <menu_manager_unload_v58.hpp>
#include <menu_manager_update_v58.hpp>
#include <swf_movie.hpp>
namespace model_renderer {
// GL-thread transport only. Callback receivers are the same native menu
// roster and the same published GS current-Level cell used by gameplay.
bool borrow_native_gs_menu_v27(std::shared_ptr<void>,
 const dh2::ui::LoadingMenuStateServicesV1&,
 std::function<bool(const char*,std::int32_t&,std::string&)>,
 dh2::loader::GSLevelServicesV2<dh2::loader::CanonicalLevelContextV1>&,std::string&);
bool borrow_current_native_level_v27(dh2::loader::CanonicalCurrentLevelBorrowV1&,std::string&);
bool bind_native_menu_application_ec_v58(std::shared_ptr<void> actual_world,
 std::shared_ptr<void> actual_application,std::uint8_t* actual_ec,std::string&);
bool borrow_native_menu_manager_v58(std::shared_ptr<void> actual_world,
 std::shared_ptr<void>& actual_manager,std::string&);
// Synchronous Script Show/Hide Init borrow of the actual registered receiver.
// NULL lookup succeeds as NULL; no alternative menu or identity is fabricated.
bool borrow_native_script_menu_v62(std::shared_ptr<void> actual_world,const char* name,
 std::shared_ptr<void>& actual_receiver,std::uintptr_t& identity,std::string&);
bool bind_native_menu_lifecycle_v58(std::shared_ptr<void> actual_world,
 dh2::ui::MenuManagerUnloadServicesV58,dh2::ui::MenuManagerUpdateServicesV58,std::string&);
bool unload_native_menu_v58(std::shared_ptr<void> actual_world,std::int32_t,std::string&);
bool prepare_native_menu_lifecycle_v93(std::shared_ptr<void> actual_world,std::string&);
bool borrow_native_campaign_drm_v93(std::shared_ptr<void> actual_world,bool&,std::string&);
bool validate_native_campaign_license_v93(std::shared_ptr<void> actual_world,std::int32_t,std::string&);
bool unload_native_menu_resource_v91(std::shared_ptr<void> actual_world,
 std::uintptr_t actual_render,dh2::ui::SwfMovie& actual_movie,std::string&);
bool borrow_native_gs_frame_menu_v58(std::shared_ptr<void> actual_world,
 dh2::loader::GSLevelUpdateServicesV44<dh2::loader::CanonicalLevelContextV1>&,std::string&);
bool refresh_actual_message_caches_stage26_v66(std::string&);
bool complete_actual_hud_refresh_stage26_v66(std::string&);
}
