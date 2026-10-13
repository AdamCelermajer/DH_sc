// Batch v1-vendor-gameswf-render_handler_glitch-fd003c-1 (render_handler_glitch, part 2 of 3).
//
// Gameloft class, not in upstream GameSWF r1714. Layout and member offsets are in
// vendor-gameswf/include/vendor-gameswf/render_handler_glitch.hpp. The bodies that are still PARTIAL need
// glitch::video::IVideoDriver vtable slots that have no name in the tree (+0x6c, +0x70, +0x78, +0x1e0, +0x1e4),
// gameswf::render_cache, gameswf::triangle_intersector, the collada effect database, CMaterial parameters or a
// BufferedRenderer::flush that takes the player context. Each body says what is missing; the ledger repeats it.
#include "vendor-gameswf/render_handler_glitch.hpp"

#include <cmath>
#include <cstdint>

#include "gameswf/gameswf_types.h"  // gameswf::rgba and gameswf::rect (complete types)
#include "glitch-video/video_driver.hpp"
#include "glitch-video/vertex_streams.hpp"

namespace {

// Identity in gameswf's 2x3 layout (m_[row][column]).
void set_identity(gameswf::matrix& m) {
    m.m_[0][0] = 1.0f;
    m.m_[0][1] = 0.0f;
    m.m_[0][2] = 0.0f;
    m.m_[1][0] = 0.0f;
    m.m_[1][1] = 1.0f;
    m.m_[1][2] = 0.0f;
}

// s_no_bitmap_data (local symbol at 0x99e1e0, 16 bytes in .data): the alpha of the default 4x4 bitmap. Read from
// the binary; all sixteen bytes are 0xff, so the default bitmap is fully opaque.
unsigned char s_no_bitmap_data[16] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

}  // namespace

// 0x7d4624 render_handler_glitch::fill_style_color(int, gameswf::rgba const&)
// The original first calls fill_style::set_bitmap on this fill side with the default bitmap (m_default_bitmap,
// +0x37c), an identity matrix, wrap mode 0 (WRAP_REPEAT) and the batch colour transform (m_cxform, +0x324). Then it
// writes the colour after the colour transform, sets the fill type to 1 and stores the bytes in rgba order. The
// original returns HIBYTE of the transformed colour in r0; the void interface ignores it, so the value is dropped.
void render_handler_glitch::fill_style_color(int fill_side, const gameswf::rgba& color) {
    fill_style& fill = m_fill_styles[fill_side];
    gameswf::matrix identity;
    set_identity(identity);
    fill.set_bitmap(&m_default_bitmap, identity, gameswf::render_handler::WRAP_REPEAT, m_cxform);

    const gameswf::rgba out = m_cxform.transform(color);
    fill.unk_00 = 1;
    fill.color[0] = out.m_r;
    fill.color[1] = out.m_g;
    fill.color[2] = out.m_b;
    fill.color[3] = out.m_a;
}

// 0x7d4bb4 render_handler_glitch::create_bitmap_info_native(int, int, gameswf::membuf const*)
// The original allocates 0x34 bytes and runs bitmap_info_ogl's constructor with the driver, the width, the
// height and the data. Plain operator new here; the original's allocator hint is not reproduced.
gameswf::bitmap_info* render_handler_glitch::create_bitmap_info_native(int width, int height,
                                                                       const membuf* data) {
    return new gameswf::bitmap_info_ogl(m_driver, width, height, data);
}

// 0x7d4d00 render_handler_glitch::create_bitmap_info_texture(glitch::video::ITexture*)
// The original allocates 0x34 bytes and runs bitmap_info_ogl(driver, texture).
gameswf::bitmap_info* render_handler_glitch::create_bitmap_info_texture(glitch::video::ITexture* texture) {
    return new gameswf::bitmap_info_ogl(m_driver, texture);
}

