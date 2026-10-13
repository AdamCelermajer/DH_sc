// Batch g1-game-character-CharAI-2ae0f1-5: the AI group map (GroupInfo copy and destructor, the default constructor
// that the original builds inline, ClearGroupInfo, AddToGroup, RemoveFromGroup), the animation step hooks, the animation
// event dispatcher, AI_SyncLastTarget and AI_IsTargetSeeking. Declarations come from dh2/CharAI.hpp. Each PARTIAL comment
// names what is missing; the ledger rows in game-character/ledger.d/g1-game-character-CharAI-2ae0f1-5.csv repeat it.
//
// Two gates recur. The anim stack level (Character +0x4c8) is CharAnimator::m_animStackIdx at +0x2c, which is private,
// so every branch that reads it is PARTIAL. CharStateMachine::m_stateFlags (Character +0x520, bit 0x1000) is private too.

#include <algorithm>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "dh2/AnimController.hpp"
#include "dh2/CharAI.hpp"
#include "dh2/Character.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"
#include "dh2/v2Controller.hpp"

// ---- group map ------------------------------------------------------------------------------------------------------

// 0x3d2cc0 CharAI::GroupInfo::~GroupInfo()
// The original frees the three vectors in reverse declaration order (m_members +0x18, m_leaders +0x0c, m_masterLeaders
// +0x00). The member destructors do the same.
CharAI::GroupInfo::~GroupInfo() {}

// 0x3d2de0 CharAI::GroupInfo::GroupInfo(CharAI::GroupInfo const&)
// The asm calls the vector copy constructor on all three vectors (the Hex-Rays first call shows a default constructor,
// which is wrong), then copies m_allMembersDead (+0x24), m_leadersDead (+0x28) and m_triggered (+0x29). pad_2a is not copied.
CharAI::GroupInfo::GroupInfo(const GroupInfo& other)
    : m_masterLeaders(other.m_masterLeaders),
      m_leaders(other.m_leaders),
      m_members(other.m_members),
      m_allMembersDead(other.m_allMembersDead),
      m_leadersDead(other.m_leadersDead),
      m_triggered(other.m_triggered) {}

// No standalone symbol: AddToGroup builds a new group in place. The original zeroes the 0x2c bytes (the vectors are empty),
// sets m_allMembersDead (+0x24) to -1 and leaves the flags at 0. That is the state written here.
CharAI::GroupInfo::GroupInfo()
    : m_allMembersDead(-1), m_leadersDead(false), m_triggered(false) {}

// 0x3d2fb0 CharAI::ClearGroupInfo()
// The original erases the whole tree (the node count is non-zero) and resets the root, the count and both ends. That is
// std::map::clear().
void CharAI::ClearGroupInfo() {
    if (!s_groups.empty()) {
        s_groups.clear();
    }
}

// 0x3d2ff8 CharAI::RemoveFromGroup()
// The role picks the vector: 0 and 3 use m_members, 1 uses m_leaders, 2 uses m_masterLeaders, and any other role does
// nothing. The search is for this CharAI: the vectors hold Character pointers and the comparison converts each stored
// Character* to its CharAI base (asm: the find is called with the CharAI pointer and no adjustment). A found entry is erased.
void CharAI::RemoveFromGroup() {
    GroupInfo* group = m_group;
    if (!group) {
        return;
    }
    std::vector<Character*>* members = nullptr;
    switch (m_groupRole) {
        case 0:
        case 3:
            members = &group->m_members;
            break;
        case 1:
            members = &group->m_leaders;
            break;
        case 2:
            members = &group->m_masterLeaders;
            break;
        default:
            return;
    }
    const auto it = std::find(members->begin(), members->end(), this);
    if (it != members->end()) {
        members->erase(it);
    }
}

