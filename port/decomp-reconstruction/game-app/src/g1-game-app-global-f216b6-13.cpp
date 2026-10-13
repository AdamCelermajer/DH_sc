// Batch g1-game-app-global-f216b6-13 (<global>, part 14 of 22): the gameswf natives of the player info, potion, stats,
// keyboard, options, save slot, buff, inventory, quest, tutorial, item details, invite and parsed-string menus,
// ELF 0x4466c4 .. 0x44f528. Declarations: dh2/global/native_player_info_options.hpp.
//
// What is reconstructed and what is not:
//   - (First wave, before the r3b-w1 redo.) Every native was PARTIAL; the redo completed the ones marked reconstructed in
//     their ledger rows, so the statuses here are historical. Each one reads its arguments and writes its result through gameswf: the fn_call
//     argument accessors (argument count, top index, value tags, to_number, to_int, to_bool, to_string, to_xstring,
//     is_number), the as_value setters (set_string, set_bool, set_double, set_as_object), the tu_string property
//     names, drop_refs, the as_array push and element get, and the as_object property setter (vtable slot 7, byte
//     offset +0x1c). None of that is in the tree, so the bodies are shells. Each comment gives the original steps so
//     the body can be filled in when the gameswf API lands.
//   - ResolveLanguageOption and SaveSlotTimeFormat (dh2/global/native_player_info_options.hpp) are the arithmetic of
//     NativeSetOptions for the 'Language' key and the strftime switch of NativeGetSaveSlotDetails. They are tested in
//     game-app/tests/g1-game-app-global-f216b6-13_test.cpp.
//   - The Hex-Rays signatures of NativeGetPossibleClassSpec, NativeGetBuffs, NativeInvGetItemsListForSlot and
//     NativeInvGetItemDetails are mis-typed (the fn_call shows as a Character*, an as_value** or an int, and one shows a
//     pointer return). The demangled symbols take const gameswf::fn_call& and return void, so the definitions use that.
//
// Facts shared by the comments:
//   - Argument layout. The arguments sit in the fn_call's argument array (its pointer at +0x0c). The argument count is at
//     +0x10 and the top index at +0x14. "top" is the last argument and "top-1" and "top-2" the ones below it. The natives
//     test the value tags: 2 is a number and 5 an object. The set calls write tag 1 for a bool (value in the byte at +4),
//     tag 2 for a double and tag 5 for an object; string values use set_string.
//   - NativeGetPlayerChar(int, bool) (0x43c388, _Z19NativeGetPlayerCharib) returns the player's Character or null. It is
//     not declared in the tree yet, so the comments name it and no body calls it.
//   - Application singleton: Singleton<Application>::s_inst (dword_99F72C). The PlayerManager is at +0x40 (dword_99F76C).
//     PlayerManager::GetLocalPlayer(id, flag) and GetRemotePlayer(id, flag) return a PlayerInfo*. PlayerInfo has no
//     header in the tree (only a forward declaration in dh2/PlayerManager.hpp). The asm gives +0x660 Character*
//     character_660 and +0x678 the player number.
//   - Globals: dword_99F760 StringManager (dh2/StringManager.hpp), dword_99F758 PyDataConstants
//     (pydata-accessors/w7_pydata12.hpp), dword_99F778 SavegameManager (dh2/SavegameManager.hpp), dword_99F764
//     ObjectManager (dh2/ObjectManager.hpp).
//   - Character offsets (the Character object is the player's character_660): +0x37c ItemInventory (dh2/Character.hpp),
//     +0x3b4 CharTimers, +0x560 CharProperties (Hex-Rays "+1376"), +0x39c int gold (asm), +0x3a8 byte (LDRB in the asm) of max
//     potions (Hex-Rays), +0x1378 buff list (sentinel) with the first node at +0x1380.
//   - Option state: isKOREAN_BUILD, lang_curr and lang_kor are the language globals. Their ELF names are not in the tree.

#include "dh2/global/native_player_info_options.hpp"

#include <cstdint>

#include "dh2/Application.hpp"
#include "dh2/CharMenuTutorialMsg.hpp"
#include "dh2/GSKeyboard.hpp"
#include "dh2/MenuMessageManager.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/Quest.hpp"
#include "dh2/Character.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/global/native_app_bindings.hpp"
#include "gameswf/gameswf_environment.h"
#include "vendor-gameswf/as_value.hpp"

