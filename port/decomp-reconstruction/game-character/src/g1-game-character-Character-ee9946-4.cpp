// Batch g1-game-character-Character-ee9946-4: the model and class name queries, the foot positions, the initial position,
// the enable and disable hooks, the potion query, the revive, the loot drop, the kill, the serialization pair, the cleanup,
// the replicated-state constructor and its destructors, the concurrent-AI map clear and the net-struct population.
// Declarations come from dh2/Character.hpp. PARTIAL comments name what is missing; the ledger rows in
// game-character/ledger.d/g1-game-character-Character-ee9946-4.csv repeat them.

#include <cstdint>
#include <map>
#include <string>

#include "core-platform/device.hpp"
#include "dh2/Character.hpp"
#include "dh2/ItemObject.hpp"
#include "dh2/PFWorld.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/VisualFXManager.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "dh2/global/item_instance.hpp"
#include "glitch-scene-collada-gui/scene_node.hpp"
#include "pydata-accessors/w7_arrays27.hpp"

namespace {

// PARTIAL placeholder for the name pointer (+8) of Arrays::ModelDict row `row` (12-byte records). The pydata row type is
// not in this translation unit (w7_arrays_00.hpp cannot be included next to the other pydata headers), so the name is nullptr.
const char* ModelDictName(int row) {
    (void)row;
    return nullptr;
}

}  // namespace

// 0x3a54d4 Character::GetCharModelName() const
// PARTIAL: the branch structure is reproduced. Every row read goes through ModelDictName, which is a placeholder. The
// faerie branch needs CharAI::m_master (private, CharAI +0x50) and the faery record (+12, no typed Structs::Faery); the master
// is taken as absent, which keeps the model id. PlayerManager::IsLocalPlayer(this) (dword_99F76C) is taken false because
// PlayerManager.hpp cannot be included here. The bound check against Arrays::ModelDict::size is not made.
const char* Character::GetCharModelName() const {
    const int charModelId = GetCharModelId();
    if (charModelId == -1) {
        return nullptr;
    }
    if (IsFaerie()) {
        // PARTIAL: the master's current faery model id (via CharAI::m_master) is not read; the model id is used.
        return ModelDictName(charModelId);
    }
    // The vtable call at +0x28 is IsPlayer(). Hex-Rays typed its result as Device*, which is a misreading of a bool.
    if (!IsPlayer() || Device::IsHighPerformance() || /* PARTIAL: PlayerManager::IsLocalPlayer(this) */ false) {
        return ModelDictName(charModelId);
    }
    // A player on a low-performance device, not the local one, uses a fixed model row chosen by m_charPropsId (+0x13c8).
    const int propsId = m_charPropsId;
    if (propsId >= 290 && propsId <= 292) {
        return ModelDictName(75);
    }
    if (propsId >= 325 && propsId <= 327) {
        return ModelDictName(76);
    }
    if (propsId >= 263 && propsId <= 265) {
        return ModelDictName(77);
    }
    // The original asserts here (Character.cpp:1191) and uses the model id row.
    return ModelDictName(charModelId);
}

// 0x3a56a4 Character::GetClassName() const
// The string id is word 6 of the sheet (m_props.m_w6, Character +0x100c) when the player class is -1. Otherwise it is word 6
// (+24) of the Arrays::CharacterTable row for the player class, read as the host element (the 900-byte file stride is not
// applied, the same convention as GetCharAI).
const char* Character::GetClassName() const {
    StringManager* strings = ApplicationGetStringManager();
    const int playerClass = SG_GetPlayerClass();
    if (playerClass == -1) {
        return strings->getString(m_props.m_w6);
    }
    const auto* rows = reinterpret_cast<const Structs::CharacterProperties*>(Arrays::CharacterTable::members);
    return strings->getString(rows[playerClass].m_w6);
}

// 0x3a5784 Character::GetPreSetAIState() const
// PARTIAL: the original's first test compares the two words at +0x13dc and +0x13e0, which are the last 8 bytes of the 24-byte
// m_aiState string under the 32-bit layout. It is read here as "the string is empty", which the host std::string does not
// reproduce word for word. The Limbus and PreSpawn comparisons match the original.
int Character::GetPreSetAIState() const {
    if (m_aiState.empty()) {
        return 3;
    }
    if (m_aiState.compare("Limbus") == 0) {
        return 0;
    }
    if (m_aiState.compare("PreSpawn") != 0) {
        return 3;
    }
    return 17;
}

