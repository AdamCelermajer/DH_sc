#pragma once
// v21.8 magefix2 test aid (harness only, used by `--player-to <zone>`): the centre of an authored trigger box is not
// necessarily a walkable point. The Mud Lord exit zone (_prim_ExitLevelZone_001) is a 2933 x 2210 box whose centre
// (-20810, 10892) lies outside the boss-room floor; the floor ends near x = -20140 inside the box. A hero placed on the
// centre has no PF floor, so the first movement/attack admission clamps it back to its previous floor point (run m2-b10old:
// "Source PF first clamp ... valid=0 cachedHit=0 worldHit=0 result=<old position>"). That looked like a post-outro "stuck"
// hero but the hero was never locked. This helper picks the walkable point inside the oriented box nearest to its centre.
#include <array>
#include <cmath>
#include <functional>
#include <optional>

namespace dh::foundation::test_aids {

struct ZoneBoxV1 {
    std::array<float, 3> centre{};
    std::array<std::array<float, 3>, 3> axes{{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};
    std::array<float, 3> half{};
};

// Floor query: true with the floor height at (x, y) near z (the caller owns the z tolerance).
using FloorProbeV1 = std::function<bool(float x, float y, float z, float& height)>;

inline bool zone_contains_v1(const ZoneBoxV1& box, const std::array<float, 3>& point, float margin = 0.0f) {
    for (unsigned axis = 0; axis < 3; ++axis) {
        float projected = 0;
        for (unsigned i = 0; i < 3; ++i) projected += (point[i] - box.centre[i]) * box.axes[axis][i];
        if (std::fabs(projected) > box.half[axis] - margin) return false;
    }
    return true;
}

// The walkable point (x, y, floor height) inside the box (kept `margin` units from its faces in X/Y/Z) that is nearest to the
// box centre in XY, or nothing when the box has no floor. The grid step bounds the answer's distance to the true nearest point.
inline std::optional<std::array<float, 3>> zone_floor_placement_v1(const ZoneBoxV1& box, const FloorProbeV1& probe,
                                                                    float step = 40.0f, float margin = 40.0f) {
    if (!probe || !(step > 0)) return std::nullopt;
    const float reach = std::hypot(std::hypot(box.half[0], box.half[1]), box.half[2]);
    std::optional<std::array<float, 3>> best;
    float bestDistance = 0;
    const int count = static_cast<int>(std::ceil(reach / step));
    for (int ix = -count; ix <= count; ++ix) {
        for (int iy = -count; iy <= count; ++iy) {
            const float x = box.centre[0] + static_cast<float>(ix) * step;
            const float y = box.centre[1] + static_cast<float>(iy) * step;
            const float squared = (x - box.centre[0]) * (x - box.centre[0]) + (y - box.centre[1]) * (y - box.centre[1]);
            if (best && squared >= bestDistance) continue;
            float height = 0;
            if (!probe(x, y, box.centre[2], height)) continue;
            if (!zone_contains_v1(box, {x, y, height}, margin)) continue;
            best = std::array<float, 3>{x, y, height};
            bestDistance = squared;
        }
    }
    return best;
}

}  // namespace dh::foundation::test_aids
