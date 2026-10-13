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
// dh2/PFWorld.hpp (Singleton<PFWorld>::s_inst, DropPath, InitObstacle) and dh2/PFGInnerEdge.hpp (the edge end points)
// share this translation unit: both compile together with PFObject.hpp (checked with check_tu.sh).

#include "dh2/PFObject.hpp"

#include <cstring>

#include "dh2/PFFloor.hpp"
#include "dh2/PFGInnerEdge.hpp"
#include "dh2/PFWorld.hpp"
#include "dh2/global/Vec3f_K.hpp"  // Vec3f_K (0x99f878), the Point3D<float> global the constructor copies into m_floorNormal

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
// The sum of the squared 3D length of each edge in m_path, in list order. Each edge's end points come from its
// virtual GetSource (vtable slot +0x24, call 9) and then GetDestination (slot +0x28, call 10). The float sum is
// accumulated as sum + ((dx*dx + dy*dy) + dz*dz) per edge, as the original does. An empty path gives 0.
float PFObject::GetPathLengthSQ() const {
    float sum = 0.0f;
    for (const PFGInnerEdge* edge : m_path) {
        const Point3D<float>* source = edge->GetSource();
        const Point3D<float>* destination = edge->GetDestination();
        const float dx = source->X - destination->X;
        const float dy = source->Y - destination->Y;
        const float dz = source->Z - destination->Z;
        sum = sum + ((dx * dx + dy * dy) + dz * dz);
    }
    return sum;
}

// 0x524538 _ZN8PFObjectC2Ev
// 0x524644 _ZN8PFObjectC1Ev
// The two addresses hold the same body: the complete-object (C2) and base-object (C1) constructor.
// The embedded PFGInnerDirectEdge (+0x4c .. +0x7b) is built with its vptr; the constructor sets its words: from and to null, weight 1.0f, distance, clearance and both points zero.
PFObject::PFObject() {
    m_owner = nullptr;                                  // +0x00
    m_flags = 8u;                                       // +0x04
    m_radius = 1.0f;                                    // +0x08
    m_room = nullptr;                                   // +0x0c
    m_floor = nullptr;                                  // +0x10
    m_moveFlags = 2u;                                   // +0x14
    m_pos = Point3D<float>{0.0f, 0.0f, 0.0f};           // +0x18 .. +0x20
    m_floorNormal = Vec3f_K;                            // +0x24 .. +0x2c
    m_obstacleRadius = 1.0f;                            // +0x30
    m_obstacleStrength = 0.0f;                          // +0x34
    // +0x38 and +0x3c: the list head and tail point at the sentinel. std::list starts empty.
    m_pathEnd = Point3D<float>{0.0f, 0.0f, 0.0f};       // +0x40 .. +0x48
    m_edge.m_from = nullptr;                            // +0x50
    m_edge.m_to = nullptr;                              // +0x54
    m_edge.m_weight = 1.0f;                             // +0x58
    m_edge.m_distance = 0.0f;                           // +0x5c
    m_edge.m_clearance = 0.0f;                          // +0x60
    m_edge.m_source = Point3D<float>{0.0f, 0.0f, 0.0f};       // +0x64 .. +0x6c
    m_edge.m_destination = Point3D<float>{0.0f, 0.0f, 0.0f};  // +0x70 .. +0x78
    m_pathSmoothIdx = 0;                                // +0x7c
    m_waypointDir = Point3D<float>{0.0f, 0.0f, 0.0f};  // +0x80 .. +0x88
    // +0x8c m_debugName is default constructed (empty).
    unk_9c = &m_debugName;                              // +0x9c self pointer to m_debugName
    unk_a0 = &m_debugName;                              // +0xa0 self pointer to m_debugName
}

// 0x524750 _ZN8PFObjectD2Ev
// 0x524f50 _ZN8PFObjectD1Ev
// The two addresses hold the same body: the complete-object (D2) and base-object (D1) destructor.
// Drops the path in the PF world (PFWorld::DropPath), takes the object out of the obstacle lists (InitObstacle with add false, radius 0, strength 0) and releases the path nodes; the buffer of m_debugName
// (the original frees it when it is not the inline one) and the embedded edge (vptr reset to off_96C6F0) go with the members.
PFObject::~PFObject() {
    PFWorld::s_inst.DropPath(*this);
    PFWorld::s_inst.InitObstacle(*this, false, 0.0f, 0.0f);
    m_path.clear();
}

// 0x524828 _ZNK8PFObject12DBG_DrawPathEv
// PARTIAL: an empty path returns, as the original does. For a non-empty path the original draws each edge in yellow
// (0xFFFFFF00), then two magenta (0xFFFF00FF) links: from m_pos to the first edge's source, and from m_pathEnd to the
// last edge's destination. Each end point is raised by 1 in Z. The edge end points are available (GetSource and
// GetDestination, as in GetPathLengthSQ). The draws need the driver at device +0x10 for the material set-up and
// IVideoDriver::draw3DLine (declared in glitch-video/video_driver.hpp), but glitch::IDevice declares no member at +0x10
// (layout request shared with CameraBase::SetActive).
void PFObject::DBG_DrawPath() const {
    if (m_path.empty()) {
        return;
    }
}

// 0x524b30 _ZNK8PFObject16DBG_DrawObstacleEv
// PARTIAL: an obstacle (m_flags bits 2 and 3 both set) draws a circle. The original centres it at (m_pos X, m_pos Y),
// with radius m_pos Z + 10 and width m_radius + m_obstacleRadius, in colour 0xFFFF00FF. Two parts are missing. The
// material is set up through the driver (device +0x10, dword_99F73C = Application::m_device). And
// SceneManager::DBG_DrawCircle needs the scene manager at device +0x1c. glitch::IDevice declares no member at either
// offset (layout request shared with CameraBase::SetActive), so the draw is left out.
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