// 0x3a57f4 Character::GetLeftFootPosition() const
// The original copies the target position, then replaces it with the absolute position of the "Bip01_L_Foot" scene node
// when the visual object has one.
Point3D<float> Character::GetLeftFootPosition() const {
    Point3D<float> position = GetTargetPosition();
    if (m_visualObject != nullptr) {
        glitch::scene::ISceneNode* node = m_visualObject->GetSpecificNode("Bip01_L_Foot");
        if (node != nullptr) {
            const glitch::core::vector3d<float> absolute = node->getAbsolutePosition();
            position = Point3D<float>{absolute.X, absolute.Y, absolute.Z};
        }
    }
    return position;
}

// 0x3a5874 Character::GetRightFootPosition() const
// Same as GetLeftFootPosition with the "Bip01_R_Foot" scene node.
Point3D<float> Character::GetRightFootPosition() const {
    Point3D<float> position = GetTargetPosition();
    if (m_visualObject != nullptr) {
        glitch::scene::ISceneNode* node = m_visualObject->GetSpecificNode("Bip01_R_Foot");
        if (node != nullptr) {
            const glitch::core::vector3d<float> absolute = node->getAbsolutePosition();
            position = Point3D<float>{absolute.X, absolute.Y, absolute.Z};
        }
    }
    return position;
}

// 0x3a58f4 Character::SetInitialPosition(Point3D<float> const&)
// Stores the position in m_initialPosition (+0x1450). When PFWorld::GetFloorHeightAt finds a floor under it, the floor
// height becomes z (+0x1458). The original's int result is dropped (the header returns void).
void Character::SetInitialPosition(const Point3D<float>& pos) {
    m_initialPosition = pos;
    float floorHeight = 0.0f;
    if (Singleton<PFWorld>::s_inst.GetFloorHeightAt(m_initialPosition, &floorHeight, nullptr, nullptr, nullptr, false)) {
        m_initialPosition.Z = floorHeight;
    }
}

// 0x3a5974 Character::Disabled()
void Character::Disabled() {
    GameObject::Disabled();
    DisableSelfFX();
}

// 0x3a598c Character::Enabled()
// Forward thunk: the original tail-calls GameObject::Enabled.
void Character::Enabled() {
    GameObject::Enabled();
}

// 0x3a5990 Character::HasPotion() const
bool Character::HasPotion() const {
    return ItemInventory::GetNumPotions() > 0;
}

// 0x3a59ac Character::Revive(GameObject*, bool)
// PARTIAL: reproduced: the death event (event 3) when dead, the low-HP sound re-arm, the dead flag, the HP/MP reset, the
// unk_118 clear, the physics reset when requested, and the AI skill update. Not made:
//  - the online block (m_netOwnerId = -1 and unk_114 = 0 when COnline::isOnlineGame_ is set): the flag at COnline +0x05
//    is private with no getter;
//  - the offline player's re-placement: the three words at Character +0x143c, +0x1440 and +0x1444 are read by the original as
//    x, y and z and floor-corrected by PFWorld::GetFloorHeightAt (Singleton<PFWorld>::s_inst is declared now), then
//    GameObject::SetPosition(pos, true) is called. The header names those words m_spawnViewRadius and unk_1440/unk_1444,
//    but their meaning as a position is not confirmed, so the placement is not made.
//    Layout request: name the words at Character +0x143c..+0x1444 (x, y, z of the revive placement).
// The reviver argument is not read by the original body.
void Character::Revive(GameObject* reviver, bool resetPhysics) {
    (void)reviver;
    if (m_dead != 0) {
        RaiseEvent(3, nullptr);
    }
    m_lowHpSoundArmed = 1;
    m_dead = 0;
    _InitHpMp();
    unk_118 = 0;
    if (resetPhysics) {
        InitPhysicalObject();
    }
    if (IsPlayer()) {
        // PARTIAL: the offline player's position restore (see the note above) is not made.
    }
    CharAI::UpdateAllSkills();
}

// 0x3a5ae4 Character::DropLoot(GameObject*) const
// The original passes the loot id, this as the dropper, the killer, -1 as the fixed power count and 0 as the flag. Its sixth
// argument word is an unset stack local that the five-argument callee never reads. The two DropLootTable overloads share the
// same argument registers, so the one taking an int fixed power count is used.
void Character::DropLoot(GameObject* killer) const {
    ItemObject::DropLootTable(GetLoot(), this, killer, -1, false);
}

