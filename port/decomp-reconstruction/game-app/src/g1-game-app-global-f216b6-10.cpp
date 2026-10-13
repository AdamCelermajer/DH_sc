// Batch g1-game-app-global-f216b6-10 (<global>, part 11 of 22): the gameswf natives that act on a player's
// character (potion, quest, stats, skills, equipment, HUD skill, lock and unlock, save, start game), ELF
// 0x43d2c8 .. 0x43eac0. Declarations: dh2/global/native_player_actions.hpp.
//
// Facts shared by the bodies:
//   - A native is void(const gameswf::fn_call&). Argument i is read by the pseudocode as the environment
//     stack entry at (first argument index - i), 12 bytes per gameswf::as_value. Type codes the pseudocode
//     checks: 1 boolean, 2 number, 5 object. The gameswf accessors are not in the tree, so no native reads an
//     argument or writes its result. Each body that acts on the values is the *OfPlayer helper, and the
//     native is PARTIAL until it can call it.
//   - NativeGetPlayerChar(player, false) returns the character, or null.
//   - Character::m_controller (v2Controllable +0x04; Character +0x378) is the v2Controller. Its m_blocked is
//     the byte at +0x08.
//   - The format string the item name uses is the binary's own text ("<font color='#%000000X'>%s</font>").
//   - Hex-Rays dropped uses of some arguments (NativeMoveCharacter). Those were checked against the assembly.
//     The asm is the source where the two differ.

#include "dh2/global/native_player_actions.hpp"

#include "dh2/Application.hpp"
#include "gameswf/gameswf_environment.h"
#include "vendor-gameswf/as_value.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/CharProperties.hpp"
#include "dh2/Character.hpp"
#include "dh2/ItemInstance.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/TrophyManager.hpp"
#include "dh2/ZoomHandler.hpp"
#include "dh2/global/charai_ai.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "dh2/v2Controllable.hpp"
#include "dh2/v2Controller.hpp"
#include "pydata-accessors/w7_pydata12.hpp"

// The pydata Arrays lookup. pydata-accessors/member_names.hpp declares Arrays::GetMemberIDByString and defines
// Structs tables that pydata-accessors/structs_records.hpp (included through CharProperties.hpp) also defines, so
// the two headers cannot share a translation unit. The declarations below match them. Arrays::TrophyTable is a
// struct in w7_arrays_00.hpp.
namespace Arrays {
struct TrophyTable;
template <class T>
int GetMemberIDByString(const char* name);
}  // namespace Arrays

namespace {

// TrophyManager::UnlockTrophy(Arrays::GetMemberIDByString<Arrays::TrophyTable>(name)). The original reads
// TrophyManager::s_instance once, after Character::SG_Save. A null instance is not reachable in the original's
// normal path; it is skipped here.
void UnlockTrophyNamed(TrophyManager* trophies, const char* name) {
    if (trophies) {
        trophies->UnlockTrophy(Arrays::GetMemberIDByString<Arrays::TrophyTable>(name));
    }
}

}  // namespace

// 0x43d2c8 NativeUsePotion(gameswf::fn_call const&)
// PARTIAL: argument 0 (player index) is not read. The PotionDrink sound (VoxSoundManager::Play with
// Arrays::GetMemberIDByString<Arrays::Sounds>("PotionDrink")) is left out: VoxSoundManager::s_instance and Play are
// not declared in the tree.
void NativeUsePotion(const gameswf::fn_call& call) {
    (void)call;
}

