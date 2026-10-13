// Test for batch v1-vendor-gameswf-pack21-d2e85b-0: the parts of the batch that run without the engine.
// Checked: TransformState's identity block, fill_style::set_bitmap (colour bytes, flag, return value), the
// array<array<vector2d<float>>>::push_back<> copy, and the gameswf::sqrt_table values read from the binary.
// Not checked: add_glyph_region, get_glyph_region, the face and glyph cache destructors and the as_* bodies. They
// call FreeType, the texture cache and the filter engine, which are not in this test.
//
// The batch source references engine symbols (FreeType, texture_cache, filter_engine, ref_counted, membuf,
// cxform::clamp, cxform and matrix constructors). The stubs below define them for the link. None of them is called
// by a check in this file except cxform::clamp, which is a no-op here, so every checked value is the batch's own.

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "base/container.h"
#include "base/membuf.h"
#include "gameswf/gameswf.h"
#include "vendor-gameswf/filter_engine.hpp"
#include "vendor-gameswf/freetype_subset.hpp"
#include "vendor-gameswf/glyph_texture_cache.hpp"
#include "vendor-gameswf/ref_counted.hpp"
#include "vendor-gameswf/render_handler_glitch.hpp"

// membuf (global in the upstream base) is released by the memory face's destructor, not by any check here.
membuf::~membuf() {}

// ---- Stubs for engine symbols referenced by the batch source (not called by the checks) ----------------------------

extern "C" {
FT_Error FT_Done_Face(FT_Face) { return 0; }
void FT_Bitmap_New(FT_Bitmap*) {}
FT_Error FT_Bitmap_Convert(FT_Library, const FT_Bitmap*, FT_Bitmap*, FT_Int) { return 0; }
FT_Error FT_Bitmap_Done(FT_Library, FT_Bitmap*) { return 0; }
FT_Error FT_Set_Pixel_Sizes(FT_Face, FT_UInt, FT_UInt) { return 0; }
FT_Error FT_Load_Char(FT_Face, FT_ULong, FT_Int32) { return 0; }
}

namespace gameswf {

void free_internal(void*, unsigned int) {}
render_handler* s_render_handler = nullptr;

ref_counted::ref_counted() {}
ref_counted::~ref_counted() {}
void ref_counted::drop_ref() {}

void filter_engine::copy(raster&, raster&) {}
void filter_engine::apply(params&) {}

void cxform::clamp() {}
cxform::cxform() {}

// texture_cache members used by add_glyph_region and get_glyph_region (not called by the checks).
texture_cache::~texture_cache() {}
void* texture_cache::lock() { return nullptr; }
int texture_cache::get_region_size_requirement(int&, int&) { return 0; }
texture_cache::region* texture_cache::find_used_region(int, int) { return nullptr; }
texture_cache::region* texture_cache::find_available_region(int, int) { return nullptr; }
void texture_cache::get_region_bounds(const texture_cache::region*, gameswf::rect&) const {}
void texture_cache::reset() {}

matrix::matrix() {}

// Defined by the batch source (gameswf::sqrt_table, 256 bytes).
extern const std::uint8_t sqrt_table[256];

}  // namespace gameswf

// tu_string members that the string helpers reach (global type; not called by the checks).
void tu_string::resize(int) {}
int tu_string::utf8_char_count(const char*, int) { return 0; }

// The used-region hash's rehash (defined by vendor-gameswf/src/v1-vendor-gameswf-pack19-4d4603-0.cpp in the build). The
// checks never insert a glyph, so this body is not reached.
template <>
void ::hash<gameswf::texture_cache::key, gameswf::texture_cache::region*,
            ::fixed_size_hash<gameswf::texture_cache::key> >::set_raw_capacity(int) {
    std::abort();
}

