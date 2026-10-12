// glitch::collada accessors on the synchronized animator set, the skinned-mesh
// and timeline bases, and the animator base's length. Only the members these
// accessors touch are mapped.
#pragma once

#include <cstdint>
#include <vector>

namespace glitch::scene {
class ISceneNode;
class CMeshBuffer;
}

namespace glitch::video {
class IVideoDriver;
}

namespace glitch::core {
template <typename T>
struct aabbox3d;
}

namespace glitch::io {
class IAttributes;
struct SAttributeReadWriteOptions;
}  // namespace glitch::io

namespace glitch::collada {

class SSkinBuffer;

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
    // The callback is called as callback(this, callbackData) when the clock reaches the
    // end of the range (original: a plain function pointer at +0x08, data word at +0x0c).
    using TimelineCallback = void (*)(CTimelineController* controller, std::int32_t data);

    // 0x666f00 _ZN6glitch7collada19CTimelineControllerD1Ev (empty)
    // 0x666efc _ZN6glitch7collada19CTimelineControllerD2Ev (empty, same as D1)
    virtual ~CTimelineController() = default;

    // 0x666e40 _ZN6glitch7collada19CTimelineControllerC1Ev (partial, see the .cpp)
    CTimelineController();

    // 0x666c18 _ZNK6glitch7collada19CTimelineController8getScaleEv
    float getScale() const;
    // 0x666cc4 _ZNK6glitch7collada19CTimelineController12getClipCountEv
    // Returns 0 when there is no clip table.
    std::int32_t getClipCount() const;
    // 0x666cd4 _ZNK6glitch7collada19CTimelineController12getClipStartEi (vtable +0x20)
    virtual std::uint32_t getClipStart(int index) const;
    // 0x666cec _ZNK6glitch7collada19CTimelineController10getClipEndEi (vtable +0x24)
    virtual std::uint32_t getClipEnd(int index) const;
    // 0x666d84 _ZNK6glitch7collada19CTimelineController11getClipNameEi
    const char* getClipName(int index) const;

    // 0x666c20 _ZN6glitch7collada19CTimelineController8setScaleEf (vtable +0x48)
    virtual void setScale(float value);
    // 0x666c28 _ZN6glitch7collada19CTimelineController7setLoopEb (vtable +0x40)
    virtual void setLoop(bool enabled);
    // 0x666c30 _ZNK6glitch7collada19CTimelineController7getLoopEv (vtable +0x44)
    virtual bool getLoop() const;
    // 0x666c38 _ZN6glitch7collada19CTimelineController8setRangeEiib (vtable +0x50)
    // The original returns a float in r0 here, which C++ callers never read; the return is void.
    virtual void setRange(std::int32_t startMs, std::int32_t endMs, bool apply);
    // 0x666d3c _ZNK6glitch7collada19CTimelineController19getCurrentClipStartEv (vtable +0x2c)
    virtual std::uint32_t getCurrentClipStart() const;
    // 0x666d54 _ZNK6glitch7collada19CTimelineController17getCurrentClipEndEv (vtable +0x30)
    virtual std::uint32_t getCurrentClipEnd() const;
    // 0x666d9c _ZNK6glitch7collada19CTimelineController19getCurrentClipIndexEv (vtable +0x38)
    virtual std::int32_t getCurrentClipIndex() const;
    // 0x666f04 _ZN6glitch7collada19CTimelineController4initEii
    void init(std::int32_t startMs, std::int32_t endMs);
    // 0x666f10 _ZN6glitch7collada19CTimelineController6jumpToEi (vtable +0x0c)
    // Returns the new time in seconds, as the original does.
    virtual float jumpTo(std::int32_t timeMs);
    // 0x666f7c _ZN6glitch7collada19CTimelineController7setClipEi (vtable +0x10)
    // Returns the length of the new clip in seconds, as the original does.
    virtual float setClip(std::int32_t clipIndex);
    // 0x667104 _ZN6glitch7collada19CTimelineController6updateEi (vtable +0x00)
    // Returns the new time in milliseconds, as the original does.
    virtual std::int32_t update(std::int32_t timeMs);

