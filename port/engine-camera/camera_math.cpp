#include "camera_math.hpp"

#include <limits>

namespace dh2::engine_camera::camera_math { namespace {
using Address = std::uintptr_t;
struct Range { Address begin, end; };

bool range(const void* pointer, std::size_t size, std::size_t alignment, Range& out) {
    const Address begin = reinterpret_cast<Address>(pointer);
    if (!pointer || !alignment || begin % alignment ||
        begin > std::numeric_limits<Address>::max() - size) return false;
    out = {begin, begin + size};
    return true;
}
bool overlaps(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
Word negate(Word value) { return value ^ 0x80000000u; }

Word length_squared(const Word v[3], const MathServices& m) {
    const Word xx = m.multiply(m.context, v[0], v[0]);
    const Word yy = m.multiply(m.context, v[1], v[1]);
    const Word xy = m.add(m.context, xx, yy);
    const Word zz = m.multiply(m.context, v[2], v[2]);
    return m.add(m.context, xy, zz);
}
void normalize_if_nonzero(Word v[3], const MathServices& m) {
    const Word squared = length_squared(v, m);
    if (m.equal(m.context, squared, 0) != 0) return;
    const Word length = m.square_root(m.context, squared);
    const Word inverse = m.divide(m.context, 0x3f800000u, length);
    v[0] = m.multiply(m.context, v[0], inverse);
    v[1] = m.multiply(m.context, v[1], inverse);
    v[2] = m.multiply(m.context, v[2], inverse);
}

} // namespace

Status build_look_at(const Vector3* position, const Vector3* target,
                     const Vector3* up, Matrix4* output,
                     const MathServices* math) {
    Range p, t, u, o, m;
    if (!range(position, sizeof(*position), alignof(Vector3), p) ||
        !range(target, sizeof(*target), alignof(Vector3), t) ||
        !range(up, sizeof(*up), alignof(Vector3), u) ||
        !range(output, sizeof(*output), alignof(Matrix4), o) ||
        !range(math, sizeof(*math), alignof(MathServices), m) ||
        !math->add || !math->subtract || !math->multiply || !math->divide ||
        !math->equal || !math->square_root || overlaps(o, m) ||
        (math->context_extent &&
         (!range(math->context, math->context_extent, 1, m) || overlaps(o, m))) ||
        overlaps(o, p) || overlaps(o, t) || overlaps(o, u))
        return Status::invalid_argument;

    const auto& s = *math;
    Word z[3];
    z[0] = s.subtract(s.context, target->xyz[0], position->xyz[0]);
    z[1] = s.subtract(s.context, target->xyz[1], position->xyz[1]);
    z[2] = s.subtract(s.context, target->xyz[2], position->xyz[2]);
    normalize_if_nonzero(z, s);

    // The source constructs z x up (including its sign), not the common
    // up x z variant. Preserve its three imported-operation sequences.
    Word x[3];
    const Word x0a = s.multiply(s.context, z[2], negate(up->xyz[1]));
    const Word x0b = s.multiply(s.context, z[1], up->xyz[2]);
    x[0] = s.add(s.context, x0a, x0b);
    const Word x1a = s.multiply(s.context, z[0], negate(up->xyz[2]));
    const Word x1b = s.multiply(s.context, z[2], up->xyz[0]);
    x[1] = s.add(s.context, x1a, x1b);
    const Word x2a = s.multiply(s.context, z[1], negate(up->xyz[0]));
    const Word x2b = s.multiply(s.context, z[0], up->xyz[1]);
    x[2] = s.add(s.context, x2a, x2b);
    normalize_if_nonzero(x, s);

    Word y[3];
    y[0] = s.subtract(s.context, s.multiply(s.context, z[2], x[1]),
                      s.multiply(s.context, z[1], x[2]));
    y[1] = s.subtract(s.context, s.multiply(s.context, z[0], x[2]),
                      s.multiply(s.context, z[2], x[0]));
    y[2] = s.subtract(s.context, s.multiply(s.context, z[1], x[0]),
                      s.multiply(s.context, z[0], x[1]));

    Word elements[16]{};
    elements[0] = x[0]; elements[1] = y[0]; elements[2] = z[0];
    elements[4] = x[1]; elements[5] = y[1]; elements[6] = z[1];
    elements[8] = x[2]; elements[9] = y[2]; elements[10] = z[2];
    const Word x_dot0 = s.multiply(s.context, x[0], position->xyz[0]);
    const Word x_dot1 = s.multiply(s.context, x[1], position->xyz[1]);
    const Word x_dot01 = s.add(s.context, x_dot0, x_dot1);
    const Word x_dot2 = s.multiply(s.context, x[2], position->xyz[2]);
    elements[12] = negate(s.add(s.context, x_dot01, x_dot2));
    const Word y_dot0 = s.multiply(s.context, y[0], position->xyz[0]);
    const Word y_dot1 = s.multiply(s.context, y[1], position->xyz[1]);
    const Word y_dot01 = s.add(s.context, y_dot0, y_dot1);
    const Word y_dot2 = s.multiply(s.context, y[2], position->xyz[2]);
    elements[13] = negate(s.add(s.context, y_dot01, y_dot2));
    const Word z_dot0 = s.multiply(s.context, z[0], position->xyz[0]);
    const Word z_dot1 = s.multiply(s.context, z[1], position->xyz[1]);
    const Word z_dot01 = s.add(s.context, z_dot0, z_dot1);
    const Word z_dot2 = s.multiply(s.context, z[2], position->xyz[2]);
    elements[14] = negate(s.add(s.context, z_dot01, z_dot2));
    elements[15] = 0x3f800000u;
    for (std::size_t i=0;i!=16;++i) output->elements[i] = elements[i];
    output->definitely_identity = 0;
    return Status::complete;
}

} // namespace dh2::engine_camera::camera_math
