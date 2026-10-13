// Batch g1-game-app-global-f216b6-14 (<global>, part 15 of 22): the gameswf natives of the cursor, HUD, save-slot,
// dialog, tutorial and multiplayer info queries (ELF 0x44e270 .. 0x450b78), and the GetNewScriptCmdImplInstance<T>()
// factories of twelve script commands (ELF 0x457714 .. 0x457a2c). Declarations: dh2/global/native_get.hpp and
// dh2/global/GetNewScriptCmdImplInstance.hpp.
//
// Facts shared by the natives:
//   - gameswf calls each native as void(const fn_call&). From the pseudocode, fn_call +0x00 is the result
//     (as_value*), +0x0c the argument stack (12-byte as_value entries), +0x10 the argument count and +0x14 the
//     index of the first argument.
//   - Every native reads its arguments or writes its result through gameswf::as_value, as_object, as_array,
//     tu_string, root and player. These are vendored in port/engine-ui/vendor/gameswf1714/gameswf, which is not on
//     the Track B include path, so no native here can read or write a value. Each native is PARTIAL with an empty
//     body, as in game-app/src/g1-game-app-global-f216b6-6.cpp.
//   - The dialog and quest queue is MenuMessageManager<DialogMsg,1>. dh2/MenuMessageManager.hpp does not exist yet.
//
// Facts shared by the factories:
//   - Each allocates 0x10 bytes (operator new(0x10u, 0) in the pseudocode). The 64 in the queue is the function body
//     size, not the allocation. The host allocates sizeof(T) and ignores the hint.
//   - The original stores m_args (+0x0c) = 0, unk_04 (+0x04) = 0 and m_cmdId (+0x08) = -1. The vtable (+0x00) is set by
//     the class constructor here.

#include "dh2/global/GetNewScriptCmdImplInstance.hpp"
#include "dh2/global/native_get.hpp"

#include "dh2/ScriptCmdImpl.hpp"
#include "dh2/Script_BlockSaveGame.hpp"
#include "dh2/Script_EnqueueCharMenuTutorialMessage.hpp"
#include "dh2/Script_EnqueueTutorialMessage.hpp"
#include "dh2/Script_FlushMessages.hpp"
#include "dh2/Script_LockTutorial.hpp"
#include "dh2/Script_SaveGame.hpp"
#include "dh2/Script_ShowTrophies.hpp"
#include "dh2/Script_SkipAllCharMenuTutorialMessages.hpp"
#include "dh2/Script_SkipAllTutorialMessages.hpp"
#include "dh2/Script_SkipCharMenuTutorialMessage.hpp"
#include "dh2/Script_SkipTutorialMessage.hpp"

