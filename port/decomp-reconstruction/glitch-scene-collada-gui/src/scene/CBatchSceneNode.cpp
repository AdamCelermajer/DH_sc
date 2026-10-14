// Bodies of glitch::scene::CBatchSceneNode (canonical class: scene/CBatchSceneNode.hpp). This file replaces the CBatchSceneNode
// bodies of src/scene_nodes.cpp and the recon free-function stand-ins in src/recon/slice_05.cpp (getMesh, the render
// registration helpers), slice_31.cpp (constructors, attribute slots, setup) and src/recon/v2/wave_02.cpp (getMesh).
//
// The visible-entry array (+0x158) is an array of SSegment POINTERS and a 32-bit word holds one exactly: the original is a
// 32-bit ARM binary, so its pointers are four bytes. computeMaxIndices 0x57e5b4 reads a record word as an SSegment* and then
// the segment's index range at +0x10 / +0x14; updateIndices 0x57f704 copies `2 * (indexEnd - indexBegin)` bytes per entry;
// renderBatchBBoxes 0x57f21c reads the pointer's box at +0x0c. The class models that with SVisibleEntry (20 bytes, five words)
// and CBatchMesh::SSegment (44 bytes, indexBegin at +0x10 and indexEnd at +0x14).
#include "glitch-scene-collada-gui/scene/CBatchSceneNode.hpp"

#include <cstring>
#include <vector>

#include "glitch-core-io/io/iattributes.hpp"
#include "glitch-core-io/os/logging.hpp"  // glitch::os::Timer::TickCount (updateInfo)
#include "glitch-scene-collada-gui/scene/CMeshBuffer.hpp"
#include "glitch-video/buffer.hpp"
#include "glitch-video/material_vertex_attribute_map.hpp"

// CMaterial comes from the core-io stub that io/interfaces.hpp (through ISceneNode.hpp) declares; glitch-video/material.hpp
// cannot be included here until that stub is replaced.

namespace glitch::scene {

namespace {

const core::vector3d<float> kOrigin{0.0f, 0.0f, 0.0f};
const core::quaternion kIdentityRotation{0.0f, 0.0f, 0.0f, 1.0f};
const core::vector3d<float> kUnitScale{1.0f, 1.0f, 1.0f};

// The index data of a visible segment: [indexBegin, indexEnd) of the segment record (SSegment +0x10 / +0x14).
std::int32_t segmentIndexCount(const CBatchMesh::SSegment* segment) {
    return static_cast<std::int32_t>(segment->indexEnd) - static_cast<std::int32_t>(segment->indexBegin);
}

}  // namespace

// 0x57f9c0 _ZN6glitch5scene15CBatchSceneNodeC2Ei (C1 0x57fc04)
// ISceneNode(id, position 0, rotation (0,0,0,1), scale 1); the mesh is a new CBatchMesh holding one use (the constructor increments
// its count after operator new(0x7c)); +0x134 is 0, +0x138 is 2; the cache flag (+0x150) and the register-once flag (+0x148) are 1,
// the batch count (+0x144) and the transparent base (+0x15c) are -1, the cache and the entry array are null; setAutomaticCulling(2).
CBatchSceneNode::CBatchSceneNode(int nodeId) : IMeshSceneNode(nodeId, kOrigin, kIdentityRotation, kUnitScale) {
    mesh = boost::intrusive_ptr<CBatchMesh>(new CBatchMesh(), true);
    unk_134 = 0;
    unk_138 = 2;
    setAutomaticCulling(2);
}

// 0x57f0d4 _ZN6glitch5scene15CBatchSceneNodeC2EiRKN5boost13intrusive_ptrINS0_10CBatchMeshEEE (C1 0x57faf8)
// The mesh handle is copied (a use is added); +0x134 and +0x138 are 2, the rest as the (id) constructor.
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

// 0x57f344 _ZN6glitch5scene15CBatchSceneNodeD1Ev (0x57f3ec D2, 0x57f49c D0)
// In order: operator delete[] of the visible-entry array (+0x158), setVisibleIndexCache(false) (which frees the cache block at
// +0x154), drop of the index buffer at +0x14c, drop of the mesh handle, then ~ISceneNode. The cache must be freed before the
// entries, exactly as the listing does.
CBatchSceneNode::~CBatchSceneNode() {
    delete[] visibleEntries;
    visibleEntries = nullptr;
    setVisibleIndexCache(false);
    dirtyBuffer.reset();
}

// 0x57e6f8 _ZNK6glitch5scene15CBatchSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// With attributes: the ISceneNode attributes, then "SegmentAutomaticCulling" (addEnum with the choice table and the word at
// +0x13c... the word at +0x138, i.e. unk_138), "RegisterSolidBatchesOnce" (+0x148), "HasVisibleIndexCache" (+0x150),
// "Max index count (solid)" (+0x13c), "Max index count (transparent)" (+0x140) and "Solid batch count" (+0x144).
// The original writes the word at +0x138 for the culling choice (0x57e738 reads this+312); unk_138 holds the same 2 the
// AutomaticCulling serializer would write.
void CBatchSceneNode::serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) const {
    if (attributes == nullptr) {
        return;
    }
    IMeshSceneNode::serializeAttributes(attributes, options);
    attributes->addEnum("SegmentAutomaticCulling", static_cast<int>(unk_138), AutomaticCullingNames, false);
    attributes->addBool("RegisterSolidBatchesOnce", flag148, false);
    attributes->addBool("HasVisibleIndexCache", visibleIndexCacheOn, false);
    attributes->addInt("Max index count (solid)", maxSolidIndices, false);
    attributes->addInt("Max index count (transparent)", maxTransparentIndices, false);
    attributes->addInt("Solid batch count", unk_144, false);
}