    // Clock state. Times are kept in milliseconds (ints) and in seconds (floats).
    std::int32_t currentTimeMs = 0;             // +0x04 last time in ms (written by update, jumpTo, setClip)
    TimelineCallback callback = nullptr;        // +0x08 called at the end of the range
    std::int32_t callbackData = 0;              // +0x0c second argument of callback
    std::int32_t rangeStartMs = 0;              // +0x10
    std::int32_t rangeEndMs = 0;                // +0x14
    bool loop = true;                           // +0x18
    float lastStep = 0.0f;                      // +0x1c step of the last update (the original stores |step| for a reverse step)
    float rangeStartSec = 0.0f;                 // +0x20
    float rangeLengthSec = 0.0f;                // +0x24
    float prevTimeSec = 0.0f;                   // +0x28
    float currentSec = 0.0f;                    // +0x2c current position in seconds
    float scale = 1.0f;                         // +0x30
    STimelineClipTable* clipTable = nullptr;    // +0x34 (non-null: setRange does not change the range)
    std::int32_t currentClipIndex = 0;          // +0x38
    bool endReported = false;                   // +0x3c set once the end callback has run (non-loop)
    bool started = false;                       // +0x3d set by the first update after a jump or clip change
};

// Forward declarations for the animator base below. The full types are in
// collada_empty_dtors.hpp (CEventsManager, SEventKeyTable, SEventCallArgs). SChannel
// has no definition in this category yet.
class CEventsManager;
struct SEventKeyTable;
struct SEventCallArgs;
struct SChannel;

// Animator base of the collada namespace. Offsets are the original 32-bit offsets,
// kept as comments only. The class is polymorphic, as in the original. Only the
// virtual slots this batch needs are declared. The base's pure slots (+0x54, +0x68,
// +0x6c, +0x70..+0x7c) are not declared, because CSceneNodeAnimatorBlender would
// become abstract without them.
class ISceneNodeAnimator {
public:
    // 0x669828 _ZN6glitch7collada18ISceneNodeAnimatorC1Ev (partial, see the .cpp)
    ISceneNodeAnimator();

    // 0x667cb4 _ZNK6glitch7collada18ISceneNodeAnimator19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE (vtable +0x00)
    virtual void serializeAttributes(glitch::io::IAttributes* attributes, glitch::io::SAttributeReadWriteOptions* options) const;
    // 0x667cb8 _ZN6glitch7collada18ISceneNodeAnimator21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE (vtable +0x04)
    virtual void deserializeAttributes(glitch::io::IAttributes* attributes, glitch::io::SAttributeReadWriteOptions* options);
    // 0x667c48 _ZN6glitch7collada18ISceneNodeAnimator10updateTimeEj (vtable +0x14, partial)
    virtual void updateTime(unsigned int time);
    // 0x669574 _ZN6glitch7collada18ISceneNodeAnimator19setCompatibleTargetERKNS0_8SChannelEPv (vtable +0x60, partial)
    virtual int setCompatibleTarget(const SChannel& channel, void* target);
    // 0x667dc4 _ZN6glitch7collada18ISceneNodeAnimator9setTargetEPKcPv (vtable +0x64, partial)
    virtual int setTarget(const char* name, void* target);

    // 0x667f18 _ZN6glitch7collada18ISceneNodeAnimator9forceBindEv (not virtual, partial)
    void forceBind();
    // 0x60fab8 _ZN6glitch7collada18ISceneNodeAnimator14setEventsTrackEPKNS0_12SEventsTrackE
    // The mangled parameter name is SEventsTrack; the header calls the same key table SEventKeyTable.
    void setEventsTrack(const SEventKeyTable* track);

    // 0x667cc4 _ZNK6glitch7collada18ISceneNodeAnimator9getLengthEv
    std::uint32_t getLength() const;

    std::uint32_t length = 0;                                        // +0x14
    CEventsManager* eventsTrack = nullptr;                           // +0x18 events manager (refcounted)
    void (*eventCallback)(const SEventCallArgs* args, void* userData) = nullptr;  // +0x1c
    void* eventUserData = nullptr;                                   // +0x20
};

namespace detail {

// Hardware skin techniques. Only the attach hook is mapped: it stores the scene node
// the technique is attached to at +0x14. The rest of each class is not modelled yet.
class CColladaHardwareMatrixSkinTechnique {
public:
    // 0x66b940 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique6attachEPNS_5scene10ISceneNodeE
    void attach(glitch::scene::ISceneNode* node);
    // 0x66c5f4 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique18computeBoundingBoxEv
    // Returns the box by value. Partial: see the body in src/tb_scene-collada-gui-glitch_collada_detail_CColladaHardwareMatrixSkin-0.cpp.
    glitch::core::aabbox3d<float> computeBoundingBox();

