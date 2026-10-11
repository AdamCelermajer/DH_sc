#include "door_source_runtime_v1.hpp"

namespace dh2::door_source_runtime_v1 {
namespace {

using Owner = object_manager_runtime_owner_v1::Owner;
using GameObject = object_manager_runtime_owner_v1::GameObject;

Status get_door(Owner& manager, std::uintptr_t identity, GameObject** out,
                std::string& error) {
    if (!identity || !out) return Status::invalid_state;
    *out = manager.find_by_identity(identity);
    if (!*out || !(*out)->door.definition_ready) {
        error = "Door source definition is not published on the canonical ObjectManager row";
        return Status::invalid_state;
    }
    return Status::complete;
}

Status run_opened(GameObject& object, std::uintptr_t identity,
                  bool event_call, const Services& services,
                  std::string& error) {
    auto& door = object.door;
    door.state_936 = 1;
    if (object.visual_object_2d8 != 0 && !event_call) {
        if (!services.play_idle) return Status::provider_unavailable;
        if (!services.play_idle(services.context, identity, "IdleOpened", true, error))
            return Status::provider_failed;
    }
    if (door.physical_present_732) {
        if (!services.set_physical_filter) return Status::provider_unavailable;
        if (!services.set_physical_filter(services.context, identity, false, error))
            return Status::provider_failed;
    }
    if (!services.flag_floor_dead_end) return Status::provider_unavailable;
    if (!services.flag_floor_dead_end(services.context, identity, false, error))
        return Status::provider_failed;
    door.nav_dead_end_883 = 1;
    object.game_object_nav_dead_end_373 = 1;
    return Status::complete;
}

Status run_closed(GameObject& object, std::uintptr_t identity,
                  bool event_call, const Services& services,
                  std::string& error) {
    auto& door = object.door;
    door.state_936 = 0;
    if (object.visual_object_2d8 != 0 && !event_call) {
        if (!services.play_idle) return Status::provider_unavailable;
        if (!services.play_idle(services.context, identity, "IdleClosed", true, error))
            return Status::provider_failed;
    }
    if (door.physical_present_732) {
        if (!services.set_physical_filter) return Status::provider_unavailable;
        if (!services.set_physical_filter(services.context, identity, true, error))
            return Status::provider_failed;
    }
    if (!services.flag_floor_dead_end) return Status::provider_unavailable;
    if (!services.flag_floor_dead_end(services.context, identity, true, error))
        return Status::provider_failed;
    door.nav_dead_end_883 = 0;
    object.game_object_nav_dead_end_373 = 0;
    return Status::complete;
}

bool immediate(bool force, bool has_visual, bool interactive,
               bool property_750, bool property_752) noexcept {
    return !has_visual || force ||
        (interactive && property_750 && !property_752);
}

} // namespace

Status opened(Owner& manager, std::uintptr_t identity, bool event_call,
              const Services& services, std::string& error) {
    GameObject* object{};
    const auto status = get_door(manager, identity, &object, error);
    if (status != Status::complete) return status;
    return run_opened(*object, identity, event_call, services, error);
}

Status closed(Owner& manager, std::uintptr_t identity, bool event_call,
              const Services& services, std::string& error) {
    GameObject* object{};
    const auto status = get_door(manager, identity, &object, error);
    if (status != Status::complete) return status;
    return run_closed(*object, identity, event_call, services, error);
}

Status set_enabled(Owner& manager, std::uintptr_t identity, bool enabled,
                   const Services& services, std::string& error) {
    GameObject* object{};
    const auto status = get_door(manager, identity, &object, error);
    if (status != Status::complete) return status;

    // ObjectBase::Enabled order: SetVisible(+0x80 from +0x8a), then
    // setUpdating(+0x85). Disabled performs those writes in reverse order.
    if (enabled) {
        object->visible_80 = object->serialized_flag_8a;
        object->updating_85 = 1;
    } else {
        object->updating_85 = 0;
        object->visible_80 = 0;
    }
    if (object->live_fields.visible_80)
        *object->live_fields.visible_80 = object->visible_80;
    if (object->live_fields.updating_85)
        *object->live_fields.updating_85 = object->updating_85;

    // GameObject::Enabled/Disabled toggles PFObject+0x1cc bit 3, changes the
    // existing PhysicalObject filter when present, and writes +0x373.
    if (enabled) object->pf_object_flags_1cc |= 8u;
    else object->pf_object_flags_1cc &= ~8u;
    if (object->door.physical_present_732) {
        if (!services.set_physical_filter) return Status::provider_unavailable;
        if (!services.set_physical_filter(services.context, identity, enabled,
                                          error))
            return Status::provider_failed;
    }
    object->door.nav_dead_end_883 = enabled ? 0 : 1;
    object->game_object_nav_dead_end_373 = enabled ? 0 : 1;

    // Door::Enabled/Disabled updates PFWorld dead-end state after the common
    // GameObject callback. The provider resolves this same row on the active
    // Level and mutates its existing floor/navigation grids.
    if (!services.flag_floor_dead_end) return Status::provider_unavailable;
    if (!services.flag_floor_dead_end(services.context, identity, enabled,
                                      error))
        return Status::provider_failed;
    return Status::complete;
}

Status open(Owner& manager, std::uintptr_t identity, bool force,
            bool has_visual, bool interactive, bool property_750,
            bool property_752, const Services& services,
            std::string& error) {
    GameObject* object{};
    const auto status = get_door(manager, identity, &object, error);
    if (status != Status::complete) return status;
    auto& door = object->door;
    if (door.animation_busy_940 || door.state_936 != 0) return Status::no_change;
    if (immediate(force, has_visual, interactive, property_750, property_752)) {
        const auto result = run_opened(*object, identity, false, services, error);
        if (result != Status::complete) return result;
    } else {
        if (!services.play_animation) return Status::provider_unavailable;
        door.state_936 = 3;
        door.animation_busy_940 = 1;
        door.animation_phase_133 = 0;
        if (!services.play_animation(services.context, identity, "Open", error))
            return Status::provider_failed;
    }
    if (!services.play_sound) return Status::provider_unavailable;
    if (!services.play_sound(services.context, identity, true, error))
        return Status::provider_failed;
    return Status::complete;
}

Status close(Owner& manager, std::uintptr_t identity, bool force,
             bool has_visual, bool interactive, bool property_750,
             bool property_752, const Services& services,
             std::string& error) {
    GameObject* object{};
    const auto status = get_door(manager, identity, &object, error);
    if (status != Status::complete) return status;
    auto& door = object->door;
    if (door.animation_busy_940 || door.state_936 != 1) return Status::no_change;
    if (immediate(force, has_visual, interactive, property_750, property_752)) {
        const auto result = run_closed(*object, identity, false, services, error);
        if (result != Status::complete) return result;
    } else {
        if (!services.play_animation) return Status::provider_unavailable;
        door.state_936 = 2;
        door.animation_busy_940 = 1;
        door.animation_phase_133 = 0;
        if (!services.play_animation(services.context, identity, "Close", error))
            return Status::provider_failed;
    }
    if (!services.play_sound) return Status::provider_unavailable;
    if (!services.play_sound(services.context, identity, false, error))
        return Status::provider_failed;
    return Status::complete;
}

} // namespace dh2::door_source_runtime_v1
