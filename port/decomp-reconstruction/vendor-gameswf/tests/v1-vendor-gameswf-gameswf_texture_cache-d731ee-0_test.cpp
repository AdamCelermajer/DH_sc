// Test for the texture_cache batch v1-vendor-gameswf-gameswf_texture_cache-d731ee-0.
//
// Covers the region arithmetic and the free and used region lists against a fake texture. The
// out-of-line members of ref_counted and bitmap_info that other batches define are stubbed here.
// Reference counts are never driven to zero, so no fake texture is freed by smart_ptr.
//
// The hash tables need their set_raw_capacity bodies: the used-region table (key) is in pack19 and the
// region-pointer table (verify_integrity) is in pack10, and the test links both. Those two files also
// reference the DH2 allocator, the font entity's destructor and tu_file::read_fully; the test stands in
// for those. Link line (check_tu.sh --test, after this file):
//   batch .cpp, pack10 .cpp, pack19 .cpp, upstream base/container.cpp, base/utf8.cpp, base/membuf.cpp,
//   base/tu_file.cpp, the membuf batch source (membuf::capacity, reserve, resize) and the tu_string batch source.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "base/membuf.h"
#include "base/tu_file.h"
#include "vendor-gameswf/gameswf_alloc.hpp"
#include "vendor-gameswf/bitmap_glyph_provider.hpp"
#include "vendor-gameswf/texture_cache.hpp"

namespace gameswf {

// Stubs for out-of-line members of ref_counted and bitmap_info that are defined in other batches.
ref_counted::ref_counted() {}
ref_counted::~ref_counted() {}
void ref_counted::add_ref() const {}
void ref_counted::drop_ref() {}
weak_proxy* ref_counted::get_weak_proxy() const { return nullptr; }
void ref_counted::detach_weak_proxy() {}

bitmap_info::~bitmap_info() {}
void bitmap_info::unlayout() {}
void bitmap_info::set_writable() {}
int bitmap_info::lock() { return 0; }
void bitmap_info::unlock() {}
void bitmap_info::set_texture(glitch::video::ITexture*) {}
void bitmap_info::set_min_filter(filter_mode) {}
void bitmap_info::set_mag_filter(filter_mode) {}
int bitmap_info::is_layout() const { return 0; }
int bitmap_info::get_internal_width() const { return get_width(); }
int bitmap_info::get_internal_height() const { return get_height(); }

// Host stand-ins for the DH2 allocator, and for the font entity's destructor (the key function that
// emits bitmap_font_entity's vtable). Their bodies are in other batches' sources. The test does not reach
// them.
void* malloc_internal(unsigned int size, swf::MemHint) { return std::malloc(size); }
void free_internal(void* p, unsigned int) { std::free(p); }
void* realloc_internal(void* p, unsigned int new_size, unsigned int, swf::MemHint) {
    return std::realloc(p, new_size);
}
bitmap_font_entity::~bitmap_font_entity() {}

}  // namespace gameswf

// pack15's tu_file::read_fully (a DH2 member of tu_file). pack19 references it. The test does not reach it.
int tu_file::read_fully(membuf*, int) { return 0; }

namespace {

using gameswf::texture_cache;
using region = texture_cache::region;

int g_failures = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (!(cond)) {                                                       \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                                    \
        }                                                                    \
    } while (0)

// Texture with a fixed size. Its lock returns no pointer here (texture_cache::lock is partial).
class fake_texture : public gameswf::bitmap_info {
public:
    fake_texture(int width, int height) : m_width(width), m_height(height) {}

    int lock() override {
        ++m_lock_calls;
        return 0;
    }
    void unlock() override { ++m_unlock_calls; }
    int get_width() const override { return m_width; }
    int get_height() const override { return m_height; }

    int m_width;
    int m_height;
    int m_lock_calls = 0;
    int m_unlock_calls = 0;
};

void test_region_size_requirement() {
    int w = 1;
    int h = 17;
    int r = texture_cache::get_region_size_requirement(w, h);
    CHECK(w == 16);
    CHECK(h == 32);
    CHECK(r == 32);  // height + 15 before rounding

    w = 0;
    h = 0;
    r = texture_cache::get_region_size_requirement(w, h);
    CHECK(w == 16);
    CHECK(h == 16);
    CHECK(r == 15);

    w = 33;
    h = 16;
    r = texture_cache::get_region_size_requirement(w, h);
    CHECK(w == 48);
    CHECK(h == 16);
    CHECK(r == 31);

    w = 16;
    h = 1;
    r = texture_cache::get_region_size_requirement(w, h);
    CHECK(w == 16);
    CHECK(h == 16);
    CHECK(r == 16);
}

void test_region_bounds() {
    texture_cache tc(64, 64, 4, true);
    fake_texture tex(64, 64);
    tc.m_texture = &tex;
    tc.m_regions.resize(16);  // 4 cells per row
    region* r = &tc.m_regions[5];
    r->cells_w = 2;
    r->cells_h = 3;

    gameswf::rect bounds;
    tc.get_region_bounds(r, bounds);
    CHECK(bounds.m_x_min == 16.0f);
    CHECK(bounds.m_x_max == 48.0f);
    CHECK(bounds.m_y_min == 16.0f);
    CHECK(bounds.m_y_max == 64.0f);
}