// gameswf's tu_loadlib.h includes windows.h, which defines GetClassName as GetClassNameA. Character::GetClassName is the
// DH2 member (the original's symbol), so the macro is removed for this unit.
#ifdef GetClassName
#undef GetClassName
#endif

// 0x4466c4 NativeGetPlayerInfos(gameswf::fn_call const&)
// The top argument is the output object (tag 5). The one below it is the player index (to_number). With three arguments
// the one below that is the remote flag (to_bool), else false. The original calls NativeGetPlayerChar(index, remote). When
// that is null nothing is written. Otherwise the object gets PlayerName (Character::SG_GetPlayerName), PlayerClass
// (Character::GetClassName) and PlayerNumber (a double: PlayerInfo +0x678 (unk_678) of PlayerManager::GetRemotePlayer(index,
// false) when remote, else of GetLocalPlayer(index, false); the asm reads +0x678 on both paths). The object is then the
// return value, set_as_object, and this happens even when the player is null.
void NativeGetPlayerInfos(const gameswf::fn_call& call) {
    gameswf::as_object* obj = call.arg(0).to_object();
    const int player = static_cast<int>(call.arg(1).to_number());
    const bool remote = call.nargs == 3 ? call.arg(2).to_bool() : false;
    Character* ch = NativeGetPlayerChar(player, remote);
    if (ch != nullptr && obj != nullptr) {
        gameswf::as_value name;
        name.set_string(ch->SG_GetPlayerName());
        obj->set_member(tu_stringi("PlayerName"), name);
        gameswf::as_value className;
        className.set_string(ch->GetClassName());
        obj->set_member(tu_stringi("PlayerClass"), className);
        PlayerManager* players = ApplicationGetInstance().m_playerManager;
        PlayerInfo* info = remote ? players->GetRemotePlayer(player, false) : players->GetLocalPlayer(player, false);
        gameswf::as_value number;
        number.set_double(static_cast<double>(info->unk_678));
        obj->set_member(tu_stringi("PlayerNumber"), number);
    }
    call.result->set_as_object(obj);
}

// 0x446978 NativeGetStringNumPotions(gameswf::fn_call const&)
// PARTIAL. The top argument is the output object (tag 5). The one below it is the player index (to_number). The
// original calls NativeGetPlayerChar(index, false). When the player exists, the string is
// StringManager::parse(out, getString(PyDataConstants::getConstant("StrID", "GAMEPLAYMENUS_POTIONS")),
// ItemInventory::GetNumPotions() of the character's inventory at +0x37c). The result is set on the object as the
// string property StrNumPotions. The return value is not set.
// Missing: the argument accessors, to_number, the tu_string names, set_string, drop_refs, the object setter,
// NativeGetPlayerChar and the StrID string table (PyDataConstants::getConstant(class, name) is declared in
// pydata-accessors/w7_pydata12.hpp).
void NativeGetStringNumPotions(const gameswf::fn_call& call) {
    (void)call;
}

