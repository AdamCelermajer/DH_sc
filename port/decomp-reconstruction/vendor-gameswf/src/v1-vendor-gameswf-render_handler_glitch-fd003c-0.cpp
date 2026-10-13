// Batch v1-vendor-gameswf-render_handler_glitch-fd003c-0: render_handler_glitch, part 1 of 3 (20 functions).
//
// Declarations and layout: vendor-gameswf/include/vendor-gameswf/render_handler_glitch.hpp (shared with parts 2 and 3;
// the part 1 declarations are the block after "0x7d4340 (another part)").
// Every function is read from the Hex-Rays pseudocode (show_batch.py). Register-level checks:
//   0x7d3e0c  set_target: vtable +0x8c with the bitmap_info field at +0x14; vtable +0x90 with the result dropped
//   0x7d3e84  end_display_callback: three vtable +0x6c calls, states 2, 0, 1 (projection, view, world)
//   0x7d4334  set_buffer_capacity: STR R1,[R0,#0x1F0]! then a tail call to BufferedRenderer::ensureBufferCapacity
//   0x7d4340  ensureBufferCapacity: operator new[](uint, glitch::memory::E_MEMORY_HINT) with hint 0; zeroes words 0,1,3,4,5 of each 24-byte entry;
//             stores the block at +0x374 and the capacity at +0x370; IBuffer::reset(size, block, true) is called on
//             the IBuffer* at [m_vertex_streams] + 0x14 between the two stores
//   0x7d43cc  line_style_color: cxform::transform(rgba) at this+0x324; line flag at +0x448 = 1; colour at +0x44c;
//             returns the alpha byte (LSR #24) in r0
//   0x7d4558  fill_style_bitmap: tail call to fill_style::set_bitmap (0x7d4430), which returns bool
//
// The line style is fill entry 2 (+0x448 = +0x3b0 + 2 * 76): its unk_00 is the flag that fill_style_color sets to 1,
// its color[4] starts at +0x44c and its unk_48 is the width at +0x490.
//
// Not reproduced (see the partial notes): the driver calls in set_target and end_display_callback (IVideoDriver
// methods with no declaration in glitch-video), the IBuffer::reset call in ensureBufferCapacity, the second context
// store in set_context (it lands in BufferedRenderer::m_textureCache), and the alpha return of line_style_color (the
// render_handler interface declares that function void).

#include "vendor-gameswf/render_handler_glitch.hpp"

#include <cstddef>
#include <cstdint>

#include "gameswf/gameswf_types.h"  // gameswf::rgba (full definition)
#include "glitch-core-io/core/memory_hint.hpp"  // glitch::memory::E_MEMORY_HINT (canonical), the hint of 0x7d4340

// Tagged global operator new[] that 0x7d4340 calls (_ZnajN6glitch6memory13E_MEMORY_HINTE), defined in
// core-platform/src/memory.cpp. core-platform/memory.hpp also declares it, but this TU cannot include that header:
// its deps/glitch_core.hpp stand-ins clash with glitch-shared/glitch_core.hpp, which render_handler_glitch.hpp uses.
void* operator new[](std::size_t size, glitch::memory::E_MEMORY_HINT hint);

// 0x7d3df4 _ZN21render_handler_glitch4openEv
// Empty body (bx lr). Overrides the pure gameswf::render_handler::open().
void render_handler_glitch::open() {
}

// 0x7d3df8 _ZN21render_handler_glitch15get_orientationEv
// Returns 0 (MOV R0, #0; BX LR). Overrides the DH2 render_handler::get_orientation().
int render_handler_glitch::get_orientation() {
    return 0;
}

// 0x7d3e00 _ZN21render_handler_glitch15set_antialiasedEb
// Empty body (bx lr). The flag is ignored. Overrides render_handler::set_antialiased(bool).
void render_handler_glitch::set_antialiased(bool /*enable*/) {
}

// 0x7d3e04 _ZN21render_handler_glitch20create_video_handlerEv
// Returns null (MOV R0, #0; BX LR). Overrides render_handler::create_video_handler().
gameswf::video_handler* render_handler_glitch::create_video_handler() {
    return nullptr;
}

// 0x7d3e0c _ZN21render_handler_glitch10set_targetEPN7gameswf11bitmap_infoE
// partial: both branches are present, but their driver calls are not made. With a target, the original calls
// the driver's pushRenderTarget at vtable +0x8c with the bitmap_info field at +0x14 (an intrusive_ptr to a render
// target, passed by const reference). With no target, it calls popRenderTarget at vtable +0x90 and drops the
// returned value. The base IVideoDriver symbols name both slots (decomp_vtable.py glitch::video::IVideoDriver),
// but the glitch-video header video_driver.hpp declares neither method, and the bitmap_info layout (the field at
// +0x14) is not in the tree.
void render_handler_glitch::set_target(gameswf::bitmap_info* target) {
    if (target != nullptr) {
        // missing: m_driver->pushRenderTarget(<bitmap_info +0x14>)
    } else {
        // missing: m_driver->popRenderTarget(), result discarded
    }
}