// 0x44e270 NativeGetCursorState(gameswf::fn_call const&)
// PARTIAL: with one argument, the original takes the controller index as to_number(argument 0), gets the controller
// from RenderFX::GetController(MenuManager::GetHUDRoot(), index), converts its (x, y) floats with
// root::screen_to_logical, and sets "x", "y" and "state" (the controller int at +0x0c) on the cursor object at that
// index. The 4-entry cursor array (at +0x154 of the object at MenuManager +0xf4) gets four 56-byte as_objects on first
// use. Any other argument count leaves the result null.
// Missing: the gameswf value, object, array, root and fn_call API; RenderFX::GetController (not declared in
// dh2/RenderFX.hpp); MenuManager::GetHUDRoot returns MenuFX* in dh2/MenuManager.hpp while the original casts it to
// RenderFX*; the MenuManager global dword_99F780 is not declared.
void NativeGetCursorState(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44e5cc NativeGetPlayerHUDInfos(gameswf::fn_call const&)
// PARTIAL: builds the HUD info object of the player that the argument selects. The character comes from a helper
// that the pseudocode calls NativeGetPlayerChar (not declared in the tree). The original reads the character's level,
// HP, MP, XP, spell, three skill percentages and their MP costs, and the potion count through Character::SG_GetSkillLevel,
// Character::SG_GetSkillInSlot, CharAI::AI_SkillInfo, AI_IsSkillUsable, AI_SpellInfo, AI_IsSpellUsable,
// CharProperties::PROPS_GetInt and ItemInventory::GetNumPotions. It also reads PyDataConstants::getConstant and
// Application::GetSavedOption. It sets the members PlayerActive, LEVEL, HP_PCT, HP_LOWPCT, CharacterDesign,
// LowHealthPercentage, MP_PCT, XP_PCT, SPELL_PCT, SPELL_MP, SKILL1_PCT, SKILL1_MP, SKILL2_PCT, SKILL2_MP, SKILL3_PCT,
// SKILL3_MP, NB_POTIONS, AVAIL_POINTS, TouchToMove and DPad.
// Missing: the gameswf API; NativeGetPlayerChar; the percentage formulas and the option mapping (not transcribed).
void NativeGetPlayerHUDInfos(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44f27c NativeGetSaveIDsList(gameswf::fn_call const&)
// PARTIAL: fills the result array with one object per savegame entry. The list comes from
// PlayerSavegame::SG_GetSavegameList(), and each slot id from PlayerSavegame::SG_GetSlotFromFilename(). Each object
// gets SlotID, InUse, PlayerName, PlayerClass, PlayerLVL, StringClassLVL, PlayerLocation, PlayerLastSave and IsActive.
// The class and location strings come from PyDataConstants::getConstant and StringManager::getString, and the level
// label is GAMEPLAYMENUS_LEVEL joined with "%d" and a space. PlayerLastSave is a localtime/strftime string in one of
// four date formats ("%d/%m     %H:%M", "%d.%m.     %H:%M", "%m.%d.     %H:%M", "%m/%d     %H:%M"). The choice of format by
// SavegameManager::getLanguage is not transcribed. The literal "invalid" also appears.
// Missing: the gameswf API (as_array::push, as_object, set_string, set_as_object, to_int); the format choice; the
// PlayerSavegame and SavegameManager members used here.
void NativeGetSaveIDsList(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44fce0 NativeGetNextQuestMessage(gameswf::fn_call const&)
// PARTIAL: requires two arguments, the second an object (type 5) and the first a number. The original casts the object
// (cast_to<as_object>), builds an empty DialogMsg, dequeues the next entry of MenuMessageManager<DialogMsg,1>, sets
// "QuestTitle" and "QuestMessage" on the object, and sets the result to the dequeue flag. The title and the text are
// the DialogMsg title (base string) and m_text (+0x18, dh2/DialogMsg.hpp). Hex-Rays prints the dequeue call with no
// argument and never assigns the title and text pointers, so the call and the two source reads must come from the
// asm (compare NativeGetNextDialogMessage, which passes its DialogMsg).
// Missing: the gameswf API; dh2/MenuMessageManager.hpp (not in the tree); the DialogMsg reads.
void NativeGetNextQuestMessage(const gameswf::fn_call& call) {
    (void)call;
}

// 0x44ff14 NativeGetNextDialogMessage(gameswf::fn_call const&)
// PARTIAL: requires one object argument (type 5). The original casts it to as_object, builds an empty DialogMsg, dequeues
// the next entry of MenuMessageManager<DialogMsg,1> into it and sets the properties "DialogTitle", "DialogMessage",
// "DialogStyle" (a number), "DialogStyleStr" (PyDataConstants::getConstantName("DialogStyles", style)) and
// "DialogActorStr". The result is the dequeue flag. The likely sources are the DialogMsg fields of dh2/DialogMsg.hpp
// (title base string, m_text +0x18, m_style +0x30, m_actorName +0x34), but Hex-Rays never assigns them in the pseudocode,
// so the reads must be taken from the asm.
// Missing: the gameswf API; dh2/MenuMessageManager.hpp; PyDataConstants::getConstantName (not declared in the tree); the
// DialogMsg reads.
void NativeGetNextDialogMessage(const gameswf::fn_call& call) {
    (void)call;
}

// 0x450298 NativeGetNextTutorialMessage(gameswf::fn_call const&)
// PARTIAL: requires one object argument (type 5). The original casts it to as_object and tests the tutorial queue, two
// globals (dword_99FB60 and dword_99FB70) compared for equality as the empty test. When the queue is non-empty, it takes
// the front entry's string id and duration, sets "TutorialMessage" to ParsePlayerName(StringManager::getString(id)) and
// "TutorialDuration" to the duration, and sets the result to whether the queue was non-empty. The pseudocode reads the
// front entry but shows no pop, so the dequeue must be checked in the asm.
// Missing: the gameswf API; the tutorial queue type and its globals (not declared in the tree); the dequeue.
void NativeGetNextTutorialMessage(const gameswf::fn_call& call) {
    (void)call;
}

// 0x4504a0 NativeGetMultiplayerQuestInfo(gameswf::fn_call const&)
// PARTIAL: requires one object argument. It fetches the room's CRoomAttributes through a virtual call at vtable +0xa4 and,
// when attributes 5 and 6 are both active, sets "act", "questName" and "questDesc" on the result object. The act label is
// StringManager::parse of the MENU_ACT string (PyDataConstants::getConstant with the "StrID" table) with attribute 5. The
// quest name and description are StringManager::getString of two ids read from a table entry at +4 and +8. When the
// function-static counter NativeGetMultiplayerQuestInfo::resendCnt is exhausted, it creates a "CMsgAskResendAttributes"
// with CMessage::CreateMessage and sends it through CMessaging::Get()->SendMsg.
// Missing: the gameswf API; the meaning of attributes 5 and 6; the table lookups and branch conditions (not transcribed).
void NativeGetMultiplayerQuestInfo(const gameswf::fn_call& call) {
    (void)call;
}

// 0x450b78 NativeGetMultiplayerRoomInfo(gameswf::fn_call const&)
// PARTIAL: the largest native (44 KB of pseudocode, not transcribed). The original builds the room object and its player slot
// array from the net player list (CNetPlayerManager: GetPlayerCount, GetPlayerIdList, GetPlayer, GetLocalPlayer, IsInitialized;
// CNetPlayerInfo::IsHost), the matching state (CMatching::Get; CMatchingGLLive: IsHost, IsMyFriend, GetGLLiveMemberIdList,
// GetMemberByMemberId), OnlineSingleton<OnlineGameState>::GetInstance(), PlayerManager::GetLocalPlayer and
// GetPlayerByInternalID, and StringManager (getString, formatForMulti). It parses "%d %d" with sscanf. It sets "name",
// "level", "PlayerId", "Class", "playerReady", "Kick", "Slot", "SlotStr" and the invitation states "None", "Friend",
// "Idle" and "Invitation", and appends the slots with gameswf::as_array::push.
// Missing: the gameswf API; the slot and invitation rules (branch conditions); the field formulas.
void NativeGetMultiplayerRoomInfo(const gameswf::fn_call& call) {
    (void)call;
}

namespace {

// The shared body of the twelve 0x10-byte factories: the derived constructor writes the vtable, then the three
// ScriptCmdImpl fields that the original stores after the allocation.
template <class T>
ScriptCmdImpl* NewScriptCmd() {
    T* cmd = new T();
    cmd->m_args = nullptr;     // +0x0c
    cmd->unk_04 = false;       // +0x04
    cmd->m_cmdId = -1;         // +0x08
    return cmd;
}

}  // namespace

// 0x457714 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_FlushMessages>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_FlushMessages>() {
    return NewScriptCmd<Script_FlushMessages>();
}

// 0x45775c ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_DoTutorial>()
// PARTIAL: stub returns nullptr. The original is NewScriptCmd<Script_DoTutorial>(). dh2/Script_DoTutorial.hpp does not
// compile: line 14 marks ~Script_DoTutorial() override, but ScriptCmdImpl's destructor is not virtual (GCC: "marked
// 'override', but does not override"). That header is read-only for this batch. Dropping the override on that line
// clears it; then this body becomes return NewScriptCmd<Script_DoTutorial>() and the include can be added.
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_DoTutorial>() {
    return nullptr;
}

// 0x4577a4 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_LockTutorial>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_LockTutorial>() {
    return NewScriptCmd<Script_LockTutorial>();
}

// 0x4577ec ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SkipAllCharMenuTutorialMessages>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SkipAllCharMenuTutorialMessages>() {
    return NewScriptCmd<Script_SkipAllCharMenuTutorialMessages>();
}

