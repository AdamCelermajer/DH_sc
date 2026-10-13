// Batch g1-game-app-global-f216b6-11 (<global>, part 12 of 22): the gameswf natives of the stats, skills, faery HUD,
// inventory, save slot, loading, server, status, quest, achievement, merchant, push-notification and Game Center
// menus, ELF 0x43eb7c .. 0x440d3c. Declarations: dh2/global/native_stats_inventory_merchant.hpp.
//
// What is reconstructed and what is not:
//   - GetQuestFunctor (0x440080) is complete. DebugSwitches::GetInstance() loads the switches and returns them,
//     which is the same sequence as the Hex-Rays load(&s_inst) followed by GetSwitch(&s_inst, name).
//   - MerchantTradePrice (header) is the arithmetic shared by the sell and buy natives. It is tested in
//     game-app/tests/g1-game-app-global-f216b6-11_test.cpp.
//   - Every native whose body is written reads its arguments with call.arg(i) (argument 0 is the top of the
//     call's stack) and writes call.result. Three natives stay PARTIAL on declarations of other categories: see
//     NativeConnectToServer, NativeHasPushNotification and NativeGLXPlayerSignin.
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

#include "core-platform/device.hpp"
#include "gameswf/gameswf_environment.h"
#include "vendor-gameswf/as_value.hpp"
#include "core-platform/deps/application.hpp"
#include "dh2/AchievementMsg.hpp"
#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/ItemInstance.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/MenuMerchant.hpp"
#include "dh2/MenuMessageManager.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/StatusMsg.hpp"
#include "dh2/TrophyManager.hpp"
#include "dh2/global/app_helpers.hpp"
#include "dh2/global/native_player_actions.hpp"
#include "dh2/global/native_stats_inventory_merchant.hpp"
#include "net-session-online/online_game_state.hpp"
#include "net-session-online/tb/online_singleton.hpp"
#include "pydata-accessors/member_names.hpp"
#include "pydata-accessors/w7_structs_2.hpp"

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
// Two arguments, both numbers: the stat type (0 Str, 1 Dex, 2 End, 3 Nrg) and the player index. The player's character raises that
// stat by one point (Character::IncStatStr / IncStatDex / IncStatEnd / IncStatNrg; other values do nothing) and the result is
// cleared to undefined.
void NativeStatsAssignPoint(const gameswf::fn_call& call) {
    if (call.nargs == 2 && call.arg(0).is_number() && call.arg(1).is_number()) {
        const int stat = call.arg(0).to_int();
        Character* ch = NativeGetPlayerChar(call.arg(1).to_int(), false);
        if (ch) {
            switch (stat) {
                case 0:
                    ch->IncStatStr();
                    break;
                case 1:
                    ch->IncStatDex();
                    break;
                case 2:
                    ch->IncStatEnd();
                    break;
                case 3:
                    ch->IncStatNrg();
                    break;
                default:
                    break;
            }
            call.result->set_undefined();
        }
    }
}

// 0x43ec98 NativeSkillsGetSkillPointsLeft(gameswf::fn_call const&)
// One number argument, the player index. The result is the character's remaining skill points (property 157) as a number.
void NativeSkillsGetSkillPointsLeft(const gameswf::fn_call& call) {
    if (call.nargs == 1 && call.arg(0).is_number()) {
        Character* ch = NativeGetPlayerChar(call.arg(0).to_int(), false);
        if (ch) {
            call.result->set_double(static_cast<double>(ch->PROPS_GetInt(157, false)));
        }
    }
}

// 0x43ed58 NativeHUDGetIsFaeryUnlocked(gameswf::fn_call const&)
// Two numbers: the faery id and the player index. The result is Character::SG_IsFaerieUnlocked(faery, -1).
void NativeHUDGetIsFaeryUnlocked(const gameswf::fn_call& call) {
    if (call.nargs == 2 && call.arg(0).is_number() && call.arg(1).is_number()) {
        const int faery = call.arg(0).to_int();
        Character* ch = NativeGetPlayerChar(call.arg(1).to_int(), false);
        if (ch) {
            call.result->set_bool(ch->SG_IsFaerieUnlocked(static_cast<unsigned int>(faery), -1));
        }
    }
}

