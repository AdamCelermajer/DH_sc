#include "glitch-scene-collada-gui/recon/slice_07.hpp"

#include "glitch-video/material.hpp"

#include <cmath>
#include <cstring>

namespace glitch::collada::recon_07 {

namespace {

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

// SAnimatedMesh (0x59a000 .. 0x59ae64) and createAnimatedMesh (0x59ae1c): canonical in src/scene/SAnimatedMesh.cpp.

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
