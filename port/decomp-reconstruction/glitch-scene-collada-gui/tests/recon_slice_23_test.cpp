// Slice 23 checks: the PSRandom normal-sample core (rejection and draw order) and the
// stub bodies' neutral results.
#include <cmath>
#include <cstdio>
#include <vector>

#include "glitch-scene-collada-gui/recon/slice_23.hpp"

using namespace glitch::collada::recon_23;

namespace {

// Feeds a fixed sequence of Rand values, cycling when exhausted.
struct Sequence {
    std::vector<float> values;
    std::size_t next = 0;
    float operator()() {
        float v = values[next % values.size()];
        ++next;
        return v;
    }
};

bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

}  // namespace

int main() {
    // First pair (0.75, 0.5) gives x = 0.5, y = 0, s = 0.25 with no rejection.
    const float expected = 0.5f * std::sqrt(std::log(0.25f) * -2.0f / 0.25f);

    Sequence plain{{0.75f, 0.5f}};
    if (!near(ps::normalSample(plain, 1.0f), expected)) {
        std::printf("normalSample plain draw mismatch\n");
        return 1;
    }

    // First pair (0.5, 0.5) gives s = 0 and is redrawn; the next pair is accepted.
    Sequence rejected{{0.5f, 0.5f, 0.75f, 0.5f}};
    if (!near(ps::normalSample(rejected, 1.0f), expected)) {
        std::printf("normalSample rejection mismatch\n");
        return 1;
    }

    // NRandVec: x and y share the factor from the first pair; z is one more NRandf draw.
    Sequence vec{{0.75f, 0.5f, 0.75f, 0.5f}};
    glitch::core::vector3d<float> v = ps::normalVector(vec, 1.0f);
    if (!near(v.X, expected) || !near(v.Y, 0.0f) || !near(v.Z, expected)) {
        std::printf("normalVector mismatch\n");
        return 1;
    }

    // Stub bodies return neutral values.
    glitch::ps::PDBox box;
    if (ps::boxWithin(box, glitch::core::vector3d<float>{}) != false) {
        std::printf("boxWithin stub should be false\n");
        return 1;
    }

    std::printf("slice_23 tests passed\n");
    return 0;
}
