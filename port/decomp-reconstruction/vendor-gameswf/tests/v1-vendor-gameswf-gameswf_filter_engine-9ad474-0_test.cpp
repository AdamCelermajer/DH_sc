// Tests for the pure passes of gameswf::filter_engine (batch v1-vendor-gameswf-gameswf_filter_engine-9ad474-0).
// The gaussian kernel and the texture_cache base are owned by other rows, so the test supplies a
// flat kernel and empty base functions. Build: check_tu.sh --test <this file> <batch .cpp>.

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "vendor-gameswf/filter.hpp"
#include "vendor-gameswf/filter_engine.hpp"

namespace {
float g_kernel_value = 1.0f;
int g_failures = 0;
}  // namespace

namespace gameswf {

void gaussian_kernel(float* kernel, int radius) {
    (void)radius;
    for (int i = 0; i < 257; ++i) kernel[i] = g_kernel_value;
}

}  // namespace gameswf

using gameswf::filter;
using gameswf::filter_engine;
using raster = filter_engine::raster;
using params = filter_engine::params;

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
            ++g_failures;                                                        \
        }                                                                        \
    } while (0)

static raster make_raster(std::uint8_t* data, int w, int h, int stride, int bpp) {
    raster r{};
    r.data = data;
    r.width = w;
    r.height = h;
    r.stride = stride;
    r.bpp = bpp;
    return r;
}

static void test_blend() {
    std::uint8_t s[4] = {100, 0, 0, 128};
    std::uint8_t d[4] = {255, 255, 255, 255};
    raster sr = make_raster(s, 1, 1, 4, 4);
    raster dr = make_raster(d, 1, 1, 4, 4);
    filter_engine::blend(sr, dr);
    // inv = 127: 255 * 127 / 255 = 127, plus the source channel.
    CHECK(d[0] == 227);
    CHECK(d[1] == 127);
    CHECK(d[2] == 127);
    CHECK(d[3] == 255);

    std::uint8_t s0[4] = {9, 9, 9, 0};
    std::uint8_t d0[4] = {1, 2, 3, 4};
    raster sr0 = make_raster(s0, 1, 1, 4, 4);
    raster dr0 = make_raster(d0, 1, 1, 4, 4);
    filter_engine::blend(sr0, dr0);
    CHECK(d0[0] == 1 && d0[1] == 2 && d0[2] == 3 && d0[3] == 4);
}

static void test_copy() {
    std::uint8_t src[8] = {1, 2, 3, 4, 5, 6, 7, 8};  // 4 x 2, one byte a pixel, stride 4
    std::uint8_t dst[12] = {0};                       // stride 6
    raster sr = make_raster(src, 4, 2, 4, 1);
    raster dr = make_raster(dst, 4, 2, 6, 1);
    filter_engine::copy(sr, dr);
    CHECK(dst[0] == 1 && dst[1] == 2 && dst[2] == 3 && dst[3] == 4);
    CHECK(dst[4] == 0 && dst[5] == 0);
    CHECK(dst[6] == 5 && dst[7] == 6 && dst[8] == 7 && dst[9] == 8);
}

static void test_clear() {
    std::uint8_t buf[24];
    std::memset(buf, 0xFF, sizeof buf);
    raster r = make_raster(buf, 2, 2, 8, 4);  // two rows of stride 8
    filter_engine::clear(r);
    bool head_zero = true;
    for (int i = 0; i < 16; ++i) head_zero = head_zero && buf[i] == 0;
    CHECK(head_zero);
    bool tail_kept = true;
    for (int i = 16; i < 24; ++i) tail_kept = tail_kept && buf[i] == 0xFF;
    CHECK(tail_kept);
}

static void test_prepare_shadow() {
    filter e{};
    e.type = 0;
    e.angle = 0.0f;
    e.distance = 4.0f;
    std::uint8_t dummy[4] = {0};
    params p{};
    p.filter = &e;
    p.src = make_raster(dummy, 10, 10, 40, 4);
    filter_engine::prepare(p);
    CHECK(p.dst.x == 4);
    CHECK(p.dst.y == 0);
    CHECK(p.dst.width == 14);
    CHECK(p.dst.height == 10);
}

