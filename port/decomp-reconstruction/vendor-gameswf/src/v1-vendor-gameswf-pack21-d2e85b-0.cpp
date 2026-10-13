// Batch v1-vendor-gameswf-pack21-d2e85b-0 (part 1 of 1). Gameloft code in libDungeonHunter2.so, rebuilt from the
// Hex-Rays pseudocode. Status of each function: vendor-gameswf/ledger.d/v1-vendor-gameswf-pack21-d2e85b-0.csv.
//
// Covers: gameswf::face_entity (constructors and destructor), gameswf::glyph_texture_cache (destructor, add and
// get glyph region), gameswf::as_s_function (get_member, destructors), gameswf::as_c_function (constructors,
// destructor), render_handler_glitch::fill_style::set_bitmap, render_handler_glitch::TransformState::TransformState,
// and the array<array<vector2d<float>>>::push_back<> instantiation. Declarations: glyph_provider.hpp,
// glyph_texture_cache.hpp, render_handler_glitch.hpp, upstream gameswf_function.h (DH2 additions marked).

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>

#include "base/membuf.h"
#include "vendor-gameswf/filter.hpp"
#include "vendor-gameswf/filter_engine.hpp"
#include "vendor-gameswf/freetype_subset.hpp"
#include "vendor-gameswf/gameswf_root.hpp"
#include "vendor-gameswf/glyph_provider.hpp"
#include "vendor-gameswf/glyph_texture_cache.hpp"
#include "vendor-gameswf/ref_counted.hpp"
#include "vendor-gameswf/render_handler_glitch.hpp"

// The as_object / as_function headers do not compile in this tree (gameswf_object.h, see vendor-gameswf/as_function.hpp).
// The as_c_function and as_s_function bodies below are compiled only when DH2_AS_OBJECT_HEADER_OK is set.
#if defined(DH2_AS_OBJECT_HEADER_OK)
#include "gameswf/gameswf_function.h"
#endif

namespace gameswf {

// gameswf::sqrt_table: 256 bytes of DH2 data in .data (symbol _ZN7gameswf10sqrt_tableE, 0x99db58). Read from the
// binary; used by glyph_texture_cache::add_glyph_region for the falloff of the glyph outline.
extern const std::uint8_t sqrt_table[256] = {
    0x00, 0x0f, 0x16, 0x1b, 0x1f, 0x23, 0x27, 0x2a, 0x2d, 0x2f, 0x32, 0x34, 0x37, 0x39, 0x3b, 0x3d,
    0x3f, 0x41, 0x43, 0x45, 0x47, 0x49, 0x4a, 0x4c, 0x4e, 0x4f, 0x51, 0x52, 0x54, 0x55, 0x57, 0x58,
    0x5a, 0x5b, 0x5d, 0x5e, 0x5f, 0x61, 0x62, 0x63, 0x64, 0x66, 0x67, 0x68, 0x69, 0x6b, 0x6c, 0x6d,
    0x6e, 0x6f, 0x70, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e,
    0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8d,
    0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x99, 0x9a, 0x9b,
    0x9c, 0x9d, 0x9e, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa2, 0xa3, 0xa4, 0xa5, 0xa5, 0xa6, 0xa7, 0xa8,
    0xa8, 0xa9, 0xaa, 0xab, 0xab, 0xac, 0xad, 0xae, 0xae, 0xaf, 0xb0, 0xb1, 0xb1, 0xb2, 0xb3, 0xb3,
    0xb4, 0xb5, 0xb6, 0xb6, 0xb7, 0xb8, 0xb8, 0xb9, 0xba, 0xba, 0xbb, 0xbc, 0xbc, 0xbd, 0xbe, 0xbe,
    0xbf, 0xc0, 0xc0, 0xc1, 0xc2, 0xc2, 0xc3, 0xc4, 0xc4, 0xc5, 0xc6, 0xc6, 0xc7, 0xc8, 0xc8, 0xc9,
    0xc9, 0xca, 0xcb, 0xcb, 0xcc, 0xcd, 0xcd, 0xce, 0xce, 0xcf, 0xd0, 0xd0, 0xd1, 0xd2, 0xd2, 0xd3,
    0xd3, 0xd4, 0xd5, 0xd5, 0xd6, 0xd6, 0xd7, 0xd8, 0xd8, 0xd9, 0xd9, 0xda, 0xda, 0xdb, 0xdc, 0xdc,
    0xdd, 0xdd, 0xde, 0xde, 0xdf, 0xe0, 0xe0, 0xe1, 0xe1, 0xe2, 0xe2, 0xe3, 0xe4, 0xe4, 0xe5, 0xe5,
    0xe6, 0xe6, 0xe7, 0xe7, 0xe8, 0xe9, 0xe9, 0xea, 0xea, 0xeb, 0xeb, 0xec, 0xec, 0xed, 0xed, 0xee,
    0xee, 0xef, 0xf0, 0xf0, 0xf1, 0xf1, 0xf2, 0xf2, 0xf3, 0xf3, 0xf4, 0xf4, 0xf5, 0xf5, 0xf6, 0xf6,
    0xf7, 0xf7, 0xf8, 0xf8, 0xf9, 0xf9, 0xfa, 0xfa, 0xfb, 0xfb, 0xfc, 0xfc, 0xfd, 0xfd, 0xfe, 0xff,
};

namespace {

// The used-region key of a glyph: face pointer, code and font size, and the packed filter bytes (texture_cache::key).
// The binary builds the same 16 bytes in add_glyph_region and in get_glyph_region.
texture_cache::key glyph_region_key(void* face, std::uint16_t code, int font_size, const glyph_texture_cache::filter_info& f) {
    texture_cache::key k;
    k.unk_0 = reinterpret_cast<std::uintptr_t>(face);
    k.unk_4 = static_cast<std::int32_t>(static_cast<std::uint32_t>(code) |
                                        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(font_size)) << 16));
    k.unk_8 = static_cast<std::int32_t>((static_cast<std::uint32_t>(f.offset_y) << 8) |
                                        (static_cast<std::uint32_t>(f.offset_x) << 16) |
                                        static_cast<std::uint32_t>(f.blur));
    k.unk_c = 0;
    return k;
}

}  // namespace

