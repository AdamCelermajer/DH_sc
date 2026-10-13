// Slice 29 reconstruction (see include/glitch-scene-collada-gui/recon/slice_29.hpp).
// Each function keeps the original address and mangled symbol in its comment.
// Only the bodies the layouts confirm are rebuilt here; the rest are ledgered
// in ledger_parts/slice_29.csv with their reason.

#include "glitch-scene-collada-gui/recon/slice_29.hpp"

namespace glitch::scene::recon_29 {

// 0x7031d8 _ZN6glitch5scene17CMeshConnectivityC2ERKN5boost13intrusive_ptrIKNS0_11CMeshBufferEEE
// 0x70320c _ZN6glitch5scene17CMeshConnectivityC1ERKN5boost13intrusive_ptrIKNS0_11CMeshBufferEEE
// (same body; one definition serves both the C1 and C2 entries)
CMeshConnectivity::CMeshConnectivity(const boost::intrusive_ptr<const CMeshBuffer>& buffer_in)
    : buffer(buffer_in) {
    // The copy of the counting handle takes the reference on the buffer (+0x04), as the original does by hand.
}

// 0x6ffdf8 _ZN6glitch5scene22CShadowVolumeSceneNode11SSilhouetteC1ERKN5boost13intrusive_ptrIKNS0_11CMeshBufferEEE
// 0x6ffe20 _ZN6glitch5scene22CShadowVolumeSceneNode11SSilhouetteC2ERKN5boost13intrusive_ptrIKNS0_11CMeshBufferEEE
// (same body; one definition serves both entries)
CShadowVolumeSceneNode::SSilhouette::SSilhouette(const boost::intrusive_ptr<const CMeshBuffer>& buffer)
    : CMeshConnectivity(buffer) {}

// 0x6fe860 _ZN6glitch5scene22CShadowVolumeSceneNode13SShadowVolumeC2Ev
// 0x6fe8cc _ZN6glitch5scene22CShadowVolumeSceneNode13SShadowVolumeC1Ev
// (same body; the member initializers in the header hold the constructor's values)
CShadowVolumeSceneNode::SShadowVolume::SShadowVolume() = default;

}  // namespace glitch::scene::recon_29
