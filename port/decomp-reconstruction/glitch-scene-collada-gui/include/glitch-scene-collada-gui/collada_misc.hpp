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

// One clip of the timeline controller's clip table (12 bytes in the original).
struct STimelineClip {
    const char* name = nullptr;  // +0x00
    std::uint32_t start = 0;     // +0x04
    std::uint32_t end = 0;       // +0x08
};

// Clip table reached through the pointer at +0x34 of the controller. The clip
// count is the first word and the clip array pointer is at +0x04.
struct STimelineClipTable {
    std::int32_t count = 0;            // +0x00
    STimelineClip* clips = nullptr;    // +0x04
};

class CTimelineController {
public:
    // 0x666f00 _ZN6glitch7collada19CTimelineControllerD1Ev (empty)
    // 0x666efc _ZN6glitch7collada19CTimelineControllerD2Ev (empty, same as D1)
    ~CTimelineController() = default;

    // 0x666c18 _ZNK6glitch7collada19CTimelineController8getScaleEv
    float getScale() const;
    // 0x666cc4 _ZNK6glitch7collada19CTimelineController12getClipCountEv
    // Returns 0 when there is no clip table.
    std::int32_t getClipCount() const;
    // 0x666cd4 _ZNK6glitch7collada19CTimelineController12getClipStartEi
    std::uint32_t getClipStart(int index) const;
    // 0x666cec _ZNK6glitch7collada19CTimelineController10getClipEndEi
    std::uint32_t getClipEnd(int index) const;
    // 0x666d84 _ZNK6glitch7collada19CTimelineController11getClipNameEi
    const char* getClipName(int index) const;

    float scale = 1.0f;                        // +0x30
    STimelineClipTable* clipTable = nullptr;   // +0x34
    std::int32_t currentClipIndex = 0;         // +0x38
};

// Animator base of the collada namespace: the length word is read directly.
class ISceneNodeAnimator {
public:
    // 0x667cc4 _ZNK6glitch7collada18ISceneNodeAnimator9getLengthEv
    std::uint32_t getLength() const;

    std::uint32_t length = 0;  // +0x14
};

}  // namespace glitch::collada