static void test_prepare_glow_blur() {
    filter e{};
    e.type = 2;
    e.blur_x = 2.0f;
    e.blur_y = 2.0f;
    std::uint8_t dummy[4] = {0};
    params p{};
    p.filter = &e;
    p.src = make_raster(dummy, 10, 10, 40, 4);
    filter_engine::prepare(p);
    // glow: +2 each axis; blur adds round(2.0 + 0.5) = 2 and offsets by round-down(2 * 0.5) = 1.
    CHECK(p.dst.width == 14);
    CHECK(p.dst.height == 14);
    CHECK(p.dst.x == -1);
    CHECK(p.dst.y == -1);
}

static void test_prepare_blur_only() {
    filter e{};
    e.type = 1;
    e.blur_x = 4.0f;
    e.blur_y = 2.0f;
    std::uint8_t dummy[4] = {0};
    params p{};
    p.filter = &e;
    p.src = make_raster(dummy, 10, 10, 40, 4);
    filter_engine::prepare(p);
    CHECK(p.dst.width == 14);
    CHECK(p.dst.height == 12);
    CHECK(p.dst.x == -2);
    CHECK(p.dst.y == -1);
}

static void test_shadow_colour() {
    filter e{};
    e.type = 0;
    e.color[0] = 10;
    e.color[1] = 20;
    e.color[2] = 30;
    e.color[3] = 200;
    e.flags = 0;
    std::uint8_t sbuf[4] = {0, 0, 0, 255};
    std::uint8_t dbuf[4] = {0, 0, 0, 0};
    params p{};
    p.filter = &e;
    p.src = make_raster(sbuf, 1, 1, 4, 4);
    p.dst = make_raster(dbuf, 1, 1, 4, 4);
    filter_engine::apply_shadow(p);
    // alpha = 200 * 255 / 255 = 200, through the binary's reciprocal.
    CHECK(dbuf[0] == 10 && dbuf[1] == 20 && dbuf[2] == 30 && dbuf[3] == 200);
}

static void test_shadow_then_blend() {
    filter e{};
    e.type = 0;
    e.color[0] = 10;
    e.color[1] = 20;
    e.color[2] = 30;
    e.color[3] = 200;
    e.flags = 0x20;
    std::uint8_t sbuf[4] = {1, 2, 3, 255};
    std::uint8_t dbuf[4] = {0, 0, 0, 0};
    params p{};
    p.filter = &e;
    p.src = make_raster(sbuf, 1, 1, 4, 4);
    p.dst = make_raster(dbuf, 1, 1, 4, 4);
    filter_engine::apply_shadow(p);
    // The opaque source covers the shadow completely.
    CHECK(dbuf[0] == 1 && dbuf[1] == 2 && dbuf[2] == 3 && dbuf[3] == 255);
}

static void test_blur_h_one_channel() {
    g_kernel_value = 1.0f;
    filter e{};
    e.type = 1;
    e.blur_x = 1.0f;
    std::uint8_t sbuf[3] = {10, 20, 30};
    std::uint8_t dbuf[3] = {0, 0, 0};
    params p{};
    p.filter = &e;
    p.src = make_raster(sbuf, 3, 1, 3, 1);
    p.dst = make_raster(dbuf, 3, 1, 3, 1);
    filter_engine::apply_blur_h(p);
    CHECK(dbuf[0] == 30);
    CHECK(dbuf[1] == 60);
    CHECK(dbuf[2] == 50);
}

static void test_blur_v_one_channel() {
    g_kernel_value = 1.0f;
    filter e{};
    e.type = 1;
    e.blur_y = 1.0f;
    std::uint8_t sbuf[3] = {10, 20, 30};  // one column, three rows, stride 1
    std::uint8_t dbuf[3] = {0, 0, 0};
    params p{};
    p.filter = &e;
    p.src = make_raster(sbuf, 1, 3, 1, 1);
    p.dst = make_raster(dbuf, 1, 3, 1, 1);
    filter_engine::apply_blur_v(p);
    CHECK(dbuf[0] == 30);
    CHECK(dbuf[1] == 60);
    CHECK(dbuf[2] == 50);
}

