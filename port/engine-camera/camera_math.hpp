#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::engine_camera::camera_math {

using Word = std::uint32_t;
using Binary32Binary = Word (*)(void*, Word, Word);
using Binary32Unary = Word (*)(void*, Word);

struct Vector3 { Word xyz[3]; };
// glitch::core::CMatrix4<float> has 16 float words and a one-byte identity
// hint. The source copies exactly 0x41 bytes, not the host struct's padded
// sizeof value.
struct Matrix4 { Word elements[16]; std::uint8_t definitely_identity; };
static_assert(offsetof(Matrix4, definitely_identity) == 64);

struct MathServices {
    void* context = nullptr;
    std::size_t context_extent = 0;
    Binary32Binary add = nullptr;
    Binary32Binary subtract = nullptr;
    Binary32Binary multiply = nullptr;
    Binary32Binary divide = nullptr;
    Binary32Binary equal = nullptr;
    Binary32Unary square_root = nullptr;
};

enum class Status : std::int32_t { complete, invalid_argument };

// Source helper glitch::core::buildCameraLookAtMatrix<float> at 0x582e64.
// Imported arithmetic is explicit because the original's operation order and
// binary32 rounding are part of the matrix producer.
Status build_look_at(const Vector3*, const Vector3*, const Vector3*, Matrix4*,
                     const MathServices*);

} // namespace dh2::engine_camera::camera_math
