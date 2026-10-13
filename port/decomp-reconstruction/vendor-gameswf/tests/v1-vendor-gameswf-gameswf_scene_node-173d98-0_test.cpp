// Test for batch v1-vendor-gameswf-gameswf_scene_node-173d98-0 (gameswf::scene_node).
// Run with check_tu.sh --test, passing this file, the batch source and the glitch-core-io CMatrix4 source.
//
// Covered: assign_corner_indices (the tail of init_corners), the bounding box accessor, get_world_mouse with
// its update step, the copy in update_inverse_transform, and a ray hit through get_collision_uv.
//
// The test supplies one callee that is not recovered: ISceneNode vtable +0x38 (the parent matrix), read through
// gameswf::scene_node::parentTransform(). It returns g_parent_matrix, which each case sets.
//
// The tu_string members below are stubs. The header's static data pulls their comdats into this object, but the
// test never creates a tu_string, so the stubs are not called.

#include <cmath>
#include <cstdint>
#include <cstdio>

#include "vendor-gameswf/gameswf_scene_node.hpp"

namespace {

int g_failures = 0;
glitch::core::CMatrix4<float> g_parent_matrix(glitch::core::CMatrix4<float>::EM4CONST_IDENTITY);

void check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

bool near(float a, float b) {
    return std::fabs(a - b) < 1e-5f;
}

}  // namespace

// Test stub for the parent matrix accessor (not recovered; see the header).
namespace gameswf {
glitch::core::CMatrix4<float> scene_node::parentTransform() const {
    return g_parent_matrix;
}
}  // namespace gameswf

// Stubs for tu_string members that are referenced only by dead comdats (see the header comment above).
void tu_string::resize(int) {}
int tu_string::utf8_char_count(const char*, int) { return 0; }
void tu_string::operator+=(const char*) {}
void tu_string::operator+=(const tu_string&) {}
bool tu_string::operator==(const tu_string&) const { return false; }

