#include "crypt_room_zone_manager_bridge_v1.hpp"

#include <cstring>
#include <limits>
#include <new>
#include <vector>

namespace dh2::crypt_room_zone_manager_bridge_v1 {
namespace {

bool matches_bounds(const crypt_module_bounds_registry_v1::Owner& bounds,
                    const crypt_room_zone_owner_v1::Owner& zones) noexcept {
    if (bounds.modules.size() != zones.zone_count()) return false;
    for (std::size_t index = 0; index < bounds.modules.size(); ++index) {
        crypt_room_zone_owner_v1::ZoneView zone{};
        if (!zones.zone_at(index, &zone) || !zone.module_name ||
            !zone.relative_minimum || !zone.relative_maximum ||
            zone.module_index != bounds.modules[index].module_index ||
            std::strcmp(zone.module_name, bounds.modules[index].module_name.c_str()) != 0)
            return false;
        for (unsigned axis = 0; axis != 3; ++axis)
            if (zone.relative_minimum[axis] != bounds.modules[index].bounds.minimum[axis] ||
                zone.relative_maximum[axis] != bounds.modules[index].bounds.maximum[axis])
                return false;
    }
    return true;
}

} // namespace

Status initialize_object_lists(
    const crypt_module_bounds_registry_v1::Owner& module_bounds,
    crypt_room_zone_owner_v1::Owner& room_zones,
    object_manager_runtime_owner_v1::Owner& object_manager,
    const module_room_zone_bounds::Services* position_services,
    const room_zone_enrollment::Services* enrollment_services,
    Result* report) noexcept {
    if (report == nullptr) return Status::invalid_argument;
    *report = {};
    report->status = Status::invalid_argument;
    report->object_manager_status = object_manager_runtime_owner_v1::Status::ok;
    report->room_zone_status = crypt_room_zone_owner_v1::Status::complete;

    if (module_bounds.modules.empty() || !enrollment_services ||
        !enrollment_services->invoke)
        return report->status;

    if (room_zones.zone_count() == 0) {
        if (!position_services || !position_services->invoke) return report->status;
        report->room_zone_status = room_zones.activate(
            module_bounds, position_services, &report->activation);
        if (report->room_zone_status != crypt_room_zone_owner_v1::Status::complete) {
            report->status = Status::room_zone_activation_failed;
            return report->status;
        }
    }

    if (!matches_bounds(module_bounds, room_zones)) {
        report->status = Status::module_registry_mismatch;
        return report->status;
    }

    const std::size_t expected_count = object_manager.object_count();
    if (expected_count > std::numeric_limits<std::uint32_t>::max()) {
        report->status = Status::object_count_limit;
        return report->status;
    }

    try {
        std::vector<room_zone_enrollment::GameObject> views;
        std::vector<room_zone_enrollment::GameObject*> ordered_views;
        views.reserve(expected_count);
        ordered_views.reserve(expected_count);

        object_manager_runtime_owner_v1::Owner::Cursor cursor{};
        object_manager.reset(&cursor);
        for (;;) {
            object_manager_runtime_owner_v1::GameObject* object = nullptr;
            report->object_manager_status = object_manager.next(&cursor, &object);
            if (report->object_manager_status !=
                object_manager_runtime_owner_v1::Status::ok) {
                report->status = Status::object_manager_enumeration_failed;
                return report->status;
            }
            if (object == nullptr) break;
            if (views.size() == expected_count || object->identity == 0) {
                report->status = Status::object_manager_enumeration_failed;
                return report->status;
            }
            views.push_back(object->enrollment_view());
        }

        if (views.size() != expected_count ||
            object_manager.object_count() != expected_count) {
            report->status = Status::object_manager_enumeration_failed;
            return report->status;
        }
        for (auto& view : views) ordered_views.push_back(&view);

        report->object_count = static_cast<std::uint32_t>(views.size());
        report->room_zone_status = room_zones.initialize_object_lists(
            ordered_views.empty() ? nullptr : ordered_views.data(),
            ordered_views.size(), enrollment_services, &report->object_lists);
        if (report->room_zone_status != crypt_room_zone_owner_v1::Status::complete) {
            report->status = Status::object_lists_initialization_failed;
            return report->status;
        }

        report->status = Status::complete;
        return report->status;
    } catch (const std::bad_alloc&) {
        report->status = Status::allocation_failure;
        return report->status;
    } catch (...) {
        report->status = Status::allocation_failure;
        return report->status;
    }
}

const char* status_name(Status status) noexcept {
    switch (status) {
    case Status::complete: return "complete";
    case Status::invalid_argument: return "invalid_argument";
    case Status::module_registry_mismatch: return "module_registry_mismatch";
    case Status::room_zone_activation_failed: return "room_zone_activation_failed";
    case Status::object_count_limit: return "object_count_limit";
    case Status::object_manager_enumeration_failed:
        return "object_manager_enumeration_failed";
    case Status::object_lists_initialization_failed:
        return "object_lists_initialization_failed";
    case Status::allocation_failure: return "allocation_failure";
    }
    return "unknown";
}

} // namespace dh2::crypt_room_zone_manager_bridge_v1
