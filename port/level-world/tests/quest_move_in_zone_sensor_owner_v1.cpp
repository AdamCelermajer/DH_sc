#include "../quest_move_in_zone_sensor_owner_v1.hpp"

#include <cassert>
#include <cstdint>
#include <string>

namespace sensor_owner =
    dh2::level_world::quest_move_in_zone_sensor_owner_v1;
namespace collision = dh2::level_world::quest_move_in_zone_collision_v1;
namespace objects = dh2::object_manager_runtime_owner_v1;
namespace events = dh2::level_world::current_level_quest_event_v1;
namespace physical = dh2::physical;

namespace {

struct Fixture {
    objects::Owner* object_manager{};
    events::Runtime* runtime{};
    objects::GameObject* zone{};
    objects::GameObject* player{};
    bool owners_enabled{true};
    unsigned owner_enabled_calls{};
    unsigned zone_virtual_calls{};
    unsigned source_filter_calls{};
    unsigned begin_calls{};
    unsigned raised_events{};
    events::Event received{};
};

bool resolve_peer(void* raw, void* peer_context,
                  objects::GameObject*& output, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    if (peer_context != &fixture || !fixture.player) return false;
    output = fixture.player;
    error.clear();
    return true;
}

bool source_zone_collision_test(void* raw, const collision::BoundZone& zone,
                                objects::GameObject& peer,
                                std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    ++fixture.source_filter_calls;
    error.clear();
    return zone.object == fixture.zone && &peer == fixture.player;
}

bool source_owners_enabled(void* raw, const collision::BoundZone& zone,
                           objects::GameObject& peer, bool& enabled,
                           std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    ++fixture.owner_enabled_calls;
    enabled = zone.object == fixture.zone && &peer == fixture.player &&
        fixture.owners_enabled;
    error.clear();
    return true;
}

bool virtual_24(void* raw, objects::GameObject& peer, bool& result,
                std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    ++fixture.zone_virtual_calls;
    result = &peer == fixture.player;
    error.clear();
    return true;
}

bool current_level(void* raw, collision::ActiveLevelView& view,
                   bool& present, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    view = {fixture.object_manager};
    present = true;
    error.clear();
    return true;
}

bool move_in_zone_constant(void*, std::int32_t& value,
                           std::string& error) {
    value = 91;
    error.clear();
    return true;
}

std::int32_t receive_event(void* raw, events::Runtime&,
                           events::Event& event, std::string&) {
    auto& fixture = *static_cast<Fixture*>(raw);
    ++fixture.raised_events;
    fixture.received = event;
    return 0;
}

bool raise_current_level_event(void* raw, events::Event& event,
                               std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    events::Result result{};
    const auto status = fixture.runtime->raise(event, result, error);
    return status == events::Status::complete;
}

unsigned actor_test(void*, void*, const physical::Filter* a,
                    const physical::Filter* b) {
    if (!a || !b || a->present != 1 || b->present != 1) return 0;
    if (a->group == b->group && a->group != 0) return a->group > 0;
    return (a->mask & b->category) != 0 &&
        (a->category & b->mask) != 0;
}

void actor_contact(void*, physical::ContactEvent, void*, const float*,
                   unsigned) {}

void actor_velocity(void*, float* output) {
    if (output) output[0] = output[1] = 0.0f;
}

}  // namespace

