// computeBoundingBox over strided float and integer components.
#include "glitch-core-io/core/bounding_box.hpp"

#include <cstdio>

#define CHECK(c)                                                       \
    do {                                                               \
        if (!(c)) {                                                    \
            std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c);   \
            ++failures;                                                \
        }                                                              \
    } while (0)

int main() {
    using glitch::core::aabbox3d;
    using glitch::core::computeBoundingBox;
    int failures = 0;

    // Two-component elements with a 12-byte stride (one padding float between them).
    const float floats[] = {1.f, 5.f, 9.f, 3.f, -2.f, 7.f};
    aabbox3d<float> box{};
    computeBoundingBox(floats, 2, 12, 2, box);
    CHECK(box.MinEdge.X == 1.f && box.MinEdge.Y == -2.f && box.MinEdge.Z == 0.f);
    CHECK(box.MaxEdge.X == 3.f && box.MaxEdge.Y == 5.f && box.MaxEdge.Z == 0.f);

    // Integer components are widened to float; dims above three reads three.
    const signed char bytes[] = {-4, 6, 2, 9};
    computeBoundingBox(bytes, 3, 1, 4, box);
    CHECK(box.MinEdge.X == -4.f && box.MaxEdge.X == 9.f);

    // Zero elements gives a zero box.
    computeBoundingBox(floats, 3, 12, 0, box);
    CHECK(box.MinEdge.X == 0.f && box.MaxEdge.Z == 0.f);

    if (failures == 0) std::printf("bounding_box_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
