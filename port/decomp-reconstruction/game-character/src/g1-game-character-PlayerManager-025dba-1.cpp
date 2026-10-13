// Batch g1-game-character-PlayerManager-025dba-1 (part 2 of 4): PlayerManager's readiness and loading queries, the
// per-class character counts, the local-player count, the player numbering, the initial setup, the controller attach
// and joining code, the inventory pack, the loading-info reset, the light refresh and the destructor
// (PlayerManager.cpp in the original).
//
// Tree conflicts that limit this file (see the ledger notes and the layout requests):
//   - dh2/Character.hpp cannot share a translation unit with dh2/PlayerManager.hpp. Character pulls in the pydata
//     IStreamBase (pydata-accessors/deps/stream_reader.hpp), PlayerManager.hpp pulls in the net-transport one
//     (net-transport/deps/i_stream_base.hpp) through StreamBuffer. Likewise pydata-accessors/w7_pydata12.hpp
//     (PyDataConstants) and Level.hpp (StreamBuffer) cannot be included with Character.hpp.
//   - dh2/Gamepad.hpp (dh2/Point3D.hpp) and dh2/LightBase.hpp / dh2/Level.hpp (core-platform/deps/game_types.hpp)
//     define Point3D twice. This file keeps Gamepad.hpp.
//   The bodies that need those types are partial stubs: they keep the reachable part of the binary's logic and say
//   what is missing at the stub.
//
// Online gates. The online bodies test, in order: COnline +0x05 (the online-game flag), the OnlineGameState byte at
// +0x24 and the state word at +0x34, CMatching::UnknownGate() (vtable +0x64) and CNetPlayerManager::IsInitialized().
// The COnline flag and the two OnlineGameState fields are not reachable from the declared headers (layout requests in
// the ledger notes), so the gates below return the offline value. Each gate still makes the singleton call the binary
// makes first.
//
// Layout facts used here (dh2/PlayerInfo.hpp, dh2/Gamepad.hpp):
//   PlayerInfo +0x660 (unk_660) is the Character the player controls. It is declared void* in PlayerInfo.hpp; the
//   binary uses it as Character* (PostInitCharacters passes it to Character::VerifySpecialization).
//   PlayerInfo +0x66c (unk_66c) is the local-player byte. +0x668 (unk_668) is the gamepad index of the player.
//   +0x670 (unk_670) is -1 for a player without a number. _UpdatePlayerNumbers writes unk_678 (player number) and
//   unk_67c (local or remote number).
//   Gamepad channel k sits at 12 + 32k. The joining code reads the previous_* fields of channels 0, 1, 10, 11, 12, 13,
//   37, 38, 39, 40 (the channel's base is value - 12, so value is previous, the minimum and maximum are
//   previous_minimum and previous_maximum, and the flag is previous_above_threshold).

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "core-platform/memory.hpp"  // gAssertLevel
#include "dh2/Gamepad.hpp"           // Gamepad::Channel, and dh2/Point3D.hpp (the Point3D used here)
#include "dh2/InputManager.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"  // dword_99F76C, the PlayerManager instance
#include "dh2/global/net_singletons.hpp"     // GetOnline, GetNetPlayerMgr, GetConnectionMgr
#include "net-session-online/c_matching_base.hpp"
#include "net-session-online/c_matching_gl_live.hpp"
#include "net-session-online/online_game_state.hpp"
#include "net-session-online/tb/online_singleton.hpp"
#include "net-transport/connection_manager.hpp"
#include "net-transport/tb/c_net_player_manager.hpp"

namespace {

const char* const kPlayerManagerCpp =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\PlayerManager\\PlayerManager.cpp";

// Class ids (Arrays::CharacterTable rows) that the three per-class counters pass.
constexpr int kWarriorCharPropsId = 263;
constexpr int kRogueCharPropsId = 325;
constexpr int kMageCharPropsId = 290;

// Gamepad channels whose threshold crossings raise EvGamepad, in the order the binary tests them. The last entry is
// channel 1, after which the binary calls CNetPlayerInfo::SetState(player, 0) when it raised an event.
constexpr int kJoiningButtons[10] = {37, 38, 39, 40, 10, 11, 12, 13, 0, 1};

// The original ASSERT(expr) macro. Level 2 faults (std::abort() here), level 1 prints to stderr and execution continues.
void AssertFailed(const char* expr, const char* file, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr, file, line);
    }
}