// Body of NativeUsePotion on the decoded player index. Without a character, or one without a potion, nothing
// happens. Otherwise the potion is used unless both HP and MP are full. The asm truncates each fraction to int and
// converts it back before the compare with 1.0f (__aeabi_f2iz, __aeabi_i2f, __aeabi_fcmplt), so the tests are
// (float)(int)hp < 1.0f and then (float)(int)mp < 1.0f (asm of 0x43d2c8, checked).
void UsePotionOnPlayer(int player) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (!ch || !ch->HasPotion()) {
        return;
    }
    // GetHPPercent and GetMPPercent return float in the binary (asm of 0x3bd2dc and 0x3bd338, float __fastcall, each
    // ends in __aeabi_fdiv). Character.hpp declares them int (layout request). The chain below is right for the
    // float return and also compiles against the declared int.
    const float hpPercent = static_cast<float>(static_cast<int>(ch->GetHPPercent()));
    const float mpPercent = static_cast<float>(static_cast<int>(ch->GetMPPercent()));
    if (hpPercent < 1.0f || mpPercent < 1.0f) {
        ch->m_controller->Cmd_UsePotion();
        // PARTIAL: the PotionDrink sound is not played (see NativeUsePotion).
    }
}

// 0x43d3b4 NativeSetCurrentQuest(gameswf::fn_call const&): SetCurrentQuestOfPlayer(argument 0 = player, argument 1 = quest).
void NativeSetCurrentQuest(const gameswf::fn_call& call) {
    SetCurrentQuestOfPlayer(static_cast<int>(call.arg(0).to_number()), static_cast<int>(call.arg(1).to_number()));
}

// Body of NativeSetCurrentQuest: Character::SG_SetCurrentQuest(quest, -1) on the player's character.
void SetCurrentQuestOfPlayer(int player, int quest) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (ch) {
        ch->SG_SetCurrentQuest(quest, -1);
    }
}

// 0x43d43c NativeStatsRemoveAssign(gameswf::fn_call const&)
// PARTIAL: arguments 0 to 4 (a, b, c, d, player) are not read.
void NativeStatsRemoveAssign(const gameswf::fn_call& call) {
    (void)call;
}

// Body of NativeStatsRemoveAssign: Character::ResetStatsChange(a, b, c, d) on the player's character.
void ResetStatsChangeOfPlayer(int player, int a, int b, int c, int d) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (ch) {
        ch->ResetStatsChange(a, b, c, d);
    }
}

// 0x43d548 NativeSkillsTrainSkill(gameswf::fn_call const&)
// PARTIAL: arguments 0 (skill), 1 (player) and, when there are three, 2 (flag) are not read, because gameswf::fn_call
// has no definition in the tree. The result is not set either. The original sets a bool (IncSkill's result) when the
// flag is set, otherwise a number (CharProperties PROPS_GetInt(157, false)) with as_value::set_double. The bool
// return of Character::IncSkill is declared now (dh2/Character.hpp), and TrainSkillOfPlayer returns it.
void NativeSkillsTrainSkill(const gameswf::fn_call& call) {
    (void)call;
}

// Body of NativeSkillsTrainSkill: Character::IncSkill(skill, flag) on the player's character. It returns the bool that
// IncSkill returns (false when there is no character). The native does not store it yet, see NativeSkillsTrainSkill.
bool TrainSkillOfPlayer(int player, int skill, bool flag) {
    Character* ch = NativeGetPlayerChar(player, false);
    return ch ? ch->IncSkill(skill, flag) : false;
}

// 0x43d63c NativeEquipSkill(gameswf::fn_call const&): EquipSkillInSlotOfPlayer(player = argument 2, slot = argument 0, skill = argument 1).
void NativeEquipSkill(const gameswf::fn_call& call) {
    EquipSkillInSlotOfPlayer(static_cast<int>(call.arg(2).to_number()), static_cast<int>(call.arg(0).to_number()),
                             static_cast<int>(call.arg(1).to_number()));
}

// Body of NativeEquipSkill: when the character can equip the skill, Character::SG_SetSkillInSlot(slot, skill).
void EquipSkillInSlotOfPlayer(int player, int slot, int skill) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (ch && ch->IsSkillEquippable(skill)) {
        ch->SG_SetSkillInSlot(slot, static_cast<unsigned int>(skill));
    }
}