// 0x7d4e28 render_handler_glitch::create_bitmap_info_empty(void)
// The original allocates 0x34 bytes and runs bitmap_info_ogl(driver).
gameswf::bitmap_info* render_handler_glitch::create_bitmap_info_empty() {
    return new gameswf::bitmap_info_ogl(m_driver);
}

// 0x7d4f8c render_handler_glitch::set_render_cache(gameswf::render_cache*)
// PARTIAL: the cache pointer is stored (+0x0c). The original then clears the cache: when the cache's count
// (+0x14) is negative it zeroes the 24-byte entries from the count up to zero, and it zeroes the count
// (+0x14), the word at +0x34 and the word at +0x24. gameswf::render_cache is not in the tree, so those
// writes are not made. The original's return value (the loop index) is not used by callers.
void render_handler_glitch::set_render_cache(gameswf::render_cache* cache) {
    m_render_cache = cache;
}

// 0x7d53cc render_handler_glitch::create_bitmap_info_target(int, int)
// The original allocates 0x34 bytes and runs bitmap_info_ogl(driver, width, height).
gameswf::bitmap_info* render_handler_glitch::create_bitmap_info_target(int width, int height) {
    return new gameswf::bitmap_info_ogl(m_driver, width, height);
}

// 0x7d551c render_handler_glitch::create_bitmap_info_alpha(int, int, unsigned char*)
// The original allocates 0x34 bytes and runs bitmap_info_ogl(driver, width, height, data).
gameswf::bitmap_info* render_handler_glitch::create_bitmap_info_alpha(int width, int height,
                                                                      unsigned char* data) {
    return new gameswf::bitmap_info_ogl(m_driver, width, height, data);
}

// 0x7d57b0 render_handler_glitch::create_bitmap_info_rgba(image::rgba*)
// The original allocates 0x34 bytes and runs bitmap_info_ogl(driver, image).
gameswf::bitmap_info* render_handler_glitch::create_bitmap_info_rgba(image::rgba* im) {
    return new gameswf::bitmap_info_ogl(m_driver, im);
}

// 0x7d5a6c render_handler_glitch::create_bitmap_info_rgb(image::rgb*)
// The original allocates 0x34 bytes and runs bitmap_info_ogl(driver, image).
gameswf::bitmap_info* render_handler_glitch::create_bitmap_info_rgb(image::rgb* im) {
    return new gameswf::bitmap_info_ogl(m_driver, im);
}

// 0x7d60f4 render_handler_glitch::render_handler_glitch(glitch::video::IVideoDriver*)   [C1]
// PARTIAL. Written: the scalar fields, the identity line matrix and colour transform, the three fill sides
// (colour 0xff, identity bitmap matrix and colour transform, flag 0), the BufferedRenderer and TransformState
// members (their constructors run as member initialisers), the vertex streams m_vertex_streams =
// CVertexStreams::allocate(1, 0x40000) (+0x378), the default bitmap m_default_bitmap = bitmap_info_ogl(driver, 4, 4,
// s_no_bitmap_data) (+0x37c), and the ensureBufferCapacity(256) call. Not written:
//   - the driver reference: the original adds one (++refcount at +4 of the driver); the glitch reference
//     API is not declared in this batch;
//   - the three setStream calls on m_vertex_streams: each one takes a vertex buffer that driver vtable slot
//     +0x78 creates (not named in the tree), and the three stream descriptors are set from it;
//   - gameswf::triangle_intersector's constructor (+0x36c, not in the tree);
//   - the collada effect database (gameswf_effects.bdae) and the eight BufferedRenderer::createBlendModeMaterial
//     calls that follow it.
// The original leaves fill_style::source_bitmap (+0x08) and unk_48 (+0x48) unset; here they take their member
// defaults (null and 0).
render_handler_glitch::render_handler_glitch(glitch::video::IVideoDriver* driver)
    : m_driver(driver),
      m_buffered(driver),
      m_vertex_streams(glitch::video::CVertexStreams::allocate(static_cast<uint8_t>(1), 0x40000u)),
      m_default_bitmap(driver, 4, 4, s_no_bitmap_data) {
    unk_04 = 1;
    unk_14 = 0x001f00ffu;

    set_identity(m_matrix);
    for (int c = 0; c < 4; ++c) {
        m_cxform.m_[c][0] = 1.0f;
        m_cxform.m_[c][1] = 0.0f;
    }

    for (fill_style& fill : m_fill_styles) {
        fill.unk_00 = 0;
        fill.color[0] = 0xff;
        fill.color[1] = 0xff;
        fill.color[2] = 0xff;
        fill.color[3] = 0xff;
        set_identity(fill.bitmap_matrix);
        for (int c = 0; c < 4; ++c) {
            fill.bitmap_cxform.m_[c][0] = 1.0f;
            fill.bitmap_cxform.m_[c][1] = 0.0f;
        }
        fill.unk_44 = 0;
    }

    ensureBufferCapacity(256);
}

