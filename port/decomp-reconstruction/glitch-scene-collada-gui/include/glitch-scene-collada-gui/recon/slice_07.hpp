// Reconstruction slice 07: mesh helpers (glitch::scene),
// kd-tree helpers, the animation streaming block and cache, billboard and scene
// node helpers, and the animation track value accessors in glitch::collada.
//
// Free functions live in glitch::collada::recon_07 so they do not collide with
// the owning classes, which are not reconstructed here. Bodies that depend on
// vtable dispatch, the mesh buffer and the streaming cache are partial stubs
// (see ledger notes). Stand-in layouts below only name the fields these bodies
// touch; offsets are the original 32-bit offsets.
#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

#include "glitch-scene-collada-gui/collada/CColladaDatabase.hpp"
#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/deps/video_stand_ins.hpp"
#include "glitch-scene-collada-gui/scene/CMeshBuffer.hpp"
#include "glitch-scene-collada-gui/scene/IMesh.hpp"
#include "glitch-scene-collada-gui/scene/SAnimatedMesh.hpp"

namespace glitch::core {
template <typename T>
struct vector2d;
template <typename T>
class CMatrix4;
}  // namespace glitch::core

namespace glitch::collada::recon_07 {

// glitch::video::E_INDEX_TYPE as the pseudocode switches on it.
using IndexType = std::uint32_t;
inline constexpr IndexType kIndex8 = 0;   // unsigned char indices
inline constexpr IndexType kIndex16 = 1;  // unsigned short indices
inline constexpr IndexType kIndex32 = 2;  // unsigned int indices

// Stand-in for glitch::collada::animation_track::SLightColor (the applicator
// object of CApplicator<SLightColor>). Channels are float at +0x14..+0x20.
struct SLightColorApplicator {
    std::uint8_t pad_0000[0x14] = {};  // +0x00..+0x13 (not mapped)
    float r = 0.0f;                    // +0x14
    float g = 0.0f;                    // +0x18
    float b = 0.0f;                    // +0x1c
    float a = 0.0f;                    // +0x20
};

// Result of the kd-tree bounding box pair used by lessThanPlane and distanceKdTree:
// std::pair<unsigned int, aabbox3d<float>> with the box at +0x04.
using KdEntry = std::pair<std::uint32_t, core::aabbox3d<float>>;

// --- glitch::scene::(anonymous)::getTriangleIndices / swapIndices ----------
// 0x59a0a4 Partial: the original's return value (pointer on 16-bit, index+1 on
// 32-bit, null on 8-bit) is not modelled; the three outputs are.
void getTriangleIndices(IndexType type, const void* indices, std::uint32_t triangle,
                        std::uint32_t& i0, std::uint32_t& i1, std::uint32_t& i2);
// 0x59a130 Reconstructed; returns the element value that was at index a3 before the swap.
std::uint32_t swapIndices(IndexType type, void* indices, std::uint32_t a, std::uint32_t b);

// 0x59aa28 Reconstructed; angles at p0, p1, p2 (law of cosines).
void getAngleWeight(core::vector3d<float>& out, const core::vector3d<float>& p0,
                    const core::vector3d<float>& p1, const core::vector3d<float>& p2);

// --- kd-tree helpers ------------------------------------------------------
// 0x5a20d4 Reconstructed.
bool lessThanPlane(const KdEntry& entry, float plane, char axis);
// 0x5a2160 Reconstructed.
float distanceKdTree(const KdEntry& entry, const core::aabbox3d<float>& box);
// 0x5a2268 Reconstructed.
float distanceKdTree(const KdEntry& a, const KdEntry& b);
// 0x5a2370 Reconstructed.
float distanceKdTree(const KdEntry& entry, float plane, char axis);

// --- glitch::scene mesh helpers (boost::intrusive_ptr handles) -------------
// 0x59a188 / 0x59a1a4: identical bodies (linker-folded); copy of the handle.
boost::intrusive_ptr<scene::IMesh> createMeshUniquePrimitives(
    const boost::intrusive_ptr<scene::IMesh>& mesh);
boost::intrusive_ptr<scene::IMesh> createMeshWelded(const boost::intrusive_ptr<scene::IMesh>& mesh,
                                                    float tolerance);

// --- SAnimatedMesh ---------------------------------------------------------
// The stand-in member view and the seven SAnimatedMesh_* / createAnimatedMesh free functions moved to the canonical class
// glitch::scene::SAnimatedMesh (scene/SAnimatedMesh.hpp, src/scene/SAnimatedMesh.cpp) in the unification wave.

// 0x59af20 / 0x59afb8 getPolyCount: reconstructed in scene/mesh_functions.hpp (src/r3a-...-glitch_scene-00081f-0.cpp).

// Mesh transforms, colours, tangents and normals: partial stubs (see ledger).
boost::intrusive_ptr<scene::IMesh> flipSurfaces(const boost::intrusive_ptr<scene::IMesh>& mesh);
boost::intrusive_ptr<scene::IMesh> createMeshCopy(const boost::intrusive_ptr<scene::IMesh>& mesh,
                                                  video::IVideoDriver* driver, std::uint32_t flags);
boost::intrusive_ptr<scene::IMesh> createMeshWith2TCoords(
    const boost::intrusive_ptr<scene::IMesh>& mesh, video::IVideoDriver* driver);
void setVertexColors(const boost::intrusive_ptr<scene::IMesh>& mesh, video::SColor color);
void setVertexColorAlpha(const boost::intrusive_ptr<scene::IMesh>& mesh, int alpha);
void makePlanarTextureMapping(const boost::intrusive_ptr<scene::IMesh>& mesh, float scale);
boost::intrusive_ptr<scene::IMesh> createMeshWithTangents(
    const boost::intrusive_ptr<scene::IMesh>& mesh, video::IVideoDriver* driver);
void scale(const boost::intrusive_ptr<scene::CMeshBuffer>& buffer, const core::vector3d<float>& s);
void scale(const boost::intrusive_ptr<scene::IMesh>& mesh, const core::vector3d<float>& s);
void recalculateNormals(const boost::intrusive_ptr<scene::CMeshBuffer>& buffer, bool a, bool b);
void recalculateNormals(const boost::intrusive_ptr<scene::IMesh>& mesh, bool a, bool b);
void transform(const boost::intrusive_ptr<scene::CMeshBuffer>& buffer,
               const core::CMatrix4<float>& matrix);
void transform(const boost::intrusive_ptr<scene::IMesh>& mesh, const core::CMatrix4<float>& matrix);

// 0x59a620 Partial: tangent accumulation (calculateTangents) is not reconstructed in this pass.
void calculateTangents(core::vector3d<float>& tangent, core::vector3d<float>& bitangent,
                       const core::vector3d<float>& p0, const core::vector3d<float>& p1,
                       const core::vector3d<float>& p2);

// --- glitch::collada animation streaming -----------------------------------
// Opaque here: the owning classes are not reconstructed in this category.
struct CColladaDatabase;
struct CAnimationStreamingManager;
struct SAnimationBlockSearchKey;

// glitch::collada::CAnimationBlock: the canonical class (collada/CColladaDatabase.hpp). The count is at +0x00 and
// the next block at +0x18.
using glitch::collada::CAnimationBlock;

// 0x60b2b4 Reconstructed: empty non-deleting destructor of COnDemandReader.
void COnDemandReader_D1(void* self);
// 0x60b2b8 Partial: reader vtable slots +0x18 and +0x0c are not modelled.
std::int32_t COnDemandReader_read(void* self, std::int32_t size, std::int32_t offset, void* dst);
// 0x60b304 Partial: same as the three-argument read, the callback is not invoked.
std::int32_t COnDemandReader_read_callback(void* self, std::int32_t size, std::int32_t offset,
                                           void* dst, void (*callback)(int, int, void*, void*),
                                           void* user);
// 0x60b350 Partial: the search tree walk is not reconstructed in this pass.
CAnimationBlock* CAnimationBlock_getBlock(CAnimationBlock* root, SAnimationBlockSearchKey& key);
// 0x60b918 Partial: the cache-entry erase and size bookkeeping are not reconstructed.
void CAnimationStreamingManager_release(CAnimationStreamingManager& self, CColladaDatabase* db);
// 0x60b990 Partial: the cache trim loop is not reconstructed; the log text is kept as a comment.
void CAnimationStreamingManager_checkMemoryUsage(CAnimationStreamingManager& self);
// 0x60ba2c / 0x60bbd8 Partial: destructor releases the cached previous block and the database.
void CAnimationBlock_D1(CAnimationBlock* self);
// 0x60bad0 Partial: unregister then delete; the operator delete is not modelled.
void CAnimationBlock_free(CAnimationBlock* self);
// 0x60bb1c Partial: drops the count and frees when it reaches one.
void CAnimationBlock_drop(CAnimationBlock* self);
// 0x60bb64 Partial: binary search and erase of the registry entry are not reconstructed.
void CAnimationStreamingManager_unregisterAnimationBlock(CAnimationStreamingManager& self,
                                                         CAnimationBlock* block);
// 0x60bc7c Partial: cache insertion and eviction are not reconstructed.
void CAnimationStreamingManager_cache(CAnimationStreamingManager& self, CAnimationBlock* block);
// 0x60c05c / 0x60c610 / 0x60c7a8 / 0x60c940 Partial: constructors load the segment from the database.
void CAnimationBlock_ctor(CAnimationBlock* self, const CColladaDatabase& db,
                          SAnimationBlockSearchKey& key, std::int32_t begin, std::int32_t end);
// 0x60c220 Partial: lookup then allocate-and-construct; allocation is not modelled.
CAnimationBlock* CAnimationStreamingManager_getAnimationBlock(CAnimationStreamingManager& self,
                                                              SAnimationBlockSearchKey& key);
// 0x60c31c Partial: next-block preparation is not reconstructed.
void CAnimationBlock_prepareNextBlock(CAnimationBlock* self);
// 0x60c3f0 Reconstructed: increments the count and prepares the next block at the first grab.
void CAnimationBlock_grab(CAnimationBlock& self);
// 0x60c414 Partial: sorted insert into the registry is not reconstructed.
void CAnimationStreamingManager_registerAnimationBlock(CAnimationStreamingManager& self,
                                                       CAnimationBlock* block);

// --- glitch::collada scene nodes -------------------------------------------
struct CBillboardSceneNode;
struct CSceneNode;
// 0x60cb24 Partial: transformed box cache and parent matrix are not dispatched here.
const core::aabbox3d<float>& CBillboardSceneNode_getTransformedBoundingBox(CBillboardSceneNode& self);
// 0x60cbb0 Partial: bounding box computation over the billboard corners is not reconstructed.
void CBillboardSceneNode_computeBoundingBox(CBillboardSceneNode& self);
// 0x60ce44 Partial: base-class destructor chain (multiple inheritance) is not modelled.
void CSceneNode_D1(CSceneNode* self);
// 0x60cec8 Partial: forwards to CSceneNode_D1 after resetting the billboard vtable.
void CBillboardSceneNode_D1(CBillboardSceneNode* self);
// 0x60cf88 Partial: absolute-position update is not reconstructed (13 KB pseudocode).
void CBillboardSceneNode_updateAbsolutePosition(CBillboardSceneNode& self, bool recursive);

// --- animation track value accessors ---------------------------------------
// 0x60dfc0 Reconstructed: writes the three channels scaled from bytes and sets alpha to 1.
float CApplicator_SLightColor_applyValueEx(SLightColorApplicator& self, const std::uint8_t* value);
// 0x60eeb4 Reconstructed: packs the three float channels at a2[5..7] back into bytes.
std::uint32_t CVirtualEx_LightColor_retrieveValue(const float* source, std::uint8_t* out);
// 0x60ef6c Partial: scene-node scale is read through vtable slot +0x90, not dispatched here.
void CVirtualEx_SceneNodeScale_retrieveValue(const void* node, float* out);
// 0x60f000 Partial: scene-node position is read through vtable slot +0xa0, not dispatched here.
void CVirtualEx_SceneNodePosition_retrieveValue(const void* node, float* out);
// 0x60f10c Partial: scene-node quaternion is read through vtable slot +0x98, not dispatched here.
void CVirtualEx_SceneNodeQuaternion_retrieveValue(const void* node, float* out);
// 0x60f148 Partial: the material-set parameter loop is not reconstructed.
void CVirtualEx_MaterialSetParam_applyValue(float* dst, const float* src, int a3, int count,
                                            float* out);

// --- glitch::collada::CColladaDatabase constructors (partial) --------------
// 0x60f25c / 0x60f2b0 Partial: CResFileManager::load is not in this category.
void CColladaDatabase_ctor_name(CColladaDatabase* self, const char* name, void* factory);
// 0x60f304 / 0x60f364 Partial: same as the name form, from an IReadFile with the bool flag.
void CColladaDatabase_ctor_file_flag(CColladaDatabase* self, void* file, bool flag, void* factory);
// 0x60f3c4 / 0x60f42c Partial: same as the name form, from an IReadFile without the flag.
void CColladaDatabase_ctor_file(CColladaDatabase* self, void* file, void* factory);

}  // namespace glitch::collada::recon_07