// 0x7d3e68 _ZN21render_handler_glitch18clear_frame_bufferEv
// Empty body (bx lr). DH2 virtual, slot +0x038.
void render_handler_glitch::clear_frame_buffer() {
}

// 0x7d3e6c _ZN21render_handler_glitch17read_frame_bufferEiiiiPh
// Empty body (bx lr). DH2 virtual, slot +0x03c. The arguments are ignored.
void render_handler_glitch::read_frame_buffer(int /*x*/, int /*y*/, int /*width*/, int /*height*/,
                                              unsigned char* /*pixels*/) {
}

// 0x7d3e70 _ZN21render_handler_glitch14set_wire_frameEb
// Stores the flag in BufferedRenderer::m_wireFrame (this+0x1f4; STRB at this+500). DH2 virtual, slot +0x040.
void render_handler_glitch::set_wire_frame(bool wire) {
    m_buffered.m_wireFrame = wire;
}

// 0x7d3e78 _ZN21render_handler_glitch11set_contextEPN7gameswf14player_contextE
// partial: the store at +0x08 is made. The second store, of the same pointer at +0x228 (552), is not made:
// +0x228 is BufferedRenderer::m_textureCache (BufferedRenderer at +0x1f0, its +0x38), which the BufferedRenderer
// header types as gameswf::texture_cache*, not as a player context. Layout conflict for the integrator.
void render_handler_glitch::set_context(gameswf::player_context* context) {
    m_context = context;
}

// 0x7d3e84 _ZN21render_handler_glitch20end_display_callbackEv
// partial: the original makes three driver calls, in this order, each taking a transform state and a matrix:
//   vtable +0x6c, state 2 (projection) with &m_projection (+0x114)
//   vtable +0x6c, state 0 (view)       with &m_view       (+0x158)
//   vtable +0x6c, state 1 (world)      with &m_world      (+0x19c)
// The state numbering follows the glitch E_TRANSFORMATION_STATE used by glitch-scene SViewFrustum. The slot is pure
// in the glitch-video IVideoDriver base and its method name is not in the tree, so the calls are not made.
void render_handler_glitch::end_display_callback() {
    // missing: m_driver, vtable +0x6c, (state 2, &m_projection)
    // missing: m_driver, vtable +0x6c, (state 0, &m_view)
    // missing: m_driver, vtable +0x6c, (state 1, &m_world)
}

// 0x7d3ee4 _ZN21render_handler_glitch10set_matrixERKN7gameswf6matrixE
// Copies the 24-byte gameswf::matrix (float m_[2][3]) to this+0x30c.
void render_handler_glitch::set_matrix(const gameswf::matrix& m) {
    m_matrix = m;
}

// 0x7d3f08 _ZN21render_handler_glitch10set_cxformERKN7gameswf6cxformE
// Copies the 32-byte gameswf::cxform (float m_[4][2]) to this+0x324.
void render_handler_glitch::set_cxform(const gameswf::cxform& cx) {
    m_cxform = cx;
}

// 0x7d3f2c _ZN21render_handler_glitch18fill_style_disableEi
// Clears the flag word of fill side fill_side (fill_style array at this+0x3b0, 76 bytes per entry).
void render_handler_glitch::fill_style_disable(int fill_side) {
    m_fill_styles[fill_side].unk_00 = 0;
}

// 0x7d3f40 _ZN21render_handler_glitch18line_style_disableEv
// Clears the flag word of the line style, which is fill entry 2 (+0x448).
void render_handler_glitch::line_style_disable() {
    m_fill_styles[2].unk_00 = 0;
}

// 0x7d3f4c _ZN21render_handler_glitch16line_style_widthEf
// Stores the width in the line style's last float, fill entry 2 at +0x490 (+0x448 + 72).
void render_handler_glitch::line_style_width(float width) {
    m_fill_styles[2].unk_48 = width;
}

// 0x7d3f54 _ZN21render_handler_glitch10is_visibleERKN7gameswf4rectE
// gameswf::rect is {m_x_min, m_x_max, m_y_min, m_y_max}. The original compares the rect with the two display
// values that begin_display sets (+0x304 width, +0x308 height):
//   width >= x_min  &&  x_max >= 0  &&  y_max >= 0  &&  height >= y_min
bool render_handler_glitch::is_visible(const gameswf::rect& bound) {
    return m_display_width >= bound.m_x_min && bound.m_x_max >= 0.0f && bound.m_y_max >= 0.0f &&
           m_display_height >= bound.m_y_min;
}

