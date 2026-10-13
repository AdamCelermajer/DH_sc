// gameswf natives of the stats, skills, faery HUD, inventory, save slot, loading, online, quest, achievement,
// merchant and Game Center menus (batch g1-game-app-global-f216b6-11, ELF 0x43eb7c .. 0x440d3c).
// Definitions: game-app/src/g1-game-app-global-f216b6-11.cpp.
//
// gameswf calls every native with one const fn_call&. The Hex-Rays int return is the leftover r0 of the last
// call, so the natives return void. The fn_call accessors and the as_value setters are not in the tree; see the
// PARTIAL notes in the .cpp.
#pragma once

class Quest;

namespace gameswf {
struct fn_call;
}  // namespace gameswf

// Quest filter as taken by Character::SG_GetNumQuests(bool (*)(Quest const*), int).
using QuestPredicate = bool (*)(const Quest*);

// 0x440080 GetQuestFunctor(char const*): the filter for a quest state name ("Active", "Closed", ...).
QuestPredicate GetQuestFunctor(const char* stateName);

// Quest predicates selected by GetQuestFunctor. Owner batch: 0x43b4dc, 0x43b4f0, 0x43b514 and 0x43b53c (not
// defined yet). Hex-Rays types the first three int; they are declared bool here so they match QuestPredicate.
// The owner batch must return bool.
bool IsNonDebugQuest(const Quest* quest);                            // 0x43b4dc
bool IsNonDebugQuestClosedQuest(const Quest* quest);                // 0x43b4f0
bool IsNonDebugQuestActiveButNotClosedQuest(const Quest* quest);    // 0x43b514
bool IsDebugQuest(const Quest* quest);                              // 0x43b53c (thunk to Quest::IsDebug)

// Merchant sell and buy price, shared by NativeMerchantSellItem (0x44058c) and NativeMerchantBuyItem (0x4408d4).
// The original computes factor = (float)percent / 100.0 (double division, stored as float), then
// (int)((float)itemValue * factor), and raises the result to 1 when it is below 1. percent is the merchant
// table's sell column (+0x04) or buy column (+0x08); itemValue is the item's value at ItemInstance +0x54.
inline int MerchantTradePrice(int itemValue, int percent) {
    const float factor = static_cast<float>(static_cast<double>(percent) / 100.0);
    const int price = static_cast<int>(static_cast<float>(itemValue) * factor);
    return price < 1 ? 1 : price;
}

// 0x43eb7c NativeStatsAssignPoint(gameswf::fn_call const&)
void NativeStatsAssignPoint(const gameswf::fn_call& call);
// 0x43ec98 NativeSkillsGetSkillPointsLeft(gameswf::fn_call const&)
void NativeSkillsGetSkillPointsLeft(const gameswf::fn_call& call);
// 0x43ed58 NativeHUDGetIsFaeryUnlocked(gameswf::fn_call const&)
void NativeHUDGetIsFaeryUnlocked(const gameswf::fn_call& call);
// 0x43ee40 NativeHUDSetActiveFaery(gameswf::fn_call const&)
void NativeHUDSetActiveFaery(const gameswf::fn_call& call);
// 0x43ef64 NativeInvUnequipItem(gameswf::fn_call const&)
void NativeInvUnequipItem(const gameswf::fn_call& call);
// 0x43f044 NativeInvGetHasOffHandWeapon(gameswf::fn_call const&)
void NativeInvGetHasOffHandWeapon(const gameswf::fn_call& call);
// 0x43f0f0 NativeInvGetHasTwoHandedWeapon(gameswf::fn_call const&)
void NativeInvGetHasTwoHandedWeapon(const gameswf::fn_call& call);
// 0x43f23c NativeInvTransmuteItem(gameswf::fn_call const&)
void NativeInvTransmuteItem(const gameswf::fn_call& call);
// 0x43f5c8 NativeGetSaveIDsListSize(gameswf::fn_call const&)
void NativeGetSaveIDsListSize(const gameswf::fn_call& call);
// 0x43f630 NativeCreateSaveSlot(gameswf::fn_call const&)
void NativeCreateSaveSlot(const gameswf::fn_call& call);
// 0x43fa58 NativeGetLoadingProgress(gameswf::fn_call const&)
void NativeGetLoadingProgress(const gameswf::fn_call& call);
// 0x43fdbc NativeConnectToServer(gameswf::fn_call const&)
void NativeConnectToServer(const gameswf::fn_call& call);
// 0x43fec4 NativeGetNextStatusMessage(gameswf::fn_call const&)
void NativeGetNextStatusMessage(const gameswf::fn_call& call);
// 0x4401d4 NativeGetQuestCount(gameswf::fn_call const&)
void NativeGetQuestCount(const gameswf::fn_call& call);
// 0x440278 NativeGetNextAchievementMessage(gameswf::fn_call const&)
void NativeGetNextAchievementMessage(const gameswf::fn_call& call);
// 0x44058c NativeMerchantSellItem(gameswf::fn_call const&)
void NativeMerchantSellItem(const gameswf::fn_call& call);
// 0x4408d4 NativeMerchantBuyItem(gameswf::fn_call const&)
void NativeMerchantBuyItem(const gameswf::fn_call& call);
// 0x440c84 NativeHasPushNotification(gameswf::fn_call const&)
void NativeHasPushNotification(const gameswf::fn_call& call);
// 0x440d3c NativeGLXPlayerSignin(gameswf::fn_call const&)
void NativeGLXPlayerSignin(const gameswf::fn_call& call);