// 0x43d6f8 NativePinchZoom(gameswf::fn_call const&): PinchZoomBy(argument 0), which holds the DPad gate.
void NativePinchZoom(const gameswf::fn_call& call) {
    PinchZoomBy(static_cast<int>(call.arg(0).to_number()));
}

// Body of NativePinchZoom. Runs only when the DPad option is set, and only with a bound camera. Then the zoom request
// (CameraLevel::m_zoomCurrent, +0x88) grows by amount times ZoomHandler::m_sensitivity, unless the camera is locked
// (m_zoomLocked, +0x86). The original's float arithmetic is PinchZoomStep (native_player_actions.hpp).
void PinchZoomBy(int amount) {
    Application& app = ApplicationGetInstance();
    if (!app.GetSavedOption("DPad")) {
        return;
    }
    ZoomHandler* zoom = app.m_zoomHandler;
    if (!zoom || !zoom->m_camera) {
        return;
    }
    CameraLevel* camera = zoom->m_camera;
    if (!camera->m_zoomLocked) {
        camera->m_zoomCurrent = PinchZoomStep(camera->m_zoomCurrent, amount, zoom->m_sensitivity);
    }
}

// 0x43d790 NativeInvGetEquipedItem(gameswf::fn_call const&)
// PARTIAL: arguments (slot, result object, player) are not read, and the result object is not written. The
// original writes the object's ItemName (sprintf of the format and the item name), ItemIndex and ItemColor, then
// sets the bool result. The ItemColor key is set from ItemInstance::GetNumPowers, as in the binary. Missing:
// the gameswf object and value API (as_object set_member, as_value constructors, set_bool, set_undefined) and the
// item name pointer that the sprintf reads at ItemInstance +0x1c (v22[7]), which ItemInstance does not declare.
// When the slot is out of range the original evaluates PyDataConstants "EquipmentSlots"/"Count" and discards it,
// so that lookup is left out.
void NativeInvGetEquipedItem(const gameswf::fn_call& call) {
    (void)call;
}

// Body of NativeInvGetEquipedItem. Returns false when there is no character, the slot is out of range, or there is
// no item in the slot. Otherwise it returns true and gives the color, index and power count of the item.
bool GetEquippedItemInfo(int player, int slot, int* color, int* index, int* numPowers) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (!ch) {
        return false;
    }
    if (!IsEquipmentSlotInRange(slot, ch->ItemInventory::GetNumEquipmentSlots())) {
        return false;
    }
    ItemInstance* item = ch->ItemInventory::GetEquippedItem(static_cast<unsigned int>(slot));
    if (!item) {
        return false;
    }
    *color = item->GetColor();
    *index = ch->ItemInventory::GetItemIndex(item);
    *numPowers = item->GetNumPowers();
    return true;
}

// 0x43da2c NativeInvAutoEquipSlot(gameswf::fn_call const&): with two arguments, AutoEquipSlotOfPlayer(player = argument 1, slot = argument 0).
void NativeInvAutoEquipSlot(const gameswf::fn_call& call) {
    if (call.nargs == 2) {
        AutoEquipSlotOfPlayer(static_cast<int>(call.arg(1).to_number()), static_cast<int>(call.arg(0).to_number()));
    }
}

// Body of NativeInvAutoEquipSlot. Dispatch is in ClassifyAutoEquipSlot. Character vtable slots: +0x138 is
// EquipAllSlotsAuto, +0x140 is EquipSlotAuto and +0x144 is UnEquipItemFromSlot (asm checked). The in-range path
// unequips first, then equips automatically, and returns the EquipSlotAuto result.
void AutoEquipSlotOfPlayer(int player, int slot) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (!ch) {
        return;
    }
    switch (ClassifyAutoEquipSlot(slot, ch->ItemInventory::GetNumEquipmentSlots())) {
        case AutoEquipAction::kEquipAll:
            ch->EquipAllSlotsAuto();
            break;
        case AutoEquipAction::kUnequipThenEquip:
            ch->UnEquipItemFromSlot(static_cast<unsigned int>(slot));
            ch->EquipSlotAuto(static_cast<unsigned int>(slot));
            break;
        case AutoEquipAction::kNone:
            break;
    }
}