// 0x446afc NativeGetPlayerStats(gameswf::fn_call const&)
// PARTIAL. The largest native in the batch (about 1,100 Hex-Rays lines). The top argument is the output object (tag 5,
// used without a null check). The one below it is the player index (to_number). The original calls
// NativeGetPlayerChar(index, false). When that is null nothing is set. Otherwise the object gets the properties below,
// in the original's order (Name first, Stun_Resist_Chance last). The object is then the return value (set_as_object).
//   Value sources. CharProperties is the character at +0x560 (Hex-Rays v8 = character + 1376). A numeric property is
//   CharProperties::PROPS_GetInt(prop, false) stored as a double (its header comment says it returns value >> 8).
//   Name is Character::SG_GetPlayerName and Class is Character::GetClassName. Icon is 3.0 for class ids 290..292,
//   2.0 for 325..327 and 1.0 otherwise (Character::SG_GetPlayerClass). HasOffHandWeapon(), HasStaff(), HasBow() and
//   IsWeaponTwoHanded (HasTwoHander(false)) are bools of the character's ItemInventory (+0x37c). The bonus terms are
//   PROPS_GetBonusDamage(offHand) >> 8, PROPS_GetBonusAttackRating(false) >> 8 and PROPS_GetBonusCritRating(false) >> 8.
//   Prop ids (PROPS_GetInt id):
//     Level 19, HP 36, HP_Bonus 37, Max_HP 38, MP 41, MP_Bonus 42, Max_MP 43, XP 33, Max_XP 34,
//     Stat_Strength 149, Stat_Dexterity 150, Stat_Endurance 151, Stat_Energy 152, Stat_Points 148,
//     Rating_Defense 59, Rating_Dodge 60, Rating_Block 61, Physical_Armor 71,
//     Resistance_Fire 74, Resistance_Water 75, Resistance_Lightning 76, Resistance_Earth 77, Resistance_Air 78,
//     Regen_HP 39, Regen_MP 44, Leech_HP 132, Leech_MP 133, Stun_Resist_Chance 138,
//     Special_Loot_Gold_Multiplier 195, Special_Loot_Magical_Chance 196,
//     Menu_Average_Melee_To_Hit 207, Menu_Average_Spell_To_Hit 208, Menu_Melee_Damage_Reduction 209,
//     Spell_Rating_Dodge 164, Spell_Rating_Critical 165, Spell_Damage_Bonus_Fire 166, _Water 167, _Lightning 168,
//     _Earth 169, _Air 170,
//     Damage_Elemental_Min_Main_Hand 95, Damage_Elemental_Max_Main_Hand 96, Damage_Elemental_Type_Main_Hand 97,
//     Damage_Elemental_Min_Off_Hand 98, Damage_Elemental_Max_Off_Hand 99, Damage_Elemental_Type_Off_Hand 100,
//     Damage_Fire_Min/Max_Main_Hand 101/102, Damage_Water_Min/Max_Main_Hand 105/106,
//     Damage_Lightning_Min/Max_Main_Hand 109/110, Damage_Earth_Min/Max_Main_Hand 113/114,
//     Damage_Air_Min/Max_Main_Hand 117/118.
//   Computed rows: Damage_Min_Main_Hand = PROPS_GetInt(79) + (PROPS_GetBonusDamage(false) >> 8),
//   Damage_Max_Main_Hand = PROPS_GetInt(80) + the same, Damage_Min_Off_Hand = PROPS_GetInt(81) + (PROPS_GetBonusDamage(true) >> 8),
//   Damage_Max_Off_Hand = PROPS_GetInt(82) + the same. Rating_Attack = PROPS_GetInt(50) + (PROPS_GetBonusAttackRating(false) >> 8).
//   Rating_Critical = PROPS_GetInt(63) + (PROPS_GetBonusCritRating(false) >> 8).
// Missing: the argument accessors, to_number, the tu_string names, set_string, set_double, set_bool, drop_refs, the
// object setter, set_as_object, NativeGetPlayerChar, and PROPS_GetBonusCritRating (not declared in
// dh2/CharProperties.hpp: layout request). PROPS_GetBonusDamage and PROPS_GetBonusAttackRating are declared there.
void NativeGetPlayerStats(const gameswf::fn_call& call) {
    (void)call;
}

// 0x4497f4 NativeGetKeyboardLastString(gameswf::fn_call const&)
// The top argument is the output object (tag 5). When it is an object, the original sets opened (bool, the byte
// GSKeyboard::s_inst.m_querying, ELF 0x9a24d4) and string (GSKeyboard::GetLastString()). The object is then the result,
// set_as_object, even when the argument is not an object (then null is returned).
void NativeGetKeyboardLastString(const gameswf::fn_call& call) {
    gameswf::as_object* obj = call.arg(0).to_object();
    if (obj != nullptr) {
        gameswf::as_value opened;
        opened.set_bool(GSKeyboard::s_inst.m_querying);
        obj->set_member(tu_stringi("opened"), opened);
        gameswf::as_value text;
        text.set_string(GSKeyboard::GetLastString());
        obj->set_member(tu_stringi("string"), text);
    }
    call.result->set_as_object(obj);
}

