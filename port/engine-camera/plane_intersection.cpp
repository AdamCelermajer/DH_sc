#include "plane_intersection.hpp"

#include <limits>

namespace dh2::engine_camera::plane_intersection { namespace {

constexpr Word kSign = 0x80000000u;
constexpr Float64Words kOne64{0x00000000u, 0x3ff00000u};
constexpr Float64Words kDeterminantTolerance{0xe2308c3au, 0x3e45798eu};

bool valid_range(const void* pointer, std::size_t size, std::size_t alignment) {
    if (!pointer || reinterpret_cast<Address>(pointer) % alignment != 0) return false;
    return reinterpret_cast<Address>(pointer) <=
        std::numeric_limits<Address>::max() - size;
}

bool overlaps(const void* a, std::size_t a_size,
              const void* b, std::size_t b_size) {
    const Address aa = reinterpret_cast<Address>(a);
    const Address bb = reinterpret_cast<Address>(b);
    return aa < bb + b_size && bb < aa + a_size;
}

bool valid_services(const MathServices* services,
                    const void* const* pointers, const std::size_t* sizes,
                    std::size_t count) {
    if (!valid_range(services, sizeof(*services), alignof(MathServices))) return false;
    for (std::size_t i = 0; i != count; ++i)
        if (!valid_range(pointers[i], sizes[i], alignof(Word)) ||
            overlaps(services, sizeof(*services), pointers[i], sizes[i])) return false;
    if (!services->multiply || !services->add || !services->subtract ||
        !services->divide || !services->equal || !services->to_double ||
        !services->to_float || !services->square_root ||
        !services->multiply_double || !services->divide_double ||
        !services->less_double) return false;
    if (services->context_extent) {
        if (!valid_range(services->context, services->context_extent, 1) ||
            overlaps(services->context, services->context_extent,
                     services, sizeof(*services))) return false;
        for (std::size_t i = 0; i != count; ++i)
            if (overlaps(services->context, services->context_extent,
                         pointers[i], sizes[i])) return false;
    }
    return true;
}

CallResult invalid() { return {Status::invalid_argument, 0}; }
CallResult complete(Word result) { return {Status::complete, result}; }

Word mul(const MathServices* m, Word a, Word b) {
    return m->multiply(m->context, a, b);
}
Word add(const MathServices* m, Word a, Word b) {
    return m->add(m->context, a, b);
}
Word div(const MathServices* m, Word a, Word b) {
    return m->divide(m->context, a, b);
}

Word length(const Word* v, const MathServices* m) {
    const Word xx = mul(m, v[0], v[0]);
    const Word yy = mul(m, v[1], v[1]);
    const Word xy = add(m, xx, yy);
    const Word zz = mul(m, v[2], v[2]);
    const Word squared = add(m, xy, zz);
    Float64Words as_double{};
    m->to_double(m->context, squared, &as_double);
    Float64Words root{};
    m->square_root(m->context, as_double, &root);
    return m->to_float(m->context, root);
}

Word dot(const Word* a, const Word* b, const MathServices* m) {
    const Word xx = mul(m, a[0], b[0]);
    const Word yy = mul(m, a[1], b[1]);
    const Word xy = add(m, xx, yy);
    const Word zz = mul(m, a[2], b[2]);
    return add(m, xy, zz);
}

CallResult intersect_line_impl(const Word* plane, const Word* line_point,
                               const Word* line_vector, Word* output,
                               const MathServices* m) {
    const Word denominator = dot(plane, line_vector, m);
    if (m->equal(m->context, denominator, 0)) return complete(0);

    const Word numerator_dot = dot(plane, line_point, m);
    const Word numerator = add(m, numerator_dot, plane[3]);
    const Word negative_numerator = numerator ^ kSign;
    const Word t = div(m, negative_numerator, denominator);

    // Source stores each coordinate immediately after its multiply and add.
    const Word x_product = mul(m, t, line_vector[0]);
    output[0] = add(m, line_point[0], x_product);
    const Word y_product = mul(m, t, line_vector[1]);
    output[1] = add(m, line_point[1], y_product);
    const Word z_product = mul(m, t, line_vector[2]);
    output[2] = add(m, line_point[2], z_product);
    return complete(1);
}

CallResult intersect_two_planes_impl(const Word* first, const Word* second,
                                     Word* line_point, Word* line_vector,
                                     const MathServices* m) {
    const Word first_length = length(first, m);
    // Plane3d's source method evaluates the other plane's dotProduct(this),
    // preserving its operand order for imported floating-point calls.
    const Word normal_dot = dot(second, first, m);
    const Word second_length = length(second, m);

    const Word determinant_product = mul(m, first_length, second_length);
    const Word dot_squared = mul(m, normal_dot, normal_dot);
    const Word determinant = m->subtract(m->context, determinant_product, dot_squared);
    Float64Words determinant64{};
    m->to_double(m->context, determinant, &determinant64);
    Float64Words determinant_magnitude64 = determinant64;
    determinant_magnitude64.high &= ~kSign;
    if (m->less_double(m->context, determinant_magnitude64, kDeterminantTolerance))
        return complete(0);

    Float64Words inverse_determinant{};
    // The source reuses the magnitude-bearing double already passed to the
    // threshold comparison; it does not reconvert the signed float result.
    m->divide_double(m->context, kOne64, determinant64, &inverse_determinant);

    const Word cross_x_a = mul(m, first[1] ^ kSign, second[2]);
    const Word cross_x_b = mul(m, first[2], second[1]);
    const Word cross_x = add(m, cross_x_a, cross_x_b);
    line_vector[0] = cross_x;
    const Word cross_y_a = mul(m, first[2] ^ kSign, second[0]);
    const Word cross_y_b = mul(m, second[2], first[0]);
    const Word cross_y = add(m, cross_y_a, cross_y_b);
    line_vector[1] = cross_y;
    const Word cross_z_a = mul(m, second[1], first[0] ^ kSign);
    const Word cross_z_b = mul(m, first[1], second[0]);
    const Word cross_z = add(m, cross_z_a, cross_z_b);
    line_vector[2] = cross_z;

    const Word first_constant_term = mul(m, first[3] ^ kSign, second_length);
    const Word second_constant_term = mul(m, second[3], normal_dot);
    const Word first_numerator = add(m, first_constant_term, second_constant_term);
    Float64Words first_numerator64{};
    m->to_double(m->context, first_numerator, &first_numerator64);
    Float64Words first_coefficient64{};
    m->multiply_double(m->context, first_numerator64, inverse_determinant,
                       &first_coefficient64);
    const Word first_coefficient = m->to_float(m->context, first_coefficient64);

    const Word second_constant_term_a = mul(m, second[3] ^ kSign, first_length);
    const Word second_constant_term_b = mul(m, first[3], normal_dot);
    const Word second_numerator = add(m, second_constant_term_a,
                                      second_constant_term_b);
    Float64Words second_numerator64{};
    m->to_double(m->context, second_numerator, &second_numerator64);
    Float64Words second_coefficient64{};
    m->multiply_double(m->context, second_numerator64, inverse_determinant,
                       &second_coefficient64);
    const Word second_coefficient = m->to_float(m->context, second_coefficient64);

    // Component calculations in the source are ordered y, z, x; stores are
    // ordered x, z, y after every calculation has completed.
    const Word point_y_first = mul(m, first_coefficient, first[1]);
    const Word point_y_second = mul(m, second_coefficient, second[1]);
    const Word point_y = add(m, point_y_first, point_y_second);
    const Word point_z_first = mul(m, first_coefficient, first[2]);
    const Word point_z_second = mul(m, second_coefficient, second[2]);
    const Word point_z = add(m, point_z_first, point_z_second);
    const Word point_x_first = mul(m, first_coefficient, first[0]);
    const Word point_x_second = mul(m, second_coefficient, second[0]);
    const Word point_x = add(m, point_x_first, point_x_second);
    line_point[0] = point_x;
    line_point[2] = point_z;
    line_point[1] = point_y;
    return complete(1);
}

} // namespace

CallResult intersect_line(const Word* plane, const Word* line_point,
                          const Word* line_vector, Word* output,
                          const MathServices* math) {
    const void* const pointers[] = {plane, line_point, line_vector, output};
    const std::size_t sizes[] = {4 * sizeof(Word), 3 * sizeof(Word),
                                 3 * sizeof(Word), 3 * sizeof(Word)};
    if (!valid_services(math, pointers, sizes, 4) ||
        overlaps(output, sizes[3], plane, sizes[0]) ||
        overlaps(output, sizes[3], line_point, sizes[1]) ||
        overlaps(output, sizes[3], line_vector, sizes[2])) return invalid();
    const MathServices snapshot = *math;
    return intersect_line_impl(plane, line_point, line_vector, output, &snapshot);
}

CallResult intersect_two_planes(const Word* first, const Word* second,
                                Word* line_point, Word* line_vector,
                                const MathServices* math) {
    const void* const pointers[] = {first, second, line_point, line_vector};
    const std::size_t sizes[] = {4 * sizeof(Word), 4 * sizeof(Word),
                                 3 * sizeof(Word), 3 * sizeof(Word)};
    if (!valid_services(math, pointers, sizes, 4) ||
        overlaps(line_point, sizes[2], first, sizes[0]) ||
        overlaps(line_point, sizes[2], second, sizes[1]) ||
        overlaps(line_vector, sizes[3], first, sizes[0]) ||
        overlaps(line_vector, sizes[3], second, sizes[1]) ||
        overlaps(line_point, sizes[2], line_vector, sizes[3])) return invalid();
    const MathServices snapshot = *math;
    return intersect_two_planes_impl(first, second, line_point, line_vector, &snapshot);
}

CallResult intersect_three_planes(const Word* first, const Word* second,
                                  const Word* third, Word* output,
                                  const MathServices* math) {
    const void* const pointers[] = {first, second, third, output};
    const std::size_t sizes[] = {4 * sizeof(Word), 4 * sizeof(Word),
                                 4 * sizeof(Word), 3 * sizeof(Word)};
    if (!valid_services(math, pointers, sizes, 4) ||
        overlaps(output, sizes[3], first, sizes[0]) ||
        overlaps(output, sizes[3], second, sizes[1]) ||
        overlaps(output, sizes[3], third, sizes[2])) return invalid();

    const MathServices snapshot = *math;
    Word line_point[3]{};
    Word line_vector[3]{};
    const CallResult lines = intersect_two_planes_impl(first, second,
                                                        line_point, line_vector, &snapshot);
    if (lines.status != Status::complete) return lines;
    if (!lines.source_boolean) return complete(0);
    return intersect_line_impl(third, line_point, line_vector, output, &snapshot);
}

Word intersect_three_planes_callback(void* context, const Word* first,
                                     const Word* second, const Word* third,
                                     Word output[3]) {
    const auto* math = static_cast<const MathServices*>(context);
    const CallResult result = intersect_three_planes(first, second, third,
                                                      output, math);
    return result.status == Status::complete ? result.source_boolean : 0;
}

} // namespace dh2::engine_camera::plane_intersection
