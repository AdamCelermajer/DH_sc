// glitch::video::detail::IMaterialParameters: indexed and array reads and writes for the three
// owner layouts (global manager, CMaterial, CMaterialRenderer), the stride rule, type codes, the
// dirty-range markers of CMaterial, and the same-type-only conversion path.
#include <cstdio>

#include "glitch-core-io/video/material_parameters.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch;
using namespace glitch::video::detail;
using glitch::core::vector3d;
using glitch::core::vector2d;
using glitch::core::vector4d;

// Index arguments are variables: a literal 0 would also match the array overloads (null pointer).
const std::uint32_t kIdx0 = 0;
const std::uint32_t kIdx1 = 1;
const std::uint32_t kIdx2 = 2;

namespace {

SShaderParameterDef makeDef(std::uint8_t type, std::uint32_t count, std::uint32_t offset) {
    static const int marker = 0;
    SShaderParameterDef def;
    def.sharedString = &marker;
    def.type = type;
    def.count = count;
    def.offset = offset;
    return def;
}

void testGlobal() {
    GlobalMaterialParameters params;
    // Three ints at offset 0 (type 1) and two vec3 floats at offset 12 (type 7).
    params.storage.values.assign(64, 0);
    const std::uint16_t ints = params.storage.defs.add("ints", makeDef(1, 3, 0));
    const std::uint16_t vecs = params.storage.defs.add("vecs", makeDef(7, 2, 12));
    CHECK(ints == 0 && vecs == 1);

    // Indexed write and read.
    CHECK(params.setParameter<int>(ints, kIdx1, 7));
    int v = 0;
    CHECK(params.getParameter<int>(ints, kIdx1, v) && v == 7);
    CHECK(!params.setParameter<int>(ints, 3u, 9));  // index == count
    CHECK(!params.getParameter<int>(ints, 3u, v));

    // Array write with stride 8 (every other int of the caller's buffer).
    const int src[6] = {1, -1, 2, -1, 3, -1};
    CHECK(params.setParameter<int>(ints, src, 8));
    CHECK(params.getParameter<int>(ints, kIdx0, v) && v == 1);
    CHECK(params.getParameter<int>(ints, kIdx1, v) && v == 2);
    CHECK(params.getParameter<int>(ints, kIdx2, v) && v == 3);

    // Array read with stride 0 is contiguous; with stride 8 it spreads into the caller's buffer.
    int contiguous[3] = {0, 0, 0};
    CHECK(params.getParameter<int>(ints, contiguous, 0));
    CHECK(contiguous[0] == 1 && contiguous[1] == 2 && contiguous[2] == 3);
    int spread[6] = {0, 0, 0, 0, 0, 0};
    CHECK(params.getParameter<int>(ints, spread, 8));
    CHECK(spread[0] == 1 && spread[2] == 2 && spread[4] == 3 && spread[1] == 0);

    // Vector of floats: indexed and array forms, type mismatch refused.
    const vector3d<float> pts[2] = {{1.f, 2.f, 3.f}, {4.f, 5.f, 6.f}};
    CHECK(params.setParameter<vector3d<float>>(vecs, pts, 0));
    vector3d<float> p{};
    CHECK(params.getParameter<vector3d<float>>(vecs, kIdx1, p));
    CHECK(p.X == 4.f && p.Y == 5.f && p.Z == 6.f);
    CHECK(!params.getParameter<float>(vecs, kIdx0, p.X));  // the parameter is a vec3, not a float
    vector3d<float> back[2] = {};
    CHECK(params.getParameter<vector3d<float>>(vecs, back, 0));
    CHECK(back[0].X == 1.f && back[1].Z == 6.f);

    // Unset ids are refused by every form.
    CHECK(!params.getParameter<int>(42, kIdx0, v));
    CHECK(!params.setParameter<int>(42, src, 0));
    CHECK(!params.setParameter<int>(65535, kIdx0, 1));

    // Colours: a 4-byte packed colour and a 16-byte float colour.
    params.storage.values.resize(128, 0);
    const std::uint16_t colour = params.storage.defs.add("colour", makeDef(16, 1, 64));
    const std::uint16_t colourf = params.storage.defs.add("colourf", makeDef(17, 1, 80));
    glitch::video::SColor c{0x11223344u};
    CHECK(params.setParameter<glitch::video::SColor>(colour, kIdx0, c));
    glitch::video::SColor cb{};
    CHECK(params.getParameter<glitch::video::SColor>(colour, kIdx0, cb) && cb.color == 0x11223344u);
    glitch::video::SColorf cf{0.25f, 0.5f, 0.75f, 1.f};
    CHECK(params.setParameter<glitch::video::SColorf>(colourf, kIdx0, cf));
    glitch::video::SColorf cfb{};
    CHECK(params.getParameter<glitch::video::SColorf>(colourf, kIdx0, cfb) && cfb.b == 0.75f);

    // Conversion forms: same-type only, the rest is partial (see the header).
    CHECK(params.getParameterCvt<int>(ints, kIdx1, v) && v == 2);
    CHECK(!params.getParameterCvt<float>(ints, kIdx1, p.X));
    CHECK(params.setParameterCvt<int>(ints, kIdx0, 5));
    CHECK(params.getParameter<int>(ints, kIdx0, v) && v == 5);
}

void testMaterial() {
    SParameterBlock block;
    block.count = 2;
    block.entries.push_back(makeDef(5, 2, 0));  // two floats at inline offset 0
    block.entries.push_back(makeDef(1, 1, 8));  // one int at inline offset 8
    MaterialParameters mat;
    mat.storage.block = &block;
    mat.storage.inlineValues.assign(32, 0);

    // The setter resets the dirty markers, as the original does (-1 at +0x0c and +0x10).
    mat.storage.dirtyFirst = 4;
    mat.storage.dirtyLast = 9;
    CHECK(mat.setParameter<int>(1, kIdx0, 11));
    CHECK(mat.storage.dirtyFirst == -1 && mat.storage.dirtyLast == -1);

    int v = 0;
    CHECK(mat.getParameter<int>(1, kIdx0, v) && v == 11);
    // An equal value is not written: the call returns false and the markers keep their values.
    mat.storage.dirtyFirst = 4;
    CHECK(!mat.setParameter<int>(1, kIdx0, 11));
    CHECK(mat.storage.dirtyFirst == 4);
    const float fl[2] = {1.5f, 2.5f};
    CHECK(mat.setParameter<float>(0, fl, 0));
    float out[2] = {};
    CHECK(mat.getParameter<float>(0, out, 0) && out[0] == 1.5f && out[1] == 2.5f);
    CHECK(!mat.getParameter<float>(0, kIdx2, out[0]));  // index == count of the entry
    CHECK(!mat.getParameter<int>(2, kIdx0, v));         // id == block count
    CHECK(!mat.setParameter<float>(7, fl, 0));
    block.count = 1;                                 // the block count bounds the ids
    CHECK(!mat.getParameter<int>(1, kIdx0, v));
}

void testRenderer() {
    RendererParameters rend;
    rend.storage.count = 2;
    rend.storage.entries.push_back(makeDef(1, 2, 0));
    rend.storage.entries.push_back(makeDef(8, 1, 8));  // one vec4 float
    rend.storage.values.assign(32, 0);

    const int ints[2] = {4, 5};
    CHECK(rend.setParameter<int>(0, ints, 0));
    int v = 0;
    CHECK(rend.getParameter<int>(0, kIdx1, v) && v == 5);
    CHECK(rend.setParameter<int>(0, kIdx0, 9));
    CHECK(rend.getParameter<int>(0, kIdx0, v) && v == 9);

    const vector4d<float> q{1.f, 2.f, 3.f, 4.f};
    CHECK(rend.setParameter<vector4d<float>>(1, &q, 16));
    vector4d<float> q2{};
    CHECK(rend.getParameter<vector4d<float>>(1, kIdx0, q2) && q2.W == 4.f);
    vector2d<float> two{};
    CHECK(!rend.getParameter<vector2d<float>>(1, kIdx0, two));  // type 8 is not vec2
    CHECK(!rend.setParameter<int>(2, kIdx0, 1));  // id >= count
}

}  // namespace

int main() {
    testGlobal();
    testMaterial();
    testRenderer();
    if (failures == 0) std::printf("material_parameters_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
