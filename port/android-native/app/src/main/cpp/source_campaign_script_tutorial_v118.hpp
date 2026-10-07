#pragma once
#include <cstdint>
#include <memory>
#include <string>
namespace dh2::loader {struct CheckedCommandBorrowV59;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
bool execute_source_campaign_script_tutorial_v118(const SourceCampaignCandidateBorrowV55&,
 const dh2::loader::CheckedCommandBorrowV59&,bool,std::int32_t,bool&,std::string&);
bool borrow_native_last_open_menu_id_v118(std::int32_t&,std::string&);
bool enqueue_source_tutorial_v118(const std::shared_ptr<void>&,std::int32_t,std::int32_t,std::string&);
bool skip_source_tutorial_v118(const std::shared_ptr<void>&,bool all,std::string&);
bool character_tutorial_operation_v118(const std::shared_ptr<void>&,std::int32_t,
 std::int32_t,const char*,const char*,std::string&);
}
