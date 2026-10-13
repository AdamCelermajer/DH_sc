// Batch g1-game-character-AISDefault-7ff887-2 (part 3 of 3 of AISDefault): the target and combat hooks (OnAttack,
// OnInteract, the melee, close, ranged and out-of-range hooks), OnUpdate, InitVCB, the combat results, anim event and
// script timer hooks. Declarations and layout come from dh2/AISDefault.hpp. The deleting destructor (0x3dc968) is
// compiler generated and is not rebuilt.
//
// Layout facts used here (32-bit offsets, the port reads them by name):
//   - CharAIScript::m_character (+0x98) is the owner Character. CharAI is the Character base at +0x3c8, so
//     CharAI::m_target is Character +0x408 and CharAI::m_targetSeeking is Character +0x412.
//   - v2Controllable::m_controller (Character +0x378) is the v2Controller that the Cmd_* calls drive.
//   - GameObject::m_pfObject (+0x1c8) holds the path list PFObject::m_path at +0x38, so the list sentinel is
//     Character +0x200. The original walks that list in several places with no body; only its emptiness is observable.
//   - CharStateMachine is the Character base at +0x4fc (the state tests use it).
//   - CharAIScript::unk_b8 (+0xb8) is the callback mask word. InitVCB writes 0x800 (OnTargetHit) and 0x1000
//     (OnTargetMissed) there, and OnCombatResults reads them.
//   - CharAIScript::unk_bc (+0xbc) is the counter that OnUpdate tests against 199.

#include <cstring>

#include "dh2/AISDefault.hpp"
#include "dh2/CharAI.hpp"
#include "dh2/CharStateMachine.hpp"
#include "dh2/Character.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/v2Controllable.hpp"
#include "dh2/v2Controller.hpp"

namespace {

// The original walks the path list (Character +0x200) in several places with an empty loop body. Those walks
// have no effect, so only the emptiness of the list is kept.
bool PathIsEmpty(const Character* character) {
    return character->m_pfObject.m_path.empty();
}

// The clear that the target hooks run once the character has no path left to walk: the target is dropped and
// AI_SyncLastTarget copies it, then the seeking flag at +0x4a of CharAI is cleared.
void DropTarget(CharAI* ai) {
    ai->AI_SetTarget(nullptr, false);
    ai->AI_SyncLastTarget();
    ai->m_targetSeeking = false;
}

// The body that OnTargetInRangedRange and OnTargetOutOfRange share (the ranged hook runs it after its enemy branch).
// While the target is sought, the character walks to the target's interaction spot. The target is dropped when the
// path is empty after the move.
void SeekInteractionSpot(Character* self) {
    CharAI* ai = self;
    if (!ai->AI_IsTargetSeeking()) {
        return;
    }
    CharStateMachine* sm = self;
    if (!sm->SM_IsIdle(false) && !sm->SM_IsAttacking() && !PathIsEmpty(self)) {
        return;
    }
    self->m_controller->Cmd_MoveTo(ai->m_target->GetInteractionSpot());
    if (PathIsEmpty(self)) {
        DropTarget(ai);
    }
}

}  // namespace

// 0x3dc12c AISDefault::OnAttack(int,int,bool)
// The two int arguments are unused by the original. The flag is the third hook argument and goes to F_MeleeAttack.
void AISDefault::OnAttack(int /*a*/, int /*b*/, bool flag) {
    CharAI* ai = m_character;
    Character* target = ai->AI_GetTargetAsCharacter();
    if (target) {
        // F_MeleeAttack and F_ApplyResult are static in the binary (Hex-Rays shows no this argument). They are called
        // through the owner, as CharAI::_HandleProjectile does; the object is ignored by the callee.
        Character::AttackResult result{};
        m_character->F_MeleeAttack(result, m_character, target, flag, false);
        m_character->F_ApplyResult(result, m_character, target, false);
        return;
    }
    // The target is not a character: the owner interacts with it (vtable slot +0x98, owner passed in r1).
    if (GameObject* object = ai->m_target) {
        object->Interact(m_character);
    }
}

