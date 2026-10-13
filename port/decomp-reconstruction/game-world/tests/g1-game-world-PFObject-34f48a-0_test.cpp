// Test for batch g1-game-world-PFObject-34f48a-0.
//
// The PFObject constructor and destructor are defined in the batch source, so a real object is built on the stack.
// No PFFloor is constructed: PFFloor's constructor is not in this batch, so the floor-positive branches of
// IsOverAHole, IsInWater and CanPathOn are not exercised here. The path-length sum needs PFGInnerEdge and the
// DBG_ draw functions need the driver or the PFWorld singleton, so only their early-return cases run.

#include <cstdio>
#include <cstring>

#include "dh2/PFObject.hpp"

namespace {

int g_failures = 0;

#define EXPECT(cond)                                                             \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
            ++g_failures;                                                        \
        }                                                                        \
    } while (0)

}  // namespace

int main() {
    {
        PFObject obj;

        // Constructor defaults (original offsets in the comments).
        EXPECT(obj.m_owner == nullptr);                         // +0x00
        EXPECT(obj.m_flags == 8u);                              // +0x04
        EXPECT(obj.m_radius == 1.0f);                           // +0x08
        EXPECT(obj.m_floorHeight == 0.0f);                      // +0x0c
        EXPECT(obj.m_floor == nullptr);                         // +0x10
        EXPECT(obj.m_moveFlags == 2u);                          // +0x14
        EXPECT(obj.m_pos.X == 0.0f && obj.m_pos.Y == 0.0f && obj.m_pos.Z == 0.0f);  // +0x18
        EXPECT(obj.m_floorNormal.X == 0.0f && obj.m_floorNormal.Y == 0.0f && obj.m_floorNormal.Z == 1.0f);  // +0x24
        EXPECT(obj.m_obstacleRadius == 1.0f);                   // +0x30
        EXPECT(obj.m_obstacleStrength == 0.0f);                 // +0x34
        EXPECT(obj.m_path.empty());                             // +0x38
        EXPECT(obj.m_pathEnd.X == 0.0f && obj.m_pathEnd.Y == 0.0f && obj.m_pathEnd.Z == 0.0f);  // +0x40
        EXPECT(obj.m_pathSmoothIdx == 0u);                      // +0x7c
        EXPECT(obj.unk_88 == 0);                                // +0x88
        EXPECT(obj.m_debugName.empty());                        // +0x8c
        EXPECT(obj.unk_9c == static_cast<void*>(&obj.m_debugName));  // +0x9c
        EXPECT(obj.unk_a0 == static_cast<void*>(&obj.m_debugName));  // +0xa0

        float weight = 0.0f;
        std::memcpy(&weight, obj.unk_50 + 8, sizeof weight);    // +0x58 weight of the embedded edge
        EXPECT(weight == 1.0f);

        // Flying and swimming are bits 0 and 1 of m_moveFlags. The default word 2 means swimming.
        EXPECT(!obj.IsFlying());
        EXPECT(obj.IsSwimming());
        obj.SetFlying(true);
        EXPECT(obj.IsFlying());
        EXPECT(obj.IsSwimming());
        EXPECT(obj.m_moveFlags == 3u);
        obj.SetSwimming(false);
        EXPECT(obj.IsFlying());
        EXPECT(!obj.IsSwimming());
        EXPECT(obj.m_moveFlags == 1u);
        obj.SetFlying(false);
        EXPECT(!obj.IsFlying());
        EXPECT(!obj.IsSwimming());
        EXPECT(obj.m_moveFlags == 0u);
        obj.SetSwimming(true);
        EXPECT(obj.IsSwimming());
        EXPECT(obj.m_moveFlags == 2u);

        // No floor: the floor queries are false and CanPathOn(nullptr) is false.
        EXPECT(!obj.IsOverAHole());
        EXPECT(!obj.IsInWater());
        EXPECT(!obj.CanPathOn(nullptr));

        // An empty path has length 0.
        EXPECT(obj.GetPathLengthSQ() == 0.0f);

        // The DBG_ draw functions return without drawing for these flag words.
        obj.DBG_DrawPath();                                     // empty path
        obj.DBG_DrawObstacle();                                 // bits 2 and 3 are not both set
        obj.DBG_DrawObstacleForces();                           // bit 1 set, so this is a moving object: nothing drawn
        obj.m_flags = 0u;
        obj.DBG_DrawObstacleForces();                           // bit 1 clear
        obj.m_flags = 2u;
        obj.DBG_DrawObstacleForces();                           // moving and not static: partial, returns
    }  // ~PFObject runs here and clears the (empty) path list.

    if (g_failures != 0) {
        std::printf("%d failure(s)\n", g_failures);
        return 1;
    }
    std::printf("PFObject tests passed\n");
    return 0;
}