// 0x3d37d0 CharAI::AddToGroup(Character*)
// Static. The original takes the Character in its first argument (Hex-Rays types it as CharAI *this); the second argument
// is unused. A Character with an empty group name joins nothing. The role name picks the role: "Normal" 0, "Leader" 1,
// "MasterLeader" 2, "Group" 3, and any other name returns. The group is keyed by (group name, ObjectBase::unk_64 at +0x64).
// The map lookup is std::map::find (the Hex-Rays descent is a lower_bound followed by an equality test). A
// missing key is inserted with a fresh GroupInfo. The Character joins m_masterLeaders for role 2, m_leaders for role 1, and
// m_members for roles 0 and 3. Then m_group (+0x34) and m_groupRole (+0x38) are set on the CharAI.
// The meaning of the key id at +0x64 is not recovered; the field keeps its ObjectBase name.
void CharAI::AddToGroup(Character* c) {
    if (c->m_charGroup.empty()) {
        return;
    }
    const char* roleName = c->m_charGroupRole.c_str();
    int role = 0;
    if (std::strcmp(roleName, "Normal") == 0) {
        role = 0;
    } else if (std::strcmp(roleName, "Leader") == 0) {
        role = 1;
    } else if (std::strcmp(roleName, "MasterLeader") == 0) {
        role = 2;
    } else if (std::strcmp(roleName, "Group") == 0) {
        role = 3;
    } else {
        return;
    }
    const std::pair<std::string, int> key(c->m_charGroup, c->unk_64);
    auto it = s_groups.find(key);
    if (it == s_groups.end()) {
        it = s_groups.emplace(key, GroupInfo()).first;
    }
    GroupInfo& group = it->second;
    if (role == 1) {
        group.m_leaders.push_back(c);
    } else if (role == 2) {
        group.m_masterLeaders.push_back(c);
    } else {
        group.m_members.push_back(c);
    }
    c->CharAI::m_group = &group;
    c->CharAI::m_groupRole = role;
}

// ---- targets -----------------------------------------------------------------------------------------------------------

// 0x3d49c4 CharAI::AI_SyncLastTarget(void)
void CharAI::AI_SyncLastTarget() {
    m_lastTarget = m_target;
}

// 0x3d49d0 CharAI::AI_IsTargetSeeking(void) const
bool CharAI::AI_IsTargetSeeking() const {
    if (!m_targetSeeking) {
        return false;
    }
    // PARTIAL: the original returns !(Character +0x520 & 0x1000). Character +0x520 is CharStateMachine::m_stateFlags (+0x24),
    // which is private to CharStateMachine, so the bit test is omitted and the seeking flag alone decides. Needs a public
    // accessor on CharStateMachine (layout request).
    return true;
}

// ---- animation hooks: sequences and step dispatch ----------------------------------------------------------------------

// 0x3d3ae4 CharAI::_OnEndOfAnimSection(void)
bool CharAI::_OnEndOfAnimSection() {
    return true;
}

// 0x3d3aec CharAI::_OnEndOfAnim(void)
bool CharAI::_OnEndOfAnim() {
    return true;
}

// 0x3d3d30 CharAI::_OnAnimSequenceEnd(void)
// The original reads the state (CharStateMachine at +0x4fc) and discards it.
bool CharAI::_OnAnimSequenceEnd() {
    m_character->CharStateMachine::SM_GetState();
    return true;
}

// 0x3d3d4c CharAI::_OnAnimSequenceBegin(void)
// The original reads the state (CharStateMachine at +0x4fc) and discards it.
bool CharAI::_OnAnimSequenceBegin() {
    m_character->CharStateMachine::SM_GetState();
    return true;
}

// 0x3d3ff8 CharAI::_OnAnimStepEnd(void)
// State 5 (attack) goes to _OnAnimStepEnd_Attack. States 6 and 7 (skill, spell) go to _OnAnimStepEnd_SkillSpell. Every other
// state returns true. The dispatch is exact; the two callees are PARTIAL.
bool CharAI::_OnAnimStepEnd() {
    const int state = m_character->CharStateMachine::SM_GetState();
    if (state == 5) {
        return _OnAnimStepEnd_Attack();
    }
    if (state < 5 || state > 7) {
        return true;
    }
    return _OnAnimStepEnd_SkillSpell();
}

// 0x3d4204 CharAI::_OnAnimStepBegin(void)
// State 4 (move) goes to _OnAnimStepBegin_Move, 5 (attack) to _OnAnimStepBegin_Attack, and 6 and 7 (skill, spell) to
// _OnAnimStepBegin_SkillSpell. Every other state returns true. The dispatch is exact; the attack and skill callees are PARTIAL.
bool CharAI::_OnAnimStepBegin() {
    switch (m_character->CharStateMachine::SM_GetState()) {
        case 4:
            return _OnAnimStepBegin_Move();
        case 5:
            return _OnAnimStepBegin_Attack();
        case 6:
        case 7:
            return _OnAnimStepBegin_SkillSpell();
        default:
            return true;
    }
}

