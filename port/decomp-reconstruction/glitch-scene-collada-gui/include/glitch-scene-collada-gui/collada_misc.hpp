// glitch::collada accessors on the synchronized animator set, the skinned-mesh
// and timeline bases, and the animator base's length. Only the members these
// accessors touch are mapped.
#pragma once

#include <cstdint>

namespace glitch::scene {
class ISceneNode;
}

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

    // 0x368ebc _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet29getSynchronizationElapsedTimeEv
    // Truncates the elapsed time to an integer.
    std::int32_t getSynchronizationElapsedTime() const;

    SSyncSegment* segments = nullptr;   // +0x60
    std::uint8_t pad_0068[0x04] = {};   // +0x68..+0x6b (not mapped)
    std::uint32_t segmentCount = 0;     // +0x6c
    std::uint8_t pad_0070[0x0c] = {};   // +0x70..+0x7b (not mapped)
    float elapsedTime = 0.0f;           // +0x7c
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

namespace detail {

// Hardware skin techniques. Only the attach hook is mapped: it stores the scene node
// the technique is attached to at +0x14. The rest of each class is not modelled yet.
class CColladaHardwareMatrixSkinTechnique {
public:
    // 0x66b940 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique6attachEPNS_5scene10ISceneNodeE
    void attach(glitch::scene::ISceneNode* node);

    std::uint8_t pad_0000[0x14] = {};                   // +0x00..+0x13 (not mapped)
    glitch::scene::ISceneNode* attachedNode = nullptr;  // +0x14
};

class CColladaHardwareQuatSkinTechnique {
public:
    // 0x66d080 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique6attachEPNS_5scene10ISceneNodeE
    void attach(glitch::scene::ISceneNode* node);

    std::uint8_t pad_0000[0x14] = {};                   // +0x00..+0x13 (not mapped)
    glitch::scene::ISceneNode* attachedNode = nullptr;  // +0x14
};

class CColladaHardwareTextureSkinTechnique {
public:
    // 0x66e778 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique6attachEPNS_5scene10ISceneNodeE
    void attach(glitch::scene::ISceneNode* node);

    std::uint8_t pad_0000[0x14] = {};                   // +0x00..+0x13 (not mapped)
    glitch::scene::ISceneNode* attachedNode = nullptr;  // +0x14
};

}  // namespace detail

// The streaming manager is a singleton: its constructor publishes itself through the
// static Instance pointer. Its members are only zeroed here.
class CAnimationStreamingManager {
public:
    // 0x60b3cc _ZN6glitch7collada26CAnimationStreamingManagerC2Ev
    // 0x60b410 _ZN6glitch7collada26CAnimationStreamingManagerC1Ev
    CAnimationStreamingManager();

    static CAnimationStreamingManager* Instance;

    std::uint32_t words[8] = {};  // +0x00..+0x1f (words that are zeroed on construction; roles not mapped)
};

// Animation-set template for scene-node transformation targets. The base class
// (IAnimationSetTemplate) is not ported, so its vtable and destructor are not modelled.
class CAnimationSetTransformationTemplate {
public:
    // 0x6e21c0 _ZN6glitch7collada35CAnimationSetTransformationTemplateC2EPKc
    // 0x6e21f4 _ZN6glitch7collada35CAnimationSetTransformationTemplateC1EPKc
    // The name argument is not stored in the original.
    explicit CAnimationSetTransformationTemplate(const char* name);
    // 0x6e2248 _ZN6glitch7collada35CAnimationSetTransformationTemplateD1Ev
    // 0x6e2298 _ZN6glitch7collada35CAnimationSetTransformationTemplateD2Ev
    // Partial: the base destructor (IAnimationSetTemplate::D2) is not called here.
    ~CAnimationSetTransformationTemplate() = default;

    std::uint32_t reserved04 = 0;  // +0x04
    std::uint32_t reserved08 = 0;  // +0x08
    std::uint32_t reserved0c = 0;  // +0x0c
};

}  // namespace glitch::collada
