// Batch g1-game-character-PlayerManager-025dba-2 (part 3 of 4 of PlayerManager): the destructors and constructor, the
// character and player removal, _AddCharacter, the online-transition check, _ManageCharacters, the death handling,
// the host-change notification, the remote and local controller checks.
// Declarations and layout come from dh2/PlayerManager.hpp. Offsets in comments are the original 32-bit offsets.
//
// Online gates: the first test is GetOnlineGameFlag() (*(GetOnline() + 5)); the full gate of the && chain is NetGateOpen()
// (game-app/src/net_gate.cpp: online flag, OnlineGameState +0x24, CMatching IsInRoom, CNetPlayerManager::IsInitialized).

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "core-platform/memory.hpp"  // gAssertLevel
#include "dh2/Application.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/CameraTarget.hpp"
#include "dh2/CharAI.hpp"
#include "dh2/CharStateMachine.hpp"
#include "dh2/Character.hpp"
#include "dh2/GameEventManager.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/InfoHUDManager.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/Level.hpp"
#include "dh2/MultiMenuManager.hpp"
#include "dh2/OnlineStatusMsg.hpp"
#include "dh2/MenuMessageManager.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/CRoomAttributes.hpp"
#include "dh2/RoomZone.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/global/Vec3f_Origin.hpp"
#include "net-session-online/c_message.hpp"
#include "net-session-online/c_messaging.hpp"
#include "net-session-online/online_game_state.hpp"
#include "net-session-online/tb/online_singleton.hpp"
#include "vox-audio/vox/sound_manager.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/ObjectBase.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/CharProperties.hpp"
#include "dh2/InputManager.hpp"
#include "dh2/Gamepad.hpp"
#include "dh2/SavegameManager.hpp"
#include "vendor-gameswf/as_value.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"
#include "pydata-accessors/w7_pydata12.hpp"  // PyDataConstants::getConstant
#include "dh2/global/IsMultiplayerGame.hpp"  // dword_99F76C, the PlayerManager instance
#include "dh2/global/ObjectManagerInstance.hpp"  // dword_99F764, the ObjectManager instance
#include "dh2/global/ctrl_camera.hpp"  // ApplicationGetInstance()
#include "dh2/global/net_singletons.hpp"  // GetOnline, GetNetPlayerMgr, GetConnectionMgr, GetMessaging
#include "net-session-online/c_matching_base.hpp"  // CMatching::Get(), UnknownGate(), LocalMemberId()
#include "net-transport/connection_manager.hpp"    // CConnectionManager::GetConnectionCount
#include "net-transport/tb/c_net_player_manager.hpp"  // CNetPlayerManager

namespace {

constexpr const char kPlayerManagerFile[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\PlayerManager\\PlayerManager.cpp";

// The original ASSERT(expr) on its failing branch. gAssertLevel 2 stops the process (the original stores 0 to address 0),
// 1 prints the message to stderr and execution continues, 0 is silent.
void ReportAssertFailure(const char* expr, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr, kPlayerManagerFile, line);
    }
}

}  // namespace

// 0x371580 _ZN13PlayerManagerD0Ev PlayerManager::~PlayerManager() (deleting destructor)
// SKIPPED: compiler-generated. It calls the complete-object destructor, then CustomFree(this); the C++ delete
// expression produces it.

// PlayerManager::s_deletedPlayerCharacterCounter (ELF .bss, 4 bytes, zero at start). Declared in dh2/PlayerManager.hpp.
int PlayerManager::s_deletedPlayerCharacterCounter = 0;

// 0x37159c _ZN13PlayerManagerD2Ev PlayerManager::~PlayerManager() (base-object destructor body)
// The original stores the vtable, then releases in this order: the quest-sync StreamBuffer (+0x6e0), the local
// player-id vector (+0x6b4), the player-id vector (+0x6a8), the player map (erased node by node, then the header
// reset), and the null record at +0x8. C++ destroys members in reverse declaration order, which runs the same
// releases in the same sequence, so the body is empty. The D1 entry at 0x3714ac is in another batch.
PlayerManager::~PlayerManager() {}

// 0x374b28 _ZN13PlayerManagerC1Ev PlayerManager::PlayerManager()
// Constructs the null record (+0x8) and the empty containers by their own constructors. The original also zeroes
// the state fields that the header does not default-initialise: the loading and death flags, the transition flags,
// the timers and the unnamed words. The C2 entry at 0x374be4 is the same body (see the ledger).
PlayerManager::PlayerManager() {
    unk_6c0 = 0;                    // +0x6c0
    m_numCharacters = 0;            // +0x6c4
    unk_6c9 = 0;                    // +0x6c9
    m_inOnlineTransition = false;   // +0x6ca
    m_readyToRollWaiting = false;   // +0x6cb
    m_readyToRollTimer = -1;        // +0x6cc, -1 when idle
    m_initialSetupDone = false;     // +0x6d0
    unk_6d4 = 0;                    // +0x6d4
    unk_6d8 = 0;                    // +0x6d8
    unk_6dc = 0;                    // +0x6dc
    m_questSyncBuffered = false;    // +0x710
    m_localDeathPending = false;    // +0x711
    m_globalDeathState = 0;         // +0x714
    unk_718 = false;                // +0x718
    m_hostChanged = false;          // +0x719
    unk_71a = false;                // +0x71a
    unk_71b = false;                // +0x71b
}

