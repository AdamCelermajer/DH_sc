#include "crypt_room_zone_owner_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <new>
#include <utility>

namespace dh2::crypt_room_zone_owner_v1 {

struct Owner::Zone {
    enum class ObjectListState : std::uint8_t { not_started, scanning, complete };

    std::uint32_t module_index = 0;
    std::string module_name;
    Address identity = 0;
    float position[3]{};
    float dimensions[3]{};
    float relative_minimum[3]{};
    float relative_maximum[3]{};
    float absolute_minimum[3]{};
    float absolute_maximum[3]{};
    std::uint8_t optional_visual_enabled = 0;
    // RoomZone's constructor initializes +0x389 active and +0x388 dirty.
    std::uint8_t active_389 = 1;
    std::uint8_t dirty_388 = 1;
    ObjectListState object_list_state = ObjectListState::not_started;
    std::vector<Address> members;

    module_room_zone_bounds::RoomZone bounds_projection() noexcept {
        module_room_zone_bounds::RoomZone result{};
        result.identity = identity;
        for (unsigned axis = 0; axis != 3; ++axis) {
            result.dimensions[axis] = &dimensions[axis];
            result.relative_minimum[axis] = &relative_minimum[axis];
            result.relative_maximum[axis] = &relative_maximum[axis];
        }
        result.optional_visual_enabled = &optional_visual_enabled;
        return result;
    }

    room_zone_enrollment::RoomZone enrollment_projection() noexcept {
        return {identity, {&absolute_minimum[0], &absolute_minimum[1],
                           &absolute_maximum[0], &absolute_maximum[1]}};
    }
};

Owner::Owner() = default;
Owner::~Owner() = default;

struct Owner::PositionAdapter {
    Zone* zone;
    const module_room_zone_bounds::Services* delegate;

    static std::int32_t invoke(void* context,
                               const module_room_zone_bounds::Request* request) {
        auto& adapter = *static_cast<PositionAdapter*>(context);
        if (!request || !adapter.zone || !adapter.delegate ||
            !adapter.delegate->invoke ||
            request->operation != module_room_zone_bounds::Operation::set_position ||
            request->room_zone != adapter.zone->identity || !request->update_position)
            return 1;
        try {
            if (adapter.delegate->invoke(adapter.delegate->context, request) != 0)
                return 1;
        } catch (...) {
            return 1;
        }

        for (unsigned axis = 0; axis != 3; ++axis) {
            adapter.zone->position[axis] = request->position[axis];
            // Zone::SetPosition(update=true) refreshes the absolute box from
            // the stored relative box and the new GameObject position.
            adapter.zone->absolute_minimum[axis] =
                adapter.zone->relative_minimum[axis] + request->position[axis];
            adapter.zone->absolute_maximum[axis] =
                adapter.zone->relative_maximum[axis] + request->position[axis];
        }
        return 0;
    }
};

struct Owner::EnrollmentAdapter {
    Owner* owner;
    const room_zone_enrollment::Services* delegate;