int main() {
    int failures = 0;
    auto check = [&failures](bool ok, const char* what) {
        if (!ok) {
            std::printf("FAIL: %s\n", what);
            ++failures;
        }
    };

    // TransformState: identity in the first block, flag 1.
    {
        render_handler_glitch::TransformState ts;
        bool identity = true;
        for (int i = 0; i < 16; ++i) {
            const float want = (i == 0 || i == 5 || i == 10 || i == 15) ? 1.0f : 0.0f;
            if (ts.m_elems[i] != want) identity = false;
        }
        check(identity, "TransformState: m_elems is the identity matrix");
        check(ts.unk_40 == 1, "TransformState: flag at +0x40 is 1");
    }

    // fill_style::set_bitmap: fill type from the wrap mode, bitmap pointer, colour bytes and flag.
    {
        gameswf::matrix m;
        gameswf::cxform cx;
        cx.m_[0][0] = 0.5f;  cx.m_[0][1] = 0.0f;
        cx.m_[1][0] = 1.0f;  cx.m_[1][1] = 0.0f;
        cx.m_[2][0] = 0.25f; cx.m_[2][1] = 0.0f;
        cx.m_[3][0] = 0.0f;  cx.m_[3][1] = 0.0f;
        int dummy_bitmap = 0;
        auto* bitmap = reinterpret_cast<gameswf::bitmap_info*>(&dummy_bitmap);

        render_handler_glitch::fill_style fs;
        const bool flag = fs.set_bitmap(bitmap, m, gameswf::render_handler::WRAP_CLAMP, cx);
        check(fs.unk_00 == 3, "set_bitmap: clamp wrap gives fill type 3");
        check(fs.source_bitmap == bitmap, "set_bitmap: bitmap pointer is stored");
        check(fs.color[0] == 127, "set_bitmap: red byte is 0.5 * 255 truncated");
        check(fs.color[1] == 255, "set_bitmap: green byte is 1.0 * 255");
        check(fs.color[2] == 63, "set_bitmap: blue byte is 0.25 * 255 truncated");
        check(fs.color[3] == 0, "set_bitmap: alpha byte is 0");
        check(flag == false, "set_bitmap: no additive term above 1 gives a false return");
        check(fs.unk_44 == 0, "set_bitmap: flag byte is 0");

        cx.m_[2][1] = 1.5f;  // additive blue term above 1
        const bool flag2 = fs.set_bitmap(bitmap, m, gameswf::render_handler::WRAP_REPEAT, cx);
        check(fs.unk_00 == 2, "set_bitmap: repeat wrap gives fill type 2");
        check(flag2 == true, "set_bitmap: additive term above 1 gives a true return");
        check(fs.unk_44 == 1, "set_bitmap: flag byte is 1");
    }

    // array<array<vector2d<float>>>::push_back<>: appends a copy of the inner array.
    {
        using Inner = array<glitch::core::vector2d<float>>;
        array<Inner> outer;
        Inner inner;
        inner.resize(3);
        for (int i = 0; i < 3; ++i) {
            inner[i].X = static_cast<float>(i);
            inner[i].Y = static_cast<float>(10 + i);
        }
        outer.push_back<Inner>(inner);
        check(outer.size() == 1, "push_back: outer size is 1");
        check(outer[0].size() == 3, "push_back: copied inner size is 3");
        bool same = outer[0].size() == 3;
        for (int i = 0; same && i < 3; ++i) {
            same = outer[0][i].X == static_cast<float>(i) && outer[0][i].Y == static_cast<float>(10 + i);
        }
        check(same, "push_back: copied points match");
        // A second append keeps the first element.
        outer.push_back<Inner>(inner);
        check(outer.size() == 2 && outer[0].size() == 3 && outer[1].size() == 3, "push_back: second append");
    }

    // gameswf::sqrt_table: values read from the binary (0x99db58).
    check(gameswf::sqrt_table[0] == 0x00, "sqrt_table[0]");
    check(gameswf::sqrt_table[1] == 0x0f, "sqrt_table[1]");
    check(gameswf::sqrt_table[128] == 0xb4, "sqrt_table[128]");
    check(gameswf::sqrt_table[240] == 0xf7, "sqrt_table[240]");
    check(gameswf::sqrt_table[254] == 0xfe, "sqrt_table[254]");
    check(gameswf::sqrt_table[255] == 0xff, "sqrt_table[255]");

    if (failures == 0) {
        std::printf("pack21 test: all checks passed\n");
    }
    return failures == 0 ? 0 : 1;
}