// 0x7d6680 render_handler_glitch::get_matrix(gameswf::matrix const&)
// Converts a gameswf 2x3 matrix (m_[row][column], translation in m_[0][2] and m_[1][2]) to the glitch 4x4
// column-major layout (see TransformState). The original zeroes its 0x41-byte result (sret), fills columns 0, 1
// and 3 from the matrix, sets elements 10 and 15 to 1, and clears the flag at +0x40. The result is returned as
// glitch::core::CMatrix4<float>, which has the same layout (M[16] and the flag).
glitch::core::CMatrix4<float> render_handler_glitch::get_matrix(const gameswf::matrix& m) const {
    const float m00 = m.m_[0][0];
    const float m01 = m.m_[0][1];
    const float m02 = m.m_[0][2];
    const float m10 = m.m_[1][0];
    const float m11 = m.m_[1][1];
    const float m12 = m.m_[1][2];

    glitch::core::CMatrix4<float> out;
    float* e = out.M;
    e[0] = m00;  // column 0: (m00, m10, 0, 0)
    e[1] = m10;
    e[2] = 0.0f;
    e[3] = 0.0f;
    e[4] = m01;  // column 1: (m01, m11, 0, 0)
    e[5] = m11;
    e[6] = 0.0f;
    e[7] = 0.0f;
    e[8] = 0.0f;  // column 2: (0, 0, 1, 0)
    e[9] = 0.0f;
    e[10] = 1.0f;
    e[11] = 0.0f;
    e[12] = m02;  // column 3: (tx, ty, 0, 1)
    e[13] = m12;
    e[14] = 0.0f;
    e[15] = 1.0f;
    out.flag = false;
    return out;
}

// 0x7d6a40 render_handler_glitch::begin_display_callback(void)
// PARTIAL: forwards to BufferedRenderer::flush. The original passes its r1 register as the player context
// (gameswf::player_context, used by texture_cache::unlock_all). The symbol has no declared argument, and the
// game header's BufferedRenderer::flush() takes none, so no context is passed here.
void render_handler_glitch::begin_display_callback() {
    m_buffered.flush();
}

// 0x7d6afc render_handler_glitch::reset(void)
// PARTIAL: the original first calls IVideoDriver::setMaterial with a null material (the function is not
// declared in the tree), then clears the textures of the batch renderer and drops two null material
// references. Only the texture clear is written.
void render_handler_glitch::reset() {
    m_buffered.clearTextures();
}

// 0x7d6ddc render_handler_glitch::flush(void)
// PARTIAL: forwards to BufferedRenderer::flush. Same gap as begin_display_callback: the original passes its
// r1 register as the player context, which the game header's flush() does not take.
void render_handler_glitch::flush() {
    m_buffered.flush();
}