// 0x44996c NativeGetNumPotions(gameswf::fn_call const&)
// The output object is the top argument (tag 5). The local player's Character is PlayerInfo +0x660 (unk_660). When it
// exists, the object gets NumPotions (ItemInventory::GetNumPotions) and MaxNumPotions (the unsigned byte of the inventory's
// m_maxPotions, Character +0x3a8). The original does not check the object before its setter, so a null object is skipped
// here.
void NativeGetNumPotions(const gameswf::fn_call& call) {
    gameswf::as_object* obj = call.arg(0).to_object();
    Character* ch = ApplicationGetInstance().m_playerManager->GetLocalPlayer(0, true)->unk_660;
    if (ch != nullptr && obj != nullptr) {
        gameswf::as_value numPotions;
        numPotions.set_double(static_cast<double>(ch->ItemInventory::GetNumPotions()));
        obj->set_member(tu_stringi("NumPotions"), numPotions);
        gameswf::as_value maxPotions;
        maxPotions.set_double(static_cast<double>(static_cast<std::uint8_t>(ch->ItemInventory::m_maxPotions)));
        obj->set_member(tu_stringi("MaxNumPotions"), maxPotions);
    }
}

// 0x449b10 NativeGetPossibleClassSpec(gameswf::fn_call const&)
// PARTIAL. The top argument is the output object (tag 5, cast to as_object). The player is
// PlayerManager::GetLocalPlayer(0, true) (asm: flag 1) and its character is PlayerInfo +0x660. The result is the
// PlayerInfo pointer (the Hex-Rays return), which is not part of the native's contract. When the object exists the
// original sets four strings from the pydata CharacterTable row of the character's class (Character::SG_GetPlayerClass(),
// row stride 900 bytes): Class1Name (StringManager::getString of the id at CharacterTable::members + 900*class + 924),
// Class1Desc (+928), Class2Name (+1824) and Class2Desc (+1828). The object is then the return value, set_as_object.
// Missing: the argument accessors, the tu_string names, set_string, drop_refs, the object setter, set_as_object,
// PlayerInfo (waits on dh2/PlayerInfo.hpp), and the CharacterTable member declarations (pydata-accessors has the struct; only the byte
// offsets are used here).
void NativeGetPossibleClassSpec(const gameswf::fn_call& call) {
    (void)call;
}