// 0x3a5b18 Character::Kill(GameObject*, bool)
// PARTIAL. Reproduced: the already-dead guard, the dead flag, the death counter for a player that dies unsilenced, the loot
// drop, the killer assignment, the aggro loop (object-of-interest clearing, the two kill counters and event 4 to each aggro
// character), and the guard for the quest events (IsRemotelyUpdated and m_isNetSpawned). Not made:
//  - the loot drop is made unconditionally: the original makes it only when the level's unk_150 (Level +0x150) is null, and
//    Level.hpp cannot be included in this translation unit (it conflicts with the pydata IStreamBase); null is assumed;
//  - the local player's death and kill trophies (PlayerManager::IsLocalPlayer and PlayerManager::GetLocalPlayer are not
//    reachable; TrophyManager::UnlockTrophy with the quest_died / killed_N trophy ids);
//  - the online branch of a player's death (COnline::isOnlineGame_ is private);
//  - the aggro redirect through Character +0x14d4 (an unnamed int32 read as a Character*): it is treated as absent;
//  - the experience share, Character::DistributeXP, which the pseudocode calls with an unset third argument and the killer's
//    object handle;
//  - the quest objective events (EventManager::RaiseAsync of KillXEnemies, ClearEnemies, KillEnemyTemplate and
//    ClearEnemyTemplate, with PyDataConstants "v2QuestObjectiveType" names and the IEvent vtables off_969A18, off_95B458,
//    off_969A00, off_9699E8 and off_9699D0). Their objective data reads ObjectBase unk_64 (+0x64).
//  - the level-null assert.
void Character::Kill(GameObject* killer, bool silent) {
    if (IsDead()) {
        return;
    }
    m_dead = 1;
    PROPS_Set(36, 0);
    if (IsPlayer() && !silent) {
        PROPS_AddInt(25, 1);
        // PARTIAL: the local player's death trophies are not made (see the note above).
        return;
    }
    DropLoot(killer);
    bool killerIsSelf = killer == this;
    if (killer != nullptr) {
        if (silent) {
            return;
        }
        m_killer = killer;
        for (int i = 0; i < AI_GetAggroCount(); ++i) {
            Character* entry = nullptr;
            float aggro = 0.0f;
            AI_GetAggroEntry(i, entry, aggro);
            if (entry == nullptr) {
                continue;
            }
            // PARTIAL: the redirect through Character +0x14d4 is treated as absent (see the note above).
            entry->RaiseEvent(4, this);
            if (!entry->IsPlayer()) {
                continue;
            }
            if (entry->m_objectOfInterest == this) {
                entry->m_objectOfInterest = nullptr;
            }
            entry->PROPS_AddInt(23, 1);
            entry->PROPS_AddInt(24, 1);
            if (killer == entry) {
                killerIsSelf = true;
            }
            // PARTIAL: the trophy block for the entry when it is the local player is not made.
        }
        if (killerIsSelf) {
            // PARTIAL: Character::DistributeXP is not called (see the note above).
        }
    } else if (silent) {
        return;
    }
    if (IsRemotelyUpdated() || m_isNetSpawned != 0) {
        return;
    }
    // PARTIAL: the quest objective events are not raised (see the note above).
}

// 0x3a6210 Character::Deserialize(IStreamBase*)
// PARTIAL: only the buff reset and the base GameObject::Deserialize are reproduced. Not made, in stream order:
//  - the monster skip for a level entered from another level (Level +0xf3, m_cameFromOtherLevel; Level.hpp cannot be included);
//  - the state int, the position, the rotation, the dead flag, the saved point and the revive position. The Point3D and bool
//    reads are the dh2 IStreamBase::readAs members, and the dh2 IStreamBase.hpp conflicts with the pydata IStreamBase that this
//    translation unit sees (it has only ReadBytes);
//  - the two CharacterProperties sheets for a non-player (m_savedProps and m_props are private to CharProperties);
//  - the CharAI group record (CharAI m_group is private) when the group is set;
//  - after the reads: the dead-flag transition (Revive, SM_SetIdleState, the controller byte at v2Controllable +0x378 +8),
//    AI_ScriptCleanUp and AI_ScriptInit, the state-driven limbus and pre-spawn branches, AI_SetTarget and AI_SyncLastTarget,
//    the SetPosition and SetRotation placement (the pseudocode drops the SetPosition arguments), VisualObject::Sync and the
//    cameraAnchor Reset (AnchorBase vtable slot +0x08).
// Do not use this body to load a save until the rest is written. Every later read is needed to keep the stream aligned.
void Character::Deserialize(IStreamBase* stream) {
    PROPS_RemoveAllBuffs();
    GameObject::Deserialize(stream);
}

// 0x3a65c4 Character::Serialize(IStreamBase*)
// PARTIAL: only GameObject::Serialize is reproduced. The rest of the record (the state int, the two sheets for a non-player,
// the position, rotation, dead flag, saved point, revive position and the CharAI group record) needs the dh2 IStreamBase
// readAs and writeAs members, which cannot be included in this translation unit (see Deserialize). Do not use this body to
// save a character until the rest is written.
void Character::Serialize(IStreamBase* stream) {
    GameObject::Serialize(stream);
}

