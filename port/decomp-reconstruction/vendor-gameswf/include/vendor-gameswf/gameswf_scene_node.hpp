// gameswf::scene_node (Gameloft, not in upstream GameSWF r1714).
//
// A scene node that draws one character's display list through a glitch scene node. In the binary the
// class derives from glitch::scene::ISceneNode (its vtable is the first word of the object) and has a
// second IReferenceCounted-shaped subobject at +0x268 (vptr and count, count 1 from the complete-object
// constructor). Neither base is complete in this tree: glitch-scene-collada-gui/scene_node.hpp maps only
// some ISceneNode members, and that map disagrees with the binary (animators at +0xfc and bindedAnimators
// at +0x104 overlap). So the base is kept as raw bytes (pad_0000), the derived members are declared at
// their binary offsets, and the code uses named members only. The offsets are comments for checking
// against the binary; no pointer arithmetic is done on them. Object size in the binary: 0x270.
//
// Methods are not virtual here: the binary overrides render, getBoundingBox and the ISceneNode hooks
// through the scene_node vtable, which is not modelled (layout request L1 in the .cpp).
//
// Bodies and status of each row: vendor-gameswf/src/v1-vendor-gameswf-gameswf_scene_node-173d98-0.cpp

#pragma once

#include <cstdint>

#include "base/container.h"                          // ::array (upstream, as texture_cache.hpp uses it)
#include "gameswf/gameswf.h"                         // player, character, root, point, matrix (upstream)
#include "glitch-core-io/boost/intrusive_ptr.hpp"    // boost::intrusive_ptr (counting form, canonical)
#include "glitch-shared/glitch_core.hpp"             // vector3d, triangle3d, line3d, aabbox3d, CMatrix4, dimension2d
#include "glitch-shared/reference_counted.hpp"       // glitch::IReferenceCounted

namespace glitch::scene {
class ISceneNode;
class IMesh;
}  // namespace glitch::scene

namespace glitch::video {
class ITexture;
}  // namespace glitch::video

namespace gameswf {

class scene_node {
public:
    // 0x778840 (C1, complete object) and 0x778c58 (C2, base object; the compiler emits it for the same source
    // constructor because the class has a virtual base). Body in the .cpp; partial.
    scene_node(gameswf::player* player, glitch::scene::ISceneNode* parent, glitch::core::dimension2d<int> size);
    // 0x776244 (D1). 0x776354 (D0, deleting destructor) is compiler-generated and not declared here.
    ~scene_node();

    // 0x776140 render(void*). Empty in the binary.
    void render(void* state);
    // 0x776144 getBoundingBox() const. Returns the box at +0x21c.
    const glitch::core::aabbox3d<float>& getBoundingBox() const;
    // 0x77616c build_dlist(character*). The binary's int return is the reused this register; the source is void.
    void build_dlist(gameswf::character* c);
    // 0x776390 onRegisterSceneNode(). Returns 0 or 1 (Hex-Rays int).
    bool onRegisterSceneNode();
    // 0x776564 get_collision_uv(line3d<float>&, point&). Returns 0 or 1 (Hex-Rays int).
    bool get_collision_uv(glitch::core::line3d<float>& ray, gameswf::point& uv);
    // 0x777350 update_inverse_transform(). Hex-Rays returns the memcpy result from a tail call; the source is void.
    void update_inverse_transform();
    // 0x777514 get_local_mouse(character*, float&, float&).
    void get_local_mouse(gameswf::character* c, float& x, float& y);
    // 0x7775c4 get_world_mouse(int&, int&). Returns the y value, as the binary does.
    int get_world_mouse(int& x, int& y);
    // 0x77825c collect_uvs(boost::intrusive_ptr<IMesh const> const&, point*, unsigned int). Returns the float
    // the Hex-Rays body ends with (a float result register).
    float collect_uvs(const boost::intrusive_ptr<const glitch::scene::IMesh>& mesh, gameswf::point* uv,
                      unsigned int count);
    // 0x7785b0 init_corners().
    void init_corners();

    // Not in the binary (split out of init_corners for testing). Given the uv of up to six corner points
    // (x, y pairs, count points), stores the index of each point in corner[] (see the .cpp for the mapping).
    static void assign_corner_indices(const float* uv, unsigned int count, std::uint16_t corner[4]);

    // Copy of the parent's absolute transform (65 bytes, flag last). Reads ISceneNode vtable +0x38, not recovered.
    glitch::core::CMatrix4<float> parentTransform() const;

    // ---- layout (32-bit offsets in comments) ----
    std::uint8_t pad_0000[0x130];                       // +0x000 glitch::scene::ISceneNode base subobject, not recovered
    glitch::scene::ISceneNode* m_parent = nullptr;      // +0x130 parent node; the ctor takes a reference on it
    glitch::video::ITexture* m_texture = nullptr;       // +0x134 texture created by addTexture (intrusive in the binary)
    glitch::IReferenceCounted* unk_138 = nullptr;       // +0x138 intrusive result of a driver call on m_texture
    std::uint16_t m_texture_param_id = 0xffff;          // +0x13c material parameter id of the texture, 0xffff when none
    glitch::core::triangle3d<float> m_triangles[2];     // +0x140 triangles from CTriangleSelector::getTriangles (max 2)
    glitch::core::vector3d<float> m_corner_points[6];   // +0x188 corner points, 3 per triangle (copied from m_triangles)
    std::uint16_t m_corner[4] = {};                     // +0x1d0 indices into m_corner_points: [0] 464, [1] 466, [2] 468, [3] 470
    glitch::core::CMatrix4<float> m_parent_transform{   // +0x1d8 copy of the parent's matrix (65 bytes, flag at +0x218)
        glitch::core::CMatrix4<float>::EM4CONST_IDENTITY};
    glitch::core::aabbox3d<float> m_bounding_box{       // +0x21c MinEdge (-1,-1,-1), MaxEdge (1,1,1) from the ctor
        glitch::core::vector3d<float>{-1.f, -1.f, -1.f}, glitch::core::vector3d<float>{1.f, 1.f, 1.f}};
    gameswf::player* m_player = nullptr;                // +0x234 player that owns the node
    ::array<gameswf::character*> m_child_list;          // +0x238 child characters (data, count, capacity)
    bool unk_244 = false;                               // +0x244 zeroed by the ctor, not read in this batch
    ::array<gameswf::character*> m_display_list;        // +0x248 display list built by build_dlist
    bool unk_254 = false;                               // +0x254 zeroed by the ctor, not read in this batch
    float m_last_viewport_w = -1.f;                     // +0x258 root width at the last update (-1 until set)
    float m_last_viewport_h = -1.f;                     // +0x25c root height at the last update (-1 until set)
    float m_mouse_x = 0.f;                              // +0x260 mouse x in pixels, from update_inverse_transform
    float m_mouse_y = 0.f;                              // +0x264 mouse y in pixels, from update_inverse_transform
    glitch::IReferenceCounted unk_268;                  // +0x268 virtual-base subobject (vptr, count 1); size 8
};

}  // namespace gameswf
