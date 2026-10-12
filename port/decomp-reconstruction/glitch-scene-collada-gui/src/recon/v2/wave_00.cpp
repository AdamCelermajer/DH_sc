#include "glitch-scene-collada-gui/recon/v2/wave_00.hpp"

namespace glitch::collada::recon_v2_00 {

using glitch::scene::CBatchMesh;
using glitch::scene::CMesh;

// 0x60e2bc _ZNK6glitch7collada16CColladaDatabase19getAnimationSegmentEi
const SAnimationSegment* getAnimationSegment(const CColladaDatabase& self, int a2) {
    // record -> data (+0x24) -> tables (+0x20) -> segments record (+0x30) {count, base}
    const auto* segments = self.record->data->tables->segments;
    const std::int32_t count = segments->count;
    if (count == 0) {
        return nullptr;
    }
    const SAnimationSegment* base = segments->segments;
    if (count > 0) {
        for (std::int32_t i = 0; i < count; ++i) {
            const SAnimationSegment* entry = base + i;
            if (entry->begin <= a2 && a2 < entry->end) {
                return entry;
            }
        }
    }
    // Not found, or a negative count: the original returns base + 24 * (count - 1)
    // (the last entry) with no search.
    return base + (count - 1);
}

// 0x578c78 _ZN6glitch5scene10CBatchMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE
void setBoundingBox(CBatchMesh& self, const core::aabbox3d<float>& box, std::uint32_t tickCount) {
    self.boundingBoxStamp = tickCount;
    self.boundingBox = box;
}

// 0x57aa8c _ZNK6glitch5scene10CBatchMesh14getBoundingBoxEv
const core::aabbox3d<float>* getBoundingBox(const CBatchMesh& self, std::uint32_t tickCount,
                                            void (*updateBoundingBox)(const CBatchMesh&)) {
    if (self.boundingBoxDirty != 0 ||
        (self.boundingBoxSource != 0 && self.boundingBoxStamp != tickCount)) {
        updateBoundingBox(self);
    }
    return &self.boundingBox;
}

// 0x6bbedc _ZN6glitch5scene5CMesh5clearEv
void clearMeshBuffers(CMesh& self) {
    if (self.buffersBegin != self.buffersEnd) {
        self.buffersEnd = self.buffersBegin;
    }
}

}  // namespace glitch::collada::recon_v2_00
