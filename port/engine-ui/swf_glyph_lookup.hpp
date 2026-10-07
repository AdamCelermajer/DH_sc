#pragma once
#include <cstdint>
namespace dh2::ui {
struct GlyphLookup48 {
    float advance, bounds[4];
    std::int32_t index;
    std::uintptr_t bitmap, face;
    std::uint32_t bitmap_flag, reserved;
};
struct GlyphLookupFont48 {
    const char* name;
    std::uint32_t code;
    std::int32_t font_size;
    std::uint32_t bold,italic,bitmap_provider,freetype_provider;
    std::uint32_t zone_count,advance_count;
    const float* advances;
};
enum class GlyphLookupService : std::uint32_t {
    bitmap_face=1, bitmap_image=2, freetype_image=3,
    freetype_face=4, embedded_lookup=5
};
struct GlyphLookupRequest48 {
    GlyphLookupService kind;
    std::uint32_t reserved;
    std::uintptr_t identity;
    GlyphLookup48* glyph;
    const GlyphLookupFont48* font;
    std::int32_t index;
    std::uint32_t found;
    std::uint64_t reserved_tail;
};
struct GlyphLookupServices16 {
    void* context;
    // 0 delivered. Face/image identities may0; image callbacks may mutate
    // advance/bounds even on missing. embedded_lookup returns found/index.
    int (*invoke)(void*,GlyphLookupRequest48*);
};
static_assert(sizeof(void*)==8);
static_assert(sizeof(GlyphLookup48)==48);
static_assert(sizeof(GlyphLookupFont48)==48);
static_assert(sizeof(GlyphLookupRequest48)==48);
static_assert(sizeof(GlyphLookupServices16)==16);
}
// 1 glyph found,0 missing,-1 malformed,-2 required service failure.
// Borrowed source providers and live font fields remain caller owned. The
// original smart-pointer/weak-player ownership is an explicit caller boundary.
extern "C" int dh2_swf_glyph_lookup(dh2::ui::GlyphLookup48*,
    const dh2::ui::GlyphLookupFont48*,const dh2::ui::GlyphLookupServices16*) noexcept;
