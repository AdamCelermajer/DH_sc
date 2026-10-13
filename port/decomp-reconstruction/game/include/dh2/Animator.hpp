// Animator: the game's scene-node animator for one COLLADA clip library. It plays the clips of its
// animation database on a scene node through an embedded AnimApplicator, and reports the end of a clip
// through the ITimelineController callback (_CBAnim) and the SetCallbacks pair.
// Vtable: the primary table (208 bytes) comes from glitch::collada::CSceneNodeAnimator, with a secondary
// table for the IReferenceCounted sub-object at +0x94. getType returns 11.
// Original 32-bit size 0x9c (the reference-counted sub-object ends at +0x9b). No operator new seen.
// Reconstructed from libDungeonHunter2.so pseudocode (ctor 0x366490 / 0x366518, dtor 0x3663b0, animateNode 0x3662c8).
#pragma once

#include <cstdint>

#include "glitch-core-io/boost/intrusive_ptr.hpp"

#include "dh2/AnimApplicator.hpp"

namespace glitch {
namespace scene {
class ISceneNode;
class ITimelineController;
}  // namespace scene
namespace collada {
class CColladaDatabase;
struct SLibraryAnimationClips;
struct STriggeredEvent;
}  // namespace collada
}  // namespace glitch

class Animator {
public:
    typedef void (*AnimCallback)(glitch::scene::ITimelineController* timeline, void* userData);
    typedef void (*EventCallback)(const glitch::collada::STriggeredEvent& event, void* userData);

    Animator(const glitch::collada::CColladaDatabase& database,
             glitch::collada::SLibraryAnimationClips& clips);              // 0x366490 / 0x366518

    virtual ~Animator();                                                   // 0x3663b0, 0x366418, 0x366434 (the three dtor bodies; D1/D0 order not confirmed)

    virtual int getType() const;                                           // 0x36621c returns 11
    // Not in the vtable: _CBAnim and the animateNode/onUnbind paths call it directly (0x366224).
    void _HandleAnimEnding(glitch::scene::ITimelineController* timeline);  // 0x366224 stores the time left in the clip, sets m_applicator.m_callbackPending
    // Timeline callback: userData is the Animator (SetCallbacks stores this). 0x366294 swaps the arguments and tail-calls _HandleAnimEnding.
    static void _CBAnim(glitch::scene::ITimelineController* timeline, void* userData);
    virtual void animateNode(glitch::scene::ISceneNode* node, unsigned int time);  // 0x3662c8 base animateNode, then AnimApplicator::AnimateNode and CheckCallback
    virtual void onUnbind(glitch::scene::ISceneNode* node);                // 0x366308 clears the applicator's reference node, then the base onUnbind

    void SetCallbacks(AnimCallback animCallback, void* animUserData,
                      EventCallback eventCallback, void* eventUserData);   // 0x366330 stores both pairs, rebinds the timeline callback, sets the applicator's callback

    // Primary vptr is implicit at +0x00. The glitch::collada::CSceneNodeAnimator base sub-object covers
    // +0x04 .. +0x57; its layout belongs to glitch-scene-collada-gui and is not reproduced here.
    uint8_t pad_04[0x54];                                                  // +0x04 .. +0x57 engine base sub-object. Base candidate: glitch::collada::ISceneNodeAnimator
                                                                           //       (collada_misc.hpp: length +0x14, eventsTrack +0x18, eventCallback +0x1c, eventUserData +0x20
                                                                           //       match). scene_node_animator.hpp CSceneNodeAnimator has no vptr and does not fit. Replacing this pad with the base is a delete.
    AnimApplicator m_applicator;                                           // +0x58 owner is this (ctor 0x366490 passes this). 0x3c bytes.
                                                                           //       _HandleAnimEnding writes m_applicator.unk_10 (+0x68) and m_callbackPending (+0x88).
    glitch::IReferenceCounted m_refSecondary;                              // +0x94 secondary IReferenceCounted sub-object (vptr, count = 1 from the ctor)
};