// 0x43dafc NativeOnHUDOpenEvent(gameswf::fn_call const&)
// PARTIAL: arguments 0 (flag, a boolean) and 1 (index, a number) are not read, and the write is left out. The asm
// stores the flag byte at Level +0x190 plus the index (ADD with no scaling, then STRB at +0x190). Level declares
// only pad_190[4] (layout request: a byte array of at least 4 entries at Level +0x190, indexed by the HUD index).
// The original does not check that a level exists.
void NativeOnHUDOpenEvent(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43dbb8 NativeReloadSkills(gameswf::fn_call const&): ReloadSkillsOfPlayer(argument 0, or the player 0 when there is no argument).
void NativeReloadSkills(const gameswf::fn_call& call) {
    const int player = call.nargs == 1 ? static_cast<int>(call.arg(0).to_number()) : 0;
    ReloadSkillsOfPlayer(player);
}

// Body of NativeReloadSkills.
void ReloadSkillsOfPlayer(int player) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (ch) {
        ch->ReloadSkills();
    }
}

// 0x43dc08 NativeSaveGame(gameswf::fn_call const&): SaveGameOfPlayer(argument 0, or the player 0 when there is no argument).
void NativeSaveGame(const gameswf::fn_call& call) {
    const int player = call.nargs == 1 ? static_cast<int>(call.arg(0).to_number()) : 0;
    SaveGameOfPlayer(player);
}

// Body of NativeSaveGame: Character::SG_Save, then the skill trophies. For each skill row (PlayerSavegame
// m_skillsSize rows, at least one), a level of 0 clears the all-skills flag and a level at the maximum unlocks
// "epic_maxskill". When the character is a player, the all-skills trophy is unlocked if the flag is still set, and
// "epic_spent_all_skill_points" when property 157 is zero and property 19 equals the MaxLevelDVeryHard constant.
// The asm checked the loop: the first level is tested before the bound, so the loop body runs once even when
// m_skillsSize is not positive. PyDataConstants is read once here instead of per iteration (a pure lookup).
void SaveGameOfPlayer(int player) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (!ch) {
        return;
    }
    ch->SG_Save();
    TrophyManager* trophies = TrophyManager::s_instance;
    PlayerSavegame* save = ch->m_savegame;
    bool allSkillsNonZero = true;  // the original's v7
    if (save && save->m_skillsSize != 0) {
        const int maxSkillLevel = ApplicationGetPyDataConstants()->getConstant("CharacterDesign", "MaxSkillLevelDVeryHard");
        int k = 0;
        do {
            const int level = ch->SG_GetSkillLevel(static_cast<unsigned int>(k));
            if (level == 0) {
                allSkillsNonZero = false;
            } else if (level == maxSkillLevel) {
                UnlockTrophyNamed(trophies, "epic_maxskill");
            }
            ++k;
        } while (k < save->m_skillsSize);
    }
    if (ch->IsPlayer()) {
        if (allSkillsNonZero) {
            UnlockTrophyNamed(trophies, "epic_all_skills_unlocked");
        }
        if (ch->PROPS_GetInt(157, false) == 0) {
            const int level = ch->PROPS_GetInt(19, false);
            if (level == ApplicationGetPyDataConstants()->getConstant("CharacterDesign", "MaxLevelDVeryHard")) {
                UnlockTrophyNamed(trophies, "epic_spent_all_skill_points");
            }
        }
    }
}

