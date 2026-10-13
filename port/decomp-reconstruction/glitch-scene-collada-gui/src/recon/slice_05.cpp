// Slice 05 reconstruction. Bodies marked partial are stubs: the members they
// read or write are not mapped in the shared scene headers yet. See the ledger
// notes for each address.
#include "glitch-scene-collada-gui/recon/slice_05.hpp"

namespace glitch::scene::recon_05 {

// 0x57e570 _ZN6glitch5scene15CBatchSceneNode11getMaterialEj
boost::intrusive_ptr<video::CMaterial> batchSceneNodeGetMaterial(const CBatchSceneNode& node, std::uint32_t index) {
    (void)node;
    (void)index;
    return {};
}

// 0x57e594 _ZNK6glitch5scene15CBatchSceneNode7getMeshEv
boost::intrusive_ptr<IMesh> batchSceneNodeGetMesh(const CBatchSceneNode& node) {
    (void)node;
    return {};
}

// 0x57e5b4 _ZN6glitch5scene15CBatchSceneNode15computeMaxIndicesEj
std::int32_t batchSceneNodeComputeMaxIndices(const CBatchSceneNode& node, std::uint32_t group) {
    (void)node;
    (void)group;
    return 0;
}

// 0x57e604 _ZN6glitch5scene15CBatchSceneNode20clearVisibleSegmentsEv
std::int32_t batchSceneNodeClearVisibleSegments(CBatchSceneNode& node) {
    (void)node;
    return 0;
}

// 0x57e650 _ZN6glitch5scene15CBatchSceneNode10updateInfoEjj
std::int32_t batchSceneNodeUpdateInfo(CBatchSceneNode& node, std::uint32_t first, std::uint32_t last) {
    (void)node;
    (void)first;
    (void)last;
    return 0;
}

// 0x57e800 _ZN6glitch5scene15CBatchSceneNode19registerSolidBatchesEv
void batchSceneNodeRegisterSolidBatches(CBatchSceneNode& node) {
    (void)node;
}

// 0x57e90c _ZNK6glitch5scene10CBatchMesh16getSegmentCenterEPv
core::vector3d<float> batchMeshGetSegmentCenter(const CBatchMesh& mesh, std::uint32_t segmentIndex) {
    (void)mesh;
    (void)segmentIndex;
    return {};
}

// 0x57ea70 _ZN6glitch5scene15CBatchSceneNode23registerTransparentBatchesEv
void batchSceneNodeRegisterTransparentBatches(CBatchSceneNode& node) {
    (void)node;
}

// 0x57ebec _ZN6glitch5scene15CBatchSceneNode27invalidateVisibleIndexCacheEj
void batchSceneNodeInvalidateVisibleIndexCache(CBatchSceneNode& node, std::uint32_t index) {
    (void)node;
    (void)index;
}

// 0x57ed0c _ZN6glitch5scene15CBatchSceneNode20setVisibleIndexCacheEb
void batchSceneNodeSetVisibleIndexCache(CBatchSceneNode& node, bool enabled) {
    (void)node;
    (void)enabled;
}

// 0x57ee34 _ZN6glitch5scene15CBatchSceneNode11postCompileEv
void batchSceneNodePostCompile(CBatchSceneNode& node) {
    (void)node;
}

// 0x57f21c _ZN6glitch5scene15CBatchSceneNode15renderBatchBBoxesEPNS_5video12IVideoDriverEj
void batchSceneNodeRenderBatchBBoxes(CBatchSceneNode& node, video::IVideoDriver* driver, std::uint32_t group) {
    (void)node;
    (void)driver;
    (void)group;
}

// 0x57f4b8 _ZN6glitch5scene15CBatchSceneNode20onRegisterSceneNodeEv
bool batchSceneNodeOnRegisterSceneNode(CBatchSceneNode& node) {
    (void)node;
    return false;
}

// 0x57f704 _ZN6glitch5scene15CBatchSceneNode13updateIndicesEjRKN5boost13intrusive_ptrINS_5video7IBufferEEERKNS4_IKS6_EE
void batchSceneNodeUpdateIndices(CBatchSceneNode& node, std::uint32_t group,
                                 const boost::intrusive_ptr<video::IBuffer>& indexBuffer,
                                 const boost::intrusive_ptr<const video::IBuffer>& vertexBuffer) {
    (void)node;
    (void)group;
    (void)indexBuffer;
    (void)vertexBuffer;
}

// 0x57f9c0 0x57fc04 _ZN6glitch5scene15CBatchSceneNodeC2Ei (constructor variants)
void batchSceneNodeConstruct(CBatchSceneNode& node, std::int32_t type) {
    (void)node;
    (void)type;
}

// 0x5800e8 _ZN6glitch5scene15CBatchSceneNode16renderSolidBatchEPNS_5video12IVideoDriverEj
void batchSceneNodeRenderSolidBatch(CBatchSceneNode& node, video::IVideoDriver* driver, std::uint32_t group) {
    (void)node;
    (void)driver;
    (void)group;
}

// 0x5804f0 _ZN6glitch5scene15CBatchSceneNode21flushTransparentBatchEPNS_5video12IVideoDriverE
void batchSceneNodeFlushTransparentBatch(CBatchSceneNode& node, video::IVideoDriver* driver) {
    (void)node;
    (void)driver;
}

// 0x5807a0 _ZN6glitch5scene15CBatchSceneNode24renderTransparentSegmentEPNS_5video12IVideoDriverERNS0_10CBatchMesh8SSegmentE
void batchSceneNodeRenderTransparentSegment(CBatchSceneNode& node, video::IVideoDriver* driver,
                                            CBatchMesh::SSegment& segment) {
    (void)node;
    (void)driver;
    (void)segment;
}

// 0x5808ac _ZN6glitch5scene15CBatchSceneNode6renderEPv
void batchSceneNodeRender(CBatchSceneNode& node, void* context) {
    (void)node;
    (void)context;
}

// 0x580a40 _ZN6glitch5scene19CBillboardSceneNode20onRegisterSceneNodeEv
bool billboardOnRegisterSceneNode(CBillboardSceneNode& node) {
    (void)node;
    return true;
}

// 0x580aa0 _ZN6glitch5scene19CBillboardSceneNode7setSizeERKNS_4core10dimension2dIfEE
void billboardSetSize(CBillboardSceneNode& node, const core::dimension2d<float>& size) {
    // The input is read before the node is written, so aliasing does not change the result.
    const float width = size.Width;
    const float height = size.Height;
    node.size = size;
    if (node.size.Width == 0.0f) {
        node.size.Width = 1.0f;
    }
    if (node.size.Height == 0.0f) {
        node.size.Height = 1.0f;
    }
    const float radius = (width + height) / 6.0f;
    node.boundingBox.MinEdge = {-radius, -radius, -radius};
    node.boundingBox.MaxEdge = {radius, radius, radius};
}

// 0x580b24 _ZN6glitch5scene19CBillboardSceneNode11getMaterialEj
boost::intrusive_ptr<video::CMaterial>* billboardGetMaterial(CBillboardSceneNode& node, std::uint32_t index) {
    (void)node;
    (void)index;
    return nullptr;
}

// 0x580b44 _ZN6glitch5scene19CBillboardSceneNode8setColorERKNS_5video6SColorE
void billboardSetColor(CBillboardSceneNode& node, const video::SColor& color) {
    (void)node;
    (void)color;
}

// 0x580b94 _ZN6glitch5scene19CBillboardSceneNode8setColorERKNS_5video6SColorES5_
void billboardSetColor(CBillboardSceneNode& node, const video::SColor& top, const video::SColor& bottom) {
    (void)node;
    (void)top;
    (void)bottom;
}

// 0x580e68 _ZN6glitch5scene19CBillboardSceneNode11setMaterialERKN5boost13intrusive_ptrINS_5video9CMaterialEEE
void billboardSetMaterial(CBillboardSceneNode& node, const boost::intrusive_ptr<video::CMaterial>& material) {
    (void)node;
    (void)material;
}

// 0x580f58 _ZN6glitch5scene19CBillboardSceneNode6renderEPv
void billboardRender(CBillboardSceneNode& node, void* context) {
    (void)node;
    (void)context;
}

// 0x5818fc 0x581c80 _ZN6glitch5scene19CBillboardSceneNodeC2EPNS_5video12IVideoDriverEi (constructor variants)
void billboardConstruct(CBillboardSceneNode& node, video::IVideoDriver* driver, std::int32_t type) {
    (void)node;
    (void)driver;
    (void)type;
}

// 0x581bc4 _ZNK6glitch5scene19CBillboardSceneNode5cloneEv
CBillboardSceneNode* billboardClone(const CBillboardSceneNode& node) {
    (void)node;
    return nullptr;
}

// 0x582290 _ZN6glitch5scene16CCameraSceneNode6removeEv
bool cameraRemove(CCameraSceneNode& node) {
    (void)node;
    return false;
}

// 0x582aa0 _ZN6glitch5scene16CCameraSceneNode21onChangedSceneManagerEv
bool cameraOnChangedSceneManager(CCameraSceneNode& node) {
    (void)node;
    return false;
}

// 0x5839d8 0x583b34 _ZN6glitch5scene15CEmptySceneNodeC2Ei (constructor variants)
void emptySceneNodeConstruct(CEmptySceneNode& node, std::int32_t type) {
    (void)node;
    (void)type;
}

// 0x583ad0 _ZNK6glitch5scene15CEmptySceneNode5cloneEv
CEmptySceneNode* emptySceneNodeClone(const CEmptySceneNode& node) {
    (void)node;
    return nullptr;
}

// 0x583e30 0x583fbc _ZN6glitch5scene15CLightSceneNodeC2ERS1_ (copy constructor variants)
void lightSceneNodeCopyConstruct(CLightSceneNode& node, const CLightSceneNode& source) {
    (void)node;
    (void)source;
}

// 0x583f94 _ZNK6glitch5scene15CLightSceneNode5cloneEv
CLightSceneNode* lightSceneNodeClone(const CLightSceneNode& node) {
    (void)node;
    return nullptr;
}

// 0x5840f0 0x584238 _ZN6glitch5scene15CLightSceneNodeC2Eb (constructor variants)
void lightSceneNodeConstruct(CLightSceneNode& node, bool flag) {
    (void)node;
    (void)flag;
}

// 0x584aa4 _ZNK6glitch5scene14CMeshSceneNode7getMeshEv
boost::intrusive_ptr<IMesh> meshSceneNodeGetMesh(const CMeshSceneNode& node) {
    (void)node;
    return {};
}

// 0x584ac0 _ZNK6glitch5scene14CMeshSceneNode14getBoundingBoxEv
core::aabbox3d<float> meshSceneNodeGetBoundingBox(const CMeshSceneNode& node) {
    (void)node;
    return {};
}

// 0x584adc _ZNK6glitch5scene14CMeshSceneNode11getMaterialEj
boost::intrusive_ptr<video::CMaterial> meshSceneNodeGetMaterial(const CMeshSceneNode& node, std::uint32_t index) {
    (void)node;
    (void)index;
    return {};
}

// 0x584b44 _ZNK6glitch5scene14CMeshSceneNode16getMaterialCountEv
std::int32_t meshSceneNodeGetMaterialCount(const CMeshSceneNode& node) {
    (void)node;
    return 0;
}

// 0x584d2c _ZN6glitch5scene14CMeshSceneNode20onRegisterSceneNodeEv
bool meshSceneNodeOnRegisterSceneNode(CMeshSceneNode& node) {
    (void)node;
    return false;
}

// 0x5850ec _ZN6glitch5scene14CMeshSceneNode7setMeshERKN5boost13intrusive_ptrINS0_5IMeshEEE
void meshSceneNodeSetMesh(CMeshSceneNode& node, const boost::intrusive_ptr<IMesh>& mesh) {
    (void)node;
    (void)mesh;
}

// 0x58521c _ZNK6glitch5scene14CMeshSceneNode5cloneEv
ISceneNode* meshSceneNodeClone(const CMeshSceneNode& node) {
    (void)node;
    return nullptr;
}

// 0x58529c _ZN6glitch5scene14CMeshSceneNodeC2ERKN5boost13intrusive_ptrINS0_5IMeshEEEiRKNS_4core8vector3dIfEERKNS5_10quaternionES9_
void meshSceneNodeConstruct(CMeshSceneNode& node, const boost::intrusive_ptr<IMesh>& mesh, std::int32_t type) {
    (void)node;
    (void)mesh;
    (void)type;
}

// 0x5872a4 _ZNK6glitch5scene24COctTreeTriangleSelector25getTrianglesFromOctTreeLineEPNS1_12SOctTreeNodeE
std::int32_t octTreeGetTrianglesFromLine(const void* selector, const void* node) {
    (void)selector;
    (void)node;
    return 0;
}

// 0x588654 0x5887ec _ZN6glitch5scene24COctTreeTriangleSelectorC2EPKNS0_5IMeshEPKNS0_10ISceneNodeEib (constructor variants)
void octTreeSelectorConstruct(void* selector, const IMesh* mesh, const ISceneNode* node, std::int32_t depth,
                              bool flag) {
    (void)selector;
    (void)mesh;
    (void)node;
    (void)depth;
    (void)flag;
}

// 0x588984 _ZNK6glitch5scene16CTriangleSelector5SetupERKNS_4core8line3dIfEE
bool triangleSelectorSetup(const void* selector, const void* line) {
    (void)selector;
    (void)line;
    return false;
}

// 0x588d8c _ZNK6glitch5scene24COctTreeTriangleSelector12getTrianglesEPNS_4core10triangle3dIfEEiRiRKNS2_8line3dIfEEPKNS2_11CMatrix4IfEE
void octTreeGetTriangles(const void* selector, void* triangles, std::int32_t capacity, std::int32_t& count,
                         const void* line, const void* transform) {
    (void)selector;
    (void)triangles;
    (void)capacity;
    (void)line;
    (void)transform;
    count = 0;
}

// 0x588e30 _ZN6glitch5scene10ISceneNode15setSceneManagerEPNS0_13CSceneManagerE
void sceneNodeSetSceneManager(ISceneNode& node, CSceneManager* manager) {
    (void)node;
    (void)manager;
}

// 0x588e80 _ZN6glitch5scene10CBatchMesh8SSegment16setSourceBufferEPKNS0_11CMeshBufferEPKNS_4core8aabbox3dIfEE
void batchSegmentSetSourceBuffer(CBatchMesh::SSegment& segment, const void* meshBuffer,
                                 const core::aabbox3d<float>* box) {
    (void)segment;
    (void)meshBuffer;
    (void)box;
}

// 0x589208 _ZN6glitch5scene13CSceneManager16getSceneNodeFromIdEiPNS0_10ISceneNodeE
ISceneNode* sceneManagerGetSceneNodeFromId(CSceneManager& manager, std::int32_t id, ISceneNode* root) {
    (void)manager;
    (void)id;
    return root;
}

// 0x589298 _ZN6glitch5scene13CSceneManager19getSceneNodeFromTypeENS0_17E_SCENE_NODE_TYPEEPNS0_10ISceneNodeE
ISceneNode* sceneManagerGetSceneNodeFromType(CSceneManager& manager, std::int32_t type, ISceneNode* root) {
    (void)manager;
    (void)type;
    return root;
}

// 0x589d58 _ZN6glitch5scene13CSceneManager19getSceneNodeFromNameEPKcPNS0_10ISceneNodeE
ISceneNode* sceneManagerGetSceneNodeFromName(CSceneManager& manager, const char* name, ISceneNode* root) {
    (void)manager;
    (void)name;
    return root;
}

// 0x589d48 _ZN6glitch5scene13CSceneManager19getSceneNodeFromUIDEPKcPNS0_10ISceneNodeE
ISceneNode* sceneManagerGetSceneNodeFromUid(CSceneManager& manager, const char* uid, ISceneNode* root) {
    (void)manager;
    (void)uid;
    return root;
}

// 0x5890c0 _ZN6glitch5scene13CSceneManager15setActiveCameraEPNS0_16ICameraSceneNodeE
void sceneManagerSetActiveCamera(CSceneManager& manager, ICameraSceneNode* camera) {
    (void)manager;
    (void)camera;
}

// 0x589328 _ZN6glitch5scene13CSceneManager13postEventFromUserERKNS_6SEventE
bool sceneManagerPostEventFromUser(CSceneManager& manager, const glitch::SEvent& event) {
    (void)manager;
    (void)event;
    return false;
}

// 0x589364 _ZN6glitch5scene13CSceneManager9saveSceneEPKcPNS0_22ISceneUserDataSerializerE
bool sceneManagerSaveScene(CSceneManager& manager, const char* filename, void* serializer) {
    (void)manager;
    (void)filename;
    (void)serializer;
    return false;
}

// 0x5893c0 _ZNK6glitch5scene13CSceneManager20getSceneNodeTypeNameENS0_17E_SCENE_NODE_TYPEE
const char* sceneManagerGetSceneNodeTypeName(const CSceneManager& manager, std::int32_t type) {
    (void)manager;
    (void)type;
    return nullptr;
}

// 0x58942c _ZN6glitch5scene13CSceneManager12addSceneNodeEPKcPNS0_10ISceneNodeE
bool sceneManagerAddSceneNode(CSceneManager& manager, const char* name, ISceneNode* node) {
    (void)manager;
    (void)name;
    (void)node;
    return false;
}

// 0x5894a4 _ZNK6glitch5scene13CSceneManager20getAnimatorTypeNameENS0_25E_SCENE_NODE_ANIMATOR_TYPEE
const char* sceneManagerGetAnimatorTypeName(const CSceneManager& manager, std::int32_t type) {
    (void)manager;
    (void)type;
    return nullptr;
}

// 0x589754 _ZN6glitch5scene13CSceneManager24removeShadowReceiverTargetEN5boost13intrusive_ptrINS_5video8ITextureEEE
void sceneManagerRemoveShadowReceiverTarget(CSceneManager& manager,
                                            const boost::intrusive_ptr<video::ITexture>& texture) {
    (void)manager;
    (void)texture;
}

// 0x5897d4 _ZN6glitch5scene13CSceneManager24removeShadowReceiverTargetEN5boost13intrusive_ptrINS0_21IShadowReceiverTargetEEE
void sceneManagerRemoveShadowReceiverTarget(CSceneManager& manager,
                                            const boost::intrusive_ptr<IShadowReceiverTarget>& target) {
    (void)manager;
    (void)target;
}

// 0x589888 _ZN6glitch5scene13CSceneManager5clearEv
void sceneManagerClear(CSceneManager& manager) {
    (void)manager;
}

// 0x589b84 _ZN6glitch5scene13CSceneManager9loadSceneEPKcPNS0_22ISceneUserDataSerializerE
bool sceneManagerLoadScene(CSceneManager& manager, const char* filename, void* serializer) {
    (void)manager;
    (void)filename;
    (void)serializer;
    return false;
}

// 0x589e18 _ZN6glitch5scene13CSceneManager11setupCameraEv
void sceneManagerSetupCamera(CSceneManager& manager) {
    (void)manager;
}

// 0x589e7c _ZN6glitch5scene13CSceneManager8drawInitEPNS_5video12IVideoDriverE
void sceneManagerDrawInit(CSceneManager& manager, video::IVideoDriver* driver) {
    (void)manager;
    (void)driver;
}

// 0x589fa4 _ZN6glitch5scene13CSceneManager24registerSceneNodesCompileEPNS0_10ISceneNodeE
void sceneManagerRegisterSceneNodesCompile(CSceneManager& manager, ISceneNode* node) {
    (void)manager;
    (void)node;
}

// 0x58a070 _ZN6glitch5scene19CSceneManagerRootNodeC2EPNS0_13CSceneManagerE
void sceneManagerRootNodeConstruct(CSceneManagerRootNode& node, CSceneManager* manager) {
    (void)node;
    (void)manager;
}

// 0x589de8 _ZN6glitch17intrusive_ptr_releaseEPKNS_22ISharedMemoryBlockHeaderINS_5video9CMaterialEEE
void intrusivePtrRelease(video::CMaterial* material) {
    (void)material;
}

}  // namespace glitch::scene::recon_05
