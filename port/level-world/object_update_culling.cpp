#include "object_update_culling.hpp"

#include <cstring>
#include <limits>

namespace dh2::object_update_culling {
namespace {
static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559,
              "culling requires IEEE binary32 float");
struct Range { std::uintptr_t begin, end; };
template<class T> bool range(const T* p, Range& out) {
    const auto start = reinterpret_cast<std::uintptr_t>(p);
    if (!p || start % alignof(T) ||
        start > std::numeric_limits<std::uintptr_t>::max() - sizeof(T)) return false;
    out = {start, start + sizeof(T)};
    return true;
}
bool overlap(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
struct Ranges {
    Range values[9]{};
    unsigned count = 0;
    template<class T> bool add(const T* p) {
        Range next{};
        if (count == 9 || !range(p, next)) return false;
        for (unsigned i = 0; i < count; ++i)
            if (overlap(next, values[i])) return false;
        values[count++] = next;
        return true;
    }
};
struct Active { const Object* object; Active* previous; };
thread_local Active* active = nullptr;
struct Guard {
    Active frame;
    explicit Guard(const Object* object) : frame{object, active} { active = &frame; }
    ~Guard() { active = frame.previous; }
};
bool running(const Object* object) {
    for (auto* frame = active; frame; frame = frame->previous)
        if (frame->object == object) return true;
    return false;
}
float number(std::uint32_t word) {
    float value;
    std::memcpy(&value, &word, sizeof value);
    return value;
}
std::uint32_t bits(float value) {
    std::uint32_t word;
    std::memcpy(&word, &value, sizeof word);
    return word;
}
float multiply(float a, float b) { volatile float value = a * b; return value; }
float add(float a, float b) { volatile float value = a + b; return value; }
Status query(const Services& bound, Object* object, std::uintptr_t identity,
             Operation operation, std::uintptr_t subject,
             Result& result, Response& response) {
    if (!bound.invoke) return Status::service_unavailable;
    const Request request{operation, identity, subject};
    response = {};
    ++result.service_calls;
    try {
        return bound.invoke(bound.context, object, &request, &response) ?
            Status::service_failed : Status::complete;
    } catch (...) { return Status::service_failed; }
}
}

Status is_remotely_updated(const Object* object, RemoteResult* out) {
    Range object_range{}, output_range{};
    if (!range(object, object_range) || !range(out, output_range) ||
        overlap(object_range, output_range) || !object->identity)
        return Status::invalid_argument;
    out->raw = object->remote_word_110 != 0xffffffffu ? 1u : object->remote_byte_118;
    return Status::complete;
}

Status evaluate(Object* object, const Aabb* aabb, const Globals* globals,
                const Services* services, Result* out) {
    Ranges ranges;
    if (!ranges.add(object) || !ranges.add(services) || !ranges.add(out) ||
        !object->identity) return Status::invalid_argument;
    // Exclude known input/output aliases before zeroing diagnostic output.
    // Null lazy inputs are valid until the original phase-one path reads them.
    if ((aabb && !ranges.add(aabb)) || (globals && !ranges.add(globals)))
        return Status::invalid_argument;
    if (running(object)) return Status::reentrant_call;
    Guard guard(object);
    const Services bound = *services;
    const auto identity = object->identity;
    *out = {};
    Response response{};
    auto status = query(bound, object, identity, Operation::get_online_byte, 0, *out, response);
    if (status != Status::complete) return status;
    if (response.raw > 255) return Status::invalid_source_fact;
    if (response.raw) {
        status = query(bound, object, identity, Operation::is_remotely_updated,
                       identity, *out, response);
        if (status != Status::complete) return status;
        // The original virtual call's return value is discarded. Re-read +86.
    }
    const auto phase = object->culling_phase_86;
    if (!phase) {
        object->culling_phase_86 = 1;
        ++out->phase_writes;
        out->can_update = 1;
        return Status::complete;
    }
    if (phase != 1) { out->can_update = 1; return Status::complete; }
    if (!aabb || !globals || !globals->application)
        return Status::invalid_source_fact;
    // Exact source capture order: maxZ, minX, minY, minZ, maxX, maxY.
    out->captured_aabb[5] = aabb->maximum[2];
    out->captured_aabb[0] = aabb->minimum[0];
    out->captured_aabb[1] = aabb->minimum[1];
    out->captured_aabb[2] = aabb->minimum[2];
    out->captured_aabb[3] = aabb->maximum[0];
    out->captured_aabb[4] = aabb->maximum[1];
    const auto application = globals->application;
    status = query(bound, object, identity, Operation::get_current_level,
                   application, *out, response);
    if (status != Status::complete) return status;
    auto* const level = response.level;
    if (level) {
        if (!ranges.add(level) || !level->identity) return Status::invalid_source_fact;
        auto* const camera = level->camera_128;
        if (!ranges.add(camera) || !camera->identity) return Status::invalid_source_fact;
        auto* const root = camera->root_8;
        if (!ranges.add(root) || !root->identity) return Status::invalid_source_fact;
        const auto root_identity = root->identity;
        status = query(bound, object, identity, Operation::get_view_frustum,
                       root_identity, *out, response);
        if (status != Status::complete) return status;
        const auto* const frustum = response.frustum;
        if (!ranges.add(frustum) || !frustum->identity) return Status::invalid_source_fact;
        out->captured_frustum = frustum->identity;
        for (unsigned index = 0; index < 6; ++index) {
            const auto& plane = frustum->planes[index];
            const float nx = number(plane.normal[0]);
            const bool x_positive = nx >= 0.0f;
            const float ny = number(plane.normal[1]);
            const float x = number(out->captured_aabb[x_positive ? 0 : 3]);
            const bool y_positive = ny >= 0.0f;
            const float nz = number(plane.normal[2]);
            const float y = number(out->captured_aabb[y_positive ? 1 : 4]);
            const bool z_positive = nz >= 0.0f;
            const float z = number(out->captured_aabb[z_positive ? 2 : 5]);
            const float xx = multiply(nx, x);
            const float yy = multiply(ny, y);
            const float xy = add(xx, yy);
            const float zz = multiply(nz, z);
            const float xyz = add(xy, zz);
            const float distance = add(xyz, number(plane.distance));
            out->last_distance_bits = bits(distance);
            ++out->planes_tested;
            // Actual relocated PLT0x30e2f8 is __aeabi_fcmpgt, not fcmplt.
            if (distance > 0.0f) return Status::complete;
        }
    }
    object->culling_phase_86 = 2;
    ++out->phase_writes;
    out->can_update = 1;
    return Status::complete;
}

}  // namespace dh2::object_update_culling