static void test_blur_h_four_channel() {
    g_kernel_value = 0.5f;
    filter e{};
    e.type = 1;
    e.blur_x = 1.0f;
    std::uint8_t sbuf[8] = {10, 20, 30, 255, 1, 2, 3, 255};
    std::uint8_t dbuf[8] = {0};
    params p{};
    p.filter = &e;
    p.src = make_raster(sbuf, 2, 1, 8, 4);
    p.dst = make_raster(dbuf, 2, 1, 8, 4);
    filter_engine::apply_blur_h(p);
    // Each output is the half-weighted sum of the two neighbours; alpha stays 255.
    CHECK(dbuf[0] == 5 && dbuf[1] == 11 && dbuf[2] == 16 && dbuf[3] == 255);
    CHECK(dbuf[4] == 5 && dbuf[5] == 11 && dbuf[6] == 16 && dbuf[7] == 255);
}

static void test_apply_dispatch() {
    filter copy_filter{};
    copy_filter.type = 0;
    std::uint8_t sbuf[4] = {1, 2, 3, 4};
    std::uint8_t dbuf[4] = {0, 0, 0, 0};
    params p{};
    p.filter = &copy_filter;
    p.src = make_raster(sbuf, 1, 1, 4, 4);
    p.dst = make_raster(dbuf, 1, 1, 4, 4);
    filter_engine::apply(p);
    CHECK(dbuf[0] == 1 && dbuf[1] == 2 && dbuf[2] == 3 && dbuf[3] == 4);
    CHECK(p.src.data == sbuf);

    // Glow: the empty glow pass, then the rasters swap, then dst (the old src) is cleared.
    filter glow{};
    glow.type = 2;
    std::uint8_t a[4] = {7, 7, 7, 7};
    std::uint8_t b[4] = {9, 9, 9, 9};
    params q{};
    q.filter = &glow;
    q.src = make_raster(a, 1, 1, 4, 4);
    q.dst = make_raster(b, 1, 1, 4, 4);
    filter_engine::apply(q);
    CHECK(q.dst.data == a);
    CHECK(q.src.data == b);
    CHECK(a[0] == 0 && a[1] == 0 && a[2] == 0 && a[3] == 0);
    CHECK(b[0] == 9 && b[1] == 9 && b[2] == 9 && b[3] == 9);

    // Blur with no radius: the only step is the final swap.
    filter none{};
    none.type = 1;
    std::uint8_t c[4] = {5, 5, 5, 5};
    std::uint8_t d[4] = {6, 6, 6, 6};
    params r{};
    r.filter = &none;
    r.src = make_raster(c, 1, 1, 4, 4);
    r.dst = make_raster(d, 1, 1, 4, 4);
    filter_engine::apply(r);
    CHECK(r.dst.data == c);
    CHECK(r.src.data == d);
}

static void test_glow_pass_is_empty() {
    std::uint8_t buf[4] = {1, 2, 3, 4};
    filter e{};
    params p{};
    p.filter = &e;
    p.src = make_raster(buf, 1, 1, 4, 4);
    filter_engine::apply_glow(p);
    CHECK(buf[0] == 1 && buf[1] == 2 && buf[2] == 3 && buf[3] == 4);
}

int main() {
    test_blend();
    test_copy();
    test_clear();
    test_prepare_shadow();
    test_prepare_glow_blur();
    test_prepare_blur_only();
    test_shadow_colour();
    test_shadow_then_blend();
    test_blur_h_one_channel();
    test_blur_v_one_channel();
    test_blur_h_four_channel();
    test_apply_dispatch();
    test_glow_pass_is_empty();
    if (g_failures != 0) {
        std::printf("%d check(s) failed\n", g_failures);
        return 1;
    }
    std::printf("all filter_engine pass tests passed\n");
    return 0;
}
