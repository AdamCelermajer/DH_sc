#include "glitch-scene-collada-gui/recon/slice_11.hpp"

#include <cstdio>
#include <cstring>

using namespace glitch::collada::recon_11;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

static SAnimationOutput makeOutput(const void* data) {
    SAnimationOutput o;
    o.data = data;
    return o;
}

int main() {
    // Identity writes 16 zero bytes and returns the pointer it was given.
    {
        std::uint32_t words[4] = {1, 2, 3, 4};
        void* r = CVirtualEx_f4_mix2_getIdentityValue(0, words);
        CHECK(r == words);
        CHECK(words[0] == 0 && words[1] == 0 && words[2] == 0 && words[3] == 0);
    }

    // uchar[4] interp1, lane 0, no default: 10 + 0.5 * (20 - 10) = 15.
    {
        const std::uint8_t d[4] = {10, 20, 30, 40};
        SAnimationOutput o = makeOutput(d);
        SAnimationAccessorStandIn acc;
        acc.output = &o;
        std::uint8_t out[4] = {0, 0, 0, 0};
        unsigned int r = CInterpreter_u4_m0_getKeyBasedValueEx_interp1(acc, 0, 1, 0.5f, out);
        CHECK(r == 15);
        CHECK(out[0] == 15);
    }

    // uchar[4] direct, lane 3 with a default: lane 3 takes the data, the rest the default.
    {
        const std::uint8_t d[4] = {0, 0, 77, 0};
        const std::uint8_t def[4] = {1, 2, 3, 4};
        SAnimationOutput o = makeOutput(d);
        SAnimationAccessorStandIn acc;
        acc.output = &o;
        acc.defaultValue = def;
        std::uint8_t out[4] = {0, 0, 0, 0};
        const std::uint8_t* r = CInterpreter_u4_m3_getKeyBasedValueEx_direct(acc, 2, out);
        CHECK(r == def);
        CHECK(out[0] == 1 && out[1] == 2 && out[2] == 3 && out[3] == 77);
    }

    // uchar[4] diff, lane 1, no default: out[0] gets the difference, nothing else changes.
    {
        const std::uint8_t d[4] = {0, 20, 0, 40};
        SAnimationOutput o = makeOutput(d);
        SAnimationAccessorStandIn acc;
        acc.output = &o;
        std::uint8_t out[4] = {9, 9, 9, 9};
        const std::uint8_t* r = CInterpreter_u4_m1_getKeyBasedValueEx_diff(acc, 1, 3, out);
        CHECK(r == nullptr);
        CHECK(out[0] == 20 && out[1] == 9 && out[2] == 9 && out[3] == 9);
    }

    // uchar[4] diff, lane 1, with default: lane 0 from the default, lane 1 the difference.
    {
        const std::uint8_t d[4] = {0, 20, 0, 40};
        const std::uint8_t def[4] = {1, 2, 3, 4};
        SAnimationOutput o = makeOutput(d);
        SAnimationAccessorStandIn acc;
        acc.output = &o;
        acc.defaultValue = def;
        std::uint8_t out[4] = {0, 0, 0, 0};
        CInterpreter_u4_m1_getKeyBasedValueEx_diff(acc, 1, 3, out);
        CHECK(out[0] == 1 && out[1] == 20 && out[2] == 3 && out[3] == 4);
    }

    // uchar[4] interp2 wraps the byte deltas: base 0, a3 = 200 (wraps to 200), a4 = 100.
    // 200 + 0.5 * (100 - 200) = 150.
    {
        const std::uint8_t d[4] = {0, 200, 100, 0};
        SAnimationOutput o = makeOutput(d);
        SAnimationAccessorStandIn acc;
        acc.output = &o;
        std::uint8_t out[4] = {0, 0, 0, 0};
        unsigned int r = CInterpreter_u4_m0_getKeyBasedValueEx_interp2(acc, 0, 1, 2, 0.5f, out);
        CHECK(r == 150);
        CHECK(out[0] == 150);
    }

    // Forwarder passes its arguments through.
    {
        const std::uint8_t d[4] = {10, 20, 30, 40};
        SAnimationOutput o = makeOutput(d);
        SAnimationAccessorStandIn acc;
        acc.output = &o;
        std::uint8_t out[4] = {0, 0, 0, 0};
        const std::uint8_t* r = CVirtualEx_u4_m2_getKeyBasedValue_direct(0, acc, 3, out);
        CHECK(r == nullptr);
        CHECK(out[0] == 40);
    }

    // float[4] interp2, lane 3, no default: 4 + 0.5 * (8 - 4) = 6 (element indices 0, 4, 8).
    {
        const float f[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        SAnimationOutput o = makeOutput(f);
        SAnimationAccessorStandIn acc;
        acc.output = &o;
        float out[4] = {0, 0, 0, 0};
        float r = CInterpreter_f4_m3_getKeyBasedValueEx_interp2(acc, 0, 4, 8, 0.5f, out);
        CHECK(r == 6.0f);
        CHECK(out[0] == 6.0f);
    }

    // Database destructor with no record is a no-op and leaves the record null.
    {
        CColladaDatabase db;
        CColladaDatabase_D1(&db);
        CHECK(db.record == nullptr);
    }

    std::printf("%s\n", failures ? "recon_slice_11: FAILED" : "recon_slice_11: ok");
    return failures != 0;
}
