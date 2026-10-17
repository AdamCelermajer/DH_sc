#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::engine_camera::frustum {

using Word = std::uint32_t;
using Address = std::uintptr_t;

struct Matrix4Words {
    Word elements[16];
};

struct PlaneSet {
    // Source order; each plane is [a,b,c,d], binary32 words.
    Word coefficients[6][4];
};

using Binary32Binary = Word (*)(void*, Word, Word);
using Binary32Unary = Word (*)(void*, Word);

struct MathServices {
    void* context;
    std::size_t context_extent;
    Binary32Binary add;
    Binary32Binary subtract;
    Binary32Binary multiply;
    Binary32Binary divide;
    Binary32Unary square_root;
};

enum class Status : std::int32_t {
    complete,
    invalid_argument
};

// Reconstructs SViewFrustum::setFrom's six plane stores and normalization.
// Math helpers are explicit imported-libm boundaries; the matrix/output use
// ARM64-safe host pointers while the actual matrix and plane values stay raw
// IEEE-754 binary32 words.
Status set_from(const Matrix4Words*, PlaneSet*, const MathServices*);

} // namespace dh2::engine_camera::frustum