// Online gates (see the file comment). Each returns the offline value until COnline and OnlineGameState expose
// their fields; the singleton calls stay in place.
bool OnlineSessionFlag() {  // COnline +0x05 (isOnlineGame_)
    return GetOnlineGameFlag();
}

bool OnlineMatchingFlag() {  // OnlineGameState +0x24 (flag24_)
    return OnlineSingleton<OnlineGameState>::GetInstance()->flag24_;
}

int OnlineGameStateWord() {  // OnlineGameState +0x34 (mode_)
    return OnlineSingleton<OnlineGameState>::GetInstance()->mode_;
}

// The four-test online check the bodies share: online session, matching room flag, CMatching::UnknownGate (vtable
// +0x64) and CNetPlayerManager::IsInitialized. The binary tests them in this order.
bool OnlineMatchingReady() {
    if (!OnlineSessionFlag()) {
        return false;
    }
    if (!OnlineMatchingFlag()) {
        return false;
    }
    if (!CMatching::Get()->UnknownGate()) {
        return false;
    }
    (void)GetNetPlayerMgr();
    return CNetPlayerManager::IsInitialized();
}

// The Character a player record controls (PlayerInfo +0x660). Only the pointer is needed, so the Character type can
// stay incomplete here. See the file comment.
const Character* CharacterOf(const PlayerInfo& player) {
    return static_cast<const Character*>(player.unk_660);
}

Character* CharacterOf(PlayerInfo& player) {
    return static_cast<Character*>(player.unk_660);
}

// EvGamepad (vtable _ZTV9EvGamepad, event id 7) carries the button id, the state (1 pressed, 0 released), the
// controller index and 1.0f. dh2/EvGamepad.hpp is not in the tree yet, so the event is not raised.
void RaiseGamepadButtonEvent(int buttonId, int state, int controllerId) {
    (void)buttonId;
    (void)state;
    (void)controllerId;
}

}  // namespace

// 0x36e8bc PlayerManager::AllClientsReadyToRoll(void)
// Partial: the online gates (file comment). Offline the function returns 1.
int PlayerManager::AllClientsReadyToRoll() {
    if (!OnlineMatchingReady()) {
        return 1;
    }
    const int numPlayers = GetNumPlayers();
    if (numPlayers <= 0) {
        return 1;
    }
    for (int i = 0; i < numPlayers; ++i) {
        PlayerInfo* player = GetPlayer(i, false);
        if (player->IsActive()) {
            // +0x525 is m_doneLoading, +0x545 is m_readyToRoll (both NetStructBool values).
            if (!player->m_doneLoading.GetValue() || (!player->IsHost() && !player->m_readyToRoll.GetValue())) {
                return 0;
            }
        }
    }
    return 1;
}

// 0x36e994 PlayerManager::AllLoadingDone(void)
int PlayerManager::AllLoadingDone() {
    const int numPlayers = GetNumPlayers();
    if (numPlayers <= 0) {
        return 1;
    }
    for (int i = 0; i < numPlayers; ++i) {
        PlayerInfo* player = GetPlayer(i, false);
        if (player->IsActive()) {
            // +0x525 m_doneLoading, +0x4e5 m_ingame, +0x505 m_inCutscene, and the PlayerManager flag at +0x71a.
            if (!player->m_doneLoading.GetValue() ||
                (unk_71a && player->IsRemote() && player->IsHost() && player->m_doneLoading.GetValue() &&
                 player->m_ingame.GetValue() && player->m_inCutscene.GetValue())) {
                return 0;
            }
        }
    }
    return 1;
}

