// glitch::scene::CBatchSceneNode: the scene node that renders a CBatchMesh (static level geometry in a few draw calls).
// Canonical header of the engine unification wave (u1-scene). It replaces the CBatchSceneNode in scene_nodes.hpp (four
// accessors, no base), layouts/scene.hpp (an empty struct), layouts/scene_evidence.hpp (evidence::CBatchSceneNode) and the
// recon_04 / recon_05 / recon_31 / recon_v2_02::CBatchSceneNodeView stand-ins.
//
// Evidence (libDungeonHunter2.so, ARM, 32-bit layout; constructors 0x57f9c0 / 0x57fc04 (id), 0x57f0d4 / 0x57faf8 (id, mesh),
// 0x57f874 / 0x57fd74 (id, mesh&, flag), 0x57fe84 / 0x57ffd4 (id, mesh, manager); tools/decomp_vtable.py
// glitch::scene::CBatchSceneNode, 328 bytes, virtual base IReferenceCounted at +0x160):
//   CBatchSceneNode derives from IMeshSceneNode (the vtable carries IMeshSceneNode::setNodeHierarchyAsShadowCaster at +0xe4 and
//   the pure setMesh / getMesh slots at +0xf4 / +0xf8, which it implements) and adds +0xfc setup, +0x100 postCompile,
//   +0x104 preRegisterSceneNode, +0x108 isSegmentVisible and +0x10c updateIndices.
//   Members (the base is 0x130 bytes; the offsets are the constructor 0x57f9c0's word indices, `(DWORD*)this + n` = 0x130 + 4n):
//     +0x130 the CBatchMesh (a reference is held; the one-argument constructor creates a CBatchMesh)
//     +0x134 0 / 2 (a mode word: the (id) constructor stores 0, the ones with a mesh 2)
//     +0x138 2, +0x13c 0, +0x140 0
//     +0x13c maxSolidIndices (postCompile 0x57ee34 recomputes it: the largest index count of the solid batches),
//     +0x140 maxTransparentIndices (the same for the transparent batches)
//     +0x144 solidBatchCount (-1 by default; CBatchMesh::sort's answer, postCompile 0x57ee34 also lowers it when a transparent
//           batch is found; the max-indices logic and registerSolidBatches 0x57e800 use it as the batch count)
//     +0x148 a byte, 1 by default, the flag argument of the (id, mesh, flag) constructor; serializeAttributes 0x57e6f8 writes it
//           as "RegisterSolidBatchesOnce". The original reads it in registerSolidBatches (see the note there).
//     +0x14c an intrusive_ptr<video::IBuffer>: the index buffer the batch node builds for one batch (renderSolidBatch
//           0x5800e8 and flushTransparentBatch 0x5804f0 create it through the driver and free its process buffer; the destructor
//           drops the handle)
//     +0x150 byte: the visible-index cache is on (1 by default; setVisibleIndexCache 0x57ed0c toggles it)
//     +0x154 the visible-index cache: a counted block holding one 12-byte slot per batch (head, capacity, base), written by
//           invalidateVisibleIndexCache 0x57ebec, freed by setVisibleIndexCache(false) and the destructor, and handed to the
//           driver as the index buffer of a draw (renderSolidBatch 0x5800e8)
//     +0x158 the visible entries: an array of 20-byte records, one per batch, allocated by postCompile 0x57ee34 (operator
//           new[](4 * (totalIndexCount + 20 * batchCount)), the cache block and this array share one allocation) and freed by
//           the destructor. A record is {count, indexBase, tick, flags, firstIndex}: updateInfo 0x57e650 sums `count` and,
//           for a non-empty record, stamps the tick with os::Timer::TickCount and ORs bit 0 of `flags` in unless indexBase
//           equals count; registerSolidBatches / registerTransparentBatches / clearVisibleSegments / computeMaxIndices /
//           renderBatchBBoxes / updateIndices read `count` as the number of visible segment POINTERS stored at
//           `firstIndex` inside the cache block (`this[+0x154] + 4 * firstIndex`, read as SSegment*).
//     +0x15c -1: the transparent batch base postCompile 0x57ee34 writes (the first batch whose material has the transparent
//           bit; render 0x5808ac skips the transparent tail with `while (this[+0x15c] > index)`)
//     +0x160 the virtual base
//   The constructors end with setAutomaticCulling(2).
//
// The visible entries are SEGMENT POINTERS: the original is a 32-bit ARM binary, so an SSegment* is four bytes and a record
// entry fits a 32-bit word exactly. computeMaxIndices 0x57e5b4 does `v = *(SSegment**)(cache + 4 * record[4])` and then
// `*(int*)(v + 20) - *(int*)(v + 16)`, i.e. the segment record's indexEnd - indexBegin (CBatchMesh::SSegment +0x14 / +0x10).
// The 20-byte record's words are therefore {count, indexBase, tick, flags, firstIndex}, never host-width pointers of their own.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/scene/CBatchMesh.hpp"
#include "glitch-scene-collada-gui/scene/CMeshSceneNode.hpp"

