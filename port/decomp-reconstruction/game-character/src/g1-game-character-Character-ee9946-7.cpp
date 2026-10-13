// Batch g1-game-character-Character-ee9946-7 (part 8 of 20 of Character): the Ctrl_* controller entry points (the
// primary vtable slots +0x34 through +0x11c, and the GameObject-side head, move and warp commands), the click handler
// Ctrl_Click, and the faery list accessors GetCharFaeryListId and GetCharFaeryList.
// Declarations come from dh2/Character.hpp, which this batch does not change. Several bodies are PARTIAL because a
// member or instance they read is not declared (see the comment above each one); the ledger rows in
// game-character/ledger.d/g1-game-character-Character-ee9946-7.csv repeat what is missing.
//
// Virtual calls: a Ctrl_* body that the original reaches through the vtable (for example "*(this + 0x54)" for
// IsRemotelyUpdated, or "*(this + 0xec)" for Ctrl_MoveTo) is written as an unqualified call on this, which dispatches
// the same way. A call the original makes non-virtually (GameObject::Stop, GameObject::PathTo, CharAI::AI_SetTarget)
// is written qualified.
//
// The base slots are void (v2Controllable). Where the original returns a value that the slot discards (the bool from
// AI_BeginSpell, the IsDead result of Ctrl_UseOOI, the RaiseEvent result of Ctrl_Stop), the body does not return it.

#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"  // dword_99F764, the ObjectManager instance
#include "dh2/global/Vec3f_Origin.hpp"
#include "dh2/global/ctrl_camera.hpp"            // ApplicationGetInstance
#include "pydata-accessors/structs_records.hpp"  // Structs::DesignSettings, Structs::FaeryList
#include "pydata-accessors/w7_arrays27.hpp"      // Arrays::FaeryListTable
#include "pydata-accessors/w7_arrays_02.hpp"     // Arrays::DesignSettingsTable

namespace {

// The vector from the first point to the second, componentwise (the Hex-Rays "v10 = target - self" sequences).
Point3D<float> Between(const Point3D<float>& from, const Point3D<float>& to) {
    return Point3D<float>{to.X - from.X, to.Y - from.Y, to.Z - from.Z};
}

}  // namespace

// 0x3ad690 _ZN9Character11Ctrl_UseOOIEP10GameObject
// Character::Ctrl_UseOOI(GameObject*)
// Does nothing while the character is dead (IsDead, vtable slot +0x34). With a target it calls ForceUseOOI; without one
// it calls UseOOI when m_objectOfInterest is set. The original returns the IsDead value or the callee's result; the
// void slot does not, so nothing is returned here.
void Character::Ctrl_UseOOI(GameObject* target) {
    if (IsDead()) {
        return;
    }
    if (target) {
        ForceUseOOI(target);
        return;
    }
    if (m_objectOfInterest) {
        UseOOI();
    }
}

// 0x3ad6f0 _ZN9Character14Ctrl_UsePotionEv
// Character::Ctrl_UsePotion()
// PARTIAL: the trophy check is not run. After the potion is removed and the potion counter (property 219) is raised for
// a player, the original unlocks "use_100_potions" in TrophyManager::s_instance once the counter passes 99, but only
// when PlayerManager::IsLocalPlayer(dword_99F76C, this) is true. dh2/PlayerManager.hpp cannot be included beside
// dh2/Character.hpp (its net-transport StreamBuffer clashes with the pydata IStreamBase), so the local-player gate is
// not evaluated and the unlock is skipped. The index lookup in Arrays::TrophyTable is not written either.
// Everything else is reproduced: the dead and HasPotion gates, RemoveOnePotion, the counter, and the two regenerations.
void Character::Ctrl_UsePotion() {
    if (IsDead()) {
        return;
    }
    if (!HasPotion()) {
        return;
    }
    ItemInventory::RemoveOnePotion();
    if (IsPlayer()) {
        PROPS_AddInt(219, 1);
        // PARTIAL: if (PlayerManager::IsLocalPlayer(dword_99F76C, this) && PROPS_GetInt(219, false) > 99)
        //              TrophyManager::s_instance->UnlockTrophy(index of "use_100_potions" in Arrays::TrophyTable)
    }
    RegenHP(-1);
    RegenMP(-1);
}