// 0x36ea50 PlayerManager::GetNumPlayerCharactersOfClass(int)
// Partial (stub): the count reads Character::m_charPropsId, which needs Character.hpp (file comment). Returns 0
// once the count is past the empty check.
int PlayerManager::GetNumPlayerCharactersOfClass(int classId) {
    (void)classId;
    if (m_numCharacters <= 0) {
        return 0;
    }
    return 0;
}

// 0x36eab8 PlayerManager::GetNumPlayerCharacterWarrior(void)
// Calls GetNumPlayerCharactersOfClass, so its value waits on the stub above.
int PlayerManager::GetNumPlayerCharacterWarrior() {
    return GetNumPlayerCharactersOfClass(kWarriorCharPropsId);
}

// 0x36eac0 PlayerManager::GetNumPlayerCharacterRogue(void)
int PlayerManager::GetNumPlayerCharacterRogue() {
    return GetNumPlayerCharactersOfClass(kRogueCharPropsId);
}

// 0x36eac8 PlayerManager::GetNumPlayerCharacterMage(void)
int PlayerManager::GetNumPlayerCharacterMage() {
    return GetNumPlayerCharactersOfClass(kMageCharPropsId);
}

// 0x36ead0 PlayerManager::GetNumLocalPlayers(bool)
// Partial: the online branch needs the gates (file comment). Offline it counts the local records of m_players in order:
// with flag set, only those whose Character is set.
int PlayerManager::GetNumLocalPlayers(bool flag) {
    if (!OnlineMatchingReady()) {
        int count = 0;
        for (const auto& entry : m_players) {
            const PlayerInfo& player = entry.second;
            if (!player.unk_66c) {
                continue;
            }
            if (flag && !CharacterOf(player)) {
                continue;
            }
            ++count;
        }
        return count;
    }
    if (!flag) {
        return static_cast<int>(m_localPlayerIdList.size());
    }
    int count = 0;
    for (std::size_t i = 0; i < m_localPlayerIdList.size(); ++i) {
        PlayerInfo* player = GetPlayer(m_localPlayerIdList[i], false);
        if (CharacterOf(*player)) {
            ++count;
        }
    }
    return count;
}

// 0x36ec54 PlayerManager::InitialSetup(Point3D<float>)
// Partial: the binary moves each local player's character to the position (GameObject::SetPosition on
// Character +0x0, with setDestination set). That call needs Character.hpp (file comment), so the loop is kept without it.
// The position store and the global PlayerManager (dword_99F76C, not this object) are as in the binary.
void PlayerManager::InitialSetup(Point3D<float> position) {
    if (m_initialSetupDone && !unk_71a) {
        return;
    }
    m_initialSetupDone = true;
    unk_6d4 = position.X;
    unk_6d8 = position.Y;
    unk_6dc = position.Z;
    int count = dword_99F76C->GetNumLocalPlayers(true);
    for (int i = 0; i < count; ++i) {
        PlayerInfo* local = dword_99F76C->GetLocalPlayer(i, true);
        (void)local;
        count = dword_99F76C->GetNumLocalPlayers(true);
    }
}

// 0x36ed0c PlayerManager::_UpdatePlayerNumbers(void)
// Partial: the online branch needs the gates (file comment) and its return value is a leftover register in the binary
// (the id-list end pointer, or 1 for an empty list), which the port does not reproduce, so it returns 0 there. The
// offline branch numbers the players of m_players in order and returns the count of the non-local players.
int PlayerManager::_UpdatePlayerNumbers() {
    if (OnlineMatchingReady()) {
        int playerIndex = 0;  // the numbered players so far
        int localIndex = 0;   // the local players so far
        int remoteIndex = 0;  // the remote players so far
        for (std::size_t idx = 0; idx < m_playerIdList.size(); ++idx) {
            PlayerInfo* player = GetPlayerByInternalID(m_playerIdList[idx], false);
            if (player->unk_670 == -1) {
                player->unk_678 = -1;
                continue;
            }
            player->unk_678 = playerIndex;
            if (player->unk_66c) {
                player->unk_67c = localIndex++;
            } else {
                player->unk_67c = remoteIndex++;
            }
            ++playerIndex;
        }
        return 0;
    }
    int index = 0;
    int localCount = 0;
    int remoteCount = 0;
    for (auto& entry : m_players) {
        PlayerInfo& player = entry.second;
        player.unk_678 = index++;
        if (player.unk_66c) {
            player.unk_67c = localCount++;
        } else {
            player.unk_67c = remoteCount++;
        }
    }
    return remoteCount;
}

