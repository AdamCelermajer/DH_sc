// PFObject: path-finding / obstacle state embedded in GameObject at +0x1c8.
// Original size 0xa4. No vtable of its own.
#pragma once

#include <cstdint>
#include <list>
#include <string>
#include "core-platform/deps/game_types.hpp"

class PFFloor;
class PFGInnerEdge;

class PFObject {
public:
    PFObject();                             // 0x524538 / 0x524644
    ~PFObject();                            // 0x524750 / 0x524f50: PFWorld::DropPath + InitObstacle(.., 0, 0, 0), frees path

    bool IsOverAHole() const;               // 0x5241c0 (floor flag bit 0)
    bool IsInWater() const;                 // 0x5241d4 (floor flag bit 1)
    bool IsFlying() const;                  // 0x5241e8 (m_moveFlags bit 0)
    void SetFlying(bool flying);            // 0x5241f4
    bool IsSwimming() const;                // 0x52420c (m_moveFlags bit 1)
    void SetSwimming(bool swimming);        // 0x524218
    bool CanPathOn(PFFloor* floor) const;   // 0x524230
    float GetPathLengthSQ() const;          // 0x524264
    void DBG_DrawPath() const;              // 0x524828
    void DBG_DrawObstacle() const;          // 0x524b30
    void DBG_DrawObstacleForces() const;    // 0x524cd0

    void* m_owner;                          // +0x00 owning GameObject (PFWorld::InitObject stores it)
    std::uint32_t m_flags;                  // +0x04 bit0 static, bit1 moving, bit2 registered obstacle, bit3 collisions enabled; 8 initially
    float m_radius;                         // +0x08 1.0f initially, set from the object extents
    float m_floorHeight;                    // +0x0c output of PFWorld::GetFloorHeightAt. Projectile::OnExpire 0x3e5078 reads GameObject +0x1d4 (this word) as a PFRoom*; the type is not settled, so it stays float (the ctor writes 0.0f)
    PFFloor* m_floor;                       // +0x10
    std::uint32_t m_moveFlags;              // +0x14 bit0 flying, bit1 swimming; 2 initially
    Point3D<float> m_pos;                   // +0x18
    Point3D<float> m_floorNormal;           // +0x24 initialised from Vec3f_K, filled by GetFloorHeightAt
    float m_obstacleRadius;                 // +0x30 1.0f initially (PFWorld::InitObstacle arg)
    float m_obstacleStrength;               // +0x34 (PFWorld::InitObstacle arg)
    std::list<const PFGInnerEdge*> m_path;  // +0x38 waypoint list (sentinel node at +0x38)
    Point3D<float> m_pathEnd;               // +0x40 final path target
    void* unk_4c;                           // +0x4c vptr of an embedded PFGInnerDirectEdge (off_96C810; dtor resets it to off_96C6F0)
    std::uint8_t unk_50[28];                // +0x50..+0x6b rest of that edge: ints 0 at +0x50/+0x54, float 1.0f at +0x58, zeros to +0x68
    std::int32_t unk_6c;                    // +0x6c 0
    std::int32_t unk_70;                    // +0x70 0
    std::int32_t unk_74;                    // +0x74 0
    std::int32_t unk_78;                    // +0x78 0
    std::uint32_t m_pathSmoothIdx;          // +0x7c ("pfo.path_smoothIdx" assert in PFWorld::DropPath)
    std::int32_t unk_80;                    // +0x80 0
    std::int32_t unk_84;                    // +0x84 0
    std::int32_t unk_88;                    // +0x88 0
    std::string m_debugName;                // +0x8c (GameObject::InitFinal assigns the object name)
    std::uint8_t pad_90[12];                // +0x90 .. +0x9b not touched by the functions recovered so far
    void* unk_9c;                           // +0x9c pointer to m_debugName, set by the constructor (the destructor's buffer test reads it)
    void* unk_a0;                           // +0xa0 pointer to m_debugName, set by the constructor; a different non-null value is a buffer the destructor frees
};
