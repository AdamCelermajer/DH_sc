// Reconstruction slice 07: mesh helpers (glitch::scene), CAppendMeshBuffer,
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

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/deps/video_stand_ins.hpp"

namespace glitch::core {
template <typename T>
struct vector2d;
template <typename T>
class CMatrix4;
}  // namespace glitch::core

namespace glitch::scene {
class IMesh;
class IAnimatedMesh;
class CMeshBuffer;
class SAnimatedMesh;
}  // namespace glitch::scene

namespace glitch::collada::recon_07 {

// glitch::video::E_INDEX_TYPE as the pseudocode switches on it.
using IndexType = std::uint32_t;
inline constexpr IndexType kIndex8 = 0;   // unsigned char indices
inline constexpr IndexType kIndex16 = 1;  // unsigned short indices
inline constexpr IndexType kIndex32 = 2;  // unsigned int indices

// Stand-in for glitch::scene::CAppendMeshBuffer, fields only.
struct CAppendMeshBufferLayout {
    std::uint8_t pad_0000[0x20] = {};  // +0x00..+0x1f (not mapped)
    std::uint32_t setupIndexCount = 0;      // +0x20 (set by setup from index used/stride)
    std::uint32_t setupCounterA = 0;        // +0x24 (cleared by setup)
    std::uint32_t setupVertexCount = 0;     // +0x28 (set by setup from vertex used/stride)
    std::uint8_t pad_002c[0x0c] = {};       // +0x2c..+0x37 (+0x2c holds a 16-bit flag set to 1)
    std::uint32_t vertexCapacityBytes = 0;  // +0x38
    std::uint32_t vertexUsedBytes = 0;      // +0x3c
    std::uint32_t indexCapacityBytes = 0;   // +0x40
    std::uint32_t indexUsedBytes = 0;       // +0x44
    std::uint32_t vertexStride = 0;         // +0x48
    std::uint32_t indexStride = 0;          // +0x4c
};

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

// --- CAppendMeshBuffer ----------------------------------------------------
// 0x5a2040 Reconstructed.
bool CAppendMeshBuffer_hasEnoughSpace(const CAppendMeshBufferLayout& self,
                                      std::uint32_t vertexCount, std::uint32_t indexCount);
// 0x5a26bc Partial: IBuffer::map is not in this category; the mapped pointers are not stored.
void CAppendMeshBuffer_beginAppend(CAppendMeshBufferLayout& self);
// 0x5a8a88 Partial: the write through the pointer at +0x14 is not modelled.
void CAppendMeshBuffer_setup(CAppendMeshBufferLayout& self);

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
// Stand-in member view: aabbox at +0x08, child list begin/end at +0x20/+0x24,
// mesh type at +0x2c. Child calls are vtable dispatches not modelled here.
struct SAnimatedMeshLayout {
    std::uint8_t pad_0000[0x08] = {};  // +0x00..+0x07 (vtable and reference count)
    core::aabbox3d<float> boundingBox;  // +0x08..+0x1f
    std::uint32_t childBegin = 0;       // +0x20 (begin pointer of the child list)
    std::uint32_t childEnd = 0;         // +0x24
    std::uint32_t meshType = 0;         // +0x2c
};

// 0x59a000 Partial: vtable slot +0x18 on the first child; index argument unused by the pseudocode.
boost::intrusive_ptr<video::CMaterial> SAnimatedMesh_getMaterial(const SAnimatedMeshLayout& self,
                                                                 std::uint32_t index);
// 0x59a03c Partial: vtable slot +0x1c on the first child; index argument unused by the pseudocode.
boost::intrusive_ptr<video::CMaterial> SAnimatedMesh_getMaterialVertexAttributeMap(
    const SAnimatedMeshLayout& self, std::uint32_t index);
// 0x59a078 Partial: tail forward of setMaterial to the first child (vtable slot +0x20).
void SAnimatedMesh_setMaterial(SAnimatedMeshLayout& self);
// 0x59a1c0 Partial: child bounding boxes via vtable slot +0x24 are not dispatched here.
void SAnimatedMesh_recalculateBoundingBox(SAnimatedMeshLayout& self);
// 0x59ad10 Partial: the child-list copy is modelled; recalculateBoundingBox is stubbed.
void SAnimatedMesh_ctor(SAnimatedMeshLayout& self, const boost::intrusive_ptr<scene::IMesh>& mesh,
                        std::uint32_t meshType);
// 0x59ae64 Partial: vtable clone of the first child not dispatched here.
boost::intrusive_ptr<scene::SAnimatedMesh> SAnimatedMesh_clone(const SAnimatedMeshLayout& self);

// 0x59ae1c Partial: allocation and constructor call; SAnimatedMesh_ctor is partial.
void createAnimatedMesh(boost::intrusive_ptr<scene::SAnimatedMesh>& out,
                        const boost::intrusive_ptr<scene::IMesh>& mesh, std::uint32_t meshType);

// 0x59af20 / 0x59afb8 Partial: primitive counts need CPrimitiveStream and vtable slots 0x10..0x34.
std::int32_t getPolyCount(const boost::intrusive_ptr<scene::IMesh>& mesh);
std::int32_t getPolyCount(const boost::intrusive_ptr<scene::IAnimatedMesh>& mesh);

// Mesh transforms, colours, tangents and normals: partial stubs (see ledger).
boost::intrusive_ptr<scene::IMesh> flipSurfaces(const boost::intrusive_ptr<scene::IMesh>& mesh);
boost::intrusive_ptr<scene::IMesh> createMeshCopy(const boost::intrusive_ptr<scene::IMesh>& mesh,
                                                  video::IVideoDriver* driver, std::uint32_t flags);
boost::intrusive_ptr<scene::IMesh> createMeshWith2TCoords(
    const boost::intrusive_ptr<scene::IMesh>& mesh, video::IVideoDriver* driver);
void setVertexColors(const boost::intrusive_ptr<scene::IMesh>& mesh, video::SColor color);
void setVertexColorAlpha(const boost::intrusive_ptr<scene::IMesh>& mesh, int alpha);
void makePlanarTextureMapping(const boost::intrusive_ptr<scene::IMesh>& mesh, float scale);
void scaleTCoords(const boost::intrusive_ptr<scene::CMeshBuffer>& buffer,
                  core::vector2d<float> scale);
void scaleTCoords(const boost::intrusive_ptr<scene::IMesh>& mesh, core::vector2d<float> scale);
boost::intrusive_ptr<scene::IMesh> createMeshWithTangents(
    const boost::intrusive_ptr<scene::IMesh>& mesh, video::IVideoDriver* driver);
void scale(const boost::intrusive_ptr<scene::CMeshBuffer>& buffer, const core::vector3d<float>& s);
void scale(const boost::intrusive_ptr<scene::IMesh>& mesh, const core::vector3d<float>& s);
void recalculateNormals(const boost::intrusive_ptr<scene::CMeshBuffer>& buffer, bool a, bool b);
void recalculateNormals(const boost::intrusive_ptr<scene::IMesh>& mesh, bool a, bool b);
void transform(const boost::intrusive_ptr<scene::CMeshBuffer>& buffer,
               const core::CMatrix4<float>& matrix);
void transform(const boost::intrusive_ptr<scene::IMesh>& mesh, const core::CMatrix4<float>& matrix);

}  // namespace glitch::collada::recon_07