// 0x36eea8 PlayerManager::GetPlayerByCharacter(Character const*,bool)
// Partial: the online branch needs the gates (file comment). Offline it returns the first record of m_players whose
// Character is the argument, or the null record (m_nullPlayer) when none matches.
PlayerInfo* PlayerManager::GetPlayerByCharacter(const Character* character, bool flag) {
    if (OnlineMatchingReady()) {
        for (std::size_t idx = 0; idx < m_playerIdList.size(); ++idx) {
            const int internalId = m_playerIdList[idx];
            PlayerInfo* player = GetPlayerByInternalID(internalId, flag);
            if (character == CharacterOf(*player)) {
                return _GetNetPlayerInfo(internalId, flag);
            }
        }
        return &m_nullPlayer;
    }
    for (auto& entry : m_players) {
        if (character == CharacterOf(entry.second)) {
            return &entry.second;
        }
    }
    return &m_nullPlayer;
}

// 0x36effc PlayerManager::IsLocalPlayer(Character const*)
int PlayerManager::IsLocalPlayer(const Character* character) {
    if (!character) {
        return 0;
    }
    PlayerInfo* player = GetPlayerByCharacter(character, false);
    return player->IsLocal() ? 1 : 0;
}

// 0x36f074 PlayerManager::IsLocalPlayerHosting(void)
// Partial: the two online tests read COnline +0x05 and OnlineGameState +0x34 (file comment). Offline the function
// returns 1.
int PlayerManager::IsLocalPlayerHosting() {
    if (OnlineSessionFlag() && static_cast<unsigned>(OnlineGameStateWord() - 3) <= 1u) {
        CMatchingGLLive* matching = static_cast<CMatchingGLLive*>(CMatching::Get());
        return matching->IsHost() ? 1 : 0;
    }
    if (OnlineSessionFlag()) {
        PlayerInfo* local = GetLocalPlayer(0, false);
        return local->IsHost() ? 1 : 0;
    }
    return 1;
}

// 0x36f0dc PlayerManager::_AttachControllerToPlayerCharacter(int)
// Partial (stub): the binary attaches the controllers to the character's v2Controllable base (Character +0x374, the
// controller slot +0x378) and sets the controller's character. Those need Character.hpp (file comment), so only the
// player record is looked up here.
void PlayerManager::_AttachControllerToPlayerCharacter(int internalId) {
    (void)GetPlayerByInternalID(internalId, false);
}

// 0x36f2bc PlayerManager::PostInitCharacters(void)
// Partial: (1) the binary calls Character::VerifySpecialization on each local active player's character. That needs
// Character.hpp (file comment), so the call is left out. (2) The offline branch with no character reports the assert
// and the binary then calls VerifySpecialization on the null pointer; the port reports and skips it. (3) The online
// branch with no character calls the matching room's vtable +0x3c, which is LeaveRoom in CMatchingGLLive; the dynamic
// type is not proven here, so the port casts to CMatchingGLLive. (4) The online gate (file comment).
void PlayerManager::PostInitCharacters() {
    const int numPlayers = GetNumPlayers();
    for (int i = 0; i < numPlayers; ++i) {
        PlayerInfo* player = GetPlayer(i, false);
        if (!player->IsLocal() || !player->IsActive()) {
            continue;
        }
        const Character* character = CharacterOf(*player);
        if (!OnlineSessionFlag()) {
            if (!character) {
                AssertFailed("c", kPlayerManagerCpp, 1091);
            }
            continue;
        }
        if (character) {
            continue;
        }
        static_cast<CMatchingGLLive*>(CMatching::Get())->LeaveRoom();
        (void)GetConnectionMgr()->DisconnectAll();
    }
}

