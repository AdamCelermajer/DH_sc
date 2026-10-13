// Batch v1-vendor-gameswf-render_handler_glitch-fd003c-2: render_handler_glitch, part 3 of 3 (10 functions).
//
// Layout and declarations: vendor-gameswf/include/vendor-gameswf/render_handler_glitch.hpp (shared with parts 1 and 2),
// and the float helpers in vendor-gameswf/include/vendor-gameswf/render_handler_glitch_detail.hpp (tested by
// tests/v1-vendor-gameswf-render_handler_glitch-fd003c-2_test.cpp).
//
// The render cache is not written by this batch: gameswf::render_cache is owned by the pack6 batch (its header is
// render_cache.hpp), and that header does not yet type the entry, vertex and uv blocks the way draw_bitmap and draw use
// them. Those two functions are partial until the layout is agreed (see their notes).
//
// Checked against the assembly (tools/decomp_asm.py) where the pseudocode was ambiguous:
//   - BufferedRenderer::flush is flush(void) (the symbol and the call sites pass only this). The second register that
//     the pseudocode shows is a pass-through of the caller's r1 and is not an argument.
//   - the mask depth increment is single precision (__aeabi_fadd with 0x3a83126f = 0.001f).
//   - the mesh gate byte at this+0x3b7 is colour alpha of fill side 0 (m_fill_styles[0].color[3]).
//   - draw_mesh_primitive calls fill_style::apply and get_matrix in the order apply, get_matrix, then the transform.
//   - draw_bitmap copies 48 staging entries into the render cache (asm bound 0x480); see its note.
#include "vendor-gameswf/render_handler_glitch.hpp"

#include <cstdint>
#include <cstring>

#include "gameswf/gameswf_types.h"  // gameswf::rgba (full definition)
#include "glitch-video/texture.hpp"
#include "glitch-video/vertex_streams.hpp"
#include "vendor-gameswf/bitmap_info_ogl.hpp"
#include "vendor-gameswf/render_handler_glitch_detail.hpp"

namespace {

using glitch::core::vector2d;
using render_handler_glitch_detail::kMaskBlendIndex;
using render_handler_glitch_detail::kQuadIndices;
using render_handler_glitch_detail::QuadPositions;

// The mask stack and the scratch points are glitch::core::vector2d<float> in the binary.
using Point = vector2d<float>;

// Primitive codes as the pseudocode passes them: 4 is the strip (draw_mesh_strip, and the strip branch of
// process_mask_intersection), 6 is the triangle list (the quads and draw_triangle_list). The enumerators are not
// recovered; the codes are the literal values from the pseudocode.
constexpr glitch::video::E_PRIMITIVE_TYPE kPrimitiveStrip = 4;
constexpr glitch::video::E_PRIMITIVE_TYPE kPrimitiveTriangles = 6;

}  // namespace

// 0x7d81e4 _ZN21render_handler_glitch14set_blend_modeEN7gameswf10blend_mode2idE
// Switches the material slot used by the next quads. Nothing happens when the current slot is the mask slot. While
// masks are open, the buffer is flushed and the mask state cleared before the switch, and set again afterwards. The
// pseudocode's second flush argument is not an argument of BufferedRenderer::flush (see the file comment).
void render_handler_glitch::set_blend_mode(gameswf::blend_mode::id mode) {
    const int new_mode = static_cast<int>(mode);
    if (m_buffered.m_materialIndex == kMaskBlendIndex) {
        return;
    }
    int current = m_buffered.m_materialIndex;  // +0x2fc (764)
    if (m_mask_depth > 0) {                    // +0x344 (836)
        m_buffered.flush();
        m_buffered.applyMaskState(0);
        current = m_buffered.m_materialIndex;
    }
    if (current != new_mode) {
        m_buffered.flush();
    }
    m_buffered.m_materialIndex = new_mode;
    if (m_mask_depth > 0) {
        m_buffered.applyMaskState(2);
    }
}

