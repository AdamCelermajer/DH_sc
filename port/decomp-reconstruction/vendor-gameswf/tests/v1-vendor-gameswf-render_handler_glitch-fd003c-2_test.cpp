// Test for the render_handler_glitch batch v1-vendor-gameswf-render_handler_glitch-fd003c-2 (part 3 of 3).
//
// Covers the pure arithmetic of the draw paths: the pixel snap, the quad corners of draw_bitmap, the matrix transform
// of draw_mesh_primitive, and the quad index words of draw_bitmap. The class itself needs the BufferedRenderer and
// glitch driver objects, so it is not linked here. The expected values are worked from the pseudocode.
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "vendor-gameswf/render_handler_glitch_detail.hpp"

namespace d = render_handler_glitch_detail;

namespace {

int g_failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL line %d: %s\n", __LINE__, #cond);        \
            ++g_failures;                                              \
        }                                                              \
    } while (0)

bool near(float a, float b) {
    return a > b - 1e-4f && a < b + 1e-4f;
}

void check_snap() {
    // 20 * ((int)v + 10) / 20 with C division: the value rounds to the nearest 20.
    CHECK(d::snap_to_pixel(0.0f) == 0.0f);
    CHECK(d::snap_to_pixel(9.9f) == 0.0f);   // (9 + 10) / 20 = 0
    CHECK(d::snap_to_pixel(10.0f) == 20.0f); // (10 + 10) / 20 = 1
    CHECK(d::snap_to_pixel(25.0f) == 20.0f); // (25 + 10) / 20 = 1
    CHECK(d::snap_to_pixel(30.0f) == 40.0f); // (30 + 10) / 20 = 2
    CHECK(d::snap_to_pixel(-10.0f) == 0.0f); // 0 / 20
    CHECK(d::snap_to_pixel(-11.0f) == 0.0f); // -1 / 20 truncates to 0
    CHECK(d::snap_to_pixel(-30.0f) == -20.0f);
}

void check_quad() {
    const float rect[4] = {0.0f, 10.0f, 0.0f, 20.0f};  // xmin, xmax, ymin, ymax

    // Identity: the corners are the rect corners, vertex 3 is the far corner.
    const float identity[6] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    d::QuadPositions q = d::quad_positions(identity, rect);
    CHECK(near(q.x[0], 0.0f) && near(q.y[0], 0.0f));
    CHECK(near(q.x[1], 10.0f) && near(q.y[1], 0.0f));
    CHECK(near(q.x[2], 0.0f) && near(q.y[2], 20.0f));
    CHECK(near(q.x[3], 10.0f) && near(q.y[3], 20.0f));

    // Translation (5, 7).
    const float translate[6] = {1.0f, 0.0f, 5.0f, 0.0f, 1.0f, 7.0f};
    q = d::quad_positions(translate, rect);
    CHECK(near(q.x[0], 5.0f) && near(q.y[0], 7.0f));
    CHECK(near(q.x[1], 15.0f) && near(q.y[1], 7.0f));
    CHECK(near(q.x[2], 5.0f) && near(q.y[2], 27.0f));
    CHECK(near(q.x[3], 15.0f) && near(q.y[3], 27.0f));

    // Swap x and y: the far corner (xmax, ymax) maps to (20, 10).
    const float swap[6] = {0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f};
    q = d::quad_positions(swap, rect);
    CHECK(near(q.x[0], 0.0f) && near(q.y[0], 0.0f));
    CHECK(near(q.x[1], 0.0f) && near(q.y[1], 10.0f));
    CHECK(near(q.x[2], 20.0f) && near(q.y[2], 0.0f));
    CHECK(near(q.x[3], 20.0f) && near(q.y[3], 10.0f));

    // Scale (2, 3) on a unit-sized rect at (1, 1).
    const float scale[6] = {2.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f};
    const float small[4] = {1.0f, 2.0f, 1.0f, 2.0f};
    q = d::quad_positions(scale, small);
    CHECK(near(q.x[0], 2.0f) && near(q.y[0], 3.0f));
    CHECK(near(q.x[1], 4.0f) && near(q.y[1], 3.0f));
    CHECK(near(q.x[2], 2.0f) && near(q.y[2], 6.0f));
    CHECK(near(q.x[3], 4.0f) && near(q.y[3], 6.0f));
}

void check_transform() {
    // Column-major translation by (3, 4, 5).
    const float translate[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 3, 4, 5, 1};
    float x = 1.0f, y = 2.0f, z = 3.0f;
    d::transform_xyz(translate, x, y, z);
    CHECK(near(x, 4.0f) && near(y, 6.0f) && near(z, 8.0f));

    // Uniform scale 2.
    const float scale[16] = {2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1};
    x = 1.0f;
    y = 2.0f;
    z = 3.0f;
    d::transform_xyz(scale, x, y, z);
    CHECK(near(x, 2.0f) && near(y, 4.0f) && near(z, 6.0f));

    // Rotation of 90 degrees about z: column 0 is (0, 1, 0), column 1 is (-1, 0, 0). The point (1, 0, 0) goes to (0, 1, 0).
    const float rot[16] = {0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    x = 1.0f;
    y = 0.0f;
    z = 0.0f;
    d::transform_xyz(rot, x, y, z);
    CHECK(near(x, 0.0f) && near(y, 1.0f) && near(z, 0.0f));
}

void check_indices() {
    // The pseudocode stores the quad's indices as three 32-bit words; read as u16 they are 0, 1, 2, 1, 2, 3.
    const uint32_t words[3] = {0x00010000u, 0x00010002u, 0x00030002u};
    uint16_t expanded[6] = {};
    std::memcpy(expanded, words, sizeof(expanded));
    bool same = true;
    for (int i = 0; i < 6; ++i) {
        same = same && expanded[i] == d::kQuadIndices[i];
    }
    CHECK(same);
    CHECK(d::kQuadIndices[0] == 0 && d::kQuadIndices[1] == 1 && d::kQuadIndices[2] == 2);
    CHECK(d::kQuadIndices[3] == 1 && d::kQuadIndices[4] == 2 && d::kQuadIndices[5] == 3);
    CHECK(d::kMaskBlendIndex == 15);
}

}  // namespace

int main() {
    check_snap();
    check_quad();
    check_transform();
    check_indices();
    if (g_failures == 0) {
        std::printf("render_handler_glitch_fd003c-2 detail: all checks passed\n");
        return 0;
    }
    std::printf("render_handler_glitch_fd003c-2 detail: %d check(s) failed\n", g_failures);
    return 1;
}