// 0x3a66f8 Character::Clean()
// PARTIAL. Reproduced in the original order: the DropAnimatedFX calls for m_selfFX and m_stateFX, the nine m_targetCircleFX
// entries (then the array is freed and cleared), DropAnimatedFX for m_cursorFX, RemoveMultiplayerHighlight, the free of the
// timer array (m_timerUtil, not cleared, as in the original), CharAnimator::DecAnimSetUsers when CharAnimator m_usersAdded
// (+0x5c, the byte at Character +0x4f8) is set, and the delete of m_savegame (PlayerSavegame has a virtual destructor).
// VisualFXManager::s_inst is declared in dh2/VisualFXManager.hpp and is defined by the integrator.
// Not made: the release of unk_14ec through its vtable slot +0x1c. The field is void* and its owner type is not declared.
void Character::Clean() {
    VisualFXManager& fx = VisualFXManager::s_inst;
    fx.DropAnimatedFX(m_selfFX);
    fx.DropAnimatedFX(m_stateFX);
    if (m_targetCircleFX != nullptr) {
        for (int i = 0; i < 9; ++i) {
            fx.DropAnimatedFX(m_targetCircleFX[i]);
        }
        CustomFree(m_targetCircleFX);
        m_targetCircleFX = nullptr;
    }
    fx.DropAnimatedFX(m_cursorFX);
    RemoveMultiplayerHighlight();
    if (m_timerUtil != nullptr) {
        CustomFree(m_timerUtil);
    }
    if (CharAnimator::m_usersAdded) {
        CharAnimator::DecAnimSetUsers();
    }
    if (m_savegame != nullptr) {
        delete m_savegame;
        m_savegame = nullptr;
    }
    // PARTIAL: the release of unk_14ec through its vtable slot +0x1c is not reproduced (see the note above).
}

// 0x3a6a24 Character::NetStructCharacter::NetStructCharacter()
// PARTIAL: the constructor of the replicated state. The original declares eleven members in this order (ids 0 to 10): three
// NetStructInterpolatedRangedFloat position members (+0x130, +0x228, +0x320), dirX, dirY, hp, killer id, state, enabled,
// killer-is-player and visible. The position members are opaque in the header (not NetStructMember types), so DeclareMember
// cannot be called in the original order, and the member ids would be wrong. The per-member change-flag setup (SetChanged on
// the non-zero starting values) is not reproduced. Layout request: type the three position members as
// NetStructInterpolatedRangedFloat so that the eleven DeclareMember calls can follow the original order.
Character::NetStructCharacter::NetStructCharacter() {
}

// 0x3a799c Character::ClearConcurrentAI()
// The original erases the whole s_concurrentAI tree and resets its header; std::map::clear does the same.
void Character::ClearConcurrentAI() {
    s_concurrentAI.clear();
}

// 0x3a7a50 Character::NetStructCharacter::~NetStructCharacter()
// The original resets the vtables of the members and of the NetStruct base, then erases the packet history map (the
// std::map at NetStruct +0x10c). The compiler generates the same member and base destruction for this empty body.
// 0x3a7b08 (the deleting destructor: this destructor, then CustomFree(this)) is compiler generated and not rebuilt.
Character::NetStructCharacter::~NetStructCharacter() {
}

// 0x3a6f0c Character::PopulateOutgoingNetStruct(bool)
// PARTIAL: a stub. The original writes each replicated member of m_netOut (+0x1508) from the live character: the three
// positions from m_position (+0x160), the direction members from m_heading (+0x1b8, +0x1bc) when m_hasHeading (+0x1b5) is
// set and the local player is not paused (zero otherwise), hp (zero when the m_props word 37 is <= 0 or the character is
// dead; the HP word is unnamed otherwise), the killer net id (m_props word 42, zero when <= 0; the killer's net id at +0x108 is unnamed), the state
// (SM_GetState when force is set, else -1), the enabled and visible flags (ObjectBase +0x8a and +0x80, written when force is
// set) and killer-is-player (m_killer->IsCharacter() && IsPlayer()). Each write is a temporary member compare followed by a
// vtable SetValue on the real member, and the temporary's change flag does not reach the real member through the header's
// NetStructMemberType API. None of the writes is made here. Also blocked: IsLocalPlayer (PlayerManager.hpp) and the unnamed
// words above. Layout requests: name the ObjectBase flags at +0x80 and +0x8a in the header; name the killer net id source.
void Character::PopulateOutgoingNetStruct(bool force) {
    (void)force;
}
