// Checks for the reconstructed visible-entry block of glitch::scene::CBatchSceneNode: postCompile 0x57ee34 (the entry array
// and the 12-byte cache slots), computeMaxIndices 0x57e5b4, clearVisibleSegments 0x57e604, updateInfo 0x57e650,
// invalidateVisibleIndexCache 0x57ebec and updateIndices 0x57f704.
//
// The visible-entry array is an array of CBatchMesh::SSegment POINTERS: the original is a 32-bit ARM binary, so a segment
// pointer is one 32-bit word. The test fills the block the way postCompile does and checks the word arithmetic against the
// listing (record {count, indexBase, tick, flags, firstIndex}, stride 20; cache slot {head, capacity, base}, stride 12;
// cache[firstIndex] is a CBatchMesh::SSegment*).
//
// This translation unit needs CBatchSceneNode.cpp and CBatchMesh.cpp linked (the engine sources; see the track B test list).
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "glitch-core-io/os/logging.hpp"
#include "glitch-scene-collada-gui/scene/CBatchSceneNode.hpp"

namespace {

int failures = 0;

void check(bool condition, const char* what) {
    if (!condition) {
        std::printf("FAIL: %s\n", what);
        ++failures;
    }
}

using glitch::scene::CBatchMesh;
using glitch::scene::CBatchSceneNode;

// postCompile: one block of 4 * (totalIndices + 20 * batchCount) bytes, the record array first (20 bytes per batch), then
// the index area the records point into.
void testPostCompileBlock() {
    boost::intrusive_ptr<CBatchMesh> mesh(new CBatchMesh(), true);
    mesh->addBatch();
    mesh->addBatch();
    CBatchSceneNode node(3, mesh);

    node.postCompile();
    check(node.unk_15c == 2, "postCompile leaves the transparent base at the batch count without transparent materials");
    check(node.visibleEntries != nullptr, "postCompile allocates the visible-entry array (+0x158)");
    check(node.maxSolidIndices == 0 && node.maxTransparentIndices == 0, "postCompile clears and recomputes the maxima");
    check(node.visibleIndexCache != nullptr, "postCompile ends with invalidateVisibleIndexCache(-1): the cache block exists");
    check(node.visibleEntries[0].firstIndex == 10, "record 0 firstIndex is 20 * batchCount / 4 words");
    check(node.visibleEntries[1].firstIndex == 10, "record 1 firstIndex advances by the batch's segment-info count");
    check(node.visibleEntries[0].tick == 1 && node.visibleEntries[0].flags == 1,
          "postCompile stamps tick 1 and flags 1 into every record");
    const auto* slots = reinterpret_cast<CBatchSceneNode::SCacheSlot*>(node.visibleIndexCache);
    check(slots[0].count == 0, "cache slot 0 starts with a zero write head");
    check(slots[0].capacity == 0, "cache slot word 1 is the total index count (0 here)");
    check(slots[0].base == 2, "cache slot word 2 is the running offset, 12 * batchCount / 4 words for the first batch");
    check(slots[1].base == 2, "cache slot 1 base advances by the batch's segment-info count");
}

// computeMaxIndices 0x57e5b4: sum of (segment->indexEnd - segment->indexBegin) over the batch's visible segment pointers.
void testComputeMaxIndices() {
    CBatchSceneNode node(1);
    auto* block = new std::uint32_t[32];
    node.visibleIndexCache = block;
    node.visibleEntries = reinterpret_cast<CBatchSceneNode::SVisibleEntry*>(block);
    CBatchMesh::SSegment segments[2];
    segments[0].indexBegin = 10;
    segments[0].indexEnd = 34;
    segments[1].indexBegin = 4;
    segments[1].indexEnd = 9;
    node.visibleEntries[0].count = 2;
    node.visibleEntries[0].firstIndex = 10;
    node.visibleEntries[1].count = 0;
    node.visibleEntries[1].firstIndex = 12;
    reinterpret_cast<CBatchMesh::SSegment**>(block)[10] = &segments[0];
    reinterpret_cast<CBatchMesh::SSegment**>(block)[11] = &segments[1];

    check(node.computeMaxIndices(0) == 29, "computeMaxIndices sums (end - begin) over the segment pointers (0x57e5b4)");
    check(node.computeMaxIndices(1) == 0, "an empty batch has no indices");
    node.visibleEntries = nullptr;
    node.visibleIndexCache = nullptr;
    delete[] block;
}

// updateInfo 0x57e650: sums the counts, stamps the tick and ORs flag bit 0 unless indexBase equals the count.
void testUpdateInfo() {
    CBatchSceneNode node(1);
    node.visibleEntries = reinterpret_cast<CBatchSceneNode::SVisibleEntry*>(new std::uint32_t[20]);
    glitch::os::Timer::TickCount = 4242u;
    node.visibleEntries[0].count = 3;
    node.visibleEntries[0].indexBase = 3;  // matches: the flags stay
    node.visibleEntries[1].count = 2;
    node.visibleEntries[1].indexBase = 7;  // does not match: bit 0 is set
    node.visibleEntries[1].flags = 2;
    node.visibleEntries[2].count = 0;  // empty: no tick, no flag
    node.visibleEntries[2].tick = 9;

    check(node.updateInfo(0, 3) == 5, "updateInfo sums the counts of the range (0x57e650)");
    check(node.visibleEntries[0].tick == 4242u && node.visibleEntries[0].flags == 0,
          "a record whose indexBase equals its count keeps its flags and takes the tick");
    check(node.visibleEntries[1].tick == 4242u && node.visibleEntries[1].flags == 3, "a mismatching record gets flag bit 0");
    check(node.visibleEntries[2].tick == 9, "an empty record is left alone");
    check(node.updateInfo(2, 2) == 0, "an empty range is 0");
    check(node.updateInfo(4, 3) == 0, "batch >= segment is 0");
    delete[] node.visibleEntries;
    node.visibleEntries = nullptr;
}

// clearVisibleSegments 0x57e604: zeroes word 0 of one record per MESH batch (the mesh's virtual +0x10), not per segment.
void testClearVisibleSegments() {
    boost::intrusive_ptr<CBatchMesh> mesh(new CBatchMesh(), true);
    mesh->addBatch();
    mesh->addBatch();
    CBatchSceneNode node(1, mesh);
    node.visibleEntries = reinterpret_cast<CBatchSceneNode::SVisibleEntry*>(new std::uint32_t[20]);
    for (int i = 0; i < 3; ++i) {
        node.visibleEntries[i].count = 7 + i;
    }
    check(node.clearVisibleSegments() == 2, "clearVisibleSegments returns the mesh's batch count (0x57e604)");
    check(node.visibleEntries[0].count == 0 && node.visibleEntries[1].count == 0, "the two mesh batches are cleared");
    check(node.visibleEntries[2].count == 9, "a record past the mesh's batch count is untouched");
    delete[] node.visibleEntries;
    node.visibleEntries = nullptr;
}

// A minimal glitch::video::IBuffer for updateIndices 0x57f704: the port's IBuffer holds its data at m_data (+0x08), so the
// pointer map(2) / the const map(1) answer is the array the test owns.
class TestBuffer : public glitch::video::IBuffer {
public:
    TestBuffer(std::uint8_t* data, std::uint32_t size)
        : IBuffer(static_cast<glitch::video::E_BUFFER_TYPE>(1), static_cast<glitch::video::E_BUFFER_USAGE>(4), size, data, 0) {}

