#pragma once
#include <cstdint>
#include <string>
namespace dh2::loader {struct CheckedCommandBorrowV59;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
bool execute_source_campaign_script_tutorial_control_v118(const SourceCampaignCandidateBorrowV55&,
 const dh2::loader::CheckedCommandBorrowV59&,bool skip,std::int32_t module,bool& handled,std::string&);
//Root's read-only loan of the SAME process MenuGlobals.last_open_menu cell.
bool borrow_native_last_open_menu_id_v118(std::int32_t&,std::string&);
}
