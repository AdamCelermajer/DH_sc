#pragma once
#include <memory>
#include <string>
#include <cstdint>
#include <functional>
namespace model_renderer {
enum class SourceCharacterInteractionMenuV114 {open_merchant,merchant_information,open_cleaner};
struct SourceCharacterInteractionMenuRequestV114 {
 SourceCharacterInteractionMenuV114 operation;
 std::uintptr_t character{};std::int32_t friendly678{-1},oid64{};
 const char* name44{}; //borrowed only for synchronous AdditionalInfos delivery
};
using SourceCharacterInteractionMenuCallbackV114=std::function<bool(const SourceCharacterInteractionMenuRequestV114&,std::string&)>;
bool bind_source_campaign_character_interaction_ui_v114(const std::shared_ptr<void>&,std::shared_ptr<void>,SourceCharacterInteractionMenuCallbackV114,std::string&);
bool source_campaign_character_interact_v114(const std::shared_ptr<void>&,std::uintptr_t,std::uintptr_t,std::string&);
bool source_campaign_character_merchant_stock_v114(const std::shared_ptr<void>&,std::uintptr_t,std::int32_t,std::string&);
//Selected virtual98 dispatch supplied by the existing Item/container owners.
bool source_campaign_noncharacter_interact_v114(const std::shared_ptr<void>&,std::uintptr_t,std::uintptr_t,std::string&);
//Exact selected Handle's IsCharacter virtual, not guessed source type numbers.
bool source_campaign_object_as_character_v114(const std::shared_ptr<void>&,std::uintptr_t,std::uintptr_t&,std::string&);
}