// 0x43ee40 NativeHUDSetActiveFaery(gameswf::fn_call const&)
// Two numbers: the faery id and the player index. The character changes faery (Character::ChangeFaery); when a level exists
// Level::PlaceFaeryAndFollowers(nullptr) places the faery and the followers again; the result is cleared to undefined.
void NativeHUDSetActiveFaery(const gameswf::fn_call& call) {
    if (call.nargs == 2 && call.arg(0).is_number() && call.arg(1).is_number()) {
        const int faery = call.arg(0).to_int();
        Character* ch = NativeGetPlayerChar(call.arg(1).to_int(), false);
        if (ch) {
            ch->ChangeFaery(static_cast<unsigned int>(faery));
            if (Singleton<Application>::s_inst.GetCurrentLevel()) {
                Singleton<Application>::s_inst.GetCurrentLevel()->PlaceFaeryAndFollowers(nullptr);
            }
            call.result->set_undefined();
        }
    }
}

// 0x43ef64 NativeInvUnequipItem(gameswf::fn_call const&)
// Two numbers: the equipment slot and the player index. The character's virtual UnEquipItemFromSlot(slot) (vtable +0x144) runs.
// The native sets no result.
void NativeInvUnequipItem(const gameswf::fn_call& call) {
    if (call.nargs == 2 && call.arg(0).is_number() && call.arg(1).is_number()) {
        const int slot = call.arg(0).to_int();
        Character* ch = NativeGetPlayerChar(call.arg(1).to_int(), false);
        if (ch) {
            ch->UnEquipItemFromSlot(static_cast<unsigned int>(slot));
        }
    }
}

// 0x43f044 NativeInvGetHasOffHandWeapon(gameswf::fn_call const&)
// One number, the player index. The result is ItemInventory::HasOffHandWeapon() of the player's inventory, false without a player.
void NativeInvGetHasOffHandWeapon(const gameswf::fn_call& call) {
    if (call.nargs == 1 && call.arg(0).is_number()) {
        Character* ch = NativeGetPlayerChar(call.arg(0).to_int(), false);
        call.result->set_bool(ch ? ch->ItemInventory::HasOffHandWeapon() : false);
    }
}

// 0x43f0f0 NativeInvGetHasTwoHandedWeapon(gameswf::fn_call const&)
// Same argument rule. The result is ItemInventory::HasTwoHander(false) of the player's inventory, false without a player.
void NativeInvGetHasTwoHandedWeapon(const gameswf::fn_call& call) {
    if (call.nargs == 1 && call.arg(0).is_number()) {
        Character* ch = NativeGetPlayerChar(call.arg(0).to_int(), false);
        call.result->set_bool(ch ? ch->ItemInventory::HasTwoHander(false) : false);
    }
}

// 0x43f23c NativeInvTransmuteItem(gameswf::fn_call const&)
// Two numbers: the item index and the player index. The character transmutes the item (INV_TransmuteItem(item, false)), its skin
// is refreshed (INV_UpdateSkin) and the result is cleared to undefined. The original also builds and destroys an empty local
// std::vector<ItemInventory::Item>, which has no effect.
void NativeInvTransmuteItem(const gameswf::fn_call& call) {
    if (call.nargs == 2 && call.arg(0).is_number() && call.arg(1).is_number()) {
        const int item = call.arg(0).to_int();
        Character* ch = NativeGetPlayerChar(call.arg(1).to_int(), false);
        if (ch) {
            ch->INV_TransmuteItem(static_cast<unsigned int>(item), false);
            ch->INV_UpdateSkin();
            call.result->set_undefined();
        }
    }
}

// Body of NativeGetSaveIDsListSize: the number of entries of PlayerSavegame::SG_GetSavegameList(false). The original
// divides the byte span of its vector by the 32-bit entry size (24 bytes); the host vector gives the count directly.
int SaveGameIdsCount() {
    return static_cast<int>(PlayerSavegame::SG_GetSavegameList(false).size());
}

