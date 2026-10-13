// AnimatorSynchronizedSet: the game's scene-node animator for a COLLADA animation set whose clips are
// synchronised. Its engine base is glitch::collada::CSceneNodeAnimatorSynchronizedSet (glitch-scene-collada-gui).
// The ctor writes its own vtable and a flag, runs the base ctor, re-points the vtables, then constructs an
// AnimApplicator at +0x84 with this as its owner. getType returns 15.
// Original 32-bit size 0xc8 (the AnimSetManager note: GetSynchronizedAnimator builds one at 0xc8 bytes).
// Reconstructed from libDungeonHunter2.so: ctor 0x36904c / 0x3690dc, dtors 0x368f54 / 0x368fcc / 0x368fe8,
// animateNode 0x368f24, getType 0x368efc.
//
// Not inherited: the engine header collada_misc.hpp models the base's data members at their 32-bit offsets
// but not its vptrs, so deriving from it would misplace every member below. The base is kept as a byte
// sub-object until glitch-scene-collada-gui's layout is unified.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-core-io/boost/intrusive_ptr.hpp"

#include "dh2/AnimApplicator.hpp"

namespace glitch {
namespace collada {
class CAnimationSet;
}  // namespace collada
namespace scene {
class ISceneNode;
}  // namespace scene
}  // namespace glitch

class AnimatorSynchronizedSet {
public:
    AnimatorSynchronizedSet(const boost::intrusive_ptr<glitch::collada::CAnimationSet>& animationSet,
                            const std::vector<std::string>& names);     // 0x36904c (C1), 0x3690dc (C2)
    virtual ~AnimatorSynchronizedSet();                                 // 0x368f54 (D1), 0x368fcc (D0), 0x368fe8 (D2)

    virtual void animateNode(glitch::scene::ISceneNode* node, unsigned int time);  // 0x368f24. Calls the base animateNode, then AnimApplicator::AnimateNode on +0x84; returns void
    virtual int getType() const;                                        // 0x368efc returns 15

    // +0x00 primary vtable pointer (implicit). The CSceneNodeAnimatorSynchronizedSet base sub-object covers
    // +0x04 .. +0x83. Its layout belongs to glitch-scene-collada-gui and is not reproduced here.
    uint8_t pad_04[0x80];                                               // +0x04 .. +0x83 engine base sub-object

    AnimApplicator m_applicator;                                        // +0x84 .. +0xbf (0x3c bytes in the original), built by the ctor as AnimApplicator(this + 0x84, this)
    void* m_vptrSecondary;                                              // +0xc0 secondary vtable pointer: off_95b720 before the base ctor, off_963268 after
    int32_t unk_0c4;                                                    // +0xc4 written 1 by the ctor before the base ctor call
};
