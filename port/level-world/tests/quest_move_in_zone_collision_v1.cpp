#include "../quest_move_in_zone_collision_v1.hpp"

#include <cstdio>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::level_world::quest_move_in_zone_collision_v1;

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    objects::Owner* manager{};
    events::Runtime* event_manager{};
    bool source_gate{true};
    bool level_present{true};
    std::int32_t event_type{91};
    std::vector<int> order;
    events::Event received{};
};

bool virtual_24(void* raw, objects::GameObject&, bool& result,
                std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.order.push_back(1);
    result = fixture.source_gate;
    error.clear();
    return true;
}

bool current_level(void* raw, ActiveLevelView& result, bool& present,
                   std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.order.push_back(2);
    present = fixture.level_present;
    result = present ? ActiveLevelView{fixture.manager}
                     : ActiveLevelView{};
    error.clear();
    return true;
}

bool move_constant(void* raw, std::int32_t& result, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.order.push_back(3);
    result = fixture.event_type;
    error.clear();
    return true;
}

bool raise_event(void* raw, events::Event& event, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    events::Result result{};
    return fixture.event_manager->raise(event, result, error) ==
        events::Status::complete;
}

std::int32_t receive(void* raw, events::Runtime&, events::Event& event,
                     std::string&) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.order.push_back(4);
    fixture.received = event;
    return 0;
}

objects::GameObject object(std::uintptr_t identity,
                           objects::SourceHandle source_handle) {
    objects::GameObject result{};
    result.identity = identity;
    result.source_handle = source_handle;
    return result;
}
}

int main() {
    try {
        objects::Owner manager;
        auto zone = object(0x1234567800000010ULL, 17);
        auto character = object(0x1234567800000020ULL, 3);
        objects::GameObject* stored_zone = nullptr;
        objects::GameObject* stored_character = nullptr;
        check(manager.add_object(17, zone, &stored_zone) == objects::Status::ok &&
              manager.add_object(3, character, &stored_character) == objects::Status::ok,
              "canonical GameObject registration failed");

        BoundZone binding{};
        std::string error;
        check(bind(manager, 17, 0x4567, binding, error) == Status::complete &&
              binding.object == stored_zone && binding.network_id == 0x4567,
              "zone did not bind to the existing source ObjectManager row");

        const float zone_position[3]{178.335f, 246.863f, 255.074f};
        const float authored_scale[3]{7.30955f, 5.70483f, 2.02567f};
        const float source_dimensions[3]{200.f, 200.f, 200.f};
        PhysicalSensor sensor{};
        check(build_physical_sensor(zone_position, authored_scale,
                                    source_dimensions, sensor, error) ==
                  Status::complete &&
              std::fabs(sensor.position[0] - 1.78335f) < 0.00001f &&
              std::fabs(sensor.position[1] - 2.46863f) < 0.00001f &&
              std::fabs(sensor.half_extents[0] - 7.30955f) < 0.00001f &&
              std::fabs(sensor.half_extents[1] - 5.70483f) < 0.00001f &&
              sensor.group_index == -5 && sensor.category_bits == 0x0800 &&
              sensor.mask_bits == 1310 && sensor.sensor,
              "Zone::InitPost source dimensions/scale/filter projection changed");

        float source_aabb_xyxy[4]{};
        check(build_init_post_local_aabb(authored_scale, source_dimensions,
                                         source_aabb_xyxy, error) ==
                  Status::complete &&
              std::fabs(source_aabb_xyxy[0] + 100.f * authored_scale[0]) < 0.00001f &&
              std::fabs(source_aabb_xyxy[1] + 100.f * authored_scale[1]) < 0.00001f &&
              std::fabs(source_aabb_xyxy[2] - 100.f * authored_scale[0]) < 0.00001f &&
              std::fabs(source_aabb_xyxy[3] - 100.f * authored_scale[1]) < 0.00001f,
              "Zone::InitPost default Property44 dimensions do not make the centered AABB");
        const float expected_source_aabb_xyxy[]{
            -100.f * authored_scale[0], -100.f * authored_scale[1],
             100.f * authored_scale[0],  100.f * authored_scale[1]};
        std::copy(std::begin(expected_source_aabb_xyxy),
                  std::end(expected_source_aabb_xyxy), source_aabb_xyxy);
        PhysicalSensor canonical_sensor{};
        check(build_physical_sensor_from_aabb(zone_position, source_aabb_xyxy,
                                              canonical_sensor, error) ==
                  Status::complete &&
              std::fabs(canonical_sensor.position[0] - sensor.position[0]) < 0.00001f &&
              std::fabs(canonical_sensor.position[1] - sensor.position[1]) < 0.00001f &&
              std::fabs(canonical_sensor.half_extents[0] - sensor.half_extents[0]) < 0.00001f &&
              std::fabs(canonical_sensor.half_extents[1] - sensor.half_extents[1]) < 0.00001f,
              "PhysicalObject sensor did not use the canonical InitPost AABB extents");

        check(bind(manager, 17, -1, binding, error) == Status::complete &&
              binding.network_id == -1,
              "default ObjectBase word +25 network id was rejected");
        check(bind(manager, 17, 0x4567, binding, error) == Status::complete,
              "source-assigned ObjectBase word +25 network id failed to bind");

        events::Runtime runtime;
        Fixture fixture{};
        fixture.manager = &manager;
        fixture.event_manager = &runtime;
        bool attached = false;
        check(runtime.attach(91, 0x8899, 0, &fixture, receive, attached, error) ==
                  events::Status::complete && attached,
              "MoveInZone test receiver attach failed");
        const Services services{&fixture, virtual_24, current_level,
                                move_constant, raise_event};
        Result result{};
        check(on_collision_begins(binding, stored_character, services,
                                  result, error) == Status::complete &&
              result.event_raised && fixture.order == std::vector<int>{1, 2, 3, 4},
              "source callback/level/constant/event ordering changed");
        check(fixture.received.objective_type == 91 &&
              fixture.received.character == stored_character->identity &&
              fixture.received.network_id == 0x4567 &&
              fixture.received.flag0 == 0 && fixture.received.flag1 == 0 &&
              fixture.received.subject_id == -1 &&
              fixture.received.object_identity == stored_zone->identity,
              "MoveInZone event payload lost its source identity fields");

        fixture.order.clear();
        fixture.source_gate = false;
        check(on_collision_begins(binding, stored_character, services,
                                  result, error) == Status::no_event &&
              fixture.order == std::vector<int>{1},
              "false virtual +0x24 gate did not stop before current Level lookup");

        fixture.order.clear();
        fixture.source_gate = true;
        fixture.level_present = false;
        check(on_collision_begins(binding, stored_character, services,
                                  result, error) == Status::no_event &&
              fixture.order == std::vector<int>{1, 2},
              "missing current Level did not stop before constant lookup");

        check(on_collision_begins(binding, nullptr, services, result, error) ==
                  Status::invalid_argument,
              "null collision object crossed the source assertion boundary");
        std::puts("{\"validation\":\"PASS\",\"canonical_zone_binding\":true,\"source_callback_order\":true,\"move_in_zone_payload\":true,\"virtual_gate_early_return\":true,\"current_level_early_return\":true}");
        return 0;
    } catch (const std::exception& exception) {
        std::fprintf(stderr, "%s\n", exception.what());
        return 1;
    }
}