// 0x7d825c _ZN21render_handler_glitch17begin_submit_maskEv
// Opens a mask level. The buffer is flushed (twice when the slot is not the mask slot), the slot is set to the mask
// slot, the depth goes up by one and the mask z (+0x348) by 0.001f. A new, empty mask run is pushed when the depth is
// past the end of the stack. The mask state 1 is then applied.
void render_handler_glitch::begin_submit_mask() {
    m_buffered.flush();
    if (m_buffered.m_materialIndex != kMaskBlendIndex) {
        m_buffered.flush();
    }
    m_buffered.m_materialIndex = kMaskBlendIndex;
    const int depth = m_mask_depth + 1;
    m_mask_depth = depth;
    m_mask_z = m_mask_z + 0.001f;
    if (depth > m_mask_stack.size()) {
        // The pseudocode builds a zeroed run (empty points, flag 0) and pushes it. Its zero-fill and reserve steps
        // act on an empty local array and change nothing.
        MaskRun run;
        m_mask_stack.push_back(run);
    }
    m_buffered.applyMaskState(1);
}

// 0x7d8340 _ZN21render_handler_glitch12disable_maskEv
// PARTIAL. Closes the innermost mask level: the buffer is flushed, the depth goes down by one, the mask state 0 is
// applied and the run that was closed is emptied. While a mask is still open, the original then changes the glitch
// material of the current slot (its technique pass state, the colour-write mask through glitch::video::setColorMask,
// and render_mask_intersection between two colour-mask writes). Those steps are not recovered: the material pass
// state (CMaterial +0x04 owner, the technique table at owner +0x18, the pass words and their dirty bytes) and the
// free function setColorMask are not in the tree, so the rest of the body is missing.
void render_handler_glitch::disable_mask() {
    m_buffered.flush();
    --m_mask_depth;
    m_buffered.applyMaskState(0);
    m_mask_stack[m_mask_depth].points.resize(0);
    if (m_mask_depth > 0) {
        // NOT RECOVERED: material pass-state updates on the slot (see the comment above), then
        // render_mask_intersection(), then the colour mask set back to (1, 1, 1, 1).
    }
}

// 0x7d85bc _ZN21render_handler_glitch15end_submit_maskEv
// Closes the mask submission. The mask intersection runs first when more than one level is open. The buffer is
// flushed (twice when the slot is not zero), the slot is set to zero and the mask state 2 is applied.
void render_handler_glitch::end_submit_mask() {
    if (m_mask_depth > 1) {
        render_mask_intersection();
    }
    m_buffered.flush();
    if (m_buffered.m_materialIndex != 0) {
        m_buffered.flush();
    }
    m_buffered.m_materialIndex = 0;
    m_buffered.applyMaskState(2);
}

// 0x7d860c _ZN21render_handler_glitch25process_mask_intersectionEP6VertexiPKtiN6glitch5video16E_PRIMITIVE_TYPEE
// PARTIAL. Turns a draw into triangle points for the mask stack. Returns false when nothing is to be done (no mask
// open, the slot is not the mask slot, or fewer than three vertices), in which case the caller draws the quads or
// mesh normally. Otherwise:
//   - the scratch points (+0x35c) get the triangle list of the draw: a strip is expanded to triangles (t, t+1, t+2),
//     an indexed list gathers its vertices through the indices, and a plain list copies its vertices;
//   - at depth 1 the scratch points are appended to the first mask run and the result is false;
//   - at depth 2 or more, each scratch triangle is intersected with every triangle of the run below it
//     (gameswf::triangle_intersector::process, 0x7889ec), and the intersection points are appended to the current run
//     as triangles (a strip of the result is expanded to a list); the result is true.
// NOT RECOVERED: triangle_intersector::process. Its class is 4 bytes at +0x36c and its body is not in the tree, so
// the stub below yields no intersection points. Every depth-2-or-more result is therefore wrong until it is replaced.
// The pseudocode pushes three result points whenever the count is positive; a count of 1 or 2 would read past the
// result, so only counts of 3 or more are pushed here.
namespace {

// Stand-in for gameswf::triangle_intersector::process(point*, point*, array<point>&). Clears the result and produces
// no points. Replace with the recovered process when the class is in the tree.
void intersect_triangle_pair(const Point* a, const Point* b, ::array<Point>& out) {
    (void)a;
    (void)b;
    out.resize(0);
}

}  // namespace

