// Test for batch v1-vendor-gameswf-gameswf_render_handler-1ca9d1-0 (gameswf::render_handler).
//   bash tools/trackb/check_tu.sh --test <this file> port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-gameswf_render_handler-1ca9d1-0.cpp
//
// The six Gameloft methods have no logic beyond their defaults: get_orientation returns 0 and the rest do
// nothing. The test checks those defaults through a render_handler pointer. It also checks that the header
// declares them virtual, since a derived class overriding them is reached through the base pointer.
// PureBase implements the upstream pure virtuals with no-op bodies so that the class can be instantiated.

#include <cstdio>
#include <cstdlib>

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"  // gameswf::rgba: a complete type is needed for by-value parameters

int g_failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                              \
        }                                                              \
    } while (0)

namespace gameswf {
namespace {

// Implements every pure virtual of render_handler (upstream gameswf.h) with a no-op or a null/false result.
struct PureBase : render_handler {
    bitmap_info* create_bitmap_info_empty() override { return nullptr; }
    bitmap_info* create_bitmap_info_alpha(int, int, unsigned char*) override { return nullptr; }
    bitmap_info* create_bitmap_info_rgb(image::rgb*) override { return nullptr; }
    bitmap_info* create_bitmap_info_rgba(image::rgba*) override { return nullptr; }
    video_handler* create_video_handler() override { return nullptr; }
    void begin_display(rgba, int, int, int, int, float, float, float, float) override {}
    void end_display() override {}
    void set_matrix(const matrix&) override {}
    void set_cxform(const cxform&) override {}
    void draw_mesh_strip(const void*, int) override {}
    void draw_triangle_list(const void*, int) override {}
    void draw_line_strip(const void*, int) override {}
    void fill_style_disable(int) override {}
    void fill_style_color(int, const rgba&) override {}
    void fill_style_bitmap(int, bitmap_info*, const matrix&, bitmap_wrap_mode, bitmap_blend_mode) override {}
    void line_style_disable() override {}
    void line_style_color(rgba) override {}
    void line_style_width(float) override {}
    void draw_bitmap(const matrix&, bitmap_info*, const rect&, const rect&, rgba) override {}
    void set_antialiased(bool) override {}
    bool test_stencil_buffer(const rect&, Uint8) override { return false; }
    void begin_submit_mask() override {}
    void end_submit_mask() override {}
    void disable_mask() override {}
    bool is_visible(const rect&) override { return false; }
    void open() override {}
};

// Overrides the six Gameloft methods and records what was called.
struct Recording : PureBase {
    int flushes = 0;
    int resets = 0;
    int orientation_sets = 0;
    int last_mode = -1;
    int begins = 0;
    int ends = 0;

    void flush() override { ++flushes; }
    void reset() override { ++resets; }
    void set_orientation(orientation_mode mode) override {
        ++orientation_sets;
        last_mode = static_cast<int>(mode);
    }
    int get_orientation() override { return 5; }
    void begin_display_callback() override { ++begins; }
    void end_display_callback() override { ++ends; }
};

void run() {
    // Base-class defaults: no observable effect, orientation 0.
    PureBase plain;
    render_handler* h = &plain;
    CHECK(h->get_orientation() == 0);
    h->flush();
    h->reset();
    h->set_orientation(static_cast<orientation_mode>(2));
    h->begin_display_callback();
    h->end_display_callback();
    CHECK(h->get_orientation() == 0);

    // Overrides are reached through the base pointer, so the declarations are virtual.
    Recording rec;
    render_handler* r = &rec;
    r->flush();
    r->reset();
    r->set_orientation(static_cast<orientation_mode>(2));
    r->begin_display_callback();
    r->end_display_callback();
    CHECK(rec.flushes == 1);
    CHECK(rec.resets == 1);
    CHECK(rec.orientation_sets == 1);
    CHECK(rec.last_mode == 2);
    CHECK(rec.begins == 1);
    CHECK(rec.ends == 1);
    CHECK(r->get_orientation() == 5);
}

}  // namespace
}  // namespace gameswf

// The gameswf.h include chain emits inline tu_string members that call out-of-line tu_string members
// (base/container.cpp, the tu_string batch). This test never runs them, so they abort if reached.
void tu_string::resize(int) { std::abort(); }
bool tu_string::operator==(const tu_string&) const { std::abort(); }
void tu_string::operator+=(const char*) { std::abort(); }
void tu_string::operator+=(const tu_string&) { std::abort(); }
int tu_string::utf8_char_count(const char*, int) { std::abort(); }

int main() {
    gameswf::run();
    if (g_failures == 0) {
        std::printf("render_handler test: all checks passed\n");
    }
    return g_failures == 0 ? 0 : 1;
}
