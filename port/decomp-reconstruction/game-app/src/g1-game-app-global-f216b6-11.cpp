// Batch g1-game-app-global-f216b6-11 (<global>, part 12 of 22): the gameswf natives of the stats, skills, faery HUD,
// inventory, save slot, loading, server, status, quest, achievement, merchant, push-notification and Game Center
// menus, ELF 0x43eb7c .. 0x440d3c. Declarations: dh2/global/native_stats_inventory_merchant.hpp.
//
// What is reconstructed and what is not:
//   - GetQuestFunctor (0x440080) is complete. DebugSwitches::GetInstance() loads the switches and returns them,
//     which is the same sequence as the Hex-Rays load(&s_inst) followed by GetSwitch(&s_inst, name).
//   - MerchantTradePrice (header) is the arithmetic shared by the sell and buy natives. It is tested in
//     game-app/tests/g1-game-app-global-f216b6-11_test.cpp.
//   - Every other native is PARTIAL. Each one reads its arguments and writes its result through gameswf: the
//     fn_call argument accessors (argument count, type tag, is_number, to_int, to_number, to_bool, to_string,
//     to_tu_string), the setters (set_bool, set_double, set_string), drop_refs and the as_object member setter.
//     None of that is in the tree, so these bodies are shells. Each comment gives the original's steps, so the
//     body can be filled in when the gameswf API lands.
//
// Shared facts used by the comments:
//   - Argument layout. The arguments are read from the top of the call's stack. "top" is the last argument and
//     "top-1" the one below it. The type tag is the byte at +1 of a 12-byte value, and 2 is the tag the natives
//     test for. The result value is the fn_call's first pointer; the natives that write it either set it or
//     clear it to undefined (drop_refs, then tag 0).
//   - NativeGetPlayerChar(int, bool) (0x43c388) returns the player's Character, or null when the index is out of
//     range. It is not declared in the tree yet, so every native that calls it is PARTIAL.
//   - Application singleton: Singleton<Application>::s_inst, the object ApplicationGetInstance() returns
//     (dh2/global/ctrl_camera.hpp). Application::GetCurrentLevel() is the current Level or null.
//   - OnlineSingleton<OnlineGameState>::GetInstance() is the online game state. Its fields at +0x27, +0x30, +0x34
//     and +0x50 are not declared in net-session-online/online_game_state.hpp.

#include <cstring>
#include <string>

#include "dh2/DebugSwitches.hpp"
#include "dh2/global/native_stats_inventory_merchant.hpp"

// 0x440080 GetQuestFunctor(char const*)
// Complete. The original tests the DisplayAllQuestInLog switch, then DisplayAllDebugOnlyQuestInLog, and then the
// state name. "Active" (or any name other than "Closed") selects the active-but-not-closed filter. "Closed"
// selects the closed filter. Both switches are read through DebugSwitches::GetInstance(), which loads the
// switch file before it returns the instance.
QuestPredicate GetQuestFunctor(const char* stateName) {
    if (DebugSwitches::GetInstance()->GetSwitch(std::string("DisplayAllQuestInLog"))) {
        return IsNonDebugQuest;
    }
    if (DebugSwitches::GetInstance()->GetSwitch(std::string("DisplayAllDebugOnlyQuestInLog"))) {
        return IsDebugQuest;
    }
    if (std::strcmp(stateName, "Active") == 0 || std::strcmp(stateName, "Closed") != 0) {
        return IsNonDebugQuestActiveButNotClosedQuest;
    }
    return IsNonDebugQuestClosedQuest;
}

