// Batch g1-game-character-Character-ee9946-6: Character::Update, the two CreatePlayer and the two CreateNPC overloads,
// the CSM_* state-machine callbacks, CTRLIsAllowed, the Ctrl_* controller entry points, and ForceUseOOI and UseOOI.
// Declarations come from dh2/Character.hpp, which this batch does not change. Several bodies are PARTIAL because the
// members they read are private in dh2/CharAI.hpp and dh2/CharStateMachine.hpp (owned by other batches), or because a
// header cannot be included in the same translation unit as Character.hpp. Each comment names what is missing; the
// ledger rows in game-character/ledger.d/g1-game-character-Character-ee9946-6.csv repeat it.

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>

#include "core-platform/memory.hpp"
#include "dh2/global/runtime_support.hpp"  // before Character.hpp: its MemoryHintState use needs the unrenamed type
#include "dh2/AnimatedFX.hpp"
#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/v2Controller.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"
#include "dh2/global/ctrl_camera.hpp"

// 0x3abe98 _ZN9Character6UpdateEv
// Character::Update(void)
// PARTIAL. Reproduced: the profiling scopes, the Give50Potions switch, the CanUpdate gate, the object-manager counter,
// the controller update, the per-frame base updates (CharTimers, CharAI, CharStateMachine, CharAnimator, GameObject),
// the combat-text cooldown, the player's object-of-interest and spot-target updates, and the multiplayer highlight.
// Not reproduced:
//  - KillPlayerOne: PlayerManager::GetPlayer(0, false) and the Character +0x1088 branch. PlayerManager.hpp cannot be
//    included with Character.hpp (its StreamBuffer member needs net-transport's IStreamBase, which pydata's
//    stream_reader.hpp also defines).
//  - Script-load gate and load: CharAI::m_script (+0x1c) is private in dh2/CharAI.hpp, so the gate cannot be evaluated
//    and LoadNInitScriptProcess is not called. The queue trim (Level::m_levelListIndex; Level.hpp has the same include
//    conflict) and the monster staged insert into s_concurrentAI (needs the offline flag of COnline, which has no getter
//    in net-session-online/include/net-session-online/c_online.hpp) belong to the load and are not run. The insert
//    itself is s_concurrentAI[real time] = this, with glitch::os::Timer::getRealTime() (static).
//  - isABot bot path: needs the online flag, PlayerInfo +0x66c, +0x678 and +0x660 (unnamed; +0x660 is void*), the potion
//    and equip timers, and object-of-interest navigation. The switch is read and nothing else runs.
//  - m_timerUtil->Update(): TimerUtil has no class declaration under game/include (forward declaration only).
//  - Faerie copy when (LG_DEVICES || P1000TF) and IsFaerie(): P1000TF is not declared, and the globals m_fairyPos_new,
//    dword_9F69B8, dword_9F69BC and dword_9F69C0 are not declared. The branch that follows the copy is the normal one,
//    which is what runs here.
//  - UpdateStateFX on frames where (frame counter & 7) == 0: dword_99F7A0 is not declared, so UpdateStateFX is not called.
//  - Merchant marker FX and the online branches of the non-player path: the VisualFXManager instance has no accessor
//    (Singleton<VisualFXManager>::s_inst), the test of GameObject unk_2fa (+0x2fa, meaning unknown) is not confirmed,
//    and the online flag is as above.
//  - Visual branch on PlayerManager::IsLocalPlayer: the non-local hide (m_targetCircleFX[m_targetCircleIndex], then
//    m_cursorFX) and the local-player target circles, object-of-interest FX and cursor. AnimatedFX +0x28 (owner) is int in
//    dh2/AnimatedFX.hpp, and v2Controller +0x09 is unnamed.
//  - Pending gold and XP scrolling text: the argument layout of F_ApplyScrollingCombatTextGold and
//    F_ApplyScrollingCombatTextXP in the listing does not match the header's parameter list.
void Character::Update() {
    PushProfilingContext("Character::Update");

    // KillPlayerOne (PARTIAL, see above).
    DebugSwitches* switches = DebugSwitches::GetInstance();
    switches->load();
    if (switches->GetSwitch("KillPlayerOne")) {
        // PARTIAL: PlayerManager::GetPlayer(0, false) and the Character +0x1088 branch are not reproduced.
    }

    // Give50Potions: the potion quantity is set to 50 for a player character.
    switches->load();
    if (switches->GetSwitch("Give50Potions") && IsPlayer()) {
        ItemInventory::SetPotionQty(50);
    }

    if (!CanUpdate()) {
        PopProfilingContext("Character::Update");
        return;
    }

    // Script-load scope. PARTIAL: the gate reads CharAI::m_script (private), so the load is not reproduced (see above).
    PushProfilingContext("Character::LoadNInitScript");
    PopProfilingContext("Character::LoadNInitScript");

    ++dword_99F764->unk_5c;  // ObjectManager +0x5c; the counter's name is not recovered
    m_controller->Update();

    // isABot debug bot path (PARTIAL, see above): the switch is read and the body is not reproduced.
    switches->load();
    (void)switches->GetSwitch("isABot");

    // m_timerUtil->Update() is not reproduced (PARTIAL, see above).
    CharTimers::Update();
    CharAI::Update();
    CharStateMachine::Update();
    CharAnimator::Update();
    GameObject::Update();

    if (m_combatTextCooldown > 0.0f) {
        m_combatTextCooldown =
            m_combatTextCooldown - static_cast<float>(static_cast<unsigned int>(ApplicationGetInstance().GetDt()));
    }

    // Faerie position copy (PARTIAL, see above). The branch below is the one both paths take.
    if (IsPlayer()) {
        UpdateObjectOfInterest();
        UpdateSpotTarget();
    } else {
        // PARTIAL: merchant marker FX and the online branches (see above). Not reproduced.
    }

    // UpdateStateFX on the frame-counter gate and the visual branch (PARTIAL, see above). Not reproduced.

    if (IsMultiplayerGame() && m_multiplayerHighlightFX) {
        m_multiplayerHighlightFX->SetVisible(true);
    }

    // PARTIAL: the pending gold and XP scrolling text, when the cutscene check passes (see above). Not reproduced.

    PopProfilingContext("Character::Update");
}

