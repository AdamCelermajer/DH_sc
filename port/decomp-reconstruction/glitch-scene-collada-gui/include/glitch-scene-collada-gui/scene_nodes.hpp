// glitch::scene node, animator and manager accessors. Each class maps only the
// members its accessors touch; base classes and the rest of the layout are not
// reconstructed yet. Offsets are kept as comments; the code uses typed members.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/scene_node.hpp"

namespace glitch {
struct SEvent;
}
namespace glitch::io {
class IWriteFile;
}

namespace glitch::scene {

class IMeshWriter;

// Animator base defaults. Each virtual is a constant or an empty body in the
// original; the type tag is 10.
class ISceneNodeAnimator {
public:
    virtual ~ISceneNodeAnimator() = default;
    // 0x599798 _ZN6glitch5scene18ISceneNodeAnimator10updateTimeEj (empty)
    virtual void updateTime(std::uint32_t time);
    // 0x59979c _ZNK6glitch5scene18ISceneNodeAnimator22isEventReceiverEnabledEv (returns false)
    virtual bool isEventReceiverEnabled() const;
    // 0x5997ac _ZN6glitch5scene18ISceneNodeAnimator7onEventERKNS_6SEventE (returns false)
    virtual bool onEvent(const glitch::SEvent& event);
    // 0x5997b4 _ZNK6glitch5scene18ISceneNodeAnimator7getTypeEv (returns 10)
    virtual std::uint32_t getType() const;
    // 0x5997bc _ZN6glitch5scene18ISceneNodeAnimator6onBindEPNS0_10ISceneNodeE (empty)
    virtual void onBind(ISceneNode* node);
    // 0x5997c0 _ZN6glitch5scene18ISceneNodeAnimator8onUnbindEPNS0_10ISceneNodeE (empty)
    virtual void onUnbind(ISceneNode* node);
    // 0x599878 _ZNK6glitch5scene18ISceneNodeAnimator9getLengthEv (returns 0)
    virtual std::uint32_t getLength() const;
    // 0x599880 _ZN6glitch5scene18ISceneNodeAnimator22computeAnimationValuesEj (empty)
    virtual void computeAnimationValues(std::uint32_t time);
    // 0x599884 _ZN6glitch5scene18ISceneNodeAnimator20applyAnimationValuesEj (empty)
    virtual void applyAnimationValues(std::uint32_t time);
};

class CEmptySceneNode {
public:
    // 0x35bea4 _ZNK6glitch5scene15CEmptySceneNode7getTypeEv (tag "emty")
    std::uint32_t getType() const;
    // 0x5839ac _ZN6glitch5scene15CEmptySceneNode6renderEPv (empty)
    void render(void* /*unused*/);
};

// Base camera scene node: no target node in the original's default.
class ICameraSceneNode {
public:
    // 0x35beb0 _ZNK6glitch5scene16ICameraSceneNode13getTargetNodeEv (returns null)
    ISceneNode* getTargetNode() const;
    // 0x35beb8 _ZN6glitch5scene16ICameraSceneNode13getTargetNodeEv (returns null)
    ISceneNode* getTargetNode();
};

class CCameraSceneNode {
public:
    // 0x581f34 _ZNK6glitch5scene16CCameraSceneNode7getTypeEv (tag "cam_")
    std::uint32_t getType() const;
    // 0x581f40 _ZN6glitch5scene16CCameraSceneNode23setInputReceiverEnabledEb
    void setInputReceiverEnabled(bool enabled);
    // 0x581f48 _ZNK6glitch5scene16CCameraSceneNode22isInputReceiverEnabledEv
    bool isInputReceiverEnabled() const;
    // 0x582130 _ZNK6glitch5scene16CCameraSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;

    bool inputReceiverEnabled = false;          // +0x165
    core::aabbox3d<float> boundingBox;          // +0x1d4
};

class CLightSceneNode {
public:
    // 0x583bec _ZNK6glitch5scene15CLightSceneNode7getTypeEv (tag "ligh")
    std::uint32_t getType() const;
};

class CMeshSceneNode {
public:
    // 0x584a98 _ZNK6glitch5scene14CMeshSceneNode7getTypeEv (tag "mesh")
    std::uint32_t getType() const;
};

class CSceneManagerRootNode {
public:
    // 0x588fdc _ZNK6glitch5scene21CSceneManagerRootNode7getTypeEv (tag "smrg")
    std::uint32_t getType() const;
};

class CSceneManager {
public:
    // 0x5890a8 _ZN6glitch5scene13CSceneManager23notifyVisibilityChangedEv
    void notifyVisibilityChanged();
    // 0x5890b4 _ZN6glitch5scene13CSceneManager22notifyHierarchyChangedEv
    // Sets the hierarchy flag and then raises the visibility flag too.
    void notifyHierarchyChanged();
    // 0x589558 _ZN6glitch5scene13CSceneManager16createMeshWriterENS0_17EMESH_WRITER_TYPEE
    // Returns null in the original.
    IMeshWriter* createMeshWriter(std::uint32_t writerType);

