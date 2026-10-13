// Bodies of glitch::scene::CBatchSceneNode (canonical class: scene/CBatchSceneNode.hpp). This file replaces the CBatchSceneNode
// bodies of src/scene_nodes.cpp and the recon free-function stand-ins in src/recon/slice_05.cpp (getMesh, the render
// registration helpers), slice_31.cpp (constructors, attribute slots, setup) and src/recon/v2/wave_02.cpp (getMesh).
#include "glitch-scene-collada-gui/scene/CBatchSceneNode.hpp"

// CMaterial comes from the core-io stub that io/interfaces.hpp (through ISceneNode.hpp) declares; glitch-video/material.hpp
// cannot be included here until that stub is replaced.

namespace glitch::scene {

namespace {

const core::vector3d<float> kOrigin{0.0f, 0.0f, 0.0f};
const core::quaternion kIdentityRotation{0.0f, 0.0f, 0.0f, 1.0f};
const core::vector3d<float> kUnitScale{1.0f, 1.0f, 1.0f};

}  // namespace

// 0x57f9c0 _ZN6glitch5scene15CBatchSceneNodeC2Ei (C1 0x57fc04)
// ISceneNode(id, position 0, rotation (0,0,0,1), scale 1); the mesh is a new CBatchMesh holding one use (the constructor increments
// its count after operator new(0x7c)); +0x134 is 0, +0x138 is 2; setAutomaticCulling(2).
CBatchSceneNode::CBatchSceneNode(int nodeId) : IMeshSceneNode(nodeId, kOrigin, kIdentityRotation, kUnitScale) {
    mesh = boost::intrusive_ptr<CBatchMesh>(new CBatchMesh(), true);
    unk_134 = 0;
    unk_138 = 2;
    setAutomaticCulling(2);
}

// 0x57f0d4 _ZN6glitch5scene15CBatchSceneNodeC2EiRKN5boost13intrusive_ptrINS0_10CBatchMeshEEE (C1 0x57faf8)
// The mesh handle is copied (a use is added); +0x134 and +0x138 are 2.
CBatchSceneNode::CBatchSceneNode(int nodeId, const boost::intrusive_ptr<CBatchMesh>& batchMesh)
    : IMeshSceneNode(nodeId, kOrigin, kIdentityRotation, kUnitScale) {
    mesh = batchMesh;
    unk_134 = 2;
    unk_138 = 2;
    setAutomaticCulling(2);
}

// 0x57f874 _ZN6glitch5scene15CBatchSceneNodeC2EiRN5boost13intrusive_ptrINS0_10CBatchMeshEEEb (C1 0x57fd74)
// As above, with the byte at +0x148 set to the flag argument.
CBatchSceneNode::CBatchSceneNode(int nodeId, boost::intrusive_ptr<CBatchMesh>& batchMesh, bool flag)
    : IMeshSceneNode(nodeId, kOrigin, kIdentityRotation, kUnitScale) {
    mesh = batchMesh;
    unk_134 = 2;
    unk_138 = 2;
    flag148 = flag;
    setAutomaticCulling(2);
}

// 0x57fe84 _ZN6glitch5scene15CBatchSceneNodeC2EiRKN5boost13intrusive_ptrINS0_10CBatchMeshEEEPNS0_13CSceneManagerE (C1 0x57ffd4)
// As the (id, mesh) constructor, with the scene manager word (+0x110) stored directly (no setSceneManager walk).
CBatchSceneNode::CBatchSceneNode(int nodeId, const boost::intrusive_ptr<CBatchMesh>& batchMesh, CSceneManager* manager)
    : IMeshSceneNode(nodeId, kOrigin, kIdentityRotation, kUnitScale) {
    mesh = batchMesh;
    unk_134 = 2;
    unk_138 = 2;
    sceneManager = manager;
    setAutomaticCulling(2);
}

// 0x57f3ec _ZN6glitch5scene15CBatchSceneNodeD1Ev (0x57f344 D2, 0x57f49c D0)
// PARTIAL: the original deletes the entry array (+0x158), turns the visible-index cache off, drops the object at +0x14c and the
// mesh, then runs ~ISceneNode. Here the entry arrays are plain pointers that are not owned (nothing in this category allocates
// them), the cache flag is cleared and the handles drop themselves.
CBatchSceneNode::~CBatchSceneNode() {
    setVisibleIndexCache(false);
}

// 0x57e6f8 _ZNK6glitch5scene15CBatchSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// PARTIAL: the 236-byte writer (the batch mesh file name and the cache switch) is not reconstructed; the base attributes are
// written.
void CBatchSceneNode::serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) const {
    IMeshSceneNode::serializeAttributes(attributes, options);
}