bool render_handler_glitch::process_mask_intersection(Vertex* vertices, int vertex_count, const uint16_t* indices,
                                                      int index_count,
                                                      glitch::video::E_PRIMITIVE_TYPE primitive) {
    if (m_mask_depth <= 0 || m_buffered.m_materialIndex != kMaskBlendIndex || vertex_count <= 2) {
        return false;
    }

    m_triangle_points.resize(0);
    int triangles = 0;
    if (primitive == kPrimitiveStrip) {
        // Strip of n vertices: n - 2 triangles, (t, t + 1, t + 2) for t = 0 .. n - 3. The pseudocode writes the first
        // triangle, then one more vertex per step, which is the same sequence.
        triangles = vertex_count - 2;
        for (int t = 0; t < triangles; ++t) {
            for (int k = 0; k < 3; ++k) {
                const Vertex& corner = vertices[t + k];
                m_triangle_points.push_back(Point{corner.x, corner.y});
            }
        }
    } else if (indices != nullptr) {
        triangles = index_count / 3;
        for (int k = 0; k < index_count; ++k) {
            const Vertex& corner = vertices[indices[k]];
            m_triangle_points.push_back(Point{corner.x, corner.y});
        }
    } else {
        triangles = vertex_count / 3;
        for (int k = 0; k < vertex_count; ++k) {
            m_triangle_points.push_back(Point{vertices[k].x, vertices[k].y});
        }
    }

    const int depth = m_mask_depth;
    if (depth <= 1) {
        // First mask level: the scratch points are the region of run 0.
        MaskRun& first = m_mask_stack[depth - 1];
        for (int i = 0; i < m_triangle_points.size(); ++i) {
            first.points.push_back(m_triangle_points[i]);
        }
        return false;
    }

    MaskRun& current = m_mask_stack[depth - 1];
    MaskRun& parent = m_mask_stack[depth - 2];
    for (int t = 0; t < triangles; ++t) {
        ::array<Point> out;
        const Point* tri = &m_triangle_points[3 * t];
        const int parent_triangles = parent.points.size() / 3;
        for (int u = 0; u < parent_triangles; ++u) {
            const Point* parent_tri = &parent.points[3 * u];
            intersect_triangle_pair(tri, parent_tri, out);
            const int n = out.size();
            if (n >= 3) {
                current.points.push_back(out[0]);
                current.points.push_back(out[1]);
                current.points.push_back(out[2]);
                for (int k = 3; k < n; ++k) {
                    current.points.push_back(out[k - 2]);
                    current.points.push_back(out[k - 1]);
                    current.points.push_back(out[k]);
                }
            }
        }
    }
    return true;
}

