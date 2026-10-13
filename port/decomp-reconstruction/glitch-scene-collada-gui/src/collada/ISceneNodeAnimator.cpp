// Bodies of glitch::collada::ISceneNodeAnimator and IColladaSceneNodeSynchronizedAnimatorInterface (canonical classes:
// collada/ISceneNodeAnimator.hpp). This file replaces src/tb_scene-collada-gui-glitch_collada_ISceneNodeAnimator-0.cpp, which
// wrote the functions against the stand-in in collada_misc.hpp (no base, five of the virtual slots).
//
// The comment before each function is `// 0xADDR symbol`. A function whose comment contains PARTIAL is ledgered partial.
#include "glitch-scene-collada-gui/collada/ISceneNodeAnimator.hpp"

#include <cstring>
#include <stdexcept>

#include "glitch-core-io/os/logging.hpp"
#include "glitch-scene-collada-gui/collada/CColladaDatabase.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_detail.hpp"

namespace glitch::collada {

using detail::channelTypeCompatible;

// 0x62fdec _ZN6glitch7collada22FxEchoNotHandledEventsERKNS0_15STriggeredEventEPv
// The original takes the event pointer only (r0); the user data is not read.
void FxEchoNotHandledEvents(const STriggeredEvent& event, void* /*userData*/) {
    os::Printer::log("Warning - Event not handled : See collada::CSceneNodeAnimator::setEventsCallback", ELL_INFORMATION);
    os::Printer::log(event.eventName, ELL_INFORMATION);
}

// 0x660dac _ZN6glitch7collada46IColladaSceneNodeSynchronizedAnimatorInterfaceD1Ev (empty; D0 0x6615f4 is skipped)
IColladaSceneNodeSynchronizedAnimatorInterface::~IColladaSceneNodeSynchronizedAnimatorInterface() = default;

// 0x669828 _ZN6glitch7collada18ISceneNodeAnimatorC1Ev and 0x6698fc (C2, the VTT form)
// The scene base is constructed first (the compiler writes the vtable words and the virtual base); then the callback is
// FxEchoNotHandledEvents and every other member word is zero. The C1 form also writes 1 into the virtual base's count
// (+0x28), which is the IReferenceCounted default here.
ISceneNodeAnimator::ISceneNodeAnimator() = default;

// 0x669738 _ZN6glitch7collada18ISceneNodeAnimatorD1Ev, 0x6697c4 (D2); D0 0x6697a8 is skipped.
// Drops the events manager (the handle does it), then the scene base drops the timeline controller.
ISceneNodeAnimator::~ISceneNodeAnimator() = default;

// 0x667cb4 _ZNK6glitch7collada18ISceneNodeAnimator19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original.
void ISceneNodeAnimator::serializeAttributes(io::IAttributes*, io::SAttributeReadWriteOptions*) const {}

// 0x667cb8 _ZN6glitch7collada18ISceneNodeAnimator21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original.
void ISceneNodeAnimator::deserializeAttributes(io::IAttributes*, io::SAttributeReadWriteOptions*) {}

// 0x667c48 _ZN6glitch7collada18ISceneNodeAnimator10updateTimeEj
// Steps the timeline controller (vptr+0x44 gets it) and, when an events manager is held, reports the interval that was crossed:
// onUpdate(previous time, new time, range start, range end). The original returns the register of the last call.
void ISceneNodeAnimator::updateTime(std::uint32_t timeMs) {
    scene::ITimelineController* controller = getTimelineCtrl();
    if (!controller) {
        return;
    }
    const std::int32_t previous = controller->currentTimeMs;
    controller->update(static_cast<std::int32_t>(timeMs));
    if (eventsTrack) {
        eventsTrack->onUpdate(previous, controller->currentTimeMs, controller->rangeStartMs, controller->rangeEndMs);
    }
}

// 0x667cbc _ZN6glitch7collada18ISceneNodeAnimator11createCloneEv
// Returns null.
scene::ISceneNodeAnimator* ISceneNodeAnimator::createClone() {
    return nullptr;
}

// 0x669538 _ZN6glitch7collada18ISceneNodeAnimator6onBindEPNS_5scene10ISceneNodeE
// Binds to a new node and, when the animator has targets (getTargetCount, vptr+0x70), binds every target to the scene.
void ISceneNodeAnimator::onBind(scene::ISceneNode* node) {
    if (boundNode != node) {
        boundNode = node;
        if (getTargetCount() != 0) {
            forceBind();
        }
    }
}

// 0x667d9c _ZN6glitch7collada18ISceneNodeAnimator8onUnbindEPNS_5scene10ISceneNodeE
// Unbinds the targets that the node's UID names (the node's getUID, vptr+0x54).
void ISceneNodeAnimator::onUnbind(scene::ISceneNode* node) {
    forceUnbind(node->getUID());
}

// 0x667cc4 _ZNK6glitch7collada18ISceneNodeAnimator9getLengthEv
std::uint32_t ISceneNodeAnimator::getLength() const {
    return length;
}

// 0x6699c4 _ZN6glitch7collada18ISceneNodeAnimator19hasCompatibleTargetERKNS0_8SChannelE
// A channel of type 14 matches a target of the same name whose channel flag byte is equal; any other channel matches a target of
// the same name whose own channel type is compatible with it (the compatibility table).
bool ISceneNodeAnimator::hasCompatibleTarget(const SChannel& channel) {
    const std::int32_t count = getTargetCount();
    if (channel.type != 14) {
        for (std::int32_t i = 0; i < count; ++i) {
            if (std::strcmp(getBindURI(i), channel.name) == 0 && channelTypeCompatible(getAnimationTrack(i)->type, channel.type)) {
                return true;
            }
        }
        return false;
    }
    for (std::int32_t i = 0; i < count; ++i) {
        if (std::strcmp(getBindURI(i), channel.name) == 0 && getAnimationTrack(i)->flag == channel.flag) {
            return true;
        }
    }
    return false;
}

// 0x669574 _ZN6glitch7collada18ISceneNodeAnimator19setCompatibleTargetERKNS0_8SChannelEPv
// As hasCompatibleTarget; the first match gets `target` (setTarget(index, target, nullptr), vptr+0x68) and the result is 1.
int ISceneNodeAnimator::setCompatibleTarget(const SChannel& channel, void* target) {
    const std::int32_t count = getTargetCount();
    if (channel.type != 14) {
        for (std::int32_t i = 0; i < count; ++i) {
            if (std::strcmp(getBindURI(i), channel.name) == 0 && channelTypeCompatible(getAnimationTrack(i)->type, channel.type)) {
                setTarget(i, target, nullptr);
                return 1;
            }
        }
        return 0;
    }
    for (std::int32_t i = 0; i < count; ++i) {
        if (std::strcmp(getBindURI(i), channel.name) == 0 && getAnimationTrack(i)->flag == channel.flag) {
            setTarget(i, target, nullptr);
            return 1;
        }
    }
    return 0;
}

// 0x667dc4 _ZN6glitch7collada18ISceneNodeAnimator9setTargetEPKcPv
// Binds `target` to the first target whose bind URI equals `name` and returns what setTarget(index, ...) returns (nothing here:
// the slot is void; the original returns its register). With no match the strcmp result is returned: non-zero, or the target
// count when it is zero.
int ISceneNodeAnimator::setTarget(const char* name, void* target) {
    const std::int32_t count = getTargetCount();
    for (std::int32_t i = 0; i < count; ++i) {
        if (std::strcmp(getBindURI(i), name) == 0) {
            setTarget(i, target, nullptr);
            return 1;
        }
    }
    return 0;
}

// 0x366214 _ZN6glitch7collada18ISceneNodeAnimator14isTrackEnabledEi
bool ISceneNodeAnimator::isTrackEnabled(std::int32_t) {
    return true;
}

// 0x3662c4 _ZN6glitch7collada18ISceneNodeAnimator11onUnbindAllEv (a thunk to forceUnbindAll)
void ISceneNodeAnimator::onUnbindAll() {
    forceUnbindAll();
}

// 0x667f18 _ZN6glitch7collada18ISceneNodeAnimator9forceBindEv
// PARTIAL, body not reconstructed. The original (30320 bytes) takes the bound scene node (+0x10) and walks the targets. For
// each target it reads the channel type through vptr+0x54 (word +0x08), then runs a switch over 0x01..0x5b. The cases look up
// particle systems, morphing meshes, lights and materials by name through CRootSceneNode (getParticleSystem, getMorphingMesh,
// getLight, getMaterial) and bind them through vptr+0x68 with the CApplicatorInfo of that channel. The CRootSceneNode lookups
// and the applicator values are not reconstructed, so this does nothing.
void ISceneNodeAnimator::forceBind() {}

// 0x667d18 _ZN6glitch7collada18ISceneNodeAnimator11forceUnbindEPKc
// Clears the target of every track whose bind URI (the channel name at +0x04) equals `name`.
void ISceneNodeAnimator::forceUnbind(const char* name) {
    const std::int32_t count = getTargetCount();
    for (std::int32_t i = 0; i < count; ++i) {
        if (std::strcmp(getAnimationTrack(i)->name, name) == 0) {
            setTarget(i, nullptr, nullptr);
        }
    }
}

// 0x667e40 _ZN6glitch7collada18ISceneNodeAnimator14forceUnbindAllEv
// For every target the node the bind URI names (searched from the bound node) forgets this animator (removeBindedAnimator,
// vptr+0x7c), and the target is cleared. The bound node is cleared at the end.
void ISceneNodeAnimator::forceUnbindAll() {
    scene::ISceneNode* root = boundNode;
    const std::int32_t count = getTargetCount();
    for (std::int32_t i = 0; i < count; ++i) {
        scene::ISceneNode* node = root->getSceneNodeFromUID(getBindURI(i));
        if (node) {
            node->removeBindedAnimator(this);
        }
        setTarget(i, nullptr, nullptr);
    }
    boundNode = nullptr;
}

// 0x60fab8 _ZN6glitch7collada18ISceneNodeAnimator14setEventsTrackEPKNS0_12SEventsTrackE
// Replaces the events manager: the old one is dropped; a non-null track gets a new CEventsManager holding the animator's
// callback and user data (the +0x1c and +0x20 words), with the last-entry index at -1.
void ISceneNodeAnimator::setEventsTrack(const SEventKeyTable* track) {
    if (track) {
        CEventsManager* manager = new CEventsManager();
        manager->keyTable = const_cast<SEventKeyTable*>(track);  // +0x14
        manager->callback = eventCallback;                       // +0x08 (from +0x1c)
        manager->callbackUserData = eventUserData;               // +0x0c (from +0x20)
        manager->lastEntry = -1;                                 // +0x10
        eventsTrack = boost::intrusive_ptr<CEventsManager>(manager);  // takes the construction use (count 1)
    } else {
        eventsTrack = nullptr;
    }
}

}  // namespace glitch::collada
