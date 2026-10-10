#pragma once
#include <cstdint>
#include <memory>
#include <string>
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace model_renderer {struct SourceWorldBorrowV61;struct SourceCampaignCandidateBorrowV55;}
namespace dh2::android_ui {
//Transport only: borrows the actual App/PM/World rather than another player
//or script/name state. Stored references to the containing World stay weak.
class SourceScriptUiWorldV97 final {
 std::weak_ptr<model_renderer::SourceWorldBorrowV61> world_;
 std::weak_ptr<application::ApplicationServicesOwnerV5> application_;
 SourceScriptUiWorldV97()=default;
public:
 static bool create(const model_renderer::SourceCampaignCandidateBorrowV55&,std::shared_ptr<SourceScriptUiWorldV97>&,std::string&);
 bool player_character(std::uintptr_t&,std::string&)const;
 bool player_name(std::uintptr_t,std::string&,std::string&)const;
 bool parse_player_name(const std::string&,bool,std::string&,std::string&)const;
 bool style_name(std::int32_t,std::string&,std::string&)const;
 bool before_stop_message(std::string&)const;
 bool before_stop_dialog(std::string&)const;
 bool publish_current_name(const char*,std::string&)const;
 bool send_script_message(bool,std::int32_t,std::int32_t,std::string&)const;
 bool store_controller_global(std::uint8_t,std::string&)const;
};
//Actual source9a5fd8 pointer: NULL before ExecuteScript's original store.
const char* source_running_script_name_v97()noexcept;
}
