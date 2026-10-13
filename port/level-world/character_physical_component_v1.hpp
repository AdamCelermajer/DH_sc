#pragma once

#include "native_body.hpp"
#include "physical_world.hpp"

#include <cstdint>

namespace dh2::character_physical_component_v1 {

// Typed semantic counterpart for the source POCharacter/POFaerie physical
// component. It deliberately does not allocate an ARM32 object or fabricate
// the original vtable. The canonical Character/GameObject and WorldObject
// callback owner stay externally owned; this owner retains its Box2D body and
// the one typed BodyState projection for that same physical component.
enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    wrong_source_type,
    callbacks_incomplete,
    world_unavailable,
    already_created,
    body_creation_failed,
    not_created,
    world_retired,
    transform_rejected,
};

struct Transform {
    float game_x{};
    float game_y{};
    float angle{};
    float radius{};
};

class Owner final {
public:
    Owner() = default;
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) = delete;
    Owner& operator=(Owner&&) = delete;

    // The source input is used directly to derive the exact type-3 definition.
    // `game_object_identity` and `character_identity` are canonical borrowed
    // identities, never pointers to a locally fabricated source object.
    Status create(physical::NativeWorld&, std::uintptr_t character_identity,
                  std::uintptr_t game_object_identity,
                  const physical::CharacterBodyInput&,
                  physical::WorldObject& callbacks) noexcept;
    Status destroy() noexcept;
    Status set_position(float game_x, float game_y) noexcept;
    Status set_transform(float game_x, float game_y, float angle) noexcept;
    Status observe(Transform*) const noexcept;

    // Source GameObject::Stop providers operate on this same POCharacter /
    // POFaerie owner. The BodyState is the canonical retained projection of
    // PhysicalObject+0x14 for its NativeBody, not a second body or registry.
    // PhysicalObject::setLinearVelocity as GameObject::UpdateSubObjects calls it
    // outside Stop (the position-from-physics drive and the zero-velocity hold).
    // Same native setter and wake rule as the Stop wrapper below.
    Status set_linear_velocity(const float xy[2]) noexcept;
    // PhysicalObject::getPosition in game units (x100): the body position that
    // GameObject::UpdateSubObjects compares against GameObject+0x160.
    Status read_position(float xy[2]) const noexcept;
    // The one retained native body, for the shared GameObject::Update runtime
    // (dh2::actor::update_actor). Null until create(); never a second body.
    physical::NativeBody* native_body() noexcept {
        return body_.body ? &body_ : nullptr;
    }
    Status stop_set_linear_velocity(const float xy[2]) noexcept;
    Status stop_set_angular_velocity(float value) noexcept;
    // Source GameObject::Stop ignores SetXForm's false return. This wrapper
    // treats that ordinary false result as completion while still rejecting
    // malformed native input.
    Status stop_set_position(float game_x, float game_y) noexcept;
    physical::BodyState* stop_put_to_sleep() noexcept;
    // PhysicalObject::pin/unpin on this existing POCharacter/POFaerie body.
    // Repeated requests preserve the underlying source helper's idempotence.
    Status set_pinned(bool pinned) noexcept;

    bool created() const noexcept { return body_.body != nullptr; }
    bool is_pinned() const noexcept { return body_.pinned == 1; }
    bool belongs_to(const physical::NativeWorld& world) const noexcept {
        return body_.body && world_ == &world && backend_ &&
               world.backend() == backend_;
    }
    std::uintptr_t character_identity() const noexcept { return character_identity_; }
    std::uintptr_t game_object_identity() const noexcept { return game_object_identity_; }
    physical::WorldObject* callbacks() const noexcept { return callbacks_; }
    const physical::CharacterBodyConfig& config() const noexcept { return config_; }

private:
    physical::NativeWorld* world_{};
    b2World* backend_{};
    physical::WorldObject* callbacks_{};
    physical::NativeBody body_{};
    physical::BodyState body_state_{};
    physical::CharacterBodyConfig config_{};
    std::uintptr_t character_identity_{};
    std::uintptr_t game_object_identity_{};
};

} // namespace dh2::character_physical_component_v1
