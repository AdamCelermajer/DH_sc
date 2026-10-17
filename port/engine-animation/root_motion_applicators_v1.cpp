#include "root_motion_applicators_v1.hpp"

#include <cstddef>
#include <cstring>

namespace {
constexpr std::uint32_t maximum_animators = 65536;
float add_source_order(float a, float b) {
    volatile float result = a + b;
    return result;
}
bool overlaps(const void* a, std::size_t an, const void* b, std::size_t bn) {
    if (!a || !b || !an || !bn) return false;
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    return x <= y ? y - x < an : x - y < bn;
}
}

int dh2::animation::root_motion_applicators_v1::collect(const Request* request) {
    if (!request || request->reserved || request->count > maximum_animators ||
        !request->output_xyz || (request->count && (!request->animators ||
         !request->services || !request->services->read_delta))) return 1;
    const std::size_t entries_bytes = std::size_t(request->count) * sizeof(AnimatorApplicator);
    if (overlaps(request, sizeof(Request), request->animators, entries_bytes) ||
        overlaps(request, sizeof(Request), request->services, sizeof(Services)) ||
        overlaps(request, sizeof(Request), request->output_xyz, 3 * sizeof(float)) ||
        overlaps(request->animators, entries_bytes, request->services, sizeof(Services)) ||
        overlaps(request->animators, entries_bytes, request->output_xyz, 3 * sizeof(float)) ||
        overlaps(request->services, sizeof(Services), request->output_xyz, 3 * sizeof(float))) return 1;

    float sum[3]{};
    for (std::uint32_t i = 0; i < request->count; ++i) {
        const auto& current = request->animators[i];
        if (!current.animator_identity || !current.applicator_identity) return 1;
        for (std::uint32_t prior = 0; prior < i; ++prior) {
            if (request->animators[prior].animator_identity == current.animator_identity) return 1;
        }
        float delta[3];
        if (request->services->read_delta(request->services->context,
                current.animator_identity, current.applicator_identity, delta)) return 2;
        for (unsigned axis = 0; axis < 3; ++axis) sum[axis] = add_source_order(sum[axis], delta[axis]);
    }
    std::memcpy(request->output_xyz, sum, sizeof(sum));
    return 0;
}