// 0x374be4 _ZN13PlayerManagerC2Ev PlayerManager::PlayerManager() (base-object constructor)
// SKIPPED: the pseudocode is identical to the complete-object constructor at 0x374b28, so the C++ constructor above serves both.

// 0x3721b8 _ZN13PlayerManager19RemoveAllCharactersEv PlayerManager::RemoveAllCharacters()
// Walks the player records by friendly index. The original's nested loop reads the record at the current index, advances
// the index, and breaks when the record has no character; the outer test then re-checks the count. That is the same as
// visiting each index once and skipping empty records. Each owned character is removed, then the count is cleared.
void PlayerManager::RemoveAllCharacters() {
    for (int index = 0; index < GetNumPlayers();) {
        Character* character = static_cast<Character*>(GetPlayer(index, false)->unk_660);
        ++index;
        if (!character) {
            continue;
        }
        RemoveCharacter(character);
    }
    m_numCharacters = 0;
}

// 0x3727c0 _ZN13PlayerManager16RemoveAllPlayersEv PlayerManager::RemoveAllPlayers()
// Removes the player at the first friendly index as many times as there are players, then clears the character count.
// The return value is the result of the last RemovePlayer call (the original leaves it in r0).
int PlayerManager::RemoveAllPlayers() {
    const int numPlayers = GetNumPlayers();
    int result = numPlayers;
    if (numPlayers > 0) {
        int removed = 0;
        do {
            const int internalId = _GetInternalIDByFriendlyID(0, false);
            ++removed;
            result = RemovePlayer(internalId);
        } while (removed != numPlayers);
    }
    m_numCharacters = 0;
    return result;
}

// 0x3726bc _ZN13PlayerManager12RemovePlayerEi PlayerManager::RemovePlayer(int)
// The removal runs when the network gate (NetGateOpen) is closed: IsPlayerInLocalMap (0 returns at once), the character of the record with
// that key is removed, the record is erased. The player numbers are then updated (also when the gate is open).
int PlayerManager::RemovePlayer(int internalId) {
    if (!NetGateOpen()) {
        const bool present = IsPlayerInLocalMap(internalId);
        if (!present) {
            return present;
        }
        const auto found = m_players.find(internalId);
        if (found != m_players.end()) {
            RemoveCharacter(static_cast<Character*>(found->second.unk_660));
            m_players.erase(found);
        }
    }
    return _UpdatePlayerNumbers();
}

// 0x371d80 _ZN13PlayerManager15RemoveCharacterEP9Character PlayerManager::RemoveCharacter(Character*)
// The body runs only when the character is not already marked for deletion (ObjectBase byte +0x81). The owned-character count (asserted above
// zero) drops; the character moves to the origin, is disabled and hidden (the virtuals +0x48 Disabled and +0x40 SetVisible(false)), loses its
// camera and multiplayer highlight and its object of interest (+0x14a4), clears its AI target and aggro, and is deleted and renamed
// "<name>_Deleted_<n>" (the static counter is incremented). With a level and a record that has an internal id the local camera retargets to the
// first local (else first) character and the light set refreshes. The record is then reset (+0x660 cleared when unk_6c9 is clear, unk_684
// zeroed, the packed-inventory counter unk_4a0 set to 0; a local record also loses class, name, level, equipment bytes, XP, gold and the in-game
// flag) and the HUD refreshes (menu_HUD_0 "completeRefresh").
void PlayerManager::RemoveCharacter(Character* a2) {
    if (!a2 || a2->m_markedForDeletion) {
        return;
    }
    PlayerInfo* info = GetPlayerByCharacter(a2, true);
    const bool noInternalId = info->unk_670 == -1;  // v8: the record has no internal id (+0x670)

    if (m_numCharacters <= 0) {
        ReportAssertFailure("m_numCreatedCharacters > 0", 1247);
    }
    m_numCharacters = m_numCharacters - 1;

    const Point3D<float> origin{0.0f, 0.0f, 0.0f};
    a2->SetPosition(origin, true);
    a2->Disabled();
    a2->SetVisible(false);
    a2->InitCam();
    a2->RemoveMultiplayerHighlight();
    a2->m_objectOfInterest = nullptr;
    a2->CharAI::AI_SetTarget(nullptr, false);
    a2->CharAI::AI_SyncLastTarget();
    a2->CharAI::AI_ClearAllAggro();
    a2->CharAI::AI_ClearAllAggroTowardMe(false);
    a2->ObjectBase::Delete();
    char deletedName[256];
    std::snprintf(deletedName, sizeof deletedName, "%s_Deleted_%d", a2->m_name.c_str(), s_deletedPlayerCharacterCounter);
    ++s_deletedPlayerCharacterCounter;
    a2->SetName(deletedName);

    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (!noInternalId) {
        if (level && info->unk_66c) {
            if (m_numCharacters > 0) {
                GameObject* target = static_cast<GameObject*>(GetLocalPlayer(0, true)->unk_660);
                if (!target) {
                    target = static_cast<GameObject*>(GetPlayer(0, false)->unk_660);
                }
                if (target) {
                    if (level->m_camera) {
                        level->m_camera->SetTarget(target, 0);
                        level->m_camera->Update();  // CameraLevel virtual +0x10
                    }
                    if (m_numCharacters == 1) {
                        static_cast<Character*>(target)->InitCam();
                    }
                }
            }
            level->UpdateLightSet(true, 4);
        }
    }

    if (!noInternalId) {
        if (!unk_6c9) {
            info->unk_660 = nullptr;           // the original stores the byte value (0) into the character pointer
        }
        info->unk_684 = 0;
        info->unk_4a0.SetValue(0);
        if (info->IsLocal()) {
            info->SetCharacterClass(-1);
            info->SetCharacterName(std::string());
            info->SetCharacterLevel(-1);
            info->unk_3b0.SetBuffer(nullptr, 0);  // +0x3b0, 36-byte array
            info->SetCharacterXP(0);
            info->SetCharacterGold(0);
            info->SetIngame(false);
        }
    }
    MenuFX* hud = MenuManager::GetInstance()->GetHUDRoot();
    if (hud != nullptr) {
        MenuManager::GetInstance()->GetHUDRoot()->InvokeASCallback("menu_HUD_0", "completeRefresh", nullptr, 0);
    }
}

