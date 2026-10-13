// Track B batch v1-vendor-gameswf-gameswf_filter_engine-9ad474-0 (owner gameswf::filter_engine).
// Gameloft addition, not in gameswf r1714.
//
// The passes follow the Hex-Rays pseudocode and were checked against the ARM assembly: float
// arithmetic is single precision (__aeabi_fmul, __aeabi_fdiv, ...), float-to-integer conversions
// saturate as the VFP helpers do (f2iz, f2uiz below), and the passes take no this pointer.

#include "vendor-gameswf/filter_engine.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <utility>

#include "vendor-gameswf/filter.hpp"

namespace gameswf {
namespace {

// __aeabi_f2iz: truncation that saturates; NaN gives 0.
int f2iz(float v) {
    if (v != v) return 0;
    if (v >= 2147483648.0f) return std::numeric_limits<int>::max();
    if (v <= -2147483648.0f) return std::numeric_limits<int>::min();
    return static_cast<int>(v);
}

// __aeabi_f2uiz: truncation to unsigned that saturates; negative and NaN give 0.
std::uint32_t f2uiz(float v) {
    if (!(v > 0.0f)) return 0;
    if (v >= 4294967296.0f) return 0xFFFFFFFFu;
    return static_cast<std::uint32_t>(v);
}

// Byte store of a float sum: the unsigned cast, then the low byte.
std::uint8_t store_sum(float v) {
    return static_cast<std::uint8_t>(f2uiz(v));
}

// blend's byte store: a value above 254 is written as 255.
std::uint8_t saturate_byte(int v) {
    return v > 254 ? std::uint8_t{255} : static_cast<std::uint8_t>(v);
}

}  // namespace

// 0x756d88 _ZN7gameswf13filter_engine5blendERNS0_6rasterES2_
// Composites src over dst. Source rows are packed at four bytes a pixel. The x and y offsets of
// either raster are not applied (the binary starts from data).
void filter_engine::blend(raster& src, raster& dst) {
    const int height = src.height;  // +0x10
    if (height <= 0) return;
    const int width = src.width;  // +0x0c
    std::uint8_t* s_row = src.data;
    std::uint8_t* d_row = dst.data;
    for (int row = 0; row < height; ++row) {
        std::uint8_t* s = s_row;
        std::uint8_t* d = d_row;
        for (int col = 0; col < width; ++col, s += 4, d += 4) {
            const int alpha = s[3];
            if (alpha == 0) continue;
            const int inv = 255 - alpha;
            d[0] = saturate_byte(d[0] * inv / 255 + s[0]);
            d[1] = saturate_byte(d[1] * inv / 255 + s[1]);
            d[2] = saturate_byte(d[2] * inv / 255 + s[2]);
            d[3] = saturate_byte(d[3] * inv / 255 + alpha);
        }
        s_row += 4 * static_cast<std::ptrdiff_t>(width);
        d_row += dst.stride;
    }
}

// 0x756f18 _ZN7gameswf13filter_engine10apply_glowERNS0_6paramsE
// The binary is a bare return (asm: bx lr). The glow pass is not in this build.
void filter_engine::apply_glow(params&) {}

// 0x7572a0 _ZN7gameswf13filter_engine17read_frame_bufferEiiii
// PARTIAL. The binary grows m_frame_pixels to 4 * width * height bytes, reads the frame through
// the render handler (vtable +0x3c) and un-premultiplies each pixel with a sqrt table while it
// swaps the first and third bytes. The render handler and the texture lock calls are not declared
// yet, so the body is stubbed. The pixel loop in the binary also stops one pixel early (its
// do-while tests count != 1).
void filter_engine::read_frame_buffer(int, int, int, int) {}

// 0x7575b0 _ZN7gameswf13filter_engine7prepareERNS0_6paramsE
// Sets the destination rectangle (offset and size) from the source rectangle and the filter.
void filter_engine::prepare(params& p) {
    const filter& e = *p.filter;
    int w = p.src.width;
    const int h = p.src.height;
    p.dst.x = 0;
    p.dst.y = 0;
    p.dst.width = w;
    p.dst.height = h;
    if (e.type != 0) {
        if (e.type == 2) {  // glow: two extra pixels on each axis
            w += 2;
            p.dst.width = w;
            p.dst.height = h + 2;
        }
    } else {  // drop shadow: offset by distance along the angle, rounded to the nearest pixel
        const float shift_x = std::cos(e.angle) * e.distance;
        const float shift_y = std::sin(e.angle) * e.distance;
        const int dx = f2iz(shift_x + 0.5f);
        const int dy = f2iz(shift_y + 0.5f);
        p.dst.x = dx;
        w += std::abs(dx);
        p.dst.height = std::abs(dy) + h;
        p.dst.y = dy;
        p.dst.width = w;
    }
    p.dst.width = w + f2iz(e.blur_x + 0.5f);
    p.dst.height += f2iz(e.blur_y + 0.5f);
    p.dst.x -= f2iz(e.blur_x * 0.5f);
    p.dst.y -= f2iz(e.blur_y * 0.5f);
}

// 0x7576d4 _ZN7gameswf13filter_engine12apply_shadowERNS0_6paramsE
// Writes the shadow colour into dst where src is opaque enough, offset by (dx, dy). When the
// filter's flag 0x20 is set, src is then composited over the shadow.
void filter_engine::apply_shadow(params& p) {
    const filter& e = *p.filter;
    const float cos_a = std::cos(e.angle);
    const float sin_a = std::sin(e.angle);
    const int dx = f2iz(e.distance * cos_a);
    const int dy = f2iz(e.distance * sin_a);
    raster& src = p.src;
    raster& dst = p.dst;
    if (src.height > 0) {
        std::uint8_t* s_row = src.data + (src.x + src.y * src.stride);
        std::uint8_t* d_row = dst.data + (dst.x + dst.y * dst.stride + 4 * dx + dst.stride * dy);
        for (int row = 0; row < src.height; ++row) {
            std::uint8_t* s = s_row;
            std::uint8_t* d = d_row;
            for (int col = 0; col < src.width; ++col, s += 4, d += 4) {
                const int alpha = s[3];
                if (alpha == 0) continue;
                d[0] = e.color[0];
                d[1] = e.color[1];
                d[2] = e.color[2];
                // (colour alpha * source alpha) / 255, with the binary's reciprocal 0x80808081.
                const std::int64_t product = 2155905153LL * e.color[3] * alpha;
                const int out_alpha = static_cast<int>(static_cast<std::uint64_t>(product) >> 32) >> 7;
                d[3] = static_cast<std::uint8_t>(out_alpha);
            }
            s_row += src.stride;
            d_row += dst.stride;
        }
    }
    if ((e.flags & 0x20) != 0) blend(src, dst);
}

// 0x75792c _ZN7gameswf13filter_engine12apply_blur_vERNS0_6paramsE
// Vertical gaussian pass from src into dst, one byte column at a time. For bpp 4 the binary
// addresses the taps with bpp * start and steps 4 bytes (asm 0x757b38, MLA with the bpp slot),
// and it advances columns by one byte. That is reproduced as found, and it is probably a Gameloft
// defect: the four-channel vertical blur does not look like a vertical pass. The one-channel path
// steps the taps by the stride, as a vertical pass should.
void filter_engine::apply_blur_v(params& p) {
    const filter& e = *p.filter;
    const float blur = e.blur_y;
    const int radius = f2iz(blur);
    const int bpp = p.src.bpp;
    std::array<float, 257> kernel{};
    gaussian_kernel(kernel.data(), radius);
    const int src_stride = p.src.stride;
    if (f2iz(blur + blur) <= 0) return;
    const int width = p.src.width;
    if (width <= 0) return;
    const int height = p.src.height;
    const int dst_stride = p.dst.stride;
    std::uint8_t* src_col0 = p.src.data + (p.src.x + p.src.y * src_stride);
    std::uint8_t* dst_col0 = p.dst.data + (p.dst.x + p.dst.y * dst_stride);
    for (int col = 0; col < width; ++col) {
        std::uint8_t* s_col = src_col0 + col;
        std::uint8_t* d_col = dst_col0 + col;
        for (int row = 0; row < height; ++row) {
            const int start = (row - radius >= 0) ? -radius : -row;
            const int end = (height > row + radius) ? radius : (height - 1 - row);
            const float* k = kernel.data() + (radius + start);
            if (bpp == 4) {
                float c0 = 0.0f;
                float c1 = 0.0f;
                float c2 = 0.0f;
                float ca = 0.0f;
                if (start <= end) {
                    const std::uint8_t* t0 = s_col + bpp * start;
                    for (int j = 0; j <= end - start; ++j) {
                        const std::uint8_t* t = t0 + 4 * j;
                        const float alpha = static_cast<float>(t[3]);
                        const float w = (k[j] * alpha) / 255.0f;
                        c0 += static_cast<float>(t[0]) * w;
                        c1 += static_cast<float>(t[1]) * w;
                        c2 += static_cast<float>(t[2]) * w;
                        ca += k[j] * alpha;
                    }
                }
                d_col[0] = store_sum(c0);
                d_col[1] = store_sum(c1);
                d_col[2] = store_sum(c2);
                d_col[3] = store_sum(ca);
            } else {
                float acc = 0.0f;
                if (start <= end) {
                    const std::uint8_t* px = s_col + static_cast<std::ptrdiff_t>(src_stride) * start;
                    for (int j = 0; j <= end - start; ++j, px += src_stride) {
                        acc += static_cast<float>(*px) * k[j];
                    }
                }
                d_col[0] = store_sum(acc);
            }
            s_col += src_stride;
            d_col += dst_stride;
        }
    }
}

// 0x757cd8 _ZN7gameswf13filter_engine12apply_blur_hERNS0_6paramsE
// Horizontal gaussian pass from src into dst. The window of column i is clamped to the row.
void filter_engine::apply_blur_h(params& p) {
    const filter& e = *p.filter;
    const float blur = e.blur_x;
    const int radius = f2iz(blur);
    const int bpp = p.src.bpp;
    std::array<float, 257> kernel{};
    gaussian_kernel(kernel.data(), radius);
    if (f2iz(blur + blur) <= 0) return;
    const int width = p.src.width;
    const int height = p.src.height;
    if (width <= 0 || height <= 0) return;
    const int src_stride = p.src.stride;
    const int dst_stride = p.dst.stride;
    const int column_step = (bpp == 4) ? 4 : 1;
    std::uint8_t* s_row = p.src.data + (p.src.x + p.src.y * src_stride);
    std::uint8_t* d_row = p.dst.data + (p.dst.x + p.dst.y * dst_stride);
    for (int row = 0; row < height; ++row) {
        std::uint8_t* s = s_row;
        std::uint8_t* d = d_row;
        for (int col = 0; col < width; ++col, s += column_step, d += column_step) {
            const int start = (col - radius >= 0) ? -radius : -col;
            const int end = (width > col + radius) ? radius : (width - 1 - col);
            const float* k = kernel.data() + (radius + start);
            if (bpp == 4) {
                float c0 = 0.0f;
                float c1 = 0.0f;
                float c2 = 0.0f;
                float ca = 0.0f;
                if (start <= end) {
                    const std::uint8_t* t0 = s + 4 * start;
                    for (int j = 0; j <= end - start; ++j) {
                        const std::uint8_t* t = t0 + 4 * j;
                        const float alpha = static_cast<float>(t[3]);
                        const float w = (k[j] * alpha) / 255.0f;
                        c0 += static_cast<float>(t[0]) * w;
                        c1 += static_cast<float>(t[1]) * w;
                        c2 += static_cast<float>(t[2]) * w;
                        ca += k[j] * alpha;
                    }
                }
                d[0] = store_sum(c0);
                d[1] = store_sum(c1);
                d[2] = store_sum(c2);
                d[3] = store_sum(ca);
            } else {
                float acc = 0.0f;
                if (start <= end) {
                    const std::uint8_t* px = s + start;
                    for (int j = 0; j <= end - start; ++j) {
                        acc += static_cast<float>(px[j]) * k[j];
                    }
                }
                d[0] = store_sum(acc);
            }
        }
        s_row += src_stride;
        d_row += dst_stride;
    }
}

// 0x75807c _ZN7gameswf13filter_engine4copyERNS0_6rasterES2_
// Copies width * bpp bytes per row from src to dst.
void filter_engine::copy(raster& src, raster& dst) {
    if (src.height <= 0) return;
    const std::size_t row_bytes = static_cast<std::size_t>(src.width * src.bpp);
    std::uint8_t* s = src.data + (src.x + src.y * src.stride);
    std::uint8_t* d = dst.data + (dst.x + dst.y * dst.stride);
    for (int row = 0; row < src.height; ++row) {
        std::memcpy(d, s, row_bytes);
        s += src.stride;
        d += dst.stride;
    }
}

// 0x7580f8 _ZN7gameswf13filter_engine5clearERNS0_6rasterE
// Zeroes height rows of stride bytes each, starting at data. LC_API_MEMSET_0 forwards to memset.
void filter_engine::clear(raster& r) {
    if (r.height <= 0) return;
    std::uint8_t* p = r.data;
    for (int row = 0; row < r.height; ++row) {
        std::memset(p, 0, static_cast<std::size_t>(r.stride));
        p += r.stride;
    }
}

// 0x758140 _ZN7gameswf13filter_engine5applyERNS0_6paramsE
// Runs the filter. Blur (type 1) runs the passes that have a positive radius and swaps the two
// rasters after each one, so the result is always in dst. Glow (type 2) runs the empty glow pass,
// swaps, and clears dst. Type 0 copies src to dst. Other types do nothing.
void filter_engine::apply(params& p) {
    switch (p.filter->type) {
    case 1:
        if (p.filter->blur_x > 0.0f) {
            apply_blur_h(p);
            std::swap(p.src, p.dst);
        }
        if (p.filter->blur_y > 0.0f) {
            apply_blur_v(p);
            std::swap(p.src, p.dst);
        }
        std::swap(p.src, p.dst);
        break;
    case 2:
        apply_glow(p);
        std::swap(p.src, p.dst);
        clear(p.dst);
        break;
    case 0:
        copy(p.src, p.dst);
        break;
    default:
        break;
    }
}

// 0x75830c _ZN7gameswf13filter_engine27collect_filtered_charactersEPNS_9characterE
// PARTIAL. The binary appends the character to m_filtered_characters when it has a filter, then
// recurses into its children (its children array and count sit in the character object). The
// character layout (flag +0x9b, world colour transform, child list +0xa8 and count +0xac) is not
// declared yet, so the body is stubbed.
void filter_engine::collect_filtered_characters(character*) {}

// 0x758434 _ZN7gameswf13filter_engine14display_cachedEPNS_9characterE
// PARTIAL. The binary looks the character up in the filter cache hash (filter_cache_infos is not
// declared), draws the cached texture region through the render handler and returns 1, or returns
// 0 when the character is the current one or has no cache entry. Stubbed; returns false.
bool filter_engine::display_cached(character*) {
    return false;
}

// 0x7590a4 _ZN7gameswf13filter_engineD1Ev
// PARTIAL. The binary clears the filter cache hash, zeroes the array tails and reserves 0 bytes in
// its two arrays, then runs the texture_cache destructor. The base and members are not declared
// yet (see filter_engine.hpp), so the body is stubbed.
filter_engine::~filter_engine() {}

// 0x757500 _ZN7gameswf13filter_engineC1Ev
// PARTIAL. The binary runs texture_cache(256, 256, 4, false), zeroes its members, and then calls
// virtual slot +0x08 of the texture object (the bitmap_info vtable). That slot's meaning is not
// identified, so it is not called. The base and members are not declared yet, so the body is
// stubbed. The create/close test defines its own constructor stub for this symbol.
filter_engine::filter_engine() {}

// 0x75918c _ZN7gameswf13filter_engine3runEPNS_4rootE
// PARTIAL. The binary collects the filtered characters of the root's display list, rebuilds the
// filter cache hash, renders each filtered character into its cache region (with the frame buffer
// read by read_frame_buffer), and then draws the cached results. The hash, character, root and
// render handler layouts are not declared, so the body is stubbed.
void filter_engine::run(root*) {}

}  // namespace gameswf