// 0x3ad83c _ZN9Character12Ctrl_EndCastEb
// Character::Ctrl_EndCast(bool)
void Character::Ctrl_EndCast(bool primary) {
    CharAI::AI_EndSpell(primary);
}

// 0x3ad84c _ZN9Character14Ctrl_BeginCastEb
// Character::Ctrl_BeginCast(bool)
// The original returns the bool from CharAI::AI_BeginSpell; the void slot discards it.
void Character::Ctrl_BeginCast(bool primary) {
    CharAI::AI_BeginSpell(primary);
}

// 0x3ad85c _ZN9Character13Ctrl_EndSkillEj
// Character::Ctrl_EndSkill(unsigned int)
void Character::Ctrl_EndSkill(unsigned int skill) {
    CharAI::AI_EndSkill(skill);
}

// 0x3ad86c _ZN9Character15Ctrl_BeginSkillEj
// Character::Ctrl_BeginSkill(unsigned int)
// The original returns the bool from CharAI::AI_BeginSkill; the void slot discards it.
void Character::Ctrl_BeginSkill(unsigned int skill) {
    CharAI::AI_BeginSkill(skill);
}

// 0x3ad87c _ZN9Character11Ctrl_AttackEP10GameObject
// Character::Ctrl_Attack(GameObject*)
void Character::Ctrl_Attack(GameObject* target) {
    CharAI::AI_DoMeleeAttack(target, false);
}

// 0x3ad890 _ZN9Character9Ctrl_StopEv
// Character::Ctrl_Stop()
// Does nothing when the character is remotely updated (IsRemotelyUpdated, vtable slot +0x54). Otherwise it stops the
// GameObject and raises event 63. The original returns the RaiseEvent result; the void slot discards it.
void Character::Ctrl_Stop() {
    if (IsRemotelyUpdated()) {
        return;
    }
    GameObject::Stop();
    RaiseEvent(63, nullptr);
}

// 0x3ad8d4 _ZN9Character16Ctrl_HeadTowardsEP10GameObject
// Character::Ctrl_HeadTowards(GameObject*)
// With a target, turns toward the target: the direction is the target's position minus this object's, passed to the
// Point3D overload (vtable slot +0xdc). Without a target and with the heading flag set (GameObject m_hasHeading, +0x1b5),
// it heads toward the zero vector.
void Character::Ctrl_HeadTowards(GameObject* target) {
    if (IsRemotelyUpdated()) {
        return;
    }
    if (target) {
        const Point3D<float> direction = Between(GameObject::GetTargetPosition(), target->GetTargetPosition());
        Ctrl_HeadTowards(direction);
    } else if (m_hasHeading) {
        Ctrl_HeadTowards(Vec3f_Origin);
    }
}

// 0x3ad9ac _ZN9Character11Ctrl_LookAtEP10GameObject
// Character::Ctrl_LookAt(GameObject*)
// Looks at the target's position through the Point3D overload (vtable slot +0xd0) when a target is given. The original
// returns this when there is none; the void slot does not return a value.
void Character::Ctrl_LookAt(GameObject* target) {
    if (target) {
        Ctrl_LookAt(target->GetTargetPosition());
    }
}

// 0x3ad9e4 _ZN9Character11Ctrl_MoveToEP10GameObject
// Character::Ctrl_MoveTo(GameObject*)
// Does nothing when the character is remotely updated. With a target it paths to the target's position.
void Character::Ctrl_MoveTo(GameObject* target) {
    if (IsRemotelyUpdated()) {
        return;
    }
    if (target) {
        GameObject::PathTo(target->GetTargetPosition());
    }
}

// 0x3ada30 _ZN9Character11Ctrl_MoveToERK7Point3DIfE
// Character::Ctrl_MoveTo(Point3D<float> const&)
void Character::Ctrl_MoveTo(const Point3D<float>& pos) {
    if (IsRemotelyUpdated()) {
        return;
    }
    GameObject::PathTo(pos);
}

// 0x3ada6c _ZN9Character11Ctrl_HeadToEP10GameObject
// Character::Ctrl_HeadTo(GameObject*)
// Sets the destination to the target's position and raises event 0 with the target as its data.
void Character::Ctrl_HeadTo(GameObject* target) {
    if (IsRemotelyUpdated()) {
        return;
    }
    if (target) {
        GameObject::SetDestination(target->GetTargetPosition());
        RaiseEvent(0, target);
    }
}