int main() {
    using gameswf::scene_node;
    using glitch::core::vector3d;

    // assign_corner_indices: the four quadrants, a boundary value and a NaN u.
    {
        const float uv[] = {
            0.1f, 0.2f,   // j0: u low,  v low   -> corner[1]
            0.1f, 0.9f,   // j1: u low,  v high  -> corner[0]
            0.9f, 0.2f,   // j2: u high, v low   -> corner[3]
            0.9f, 0.9f,   // j3: u high, v high  -> corner[2]
            0.5f, 0.5f,   // j4: u == 0.5 counts as high, v == 0.5 counts as low -> corner[3]
        };
        std::uint16_t corner[4] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD};
        scene_node::assign_corner_indices(uv, 5, corner);
        check(corner[0] == 1, "corner[0] is point 1 (u low, v high)");
        check(corner[1] == 0, "corner[1] is point 0 (u low, v low)");
        check(corner[2] == 3, "corner[2] is point 3 (u high, v high)");
        check(corner[3] == 4, "corner[3] is point 4 (boundary, last write wins)");
    }
    {
        // A NaN u is not below 0.5, so it takes the high-u branch.
        const float nanU = std::nanf("");
        const float uv[] = {nanU, 0.2f};
        std::uint16_t corner[4] = {0, 0, 0, 0};
        scene_node::assign_corner_indices(uv, 1, corner);
        check(corner[3] == 0 && corner[0] == 0 && corner[1] == 0 && corner[2] == 0,
              "NaN u writes index 0 into the high-u low-v slot only");
    }
    {
        // Count zero writes nothing; a partial count writes only its own points and keeps the other slots.
        std::uint16_t corner[4] = {7, 8, 9, 10};
        const float uv[] = {0.1f, 0.2f, 0.9f, 0.9f};
        scene_node::assign_corner_indices(uv, 0, corner);
        check(corner[0] == 7 && corner[1] == 8 && corner[2] == 9 && corner[3] == 10, "count 0 changes nothing");
        scene_node::assign_corner_indices(uv, 1, corner);
        check(corner[1] == 0 && corner[0] == 7 && corner[2] == 9 && corner[3] == 10,
              "count 1 writes only the slot of point 0");
    }

    // Bounding box accessor: the box at +0x21c, MinEdge (-1,-1,-1), MaxEdge (1,1,1).
    {
        scene_node node(nullptr, nullptr, glitch::core::dimension2d<int>{0, 0});
        const glitch::core::aabbox3d<float>& box = node.getBoundingBox();
        check(&box == &node.m_bounding_box, "getBoundingBox returns the member");
        check(near(box.MinEdge.X, -1.f) && near(box.MinEdge.Y, -1.f) && near(box.MinEdge.Z, -1.f),
              "bounding box MinEdge is (-1,-1,-1)");
        check(near(box.MaxEdge.X, 1.f) && near(box.MaxEdge.Y, 1.f) && near(box.MaxEdge.Z, 1.f),
              "bounding box MaxEdge is (1,1,1)");
    }

    // Constructor defaults and get_world_mouse: the mouse position is truncated toward zero and the update
    // copies the parent matrix (translation 5,6,7 here).
    {
        scene_node node(nullptr, nullptr, glitch::core::dimension2d<int>{320, 480});
        check(node.m_texture_param_id == 0xffff, "texture param id starts at 0xffff");
        check(node.m_last_viewport_w == -1.f && node.m_last_viewport_h == -1.f, "viewport starts at -1");
        check(node.m_parent_transform.flag, "parent transform starts as the identity (flag set)");
        check(near(node.m_parent_transform.M[0], 1.f) && near(node.m_parent_transform.M[15], 1.f) &&
                  near(node.m_parent_transform.M[12], 0.f),
              "parent transform starts as the identity");

        g_parent_matrix = glitch::core::CMatrix4<float>(glitch::core::CMatrix4<float>::EM4CONST_IDENTITY);
        g_parent_matrix.M[12] = 5.f;
        g_parent_matrix.M[13] = 6.f;
        g_parent_matrix.M[14] = 7.f;

        node.m_mouse_x = 12.7f;
        node.m_mouse_y = -3.2f;
        int x = 0;
        int y = 0;
        const int result = node.get_world_mouse(x, y);
        check(x == 12, "get_world_mouse x truncates 12.7 to 12");
        check(y == -3 && result == -3, "get_world_mouse y truncates -3.2 to -3 and returns it");
        check(near(node.m_parent_transform.M[12], 5.f) && near(node.m_parent_transform.M[13], 6.f) &&
                  near(node.m_parent_transform.M[14], 7.f),
              "update_inverse_transform copies the parent matrix");
    }

    // get_collision_uv: one triangle in the z = 0 plane (0,0,0), (1,0,0), (0,1,0) with the corner points
    // P1 = (0,0,0) at index 1, P2 = (1,0,0) at index 3, P3 = (0,1,0) at index 0. The uv of a hit (x, y) is (x, y).
    {
        scene_node node(nullptr, nullptr, glitch::core::dimension2d<int>{0, 0});
        g_parent_matrix = glitch::core::CMatrix4<float>(glitch::core::CMatrix4<float>::EM4CONST_IDENTITY);

        node.m_triangles[0].A = vector3d<float>{0.f, 0.f, 0.f};
        node.m_triangles[0].B = vector3d<float>{1.f, 0.f, 0.f};
        node.m_triangles[0].C = vector3d<float>{0.f, 1.f, 0.f};
        node.m_corner_points[0] = vector3d<float>{0.f, 1.f, 0.f};   // P3
        node.m_corner_points[1] = vector3d<float>{0.f, 0.f, 0.f};   // P1
        node.m_corner_points[3] = vector3d<float>{1.f, 0.f, 0.f};   // P2
        node.m_corner[0] = 0;
        node.m_corner[1] = 1;
        node.m_corner[3] = 3;

        glitch::core::line3d<float> ray{vector3d<float>{0.25f, 0.5f, 1.f}, vector3d<float>{0.25f, 0.5f, -1.f}};
        gameswf::point uv(0.f, 0.f);
        const bool hit = node.get_collision_uv(ray, uv);
        check(hit, "ray through the triangle hits");
        check(near(uv.m_x, 0.25f) && near(uv.m_y, 0.5f), "uv of the hit is (0.25, 0.5)");

        // A ray that passes beside the triangle (x = 2) misses and leaves the uv alone.
        glitch::core::line3d<float> miss{vector3d<float>{2.f, 0.5f, 1.f}, vector3d<float>{2.f, 0.5f, -1.f}};
        gameswf::point untouched(9.f, 9.f);
        check(!node.get_collision_uv(miss, untouched), "ray beside the triangle misses");
        check(untouched.m_x == 9.f && untouched.m_y == 9.f, "a miss leaves the uv unchanged");
    }

    if (g_failures == 0) {
        std::printf("scene_node test: all checks passed\n");
        return 0;
    }
    std::printf("scene_node test: %d failure(s)\n", g_failures);
    return 1;
}
