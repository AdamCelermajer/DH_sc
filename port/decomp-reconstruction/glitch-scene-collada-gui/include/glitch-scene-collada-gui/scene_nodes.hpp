// glitch::scene node, animator and manager accessors. Each class maps only the
// members its accessors touch; base classes and the rest of the layout are not
// reconstructed yet. Offsets are kept as comments; the code uses typed members.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/scene_node.hpp"

namespace glitch {
struct SEvent;
}
namespace glitch::io {
class IWriteFile;
}
namespace glitch::video {
class CLight;
}

namespace glitch::scene {

class IMeshWriter;
class IMesh;
struct SViewFrustum;
class ITimelineController;

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

    // 0x599868 _ZNK6glitch5scene18ISceneNodeAnimator15getTimelineCtrlEv (returns the +0x08 pointer)
    ITimelineController* getTimelineCtrl() const;
    // 0x599870 _ZN6glitch5scene18ISceneNodeAnimator15getTimelineCtrlEv (returns the +0x08 pointer)
    ITimelineController* getTimelineCtrl();

    // +0x08: timeline controller pointer, written by setTimelineCtrl at 0x599818 (which holds a
    // reference on it). The 32-bit +0x04 word is not mapped, so no pad is declared here and the
    // 64-bit offset of this member is not the original's.
    ITimelineController* timelineCtrl = nullptr;
};

class CEmptySceneNode {
public:
    // 0x35bea4 _ZNK6glitch5scene15CEmptySceneNode7getTypeEv (tag "emty")
    std::uint32_t getType() const;
    // 0x5839ac _ZN6glitch5scene15CEmptySceneNode6renderEPv (empty)
    void render(void* /*unused*/);
    // 0x5839b0 _ZNK6glitch5scene15CEmptySceneNode14getBoundingBoxEv (returns the box at +0x130)
    const core::aabbox3d<float>& getBoundingBox() const;

    // Base region below +0x130 (ISceneNode base and the vptr words) is not mapped yet.
    std::uint8_t pad_0x00[0x130];
    core::aabbox3d<float> boundingBox;  // +0x130..+0x147 (min at +0x130, max at +0x13c)
};

// Base camera scene node: no target node in the original's default.
class ICameraSceneNode {
public:
    // 0x35beb0 _ZNK6glitch5scene16ICameraSceneNode13getTargetNodeEv (returns null)
    ISceneNode* getTargetNode() const;
    // 0x35beb8 _ZN6glitch5scene16ICameraSceneNode13getTargetNodeEv (returns null)
    ISceneNode* getTargetNode();
    // 0x35bec0 _ZNK6glitch5scene16ICameraSceneNode12isOrthogonalEv (returns the flag byte at +0x134)
    bool isOrthogonal() const;

