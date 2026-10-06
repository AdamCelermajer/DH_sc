#pragma once
#include "../game-data/item_presentation_v5.hpp"
#include <functional>
namespace dh2::ui {
struct ItemLanguagePowerServicesV1 {
 std::shared_ptr<void> owner;
 // Clears BOTH actual PowerInfo records and actual item.powers backing.
 // Must retain the same ItemPresentationOwner, no replacement item instance.
 std::function<bool(data::ItemInstanceV1&,std::string&)> clear;
 std::function<bool(data::ItemInstanceV1&,std::int32_t,std::int32_t,std::string&)> add;
};
struct ItemLanguageReceiptV1 {std::uint32_t phase{},powers_rebuilt{};};
// Complete ItemInstance::UpdateLocalization3fc1b4 on the supplied actual item.
// Empty native powers is a genuine zero-iteration branch. Required powered
// owner failure leaves preceding name/stat/req writes intact.
bool item_update_localization_v1(data::ItemInstanceV1&,const data::ItemTextServicesV5&,
 const ItemLanguagePowerServicesV1&,ItemLanguageReceiptV1&,std::string&);
}