// 0x372220 _ZN13PlayerManager13_AddCharacterEi PlayerManager::_AddCharacter(int)
// Spawns the player's character when the record has a class and no character yet. The return value of the original is
// the record pointer's bits on the no-spawn path and the r0 residue of the last callee on the other paths; callers
// (_ManageCharacters) ignore it, so this port returns 0.
// The new character takes the record's two numbers (+0x1f88 the record's +0x674, +0x1f8c the record's +0x670). Online, a client character is
// placed at the host character's position and rotation, takes its initial position and rotation, joins the host's room zone (else the object
// manager's no-room list, with the zone-entered notification). Then InitAll, InitCam (local and in room), the idle state, the three replicated
// skill slots and the replicated skill levels (30 bytes, each applied when not negative and within the savegame's skill count), visibility
// (shown for a local record, else the record's in-game flag), QuickSave of the level, the owned count + 1 and the controllers: offline or
// for the record's own character the controller (and, for a local record, the light) is attached, otherwise the character is removed again.
int PlayerManager::_AddCharacter(int internalId) {
    PlayerInfo* info = GetPlayerByInternalID(internalId, false);
    if (info->m_characterClass.GetValue() != -1 && info->unk_660 == nullptr) {
        char name[512];
        std::snprintf(name, sizeof name, "PlayerCharacter_%d", internalId);
        ObjectHandle handle = dword_99F764->Spawn("Character", name, true, true);
        Character* character = handle;
        if (!character) {
            ReportAssertFailure("c", 1124);
        }
        info->unk_660 = character;
        character->InitializePlayerSavegame();
        if (info->IsLocal()) {
            character->SG_SetSlot(static_cast<unsigned int>(info->unk_664));
        } else {
            character->SG_SetPlayerClass(info->m_characterClass.GetValue());
        }
        character->m_netLocalNumber = info->unk_674;  // +0x1f88
        character->m_netInternalId = info->unk_670;   // +0x1f8c

        if (GetOnlineGameFlag()) {
            if (!info->IsHost()) {
                Character* host = GetHostingPlayer()->unk_660;
                if (host) {
                    character->SetPosition(host->m_position, true);
                    character->SetRotation(host->m_rotation);
                    character->SetInitialPosition(host->m_initialPosition);
                    character->m_initialRotation = host->m_initialRotation;
                    if (!host->m_roomZone || !host->m_roomZone->AddInitialObject(character)) {
                        dword_99F764->AddNoRoomObject(character);
                        character->unk_2ef = 1;
                        character->ZoneEntered();
                    }
                }
            }
        }

        character->InitAll();
        if (info->IsLocal() && info->IsInRoom()) {
            character->InitCam();
        }
        character->CharStateMachine::SM_SetIdleState(false);

        // Replicated skill bytes: three slots, each clamped to -1 when below -1.
        std::int8_t slots[3] = {0, 0, 0};
        info->unk_3d8.GetBuffer(slots, 0);  // the second argument is not read by the original (see the header)
        for (int i = 0; i != 3; ++i) {
            int value = slots[i];
            if (value < -1) {
                slots[i] = -1;
                value = -1;
            }
            character->SG_SetSkillInSlot(i, static_cast<unsigned int>(value));
        }
        if (character->m_savegame && character->m_savegame->m_skillsSize > 30) {
            ReportAssertFailure("PlayerInfo::NETWORK_SKILL_LIST_SIZE >= c->SG_GetSkillSize()", 1188);
        }
        std::int8_t levels[30];
        std::memset(levels, 0, sizeof levels);
        info->unk_400.GetBuffer(levels, 0);
        for (int j = 0; j != 30; ++j) {
            PlayerSavegame* savegame = character->m_savegame;
            if (savegame && savegame->m_skillsSize > j) {
                const int level = levels[j];
                if (level >= 0) {
                    character->SG_SetSkillLevel(static_cast<unsigned int>(j), level);
                }
            }
        }
        character->SetVisible(info->IsLocal() ? true : info->m_ingame.GetValue());

        if (Level* level = ApplicationGetInstance().GetCurrentLevel()) {
            level->QuickSave(false);
        }
        ++m_numCharacters;

        if (!GetOnlineGameFlag() || character == GetPlayerByInternalID(internalId, true)->unk_660) {
            _AttachControllerToPlayerCharacter(internalId);
            if (info->IsLocal()) {
                _AttachLightToPlayerCharacter(internalId);
            }
        } else {
            RemoveCharacter(character);
        }
        return 0;
    }
    return 0;
}