// 0x7d8df4 _ZN21render_handler_glitch11draw_bitmapERKN7gameswf6matrixEPNS0_11bitmap_infoERKNS0_4rectES8_NS0_4rgbaE
// PARTIAL. Draws one bitmap quad. Nothing is drawn when the colour's alpha is zero. Otherwise the four corners come
// from the matrix and the coordinate rect (quad_positions), the texture coordinates from the uv rect, and every
// staging entry gets the current depth and the colour. The quad is snapped to whole pixels when the byte at +0x04
// is set. The bitmap's texture gets wrap mode 1 and is bound to the buffer, the quad is handed to process_mask_intersection
// and, when that returns false, to queueIndexedTriangles.
// NOT RECONSTRUCTED: the render cache record, which the pseudocode appends when m_render_cache is set. The record
// layout is not agreed in the shared render_cache header (see the note in the body). The pseudocode's cache copy also
// loops 48 staging entries (asm 0x7d9294, bound 0x480) while the entry counts four; that bound needs checking too.
void render_handler_glitch::draw_bitmap(const gameswf::matrix& m, gameswf::bitmap_info* bi,
                                        const gameswf::rect& coords, const gameswf::rect& uv_coords,
                                        gameswf::rgba color) {
    if (color.m_a == 0) {
        return;
    }
    const float rect[4] = {coords.m_x_min, coords.m_x_max, coords.m_y_min, coords.m_y_max};
    const float uv[4] = {uv_coords.m_x_min, uv_coords.m_x_max, uv_coords.m_y_min, uv_coords.m_y_max};
    const float mat[6] = {m.m_[0][0], m.m_[0][1], m.m_[0][2], m.m_[1][0], m.m_[1][1], m.m_[1][2]};
    const QuadPositions quad = render_handler_glitch_detail::quad_positions(mat, rect);

    bi->layout();
    gameswf::bitmap_info_ogl* ogl = static_cast<gameswf::bitmap_info_ogl*>(bi);
    if (ogl->m_texture) {
        ogl->m_texture->setWrap(1);
    }
    m_buffered.setTexture(ogl->m_texture);

    Vertex* staging = reinterpret_cast<Vertex*>(m_vertices);
    const float depth = m_mask_z;
    staging[0].u = uv[0];
    staging[0].v = uv[2];
    staging[1].u = uv[1];
    staging[1].v = uv[2];
    staging[2].u = uv[0];
    staging[2].v = uv[3];
    staging[3].u = uv[1];
    staging[3].v = uv[3];

    // The packed colour is the rgba word the pseudocode passes as a6 (the bytes r, g, b, a in memory order).
    uint32_t packed = 0;
    std::memcpy(&packed, &color, sizeof(packed));
    for (int i = 0; i < 4; ++i) {
        staging[i].x = quad.x[i];
        staging[i].y = quad.y[i];
        staging[i].z = depth;
        staging[i].color = packed;
        if (unk_04 != 0) {
            staging[i].x = render_handler_glitch_detail::snap_to_pixel(staging[i].x);
            staging[i].y = render_handler_glitch_detail::snap_to_pixel(staging[i].y);
        }
    }

    m_vertex_streams->m_field08 = 4;
    const bool masked = process_mask_intersection(staging, 4, kQuadIndices, 6, kPrimitiveTriangles);
    if (!masked) {
        m_buffered.queueIndexedTriangles(m_vertex_streams, kQuadIndices, 6);
    }

    // NOT RECONSTRUCTED: the render cache record (see the note above). With m_render_cache set, the pseudocode appends
    // six indices, four vertices (x, y, z), four uv pairs and one entry (bitmap, colour, vertex and index bases,
    // counts 4 and 6). The shared render_cache header (vendor-gameswf/include/vendor-gameswf/render_cache.hpp, batch
    // pack6) types those words as an entry of six int32 words and as owned blocks with a void* buffer, so a bitmap
    // pointer and typed vertex and uv arrays cannot be stored there yet. Nothing is recorded by this rebuild.
    (void)m_render_cache;
}

