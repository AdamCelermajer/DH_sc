#include "glitch-scene-collada-gui/recon/slice_07.hpp"

#include "glitch-video/material.hpp"

#include <cmath>
#include <cstring>

namespace glitch::collada::recon_07 {

namespace {

// Element read/write helpers for the index buffers. The original indexes these
// by element (not by byte) after scaling by the index size; memcpy keeps the
// access valid for any alignment.
template <typename T>
T loadElement(const void* base, std::uint32_t index) {
    T value{};
    std::memcpy(&value, static_cast<const std::uint8_t*>(base) + index * sizeof(T), sizeof(T));
    return value;
}

template <typename T>
void storeElement(void* base, std::uint32_t index, T value) {
    std::memcpy(static_cast<std::uint8_t*>(base) + index * sizeof(T), &value, sizeof(T));
}

float squaredDistance(const core::vector3d<float>& a, const core::vector3d<float>& b) {
    const float dx = a.X - b.X;
    const float dy = a.Y - b.Y;
    const float dz = a.Z - b.Z;
    return dx * dx + dy * dy + dz * dz;
}

// Centre of the kd-tree box (min + max) / 2 per axis; the box is at +0x04 of the pair.
core::vector3d<float> kdCentre(const KdEntry& entry) {
    const core::aabbox3d<float>& box = entry.second;
    return core::vector3d<float>{(box.MinEdge.X + box.MaxEdge.X) * 0.5f,
                                 (box.MinEdge.Y + box.MaxEdge.Y) * 0.5f,
                                 (box.MinEdge.Z + box.MaxEdge.Z) * 0.5f};
}

float axisComponent(const core::vector3d<float>& v, char axis) {
    switch (axis) {
        case 0:
            return v.X;
        case 1:
            return v.Y;
        default:
            return v.Z;
    }
}

}  // namespace

// 0x59a0a4 _ZN6glitch5scene12_GLOBAL__N_116getTriangleIndicesENS_5video12E_INDEX_TYPEEPKvjRjS5_S5_
// Partial: the original's return value (pointer, index+1, or null) is not modelled.
void getTriangleIndices(IndexType type, const void* indices, std::uint32_t triangle,
                        std::uint32_t& i0, std::uint32_t& i1, std::uint32_t& i2) {
    switch (type) {
        case kIndex16:
            i0 = loadElement<std::uint16_t>(indices, triangle);
            i1 = loadElement<std::uint16_t>(indices, triangle + 1);
            i2 = loadElement<std::uint16_t>(indices, triangle + 2);
            break;
        case kIndex32:
            i0 = loadElement<std::uint32_t>(indices, triangle);
            i1 = loadElement<std::uint32_t>(indices, triangle + 1);
            i2 = loadElement<std::uint32_t>(indices, triangle + 2);
            break;
        case kIndex8:
            i0 = loadElement<std::uint8_t>(indices, triangle);
            i1 = loadElement<std::uint8_t>(indices, triangle + 1);
            i2 = loadElement<std::uint8_t>(indices, triangle + 2);
            break;
        default:
            break;
    }
}

// 0x59a130 _ZN6glitch5scene12_GLOBAL__N_110swapIndicesENS_5video12E_INDEX_TYPEEPvjj
// Returns the element that was at index a before the swap; unknown types return the type.
std::uint32_t swapIndices(IndexType type, void* indices, std::uint32_t a, std::uint32_t b) {
    switch (type) {
        case kIndex16: {
            const auto old = loadElement<std::uint16_t>(indices, a);
            storeElement<std::uint16_t>(indices, a, loadElement<std::uint16_t>(indices, b));
            storeElement<std::uint16_t>(indices, b, old);
            return old;
        }
        case kIndex32: {
            const auto old = loadElement<std::uint32_t>(indices, a);
            storeElement<std::uint32_t>(indices, a, loadElement<std::uint32_t>(indices, b));
            storeElement<std::uint32_t>(indices, b, old);
            return old;
        }
        case kIndex8: {
            const auto old = loadElement<std::uint8_t>(indices, a);
            storeElement<std::uint8_t>(indices, a, loadElement<std::uint8_t>(indices, b));
            storeElement<std::uint8_t>(indices, b, old);
            return old;
        }
        default:
            return type;
    }
}

// 0x59aa28 _ZN6glitch5scene12_GLOBAL__N_114getAngleWeightERKNS_4core8vector3dIfEES5_S5_
// out[0] is the angle at p0, out[1] at p1, out[2] at p2 (law of cosines on the
// squared edge lengths; p0 is the original a2, p1 a3 and p2 a4).
void getAngleWeight(core::vector3d<float>& out, const core::vector3d<float>& p0,
                    const core::vector3d<float>& p1, const core::vector3d<float>& p2) {
    const float q1r2 = squaredDistance(p1, p2);  // |QR|^2
    const float p0r2 = squaredDistance(p0, p2);  // |PR|^2
    const float p0q1 = squaredDistance(p0, p1);  // |PQ|^2
    const float lenQR = std::sqrt(q1r2);
    const float lenPR = std::sqrt(p0r2);
    const float lenPQ = std::sqrt(p0q1);

    const float angleP = std::acos((p0r2 + p0q1 - q1r2) / (lenPQ * (lenPR + lenPR)));
    const float angleQ = std::acos((p0q1 - p0r2 + q1r2) / (lenPQ * (lenQR + lenQR)));
    const float angleR = std::acos((p0r2 - p0q1 + q1r2) / (lenQR * (lenPR + lenPR)));

    // The original stores the three angles at +0x00, +0x04 and +0x08 of the output.
    out.X = angleP;  // +0x00
    out.Y = angleQ;  // +0x04
    out.Z = angleR;  // +0x08
}

// 0x5a20d4 _ZN6glitch13lessThanPlaneERKSt4pairIjNS_4core8aabbox3dIfEEEfc
bool lessThanPlane(const KdEntry& entry, float plane, char axis) {
    return axisComponent(kdCentre(entry), axis) < plane;
}

// 0x5a2160 _ZN6glitch13distanceKdTreeERKSt4pairIjNS_4core8aabbox3dIfEEEERKS5_
float distanceKdTree(const KdEntry& entry, const core::aabbox3d<float>& box) {
    const core::vector3d<float> other{(box.MinEdge.X + box.MaxEdge.X) * 0.5f,
                                      (box.MinEdge.Y + box.MaxEdge.Y) * 0.5f,
                                      (box.MinEdge.Z + box.MaxEdge.Z) * 0.5f};
    return squaredDistance(kdCentre(entry), other);
}

// 0x5a2268 _ZN6glitch13distanceKdTreeERKSt4pairIjNS_4core8aabbox3dIfEEEES7_
float distanceKdTree(const KdEntry& a, const KdEntry& b) {
    return squaredDistance(kdCentre(a), kdCentre(b));
}

// 0x5a2370 _ZN6glitch13distanceKdTreeERKSt4pairIjNS_4core8aabbox3dIfEEEEfc
float distanceKdTree(const KdEntry& entry, float plane, char axis) {
    const float d = axisComponent(kdCentre(entry), axis) - plane;
    return d * d;
}

// 0x59a188 _ZN6glitch5scene22createMeshUniquePrimitivesERKN5boost13intrusive_ptrINS0_5IMeshEEE
// Copy of the handle (the original adds a reference; the deps stand-in does not count).
boost::intrusive_ptr<scene::IMesh> createMeshUniquePrimitives(
    const boost::intrusive_ptr<scene::IMesh>& mesh) {
    return mesh;
}

// 0x59a1a4 _ZN6glitch5scene15createMeshWeldedERKN5boost13intrusive_ptrINS0_5IMeshEEEf
// Same body as createMeshUniquePrimitives (linker-folded); the float argument is unused.
boost::intrusive_ptr<scene::IMesh> createMeshWelded(const boost::intrusive_ptr<scene::IMesh>& mesh,
                                                    float /*tolerance*/) {
    return mesh;
}

// SAnimatedMesh (0x59a000 .. 0x59ae64) and createAnimatedMesh (0x59ae1c): canonical in src/scene/SAnimatedMesh.cpp.

// 0x59b084 _ZN6glitch5scene12flipSurfacesERKN5boost13intrusive_ptrINS0_5IMeshEEE
// Partial stub: 3.9 KB of pseudocode, not reconstructed in this pass.
boost::intrusive_ptr<scene::IMesh> flipSurfaces(const boost::intrusive_ptr<scene::IMesh>& mesh) {
    return mesh;
}

// 0x59b2a4 _ZN6glitch5scene14createMeshCopyERKN5boost13intrusive_ptrINS0_5IMeshEEEPN6glitch5video12IVideoDriverEj
// Partial stub: 12 KB of pseudocode, not reconstructed in this pass.
boost::intrusive_ptr<scene::IMesh> createMeshCopy(const boost::intrusive_ptr<scene::IMesh>& mesh,
                                                  video::IVideoDriver* /*driver*/,
                                                  std::uint32_t /*flags*/) {
    return mesh;
}

// 0x59ba3c _ZN6glitch5scene20createMeshWith2TCoordsERKN5boost13intrusive_ptrINS0_5IMeshEEEPN6glitch5video12IVideoDriverE
// Partial: forwards to createMeshCopy with flags 0xFFFFFFFF and 6, as the pseudocode does.
boost::intrusive_ptr<scene::IMesh> createMeshWith2TCoords(const boost::intrusive_ptr<scene::IMesh>& mesh,
                                                          video::IVideoDriver* driver) {
    return createMeshCopy(mesh, driver, 0xFFFFFFFFu);
}

// 0x59ba90 _ZN6glitch5scene15setVertexColorsERKN5boost13intrusive_ptrINS0_5IMeshEEENS_5video6SColorE
// Partial stub: 3.2 KB of pseudocode, not reconstructed in this pass.
void setVertexColors(const boost::intrusive_ptr<scene::IMesh>& /*mesh*/, video::SColor /*color*/) {}

// 0x59bc60 _ZN6glitch5scene18setVertexColorAlphaERKN5boost13intrusive_ptrINS0_5IMeshEEEi
// Partial stub: 7.5 KB of pseudocode, not reconstructed in this pass.
void setVertexColorAlpha(const boost::intrusive_ptr<scene::IMesh>& /*mesh*/, int /*alpha*/) {}

// 0x59c198 _ZN6glitch5scene24makePlanarTextureMappingERKN5boost13intrusive_ptrINS0_5IMeshEEEf
// Partial stub: 9.5 KB of pseudocode, not reconstructed in this pass.
void makePlanarTextureMapping(const boost::intrusive_ptr<scene::IMesh>& /*mesh*/, float /*scale*/) {}

// 0x59c970 _ZN6glitch5scene22createMeshWithTangentsERKN5boost13intrusive_ptrINS0_5IMeshEEEPN6glitch5video12IVideoDriverE
// Partial stub: 25 KB of pseudocode, not reconstructed in this pass.
boost::intrusive_ptr<scene::IMesh> createMeshWithTangents(const boost::intrusive_ptr<scene::IMesh>& mesh,
                                                          video::IVideoDriver* /*driver*/) {
    return mesh;
}

// 0x59dc1c _ZN6glitch5scene5scaleERKN5boost13intrusive_ptrINS0_11CMeshBufferEEERKNS_4core8vector3dIfEE
// Partial stub: 3.9 KB of pseudocode, not reconstructed in this pass.
void scale(const boost::intrusive_ptr<scene::CMeshBuffer>& /*buffer*/,
           const core::vector3d<float>& /*s*/) {}

// 0x59dea4 _ZN6glitch5scene5scaleERKN5boost13intrusive_ptrINS0_5IMeshEEERKNS_4core8vector3dIfEE
// Partial stub: 1.7 KB of pseudocode, not reconstructed in this pass.
void scale(const boost::intrusive_ptr<scene::IMesh>& /*mesh*/, const core::vector3d<float>& /*s*/) {}

// 0x59dfe4 _ZN6glitch5scene18recalculateNormalsERKN5boost13intrusive_ptrINS0_11CMeshBufferEEEbb
// Partial stub: 11.7 KB of pseudocode, not reconstructed in this pass.
void recalculateNormals(const boost::intrusive_ptr<scene::CMeshBuffer>& /*buffer*/, bool /*a*/,
                        bool /*b*/) {}

// 0x59e698 _ZN6glitch5scene18recalculateNormalsERKN5boost13intrusive_ptrINS0_5IMeshEEEbb
// Partial stub: 1.4 KB of pseudocode, not reconstructed in this pass.
void recalculateNormals(const boost::intrusive_ptr<scene::IMesh>& /*mesh*/, bool /*a*/, bool /*b*/) {}

// 0x59e728 _ZN6glitch5scene12_GLOBAL__N_19transformERKN5boost13intrusive_ptrINS0_11CMeshBufferEEERKNS_4core7CMatrix4IfEEPNS_...
// Partial stub: 7.2 KB of pseudocode (anonymous-namespace transform), not reconstructed in this pass.
void transform(const boost::intrusive_ptr<scene::CMeshBuffer>& /*buffer*/,
               const core::CMatrix4<float>& /*matrix*/) {}

// 0x59ee64 _ZN6glitch5scene9transformERKN5boost13intrusive_ptrINS0_5IMeshEEERKNS_4core7CMatrix4IfEE
// Partial stub: 2.4 KB of pseudocode, not reconstructed in this pass.
void transform(const boost::intrusive_ptr<scene::IMesh>& /*mesh*/, const core::CMatrix4<float>& /*matrix*/) {}

// 0x59a620 _ZN6glitch5scene12_GLOBAL__N_116calculateTangentsERNS_4core8vector3dIfEES5_RKS4_S7_S7_
// Partial stub: 3.6 KB of pseudocode, not reconstructed in this pass.
void calculateTangents(core::vector3d<float>& /*tangent*/, core::vector3d<float>& /*bitangent*/,
                       const core::vector3d<float>& /*p0*/, const core::vector3d<float>& /*p1*/,
                       const core::vector3d<float>& /*p2*/) {}

// 0x60b2b4 _ZN6glitch7collada14COnDemandReaderD2Ev
// Empty non-deleting destructor: nothing to tear down in the stand-in.
void COnDemandReader_D1(void* /*self*/) {}

// 0x60b2b8 _ZN6glitch7collada14COnDemandReader4readEiiPv
// Partial: the reader vtable calls (+0x18 seek, +0x0c read) are not modelled.
std::int32_t COnDemandReader_read(void* /*self*/, std::int32_t /*size*/, std::int32_t /*offset*/,
                                  void* /*dst*/) {
    return 0;
}

// 0x60b304 _ZN6glitch7collada14COnDemandReader4readEiiPvPFviiS2_S2_ES2_
// Partial: the callback variant reads the same way; the callback is not invoked.
std::int32_t COnDemandReader_read_callback(void* /*self*/, std::int32_t /*size*/,
                                           std::int32_t /*offset*/, void* /*dst*/,
                                           void (* /*callback*/)(int, int, void*, void*),
                                           void* /*user*/) {
    return 0;
}

// 0x60b350 _ZNK6glitch7collada14CAnimationBlock8getBlockERNS0_25SAnimationBlockSearchKeyE
// Partial: the tree walk over the block registry is not reconstructed in this pass.
CAnimationBlock* CAnimationBlock_getBlock(CAnimationBlock* /*root*/,
                                          SAnimationBlockSearchKey& /*key*/) {
    return nullptr;
}

// 0x60b918 _ZN6glitch7collada26CAnimationStreamingManager7releaseEPNS0_16CColladaDatabaseE
// Partial: the cache-entry erase and size bookkeeping are not reconstructed.
void CAnimationStreamingManager_release(CAnimationStreamingManager& /*self*/,
                                        CColladaDatabase* /*db*/) {}

// 0x60b990 _ZN6glitch7collada26CAnimationStreamingManager16checkMemoryUsageEv
// Partial: the trim loop is not reconstructed; the original logs "AnimationStreamingManager :
// Memory usage exceed maximum cache size" when no entry can be evicted.
void CAnimationStreamingManager_checkMemoryUsage(CAnimationStreamingManager& /*self*/) {}

// 0x60ba2c / 0x60bbd8 _ZN6glitch7collada14CAnimationBlockD1Ev
// Partial: the previous-block release and the database member destructor are not reconstructed.
void CAnimationBlock_D1(CAnimationBlock* /*self*/) {}

// 0x60bad0 _ZN6glitch7collada14CAnimationBlock4freeEv
// Partial: unregister then destroy; the operator delete is not modelled.
void CAnimationBlock_free(CAnimationBlock* /*self*/) {}

// 0x60bb1c _ZN6glitch7collada14CAnimationBlock4dropEv
// Partial: frees when the count falls to one and the cached links allow it; free is stubbed.
void CAnimationBlock_drop(CAnimationBlock* self) {
    self->refCount -= 1;
    if (self->refCount == 1) {
        CAnimationBlock_free(self);
    }
}

// 0x60bb64 _ZN6glitch7collada26CAnimationStreamingManager23unregisterAnimationBlockEPNS0_14CAnimationBlockE
// Partial: the lower-bound search and erase are not reconstructed.
void CAnimationStreamingManager_unregisterAnimationBlock(CAnimationStreamingManager& /*self*/,
                                                         CAnimationBlock* /*block*/) {}

// 0x60bc7c _ZN6glitch7collada26CAnimationStreamingManager5cacheEPNS0_14CAnimationBlockE
// Partial stub: 3.9 KB of pseudocode, not reconstructed in this pass.
void CAnimationStreamingManager_cache(CAnimationStreamingManager& /*self*/,
                                      CAnimationBlock* /*block*/) {}

// 0x60c05c / 0x60c610 / 0x60c7a8 / 0x60c940 _ZN6glitch7collada14CAnimationBlockC2E...
// Partial: constructors load the segment from the database; the loader is not reconstructed.
void CAnimationBlock_ctor(CAnimationBlock* /*self*/, const CColladaDatabase& /*db*/,
                          SAnimationBlockSearchKey& /*key*/, std::int32_t /*begin*/,
                          std::int32_t /*end*/) {}

// 0x60c220 _ZN6glitch7collada26CAnimationStreamingManager17getAnimationBlockERNS0_25SAnimationBlockSearchKeyE
// Partial: lookup then allocate-and-construct; allocation is not modelled.
CAnimationBlock* CAnimationStreamingManager_getAnimationBlock(CAnimationStreamingManager& /*self*/,
                                                              SAnimationBlockSearchKey& /*key*/) {
    return nullptr;
}

// 0x60c31c _ZN6glitch7collada14CAnimationBlock16prepareNextBlockEv
// Partial stub: 1.7 KB of pseudocode, not reconstructed in this pass.
void CAnimationBlock_prepareNextBlock(CAnimationBlock* /*self*/) {}

// 0x60c3f0 _ZN6glitch7collada14CAnimationBlock4grabEv
// Increments the count; prepares the next block at the first grab when none is linked.
void CAnimationBlock_grab(CAnimationBlock& self) {
    self.refCount += 1;
    if (self.refCount == 2 && self.next == nullptr) {
        CAnimationBlock_prepareNextBlock(&self);
    }
}

// 0x60c414 _ZN6glitch7collada26CAnimationStreamingManager20registerAnimationBlockEPNS0_14CAnimationBlockE
// Partial stub: 3.3 KB of pseudocode, not reconstructed in this pass.
void CAnimationStreamingManager_registerAnimationBlock(CAnimationStreamingManager& /*self*/,
                                                       CAnimationBlock* /*block*/) {}

// 0x60cb24 _ZNK6glitch7collada19CBillboardSceneNode24getTransformedBoundingBoxEv
// Partial: the parent matrix transform (vtable slot +0x38 on the parent) is not dispatched here.
const core::aabbox3d<float>& CBillboardSceneNode_getTransformedBoundingBox(CBillboardSceneNode& /*self*/) {
    static const core::aabbox3d<float> kEmpty{};
    return kEmpty;
}

// 0x60cbb0 _ZN6glitch7collada19CBillboardSceneNode18computeBoundingBoxEv
// Partial stub: bounding box over the billboard corners is not reconstructed in this pass.
void CBillboardSceneNode_computeBoundingBox(CBillboardSceneNode& /*self*/) {}

// 0x60ce44 _ZN6glitch7collada10CSceneNodeD2Ev
// Partial: the two base-class vtable resets and the ISceneNode destructor call are not modelled.
void CSceneNode_D1(CSceneNode* /*self*/) {}

// 0x60cec8 _ZN6glitch7collada19CBillboardSceneNodeD2Ev
// Partial: resets the billboard vtable and forwards to CSceneNode_D1.
void CBillboardSceneNode_D1(CBillboardSceneNode* self) {
    CSceneNode_D1(reinterpret_cast<CSceneNode*>(self));
}

// 0x60cf88 _ZN6glitch7collada19CBillboardSceneNode20updateAbsolutePositionEb
// Partial stub: 13.7 KB of pseudocode, not reconstructed in this pass.
void CBillboardSceneNode_updateAbsolutePosition(CBillboardSceneNode& /*self*/, bool /*recursive*/) {}

// 0x60dfc0 _ZN6glitch7collada15animation_track13CApplicatorINS1_10SLightColorEE10applyValueExEPKh
// Writes the three channels from the byte value (divided by 255) and sets alpha to 1.
float CApplicator_SLightColor_applyValueEx(SLightColorApplicator& self, const std::uint8_t* value) {
    const float b = static_cast<float>(value[2]) / 255.0f;
    self.r = static_cast<float>(value[0]) / 255.0f;  // +0x14
    self.g = static_cast<float>(value[1]) / 255.0f;  // +0x18
    self.b = b;                                      // +0x1c
    self.a = 1.0f;                                   // +0x20
    return b;
}

// 0x60eeb4 _ZNK6glitch7collada15animation_track10CVirtualEx...CLightColorMixin...retrieveValue
// Packs the three float channels (source +0x14..+0x1c) back into bytes.
std::uint32_t CVirtualEx_LightColor_retrieveValue(const float* source, std::uint8_t* out) {
    out[0] = static_cast<std::uint8_t>(static_cast<std::uint32_t>(source[5] * 255.0f));
    out[1] = static_cast<std::uint8_t>(static_cast<std::uint32_t>(source[6] * 255.0f));
    const auto last = static_cast<std::uint32_t>(source[7] * 255.0f);
    out[2] = static_cast<std::uint8_t>(last);
    return last;
}

// 0x60ef6c _ZNK...CSceneNodeScaleMixin...retrieveValue
// Partial: the scene-node scale is read through vtable slot +0x90; not dispatched here.
void CVirtualEx_SceneNodeScale_retrieveValue(const void* /*node*/, float* /*out*/) {}

// 0x60f000 _ZNK...CSceneNodePositionMixin...retrieveValue
// Partial: the scene-node position is read through vtable slot +0xa0; not dispatched here.
void CVirtualEx_SceneNodePosition_retrieveValue(const void* /*node*/, float* /*out*/) {}

// 0x60f10c _ZNK...CSceneNodeQuaternionMixin...retrieveValue
// Partial: the scene-node quaternion is read through vtable slot +0x98; not dispatched here.
void CVirtualEx_SceneNodeQuaternion_retrieveValue(const void* /*node*/, float* /*out*/) {}

// 0x60f148 _ZN...CVirtualEx<CApplyValueEx<float[2], CMixin<float,2,SMaterialSetParam<...>>>>::...
// Partial: the blend loop over the material-set parameters is not reconstructed in this pass.
void CVirtualEx_MaterialSetParam_applyValue(float* /*dst*/, const float* /*src*/, int /*a3*/,
                                            int /*count*/, float* /*out*/) {}

// 0x60f25c / 0x60f2b0 _ZN6glitch7collada16CColladaDatabaseC2EPKcPNS0_15CColladaFactoryE
// Partial: CResFileManager::load is not in this category; the factory store and ref count are modelled as comments.
void CColladaDatabase_ctor_name(CColladaDatabase* /*self*/, const char* /*name*/, void* /*factory*/) {}

// 0x60f304 / 0x60f364 _ZN6glitch7collada16CColladaDatabaseC2EPN6glitch2io9IReadFileEbPNS0_15CColladaFactoryE
// Partial: CResFileManager::load (with the bool flag) is not in this category.
void CColladaDatabase_ctor_file_flag(CColladaDatabase* /*self*/, void* /*file*/, bool /*flag*/,
                                     void* /*factory*/) {}

// 0x60f3c4 / 0x60f42c _ZN6glitch7collada16CColladaDatabaseC2EPN6glitch2io9IReadFileEPNS0_15CColladaFactoryE
// Partial: CResFileManager::load (without the flag) is not in this category.
void CColladaDatabase_ctor_file(CColladaDatabase* /*self*/, void* /*file*/, void* /*factory*/) {}

}  // namespace glitch::collada::recon_07
