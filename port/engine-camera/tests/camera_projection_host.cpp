#include "camera_projection.hpp"
#include "projection_matrix.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace {
using namespace dh2::engine_camera;
int checks = 0;
bool near(float a, float b) { return std::fabs(a - b) < 2.0e-6f; }
std::uint32_t bits(float value) {
    std::uint32_t word;
    std::memcpy(&word, &value, sizeof(word));
    return word;
}
float value(std::uint32_t word) {
    float result;
    std::memcpy(&result, &word, sizeof(result));
    return result;
}
#define CHECK(x) do { ++checks; if (!(x)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x); return 1; } } while (0)

struct Calls { camera_projection::CameraNode* expected; int orthogonal; int transforms; std::uint32_t state; };
bool query(void* p, const camera_projection::CameraNode*) noexcept {
    return static_cast<Calls*>(p)->orthogonal != 0;
}
void transform(void* p, void* frustum, std::uint32_t state) noexcept {
    auto& calls = *static_cast<Calls*>(p);
    ++calls.transforms;
    calls.state = state;
    if (frustum != reinterpret_cast<std::uint8_t*>(calls.expected) + 0x168)
        calls.state = 0xffffffffu;
}
}

int main() {
    camera_math::Matrix4 matrix{};
    CHECK(projection_matrix::perspective_fov(bits(1.57079632679f), bits(1.5f),
        bits(0.1f), bits(100.0f), &matrix) == projection_matrix::Status::complete);
    CHECK(near(value(matrix.elements[0]), 2.0f / 3.0f));
    CHECK(near(value(matrix.elements[5]), 1.0f));
    CHECK(near(value(matrix.elements[10]), 100.0f / 99.9f));
    CHECK(value(matrix.elements[11]) == 1.0f);
    CHECK(near(value(matrix.elements[14]), -10.0f / 99.9f));
    CHECK(matrix.definitely_identity == 0);
    CHECK(projection_matrix::fix_up_for_gles_default_orientation(&matrix) ==
          projection_matrix::Status::complete);
    CHECK(near(value(matrix.elements[10]), 100.1f / 99.9f));
    CHECK(near(value(matrix.elements[14]), -20.0f / 99.9f));
    CHECK(projection_matrix::perspective_fov_infinite(bits(1.57079632679f), bits(1.5f),
        bits(0.1f), &matrix) == projection_matrix::Status::complete);
    CHECK(value(matrix.elements[10]) == 1.0f && value(matrix.elements[11]) == 1.0f);
    CHECK(value(matrix.elements[14]) == -0.1f);
    CHECK(projection_matrix::fix_up_for_gles_default_orientation(&matrix) ==
          projection_matrix::Status::complete);
    CHECK(value(matrix.elements[10]) == 1.0f && value(matrix.elements[14]) == -0.2f);
    CHECK(projection_matrix::orthographic(bits(10.0f), bits(8.0f), bits(1.0f),
        bits(11.0f), &matrix) == projection_matrix::Status::complete);
    CHECK(value(matrix.elements[0]) == 0.2f && value(matrix.elements[5]) == 0.25f);
    CHECK(value(matrix.elements[10]) == 0.1f && value(matrix.elements[14]) == -0.1f);
    CHECK(value(matrix.elements[15]) == 1.0f);
    CHECK(projection_matrix::fix_up_for_gles_default_orientation(&matrix) ==
          projection_matrix::Status::complete);
    CHECK(value(matrix.elements[10]) == 0.2f && value(matrix.elements[14]) == -1.2f);

    camera_projection::CameraNode node{};
    Calls calls{&node, 0, 0, 0};
    camera_projection::Services services{&calls, sizeof(calls), &query, &transform};
    node.fov = bits(1.57079632679f);
    node.aspect = bits(1.5f);
    node.near_plane = bits(0.1f);
    node.far_plane = bits(100.0f);
    CHECK(camera_projection::recalculate_projection_matrix(&node, &services) == camera_projection::Status::complete);
    CHECK(calls.transforms == 1 && calls.state == 2);
    CHECK(near(value(node.projection_matrix.elements[10]), 100.0f / 99.9f));
    node.far_at_infinity = 1;
    CHECK(camera_projection::recalculate_projection_matrix(&node, &services) == camera_projection::Status::complete);
    CHECK(calls.transforms == 2 && value(node.projection_matrix.elements[14]) == -0.1f);
    calls.orthogonal = 1;
    node.orthographic_half_height = bits(4.0f);
    CHECK(camera_projection::recalculate_projection_matrix(&node, &services) == camera_projection::Status::complete);
    CHECK(calls.transforms == 3 && near(value(node.projection_matrix.elements[0]), 1.0f / 6.0f));
    CHECK(value(node.projection_matrix.elements[5]) == 0.25f);
    std::puts("camera projection host: PASS");
    return 0;
}
