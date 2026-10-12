#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CSceneNodeAnimatorSet-0.
// Most bodies in this class reach the ISceneNodeAnimator base, the CAnimationSet database
// chain, SAnimationAccessor and the vtables of the track objects, none of which are mapped
// in this category yet. Those bodies are partial and say what is missing. Nothing is guessed.

namespace glitch::collada {

// 0x65f114 _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
// Reads the current animation index at +0x50.
std::int32_t CSceneNodeAnimatorSet::getCurrentAnimation() const {
    return currentAnimation;
}

// 0x65f188 _ZN6glitch7collada21CSceneNodeAnimatorSet13getTargetSizeEi
// partial: the original loads tracks[index] from the animation set table at +0x18 and
// returns the result of that object's virtual slot +0x08. The track object type and its
// vtable are not mapped, so the call is not expressed and this returns 0.
int CSceneNodeAnimatorSet::getTargetSize(int index) {
    (void)index;
    return 0;
}

// 0x65f1ac _ZN6glitch7collada21CSceneNodeAnimatorSet14getTargetsSizeEv
// partial: the original returns 0 when the animation set's animated track count (+0x3c) is 0.
// Otherwise it sums the virtual slot +0x08 results of every track object in the table at +0x18.
// The track object type and its vtable are not mapped, so the sum is not expressed and this
// returns 0.
int CSceneNodeAnimatorSet::getTargetsSize() {
    return 0;
}

// 0x65f208 _ZN6glitch7collada21CSceneNodeAnimatorSet9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
// partial: the original stores the target pointer at buffer +0x28 [index], then drops the
// entry at the table +0x34 [index] through its vtable +0x04 (and clears the entry). When the
// applicator is non-null, it stores the result of the applicator's virtual slot +0x08 call in
// that entry and returns it. The element types of the +0x28 buffer and the +0x34 table are not
// confirmed, and the drop and clone virtuals are not typed, so only the stub return is given.
int CSceneNodeAnimatorSet::setTarget(int index, void* target, const animation_track::CApplicatorInfo* applicator) {
    (void)index;
    (void)target;
    (void)applicator;
    return 0;
}

// 0x65f270 _ZN6glitch7collada21CSceneNodeAnimatorSet11animateNodeEPNS_5scene10ISceneNodeEj
// partial: the original is a virtual dispatch through the object's own vtable slot +0x50
// (applyAnimationValues). It is written here as a direct call. A subclass that overrides
// applyAnimationValues is not dispatched to, because this class is not polymorphic in the
// port yet. The node argument is not used by the original.
void CSceneNodeAnimatorSet::animateNode(glitch::scene::ISceneNode* node, unsigned int time) {
    (void)node;
    applyAnimationValues(time);
}

// 0x65f418 _ZN6glitch7collada21CSceneNodeAnimatorSet20applyAnimationValuesEj
// partial: the body depends on the ISceneNodeAnimator base (updateTime, the timeline
// controller at vtable slot +0x44, the frame and length fields at +0x14), the
// CAnimationSet database chain (getDatabase and its clip library and version fields),
// the per-track buffers at +0x28 and +0x40, the applicator table at +0x34 and
// SAnimationAccessor::applyValue (0x66a010). None of these are mapped, so the body is
// not rebuilt.
void CSceneNodeAnimatorSet::applyAnimationValues(unsigned int time) {
    (void)time;
}

// 0x65f5dc _ZN6glitch7collada21CSceneNodeAnimatorSet22computeAnimationValuesEj
// partial: same missing pieces as applyAnimationValues (the ISceneNodeAnimator base, the
// timeline controller at vtable slot +0x44, the CAnimationSet database chain, the per-track
// buffers and SAnimationAccessor::getValue at 0x66a15c). Also the track's clone vtable slot
// +0x08 and memcpy from the stored source. The body is not rebuilt.
void CSceneNodeAnimatorSet::computeAnimationValues(unsigned int time) {
    (void)time;
}

// 0x65f7b4 _ZN6glitch7collada21CSceneNodeAnimatorSet17getAnimationValueEiiPv
// partial: the original copies the stored source value for the track into out (size from
// the track object's vtable +0x08), then, when the entry's kind is 2, reads the value through
// SAnimationAccessor::getValue with getAnimationData(index). The track object vtable, the
// accessor and getAnimationData (not mapped in this class) are missing, so this returns null.
void* CSceneNodeAnimatorSet::getAnimationValue(int a, int b, void* out) {
    (void)a;
    (void)b;
    (void)out;
    return nullptr;
}

// 0x65f8c8 _ZN6glitch7collada21CSceneNodeAnimatorSet19setCurrentAnimationEi
// partial: the current animation index is stored at +0x50, which is reconstructed here.
// The rest is not: it sets the ISceneNodeAnimator length (+0x14) from getAnimationLength,
// the track base (+0x4c) from the animated track count times the index, fetches the clip
// library from CAnimationSet::getDatabase and CColladaDatabase::getAnimationClipLibrary (the
// database return type in animation_set.hpp does not match the pseudocode), then drives the
// timeline controller and calls its vtable +0x40 with the animation start and end, or
// calls the vtable +0x40 slot on the controller when a clip library is present. It finishes
// with setEventsTrack from the database's event table. None of these are mapped, and the
// return value of the original tail call is not typed. Returns null.
glitch::IReferenceCounted* CSceneNodeAnimatorSet::setCurrentAnimation(int index) {
    currentAnimation = index;
    return nullptr;
}

// 0x660af4 _ZN6glitch7collada21CSceneNodeAnimatorSet4initERKN5boost13intrusive_ptrINS0_13CAnimationSetEEE
// partial: the animation set handle is stored at +0x24. The original counts the animated tracks
// and resizes four per-track vectors (+0x28, +0x34, +0x40 and the applicator and int tables) to
// that count, then creates a 0x48-byte CTimelineController, passes it to the virtual slot +0x3c
// (setTimelineCtrl), and calls the virtual slot +0x88 (setCurrentAnimation) with 0. The vectors,
// the timeline controller and the virtual calls are not mapped here. The intrusive reference
// count on the set is not modelled, because CAnimationSet has no reference count in the port.
void CSceneNodeAnimatorSet::init(const boost::intrusive_ptr<CAnimationSet>& set) {
    animationSet = set.get();
}

// 0x662c90 _ZN6glitch7collada21CSceneNodeAnimatorSetC2Ev
// partial: the VTT-taking base constructor. The original calls the ISceneNodeAnimator base
// constructor, writes the vtable pointers at +0x00 and +0x04 and zeroes the members from +0x24
// to +0x48. The base is not mapped, and the vtable stores are not expressed in C++. The
// members with default initializers are zeroed here, and the base is not called.
CSceneNodeAnimatorSet::CSceneNodeAnimatorSet() {}

// 0x660c20 _ZN6glitch7collada21CSceneNodeAnimatorSetC1ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEE
// partial: the original calls the ISceneNodeAnimator base constructor, writes the vtable
// pointers (+0x00, +0x04, +0x58 and +0x84) and the self pointer, stores the animation set with a
// reference count increment, zeroes the members from +0x54 to +0x7c, and then calls init. The
// base and the vtable stores are not expressed. The animation set handle is stored here, and init
// is called.
CSceneNodeAnimatorSet::CSceneNodeAnimatorSet(const boost::intrusive_ptr<CAnimationSet>& set)
    : animationSet(set.get()) {
    init(set);
}

// 0x660ce4 _ZN6glitch7collada21CSceneNodeAnimatorSetC2ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEE
// partial: the base-object (C2) variant of the constructor above. It takes the VTT argument,
// which is not a C++ parameter, so the port maps it onto the C1 constructor above. The
// secondary-base vtable stores (+0x04 and the VTT-based offsets) are not expressed.

}  // namespace glitch::collada
