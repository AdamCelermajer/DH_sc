#include "frustum.hpp"

#include <limits>

namespace dh2::engine_camera::frustum { namespace {

bool valid_range(const void* pointer, std::size_t size, std::size_t alignment) {
    if (!pointer || reinterpret_cast<Address>(pointer) % alignment != 0) return false;
    return reinterpret_cast<Address>(pointer) <=
        std::numeric_limits<Address>::max() - size;
}

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const Address a = reinterpret_cast<Address>(left);
    const Address b = reinterpret_cast<Address>(right);
    return a < b + right_size && b < a + left_size;
}

} // namespace

Status set_from(const Matrix4Words* matrix, PlaneSet* planes,
                const MathServices* math) {
    static_assert(sizeof(Matrix4Words) == 16 * sizeof(Word));
    static_assert(sizeof(PlaneSet) == 6 * 4 * sizeof(Word));

    if (!valid_range(matrix, sizeof(*matrix), alignof(Matrix4Words)) ||
        !valid_range(planes, sizeof(*planes), alignof(PlaneSet)) ||
        !valid_range(math, sizeof(*math), alignof(MathServices)) ||
        !math->add || !math->subtract || !math->multiply || !math->divide ||
        !math->square_root ||
        overlaps(matrix, sizeof(*matrix), planes, sizeof(*planes)) ||
        overlaps(matrix, sizeof(*matrix), math, sizeof(*math)) ||
        overlaps(planes, sizeof(*planes), math, sizeof(*math)))
        return Status::invalid_argument;
    if (math->context_extent &&
        (!valid_range(math->context, math->context_extent, alignof(std::max_align_t)) ||
         overlaps(math->context, math->context_extent, matrix, sizeof(*matrix)) ||
         overlaps(math->context, math->context_extent, planes, sizeof(*planes)) ||
         overlaps(math->context, math->context_extent, math, sizeof(*math))))
        return Status::invalid_argument;

    const Word* const m = matrix->elements;
    Word (*const p)[4] = planes->coefficients;
    void* const context = math->context;

    // Source stores (frustum+0x2c), in matrix-word order.
    p[2][0] = math->add(context, m[3], m[0]);
    p[2][1] = math->add(context, m[7], m[4]);
    p[2][2] = math->add(context, m[11], m[8]);
    p[2][3] = math->add(context, m[15], m[12]);

    // Source stores (frustum+0x3c), in matrix-word order.
    p[3][0] = math->subtract(context, m[3], m[0]);
    p[3][1] = math->subtract(context, m[7], m[4]);
    p[3][2] = math->subtract(context, m[11], m[8]);
    p[3][3] = math->subtract(context, m[15], m[12]);

    // Source stores (frustum+0x5c), in matrix-word order.
    p[5][0] = math->subtract(context, m[3], m[1]);
    p[5][1] = math->subtract(context, m[7], m[5]);
    p[5][2] = math->subtract(context, m[11], m[9]);
    p[5][3] = math->subtract(context, m[15], m[13]);

    // Source stores (frustum+0x4c), in matrix-word order.
    p[4][0] = math->add(context, m[3], m[1]);
    p[4][1] = math->add(context, m[7], m[5]);
    p[4][2] = math->add(context, m[11], m[9]);
    p[4][3] = math->add(context, m[15], m[13]);

    // Source stores (frustum+0x0c), in matrix-word order.
    p[0][0] = math->subtract(context, m[3], m[2]);
    p[0][1] = math->subtract(context, m[7], m[6]);
    p[0][2] = math->subtract(context, m[11], m[10]);
    p[0][3] = math->subtract(context, m[15], m[14]);

    // Source stores (frustum+0x1c): direct matrix words, no arithmetic.
    p[1][0] = m[2];
    p[1][1] = m[6];
    p[1][2] = m[10];
    p[1][3] = m[14];

    constexpr Word one = 0x3f800000u;
    constexpr Word sign = 0x80000000u;
    for (std::size_t plane = 0; plane != 6; ++plane) {
        const Word aa = math->multiply(context, p[plane][0], p[plane][0]);
        const Word bb = math->multiply(context, p[plane][1], p[plane][1]);
        const Word ab = math->add(context, aa, bb);
        const Word cc = math->multiply(context, p[plane][2], p[plane][2]);
        const Word length_squared = math->add(context, ab, cc);
        const Word length = math->square_root(context, length_squared);
        const Word reciprocal = math->divide(context, one, length);
        // The source uses integer ADD #0x80000000, toggling the sign bit.
        const Word negative_reciprocal = reciprocal ^ sign;
        for (std::size_t component = 0; component != 4; ++component)
            p[plane][component] = math->multiply(
                context, p[plane][component], negative_reciprocal);
    }
    return Status::complete;
}

} // namespace dh2::engine_camera::frustum