// 0x7d0cf8 gameswf::face_entity::face_entity(FT_FaceRec_*, gameswf::membuf*, gameswf::tu_string&)
// Memory face: the membuf (the font file bytes) is owned by the face and freed by the destructor.
face_entity::face_entity(FT_Face face, membuf* data, const ::tu_string& name)
    : m_data(data), m_face(face) {
    m_name = name;
}

// 0x7d0d7c gameswf::face_entity::face_entity(FT_FaceRec_*, gameswf::tu_string&)
// Face read from a file: no membuf.
face_entity::face_entity(FT_Face face, const ::tu_string& name) : m_face(face) {
    m_name = name;
}

// 0x7d1c04 gameswf::face_entity::~face_entity()  [D1]
// Frees the FreeType face, then each glyph (its bitmap reference is dropped, then the glyph storage is freed), then
// the memory face's membuf, then the glyph hash. The name's heap text is released by the tu_string member. The binary
// walks the hash's table slots directly; the iterator visits the same live entries (not empty and not a tombstone).
// The binary's check of "this" against -40 is always true and is not reproduced.
face_entity::~face_entity() {
    FT_Done_Face(m_face);
    for (auto it = m_glyphs.begin(); it != m_glyphs.end(); ++it) {
        glyph_entity* glyph = it->second;
        if (glyph != nullptr) {
            if (bitmap_info* bitmap = glyph->m_bi.get_ptr()) {
                bitmap->drop_ref();
            }
            free_internal(glyph, 0);
        }
    }
    if (m_data != nullptr) {
        m_data->~membuf();
        free_internal(m_data, 0);
    }
    m_glyphs.clear();
}

// 0x7d1a28 gameswf::glyph_texture_cache::~glyph_texture_cache()  [D1]
// PARTIAL: the binary first runs a loop that zeroes bytes from a negative m_pixels size up to index 0 (a write
// below the buffer, reachable only for a negative size, which a valid array never has). That branch is not
// reproduced. The size is set to 0 and the buffer released, as in the binary. The texture_cache base destructor runs
// after this body.
glyph_texture_cache::~glyph_texture_cache() {
    m_pixels.resize(0);
}

