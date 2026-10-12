// Test for slice 26 (getLowestRoot). Simple asserts; returns 1 on failure.
#include <cmath>
#include <cstdio>

#include "glitch-scene-collada-gui/recon/slice_26.hpp"

using glitch::collada::recon_26::getLowestRoot;

int main() {
    float t = 0.0f;

    // t^2 - 3t + 2 = 0 has roots 1 and 2; the lowest in (0, 10) is 1.
    if (getLowestRoot(1.0f, -3.0f, 2.0f, 10.0f, &t) != 1 || std::fabs(t - 1.0f) > 1e-5f) {
        std::fprintf(stderr, "root in range failed\n");
        return 1;
    }

    // Negative discriminant: no real roots.
    if (getLowestRoot(1.0f, 0.0f, 1.0f, 10.0f, &t) != 0) {
        std::fprintf(stderr, "negative discriminant failed\n");
        return 1;
    }

    // Both roots (1 and 2) are beyond maxT = 0.5.
    if (getLowestRoot(1.0f, -3.0f, 2.0f, 0.5f, &t) != 0) {
        std::fprintf(stderr, "out of range failed\n");
        return 1;
    }

    // Roots -1 and 2: the negative one is skipped, 2 is returned.
    if (getLowestRoot(1.0f, -1.0f, -2.0f, 10.0f, &t) != 1 || std::fabs(t - 2.0f) > 1e-5f) {
        std::fprintf(stderr, "negative root skip failed\n");
        return 1;
    }
    return 0;
}
