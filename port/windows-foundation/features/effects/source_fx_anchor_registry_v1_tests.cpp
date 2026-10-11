#include "source_fx_anchor_registry_v1.hpp"

#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::effects;
using Op = dh2::fx::MeshFxOperationV1;

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
void query_scalar(SourceFxAnchorRegistryV1& registry, std::uintptr_t identity,
                  Op operation, std::uint32_t expected) {
    dh2::fx::MeshFxRequestV1 request{};
    request.operation = operation;
    request.identity = identity;
    bool handled = false;
    std::string error;
    check(registry.query(request, handled, error), error.c_str());
    check(handled && request.result == expected, "lifecycle query value");
}
void query_vector(SourceFxAnchorRegistryV1& registry, std::uintptr_t identity,
                  Op operation, const std::array<float, 3>& expected) {
    dh2::fx::MeshFxRequestV1 request{};
    request.operation = operation;
    request.identity = identity;
    bool handled = false;
    std::string error;
    check(registry.query(request, handled, error), error.c_str());
    check(handled, "level object query must be claimed");
    for (unsigned i = 0; i < 3; ++i)
        check(std::abs(request.point[i] - expected[i]) < 0.0001f, "source anchor transform value");
}
}

int main() {
    try {
        SourceFxAnchorRegistryV1 registry;
        std::string error;
        check(registry.register_actor("Player", 6, 17, error), error.c_str());
        SourceFxAnchorKeyV1 actor_key;
        bool found = false;
        check(registry.resolve("Player", 6, actor_key, found, error) && found,
              "resolve direct session actor");
        check(actor_key.kind == SourceFxAnchorKindV1::actor &&
              actor_key.manager_identity == 17 && actor_key.generation == registry.generation(),
              "actor keeps its actual session identity");
        dh2::fx::MeshFxRequestV1 actor_request{};
        actor_request.operation = Op::anchor_position;
        actor_request.identity = actor_key.manager_identity;
        bool handled = true;
        check(registry.query(actor_request, handled, error) && !handled,
              "real ActorId stays on the factory's existing live-session provider path");

        SourceFxAnchorPoseV1 pose;
        pose.position = {-503.271f, 1505.86f, 128.244f};
        pose.rotation = {0.f, 0.f, 0.f};
        pose.scale = {1.f, 1.f, 1.f};
        pose.alive = false;
        pose.disabled = true;
        pose.stationary = true;
        int provider_calls = 0;
        SourceFxAnchorKeyV1 waypoint_key;
        check(registry.register_object("_prim_Waypoint_WitchSpotter", 0,
            [&](SourceFxAnchorPoseV1& out, std::string&) {
                ++provider_calls;
                out = pose;
                return true;
            }, waypoint_key, error), error.c_str());
        check(waypoint_key.kind == SourceFxAnchorKindV1::level_object &&
              SourceFxAnchorRegistryV1::is_level_object_identity(waypoint_key.manager_identity) &&
              waypoint_key.manager_identity != actor_key.manager_identity,
              "waypoint is a tagged source-object identity, never a fake ActorId");
        check(registry.current(waypoint_key), "new source object key is current");

        SourceFxAnchorKeyV1 resolved;
        check(registry.resolve("_prim_Waypoint_WitchSpotter", 0, resolved, found, error) && found,
              "resolve exact source object and module");
        check(resolved.manager_identity == waypoint_key.manager_identity,
              "resolved key identifies registered source object");
        query_vector(registry, waypoint_key.manager_identity, Op::anchor_position, pose.position);
        query_vector(registry, waypoint_key.manager_identity, Op::anchor_rotation, pose.rotation);
        query_vector(registry, waypoint_key.manager_identity, Op::anchor_scale, pose.scale);
        query_scalar(registry, waypoint_key.manager_identity, Op::anchor_dead, 1);
        query_scalar(registry, waypoint_key.manager_identity, Op::anchor_disabled, 1);
        query_scalar(registry, waypoint_key.manager_identity, Op::anchor_stationary, 1);
        check(provider_calls == 6, "each source anchor operation samples the provider");
        pose.position = {-400.f, 1400.f, 90.f};
        query_vector(registry, waypoint_key.manager_identity, Op::anchor_position, pose.position);
        check(provider_calls == 7, "moving object provider is resampled on the next frame");

        SourceFxAnchorKeyV1 ignored;
        check(!registry.register_object("_prim_Waypoint_WitchSpotter", 0,
              [](SourceFxAnchorPoseV1&, std::string&) { return true; }, ignored, error),
              "duplicate source name/module is rejected");
        check(!registry.register_actor("Player", 6, 18, error),
              "duplicate actor name/module is rejected");
        check(!registry.register_object("MissingProvider", 0, {}, ignored, error),
              "missing object provider is rejected");
        check(registry.resolve("Absent", 0, resolved, found, error) && !found,
              "missing source name is explicit and harmless");

        SourceFxAnchorKeyV1 failing_key;
        check(registry.register_object("BrokenWaypoint", 0,
              [](SourceFxAnchorPoseV1&, std::string& e) { e = "source object lease expired"; return false; },
              failing_key, error), error.c_str());
        dh2::fx::MeshFxRequestV1 failing_request{};
        failing_request.operation = Op::anchor_position;
        failing_request.identity = failing_key.manager_identity;
        handled = false;
        check(!registry.query(failing_request, handled, error) && handled &&
              error == "source object lease expired",
              "provider lease failure is returned without actor fallback");

        dh2::fx::MeshFxRequestV1 stale_request{};
        stale_request.operation = Op::anchor_position;
        stale_request.identity = waypoint_key.manager_identity;
        registry.clear_for_new_world();
        check(registry.generation() != waypoint_key.generation && !registry.current(waypoint_key) &&
              !registry.current(actor_key),
              "world replacement advances generation and invalidates old key");
        handled = false;
        check(!registry.query(stale_request, handled, error) && handled && !error.empty(),
              "stale tagged object identity fails as handled, without actor fallback");
        check(registry.register_object("_prim_Waypoint_WitchSpotter", 0,
              [&](SourceFxAnchorPoseV1& out, std::string&) { out = pose; return true; },
              ignored, error), error.c_str());
        check(ignored.manager_identity != waypoint_key.manager_identity &&
              ignored.generation != waypoint_key.generation,
              "same name in replacement world receives a fresh identity");
        std::cout << "PASS | typed source FX anchor registry, transform/lifetime, duplicate and generation branches\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FAIL | " << e.what() << '\n';
        return 1;
    }
}
