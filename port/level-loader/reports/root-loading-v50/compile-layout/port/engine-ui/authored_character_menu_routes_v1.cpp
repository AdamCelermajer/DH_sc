#include "authored_character_menu_bridge_v1.hpp"
#include <cstring>
namespace dh2::ui {
namespace {
#include "reference/character-movie-startup-v1/native_callback_names_v1.inc"
constexpr const char* query_names[]{
 "NativeEquipSkill","NativeGetPlayerStats","NativeGetSkillDetails",
 "NativeHUDGetActiveFaery","NativeHUDGetIsFaeryUnlocked","NativeHUDSetActiveFaery",
 "NativeInvAutoEquipSlot","NativeInvDropItem","NativeInvEquipItem",
 "NativeInvGetEquipedItem","NativeInvGetHasOffHandWeapon","NativeInvGetHasTwoHandedWeapon",
 "NativeInvGetItemDetails","NativeInvGetItemsListForSlot","NativeInvGetPlayerGold",
 "NativeInvTransmuteItem","NativeInvUnequipItem","NativeSaveGame",
 "NativeSkillGetEquipedSkillsIDs","NativeSkillsGetSkillPointsLeft",
 "NativeSkillsTrainSkill","NativeStatsAssignPoint","NativeSwapEquipment"
};
bool same(const char* a,const char* b){return a&&std::strcmp(a,b)==0;}
}
// Pure authored callback catalog stays with engine-ui. The dispatch owner
// lives beside its World query providers, preserving one-way library linkage.
AuthoredCharacterMenuRouteV1 AuthoredCharacterMenuBridgeV1::route_for(const char* name) noexcept {
 for(const auto* query:query_names)if(same(name,query))return AuthoredCharacterMenuRouteV1::query;
 if(same(name,"NativeReloadSkills"))return AuthoredCharacterMenuRouteV1::reload;
 if(same(name,"NativePushMenu")||same(name,"NativePopMenu")||same(name,"NativePopAllAbove")||same(name,"NativePopAllMenus"))return AuthoredCharacterMenuRouteV1::navigation;
 if(same(name,"NativeChangeRolloverInputBehavior"))return AuthoredCharacterMenuRouteV1::rollover;
 return AuthoredCharacterMenuRouteV1::application;
}
std::vector<std::string> AuthoredCharacterMenuBridgeV1::native_actions(){
 std::vector<std::string> names(std::begin(source_native_callbacks),std::end(source_native_callbacks));
 names.emplace_back("NativeLoadSettings");names.emplace_back("NativeGetStringFromSymbol");return names;
}
}
