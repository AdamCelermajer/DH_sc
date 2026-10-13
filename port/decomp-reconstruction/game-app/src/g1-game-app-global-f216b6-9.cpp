// Batch g1-game-app-global-f216b6-9 (<global>, part 10 of 22): the quest predicates IsNonDebugQuestClosedQuest,
// IsNonDebugQuestActiveButNotClosedQuest and the IsDebugQuest thunk, and the gameswf natives of the app bindings
// (ELF 0x43c098 .. 0x43d288), with NativeGetPlayerChar. Declarations: dh2/global/quest_predicates.hpp and
// dh2/global/native_app_bindings.hpp.
//
// Facts shared by the bodies:
//   - gameswf natives take one const fn_call&. Argument n is read from the value at index *(fn_call +0x14) - n in the
//     array whose base is **(fn_call +0x0c). fn_call +0x10 is the argument count. The as_value accessors (to_number,
//     to_string, to_xstring, to_bool, set_double) are not in the tree, so a native that reads an argument or stores
//     its result is PARTIAL and its body is left out (see the rows and native_app_bindings.hpp).
//   - The PlayerManager is Application +0x40 (dword_99F76C, extern in dh2/global/IsMultiplayerGame.hpp). PlayerInfo
//     (dh2/PlayerInfo.hpp) is the GetLocalPlayer and GetRemotePlayer result; its character is unk_660 at +0x660.
//     NativeGetPlayerChar and NativeHUDSpell use it and are reconstructed.
//   - The helpers PickHintPageOffset and CreditMovementValue hold the arithmetic of 0x43cb24 and 0x43ccec. They
//     reference no other symbol, so the test links them alone.

#include <cstdint>

#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/Quest.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "dh2/global/native_app_bindings.hpp"
#include "dh2/global/quest_predicates.hpp"
#include "dh2/v2Controller.hpp"

// 0x43b4f0 IsNonDebugQuestClosedQuest(Quest const*)
// Hex-Rays reads the state (+0x00) first, then calls IsNonDebugQuest. When that is true, the result is state > 12,
// which is QUEST_CLOSED (12): the post-closed state (13) is the only state above it. Otherwise the zero result of
// IsNonDebugQuest is returned.
bool IsNonDebugQuestClosedQuest(const Quest* quest) {
    const int state = quest->m_state;                   // +0x00
    if (IsNonDebugQuest(quest)) {
        return state > Quest::QUEST_CLOSED;             // 12
    }
    return false;
}

// 0x43b514 IsNonDebugQuestActiveButNotClosedQuest(Quest const*)
// Hex-Rays: (unsigned)(state - 6) <= 6 when IsNonDebugQuest is true. That is the range QUEST_ACTIVE (6) to
// QUEST_CLOSED (12), both included. Otherwise the zero result of IsNonDebugQuest is returned.
bool IsNonDebugQuestActiveButNotClosedQuest(const Quest* quest) {
    const int state = quest->m_state;                   // +0x00
    if (IsNonDebugQuest(quest)) {
        constexpr unsigned int kSpan = Quest::QUEST_CLOSED - Quest::QUEST_ACTIVE;  // 6
        return static_cast<unsigned int>(state - Quest::QUEST_ACTIVE) <= kSpan;
    }
    return false;
}

// 0x43b53c IsDebugQuest(Quest const*)
// Thunk: a tail branch to Quest::IsDebug, so it is forwarded and not reimplemented.
bool IsDebugQuest(const Quest* quest) {
    return quest->IsDebug();
}