// 0x7d6de4 render_handler_glitch::end_display(void)
// PARTIAL: flushes the batch, then restores the driver state the original saved in begin_display. Omitted,
// in the original's order: setTransform(2, m_saved_state.projection), setTransform(0, m_saved_state.view),
// setTransform(1, m_saved_state.world) through driver vtable slot +0x6c (not named in the tree); the viewport
// restore on the current render target (driver+0xcc, IRenderTarget::setViewport, slot +0x0c) with
// m_saved_state.viewport. Written: setOption(256, m_saved_option_256), which the original calls through slot +0xa0
// (named IVideoDriver::setOption in the tree, but not declared virtual there). Omitted last: driver slot
// +0x1e4 with the address of unk_14.
void render_handler_glitch::end_display() {
    m_buffered.flush();
    m_driver->setOption(256, m_saved_option_256);
}

// 0x7d71bc render_handler_glitch::render_mask_intersection(void)
// PARTIAL: the texture bind and the vertex and batch loop are written. The original's flush calls (the one in
// the loop and the one at the end) pass its r1 register as the player context, as begin_display_callback does;
// the game header's BufferedRenderer::flush() takes none, so no context is passed. The loop keeps the original's
// exit: when the point index reaches the run size, it flushes and returns without queueing the vertices still
// pending in the current batch.
void render_handler_glitch::render_mask_intersection() {
    // The default bitmap's texture (its +0x10 word, +0x38c in the original) is bound first.
    m_buffered.setTexture(m_default_bitmap.m_texture);

    const MaskRun& run = m_mask_stack[m_mask_depth - 1];
    const int run_count = run.points.size();

    ensureBufferCapacity(256);
    if (run_count > 0) {
        Vertex* verts = reinterpret_cast<Vertex*>(m_vertices);
        int point = 0;   // the original's v6: index into the run's points
        int vertex = 0;  // the original's v7: vertices in the current batch
        do {
            for (;;) {
                Vertex& v = verts[vertex++];
                bool full = vertex > 255;
                const glitch::core::vector2d<float> p = run.points[point];
                if (vertex == run_count) {
                    full = true;
                }
                ++point;
                v.color = 0xffffffffu;  // white: the four bytes are set to -1
                v.z = m_mask_z;
                v.x = p.X;
                v.y = p.Y;
                if (full) {
                    break;
                }
                if (point == run_count) {
                    m_buffered.flush();
                    return;
                }
            }
            m_vertex_streams->m_field08 = static_cast<uint32_t>(vertex);
            m_buffered.queueBuffer(m_vertex_streams, 6);
            vertex = 0;
        } while (point != run_count);
    }
    m_buffered.flush();
}

