#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::engine_camera::plane_intersection {

using Word = std::uint32_t;
using Address = std::uintptr_t;

struct Float64Words { Word low; Word high; };

using Float32Binary = Word (*)(void*, Word, Word);
using Float32Predicate = Word (*)(void*, Word, Word);
using Float32To64 = void (*)(void*, Word, Float64Words*);
using Float64To32 = Word (*)(void*, Float64Words);
using Float64Binary = void (*)(void*, Float64Words, Float64Words, Float64Words*);
using Float64Unary = void (*)(void*, Float64Words, Float64Words*);
using Float64Predicate = Word (*)(void*, Float64Words, Float64Words);

// These are the exact imported floating-point operation boundaries observed
// in the three original plane-intersection bodies.
struct MathServices {
    void* context;
    std::size_t context_extent;
    Float32Binary multiply;
    Float32Binary add;
    Float32Binary subtract;
    Float32Binary divide;
    Float32Predicate equal;
    Float32To64 to_double;
    Float64To32 to_float;
    Float64Unary square_root;
    Float64Binary multiply_double;
    Float64Binary divide_double;
    Float64Predicate less_double;
};

enum class Status : std::int32_t { complete, invalid_argument };

struct CallResult {
    Status status;
    Word source_boolean;
};

// Planes are four raw words [normal.x, normal.y, normal.z, D]; vectors are
// three raw binary32 words. Output is not touched when the source returns false.
CallResult intersect_line(const Word* plane, const Word* line_point,
                          const Word* line_vector, Word* output,
                          const MathServices*);
CallResult intersect_two_planes(const Word* first, const Word* second,
                                Word* line_point, Word* line_vector,
                                const MathServices*);
CallResult intersect_three_planes(const Word* first, const Word* second,
                                  const Word* third, Word* output,
                                  const MathServices*);

// Adapter for frustum_bounds::IntersectThreePlanes. `context` must point to a
// live MathServices object; checked callers should use intersect_three_planes.
Word intersect_three_planes_callback(void* context, const Word* first,
                                     const Word* second, const Word* third,
                                     Word output[3]);

} // namespace dh2::engine_camera::plane_intersection