// 0x449e70 NativeSetOptions(gameswf::fn_call const&)
// PARTIAL. Arguments: the top one is the option key (to_string), the one below it the output object (tag 5, cast to
// as_object), and the one below that the value (to_number). For the key 'Language' the stored value is
// ResolveLanguageOption(value, isKOREAN_BUILD, lang_kor, lang_curr), which is also the new lang_curr. For every key:
//   - SavegameManager::setOption(key, value), then UpdateSavedValues() (free function, not declared in the tree).
//   - 'VolumeFX' (channel 1) or 'VolumeMusic' (channel 2): when VoxSoundManager::s_instance is set,
//     VoxSoundManager::SetSoundVolume(instance, channel, (float)Application::GetSavedOption(key)).
//   - Option = SavegameManager::getOption(key). The string is '' when getOptionString(key) - Option == -1, else
//     StringManager::getString(getOptionString(key)).
//   - 'Language' only: MenuManager::ResetFonts() on MenuManager::GetInstance(nullptr) (dh2/MenuManager.hpp).
//   - When the object exists, CurrentOption (double, Option) and OptionString (string) are set on it and the object is
//     the return value. Otherwise the return value is the result of the key compare against 'Language'.
// Missing: the argument accessors, to_string, to_number, the tu_string names, set_double, set_string, drop_refs, the
// object setter, set_as_object, UpdateSavedValues, isKOREAN_BUILD, lang_curr and lang_kor (not declared in the tree),
// and the VoxSoundManager declarations (vox-audio/include/vox-audio/vox/sound_manager.hpp, not checked here).
void NativeSetOptions(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44a298 NativeGetOptionParameters(gameswf::fn_call const&)
// PARTIAL. Arguments: the top one is the option key (to_string), the one below it the output object (tag 5, cast to
// as_object). Option = getOption(key), OptionMax = getOptionMax(key), and the string index is getOptionString(key). The
// string is '' when index - Option == -1, else StringManager::getString(index). When the object exists: in a Korean
// build, for the key 'Language', OptionMax is 5. The object gets NumOptions (double, OptionMax), CurrentOption (double,
// Option) and OptionString (string), and is the return value (set_as_object). Then, in a Korean build with the key
// 'Language', lang_kor is set to Option. That write runs even when there is no object. The return value otherwise is
// the key compare result.
// Missing: the argument accessors, to_string, to_number, the tu_string names, set_double, set_string, drop_refs, the
// object setter, set_as_object, isKOREAN_BUILD and lang_kor (not declared in the tree).
void NativeGetOptionParameters(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44a5c4 NativeInvGetPlayerGold(gameswf::fn_call const&)
// PARTIAL. The top argument is the player index (to_number). With two arguments, the one below it is the output
// object (tag 5); the object is then used only if its tag is 5. With three arguments the one below that is the remote
// flag (to_bool) and no object is used. The original calls NativeGetPlayerChar(index, remote). When the player exists,
// GoldString = StringManager::parse('^d', gold) with gold = the Character's int at +0x39c. With an object, the object
// gets Gold (double, +0x39c) and GoldString (string), and is the return value (set_as_object). Without one, the return
// value is GoldString (set_string on the return slot).
// Missing: the argument accessors, to_number, to_bool, the tu_string names, set_double, set_string, drop_refs, the
// object setter, set_as_object, NativeGetPlayerChar, and the gold field (layout request: Character +0x39c int, read by
// the asm of this native as LDR [character,#0x39C]).
void NativeInvGetPlayerGold(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44a820 NativeHUDGetActiveFaery(gameswf::fn_call const&)
// The player index is to_number of the top argument, and the character is NativeGetPlayerChar(index, false). With two
// arguments and an object below the top, the object gets Id (SG_GetCurrentFaerieId(-1)) and Upgraded (SG_GetFaerieLevel(id,
// -1) > 0) and is the result (set_as_object). Otherwise the result is the Id as a number. No character: nothing is set.
void NativeHUDGetActiveFaery(const gameswf::fn_call& call) {
    const int player = static_cast<int>(call.arg(0).to_number());
    gameswf::as_object* obj = nullptr;
    if (call.nargs == 2 && call.arg(1).is_object()) {
        obj = call.arg(1).to_object();
    }
    Character* ch = NativeGetPlayerChar(player, false);
    if (ch == nullptr) {
        return;
    }
    if (obj != nullptr) {
        gameswf::as_value id;
        id.set_double(static_cast<double>(ch->SG_GetCurrentFaerieId(-1)));
        obj->set_member(tu_stringi("Id"), id);
        gameswf::as_value upgraded;
        const int faerieId = ch->SG_GetCurrentFaerieId(-1);
        upgraded.set_bool(ch->SG_GetFaerieLevel(static_cast<unsigned int>(faerieId), -1) > 0);
        obj->set_member(tu_stringi("Upgraded"), upgraded);
        call.result->set_as_object(obj);
    } else {
        call.result->set_double(static_cast<double>(ch->SG_GetCurrentFaerieId(-1)));
    }
}

// 0x44aa28 NativeGetSaveSlotDetails(gameswf::fn_call const&)
// PARTIAL. Arguments: the top one is the index into the list of save slots (to_number). The one below it is the output
// object (tag 5, cast to as_object). With three arguments the one below that is the difficulty filter (to_number), else
// -1. The original checks the save slots 0..3 with PlayerSavegame::SG_Exists. Existing slots go into one list and the
// rest into a second. The chosen slot is the existing entry at the index when the existing list has 4 entries or the
// index is below its count (read without a range check, as in the original), else the first free slot. Its
// PlayerSavegame is loaded (PlayerSavegame(slot, 17, false)).
//   When the slot is in use: the level text is the class name, then ' ', then GAMEPLAYMENUS_LEVEL, then ' ' and the
//   level. The time text is strftime(SaveSlotTimeFormat(SavegameManager::getLanguage()), localtime(save time)), into an
//   80-byte buffer. A difficulty filter of -1 becomes PlayerSavegame::m_difficultyLevel.
//   The object gets SlotID (double, the chosen slot), InUse (bool), PlayerName, PlayerClass (class name string),
//   PlayerLVL (double, the level, or -1 when free), StringClassLVL (class plus level text), PlayerLocation (name id at
//   +36 of the LevelList row of the save's level, 72-byte rows), PlayerLastSave (time text, '' when free),
//   PlayerCurrentAct (int at SG_GetQuestSG() + 4*difficulty + 68, or 1 when free), Difficulty (the filter when in use,
//   else 0) and DifficultyUnlocked (double, a local the pseudocode fills from the save record and does not identify,
//   or 0 when free). The object is then the return value (set_as_object). The Hex-Rays loop that picks the slot is garbled: a stack variable is reused as the loop
//   counter and the same stack slot is read by a debug printf('INFO: %d'). Only this summary is recovered, and that
//   printf is not reproduced.
// Missing: the argument accessors, to_number, the tu_string names, set_string, set_double, set_bool, drop_refs, the
// object setter, set_as_object, the PlayerSavegame fields used above (m_difficultyLevel, SG_GetQuestSG, the save time
// and the save field at +75; dh2/PlayerSavegame.hpp must be checked), LevelList and CharacterTable row members.
void NativeGetSaveSlotDetails(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44b604 NativeGetBuffs(gameswf::fn_call const&)
// PARTIAL. The Hex-Rays signature (Character* taking the fn_call) is mis-typed. With two or three arguments: the top
// one must be an object (tag 5, cast to as_array) and the one below it a number (the player index). With three, the
// one below that must have tag 0 or 1 (undefined or bool; any other tag returns at once), and it is the remote flag
// (to_bool). The original calls NativeGetPlayerChar(index, remote). The character's buff list is then walked from its
// sentinel (+0x1378, first node at +0x1380). For each buff an object is pushed onto the array (as_array::push, with a
// reference added): BuffCount (double, the deque distance from the node's iterator to the container end), BuffIcon
// (string, the icon at node +0x30), BuffTime and BuffTotalTime (doubles, the first and second out values of
// CharTimers::TMR_TimeLeft(timer id, elapsed, interval) on the character's CharTimers at +0x3b4, with the timer id read
// from BuffInst +904 and used only when that id is not -1). The end-of-list step (a deque distance test of 1 and two
// advances) is a Hex-Rays mis-decode and is not recovered. The array is then the return value (set_as_object).
// Missing: the argument accessors, is_number, to_number, to_bool, the tu_string names, set_string, set_double, drop_refs,
// the as_array push and the object setter, NativeGetPlayerChar, and the buff list node declaration (layout request:
// Character +0x1378 buff list sentinel and the node layout with the icon string at +0x30).
void NativeGetBuffs(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44bb94 NativeInvGetItemsListForSlot(gameswf::fn_call const&)
// PARTIAL. The Hex-Rays signature (as_value** taking the fn_call) is mis-typed. Arguments, from the top: the equipment
// slot (a number, tag 2, to_int), the output array (tag 5, cast to as_array), and the player index (a number, to_int).
// The original calls NativeGetPlayerChar(index, false). When the player exists, the slot is compared with the
// EquipmentSlots constants (Count, LeftHand, LeftHandRingFinger; the RightHand and RightHandRingFinger lookups have no
// effect). When the slot equals Count, ItemInventory::GetValuableItems(inventory at +0x37c) fills a list, else
// ItemInventory::GetItemListForSlot(inventory, slot) does. The list is sorted with SortByEquipability, and the array is
// the return value (set_as_object). The pseudocode has no loop that pushes the entries into the array, so the filling
// step is not recovered. It is most likely a Hex-Rays loss, not a missing call.
// Missing: the argument accessors, to_int, is_number, the tu_string names, set_as_object, NativeGetPlayerChar, and the
// array push. PyDataConstants::getConstant (pydata-accessors/w7_pydata12.hpp) and SortByEquipability
// (dh2/SortByEquipability.hpp) are declared.
void NativeInvGetItemsListForSlot(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44c444 NativeGetQuestDetails(gameswf::fn_call const&)
// The output object is the top argument, the player index is the argument below it and the quest index the third from the
// top. The quest index must satisfy 0 <= index < SG_GetNumQuests(-1) of the player's character (NativeGetPlayerChar(player,
// false)); then the object gets Title, PreDescription, ObjectiveDescription, PostDescription (strings) and IsPrimary (bool)
// from Character::SG_GetQuestByID(index, -1). The native returns nothing.
void NativeGetQuestDetails(const gameswf::fn_call& call) {
    gameswf::as_object* obj = call.arg(0).to_object();
    const int player = static_cast<int>(call.arg(1).to_number());
    const int index = static_cast<int>(call.arg(2).to_number());
    Character* ch = NativeGetPlayerChar(player, false);
    if (ch == nullptr || obj == nullptr) {
        return;
    }
    if (index < 0 || index >= ch->SG_GetNumQuests(-1)) {
        return;
    }
    Quest* quest = ch->SG_GetQuestByID(index, -1);
    gameswf::as_value title;
    title.set_string(quest->GetTitle());
    obj->set_member(tu_stringi("Title"), title);
    gameswf::as_value pre;
    pre.set_string(quest->GetPreDescription());
    obj->set_member(tu_stringi("PreDescription"), pre);
    gameswf::as_value objective;
    objective.set_string(quest->GetObjectiveDescription().c_str());
    obj->set_member(tu_stringi("ObjectiveDescription"), objective);
    gameswf::as_value post;
    post.set_string(quest->GetPostDescription());
    obj->set_member(tu_stringi("PostDescription"), post);
    gameswf::as_value primary;
    primary.set_bool(quest->IsPrimary());
    obj->set_member(tu_stringi("IsPrimary"), primary);
}

// 0x44c7a0 NativeGetCharMenuTutorialMessage(gameswf::fn_call const&)
// The original builds a temporary CharMenuTutorialMsg (id -1, two empty strings) and destroys it at once; nothing reads it,
// so it is not written. The front of the CharMenuTutorialMsg queue (Singleton<MenuMessageManager<CharMenuTutorialMsg,1>>,
// queue 0) is read, not popped. When the queue is non-empty, the object gets TutorialMessage (StringManager::getString of
// the message id), TutorialFrame (m_frame) and TutorialMenu (m_menu), and the result is true. Otherwise the result is false.
void NativeGetCharMenuTutorialMessage(const gameswf::fn_call& call) {
    gameswf::as_object* obj = call.arg(0).to_object();
    const auto& queue = Singleton<MenuMessageManager<CharMenuTutorialMsg, 1>>::s_inst.m_queues[0];
    bool found = false;
    if (!queue.empty()) {
        const CharMenuTutorialMsg& front = queue.front();
        if (obj != nullptr) {
            gameswf::as_value message;
            message.set_string(ApplicationGetInstance().m_stringManager->getString(front.m_stringId));
            obj->set_member(tu_stringi("TutorialMessage"), message);
            gameswf::as_value frame;
            frame.set_string(front.m_frame.c_str());
            obj->set_member(tu_stringi("TutorialFrame"), frame);
            gameswf::as_value menu;
            menu.set_string(front.m_menu.c_str());
            obj->set_member(tu_stringi("TutorialMenu"), menu);
        }
        found = true;
    }
    call.result->set_bool(found);
}

// 0x44ca5c NativeInvGetItemDetails(gameswf::fn_call const&)
// PARTIAL. The Hex-Rays signature (int taking the fn_call) is mis-typed. Arguments: the top one is the item index
// (a number, tag 2), the one below it the output object (tag 5), and the one below that the player index (a number).
// With six arguments, the three below the top are the merchant probe: a bool (tag 1, read as the flag v14) at top-3, a
// name (tag 3 or 4) at top-4 and a number at top-5. The probe is ObjectManager::GetObjectByName(name, number) and keeps
// the result only when Character::IsMerchant() is true. The original then calls NativeGetPlayerChar(index, false).
// When the player exists and the item is found: when the flag is set and a merchant was probed, the item comes from the
// merchant's inventory (ItemInventory::GetItem on the merchant's +0x37c), else from the player's inventory.
// Merchant factors: when a merchant was probed, row = MerchantTable::members + 20 * Character::GetLoot(); f1 = row
// +4 / 100 and f2 = row +8 / 100 (floats). Otherwise both are 0. With value = item +84 (int):
//   When the item's byte at +105 is set and the flag is set, buy = int(value * f1) and sell = int(value * f2).
//   Otherwise buy = int(value * f2) and sell = int(value * f1). Each is clamped to at least 1.
//   ItemTransmuteValue = (TransmuteMultiplier * ((value << 8) * (CharProperties::_GetProperty(197) + 256) >> 8)) >> 16,
//   clamped to at least 1. TransmuteMultiplier is PyDataConstants::getConstant('CharacterDesign', 'TransmuteMultiplier').
//   ItemValue is value (double). ItemValueString, ItemBuyValueString, ItemSellValueString and ItemTransmuteValueString
//   are StringManager::parse('^d', ...) of the four numbers.
//   Also set: ItemEquippable (bool, ItemInstance::IsEquippableBy(player)), ItemReqsDesc (string, item +76), ItemStatsDesc
//   (string, item +52), ItemMagics (double, GetNumPowers()), ItemIcon (ItemInstance::GetIconName(), cast to a string), IsStackable
//   (bool) and ItemPowers<i>Desc (GetPowerDesc(i) for each power). The object is then the return value (set_as_object).
// Missing: the argument accessors, to_int, to_number, to_bool, to_string, is_number, the tu_string names, set_string,
// set_double, set_bool, drop_refs, the object setter, set_as_object, MerchantTable (pydata), and NativeGetPlayerChar.
// Also open: ObjectManager::GetObjectByName is declared in dh2/ObjectManager.hpp as (name, flags, active, type), and the
// pseudocode's argument mapping (the probe's id and a nullptr) is not confirmed. CharProperties::_GetProperty is declared
// in dh2/CharProperties.hpp as (sheet, propId), and the pseudocode passes two pointers (+1376 and +4084), so that call is
// not matched. ItemInstance::GetIconName() is declared int in dh2/ItemInstance.hpp, but the pseudocode casts its result
// to const char*; the header comment says it is the icon name id, so the string lookup is still to confirm.
void NativeInvGetItemDetails(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44d770 NativeInvitePlayer(gameswf::fn_call const&)
// PARTIAL. With one or more arguments, the top one is an output array (tag 5, cast to as_array). The native does
// nothing unless COnline::isOnlineGame_ (GetOnline() +0x05) is set and the OnlineGameState at +0x34 is 3 (the
// multiplayer state). It then takes CMatching::Get(OnlineGameState) as a CMatchingGLLive and branches on
// CMatchingGLLive::GLFriendsListLoaded().
//   Loaded: every entry of GetGLFriendsList() is pushed (when arguments exist) as an object with name (entry +12),
//   id (entry +8), sent (bool, entry +17) and online (bool, entry +16). The temporary list nodes (size 0x14) are freed.
//   Not loaded: for each index below GetGLFriendsCount(), name = GetGLFriendsName(i) and number = GetGLFriendsNumber(i)
//   are duplicated with strdup, and state = GetGLFriendState(i). When arguments exist, an object {name, id, sent (bool),
//   online = state == 2} is pushed. Then CMatchingGLLive::AddFriendInGameList(id, name, state == 2) is called. After the
//   loop GLFriendsListLoaded(true) is set.
// Missing: the argument accessors, the tu_string names, set_string, set_bool, drop_refs, the object setter, the
// as_array push, GetOnline() (thunk to COnline::GetInstance, not in the tree), the OnlineGameState field at +0x34 (not
// declared in net-session-online/online_game_state.hpp), and CMatching::Get. In the not-loaded branch the pseudocode
// leaves the CMatching argument unset on one path (v10), so that call target is unresolved.
void NativeInvitePlayer(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44de4c NativeGetParsedString(gameswf::fn_call const&)
// PARTIAL. Needs two arguments. The top one is the format symbol (tag 3 or 4, to_xstring), and the one below it an
// array (tag 5, cast to as_array). When the symbol is present, StringManager::getStringFromSymbol(symbol) gives the
// format. Each array element is converted to a VarArgs variant: a number (tag 2) gives an int (to_number, truncated) and
// a float (to_number); a string (tag 3 or 4) gives a C string (to_xstring); other tags stay zero. The variants are
// passed to StringManager::parseEx(out, format, args), and out is set as the return value (set_string on the return
// slot).
// Missing: the argument accessors, to_number, to_xstring, the as_array length and element get (vtable slot 10), the
// tu_string names, set_string, drop_refs, and VarArgs (forward declared in dh2/StringManager.hpp, no definition).
// StringManager::getStringFromSymbol and StringManager::parseEx are declared in dh2/StringManager.hpp.
void NativeGetParsedString(const gameswf::fn_call& call) {
    (void)call;
}