// 0x7d1da4 gameswf::glyph_texture_cache::add_glyph_region(ushort, void*, int, gameswf::glyph_texture_cache::filter_info&, bool)
//
// Rasterises a glyph into the texture. Steps, as the binary runs them:
//   1. The texture is locked. The FreeType face is set to the font size (scaled by the glyph provider's scale).
//   2. The glyph is loaded and rendered. A mono glyph (pixel_mode 1) is converted to 8-bit gray and its bytes are
//      negated, in a temporary bitmap that is freed at the end.
//   3. The filter bytes are scaled into a blur radius (blur) and two offsets (offset_x, offset_y). The region is the
//      glyph's pitch and rows plus 1, grown by the blur or the offsets. The region is rounded to 16-pixel cells.
//   4. A used or available region is taken; it is stamped with the cache clock and entered in the used-region hash
//      under the key of the glyph and filter.
//   5. Blur case (blur != 0): a falloff mask is built in m_pixels with sqrt_table. Offset case: the glyph is copied
//      at (offset_x, offset_y) into m_pixels, then gameswf::filter_engine::apply blurs it into the second half of
//      m_pixels (offset past the first image).
//   6. The result is copied into the texture: with bpp 1 as the alpha row, with bpp 4 as white rgb and the alpha byte.
// The binary's buffer size in the offset case counts the offset twice (region_w and region_h already include it,
// and the size formula adds it again); that is reproduced as is.
// Early returns (no region, glyph not loaded) leave the temporary bitmap unfreed, as in the binary.
int glyph_texture_cache::add_glyph_region(std::uint16_t code, void* face_arg, int font_size,
                                          const filter_info& filter, bool used_only) {
    face_entity* face = static_cast<face_entity*>(face_arg);
    glyph_provider* owner = m_owner;
    const float scale = owner->m_scale;
    const int filter_blur = filter.blur;
    const int filter_x = filter.offset_x;
    const int filter_y = filter.offset_y;

    unsigned char* const lock_base = static_cast<unsigned char*>(lock());
    FT_Set_Pixel_Sizes(face->m_face, 0,
                       static_cast<FT_UInt>(static_cast<int>(static_cast<float>(font_size) * scale)));
    if (FT_Load_Char(face->m_face, code, 4u) != 0) {
        return 0;
    }

    FT_GlyphSlot glyph = face->m_face->glyph;
    const bool mono = glyph->bitmap.pixel_mode == 1;
    FT_Bitmap converted{};
    FT_Bitmap* bmp = &glyph->bitmap;
    if (mono) {
        FT_Bitmap_New(&converted);
        FT_Bitmap_Convert(owner->m_library, &glyph->bitmap, &converted, 1);
        const int count = converted.width * converted.rows;
        for (int i = 0; i < count; ++i) {
            converted.buffer[i] = static_cast<unsigned char>(-static_cast<int>(converted.buffer[i]));
        }
        bmp = &converted;
    }

    // Filter bytes, scaled. v13 is the blur radius, v14 and v16 the offsets (v16 is the low byte of v15).
    const int v13 = static_cast<std::uint8_t>(static_cast<unsigned int>(static_cast<float>(filter_blur) * scale));
    const int v14 = static_cast<std::uint8_t>(static_cast<unsigned int>(static_cast<float>(filter_x) * scale));
    const unsigned int v15 = static_cast<unsigned int>(static_cast<float>(filter_y) * scale);
    const int v16 = static_cast<std::uint8_t>(v15);

    // Region size: glyph pitch and rows plus 1, grown by the blur (both sides) or by the offsets.
    int region_w = bmp->pitch + 1;
    int region_h = bmp->rows + 1;
    if (v13 != 0) {
        region_w += 2 * v13;
        region_h += 2 * v13;
    } else if ((v16 | v14) != 0) {
        region_w += 2 * v14;
        region_h += 2 * v16;
    }
    texture_cache::get_region_size_requirement(region_w, region_h);

    region* r = used_only ? find_used_region(region_w, region_h) : find_available_region(region_w, region_h);
    if (r == nullptr) {
        return 0;
    }
    r->last_clock = m_clock;
    m_clock += 1;
    m_used[glyph_region_key(face_arg, code, font_size, filter)] = r;

    gameswf::rect bounds;
    get_region_bounds(r, bounds);
    const float y_min = bounds.m_y_min;
    const int bpp = m_bpp;
    const int tex_w = m_texture->get_width();
    unsigned char* const dest = lock_base + static_cast<int>(
        static_cast<float>(static_cast<float>(y_min * static_cast<float>(tex_w)) * static_cast<float>(bpp)) +
        static_cast<float>(static_cast<float>(bpp) * bounds.m_x_min));
    const int stride = bpp * tex_w;

    // Clear the region rows in the texture.
    if (region_h > 0) {
        unsigned char* row = dest;
        for (int i = 0; i < region_h; ++i) {
            std::memset(row, 0, static_cast<std::size_t>(region_w * bpp));
            row += stride;
        }
    }

    // Source of the copy below: the glyph bitmap (or its gray conversion).
    unsigned char* src = bmp->buffer;
    int src_stride = bmp->pitch;
    int src_w = bmp->width;
    int src_h = bmp->rows;

    if (v13 != 0) {
        // Blur case: falloff mask in m_pixels.
        const int area = region_w * region_h;
        m_pixels.resize(area);
        if (area > 0) {
            std::memset(&m_pixels[0], 0, static_cast<std::size_t>(area));
        }
        unsigned char* pix = area > 0 ? &m_pixels[0] : nullptr;
        const int v43 = region_w;
        const float v73 = static_cast<float>(v13) + static_cast<float>(v13);
        const int v81 = v13 + 1;
        const int v82 = 2 * v13 + 1;
        const int rows = bmp->rows;
        for (int v75 = v82, v80 = v81; v75 >= 0; --v75, --v80) {
            const int v77 = v80 < 0 ? -v80 : v80;
            for (int v69 = v82, v71 = v81; v69 >= 0; --v69, --v71) {
                const int v46 = v71 < 0 ? -v71 : v71;
                const float v47 = static_cast<float>(
                    static_cast<float>(static_cast<float>(v73 - static_cast<float>(v46)) - static_cast<float>(v77)) /
                    v73 * 255.0);
                const int v49 = v47 >= 0.0f ? (v47 < 255.0f ? static_cast<int>(v47) : 255) : 0;
                const float v50 = static_cast<float>(
                    static_cast<float>(static_cast<float>(sqrt_table[v49]) / 255.0) * 255.0);
                const int v51 = v50 >= 0.0f ? (v50 < 255.0f ? static_cast<int>(v50) : 255) : 0;
                const float v52 = static_cast<float>(static_cast<float>(sqrt_table[v51]) / 255.0);
                for (int row = 0; row < rows; ++row) {
                    const unsigned char* src_row = src + row * src_stride;
                    unsigned char* dst = pix + v69 + (v75 + row) * v43;
                    for (int i = 0; i < src_w; ++i) {
                        int v57 = static_cast<int>(static_cast<float>(static_cast<float>(src_row[i]) * v52));
                        int v58 = dst[i];
                        if (v57 >= 255) {
                            v57 = 255;
                        }
                        if (v57 >= v58) {
                            v58 = v57;
                        }
                        dst[i] = static_cast<unsigned char>(v58);
                    }
                }
            }
        }
        src = pix;
        src_stride = v43;
        src_w = region_w - 1;
        src_h = region_h - 1;
    } else if ((v16 | v14) != 0) {
        // Offset case: copy the glyph into m_pixels at (v14, v16), then blur it into the second half.
        const int v60 = (region_w + 2 * v14) * (region_h + 2 * v16);
        const int v61 = 2 * v60;
        m_pixels.resize(v61);
        if (v61 > 0) {
            std::memset(&m_pixels[0], 0, static_cast<std::size_t>(v61));
        }
        unsigned char* const pix = &m_pixels[0];

        filter_engine::raster copy_src{bmp->buffer, 0, 0, bmp->width, bmp->rows, bmp->pitch, 1};
        filter_engine::raster copy_dst{pix, v14, v16, region_w - 1, region_h - 1, region_w, 1};
        filter_engine::copy(copy_src, copy_dst);

        // The blur filter: type 1 (blur), radii (v14, v16), and the field the binary writes as 1 at +0x28.
        // Qualified: the function's parameter `filter` (filter_info) hides the type name here.
        gameswf::filter blur;
        blur.type = 1;
        blur.blur_x = static_cast<float>(v14);
        blur.blur_y = static_cast<float>(v16);
        blur.unk_28 = 1;

        filter_engine::params p;
        p.filter = &blur;
        p.src = filter_engine::raster{pix, 0, 0, region_w - 1, region_h - 1, region_w, 1};
        p.dst = filter_engine::raster{pix + v60, 0, 0, region_w - 1, region_h - 1, region_w, 1};
        filter_engine::apply(p);

        src = pix + v60;
        src_stride = region_w;
        src_w = region_w - 1;
        src_h = region_h - 1;
    }

    // Copy the rows into the texture region.
    if (src_h > 0) {
        unsigned char* dst_row = dest;
        for (int j = 0; j != src_h; ++j) {
            unsigned char* d = dst_row;
            if (bpp == 1) {
                if (src_w > 0) {
                    std::memcpy(dst_row, src, static_cast<std::size_t>(src_w));
                }
            } else if (src_w > 0) {
                for (int k = 0; k != src_w; ++k) {
                    d[0] = 0xff;
                    d[1] = 0xff;
                    d[2] = 0xff;
                    d[3] = src[k];
                    d += 4;
                }
            }
            dst_row += stride;
            src += src_stride;
        }
    }

    if (mono) {
        FT_Bitmap_Done(owner->m_library, &converted);
    }
    return 1;
}

