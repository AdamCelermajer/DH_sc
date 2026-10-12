// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CSceneNodeAnimator-0.
// Owner class: glitch::collada::CSceneNodeAnimator (declared in scene_node_animator.hpp).
//
// All three functions are partial. The owner header's SAnimatorTarget is a host struct with
// the track ids at +0x10 and +0x14. The original target vector uses 16-byte records with
// the target pointer at +0x04 and the applicator object pointer at +0x08 (setTarget writes
// both). That record layout is not mapped, so no store is written here (see each note).
//
// Layout requests for the owner header (integrator):
//   - a target record with void* target and CApplicatorInfo* applicator (host width), and
//     the 16-byte original stride noted; the track and trackEx words must be reconciled with it.
//   - the base ISceneNodeAnimator (updateTime at vtable +0x14, virtual slot +0x44 used as the
//     time source) so the time and index path can be written.
//   - the byte at this+0x34, the word at this+0x38 and this+0x14 (time modulus), and this+0x50
//     (gate word) are not declared.
#include "glitch-scene-collada-gui/scene_node_animator.hpp"

namespace glitch::collada {

// 0x65d408 _ZN6glitch7collada18CSceneNodeAnimator9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
// partial. Original:
//   record = targets.begin + 16 * index
//   record[+4] = target
//   if (record[+8] != 0) { record[+8]->vtable[+4]() (deleting release); record[+8] = 0; }
//   if (applicator != 0) record[+8] = applicator->vtable[+8]() (clone)
// Missing:
//   - the record fields +0x04 (target) and +0x08 (applicator). SAnimatorTarget has no void*
//     or applicator slot at those offsets, so nothing is stored.
//   - the virtual release (CApplicatorInfo vtable +0x04) and clone (vtable +0x08): the class
//     declares no virtuals.
//   - the original returns the last call's result; the caller's use of it is not mapped.
void CSceneNodeAnimator::setTarget(int index, void* target, const animation_track::CApplicatorInfo* applicator) {
    (void)index;
    (void)target;
    (void)applicator;
}

// 0x65d80c _ZN6glitch7collada18CSceneNodeAnimator22computeAnimationValuesEj
// partial. Original gate: if ((targets.end - targets.begin) >> 4 != 0 || this[+0x50]) then
//   updateTime(time) (ISceneNodeAnimator vtable +0x14), index = vtable[+0x44](this) ? its +4 word
//   : this[+0x38] + time % this[+0x14]; data = getAnimationData(this, index); then for each
//   target record: if record[+4] != 0, SAnimationAccessor::getValue(record[+0], accessor data,
//   record[+4], pointer), where the pointer is record+0x0c, or the first record's +0x0c when the
//   flag byte at this+0x34 is set.
// Missing: the base class (this class does not derive from ISceneNodeAnimator), the gate word
//   at +0x50, the flag byte at +0x34, the time source and modulus words (+0x38, +0x14), the
//   getAnimationData helper, and the SAnimationAccessor getValue signature. Nothing is done here.
// The original's return value (the object pointer) is not mapped; the function returns void.
void CSceneNodeAnimator::computeAnimationValues(unsigned int time) {
    (void)time;
}

// 0x65d91c _ZN6glitch7collada18CSceneNodeAnimator20applyAnimationValuesEj
// partial. Same gate, time source, and index selection as computeAnimationValues. Per target
// record it calls SAnimationAccessor::applyValue(record[+0], record[+4], record[+8] (applicator),
// the flag-selected pointer, the block flag) instead of getValue.
// Missing: the same pieces listed for computeAnimationValues, plus the applyValue signature and
//   the SAnimationAccessor helper's declaration. Nothing is done here. Returns void (the original's
//   return is the object pointer and is not mapped).
void CSceneNodeAnimator::applyAnimationValues(unsigned int time) {
    (void)time;
}

}  // namespace glitch::collada
