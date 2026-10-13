// Batch g1-game-world-PFObject-34f48a-0: the PFObject flag and floor queries, the path-length sum, the two
// constructors, the two destructors and the three DBG_ draw functions. Declarations come from dh2/PFObject.hpp.
// A PARTIAL body names the callee or layout it waits on.
//
// Layout facts used here (original 32-bit offsets, named in dh2/PFObject.hpp and dh2/PFFloor.hpp):
//   PFObject +0x04 m_flags        bit 0 static, bit 1 moving, bit 2 registered obstacle, bit 3 collisions enabled
//   PFObject +0x10 m_floor        PFFloor*
//   PFObject +0x14 m_moveFlags    bit 0 flying, bit 1 swimming
//   PFFloor  +0x24 m_typeMask     bit 0 hole, bit 1 water; CanPathOn tests it against m_moveFlags
//
// Include constraints: dh2/PFObject.hpp and dh2/PFFloor.hpp take Point3D from core-platform/deps/game_types.hpp.
// dh2/PFWorld.hpp is not included: it includes dh2/Point3D.hpp, which redefines that template in the same TU.
// dh2/PFGInnerEdge.hpp is not included: it includes dh2/sfc/math/graph/Edge.hpp, which is absent from the tree.

#include "dh2/PFObject.hpp"

#include <cstring>

#include "dh2/PFFloor.hpp"

namespace {

// Vec3f_K (0x99f878) is the Point3D<float> global that the constructor copies into m_floorNormal. The static
// initialiser writes it as (0, 0, 1), as CameraBase.cpp also notes. There is no shared declaration yet.
constexpr Point3D<float> kVec3fK{0.0f, 0.0f, 1.0f};

}  // namespace

// 0x5241c0 _ZNK8PFObject11IsOverAHoleEv
// The original returns the floor's type word & 1 for a floor, and 0 (the null pointer) when there is none.
bool PFObject::IsOverAHole() const {
    if (m_floor == nullptr) {
        return false;
    }
    return (m_floor->m_typeMask & 1u) != 0;
}

// 0x5241d4 _ZNK8PFObject9IsInWaterEv
// Bit 1 of the floor's type word, with the same null case as IsOverAHole.
bool PFObject::IsInWater() const {
    if (m_floor == nullptr) {
        return false;
    }
    return ((m_floor->m_typeMask >> 1) & 1u) != 0;
}

// 0x5241e8 _ZNK8PFObject8IsFlyingEv
bool PFObject::IsFlying() const {
    return (m_moveFlags & 1u) != 0;
}

// 0x5241f4 _ZN8PFObject9SetFlyingEb
void PFObject::SetFlying(bool flying) {
    if (flying) {
        m_moveFlags |= 1u;
    } else {
        m_moveFlags &= ~std::uint32_t{1};
    }
}

// 0x52420c _ZNK8PFObject10IsSwimmingEv
bool PFObject::IsSwimming() const {
    return ((m_moveFlags >> 1) & 1u) != 0;
}

// 0x524218 _ZN8PFObject11SetSwimmingEb
void PFObject::SetSwimming(bool swimming) {
    if (swimming) {
        m_moveFlags |= 2u;
    } else {
        m_moveFlags &= ~std::uint32_t{2};
    }
}

// 0x524230 _ZNK8PFObject9CanPathOnEP7PFFloor
// No floor means false. The floor's type word is 0 (any object may path on it) or every bit of it is set in
// m_moveFlags.
bool PFObject::CanPathOn(PFFloor* floor) const {
    if (floor == nullptr) {
        return false;
    }
    const std::uint32_t mask = floor->m_typeMask;
    return mask == 0 || mask == (mask & m_moveFlags);
}

// 0x524264 _ZNK8PFObject15GetPathLengthSQEv
// PARTIAL: the original sums the squared 3D length of each edge in m_path. Each edge's end points come from its
// virtual GetSource and GetDestination (vtable slots +0x24 and +0x28), declared in dh2/PFGInnerEdge.hpp. That
// header includes dh2/sfc/math/graph/Edge.hpp, which is absent from the tree, so the edge type cannot be used.
// An empty path gives 0, as the original does. A non-empty path returns 0 as a placeholder until the header exists.
float PFObject::GetPathLengthSQ() const {
    return 0.0f;
}