namespace glitch::video {
class IBuffer;
}  // namespace glitch::video

namespace glitch::scene {

class CBatchSceneNode : public IMeshSceneNode {
public:
    // One visible-entry record (+0x158, 20 bytes): the count of visible segments of a batch, the index inside the cache
    // block (slot) of its batch, the tick of the last update, the dirty/match flags and the base of the batch's cache slot.
    struct SVisibleEntry {
        std::int32_t count;       // +0x00 number of SSegment* entries for the batch
        std::int32_t indexBase;   // +0x04 value `count` is compared against by updateInfo (the index of the batch's slot)
        std::uint32_t tick;       // +0x08 os::Timer::TickCount of the last updateInfo
        std::uint32_t flags;      // +0x0c bit 0 set while the cached indices are stale
        std::uint32_t firstIndex; // +0x10 word offset of the batch's SSegment* array inside the cache block
    };
    static_assert(sizeof(SVisibleEntry) == 20, "the original record is 20 bytes");

    // One cache slot (+0x154, 12 bytes), the counted block that both invalidate paths rebuild.
    struct SCacheSlot {
        std::int32_t count;      // +0x00 used as the batch's write head, zeroed by clearVisibleSegments
        std::int32_t capacity;   // +0x04 total index capacity of the block (indices + cache slots)
        std::uint32_t base;      // +0x08 word offset where this batch's indices start / its own slot
    };
    static_assert(sizeof(SCacheSlot) == 12, "the original cache slot is 12 bytes");

    // 0x57f9c0 (C2) / 0x57fc04 (C1): a node with a new empty CBatchMesh.
    explicit CBatchSceneNode(int id);
    // 0x57f0d4 (C2) / 0x57faf8 (C1): a node over `mesh`.
    CBatchSceneNode(int id, const boost::intrusive_ptr<CBatchMesh>& mesh);
    // 0x57f874 (C2) / 0x57fd74 (C1).
    CBatchSceneNode(int id, boost::intrusive_ptr<CBatchMesh>& mesh, bool flag);
    // 0x57fe84 (C2) / 0x57ffd4 (C1): a node over `mesh` that registers with `manager`.
    CBatchSceneNode(int id, const boost::intrusive_ptr<CBatchMesh>& mesh, CSceneManager* manager);
    // 0x57f344, 0x57f3ec (D1), 0x57f49c (D0).
    ~CBatchSceneNode() override;