// 0x3d4120 CharAI::_OnAnimStepBegin_Move(void)
// With a target that the AI is seeking, the original compares the squared distance from m_character->m_destination (GameObject
// +0x1a8) to the target's position against AI_GetMeleeRadiusSq(). When the target is outside that radius, it moves the
// character towards the target through the controller (v2Controllable m_controller, +0x378).
bool CharAI::_OnAnimStepBegin_Move() {
    if (!m_target) {
        return true;
    }
    if (!AI_IsTargetSeeking()) {
        return true;
    }
    const Point3D<float>& targetPosition = m_target->GetTargetPosition();
    const float dx = targetPosition.X - m_character->m_destination.X;
    const float dy = targetPosition.Y - m_character->m_destination.Y;
    const float dz = targetPosition.Z - m_character->m_destination.Z;
    if (AI_GetMeleeRadiusSq() > (dx * dx + dy * dy + dz * dz)) {
        return true;
    }
    m_character->m_controller->Cmd_MoveTo(m_target);
    return true;
}

// 0x3d3d68 CharAI::_OnAnimStepEnd_SkillSpell(void)
// PARTIAL: the original tests StepIndex == 0 and the anim stack level == 1 (Character +0x4c8, CharAnimator::m_animStackIdx at
// +0x2c, private: layout request). When both hold and m_skillStepEnded (+0xd1) is set, it calls CharAnimator::ANIM_StopLoop(true).
// The stack level is not readable here, so the test and the stop are omitted.
bool CharAI::_OnAnimStepEnd_SkillSpell() {
    return true;
}

// 0x3d3dd4 CharAI::_OnAnimStepBegin_SkillSpell(void)
// PARTIAL: the same stack-level test as _OnAnimStepEnd_SkillSpell gates the body. When it holds, the original sets
// m_skillStepBegun (+0xd0) and, if m_skillStepEnded (+0xd1) is set, calls CharAnimator::ANIM_StopLoop(true). Neither write
// is made here because the stack level is private (layout request).
bool CharAI::_OnAnimStepBegin_SkillSpell() {
    return true;
}

// 0x3d3e44 CharAI::_OnAnimStepEnd_Attack(void)
// When the character has no combo attack, the original returns. Past that gate every branch depends on the anim stack
// level (Character +0x4c8, private CharAnimator m_animStackIdx: layout request). The level 0 path ends the attack: it clears
// m_attacking (+0x78) and raises event 27, and event 28 as well on the last step. The level 1 path chooses between skipping the
// next step and clearing m_attacking, from the step index, the target and the interaction type 8. Both paths are omitted here.
bool CharAI::_OnAnimStepEnd_Attack() {
    if (!m_character->HasComboAttack()) {
        return true;
    }
    return true;
}

// 0x3d4044 CharAI::_OnAnimStepBegin_Attack(void)
// PARTIAL: the body depends on the anim stack level (Character +0x4c8, private CharAnimator m_animStackIdx: layout request).
// Level 0 stores the step index in m_attackStep (+0x74) and raises event 26 with it. Level 1 on step 0 sets m_attackLastStep
// (+0x79), looks at the target through the controller and calls OnPreAttack; on later steps it sets m_attackLastStep and
// m_attackCommitted (+0x7a) from the last-step test. Both paths are omitted here.
bool CharAI::_OnAnimStepBegin_Attack() {
    return true;
}

// ---- animation events ----------------------------------------------------------------------------------------------