int main() {
    objects::Owner object_manager;
    int zone_source_owner{};
    int player_source_owner{};
    objects::GameObject zone_object{};
    zone_object.identity = reinterpret_cast<std::uintptr_t>(&zone_source_owner);
    zone_object.source_handle = 17;
    objects::GameObject player_object{};
    player_object.identity = reinterpret_cast<std::uintptr_t>(&player_source_owner);
    player_object.source_handle = 3;

    objects::GameObject* zone = nullptr;
    objects::GameObject* player = nullptr;
    assert(object_manager.add_object(17, zone_object, &zone) == objects::Status::ok);
    assert(object_manager.add_object(3, player_object, &player) == objects::Status::ok);

    collision::BoundZone bound_zone{};
    std::string error;
    assert(collision::bind(object_manager, 17, 0x4567, bound_zone, error) ==
           collision::Status::complete);
    assert(bound_zone.object == zone);

    const float world_position[3]{0.0f, 0.0f, 0.0f};
    const float local_aabb[4]{-100.0f, -100.0f, 100.0f, 100.0f};
    collision::PhysicalSensor sensor{};
    assert(collision::build_physical_sensor_from_aabb(
               world_position, local_aabb, sensor, error) ==
           collision::Status::complete);

    events::Runtime runtime;
    Fixture fixture{&object_manager, &runtime, zone, player};
    bool event_attached = false;
    assert(runtime.attach(91, 0x9988, 0, &fixture, receive_event,
                          event_attached, error) == events::Status::complete);
    assert(event_attached);
    const sensor_owner::SensorServices sensor_services{
        &fixture, resolve_peer, source_owners_enabled,
        source_zone_collision_test};
    const collision::Services collision_services{
        &fixture, virtual_24, current_level, move_in_zone_constant,
        raise_current_level_event};

    physical::NativeWorld world;
    const float world_bounds[4]{-50.0f, -50.0f, 50.0f, 50.0f};
    world.load(world_bounds);
    const auto baseline_bodies = world.backend()->GetBodyCount();

    sensor_owner::Owner sensor_owner;
    assert(sensor_owner.attach(world, bound_zone, sensor, sensor_services,
                               collision_services, error) ==
           sensor_owner::Status::complete);
    assert(sensor_owner.attached() && world.backend()->GetBodyCount() ==
           baseline_bodies + 1);

    Fixture* peer_context = &fixture;
    physical::WorldObject player_callbacks{
        peer_context, actor_test, actor_contact, actor_velocity, {0.0f, 0.0f},
        0.0f, 0};
    b2BodyDef player_definition;
    player_definition.position.Set(0.0f, 0.0f);
    player_definition.userData = &player_callbacks;
    b2Body* player_body = world.create(&player_definition);
    assert(player_body);
    b2CircleDef player_shape;
    player_shape.radius = 0.2f;
    player_shape.density = 1.0f;
    player_shape.userData = &player_callbacks;
    player_shape.filter.groupIndex = 0;
    player_shape.filter.categoryBits = 0x0100;
    player_shape.filter.maskBits = 0x0800;
    assert(player_body->CreateShape(&player_shape));
    player_body->SetMassFromShapes();

    world.update(16);
    assert(fixture.owner_enabled_calls > 0 && fixture.source_filter_calls > 0 &&
           fixture.zone_virtual_calls > 0 &&
           fixture.raised_events == 1);
    assert(fixture.received.objective_type == 91 &&
           fixture.received.character == player->identity &&
           fixture.received.network_id == 0x4567 &&
           fixture.received.object_identity == zone->identity);
    assert(sensor_owner.last_contact_status() == sensor_owner::Status::complete &&
           sensor_owner.last_contact_result().event_raised);

    // Source PhysicalObject::onCollisionTest gates the filter and POZone
    // virtual +0xc8 side effect on both owner +0x80 enabled facts.
    player_body->SetXForm(b2Vec2(5.0f, 5.0f), 0.0f);
    world.update(16);
    const auto source_filter_calls_before_disabled = fixture.source_filter_calls;
    const auto zone_virtual_calls_before_disabled = fixture.zone_virtual_calls;
    fixture.owners_enabled = false;
    player_body->SetXForm(b2Vec2(0.0f, 0.0f), 0.0f);
    world.update(16);
    assert(!sensor_owner.last_test_allowed() &&
           fixture.source_filter_calls == source_filter_calls_before_disabled &&
           fixture.zone_virtual_calls == zone_virtual_calls_before_disabled &&
           fixture.raised_events == 1);
    // Box2D does not re-evaluate ShouldCollide for an existing contact just
    // because source owner flags change. Force a real end/begin transition.
    player_body->SetXForm(b2Vec2(5.0f, 5.0f), 0.0f);
    world.update(16);
    fixture.owners_enabled = true;
    player_body->SetXForm(b2Vec2(0.0f, 0.0f), 0.0f);
    world.update(16);
    assert(sensor_owner.last_test_allowed() && fixture.raised_events == 2);

    assert(sensor_owner.detach(error) == sensor_owner::Status::complete);
    assert(!sensor_owner.attached() && world.backend()->GetBodyCount() ==
           baseline_bodies + 1);
    assert(sensor_owner.detach(error) == sensor_owner::Status::complete);
    assert(!sensor_owner.attached() && world.backend()->GetBodyCount() ==
           baseline_bodies + 1);

    assert(sensor_owner.attach(world, bound_zone, sensor, sensor_services,
                               collision_services, error) ==
           sensor_owner::Status::complete);
    assert(sensor_owner.attached() && world.backend()->GetBodyCount() ==
           baseline_bodies + 2);
    world.update(16);
    assert(fixture.raised_events == 3 &&
           sensor_owner.last_contact_result().event_raised);

    assert(sensor_owner.detach(error) == sensor_owner::Status::complete);
    world.destroy(player_body);
    assert(world.backend()->GetBodyCount() == baseline_bodies);
    return 0;
}