// 0x457834 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SkipCharMenuTutorialMessage>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SkipCharMenuTutorialMessage>() {
    return NewScriptCmd<Script_SkipCharMenuTutorialMessage>();
}

// 0x45787c ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_EnqueueCharMenuTutorialMessage>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_EnqueueCharMenuTutorialMessage>() {
    return NewScriptCmd<Script_EnqueueCharMenuTutorialMessage>();
}

// 0x4578c4 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SkipAllTutorialMessages>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SkipAllTutorialMessages>() {
    return NewScriptCmd<Script_SkipAllTutorialMessages>();
}

// 0x45790c ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SkipTutorialMessage>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SkipTutorialMessage>() {
    return NewScriptCmd<Script_SkipTutorialMessage>();
}

// 0x457954 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_EnqueueTutorialMessage>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_EnqueueTutorialMessage>() {
    return NewScriptCmd<Script_EnqueueTutorialMessage>();
}

// 0x45799c ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_BlockSaveGame>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_BlockSaveGame>() {
    return NewScriptCmd<Script_BlockSaveGame>();
}

// 0x4579e4 ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SaveGame>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_SaveGame>() {
    return NewScriptCmd<Script_SaveGame>();
}

// 0x457a2c ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_ShowTrophies>()
template <>
ScriptCmdImpl* GetNewScriptCmdImplInstance<Script_ShowTrophies>() {
    return NewScriptCmd<Script_ShowTrophies>();
}