    // Base region below +0x134 (ISceneNode base and the vptr word at +0x130) is not mapped yet.
    std::uint8_t pad_0x00[0x134];
    std::uint8_t orthogonal = 0;  // +0x134 width=1 (ctor 0x582b18 writes 0)
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
    // 0x581f50 _ZNK6glitch5scene16CCameraSceneNode19getProjectionMatrixEv
    const core::CMatrix4<float>& getProjectionMatrix() const;
    // 0x581f58 _ZNK6glitch5scene16CCameraSceneNode13getViewMatrixEv
    const core::CMatrix4<float>& getViewMatrix() const;
    // 0x581fe4 _ZN6glitch5scene16CCameraSceneNode9setTargetERKNS_4core8vector3dIfEE
    void setTarget(const core::vector3d<float>& target);
    // 0x582000 _ZNK6glitch5scene16CCameraSceneNode9getTargetEv
    const core::vector3d<float>& getTarget() const;
    // 0x582008 _ZN6glitch5scene16CCameraSceneNode11setUpVectorERKNS_4core8vector3dIfEE
    void setUpVector(const core::vector3d<float>& upVector);
    // 0x582024 _ZNK6glitch5scene16CCameraSceneNode11getUpVectorEv
    const core::vector3d<float>& getUpVector() const;
    // 0x58202c _ZNK6glitch5scene16CCameraSceneNode12getNearValueEv
    float getNearValue() const;
    // 0x582034 _ZNK6glitch5scene16CCameraSceneNode11getFarValueEv
    float getFarValue() const;
    // 0x58203c _ZNK6glitch5scene16CCameraSceneNode14getAspectRatioEv
    float getAspectRatio() const;
    // 0x582044 _ZNK6glitch5scene16CCameraSceneNode6getFOVEv
    float getFOV() const;
    // 0x58204c _ZN6glitch5scene16CCameraSceneNode17getFarToInifinityEv
    bool getFarToInifinity();
    // 0x582054 _ZN6glitch5scene16CCameraSceneNode12setNearValueEf
    void setNearValue(float value);
    // 0x58206c _ZN6glitch5scene16CCameraSceneNode11setFarValueEf
    void setFarValue(float value);
    // 0x582084 _ZN6glitch5scene16CCameraSceneNode14setAspectRatioEf
    void setAspectRatio(float value);
    // 0x58209c _ZN6glitch5scene16CCameraSceneNode6setFOVEf
    void setFOV(float value);
    // 0x5820b4 _ZN6glitch5scene16CCameraSceneNode6setMAGEf
    void setMAG(float value);
    // 0x5820cc _ZN6glitch5scene16CCameraSceneNode17setFarToInifinityEb
    void setFarToInifinity(bool value);
    // 0x5820e4 _ZN6glitch5scene16CCameraSceneNode6renderEPv
    void render(void* /*unused*/);
    // 0x582138 _ZNK6glitch5scene16CCameraSceneNode14getViewFrustumEv
    SViewFrustum* getViewFrustum() const;
    // 0x5823ac _ZN6glitch5scene16CCameraSceneNode19setProjectionMatrixERKNS_4core8CMatrix4IfEEb
    void setProjectionMatrix(const core::CMatrix4<float>& matrix, bool isOrthogonal);

    // Fields in offset order. The base region (ISceneNode, vptr at +0x130) is not mapped here.
    std::uint8_t orthogonal = 0;                // +0x134 (ICameraSceneNode flag byte, written by setProjectionMatrix)
    core::vector3d<float> target;               // +0x138
    core::vector3d<float> upVector;             // +0x144
    float mag = 0.0f;                           // +0x150
    float fov = 0.0f;                           // +0x154
    float aspectRatio = 0.0f;                   // +0x158
    float nearValue = 0.0f;                     // +0x15c
    float farValue = 0.0f;                      // +0x160
    bool farToInfinity = false;                 // +0x164
    bool inputReceiverEnabled = false;          // +0x165
    // SViewFrustum at +0x168 is not mapped (type and size unknown).
    core::aabbox3d<float> boundingBox;          // +0x1d4
    core::CMatrix4<float> viewMatrix;           // +0x1ec
    core::CMatrix4<float> projectionMatrix;     // +0x274
};

class CLightSceneNode {
public:
    // 0x583bec _ZNK6glitch5scene15CLightSceneNode7getTypeEv (tag "lght")
    std::uint32_t getType() const;
    // 0x583c18 _ZN6glitch5scene15CLightSceneNode13doLightRecalcEv
    // Partial: needs glitch::video::CLight fields at +0x40 (float range, FLT_MAX means
    // no culling box) and +0x58 (uint16 count). CLight has no definition in this category.
    void doLightRecalc();
    // 0x583cf0 _ZNK6glitch5scene15CLightSceneNode14getBoundingBoxEv
    // Partial: compares recalcCount with light->+0x58, which is not reachable here.
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x583d18 _ZN6glitch5scene15CLightSceneNode6renderEPv
    // Partial: the driver is read from the scene manager at +0x110 (+0x14 of it), and that
    // type is not mapped. The call is IVideoDriver::addDynamicLight(const intrusive_ptr<CLight>&).
    int render(void* arg);
    // 0x583d34 _ZN6glitch5scene15CLightSceneNode19onRegisterSceneNodeEv
    // Partial: calls vtable slot +36 of the scene manager at +0x110. The slot's target
    // and the manager's class are not identified.
    int onRegisterSceneNode();