// 0x37193c _ZN13PlayerManager22_CheckOnlineTransitionEv PlayerManager::_CheckOnlineTransition()
// Outside a transition with the network gate open and records in the map, the local map is cleared. Online in OnlineGameState mode 3 or 4 with a
// level and no live connection (CConnectionManager::GetConnectionCount(1) == 0) the application goes to the main menu (event 3). With the
// gate open the transition flag is set and the net player manager's player-id and local-player-id lists are copied into
// m_playerIdList and m_localPlayerIdList; with it closed the host-change flag and the transition flag are cleared, the loading info is
// cleared and the quest-sync buffer is cleared (its result is the return value; the open path returns a stale register).
int PlayerManager::_CheckOnlineTransition() {
    if (!m_inOnlineTransition && NetGateOpen() && !m_players.empty()) {
        m_players.clear();
    }
    if (GetOnlineGameFlag()) {
        if (static_cast<unsigned>(OnlineSingleton<OnlineGameState>::GetInstance()->mode_ - 3) <= 1u) {
            if (ApplicationGetInstance().GetCurrentLevel()) {
                if (!GetConnectionMgr()->GetConnectionCount(1)) {
                    ApplicationGetInstance().GoToMainMenu(static_cast<GoToMainMenuEvent>(3));
                }
            }
        }
    }
    if (NetGateOpen()) {
        m_inOnlineTransition = true;
        m_playerIdList = GetNetPlayerMgr()->GetPlayerIdList();
        m_localPlayerIdList = GetNetPlayerMgr()->GetLocalPlayerIdList();
        return 0;
    }
    m_hostChanged = false;
    m_inOnlineTransition = false;
    ClearLoadingInfo();
    return ClearQuestSyncBuffer();
}

// 0x37565c _ZN13PlayerManager18ReviveLocalPlayersEb PlayerManager::ReviveLocalPlayers(bool)
// The death state is reset (m_localDeathPending, m_globalDeathState), the checkpoint reloads when offline and the effects volume returns to the
// saved "VolumeFX". Each local player's character loses its buffs, collects the characters that follow it (faeries and characters whose master
// it is), and is placed: offline (or hosting with the flag) at its saved revive position (when not the origin, with the followers placed at
// the position it looks at), as a client at the hosting player's character position. Its object of interest is cleared, it revives (no reviver,
// true), enters the idle state and gets at least the minimum number of potions (CharacterDesign MinimumPotionsOnDeath). The music restarts
// (StopAllMusic(2), then the level's music, loop, 1000 ms fade).
void PlayerManager::ReviveLocalPlayers(bool flag) {
    m_localDeathPending = false;      // +0x711
    m_globalDeathState = 0;           // +0x714
    if (!GetOnlineGameFlag()) {
        Level* level = ApplicationGetInstance().GetCurrentLevel();
        level->LoadCheckpoint();
    }
    VoxSoundManager* sound = VoxSoundManager::s_instance;
    sound->SetSoundVolume(1, static_cast<float>(ApplicationGetInstance().GetSavedOption("VolumeFX")));

    for (int index = 0; index < dword_99F76C->GetNumLocalPlayers(false); ++index) {
        Character* character = dword_99F76C->GetLocalPlayer(index, false)->unk_660;
        if (!character) {
            continue;
        }
        character->CharProperties::PROPS_RemoveAllBuffs();
        std::list<Character*> followers;
        for (auto& entry : dword_99F764->m_objectMap) {
            if (entry.second.m_object == nullptr) {
                continue;
            }
            ObjectHandle handle = entry.second.m_object->GetHandle();
            Character* other = handle;
            if (!other) {
                continue;
            }
            if (other->IsFaerie() || character == other->m_master) {
                followers.push_back(other);
            }
        }
        if (!GetOnlineGameFlag() || (IsLocalPlayerHosting() && flag)) {
            Point3D<float> position = character->m_revivePosition;
            if (!(position == Vec3f_Origin)) {
                character->SetPosition(position, true);
                character->GetLookAtVec(position);
                const Point3D<float>& target = character->GetTargetPosition();
                position.X = position.X + target.X;
                position.Y = position.Y + target.Y;
                position.Z = position.Z + target.Z;
                for (Character* follower : followers) {
                    follower->SetPosition(position, true);
                }
            }
        } else {
            PlayerInfo* hosting = GetHostingPlayer();
            if (hosting->IsActive()) {
                Character* hostCharacter = hosting->unk_660;
                if (hostCharacter) {
                    Point3D<float> position = hostCharacter->m_position;
                    if (!(position == Vec3f_Origin)) {
                        character->SetPosition(position, true);
                        character->GetLookAtVec(position);
                    }
                }
            }
        }
        character->m_objectOfInterest = nullptr;  // +0x14a4
        character->Revive(nullptr, true);
        character->CharStateMachine::SM_SetIdleState(false);
        PyDataConstants* constants = ApplicationGetInstance().m_pyDataConstants;
        if (character->ItemInventory::GetNumPotions() < constants->getConstant("CharacterDesign", "MinimumPotionsOnDeath")) {
            character->ItemInventory::SetPotionQty(constants->getConstant("CharacterDesign", "MinimumPotionsOnDeath"));
        }
    }
    sound->StopAllMusic(2);
    sound->PlayMusic(ApplicationGetInstance().GetCurrentLevel()->m_musicSoundId, true, false, 1000);
}

