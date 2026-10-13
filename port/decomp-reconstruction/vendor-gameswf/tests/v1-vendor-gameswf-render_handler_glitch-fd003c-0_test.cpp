// Test for batch v1-vendor-gameswf-render_handler_glitch-fd003c-0 (render_handler_glitch, part 1 of 3).
//   bash tools/trackb/check_tu.sh --test <this file> port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-render_handler_glitch-fd003c-0.cpp
//        port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-gameswf_tu_string-39e5bd-0.cpp
//
// The class's constructor, destructor and vtable belong to other parts of the class, so the test never constructs
// the object. It keeps zeroed storage of the class size and calls only non-virtual members, through qualified names,
// so no vtable is needed. The stand-ins below are the code outside this batch that these functions call:
//   BufferedRenderer::ensureBufferCapacity(int)                 (0x7d4284), records the call
//   render_handler_glitch::fill_style::set_bitmap(...)          (0x7d4430), records its arguments and returns true
//   operator new[](std::size_t, glitch::memory::E_MEMORY_HINT)  (core-platform/src/memory.cpp), malloc(), records the
//       size and the hint; the memory is not zeroed
//   gameswf::cxform::transform(rgba) const, gameswf::cxform::cxform(), gameswf::matrix::matrix()   (upstream), identity
// The tu_string batch source is linked too: the inline tu_string members that the headers pull in (dllexport) call
// tu_string::resize and friends, which that batch defines.

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "gameswf/gameswf_types.h"
#include "glitch-core-io/core/memory_hint.hpp"
#include "vendor-gameswf/render_handler_glitch.hpp"

// ---- stand-ins -------------------------------------------------------------------------------------------------

namespace {
int g_bufferCapacityCalls = 0;
int g_bufferCapacityLast = -1;
const void* g_setBitmapThis = nullptr;
gameswf::render_handler::bitmap_wrap_mode g_setBitmapWrap = gameswf::render_handler::WRAP_REPEAT;
int g_hintedNewCalls = 0;
std::size_t g_hintedNewSize = 0;
int g_hintedNewHint = -1;
}  // namespace

void BufferedRenderer::ensureBufferCapacity(int vertexCount) {
    ++g_bufferCapacityCalls;
    g_bufferCapacityLast = vertexCount;
}

void* operator new[](std::size_t size, glitch::memory::E_MEMORY_HINT hint) {
    ++g_hintedNewCalls;
    g_hintedNewSize = size;
    g_hintedNewHint = static_cast<int>(hint);
    return std::malloc(size);
}

bool render_handler_glitch::fill_style::set_bitmap(gameswf::bitmap_info* /*bitmap*/, const gameswf::matrix& /*m*/,
                                                   gameswf::render_handler::bitmap_wrap_mode wrap,
                                                   const gameswf::cxform& /*cx*/) {
    g_setBitmapThis = this;
    g_setBitmapWrap = wrap;
    return true;
}

gameswf::rgba gameswf::cxform::transform(const gameswf::rgba in) const {
    return in;
}
gameswf::cxform::cxform() {}
gameswf::matrix::matrix() {}

// tu_string::resize and tu_string::utf8_char_count (upstream base/container.cpp, not linked here): the tu_string
// batch source references them. This test never reaches a string member, so they abort if called.
void tu_string::resize(int) {
    std::abort();
}

int tu_string::utf8_char_count(const char*, int) {
    std::abort();
}

// ---- checks ----------------------------------------------------------------------------------------------------

