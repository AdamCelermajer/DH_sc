#pragma once
#include "navigation.hpp"
namespace dh2::collision {
using Triangle=navigation::Triangle;
struct Ray {float start[3], end[3];};
// Triangles are the ordered, transformed output of a triangle selector. The
// selector/BVH itself is a separate reconstruction boundary.
struct Floor {const Triangle* triangles;std::uint32_t count,reserved;float minimum[3],maximum[3];};
struct FloorSet {const Floor* floors;std::uint32_t count,reserved;};
struct Result {std::uint32_t hit,index;float point[3];Triangle triangle;};
static_assert(sizeof(Ray)==24&&sizeof(Result)==56);
}
extern "C" {
// 1 hit, 0 miss, -1 invalid caller storage. Plane intersections outside the
// triangle still write point, matching getIntersectionWithLine.
int dh2_collision_line(float* point,const dh2::collision::Triangle*,const float* origin,const float* direction);
// On a miss, Result point/triangle retain the caller's bytes. Header becomes
// hit=0/index=UINT_MAX. Finite and IEEE nonfinite arithmetic follow originals.
int dh2_collision_raycast(dh2::collision::Result*,const dh2::collision::Triangle*,std::uint32_t count,const dh2::collision::Ray*);
int dh2_collision_floor(dh2::collision::Result*,const dh2::collision::Floor*,const float* point);
// Graph::query callback. Caller owns the indexed FloorSet and ordered selector
// snapshots. No original service pointers or translation runtime are required.
std::uint32_t dh2_collision_floor_query(void*,std::uint32_t floor,const float* point);
}