// 0x375af0 _ZN13PlayerManager19_HandleGlobalDeathsEv PlayerManager::_HandleGlobalDeaths()
// The death sequence, one step per call, keyed by m_globalDeathState:
//   0: state 1; the fade-to-black menu is pushed (the multi-menu manager pops first, the effects volume goes to 0) when not visible; the local
//      pending flag and the local death timer widget are cleared.
//   2: the local record's timer is reset to -1 when it was set and, for a non-host, the local character moves to the level's spawn point, redoes the
//      quest sync, the object manager's byte +0x160 is cleared and the game events and objects are re-initialised. When every owned character
//      has a negative timer the state becomes 3 and the hosting player reloads the checkpoint, places its character at its revive position and
//      forces a full update.
//   3: hosting: state 4; otherwise state 4 once the local savegame is quest-synced and the object manager's byte +0x160 is set.
//   4: state 5 with a 2000 ms message timer (function-local static).
//   5: the timer runs down by the frame time; below 0 the fade screens swap, the local players revive (with the flag) and unk_718 is cleared.
void PlayerManager::_HandleGlobalDeaths() {
    static int messageDisplayTimer = 0;  // PlayerManager::_HandleGlobalDeaths()::messageDisplayTimer (.bss)
    switch (m_globalDeathState) {
        case 0: {
            m_globalDeathState = 1;
            MenuManager* manager = ApplicationGetInstance().m_menuManager;
            MenuBase* fade = manager->GetMenuByName("menu_FadeToBlackScreen");
            if (!fade->IsVisible()) {
                MenuManager::GetInstance()->m_multiMenuManager->PopMenu(true);
                manager->PushMenu("menu_FadeToBlackScreen");
                VoxSoundManager::s_instance->SetSoundVolume(1, 0.0f);
            }
            m_localDeathPending = false;
            InfoHUDManager::GetInstance()->HideLocalDeathTimer();
            return;
        }
        case 2: {
            PlayerInfo* local = GetLocalPlayer(0, false);
            if (local->m_characterDeathTimer.GetValue() >= 0) {
                local->SetCharacterDeathTimer(-1);
                if (!local->IsHost()) {
                    Character* localCharacter = GetLocalPlayer(0, true)->unk_660;
                    if (localCharacter) {
                        GameObject* spawnPoint = ApplicationGetInstance().GetCurrentLevel()->GetSpawnPoint();
                        if (spawnPoint) {
                            localCharacter->SetPosition(spawnPoint->m_position, true);
                        }
                        localCharacter->SG_RedoQuestSync();
                    }
                    dword_99F764->unk_160 = 0;
                    Level* level = ApplicationGetInstance().GetCurrentLevel();
                    if (level) {
                        level->m_gameEventManager->ReInit();
                    }
                    dword_99F764->ObjectReInit();
                }
            }
            bool allDead = true;
            for (int index = 0; index < GetNumPlayers(); ++index) {
                Character* character = GetPlayer(index, false)->unk_660;
                if (!character) {
                    continue;
                }
                if (GetPlayer(index, false)->m_characterDeathTimer.GetValue() >= 0) {
                    allDead = false;
                }
            }
            if (allDead) {
                m_globalDeathState = 3;
                Character* host = GetLocalPlayer(0, true)->unk_660;
                if (IsLocalPlayerHosting() && host) {
                    host->SG_RedoQuestSync();
                }
                if (IsLocalPlayerHosting()) {
                    Level* level = ApplicationGetInstance().GetCurrentLevel();
                    level->LoadCheckpoint();
                    if (host) {
                        host->SetPosition(host->m_revivePosition, true);
                    }
                    dword_99F764->ForceFullUpdate();
                }
            }
            return;
        }
        case 3: {
            if (IsLocalPlayerHosting()) {
                m_globalDeathState = 4;
                return;
            }
            Character* localCharacter = GetLocalPlayer(0, true)->unk_660;
            bool ready;
            if (localCharacter) {
                PlayerSavegame* savegame = localCharacter->m_savegame;
                if (!savegame) {
                    return;
                }
                ready = savegame->m_questSynced && (dword_99F764->unk_160 & 1);
            } else {
                ready = (dword_99F764->unk_160 & 1) != 0;
            }
            if (ready) {
                m_globalDeathState = 4;
            }
            return;
        }
        case 4: {
            m_globalDeathState = 5;
            messageDisplayTimer = 2000;
            return;
        }
        case 5: {
            messageDisplayTimer = messageDisplayTimer - ApplicationGetInstance().GetDt();
            if (messageDisplayTimer < 0) {
                MenuManager* menus = ApplicationGetInstance().m_menuManager;
                menus->PopMenu("menu_FadeToBlackScreen");
                menus->PushMenu("menu_FadeFromBlackScreen");
                ReviveLocalPlayers(true);
                unk_718 = false;
            }
            return;
        }
        default:
            return;
    }
}

