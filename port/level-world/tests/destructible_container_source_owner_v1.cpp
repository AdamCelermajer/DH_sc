#include "../destructible_container_source_owner_v1.hpp"

#include <cstdlib>
#include <iostream>

using namespace dh2::destructible_container_source_owner_v1;
using dh2::object_manager_runtime_owner_v1::Owner;
using dh2::object_manager_runtime_owner_v1::SourceInteractiveClass;
using dh2::world::SourceObjectCensusV1;

namespace {

void require(bool value, const char* message) {
    if (!value) {
        std::cerr << message << '\n';
        std::exit(1);
    }
}

SourceObjectCensusV1 source_row() {
    SourceObjectCensusV1 source{};
    source.module_index = 3;
    source.source_module_id = 3;
    source.source_record = 17;
    source.source_handle = 91;
    source.name = "_prim_DestructibleContainer";
    source.gametype = "DestructibleContainer";
    source.template_name = "DestructibleContainer";
    source.data_desc = "Swamp_Normal_DestructibleBarrel";
    source.data_desc_pydata = "DestructibleContainers";
    source.world.position[0] = 12.5f;
    source.world.position[1] = -7.0f;
    source.world.position[2] = 250.0f;
    source.world.scale[0] = source.world.scale[1] = source.world.scale[2] = 1.0f;
    return source;
}

} // namespace

int main() {
    Owner manager;
    Record record{};
    auto source = source_row();
    InitPostFacts facts{};
    facts.aabb = {-42.0f, -21.0f, 42.0f, 21.0f, 1};
    facts.container_state_394 = 2;
    std::string error;
    require(initialize_init_post(manager, source, facts, record, error) ==
                Status::complete,
            "completed InitPost facts failed to publish");
    require(error.empty() && record.object && record.object->identity ==
                reinterpret_cast<std::uintptr_t>(&record),
            "record identity is not the canonical manager identity");
    require(manager.find_by_source_handle(91) == record.object &&
                manager.find_by_identity(record.object->identity) == record.object,
            "source handle and identity do not resolve to the same canonical row");
    require(record.object->visible_80 == 1 && record.object->visible_80_ready == 1 &&
                record.object->save_eligibility_28 == 1,
            "authored visibility/default and skip_checkpoint save gate were conflated with InitPost condition state");
    require(record.object->source_provenance_ready &&
                record.object->source_module_index == 3 &&
                record.object->source_record == 17 &&
                record.object->world_x == 12.5f && record.object->world_y == -7.0f,
            "MGP provenance or source world position was not retained");
    require(record.object->interaction_radius_94_ready &&
                record.object->interaction_radius_94 == 21.0f,
            "interaction radius did not use half the smaller completed AABB extent");
    float final_bounds[4]{};
    require(init_final_absolute_bounds(record, final_bounds, error) ==
                Status::complete && final_bounds[0] == -42.0f &&
                final_bounds[1] == -21.0f && final_bounds[2] == 42.0f &&
                final_bounds[3] == 21.0f,
            "Container InitFinal did not retain the exact InitPost absolute AABB");
    auto* retained = record.object;
    require(initialize_init_post(manager, source, facts, record, error) ==
                Status::invalid_argument && record.object == retained,
            "repeated initialization must preserve the canonical published owner");
    require(record.object->live_fields.deleted_81 == &record.deleted_81 &&
                record.object->live_fields.serialized_flag_8a == &record.enabled_8a,
            "deleted and enabled fields must bind to distinct canonical bytes");
    bool interactive = false;
    require(dh2::object_manager_runtime_owner_v1::evaluate_source_interactive(
                *record.object, SourceInteractiveClass::container, 44,
                &interactive) && interactive,
            "Container::IsInteractive projection was not published");
    dh2::object_manager_runtime_owner_v1::Owner::RoomObjectCursor cursor{};
    std::uintptr_t room_identity = 0;
    require(manager.room_object_cursor_reset(&cursor) &&
                !manager.room_object_cursor_at_end(&cursor) &&
                manager.room_object_cursor_get(&cursor, &room_identity) &&
                room_identity == record.object->identity,
            "InitPost producer did not append the canonical identity to room list");

    Owner unavailable_manager;
    Record unavailable_record{};
    InitPostFacts unavailable_facts = facts;
    unavailable_facts.aabb.ready = 0;
    require(initialize_init_post(unavailable_manager, source, unavailable_facts,
                                 unavailable_record, error) == Status::complete &&
                unavailable_manager.find_by_source_handle(91) ==
                    unavailable_record.object &&
                !unavailable_record.object->interaction_radius_94_ready,
            "unavailable AABB must not invent a radius or block source factory publication");
    require(init_final_absolute_bounds(unavailable_record, final_bounds, error) ==
                Status::aabb_unavailable,
            "Container InitFinal must not invent physical bounds without the source AABB");
    Owner inactive_manager;
    Record inactive_record{};
    auto inactive_facts = unavailable_facts;
    inactive_facts.condition_enabled = 0;
    inactive_facts.container_state_394 = 4;
    require(initialize_init_post(inactive_manager, source, inactive_facts,
                                 inactive_record, error) == Status::complete &&
                inactive_record.visible_80 == 1 && inactive_record.enabled_8a == 1 &&
                inactive_record.zoning_enabled_2ee == 1,
            "false activation condition must not overwrite authored visibility or constructor enabled/zoning facts");

    Owner overridden_manager;
    Record overridden_record{};
    auto overridden_source = source;
    overridden_source.source_handle = 92;
    overridden_source.name = "_prim_hidden_skip_checkpoint";
    auto overridden_facts = facts;
    overridden_facts.visible_80 = 0;
    overridden_facts.save_eligibility_28 = 0;
    require(initialize_init_post(overridden_manager, overridden_source,
                                 overridden_facts, overridden_record, error) ==
                Status::complete && overridden_record.object->visible_80 == 0 &&
                overridden_record.object->visible_80_ready == 1 &&
                overridden_record.object->save_eligibility_28 == 0,
            "MGP visible/skip_checkpoint overrides did not publish onto the same ObjectManager row");
    return 0;
}