// 0x43eb7c NativeStatsAssignPoint(gameswf::fn_call const&)
// PARTIAL. Needs exactly two arguments. The top one must have tag 2 and must be the stat type (to_int, 0..3). The
// one below it must be a number, and it is the player index. The original calls NativeGetPlayerChar(index, false).
// Stat 0 calls Character::IncStatStr, 1 IncStatDex, 2 IncStatEnd and 3 IncStatNrg. Other values do nothing. Then
// the result is cleared (drop_refs, tag 0). Missing: the argument accessors, is_number, to_int, drop_refs.
void NativeStatsAssignPoint(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ec98 NativeSkillsGetSkillPointsLeft(gameswf::fn_call const&)
// PARTIAL. Needs exactly one argument with tag 2, read as the player index (to_int). When NativeGetPlayerChar(index,
// false) gives a player, the result is CharProperties::PROPS_GetInt(157, false) as a double (the skill points
// left property, reached through the Character's CharProperties base at +0x560). Otherwise the result is not set.
// Missing: the argument accessors and set_double.
void NativeSkillsGetSkillPointsLeft(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ed58 NativeHUDGetIsFaeryUnlocked(gameswf::fn_call const&)
// PARTIAL. Needs exactly two arguments. The top one is the faery id (to_int) and the one below it the player index
// (to_int). For a player from NativeGetPlayerChar(index, false), the result is
// Character::SG_IsFaerieUnlocked(faery, -1) as a bool. Missing: the argument accessors and set_bool.
void NativeHUDGetIsFaeryUnlocked(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ee40 NativeHUDSetActiveFaery(gameswf::fn_call const&)
// PARTIAL. Needs exactly two arguments. The top one is the faery id (to_int) and the one below it the player index.
// For a player from NativeGetPlayerChar(index, false), the original calls Character::ChangeFaery(faery). When the
// current level exists (Application::GetCurrentLevel()), it calls Level::PlaceFaeryAndFollowers(nullptr). The
// original reads GetCurrentLevel twice, which is harmless because the getter has no side effects. Then the result
// is cleared (drop_refs, tag 0). Missing: the argument accessors and drop_refs.
void NativeHUDSetActiveFaery(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ef64 NativeInvUnequipItem(gameswf::fn_call const&)
// PARTIAL. Needs exactly two arguments. The top one is the equipment slot (to_int) and the one below it the player
// index. For a player from NativeGetPlayerChar(index, false), the original calls the Character's virtual
// UnEquipItemFromSlot(slot) (vtable +0x144). The native writes no result. Missing: the argument accessors.
void NativeInvUnequipItem(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43f044 NativeInvGetHasOffHandWeapon(gameswf::fn_call const&)
// PARTIAL. Needs exactly one argument with tag 2, read as the player index. The result is the bool
// ItemInventory::HasOffHandWeapon() of the player's inventory (the ItemInventory base at +0x37c). It is false when the
// player is missing. The call must be qualified as ItemInventory:: because Character may hide the name. Missing: the
// argument accessors and set_bool.
void NativeInvGetHasOffHandWeapon(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43f0f0 NativeInvGetHasTwoHandedWeapon(gameswf::fn_call const&)
// PARTIAL. Same argument rule as NativeInvGetHasOffHandWeapon. The result is ItemInventory::HasTwoHander(false) of
// the player's inventory, and false when the player is missing. Missing: the argument accessors and set_bool.
void NativeInvGetHasTwoHandedWeapon(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43f23c NativeInvTransmuteItem(gameswf::fn_call const&)
// PARTIAL. Needs exactly two arguments. The top one is the item index (to_int) and the one below it the player index.
// The original checks that the value below the top is a number. For a player from NativeGetPlayerChar(index, false), it
// calls Character::INV_TransmuteItem(item, false), then Character::INV_UpdateSkin(), then clears the result (drop_refs,
// tag 0). The local std::vector<ItemInventory::Item> in the original is zeroed and destroyed with no effect. Missing:
// the argument accessors and drop_refs.
void NativeInvTransmuteItem(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43f5c8 NativeGetSaveIDsListSize(gameswf::fn_call const&)
// PARTIAL. Takes no arguments. The original fills a std::vector<std::string> with PlayerSavegame::SG_GetSavegameList
// (the vector and a false flag), sets the result to the number of entries, and destroys the vector. The original's
// 32-bit entry is 24 bytes, so the count is the byte span divided by 24. Missing: PlayerSavegame.hpp (no header yet
// declares SG_GetSavegameList) and set_double.
void NativeGetSaveIDsListSize(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43f630 NativeCreateSaveSlot(gameswf::fn_call const&)
// PARTIAL. Needs exactly two arguments, both to_tu_string. The top one is the save name and the one below it the
// character class name. The class name is matched against Arrays::CharacterTable member names (count and names
// from pydata). A class ID that IsPlayableClassID accepts (0x439c34) gets the next free save slot. The original then
// builds a PlayerSavegame in that slot (ctor with slot, 1, 0), zeroes the static m_difficultyLevel, generates seeds,
// sets the save date, runs the per-difficulty loop over Arrays::GameDifficulties::size, saves, sets the result to the
// slot number, and destroys the savegame. The listing also stores the name and the class ID in a local record (v15)
// that is not shown being saved with the savegame.
// Two points are unclear in the listing. The result of IsPlayableClassID is passed on to
// SG_GetNextFreeSlot. The fields written by the difficulty loop have no declaration. Missing: PlayerSavegame.hpp,
// the pydata tables, the argument accessors and set_double.
void NativeCreateSaveSlot(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43fa58 NativeGetLoadingProgress(gameswf::fn_call const&)
// PARTIAL. Takes no arguments. When Application::GetCurrentLevel() is set, the result is Level::m_loadProgressPercent
// (Level +0x30). Otherwise a local OnlineGameState is constructed and destroyed, and the result is 0 when the online
// state at OnlineSingleton<OnlineGameState>::GetInstance() +0x34 is 3, and 100 for any other value. Missing: the
// OnlineGameState +0x34 field and set_double.
void NativeGetLoadingProgress(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43fdbc NativeConnectToServer(gameswf::fn_call const&)
// PARTIAL. Needs exactly one argument, read with to_number as the room index. The room list is a std::vector<tRoomInfo>
// (24 bytes each, the divisor in the listing). When the online state at +0x34 is 2, the list comes from the CMatching
// singleton, through its virtual at vtable +0x90 (the call that fills the list). Otherwise it is copied from the
// OnlineGameState tRoomInfo vector at +0x50. When the index is in range (0 .. count-1), the original stores it at
// OnlineGameState +0x30 and writes 103 to +0x14, the state machine's state word, directly and without the machine's
// transition. Then the result is true. Missing: the argument accessors, set_bool, the CMatching vtable slot, and the
// OnlineGameState +0x30, +0x34 and +0x50 fields.
void NativeConnectToServer(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43fec4 NativeGetNextStatusMessage(gameswf::fn_call const&)
// PARTIAL. Takes one argument, or two. The top argument must have tag 2. When there are two, the one below must have
// tag 0 or 1 (undefined or null). The listing also builds the string "not yet supported with no hud code" and calls
// PlayerManager::GetLocalPlayer(0, false) and as_value::to_bool on the lower argument. Neither result is used. When the
// status queue (head dword_99FA90, end dword_99FAA0) is not empty, the head entry's string is copied and set as the
// result. The listing shows no removal of the head entry. Missing: the argument accessors, set_string, and the queue
// node layout.
void NativeGetNextStatusMessage(const gameswf::fn_call& call) {
    (void)call;
}

// 0x4401d4 NativeGetQuestCount(gameswf::fn_call const&)
// PARTIAL. Takes no argument count check. The top argument is the player index (to_number). The one below it is the
// quest state name (to_tu_string). For a player from NativeGetPlayerChar(index, false), the result is
// Character::SG_GetNumQuests(GetQuestFunctor(name), -1). Otherwise the result is -1. Missing: the argument accessors,
// to_tu_string (the name is inline at +1, or a pointer at +0x0c when the first byte is -1), and set_double.
void NativeGetQuestCount(const gameswf::fn_call& call) {
    (void)call;
}

// 0x440278 NativeGetNextAchievementMessage(gameswf::fn_call const&)
// PARTIAL. Needs exactly one argument with tag 5 (an object). The object is cast with gameswf::cast_to<as_object>.
// A local AchievementMsg is built with empty name and description, trophy type 0, grade 0 and label 0. When the
// achievement queue (begin dword_99FBE4, end dword_99FBF4) is not empty, the head AchievementMsg is copied into the
// local (its name is the string base, its description is at +0x18, trophy type +0x30, grade +0x34 and label +0x38, from
// dh2/AchievementMsg.hpp). The object then gets the members Name, Desc, TrophyType, Grade and Label through its vtable
// slot +0x1c. The result is true when an entry was copied, and false otherwise. The head entry is not removed here.
// Missing: the object member-setter signature, the as_value constructors, set_bool and the argument accessors.
void NativeGetNextAchievementMessage(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44058c NativeMerchantSellItem(gameswf::fn_call const&)
// PARTIAL. Takes no argument count check. The top argument is the item index (to_number). The one below is the player
// index (to_number). The one below that is the merchant object's name (to_string), and the fourth is a number that goes
// to ObjectManager::GetObjectByName as an int. The original first gets menu_Merchant from MenuManager::GetInstance().
// It only continues when the menu's byte at +0xc4 is 0, and it sets that byte to 1 and never clears it in this
// function. Then it checks that the object is a merchant (IsMerchant) and that the item index is in range (0 .. count-1).
// It takes the sell percent from Arrays::MerchantTable (+0x04, via Character::GetLoot), prices the
// item with MerchantTradePrice(item +0x54, percent), adds the gold to the player's inventory, sets the item's byte at
// +0x69 to 1, and transfers the item to the merchant's inventory (quantity 1, no stack, convert gold). It then calls
// INV_UpdateSkin, adds 1 to the sold counter (CharProperties 220), and unlocks the sell_100_items trophy when the
// counter is above 99. The result is always false. The DebugSwitches isTracingMerchant read has no effect. Missing: the
// argument accessors, the ItemInstance fields (+0x54, +0x69), the MerchantTable row, the menu +0xc4 field, the
// ObjectManager lookup, TrophyManager, and set_bool.
void NativeMerchantSellItem(const gameswf::fn_call& call) {
    (void)call;
}

// 0x4408d4 NativeMerchantBuyItem(gameswf::fn_call const&)
// PARTIAL. Takes no argument count check. The top argument is the item index (to_number). The one below it is the
// player index (to_number). The one below that is the merchant object's name (to_string), and the fourth is a number
// for ObjectManager::GetObjectByName. The original gets the merchant with the same lookup, and checks IsMerchant on it.
// It checks that the item index is in range. The price is MerchantTradePrice(item +0x54, percent), where percent is the
// merchant table's buy column (+0x08) when the item's byte at +0x69 is 0, and its sell column (+0x04) when it is set.
// The result depends on the order of these checks:
//   - price above the player's gold (ItemInventory::m_gold, +0x20 in the inventory, 924 in the Character) gives 1.
//   - a full inventory with an item that is not a potion (item type 14, the item's type id at row +0x58) gives 2.
//   - a potion when the potion count equals the player's maximum potions (ItemInventory +0x2c, int8) gives 3.
//   - otherwise the player's gold drops by price, the item goes to the player (quantity 1, no stack, convert gold),
//     the byte at +0x69 is cleared, CharProperties 221 (the spend counter behind the spend_* trophies) grows by price, the trophy spend_1000_gold is
//     unlocked at 1000 or more, and spend_10000_gold at more than 9999. The result is 0.
// The result is also 0 when any lookup or check fails. Missing: the argument accessors, the ItemInstance fields,
// the MerchantTable row, the ObjectManager lookup, TrophyManager, and set_double.
void NativeMerchantBuyItem(const gameswf::fn_call& call) {
    (void)call;
}

// 0x440c84 NativeHasPushNotification(gameswf::fn_call const&)
// PARTIAL. Takes no arguments. The gate is Device::IsHighPerformance() || Is_In_Multiplayer_Mode || SHARP_DEVICES ||
// HTC_DEVICES. When the gate is open, the result is (byte_99F83A || byte_99F81A || the GameCenter singleton's byte
// +0x15, the start gate). Otherwise the result is false. Missing: the globals Is_In_Multiplayer_Mode, SHARP_DEVICES,
// HTC_DEVICES, byte_99F83A and byte_99F81A (no declarations), the GameCenter +0x15 byte (game_center.hpp has only the
// StartGateForTest accessor), and set_bool.
void NativeHasPushNotification(const gameswf::fn_call& call) {
    (void)call;
}

// 0x440d3c NativeGLXPlayerSignin(gameswf::fn_call const&)
// PARTIAL. Returns at once unless GetOnline() has its online-game flag at +0x05 (COnline::isOnlineGame_, which has no
// getter yet). Then it needs exactly three arguments: the login name (to_tu_string, the top one), the password (to_tu_string,
// the one below) and the remember flag (to_bool, the bottom one). The original also needs the OnlineGameState byte
// at +0x27. In that case it stores the remember flag in the save_remember global, copies the name and the password
// with strdup, stores the remember flag in CMatching::Get()+27633, and builds a CSignInCredentials (12 bytes via
// CustomAlloc) from the name, the password and GameCenter's +0x18 pointer. Then it calls the sign-in object's virtual at
// vtable +0x10 with the credentials and returns that result. The native sets no result. The argument to CMatching::Get
// and CSignIn::Get in the listing does not match their parameterless forms, so it is probably a decompiler artifact.
// Missing: COnline::isOnlineGame_ getter, OnlineGameState +0x27, GameCenter +0x18, the CSignIn::Get path, the argument
// accessors, and the tu_string text readers.
void NativeGLXPlayerSignin(const gameswf::fn_call& call) {
    (void)call;
}