// 0x3d3af4 CharAI::_ParseObjectAnimEvent(char const*)
// The name is "object/anim". "camera/anim" plays a camera anim; any other object is looked up by name. A Character gets
// the anim by its index in Arrays::AnimTable. Any other object with a visual gets PlayClip on its anim controller.
void CharAI::_ParseObjectAnimEvent(const char* name) {
    const char* slash = std::strchr(name, '/');
    if (!slash) {
        return;
    }
    const std::string objectName(name, slash);  // LC_API_STRNCPY_0 into a local buffer, terminated
    const char* animName = slash + 1;
    if (objectName == "camera") {
        // PARTIAL: the original reads Application::GetCurrentLevel()->m_camera (Level +0x128, public), tests
        // CameraLevel::CanPlayShakeAnim(camera, m_character), then plays the Arrays::AnimDict entry for animName through
        // CameraLevel::PlayAnim(camera, index, false, true). CameraLevel has no header in the tree, the Application singleton is
        // not reachable from this translation unit, and the AnimDict name table is not wired (see the layout requests).
        return;
    }
    ObjectHandle handle = dword_99F764->GetObjectByName(objectName.c_str(), -1, false, nullptr);
    if (Character* character = handle) {
        // PARTIAL: the original finds animName in Arrays::AnimTable (-1 when absent) and calls
        // CharAnimator::ANIM_Set(index) on the character's CharAnimator subobject (+0x49c). Arrays::AnimTable is declared
        // two ways in pydata-accessors (member_names.hpp as a NameTable, w7_arrays_02.hpp as members and size), so the lookup
        // is not written and the anim is not set.
        (void)character;
        return;
    }
    if (GameObject* object = handle) {
        VisualObject* visual = object->m_visualObject;  // +0x2d8
        if (visual) {
            AnimController* anim = visual->m_animController;  // +0x38
            if (anim) {
                anim->PlayClip(animName, false, 0, 0);  // the const char* overload, vtable slot +0x20
            }
        }
    }
}

// 0x3d4434 CharAI::_OnAnimEvent(char const*)
// The prefixes route the event: "ev_" goes to the virtual OnAnimEvent (the script hook), "an_" to _ParseObjectAnimEvent,
// "fx_" to the animated effect table, "sfx_" to the sound table. Any other name is handled by the loaded state:
// 5 (attack) picks the attack event, 6 (skill) "do_skill", 7 (spell) "do_spell", 13 (interact) "interact".
// PARTIAL: the trace probes (DebugSwitches::load and GetSwitch on a stack-built name before each branch) are not written,
// because their names are built on the stack and are not recovered; they return nothing and change no state. The "fx_" and
// "sfx_" branches are omitted (VisualFXManager and VoxSoundManager singletons and the Arrays::AnimatedEffectTable and
// Arrays::Sounds name tables are not declared), and so is the ranged projectile spawn in the attack state (the
// ProjectileManager singleton is not declared).
bool CharAI::_OnAnimEvent(const char* name) {
    const int stepIndex = m_character->CharAnimator::ANIM_GetStepIndex();
    m_character->CharAnimator::ANIM_GetStepCount();
    if (std::strncmp(name, "ev_", 3) == 0) {
        OnAnimEvent(name + 3);
        return true;
    }
    if (std::strncmp(name, "an_", 3) == 0) {
        _ParseObjectAnimEvent(name + 3);
        return true;
    }
    if (std::strncmp(name, "fx_", 3) == 0) {
        // PARTIAL: the animated effect named name+3 (Arrays::AnimatedEffectTable index) is played at the target position
        // through VisualFXManager::PlayAnimFXSet on Singleton<VisualFXManager>::s_inst. Not written, see the file comment.
        return true;
    }
    if (std::strncmp(name, "sfx_", 4) == 0) {
        // PARTIAL: the sound named name+4 (Arrays::Sounds index) is played at the target position through
        // VoxSoundManager::Play3D on VoxSoundManager::s_instance. Not written, see the file comment.
        return true;
    }
    switch (m_character->CharStateMachine::SM_GetState()) {
        case 5: {
            int unusedA = 0;  // the original passes one scratch slot for the first two outputs; neither is read here
            int unusedB = 0;
            int projectileIndex = 0;
            if (m_character->CanRangeAttack(unusedA, unusedB, projectileIndex)) {
                // PARTIAL: for "attack_ranged", "attack_mainhand" and "do_skill" the original spawns projectile
                // projectileIndex owned by m_character, with CharAI::_HandleProjectile as the callback, through
                // ProjectileManager::Spawn on Singleton<ProjectileManager>::s_inst. The singleton is not declared in the tree.
            } else if (std::strcmp(name, "attack_mainhand") != 0) {
                if (std::strcmp(name, "attack_offhand") == 0) {
                    OnAttack(m_attackStep, stepIndex - 1, true);
                }
            } else {
                OnAttack(m_attackStep, stepIndex - 1, false);
            }
            break;
        }
        case 6:
            if (std::strcmp(name, "do_skill") == 0) {
                _SkillEvent();
            }
            break;
        case 7:
            if (std::strcmp(name, "do_spell") == 0) {
                _SpellEvent();
            }
            break;
        case 13:  // 0xD
            if (std::strcmp(name, "interact") == 0) {
                OnInteract();
            }
            break;
        default:
            break;
    }
    return true;
}
