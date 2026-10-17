#include "character_faery_live_binding_v1.hpp"

namespace dh2::character_faery_live_binding_v1 {
namespace {
namespace ctor = character_constructor_owner_v1;

bool source_character_ready(const character_runtime_factory_v1::Record& record,
                            const game_object_position_owner_v1::Owner& position) {
    const auto identity = record.constructor.identity;
    if (!identity || record.character.identity != identity ||
        record.game_object.identity != identity || position.identity != identity ||
        !record.constructor.constructor_complete ||
        record.constructor.registered_states != ctor::all_registered_states ||
        !record.object_registered || !record.character_listed ||
        !record.source_properties_ready() || !record.init_post_complete() ||
        !record.init_final_complete()) {
        return false;
    }

    // A Faery needs the same canonical Character components used by the
    // constructor before the Faery-specific script/visual owners are queried.
    constexpr ctor::Component required[] = {
        ctor::Component::game_object,
        ctor::Component::ai,
        ctor::Component::animator,
        ctor::Component::state_machine,
        ctor::Component::properties,
        ctor::Component::controller,
    };
    for (const auto component : required) {
        if (!record.components.find(component)) return false;
    }
    return true;
}
} // namespace

Status validate_character_binding(const Binding& binding, bool faery,
                                  std::string& error) {
    if (!binding.character || !binding.position ||
        !binding.placement.invoke || !binding.validate_graph) {
        error = "Faery placement requires a borrowed live graph and source providers";
        return Status::invalid_argument;
    }
    if (!source_character_ready(*binding.character, *binding.position)) {
        error = "Faery Character factory, source component construction, properties, InitPost or InitFinal is incomplete";
        return Status::graph_incomplete;
    }
    if (!binding.components.physical || !binding.position->physical_object ||
        binding.position->physical_object != reinterpret_cast<
            game_object_position_owner_v1::Identity>(binding.components.physical)) {
        error = "Faery physical owner is missing or does not match this Character's position owner";
        return Status::graph_incomplete;
    }
    if (!binding.components.visual || !binding.position->visual_object ||
        binding.position->visual_object != reinterpret_cast<
            game_object_position_owner_v1::Identity>(binding.components.visual)) {
        error = "Faery VisualObject owner is missing or does not match this Character's position owner";
        return Status::graph_incomplete;
    }
    if (faery && !binding.components.ais_faery) {
        error = "Faery AISFaery owner is missing";
        return Status::graph_incomplete;
    }
    if (faery && !binding.components.script_session) {
        error = "Faery AISFaery CharAI/LuaScript session is missing";
        return Status::graph_incomplete;
    }
    if (faery && !character_faery_script_session_v1::ready(
            *binding.components.script_session,
            binding.character->constructor.identity,
            reinterpret_cast<character_faery_script_session_v1::Identity>(
                binding.character->components.find(
                    character_constructor_owner_v1::Component::ai)),
            reinterpret_cast<character_faery_script_session_v1::Identity>(
                binding.components.ais_faery),
            binding.components.script_services)) {
        error = "Faery AISFaery session does not match this Character/CharAI owner or is not initialized";
        return Status::graph_incomplete;
    }
    if (faery && !binding.components.pofaerie) {
        error = "Faery POFaerie physical callback owner is missing";
        return Status::graph_incomplete;
    }
    const auto& position = *binding.position;
    const auto& services = binding.position_services;
    if ((position.instance_transform && !services.translate_instance_transform) ||
        !services.update_absolute_aabb || !services.physical_set_position ||
        !services.visual_sync_position || !services.set_destination ||
        !services.visual_force_update_position) {
        error = "Faery GameObject position providers are incomplete";
        return Status::graph_incomplete;
    }
    try {
        if (binding.validate_graph(binding.validation_context,
                *binding.character, *binding.position,
                binding.components, error) == 0) {
            error.clear();
            return Status::complete;
        }
    } catch (...) {
        error = "Faery source graph validator threw";
    }
    if (error.empty()) error = "Faery source graph does not bind to one Character identity";
    return Status::graph_stale;
}

Status validate_binding(const Binding& binding, std::string& error) {
    return validate_character_binding(binding, true, error);
}

Status Owner::validate(std::string& error) const {
    return validate_binding(binding_, error);
}

int Owner::dispatch(void* raw,
                    const character_faery_placement_v1::Request& request,
                    character_faery_placement_v1::Reply* reply) {
    auto& self = *static_cast<Owner*>(raw);
    if (!reply) return 1;
    auto& binding = self.binding_;
    if (request.operation == character_faery_placement_v1::Operation::set_position) {
        if (!binding.position || request.subject != binding.position->identity ||
            !request.vector)
            return 1;
        std::string error;
        const game_object_position_owner_v1::Point3 point{
            request.vector->x, request.vector->y, request.vector->z};
        return game_object_position_owner_v1::set_position(
            binding.position, point, true, &binding.position_services, error) ==
            game_object_position_owner_v1::Status::complete ? 0 : 1;
    }
    if (request.operation == character_faery_placement_v1::Operation::force_update_position) {
        if (!binding.position || request.subject != binding.position->identity)
            return 1;
        std::string error;
        return game_object_position_owner_v1::force_update_position(
            binding.position, &binding.position_services, error) ==
            game_object_position_owner_v1::Status::complete ? 0 : 1;
    }
    return binding.placement.invoke(binding.placement.context, request, reply);
}

Status Owner::place(std::uintptr_t explicit_player_character,
                    character_faery_placement_v1::Result* result,
                    std::string& error) {
    error.clear();
    if (!result || !explicit_player_character) {
        error = "Faery placement requires an explicit live Player Character";
        return Status::invalid_argument;
    }
    const auto status = validate(error);
    if (status != Status::complete) return status;
    const auto placement_status = runtime_.place(explicit_player_character,
                                                  result);
    if (placement_status != character_faery_placement_v1::Status::complete) {
        error = "Source Faery placement service failed at operation " +
            std::to_string(static_cast<unsigned>(result->last_operation));
        return Status::placement_failed;
    }
    error.clear();
    return Status::complete;
}

} // namespace dh2::character_faery_live_binding_v1
