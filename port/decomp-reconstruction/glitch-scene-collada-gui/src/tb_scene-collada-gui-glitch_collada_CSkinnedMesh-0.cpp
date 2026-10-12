// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CSkinnedMesh-0.
// Owner: glitch::collada::CSkinnedMesh (glitch-scene-collada-gui/skinned_mesh.hpp).
//
// Every function keeps the original control flow from the Hex-Rays pseudocode. Calls to virtual
// slots and helpers whose classes are not mapped yet go through the file-local hooks below. A hook
// that is null makes its call a no-op, so the rows stay partial (see the ledger notes). Reference
// counts follow the original's intrusive_ptr assignments.

#include <cstring>

#include "glitch-scene-collada-gui/collada_database.hpp"
#include "glitch-scene-collada-gui/skinned_mesh.hpp"
#include "glitch-shared/reference_counted.hpp"

namespace glitch::collada {

namespace {

// constructGeometry(&out, this+12, driver, name). Returns an owned (+1) reference or null.
// The hook receives the owner because the embedded database copy at +0x0c is not stored yet.
using ConstructGeometryFn = glitch::IReferenceCounted* (*)(CSkinnedMesh* owner, video::IVideoDriver* driver,
                                                           const char* name);
ConstructGeometryFn g_constructGeometry = nullptr;

// constructController(&out, this+12, driver, name, root). Returns an owned (+1) reference or null.
using ConstructControllerFn = glitch::IReferenceCounted* (*)(CSkinnedMesh* owner, video::IVideoDriver* driver,
                                                             const char* name, CRootSceneNode* root);
ConstructControllerFn g_constructController = nullptr;

// Mesh vtable slot +0x24 (offset 36): returns a pointer to six words of bounding box, or null.
using MeshBoundingBoxFn = const float* (*)(glitch::IReferenceCounted* mesh);
MeshBoundingBoxFn g_meshBoundingBox = nullptr;

// Mesh vtable slot +0x14 (offset 20): returns an owned (+1) buffer reference for `index`, or null.
using MeshBufferRefFn = glitch::IReferenceCounted* (*)(glitch::IReferenceCounted* mesh, unsigned int index);
MeshBufferRefFn g_meshBufferRef = nullptr;

// Skin technique vtable slot +0x08 (third entry): writes the technique's box (six words) to `out`.
using TechniqueBoundingBoxFn = void (*)(float out[6], void* technique);
TechniqueBoundingBoxFn g_techniqueBoundingBox = nullptr;

// reverifySkinTechnique(entry), 0x66383c. Its result is not used by skin().
using ReverifySkinTechniqueFn = int (*)(CSkinnedMesh* owner, SSkinnedBufferEntry* entry);
ReverifySkinTechniqueFn g_reverifySkinTechnique = nullptr;

// Technique vtable slot +0x14 (offset 20): update for one buffer; nonzero means the bit is set.
using TechniqueUpdateFn = int (*)(void* technique, SSkinnedBufferEntry* entry, glitch::IReferenceCounted* buffer,
                                  std::uint32_t materialWord, std::uint8_t skinFlag);
TechniqueUpdateFn g_techniqueUpdate = nullptr;

// Technique vtable slot +0x18 (offset 24): applies the technique to one buffer.
using TechniqueApplyFn = void (*)(void* technique, SSkinnedBufferEntry* entry, glitch::IReferenceCounted* buffer);
TechniqueApplyFn g_techniqueApply = nullptr;

}  // namespace

// 0x663648 glitch::collada::CSkinnedMesh::computeBoundingBox()
// partial: the box comes from the skin technique at +0x3c through its vtable slot +0x08, which is not
// named yet (g_techniqueBoundingBox). With the hook null, the stored box is left unchanged; the
// original copies whatever the slot returns. The original's R0 at exit is the last copied word and is
// not used by any caller, so this returns void. A null technique is a crash in the original; here it
// is skipped.
void CSkinnedMesh::computeBoundingBox() {
    if (g_techniqueBoundingBox == nullptr || skinTechnique == nullptr) return;
    float box[6] = {};
    g_techniqueBoundingBox(box, skinTechnique);
    std::memcpy(boundingBox, box, sizeof(boundingBox));  // +0x24..+0x38 from the six returned words
}

// 0x664af8 glitch::collada::CSkinnedMesh::instanciateMesh(glitch::video::IVideoDriver*, glitch::collada::CRootSceneNode*)
// partial: the name passed to the constructors is *(*(this+7)+112)+1 (the controller name after '#'). That
// data layout is not mapped, so the name is null here. The geometry and controller constructors are hooks.
// Not reproduced: the buffer-table resize at +0x5c (SSkinBuffer, 0x14 bytes each, count from the mesh's
// vtable slot +0x10). The owner keeps no buffer count, so the resize is skipped. The original's
// unconditional call on this+26 when neither constructor returned a mesh is also skipped.
void CSkinnedMesh::instanciateMesh(video::IVideoDriver* driver, CRootSceneNode* root) {
    const char* name = nullptr;  // see note above

    // Geometry first. The original grabs and drops the returned reference around the test, which nets to
    // nothing, then jumps to the shared adoption block.
    glitch::IReferenceCounted* v8 = g_constructGeometry ? g_constructGeometry(this, driver, name) : nullptr;
    if (v8 == nullptr) {
        v8 = g_constructController ? g_constructController(this, driver, name, root) : nullptr;
    }

    if (v8 != nullptr) {
        // Shared block (LABEL_5): this->mesh takes its own reference; the old mesh is dropped.
        ++v8->referenceCounter;
        glitch::IReferenceCounted* previous = mesh;
        mesh = v8;
        if (previous != nullptr) previous->drop();

        // Box from the mesh's vtable slot +0x24: six words copied to +0x24..+0x38.
        const float* box = g_meshBoundingBox ? g_meshBoundingBox(mesh) : nullptr;
        if (box != nullptr) std::memcpy(boundingBox, box, sizeof(boundingBox));
    }

    // Release the local reference the constructor returned.
    if (v8 != nullptr) v8->drop();
}

// 0x664ee0 glitch::collada::CSkinnedMesh::skin(unsigned int)
// partial: reverifySkinTechnique (0x66383c) is a hook. The technique update (slot +0x14) and apply (slot +0x18)
// on the technique at entry+0x0c, and the mesh's buffer reference (slot +0x14), are not named. The fourth
// argument of the update call is a pointer chain through the material fields at entry+0x04, which are not
// mapped; it is passed as 0 (the update is skipped while its hook is null). With the hooks null the state
// is left as it was. The original returns the reference it just dropped, which is a dangling pointer on
// return; that return value is not reproduced, so this returns void.
void CSkinnedMesh::skin(unsigned int index) {
    SSkinnedBufferEntry& entry = buffers[index];  // +0x5c table, 0x14-byte stride

    if (g_reverifySkinTechnique != nullptr) g_reverifySkinTechnique(this, &entry);

    if (entry.requestedState != entry.appliedState) {
        if (g_techniqueUpdate != nullptr && g_meshBufferRef != nullptr && mesh != nullptr &&
            entry.technique != nullptr) {
            glitch::IReferenceCounted* buffer = g_meshBufferRef(mesh, index);
            const int updated = g_techniqueUpdate(entry.technique, &entry, buffer, /*not reproduced*/ 0u, skinFlag);
            const std::uint32_t bit = 1u << (index & 0x1fu);
            if (updated) {
                techniqueMask |= bit;  // +0x14
            } else {
                techniqueMask &= ~bit;
            }
            if (buffer != nullptr) buffer->drop();
            entry.appliedState = entry.requestedState;  // +0x11 <- +0x10
        }
    }

    // The original re-reads the entry after the update branch; the entry pointer is unchanged.
    void* technique = entry.technique;
    if (g_techniqueApply != nullptr && g_meshBufferRef != nullptr && mesh != nullptr && technique != nullptr) {
        glitch::IReferenceCounted* buffer = g_meshBufferRef(mesh, index);
        g_techniqueApply(technique, &entry, buffer);
        if (buffer != nullptr) buffer->drop();
    }
}

// 0x6664f0 glitch::collada::CSkinnedMesh::CSkinnedMesh(const glitch::collada::CColladaDatabase&, glitch::video::IVideoDriver*, glitch::collada::SController&, glitch::collada::CRootSceneNode*)
// partial: the stored database copy at +0x0c..+0x14 has no member here, so `database` is not kept. The vtable
// stores (off_976818, off_9847D0, the unk_9F6F08 default) are not made. Not reproduced: the SControllerData
// pointer at +0x1c (from SController +0x08), the four technique objects (CColladaHardwareTextureSkinTechnique
// at +0x3c and the matrix, quaternion and software skin techniques at +0x40..+0x48, each 0x30 to 0x38 bytes),
// and the onDemand lookups for the skin data and the per-joint records. Those techniques need their
// constructors and sizes checked against the layout headers. The member defaults and instanciateMesh are
// done in the original's order.
CSkinnedMesh::CSkinnedMesh(const CColladaDatabase& database, video::IVideoDriver* driver, SController& controller,
                           CRootSceneNode* root)
    : controllerName(controller.name) {
    static_cast<void>(database);  // +0x0c copy not reproduced (see note above)

    // Default box, stored before instanciateMesh: min (-10, -10, -10) then max (1, 1, 1).
    boundingBox[0] = -10.0f;
    boundingBox[1] = -10.0f;
    boundingBox[2] = -10.0f;
    boundingBox[3] = 1.0f;
    boundingBox[4] = 1.0f;
    boundingBox[5] = 1.0f;

    skinFlag = 0;
    techniqueMask = 0;

    instanciateMesh(driver, root);
}

}  // namespace glitch::collada
