#pragma once

#include "quest_move_in_zone_collision_v1.hpp"
#include "physical_world.hpp"

#include <Box2D.h>
#include <string>

namespace dh2::level_world::quest_move_in_zone_sensor_owner_v1 {

namespace objects = object_manager_runtime_owner_v1;
namespace collision = quest_move_in_zone_collision_v1;

// Services bound to the current ObjectManager/renderer physical owners. The
// first maps an actual peer WorldObject context to its canonical GameObject;
// the second reproduces POZone::onCollisionTest's owner GameObject virtual
// +0xc8 side effect after the source group/mask test succeeds.
struct SensorServices {
    void* context{};
    bool (*resolve_game_object)(void*, void* peer_context,
                                objects::GameObject*& output,
                                std::string& error){};
    // PhysicalObject::onCollisionTest first checks both GameObject +0x80
    // enabled bytes. This callback projects those canonical live source facts.
    bool (*source_owners_enabled)(void*, const collision::BoundZone&,
                                  objects::GameObject& peer, bool& enabled,
                                  std::string& error){};
    bool (*source_collision_test)(void*, const collision::BoundZone&,
                                  objects::GameObject& peer,
                                  std::string& error){};
};

enum class Status : unsigned char {
    complete,
    invalid_argument,
    already_attached,
    world_unavailable,
    stale_zone,
    body_creation_failed,
    source_callback_failed,
    peer_resolution_failed,
    quest_event_failed,
    world_retired,
};

// One owner for the native static polygon body and its callback storage.
// Attach it to the Level which owns the canonical BoundZone and shared
// NativeWorld; destroy the body before destroying this Owner.
class Owner final {
public:
    Owner() noexcept;
    ~Owner();
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) = delete;
    Owner& operator=(Owner&&) = delete;

    Status attach(physical::NativeWorld&, const collision::BoundZone&,
                  const collision::PhysicalSensor&,
                  const SensorServices&, const collision::Services&,
                  std::string& error) noexcept;
    Status detach(std::string& error) noexcept;

    bool attached() const noexcept { return body_ != nullptr; }
    b2Body* body() const noexcept { return body_; }
    bool last_test_allowed() const noexcept { return last_test_allowed_; }
    Status last_test_status() const noexcept { return last_test_status_; }
    Status last_contact_status() const noexcept { return last_contact_status_; }
    const std::string& last_test_error() const noexcept { return last_test_error_; }
    const std::string& last_contact_error() const noexcept { return last_contact_error_; }
    const collision::Result& last_contact_result() const noexcept {
        return last_contact_result_;
    }

private:
    static unsigned test(void*, void*, const physical::Filter*,
                         const physical::Filter*) noexcept;
    static void contact(void*, physical::ContactEvent, void*, const float*,
                        unsigned) noexcept;
    static void velocity(void*, float*) noexcept;
    void record_test_failure(Status, const char*) noexcept;
    void record_contact_failure(Status, const char*) noexcept;

    physical::NativeWorld* world_{};
    b2World* backend_{};
    b2Body* body_{};
    physical::WorldObject callbacks_{};
    collision::BoundZone zone_{};
    SensorServices sensor_services_{};
    collision::Services collision_services_{};
    Status last_test_status_{Status::complete};
    bool last_test_allowed_{};
    Status last_contact_status_{Status::complete};
    collision::Result last_contact_result_{};
    std::string last_test_error_{};
    std::string last_contact_error_{};
};

}  // namespace dh2::level_world::quest_move_in_zone_sensor_owner_v1