// 0x3dc1b0 AISDefault::OnInteract(void)
void AISDefault::OnInteract() {
    CharAI* ai = m_character;
    GameObject* target = ai->m_target;
    if (!target) {
        return;
    }
    // A character target that is a monster is not interacted with.
    if (target->IsCharacter() && ai->AI_GetTargetAsCharacter()->IsMonster()) {
        return;
    }
    target->Interact(m_character);
    // The original reads the target again after the call. A target cleared by the call is a null read there; the port
    // stops instead.
    target = ai->m_target;
    if (!target) {
        return;
    }
    if (target->unk_2fa != 0) {  // GameObject +0x2fa (unk_2fa, meaning not recovered): a non-zero value skips the reset
        return;
    }
    // A character target that is a merchant keeps the target.
    if (target->IsCharacter() && ai->AI_GetTargetAsCharacter()->IsMerchant()) {
        return;
    }
    DropTarget(ai);
}

// 0x3dc284 AISDefault::OnTargetInMeleeRange(void)
void AISDefault::OnTargetInMeleeRange() {
    Character* self = m_character;
    CharAI* ai = self;
    if (ai->AI_IsEnemy(nullptr)) {
        self->m_controller->Cmd_Stop();
        self->m_controller->Cmd_Attack(ai->m_target);
        return;
    }
    if (ai->AI_IsInInteractionRange(ai->m_target)) {
        self->m_controller->Cmd_Stop();
        ai->AI_InteractWith(nullptr);
    }
}

// 0x3dc300 AISDefault::OnTargetInCloseRange(void)
void AISDefault::OnTargetInCloseRange() {
    Character* self = m_character;
    CharAI* ai = self;
    CharStateMachine* sm = self;
    if (ai->AI_IsEnemy(nullptr)) {
        if (!ai->AI_IsTargetSeeking()) {
            return;
        }
        if (!sm->SM_IsIdle(false) && !sm->SM_IsAttacking() && !PathIsEmpty(self)) {
            return;
        }
        // Back away from the target: the point beyond the character by the same offset that the target has from it.
        const Point3D<float>& selfPos = self->GetTargetPosition();
        const Point3D<float>& targetPos = ai->m_target->GetTargetPosition();
        Point3D<float> away;
        away.X = (selfPos.X - targetPos.X) + selfPos.X;
        away.Y = (selfPos.Y - targetPos.Y) + selfPos.Y;
        away.Z = (selfPos.Z - targetPos.Z) + selfPos.Z;
        self->m_controller->Cmd_MoveTo(away);
        if (PathIsEmpty(self)) {
            DropTarget(ai);
        }
        return;
    }
    if (ai->AI_IsInInteractionRange(ai->m_target)) {
        self->m_controller->Cmd_Stop();
        ai->AI_InteractWith(nullptr);
        return;
    }
    if (!PathIsEmpty(self)) {
        return;
    }
    self->m_controller->Cmd_MoveTo(ai->m_target->GetInteractionSpot());
    if (PathIsEmpty(self)) {
        DropTarget(ai);
    }
}

// 0x3dc560 AISDefault::OnTargetInRangedRange(void)
void AISDefault::OnTargetInRangedRange() {
    Character* self = m_character;
    CharAI* ai = self;
    if (ai->AI_IsEnemy(nullptr)) {
        self->m_controller->Cmd_Stop();
        self->m_controller->Cmd_Attack(ai->m_target);
        return;
    }
    SeekInteractionSpot(self);
}

// 0x3dc698 AISDefault::OnTargetOutOfRange(void)
void AISDefault::OnTargetOutOfRange() {
    SeekInteractionSpot(m_character);
}

