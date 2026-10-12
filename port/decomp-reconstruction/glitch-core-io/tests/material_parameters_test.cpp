// glitch::video::detail::IMaterialParameters: indexed and array reads and writes for the three
// owner layouts (global manager, CMaterial, CMaterialRenderer), the stride rule, type codes, the
// dirty-range markers of CMaterial, and the Convertions-gated conversion forms.
#include <cmath>
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
}

// The conversion forms of the global manager. The indexed forms touch the entry's first element.
void testGlobalConversions() {
    GlobalMaterialParameters params;
    params.storage.values.assign(128, 0);
    const std::uint16_t ints = params.storage.defs.add("ints", makeDef(1, 3, 0));        // int[3] at 0
    const std::uint16_t floats = params.storage.defs.add("floats", makeDef(5, 2, 16));   // float[2] at 16
    const std::uint16_t vec = params.storage.defs.add("vec", makeDef(7, 1, 32));         // vec3f at 32
    const std::uint16_t colour = params.storage.defs.add("colour", makeDef(16, 1, 48));  // SColor at 48
    const std::uint16_t colourf = params.storage.defs.add("colourf", makeDef(17, 1, 64)); // SColorf at 64
    const int iv[3] = {1, 2, 3};
    std::memcpy(params.storage.values.data() + 0, iv, sizeof iv);
    const float fv[2] = {1.5f, -2.5f};
    std::memcpy(params.storage.values.data() + 16, fv, sizeof fv);
    const float v3f[3] = {1.f, 2.f, 3.f};
    std::memcpy(params.storage.values.data() + 32, v3f, sizeof v3f);

    // Indexed reads: the index is range-checked, the first element is read.
    int v = 0;
    CHECK(params.getParameterCvt<int>(ints, kIdx2, v) && v == 1);
    CHECK(!params.getParameterCvt<int>(ints, 3u, v));
    float f = 0.f;
    CHECK(params.getParameterCvt<float>(ints, kIdx1, f) && f == 1.f);   // int stored, read as float
    CHECK(params.getParameterCvt<int>(floats, kIdx0, v) && v == 1);     // float stored, truncated to int
    CHECK(params.getParameterCvt<float>(floats, kIdx1, f) && f == 1.5f);
    vector3d<float> vec3{};
    CHECK(!params.getParameterCvt<vector3d<float>>(ints, kIdx0, vec3));  // gate 0x80 is closed for type 1
    CHECK(params.getParameterCvt<vector3d<float>>(vec, kIdx0, vec3) && vec3.Z == 3.f);
    CHECK(!params.getParameterCvt<int>(vec, kIdx0, v));                  // gate 0x2 is closed for type 7

    // Colour reads: packed colour to float colour, and float colour to packed colour.
    glitch::video::SColor packed{0x11223344u};
    CHECK(params.setParameter<glitch::video::SColor>(colour, kIdx0, packed));
    glitch::video::SColorf cf{};
    CHECK(params.getParameterCvt<glitch::video::SColorf>(colour, kIdx0, cf));
    CHECK(std::fabs(cf.r - 68.f / 255.f) < 1e-6f && std::fabs(cf.a - 17.f / 255.f) < 1e-6f);
    const glitch::video::SColorf cfIn{1.f, 0.f, 0.5f, 1.f};
    CHECK(params.setParameter<glitch::video::SColorf>(colourf, kIdx0, cfIn));
    glitch::video::SColor back{};
    CHECK(params.getParameterCvt<glitch::video::SColor>(colourf, kIdx0, back));
    CHECK(back.color == 0xFF7F00FFu);  // r 255, g 0, b floor(127.5) = 127, a 255 (bytes in memory order)

    // Indexed writes: int and float convert each way (float to int truncates, saturating).
    CHECK(params.setParameterCvt<float>(ints, kIdx0, 2.9f));
    CHECK(params.getParameter<int>(ints, kIdx0, v) && v == 2);
    CHECK(params.setParameterCvt<float>(ints, kIdx0, 1e30f));
    CHECK(params.getParameter<int>(ints, kIdx0, v) && v == 0x7FFFFFFF);
    CHECK(params.setParameterCvt<int>(floats, kIdx0, 4));
    CHECK(params.getParameter<float>(floats, kIdx0, f) && f == 4.f);
    CHECK(params.setParameterCvt<int>(ints, kIdx0, 5));
    CHECK(params.getParameter<int>(ints, kIdx0, v) && v == 5);
    CHECK(!params.setParameterCvt<float>(vec, kIdx0, 1.f));              // type 7 takes no float
    const vector3d<float> vset{1.f, 2.f, 3.f};
    CHECK(!params.setParameterCvt<vector3d<float>>(ints, kIdx0, vset));

    // Array writes: ints from floats with stride 4 (contiguous); a stride of 0 does nothing for ints.
    const float srcF[3] = {7.9f, -1.2f, 2.0f};
    CHECK(params.setParameterCvt<float>(ints, srcF, 0));                  // stride 0, other type: no-op
    CHECK(params.getParameter<int>(ints, kIdx0, v) && v == 5);
    CHECK(params.setParameterCvt<float>(ints, srcF, 4));
    CHECK(params.getParameter<int>(ints, kIdx0, v) && v == 7);
    CHECK(params.getParameter<int>(ints, kIdx1, v) && v == -1);
    CHECK(params.getParameter<int>(ints, kIdx2, v) && v == 2);
    glitch::video::SColorf srcC[1] = {{0.f, 1.f, 0.f, 1.f}};
    CHECK(params.setParameterCvt<glitch::video::SColorf>(colour, srcC, 16));
    glitch::video::SColor outSC{};
    CHECK(params.getParameter<glitch::video::SColor>(colour, kIdx0, outSC) && outSC.color == 0xFF00FF00u);

    // Array reads: an int array read copies int-stored values only; a float array read converts ints.
    int outI[3] = {0, 0, 0};
    CHECK(params.getParameterCvt<int>(ints, outI, 0));
    CHECK(outI[0] == 7 && outI[1] == -1 && outI[2] == 2);
    float outF[4] = {-9.f, -9.f, -9.f, -9.f};
    CHECK(params.getParameterCvt<float>(ints, outF, 0));                  // stride 0, other type: no-op
    CHECK(outF[0] == -9.f);
    CHECK(params.getParameterCvt<float>(ints, outF, 4));
    CHECK(outF[0] == 7.f && outF[1] == -1.f && outF[2] == 2.f && outF[3] == -9.f);
    CHECK(params.getParameterCvt<float>(floats, outF, 8));                // float stored, stride 8
    CHECK(outF[0] == 4.f && outF[2] == -2.5f && outF[1] == -1.f);  // floats[0] was set to 4 above
    int outH[2] = {-7, -7};
    CHECK(params.getParameterCvt<int>(floats, outH, 0));                  // int array read skips floats
    CHECK(outH[0] == -7 && outH[1] == -7);
    glitch::video::SColorf outC[1] = {{9.f, 9.f, 9.f, 9.f}};
    CHECK(params.getParameterCvt<glitch::video::SColorf>(colour, outC, 0)); // packed, stride 0: no-op
    CHECK(outC[0].r == 9.f);
    CHECK(params.getParameterCvt<glitch::video::SColorf>(colour, outC, 16));
    CHECK(std::fabs(outC[0].g - 1.f) < 1e-6f && outC[0].r == byteToFloat(0x00) && outC[0].a == byteToFloat(0xFF));
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
    // An equal value is written again and reports success, but the markers keep their values.
    mat.storage.dirtyFirst = 4;
    CHECK(mat.setParameter<int>(1, kIdx0, 11));
    CHECK(mat.storage.dirtyFirst == 4);
    CHECK(mat.getParameter<int>(1, kIdx0, v) && v == 11);
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

// CMaterial conversion forms: the dirty range follows the compare of each indexed write, an array write
// always marks it, and SColorf values compare within the 1e-6 tolerance of SColorf::equals.
void testMaterialConversions() {
    SParameterBlock block;
    block.count = 4;
    block.entries.push_back(makeDef(5, 2, 0));    // float[2] at inline offset 0
    block.entries.push_back(makeDef(1, 1, 8));    // int at inline offset 8
    block.entries.push_back(makeDef(16, 1, 12));  // packed colour at inline offset 12
    block.entries.push_back(makeDef(17, 1, 16));  // float colour at inline offset 16
    MaterialParameters mat;
    mat.storage.block = &block;
    mat.storage.inlineValues.assign(64, 0);

    // Indexed float write from an int: the stored 1.5 changes, so the range is marked.
    const float fl[2] = {1.5f, 2.5f};
    CHECK(mat.setParameter<float>(0, fl, 0));
    mat.storage.dirtyFirst = 4;
    CHECK(mat.setParameterCvt<int>(0, kIdx1, 3));
    CHECK(mat.storage.dirtyFirst == -1);
    float f = 0.f;
    CHECK(mat.getParameter<float>(0, kIdx0, f) && f == 3.f);  // the first element is written
    CHECK(mat.getParameter<float>(0, kIdx1, f) && f == 2.5f); // the index is not used by the write
    // The same value again: written, not marked.
    mat.storage.dirtyFirst = 4;
    CHECK(mat.setParameterCvt<int>(0, kIdx0, 3));
    CHECK(mat.storage.dirtyFirst == 4);

    // Int entry written from a float: truncation, and a change marks the range.
    CHECK(mat.setParameterCvt<float>(1, kIdx0, 2.5f));
    int v = 0;
    CHECK(mat.getParameter<int>(1, kIdx0, v) && v == 2);
    CHECK(mat.setParameterCvt<float>(1, kIdx0, -0.5f));  // truncates toward zero
    CHECK(mat.getParameter<int>(1, kIdx0, v) && v == 0);

    // Float colour: a write within 1e-6 of the stored value is not a change.
    const glitch::video::SColorf cf{0.5f, 0.5f, 0.5f, 0.5f};
    CHECK(mat.setParameter<glitch::video::SColorf>(3, kIdx0, cf));
    mat.storage.dirtyFirst = 4;
    const glitch::video::SColorf nearCf{0.5000005f, 0.5f, 0.5f, 0.5f};
    CHECK(mat.setParameterCvt<glitch::video::SColorf>(3, kIdx0, nearCf));
    CHECK(mat.storage.dirtyFirst == 4);
    const glitch::video::SColorf farCf{0.6f, 0.5f, 0.5f, 0.5f};
    CHECK(mat.setParameterCvt<glitch::video::SColorf>(3, kIdx0, farCf));
    CHECK(mat.storage.dirtyFirst == -1);

    // Packed colour entry written from a float colour: bytes 0, 255, 127, 255 in memory order.
    const glitch::video::SColorf cfIn{0.f, 1.f, 0.5f, 1.f};
    CHECK(mat.setParameterCvt<glitch::video::SColorf>(2, kIdx0, cfIn));
    glitch::video::SColor sc{};
    CHECK(mat.getParameter<glitch::video::SColor>(2, kIdx0, sc) && sc.color == 0xFF7FFF00u);

    // An array write of packed colours into the float colour entry converts each element, always marks.
    const glitch::video::SColor src4[1] = {glitch::video::SColor{0x00FF0000u}};
    mat.storage.dirtyFirst = 4;
    CHECK(mat.setParameterCvt<glitch::video::SColor>(3, src4, 4));
    CHECK(mat.storage.dirtyFirst == -1);
    glitch::video::SColorf got{};
    CHECK(mat.getParameter<glitch::video::SColorf>(3, kIdx0, got));
    CHECK(got.r == 0.f && got.g == 0.f && std::fabs(got.b - 1.f) < 1e-6f && got.a == 0.f);
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

    // Conversion forms: a float4 read as SColorf (both gate 0x20000 and type 8), an int entry refused.
    glitch::video::SColorf asColour{};
    CHECK(rend.getParameterCvt<glitch::video::SColorf>(1, kIdx0, asColour));
    CHECK(asColour.r == 1.f && asColour.g == 2.f && asColour.b == 3.f && asColour.a == 4.f);
    CHECK(!rend.getParameterCvt<vector4d<float>>(0, kIdx0, q2));  // int entry: gate 0x100 is closed
    CHECK(rend.setParameterCvt<vector4d<float>>(1, kIdx0, vector4d<float>{5.f, 6.f, 7.f, 8.f}));
    CHECK(rend.getParameter<vector4d<float>>(1, kIdx0, q2) && q2.Z == 7.f);
}

}  // namespace

int main() {
    testGlobal();
    testGlobalConversions();
    testMaterial();
    testMaterialConversions();
    testRenderer();
    if (failures == 0) std::printf("material_parameters_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