// 0x57ed98 _ZN6glitch5scene15CBatchSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// With attributes: the ISceneNode attributes, "SegmentAutomaticCulling" into +0x138 (getEnum with the choice table),
// "RegisterSolidBatchesOnce" into +0x148, and, when "HasVisibleIndexCache" differs from the current flag, setVisibleIndexCache
// with it. The max-index and batch-count attributes the serializer writes are not read back.
void CBatchSceneNode::deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) {
    if (attributes == nullptr) {
        return;
    }
    IMeshSceneNode::deserializeAttributes(attributes, options);
    unk_138 = static_cast<std::uint32_t>(attributes->getEnum("SegmentAutomaticCulling", AutomaticCullingNames));
    flag148 = attributes->getBool("RegisterSolidBatchesOnce");
    const bool cache = attributes->getBool("HasVisibleIndexCache");
    if (visibleIndexCacheOn != cache) {
        setVisibleIndexCache(cache);
    }
}

// 0x57f4b8 _ZN6glitch5scene15CBatchSceneNode19onRegisterSceneNodeEv
// PARTIAL: the 588-byte body reaches the CBatchMesh batch and segment records (initStaticSegmentBoundingBoxes, the frustum
// intersectors, addVisibleSegments) and the sorted batch order; only the base registration result is returned here. The
// visible-entry layout is no longer the blocker - it is mapped above.
int CBatchSceneNode::onRegisterSceneNode() {
    return ISceneNode::onRegisterSceneNode();
}

// 0x5808ac _ZN6glitch5scene15CBatchSceneNode6renderEPv
// The argument is a packed dispatch word, not a context: the low 16 bits are (batch index + 1), the high 16 bits are the index
// of a segment inside the transparent batch. With a driver: setTransform(1, the absolute matrix), then, when the driver has
// 0x100 in its option flags, setState(256, false) and the transparent path - a word >= 0x10000 draws that one transparent
// segment, else it draws solid batch (word - 1); without the flag the same dispatch happens after the option test. With a
// zero/absent batch index and a batch count above zero it draws the solid batches [index, unk_144) and flushes the
// transparent queue, and it restores setState(256, true) when the driver option was set. PARTIAL: IVideoDriver in the port
// has no setState (+0xa0) and no option accessor; the mesh is not drawn here.
void CBatchSceneNode::render(void* renderContext) {
    (void)renderContext;
}

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
// CBatchMesh::sort(driver) is stored into both +0x15c and +0x144, then CBatchMesh::updateBoundingBox(),
// CBatchMesh::initStaticSegmentBoundingBoxes() and the mesh's vtable +0x3c (CBatchMesh::init) run; with `flag` false the
// vtable +0x100 slot (CBatchSceneNode::postCompile) is dispatched, which is the cache set-up.
// PARTIAL: CBatchMesh::sort (0x57c19c) and initStaticSegmentBoundingBoxes (0x57b174) have no body in the port, so the two
// stores and the postCompile dispatch that depend on sort's answer are not written. updateBoundingBox and the mesh's init are
// called here (both are empty/reconstructed in the port).
void CBatchSceneNode::setup(bool flag, video::IVideoDriver* /*driver*/) {
    if (mesh) {
        mesh->updateBoundingBox();
        mesh->init();
    }
    if (!flag) {
        postCompile();
    }
}