    // Layout (original offsets). The base class is not reconstructed yet, so only these
    // words are mapped here.
    boost::intrusive_ptr<glitch::video::CLight> light;  // +0x134 (word 77), passed by address to addDynamicLight
    std::uint32_t recalcCount = 0;                      // +0x138 (word 78), last light count seen by doLightRecalc
    core::aabbox3d<float> boundingBox;                  // +0x13c..+0x154 (words 79..84), min then max
};

class CMeshSceneNode {
public:
    // Default state used by scene_nodes_test.cpp. The original has no default constructor.
    CMeshSceneNode() = default;
    // 0x585118 _ZN6glitch5scene14CMeshSceneNodeC2ERKN5boost13intrusive_ptrINS0_5IMeshEEEiRKNS_4core8vector3dIfEERKNS5_10quaternionES9_
    // PARTIAL: the ISceneNode base constructor is not reconstructed, so id, position, rotation and scale are not forwarded.
    CMeshSceneNode(const boost::intrusive_ptr<IMesh>& meshValue, int id, const core::vector3d<float>& position,
                   const core::quaternion& rotation, const core::vector3d<float>& scale);
    // 0x584a98 _ZNK6glitch5scene14CMeshSceneNode7getTypeEv (tag "mesh")
    std::uint32_t getType() const;
    // 0x585370 _ZN6glitch5scene14CMeshSceneNode6renderEPv
    // PARTIAL: the mesh-buffer loop and the driver calls are not reconstructed (see the source file).
    void render(void* context);

    boost::intrusive_ptr<IMesh> mesh;  // +0x130 (reference counted in the original)
    std::uint32_t renderCount = 0;     // +0x134 (incremented by render)
    std::uint32_t field13c = 1;        // +0x13c (set to 1 by the constructor)
};

class CSceneManagerRootNode {
public:
    // 0x588fdc _ZNK6glitch5scene21CSceneManagerRootNode7getTypeEv (tag "smgr")
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

    // 0x5891ac _ZN6glitch5scene13CSceneManager14setShadowColorENS_5video6SColorE
    // Writes the colour into all four shadow colour slots.
    void setShadowColor(video::SColor color);
    // 0x589354 _ZN6glitch5scene13CSceneManager22setSceneNodeRenderPassENS0_24E_SCENE_NODE_RENDER_PASSE
    // Returns the previous render pass.
    std::uint32_t setSceneNodeRenderPass(std::uint32_t pass);
    // 0x589508 _ZN6glitch5scene13CSceneManager15setAmbientLightERKNS_5video7SColorfE
    void setAmbientLight(const video::SColorf& color);
    // 0x589518 _ZNK6glitch5scene13CSceneManager22getCurrentRenderedNodeEPPvPi
    // The node and the second word are stored through the pointers when they are not null.
    std::uint32_t getCurrentRenderedNode(void** node, std::int32_t* second) const;
    // 0x589538 _ZNK6glitch5scene13CSceneManager19getNextRenderedNodeEPPvPi
    std::uint32_t getNextRenderedNode(void** node, std::int32_t* second) const;

    // 0x58ab28 _ZNK6glitch5scene13CSceneManager8isCulledEPKNS0_10ISceneNodeE (partial, see the .cpp)
    bool isCulled(const ISceneNode* node) const;
    // 0x58b728 _ZN6glitch5scene13CSceneManager7drawAllERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEE (partial)
    void drawAll(const std::vector<ISceneNode*>& nodes);
    // 0x58b7f4 _ZN6glitch5scene13CSceneManager7drawAllEPNS0_10ISceneNodeE (partial)
    void drawAll(ISceneNode* node);
    // 0x58f348 _ZN6glitch5scene13CSceneManager24registerNodeForRenderingEPNS0_10ISceneNodeERKN5boost13intrusive_ptrINS_5video9CMaterialEEEPvNS0_24E_SCENE_NODE_RENDER_PASSEPKNS_4core8vector3dIfEEi (partial)
    bool registerNodeForRendering(ISceneNode* node, const boost::intrusive_ptr<video::CMaterial>& material,
                                  void* userData, std::uint32_t pass, const core::vector3d<float>* position,
                                  int sortOrder);

