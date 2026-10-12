// Wave 03 reconstructions. Only the bodies that can be written against
// layouts/scene.hpp are here. See include/glitch-scene-collada-gui/recon/v2/wave_03.hpp.
#include "glitch-scene-collada-gui/recon/v2/wave_03.hpp"

namespace glitch::scene {

// Extern, not reconstructed in this wave: createMeshCopy is the stub at
// 0x59b2a4 (slice_07). Its signature follows the pseudocode call site in
// createMeshWith2TCoords: the result is returned through the first argument.
void createMeshCopy(IMesh** out, IMesh* const* mesh, video::IVideoDriver* driver,
                    unsigned int flagsA, unsigned int flagsB);

// Extern: glitch::IReferenceCounted::drop applied to an IMesh. Its body
// (decrement and free at zero) is not in the layouts.
void referenceDrop(IMesh* object);

namespace recon_v2_03 {

IMesh** createMeshUniquePrimitives(IMesh** result, IMesh* const* a2) {
    IMesh* handle = *a2;
    *result = handle;
    if (handle != nullptr) {
        ++handle->referenceCount;
    }
    return result;
}

IMesh** createMeshWelded(IMesh** result, IMesh* const* a2) {
    IMesh* handle = *a2;
    *result = handle;
    if (handle != nullptr) {
        ++handle->referenceCount;
    }
    return result;
}

IMesh** createMeshWith2TCoords(IMesh** result, IMesh* const* a2, video::IVideoDriver* a3) {
    IMesh* copy = nullptr;
    createMeshCopy(&copy, a2, a3, 0xFFFFFFFFu, 6u);
    *result = copy;
    if (copy != nullptr) {
        ++copy->referenceCount;
    }
    if (copy != nullptr) {
        referenceDrop(copy);
    }
    return result;
}

}  // namespace recon_v2_03
}  // namespace glitch::scene