// 0x3acd58 _ZN9Character12CreatePlayerEPS_jjPKc
// Character::CreatePlayer(Character*, unsigned int, unsigned int, char const*)
// PARTIAL: when source is null and gAssertLevel is 2, the original stores 0 to address 0 (a deliberate crash) before it
// returns nullptr. That store is not reproduced. gAssertLevel 1 prints the assert message to stderr (the third _iob
// entry). The slot and a parameters are not read by the body (Hex-Rays types the slot as Character*).
Character* Character::CreatePlayer(Character* source, unsigned int slot, unsigned int a, const char* name) {
    (void)slot;
    (void)a;
    if (!source) {
        if (gAssertLevel == 2) {
            // PARTIAL: the original writes 0 to address 0 here. Not reproduced.
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "tpl",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Character_Creators.cpp",
                         58);
        }
        return nullptr;
    }

    // The argument order is the one the original passes ("Character", then the object name). The header names the
    // Spawn parameters (name, type) and game-world's Level note records that the header does not match its call.
    ObjectHandle handle = dword_99F764->Spawn("Character", name, true, true);
    Character* target = handle;
    if (target) {
        static_cast<PropertyMap&>(static_cast<ObjectBase&>(*target))
            .CloneProperties(static_cast<PropertyMap&>(static_cast<ObjectBase&>(*source)));
        target->ObjectBase::m_name.assign(name);
        target->SG_SetSlot(static_cast<unsigned int>(source->SG_GetSlot()));
        target->InitPost();
        target->InitFinal();
        target->CharStateMachine::SM_SetIdleState(false);
    }
    return target;
}

// 0x3acea4 _ZN9Character12CreatePlayerEjjjPKcbb
// Character::CreatePlayer(unsigned int, unsigned int, unsigned int, char const*, bool, bool)
// The first parameter is the savegame slot, used when c is false; c true selects slot -1. The second and third
// parameters are not read by the body. d true skips the idle state. The Spawn argument order is the original's (see
// the other CreatePlayer).
Character* Character::CreatePlayer(unsigned int slot, unsigned int a, unsigned int b, const char* name, bool c, bool d) {
    (void)a;
    (void)b;
    ObjectHandle handle = dword_99F764->Spawn("Character", name, true, true);
    Character* player = handle;
    if (player) {
        if (player->IsPlayer()) {
            player->InitializePlayerSavegame();
        }
        player->SG_SetSlot(c ? 0xffffffffu : slot);
        player->InitPost();
        player->InitFinal();
        if (!d) {
            player->CharStateMachine::SM_SetIdleState(false);
        }
    }
    return player;
}

