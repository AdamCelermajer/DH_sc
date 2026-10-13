// Batch g1-game-character-PlayerManager-025dba-2 (part 3 of 4 of PlayerManager): the destructors and constructor, the
// character and player removal, _AddCharacter, the online-transition check, _ManageCharacters, the death handling,
// the host-change notification, the remote and local controller checks.
// Declarations and layout come from dh2/PlayerManager.hpp. Offsets in comments are the original 32-bit offsets.
// Where a branch depends on a value the tree does not declare, the function is partial and a PARTIAL comment names
// the missing piece; the ledger rows in game-character/ledger.d/g1-game-character-PlayerManager-025dba-2.csv repeat it.
//
// Online gate: Application::IsOnlineGame() is *(GetOnline() + 5) (0x320668), so the first test of every online gate
// is readable. Most gates continue with OnlineGameState's byte at +0x24, which online_game_state.hpp does not model,
// so the online branch of those functions is partial. The offline branch is exact wherever the gate starts with the
// online test: every later term of the && chain is then false and the else path runs.

#include <cstdint>
#include <cstdio>
#include <cstdlib>
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
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/ObjectBase.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/CharProperties.hpp"
#include "dh2/InputManager.hpp"
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
// The original tests the online gate first and runs the removal when the gate is false. The offline branch is exact:
// IsPlayerInLocalMap (0 returns at once), the character of the record with that key is removed, the record is erased.
// PARTIAL: the online branch. Its gate needs OnlineGameState's byte at +0x24 (not modelled in online_game_state.hpp),
// CMatching::Get()->UnknownGate() and CNetPlayerManager::IsInitialized(). When the gate is true the original skips the
// removal; that path is not selected here.
int PlayerManager::RemovePlayer(int internalId) {
    if (!ApplicationGetInstance().IsOnlineGame()) {
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
// PARTIAL. The body runs only when the character is not already marked for deletion (ObjectBase byte +0x81).
// Written: the owned-character count (asserted above zero), the zero position, the Character and CharAI teardown
// (InitCam, RemoveMultiplayerHighlight, AI target and aggro resets), the delete, the reset of the record's character
// pointer and its unk_684, the replicated-field resets for a local record (the class, name, level, byte array, XP,
// gold and the in-game flag), and the local camera retarget on the first record.
// Not written:
//   - Character virtual slots +0x48 and +0x40 (named nowhere in the tree): the calls on the character before the AI
//     reset and the call with 0 after the position.
//   - The zero stored at Character +0x14a4 (not declared on Character; m_selfFX is +0x1484, a different word).
//   - The rename "<name>_Deleted_<n>": the original reads the name as a char pointer at Character +0x44, which
//     ObjectBase.hpp does not declare (its name is std::string m_name at +0x30). The static counter is written.
//   - CameraLevel's virtual slot +0x10 after the retarget, and the RenderFX::InvokeASCallback "completeRefresh" on
//     the HUD root (InvokeASCallback is not declared in dh2/RenderFX.hpp).
void PlayerManager::RemoveCharacter(Character* a2) {
    if (!a2 || a2->m_markedForDeletion) {
        return;
    }
    PlayerInfo* info = GetPlayerByCharacter(a2, true);
    const bool noInternalId = info->unk_670 == -1;  // v8: the record has no internal id (+0x670)

    if (m_numCharacters <= 0) {
        ReportAssertFailure("m_numCharacters > 0", 1247);
    }
    m_numCharacters = m_numCharacters - 1;

    const Point3D<float> origin{0.0f, 0.0f, 0.0f};
    a2->SetPosition(origin, true);
    // PARTIAL: virtual slots +0x48 and +0x40 of Character (see the comment above) are not called.
    a2->InitCam();
    a2->RemoveMultiplayerHighlight();
    // PARTIAL: the original stores 0 at Character +0x14a4 (index 1321). Character.hpp does not declare that word.
    a2->CharAI::AI_SetTarget(nullptr, false);
    a2->CharAI::AI_SyncLastTarget();
    a2->CharAI::AI_ClearAllAggro();
    a2->CharAI::AI_ClearAllAggroTowardMe(false);
    a2->ObjectBase::Delete();
    // PARTIAL: the "<name>_Deleted_<n>" rename is not written (see the comment above).
    ++s_deletedPlayerCharacterCounter;

    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (!noInternalId) {
        if (level && info->unk_66c) {
            if (m_numCharacters > 0) {
                GameObject* target = static_cast<GameObject*>(GetLocalPlayer(0, true)->unk_660);
                if (!target) {
                    target = static_cast<GameObject*>(GetPlayer(0, false)->unk_660);
                }
                if (target) {
                    // PARTIAL: CameraLevel's virtual slot +0x10 (the camera update after the retarget) is not called.
                    level->m_camera->SetTarget(target, 0);
                    if (m_numCharacters == 1) {
                        static_cast<Character*>(target)->InitCam();
                    }
                }
            }
            level->UpdateLightSet(true, 4);
        }
    }

    if (!noInternalId) {
        // LABEL_14: reset the record. The original reaches this block whenever the character has an internal id, with or
        // without a level (the level only chooses the camera branch above).
        if (!unk_6c9) {
            info->unk_660 = nullptr;           // the original stores the byte value (0) into the character pointer
        }
        info->unk_684 = 0;
        // PARTIAL: the original writes unk_4a0 (+0x4a0) through its vtable setter with the address of an uninitialised
        // stack word (v22), so the value is not known and no write is made here.
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
    // PARTIAL: the HUD refresh ("menu_HUD_0", "completeRefresh") on MenuManager::GetInstance()->GetHUDRoot() is not
    // written; RenderFX::InvokeASCallback is not declared in dh2/RenderFX.hpp.
}

// 0x372220 _ZN13PlayerManager13_AddCharacterEi PlayerManager::_AddCharacter(int)
// Spawns the player's character when the record has a class and no character yet. The return value of the original is
// the record pointer's bits on the no-spawn path and the r0 residue of the last callee on the other paths; callers
// (_ManageCharacters) ignore it, so this port returns 0.
// PARTIAL: the online branch (the host-position copy through GetHostingPlayer, +0x1450 initial position and the room
// zone insert), the virtual slot +0x40 (named nowhere) called with the in-level flag, and the skill-level loop, whose
// bound is the savegame's skill count (not declared on PlayerSavegame). Also not written: the two words the original
// stores on the new character at +0x1f88 (the record's +0x674) and +0x1f8c (the record's +0x670). Character's declared
// layout ends at +0x1f88 (the original allocation is 0x1f90), so no member exists for them (layout request).
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

        if (ApplicationGetInstance().IsOnlineGame()) {
            // PARTIAL: the online branch (see above) is not written.
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
        // PARTIAL: the skill-level loop (unk_400, 30 bytes) is not written; its bound is the savegame skill count.
        // PARTIAL: virtual slot +0x40 of Character (named nowhere) is called with the in-level flag here.

        if (Level* level = ApplicationGetInstance().GetCurrentLevel()) {
            level->QuickSave(false);
        }
        ++m_numCharacters;

        if (!ApplicationGetInstance().IsOnlineGame() || character == GetPlayerByInternalID(internalId, true)->unk_660) {
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
// The offline branch is exact. With the online flag clear, every gate of the original is false (each is an && chain
// that starts with the flag), so the else path runs: the host-change flag and the transition flag are cleared, the
// loading info is cleared, and the quest-sync buffer is cleared (its result is the return value).
// PARTIAL: the online branch. The map is cleared when the transition gate holds, the connection check reads the
// OnlineGameState int at +0x34 (not modelled) and calls Application::GoToMainMenu(3) when no connection remains, and the
// transition is entered with the net player manager's id lists copied into m_playerIdList and m_localPlayerIdList.
// Each of those gates needs OnlineGameState's byte at +0x24, which online_game_state.hpp does not model.
int PlayerManager::_CheckOnlineTransition() {
    if (!ApplicationGetInstance().IsOnlineGame()) {
        m_hostChanged = false;
        m_inOnlineTransition = false;
        ClearLoadingInfo();
        return ClearQuestSyncBuffer();
    }
    // PARTIAL: online branch (see above) is not written. The return value is 0 here.
    return 0;
}

// 0x37565c _ZN13PlayerManager18ReviveLocalPlayersEb PlayerManager::ReviveLocalPlayers(bool)
// PARTIAL. Written: the death state reset (m_localDeathPending, m_globalDeathState), the checkpoint reload when offline,
// the buff removal and the per-local-player revive (Character::Revive with no reviver, the idle state, and the potion
// minimum from the CharacterDesign constant).
// Not written:
//   - VoxSoundManager: the VolumeFX setting is read (Application::GetSavedOption) but SetSoundVolume, StopAllMusic and
//     PlayMusic are not declared in dh2 (the sound manager has no game-side header).
//   - The position copy for each local character: from the local record's saved position when the local player is the
//     host, or from the hosting player's position otherwise. The flag argument only selects this branch.
//   - The object scan that collects the local player's followers (a walk of ObjectManager's list at +0x0c..+0x20 with
//     the Faerie and owner tests), which moves them to the revived position. The list fields are not declared.
//   - The Character word at +0x14a4 cleared before the revive (not declared on Character).
void PlayerManager::ReviveLocalPlayers(bool flag) {
    m_localDeathPending = false;      // +0x711
    m_globalDeathState = 0;           // +0x714
    if (!ApplicationGetInstance().IsOnlineGame()) {
        Level* level = ApplicationGetInstance().GetCurrentLevel();
        level->LoadCheckpoint();
    }
    // PARTIAL: VoxSoundManager::SetSoundVolume(s_instance, 1, GetSavedOption("VolumeFX")) is not written.
    (void)flag;  // the flag only gates the position copy in the original, which is partial (see the ledger)

    const int count = GetNumLocalPlayers(false);
    for (int index = 0; index < count; ++index) {
        Character* character = static_cast<Character*>(GetLocalPlayer(index, false)->unk_660);
        if (!character) {
            continue;
        }
        character->CharProperties::PROPS_RemoveAllBuffs();
        // PARTIAL: the follower scan and the position copy (see above) are not written. The character is revived at
        // its current place when the position copy is skipped.
        character->Revive(nullptr, true);
        character->CharStateMachine::SM_SetIdleState(false);
        const int minimumPotions = ApplicationGetInstance().m_pyDataConstants->getConstant("CharacterDesign", "MinimumPotionsOnDeath");
        if (character->ItemInventory::GetNumPotions() < minimumPotions) {
            character->ItemInventory::SetPotionQty(minimumPotions);
        }
    }
    // PARTIAL: the music restart (StopAllMusic, PlayMusic on the current level's music id) is not written.
}

// 0x375af0 _ZN13PlayerManager19_HandleGlobalDeathsEv PlayerManager::_HandleGlobalDeaths()
// The death sequence, one step per call, keyed by m_globalDeathState. States 0, 3 (the hosting branch), 4 and 5 are
// written. State 2 is partial: the revive placement and the object reinit are written, the spawn-point move and the
// objects' +0x160 position are not (no field named there). The message timer is a function-local static (.bss, zero).
void PlayerManager::_HandleGlobalDeaths() {
    static int messageDisplayTimer = 0;  // PlayerManager::_HandleGlobalDeaths()::messageDisplayTimer (.bss)
    switch (m_globalDeathState) {
        case 0: {
            m_globalDeathState = 1;
            MenuManager* menus = MenuManager::GetInstance();
            MenuBase* fade = menus->GetMenuByName("menu_FadeToBlackScreen");
            if (!fade->IsVisible()) {
                // PARTIAL: the original calls virtual slot +0x38 on the menu manager's object at +0xf4 with 1 (the fade
                // screen's setup), and sets the sound volume to 0. Neither is declared in dh2.
                menus->PushMenu("menu_FadeToBlackScreen");
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
                    // PARTIAL: the spawn-point move (GameObject +0x160 position) is not written.
                    Character* character = static_cast<Character*>(GetLocalPlayer(0, true)->unk_660);
                    if (character) {
                        character->SG_RedoQuestSync();
                    }
                    // PARTIAL: ObjectManager word +0x160 is cleared by the original; not declared on ObjectManager.
                    Level* level = ApplicationGetInstance().GetCurrentLevel();
                    if (level) {
                        level->m_gameEventManager->ReInit();
                    }
                    dword_99F764->ObjectReInit();
                }
            }
            bool allDead = true;
            const int count = GetNumPlayers();
            for (int index = 0; index < count; ++index) {
                Character* character = static_cast<Character*>(GetPlayer(index, false)->unk_660);
                if (!character) {
                    continue;
                }
                if (GetPlayer(index, false)->m_characterDeathTimer.GetValue() >= 0) {
                    allDead = false;
                }
            }
            if (allDead) {
                m_globalDeathState = 3;
                Character* host = static_cast<Character*>(GetLocalPlayer(0, true)->unk_660);
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
            }
            // PARTIAL: the non-hosting test reads PlayerSavegame's flag (+0x14 of the savegame) and ObjectManager's word
            // +0x160, neither declared. The state stays 3 in that case.
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
                MenuManager* menus = MenuManager::GetInstance();
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
// The check runs only in an online game on level step 38. Offline the function returns at once (exact). Online, the
// all-dead test is written; when every character is dead (or unset with a negative timer) the original sends the
// CMsgGlobalDeath message once and runs _HandleGlobalDeaths.
// PARTIAL: the message send. CMessage::CreateMessage's last two arguments are not recovered from the pseudocode.
void PlayerManager::_CheckGlobalDeaths() {
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (!level || level->m_loadStep != 38 || !ApplicationGetInstance().IsOnlineGame()) {
        return;
    }
    bool allDead = GetNumPlayers() != 0;
    const int count = GetNumPlayers();
    for (int index = 0; index < count; ++index) {
        PlayerInfo* player = GetPlayer(index, false);
        Character* character = static_cast<Character*>(player->unk_660);
        if (!character) {
            allDead = false;
            continue;
        }
        if (character->IsDead()) {
            continue;
        }
        if (player->m_characterDeathTimer.GetValue() < 0) {
            allDead = false;
        }
    }
    if (allDead) {
        if (m_globalDeathState == 0) {
            // PARTIAL: GetMessaging()->SendMsg(CMessage::CreateMessage("CMsgGlobalDeath", ...)) is not written.
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
// PARTIAL: the offline sound volume (VoxSoundManager::SetSoundVolume to 0) is not written.
void PlayerManager::_HandleLocalDeaths() {
    if (ApplicationGetInstance().IsOnlineGame()) {
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
            // PARTIAL: VoxSoundManager::SetSoundVolume(s_instance, 1, 0.0) is not written.
        }
    }
}

// 0x376100 _ZN13PlayerManager17_CheckLocalDeathsEv PlayerManager::_CheckLocalDeaths()
// On level step 38, when the local character is dead and at least one character is owned, the local death handling
// runs. The return value is the dead flag (the original leaves the callee's r0 in place on the handling path, which is
// not reproduced).
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
// PARTIAL. Written: the host assertion on the record, the host-change flag when a level is loaded, the ready-to-roll
// reset for a local record that is not in the game, and the final death-state step (3 becomes 2).
// Not written: the chat line for a record with a character (OnlineStatusMsg queue, StringManager text, the debug
// cached-character refresh) and the lobby notification (RenderFX::InvokeASCallback is not declared), and the
// CRoomAttributes update that writes the player name into the room (its SetAttributeBin signature is not in the tree).
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
    // PARTIAL: the character branch (chat line and room-name update) and the lobby branch are not written.
    if (m_globalDeathState == 3) {
        m_globalDeathState = 2;
    }
}

// 0x378a40 _ZN13PlayerManager10_AddPlayerEiiib PlayerManager::_AddPlayer(int, int, int, bool)
// The offline branch is exact: the record is built in a temporary PlayerInfo (unk_670 the internal id, unk_66c the flag,
// unk_668 the third argument, unk_674 the second) and stored in the map at that key, then the player numbers are updated.
// The original does this only when the internal id is not already in the local map; otherwise it returns that result.
// PARTIAL: the online branch, which updates an existing record in place when the online matching gate holds. The gate
// needs OnlineGameState's byte at +0x24, which online_game_state.hpp does not model.
int PlayerManager::_AddPlayer(int internalId, int a3, int a4, bool flag) {
    if (ApplicationGetInstance().IsOnlineGame()) {
        // PARTIAL: online branch (see above) is not written.
        return 0;
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
// Offline: exact no-op (the first gate is the online flag). Online: PARTIAL. The original walks the player-id list and
// re-adds each remote record whose controller id differs, through _AddPlayer. Its gate needs the OnlineGameState byte
// at +0x24, which online_game_state.hpp does not model.
void PlayerManager::_CheckRemoteControllers() {
    if (!ApplicationGetInstance().IsOnlineGame()) {
        return;
    }
    // PARTIAL: the remote-controller walk is not written (see above).
}

// 0x378c80 _ZN13PlayerManager22_CheckLocalControllersEv PlayerManager::_CheckLocalControllers()
// The return value is the number of gamepads, at least 1 (the original keeps that count in its loop bound and returns
// it on every path). PARTIAL: the per-controller loop is not written. It reads each controller through InputManager's
// virtual slot +0x08 (not declared), and adds, updates or removes local records from the controller state.
unsigned int PlayerManager::_CheckLocalControllers() {
    int gamepads = InputManager::GetInstance()->GetNumGamepads();
    if (gamepads < 1) {
        gamepads = 1;
    }
    // PARTIAL: the controller loop is not written (see above).
    return static_cast<unsigned int>(gamepads);
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