// 0x7d93ac _ZN21render_handler_glitch4drawEPN7gameswf12render_cacheE
// Replays a render cache. For each entry: the entry's bitmap texture is bound to the buffer; while a mask is open the
// cache's vertices of the entry get the current depth as z; the staging buffer is grown to the entry's vertex count;
// each staging entry gets the entry's colour byte; the entry's indices go through process_mask_intersection, and when
// that returns false, into queueIndexedTriangles. The staging positions are not copied from the cache in this
// function (the pseudocode and the assembly at 0x7d93ac have no such loop), so the replay draws whatever positions
// the staging buffer holds.
// PARTIAL: the replay loop is written in the pseudocode's order in the comment below, but the body is not compiled
// yet. Each entry holds a bitmap pointer (bitmap +0x10 is the texture), a colour byte, a vertex base and count, and an
// index base and count. The shared render_cache header (batch pack6) types the entries as six int32 words and the
// vertex and uv blocks as owned blocks, so none of those fields can be read as typed values yet. Layout request in the
// batch notes: entry {bitmap_info* texture; uint32_t colour; int vertex_base, vertex_count, index_base, index_count}
// (24 bytes at +0x10, array<entry>), vertices array<vector3df> at +0x20 with borrowed flag +0x2c, uvs array<vector2df>
// at +0x30 with borrowed flag +0x3c, indices array<uint16_t> at +0x40 (already typed in that header).
//
// Pseudocode order, for the integrator: for each entry, bind its texture to the buffer; while masks are open
// (depth > 0) set the z of the entry's cache vertices to m_mask_z; ensureBufferCapacity(vertex_count); set the low byte
// of each staging colour from the entry colour; set the streams' vertex count; process_mask_intersection on the entry's
// indices (6 for a quad) and queueIndexedTriangles when that returns false. The pseudocode has no loop that copies
// cache positions into the staging buffer (checked in the assembly of 0x7d93ac), so the replay draws staging positions.
void render_handler_glitch::draw(gameswf::render_cache* cache) {
    (void)cache;
}

// 0x7d94f8 _ZN21render_handler_glitch19draw_mesh_primitiveEiPKviPKti
// Draws a mesh given as (x, y) pairs. Nothing is done unless fill side 0's colour alpha is non-zero. The staging buffer
// is grown to count + 2; each entry takes x and y from the input and the current depth as z. Fill side 0 applies its
// state to the staging entries, the matrix is taken from m_matrix, and the entries are transformed by it (x, y, z).
// The result then goes through process_mask_intersection and, when that returns false, to queueBuffer (no indices, or
// no index count) or to queueIndexedTriangles.
void render_handler_glitch::draw_mesh_primitive(glitch::video::E_PRIMITIVE_TYPE primitive, const void* coords,
                                                int vertex_count, const uint16_t* indices, int index_count) {
    // The gate is the byte at this+0x3b7, which is colour alpha of fill side 0 (m_fill_styles[0].color[3]).
    if (m_fill_styles[0].color[3] == 0) {
        return;
    }
    ensureBufferCapacity(vertex_count + 2);

    Vertex* staging = reinterpret_cast<Vertex*>(m_vertices);
    const float* input = static_cast<const float*>(coords);
    for (int i = 0; i < vertex_count; ++i) {
        staging[i].x = input[2 * i];
        staging[i].y = input[2 * i + 1];
        staging[i].z = m_mask_z;
    }

    m_fill_styles[0].apply(m_driver, m_buffered, staging, vertex_count);
    const TransformState world = get_matrix(m_matrix);
    for (int i = 0; i < vertex_count; ++i) {
        render_handler_glitch_detail::transform_xyz(world.m_elems, staging[i].x, staging[i].y, staging[i].z);
    }

    m_vertex_streams->m_field08 = static_cast<uint32_t>(vertex_count);
    if (!process_mask_intersection(staging, vertex_count, indices, index_count, primitive)) {
        const bool plain = (indices == nullptr) || (index_count == 0);
        if (plain) {
            m_buffered.queueBuffer(m_vertex_streams, primitive);
        } else {
            m_buffered.queueIndexedTriangles(m_vertex_streams, indices, index_count);
        }
    }
}

// 0x7d9734 _ZN21render_handler_glitch18draw_triangle_listEPKviPKti
// Triangle list with indices: draw_mesh_primitive with the triangle-list primitive.
void render_handler_glitch::draw_triangle_list(const void* coords, int vertex_count, const uint16_t* indices,
                                               int index_count) {
    draw_mesh_primitive(kPrimitiveTriangles, coords, vertex_count, indices, index_count);
}

// 0x7d9764 _ZN21render_handler_glitch15draw_mesh_stripEPKvi
// Triangle strip without indices: draw_mesh_primitive with the strip primitive and no index buffer.
void render_handler_glitch::draw_mesh_strip(const void* coords, int vertex_count) {
    draw_mesh_primitive(kPrimitiveStrip, coords, vertex_count, nullptr, 0);
}
