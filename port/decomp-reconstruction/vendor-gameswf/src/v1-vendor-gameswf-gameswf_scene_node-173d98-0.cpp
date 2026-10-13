// Track B batch v1-vendor-gameswf-gameswf_scene_node-173d98-0: gameswf::scene_node (Gameloft class, 14 rows).
//
// The class and its layout are in vendor-gameswf/include/vendor-gameswf/gameswf_scene_node.hpp.
//
// Rows in this batch:
//   reconstructed   render (empty), getBoundingBox (returns the box at +0x21c), get_world_mouse.
//   partial         ~scene_node (D1), build_dlist, onRegisterSceneNode, get_collision_uv (whole body written; the
//                   parent matrix accessor is not recovered), update_inverse_transform, get_local_mouse,
//                   collect_uvs, init_corners, scene_node C1 (complete-object constructor).
//   skipped         ~scene_node D0 (compiler-generated deleting destructor: D1, then the free), scene_node C2
//                   (base-object constructor the compiler emits for the same source constructor as C1; the
//                   virtual base is set up only by C1, and C1 carries the body).
//
// Not in the binary (split out for testing): gameswf::scene_node::assign_corner_indices, the classification
// tail of init_corners. Its test is tests/v1-vendor-gameswf-gameswf_scene_node-173d98-0_test.cpp.
//
// Layout requests (the header carries the layout that is known; these are not applied anywhere):
//   L1  scene_node vtable. The binary overrides render, getBoundingBox and the ISceneNode hooks through it,
//       and the virtual base at +0x268 has a vtable of its own. Not modelled; the methods are non-virtual.
//   L2  glitch::scene::ISceneNode, the 304-byte base (+0x000..+0x12f): vptr, reference count at +0x04, name,
//       the scene manager and the transform block. glitch-scene-collada-gui/scene_node.hpp maps part of it,
//       but disagrees with the binary (its animators at +0xfc and bindedAnimators at +0x104 overlap), so it is
//       not used. ISceneNode vtable slot +0x38 returns the absolute transform (65 bytes); parentTransform()
//       stands for that call until the slot is named.
//   L3  gameswf::character: +0x3c weak parent reference (weak_ptr<character>, check_proxy), +0x40 the raw
//       parent pointer, +0x54 m_custom (declared in the DH2 header), +0x9b visibility byte, +0xa8/+0xac
//       child data and count, get_world_cxform (float +0x18 is tested by build_dlist).
//   L4  gameswf::custom (character +0x54): +0x68 scene_node* owner. The DH2 custom struct does not declare it yet.
//   L5  gameswf::root: float +0x48 and +0x4c (viewport width and height, compared with m_last_viewport_w/h),
//       int +0x1c and +0x20 (screen to logical scale). Names not recovered.
//   L6  gameswf::player: +0xac (render object) -> +0x28 camera object, vtable +0x14 (unproject a screen point
//       onto a ray), and the chain +0x28 -> +0xd4 -> +0x1c -> +0x2c used by onRegisterSceneNode.
//   L7  glitch::video::CTextureManager::addTexture (texture from the size argument), driver vtable +0x84 (view of
//       a texture), material renderer getParameterID, and IMaterialParameters::setParameter<intrusive_ptr<ITexture>>.
//       None of these are in the tree.
//   L8  glitch::scene::CTriangleSelector (constructed from the mesh, getTriangles(triangle3d* out, max 2, int*
//       count, nullptr)). Not in the tree.
//   L9  glitch::scene::IMesh vtable +0x10 (vertex count) and +0x14 (vertex stream), and the render object
//       vtable +0xf8 (mesh of the node). glitch::video::CVertexStreams: +0x0c flag byte, +0x10 vertex count,
//       +0x17 and +0x18 component attribute and type, +0x24 stream array. glitch::video::IBuffer::map(1) is the
//       read map at 0x5a1adc (confirm the callee address before writing collect_uvs).
//   L10 gameswf::collect<T> (template, the component reader for collect_uvs). Not in the tree.
//   L11 gameswf::scene_node::NODE_NAME (the name set in the constructor): value and type not recovered.
//   L12 The ISceneNode base constructor arguments in C1: id -1, position (0,0,0), rotation (identity quaternion),
//       scale (1,1,1). These are binary facts; the base constructor is not in the tree.
//
// Notes on the Hex-Rays signatures: the mangled names carry no return types, so the return types are taken from
// the pseudocode. Where the pseudocode returns a register left over from a tail call (update_inverse_transform
// returns the memcpy result, build_dlist returns the reused this), the source is void.

