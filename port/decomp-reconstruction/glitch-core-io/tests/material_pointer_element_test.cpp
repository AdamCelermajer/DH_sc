// Pointer slots (intrusive_ptr<ITexture>, intrusive_ptr<CLight>), setParameterElement and the
// SColorf setter of CMaterial. Returns non-zero on failure.
#include <cstdio>

#include "glitch-core-io/video/material_parameters.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch::video;
using namespace glitch::video::detail;
using boost::intrusive_ptr;

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
}  // namespace

int main() {
    // Texture slots: code 12 + (slotType & 3). Two textures, one sub-type 1 (code 13), one sub-type 0.
    {
        GlobalMaterialParameters params;
        params.storage.values.assign(64, 0);
        const std::uint16_t tex = params.storage.defs.add("tex", makeDef(13, 2, 0));
        intrusive_ptr<ITexture> a(new ITexture());
        a->slotType = 1;  // code 13
        intrusive_ptr<ITexture> b(new ITexture());
        b->slotType = 0;  // code 12, does not match the slot
        CHECK(params.setPointer(tex, 0u, a));
        CHECK(a->referenceCounter == 2);  // caller plus slot
        CHECK(!params.setPointer(tex, 1u, b));
        intrusive_ptr<ITexture> out;
        CHECK(params.getPointer(tex, 0u, out) && out.get() == a.get());
        CHECK(a->referenceCounter == 3);
        out = nullptr;
        CHECK(a->referenceCounter == 2);
        CHECK(params.setPointer(tex, 0u, intrusive_ptr<ITexture>()));
        CHECK(a->referenceCounter == 1);  // slot released its use
    }
    // CLight: code 18 only.
    {
        GlobalMaterialParameters params;
        params.storage.values.assign(32, 0);
        const std::uint16_t light = params.storage.defs.add("light", makeDef(18, 1, 0));
        const std::uint16_t tex = params.storage.defs.add("tex", makeDef(12, 1, 8));
        intrusive_ptr<CLight> l(new CLight());
        CHECK(params.setPointer(light, 0u, l));
        intrusive_ptr<CLight> back;
        CHECK(params.getPointer(light, 0u, back) && back.get() == l.get());
        intrusive_ptr<ITexture> wrong;
        CHECK(!params.getPointer(tex, 0u, wrong) || wrong.get() == nullptr);
        intrusive_ptr<CLight> wrongLight;
        CHECK(!params.getPointer(tex, 0u, wrongLight));  // code 12 is not a light
    }
    // setParameterElement on the global manager: int and byte components, and the code gate.
    {
        GlobalMaterialParameters params;
        params.storage.values.assign(64, 0);
        const std::uint16_t vec = params.storage.defs.add("vec", makeDef(4, 2, 0));  // vec4 int
        CHECK(params.setParameterElement(vec, 1u, std::uint8_t{3}, 9));  // element 1, component 3
        // Stored at 4 * (index + component) = 16 bytes into the buffer, which is the first int of element 1.
        glitch::core::vector4d<int> v{};
        CHECK(params.getParameter<glitch::core::vector4d<int>>(vec, 1u, v) && v.X == 9);
        const std::uint16_t fl = params.storage.defs.add("fl", makeDef(5, 1, 32));  // float
        CHECK(!params.setParameterElement(fl, 0u, std::uint8_t{1}, 2));  // component past one float
        CHECK(!params.setParameterElement(vec, 0u, std::uint8_t{0}, 1.0f));  // base type float vs int
    }
    // setParameterAt<SColorf> on CMaterial marks the dirty range only when the colour changes.
    {
        MaterialParameters params;
        glitch::video::SColorf dst{0.f, 0.f, 0.f, 0.f};
        glitch::video::SColor same{};  // all bytes zero: equal to dst, no dirty mark
        params.storage.dirtyFirst = 0;
        params.storage.dirtyLast = 0;
        setParameterAt(params.storage, &dst, same);
        CHECK(params.storage.dirtyFirst == 0 && params.storage.dirtyLast == 0);
        glitch::video::SColor red{};
        const_cast<std::uint8_t*>(reinterpret_cast<const std::uint8_t*>(&red))[0] = 255;
        setParameterAt(params.storage, &dst, red);
        CHECK(params.storage.dirtyFirst == -1 && params.storage.dirtyLast == -1);
        CHECK(dst.r == 1.0f);
    }
    if (failures == 0) std::printf("material_pointer_element_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