// 0x57ed98 _ZN6glitch5scene15CBatchSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// PARTIAL: as above for the reader.
void CBatchSceneNode::deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) {
    IMeshSceneNode::deserializeAttributes(attributes, options);
}

// 0x57f4b8 _ZN6glitch5scene15CBatchSceneNode19onRegisterSceneNodeEv
// PARTIAL: the 588-byte body registers the node with the scene manager (solid and transparent lists) when it is visible; only
// the base registration result is returned.
int CBatchSceneNode::onRegisterSceneNode() {
    return ISceneNode::onRegisterSceneNode();
}

// 0x5808ac _ZN6glitch5scene15CBatchSceneNode6renderEPv
// PARTIAL: the 276-byte body draws the solid batches and flushes the transparent ones through the driver; not reconstructed.
void CBatchSceneNode::render(void* /*renderContext*/) {}

// 0x57e538 _ZNK6glitch5scene15CBatchSceneNode14getBoundingBoxEv
// The mesh's box (vtable +0x24 of the mesh at +0x130).
const core::aabbox3d<float>& CBatchSceneNode::getBoundingBox() const {
    return mesh->getBoundingBox();
}

// 0x57e524 _ZNK6glitch5scene15CBatchSceneNode25getTransformedBoundingBoxEv
// Calls getBoundingBox() (vtable +0x30 of this): the box is not transformed.
const core::aabbox3d<float>& CBatchSceneNode::getTransformedBoundingBox() const {
    return getBoundingBox();
}

// 0x57e570 _ZNK6glitch5scene15CBatchSceneNode11getMaterialEj
// The mesh's material at the index (vtable +0x18 of the mesh).
boost::intrusive_ptr<video::CMaterial> CBatchSceneNode::getMaterial(std::uint32_t index) const {
    return mesh->getMaterial(index);
}

// 0x57e554 _ZNK6glitch5scene15CBatchSceneNode16getMaterialCountEv
// The mesh's buffer count (vtable +0x10 of the mesh).
std::uint32_t CBatchSceneNode::getMaterialCount() const {
    return mesh->getMeshBufferCount();
}

// 0x50cd2c _ZNK6glitch5scene15CBatchSceneNode7getTypeEv
E_SCENE_NODE_TYPE CBatchSceneNode::getType() const {
    return ESNT_BATCH;  // 'basn'
}

// 0x57e5b0 _ZN6glitch5scene15CBatchSceneNode7setMeshERKN5boost13intrusive_ptrINS0_5IMeshEEE
// The original body is empty: the mesh handle is not stored.
void CBatchSceneNode::setMesh(const boost::intrusive_ptr<IMesh>& /*newMesh*/) {}

// 0x57e594 _ZNK6glitch5scene15CBatchSceneNode7getMeshEv
// The handle at +0x130 with a use added.
boost::intrusive_ptr<IMesh> CBatchSceneNode::getMesh() const {
    return boost::intrusive_ptr<IMesh>(mesh.get(), true);
}

// 0x57f074 _ZN6glitch5scene15CBatchSceneNode5setupEbPNS_5video12IVideoDriverE
// PARTIAL: not reconstructed (96 bytes: it forwards the flag and the driver to the mesh's initialisation).
void CBatchSceneNode::setup(bool /*flag*/, video::IVideoDriver* /*driver*/) {}

// 0x57ee34 _ZN6glitch5scene15CBatchSceneNode11postCompileEv
// PARTIAL: not reconstructed (576 bytes of cache set-up after the batches are compiled).
void CBatchSceneNode::postCompile() {}

// 0x50cd38 _ZN6glitch5scene15CBatchSceneNode20preRegisterSceneNodeEv (empty)
void CBatchSceneNode::preRegisterSceneNode() {}

// 0x50cd3c _ZN6glitch5scene15CBatchSceneNode16isSegmentVisibleERKNS0_10CBatchMesh8SSegmentE
bool CBatchSceneNode::isSegmentVisible(const CBatchMesh::SSegment& segment) {
    return segment.visible != 0;  // byte +0x20
}

// 0x57f704 _ZN6glitch5scene15CBatchSceneNode13updateIndicesEjRKN5boost13intrusive_ptrINS_5video7IBufferEEERKNS3_IKS5_EE
// PARTIAL: not reconstructed (352 bytes of index-buffer upload).
void CBatchSceneNode::updateIndices(std::uint32_t /*batch*/, const boost::intrusive_ptr<video::IBuffer>& /*buffer*/,
                                    const boost::intrusive_ptr<const video::IBuffer>& /*source*/) {}

