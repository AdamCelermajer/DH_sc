// Wave 00 reconstructions (partial-row rerun). Built only against the layout
// headers: layouts/collada_db.hpp and layouts/scene.hpp.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/layouts/collada_db.hpp"
#include "glitch-scene-collada-gui/layouts/scene.hpp"

namespace glitch::collada::recon_v2_00 {

// 0x60e2bc _ZNK6glitch7collada16CColladaDatabase19getAnimationSegmentEi
// Returns the segment entry that contains a2 (begin <= a2 < end). Port difference:
// the original returns a guest address (int); this returns the host entry pointer.
const glitch::collada::SAnimationSegment* getAnimationSegment(
    const glitch::collada::CColladaDatabase& self, int a2);

// 0x578c78 _ZN6glitch5scene10CBatchMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE
// The original stamps the box with glitch::os::Timer::TickCount, which is not
// ported; the caller supplies that tick.
void setBoundingBox(glitch::scene::CBatchMesh& self,
                    const glitch::core::aabbox3d<float>& box,
                    std::uint32_t tickCount);

// 0x57aa8c _ZNK6glitch5scene10CBatchMesh14getBoundingBoxEv
// Refresh runs when the dirty byte is set, or when the source is set and the stamp
// differs from the tick. The refresh (updateBoundingBox, not ported) is supplied by
// the caller, and so is the tick (os::Timer::TickCount, not ported).
const glitch::core::aabbox3d<float>* getBoundingBox(
    const glitch::scene::CBatchMesh& self,
    std::uint32_t tickCount,
    void (*updateBoundingBox)(const glitch::scene::CBatchMesh&));

// 0x6bbedc _ZN6glitch5scene5CMesh5clearEv
// Empties the buffer range by moving the end to the begin. The release of the
// SBuffer references is not modelled (no refcount model).
void clearMeshBuffers(glitch::scene::CMesh& self);

}  // namespace glitch::collada::recon_v2_00