// 0x43de30 NativeStartFromGCInvite(gameswf::fn_call const&)
// PARTIAL, stub: the body is not reconstructed. The optional argument 0 (save slot) is not read. The
// original tests Device::IsHighPerformance, Is_In_Multiplayer_Mode, SHARP_DEVICES and HTC_DEVICES, then
// PlayerSavegame::SG_Exists(slot), Application::IsWifiEnabled and OnlineGameState::Init(3), and it calls
// RenderFX::InvokeASCallback("_root", "SettingInvitationPending" / "GoToVerificationScreen") with gameswf values.
// Missing: RenderFX::InvokeASCallback and the gameswf value API, the two .bss flags (byte_99F81A, byte_99F83A) that
// have no names yet, and the result set_bool.
void NativeStartFromGCInvite(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43e0d0 NativeStartGame(gameswf::fn_call const&)
// PARTIAL, stub: the body is not reconstructed. The optional argument 0 (difficulty) is not read. The original
// returns when a level exists and its m_loadStep (Level +0x130) is 38. Otherwise it reads the local player through
// PlayerManager::GetLocalPlayer(0, false) and its save slot at +0x664, builds a PlayerSavegame(slot, 1, 0), and,
// when m_unlockedDifficulty (PlayerSavegame +0x3c) is at least the argument, sets PlayerSavegame::m_difficultyLevel
// to it. It then saves, picks the level from Arrays::LevelList and the difficulty table, and calls
// Application::LoadLevel with ten arguments. Missing: the local player's layout (PlayerInfo is incomplete; the
// original casts it to PlayerSavegame), GetOnline()->+0x05, the Arrays::LevelList and DesignSettingsTable stride
// reads, and the gameswf argument.
void NativeStartGame(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43e308 NativeStartMultiplayerGame(gameswf::fn_call const&)
// PARTIAL, stub: the body is not reconstructed. The original first sets OnlineGameState +0x14 (the state machine's
// state) to 300 on every path, then branches on the mode at +0x34 (asm of 0x43e308, checked):
//   - mode != 3: tail-calls NativeStartGame(call). NativeStartGame is called only on this path.
//   - mode == 3 and the byte at +0x28 is set: returns, with nothing else done.
//   - mode == 3 and the byte at +0x28 is clear: calls CMatchingGLLive::StartGame on CMatching::Get(), then sets
//     +0x28 to 1.
// The branch is ClassifyMultiplayerStart (native_player_actions.hpp). Missing: the OnlineGameState fields +0x14
// (stateMachine_ is private, with no setter), +0x28 and +0x34 (not declared in net-session-online/online_game_state.hpp;
// layout request), and the CMatching::Get() call with CMatchingGLLive::StartGame. The CMatching and CMatchingGLLive
// headers are present now, so only the OnlineGameState fields and the mode read block the body.
void NativeStartMultiplayerGame(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43e494 NativeStatsReset(gameswf::fn_call const&)
// PARTIAL: arguments 0 (player, a number) and 1 (quote-only flag, a boolean) are not read, and the result is not
// set. The original sets the result to the gold cost, or clears the result type when there is no character.
void NativeStatsReset(const gameswf::fn_call& call) {
    (void)call;
}

// Body of NativeStatsReset. The cost is "StatsReset_GoldCostPerLevel" times the character's level. Unless
// quoteOnly is set, the character's stats and skills are reset and the cost is taken from the gold. Returns false
// when there is no character (the original then clears the result).
bool ResetStatsOfPlayer(int player, bool quoteOnly, int* costOut) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (!ch) {
        return false;
    }
    const int cost = ApplicationGetPyDataConstants()->getConstant("CharacterDesign", "StatsReset_GoldCostPerLevel") *
                     ch->GetLevel();
    if (!quoteOnly) {
        ch->ResetStats();
        ch->ResetSkills();
        ch->ItemInventory::AddGold(-cost);
    }
    *costOut = cost;
    return true;
}

// 0x43e600 NativeInvEquipItem(gameswf::fn_call const&)
// PARTIAL: arguments 0 (item), 1 (slot) and 2 (player) are not read, and the result is not cleared. The asm shows the
// call is Character::EquipItemToSlot(item, slot) (vtable +0x13c), with the first argument as the item.
void NativeInvEquipItem(const gameswf::fn_call& call) {
    (void)call;
}

// Body of NativeInvEquipItem: Character::EquipItemToSlot(item, slot) on the player's character.
void EquipItemInSlotOfPlayer(int player, int item, int slot) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (ch) {
        ch->EquipItemToSlot(static_cast<unsigned int>(item), static_cast<unsigned int>(slot));
    }
}

// 0x43e734 NativeHUDSkill(gameswf::fn_call const&): with one numeric argument, HudSkillForLocalPlayer(slot).
void NativeHUDSkill(const gameswf::fn_call& call) {
    if (call.nargs == 1 && call.arg(0).is_number()) {
        HudSkillForLocalPlayer(call.arg(0).to_int());
    }
}

// Body of NativeHUDSkill on the local player's character. Acts only when the character may act (CTRLIsAllowed) and
// the skill slot holds a skill. The original begins and ends the skill on the character's controller.
void HudSkillOnCharacter(Character* ch, int slot) {
    if (!ch || !ch->CTRLIsAllowed()) {
        return;
    }
    const int skill = ch->SG_GetSkillInSlot(slot);
    if (skill == -1) {
        return;
    }
    ch->m_controller->Cmd_BeginSkill(static_cast<unsigned int>(skill));
    ch->m_controller->Cmd_EndSkill(static_cast<unsigned int>(skill));
}

// Body of NativeHUDSkill for the local player. The original takes GetLocalPlayer(0, true) (PlayerManager on
// Application +0x40) and its character at PlayerInfo +0x660 (PlayerInfo::unk_660), then calls HudSkillOnCharacter. The
// original does not test the player pointer; the null test here is a guard that cannot change a reachable result.
void HudSkillForLocalPlayer(int slot) {
    PlayerInfo* local = ApplicationGetInstance().m_playerManager->GetLocalPlayer(0, true);
    if (!local) {
        return;
    }
    HudSkillOnCharacter(local->unk_660, slot);
}

// 0x43e830 NativeMoveCharacter(gameswf::fn_call const&)
// PARTIAL, stub: the body is not reconstructed. The original checks that argument 0 is a number and not NaN
// (asm: __aeabi_dcmpun), that arguments 1 and 2 are numbers, and that the character may act (CTRLIsAllowed). It
// then takes the unit vector (1, -1, 0), rotates it by 90 - angle about the origin (Point3D<float>::normalize and
// rotateXYBy), scales it by argument 1, and calls v2Controller::Cmd_HeadTowards(Point3D<float> const&) with the
// result. Missing: the Point3D<float> methods (the game_types.hpp Point3D is a plain struct) and the gameswf
// argument and type checks.
void NativeMoveCharacter(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ea0c NativeUnlockCharacter(gameswf::fn_call const&): with one numeric argument, UnlockCharacterControl(player).
void NativeUnlockCharacter(const gameswf::fn_call& call) {
    if (call.nargs == 1 && call.arg(0).is_number()) {
        UnlockCharacterControl(call.arg(0).to_int());
    }
}

// Body of NativeUnlockCharacter: clears the controller's block flag and the zoom lock.
void UnlockCharacterControl(int player) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (!ch) {
        return;
    }
    ch->m_controller->m_blocked = false;
    NativeSetLockZoom(false);
}

// 0x43eac0 NativeLockCharacter(gameswf::fn_call const&): with one numeric argument, LockCharacterControl(player).
void NativeLockCharacter(const gameswf::fn_call& call) {
    if (call.nargs == 1 && call.arg(0).is_number()) {
        LockCharacterControl(call.arg(0).to_int());
    }
}

// Body of NativeLockCharacter: stops the character, sets the controller's block flag and the zoom lock.
void LockCharacterControl(int player) {
    Character* ch = NativeGetPlayerChar(player, false);
    if (!ch) {
        return;
    }
    ch->m_controller->Cmd_Stop();
    ch->m_controller->m_blocked = true;
    NativeSetLockZoom(true);
}