    // vptr+0x00 / +0x04.
    void serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) const override;  // 0x57e6f8
    void deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) override;      // 0x57ed98
    int onRegisterSceneNode() override;                                         // vptr+0x10, 0x57f4b8
    void render(void* renderContext) override;                                  // vptr+0x1c, 0x5808ac
    const core::aabbox3d<float>& getBoundingBox() const override;               // vptr+0x30, 0x57e538
    const core::aabbox3d<float>& getTransformedBoundingBox() const override;    // vptr+0x34, 0x57e524
    boost::intrusive_ptr<video::CMaterial> getMaterial(std::uint32_t index) const override;  // vptr+0x84, 0x57e570
    std::uint32_t getMaterialCount() const override;                            // vptr+0x88, 0x57e554
    E_SCENE_NODE_TYPE getType() const override;                                 // vptr+0xbc, 0x50cd2c ('basn')
    void setMesh(const boost::intrusive_ptr<IMesh>& mesh) override;             // vptr+0xf4, 0x57e5b0 (empty)
    boost::intrusive_ptr<IMesh> getMesh() const override;                       // vptr+0xf8, 0x57e594

    // vptr+0xfc .. +0x10c.
    virtual void setup(bool flag, video::IVideoDriver* driver);                 // 0x57f074
    virtual void postCompile();                                                 // 0x57ee34
    virtual void preRegisterSceneNode();                                        // 0x50cd38 (empty)
    virtual bool isSegmentVisible(const CBatchMesh::SSegment& segment);         // 0x50cd3c
    // vptr+0x10c. The original returns the copied index count in r0; a return type is not part of the mangled symbol, so the
    // port declares it (rule 10 is unaffected).
    virtual std::int32_t updateIndices(std::uint32_t batch, const boost::intrusive_ptr<video::IBuffer>& buffer,
                                       const boost::intrusive_ptr<const video::IBuffer>& source);  // 0x57f704

    // Non-virtual members.
    std::int32_t computeMaxIndices(std::uint32_t batch);                        // 0x57e5b4 (int in the binary)
    std::int32_t clearVisibleSegments();                                        // 0x57e604 (int in the binary)
    std::int32_t updateInfo(std::uint32_t batch, std::uint32_t segment);        // 0x57e650 (int in the binary)
    void registerSolidBatches();                                                // 0x57e800
    void registerTransparentBatches();                                          // 0x57ea70
    void invalidateVisibleIndexCache(std::uint32_t batch);                      // 0x57ebec
    void setVisibleIndexCache(bool enabled);                                    // 0x57ed0c
    void renderBatchBBoxes(video::IVideoDriver* driver, std::uint32_t batch);   // 0x57f21c
    void renderSolidBatch(video::IVideoDriver* driver, std::uint32_t batch);    // 0x5800e8
    void flushTransparentBatch(video::IVideoDriver* driver);                    // 0x5804f0
    void renderTransparentSegment(video::IVideoDriver* driver, CBatchMesh::SSegment& segment);  // 0x5807a0
    // 0x50cdb0 (SFrustumBoxIntersector3), 0x50d198 (SUniverseIntersector), 0x50d2c0 (SBoxIntersector), 0x50d49c
    // (SFrustumBoxIntersector): member templates over the intersector type; no bodies (not reconstructed).
    template <typename Intersector>
    void addVisibleSegments(std::uint32_t batch, Intersector intersector);

    // Helpers of the visible-entry block (not in the original: named views of the word arithmetic). They are the only way
    // the class touches the entries and the cache, so the layout lives in one place.
    SVisibleEntry* visibleEntry(std::uint32_t batch) const;        // +0x158 + 20 * batch
    SCacheSlot* cacheSlot(std::uint32_t batch) const;              // +0x154 + 12 * batch
    CBatchMesh::SSegment* visibleSegment(std::uint32_t batch, std::uint32_t position) const;  // cache[entry->firstIndex + position]

    boost::intrusive_ptr<CBatchMesh> mesh;                                 // +0x130
    std::uint32_t unk_134 = 0;                                             // +0x134
    std::uint32_t unk_138 = 2;                                             // +0x138
    std::int32_t maxSolidIndices = 0;                                      // +0x13c
    std::int32_t maxTransparentIndices = 0;                                // +0x140
    std::int32_t unk_144 = -1;                                             // +0x144 solidBatchCount (CBatchMesh::sort's answer)
    bool flag148 = true;                                                   // +0x148 "RegisterSolidBatchesOnce"
    boost::intrusive_ptr<video::IBuffer> dirtyBuffer;                      // +0x14c
    bool visibleIndexCacheOn = true;                                       // +0x150
    std::uint32_t* visibleIndexCache = nullptr;                            // +0x154 12-byte SCacheSlot records
    SVisibleEntry* visibleEntries = nullptr;                               // +0x158 20-byte records
    std::int32_t unk_15c = -1;                                             // +0x15c transparent batch base
};

}  // namespace glitch::scene
