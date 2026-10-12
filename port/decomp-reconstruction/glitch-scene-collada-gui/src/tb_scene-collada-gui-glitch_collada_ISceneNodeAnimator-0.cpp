// Track B batch e1-glitch-scene-collada-gui-glitch_collada_ISceneNodeAnimator-0.
// Owner class: glitch::collada::ISceneNodeAnimator (declared in collada_misc.hpp).
#include "glitch-scene-collada-gui/collada_misc.hpp"
#include "glitch-scene-collada-gui/collada_empty_dtors.hpp"  // CEventsManager, SEventKeyTable

namespace glitch::collada {

// 0x669828 _ZN6glitch7collada18ISceneNodeAnimatorC1Ev
// partial: the original writes the primary and secondary vtable pointers (+0x00, +0x04,
// +0x24, plus the IObject base vtables) and calls glitch::IObject::IObject. It also writes
// +0x28 = 1, whose owner is not mapped. Missing: the vtable and base-object stores (no host
// equivalent here, and IObject is not declared in this category), the +0x28 word, and the
// FxEchoNotHandledEvents callback at +0x1c (that function is not declared, so the callback
// stays null). The zeroed words are the defaults of length (+0x14), eventsTrack (+0x18)
// and eventUserData (+0x20).
ISceneNodeAnimator::ISceneNodeAnimator() {}

// 0x6698fc _ZN6glitch7collada18ISceneNodeAnimatorC2Ev
// skipped: the second constructor takes the VTT, and its vptr and virtual-base stores
// come from the VTT words. The host has no VTT, so this body is not rebuilt. The field
// initialisation is done by the C1 row (0x669828).

// 0x667cb4 _ZNK6glitch7collada18ISceneNodeAnimator19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original.
void ISceneNodeAnimator::serializeAttributes(glitch::io::IAttributes* attributes,
                                             glitch::io::SAttributeReadWriteOptions* options) const {
    (void)attributes;
    (void)options;
}

// 0x667cb8 _ZN6glitch7collada18ISceneNodeAnimator21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original.
void ISceneNodeAnimator::deserializeAttributes(glitch::io::IAttributes* attributes,
                                               glitch::io::SAttributeReadWriteOptions* options) {
    (void)attributes;
    (void)options;
}

// 0x667c48 _ZN6glitch7collada18ISceneNodeAnimator10updateTimeEj
// partial. The original does three things, in this order:
//   1. ctl = this->getTimelineCtrl() through vtable +0x44 (scene::ISceneNodeAnimator's
//      getTimelineCtrl, which returns the word at +0x08). If ctl is null, return.
//   2. previousFrame = ctl->currentFrame (+0x04), then ctl->update(time) through slot 0.
//   3. If eventsTrack (+0x18) is set, eventsTrack->onUpdate(previousFrame,
//      ctl->currentFrame, ctl->+0x10, ctl->+0x14), which is the four-argument form at
//      CEventsManager vtable +0x10.
// Missing: (a) this collada class does not declare getTimelineCtrl, and it does not derive
// from the scene base that declares it, so step 1 is not expressed; (b) the loop words at
// ctl +0x10 and +0x14 are not declared on ITimelineController, so step 3 is not expressed.
// Until those are declared this function does nothing. The original returns the register
// of its last call, which no caller reads, so the host returns void.
void ISceneNodeAnimator::updateTime(unsigned int time) {
    (void)time;
}

// 0x669574 _ZN6glitch7collada18ISceneNodeAnimator19setCompatibleTargetERKNS0_8SChannelEPv
// partial. The original works in two modes. For SChannel type 14 it matches the child name
// (+0x04) and the target's word at +0x0c against each target. For other types it matches the
// name and then tests bit (type) of CompatibilityTable entry targetType (the table is
// IColladaSceneNodeAnimatorChannelType::CompatibilityTable, 12-byte stride). On a match it
// calls setTarget(index, target, 0) through vtable +0x68 and returns 1. The loop bounds come
// from vtable +0x70 (count) and +0x6c (name), and the channel descriptor from +0x54. None of
// those pure slots is declared here, the SChannel fields are not declared, and the
// compatibility table is not mapped. Until they are, this returns 0, meaning no target was set.
int ISceneNodeAnimator::setCompatibleTarget(const SChannel& channel, void* target) {
    (void)channel;
    (void)target;
    return 0;
}

// 0x667dc4 _ZN6glitch7collada18ISceneNodeAnimator9setTargetEPKcPv
// partial. The original takes the target count from vtable +0x70, compares each target's
// name (vtable +0x6c, a strcmp against the argument) and, on the first match, calls
// setTarget(index, target, 0) through vtable +0x68, returning that call's value. It returns
// the strcmp value when the count is zero. The name, count and index-setter slots are pure
// in this base and are not declared here. Declaring them would make
// CSceneNodeAnimatorBlender abstract, because that class does not implement the name getter.
// Until they are declared this returns 0, meaning no target was set.
int ISceneNodeAnimator::setTarget(const char* name, void* target) {
    (void)name;
    (void)target;
    return 0;
}

// 0x667f18 _ZN6glitch7collada18ISceneNodeAnimator9forceBindEv
// partial, body not reconstructed. The original (30320 bytes) takes the bound scene node
// from +0x10 and walks the targets. For each target it reads the channel type through
// vtable +0x54 (word +0x08), then runs a switch over 0x01..0x5b. The cases look up particle
// system, morphing mesh, light and material fields by name through CRootSceneNode
// (getParticleSystem, getMorphingMesh, getLight, getMaterial), and bind them through vtable
// +0x68. Missing: the pure slots +0x54, +0x68, +0x6c and +0x70 (not declared); the
// CRootSceneNode lookups and the particle-system name accessors (vtable +0xfc of the
// particle system); the material parameter ID lookup; and the CApplicatorInfo values written
// on each bind. Until those are declared this function does nothing.
void ISceneNodeAnimator::forceBind() {}

// 0x60fab8 _ZN6glitch7collada18ISceneNodeAnimator14setEventsTrackEPKNS0_12SEventsTrackE
// The old events manager (+0x18) is released first, with IReferenceCounted::drop semantics:
// the count at +0x04 is decremented, and the object is deleted when the count was 1. The
// onDelete slot of CEventsManager (vtable +0x08) is the empty IReferenceCounted::onDelete, so
// the delete is written inline. A non-null track then creates a new CEventsManager with
// keyTable = track (+0x14), callback = eventCallback (+0x1c, into +0x08), userData =
// eventUserData (+0x20, into +0x0c), count = 1 (+0x04) and lastEntry = -1 (+0x10). With a null
// track, eventsTrack is set to null.
void ISceneNodeAnimator::setEventsTrack(const SEventKeyTable* track) {
    if (eventsTrack) {
        CEventsManager* old = eventsTrack;
        if (old->referenceCounter-- == 1) {
            delete old;
        }
    }
    if (track) {
        CEventsManager* manager = new CEventsManager();
        manager->keyTable = const_cast<SEventKeyTable*>(track);  // +0x14
        manager->callback = eventCallback;                       // +0x08 (from +0x1c)
        manager->callbackUserData = eventUserData;               // +0x0c (from +0x20)
        manager->referenceCounter = 1;                           // +0x04
        manager->lastEntry = -1;                                 // +0x10
        eventsTrack = manager;
    } else {
        eventsTrack = nullptr;
    }
}

}  // namespace glitch::collada
