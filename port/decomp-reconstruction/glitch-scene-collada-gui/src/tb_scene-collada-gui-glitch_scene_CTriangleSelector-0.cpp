// Track B batch e1-glitch-scene-collada-gui-glitch_scene_CTriangleSelector-0.
// Bodies for glitch::scene::CTriangleSelector. The class is declared in tb/glitch_scene_CTriangleSelector.hpp.
//
// Open item shared by four bodies: every node-matrix read in the original is a virtual call on the
// ISceneNode at vtable +0x38 (returns a const CMatrix4<float>*). The slot name is not mapped (the
// decomp_vtable listing for ISceneNode starts with a raw word, so the offset does not resolve). The
// helper below returns null, and each body that needs the node matrix skips that step and is ledgered partial.
#include "glitch-scene-collada-gui/tb/glitch_scene_CTriangleSelector.hpp"

#include <cstddef>

namespace glitch::scene {

namespace {

// ISceneNode vtable +0x38: node absolute matrix (unresolved). Stays null until the slot is mapped.
const core::CMatrix4<float>* nodeAbsoluteTransform(const ISceneNode* /*node*/) {
    return nullptr;
}

// Row-vector transform used by the original: out = p * M, with the translation in M[12..14].
// Each component is ((x*M[a] + y*M[b]) + z*M[c]) + M[d], in the order the pseudocode computes it.
core::vector3d<float> transformPoint(const core::CMatrix4<float>& m, const core::vector3d<float>& p) {
    core::vector3d<float> r;
    r.X = p.X * m.M[0] + p.Y * m.M[4] + p.Z * m.M[8] + m.M[12];
    r.Y = p.X * m.M[1] + p.Y * m.M[5] + p.Z * m.M[9] + m.M[13];
    r.Z = p.X * m.M[2] + p.Y * m.M[6] + p.Z * m.M[10] + m.M[14];
    return r;
}

}  // namespace

// 0x596598 _ZN6glitch5scene17CTriangleSelectorC2ERKN5boost13intrusive_ptrIKNS0_5IMeshEEEPKNS0_10ISceneNodeEb
// PARTIAL. Done: the member initialisation (the words the original writes), the identity transform and the
// default box, plus the bake step (the transform loop, applied when the node matrix is available).
// Missing: the mesh-buffer traversal. The original counts the indices of every mesh buffer (through the
// IMesh vtable slots +0x10 and +0x14, names by usage), reserves total/3 triangles, and calls
// createTriangles<T> for the seven index types (anonymous namespace, not in this batch). The vertex
// streams and index map come from CMeshBuffer and CPrimitiveStream layouts that are not mapped here.
// Missing: the vtable pointer store at +0x00.
CTriangleSelector::CTriangleSelector(const boost::intrusive_ptr<const IMesh>& mesh, const ISceneNode* sceneNode,
                                     bool bake)
    : referenceCount(1), node(sceneNode), bakeNodeTransform(bake) {
    (void)mesh;  // mesh-buffer traversal not reconstructed (see above)

    boundingBox.MinEdge.X = -1.0f;  // +0x44
    boundingBox.MinEdge.Y = -1.0f;  // +0x48
    boundingBox.MinEdge.Z = -1.0f;  // +0x4c
    boundingBox.MaxEdge.X = 1.0f;   // +0x50
    boundingBox.MaxEdge.Y = 1.0f;   // +0x54
    boundingBox.MaxEdge.Z = 1.0f;   // +0x58

    transform = core::CMatrix4<float>(core::CMatrix4<float>::EM4CONST_IDENTITY);  // +0x5c..+0x9c

    if (node && bakeNodeTransform) {
        // Bake step: each vertex is taken through the node matrix. The matrix comes from the unresolved slot.
        if (const core::CMatrix4<float>* nodeMatrix = nodeAbsoluteTransform(node)) {
            const core::CMatrix4<float> local = *nodeMatrix;
            for (core::triangle3d<float>& t : triangles) {
                t.A = transformPoint(local, t.A);
                t.B = transformPoint(local, t.B);
                t.C = transformPoint(local, t.C);
            }
        }
    }
}

// 0x590f20 _ZNK6glitch5scene17CTriangleSelector16getTriangleCountEv
// Reconstructed: (end - begin) bytes / 36, which is the element count of the triangle vector.
int CTriangleSelector::getTriangleCount() const {
    return static_cast<int>(triangles.size());
}

// 0x591464 _ZNK6glitch5scene17CTriangleSelector12getTrianglesEPNS_4core10triangle3dIfEEiRiPKNS2_8CMatrix4IfEE
// PARTIAL. Done: the clamp of maxCount to the triangle count, the identity start with the flag set, the copy
// of the source matrix, the copy branch (identity flag) and the transform branch, and outCount.
// Missing: the node step (multEq of the matrix by the node matrix, guarded by !bakeNodeTransform) is skipped
// because the node matrix is unresolved, so the identity flag tested below can differ from the original.
// The Hex-Rays float return is the register leftover and is not reproduced (void here).
void CTriangleSelector::getTriangles(core::triangle3d<float>* out, int maxCount, int& outCount,
                                     const core::CMatrix4<float>* source) const {
    const int total = getTriangleCount();
    if (maxCount >= total) maxCount = total;

    core::CMatrix4<float> m(core::CMatrix4<float>::EM4CONST_IDENTITY);
    if (source) m = *source;

    if (node && !bakeNodeTransform) {
        // PARTIAL: the original multiplies m by the node matrix here (detail::CMatrix4Base<float>::multEq).
        (void)nodeAbsoluteTransform(node);
    }

    if (m.flag) {
        for (int i = 0; i < maxCount; ++i) out[i] = triangles[static_cast<std::size_t>(i)];
    } else {
        for (int i = 0; i < maxCount; ++i) {
            const core::triangle3d<float>& src = triangles[static_cast<std::size_t>(i)];
            out[i].A = transformPoint(m, src.A);
            out[i].B = transformPoint(m, src.B);
            out[i].C = transformPoint(m, src.C);
        }
    }
    outCount = maxCount;
}

// 0x586a34 _ZNK6glitch5scene17CTriangleSelector5SetupEPKNS_4core8CMatrix4IfEE
// PARTIAL. Done: the identity start (flag cleared, zeroed matrix, diagonal 1, flag set) and the copy of the
// source matrix (its 65 bytes: 16 floats and the flag).
// Missing: the multEq of the stored matrix by the node matrix (guarded by node && !bakeNodeTransform); the node
// matrix is unresolved.
void CTriangleSelector::Setup(const core::CMatrix4<float>* source) const {
    transform.flag = false;
    for (float& v : transform.M) v = 0.0f;
    transform.M[15] = 1.0f;
    transform.flag = true;
    transform.M[0] = 1.0f;
    transform.M[5] = 1.0f;
    transform.M[10] = 1.0f;
    if (source) transform = *source;

    if (node && !bakeNodeTransform) {
        // PARTIAL: the original applies the node matrix to transform here (multEq). Not written.
        (void)nodeAbsoluteTransform(node);
    }
}

// 0x587604 _ZNK6glitch5scene17CTriangleSelector5SetupERKNS_4core8aabbox3dIfEE
// PARTIAL. Done: the box copy (min at +0x44, max at +0x50).
// Missing: the node step (copy of the node matrix, makeInverse, transformBox on the box), skipped because the
// node matrix is unresolved.
void CTriangleSelector::Setup(const core::aabbox3d<float>& box) const {
    boundingBox = box;

    if (node && !bakeNodeTransform) {
        // PARTIAL: the original takes the node matrix, inverts a copy and transforms boundingBox with it. Not written.
        (void)nodeAbsoluteTransform(node);
    }
}

// 0x586acc _ZNK6glitch5scene17CTriangleSelector9AddResultERKNS_4core10triangle3dIfEE
// Reconstructed. Appends the triangle to the result buffer, transforms the appended copy by the stored matrix
// when the identity flag is clear, bumps the count, and returns count == capacity.
bool CTriangleSelector::AddResult(const core::triangle3d<float>& triangle) const {
    core::triangle3d<float>& slot = resultTriangles[resultCount];  // +0xa0 + 36 * count
    slot = triangle;
    if (!transform.flag) {
        slot.A = transformPoint(transform, slot.A);
        slot.B = transformPoint(transform, slot.B);
        slot.C = transformPoint(transform, slot.C);
    }
    const std::uint32_t count = resultCount + 1;
    resultCount = count;
    return count == resultCapacity;
}

// 0x586ec8 _ZNK6glitch5scene17CTriangleSelector11TestWithBoxERSt6vectorINS_4core10triangle3dIfEENS3_10SAllocatorIS5_LNS_6memory13E_MEMORY_HINTE0EEEE
// Reconstructed. A candidate is kept when it overlaps the box on each axis: on an axis, not all three corners
// are below the min, and not all three are above the max. Kept candidates go to AddResult, and the loop stops
// when AddResult reports the buffer full. The Hex-Rays float return is the register leftover; the two callers
// in this batch's scope (0x5870fc and 0x591ca8) discard it, so the body returns void.
void CTriangleSelector::TestWithBox(std::vector<core::triangle3d<float>>& candidates) const {
    const float minX = boundingBox.MinEdge.X;
    const float minY = boundingBox.MinEdge.Y;
    const float minZ = boundingBox.MinEdge.Z;
    const float maxX = boundingBox.MaxEdge.X;
    const float maxY = boundingBox.MaxEdge.Y;
    const float maxZ = boundingBox.MaxEdge.Z;

    for (const core::triangle3d<float>& t : candidates) {
        const bool overlapX = (t.A.X >= minX || t.B.X >= minX || t.C.X >= minX) &&
                              (t.A.X <= maxX || t.B.X <= maxX || t.C.X <= maxX);
        if (!overlapX) continue;
        const bool overlapY = (t.A.Y >= minY || t.B.Y >= minY || t.C.Y >= minY) &&
                              (t.A.Y <= maxY || t.B.Y <= maxY || t.C.Y <= maxY);
        if (!overlapY) continue;
        const bool overlapZ = (t.A.Z >= minZ || t.B.Z >= minZ || t.C.Z >= minZ) &&
                              (t.A.Z <= maxZ || t.B.Z <= maxZ || t.C.Z <= maxZ);
        if (!overlapZ) continue;
        if (AddResult(t)) break;
    }
}

}  // namespace glitch::scene
