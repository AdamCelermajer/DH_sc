#include "door_runtime_v1.hpp"
#include "door_world_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::doors;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply isolated asset-stage root");
        AssetCatalog assets(argv[1]);
        containers::ContainerTablesV1 tables;
        std::string error;
        check(tables.load(assets, error), "load source door tables: " + error);
        const auto& borrow = tables.borrow();
        check(borrow.doors.names && !borrow.doors.names->empty(), "source Arrays::Doors rows available");

        ActorDefinition definition{};
        definition.stableId = 0xD00F;
        definition.name = "restore-contract-fixture";
        definition.gametype = "Door";
        definition.properties["data"] = borrow.doors.names->front();
        definition.properties["door_collision"] = "0";
        definition.properties["rotation"] = "0.0,0.0,90.5281";
        definition.properties["scale"] = "1.92012,1.0,1.86059";
        auto second_definition = definition;
        second_definition.stableId += 1;
        second_definition.name += "-second";
        DoorRuntimeV1 runtime;
        std::vector<std::string> notices;
        check(runtime.load(tables, {definition, second_definition}, notices, error), "load runtime fixture: " + error);
        check(runtime.size() == 2, "two source Door declarations loaded");
        check(runtime.instance(0).rotation_degrees[2] > 90.52f &&
              runtime.instance(0).rotation_degrees[2] < 90.54f &&
              runtime.instance(0).visual_scale[0] > 1.92f &&
              runtime.instance(0).visual_scale[2] > 1.86f,
              "source comma-separated MGP rotation and scale tuples are parsed");
        check(runtime.adopt(assets, notices, error), "adopt source door visual: " + error);
        dh2::data::AiTables ai{};
        check(load_original_ai_tables(assets, "original-cache/data/pydata/", ai, error),
              "load source AI/faction rows: " + error);
        dh2::data::CombatRandom random{1, 0};
        PlayableActorWorld world(ai, random);
        check(bind_door_world_objects_v1(world, runtime, notices, error), "bind door world objects: " + error);

        for (std::uint8_t serialized = 0; serialized <= 3; ++serialized) {
            (void)runtime.take_sound_requests();
            (void)runtime.take_changed();
            check(world.set_object_component(definition.stableId, door_state_component_v1, {serialized}, error),
                  "set persisted restore fixture: " + error);
            check(restore_door_world_state_v1(world, runtime, error), "restore persisted state: " + error);
            const bool opened = serialized == 1 || serialized == 3;
            check(runtime.state(0) == (opened ? DoorStateV1::opened : DoorStateV1::closed),
                  "restore maps serialized state to source terminal state");
            check(runtime.collision_closed(0) == !opened && !runtime.busy(0) && !runtime.blocking(0),
                  "restore sets source filter and clears transition busy state");
            check(runtime.take_sound_requests().empty(), "restore does not replay transition sound");
            check(runtime.take_changed().empty(), "restore does not enqueue transition checkpoint event");
            const auto* restored_object = world.find_object(definition.stableId);
            check(restored_object && restored_object->state_components.at(door_state_component_v1).front() ==
                  static_cast<std::uint8_t>(opened ? DoorStateV1::opened : DoorStateV1::closed),
                  "restore rewrites persisted transitional value to normalized terminal checkpoint");

            if (opened) {
                check(runtime.close(0, true, error), "close after opened restore: " + error);
                check(runtime.state(0) == DoorStateV1::closed && runtime.collision_closed(0),
                      "close command is admitted after opened restore");
            } else {
                check(runtime.open(0, true, error), "open after closed restore: " + error);
                check(runtime.state(0) == DoorStateV1::opened && !runtime.collision_closed(0),
                      "open command is admitted after closed restore");
            }
        }

        check(runtime.restore(0, DoorStateV1::opened, error), "prepare invalid restore atomicity: " + error);
        const auto old_state = runtime.state(0);
        const bool old_filter = runtime.collision_closed(0);
        check(world.set_object_component(definition.stableId, door_state_component_v1, {3}, error),
              "prepare first door for multi-entry atomic restore: " + error);
        check(world.set_object_component(second_definition.stableId, door_state_component_v1, {4}, error),
              "prepare malformed second door: " + error);
        (void)runtime.take_sound_requests();
        check(!restore_door_world_state_v1(world, runtime, error), "invalid serialized door state rejected");
        const auto* first_after = world.find_object(definition.stableId);
        const auto* second_after = world.find_object(second_definition.stableId);
        check(runtime.state(0) == old_state && runtime.collision_closed(0) == old_filter && !runtime.busy(0) &&
              first_after->state_components.at(door_state_component_v1) == std::vector<std::uint8_t>{3} &&
              second_after->state_components.at(door_state_component_v1) == std::vector<std::uint8_t>{4} &&
              runtime.take_sound_requests().empty() && runtime.take_changed().empty(),
              "invalid restore leaves state, filter, events, and sound untouched");
        // Authored clip timelines do not start at zero (the source close clip spans 8000..9333 ms).
        // The visual clock is relative to that start, so a transition finishes at the absolute end.
        check(!door_clip_reached_v1(8000.0, 1.332, 9333.0), "close clip must not finish before its authored end");
        check(door_clip_reached_v1(8000.0, 1.333, 9333.0), "close clip must finish at its authored end");
        check(!door_clip_reached_v1(8000.0, 0.499, 8500.0), "marker must not fire before its authored time");
        check(door_clip_reached_v1(8000.0, 0.5, 8500.0), "marker must fire at its authored time");
        check(door_clip_reached_v1(0.0, 2.0, 2000.0), "zero-start clip keeps its absolute timeline");
        // Regression: the pre-fix comparison (no clip start) stalls at the clip duration and never completes.
        check(!door_clip_reached_v1(0.0, 1.333, 9333.0), "relative clock must not be compared with absolute end");

        std::cout << "PASS: door restore terminal-state, admission, and atomic invalid cases\n";
        std::cout << "PASS: door transition clock uses authored clip timeline\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