// 0x3ad064 _ZN9Character9CreateNPCEPKcS1_bb
// Character::CreateNPC(char const*, char const*, bool, bool)
// PARTIAL: the listing passes five arguments to ObjectManager::Spawn (the sixth, silent, is not set), so the silent value
// the original reads is not recoverable. false is passed here. The fourth parameter is not read by the body.
Character* Character::CreateNPC(const char* name, const char* a, bool b, bool c) {
    (void)c;
    static unsigned int counter = 0;  // Character::CreateNPC(char const*,char const*,bool,bool)::counter
    char generated[20];
    if (!a) {
        a = generated;
        std::snprintf(generated, sizeof generated, "NPC_%04u", ++counter);
    }
    ObjectHandle handle = dword_99F764->Spawn("Character", a, true, false);
    Character* npc = handle;
    if (npc) {
        const std::size_t length = std::strlen(name);
        if (b) {
            npc->m_charTemplate.assign(name, length);
        } else {
            npc->m_charPropsName.assign(name, length);
        }
        npc->InitPost();
        npc->InitFinal();
        npc->CharStateMachine::SM_SetIdleState(false);
    }
    return npc;
}

// 0x3ad1f8 _ZN9Character9CreateNPCEiPKcb
// Character::CreateNPC(int, char const*, bool)
// PARTIAL: the original takes the template name from Arrays::CharacterTable::m_memberNames[a] and calls the four-argument
// CreateNPC with false and b. That name table is declared in pydata-accessors/member_names.hpp, which cannot be included
// in the same translation unit as the w7 pydata headers that Character.hpp pulls in (Structs::SoundAutoGen is defined in
// both). The name is not read here, so this stub returns nullptr instead of spawning the NPC.
Character* Character::CreateNPC(int a, const char* name, bool b) {
    (void)a;
    (void)name;
    (void)b;
    return nullptr;
}

// 0x3ad22c _ZN9Character10CSM_AttackEiPviRi
// Character::CSM_Attack(int, void*, int, int&)
// PARTIAL: the original returns !(CharStateMachine +0x2c bit 0), that is !(m_statusFlags & 1). m_statusFlags is private
// in dh2/CharStateMachine.hpp. Stub: true.
bool Character::CSM_Attack(int, void*, int, int&) {
    return true;
}

// 0x3ad23c _ZN9Character11CSM_InjuredEiPviRi
// Character::CSM_Injured(int, void*, int, int&)
bool Character::CSM_Injured(int, void*, int, int&) {
    return true;
}

// 0x3ad244 _ZN9Character15CSM_InterruptedEiPviRi
// Character::CSM_Interrupted(int, void*, int, int&)
// PARTIAL: stateId 6 returns CharStateMachine +0x24 bit 16, stateId 10 returns the inverse of +0x2c bit 5 (m_statusFlags),
// and stateId 5 returns CharAI +0x79 (m_attackLastStep). All three members are private in their headers. Every other id
// returns true, which is the original's default. Stub: true for every id.
bool Character::CSM_Interrupted(int, void*, int, int&) {
    return true;
}

// 0x3ad280 _ZN9Character20CSM_StoppedAttackingEiPviRi
// Character::CSM_StoppedAttacking(int, void*, int, int&)
// PARTIAL: stateId 5 returns CharAI +0x7a (m_attackCommitted), private in dh2/CharAI.hpp. Other ids return false, which is
// reproduced. Stub for stateId 5: false.
bool Character::CSM_StoppedAttacking(int, void*, int, int&) {
    return false;
}

// 0x3ad290 _ZN9Character13CSM_StopSkillEiPviRi
// Character::CSM_StopSkill(int, void*, int, int&)
// PARTIAL: returns CharStateMachine +0x24 bit 15 (m_stateFlags), private in dh2/CharStateMachine.hpp. Stub: false.
bool Character::CSM_StopSkill(int, void*, int, int&) {
    return false;
}

// 0x3ad29c _ZN9Character22CSM_CanStopInteractionEiPviRi
// Character::CSM_CanStopInteraction(int, void*, int, int&)
// PARTIAL: returns 0 when CharStateMachine +0x48 (m_interactKind) is 4 or 5, and 1 otherwise. CSWTCH_5399, the table read
// for those two values, is {0, 0, 0, 0} in the binary (asm chunk_018). m_interactKind is private in
// dh2/CharStateMachine.hpp. Stub: true.
bool Character::CSM_CanStopInteraction(int, void*, int, int&) {
    return true;
}