// 0x3dc798 AISDefault::OnUpdate(void)
// Over 199 updates the counter (+0xbc) resets and the character is paused for 1000 ms and stops moving.
void AISDefault::OnUpdate() {
    if (unk_bc > 199u) {
        unk_bc = 0;
        CharAI* ai = m_character;
        ai->AI_PauseUpdate(1000u);
        m_character->m_controller->Cmd_Stop();
    }
}

// 0x3dc7d8 AISDefault::InitVCB(void)
// Sets the callback mask (+0xb8): 0x800 when the script defines OnTargetHit, 0x1000 when it defines OnTargetMissed.
// The return value is the OnTargetMissed bit alone, as the original returns it (no caller reads it).
int AISDefault::InitVCB() {
    unk_b8 = 0;
    const std::uint32_t hit = IsInVFTable("OnTargetHit") ? 0x800u : 0u;
    unk_b8 = hit;
    const std::uint32_t missed = IsInVFTable("OnTargetMissed") ? 0x1000u : 0u;
    unk_b8 = missed | hit;
    return static_cast<int>(missed);
}

// 0x3dc968 AISDefault::~AISDefault()
// The deleting destructor (D0) is compiler generated: it runs the destructor and frees the object. Nothing is written
// here; the complete-object destructor is in the part 2 batch.

// 0x3dc9a8 AISDefault::OnCombatResults(Character *,Character *,void *)
// PARTIAL: the Lua call is not made. The original pushes a2 and a3 as userdata into an sfc::script::lua::Arguments and
// calls LuaScript::Call with "OnTargetMissed" or "OnTargetHit". The Arguments class has no header in the tree yet
// (dh2/sfc/script/lua/Arguments.hpp), so the two call sites are marked below. The branch that picks the hook is
// reproduced: a result whose low two flag bits are set is a miss, otherwise a hit.
void AISDefault::OnCombatResults(Character* a2, Character* a3, void* a4) {
    (void)a2;
    (void)a3;
    const auto* results = static_cast<const Character::AttackResult*>(a4);
    if ((results->m_resultFlags & 3u) != 0) {
        if ((unk_b8 & 0x1000u) != 0) {
            // LuaScript::Call("OnTargetMissed", args) is not made.
        }
    } else if ((unk_b8 & 0x800u) != 0) {
        // LuaScript::Call("OnTargetHit", args) is not made.
    }
}

// 0x3dca50 AISDefault::OnAnimEvent(char const*)
// PARTIAL. Not made:
//   - LuaScript::Call("OnAnimEvent") with the event name and CharAnimator::m_lastEventData (Character +0x4f4). The
//     Arguments class has no header in the tree yet.
//   - The footstep effect: VisualFXManager::PlayAnimFXSet on Singleton<VisualFXManager>::s_inst (no accessor), the
//     CharEffect flag for the character (the row index is Character +0x1014, which GetFXFootprint cannot reach yet)
//     and the footstep row for the floor material (PFFloor +0x3c, not declared in dh2/PFFloor.hpp).
// Reproduced: the step_right and step_left test, and the foot position chosen for each foot.
void AISDefault::OnAnimEvent(const char* name) {
    const bool rightFoot = std::strcmp(name, "step_right") == 0;
    if (!rightFoot && std::strcmp(name, "step_left") != 0) {
        return;
    }
    // The position the footstep effect plays at. The effect call itself is not made (see above).
    const Point3D<float> footPosition =
        rightFoot ? m_character->GetRightFootPosition() : m_character->GetLeftFootPosition();
    (void)footPosition;
}

// 0x3dcc80 AISDefault::OnScriptTimer(uint)
// PARTIAL: the original pushes the timer id into an sfc::script::lua::Arguments and calls LuaScript::Call("OnTimer").
// The Arguments class has no header in the tree yet, so the call is not made.
void AISDefault::OnScriptTimer(unsigned int id) {
    (void)id;
}