    bool hierarchyChanged = false;   // +0x288
    bool visibilityChanged = false;  // +0x289
};

class CBillboardSceneNode {
public:
    // 0x580a2c _ZNK6glitch5scene19CBillboardSceneNode7getTypeEv (tag "bill")
    std::uint32_t getType() const;
    // 0x580a98 _ZNK6glitch5scene19CBillboardSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x580b34 _ZNK6glitch5scene19CBillboardSceneNode16getMaterialCountEv (returns 1)
    std::uint32_t getMaterialCount() const;
    // 0x580b3c _ZNK6glitch5scene19CBillboardSceneNode7getSizeEv
    const core::dimension2d<float>& getSize() const;

    core::dimension2d<float> size;           // +0x134
    core::aabbox3d<float> boundingBox;       // +0x13c
};

class CBatchMesh {
public:
    // One batch segment. The index pair is copied out by getIndexRange; the
    // visible flag is the byte at +0x20 read by CBatchSceneNode::isSegmentVisible.
    struct SSegment {
        std::uint32_t indexBegin = 0;  // +0x10
        std::uint32_t indexEnd = 0;    // +0x14
        std::uint8_t visible = 0;      // +0x20
    };

    // 0x50cd24 _ZN6glitch5scene10CBatchMesh4initEv (empty)
    void init();
    // 0x50cd28 _ZNK6glitch5scene10CBatchMesh20saveSegmentExtraDataEPKvPNS_2io10IWriteFileEb (empty)
    void saveSegmentExtraData(const void* data, glitch::io::IWriteFile* file, bool flag) const;
    // 0x50cd10 _ZNK6glitch5scene10CBatchMesh13getIndexRangeERjS2_PKNS1_8SSegmentE
    void getIndexRange(std::uint32_t& first, std::uint32_t& second, const SSegment* segment) const;
};

class CBatchSceneNode {
public:
    // 0x50cd2c _ZNK6glitch5scene15CBatchSceneNode7getTypeEv (tag "batc")
    std::uint32_t getType() const;
    // 0x50cd38 _ZN6glitch5scene15CBatchSceneNode20preRegisterSceneNodeEv (empty)
    void preRegisterSceneNode();
    // 0x50cd3c _ZN6glitch5scene15CBatchSceneNode16isSegmentVisibleERKNS0_10CBatchMesh8SSegmentE
    bool isSegmentVisible(const CBatchMesh::SSegment& segment);
};

class IMesh {
public:
    // 0x596c6c _ZNK6glitch5scene5IMesh15getUserPropertyEv (returns null)
    const void* getUserProperty() const;
};

// Timeline controller base (scene namespace). The destructor body is empty in
// the original (D1 is a bare return).
class ITimeController {
public:
    // 0x666be8 _ZN6glitch5scene15ITimeControllerD1Ev (empty)
    virtual ~ITimeController() = default;
    // 0x666be4 _ZN6glitch5scene15ITimeController6updateEi (empty)
    virtual void update(int time);
};

// Timeline controller with a current frame; scale is fixed at 1.0.
class ITimelineController : public ITimeController {
public:
    // 0x666da4 _ZN6glitch5scene19ITimelineControllerD1Ev (empty)
    ~ITimelineController() override = default;
    // 0x666bfc _ZN6glitch5scene19ITimelineController6jumpToEi
    virtual void jumpTo(int frame);
    // 0x666c04 _ZNK6glitch5scene19ITimelineController7getLoopEv (returns false)
    virtual bool getLoop() const;
    // 0x666c0c _ZN6glitch5scene19ITimelineController8setScaleEf (empty)
    virtual void setScale(float scale);
    // 0x666c10 _ZNK6glitch5scene19ITimelineController8getScaleEv (returns 1.0f)
    virtual float getScale() const;

    int currentFrame = 0;  // +0x04
};

}  // namespace glitch::scene