// 0x7d4334 _ZN21render_handler_glitch19set_buffer_capacityEN7gameswf14render_handler11buffer_modeEi
// Stores the buffer mode in BufferedRenderer::unk_00 (this+0x1f0), then calls BufferedRenderer::ensureBufferCapacity
// on the embedded object (a tail call in the original).
void render_handler_glitch::set_buffer_capacity(gameswf::render_handler::buffer_mode mode, int vertex_count) {
    m_buffered.unk_00 = static_cast<int>(mode);
    m_buffered.ensureBufferCapacity(vertex_count);
}

// 0x7d4340 _ZN21render_handler_glitch20ensureBufferCapacityEi
// partial: the growth path is reproduced except the IBuffer::reset call. Growing allocates 24 * n bytes with
// ::operator new[](size, glitch::memory::E_MEMORY_HINT(0)). The binary calls _ZnajN6glitch6memory13E_MEMORY_HINTE
// with R1 = 0, the hinted overload and not plain new[]. It then zeroes words 0, 1, 3, 4 and 5 of each entry (word 2
// is left as allocated), stores the block at +0x374, calls IBuffer::reset(size, block, true) on the IBuffer* at
// +0x14 of the object in m_vertex_streams (+0x378), and stores the new capacity at +0x370. The reset call is
// missing. Its target is the buffer of the object's first stream entry (entry 0 is at +0x14 and SVertexStream::buffer
// is its +0x00 in glitch-video/vertex_streams.hpp), but that header keeps the entries in a std::vector, so the mapping
// is left to the integrator. reset takes ownership of the block, so the hinted allocation above is the one that
// matches it. glitch-video's IBuffer::reset (src/buffer.cpp) releases owned data with plain delete[]; that release
// path is not checked against the binary. The old block is not freed (as in the original).
void render_handler_glitch::ensureBufferCapacity(int vertex_count) {
    if (m_vertex_capacity >= vertex_count) {
        return;
    }
    const uint32_t byte_size = 24u * static_cast<uint32_t>(vertex_count);
    uint8_t* block = static_cast<uint8_t*>(::operator new[](byte_size, glitch::memory::E_MEMORY_HINT(0)));
    if (vertex_count > 0) {
        // Six 32-bit words per entry, the layout the driver reads. Word 2 is not written.
        uint32_t* words = reinterpret_cast<uint32_t*>(block);
        for (int i = 0; i < vertex_count; ++i) {
            uint32_t* entry = words + 6 * i;
            entry[0] = 0;
            entry[1] = 0;
            entry[3] = 0;
            entry[4] = 0;
            entry[5] = 0;
        }
    }
    m_vertices = block;
    // missing: IBuffer::reset(byte_size, block, true) on the IBuffer* at m_vertex_streams->+0x14
    m_vertex_capacity = vertex_count;
}

// 0x7d43cc _ZN21render_handler_glitch16line_style_colorEN7gameswf4rgbaE
// partial: the stores are reproduced. The colour goes through the cxform at this+0x324, then the line style's flag
// (fill entry 2, unk_00) is set to 1 and the four colour bytes are stored at +0x44c in rgba order. The original also
// returns the alpha byte of the transformed colour in r0 (HIBYTE of the packed value). The render_handler interface
// declares this function void, so that return value is not produced.
void render_handler_glitch::line_style_color(gameswf::rgba color) {
    const gameswf::rgba transformed = m_cxform.transform(color);
    fill_style& line = m_fill_styles[2];
    line.unk_00 = 1;
    line.color[0] = transformed.m_r;
    line.color[1] = transformed.m_g;
    line.color[2] = transformed.m_b;
    line.color[3] = transformed.m_a;
}

// 0x7d4558 _ZN21render_handler_glitch17fill_style_bitmapEiPN7gameswf11bitmap_infoERKNS0_6matrixENS0_14render_handler16bitmap_wrap_modeE
// Forwards to fill_style::set_bitmap of fill side fill_side with the cxform at this+0x324, and returns its bool.
// The DH2 signature has four arguments; upstream's five-argument pure virtual is a separate overload.
bool render_handler_glitch::fill_style_bitmap(int fill_side, gameswf::bitmap_info* bitmap, const gameswf::matrix& m,
                                              gameswf::render_handler::bitmap_wrap_mode wrap) {
    return m_fill_styles[fill_side].set_bitmap(bitmap, m, wrap, m_cxform);
}
