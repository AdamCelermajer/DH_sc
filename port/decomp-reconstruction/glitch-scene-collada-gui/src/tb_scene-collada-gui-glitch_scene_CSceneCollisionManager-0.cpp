// Track B batch e1-glitch-scene-collada-gui-glitch_scene_CSceneCollisionManager-0.
// Bodies for glitch::scene::CSceneCollisionManager. The class is declared in
// tb/glitch_scene_CSceneCollisionManager.hpp and the selector interface in tb/glitch_scene_ITriangleSelector.hpp.
#include "glitch-scene-collada-gui/tb/glitch_scene_CSceneCollisionManager.hpp"

#include <cstddef>
#include <limits>

namespace glitch::scene {

namespace {

// Squared distance between two points, summed in the order the original uses: ((dx*dx + dy*dy) + dz*dz).
float squaredDistance(const core::vector3d<float>& a, const core::vector3d<float>& b) {
    const float dx = a.X - b.X;
    const float dy = a.Y - b.Y;
    const float dz = a.Z - b.Z;
    return ((dx * dx) + (dy * dy)) + (dz * dz);
}

// The loose span test the original applies per axis: some corner of the triangle reaches the low bound and
// some corner is still under the high bound. This is the form in the pseudocode, not a strict overlap test.
bool overlapsSpan(float a, float b, float c, float lo, float hi) {
    return (a >= lo || b >= lo || c >= lo) && (hi >= a || b <= hi || c <= hi);
}

}  // namespace

// 0x6c62e8 _ZN6glitch5scene22CSceneCollisionManager17getCollisionPointERKNS_4core6line3dIfEEPNS0_17ITriangleSelectorERNS2_8vector3dIfEERNS2_10triangle3dIfEE
// Reconstructed from the Hex-Rays body. The return value is the original int 0/1, returned here as bool.
// The selector is called through its vtable slots +0x0c (getTriangleCount) and +0x14 (getTriangles with a box).
// The triangle at index i is the one the original reads at buffer base + 36 * i; the hit test calls the
// glitch triangle3d::getIntersectionWithLine (0x58615c), which the shared core header provides.
bool CSceneCollisionManager::getCollisionPoint(const core::line3d<float>& ray, ITriangleSelector* selector,
                                               core::vector3d<float>& outPoint,
                                               core::triangle3d<float>& outTriangle) {
    if (!selector) return false;

    const core::vector3d<float>& start = ray.start;  // a2[0..2]
    const core::vector3d<float>& end = ray.end;      // a2[3..5]

    // Grow the scratch buffer to the selector's triangle count. The original compares the count with the element
    // count (end - begin) / 36 and resizes with zero-valued triangles. It never shrinks.
    const int triangleCount = selector->getTriangleCount();  // vtable +0x0c
    if (triangleCount > static_cast<int>(triangles.size())) {
        triangles.resize(static_cast<std::size_t>(triangleCount), core::triangle3d<float>{});
    }

    // Bounding box of the segment: min and max of start and end on each axis. The comparisons are the original's.
    core::aabbox3d<float> box;
    box.MinEdge.X = start.X;  // v51
    box.MaxEdge.X = start.X;  // v54
    box.MinEdge.Y = start.Y;  // v52
    box.MaxEdge.Y = start.Y;  // v55
    box.MinEdge.Z = start.Z;  // v53
    box.MaxEdge.Z = start.Z;  // v56
    if (start.X < end.X) box.MaxEdge.X = end.X;
    if (start.Y < end.Y) box.MaxEdge.Y = end.Y;
    if (end.Z > box.MaxEdge.Z) box.MaxEdge.Z = end.Z;
    if (end.X < box.MinEdge.X) box.MinEdge.X = end.X;
    if (end.Y < box.MinEdge.Y) box.MinEdge.Y = end.Y;
    if (end.Z < box.MinEdge.Z) box.MinEdge.Z = end.Z;

    // Collect the triangles that overlap the box. outCount is the number the selector wrote (v62).
    int outCount = 0;
    selector->getTriangles(triangles.data(), triangleCount, outCount, box, nullptr);  // vtable +0x14

    // Unit direction of the segment: the original normalizes end - start (v60, then copied to v61).
    core::vector3d<float> dir{end.X - start.X, end.Y - start.Y, end.Z - start.Z};
    dir.normalize();

    // Squared segment length (v46), taken before the axis bounds are reordered.
    const float lengthSq = squaredDistance(start, end);

    // Per-axis low and high bounds used by the overlap tests (v19/v20, v36/v37, v38/v39). The swap is the original's.
    float minX = start.X, maxX = end.X;  // v19, v20
    if (minX >= maxX) {
        minX = end.X;
        maxX = start.X;
    }
    float minY = start.Y, maxY = end.Y;  // v36, v37
    if (minY >= maxY) {
        minY = end.Y;
        maxY = start.Y;
    }
    float minZ = start.Z, maxZ = end.Z;  // v38, v39
    if (minZ >= maxZ) {
        minZ = end.Z;
        maxZ = start.Z;
    }

    bool found = false;
    // The best hit distance starts at FLT_MAX (v44 = 3.4028e38).
    float bestDistSq = std::numeric_limits<float>::max();
    // Hit point of the last intersection call (v57..v59). It is set once, before the loop, as in the original.
    core::vector3d<float> hit{};

    for (int i = 0; i < outCount; ++i) {
        const core::triangle3d<float>& tri = triangles[static_cast<std::size_t>(i)];

        // Cull on each axis with the span test.
        if (!overlapsSpan(tri.A.X, tri.B.X, tri.C.X, minX, maxX)) continue;
        if (!overlapsSpan(tri.A.Y, tri.B.Y, tri.C.Y, minY, maxY)) continue;
        if (!overlapsSpan(tri.A.Z, tri.B.Z, tri.C.Z, minZ, maxZ)) continue;

        // Keep the triangle only if a corner is nearer to the start than the best hit so far.
        const bool nearer = (bestDistSq > squaredDistance(start, tri.A)) ||
                            (bestDistSq > squaredDistance(start, tri.B)) ||
                            (bestDistSq > squaredDistance(start, tri.C));
        if (!nearer) continue;

        // Intersect the segment with the triangle's plane and inside test.
        if (!tri.getIntersectionWithLine(start, dir, hit)) continue;

        const float hitToStartSq = squaredDistance(hit, start);  // v32
        if (!(lengthSq > hitToStartSq)) continue;
        if (!(lengthSq > squaredDistance(hit, end))) continue;  // the hit lies on the segment
        if (!(bestDistSq > hitToStartSq)) continue;

        bestDistSq = hitToStartSq;
        outTriangle = tri;  // *a5 = triangle words (9 words copied)
        outPoint = hit;     // *a4 = hit (3 floats)
        found = true;
    }

    return found;
}

}  // namespace glitch::scene