    video::SColor shadowColors[4];         // +0xf4..+0x103 (four colour words)
    video::SColorf ambientLight;           // +0x104..+0x113
    void* currentRenderedNode = nullptr;   // +0xa0 (written by getCurrentRenderedNode)
    std::int32_t currentRenderedSecond = 0;  // +0xa4
    std::uint32_t currentRenderedWord = 0;   // +0x9c (returned)
    void* nextRenderedNode = nullptr;      // +0xac (written by getNextRenderedNode)
    std::int32_t nextRenderedSecond = 0;   // +0xb0
    std::uint32_t nextRenderedWord = 0;    // +0xa8 (returned)
    std::uint32_t renderPass = 0;          // +0x174
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

class IMesh;

class CBatchMesh {
public:
    // One batch record, 0x14 bytes in the original. Only the first three words (the
    // object pointers) and the three 16-bit fields that the constructors zero are
    // named; their meaning is not mapped.
    struct SBatch {
        void* meshBuffer = nullptr;          // +0x00
        void* material = nullptr;            // +0x04
        void* vertexAttributeMap = nullptr;  // +0x08
        std::uint16_t field0C = 0;           // +0x0c
        std::uint16_t field0E = 0;           // +0x0e
        std::uint16_t field10 = 0;           // +0x10
        // 0x578d1c _ZN6glitch5scene10CBatchMesh6SBatchC2Ev (zeroes every field)
        SBatch() = default;
        // 0x578d3c _ZN6glitch5scene10CBatchMesh6SBatchC1Ev
        // 0x578d5c _ZN6glitch5scene10CBatchMesh6SBatchC2Et
        explicit SBatch(std::uint16_t value);
    };

    // 0x578c1c _ZNK6glitch5scene10CBatchMesh5cloneEv (null)
    boost::intrusive_ptr<IMesh> clone() const;
    // 0x578c28 _ZNK6glitch5scene10CBatchMesh18getMeshBufferCountEv
    std::int32_t getMeshBufferCount() const;
    // 0x57aa8c _ZNK6glitch5scene10CBatchMesh14getBoundingBoxEv (partial: refresh call not reconstructed)
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x578c78 _ZN6glitch5scene10CBatchMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE (partial: stores a global word at +0x6c)
    void setBoundingBox(const core::aabbox3d<float>& box);

    std::vector<SBatch> batches;           // +0x20 (begin) .. +0x24 (end)
    core::aabbox3d<float> boundingBox;     // +0x50..+0x67
    void* boundingBoxSource = nullptr;     // +0x68 (checked by getBoundingBox)
    std::uint32_t boundingBoxStamp = 0;    // +0x6c (set by setBoundingBox)
    bool boundingBoxDirty = false;         // +0x74
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
    // 0x50cd2c _ZNK6glitch5scene15CBatchSceneNode7getTypeEv (tag "basn")
    std::uint32_t getType() const;
    // 0x50cd38 _ZN6glitch5scene15CBatchSceneNode20preRegisterSceneNodeEv (empty)
    void preRegisterSceneNode();
    // 0x57e5b0 _ZN6glitch5scene15CBatchSceneNode7setMeshERKN5boost13intrusive_ptrINS0_5IMeshEEE (empty)
    void setMesh(const boost::intrusive_ptr<IMesh>& mesh);
    // 0x50cd3c _ZN6glitch5scene15CBatchSceneNode16isSegmentVisibleERKNS0_10CBatchMesh8SSegmentE
    bool isSegmentVisible(const CBatchMesh::SSegment& segment);
};

class IMesh {
public:
    // 0x596c68 _ZN6glitch5scene5IMeshD1Ev (empty)
    ~IMesh() = default;
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
