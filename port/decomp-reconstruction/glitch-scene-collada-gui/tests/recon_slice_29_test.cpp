// Slice 29 test: checks the rebuilt constructors against their pseudocode.
#include "glitch-scene-collada-gui/recon/slice_29.hpp"

namespace rs = glitch::scene::recon_29;

int main() {
    // SShadowVolume default constructor (0x6fe860 / 0x6fe8cc).
    rs::CShadowVolumeSceneNode::SShadowVolume volume;
    if (volume.silhouette != nullptr) return 1;
    if (volume.word_0x30 != 0 || volume.word_0x3c != 0) return 1;
    if (volume.half_0x40 != 255 || volume.half_0x42 != 6) return 1;
    if (volume.half_0x44 != 0 || volume.half_0x46 != 0xFFFF || volume.half_0x48 != 0) return 1;

    // CMeshConnectivity constructor (0x7031d8): takes a reference on the buffer.
    glitch::scene::CMeshBuffer buffer;
    buffer.referenceCounter = 1;  // held by the test (stack object: the count must never reach zero)
    boost::intrusive_ptr<const glitch::scene::CMeshBuffer> handle(&buffer, true);
    if (buffer.referenceCounter != 2) return 1;
    rs::CMeshConnectivity connectivity(handle);
    if (connectivity.buffer.get() != &buffer) return 1;
    if (buffer.referenceCounter != 3) return 1;
    for (std::uint32_t word : connectivity.reserved_0x04) {
        if (word != 0) return 1;
    }

    // Null handle: no reference is taken and nothing is touched.
    boost::intrusive_ptr<const glitch::scene::CMeshBuffer> empty;
    rs::CMeshConnectivity none(empty);
    if (none.buffer.get() != nullptr) return 1;

    // SSilhouette constructor (0x6ffdf8): base constructor, then zeroed words.
    rs::CShadowVolumeSceneNode::SSilhouette silhouette(handle);
    if (buffer.referenceCounter != 4) return 1;
    if (silhouette.data_0x18 != nullptr || silhouette.array_0x24 != nullptr) return 1;
    if (silhouette.word_0x1c != 0 || silhouette.word_0x20 != 0) return 1;

    return 0;
}