void test_subdivide() {
    texture_cache tc(64, 64, 4, true);
    fake_texture tex(64, 64);
    tc.m_texture = &tex;
    tc.m_regions.resize(16);
    region* r = &tc.m_regions[0];
    r->cells_w = 4;
    r->cells_h = 4;

    tc.subdivide_region(r, 2, 1);
    CHECK(r->cells_w == 2);
    CHECK(r->cells_h == 1);
    CHECK(tc.m_free.size() == 2);
    CHECK(tc.m_free[0] == &tc.m_regions[2]);  // strip to the right
    CHECK(tc.m_free[1] == &tc.m_regions[4]);  // strip below
    CHECK(tc.m_regions[2].cells_w == 2);
    CHECK(tc.m_regions[2].cells_h == 1);
    CHECK(tc.m_regions[2].last_clock == 0);
    CHECK(tc.m_regions[4].cells_w == 4);
    CHECK(tc.m_regions[4].cells_h == 3);
    CHECK(tc.m_regions[4].last_clock == 0);
}

void test_find_available() {
    texture_cache tc(64, 64, 4, true);
    fake_texture tex(64, 64);
    tc.m_texture = &tex;
    tc.reset();  // one free region of 4 x 4 cells
    CHECK(tc.m_free.size() == 1);

    CHECK(tc.find_available_region(128, 16) == nullptr);  // 8 cells wide, nothing fits
    CHECK(tc.m_free.size() == 1);

    region* got = tc.find_available_region(32, 16);  // 2 x 1 cells
    CHECK(got == &tc.m_regions[0]);
    CHECK(got->cells_w == 2);
    CHECK(got->cells_h == 1);
    CHECK(tc.m_free.size() == 2);  // the strips left over from the cut
    CHECK(tc.m_free[0] == &tc.m_regions[2]);
    CHECK(tc.m_free[1] == &tc.m_regions[4]);
}

void test_find_used() {
    texture_cache tc(64, 64, 4, true);
    fake_texture tex(64, 64);
    tc.m_texture = &tex;
    tc.m_regions.resize(16);

    region* a = &tc.m_regions[0];  // newer
    a->cells_w = 4;
    a->cells_h = 4;
    a->last_clock = 5;
    region* b = &tc.m_regions[2];  // older
    b->cells_w = 2;
    b->cells_h = 2;
    b->last_clock = 3;

    texture_cache::key ka;
    ka.unk_0 = 1;
    texture_cache::key kb;
    kb.unk_0 = 2;
    tc.m_used.add(ka, a);
    tc.m_used.add(kb, b);
    tc.m_clock = 7;

    region* got = tc.find_used_region(16, 16);  // 1 x 1 cells: both fit, b is older
    CHECK(got == b);
    CHECK(tc.m_used.size() == 1);
    CHECK(tc.m_used.find(kb) == tc.m_used.end());
    CHECK(tc.m_used.find(ka) != tc.m_used.end());
    CHECK(b->cells_w == 1);
    CHECK(b->cells_h == 1);
    CHECK(tc.m_last_clock == 7);
    CHECK(tc.m_free.size() == 2);  // right and lower strips of b

    CHECK(tc.find_used_region(256, 16) == nullptr);  // 16 cells wide, nothing fits
}

void test_reset() {
    texture_cache tc(64, 64, 4, true);
    fake_texture tex(64, 64);
    tc.m_texture = &tex;

    std::vector<unsigned char> pixels(64 * 64 * 4, 0xFF);
    tc.m_locked = pixels.data();
    region stale;
    texture_cache::key k;
    k.unk_0 = 9;
    tc.m_used.add(k, &stale);
    tc.m_clock = 4;
    tc.m_last_clock = 0;

    tc.reset();
    CHECK(tc.m_clock == 5);
    CHECK(tc.m_last_clock == 5);
    CHECK(tc.m_used.is_empty());

    bool zeroed = true;
    for (unsigned char v : pixels) {
        if (v != 0) {
            zeroed = false;
        }
    }
    CHECK(zeroed);

    CHECK(tc.m_regions.size() == 16);
    CHECK(tc.m_free.size() == 1);
    CHECK(tc.m_free[0] == &tc.m_regions[0]);
    CHECK(tc.m_regions[0].cells_w == 4);
    CHECK(tc.m_regions[0].cells_h == 4);
    CHECK(tc.m_regions[0].last_clock == 0);

    tc.m_locked = nullptr;  // the pixel buffer is local to this test
}

void test_verify_integrity() {
    texture_cache tc(64, 64, 4, true);
    fake_texture tex(64, 64);
    tc.m_texture = &tex;
    tc.reset();

    CHECK(tc.verify_integrity(nullptr) == 64);  // returns the texture height
    tc.verify_integrity();
    CHECK(tc.m_free.size() == 1);
}

void test_destructor_unlocks() {
    fake_texture tex(64, 64);
    std::vector<unsigned char> pixels(16, 0);
    texture_cache* tc = new texture_cache(64, 64, 4, true);
    tc->m_texture = &tex;
    tc->m_locked = pixels.data();
    delete tc;
    CHECK(tex.m_unlock_calls == 1);
}

}  // namespace

int main() {
    test_region_size_requirement();
    test_region_bounds();
    test_subdivide();
    test_find_available();
    test_find_used();
    test_reset();
    test_verify_integrity();
    test_destructor_unlocks();
    if (g_failures != 0) {
        std::fprintf(stderr, "%d check(s) failed\n", g_failures);
        return 1;
    }
    std::printf("texture_cache tests passed\n");
    return 0;
}