// 0x57ee34 _ZN6glitch5scene15CBatchSceneNode11postCompileEv
// The mesh buffer count drives everything. When the transparent base (+0x15c) is -1 it is recomputed: it starts at the batch
// count and every batch whose material's first render pass has bit 16 of the second state word set lowers it by one (the
// transparent batches sort to the end), leaving the first transparent batch index; the loop then ends.
// The entry array (+0x158) is operator delete[]d and rebuilt as a block of 4 * (totalIndexCount + 20 * batchCount) bytes,
// where totalIndexCount is the sum over the batches of (batch.endSegmentInfo - batch.firstSegmentInfo) - the count of segment
// infos, which is what the batch-node index counts are expressed in. +0x13c and +0x140 are cleared. Each of the `batchCount`
// 20-byte records gets {count = 0, indexBase = 0, tick = 1, flags = 1, firstIndex = running}, with `running` starting at
// 20 * batchCount / 4 words (= the size of the record array) and advanced by that batch's segment-info count; each batch's
// material decides whether its (buffer's vertex attribute map) size raises +0x13c (solid) or +0x140 (transparent). The body
// ends with invalidateVisibleIndexCache(0xFFFFFFFF).
void CBatchSceneNode::postCompile() {
    if (!mesh) {
        return;
    }
    const std::uint32_t batchCount = mesh->getMeshBufferCount();
    if (unk_15c == -1) {
        std::uint32_t firstTransparent = batchCount;
        for (std::uint32_t index = 0; index < firstTransparent;) {
            const boost::intrusive_ptr<video::CMaterial> material = mesh->getMaterial(index);
            bool transparent = false;
            if (material && material->m_renderer) {
                const video::STechnique& technique = material->m_renderer->m_techniques[material->getTechnique()];
                transparent = (technique.passes[0].stateWords[1] & 0x10000u) != 0;
            }
            if (transparent) {
                --firstTransparent;
            } else {
                ++index;
            }
        }
        unk_15c = static_cast<std::int32_t>(firstTransparent);
    }

    std::uint32_t totalIndices = 0;
    for (std::uint32_t index = 0; index < batchCount; ++index) {
        const CBatchMesh::SBatch& batch = mesh->batches[index];
        totalIndices += static_cast<std::uint32_t>(batch.endSegmentInfo) - batch.firstSegmentInfo;
    }

    delete[] visibleEntries;
    visibleEntries = nullptr;
    maxSolidIndices = 0;
    maxTransparentIndices = 0;

    const std::uint32_t recordWords = 5u * batchCount;
    visibleEntries = reinterpret_cast<SVisibleEntry*>(new std::uint32_t[recordWords + static_cast<std::size_t>(totalIndices)]);
    std::uint32_t running = recordWords;
    for (std::uint32_t index = 0; index < batchCount; ++index) {
        SVisibleEntry& entry = visibleEntries[index];
        entry.count = 0;
        entry.indexBase = 0;
        entry.tick = 1;
        entry.flags = 1;
        entry.firstIndex = running;
        const CBatchMesh::SBatch& batch = mesh->batches[index];
        running += static_cast<std::uint32_t>(batch.endSegmentInfo) - batch.firstSegmentInfo;

        const boost::intrusive_ptr<video::CMaterial> material = mesh->getMaterial(index);
        const boost::intrusive_ptr<video::CMaterialVertexAttributeMap> attributeMap =
            mesh->getMaterialVertexAttributeMap(index);
        const std::uint32_t size = attributeMap ? static_cast<std::uint32_t>(attributeMap->m_maps.size()) : 0;
        if (index < static_cast<std::uint32_t>(unk_15c)) {
            if (maxSolidIndices < static_cast<std::int32_t>(size)) {
                maxSolidIndices = static_cast<std::int32_t>(size);
            }
        } else if (maxTransparentIndices < static_cast<std::int32_t>(size)) {
            maxTransparentIndices = static_cast<std::int32_t>(size);
        }
        (void)material;
    }
    invalidateVisibleIndexCache(0xFFFFFFFFu);
}

