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
// The second bool of ObjectManager::Spawn is the fourth parameter c. The Hex-Rays listing drops that sixth argument; the
// assembly at 0x3ad064 stores the register that held c at [sp, #4] of the Spawn call (the first bool is the constant 1).
// b selects m_charTemplate (true) or m_charPropsName (false).
Character* Character::CreateNPC(const char* name, const char* a, bool b, bool c) {
    static unsigned int counter = 0;  // Character::CreateNPC(char const*,char const*,bool,bool)::counter
    char generated[20];
    if (!a) {
        a = generated;
        std::snprintf(generated, sizeof generated, "NPC_%04u", ++counter);
    }
    ObjectHandle handle = dword_99F764->Spawn("Character", a, true, c);
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
// Returns !(CharStateMachine m_statusFlags & 1). The listing reads the byte at Character +0x528, which is +0x2c of the
// CharStateMachine base, so the low byte of the word is the bit tested here.
bool Character::CSM_Attack(int, void*, int, int&) {
    return !(CharStateMachine::m_statusFlags & 1u);
}

// 0x3ad23c _ZN9Character11CSM_InjuredEiPviRi
// Character::CSM_Injured(int, void*, int, int&)
bool Character::CSM_Injured(int, void*, int, int&) {
    return true;
}

// 0x3ad244 _ZN9Character15CSM_InterruptedEiPviRi
// Character::CSM_Interrupted(int, void*, int, int&)
// Switch on stateId (the third parameter): 6 returns bit 16 of CharStateMachine m_stateFlags (HIWORD & 1); 10 returns the
// inverse of bit 5 of m_statusFlags; 5 returns CharAI m_attackLastStep. Every other id returns true.
bool Character::CSM_Interrupted(int, void*, int stateId, int&) {
    switch (stateId) {
        case 6:
            return ((CharStateMachine::m_stateFlags >> 16) & 1u) != 0;
        case 10:
            return (((CharStateMachine::m_statusFlags ^ 0x20u) >> 5) & 1u) != 0;
        case 5:
            return CharAI::m_attackLastStep;
        default:
            return true;
    }
}

// 0x3ad280 _ZN9Character20CSM_StoppedAttackingEiPviRi
// Character::CSM_StoppedAttacking(int, void*, int, int&)
// stateId 5 returns CharAI m_attackCommitted; every other id returns false.
bool Character::CSM_StoppedAttacking(int, void*, int stateId, int&) {
    if (stateId == 5) {
        return CharAI::m_attackCommitted;
    }
    return false;
}

// 0x3ad290 _ZN9Character13CSM_StopSkillEiPviRi
// Character::CSM_StopSkill(int, void*, int, int&)
// Returns bit 15 of CharStateMachine m_stateFlags.
bool Character::CSM_StopSkill(int, void*, int, int&) {
    return ((CharStateMachine::m_stateFlags >> 15) & 1u) != 0;
}

// 0x3ad29c _ZN9Character22CSM_CanStopInteractionEiPviRi
// Character::CSM_CanStopInteraction(int, void*, int, int&)
// When CharStateMachine::m_interactKind (+0x48) is 4 or 5, the result is CSWTCH_5399[kind - 4]; otherwise 1. The table
// is four bytes at 0x8c3784, all zero (IDA get_bytes, 2026-10-13), so the result is 0 for kinds 4 and 5.
bool Character::CSM_CanStopInteraction(int, void*, int, int&) {
    static constexpr std::uint8_t kCSWTCH_5399[4] = {0, 0, 0, 0};
    const unsigned int kind = static_cast<unsigned int>(CharStateMachine::m_interactKind);
    if (kind - 4u <= 1u) {
        return kCSWTCH_5399[kind - 4u] != 0;
    }
    return true;
}

// 0x3ad2c8 _ZN9Character20CSM_AfterInteractionEiPviRi
// Character::CSM_AfterInteraction(int, void*, int, int&)
// nextState becomes 18 when m_interactKind (+0x48) is 4. The result is 1 in every case.
bool Character::CSM_AfterInteraction(int, void*, int, int& nextState) {
    if (CharStateMachine::m_interactKind == 4) {
        nextState = 18;
    }
    return true;
}

// 0x3ad2e4 _ZN9Character9CSM_SpawnEiPviRi
// Character::CSM_Spawn(int, void*, int, int&)
// stateId 0 returns CanRespawn(). Any state other than 17 returns true. For 17 (PreSpawn): with no group in CharAI m_group
// (+0x34) the result is the m_autoSpawn property (+0x1430). With a group, GroupInfo::CanSpawn(CharAI m_character) decides;
// when it allows the spawn the result is m_autoSpawn, otherwise 0.
bool Character::CSM_Spawn(int, void*, int stateId, int&) {
    if (stateId == 0) {
        return CanRespawn();
    }
    if (stateId != 17) {
        return true;
    }
    CharAI::GroupInfo* group = CharAI::m_group;
    if (!group) {
        return m_autoSpawn;
    }
    if (group->CanSpawn(CharAI::m_character)) {
        return m_autoSpawn;
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
// With a target, and only when CharAI m_target (+0x40) is clear and the state machine is idle (SM_IsIdle(false)) or
// moving (SM_IsMoving(false)): CharAI::AI_SetTarget(target, false) and CharAI m_targetSeeking (+0x4a) set to true.
void Character::ForceUseOOI(GameObject* target) {
    if (target && !CharAI::m_target) {
        if (SM_IsIdle(false) || SM_IsMoving(false)) {
            CharAI::AI_SetTarget(target, false);
            CharAI::m_targetSeeking = true;
        }
    }
}

// 0x3ad614 _ZN9Character6UseOOIEv
// Character::UseOOI()
// The same as ForceUseOOI, with m_objectOfInterest (+0x14a4) as the target.
void Character::UseOOI() {
    if (m_objectOfInterest && !CharAI::m_target) {
        if (SM_IsIdle(false) || SM_IsMoving(false)) {
            CharAI::AI_SetTarget(m_objectOfInterest, false);
            CharAI::m_targetSeeking = true;
        }
    }
}
