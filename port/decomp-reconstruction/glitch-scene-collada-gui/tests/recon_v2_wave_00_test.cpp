// Tests for src/recon/v2/wave_00.cpp. Host-side only; no externs are needed.
#include <cstdio>
#include <type_traits>
#include <utility>

#include "glitch-scene-collada-gui/recon/v2/wave_00.hpp"

namespace {

int failures = 0;

#define CHECK(cond)                                                   \
    do {                                                              \
        if (!(cond)) {                                                \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                               \
        }                                                             \
    } while (0)

using namespace glitch;
using DataHolder = std::remove_pointer_t<decltype(std::declval<collada::CResFile&>().data)>;
using TableHolder = std::remove_pointer_t<decltype(std::declval<collada::SDatabaseTables&>().segments)>;

// Builds record -> data -> tables -> {count, segments} around a segment array.
struct Db {
    collada::SDatabaseTables tables{};
    TableHolder segRecord{};
    DataHolder dataHolder{};
    collada::CResFile record{};
    collada::CColladaDatabase db{};

    Db(collada::SAnimationSegment* segs, std::int32_t count) {
        segRecord.count = count;
        segRecord.segments = segs;
        tables.segments = &segRecord;
        dataHolder.tables = &tables;
        record.data = &dataHolder;
        db.record = &record;
    }
};

void testAnimationSegment() {
    collada::SAnimationSegment segs[3] = {{0, 10, {}}, {10, 20, {}}, {20, 30, {}}};
    {
        Db d(segs, 3);
        CHECK(collada::recon_v2_00::getAnimationSegment(d.db, 0) == &segs[0]);
        CHECK(collada::recon_v2_00::getAnimationSegment(d.db, 15) == &segs[1]);
        CHECK(collada::recon_v2_00::getAnimationSegment(d.db, 29) == &segs[2]);
        // Not found: the last entry is returned, as in the original.
        CHECK(collada::recon_v2_00::getAnimationSegment(d.db, 99) == &segs[2]);
    }
    {
        Db d(segs, 0);
        CHECK(collada::recon_v2_00::getAnimationSegment(d.db, 5) == nullptr);
    }
    {
        // Negative count: base + 24 * (count - 1) with no search. Base is the
        // element 3 of a 4-entry array, count -2, so the result is element 0.
        collada::SAnimationSegment arr[4] = {};
        Db d(&arr[3], -2);
        CHECK(collada::recon_v2_00::getAnimationSegment(d.db, 5) == &arr[0]);
    }
}

int refreshCalls = 0;
void countRefresh(const glitch::scene::CBatchMesh&) { ++refreshCalls; }

void testBoundingBox() {
    scene::CBatchMesh m{};
    core::aabbox3d<float> box{{1, 2, 3}, {4, 5, 6}};
    collada::recon_v2_00::setBoundingBox(m, box, 77u);
    CHECK(m.boundingBoxStamp == 77u);
    CHECK(m.boundingBox.MinEdge.X == 1.0f && m.boundingBox.MaxEdge.Z == 6.0f);

    // Dirty flag set: refresh runs and the box pointer is returned.
    refreshCalls = 0;
    m.boundingBoxDirty = 1;
    const core::aabbox3d<float>* p = collada::recon_v2_00::getBoundingBox(m, 77u, countRefresh);
    CHECK(refreshCalls == 1);
    CHECK(p == &m.boundingBox);

    // Clean, no source: no refresh.
    refreshCalls = 0;
    m.boundingBoxDirty = 0;
    m.boundingBoxSource = 0;
    collada::recon_v2_00::getBoundingBox(m, 77u, countRefresh);
    CHECK(refreshCalls == 0);

    // Source set and stamp differs from the tick: refresh.
    m.boundingBoxSource = 1;
    collada::recon_v2_00::getBoundingBox(m, 78u, countRefresh);
    CHECK(refreshCalls == 1);

    // Source set and stamp equals the tick: no refresh.
    collada::recon_v2_00::getBoundingBox(m, 77u, countRefresh);
    CHECK(refreshCalls == 1);
}

void testClearMeshBuffers() {
    scene::CMesh mesh{};
    mesh.buffersBegin = 0x1000;
    mesh.buffersEnd = 0x1024;
    collada::recon_v2_00::clearMeshBuffers(mesh);
    CHECK(mesh.buffersEnd == 0x1000);
    CHECK(mesh.buffersBegin == 0x1000);
    collada::recon_v2_00::clearMeshBuffers(mesh);  // empty: no change
    CHECK(mesh.buffersEnd == 0x1000);
}

}  // namespace

int main() {
    testAnimationSegment();
    testBoundingBox();
    testClearMeshBuffers();
    if (failures == 0) {
        std::printf("recon_v2_wave_00: all checks passed\n");
        return 0;
    }
    std::printf("recon_v2_wave_00: %d failure(s)\n", failures);
    return 1;
}
