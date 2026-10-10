#include "../character_faery_record_physical_v1.hpp"

#include <cassert>
#include <cmath>
#include <string>

namespace record_physical = dh2::character_faery_record_physical_v1;
namespace physical_component = dh2::character_physical_component_v1;
namespace physical = dh2::physical;
namespace factory = dh2::character_runtime_factory_v1;

struct CallbackFixture { unsigned calls{}; };
static unsigned on_test(void* raw, void*, const physical::Filter*,
                        const physical::Filter*) {
    ++static_cast<CallbackFixture*>(raw)->calls;
    return 1;
}
static void on_contact(void* raw, physical::ContactEvent, void*, const float*,
                       unsigned) {
    ++static_cast<CallbackFixture*>(raw)->calls;
}
static void on_velocity(void* raw, float* output) {
    ++static_cast<CallbackFixture*>(raw)->calls;
    output[0] = output[1] = 0.0f;
}

int main() {
    factory::Record record;
    const auto identity = reinterpret_cast<std::uintptr_t>(&record.game_object);
    record.game_object.identity = identity;
    record.character.identity = identity;
    record.constructor.identity = identity;
    record.source_handle = 4;
    record.object_registered = true;
    record.character_listed = true;

    physical::NativeWorld world;
    const float bounds[4]{-100, -100, 100, 100};
    world.load(bounds);
    const auto ground_bodies = world.backend()->GetBodyCount();
    int source_character_owner = 7;
    physical::CharacterBodyInput input{};
    input.owner = &source_character_owner;
    input.new_physical = &source_character_owner;
    input.character_type = 3;
    input.absolute_bounds[0] = 0;
    input.absolute_bounds[1] = 0;
    input.absolute_bounds[2] = 72;
    input.absolute_bounds[3] = 60;
    input.position[0] = 125;
    input.position[1] = -50;
    CallbackFixture callback_fixture{};
    physical::WorldObject callbacks{&callback_fixture, on_test, on_contact,
                                    on_velocity, {0, 0}, 0, 0};
    int source_pofaerie_object = 11;
    const auto pofaerie_identity = reinterpret_cast<std::uintptr_t>(
        &source_pofaerie_object);
    std::string error;

    assert(record_physical::attach(record, 0, world, input, callbacks, error) ==
           record_physical::Status::source_owner_missing);
    assert(world.backend()->GetBodyCount() == ground_bodies &&
           !record.source_pofaerie_identity);
    assert(record_physical::attach(record, pofaerie_identity, world, input,
        callbacks, error) == record_physical::Status::complete);
    assert(record.source_pofaerie_identity == pofaerie_identity &&
           record.pofaerie_physical &&
           record.pofaerie_physical->character_identity() == identity &&
           record.pofaerie_physical->game_object_identity() == identity &&
           record.object_registered && record.character_listed &&
           record.source_handle == 4 &&
           world.backend()->GetBodyCount() == ground_bodies + 1);
    assert(record_physical::attach(record, pofaerie_identity, world, input,
        callbacks, error) == record_physical::Status::already_attached);

    physical_component::Transform transform{};
    assert(record_physical::observe(record, pofaerie_identity, &transform,
        error) == record_physical::Status::complete);
    assert(std::fabs(transform.game_x - 125.0f) < 0.001f &&
           std::fabs(transform.game_y + 50.0f) < 0.001f);
    assert(record_physical::set_transform(record, pofaerie_identity + 1,
        0, 0, 0, error) == record_physical::Status::identity_mismatch);
    assert(record_physical::set_transform(record, pofaerie_identity,
        -25, 12.5f, 0.75f, error) == record_physical::Status::complete);
    assert(record_physical::observe(record, pofaerie_identity, &transform,
        error) == record_physical::Status::complete);
    assert(std::fabs(transform.game_x + 25.0f) < 0.001f &&
           std::fabs(transform.game_y - 12.5f) < 0.001f &&
           std::fabs(transform.angle - 0.75f) < 0.001f);

    assert(record_physical::detach(record, pofaerie_identity + 1, error) ==
           record_physical::Status::identity_mismatch);
    assert(record_physical::detach(record, pofaerie_identity, error) ==
           record_physical::Status::complete);
    assert(!record.source_pofaerie_identity &&
           !record.pofaerie_physical &&
           record.object_registered && record.character_listed &&
           record.source_handle == 4 &&
           world.backend()->GetBodyCount() == ground_bodies);

    // A record that was not published through its existing owners cannot gain
    // an untracked physical component.
    record.object_registered = false;
    assert(record_physical::attach(record, pofaerie_identity, world, input,
        callbacks, error) == record_physical::Status::incomplete_record);
    assert(world.backend()->GetBodyCount() == ground_bodies);
    return 0;
}
