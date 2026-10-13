// Bodies of glitch::scene::CBatchMesh and CBatchMesh::SBatch (canonical class: scene/CBatchMesh.hpp). This file replaces the
// CBatchMesh bodies of src/scene_nodes.cpp and the recon free-function stand-ins in src/recon/v2/wave_00.cpp (setBoundingBox,
// getBoundingBox), wave_02.cpp (constructors, getMeshBuffer) and wave_13.cpp (loadSegmentExtraData).
#include "glitch-scene-collada-gui/scene/CBatchMesh.hpp"

#include "glitch-core-io/os/logging.hpp"
#include "glitch-video/material.hpp"
#include "glitch-video/material_vertex_attribute_map.hpp"
#include "glitch-scene-collada-gui/scene/CMeshBuffer.hpp"

namespace glitch::scene {

// ---------------------------------------------------------------------------------------------------------------------
// SBatch
// ---------------------------------------------------------------------------------------------------------------------

// 0x578d1c _ZN6glitch5scene10CBatchMesh6SBatchC2Ev
// 0x578d3c _ZN6glitch5scene10CBatchMesh6SBatchC1Ev
// Zeroes every field (the member initialisers).
CBatchMesh::SBatch::SBatch() = default;

// 0x578d5c _ZN6glitch5scene10CBatchMesh6SBatchC2Et
// 0x578d7c _ZN6glitch5scene10CBatchMesh6SBatchC1Et
// Both 16-bit range words (+0x0c, +0x0e) take the value; +0x10 is zeroed.
CBatchMesh::SBatch::SBatch(std::uint16_t value) : firstSegmentInfo(value), endSegmentInfo(value), field10(0) {}

CBatchMesh::SBatch::SBatch(const SBatch& other) = default;
CBatchMesh::SBatch& CBatchMesh::SBatch::operator=(const SBatch& other) = default;
// The destructor drops the vertex-attribute map, the material and the mesh buffer (the handles do it).
CBatchMesh::SBatch::~SBatch() = default;

// ---------------------------------------------------------------------------------------------------------------------
// CBatchMesh
// ---------------------------------------------------------------------------------------------------------------------

// 0x578924 _ZN6glitch5scene10CBatchMeshC2Ev
// 0x5789e4 _ZN6glitch5scene10CBatchMeshC1Ev
// The reference count starts at 0; the two boxes are the unit box; the stamp is -1, the stride 44, the dirty byte 1. Every other
// word is zero (the member initialisers).
CBatchMesh::CBatchMesh() {
    referenceCounter = 0;
}

// 0x578aa4 _ZN6glitch5scene10CBatchMeshC2Ej
// 0x578b60 _ZN6glitch5scene10CBatchMeshC1Ej
// Same as the default constructor with the stride (+0x70) set to the argument.
CBatchMesh::CBatchMesh(std::uint32_t stride) {
    referenceCounter = 0;
    segmentStride = stride;
}

// 0x57a59c _ZN6glitch5scene10CBatchMeshD1Ev (D2 0x57a620; D0 0x57a604 skipped)
// Frees the block at +0x2c, destroys the batch vector (each batch drops its three handles), frees the blocks at +0x14 and +0x08.
CBatchMesh::~CBatchMesh() = default;

// 0x578c1c _ZNK6glitch5scene10CBatchMesh5cloneEv
// Returns a null handle (the original stores 0 into the result slot).
boost::intrusive_ptr<IMesh> CBatchMesh::clone() const {
    return boost::intrusive_ptr<IMesh>();
}

// 0x578c28 _ZNK6glitch5scene10CBatchMesh18getMeshBufferCountEv
// (end - begin) / 20 in the original, the element count here.
std::uint32_t CBatchMesh::getMeshBufferCount() const {
    return static_cast<std::uint32_t>(batches.size());
}

// 0x578c50 _ZNK6glitch5scene10CBatchMesh13getMeshBufferEj
// The first word of the batch record, with a use added. The original does not test the index; out of range gives null here.
boost::intrusive_ptr<CMeshBuffer> CBatchMesh::getMeshBuffer(std::uint32_t index) const {
    if (index >= batches.size()) {
        return boost::intrusive_ptr<CMeshBuffer>();
    }
    return batches[index].meshBuffer;
}

// 0x578ccc _ZNK6glitch5scene10CBatchMesh11getMaterialEj
boost::intrusive_ptr<video::CMaterial> CBatchMesh::getMaterial(std::uint32_t index) const {
    if (index >= batches.size()) {
        return boost::intrusive_ptr<video::CMaterial>();
    }
    return batches[index].material;
}

// 0x578cf4 _ZNK6glitch5scene10CBatchMesh24getMaterialVertexAttributeMapEj
boost::intrusive_ptr<video::CMaterialVertexAttributeMap> CBatchMesh::getMaterialVertexAttributeMap(std::uint32_t index) const {
    if (index >= batches.size()) {
        return boost::intrusive_ptr<video::CMaterialVertexAttributeMap>();
    }
    return batches[index].vertexAttributeMap;
}

// 0x57a2ac _ZN6glitch5scene10CBatchMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS4_INS5_27CMaterialVertexAttributeMapEEE
// An index past the end is ignored. The material (+0x04) is stored first, then the map (+0x08); each takes a use and drops the
// old handle.
void CBatchMesh::setMaterial(std::uint32_t index, const boost::intrusive_ptr<video::CMaterial>& material,
                             const boost::intrusive_ptr<video::CMaterialVertexAttributeMap>& vertexAttributeMap) {
    if (index >= batches.size()) {
        return;
    }
    batches[index].material = material;
    batches[index].vertexAttributeMap = vertexAttributeMap;
}

// 0x57aa8c _ZNK6glitch5scene10CBatchMesh14getBoundingBoxEv
// Refreshes the box when the dirty byte is set, or when the source is set and the stamp differs from the current tick.
const core::aabbox3d<float>& CBatchMesh::getBoundingBox() const {
    if (boundingBoxDirty || (boundingBoxSource != 0 && boundingBoxStamp != os::Timer::TickCount)) {
        updateBoundingBox();
    }
    return boundingBox;
}

// 0x578c78 _ZN6glitch5scene10CBatchMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE
// Stamps the box with os::Timer::TickCount and copies the six words into +0x50 .. +0x67.
void CBatchMesh::setBoundingBox(const core::aabbox3d<float>& box) {
    boundingBoxStamp = os::Timer::TickCount;
    boundingBox = box;
}

// 0x57db94 _ZNK6glitch5scene10CBatchMesh4loadEPNS_2io9IReadFileEPNS_5video12IVideoDriverEPNS2_11IFileSystemEPNS_7collada14CRootSceneNodeEPNS8_14CColladaFactoryE
// SKIPPED: the 2412-byte reader of the batch file format is not reconstructed; the stub reports failure.
bool CBatchMesh::load(io::IReadFile* /*file*/, video::IVideoDriver* /*driver*/, io::IFileSystem* /*fileSystem*/,
                      collada::CRootSceneNode* /*root*/, collada::CColladaFactory* /*factory*/) {
    return false;
}

// 0x57cd48 _ZNK6glitch5scene10CBatchMesh4saveEPNS_2io10IWriteFileEPNS_5video12IVideoDriverENS_2os8E_ENDIANEbb
// SKIPPED: the 2788-byte writer is not reconstructed; the stub reports failure.
bool CBatchMesh::save(io::IWriteFile* /*file*/, video::IVideoDriver* /*driver*/, std::int32_t /*endian*/, bool /*flagA*/,
                      bool /*flagB*/) const {
    return false;
}

// 0x50cd10 _ZNK6glitch5scene10CBatchMesh13getIndexRangeERjS2_PKNS1_8SSegmentE
void CBatchMesh::getIndexRange(std::uint32_t& first, std::uint32_t& last, const SSegment* segment) const {
    first = segment->indexBegin;  // +0x10
    last = segment->indexEnd;     // +0x14
}

// 0x50cd24 _ZN6glitch5scene10CBatchMesh4initEv (empty)
void CBatchMesh::init() {}

// 0x50cd28 _ZNK6glitch5scene10CBatchMesh20saveSegmentExtraDataEPKvPNS_2io10IWriteFileEb (empty)
void CBatchMesh::saveSegmentExtraData(const void* /*data*/, io::IWriteFile* /*file*/, bool /*flag*/) const {}

// 0x578920 _ZN6glitch5scene10CBatchMesh20loadSegmentExtraDataEPvPNS_2io9IReadFileEb (empty)
void CBatchMesh::loadSegmentExtraData(void* /*data*/, io::IReadFile* /*file*/, bool /*flag*/) {}

// 0x57a4a8 _ZN6glitch5scene10CBatchMesh5stripEv
// Shrinks the batch vector, the segment-info vector and the segment byte vector to their sizes.
void CBatchMesh::strip() {
    batches.shrink_to_fit();
    segmentInfos.shrink_to_fit();
    segmentData.shrink_to_fit();
}

// 0x57a6d4 _ZNK6glitch5scene10CBatchMesh17updateBoundingBoxEv
// PARTIAL: the 944-byte recomputation of the box from the segment boxes is not reconstructed; the stored box is kept.
void CBatchMesh::updateBoundingBox() const {}

// 0x57b900 _ZN6glitch5scene10CBatchMesh8addBatchEv
// Appends a batch whose range words start at the current segment-info count and returns the index of the new batch.
std::int32_t CBatchMesh::addBatch() {
    const std::int32_t index = static_cast<std::int32_t>(batches.size());
    batches.push_back(SBatch(static_cast<std::uint16_t>(segmentInfos.size())));
    return index;
}

// 0x57bd44 _ZN6glitch5scene10CBatchMesh5clearEv
// PARTIAL: the original first walks every batch and clears two words of each segment's owner (the object at segment +0x08, word
// +0x30: its fields +0x18 / +0x1c); that walk reads 32-bit pointers inside the segment records and is not written. Then the batch
// vector is erased and the segment byte vector and the segment-info vector are emptied (written here).
void CBatchMesh::clear() {
    batches.clear();
    segmentData.clear();
    segmentInfos.clear();
}

}  // namespace glitch::scene