// 0x43c098 NativeConectionType(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original requires exactly one argument (fn_call +0x10 == 1) of string type (the type
// byte is 3 or 4). It dispatches on the string with strcmp against "Bluetooth", "Online", "Wireless" and "PSN".
// Each branch reads COnline::isOnlineGame_ (+0x05, private, no getter, reached through GetOnline(), a thunk to
// COnline::GetInstance()) and the OnlineGameState field at +0x34 (not declared), calls
// CMatching::SetMatchingProvider(0), a virtual CMatching slot at +0x2c (slot 11, unnamed), and
// COnline::SetIsOnlineGame(false). It calls OnlineGameState::Init with mode 2 (Bluetooth), 3 (Online) or 1
// (Wireless, PSN). Online and Wireless call Application::GoToMainMenu(5) and return when Wi-Fi is off
// (Application::IsWifiEnabled). Waits on the as_value string accessor, OnlineGameState +0x34, COnline
// isOnlineGame_ and the slot-11 name.
void NativeConectionType(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43c388 NativeGetPlayerChar(int, bool)
// Reconstructed (asm 0x43c388 checked). The PlayerManager is Application +0x40 (dword_99F76C). It returns nullptr when
// the index is negative or not below PlayerManager::GetNumPlayers(). Otherwise it returns the character at PlayerInfo
// +0x660 of PlayerManager::GetRemotePlayer(index, false) when remote, or of PlayerManager::GetLocalPlayer(index, false).
// The +0x660 field is PlayerInfo::unk_660 (Hex-Rays *(player + 1632)); its name in the header is unk_660.
Character* NativeGetPlayerChar(int playerIndex, bool remote) {
    PlayerManager* players = ApplicationGetInstance().m_playerManager;
    if (playerIndex < 0 || playerIndex >= players->GetNumPlayers()) {
        return nullptr;
    }
    if (remote) {
        return players->GetRemotePlayer(playerIndex, false)->unk_660;
    }
    return players->GetLocalPlayer(playerIndex, false)->unk_660;
}

// 0x43cabc NativeHUDSpell(gameswf::fn_call const&)
// Reconstructed (asm 0x43cabc checked). The local player 0 (PlayerManager::GetLocalPlayer(0, true)) gives its character
// (PlayerInfo +0x660). When the character is set and Character::CTRLIsAllowed() holds, the character's controller
// (Character +0x378, v2Controllable::m_controller) runs Cmd_BeginCast(false) and then Cmd_EndCast(false). The player
// pointer is not tested for null, as in the asm (LDR R4, [R0,#0x660]). The argument is not read.
void NativeHUDSpell(const gameswf::fn_call& /*call*/) {
    PlayerInfo* localPlayer = ApplicationGetInstance().m_playerManager->GetLocalPlayer(0, true);
    Character* character = localPlayer->unk_660;
    if (character != nullptr && character->CTRLIsAllowed()) {
        character->m_controller->Cmd_BeginCast(false);
        character->m_controller->Cmd_EndCast(false);
    }
}

// 0x43cb24 NativeGetLoadingTipStrID(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original picks a hint page with PickHintPageOffset (below, written and tested), then
// increments Random::s_debugRandomCounters[0], and stores the StrID int at +8 of the chosen 12-byte row of
// Arrays::HintPages (members and size, pydata-accessors/w7_arrays27.hpp) with gameswf::as_value::set_double. Missing:
// the set_double store, and the row read, which decodes the int from the file layout of the members buffer.
void NativeGetLoadingTipStrID(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43cc00 NativeGetIPodPlaylistCount(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original stores (double)VoxSoundManager::GetIPodPlaylistCount(VoxSoundManager::s_instance)
// with set_double. Neither the instance nor GetIPodPlaylistCount is declared in vox-audio/vox/sound_manager.hpp, and
// the set_double accessor is not in the tree.
void NativeGetIPodPlaylistCount(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43cc40 NativeGetMonsterHUDHP(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original takes the local player's character (PlayerInfo +0x660, see
// NativeHUDSpell). With no character it stores 0. Otherwise, with no object of interest (Character::m_objectOfInterest,
// +0x14a4, declared) it returns without storing anything. With one, it stores 100 * obj[+0x1088] / obj[+0x1090]
// (integer division). GameObject does not declare +0x1088 or +0x1090. Waits on the same player lookup, on those two
// GameObject fields and on set_double.
void NativeGetMonsterHUDHP(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ccc8 NativeFindFreeSaveSlot(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original stores (double)(int)PlayerSavegame::SG_GetNextFreeSlot() with
// as_value::set_double on the fn_call's result. The assembly (0x43ccc8) calls SG_GetNextFreeSlot with no receiver
// (R0 is not set for the call), but dh2/PlayerSavegame.hpp declares it a non-static member (0x4660c8). That
// declaration needs checking against the asm. Missing: the as_value::set_double accessor.
void NativeFindFreeSaveSlot(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ccec NativeGetCreditMovement(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original stores the value of CreditMovementValue (below) with set_double. The value
// is 2.0 when the global HTC_DEVICES is set. Otherwise it is (double)(int)(Application::GetDt() / 25u + 1), with
// Application::GetDt on the Application singleton (ApplicationGetInstance(), dh2/global/ctrl_camera.hpp). The
// set_double store is missing.
void NativeGetCreditMovement(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43cd68 NativeSetCurrentDifficulty(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original converts argument 0 with to_number and stores the int at +0x0c of the object
// at dword_99F778 (unnamed global, see net-session-online/include/net-session-online/game_center.hpp). Missing: the
// to_number accessor, and the type of that object (layout request: dword_99F778 object, +0x0c int difficulty).
void NativeSetCurrentDifficulty(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43cdac NativeChangeRolloverInputBehavior(gameswf::fn_call const&)
// PARTIAL: stub, no body. Argument 0 (to_number) is an index and argument 1 (to_bool) a flag. The original takes
// MenuManager::GetInstance()->m_multiMenuManager (+0xf4, declared in dh2/MenuManager.hpp). For an unsigned index of
// 0..3 it selects the RenderFX* at that object's +0x134 + 4 * index (nullptr otherwise). It then calls
// RenderFX::SetInputBehavior(render, 132) when the flag is set, and (render, 4) when it is clear. Missing: the
// as_value accessors, the array at +0x134 (not declared), and RenderFX::SetInputBehavior (not declared in
// dh2/RenderFX.hpp).
void NativeChangeRolloverInputBehavior(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ce3c NativeOpenKeyboard(gameswf::fn_call const&)
// PARTIAL: stub, no body. Argument 0 (to_string) is the keyboard kind, argument 1 (to_number) the maximum length and
// argument 2 (to_string) the initial text. Unless the byte at 0x9a24d4 (unnamed) is set, the original calls
// GSKeyboard::QueryString(kind, (unsigned)maxLength, text, kind == "PlayerName"). The pseudocode passes one more
// argument (the native's own flag register, a5) than the declared QueryString in dh2/GSKeyboard.hpp, which takes four.
// Missing: the as_value accessors, the byte at 0x9a24d4, and the arity question.
void NativeOpenKeyboard(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43cf04 NativeIPodSetPlaylist(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original calls VoxSoundManager::SetIPodPlaylist(VoxSoundManager::s_instance,
// (int)to_number(argument 0)). Neither the instance nor SetIPodPlaylist is declared in vox-audio/vox/sound_manager.hpp.
void NativeIPodSetPlaylist(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43cf50 NativeAssignSaveSlotToPlayer(gameswf::fn_call const&)
// PARTIAL: stub, no body. Argument 0 (to_number) is the slot and argument 1 (to_number) the local player id. When
// both are not negative, the original calls AssignSaveSlotToPlayer(slot, localPlayerId) (0x43aac4, declared in
// dh2/global/native_app_hud_level.hpp), which writes the slot into dword_99F778 + 8. Otherwise the converted slot is
// returned. Missing: the as_value::to_number accessor.
void NativeAssignSaveSlotToPlayer(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43cfb4 NativeInvDropItem(gameswf::fn_call const&)
// PARTIAL: stub, no body. Argument 0 (to_number) is an inventory index. The original takes the local player's
// character (PlayerInfo +0x660, see NativeHUDSpell). Its ItemInventory base (Character +0x37c, declared) is the
// source. Online (GetOnline()+5 set), it reads the item id, power count and power ids (ItemInstance::GetItemId,
// GetNumPowers and GetPowerId, all declared in dh2/ItemInstance.hpp), builds a CMsgDropLoot with
// CMessage::CreateMessage, and sends it with CMessaging::SendMsg, using the network id from
// ObjectManager::GetNetworkIdByObjectBase (ObjectManager instance dword_99F764; ObjectManager has no header). Then it
// moves one item with ItemInventory::TransferItemTo(index, temp, 1, false, false), which is declared, into a
// temporary ItemInventory. It finishes with ItemObject::DropInventory(temp, character, character, nullptr, <stack
// argument>) on that temporary. ItemObject is not declared, and the last argument is not recovered.
void NativeInvDropItem(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43d178 NativeSelectClassSpec(gameswf::fn_call const&)
// PARTIAL: stub, no body. Argument 0 (to_number) is a class offset. The original takes the local player's character
// (PlayerInfo +0x660, see NativeHUDSpell). When it is set, it computes class = offset + 1 + Character::SG_GetPlayerClass(),
// calls CharProperties::LoadPropertiesForClassSelect(class) on the CharProperties base (Character +0x560), calls
// Character::SG_SetPlayerClass(class), calls CharAI::AI_ReloadSkills() on the CharAI base (Character +0x3c8), and
// calls Character::SG_Save(). Those methods are declared. Missing: the as_value accessor and the player lookup.
void NativeSelectClassSpec(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43d220 NativeSetSaveSlotIDToMainMenu(gameswf::fn_call const&)
// PARTIAL: stub, no body. Argument 0 (to_number) is a slot id. The flag is 0 unless the argument count is above 1,
// in which case it is argument 1 (to_bool). The original calls MenuMainMenu::GetInstance() and discards the result,
// then calls MenuMainMenu::ChangeCharacterToDisplay(int slot, bool flag). The assembly (0x43d220) sets R0 to the slot
// before that call, so the method is static: two arguments. The third argument in the pseudocode is register residue.
// MenuMainMenu has no header (layout request: a declaration of the static ChangeCharacterToDisplay(int, bool) and
// GetInstance()). Missing: the as_value accessors.
void NativeSetSaveSlotIDToMainMenu(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43d288 NativeEraseSaveSlot(gameswf::fn_call const&)
// PARTIAL: stub, no body. Argument 0 (to_number) is a save slot. When it is not negative, the original calls
// Savegame::FlushJobs(nullptr) and then Savegame::DeleteAllSlotFiles(slot). Both are declared in dh2/Savegame.hpp.
// Missing: the as_value::to_number accessor.
void NativeEraseSaveSlot(const gameswf::fn_call& call) {
    (void)call;
}

// Helper of NativeGetLoadingTipStrID: the page pick (inline arithmetic of 0x43cb24, not a separate function).
// The original keeps the table size as the byte offset when the table is empty, which gives row 0. Otherwise the seed
// advances (59051 * seed + 177149) % 0xDAF26B in 32-bit unsigned arithmetic, and the row is seed % pageCount. The
// negative-row test is kept as in the pseudocode; it never fires for a table of fewer than 2^31 rows.
std::uint32_t PickHintPageOffset(std::uint32_t& seed, std::uint32_t pageCount) {
    std::uint32_t offset = pageCount;
    if (pageCount != 0) {
        seed = (59051u * seed + 177149u) % 0xDAF26Bu;
        std::uint32_t row = seed % pageCount;
        if ((row & 0x80000000u) != 0) {
            row = 0u - row;
        }
        offset = 12u * row;
    }
    return offset;
}

// Helper of NativeGetCreditMovement: the value of 0x43ccec (the body is inline arithmetic, not a separate function).
// The division is unsigned, as the pseudocode divides an int by 0x19u.
double CreditMovementValue(bool htcDevice, int frameTimeMs) {
    if (htcDevice) {
        return 2.0;
    }
    const unsigned int steps = static_cast<unsigned int>(frameTimeMs) / 0x19u + 1u;
    return static_cast<double>(static_cast<int>(steps));
}