// 0x43f5c8 NativeGetSaveIDsListSize(gameswf::fn_call const&): the number of save ids (SaveGameIdsCount, the size of the
// PlayerSavegame::SG_GetSavegameList result), as a number.
void NativeGetSaveIDsListSize(const gameswf::fn_call& call) {
    call.result->set_double(SaveGameIdsCount());
}

// 0x43f630 NativeCreateSaveSlot(gameswf::fn_call const&)
// Two string arguments: the save name (argument 0) and the character class name (argument 1). The class name is looked up in
// Arrays::CharacterTable::m_memberNames (-1 when missing). A playable class (IsPlayableClassID) gets the next free save slot, and
// a PlayerSavegame is built for it (loaded with mode 1) with the save name, level 1 and the class; the running difficulty is
// reset to 0, seeds and save date are generated, the highest unlocked difficulty is 1, and for every difficulty the level id is
// 41, the entry point 0 and the spawn-point flag set. The save is written and the result is the slot as a number. Any other
// class leaves the result alone.
void NativeCreateSaveSlot(const gameswf::fn_call& call) {
    if (call.nargs != 2) {
        return;
    }
    const char* saveName = call.arg(0).to_tu_string().c_str();
    const char* className = call.arg(1).to_tu_string().c_str();
    const int classId = pydata::FindMemberName(Arrays::CharacterTable::m_memberNames,
                                               static_cast<int>(Arrays::CharacterTable::size), className);
    if (IsPlayableClassID(classId)) {
        const std::uint32_t slot = PlayerSavegame::SG_GetNextFreeSlot();
        PlayerSavegame save(slot, 1, false);
        save.m_name = saveName;
        save.m_playerLevel = 1;      // +0x30
        save.m_playerClass = classId;  // +0x34
        if (static_cast<std::int32_t>(save.m_saveDate) < 0) {
            save.m_saveDate = 0;     // +0x38
        }
        PlayerSavegame::m_difficultyLevel = 0;
        save.SG_GenerateSeeds();
        save.SG_SetSaveDate();
        save.m_unlockedDifficulty = 1;  // +0x3c
        for (std::uint32_t difficulty = 0; difficulty != Arrays::GameDifficulties::size; ++difficulty) {
            save.unk_50[difficulty] = 41;            // +0x50 level id
            save.m_levelEntryPoint[difficulty] = 0;  // +0x40
            save.m_useSpawnPoint[difficulty] = true; // +0x4c
        }
        save.SG_Save();
        call.result->set_double(static_cast<double>(slot));
    }
}

// 0x43fa58 NativeGetLoadingProgress(gameswf::fn_call const&)
// No arguments. With a current level the result is its load progress (Level +0x30); otherwise it is 0 while the online state is in
// mode 3 (GLLive) and 100 in any other mode. The original also constructs and destroys a local OnlineGameState, which does not
// touch the singleton and is not repeated.
void NativeGetLoadingProgress(const gameswf::fn_call& call) {
    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    int percent;
    if (level) {
        percent = level->m_loadProgressPercent;
    } else {
        percent = OnlineSingleton<OnlineGameState>::GetInstance()->mode_ == 3 ? 0 : 100;
    }
    call.result->set_double(static_cast<double>(percent));
}

