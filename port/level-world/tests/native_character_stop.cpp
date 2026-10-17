#include "native_character_stop.hpp"

#include "Box2D.h"
#include "navigation_controller.hpp"
#include "navigation_motion.hpp"
#include "navigation_path.hpp"
#include "navigation_world.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

namespace {
using namespace dh2;

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

bool near(float left, float right, float tolerance = 1e-5f) {
    return std::fabs(left - right) <= tolerance;
}

struct Fixture {
    const std::uint64_t identity = 0x100000021ull;
    character::Coordinator character{identity};
    actor::RuntimeState runtime{};
    std::uint8_t moving = 1;
    std::uint8_t heading_active = 1;
    std::array<navigation::PathSegment, 4> route{};
    std::uintptr_t body_identity = 0;
    std::unique_ptr<b2World> world;
    void* body_owner_token = nullptr;
    b2Body* backend = nullptr;
    physical::NativeBody native{};

    explicit Fixture(std::uint32_t flags, bool make_body = true,
                     float world_extent = 10.f) {
        body_owner_token = this;
        character.state.flags = flags;
        // GameObject coordinates are centimeters; the real Box2D body is at
        // (1,-0.5) meters and Stop converts source XY by 0.01.
        runtime.subobjects.position[0] = 100.f;
        runtime.subobjects.position[1] = -50.f;
        runtime.subobjects.position[2] = 2.f;
        runtime.subobjects.destination[0] = 8.f;
        runtime.subobjects.destination[1] = 6.f;
        runtime.subobjects.destination[2] = -3.f;
        runtime.subobjects.heading[0] = 0.6f;
        runtime.subobjects.heading[1] = 0.8f;
        runtime.controller.position[0] = runtime.subobjects.position[0];
        runtime.controller.position[1] = runtime.subobjects.position[1];
        runtime.controller.position[2] = runtime.subobjects.position[2];
        runtime.controller.destination[0] = runtime.subobjects.destination[0];
        runtime.controller.destination[1] = runtime.subobjects.destination[1];
        runtime.controller.destination[2] = runtime.subobjects.destination[2];
        runtime.controller.path_requested = 1;
        runtime.controller.heading.active = 1;
        runtime.controller.heading.direction[0] = 0.6f;
        runtime.controller.heading.direction[1] = 0.8f;
        runtime.path.segments = route.data();
        runtime.path.capacity = static_cast<std::uint32_t>(route.size());
        runtime.path.position[0] = runtime.subobjects.position[0];
        runtime.path.position[1] = runtime.subobjects.position[1];
        runtime.path.position[2] = runtime.subobjects.position[2];
        runtime.path.target[0] = 8.f;
        runtime.path.target[1] = 6.f;
        runtime.path.target[2] = -3.f;
        runtime.path.count = 1;
        runtime.path.owned = 1;
        route[0].edge = ~std::uint32_t(0);
        route[0].weight = 1.f;
        std::memcpy(route[0].source, runtime.path.position, 12);
        std::memcpy(route[0].target, runtime.path.target, 12);
        runtime.body.flags = 0x58;
        runtime.body.force[0] = 12.f;
        runtime.body.force[1] = -13.f;
        runtime.body.torque = 14.f;
        runtime.body.sleep_time = 15.f;

        b2AABB bounds;
        bounds.lowerBound.Set(-world_extent, -world_extent);
        bounds.upperBound.Set(world_extent, world_extent);
        world = std::make_unique<b2World>(bounds, b2Vec2(0.f, 0.f), true);
        if (!make_body) return;

        b2BodyDef definition;
        definition.userData = &body_owner_token;
        definition.position.Set(1.f, -0.5f);
        definition.angle = 0.25f;
        backend = world->CreateBody(&definition);
        require(backend != nullptr, "Box2D body creation failed");
        b2CircleDef shape;
        shape.userData = &body_owner_token;
        shape.radius = 0.25f;
        shape.density = 1.f;
        backend->CreateShape(&shape);
        backend->SetMassFromShapes();
        backend->SetLinearVelocity(b2Vec2(2.f, -1.f));
        backend->SetAngularVelocity(0.75f);
        backend->ApplyForce(b2Vec2(10.f, 5.f), backend->GetWorldCenter());
        backend->ApplyTorque(3.f);
        native = {backend, shape.radius, 0};
        body_identity = reinterpret_cast<std::uintptr_t>(&native);
        require(dh2_native_body_refresh_view(&runtime.body, &native) == 0,
                "Initial logical Box2D view failed");
    }

