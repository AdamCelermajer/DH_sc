// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CModularSkinnedMesh-0.
// Owner: glitch::collada::CModularSkinnedMesh (declared in
// glitch-scene-collada-gui/modular_skinned_mesh.hpp).

#include "glitch-scene-collada-gui/modular_skinned_mesh.hpp"

namespace glitch::collada {

namespace {

// Module bounding box. The original calls the module's virtual at ISkinnedMesh vtable slot +0x24
// (offset 36 of the vtable) and reads the returned const aabbox3d<float>* as six words. That slot
// is pure in the ISkinnedMesh base vtable and its concrete name is not mapped yet, so the hook is
// null. While it is null, a module contributes nothing to the box (see computeBoundingBox).
using ModuleBoundingBoxFn = const core::aabbox3d<float>* (*)(const ISkinnedMesh* module);
ModuleBoundingBoxFn g_moduleBoundingBox = nullptr;

inline const core::aabbox3d<float>* moduleBoundingBox(const ISkinnedMesh* module) {
    return g_moduleBoundingBox ? g_moduleBoundingBox(module) : nullptr;
}

}  // namespace

// 0x646c1c glitch::collada::CModularSkinnedMesh::computeBoundingBox()
// partial: the original's search for the first module, when no module is non-null, runs its
// second loop one element past the end of the module vector (asm 0x646c80 to 0x646cc0). That
// read is not reproduced; here no module is found and the box is left as it was.
// The original's return value (R0 at exit) is not used by any caller, so this returns void.
void CModularSkinnedMesh::computeBoundingBox() {
    auto first = currentModules.begin();                 // +0x24
    const auto last = currentModules.end();              // +0x28

    // Skip leading slots whose module pointer (+0x04) is null.
    while (first != last && first->module == nullptr) {
        ++first;
    }

    if (first != last) {
        // The first non-null module seeds the box at +0x40..+0x54 (six words, min then max).
        if (const core::aabbox3d<float>* seed = moduleBoundingBox(first->module)) {
            boundingBox = *seed;
        }
        // Every later non-null module is merged into the box with addInternalBox.
        for (auto it = first + 1; it != last; ++it) {
            if (it->module == nullptr) {
                continue;
            }
            if (const core::aabbox3d<float>* box = moduleBoundingBox(it->module)) {
                boundingBox.addInternalBox(*box);
            }
        }
    }

    boundingBoxDirty = false;  // +0x59 byte set to 0
}

}  // namespace glitch::collada