// 0x7d73d0 render_handler_glitch::begin_display(gameswf::rgba, int, int, int, int, float, float, float, float)
// PARTIAL. Written: the display width and height (fabs of x1-x0, y1-y0), mask depth and z reset, mask run sizes
// set to zero, BufferedRenderer::reset, setOption(256, false), the projection matrix (m_state.projection, state 2)
// and the view matrix (m_state.view, state 0: ortho with translation -(x1+x0)/w, -(y1+y0)/h). The background colour
// is not used by the original. Omitted, in the original's order:
//   - the saves of the driver's transform states 2, 0 and 1 into m_saved_state (getTransform through vtable slot +0x70);
//   - the saves of the current render target's viewport and of the driver's option 256 bit (driver+0x88);
//   - driver slot +0x1e0 with the address of unk_14;
//   - driver slot +0xa8 with argument 2;
//   - setTransform(2, m_state.projection), setTransform(0, m_state.view) (slot +0x6c, not named in the tree);
//   - the IRenderTarget::setViewport call with the rectangle (viewport_x0, viewport_y0, viewport_x0 + width,
//     viewport_y0 + height);
//   - the world matrix (state 1): the original copies glitch::core::IdentityMatrix (not in the tree) and
//     passes it to setTransform, so m_state.world is left as it was.
// The mask stack is reset to size zero for each run. The original also writes a zero for any run with a
// negative size; a gameswf::array never has one, so that path is not written.
void render_handler_glitch::begin_display(gameswf::rgba background_color, int viewport_x0,
                                          int viewport_y0, int viewport_width, int viewport_height,
                                          float x0, float x1, float y0, float y1) {
    (void)background_color;
    (void)viewport_x0;
    (void)viewport_y0;
    (void)viewport_width;
    (void)viewport_height;

    const float w = x1 - x0;
    const float h = y1 - y0;
    m_display_width = std::fabs(w);
    m_display_height = std::fabs(h);

    m_mask_depth = 0;
    m_mask_z = 0.0f;
    for (int i = 0; i < m_mask_stack.size(); ++i) {
        m_mask_stack[i].points.resize(0);
    }

    m_buffered.reset();
    m_driver->setOption(256, false);

    // State 2 (projection): an orthographic-style matrix with depth mapped to 0..1.
    float* p = m_state.projection.M;
    p[0] = 1.0f;
    p[1] = 0.0f;
    p[2] = 0.0f;
    p[3] = 0.0f;
    p[4] = 0.0f;
    p[5] = -1.0f;
    p[6] = 0.0f;
    p[7] = 0.0f;
    p[8] = 0.0f;
    p[9] = 0.0f;
    p[10] = 0.5f;
    p[11] = 0.0f;
    p[12] = 0.0f;
    p[13] = 0.0f;
    p[14] = 0.5f;
    p[15] = 1.0f;
    m_state.projection.flag = false;
    // Omitted: setTransform(2, m_state.projection).

    // State 0 (view): maps the display rectangle to clip space.
    float* v = m_state.view.M;
    v[0] = 2.0f / w;
    v[1] = 0.0f;
    v[2] = 0.0f;
    v[3] = 0.0f;
    v[4] = 0.0f;
    v[5] = 2.0f / h;
    v[6] = 0.0f;
    v[7] = 0.0f;
    v[8] = 0.0f;
    v[9] = 0.0f;
    v[10] = -1.0f;
    v[11] = 0.0f;
    v[12] = -(x1 + x0) / w;
    v[13] = -(y1 + y0) / h;
    v[14] = -0.0f;
    v[15] = 1.0f;
    m_state.view.flag = false;
    // Omitted: setTransform(0, m_state.view).

    // Omitted: setTransform(1, m_state.world) after copying glitch::core::IdentityMatrix into m_state.world.
}