// 0x375eb4 _ZN13PlayerManager18_CheckGlobalDeathsEv PlayerManager::_CheckGlobalDeaths()
// The check runs only in an online game on level step 38. When every character is dead (or has a negative death timer) the CMsgGlobalDeath
// message is sent once (while the death state is 0) and _HandleGlobalDeaths runs; otherwise it runs only while a death state is active.
void PlayerManager::_CheckGlobalDeaths() {
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (!level || level->m_loadStep != 38 || !GetOnlineGameFlag()) {
        return;
    }
    bool allDead = GetNumPlayers() != 0;
    for (int index = 0; index < GetNumPlayers(); ++index) {
        Character* character = GetPlayer(index, false)->unk_660;
        if (!character) {
            allDead = false;
            continue;
        }
        if (character->IsDead()) {
            continue;
        }
        if (GetPlayer(index, false)->m_characterDeathTimer.GetValue() < 0) {
            allDead = false;
        }
    }
    if (allDead) {
        if (m_globalDeathState == 0) {
            GetMessaging()->SendMsg(CMessage::CreateMessage("CMsgGlobalDeath", true));
        }
        _HandleGlobalDeaths();
    } else if (m_globalDeathState != 0) {
        _HandleGlobalDeaths();
    }
}

// 0x375fe8 _ZN13PlayerManager18_HandleLocalDeathsEv PlayerManager::_HandleLocalDeaths()
// Online: when the death state is idle, a pending local death counts the local record's timer down by the frame time,
// and at zero clears the pending flag, hides the timer and revives. Otherwise it starts the pending death and shows the
// timer. Offline: the fade screen is pushed when it is not already visible.
// Offline the effects volume also goes to 0 (VoxSoundManager::SetSoundVolume(1, 0.0)).
void PlayerManager::_HandleLocalDeaths() {
    if (GetOnlineGameFlag()) {
        if (m_globalDeathState == 0) {
            if (m_localDeathPending) {
                PlayerInfo* local = GetLocalPlayer(0, false);
                const int remaining = local->m_characterDeathTimer.GetValue() - ApplicationGetInstance().GetDt();
                if (remaining < 0) {
                    m_localDeathPending = false;
                    local->SetCharacterDeathTimer(-1);
                    InfoHUDManager::GetInstance()->HideLocalDeathTimer();
                    ReviveLocalPlayers(false);
                } else {
                    local->SetCharacterDeathTimer(remaining);
                }
            } else {
                m_localDeathPending = true;
                ResetCharacterDeathTimer();
                InfoHUDManager::GetInstance()->ShowLocalDeathTimer();
            }
        }
    } else {
        MenuManager* menus = MenuManager::GetInstance();
        MenuBase* fade = menus->GetMenuByName("menu_FadeToBlackScreen");
        if (!fade->IsVisible()) {
            menus->PushMenu("menu_FadeToBlackScreen");
            VoxSoundManager::s_instance->SetSoundVolume(1, 0.0f);
        }
    }
}

// 0x376100 _ZN13PlayerManager17_CheckLocalDeathsEv PlayerManager::_CheckLocalDeaths()
// On level step 38, when the local character is dead and at least one character is owned, the local death handling
// runs. The return value is the dead flag (the original leaves a stale register on some paths; callers ignore it).
int PlayerManager::_CheckLocalDeaths() {
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (!level || level->m_loadStep != 38) {
        return 0;
    }
    Character* character = static_cast<Character*>(GetLocalPlayer(0, false)->unk_660);
    if (!character) {
        return 0;
    }
    const bool dead = character->IsDead();
    if (dead && m_numCharacters > 0) {
        _HandleLocalDeaths();
    }
    return dead ? 1 : 0;
}

