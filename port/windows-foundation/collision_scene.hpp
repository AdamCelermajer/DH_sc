#pragma once
#include "asset_catalog.hpp"
#include "renderer.hpp"
#include <string>

namespace dh::foundation {
struct CollisionTriangle { Vec3 a,b,c; bool floor = true; };
struct FloorHit { Vec3 point,normal; std::size_t triangle = 0; };
struct CollisionScene {
    std::vector<CollisionTriangle> triangles;
    std::vector<std::string> notices;
    std::size_t floorInstances = 0, collisionInstances = 0;
    // XY is the original world's horizontal plane. Returns the nearest floor
    // height within the caller's upward step/downward drop limits.
    bool floor(float x,float y,float referenceZ,FloorHit& hit,
               float maxStepUp=100,float maxDrop=2000) const;
    // Finite segment, two-sided original floor/collision triangles.
    bool raycast(Vec3 from,Vec3 to,FloorHit& hit) const;
};
bool decode_collision_module(const std::vector<std::uint8_t>& bytes,
                             const std::string& authoredNode,const Mat4& placement,
                             CollisionScene& output,std::string& error);
bool load_collision_level(AssetCatalog& assets,const std::filesystem::path& manifestRelative,
                          CollisionScene& output,std::string& error);
}
