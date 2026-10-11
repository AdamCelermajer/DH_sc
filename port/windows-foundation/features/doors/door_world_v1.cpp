#include "door_world_v1.hpp"

namespace dh::foundation::doors {

bool bind_door_world_objects_v1(PlayableActorWorld& world, const DoorRuntimeV1& runtime,
                                std::vector<std::string>& notices, std::string& error) {
    error.clear();
    for (std::size_t i = 0; i < runtime.size(); ++i) {
        const auto& instance = runtime.instance(i);
        if (instance.stableId == 0) {
            notices.push_back("door without a stable ID is not persisted: " + instance.name);
            continue;
        }
        if (world.find_object(instance.stableId)) continue;  // bound by another owner (reused)
        if (world.find_actor(instance.stableId)) {
            notices.push_back("door ID belongs to an actor, not persisted: " + instance.name);
            continue;
        }
        WorldObject object;
        object.id = instance.stableId;
        object.name = instance.name;
        object.transform.position = {instance.transform[12], instance.transform[13], instance.transform[14]};
        object.visual.model = instance.visual_file;
        object.visual.visible = true;
        std::string bindError;
        if (!world.bind_object(std::move(object), bindError)) {
            notices.push_back("door object bind failed, not persisted: " + instance.name + ": " + bindError);
            continue;
        }
    }
    return true;
}

bool persist_door_world_state_v1(PlayableActorWorld& world, DoorRuntimeV1& runtime, std::string& error) {
    error.clear();
    for (const std::size_t index : runtime.take_changed()) {
        const auto& instance = runtime.instance(index);
        if (!world.find_object(instance.stableId)) continue;  // not bound (noticed at bind time)
        std::vector<std::uint8_t> bytes{std::uint8_t(runtime.state(index))};
        if (!world.set_object_component(instance.stableId, door_state_component_v1, std::move(bytes), error))
            return false;
    }
    return true;
}

bool restore_door_world_state_v1(PlayableActorWorld& world, DoorRuntimeV1& runtime, std::string& error) {
    error.clear();
    // Validate every persisted component before changing runtime/world state.
    for (std::size_t index = 0; index < runtime.size(); ++index) {
        const auto* object = world.find_object(runtime.instance(index).stableId);
        if (!object) continue;
        const auto found = object->state_components.find(door_state_component_v1);
        if (found == object->state_components.end()) continue;
        if (found->second.size() != 1 || found->second[0] > 3) {
            error = "Door state component is malformed: " + runtime.instance(index).name;
            return false;
        }
    }

    for (std::size_t index = 0; index < runtime.size(); ++index) {
        const auto& instance = runtime.instance(index);
        const auto* object = world.find_object(instance.stableId);
        if (!object) continue;
        const auto found = object->state_components.find(door_state_component_v1);
        if (found == object->state_components.end()) continue;  // no component: authored start state
        // Door::Deserialize: 1 and 3 are opened, others closed. A persisted transition is finished at once.
        DoorRestoreProjectionV1 restored{};
        if (!project_door_restore_v1(found->second[0], restored)) {
            error = "Door state component is malformed: " + instance.name;
            return false;
        }
        if (!runtime.restore(index, restored.state, error)) return false;
        const std::uint8_t terminal = static_cast<std::uint8_t>(restored.state);
        if (found->second[0] != terminal &&
            !world.set_object_component(instance.stableId, door_state_component_v1, {terminal}, error))
            return false;
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::doors