namespace {
int g_failures = 0;

#define CHECK(cond)                                              \
    do {                                                         \
        if (!(cond)) {                                           \
            std::printf("FAIL line %d: %s\n", __LINE__, #cond); \
            ++g_failures;                                        \
        }                                                        \
    } while (0)
}  // namespace

int main() {
    alignas(render_handler_glitch) unsigned char storage[sizeof(render_handler_glitch)];
    std::memset(storage, 0, sizeof storage);
    render_handler_glitch& g = *reinterpret_cast<render_handler_glitch*>(storage);

    // get_orientation, create_video_handler, the empty functions.
    CHECK(g.render_handler_glitch::get_orientation() == 0);
    CHECK(g.render_handler_glitch::create_video_handler() == nullptr);
    g.render_handler_glitch::open();
    g.render_handler_glitch::set_antialiased(true);
    g.render_handler_glitch::clear_frame_buffer();
    g.render_handler_glitch::read_frame_buffer(0, 0, 1, 1, nullptr);

    // is_visible: width against x_min, x_max and height against zero, height against y_min.
    g.m_display_width = 100.0f;
    g.m_display_height = 50.0f;
    gameswf::rect r;
    r.m_x_min = 10.0f;
    r.m_x_max = 20.0f;
    r.m_y_min = 5.0f;
    r.m_y_max = 15.0f;
    CHECK(g.render_handler_glitch::is_visible(r));
    gameswf::rect r2 = r;
    r2.m_x_min = 200.0f;
    CHECK(!g.render_handler_glitch::is_visible(r2));
    gameswf::rect r3 = r;
    r3.m_x_max = -1.0f;
    CHECK(!g.render_handler_glitch::is_visible(r3));
    gameswf::rect r4 = r;
    r4.m_y_max = -1.0f;
    CHECK(!g.render_handler_glitch::is_visible(r4));
    gameswf::rect r5 = r;
    r5.m_y_min = 60.0f;
    CHECK(!g.render_handler_glitch::is_visible(r5));

    // set_matrix and set_cxform copy the whole value.
    gameswf::matrix mm;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            mm.m_[i][j] = static_cast<float>(i * 3 + j + 1);
        }
    }
    g.render_handler_glitch::set_matrix(mm);
    CHECK(g.m_matrix.m_[0][0] == 1.0f);
    CHECK(g.m_matrix.m_[1][2] == 6.0f);
    gameswf::cxform cx;
    cx.m_[3][1] = 0.5f;
    cx.m_[0][0] = 0.25f;
    g.render_handler_glitch::set_cxform(cx);
    CHECK(g.m_cxform.m_[3][1] == 0.5f);
    CHECK(g.m_cxform.m_[0][0] == 0.25f);

    // Fill entry 1 is cleared, entry 0 is untouched.
    g.m_fill_styles[0].unk_00 = 1;
    g.m_fill_styles[1].unk_00 = 1;
    g.render_handler_glitch::fill_style_disable(1);
    CHECK(g.m_fill_styles[1].unk_00 == 0);
    CHECK(g.m_fill_styles[0].unk_00 == 1);

    // Line style is fill entry 2: width, colour through the cxform (identity stand-in), flag set and cleared.
    g.render_handler_glitch::line_style_width(2.5f);
    CHECK(g.m_fill_styles[2].unk_48 == 2.5f);
    g.render_handler_glitch::line_style_color(gameswf::rgba(1, 2, 3, 4));
    CHECK(g.m_fill_styles[2].unk_00 == 1);
    CHECK(g.m_fill_styles[2].color[0] == 1 && g.m_fill_styles[2].color[1] == 2 &&
          g.m_fill_styles[2].color[2] == 3 && g.m_fill_styles[2].color[3] == 4);
    g.render_handler_glitch::line_style_disable();
    CHECK(g.m_fill_styles[2].unk_00 == 0);

    // Wire frame flag lives in the embedded BufferedRenderer.
    g.render_handler_glitch::set_wire_frame(true);
    CHECK(g.m_buffered.m_wireFrame);
    g.render_handler_glitch::set_wire_frame(false);
    CHECK(!g.m_buffered.m_wireFrame);

    // Buffer capacity: the mode goes to unk_00 and the count goes to BufferedRenderer::ensureBufferCapacity.
    g.render_handler_glitch::set_buffer_capacity(static_cast<gameswf::render_handler::buffer_mode>(2), 64);
    CHECK(g.m_buffered.unk_00 == 2);
    CHECK(g_bufferCapacityCalls == 1);
    CHECK(g_bufferCapacityLast == 64);

    // Context: the +0x08 pointer is stored.
    int context_marker = 0;
    g.render_handler_glitch::set_context(reinterpret_cast<gameswf::player_context*>(&context_marker));
    CHECK(g.m_context == reinterpret_cast<gameswf::player_context*>(&context_marker));

    // Bitmap fill forwards to fill entry 1 with the wrap mode and returns set_bitmap's result.
    const bool filled = g.render_handler_glitch::fill_style_bitmap(1, nullptr, mm, gameswf::render_handler::WRAP_CLAMP);
    CHECK(filled);
    CHECK(g_setBitmapThis == &g.m_fill_styles[1]);
    CHECK(g_setBitmapWrap == gameswf::render_handler::WRAP_CLAMP);

    // ensureBufferCapacity: growth allocates 24 * n bytes through operator new[](size, E_MEMORY_HINT 0), the call the
    // binary makes, and zeroes words 0, 1, 3, 4, 5 of each 24-byte entry. A smaller request keeps the block and
    // allocates nothing.
    CHECK(g.m_vertex_capacity == 0);
    g.ensureBufferCapacity(10);
    CHECK(g.m_vertex_capacity == 10);
    CHECK(g.m_vertices != nullptr);
    CHECK(g_hintedNewCalls == 1);
    CHECK(g_hintedNewSize == 240);
    CHECK(g_hintedNewHint == 0);
    {
        const uint32_t* words = reinterpret_cast<const uint32_t*>(g.m_vertices);
        for (int i = 0; i < 10; ++i) {
            const uint32_t* entry = words + 6 * i;
            CHECK(entry[0] == 0 && entry[1] == 0 && entry[3] == 0 && entry[4] == 0 && entry[5] == 0);
        }
    }
    const uint8_t* block = g.m_vertices;
    g.ensureBufferCapacity(5);
    CHECK(g.m_vertex_capacity == 10);
    CHECK(g.m_vertices == block);
    CHECK(g_hintedNewCalls == 1);
    g.ensureBufferCapacity(20);
    CHECK(g.m_vertex_capacity == 20);
    CHECK(g_hintedNewCalls == 2);
    CHECK(g_hintedNewSize == 480);
    CHECK(g_hintedNewHint == 0);

    if (g_failures == 0) {
        std::printf("render_handler_glitch part 1: all checks passed\n");
        return 0;
    }
    std::printf("render_handler_glitch part 1: %d failure(s)\n", g_failures);
    return 1;
}
