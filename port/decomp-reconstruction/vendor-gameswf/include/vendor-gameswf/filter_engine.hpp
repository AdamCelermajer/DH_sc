// gameswf::filter_engine as the DH2 binary has it (Gameloft addition; not in gameswf r1714).
//
// The binary object is 0x68 bytes (create_filter_engine calls operator new(0x68)). Its base is
// texture_cache (vendor-gameswf/texture_cache.hpp, 0x40 bytes in the binary) and its own members
// start at +0x40:
//   +0x40 array<character*>   +0x4c byte   +0x50 array<uint8_t>   +0x5c byte
//   +0x60 hash<character*, filter_cache_infos> (one pointer)   +0x64 character*
// They are not declared here yet. On a 64-bit host the texture_cache base is larger than 0x40 bytes,
// so declaring it would change sizeof(filter_engine) away from 0x68, which the create/close test
// (tests/v1-vendor-gameswf-gameswf-fddef6-0_test.cpp) asserts, and it would break that test's
// constructor stub. The integrator decides the host size, then declares the base and members.
// Until then the object is a 0x68 pad, and the members are not used by any body that is written.
//
// Bodies:
//   - create_filter_engine, close_filter_engine, gaussian_kernel:
//       vendor-gameswf/src/v1-vendor-gameswf-gameswf-fddef6-0.cpp
//   - the static passes, and the partial constructor, destructor and members:
//       vendor-gameswf/src/v1-vendor-gameswf-gameswf_filter_engine-9ad474-0.cpp

#pragma once

#include <cstdint>

namespace gameswf {

class character;
struct filter;
class root;

class filter_engine {
public:
    // A pixel rectangle used by the passes. Seven words in the 32-bit binary (28 bytes).
    struct raster {
        std::uint8_t* data;  // +0x00 pixel buffer
        int x;               // +0x04 added to data without scaling (copy, apply_shadow)
        int y;               // +0x08 row offset, multiplied by stride
        int width;           // +0x0c pixels per row
        int height;          // +0x10 rows
        int stride;          // +0x14 bytes per row
        int bpp;             // +0x18 bytes per pixel
    };

    // Argument of the filter passes. src (+0x04) and dst (+0x20) are the two rasters that apply
    // swaps. Fields after +0x3c are not used by any row of this batch and are not declared.
    struct params {
        // The member is named filter, as the binary's field, so the type is spelled with its namespace
        // (an unqualified filter here would change meaning inside the class).
        gameswf::filter* filter;  // +0x00 filter definition (gameswf/filter.hpp, the 44-byte element)
        raster src;               // +0x04
        raster dst;               // +0x20
    };

    filter_engine();   // 0x757500 (C1). PARTIAL, see the batch .cpp.
    ~filter_engine();  // 0x7590a4 (D1). PARTIAL, see the batch .cpp.

    // gameswf::filter_engine::s_instance. Null until create_filter_engine runs.
    static inline filter_engine* s_instance = nullptr;

    // Passes. The binary takes no this pointer for these.
    static void blend(raster& src, raster& dst);  // 0x756d88
    static void apply_glow(params& p);            // 0x756f18
    static void prepare(params& p);               // 0x7575b0
    static void apply_shadow(params& p);          // 0x7576d4
    static void apply_blur_v(params& p);          // 0x75792c
    static void apply_blur_h(params& p);          // 0x757cd8
    static void copy(raster& src, raster& dst);   // 0x75807c
    static void clear(raster& r);                 // 0x7580f8
    static void apply(params& p);                 // 0x758140

    // Members that need the frame buffer, character, filter cache and render handler layouts. PARTIAL.
    void read_frame_buffer(int x, int y, int width, int height);  // 0x7572a0
    void collect_filtered_characters(character* c);               // 0x75830c
    bool display_cached(character* c);                            // 0x758434
    void run(root* r);                                            // 0x75918c

    // +0x00..+0x67: base texture_cache and members not declared yet (see above). Original size 0x68.
    std::uint8_t pad_0000[0x68];
};

void create_filter_engine();                      // 0x75756c
void close_filter_engine();                       // 0x759140
void gaussian_kernel(float* kernel, int radius);  // 0x757840, kernel holds 2 * radius + 1 weights

}  // namespace gameswf
