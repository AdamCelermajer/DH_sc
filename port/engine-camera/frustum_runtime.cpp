#include "frustum_runtime.hpp"
#include "plane_intersection.hpp"

#include <cmath>
#include <cstring>
#include <limits>

namespace dh2::engine_camera::frustum_runtime { namespace {
using Word = std::uint32_t;
using Double = plane_intersection::Float64Words;

static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559);
static_assert(sizeof(double) == 8 && std::numeric_limits<double>::is_iec559);

float as_float(Word word) {
    float value; std::memcpy(&value, &word, sizeof(value)); return value;
}
Word as_word(float value) {
    if (std::isnan(value)) return 0x7fc00000u;
    Word word; std::memcpy(&word, &value, sizeof(word)); return word;
}
double as_double(Double words) {
    const std::uint64_t bits = std::uint64_t(words.low) |
                              (std::uint64_t(words.high) << 32);
    double value; std::memcpy(&value, &bits, sizeof(value)); return value;
}
Double as_words(double value) {
    std::uint64_t bits;
    if (std::isnan(value)) bits = UINT64_C(0x7ff8000000000000);
    else std::memcpy(&bits, &value, sizeof(bits));
    return {Word(bits), Word(bits >> 32)};
}
Word add(void*, Word a, Word b) { return as_word(as_float(a) + as_float(b)); }
Word subtract(void*, Word a, Word b) { return as_word(as_float(a) - as_float(b)); }
Word multiply(void*, Word a, Word b) { return as_word(as_float(a) * as_float(b)); }
Word divide(void*, Word a, Word b) { return as_word(as_float(a) / as_float(b)); }
Word equal(void*, Word a, Word b) { return Word(as_float(a) == as_float(b)); }
Word greater(void*, Word a, Word b) { return Word(as_float(a) > as_float(b)); }
Word less(void*, Word a, Word b) { return Word(as_float(a) < as_float(b)); }
Word square_root_float(void*, Word a) { return as_word(std::sqrt(as_float(a))); }
void to_double(void*, Word a, Double* out) { *out = as_words(double(as_float(a))); }
Word to_float(void*, Double a) { return as_word(float(as_double(a))); }
void square_root(void*, Double a, Double* out) { *out = as_words(std::sqrt(as_double(a))); }
void multiply_double(void*, Double a, Double b, Double* out) {
    *out = as_words(as_double(a) * as_double(b));
}
void divide_double(void*, Double a, Double b, Double* out) {
    *out = as_words(as_double(a) / as_double(b));
}
Word less_double(void*, Double a, Double b) { return Word(as_double(a) < as_double(b)); }

struct IntersectionContext {
    plane_intersection::MathServices math;
    bool failed;
};
Word intersect(void* opaque, const Word* a, const Word* b,
               const Word* c, Word out[3]) {
    auto& context = *static_cast<IntersectionContext*>(opaque);
    const auto result = plane_intersection::intersect_three_planes(a,b,c,out,&context.math);
    if (result.status != plane_intersection::Status::complete) context.failed = true;
    return result.source_boolean;
}

template<class T> bool valid(const T* pointer) {
    const auto value = reinterpret_cast<std::uintptr_t>(pointer);
    return pointer && value % alignof(T) == 0 &&
           value <= std::numeric_limits<std::uintptr_t>::max() - sizeof(T);
}
bool overlaps(const Matrix* matrix, const Frustum* frustum) {
    const auto a = reinterpret_cast<std::uintptr_t>(matrix);
    const auto b = reinterpret_cast<std::uintptr_t>(frustum);
    return a < b + sizeof(*frustum) && b < a + sizeof(*matrix);
}
} // namespace

Status set_from(const Matrix* matrix, Frustum* out) {
    if (!valid(matrix) || !valid(out) || overlaps(matrix,out))
        return Status::invalid_argument;
    Frustum pending = *out;
    frustum::PlaneSet planes{};
    const frustum::MathServices extraction{
        nullptr,0,&add,&subtract,&multiply,&divide,&square_root_float};
    if (frustum::set_from(matrix,&planes,&extraction) != frustum::Status::complete)
        return Status::dependency_failed;
    std::memcpy(pending.planes,planes.coefficients,sizeof(pending.planes));
    IntersectionContext context{{nullptr,0,&multiply,&add,&subtract,&divide,&equal,
                                 &to_double,&to_float,&square_root,
                                 &multiply_double,&divide_double,&less_double},false};
    const frustum_bounds::Services bounds{&context,sizeof(context),&intersect,&greater,&less};
    if (frustum_bounds::recalculate(&pending,&bounds) != frustum_bounds::Status::complete ||
        context.failed) return Status::dependency_failed;
    *out = pending;
    return Status::complete;
}
} // namespace dh2::engine_camera::frustum_runtime