// 0x524538 _ZN8PFObjectC2Ev
// 0x524644 _ZN8PFObjectC1Ev
// The two addresses hold the same body: the complete-object (C2) and base-object (C1) constructor.
// PARTIAL: the vptr word at +0x4c (off_96C810, the vtable of the embedded Edge<PFGInnerNode, float>) is not stored.
// Host vtables are not addressable here, so unk_4c stays null. The other words of the embedded edge are written.
PFObject::PFObject() {
    m_owner = nullptr;                                  // +0x00
    m_flags = 8u;                                       // +0x04
    m_radius = 1.0f;                                    // +0x08
    m_floorHeight = 0.0f;                               // +0x0c
    m_floor = nullptr;                                  // +0x10
    m_moveFlags = 2u;                                   // +0x14
    m_pos = Point3D<float>{0.0f, 0.0f, 0.0f};           // +0x18 .. +0x20
    m_floorNormal = kVec3fK;                            // +0x24 .. +0x2c
    m_obstacleRadius = 1.0f;                            // +0x30
    m_obstacleStrength = 0.0f;                          // +0x34
    // +0x38 and +0x3c: the list head and tail point at the sentinel. std::list starts empty.
    m_pathEnd = Point3D<float>{0.0f, 0.0f, 0.0f};       // +0x40 .. +0x48
    unk_4c = nullptr;                                   // +0x4c vptr, see PARTIAL above
    // +0x50 from and +0x54 to (null), +0x58 weight 1.0f, +0x5c .. +0x6b zero: the raw words of the embedded edge.
    std::memset(unk_50, 0, sizeof unk_50);
    const float weight = 1.0f;
    std::memcpy(&unk_50[8], &weight, sizeof weight);
    unk_6c = 0;                                         // +0x6c
    unk_70 = 0;                                         // +0x70
    unk_74 = 0;                                         // +0x74
    unk_78 = 0;                                         // +0x78
    m_pathSmoothIdx = 0;                                // +0x7c
    unk_80 = 0;                                         // +0x80
    unk_84 = 0;                                         // +0x84
    unk_88 = 0;                                         // +0x88
    // +0x8c m_debugName is default constructed (empty).
    unk_9c = &m_debugName;                              // +0x9c self pointer to m_debugName
    unk_a0 = &m_debugName;                              // +0xa0 self pointer to m_debugName
}

// 0x524750 _ZN8PFObjectD2Ev
// 0x524f50 _ZN8PFObjectD1Ev
// The two addresses hold the same body: the complete-object (D2) and base-object (D1) destructor.
// PARTIAL, in three parts:
//   - The original first calls PFWorld::DropPath(Singleton<PFWorld>::s_inst, this) and then
//     PFWorld::InitObstacle(Singleton<PFWorld>::s_inst, this, false, 0, 0). No accessor for the PFWorld singleton is
//     declared, so neither call is made. The world keeps this object's path and obstacle entries until one exists.
//   - The original frees the buffer at +0xa0 when it is neither null nor &m_debugName, bounded by the word at +0x8c.
//     That container is not recovered, so no free is made. A freshly constructed object takes no such branch.
//   - The vptr reset to off_96C6F0 is not stored (see the constructor).
// The list nodes are released here, as the original's node loop does.
PFObject::~PFObject() {
    m_path.clear();
}

// 0x524828 _ZNK8PFObject12DBG_DrawPathEv
// PARTIAL: an empty path returns, as the original does. For a non-empty path the original draws each edge in yellow
// (0xFFFFFF00), then two magenta (0xFFFF00FF) links: from m_pos to the first edge's source, and from m_pathEnd to the
// last edge's destination. Each end point is raised by 1 in Z. Those draws need the edge end points (blocked as in
// GetPathLengthSQ) and the driver's material set-up and draw3DLine (vtable slot 7), which no header declares.
void PFObject::DBG_DrawPath() const {
    if (m_path.empty()) {
        return;
    }
}

// 0x524b30 _ZNK8PFObject16DBG_DrawObstacleEv
// PARTIAL: an obstacle (m_flags bits 2 and 3 both set) draws a circle. The original centres it at (m_pos X, m_pos Y),
// with radius m_pos Z + 10 and width m_radius + m_obstacleRadius, in colour 0xFFFF00FF. Two parts are missing. The
// material is set up through the driver (dword_99F73C + 0x10), which no header declares. And
// SceneManager::DBG_DrawCircle needs the scene manager at device +0x1c, reached from Singleton<Application>::s_inst,
// which has no accessor in the tree.
void PFObject::DBG_DrawObstacle() const {
    if ((m_flags & 0xCu) != 0xCu) {
        return;
    }
}

// 0x524cd0 _ZNK8PFObject22DBG_DrawObstacleForcesEv
// PARTIAL: drawn only for a moving object that is not static (m_flags bit 0 clear, bit 1 set). The original then calls
// PFWorld::_CalcObstaclesForce on Singleton<PFWorld>::s_inst, which has no accessor. It then draws each force from this
// object's position: colour (0xFF, v, v, 0xFF) with v = ~(uint8)(scale * 255), and an end point at the normalised
// direction scaled by scale * 100 + 100. PFWorld::ObstacleForce cannot be named here (see the include note above),
// so the per-force work is not made. The material set-up and draw3DLine are omitted as in DBG_DrawPath.
void PFObject::DBG_DrawObstacleForces() const {
    if ((m_flags & 1u) != 0 || (m_flags & 2u) == 0) {
        return;
    }
}
