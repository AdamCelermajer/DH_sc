#pragma once

#include "../world-data/world.hpp"

#include <cstdint>

// A bounded, owned projection of source Character records. These are port
// records, not original ObjectBase/Character objects or their runtime ABI.
namespace dh2::actors {

constexpr std::uint32_t MAX_ACTORS = 512;
constexpr std::uint32_t MAX_LEVEL_NAME = 96;
constexpr std::uint32_t MAX_MODULE_NAME = 160;
constexpr std::uint32_t MAX_SOURCE_PATH = 256;
constexpr std::uint32_t MAX_ACTOR_NAME = 128;
constexpr std::uint32_t MAX_TEMPLATE_NAME = 128;
constexpr std::uint32_t MAX_TEMPLATE_CLASS = 96;
constexpr std::uint32_t MAX_AI_STATE = 64;

enum class Error : std::uint32_t {
    ok,
    argument,
    capacity,
    allocation,
    duplicate_name,
    invalid_source_record
};

enum class Lifecycle : std::uint8_t {
    registered,
    spawn_requested
};

enum class SpawnResult : std::uint8_t {
    requested,
    already_requested,
    lookup_miss,
    invalid_request
};

struct ActorInstance {
    char level_name[MAX_LEVEL_NAME];
    char module_name[MAX_MODULE_NAME];
    char source_path[MAX_SOURCE_PATH];
    std::uint32_t module_index;
    std::uint32_t source_record;
    std::uint32_t source_begin;
    std::uint32_t source_end;

    char name[MAX_ACTOR_NAME];
    char character_template[MAX_TEMPLATE_NAME];
    char template_data_class[MAX_TEMPLATE_CLASS];
    char editor_template_name[MAX_TEMPLATE_NAME];
    char ai_state[MAX_AI_STATE];
    std::uint8_t has_character_template;
    std::uint8_t has_template_data_class;
    std::uint8_t has_editor_template_name;
    std::uint8_t has_ai_state;
    std::uint8_t has_auto_spawn;
    std::uint8_t auto_spawn;

    float local_position[3];
    float world_position[3];
    float local_rotation_degrees[3];
    float local_scale[3];

    Lifecycle lifecycle;
    std::uint32_t spawn_transition_count;
};

struct Registry {
    ActorInstance* actors;
    std::uint32_t actor_count;
    std::uint32_t request_count;
    std::uint32_t lookup_miss_count;
    char last_lookup_miss[MAX_ACTOR_NAME];
};

// `out` must be zero initialized or hold a Registry previously initialized by
// this API. The source must be a successfully imported dh2::world::SourceLevel. On
// failure, `out` is unchanged; on success, it owns copied records and can
// outlive/free the SourceLevel. Only gametype="Character" MGP records are
// copied. Duplicate exact names reject the replacement as ambiguous.
Error init(Registry* out, const dh2::world::SourceLevel* level);
void destroy(Registry* registry);

const ActorInstance* find(const Registry* registry, const char* exact_name);

// This records a port-owned lifecycle request on an already registered actor.
// It does not construct an actor, run the native state machine, or make an
// actor visible. A miss is returned and counted without changing any actor.
SpawnResult request_spawn(Registry* registry, const char* exact_name);

const char* error_name(Error error);
const char* spawn_result_name(SpawnResult result);
const char* lifecycle_name(Lifecycle lifecycle);

}  // namespace dh2::actors