// 0x50cd38 _ZN6glitch5scene15CBatchSceneNode20preRegisterSceneNodeEv (empty)
void CBatchSceneNode::preRegisterSceneNode() {}

// 0x50cd3c _ZNK6glitch5scene15CBatchSceneNode16isSegmentVisibleERKNS0_10CBatchMesh8SSegmentE
bool CBatchSceneNode::isSegmentVisible(const CBatchMesh::SSegment& segment) {
    return segment.visible != 0;  // byte +0x20
}

// 0x57f704 _ZN6glitch5scene15CBatchSceneNode13updateIndicesEjRKN5boost13intrusive_ptrINS_5video7IBufferEEERKNS3_IKS5_EE
// Maps the destination buffer for writing (access 2) and the source for reading (access 1), then walks the batch's visible
// entries: for every SSegment* it copies the segment's index range (2 * (indexEnd - indexBegin) bytes at the mapped source
// pointer + 2 * indexBegin) to the write cursor and sums the copied index count. After the copy both buffers lose one map
// reference (the map count byte at +0x13 of the buffer; the last reference runs the driver's unmap callback when flag 0x20 is
// set) and the batch's cache slot head (+0x154 + 12 * batch, word 0) is zeroed. Returns the index count.
std::int32_t CBatchSceneNode::updateIndices(std::uint32_t batch, const boost::intrusive_ptr<video::IBuffer>& buffer,
                                            const boost::intrusive_ptr<const video::IBuffer>& source) {
    struct MapRelease {
        const video::IBuffer* buffer;
        ~MapRelease() {
            if (buffer == nullptr) {
                return;
            }
            const std::uint8_t count = buffer->m_mapCount13;
            if ((count & 0x1Fu) > 1) {
                buffer->m_mapCount13 = static_cast<std::uint8_t>(((count & 0x1Fu) - 1) | (count & 0xE0u));
                return;
            }
            if ((buffer->m_flags12 & video::IBuffer::kFlagUnmapCallback) != 0) {
                buffer->unmapImpl();
            }
            buffer->m_mapCount13 = 0;
        }
    };

    std::uint8_t* out = buffer->map(2);
    const std::uint8_t* in = source->map(1);
    std::int32_t copied = 0;
    const SVisibleEntry* entry = visibleEntry(batch);
    if (entry != nullptr) {
        const std::uint32_t count = static_cast<std::uint32_t>(entry->count);
        for (std::uint32_t index = 0; index < count; ++index) {
            const CBatchMesh::SSegment* segment = visibleSegment(batch, index);
            const std::uint32_t begin = segment->indexBegin;
            const std::int32_t length = segmentIndexCount(segment);
            std::memcpy(out, in + 2 * begin, static_cast<std::size_t>(2 * length));
            out += 2 * length;
            copied += length;
        }
    }
    {
        // The original decrements both map counts inline (the buffer's byte at +0x13; the last reference of a counted map
        // runs IBuffer::unmapImpl when flag 0x20 is set).
        const MapRelease releaseOut{buffer.get()};
        const MapRelease releaseIn{source.get()};
    }
    if (SCacheSlot* slot = cacheSlot(batch)) {
        slot->count = 0;  // the original writes this unconditionally; the port's cacheSlot is the only null-guarded view
    }
    return copied;
}

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

// Not in the original: the named views of the word arithmetic of the visible-entry block. visibleEntry / cacheSlot return
// null when the block is absent (the original dereferences unconditionally); visibleSegment returns null for a missing entry.
CBatchSceneNode::SVisibleEntry* CBatchSceneNode::visibleEntry(std::uint32_t batch) const {
    if (visibleEntries == nullptr) {
        return nullptr;
    }
    return visibleEntries + batch;
}

