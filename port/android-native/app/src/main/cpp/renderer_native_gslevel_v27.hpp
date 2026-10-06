#pragma once
#include "../level-loader/native_gslevel_runtime_v27.hpp"
namespace model_renderer {
// GL-thread transport only. Callback receivers are the same native menu
// roster and the same published GS current-Level cell used by gameplay.
bool borrow_native_gs_menu_v27(std::shared_ptr<void>,
 const dh2::ui::LoadingMenuStateServicesV1&,
 std::function<bool(const char*,std::int32_t&,std::string&)>,
 dh2::loader::GSLevelServicesV2<dh2::loader::CanonicalLevelContextV1>&,std::string&);
bool borrow_current_native_level_v27(dh2::loader::CanonicalCurrentLevelBorrowV1&,std::string&);
}
