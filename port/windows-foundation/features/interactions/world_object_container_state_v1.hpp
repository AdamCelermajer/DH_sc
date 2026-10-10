#pragma once

#include "../../world.hpp"
#include "../../actor_definitions.hpp"
#include "../../actor_state.hpp"
#include <functional>

namespace dh::foundation::interactions {

// Version is part of the key so this component can coexist with later feature
// codecs. The value is exactly the source OBJS payload: visible80, enabled8a,
// archetype270 (little-endian u32), and the container state394 byte.
inline constexpr const char* source_container_objs_component_v1 =
    "dh2.source-container.objs.v1";
inline constexpr std::size_t source_container_objs_payload_bytes_v1 = 7;

struct SourceContainerObjsFieldsV1 {
    std::uint8_t visible80{};
    std::uint8_t enabled8a{};
    std::int32_t archetype270{};
    std::uint8_t state394{};
};

bool encode_source_container_objs_v1(const SourceContainerObjsFieldsV1&,
                                     std::vector<std::uint8_t>&,
                                     std::string& error);
bool decode_source_container_objs_v1(const std::vector<std::uint8_t>&,
                                     SourceContainerObjsFieldsV1&,
                                     std::string& error);
bool bind_source_container_objs_v1(WorldObject&,
                                   const SourceContainerObjsFieldsV1&,
                                   std::string& error);
bool read_source_container_objs_v1(const WorldObject&,
                                   SourceContainerObjsFieldsV1&,
                                   std::string& error);
bool write_source_container_state394_v1(WorldObject&, std::uint8_t,
                                        std::string& error);

struct SourceObjectSaveKeyV1 {
    std::string gametype;
    std::string map_name;
    std::int32_t room{-1};
};
using SourceObjectRoomResolverV1 = bool(*)(
    void*, const ActorDefinition&, std::int32_t&, std::string&);

// Level OBJS stores (gametype,map_name,room), not ActorDefinition::stableId.
// Resolve only an exact unique authored definition in the current source set.
bool source_object_id_for_save_key_v1(
    const std::vector<ActorDefinition>&,
    const SourceObjectSaveKeyV1&, void*, SourceObjectRoomResolverV1,
    ActorId&, std::string& error);
bool source_object_save_key_v1(const ActorDefinition&, std::int32_t room,
                               SourceObjectSaveKeyV1&, std::string& error);

enum class SourceContainerRestoreVisualV1 { none, idle, idleactive };
SourceContainerRestoreVisualV1 source_container_restore_visual_v1(
    std::uint8_t state394) noexcept;

} // namespace dh::foundation::interactions
