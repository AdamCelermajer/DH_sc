// glitch::collada accessors on the synchronized animator set, the skinned-mesh
// and timeline bases, and the animator base's length. Only the members these
// accessors touch are mapped.
#pragma once

#include <cstdint>

namespace glitch::collada {

// One synchronisation segment: a timestamp and a length, 32 bytes apart.
struct SSyncSegment {
    std::uint32_t timestamp;  // +0x00
    std::uint32_t length;     // +0x04
    std::uint32_t reserved[6];
};

class CSceneNodeAnimatorSynchronizedSet {
public:
    // 0x368db4 _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet14getNumSegmentsEv
    std::uint32_t getNumSegments() const;
    // 0x368eac _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet13hasReachedEndEv
    bool hasReachedEnd() const;
    // 0x368dc4 _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet24getSynchronizationLengthEj
    // Unchecked index, as in the original.
    std::uint32_t getSynchronizationLength(unsigned index) const;
    // 0x368dfc _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet27getSynchronizationTimestampEj
    std::uint32_t getSynchronizationTimestamp(unsigned index) const;

    SSyncSegment* segments = nullptr;   // +0x60
    std::uint32_t segmentCount = 0;     // +0x6c
    bool reachedEnd = false;            // +0x80
};

class ISkinnedMesh {
public:
    // 0x669b64 _ZN6glitch7collada12ISkinnedMesh20setIsSkinningEnabledEb
    void setIsSkinningEnabled(bool enabled);

    bool skinningEnabled = false;  // +0x18
};

class CTimelineController {
public:
    // 0x666c18 _ZNK6glitch7collada19CTimelineController8getScaleEv
    float getScale() const;

    float scale = 1.0f;  // +0x30
};

// Animator base of the collada namespace: the length word is read directly.
class ISceneNodeAnimator {
public:
    // 0x667cc4 _ZNK6glitch7collada18ISceneNodeAnimator9getLengthEv
    std::uint32_t getLength() const;

    std::uint32_t length = 0;  // +0x14
};

}  // namespace glitch::collada
