#include "character_script_summon_v1.hpp"

#include <utility>

namespace dh2::character_script_summon_v1 {
namespace {

bool valid(const Request& request, const Services& services) noexcept {
    const auto& args = request.arguments;
    return request.container_identity != 0 && args.character_table_oid >= 0 &&
        !request.character_table_name.empty() && args.request_spawn_state <= 1 &&
        args.relative_to_summoner <= 1 && services.prepare_placement &&
        services.load_default_properties && services.set_initial_position &&
        services.set_rotation && services.set_summon_flag_5348 &&
        services.try_room_enrollment && services.add_no_room_object &&
        services.zone_entered && services.set_spawn_state;
}

Status fail(Status status, Stage stage, Result& result, std::string& error,
            const char* fallback) {
    result.reached = stage;
    if (error.empty()) error = fallback;
    return status;
}

} // namespace

Status summon(character_runtime_factory_v1::Owner& factory,
              const Request& request, const manager::GameObject& manager_seed,
              const aggro::GameObject& aggro_seed,
              const character_runtime_factory_v1::Services& factory_services,
              const Services& services, Result* output, std::string& error) {
    if (!output) return Status::invalid_argument;
    *output = {};
    if (!valid(request, services)) {
        error = "Summon request or required source lifecycle service is invalid";
        return Status::invalid_argument;
    }

    Placement placement{};
    if (services.prepare_placement(services.context, request, &placement, error) != 0)
        return fail(Status::placement_failed, Stage::none, *output, error,
                    "source Summon placement query failed");
    output->reached = Stage::placement_prepared;
    output->floor_hit = placement.floor_hit;

    character_runtime_factory_v1::Record* record = nullptr;
    character_runtime_factory_v1::Result factory_result{};
    std::string generated_name;
    const auto created = factory.create_script_npc(
        request.arguments.character_table_oid, request.character_table_name,
        manager_seed, aggro_seed, factory_services, &record, &factory_result,
        generated_name, error);
    output->factory_stage_mask = factory_result.constructor.completed_steps;
    if (created != character_runtime_factory_v1::Status::complete || !record ||
        !record->character.identity || !record->object_registered ||
        !record->character_listed) {
        if (error.empty()) error = "canonical Character factory did not publish the Summon";
        return Status::factory_failed;
    }
    output->character_identity = record->character.identity;
    output->source_handle = record->source_handle;
    output->reached = Stage::character_constructed;
    output->spawn_state_requested = request.arguments.request_spawn_state != 0;

    if (services.load_default_properties(services.context, *record, error) != 0)
        return fail(Status::properties_failed, Stage::character_constructed,
                    *output, error, "CharacterTable default property load failed");
    character_runtime_factory_v1::Result property_result{};
    const auto properties_ready = factory.mark_source_properties_ready(
        record->source_handle, &property_result, error);
    if (properties_ready != character_runtime_factory_v1::Status::complete ||
        !record->source_properties_ready())
        return fail(Status::properties_failed, Stage::character_constructed,
                    *output, error, "canonical Character properties did not become ready");
    output->reached = Stage::properties_loaded;

    if (services.set_initial_position(services.context, *record,
                                      placement.position.data(), error) != 0)
        return fail(Status::placement_failed, Stage::properties_loaded,
                    *output, error, "Character initial placement failed");
    output->reached = Stage::positioned;
    if (services.set_rotation(services.context, *record,
                              placement.rotation.data(), error) != 0)
        return fail(Status::placement_failed, Stage::positioned,
                    *output, error, "Character Summon rotation failed");
    output->reached = Stage::rotated;
    if (services.set_summon_flag_5348(services.context, *record, error) != 0)
        return fail(Status::service_failed, Stage::rotated,
                    *output, error, "Character+5348/0x14e4 Summon flag write failed");
    output->reached = Stage::summon_flag_written;

    const int room_status = services.try_room_enrollment(
        services.context, request.container_identity, *record, error);
    if (room_status < 0)
        return fail(Status::room_failed, Stage::summon_flag_written,
                    *output, error, "RoomZone enrollment failed");
    output->room_accepted = room_status == 1;
    if (output->room_accepted) {
        output->reached = Stage::room_enrolled;
    } else {
        if (services.add_no_room_object(services.context, *record, error) != 0)
            return fail(Status::no_room_failed, Stage::summon_flag_written,
                        *output, error, "AddNoRoomObject fallback failed");
        output->no_room_fallback = true;
        output->reached = Stage::no_room_fallback;
        if (services.zone_entered(services.context, *record, error) != 0)
            return fail(Status::zone_failed, Stage::no_room_fallback,
                        *output, error, "ZoneEntered fallback failed");
        output->reached = Stage::zone_entered;
    }

    if (request.arguments.request_spawn_state != 0) {
        if (services.set_spawn_state(services.context, *record, error) != 0)
            return fail(Status::spawn_state_failed, output->reached,
                        *output, error, "SM_SetSpawnState(false,false) failed");
        output->reached = Stage::spawn_state_applied;
    }
    output->reached = Stage::complete;
    return Status::complete;
}

} // namespace dh2::character_script_summon_v1