    native_character_stop::LiveView view() {
        return {identity, &character, &runtime, &moving, &heading_active,
                body_identity, body_identity ? &native : nullptr};
    }
};

// SetXForm synchronously asks this filter about a newly overlapping proxy.
// No Box2D entities are created or destroyed from inside the callback.
struct StopObserver final : b2ContactFilter {
    Fixture& fixture;
    native_character_stop::LiveView& live;
    bool fail_after_effects;
    bool called = false;
    bool saw_prefix = false;
    bool nested_completed = false;
    physical::NativeBody* replacement = nullptr;

    StopObserver(Fixture& owner, native_character_stop::LiveView& view,
                 bool fail)
        : fixture(owner), live(view), fail_after_effects(fail) {}

    bool ShouldCollide(b2Shape*, b2Shape*) override {
        called = true;
        saw_prefix = fixture.runtime.path.count == 0 &&
            fixture.runtime.path.owned == 0 && fixture.moving == 0 &&
            fixture.heading_active == 0 &&
            fixture.runtime.subobjects.destination[0] == 300.f &&
            fixture.runtime.subobjects.heading[0] == 0.f &&
            fixture.runtime.subobjects.heading[1] == 0.f &&
            fixture.runtime.subobjects.heading[2] == 0.f &&
            fixture.backend->GetLinearVelocity().x == 0.f &&
            fixture.backend->GetLinearVelocity().y == 0.f &&
            fixture.backend->GetAngularVelocity() == 0.f &&
            !fixture.backend->IsSleeping();

        // Reenter through the policy-clear branch while the outer transform
        // is on the stack, then author new GameObject fields after that Stop.
        fixture.runtime.subobjects.position[0] = 500.f;
        fixture.character.state.flags = 0x2380u;
        native_character_stop::Result nested{};
        nested_completed = native_character_stop::stop(&live, &nested) ==
            native_character_stop::Status::complete && nested.phase == 2 &&
            nested.physical_setters == 0 &&
            fixture.runtime.subobjects.destination[0] == 500.f;
        fixture.runtime.subobjects.destination[0] = 901.f;
        fixture.runtime.subobjects.heading[0] = 0.25f;
        fixture.moving = fixture.heading_active = 1;
        // Native SetXForm already took effect; the following refresh fails.
        if (fail_after_effects) fixture.native.pinned = 2;
        if (replacement) {
            live.native_body = replacement;
            live.physical_identity = reinterpret_cast<std::uintptr_t>(replacement);
        }
        return false;
    }
};

unsigned forbidden_controller_calls = 0;
unsigned forbidden_route_calls = 0;

}  // namespace

extern "C" int dh2_nav_avoid_obstacles(
    dh2::navigation::AvoidanceResult*, float*,
    const dh2::navigation::AvoidanceRequest*) {
    ++forbidden_controller_calls;
    return 1;
}

extern "C" int dh2_nav_validate_direction(
    std::uint32_t*, float*, const dh2::navigation::DirectionRequest*) {
    ++forbidden_controller_calls;
    return 1;
}

extern "C" int dh2_nav_route(
    dh2::navigation::RouteResult*, const dh2::navigation::RouteRequest*,
    dh2::navigation::RouteWorkspace*) {
    ++forbidden_route_calls;
    return 1;
}