// 0x3adac8 _ZN9Character11Ctrl_HeadToERK7Point3DIfE
// Character::Ctrl_HeadTo(Point3D<float> const&)
// Turns toward the point (the direction is the point minus m_position, +0x160, with heading), sets it as the
// destination and raises event 0 with no data.
void Character::Ctrl_HeadTo(const Point3D<float>& pos) {
    if (IsRemotelyUpdated()) {
        return;
    }
    const Point3D<float> direction = Between(m_position, pos);
    GameObject::SetHeadingDirection(direction, true);
    GameObject::SetDestination(pos);
    RaiseEvent(0, nullptr);
}

// 0x3adb60 _ZN9Character16Ctrl_HeadTowardsERK7Point3DIfE
// Character::Ctrl_HeadTowards(Point3D<float> const&)
// PARTIAL: when the direction is longer than sqrt(0.0001) and the character is not using a skill and not casting, the
// original reads CharAI::m_target (+0x40) and, when it is set and the direction is more than 1.5708 radians from the
// direction to that target (Point3D::angle, 0x313058), clears the target with CharAI::AI_SetTarget(nullptr, false) and
// syncs it with CharAI::AI_SyncLastTarget(). m_target is private in dh2/CharAI.hpp and Point3D::angle is declared only in
// dh2/Point3D.hpp, which cannot be included beside dh2/Character.hpp, so that test is not run. The heading and event
// 0 are reproduced, as is the near-zero branch.
void Character::Ctrl_HeadTowards(const Point3D<float>& pos) {
    const float lengthSquared = (pos.X * pos.X + pos.Y * pos.Y) + pos.Z * pos.Z;
    if (lengthSquared > 0.0001f) {
        if (SM_IsUsingSkill() || SM_IsCasting()) {
            return;
        }
        // PARTIAL: the m_target and angle test (see above) is not run.
        GameObject::SetHeadingDirection(pos, true);
        RaiseEvent(0, nullptr);
    } else if (m_hasHeading) {
        GameObject::SetHeadingDirection(pos, true);
        Ctrl_Stop();
    }
}

// 0x3adcf4 _ZN9Character11Ctrl_WarpToERK7Point3DIfE
// Character::Ctrl_WarpTo(Point3D<float> const&)
// PARTIAL: the original validates the point with PFWorld::ValidatePosition(Singleton<PFWorld>::s_inst, point, &m_pfObject).
// No PFWorld instance is declared (Singleton<PFWorld>::s_inst has no accessor in game/include), so the validation cannot be
// called. When the point is valid the original calls GameObject::SetPosition(point, true), stores the validated point in
// m_pfObject.m_pos (PFObject +0x18, the words at GameObject +0x1e0..+0x1e8), calls Ctrl_Stop and raises event 1. When it is
// not valid it does nothing. Neither branch runs here. Only the remotely-updated gate is reproduced.
void Character::Ctrl_WarpTo(const Point3D<float>& pos) {
    if (IsRemotelyUpdated()) {
        return;
    }
    (void)pos;
}

// 0x3addbc _ZNK9Character11Ctrl_LookAtERK7Point3DIfE
// Character::Ctrl_LookAt(Point3D<float> const&)
// Thunk: forwards to GameObject::LookAt(Point3D<float> const&) (0x393cec). Forwarded, not reimplemented.
void Character::Ctrl_LookAt(const Point3D<float>& pos) {
    GameObject::LookAt(pos);
}

