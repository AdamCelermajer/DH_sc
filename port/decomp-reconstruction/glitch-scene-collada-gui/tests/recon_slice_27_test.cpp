// Layout check for recon slice 27. The CMeshBuffer constructor rows (0x6d11d4, 0x6d5ed4)
// depend on these offsets, so the test pins them.
#include <cstddef>

#include "glitch-scene-collada-gui/recon/slice_27.hpp"

int main() {
    using glitch::scene::CMeshBuffer;
    if (sizeof(CMeshBuffer) != 0x38) return 1;
    if (offsetof(CMeshBuffer, vertexStreams) != 0x14) return 1;
    if (offsetof(CMeshBuffer, primitiveWord24) != 0x24) return 1;
    if (offsetof(CMeshBuffer, primitiveWord28) != 0x28) return 1;
    if (offsetof(CMeshBuffer, primitiveHalf2C) != 0x2c) return 1;
    if (offsetof(CMeshBuffer, owner) != 0x30) return 1;
    return 0;
}