int main() {
    try {
        unsigned scenarios = 0;

        // Authored Idle and Move words both have position-from-physics clear.
        for (const auto flags : {0x2380u, 0x23c1u}) {
            Fixture fixture(flags);
            const auto route_address = fixture.runtime.path.segments;
            const auto before_position = fixture.backend->GetPosition();
            const auto before_linear = fixture.backend->GetLinearVelocity();
            const float before_angular = fixture.backend->GetAngularVelocity();
            auto live = fixture.view();
            native_character_stop::Result result{};
            require(native_character_stop::stop(&live, &result) ==
                        native_character_stop::Status::complete,
                    "Idle/Move Stop failed");
            require(result.source_status == 0 && result.phase == 2 &&
                        result.service_calls == 2 && result.physical_setters == 0,
                    "Idle/Move Stop physical branch selection");
            require(fixture.runtime.path.count == 0 &&
                        fixture.runtime.path.owned == 0 &&
                        fixture.runtime.path.segments == route_address &&
                        fixture.runtime.path.target[0] ==
                            fixture.runtime.path.position[0],
                    "Idle/Move route release or stable backing");
            require(fixture.runtime.subobjects.destination[0] ==
                        fixture.runtime.subobjects.position[0] &&
                        fixture.runtime.subobjects.destination[1] ==
                        fixture.runtime.subobjects.position[1] &&
                        fixture.moving == 0 && fixture.heading_active == 0,
                    "Idle/Move source-owned GameObject writes");
            require(fixture.runtime.controller.path_requested == 1 &&
                        fixture.runtime.controller.heading.active == 1,
                    "Stop invented controller flags");
            require(fixture.backend->GetPosition().x == before_position.x &&
                        fixture.backend->GetPosition().y == before_position.y &&
                        fixture.backend->GetLinearVelocity().x == before_linear.x &&
                        fixture.backend->GetLinearVelocity().y == before_linear.y &&
                        fixture.backend->GetAngularVelocity() == before_angular,
                    "Policy-clear Stop changed live Box2D body");
            ++scenarios;
        }

        // A live flag-word change is observed at call time, not cached in the
        // typed view. Bit 1 enabled runs every source physical operation.
        {
            Fixture fixture(0x2380u);
            auto live = fixture.view();
            fixture.character.state.flags = 0x2382u;
            native_character_stop::Result result{};
            require(native_character_stop::stop(&live, &result) ==
                        native_character_stop::Status::complete,
                    "Physics-enabled Stop failed");
            require(result.phase == 7 && result.service_calls == 5 &&
                        result.physical_setters == 3 && result.last_operation ==
                            static_cast<unsigned>(game_object_stop::Operation::set_position),
                    "Physics-enabled source callback sequence");
            require(fixture.runtime.path.count == 0 && fixture.backend->IsSleeping(),
                    "Physics-enabled Stop did not release route/sleep body");
            const auto position = fixture.backend->GetPosition();
            require(near(position.x, 1.f) && near(position.y, -0.5f) &&
                        near(fixture.backend->GetAngle(), 0.25f),
                    "Native XY transform or angle preservation");
            require(fixture.backend->GetLinearVelocity().x == 0.f &&
                        fixture.backend->GetLinearVelocity().y == 0.f &&
                        fixture.backend->GetAngularVelocity() == 0.f,
                    "Native Stop velocity clearing");
            require(fixture.runtime.body.flags & physical::sleeping_flag,
                    "Logical body sleeping flag not synchronized");
            require(fixture.runtime.body.linear_velocity[0] == 0.f &&
                        fixture.runtime.body.linear_velocity[1] == 0.f &&
                        fixture.runtime.body.angular_velocity == 0.f &&
                        fixture.runtime.body.force[0] == 0.f &&
                        fixture.runtime.body.force[1] == 0.f &&
                        fixture.runtime.body.torque == 0.f &&
                        fixture.runtime.body.sleep_time == 0.f,
                    "Logical Stop body reset");

            // Wake and step after Stop. A leftover force or torque would now
            // move the genuine body; source Stop clears both at its final write.
            require(dh2_native_body_wake(&fixture.native) == 0,
                    "Box2D wake for clear-force check failed");
            fixture.world->Step(0.1f, 10);
            require(fixture.backend->GetLinearVelocity().x == 0.f &&
                        fixture.backend->GetLinearVelocity().y == 0.f &&
                        fixture.backend->GetAngularVelocity() == 0.f,
                    "Stop left native force/torque accumulated");

            // Mirror the normal frame prelude: Object fields feed the path
            // coordinator. Empty path at its Stop destination cannot resume.
            std::memcpy(fixture.runtime.controller.position,
                        fixture.runtime.subobjects.position, 12);
            std::memcpy(fixture.runtime.controller.destination,
                        fixture.runtime.subobjects.destination, 12);
            fixture.runtime.object = {};
            navigation::ControllerWorkspace workspace{};
            navigation::ControllerPolicy policy{1, 0, 0, 0};
            navigation::ControllerResult frame{};
            navigation::ControllerRequest request{
                &fixture.runtime.controller, &fixture.runtime.path,
                &fixture.runtime.object, nullptr, nullptr, nullptr, &policy,
                &workspace, fixture.identity};
            require(dh2_nav_update_path(&frame, &request) == 0,
                    "Subsequent cleared-route path frame failed");
            require(frame.at_destination && frame.stopped &&
                        fixture.runtime.path.count == 0 &&
                        fixture.runtime.controller.path_requested == 0 &&
                        fixture.runtime.controller.heading.active == 0 &&
                        fixture.runtime.subobjects.destination[0] ==
                            fixture.runtime.subobjects.position[0],
                    "Cleared route resumed in next controller frame");
            require(forbidden_controller_calls == 0 && forbidden_route_calls == 0,
                    "Cleared route reached collision/avoidance service");
            ++scenarios;
        }

        // Physical pointer absent is distinct from stale/nonzero identity.
        {
            Fixture fixture(0x2382u, false);
            auto live = fixture.view();
            native_character_stop::Result result{};
            require(native_character_stop::stop(&live, &result) ==
                        native_character_stop::Status::complete &&
                        result.source_status == 0 && result.phase == 2 &&
                        result.service_calls == 1 && result.physical_setters == 0 &&
                        fixture.runtime.path.count == 0,
                    "Absent physical object source branch");
            ++scenarios;
        }

        for (const bool fail_after_effects : {false, true}) {
            Fixture fixture(0x2382u);
            b2BodyDef obstacle_definition;
            obstacle_definition.position.Set(3.f, -0.5f);
            auto* obstacle = fixture.world->CreateBody(&obstacle_definition);
            b2CircleDef obstacle_shape;
            obstacle_shape.radius = 0.25f;
            obstacle->CreateShape(&obstacle_shape);
            fixture.runtime.subobjects.position[0] = 300.f;
            auto live = fixture.view();
            StopObserver observer(fixture, live, fail_after_effects);
            fixture.world->SetContactFilter(&observer);
            native_character_stop::Result result{};
            const auto status = native_character_stop::stop(&live, &result);
            fixture.world->SetContactFilter(nullptr);
            require(observer.called && observer.saw_prefix,
                    "Native transform observer missed source-prefix writes");
            require(observer.nested_completed,
                    "Native transform callback could not reenter logical Stop");
            require(fixture.runtime.subobjects.position[0] == 500.f &&
                        fixture.runtime.subobjects.destination[0] == 901.f &&
                        fixture.runtime.subobjects.heading[0] == 0.25f &&
                        fixture.moving == 1 && fixture.heading_active == 1,
                    "Outer Stop overwrote synchronous callback GameObject edits");
            require(near(fixture.backend->GetPosition().x, 3.f),
                    "Callback changed already-dispatched transform arguments");
            require(status == (fail_after_effects
                        ? native_character_stop::Status::source_failure
                        : native_character_stop::Status::complete) &&
                        result.phase == (fail_after_effects ? 5u : 7u) &&
                        result.physical_setters == (fail_after_effects ? 2u : 3u) &&
                        fixture.backend->IsSleeping() == !fail_after_effects,
                    "Callback effects changed the completed physical prefix");
            ++scenarios;
        }

        {
            Fixture fixture(0x2382u);
            b2BodyDef obstacle_definition;
            obstacle_definition.position.Set(3.f, -0.5f);
            auto* obstacle = fixture.world->CreateBody(&obstacle_definition);
            b2CircleDef shape;
            shape.radius = 0.25f;
            obstacle->CreateShape(&shape);
            b2BodyDef replacement_definition;
            replacement_definition.position.Set(-3.f, 0.f);
            auto* replacement_body = fixture.world->CreateBody(&replacement_definition);
            shape.density = 1.f;
            replacement_body->CreateShape(&shape);
            replacement_body->SetMassFromShapes();
            replacement_body->SetLinearVelocity(b2Vec2(4.f, -3.f));
            replacement_body->SetAngularVelocity(2.f);
            replacement_body->ApplyForce(b2Vec2(7.f, 9.f),
                                         replacement_body->GetWorldCenter());
            replacement_body->ApplyTorque(5.f);
            physical::NativeBody replacement{replacement_body, 0.25f, 0};
            fixture.runtime.subobjects.position[0] = 300.f;
            auto live = fixture.view();
            StopObserver observer(fixture, live, false);
            observer.replacement = &replacement;
            fixture.world->SetContactFilter(&observer);
            native_character_stop::Result result{};
            const auto status = native_character_stop::stop(&live, &result);
            fixture.world->SetContactFilter(nullptr);
            require(status == native_character_stop::Status::complete &&
                        result.phase == 7 && observer.called &&
                        observer.saw_prefix && observer.nested_completed &&
                        !fixture.backend->IsSleeping() && replacement_body->IsSleeping() &&
                        near(fixture.runtime.body.position[0], -3.f),
                    "Final Stop reset did not resolve replacement owner freshly");
            replacement_body->WakeUp();
            fixture.world->Step(0.1f, 10);
            require(replacement_body->GetLinearVelocity().x == 0.f &&
                        replacement_body->GetLinearVelocity().y == 0.f &&
                        replacement_body->GetAngularVelocity() == 0.f,
                    "Replacement owner retained native force or torque");
            ++scenarios;
        }

        {
            Fixture fixture(0x2301u);
            auto live = fixture.view();
            live.moving = reinterpret_cast<std::uint8_t*>(&fixture.character.state.flags);
            native_character_stop::Result result{91, 92, 93, 94, 95};
            require(native_character_stop::stop(&live, &result) ==
                        native_character_stop::Status::invalid_view &&
                        fixture.runtime.path.count == 1 &&
                        fixture.character.state.flags == 0x2301u &&
                        result.source_status == 91,
                    "Movement byte aliased the live Character policy");
            ++scenarios;
        }
        {
            Fixture fixture(0x2382u);
            auto live = fixture.view();
            auto* aliased_result = reinterpret_cast<native_character_stop::Result*>(
                fixture.runtime.path.segments);
            const auto before_segment = fixture.route[0];
            require(native_character_stop::stop(&live, aliased_result) ==
                        native_character_stop::Status::invalid_view &&
                        fixture.runtime.path.count == 1 &&
                        std::memcmp(&before_segment, &fixture.route[0],
                                    sizeof(before_segment)) == 0,
                    "Stop result aliased live route backing");
            ++scenarios;
        }
        {
            Fixture fixture(0x2382u);
            auto live = fixture.view();
            live.physical_identity += 8;
            native_character_stop::Result result{91, 92, 93, 94, 95};
            const auto before_path = fixture.runtime.path.count;
            require(native_character_stop::stop(&live, &result) ==
                        native_character_stop::Status::body_mismatch &&
                        fixture.runtime.path.count == before_path &&
                        result.source_status == 91 && result.phase == 92,
                    "Mismatched physical identity was not rejected atomically");
            ++scenarios;
        }
        {
            Fixture fixture(0x2382u);
            auto live = fixture.view();
            live.native_body = nullptr;  // source +0x2dc remains nonzero
            native_character_stop::Result result{91, 92, 93, 94, 95};
            const auto before_path = fixture.runtime.path.count;
            require(native_character_stop::stop(&live, &result) ==
                        native_character_stop::Status::body_mismatch &&
                        fixture.runtime.path.count == before_path &&
                        result.source_status == 91 && result.phase == 92,
                    "Nonzero physical identity with absent wrapper was accepted");
            ++scenarios;
        }
        {
            Fixture fixture(0x2382u);
            auto live = fixture.view();
            fixture.native.body = nullptr;  // wrapper exists; backend body does not
            native_character_stop::Result result{91, 92, 93, 94, 95};
            const auto before_path = fixture.runtime.path.count;
            require(native_character_stop::stop(&live, &result) ==
                        native_character_stop::Status::body_mismatch &&
                        fixture.runtime.path.count == before_path &&
                        result.source_status == 91 && result.phase == 92,
                    "Missing native Box2D body was accepted");
            ++scenarios;
        }

        // SetXForm false for an out-of-world target is an ordinary source
        // result and still reaches the original final body reset.
        {
            Fixture fixture(0x2382u, true, 10.f);
            fixture.runtime.subobjects.position[0] = 1500.f;
            fixture.runtime.subobjects.position[1] = 0.f;
            fixture.runtime.path.position[0] = 1500.f;
            fixture.runtime.path.position[1] = 0.f;
            auto live = fixture.view();
            native_character_stop::Result result{};
            require(native_character_stop::stop(&live, &result) ==
                        native_character_stop::Status::complete &&
                        result.phase == 7 && fixture.backend->IsFrozen() &&
                        fixture.backend->IsSleeping(),
                    "False SetXForm result did not follow source ignored-result path");
            ++scenarios;
        }

        // A malformed transform fails at the setter but preserves earlier
        // DropPath and GameObject writes; Stop does not roll back source effects.
        {
            Fixture fixture(0x2382u);
            fixture.runtime.subobjects.position[0] =
                std::numeric_limits<float>::infinity();
            auto live = fixture.view();
            native_character_stop::Result result{};
            require(native_character_stop::stop(&live, &result) ==
                        native_character_stop::Status::source_failure &&
                        result.source_status == static_cast<int>(
                            game_object_stop::Status::service_failed) &&
                        result.phase == 5 && result.service_calls == 5 &&
                        fixture.runtime.path.count == 0 &&
                        fixture.moving == 0 && fixture.heading_active == 0 &&
                        std::isinf(fixture.runtime.subobjects.destination[0]),
                    "Late Stop service error lost source-prefix effects");
            ++scenarios;
        }

        std::cout << "{\"validation\":\"PASS\",\"scenarios\":"
                  << scenarios
                  << ",\"real_box2d\":true,\"source_path_drop\":true,"
                     "\"policy_fresh_read\":true,\"native_put_to_sleep\":true,"
                     "\"synchronous_transform_reentry\":true,"
                     "\"callback_prefix_effects\":true,"
                     "\"fresh_final_owner\":true,\"owner_alias_rejection\":true,"
                     "\"subsequent_empty_path_frame\":true,\"controller_flags_written_by_adapter\":false,"
                     "\"mismatches\":0}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 2;
    }
}
