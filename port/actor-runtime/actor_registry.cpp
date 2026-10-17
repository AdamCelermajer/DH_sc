#include "actor_registry.hpp"

#include <cmath>
#include <cstdlib>
#include <cstring>

namespace dh2::actors {
namespace {

bool bounded_length(const char* value, std::uint32_t capacity,
                    std::uint32_t* length) {
    if (!value || !capacity || !length) return false;
    std::uint32_t i = 0;
    while (i < capacity && value[i] != '\0') ++i;
    if (i == capacity) return false;
    *length = i;
    return true;
}

bool copy_text(char* output, std::uint32_t capacity, const char* input,
               bool allow_empty) {
    std::uint32_t length = 0;
    if (!output || !input || !bounded_length(input, capacity, &length) ||
        (!allow_empty && length == 0)) return false;
    std::memcpy(output, input, length + 1U);
    return true;
}

bool copy_optional_field(const dh2::world::Object* object, const char* name,
                         char* output, std::uint32_t capacity,
                         std::uint8_t* present) {
    if (!object || !name || !output || !capacity || !present ||
        (object->field_count && !object->fields)) return false;
    *present = 0;
    output[0] = '\0';
    const char* value = nullptr;
    for (std::uint32_t i = 0; i < object->field_count; ++i) {
        const auto& field = object->fields[i];
        if (!field.name || !field.value) return false;
        if (std::strcmp(field.name, name) == 0) {
            if (value) return false;
            value = field.value;
        }
    }
    if (!value) return true;
    if (!copy_text(output, capacity, value, true)) return false;
    *present = 1;
    return true;
}

bool finite3(const float value[3]) {
    return value && std::isfinite(value[0]) && std::isfinite(value[1]) &&
           std::isfinite(value[2]);
}

bool is_character(const dh2::world::Object& object) {
    return object.gametype && std::strcmp(object.gametype, "Character") == 0;
}

bool copy_actor(ActorInstance* output, const dh2::world::SourceLevel* level,
                const dh2::world::Object* object) {
    if (!output || !level || !object || !is_character(*object) ||
        object->kind != dh2::world::RecordKind::mgp ||
        object->module_index == dh2::world::no_module ||
        object->module_index >= level->module_count || !level->modules ||
        !object->source_path || !object->name || !finite3(object->local.position) ||
        !finite3(object->local.rotation_degrees) || !finite3(object->local.scale) ||
        !finite3(object->world_position)) return false;

    const auto& module = level->modules[object->module_index];
    if (!module.record.name || !module.mgp_loaded ||
        !copy_text(output->level_name, sizeof(output->level_name), level->name, false) ||
        !copy_text(output->module_name, sizeof(output->module_name),
                   module.record.name, false) ||
        !copy_text(output->source_path, sizeof(output->source_path),
                   object->source_path, false) ||
        !copy_text(output->name, sizeof(output->name), object->name, false) ||
        !copy_optional_field(object, "char_template", output->character_template,
                             sizeof(output->character_template),
                             &output->has_character_template) ||
        !copy_optional_field(object, "char_template_pydata", output->template_data_class,
                             sizeof(output->template_data_class),
                             &output->has_template_data_class) ||
        !copy_optional_field(object, "_templateName", output->editor_template_name,
                             sizeof(output->editor_template_name),
                             &output->has_editor_template_name) ||
        !copy_optional_field(object, "ai_state", output->ai_state,
                             sizeof(output->ai_state), &output->has_ai_state)) return false;

    char auto_spawn[8]{};
    if (!copy_optional_field(object, "auto_spawn", auto_spawn,
                             sizeof(auto_spawn), &output->has_auto_spawn)) return false;
    if (output->has_auto_spawn) {
        if (std::strcmp(auto_spawn, "0") == 0) output->auto_spawn = 0;
        else if (std::strcmp(auto_spawn, "1") == 0) output->auto_spawn = 1;
        else return false;
    } else {
        output->auto_spawn = 0;
    }

    output->module_index = object->module_index;
    output->source_record = object->source_record;
    output->source_begin = object->source_begin;
    output->source_end = object->source_end;
    for (std::uint32_t axis = 0; axis < 3; ++axis) {
        output->local_position[axis] = object->local.position[axis];
        output->world_position[axis] = object->world_position[axis];
        output->local_rotation_degrees[axis] = object->local.rotation_degrees[axis];
        output->local_scale[axis] = object->local.scale[axis];
    }
    output->lifecycle = Lifecycle::registered;
    output->spawn_transition_count = 0;
    return true;
}

bool name_is_unique(const ActorInstance* actors, std::uint32_t count,
                    const char* name) {
    for (std::uint32_t i = 0; i < count; ++i)
        if (std::strcmp(actors[i].name, name) == 0) return false;
    return true;
}

void saturating_increment(std::uint32_t* value) {
    if (*value != UINT32_MAX) ++*value;
}

}  // namespace

Error init(Registry* out, const dh2::world::SourceLevel* level) {
    if (!out || !level || !level->name ||
        (level->module_count && !level->modules) ||
        (level->entity_count && !level->entities)) return Error::argument;

    std::uint32_t actor_count = 0;
    for (std::uint32_t i = 0; i < level->entity_count; ++i) {
        const auto& object = level->entities[i];
        if (is_character(object)) {
            if (actor_count == MAX_ACTORS) return Error::capacity;
            ++actor_count;
        }
    }

    ActorInstance* candidate = nullptr;
    if (actor_count) {
        if (static_cast<std::size_t>(actor_count) >
            SIZE_MAX / sizeof(ActorInstance)) return Error::capacity;
        candidate = static_cast<ActorInstance*>(
            std::calloc(actor_count, sizeof(ActorInstance)));
        if (!candidate) return Error::allocation;
    }

    std::uint32_t next_index = 0;
    for (std::uint32_t i = 0; i < level->entity_count; ++i) {
        const auto& object = level->entities[i];
        if (!is_character(object)) continue;
        if (!copy_actor(&candidate[next_index], level, &object)) {
            std::free(candidate);
            return Error::invalid_source_record;
        }
        if (!name_is_unique(candidate, next_index, candidate[next_index].name)) {
            std::free(candidate);
            return Error::duplicate_name;
        }
        ++next_index;
    }

    std::free(out->actors);
    out->actors = candidate;
    out->actor_count = actor_count;
    out->request_count = 0;
    out->lookup_miss_count = 0;
    out->last_lookup_miss[0] = '\0';
    return Error::ok;
}

void destroy(Registry* registry) {
    if (!registry) return;
    std::free(registry->actors);
    *registry = {};
}

const ActorInstance* find(const Registry* registry, const char* exact_name) {
    if (!registry || !exact_name) return nullptr;
    for (std::uint32_t i = 0; i < registry->actor_count; ++i)
        if (std::strcmp(registry->actors[i].name, exact_name) == 0)
            return &registry->actors[i];
    return nullptr;
}

SpawnResult request_spawn(Registry* registry, const char* exact_name) {
    if (!registry || !exact_name) return SpawnResult::invalid_request;
    std::uint32_t length = 0;
    if (!bounded_length(exact_name, MAX_ACTOR_NAME, &length) || length == 0)
        return SpawnResult::invalid_request;

    saturating_increment(&registry->request_count);
    ActorInstance* actor = nullptr;
    for (std::uint32_t i = 0; i < registry->actor_count; ++i) {
        if (std::strcmp(registry->actors[i].name, exact_name) == 0) {
            actor = &registry->actors[i];
            break;
        }
    }
    if (!actor) {
        saturating_increment(&registry->lookup_miss_count);
        std::memcpy(registry->last_lookup_miss, exact_name, length);
        registry->last_lookup_miss[length] = '\0';
        return SpawnResult::lookup_miss;
    }
    if (actor->lifecycle == Lifecycle::spawn_requested)
        return SpawnResult::already_requested;
    actor->lifecycle = Lifecycle::spawn_requested;
    saturating_increment(&actor->spawn_transition_count);
    return SpawnResult::requested;
}

const char* error_name(Error error) {
    switch (error) {
        case Error::ok: return "ok";
        case Error::argument: return "argument";
        case Error::capacity: return "capacity";
        case Error::allocation: return "allocation";
        case Error::duplicate_name: return "duplicate_name";
        case Error::invalid_source_record: return "invalid_source_record";
    }
    return "unknown";
}

const char* spawn_result_name(SpawnResult result) {
    switch (result) {
        case SpawnResult::requested: return "requested";
        case SpawnResult::already_requested: return "already_requested";
        case SpawnResult::lookup_miss: return "lookup_miss";
        case SpawnResult::invalid_request: return "invalid_request";
    }
    return "unknown";
}

const char* lifecycle_name(Lifecycle lifecycle) {
    switch (lifecycle) {
        case Lifecycle::registered: return "registered";
        case Lifecycle::spawn_requested: return "spawn_requested";
    }
    return "unknown";
}

}  // namespace dh2::actors