// 0x376380 _ZN13PlayerManager25OnHostChangedNotificationEi PlayerManager::OnHostChangedNotification(int)
// The record must be the host (assert "pInfo.IsHost()", line 2095). With a level loaded the host-change flag is set; a local record that is not
// in the game gets its ready-to-roll flag reset and the timers cleared. A record with a character queues the status line
// "<name> is now host" (StrID MENU_IS_NOW_HOST with the player name as the %s argument) on the OnlineStatusMsg queue; a record without one tells the
// lobby (menu_MultiplayerLobby "PlayerBecameHost") when this client is hosting. In both cases the room's attribute 3 (binary) becomes the
// record's character name. A death state of 3 steps back to 2.
void PlayerManager::OnHostChangedNotification(int internalId) {
    PlayerInfo* info = GetPlayerByInternalID(internalId, false);
    if (!info->IsHost()) {
        ReportAssertFailure("pInfo.IsHost()", 2095);
    }
    if (ApplicationGetInstance().GetCurrentLevel()) {
        m_hostChanged = true;
    }
    if (info->IsLocal() && !info->m_ingame.GetValue()) {
        info->SetReadyToRoll(info->m_ingame.GetValue());
        m_readyToRollTimer = -1;
        m_readyToRollWaiting = false;
    }
    if (Character* character = info->unk_660) {
        std::string text;
        Application& app = ApplicationGetInstance();
        const char* format = app.m_stringManager->getString(app.m_pyDataConstants->getConstant("StrID", "MENU_IS_NOW_HOST"));
        const std::string playerName = StringManager::formatForMulti(std::string(character->SG_GetPlayerName()), false);
        app.m_stringManager->parse(text, format, playerName.c_str());
        MenuMessageManager<OnlineStatusMsg, 1>& statusMessages = MenuMessageManager<OnlineStatusMsg, 1>::s_inst;
        statusMessages.m_queues[0].push_back(OnlineStatusMsg(text));
        if (statusMessages.m_queues[0].size() == 1) {
            statusMessages.Invoke(OnlineStatusMsg::s_StartFuncName[0], 0);
        }
    } else if (IsLocalPlayerHosting()) {
        if (MenuManager::GetInstance()->GetRoot() != nullptr) {
            MenuManager::GetInstance()->GetRoot()->InvokeASCallback("menu_MultiplayerLobby", "PlayerBecameHost", nullptr, 0);
        }
    }
    CRoomAttributes attributes(*CMatching::Get()->GetRoomAttributes());
    const std::string roomName = info->m_characterName.GetValue();
    attributes.SetAttributeBin(ROOM_BIN_ATTRIBUTE_3, roomName.data(), static_cast<unsigned int>(roomName.size()));
    *CMatching::Get()->GetRoomAttributes() = attributes;
    if (m_globalDeathState == 3) {
        m_globalDeathState = 2;
    }
}

// 0x378a40 _ZN13PlayerManager10_AddPlayerEiiib PlayerManager::_AddPlayer(int, int, int, bool)
// Gate open: the record of the internal id, when active, takes the flag (+0x66c), the pad index (a4, +0x668), the member id (a3, +0x674) and
// the internal id (+0x670), its NetStruct is enabled, and the player numbers are updated (the result of the active test is returned otherwise).
// Gate closed: when the internal id is not already in the local map the record is built in a temporary PlayerInfo (unk_670 the internal id,
// unk_66c the flag, unk_668 the third argument, unk_674 the second) and stored in the map at that key, then the player numbers are updated;
// otherwise the already-present result is returned.
int PlayerManager::_AddPlayer(int internalId, int a3, int a4, bool flag) {
    if (NetGateOpen()) {
        PlayerInfo* record = GetPlayerByInternalID(internalId, false);
        const bool active = record->IsActive();
        if (active) {
            record->unk_66c = flag ? 1 : 0;
            record->unk_668 = a4;
            record->unk_674 = a3;
            record->unk_670 = internalId;
            record->SetEnabled(true);
            return _UpdatePlayerNumbers();
        }
        return active;
    }
    const bool present = IsPlayerInLocalMap(internalId);
    if (present) {
        return present;
    }
    // The original fills a default-built temporary and copy-assigns it into the new map element. PlayerInfo's copy
    // operations are deleted in dh2/PlayerInfo.hpp, and the temporary's only non-default state is these four fields, so
    // they are set on the element the map default-built for this absent key (the same state).
    PlayerInfo& record = m_players[internalId];
    record.unk_670 = internalId;   // +0x670
    record.unk_66c = flag ? 1 : 0; // +0x66c
    record.unk_668 = a4;           // +0x668
    record.unk_674 = a3;           // +0x674
    return _UpdatePlayerNumbers();
}

// 0x378b94 _ZN13PlayerManager23_CheckRemoteControllersEv PlayerManager::_CheckRemoteControllers()
// With the network gate open, every player in the id list whose matching member id differs from this client's (CMatching::LocalMemberId,
// vtable +0x6c) and whose record does not carry that internal id yet is added: _AddPlayer(id, member id, -1, false). Gate closed: nothing.
void PlayerManager::_CheckRemoteControllers() {
    if (!NetGateOpen()) {
        return;
    }
    const int localMember = CMatching::Get()->LocalMemberId();
    for (std::size_t i = 0; i != m_playerIdList.size(); ++i) {
        const int id = m_playerIdList[i];
        const int member = static_cast<PlayerInfo*>(GetNetPlayerMgr()->GetPlayer(id, false))->matching_member_id_.GetValue();
        if (localMember != member) {
            if (id != GetPlayerByInternalID(id, false)->unk_670) {
                _AddPlayer(id, member, -1, false);
            }
        }
    }
}