    static std::int32_t invoke(void* context,
                               const room_zone_enrollment::Request* request,
                               std::uint32_t* value) {
        auto& adapter = *static_cast<EnrollmentAdapter*>(context);
        if (!request || !adapter.owner || !adapter.delegate ||
            !adapter.delegate->invoke || !value)
            return 1;

        Zone* zone = nullptr;
        const bool append = request->operation ==
            room_zone_enrollment::Operation::append_to_room;
        const bool remove = request->operation ==
            room_zone_enrollment::Operation::remove_from_room;
        if (append || remove) {
            zone = adapter.owner->find_identity(request->room_zone);
            if (append && zone) {
                try {
                    // Reserve before the source list callback so a local
                    // allocation failure cannot leave a one-sided append.
                    zone->members.reserve(zone->members.size() + 1);
                } catch (...) {
                    return 1;
                }
            }
        }

        try {
            if (adapter.delegate->invoke(adapter.delegate->context,
                                         request, value) != 0)
                return 1;
        } catch (...) {
            return 1;
        }

        if (append && zone) {
            zone->members.push_back(request->object); // capacity reserved above
        } else if (remove && zone) {
            const auto found = std::find(zone->members.begin(), zone->members.end(),
                                         request->object);
            if (found != zone->members.end()) zone->members.erase(found);
        }
        return 0;
    }
};

namespace {

constexpr std::uint32_t no_module = std::numeric_limits<std::uint32_t>::max();

bool valid_entry(const crypt_module_bounds_registry_v1::Entry& entry) noexcept {
    if (entry.module_name.empty() || entry.root_id.empty()) return false;
    for (unsigned axis = 0; axis != 3; ++axis) {
        const float minimum = entry.bounds.minimum[axis];
        const float maximum = entry.bounds.maximum[axis];
        if (!std::isfinite(minimum) || !std::isfinite(maximum) || minimum > maximum)
            return false;
    }
    return true;
}

struct BusyReset {
    bool& value;
    ~BusyReset() { value = false; }
};

float binary32(std::uint32_t word) noexcept {
    float value = 0.0f;
    std::memcpy(&value, &word, sizeof(value));
    return value;
}

} // namespace

Owner::Zone* Owner::find_module(std::uint32_t index) noexcept {
    for (const auto& zone : zones_)
        if (zone->module_index == index) return zone.get();
    return nullptr;
}

Owner::Zone* Owner::find_identity(Address identity) noexcept {
    for (const auto& zone : zones_)
        if (zone->identity == identity) return zone.get();
    return nullptr;
}

bool Owner::has_members() const noexcept {
    for (const auto& zone : zones_)
        if (!zone->members.empty()) return true;
    return false;
}

Status Owner::activate(const crypt_module_bounds_registry_v1::Owner& source,
                       const module_room_zone_bounds::Services* services,
                       ActivationResult* report) noexcept {
    if (!report) return Status::invalid_argument;
    *report = {};
    report->failed_module_index = no_module;
    if (operation_in_progress_) return Status::reentrant_operation;
    if (!services || !services->invoke || source.modules.empty() ||
        source.modules.size() > crypt_module_bounds_registry_v1::max_modules)
        return Status::invalid_argument;
    if (!zones_.empty())
        return has_members() ? Status::members_present : Status::already_active;

    operation_in_progress_ = true;
    const BusyReset reset{operation_in_progress_};
    report->module_count = static_cast<std::uint32_t>(source.modules.size());

    try {
        std::vector<std::unique_ptr<Zone>> candidate;
        candidate.reserve(source.modules.size());
        for (std::size_t i = 0; i < source.modules.size(); ++i) {
            const auto& entry = source.modules[i];
            if (!valid_entry(entry)) {
                report->failed_module_index = entry.module_index;
                return Status::invalid_argument;
            }
            for (std::size_t prior = 0; prior < i; ++prior) {
                if (source.modules[prior].module_index == entry.module_index) {
                    report->failed_module_index = entry.module_index;
                    return Status::invalid_argument;
                }
            }

            auto zone = std::make_unique<Zone>();
            zone->module_index = entry.module_index;
            zone->module_name = entry.module_name;
            zone->identity = reinterpret_cast<Address>(zone.get());

            auto projection = zone->bounds_projection();
            const module_room_zone_bounds::Box3 box{
                entry.bounds.minimum, entry.bounds.maximum};
            PositionAdapter adapter{zone.get(), services};
            const module_room_zone_bounds::Services position_services{
                &adapter, &PositionAdapter::invoke};
            module_room_zone_bounds::Result bounds_result{};
            const auto bounds_status = module_room_zone_bounds::initialize(
                &projection, &box, &position_services, &bounds_result);
            if (bounds_status != module_room_zone_bounds::Status::complete) {
                report->failed_module_index = entry.module_index;
                report->bounds_status = bounds_status;
                return Status::bounds_initialization_failed;
            }

            candidate.push_back(std::move(zone));
            ++report->initialized_count;
        }

        zones_.swap(candidate);
        report->bounds_status = module_room_zone_bounds::Status::complete;
        return Status::complete;
    } catch (const std::bad_alloc&) {
        return Status::allocation_failure;
    } catch (...) {
        return Status::allocation_failure;
    }
}

Status Owner::enroll(Zone* zone, room_zone_enrollment::GameObject* object,
                     const room_zone_enrollment::Services* services,
                     EnrollmentResult* report) noexcept {
    if (!zone || !object || !services || !services->invoke || !report)
        return Status::invalid_argument;
    report->module_index = zone->module_index;
    auto projection = zone->enrollment_projection();
    EnrollmentAdapter adapter{this, services};
    const room_zone_enrollment::Services wrapped{&adapter, &EnrollmentAdapter::invoke};
    const auto status = room_zone_enrollment::add_initial_object(
        &projection, object, &wrapped, &report->enrollment);
    return status == room_zone_enrollment::Status::complete
        ? Status::complete : Status::enrollment_failed;
}

Status Owner::add_initial_object(
    std::uint32_t module_index, room_zone_enrollment::GameObject* object,
    const room_zone_enrollment::Services* services,
    EnrollmentResult* report) noexcept {
    if (!report) return Status::invalid_argument;
    *report = {};
    report->module_index = module_index;
    if (operation_in_progress_) return Status::reentrant_operation;
    if (!object || !services || !services->invoke)
        return Status::invalid_argument;
    Zone* zone = find_module(module_index);
    if (!zone) return Status::module_not_found;

    operation_in_progress_ = true;
    const BusyReset reset{operation_in_progress_};
    return enroll(zone, object, services, report);
}

Status Owner::initialize_object_lists(
    room_zone_enrollment::GameObject* const* objects, std::size_t object_count,
    const room_zone_enrollment::Services* services,
    ObjectListResult* report) noexcept {
    if (!report) return Status::invalid_argument;
    *report = {};
    report->failed_module_index = no_module;
    report->failed_object_index = no_module;
    if (operation_in_progress_) return Status::reentrant_operation;
    if (zones_.empty() || (object_count && !objects) || !services ||
        !services->invoke || object_count > no_module)
        return Status::invalid_argument;
    for (const auto& zone : zones_)
        if (zone->object_list_state != Zone::ObjectListState::not_started)
            return Status::object_lists_already_initialized;
    for (std::size_t i = 0; i < object_count; ++i)
        if (!objects[i]) {
            report->failed_object_index = static_cast<std::uint32_t>(i);
            return Status::invalid_argument;
        }

    operation_in_progress_ = true;
    const BusyReset reset{operation_in_progress_};
    for (const auto& zone : zones_) {
        zone->object_list_state = Zone::ObjectListState::scanning;
        for (std::size_t i = 0; i < object_count; ++i) {
            EnrollmentResult enrollment{};
            ++report->object_visits;
            const auto status = enroll(zone.get(), objects[i], services, &enrollment);
            if (status != Status::complete) {
                report->failed_module_index = zone->module_index;
                report->failed_object_index = static_cast<std::uint32_t>(i);
                return status;
            }
            if (enrollment.enrollment.accepted) ++report->successful_add_calls;
        }
        zone->object_list_state = Zone::ObjectListState::complete;
        ++report->zones_completed;
    }
    return Status::complete;
}

Status Owner::update_frustum(const std::uint32_t planes[6][4],
                             const float* player_position,
                             const UpdateServices* services,
                             UpdateResult* report) noexcept {
    if (!report) return Status::invalid_argument;
    *report = {};
    report->failed_module_index = no_module;
    if (operation_in_progress_ || !planes || zones_.empty() ||
        !services || !services->invoke)
        return operation_in_progress_ ? Status::reentrant_operation
                                      : Status::invalid_argument;

    operation_in_progress_ = true;
    const BusyReset reset{operation_in_progress_};
    const auto invoke_transition = [&](Zone& zone, TransitionOperation operation,
                                       Address actor, std::uint32_t* value) {
        const TransitionRequest request{operation, zone.module_index,
                                        zone.identity, actor};
        std::int32_t callback_status = 1;
        try { callback_status = services->invoke(services->context, &request, value); }
        catch (...) { callback_status = 1; }
        if (callback_status != 0) {
            report->failed_module_index = zone.module_index;
            report->failed_operation = operation;
            return Status::transition_failed;
        }
        return Status::complete;
    };
    const auto run_transition = [&](Zone& zone, bool entering) {
        const auto actor_operation = entering
            ? TransitionOperation::zone_entered : TransitionOperation::zone_exited;
        for (const Address actor : zone.members) {
            const auto status = invoke_transition(zone, actor_operation, actor, nullptr);
            if (status != Status::complete) return status;
        }
        const auto room_operation = entering
            ? TransitionOperation::add_room_object
            : TransitionOperation::remove_room_object;
        return invoke_transition(zone, room_operation, 0, nullptr);
    };
    for (auto& owner : zones_) {
        Zone& zone = *owner;
        ++report->zones_tested;
        bool outside = false;
        for (unsigned plane_index = 0; plane_index != 6; ++plane_index) {
            const auto* plane = planes[plane_index];
            const float nx = binary32(plane[0]);
            const float ny = binary32(plane[1]);
            const float nz = binary32(plane[2]);
            const float x = nx >= 0.0f ? zone.absolute_minimum[0]
                                       : zone.absolute_maximum[0];
            const float y = ny >= 0.0f ? zone.absolute_minimum[1]
                                       : zone.absolute_maximum[1];
            const float z = nz >= 0.0f ? zone.absolute_minimum[2]
                                       : zone.absolute_maximum[2];
            // Volatile stores preserve the source's multiply/add order and
            // prevent the compiler from contracting this test into FMAs.
            volatile float x_term = nx * x;
            volatile float y_term = ny * y;
            volatile float xy = x_term + y_term;
            volatile float z_term = nz * z;
            volatile float xyz = xy + z_term;
            volatile float distance = xyz + binary32(plane[3]);
            ++report->planes_tested;
            if (distance > 0.0f) {
                outside = true;
                break;
            }
        }

        if (outside) {
            if (zone.active_389 || zone.dirty_388) {
                const auto status = run_transition(zone, false);
                if (status != Status::complete) return status;
                zone.active_389 = 0;
                ++report->zones_deactivated;
            }
            zone.active_389 = 0;
        } else {
            ++report->zones_visible;
            if (!zone.active_389 || zone.dirty_388) {
                const auto status = run_transition(zone, true);
                if (status != Status::complete) return status;
                zone.active_389 = 1;
                ++report->zones_activated;
            }
            std::uint32_t visited = 0;
            auto status = invoke_transition(zone,
                TransitionOperation::query_module_visited, 0, &visited);
            if (status != Status::complete) return status;
            if (visited > 0xffu) {
                report->failed_module_index = zone.module_index;
                report->failed_operation = TransitionOperation::query_module_visited;
                return Status::transition_failed;
            }
            if (!visited && player_position &&
                zone.absolute_minimum[0] <= player_position[0] &&
                player_position[0] <= zone.absolute_maximum[0] &&
                zone.absolute_minimum[1] <= player_position[1] &&
                player_position[1] <= zone.absolute_maximum[1]) {
                status = invoke_transition(zone,
                    TransitionOperation::set_module_visited, 0, nullptr);
                if (status != Status::complete) return status;
                ++report->zones_visited;
            }
        }
        zone.dirty_388 = 0;
    }
    return Status::complete;
}

Status Owner::mark_dirty(std::uint32_t module_index) noexcept {
    if (operation_in_progress_) return Status::reentrant_operation;
    Zone* zone = find_module(module_index);
    if (!zone) return Status::module_not_found;
    zone->dirty_388 = 1;
    return Status::complete;
}

Status Owner::forget_source_unlinked_actor(std::uint32_t module_index,
                                           Address actor) noexcept {
    if (operation_in_progress_) return Status::reentrant_operation;
    if (!actor) return Status::invalid_argument;
    Zone* zone = find_module(module_index);
    if (!zone) return Status::module_not_found;
    const auto found = std::find(zone->members.begin(), zone->members.end(), actor);
    if (found == zone->members.end()) return Status::actor_not_tracked;
    zone->members.erase(found);
    return Status::complete;
}

Status Owner::clear() noexcept {
    if (operation_in_progress_) return Status::reentrant_operation;
    if (has_members()) return Status::members_present;
    zones_.clear();
    return Status::complete;
}

std::size_t Owner::zone_count() const noexcept {
    return zones_.size();
}

bool Owner::zone_at(std::size_t index, ZoneView* view) const noexcept {
    if (!view || index >= zones_.size()) return false;
    const auto& zone = *zones_[index];
    *view = {zone.module_index, zone.identity, zone.module_name.c_str(),
             zone.position, zone.dimensions, zone.relative_minimum,
             zone.relative_maximum, zone.absolute_minimum, zone.absolute_maximum,
             zone.members.empty() ? nullptr : zone.members.data(), zone.members.size(),
             zone.active_389, zone.dirty_388};
    return true;
}

const char* status_name(Status status) noexcept {
    switch (status) {
    case Status::complete: return "complete";
    case Status::invalid_argument: return "invalid_argument";
    case Status::already_active: return "already_active";
    case Status::object_lists_already_initialized: return "object_lists_already_initialized";
    case Status::bounds_initialization_failed: return "bounds_initialization_failed";
    case Status::enrollment_failed: return "enrollment_failed";
    case Status::module_not_found: return "module_not_found";
    case Status::members_present: return "members_present";
    case Status::actor_not_tracked: return "actor_not_tracked";
    case Status::reentrant_operation: return "reentrant_operation";
    case Status::allocation_failure: return "allocation_failure";
    case Status::transition_failed: return "transition_failed";
    }
    return "unknown";
}

} // namespace dh2::crypt_room_zone_owner_v1