// 0x7d77b0 render_handler_glitch::draw_line_strip(void const*, int)
// PARTIAL. The coordinates are float (x, y) pairs (8 bytes each), as the original reads them. Written: the batch
// flush, the line scale, the material choice, the vertex transform and colour, and the vertex count on the vertex
// streams. Omitted: the write of the line width into the selected material's parameter block (CMaterial's technique
// table at +0x18 is not in the tree), the texture parameter (setParameter of m_default_bitmap.m_texture, the +0x38c
// word of the default bitmap, on the material; the CMaterial parameter API is not in the tree),
// IVideoDriver::setMaterial, and the one-argument IVideoDriver::draw call that sends the batch (no declaration in the tree).
void render_handler_glitch::draw_line_strip(const void* coords, int vertex_count) {
    m_buffered.flush();

    // Line scale: the length of the x axis of the line matrix, signed by the determinant of its 2x2 part,
    // plus the length of the y axis, times the fill side 2 scale (+0x490), halved and divided by 20.
    const float a = m_matrix.m_[0][0];
    const float b = m_matrix.m_[0][1];
    const float d = m_matrix.m_[1][0];
    const float e = m_matrix.m_[1][1];
    float axis_x = std::sqrt(a * a + b * b);
    if (e * a - d * b < 0.0f) {
        axis_x = -axis_x;
    }
    const float axis_y = std::sqrt(e * e + d * d);
    float scale = ((std::fabs(axis_x) + std::fabs(axis_y)) * m_fill_styles[2].unk_48) * 0.5f / 20.0f;
    // The original writes this value into the selected material's line-width parameter (omitted).
    const float line_width = scale <= 1.0f ? 1.0f : scale;
    (void)line_width;

    // Material selection: the batch renderer's material for m_materialIndex, or entry 0 when that slot has
    // no material. Only the selection is written; the parameter write is omitted.
    const int material_index = m_buffered.m_materialIndex;
    const BufferedRenderer::MaterialData& selected =
        m_buffered.m_materials[material_index].material ? m_buffered.m_materials[material_index]
                                                       : m_buffered.m_materials[0];
    (void)selected;
    // Omitted: setParameter<intrusive_ptr<ITexture>>(selected.material, selected.textureParam, 0, m_default_bitmap.m_texture),
    // IVideoDriver::setMaterial(driver, selected.material).

    // The line colour is fill side 2's colour.
    const uint8_t cr = m_fill_styles[2].color[0];
    const uint8_t cg = m_fill_styles[2].color[1];
    const uint8_t cb = m_fill_styles[2].color[2];
    const uint8_t ca = m_fill_styles[2].color[3];

    ensureBufferCapacity(vertex_count);
    const glitch::core::CMatrix4<float> mat = get_matrix(m_matrix);
    const float* M = mat.M;
    const float* pts = static_cast<const float*>(coords);
    const float z = m_mask_z;

    // Packed rgba, low byte first, as the four byte stores of the original give it.
    const uint32_t packed = uint32_t(cr) | (uint32_t(cg) << 8) | (uint32_t(cb) << 16) | (uint32_t(ca) << 24);
    Vertex* verts = reinterpret_cast<Vertex*>(m_vertices);
    for (int i = 0; i < vertex_count; ++i) {
        const float x = pts[2 * i];
        const float y = pts[2 * i + 1];
        Vertex& v = verts[i];
        v.u = 0.0f;  // the original zeroes the first two words of the entry
        v.v = 0.0f;
        v.color = packed;
        v.x = ((x * M[0] + y * M[4]) + z * M[8]) + M[12];
        v.y = ((x * M[1] + y * M[5]) + z * M[9]) + M[13];
        v.z = ((x * M[2] + y * M[6]) + z * M[10]) + M[14];
    }

    m_vertex_streams->m_field08 = static_cast<uint32_t>(vertex_count);
    // Omitted: IVideoDriver::draw(driver) with the vertex streams, then the reference drop.
}

// 0x7d7cf4 render_handler_glitch::~render_handler_glitch(void)   [D1]
// PARTIAL. Released in the original's order: the vertex streams and the vertex pointer, the triangle point
// array, each mask run, the mask stack. The members are then destroyed in reverse declaration order: the default
// bitmap (m_default_bitmap, the bitmap_info_ogl destructor) first, then the batch renderer (m_buffered), as the
// original does. Omitted: the destructor of triangle_intersector (+0x36c, not in the tree), and the driver
// reference drop (the reference is a raw pointer here; the drop is not declared in this batch).
render_handler_glitch::~render_handler_glitch() {
    m_vertex_streams = nullptr;
    m_vertices = nullptr;

    // Omitted: ~triangle_intersector on pad_36c.

    m_triangle_points.resize(0);
    m_triangle_points.reserve(0);

    for (int i = 0; i < m_mask_stack.size(); ++i) {
        m_mask_stack[i].points.resize(0);
        m_mask_stack[i].points.reserve(0);
    }
    m_mask_stack.resize(0);
    m_mask_stack.reserve(0);

    // Omitted: IReferenceCounted::drop on m_driver, then m_driver = nullptr.
}

// 0x7d7f0c render_handler_glitch::~render_handler_glitch(void)   [D0, deleting destructor]
// SKIPPED: the deleting destructor is generated by the compiler from ~render_handler_glitch() above. It calls
// the D1 body and then frees the object.