// 0x57ed0c _ZN6glitch5scene15CBatchSceneNode20setVisibleIndexCacheEb
// Stores the switch (+0x150). Turning the cache on, from off, invalidates every batch when the mesh has batches (its batch
// vector at +0x20 .. +0x24 of CBatchMesh, 20-byte records). Turning it off, from on, frees the cache array (+0x154).
void CBatchSceneNode::setVisibleIndexCache(bool enabled) {
    const bool was = visibleIndexCacheOn;
    visibleIndexCacheOn = enabled;
    if (enabled) {
        if (!was && mesh && !mesh->batches.empty()) {
            invalidateVisibleIndexCache(0xFFFFFFFFu);
        }
    } else if (was) {
        delete[] visibleIndexCache;
        visibleIndexCache = nullptr;
    }
}

// 0x57e5b4 _ZN6glitch5scene15CBatchSceneNode15computeMaxIndicesEj
// PARTIAL: the original sums (end - begin) of the segments listed for the batch in the visible-entry array (+0x158). The entries are
// declared as 32-bit words (visibleEntries), but the list stores segment pointers, which are host width. Not reproduced; returns 0.
std::int32_t CBatchSceneNode::computeMaxIndices(std::uint32_t batch) {
    (void)batch;
    return 0;
}

// 0x57e604 _ZN6glitch5scene15CBatchSceneNode19clearVisibleSegmentsEv
// PARTIAL: the original zeroes the entry count of each visible segment record; the segment count comes from the mesh's virtual slot +0x10
// (not named) and the records share the layout blocked above. Not reproduced.
std::int32_t CBatchSceneNode::clearVisibleSegments() { return 0; }

// 0x57e650 _ZN6glitch5scene15CBatchSceneNode10updateInfoEjj
// For the visible-entry records [batch, segment) (20 bytes each: count, match, tick, flags, first): sums the counts and, for each
// non-empty record, sets flag bit 0 unless the match word equals the count. Returns the summed count.
// PARTIAL: the original also stores glitch::os::Timer::TickCount (not declared) in the tick word; that store is skipped.
std::int32_t CBatchSceneNode::updateInfo(std::uint32_t batch, std::uint32_t segment) {
    std::int32_t total = 0;
    if (batch >= segment || !visibleEntries) {
        return 0;
    }
    for (std::uint32_t i = batch; i < segment; ++i) {
        std::uint32_t* record = visibleEntries + 5 * i;
        const std::uint32_t count = record[0];
        total += static_cast<std::int32_t>(count);
        if (count) {
            record[3] = (record[1] == count) ? record[3] : (record[3] | 1u);
        }
    }
    return total;
}

// 0x57e800 _ZN6glitch5scene15CBatchSceneNode18registerSolidBatchesEv
// PARTIAL: not reconstructed (the solid batch registration walks the mesh segments and the visible-entry array).
void CBatchSceneNode::registerSolidBatches() {}

// 0x57ea70 _ZN6glitch5scene15CBatchSceneNode25registerTransparentBatchesEv
// PARTIAL: not reconstructed (as registerSolidBatches, for the transparent segments).
void CBatchSceneNode::registerTransparentBatches() {}

// 0x57ebec _ZN6glitch5scene15CBatchSceneNode26invalidateVisibleIndexCacheEj
// PARTIAL: not reconstructed (clears the cache entry of the batch, blocked by the entry layout above).
void CBatchSceneNode::invalidateVisibleIndexCache(std::uint32_t batch) {
    (void)batch;
}

// 0x57f21c _ZN6glitch5scene15CBatchSceneNode16renderBatchBBoxesEPNS_5video12IVideoDriverEj
// PARTIAL: not reconstructed (debug draw of the batch boxes through the driver).
void CBatchSceneNode::renderBatchBBoxes(video::IVideoDriver* driver, std::uint32_t batch) {
    (void)driver;
    (void)batch;
}

// 0x5800e8 _ZN6glitch5scene15CBatchSceneNode15renderSolidBatchEPNS_5video12IVideoDriverEj
// PARTIAL: not reconstructed (draws the visible segments of one solid batch through the driver).
void CBatchSceneNode::renderSolidBatch(video::IVideoDriver* driver, std::uint32_t batch) {
    (void)driver;
    (void)batch;
}

// 0x5804f0 _ZN6glitch5scene15CBatchSceneNode21flushTransparentBatchEPNS_5video12IVideoDriverE
// PARTIAL: not reconstructed (draws and clears the queued transparent segments).
void CBatchSceneNode::flushTransparentBatch(video::IVideoDriver* driver) {
    (void)driver;
}

// 0x5807a0 _ZN6glitch5scene15CBatchSceneNode22renderTransparentSegmentEPNS_5video12IVideoDriverERNS0_10CBatchMesh8SSegmentE
// PARTIAL: not reconstructed (draws one transparent segment through the driver).
void CBatchSceneNode::renderTransparentSegment(video::IVideoDriver* driver, CBatchMesh::SSegment& segment) {
    (void)driver;
    (void)segment;
}

}  // namespace glitch::scene