// 0x7d266c gameswf::glyph_texture_cache::get_glyph_region(ushort, void*, int, gameswf::glyph_texture_cache::filter_info&, gameswf::rect&)
// PARTIAL: the binary returns a float in r0. On a miss that the add cannot satisfy the value is the region index
// reinterpreted as float bits, and on a hit it is whatever texture_cache::get_region_bounds leaves in r0 (that
// function is void in this tree). Neither is reproduced. This returns true when the region's bounds were written.
// The caller (glyph_provider::get_char_image, 0x7d1614) is not reconstructed, so the return contract is not confirmed.
bool glyph_texture_cache::get_glyph_region(std::uint16_t code, void* face, int font_size,
                                           const filter_info& filter, rect& bounds) {
    const texture_cache::key k = glyph_region_key(face, code, font_size, filter);
    region* r = nullptr;
    if (!m_used.get(k, &r)) {
        if (!add_glyph_region(code, face, font_size, filter, false)) {
            s_render_handler->flush();
            if (!add_glyph_region(code, face, font_size, filter, true)) {
                reset();
                add_glyph_region(code, face, font_size, filter, false);
            }
        }
        r = nullptr;
        if (!m_used.get(k, &r)) {
            return false;
        }
    }
    if (r == nullptr) {
        return false;
    }
    get_region_bounds(r, bounds);
    return true;
}

