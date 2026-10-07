#pragma once
#include <cstddef>
#include <cstdint>

namespace dh2::ui {
enum class FontResolveService : std::uint32_t {
    debug_load=1, debug_get_switch=2, language=3,
    rewrite_path=4, open_read=5, close_read=6
};
struct FontResolveInput24 {
    const char* name;
    const char* base_path;
    std::uint32_t bold, italic;
};
struct FontResolveOutput32 {
    char* bytes;
    std::size_t capacity, length;
    std::uint32_t found, reserved;
};
struct FontResolveRequest40 {
    FontResolveService kind;
    std::uint32_t reserved;
    const char* text;
    char* buffer;
    std::size_t capacity;
    std::uintptr_t value;
};
struct FontResolveServices16 {
    void* context;
    // Return0 delivered. language/open return through value; rewrite_path
    // writes a terminated URI to buffer. open value0 is genuine missing.
    int (*invoke)(void*,FontResolveRequest40*);
};
static_assert(sizeof(void*)==8);
static_assert(sizeof(FontResolveInput24)==24);
static_assert(sizeof(FontResolveOutput32)==32);
static_assert(sizeof(FontResolveRequest40)==40);
static_assert(sizeof(FontResolveServices16)==16);
}
// 0 delivered (found may0), -1 malformed/budget, -2 required service failed.
// Caller spans and services remain borrowed/stable throughout synchronous calls.
// This reproduces the no-atlas game resolver, not file-manager implementation.
extern "C" int dh2_swf_font_resolve(dh2::ui::FontResolveOutput32*,
    const dh2::ui::FontResolveInput24*,const dh2::ui::FontResolveServices16*) noexcept;