// 0x378c80 _ZN13PlayerManager22_CheckLocalControllersEv PlayerManager::_CheckLocalControllers()
// Walks the gamepads (at least one). A pad counts as in use when it is connected (pad 0 always). With the network gate closed the record id is
// the pad index and it needs adding when absent from the local map; with it open the net player manager's local player of that index is used
// (its record id, member id; a record whose number differs needs adding, and when it is the first local player's the record's save slot is
// taken from the savegame manager's last slot if unset). A pad in use either adds the player (_AddPlayer(id, member, pad, true)) or, for a record
// without a save slot, runs the joining check (_UpdateJoiningController while the net state is 2, otherwise after the join button
// channel 14 changes state the HUD gets "onNewPlayerLocal" with the player's local number and the net state becomes 2). Returns the pad count.
unsigned int PlayerManager::_CheckLocalControllers() {
    InputManager* input = InputManager::GetInstance();
    int numPads = input->GetNumGamepads();
    if (numPads < 1) {
        numPads = 1;
    }
    for (int i = 0; i < numPads; ++i) {
        Gamepad* pad = input->GetGamepad(i);
        bool padInUse = pad->m_connected;
        if (i == 0) {
            padInUse = true;
        }
        int internalId;
        int memberId = -1;
        bool needsAdding;
        if (!NetGateOpen()) {
            needsAdding = !IsPlayerInLocalMap(i);
            internalId = i;
        } else {
            PlayerInfo* netLocal = static_cast<PlayerInfo*>(GetNetPlayerMgr()->GetLocalPlayer(i));
            if (netLocal->IsActive()) {
                internalId = netLocal->member_id_.GetValue();
                memberId = netLocal->matching_member_id_.GetValue();
            } else {
                internalId = -1;
            }
            needsAdding = internalId != netLocal->unk_670;
            if (needsAdding) {
                if (netLocal->unk_670 == GetLocalPlayer(0, false)->unk_670) {
                    if (netLocal->unk_664 == -1) {
                        const int lastSlot = ApplicationGetInstance().m_savegameManager->m_lastSlot;
                        if (lastSlot != -1) {
                            netLocal->unk_664 = lastSlot;
                        }
                    }
                }
            }
            if (internalId == -1) {
                continue;
            }
        }
        if (padInUse) {
            if (needsAdding) {
                _AddPlayer(internalId, memberId, i, true);
            } else {
                PlayerInfo* record = GetPlayerByInternalID(internalId, false);
                if (record->unk_664 == -1) {
                    if (record->unk_240.GetValue() == 2) {
                        _UpdateJoiningController(i, internalId);
                    } else {
                        const Gamepad::Channel& joinButton = pad->m_buttons[14];
                        const float threshold = ((joinButton.previous_maximum + joinButton.previous_minimum) + 1.0f) * 0.5f;
                        const bool above = joinButton.previous >= threshold;
                        if (above != joinButton.previous_above_threshold) {
                            if (Level* level = ApplicationGetInstance().GetCurrentLevel()) {
                                gameswf::as_value number;
                                number.set_double(static_cast<double>(record->unk_67c));
                                MenuManager::GetInstance()->GetHUDRoot()->InvokeASCallback("menu_HUD_0", "onNewPlayerLocal", &number, 1);
                                record->SetState(2);
                            }
                        }
                    }
                }
            }
        }
    }
    return static_cast<unsigned int>(numPads);
}

// 0x37280c _ZN13PlayerManager17_ManageCharactersEv PlayerManager::_ManageCharacters()
// PARTIAL. The original walks the player records, and for each active record synchronises the replicated fields with the
// owned character (class, level, potions, XP, gold, skills, the ready-to-roll and in-game flags), spawns a character where
// one is missing, and runs the online loading and ready-to-roll handshake. Only the walk over the active records is written
// here, with the per-record body partial. The tail after the walk is written for the offline case, where every online
// branch is skipped: the loading info is cleared and the characters are removed when the record is not loading.
// The forced-resend timers are function-local statics. Their initial values are -1 (.data, read from the binary).
// The return value is the constant the original leaves in r0 at its final return (16556).
int PlayerManager::_ManageCharacters() {
    static int forceResendTimer = -1;                 // PlayerManager::_ManageCharacters()::forceResendTimer (.data, -1)
    static int forceResendLoadingInfoTimer = -1;      // PlayerManager::_ManageCharacters()::forceResendLoadingInfoTimer (.data, -1)
    (void)forceResendTimer;
    (void)forceResendLoadingInfoTimer;

    const int numPlayers = GetNumPlayers();
    for (int index = 0; index < numPlayers; ++index) {
        PlayerInfo* player = GetPlayer(index, false);
        if (!player->IsActive()) {
            continue;
        }
        // PARTIAL: the per-record synchronisation (see above) is not written.
    }

    // Tail. The original's online block (the loading handshake and the resend timers, see the PARTIAL note above) only sets
    // the resend state and falls through to the same final test, so that test is exact for both the online and the
    // offline case: when the loading flag unk_6c9 is clear and characters are owned, clear the loading info and remove them.
    if (!unk_6c9 && m_numCharacters > 0) {
        ClearLoadingInfo();
        RemoveAllCharacters();
    }
    return 16556;
}