    std::uint8_t pad_0000[0x14] = {};                   // +0x00..+0x13 (not mapped)
    glitch::scene::ISceneNode* attachedNode = nullptr;  // +0x14
};

class CColladaHardwareQuatSkinTechnique {
public:
    // 0x66d080 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique6attachEPNS_5scene10ISceneNodeE
    void attach(glitch::scene::ISceneNode* node);
    // 0x66df88 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique15preparePtrCacheEv
    // Declared only: the member body is not reconstructed yet (the free stub in recon/slice_23 is not it).
    void preparePtrCache();
    // 0x66e384 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique18computeBoundingBoxEv
    // Returns the box by value (sret). Partial: needs SSkin and SSkinCache layouts; see the .cpp.
    glitch::core::aabbox3d<float> computeBoundingBox();

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

// Software skin technique. Only the attach hook is mapped; its constructor (C1 and C2)
// also stores the skin and cache references, which are not reconstructed here.
class CColladaSoftwareSkinTechnique {
public:
    // 0x66f6c8 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique6attachEPNS_5scene10ISceneNodeE
    void attach(glitch::scene::ISceneNode* node);

    // 0x66f8ec _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
    // Partial: stream setup and the process-buffer request are not reconstructed (see the .cpp).
    int init(glitch::collada::SSkinBuffer& skinBuffer, glitch::scene::CMeshBuffer* meshBuffer,
             glitch::video::IVideoDriver* driver, bool skipProcessBuffer);
    // 0x66fab4 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique15preparePtrCacheEv
    // Partial: the skin's joint-name array and the scene-node virtual slot are not mapped.
    void preparePtrCache();
    // 0x66fb84 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique17computeBoundingBoxEv
    // Partial: the receiver and argument roles disagree with the symbol (see the .cpp).
    void computeBoundingBox();
    // 0x66fe34 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique12prepareCacheEv
    // Partial: the SMatrix layout and the skin-cache matrix vector are not mapped.
    void prepareCache();
    // 0x66ff48 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
    // Partial: vertex-stream and index decoding need the SSkinBuffer and mesh-buffer layouts.
    int skin(glitch::collada::SSkinBuffer& skinBuffer, glitch::scene::CMeshBuffer* meshBuffer);

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

class CSceneNode;
struct SNode;
struct SChannel;

// One transformation target: a 16-byte record in the original (flag byte at +0x00, type word at
// +0x04, scene node at +0x08). addTransformationTargets pushes types 1, 5 and 10. The
// SNode overload never stores the node word (operator new does not zero it).
struct SAnimationTemplate {
    std::uint8_t flag = 0;        // +0x00 set to 1 by isAnimationExist when the channel matches
    std::uint32_t type = 0;       // +0x04 target kind: 1, 5 or 10
    CSceneNode* node = nullptr;   // +0x08 scene node the target was built from
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

    // 0x6e22cc _ZN6glitch7collada35CAnimationSetTransformationTemplate16isAnimationExistEPKNS0_8SChannelE
    // Partial: the channel name (+0x04) and type index (+0x08) and the scene-node name getter
    // (vtable slot +0x54) are not mapped; the body returns 0 (see the .cpp).
    int isAnimationExist(const SChannel* channel);
    // 0x6e23d0 _ZNK6glitch7collada35CAnimationSetTransformationTemplate15getDefaultValueEPKNS0_8SChannelEPPv
    // Partial: the database default-value lookup on each node (node +0x14c) is not mapped (see the .cpp).
    int getDefaultValue(const SChannel* channel, void** out) const;
    // 0x6e24e8 _ZN6glitch7collada35CAnimationSetTransformationTemplate24addTransformationTargetsEPNS0_10CSceneNodeE
    // Partial: the three target pushes are reconstructed; the child walk over the node's +0xf4 list is not.
    void addTransformationTargets(CSceneNode* node);
    // 0x6e26d4 _ZN6glitch7collada35CAnimationSetTransformationTemplate24addTransformationTargetsERNS0_5SNodeE
    // Partial: the three target pushes are reconstructed (node left null, as the original never stores it);
    // the child loop over SNode's count (+0x38) and child base (+0x3c, stride 0x50) is not.
    void addTransformationTargets(SNode& node);

    std::vector<SAnimationTemplate*> targets;  // +0x04 vector begin, +0x08 end, +0x0c capacity
};

}  // namespace glitch::collada
