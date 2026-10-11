#include "container_world_v1.hpp"

#include "../interactions/world_object_container_state_v1.hpp"

namespace dh::foundation::containers {
namespace {

bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool encode_hits(std::uint32_t value, std::vector<std::uint8_t>& out) {
    out = {std::uint8_t(value), std::uint8_t(value >> 8), std::uint8_t(value >> 16), std::uint8_t(value >> 24)};
    return true;
}

bool decode_hits(const std::vector<std::uint8_t>& bytes, std::uint32_t& value) {
    if (bytes.size() != 4) return false;
    value = std::uint32_t(bytes[0]) | std::uint32_t(bytes[1]) << 8 |
            std::uint32_t(bytes[2]) << 16 | std::uint32_t(bytes[3]) << 24;
    return true;
}

} // namespace

bool bind_container_world_objects_v1(PlayableActorWorld& world, const ContainerRuntimeV1& runtime,
                                     std::vector<std::string>& notices, std::string& error) {
    error.clear();
    for (std::size_t i = 0; i < runtime.size(); ++i) {
        const auto& instance = runtime.instance(i);
        if (instance.stableId == 0) {
            notices.push_back("container without a stable ID is not persisted: " + instance.name);
            continue;
        }
        if (world.find_object(instance.stableId) || world.find_actor(instance.stableId)) {
            notices.push_back("container ID already bound, not persisted: " + instance.name);
            continue;
        }
        WorldObject object;
        object.id = instance.stableId;
        object.name = instance.name;
        // Authored translation (column 3 of the column-major matrix). Rotation and
        // scale are not needed by the container drop or the OBJS state.
        object.transform.position = {instance.transform[12], instance.transform[13], instance.transform[14]};
        object.visual.model = instance.visual_file;
        object.visual.visible = true;
        interactions::SourceContainerObjsFieldsV1 fields;
        fields.visible80 = 1;
        fields.enabled8a = 1;
        fields.archetype270 = 0;
        fields.state394 = kContainerStateIdle;
        if (!interactions::bind_source_container_objs_v1(object, fields, error)) return false;
        std::string bindError;
        if (!world.bind_object(std::move(object), bindError)) {
            notices.push_back("container object bind failed, not persisted: " + instance.name + ": " + bindError);
            continue;
        }
    }
    return true;
}

bool persist_container_world_state_v1(PlayableActorWorld& world, ContainerRuntimeV1& runtime,
                                      std::string& error) {
    error.clear();
    for (const std::size_t index : runtime.take_changed()) {
        const auto& instance = runtime.instance(index);
        auto* object = world.find_object(instance.stableId);
        if (!object) continue;  // not bound (noticed at bind time)
        if (!interactions::write_source_container_state394_v1(*object, runtime.state(index), error)) return false;
        std::vector<std::uint8_t> bytes;
        encode_hits(runtime.hits_remaining(index), bytes);
        if (!world.set_object_component(instance.stableId, container_hits_component_v1, std::move(bytes), error))
            return false;
    }
    return true;
}

bool restore_container_world_state_v1(const PlayableActorWorld& world, ContainerRuntimeV1& runtime,
                                      std::string& error) {
    error.clear();
    for (std::size_t index = 0; index < runtime.size(); ++index) {
        const auto& instance = runtime.instance(index);
        // Not bound at bind time (noticed there): nothing persisted, runtime left as is.
        const auto* object = world.find_object(instance.stableId);
        if (!object) continue;
        interactions::SourceContainerObjsFieldsV1 fields;
        if (!interactions::read_source_container_objs_v1(*object, fields, error)) return false;
        std::uint32_t hits = ~std::uint32_t(0);  // no component: full stage count (restore() clamps)
        const auto found = object->state_components.find(container_hits_component_v1);
        if (found != object->state_components.end() && !decode_hits(found->second, hits))
            return fail(error, "Container hits component is malformed");
        if (!runtime.restore(index, fields.state394, hits, error)) return false;
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::containers
