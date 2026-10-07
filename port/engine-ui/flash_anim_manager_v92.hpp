#pragma once
#include "combat_flash_swf_v1.hpp"
#include <functional>
namespace dh2::ui {
struct FlashAnimApplicationServicesV92 {
 std::shared_ptr<void> actual_application;
 std::function<bool(std::uintptr_t&,std::string&)> current_level;
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> level_phase;
 std::function<bool(std::uint32_t&,std::string&)> application_dt;
 std::function<bool(std::int32_t&,std::string&)> assertion_mode;
};
// Process host owns ONE instance through all HUD resources. Backend/queue
// survives identity reset/reload; weak movie/projection endpoints do not dangle
// or form a process-manager -> HUD -> process-manager ownership cycle.
class FlashAnimManagerV92 {
 std::shared_ptr<CombatFlashSwfV1> backend_;
 FlashAnimApplicationServicesV92 application_;
 std::shared_ptr<void> menu_update_owner_;
 std::uintptr_t scanned_fx_{};
 bool updating_{};
public:
 FlashAnimManagerV92();
 std::shared_ptr<CombatFlashSwfV1> backend()const noexcept{return backend_;}
 bool bind_hud(const std::shared_ptr<SwfMovie>&,std::weak_ptr<void>,CombatFlashProjectionV1,std::string&);
 bool scan_hud(std::string&);
 bool adopt_successful_scan(const CombatFlashScanReceiptV92&,std::string&);
 bool reset_scan_for_anims(std::uintptr_t actual_menu_fx,std::string&);
 bool discard_unscanned_hud(std::uintptr_t actual_movie,std::string&);
 bool claim_menu_update(std::shared_ptr<void> actual_menu_manager,FlashAnimApplicationServicesV92,std::string&);
 bool release_menu_update(const std::shared_ptr<void>& actual_menu_manager,std::string&);
 bool menu_update_owned()const noexcept{return bool(menu_update_owner_);}
 bool update(std::string&);
 std::uintptr_t scanned_fx()const noexcept{return scanned_fx_;}
};
}
