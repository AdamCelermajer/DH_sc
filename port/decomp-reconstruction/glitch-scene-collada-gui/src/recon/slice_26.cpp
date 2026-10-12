// Slice 26 reconstruction (see include/glitch-scene-collada-gui/recon/slice_26.hpp).
// Only getLowestRoot has a body here. The other rows are partial or skipped
// and are recorded in ledger_parts/slice_26.csv.

#include <cmath>

#include "glitch-scene-collada-gui/recon/slice_26.hpp"

namespace glitch::collada::recon_26 {

// 0x6c4170 _ZN6glitch5scene22CSceneCollisionManager13getLowestRootEffffPf
int getLowestRoot(float a, float b, float c, float maxT, float* out) {
    const float discriminant = (b * b) + ((a * -4.0f) * c);
    if (discriminant < 0.0f) {
        return 0;
    }
    const float root = std::sqrt(discriminant);
    const float twoA = a + a;
    float lo = (-b - root) / twoA;
    float hi = (root - b) / twoA;
    if (lo > hi) {
        const float tmp = lo;
        lo = hi;
        hi = tmp;
    }
    if (lo > 0.0f && lo < maxT) {
        *out = lo;
        return 1;
    }
    if (hi > 0.0f && hi < maxT) {
        *out = hi;
        return 1;
    }
    return 0;
}

}  // namespace glitch::collada::recon_26