CBatchSceneNode::SCacheSlot* CBatchSceneNode::cacheSlot(std::uint32_t batch) const {
    if (visibleIndexCache == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<SCacheSlot*>(visibleIndexCache) + batch;
}

CBatchMesh::SSegment* CBatchSceneNode::visibleSegment(std::uint32_t batch, std::uint32_t position) const {
    const SVisibleEntry* entry = visibleEntry(batch);
    if (entry == nullptr || visibleIndexCache == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<CBatchMesh::SSegment**>(visibleIndexCache + entry->firstIndex)[position];
}

// 0x57e5b4 _ZN6glitch5scene15CBatchSceneNode17computeMaxIndicesEj
// The word at +0x158 + 20 * batch, read as a word offset into the cache block, addresses the batch's list of visible SSegment*
// entries; the batch's entry count (record word 0) is the number of them. The result is the sum of
// (*(int*)(segment + 20) - *(int*)(segment + 16)), i.e. SSegment::indexEnd - SSegment::indexBegin, over that list; 0 for an
// empty list. Verified against Hex-Rays 0x57e5b4 and the listing. Deviation: the original dereferences the entry array and the
// segment pointer list unconditionally; the port's visibleEntry()/visibleSegment() return null for a missing block.
std::int32_t CBatchSceneNode::computeMaxIndices(std::uint32_t batch) {
    const SVisibleEntry* entry = visibleEntry(batch);
    if (entry == nullptr) {
        return 0;
    }
    std::int32_t total = 0;
    const std::uint32_t count = static_cast<std::uint32_t>(entry->count);
    for (std::uint32_t index = 0; index < count; ++index) {
        total += segmentIndexCount(visibleSegment(batch, index));
    }
    return total;
}

// 0x57e604 _ZN6glitch5scene15CBatchSceneNode19clearVisibleSegmentsEv
// Reads the mesh's batch count (vtable +0x10) and zeroes word 0 of every one of that many 20-byte visible-entry records: the
// batch's list of visible segments becomes empty. Returns the batch count (0 when the mesh has none). Verified against
// Hex-Rays 0x57e604: the loop index runs to the mesh's virtual +0x10, not to a per-batch segment count, and the stride is 20.
std::int32_t CBatchSceneNode::clearVisibleSegments() {
    const std::uint32_t batchCount = mesh ? mesh->getMeshBufferCount() : 0;
    for (std::uint32_t index = 0; index < batchCount; ++index) {
        SVisibleEntry* entry = visibleEntry(index);
        if (entry != nullptr) {
            entry->count = 0;
        }
    }
    return static_cast<std::int32_t>(batchCount);
}

// 0x57e650 _ZN6glitch5scene15CBatchSceneNode10updateInfoEjj
// For the visible-entry records [batch, segment) (20 bytes each: count, indexBase, tick, flags, firstIndex): sums `count` and,
// for each non-empty record, stamps the tick with glitch::os::Timer::TickCount and sets flags bit 0 unless indexBase equals
// count. Returns the summed count. Verified against Hex-Rays 0x57e650: the count is at this+0x158 + 20i, the tick is the
// TickCount static at 0x9f6cfc (os::Timer::TickCount, declared in glitch-core-io/os/logging.hpp), and the flag word becomes
// record[3] | 1 only when record[1] != record[0]. Only deviation: the port keeps its null-array guard, which is host
// necessitated (the original dereferences unconditionally).
std::int32_t CBatchSceneNode::updateInfo(std::uint32_t batch, std::uint32_t segment) {
    std::int32_t total = 0;
    if (batch >= segment || visibleEntries == nullptr) {
        return 0;
    }
    for (std::uint32_t i = batch; i < segment; ++i) {
        SVisibleEntry* entry = visibleEntry(i);
        const std::int32_t count = entry->count;
        total += count;
        if (count) {
            entry->flags = (entry->indexBase == count) ? entry->flags : (entry->flags | 1u);
            entry->tick = os::Timer::TickCount;
        }
    }
    return total;
}

// 0x57e800 _ZN6glitch5scene15CBatchSceneNode18registerSolidBatchesEv
// Walks the solid batches [0, unk_144): for a batch whose visible-entry count is non-zero it takes the batch's
// material (mesh vtable +0x18) and the driver's sorted batch order (CBatchDriver::sortedBatches: a null-terminated uint16 list
// that starts at element 1 and is read as the *batch index + 1*), gives both to the driver's compile entry point (vtable
// +0x24 of the batch driver, the 7-argument CBatchDriver::compile: node, material, packed batch index, 4, null, 0x7fffffff,
// then per-segment compile info) and reports the batch to its ISegmentCompileCallback. Every batch then gets the same entry
// bookkeeping updateInfo applies, with the tick read from os::Timer::TickCount. The object's +0x148 flag gates the whole body.
// PARTIAL: the port has no glitch::scene::CBatchDriver (IVideoDriver carries no compile slot; the batch driver, the sorted
// batch order and the segment compile callback are not reconstructed), so nothing but the gate is written here.
void CBatchSceneNode::registerSolidBatches() {
    if (!unk_144) {
        return;
    }
}

// 0x57ea70 _ZN6glitch5scene15CBatchSceneNode25registerTransparentBatchesEv
// Walks the transparent batches [unk_144, the mesh's batch count): for each visible segment of the batch it takes the
// segment's centre (CBatchMesh::getSegmentCenter 0x57e90c), passes it as the compile info to the driver's compile entry point
// (vtable +0x24, packed as `batch | ((segment->field34 + 1) << 16)`, flag 8, the centre, the segment's +0x22 word) and reports
// the batch to its callback. Then it zeroes the batch's entry count and stamps the tick with os::Timer::TickCount.
// PARTIAL: as registerSolidBatches, the batch driver's compile entry point and CBatchMesh::getSegmentCenter have no body in
// the port.
void CBatchSceneNode::registerTransparentBatches() {}

// 0x57ebec _ZN6glitch5scene15CBatchSceneNode26invalidateVisibleIndexCacheEj
// With the cache off (+0x150 byte) nothing happens. With batch == -1: takes the batch count (+0x144, the mesh's first
// word), sums every batch's segment count (SBatch +0x0c .. +0x0e of the mesh's batch vector at +0x20) into `total`, and
// operator delete[]s the old cache block (+0x154). A new block of 2 * (total + (12 * batchCount) / 2) bytes is allocated:
// `total` 16-bit indices followed by the batchCount 12-byte slots, in one block. For each batch the slot gets
// {word 0 = 0, word 1 = total, word 2 = running}, running starts at 12 * batchCount / 4 words (= the index area's size) and
// grows by the batch's segment count, and the batch's visible-entry flags (+0x158 + 20 * batch + 12) get bit 0 set.
// With a batch index: only that batch's flags get bit 0. Verified against Hex-Rays 0x57ebec.
void CBatchSceneNode::invalidateVisibleIndexCache(std::uint32_t batch) {
    if (!visibleIndexCacheOn) {
        return;
    }
    if (batch == 0xFFFFFFFFu) {
        const std::uint32_t batchCount = mesh ? mesh->getMeshBufferCount() : 0;
        if (batchCount == 0) {
            return;
        }
        std::uint32_t totalIndices = 0;
        for (std::uint32_t index = 0; index < batchCount; ++index) {
            const CBatchMesh::SBatch& record = mesh->batches[index];
            totalIndices += static_cast<std::uint32_t>(record.endSegmentInfo) - record.firstSegmentInfo;
        }
        const std::uint32_t slotWords = (12u * batchCount) >> 2;  // the slot area in 32-bit words
        delete[] visibleIndexCache;
        visibleIndexCache = new std::uint32_t[totalIndices + slotWords];

        std::uint32_t running = slotWords;  // the index area starts after the slots
        for (std::uint32_t index = 0; index < batchCount; ++index) {
            SCacheSlot* slot = cacheSlot(index);
            const CBatchMesh::SBatch& record = mesh->batches[index];
            const std::uint32_t count = static_cast<std::uint32_t>(record.endSegmentInfo) - record.firstSegmentInfo;
            slot->count = 0;
            slot->capacity = static_cast<std::int32_t>(totalIndices);  // word 1 (+0x04) takes the total index count
            slot->base = running;                                      // word 2 (+0x08) takes the running offset
            visibleEntry(index)->flags |= 1u;
            running += count;
        }
        return;
    }
    visibleEntry(batch)->flags |= 1u;
}

// 0x57f21c _ZN6glitch5scene15CBatchSceneNode16renderBatchBBoxesEPNS_5video12IVideoDriverEj
// Takes the driver's material renderer manager (+0xdc), creates the renderer of its material when its type word (+0x2c of the
// manager) is 0xffff, gets that renderer's cached material instance and sets it on the driver (driver vtable +0x24) with the
// renderer's technique, or technique 0xff for a null material. Then it draws the batch's visible segment boxes: the
// SSegment* list of the entry, and for each segment the driver's draw3DBox (vtable +0x28) over the box at segment +0x0c
// (the aabbox3d pointer of the segment) in green (0xFF00FF00).
// PARTIAL: the renderer type word is at IVideoDriver +0x2c... the original reads the manager at +0xdc and its type word at
// +0x2c; the port's CMaterialRendererManager declares neither that word nor createMaterialRenderer, and IVideoDriver exposes
// no accessor for +0xdc. draw3DBox at the driver's vtable +0x28 is likewise absent.
void CBatchSceneNode::renderBatchBBoxes(video::IVideoDriver* driver, std::uint32_t batch) {
    (void)driver;
    (void)batch;
}

// 0x5800e8 _ZN6glitch5scene15CBatchSceneNode15renderSolidBatchEPNS_5video12IVideoDriverEj
// Returns when the batch's visible-entry count is zero. Otherwise it takes the batch's vertex-attribute map (mesh vtable
// +0x1c) and the batch's material (mesh vtable +0x18), then, with the cache on (+0x150), rebuilds the index buffer: the
// cache block is handed to the driver's createBuffer as a 16-bit index buffer (type 1, usage 4, 2 * slot->count bytes at
// cache + 2 * slot->base) and, when the entry's flags are set, updateIndices(batch, the new buffer, the batch mesh buffer's
// index buffer) writes it and stores the returned index count in the slot head. With the cache off (or when both
// maxSolidIndices (+0x13c) and computeMaxIndices(batch) are zero) it allocates a fresh process buffer, creates a buffer over
// it and runs the same updateIndices. The material, the attribute map and the batch's stream table (mesh buffer +0x14) are
// given to the driver (setMaterial, vtable +0x24 of the material) and the draw slot (vtable +0x1c of the driver, the
// 3-argument draw) is issued; then the created buffer and the process buffer are released and the entry's count word is
// restored into the batch's index word at +0x14 (record word 1).
// PARTIAL: the driver's createBuffer, setMaterial and draw are declared-only in the port and the batch mesh buffer's index
// buffer is not named there, so only the early-out is written. The cache/entry arithmetic this body would use is implemented
// in invalidateVisibleIndexCache / updateIndices / computeMaxIndices.
void CBatchSceneNode::renderSolidBatch(video::IVideoDriver* driver, std::uint32_t batch) {
    (void)driver;
    (void)batch;
}

// 0x5804f0 _ZN6glitch5scene15CBatchSceneNode21flushTransparentBatchEPNS_5video12IVideoDriverE
// Returns when the transparent batch's (index +0x15c) visible-entry count is zero. Otherwise it takes the batch's material
// and attribute map, rebuilds the index buffer from the cache block when maxTransparentIndices (+0x140) is set (reusing the
// buffer at +0x14c through IBuffer::reset, otherwise creating one), runs updateIndices on it, sets the material and draws the
// batch's stream table, then zeroes the batch's entry count and releases the process buffer.
// PARTIAL: as renderSolidBatch, the driver's createBuffer / setMaterial / draw and the batch mesh buffer's index buffer are
// not available in the port.
void CBatchSceneNode::flushTransparentBatch(video::IVideoDriver* driver) {
    (void)driver;
}

// 0x5807a0 _ZN6glitch5scene15CBatchSceneNode24renderTransparentSegmentEPNS_5video12IVideoDriverERNS0_10CBatchMesh8SSegmentE
// Sets the segment's material and draws the segment's stream table for the transparent pass. It is called from render with
// the segment the packed word selected. PARTIAL: the driver's draw entry point is not available in the port.
void CBatchSceneNode::renderTransparentSegment(video::IVideoDriver* driver, CBatchMesh::SSegment& segment) {
    (void)driver;
    (void)segment;
}

}  // namespace glitch::scene