    void bindImpl(bool) override {}
    void unbindImpl() override {}
    std::uint8_t* mapImpl(std::uint32_t) const override { return m_data; }
    void unmapImpl() const override { ++unmaps; }
    std::uint8_t* getMappedPtr() const override { return m_data; }
    boost::intrusive_ptr<glitch::video::IBuffer> cloneImpl() const override { return {}; }

    mutable int unmaps = 0;
};

// updateIndices: the segment's index range is copied out of the source buffer into the destination, the segment pointer is
// read as a 32-bit word of the cache block, and the batch's write head is reset.
void testUpdateIndices() {
    CBatchSceneNode node(1);
    auto* block = new std::uint32_t[16];
    node.visibleIndexCache = block;
    node.visibleEntries = reinterpret_cast<CBatchSceneNode::SVisibleEntry*>(new std::uint32_t[8]);
    CBatchMesh::SSegment segment;
    segment.indexBegin = 2;
    segment.indexEnd = 5;  // three indices
    node.visibleEntries[0].count = 1;
    node.visibleEntries[0].firstIndex = 5;
    reinterpret_cast<CBatchMesh::SSegment**>(block)[5] = &segment;
    reinterpret_cast<CBatchSceneNode::SCacheSlot*>(block)[0].count = 77;

    std::uint16_t indices[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    std::uint16_t out[8] = {};
    boost::intrusive_ptr<glitch::video::IBuffer> source(new TestBuffer(reinterpret_cast<std::uint8_t*>(indices), sizeof(indices)));
    boost::intrusive_ptr<glitch::video::IBuffer> destination(new TestBuffer(reinterpret_cast<std::uint8_t*>(out), sizeof(out)));

    const std::int32_t copied = node.updateIndices(0, destination, boost::intrusive_ptr<const glitch::video::IBuffer>(source));
    check(copied == 3, "updateIndices returns the copied index count (0x57f704)");
    check(out[0] == 2 && out[1] == 3 && out[2] == 4, "the segment's [begin, end) indices are copied");
    check(out[3] == 0, "nothing past the segment is written");
    check(reinterpret_cast<CBatchSceneNode::SCacheSlot*>(block)[0].count == 0, "the batch's cache-slot write head is cleared");
    check(destination->m_mapCount13 == 0 && source->m_mapCount13 == 0, "both buffers lose their map reference");

    auto* entryBlock = node.visibleEntries;
    node.visibleEntries = nullptr;
    node.visibleIndexCache = nullptr;
    delete[] block;
    delete[] entryBlock;
}

// invalidateVisibleIndexCache(-1) rebuilds the slot table: capacity = the total index count, base = the running offset.
void testInvalidateVisibleIndexCache() {
    boost::intrusive_ptr<CBatchMesh> mesh(new CBatchMesh(), true);
    mesh->addBatch();  // batch 0: segment infos [0, 0)
    mesh->segmentInfos.push_back({0, 0});
    mesh->segmentInfos.push_back({0, 1});
    mesh->batches[0].endSegmentInfo = 2;  // two segment infos in batch 0
    mesh->addBatch();                     // batch 1: none
    CBatchSceneNode node(1, mesh);
    node.visibleEntries = reinterpret_cast<CBatchSceneNode::SVisibleEntry*>(new std::uint32_t[12]);
    node.invalidateVisibleIndexCache(0xFFFFFFFFu);

    const auto* slots = reinterpret_cast<CBatchSceneNode::SCacheSlot*>(node.visibleIndexCache);
    check(slots[0].capacity == 2, "the slot's word 1 is the total index count (0x57ebec)");
    check(slots[0].base == 6, "the index area starts after the two 12-byte slots (6 words)");
    check(slots[1].base == 8, "the next batch's base advances by its segment count");
    check(node.visibleEntries[0].flags == 1, "the batch's entry flags get bit 0");
    node.visibleEntries = nullptr;
    node.setVisibleIndexCache(false);
}

}  // namespace

int main() {
    testPostCompileBlock();
    testComputeMaxIndices();
    testUpdateInfo();
    testClearVisibleSegments();
    testUpdateIndices();
    testInvalidateVisibleIndexCache();
    if (failures == 0) {
        std::printf("scene_batch_visible_test: all checks passed\n");
        return 0;
    }
    return 1;
}
