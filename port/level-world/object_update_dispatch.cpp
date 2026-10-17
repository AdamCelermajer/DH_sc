#include "object_update_dispatch.hpp"

#include <limits>

namespace dh2::object_update_dispatch {
namespace {
struct Range { std::uintptr_t begin, end; };

template<class T> bool range(const T* value, Range& out) {
    const auto begin = reinterpret_cast<std::uintptr_t>(value);
    if (!value || begin % alignof(T) ||
        begin > std::numeric_limits<std::uintptr_t>::max() - sizeof(T)) return false;
    out = {begin, begin + sizeof(T)};
    return true;
}

bool overlaps(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }

struct Active { const Object* object; std::uintptr_t identity; Range output; Active* previous; };
thread_local Active* active = nullptr;

struct Guard {
    Active frame;
    Guard(const Object* object, std::uintptr_t identity, Range output)
        : frame{object,identity,output,active} { active = &frame; }
    ~Guard() { active = frame.previous; }
};

bool running(const Object* object,std::uintptr_t identity) {
    for (auto* frame = active; frame; frame = frame->previous)
        if (frame->object==object || frame->identity == identity) return true;
    return false;
}
bool outside_active_outputs(Range value) {
    for (auto* frame=active;frame;frame=frame->previous)
        if (overlaps(value,frame->output)) return false;
    return true;
}

Status invoke(const Services& services, Object* object, std::uintptr_t identity, Operation operation,
              std::uintptr_t subject, const ManagerContext* manager,
              Result& result, Response& response) {
    if (!services.invoke) return Status::service_unavailable;
    response = {};
    const bool unloading = operation == Operation::unload_script_process;
    const Request request{operation, identity, subject,
                          unloading && manager ? manager->unload_arg1 : 0,
                          0,
                          unloading && manager ? manager->unload_arg3 : 0};
    ++result.service_calls;
    try {
        return services.invoke(services.context, object, &request, &response) == 0 ?
            Status::complete : Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
}
}

Status dispatch(Object* object, const ManagerContext* manager,
                const Services* services, Result* out) {
    Range object_range{}, manager_range{}, services_range{}, result_range{};
    if (!range(object, object_range) || !range(services, services_range) ||
        !range(out, result_range) ||
        !outside_active_outputs(object_range) || !outside_active_outputs(services_range) ||
        !outside_active_outputs(result_range) ||
        (manager && !range(manager, manager_range)) ||
        overlaps(object_range, services_range) || overlaps(object_range, result_range) ||
        overlaps(services_range, result_range) ||
        (manager && (!outside_active_outputs(manager_range) || overlaps(manager_range, object_range) ||
                     overlaps(manager_range, services_range) ||
                     overlaps(manager_range, result_range))))
        return Status::invalid_argument;
    if (!object->identity) return Status::invalid_argument;
    const auto identity=object->identity;
    if (running(object,identity)) return Status::reentrant_call;

    Guard guard(object,identity,result_range);
    const Services bound = *services;
    *out = {};

    Response response{};
    auto status = invoke(bound, object, identity, Operation::is_character,
                         0x24, nullptr, *out, response);
    if (status != Status::complete) return status;
    if (response.raw) {
        status = invoke(bound, object, identity, Operation::update_ai_pointers,
                        0x3a4344, nullptr, *out, response);
        if (status != Status::complete) return status;
    }

    // These are live reads after IsCharacter/UpdateAIPointers. The source
    // bypasses the online query only when both bytes are nonzero.
    bool needs_online_check = object->update_gate_85 == 0;
    if (!needs_online_check) needs_online_check = object->update_gate_8a == 0;
    if (needs_online_check) {
        status = invoke(bound, object, identity, Operation::get_online_byte,
                        0x7fd794, nullptr, *out, response);
        if (status != Status::complete) return status;
        if (response.raw > 255) return Status::invalid_source_fact;
        if (response.raw) {
            status = invoke(bound, object, identity, Operation::is_remotely_updated,
                            0x54, nullptr, *out, response);
            if (status != Status::complete) return status;
            if (response.raw) needs_online_check = false;
        }
    }

    if (needs_online_check) {
        // ObjectManager::Update stores zero unconditionally before the second
        // IsCharacter call, including when this byte was already zero.
        object->culling_phase_86 = 0;
        ++out->culling_phase_writes;
        status = invoke(bound, object, identity, Operation::is_character,
                        0x24, nullptr, *out, response);
        if (status != Status::complete) return status;
        if (response.raw) {
            if (!manager) return Status::invalid_source_fact;
            status = invoke(bound, object, identity, Operation::unload_script_process,
                            0x3a7b24, manager, *out, response);
            if (status != Status::complete) return status;
        }
        out->route = Route::skipped_offline;
        return Status::complete;
    }

    // This bounded slice does not include ObjectManager's nonzero deletion
    // byte path (MarkForDeletion + list unlink/free). Do not report success.
    if (object->deletion_81) return Status::unsupported_deletion_branch;

    object->update_marker_88 = 0;
    ++out->update_marker_writes;
    status = invoke(bound, object, identity, Operation::dispatch_virtual_update,
                    0x2c, nullptr, *out, response);
    if (status != Status::complete) return status;
    // The original manager does not branch on the virtual Update return value.
    out->route = Route::update_dispatched;
    return Status::complete;
}

}  // namespace dh2::object_update_dispatch