// 0x3addc8 _ZN9Character10Ctrl_ClickERK7Point3DIfEb
// Character::Ctrl_Click(Point3D<float> const&, bool)
// PARTIAL. Reproduced: the CTRLIsAllowed gate; the casting or skill branch (a held click clears the queued click, a
// released one queues the point in m_spotTargetPos and m_spotTargetValidPos with pending set); the clear of the queued
// click; the nearest interactive character within the click radius (DesignSettings m_w11 for an enemy, m_w20 otherwise),
// which becomes the target through CharAI::AI_SetTarget; and the move or head-to fallback (held: Ctrl_MoveTo, else
// Ctrl_HeadTo) with the target cleared and last target synced. Not reproduced: the write of m_stickyTarget (CharAI +0x4b),
// which is private in dh2/CharAI.hpp.
// Not reproduced: the second search. For a held click without the DPad, when no character was found, the original walks
// ObjectManager's object map (m_objectMap, std::map<int, ObjectListItem>) for the nearest interactive non-Character object
// within DesignSettings m_w21 of the click, and sets the target to it. ObjectListItem has no declared layout, and the
// object's type-name test (the word at GameObject +0x5c compared with "Character") is not named either. The search is
// therefore not run, and such a click takes the fallback below. The debug switch reads are kept.
void Character::Ctrl_Click(const Point3D<float>& pos, bool held) {
    if (!CTRLIsAllowed()) {
        return;
    }
    const bool isUsingDPad = ApplicationGetInstance().IsUsingDPad();
    if (SM_IsCasting() || SM_IsUsingSkill()) {
        if (held) {
            m_spotTargetPending = 0;
            m_spotTargetFXId = -1;
        } else {
            m_spotTargetPending = 1;
            m_spotTargetPos = pos;
            m_spotTargetValidPos = pos;
            m_spotTargetFXId = -1;
        }
        return;
    }

    m_spotTargetPending = 0;
    m_spotTargetFXId = -1;

    // The nearest interactive character to the click (ObjectManager m_characters, in list order).
    GameObject* target = nullptr;
    float bestDistanceSquared = 1000000.0f;
    for (Character* candidate : dword_99F764->m_characters) {
        const Point3D<float>& candidatePos = candidate->GetTargetPosition();
        if (candidate == this || !candidate->IsInteractive(this)) {
            continue;
        }
        if (isUsingDPad && !AI_IsEnemy(candidate)) {
            continue;
        }
        if (AI_IsNeutral(candidate)) {
            continue;
        }
        const float dx = pos.X - candidatePos.X;
        const float dy = pos.Y - candidatePos.Y;
        const float dz = pos.Z - candidatePos.Z;
        const float distanceSquared = (dx * dx + dy * dy) + dz * dz;
        if (bestDistanceSquared > distanceSquared) {
            const Structs::DesignSettings* design =
                reinterpret_cast<const Structs::DesignSettings*>(Arrays::DesignSettingsTable::members);
            const float radius = AI_IsEnemy(candidate) ? design->m_w11 : design->m_w20;
            if (candidate->IsNearby(pos, radius)) {
                bestDistanceSquared = distanceSquared;
                target = candidate;
            }
        }
    }

    if (target) {
        DebugSwitches::GetInstance()->load();
        DebugSwitches::GetInstance()->GetSwitch("isTracingChar_CTRL");
        // PARTIAL: the original sets CharAI +0x4b (m_stickyTarget) to 1 here. That member is private in dh2/CharAI.hpp,
        // so the write is not made; the target is set below.
        CharAI::AI_SetTarget(target, false);
        return;
    }

    // PARTIAL: the second search (see above) is not run; the click falls through to the move or head-to branch.
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch("UseClickToMove") || !isUsingDPad) {
        DebugSwitches::GetInstance()->load();
        DebugSwitches::GetInstance()->GetSwitch("isTracingChar_CTRL");
        CharAI::AI_SetTarget(nullptr, false);
        CharAI::AI_SyncLastTarget();
        if (held) {
            Ctrl_MoveTo(pos);
        } else {
            Ctrl_HeadTo(pos);
        }
    }
}

// 0x3ae5a0 _ZNK9Character18GetCharFaeryListIdEv
// Character::GetCharFaeryListId() const
// PARTIAL: the original reads the faery list index from Character +0x106c (CharProperties +0xb0c, inside the m_props
// sheet). No class declares that word, so the index is taken as 0 until the layout request is applied. The range check
// against Arrays::FaeryListTable::size, and the return of 0 when it fails, are reproduced.
int Character::GetCharFaeryListId() const {
    const int index = 0;
    if (index < 0 || index >= static_cast<int>(Arrays::FaeryListTable::size)) {
        return 0;
    }
    return index;
}

// 0x3ae5dc _ZNK9Character16GetCharFaeryListEv
// Character::GetCharFaeryList() const
// PARTIAL: the row address is exact (the Structs::FaeryList row at the index, indexed as a typed array from
// Arrays::FaeryListTable::members). The index comes from GetCharFaeryListId, which is partial (see above), so the row
// returned is row 0 until that field is recovered.
const void* Character::GetCharFaeryList() const {
    return reinterpret_cast<const Structs::FaeryList*>(Arrays::FaeryListTable::members) + GetCharFaeryListId();
}