// 0x36f4b4 PlayerManager::_UpdateJoiningController(int,int)
// Partial: EvGamepad (dh2/EvGamepad.hpp) is not in the tree, so the button events are not raised (see
// RaiseGamepadButtonEvent). The threshold tests and the SetState call after the last button are reconstructed.
void PlayerManager::_UpdateJoiningController(int controllerId, int internalId) {
    Gamepad* pad = InputManager::GetInstance()->GetGamepad(controllerId);
    PlayerInfo* player = GetPlayerByInternalID(internalId, false);
    bool raisedLast = false;
    constexpr int kCount = static_cast<int>(sizeof(kJoiningButtons) / sizeof(kJoiningButtons[0]));
    for (int k = 0; k < kCount; ++k) {
        const int button = kJoiningButtons[k];
        const Gamepad::Channel& channel = pad->m_buttons[button];
        // The binary's threshold: (previous_maximum + previous_minimum + 1.0) * 0.5.
        const float threshold = ((channel.previous_maximum + channel.previous_minimum) + 1.0f) * 0.5f;
        const bool above = channel.previous >= threshold;
        bool raised = false;
        if (above != channel.previous_above_threshold) {
            RaiseGamepadButtonEvent(button, above ? 1 : 0, controllerId);
            raised = true;
        }
        if (k == kCount - 1) {
            raisedLast = raised;
        }
    }
    if (raisedLast) {
        player->SetState(0);
    }
}

// 0x370020 PlayerManager::_PackInventory(PlayerInfo &)
// Partial (stub): the binary packs the equipment of the player's character (an XOR hash of the equipped item ids into
// unk_4a0, one id per slot into unk_3b0). That needs the Character's ItemInventory base and Level::m_isLoaded, and
// Level.hpp cannot share a translation unit with Gamepad.hpp (file comment). Only the local-player gate is kept.
void PlayerManager::_PackInventory(PlayerInfo& info) {
    if (!info.unk_66c) {
        return;
    }
}

// 0x370c9c PlayerManager::ResetCharacterDeathTimer(void)
// Partial (stub): the binary sets the local player's death timer to PyDataConstants "CharacterDesign" /
// "MultiplayerDeathTimer". The pydata header cannot share a translation unit with PlayerManager.hpp (file comment),
// so the value is not read and the timer is not set.
void PlayerManager::ResetCharacterDeathTimer() {
    (void)GetLocalPlayer(0, false);
}

// 0x370e00 PlayerManager::ClearLoadingInfo(void)
void PlayerManager::ClearLoadingInfo() {
    (void)GetOnline();
    m_readyToRollTimer = -1;
    m_initialSetupDone = false;
    m_readyToRollWaiting = false;
    m_questSyncBuffered = false;
    unk_71a = false;
    PlayerInfo* local = GetLocalPlayer(0, false);
    local->ClearLoadingInfo();
    m_globalDeathState = 0;
    m_localDeathPending = false;
}

// 0x371050 PlayerManager::_AttachLightToPlayerCharacter(int)
// Partial (stub): the binary refreshes the attachment of every light base of the object manager (dword_99F764,
// LightBase +0x58). LightBase.hpp defines Point3D differently from Gamepad.hpp, which this file keeps (file comment).
// The argument is not read by the binary.
void PlayerManager::_AttachLightToPlayerCharacter(int /*internalId*/) {}

// 0x3714ac PlayerManager::~PlayerManager() (D1, complete object)
// Not defined here. A class has one destructor body, and g1-game-character-PlayerManager-025dba-2.cpp already defines
// PlayerManager::~PlayerManager() {} for 0x37159c (D2) and 0x371580 (D0). PlayerManager has no virtual base, so D1 and
// D2 are the same body (the binary frees the members in declaration order: StreamBuffer, id lists, player map, null
// record). A second definition here would be a duplicate symbol at link time.
