#include "physical_world.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh2::physical;

namespace {
void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

struct ActorContext {
    bool throw_filter{};
    bool allow_collision{true};
    bool throw_contact{};
    unsigned filter_calls{};
    unsigned contact_calls{};
};

unsigned test_filter(void* raw, void*, const Filter*, const Filter*) {
    auto& actor = *static_cast<ActorContext*>(raw);
    ++actor.filter_calls;
    if (actor.throw_filter) throw std::runtime_error("intentional construction filter provider failure");
    return actor.allow_collision;
}

void contact(void* raw, ContactEvent, void*, const float*, unsigned) {
    auto& actor = *static_cast<ActorContext*>(raw);
    ++actor.contact_calls;
    if (actor.throw_contact) throw std::runtime_error("intentional Step contact provider failure");
}

void velocity(void*, float* out) { out[0] = 0; out[1] = 0; }

WorldObject owner(ActorContext& context) {
    return {&context, test_filter, contact, velocity, {0, 0}, 1, 0};
}

CharacterBodyConfig body_at(float x) {
    CharacterBodyConfig config{};
    config.enabled = 1;
    config.body.mass = 1;
    config.body.position[0] = x;
    config.body.allow_sleep = 1;
    config.shape.kind = 0;
    config.shape.density = 1;
    config.shape.friction = 0.3f;
    config.shape.radius = 1;
    config.shape.category_bits = 1;
    config.shape.mask_bits = 0xffff;
    return config;
}

unsigned count_owner_bodies(const NativeWorld& world, const WorldObject* owner) {
    unsigned count = 0;
    for (auto* body = world.backend()->GetBodyList(); body; body = body->GetNext()) {
        if (body->GetUserData() == owner) ++count;
    }
    return count;
}
} // namespace

int main() {
    try {
        const float bounds[]{-20, -20, 20, 20};
        NativeWorld world;
        world.load(bounds);

        ActorContext stable_context, failed_context;
        auto stable_owner = owner(stable_context);
        auto failed_owner = owner(failed_context);
        auto* stable = world.create_character(body_at(0), &stable_owner);
        check(stable && world.backend()->GetBodyCount() == 2, "first overlapping actor did not create");

        failed_context.throw_filter = true;
        bool observed_original_failure = false;
        try {
            (void)world.create_character(body_at(0.25f), &failed_owner);
        } catch (const std::runtime_error& error) {
            observed_original_failure = std::string(error.what()) == "intentional construction filter provider failure";
        }
        check(observed_original_failure, "creation callback failure was swallowed or replaced");
        check(world.cleanup_delivery_idle_v106(), "failure left NativeWorld delivery busy");
        check(world.backend()->GetBodyCount() == 2 && count_owner_bodies(world, &failed_owner) == 0,
              "failed character left an orphan native body");
        check(count_owner_bodies(world, &stable_owner) == 1 && stable->GetShapeList(),
              "failed creation damaged the existing live body");

        // A normal source decision to deny collision must still complete body creation.
        failed_context.throw_filter = false;
        failed_context.allow_collision = false;
        auto* denied = world.create_character(body_at(0.25f), &failed_owner);
        check(denied && world.backend()->GetBodyCount() == 3,
              "legitimate denied collision was treated as provider failure");
        check(!world.ShouldCollide(stable->GetShapeList(), denied->GetShapeList()),
              "provider denial did not remain a normal collision result");
        world.destroy(denied);
        check(world.backend()->GetBodyCount() == 2 && world.cleanup_delivery_idle_v106(),
              "denied-collision body cleanup failed");

        // Repairing the provider permits a fresh retry and preserves the original body.
        failed_context.allow_collision = true;
        auto* retried = world.create_character(body_at(0.25f), &failed_owner);
        check(retried && world.backend()->GetBodyCount() == 3 &&
              world.ShouldCollide(stable->GetShapeList(), retried->GetShapeList()),
              "good-provider retry did not create an overlapping collidable body");
        check(count_owner_bodies(world, &stable_owner) == 1 && count_owner_bodies(world, &failed_owner) == 1,
              "retry changed native owner identities");

        // Step callback failures keep their existing post-unlock, no-replay contract.
        stable_context.throw_contact = true;
        bool step_reported = false;
        try { world.update(16); }
        catch (const std::runtime_error& error) {
            step_reported = std::string(error.what()) == "intentional Step contact provider failure";
        }
        check(step_reported && stable_context.contact_calls == 1,
              "Step contact failure was not reported exactly once");
        check(world.cleanup_delivery_idle_v106(), "Step failure left NativeWorld delivery busy");
        stable_context.throw_contact = false;
        world.update(16);
        if (!world.cleanup_delivery_idle_v106() || stable_context.contact_calls < 2) {
            throw std::runtime_error("Step did not recover after the failed callback was repaired: contacts=" +
                                     std::to_string(stable_context.contact_calls) + ", idle=" +
                                     std::to_string(world.cleanup_delivery_idle_v106()));
        }

        world.destroy(retried);
        world.destroy(stable);
        check(world.backend()->GetBodyCount() == 1 && world.cleanup_delivery_idle_v106(),
              "final body cleanup mismatch");
        std::cout << "{\"creation_callback_failure_cleaned\":true,\"denied_collision_remained_success\":true,"
                     "\"provider_retry_succeeded\":true,\"step_failure_recovery\":true,\"orphan_bodies\":0}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