#if defined(DH2_AS_OBJECT_HEADER_OK)

// 0x7d2878 gameswf::as_s_function::get_member(gameswf::tu_stringi const&, gameswf::as_value*)
// The DH2 builtin table with id 5 (the as_s_function methods, past BUILTIN_COUNT) is tried first; then the
// inherited lookup.
bool as_s_function::get_member(const tu_stringi& name, as_value* val) {
    if (gameswf::get_builtin(static_cast<builtin_object>(5), name, val)) {
        return true;
    }
    return as_function::get_member(name, val);
}

// 0x7d2c8c gameswf::as_s_function::~as_s_function()  [D1]
// 0x7d2d70 gameswf::as_s_function::~as_s_function()  [D2, same body]
// PARTIAL: the binary also releases three DH2 fields that this layout does not declare: a ref_counted at +0x38
// (drop_ref), a counted buffer at +0x3c (counted_buffer::drop_ref) and a counted word buffer at +0x74 (freed at zero).
// It resets the arguments array (+0x60) and the with-stack array (+0x48). Here the upstream members are destroyed
// implicitly, and the DH2 fields are not released.
as_s_function::~as_s_function() {
}

// 0x7d28b4 gameswf::as_c_function::as_c_function(gameswf::player*, void (*)(gameswf::fn_call const&))  [C1]
// 0x7d291c gameswf::as_c_function::as_c_function(gameswf::player*, void (*)(gameswf::fn_call const&))  [C2, same body]
// PARTIAL: the binary stores a new as_object(player) in a smart pointer at +0x38 (the prototype) and the function at
// +0x3c. The upstream constructor calls builtin_member("prototype") instead, and the +0x38 member is not declared.
// Only the function pointer is stored here.
as_c_function::as_c_function(player* p, as_c_function_ptr func) : as_function(p), m_func(func) {
}