// 0x43fdbc NativeConnectToServer(gameswf::fn_call const&)
// PARTIAL. One argument, the room index (a number). The rooms are a std::vector<tRoomInfo> (968 bytes each): when the online state
// (OnlineSingleton<OnlineGameState>::GetInstance()) has mode_ 2 they come from CMatching::Get() through its virtual at vtable +0x90
// (CMatchingGLLive::GetRoomList), otherwise they are the state's own vector (+0x50). An index in [0, count) is stored at the state's
// +0x30 and the state machine state (+0x14) becomes 103 (OnlineGameState::SetState). The result is true. Blocked on
// net-session-online: GetRoomList (0x82051c) is a skipped row there, the state's room vector (roomInfos_) has no accessor and
// +0x30 has no field.
void NativeConnectToServer(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43fec4 NativeGetNextStatusMessage(gameswf::fn_call const&)
// One or two arguments: a number and, when there are two, a second one that is undefined or a boolean. The original builds the
// text "not yet supported with no hud code", reads GetLocalPlayer(0, false) and the second argument, and uses none of them.
// When the status queue 0 of MenuMessageManager<StatusMsg, 4> has a message, its text is the result. The message is not removed.
void NativeGetNextStatusMessage(const gameswf::fn_call& call) {
    const int count = call.nargs;
    if (count != 1 && count != 2) {
        return;
    }
    if (!call.arg(0).is_number()) {
        return;
    }
    if (count == 2 && !call.arg(1).is_bool() && !call.arg(1).is_undefined()) {
        return;
    }
    Singleton<Application>::s_inst.m_playerManager->GetLocalPlayer(0, false);
    if (count == 2) {
        call.arg(1).to_bool();
    }
    const std::deque<StatusMsg>& queue = MenuMessageManager<StatusMsg, 4>::s_inst.m_queues[0];
    if (!queue.empty()) {
        const std::string text = queue.front();
        call.result->set_string(text.c_str());
    }
}

// 0x4401d4 NativeGetQuestCount(gameswf::fn_call const&)
// Argument 0 is the player index, argument 1 the quest state name. The result is the number of the character's quests that
// GetQuestFunctor(name) accepts (Character::SG_GetNumQuests(filter, -1)), or -1 without a character.
void NativeGetQuestCount(const gameswf::fn_call& call) {
    const int player = static_cast<int>(call.arg(0).to_number());
    const char* stateName = call.arg(1).to_tu_string().c_str();
    Character* ch = NativeGetPlayerChar(player, false);
    int count = -1;
    if (ch) {
        count = ch->SG_GetNumQuests(GetQuestFunctor(stateName), -1);
    }
    call.result->set_double(static_cast<double>(count));
}

// 0x440278 NativeGetNextAchievementMessage(gameswf::fn_call const&)
// One object argument. The head message of the achievement queue (MenuMessageManager<AchievementMsg, 1>, queue 0) is copied into a
// local AchievementMsg (empty name and description, zeros otherwise) and the object gets Name, Desc, TrophyType, Grade and Label.
// The result is true when a message was copied, false when the queue is empty. The message is not removed.
void NativeGetNextAchievementMessage(const gameswf::fn_call& call) {
    if (call.nargs != 1 || !call.arg(0).is_object()) {
        return;
    }
    gameswf::as_object* object = call.arg(0).to_object();
    AchievementMsg message(std::string(), std::string(), 0, 0, 0);
    const std::deque<AchievementMsg>& queue = MenuMessageManager<AchievementMsg, 1>::s_inst.m_queues[0];
    bool copied = false;
    if (!queue.empty()) {
        const AchievementMsg& head = queue.front();
        static_cast<std::string&>(message) = head;
        message.m_desc = head.m_desc;
        message.m_trophyType = head.m_trophyType;
        message.m_grade = head.m_grade;
        message.m_label = head.m_label;
        object->set_member(tu_stringi("Name"), gameswf::as_value(static_cast<const std::string&>(message).c_str()));
        object->set_member(tu_stringi("Desc"), gameswf::as_value(message.m_desc.c_str()));
        object->set_member(tu_stringi("TrophyType"), gameswf::as_value(message.m_trophyType));
        object->set_member(tu_stringi("Grade"), gameswf::as_value(message.m_grade));
        object->set_member(tu_stringi("Label"), gameswf::as_value(message.m_label));
        copied = true;
    }
    call.result->set_bool(copied);
}

// 0x44058c NativeMerchantSellItem(gameswf::fn_call const&)
// Arguments: the item index (0), the player index (1), the merchant object's name (2) and the object-lookup flags (3). Only when
// the "menu_Merchant" menu exists and its busy byte (+0xc4, cleared by MenuMerchant::Update every frame) is clear, the byte is set
// and the sale is made: the merchant (an object of that name with IsMerchant) buys the player's item at its row's sell percentage
// (Arrays::MerchantTable row of GetLoot, +0x04) with price MerchantTradePrice(item value, percent). The gold goes to the player,
// the item is marked as buy-back (ItemInstance +0x69) and moved to the merchant (quantity 1, no stack, gold converted), the skin
// is refreshed, the sold counter (property 220) counts up and unlocks "sell_100_items" above 99. The result is always false.
void NativeMerchantSellItem(const gameswf::fn_call& call) {
    MenuMerchant* menu = static_cast<MenuMerchant*>(MenuManager::GetInstance()->GetMenuByName("menu_Merchant"));
    if (menu && !menu->unk_c4) {
        menu->unk_c4 = true;
        const int itemIndex = static_cast<int>(call.arg(0).to_number());
        const int player = static_cast<int>(call.arg(1).to_number());
        const char* merchantName = call.arg(2).to_string();
        const int flags = static_cast<int>(call.arg(3).to_number());
        Character* ch = NativeGetPlayerChar(player, false);
        if (ch) {
            ObjectHandle handle = Singleton<Application>::s_inst.m_objectManager->GetObjectByName(merchantName, flags, false, nullptr);
            Character* merchant = handle.operator Character*();
            if (merchant && merchant->IsMerchant()) {
                ItemInventory& playerInventory = *ch;
                if (static_cast<unsigned int>(itemIndex) < static_cast<unsigned int>(playerInventory.GetNumItems()) && itemIndex >= 0) {
                    const Structs::Merchant* row = reinterpret_cast<const Structs::Merchant*>(Arrays::MerchantTable::members) + merchant->GetLoot();
                    ItemInstance* item = playerInventory.GetItem(static_cast<unsigned int>(itemIndex));
                    const int price = MerchantTradePrice(item->m_value, row->m_w1);  // sell column +0x04
                    item->GetItem();
                    playerInventory.AddGold(price);
                    DebugSwitches::s_inst.load();
                    (void)DebugSwitches::s_inst.GetSwitch(std::string("isTracingMerchant"));
                    item->unk_69 = true;
                    playerInventory.TransferItemTo(static_cast<unsigned int>(itemIndex), static_cast<ItemInventory&>(*merchant), 1, false, true);
                    ch->INV_UpdateSkin();
                    ch->PROPS_AddInt(220, 1);
                    TrophyManager* trophies = TrophyManager::s_instance;
                    if (ch->PROPS_GetInt(220, false) > 99) {
                        trophies->UnlockTrophy(Arrays::GetMemberIDByString<Arrays::TrophyTable>("sell_100_items"));
                    }
                }
            }
        }
    }
    call.result->set_bool(false);
}

// 0x4408d4 NativeMerchantBuyItem(gameswf::fn_call const&)
// Arguments as for the sale. The result is a number: 1 when the price exceeds the player's gold, 2 when the inventory is full and
// the item is not a potion (item type 14), 3 when it is a potion and the potion count already equals the player's maximum, and
// 0 otherwise (also when a lookup or check fails). The price is MerchantTradePrice(item value, percent) with the merchant row's
// buy column (+0x08), or its sell column (+0x04) for an item marked as buy-back. A purchase takes the gold, clears the buy-back
// mark, moves the item to the player (quantity 1, no stack, gold converted), adds the price to the spent counter (property 221) and
// unlocks "spend_1000_gold" at 1000 and "spend_10000_gold" above 9999.
void NativeMerchantBuyItem(const gameswf::fn_call& call) {
    const int itemIndex = static_cast<int>(call.arg(0).to_number());
    const int player = static_cast<int>(call.arg(1).to_number());
    const char* merchantName = call.arg(2).to_string();
    const int flags = static_cast<int>(call.arg(3).to_number());
    double result = 0.0;
    Character* ch = NativeGetPlayerChar(player, false);
    if (ch) {
        ObjectHandle handle = Singleton<Application>::s_inst.m_objectManager->GetObjectByName(merchantName, flags, false, nullptr);
        Character* merchant = handle.operator Character*();
        if (merchant && merchant->IsMerchant()) {
            ItemInventory& merchantInventory = *merchant;
            if (static_cast<unsigned int>(itemIndex) < static_cast<unsigned int>(merchantInventory.GetNumItems()) && itemIndex >= 0) {
                const Structs::Merchant* row = reinterpret_cast<const Structs::Merchant*>(Arrays::MerchantTable::members) + merchant->GetLoot();
                ItemInstance* item = merchantInventory.GetItem(static_cast<unsigned int>(itemIndex));
                const int percent = item->unk_69 ? row->m_w1 : row->m_w2;  // sell column for a buy-back item, else the buy column
                const int price = MerchantTradePrice(item->m_value, percent);
                const int itemType = item->GetItem()->m_w22;  // Structs::Item +0x58
                ItemInventory& playerInventory = *ch;
                bool purchase = false;
                if (price > playerInventory.m_gold) {  // ItemInventory +0x20
                    result = 1.0;
                } else {
                    const bool potion = itemType == 14;
                    if (playerInventory.IsInventoryFull() && !potion) {
                        result = 2.0;
                    } else if (potion && playerInventory.GetNumPotions() == playerInventory.m_maxPotions) {
                        result = 3.0;
                    } else {
                        purchase = true;
                    }
                }
                if (purchase) {
                    playerInventory.AddGold(-price);
                    DebugSwitches::s_inst.load();
                    (void)DebugSwitches::s_inst.GetSwitch(std::string("isTracingMerchant"));
                    item->unk_69 = false;
                    merchantInventory.TransferItemTo(static_cast<unsigned int>(itemIndex), playerInventory, 1, false, true);
                    ch->PROPS_AddInt(221, price);
                    TrophyManager* trophies = TrophyManager::s_instance;
                    if (ch->PROPS_GetInt(221, false) >= 1000) {
                        trophies->UnlockTrophy(Arrays::GetMemberIDByString<Arrays::TrophyTable>("spend_1000_gold"));
                    }
                    if (ch->PROPS_GetInt(221, false) > 9999) {
                        trophies->UnlockTrophy(Arrays::GetMemberIDByString<Arrays::TrophyTable>("spend_10000_gold"));
                    }
                }
            }
        }
    }
    call.result->set_double(result);
}

// 0x440c84 NativeHasPushNotification(gameswf::fn_call const&)
// PARTIAL. No arguments. The result is false when the device gate (PushNotificationGate: Device::IsHighPerformance,
// Is_In_Multiplayer_Mode, SHARP_DEVICES, HTC_DEVICES) is closed; otherwise it is true when byte_99F83A or byte_99F81A (the GameCenter
// invite flags, dh2/global/gc_invite_flags.hpp) is set or the byte at +0x15 of GCSingleton<GameCenter>::GetInstance() (the start
// gate) is set. Blocked on the GameCenter duplicate: GCSingleton<GameCenter> (core-platform/gc_singleton.hpp) instantiates
// core-platform/deps/game_center.hpp, a stand-in class, and net-session-online's real GameCenter has the gate (startGate_) private.
void NativeHasPushNotification(const gameswf::fn_call& call) {
    (void)call;
}

// The start gate of NativeHasPushNotification. The original passes Device::IsHighPerformance() (the call takes the
// fn_call pointer in the pseudocode, but the function is static with no argument) and tests the three globals.
bool PushNotificationGate() {
    return Device::IsHighPerformance() || Is_In_Multiplayer_Mode || SHARP_DEVICES || HTC_DEVICES;
}

// 0x440d3c NativeGLXPlayerSignin(gameswf::fn_call const&)
// PARTIAL. Returns at once unless this is an online game (GetOnlineGameFlag) with three arguments: the login name, the password
// (both tu_strings) and the remember flag (a boolean), and the online state's byte at +0x27 (OnlineGameState::flag27_) is set. Then
// the flag is stored in the global save_remember, the name and the password are strdup'ed, the flag is stored in the
// CMatching::Get() byte at +27633, a CSignInCredentials (12 bytes from CustomAlloc: name, password, GameCenter +0x18 pointer) is built,
// and CSignIn::Get()'s virtual at vtable +0x10 is called with it (its result is the native's Hex-Rays return, not a result value).
// Blocked on net-session-online: CSignInCredentials, the CSignIn virtual, the save_remember global, the login-remember byte of
// CMatching and the GameCenter singleton (see NativeHasPushNotification) are not declared.
void NativeGLXPlayerSignin(const gameswf::fn_call& call) {
    (void)call;
}