// 0x3ad2c8 _ZN9Character20CSM_AfterInteractionEiPviRi
// Character::CSM_AfterInteraction(int, void*, int, int&)
// PARTIAL: sets nextState to 18 when CharStateMachine +0x48 (m_interactKind) is 4. m_interactKind is private in
// dh2/CharStateMachine.hpp. Stub: returns true and leaves nextState unchanged.
bool Character::CSM_AfterInteraction(int, void*, int, int&) {
    return true;
}

// 0x3ad2e4 _ZN9Character9CSM_SpawnEiPviRi
// Character::CSM_Spawn(int, void*, int, int&)
// Reproduced: stateId 0 returns CanRespawn(), and every other state except 17 returns true.
// PARTIAL: stateId 17 (PreSpawn) asks the group CharAI::m_group (+0x34) with CharAI::m_character (+0x04), and falls back
// to the auto-spawn property when there is no group. Both are private in dh2/CharAI.hpp. Stub for 17: false.
bool Character::CSM_Spawn(int, void*, int stateId, int&) {
    if (stateId == 0) {
        return CanRespawn();
    }
    if (stateId != 17) {
        return true;
    }
    return false;
}

// 0x3ad418 _ZN9Character13Ctrl_RotateByEf
// Character::Ctrl_RotateBy(float)
// The original also returns the new target yaw as a float. The base slot (v2Controllable::Ctrl_RotateBy) is void, so
// nothing is returned here.
void Character::Ctrl_RotateBy(float angle) {
    m_targetYaw = m_targetYaw + angle;
}

// 0x3ad430 _ZNK9Character13CTRLIsAllowedEv
// Character::CTRLIsAllowed() const
bool Character::CTRLIsAllowed() const {
    return !SM_IsScared(true) && !SM_IsStunned(true);
}

// 0x3ad478 _ZN9Character12Ctrl_OpenIGMEv
// Character::Ctrl_OpenIGM()
// When the in-game menu is open, pops the menu stack to menu_Ingame and then pops the menu; otherwise pushes menu_Ingame.
// The menu manager's OnFSCommand is its vtable slot +0x10.
void Character::Ctrl_OpenIGM() {
    if (MenuBase::s_igmOpened) {
        MenuManager::GetInstance()->OnFSCommand("PopAllAbove", "menu_Ingame");
        MenuManager::GetInstance()->OnFSCommand("PopMenu", "");
    } else {
        MenuManager::GetInstance()->OnFSCommand("PushMenu", "menu_Ingame");
    }
}

// 0x3ad528 _ZN9Character9Ctrl_KillEP10GameObjectb
// Character::Ctrl_Kill(GameObject*, bool)
void Character::Ctrl_Kill(GameObject* killer, bool silent) {
    if (!IsDead()) {
        Kill(killer, silent);
        RaiseEvent(2, killer);
    }
}

// 0x3ad57c _ZN9Character15Ctrl_DropObjectEv
// Character::Ctrl_DropObject()
void Character::Ctrl_DropObject() {
    if (!IsDead()) {
        RaiseEvent(6, nullptr);
    }
}

// 0x3ad5ac _ZN9Character11ForceUseOOIEP10GameObject
// Character::ForceUseOOI(GameObject*)
// PARTIAL: with a target, the original does nothing when CharAI +0x40 (m_target) is set. Otherwise, when the state machine
// is idle or moving, it calls CharAI::AI_SetTarget(target, false) and sets CharAI +0x4a (m_targetSeeking) to true. Both
// members are private in dh2/CharAI.hpp, so only the null check is reproduced.
void Character::ForceUseOOI(GameObject* target) {
    if (target) {
        // PARTIAL: the m_target check, the idle or moving check, AI_SetTarget and the m_targetSeeking write (see above).
    }
}

// 0x3ad614 _ZN9Character6UseOOIEv
// Character::UseOOI()
// PARTIAL: the same as ForceUseOOI, with m_objectOfInterest as the target. Only the null check is reproduced; the
// m_target check, the idle or moving check, AI_SetTarget and the m_targetSeeking write are not (see ForceUseOOI).
void Character::UseOOI() {
    if (m_objectOfInterest) {
        // PARTIAL (see ForceUseOOI).
    }
}