// 0x7d2c44 gameswf::as_c_function::~as_c_function()  [D1, the destructor without delete]
// PARTIAL: the binary releases the prototype at +0x38 (drop_ref) before the as_object destructor. The +0x38 member is
// not declared, so the release is not written.
as_c_function::~as_c_function() {
}

#endif  // DH2_AS_OBJECT_HEADER_OK

}  // namespace gameswf

// 0x7d4430 render_handler_glitch::fill_style::set_bitmap(gameswf::bitmap_info*, gameswf::matrix const&, gameswf::render_handler::bitmap_wrap_mode, gameswf::cxform const&)
// Sets the fill side for a bitmap fill: the fill type (3 for a clamped wrap, 2 for repeat), the bitmap pointer, the
// matrix, and a copy of the colour transform, clamped. The four colour bytes are the clamped multipliers (r, g, b, a)
// times 255. The flag is set when any of the four additive terms is above 1. The return value is that flag.
bool render_handler_glitch::fill_style::set_bitmap(gameswf::bitmap_info* bitmap, const gameswf::matrix& m,
                                                   gameswf::render_handler::bitmap_wrap_mode wrap,
                                                   const gameswf::cxform& cx) {
    unk_00 = static_cast<int>(wrap) != 0 ? 3 : 2;
    source_bitmap = bitmap;
    bitmap_matrix = m;
    bitmap_cxform = cx;
    bitmap_cxform.clamp();
    for (int c = 0; c < 4; ++c) {
        color[c] = static_cast<std::uint8_t>(
            static_cast<unsigned int>(static_cast<float>(bitmap_cxform.m_[c][0] * 255.0)));
    }
    const bool over = bitmap_cxform.m_[0][1] > 1.0f || bitmap_cxform.m_[1][1] > 1.0f ||
                      bitmap_cxform.m_[2][1] > 1.0f || bitmap_cxform.m_[3][1] > 1.0f;
    unk_44 = over ? 1 : 0;
    return over;
}

// 0x7d6050 render_handler_glitch::TransformState::TransformState(void)
// PARTIAL: the binary constructor writes three identity blocks, 0x44 bytes apart (at +0x00, +0x44 and +0x88, flags at
// +0x40, +0x84 and +0xc8 set to 1), and zeroes the 16 bytes at +0xcc..+0xdb (object size 0xdc). The TransformState
// declared in render_handler_glitch.hpp is 0x41 bytes (m_elems and unk_40), so only the first block is written here.
// Layout request for the integrator: the object size in the binary and the two extra blocks.
render_handler_glitch::TransformState::TransformState() {
    for (float& e : m_elems) {
        e = 0.0f;
    }
    m_elems[0] = 1.0f;
    m_elems[5] = 1.0f;
    m_elems[10] = 1.0f;
    m_elems[15] = 1.0f;
    unk_40 = 1;
}

// 0x7d7c18 gameswf::array<gameswf::array<glitch::core::vector2d<float> > >::push_back<gameswf::array<glitch::core::vector2d<float> > >(gameswf::array<glitch::core::vector2d<float> > const&)
// Appends a copy of val. The new slot is constructed in the reserved storage, which the binary also does by zeroing it
// and copying the points.
template <>
template <>
void array<array<glitch::core::vector2d<float>>>::push_back<array<glitch::core::vector2d<float>>>(
        const array<glitch::core::vector2d<float>>& val) {
    const int old_size = m_size;
    const int new_size = old_size + 1;
    if (new_size > m_buffer_size) {
        reserve(new_size + (new_size >> 1));
    }
    new (m_buffer + old_size) array<glitch::core::vector2d<float>>(val);
    m_size = new_size;
}