#include <algorithm>
#include <limits>

#include "vendor-gameswf/gameswf_scene_node.hpp"

namespace gameswf {

// ---------------------------------------------------------------------------------------------------------------
// 0x776140 _ZN7gameswf10scene_node6renderEPv
// Empty in the binary (ISceneNode render hook; the vtable is not modelled, see L1).
void scene_node::render(void* /*state*/) {
}

// ---------------------------------------------------------------------------------------------------------------
// 0x776144 _ZNK7gameswf10scene_node14getBoundingBoxEv
// Returns the address of the box at this+0x21c (540). The mangled name has no return type; Hex-Rays shows char*.
const glitch::core::aabbox3d<float>& scene_node::getBoundingBox() const {
    return m_bounding_box;
}

// ---------------------------------------------------------------------------------------------------------------
// 0x77616c _ZN7gameswf10scene_node11build_dlistEPNS_9characterE
// PARTIAL: the character checks are not written. Binary behaviour, kept for the integrator:
//   if (c->+0x9b byte) and (get_world_cxform(c)->+0x18 float >= 0):
//     custom = c->+0x54; if (custom && custom->+0x68 == this) append c to m_display_list (count +0x24c)
//     if the character's virtual at vtable +0x08 with argument 2 returns true (name not recovered):
//       for each child c->+0xa8[i] (count c->+0xac): build_dlist(child)
// The Hex-Rays int return is the reused this register; the source is void.
void scene_node::build_dlist(gameswf::character* c) {
    (void)c;
}

// ---------------------------------------------------------------------------------------------------------------
// 0x776244 _ZN7gameswf10scene_nodeD1Ev
// PARTIAL: the binary clears m_child_list and m_display_list (each element set to null, then count 0 and
// reserve(0)), releases unk_138 (+0x138) and m_texture (+0x134), then runs the ISceneNode base destructor.
// The member arrays are destroyed by their own destructors. The release of m_texture is not written: ITexture
// is not a complete type in this tree.
scene_node::~scene_node() {
    if (unk_138) {
        unk_138->drop();
    }
}

// ---------------------------------------------------------------------------------------------------------------
// 0x776390 _ZN7gameswf10scene_node19onRegisterSceneNodeEv
// PARTIAL: not written. Binary behaviour, kept for the integrator:
//   if m_child_list is empty, or no child has its +0x9b byte set: return 0.
//   Otherwise: if m_child_list has one element, copy it to m_display_list; else build_dlist(root movie).
//   Then render through the player's render object (L6): set the scene's state, the begin/end display of each
//   child with its owner pointer (+0x68 of its custom block) cleared for the call and restored after.
//   Returns 1.
bool scene_node::onRegisterSceneNode() {
    return false;
}

// ---------------------------------------------------------------------------------------------------------------
// 0x776564 _ZN7gameswf10scene_node16get_collision_uvERN6glitch4core6line3dIfEERNS_5pointE
// PARTIAL: the body below is complete except for the parent matrix, which is read through ISceneNode vtable +0x38
// (parentTransform, layout request L2). Nothing else is unknown. The test cannot construct the object, so this
// function has no standalone test.
bool scene_node::get_collision_uv(glitch::core::line3d<float>& ray, gameswf::point& uv) {
    // The parent matrix (65 bytes, flag last) is copied to a local and used for the two triangles.
    glitch::core::CMatrix4<float> m = parentTransform();

    // Each vertex: x' = M0 x + M4 y + M8 z + M12, y' = M1 x + M5 y + M9 z + M13, z' = M2 x + M6 y + M10 z + M14.
    auto transform = [&m](const glitch::core::vector3d<float>& p) {
        glitch::core::vector3d<float> r;
        r.X = m.M[12] + (((m.M[0] * p.X) + (m.M[4] * p.Y)) + (m.M[8] * p.Z));
        r.Y = m.M[13] + (((m.M[1] * p.X) + (m.M[5] * p.Y)) + (m.M[9] * p.Z));
        r.Z = m.M[14] + (((m.M[2] * p.X) + (m.M[6] * p.Y)) + (m.M[10] * p.Z));
        return r;
    };
    glitch::core::triangle3d<float> world[2];
    for (int i = 0; i < 2; ++i) {
        world[i].A = transform(m_triangles[i].A);
        world[i].B = transform(m_triangles[i].B);
        world[i].C = transform(m_triangles[i].C);
    }

    // Ray direction (end - start), normalised. The segment's squared length and its bounding box.
    glitch::core::vector3d<float> dir{ray.end.X - ray.start.X, ray.end.Y - ray.start.Y, ray.end.Z - ray.start.Z};
    dir.normalize();

    const glitch::core::vector3d<float>& start = ray.start;
    const glitch::core::vector3d<float>& end = ray.end;
    const float segLen2 = (((start.X - end.X) * (start.X - end.X)) + ((start.Y - end.Y) * (start.Y - end.Y)))
                          + ((start.Z - end.Z) * (start.Z - end.Z));

    float bx0 = start.X, bx1 = end.X;
    if (bx0 >= bx1) std::swap(bx0, bx1);
    float by0 = start.Y, by1 = end.Y;
    if (by0 >= by1) std::swap(by0, by1);
    float bz0 = start.Z, bz1 = end.Z;
    if (bz0 >= bz1) std::swap(bz0, bz1);

    auto dist2 = [](const glitch::core::vector3d<float>& a, const glitch::core::vector3d<float>& b) {
        const float dx = a.X - b.X;
        const float dy = a.Y - b.Y;
        const float dz = a.Z - b.Z;
        return ((dx * dx) + (dy * dy)) + (dz * dz);
    };

    glitch::core::vector3d<float> hit{0.f, 0.f, 0.f};
    float best = std::numeric_limits<float>::max();
    bool found = false;
    for (int i = 0; i < 2; ++i) {
        const glitch::core::triangle3d<float>& tri = world[i];
        const bool xOverlap = (tri.A.X >= bx0 || tri.B.X >= bx0 || tri.C.X >= bx0) &&
                              (bx1 >= tri.A.X || tri.B.X <= bx1 || tri.C.X <= bx1);
        const bool yOverlap = (tri.A.Y >= by0 || tri.B.Y >= by0 || tri.C.Y >= by0) &&
                              (by1 >= tri.A.Y || tri.B.Y <= by1 || tri.C.Y <= by1);
        const bool zOverlap = (tri.A.Z >= bz0 || tri.B.Z >= bz0 || tri.C.Z >= bz0) &&
                              (bz1 >= tri.A.Z || tri.B.Z <= bz1 || tri.C.Z <= bz1);
        if (!(xOverlap && yOverlap && zOverlap)) continue;

        // The binary runs the intersection when the best distance is above any corner's distance to the start.
        if (best > dist2(start, tri.A) || best > dist2(start, tri.B) || best > dist2(start, tri.C)) {
            if (tri.getIntersectionWithLine(start, dir, hit)) {
                const float d = dist2(hit, start);
                if (segLen2 > d && segLen2 > dist2(hit, end) && best > d) {
                    best = d;
                    found = true;
                }
            }
        }
    }
    if (!found) {
        return false;
    }

    // Map the hit point into the parent's space with the inverse matrix.
    m.makeInverse();
    const float hx = hit.X;
    const float hy = hit.Y;
    const float hz = hit.Z;
    const float ly = (((hx * m.M[1]) + (hy * m.M[5])) + (hz * m.M[9])) + m.M[13];
    const float lz0 = (hx * m.M[2]) + (hy * m.M[6]);
    const float lx = (((hx * m.M[0]) + (hy * m.M[4])) + (hz * m.M[8])) + m.M[12];
    const float lz = (lz0 + (hz * m.M[10])) + m.M[14];

    // Edges from corner 1 (index [466]) to corner 3 ([470]) and corner 0 ([464]).
    const glitch::core::vector3d<float>& p1 = m_corner_points[m_corner[1]];
    const glitch::core::vector3d<float>& p2 = m_corner_points[m_corner[3]];
    const glitch::core::vector3d<float>& p3 = m_corner_points[m_corner[0]];
    const float d1x = p2.X - p1.X;
    const float d1y = p2.Y - p1.Y;
    const float d1z = p2.Z - p1.Z;
    const float d2x = p3.X - p1.X;
    const float d2y = p3.Y - p1.Y;
    const float d2z = p3.Z - p1.Z;
    const float qx = lx - p1.X;
    const float qy = ly - p1.Y;
    const float qz = lz - p1.Z;
    uv.m_x = (((d1x * qx) + (d1y * qy)) + (d1z * qz)) / (((d1x * d1x) + (d1y * d1y)) + (d1z * d1z));
    uv.m_y = (((d2x * qx) + (d2y * qy)) + (d2z * qz)) / (((d2x * d2x) + (d2y * d2y)) + (d2z * d2z));
    return true;
}

// ---------------------------------------------------------------------------------------------------------------
// 0x777350 _ZN7gameswf10scene_node24update_inverse_transformEv
// PARTIAL: the root viewport test, the camera unproject and get_collision_uv call (LABEL_2), the screen to
// logical scaling, and the fallback mouse position (-943501312 as a bit pattern) are not written (L4, L5, L6).
// The copy at the end is written: every path of the binary ends by copying the parent matrix into
// m_parent_transform. The viewport save (m_last_viewport_w/h from the root) is not written.
void scene_node::update_inverse_transform() {
    m_parent_transform = parentTransform();
}

// ---------------------------------------------------------------------------------------------------------------
// 0x777514 _ZN7gameswf10scene_node15get_local_mouseEPNS_9characterERfS3_
// PARTIAL: the mapping through a parent's world matrix is not written. The binary takes the parent from the
// character's weak reference (check_proxy, layout L3) and maps (px, py) with matrix::transform_by_inverse. This
// body is correct only when the character has no parent.
void scene_node::get_local_mouse(gameswf::character* c, float& x, float& y) {
    update_inverse_transform();
    const float px = m_mouse_x * 20.0f;   // pixels to twips
    const float py = m_mouse_y * 20.0f;
    (void)c;
    x = px;
    y = py;
}

// ---------------------------------------------------------------------------------------------------------------
// 0x7775c4 _ZN7gameswf10scene_node15get_world_mouseERiS1_
// Updates the inverse transform, then returns the mouse position in pixels, truncated to int. The binary returns
// the y value (the last stored result).
int scene_node::get_world_mouse(int& x, int& y) {
    update_inverse_transform();
    x = static_cast<int>(m_mouse_x);
    const int result = static_cast<int>(m_mouse_y);
    y = result;
    return result;
}

// ---------------------------------------------------------------------------------------------------------------
// 0x77825c _ZN7gameswf10scene_node11collect_uvsERKN5boost13intrusive_ptrIKN6glitch5scene5IMeshEEEPNS_5pointEj
// PARTIAL: not written. Binary behaviour (read from the Hex-Rays body), kept for the integrator:
//   the mesh's virtual at +0x10 must return a nonzero float; the mesh buffer is vtable +0x14 (arg 0), borrowed.
//   Buffer: vertex count at +0x20 (the count is clamped to the argument), stream object at +0x14 (addref),
//   index buffer at +0x18 (IBuffer::map(1), layout L9), index type at +0x2c (1, 2 or 4 bytes per index).
//   Stream object: flag byte +0x0c; when set and the component count (+0x18) is 2, the component type at +0x2e
//   picks gameswf::collect<T> (0 signed char, 1 unsigned char, 2 short, 3 unsigned short, 4 int, 5 unsigned int,
//   6 float) over the stream at +0x24 with count/3 triangles. The float block at stream+0x10 then scales each
//   uv pair: u = s18 * u + s24, v = s1c * v + s28 (L10).
//   Returns the float left in the result register.
float scene_node::collect_uvs(const boost::intrusive_ptr<const glitch::scene::IMesh>& mesh, gameswf::point* uv,
                              unsigned int count) {
    (void)mesh;
    (void)uv;
    (void)count;
    return 0.f;
}

// ---------------------------------------------------------------------------------------------------------------
// 0x7785b0 _ZN7gameswf10scene_node12init_cornersEv
// PARTIAL: the mesh lookup, the CTriangleSelector and its getTriangles call into m_triangles (L7, L8), the copy of
// m_triangles into m_corner_points (triangle i to points 3i..3i+2, 9 floats each), the uv array from collect_uvs
// (L9, L10) and the release of the temporaries are not written. The index assignment that follows is
// assign_corner_indices (below), which the integrator calls with the uv array once collect_uvs is in place.
void scene_node::init_corners() {
}

// ---------------------------------------------------------------------------------------------------------------
// Not in the binary: the tail of init_corners (0x7785b0), split out so it can be tested. For each corner point j
// (uv[2j] = u, uv[2j+1] = v), the binary stores j in one of four slots of corner[]:
//   u <  0.5, v <= 0.5 -> corner[1]   (offset +0x1d2, i.e. 466)
//   u <  0.5, v >  0.5 -> corner[0]   (offset +0x1d0, i.e. 464)
//   u >= 0.5, v <= 0.5 -> corner[3]   (offset +0x1d6, i.e. 470)
//   u >= 0.5, v >  0.5 -> corner[2]   (offset +0x1d4, i.e. 468)
// Other slots keep their value. A NaN u takes the u >= 0.5 branch, as the binary's "u < 0.5" compare does.
void scene_node::assign_corner_indices(const float* uv, unsigned int count, std::uint16_t corner[4]) {
    for (unsigned int j = 0; j < count; ++j) {
        const float u = uv[2 * j];
        const float v = uv[2 * j + 1];
        const bool uLow = u < 0.5f;
        const bool vLow = v <= 0.5f;
        if (uLow) {
            corner[vLow ? 1 : 0] = static_cast<std::uint16_t>(j);
        } else {
            corner[vLow ? 3 : 2] = static_cast<std::uint16_t>(j);
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------
// 0x778840 _ZN7gameswf10scene_nodeC1EPNS_6playerEPN6glitch5scene10ISceneNodeENS3_4core11dimension2dIiEE
// PARTIAL. The member defaults are in the header and match the binary: the matrix is the identity (flag set), the
// box is (-1,-1,-1)..(1,1,1), m_texture_param_id is 0xffff, m_last_viewport is -1, the mouse position and the
// arrays are zero, m_player is the player argument. Not written (binary order):
//   the ISceneNode base constructor (L12) and the vtable writes, the reference on the parent (its count +1),
//   setName(NODE_NAME) (L11), init_corners (written below, partial), CTextureManager::addTexture(size) and the
//   driver calls that follow it (L7), the material parameter binding with m_texture_param_id, and the
//   complete-object virtual base at +0x268 (its count of 1 is the default in the header).
scene_node::scene_node(gameswf::player* player, glitch::scene::ISceneNode* parent,
                       glitch::core::dimension2d<int> size)
    : m_parent(parent), m_player(player) {
    (void)size;
    init_corners();
}

// 0x778c58 _ZN7gameswf10scene_nodeC2EPNS_6playerEPN6glitch5scene10ISceneNodeENS3_4core11dimension2dIiEE
// SKIPPED: the base-object constructor the compiler emits for the same source constructor as C1 (the class has a
// virtual base at +0x268, which only the complete-object constructor sets up). The behaviour is C1's.

// 0x776354 _ZN7gameswf10scene_nodeD0Ev
// SKIPPED: the compiler-generated deleting destructor. It calls D1 and then frees the object.

}  // namespace gameswf
