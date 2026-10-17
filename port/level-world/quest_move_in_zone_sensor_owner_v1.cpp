#include "quest_move_in_zone_sensor_owner_v1.hpp"

#include <cmath>
#include <exception>

namespace dh2::level_world::quest_move_in_zone_sensor_owner_v1 {
namespace {

bool live_zone(const collision::BoundZone& zone) noexcept {
    return zone.object_manager && zone.object && zone.object->identity &&
        zone.object->source_handle >= 0 &&
        zone.object_manager->find_by_identity(zone.object->identity) == zone.object;
}

bool valid_sensor(const collision::PhysicalSensor& sensor) noexcept {
    return sensor.sensor && sensor.group_index == -5 &&
        sensor.category_bits == 0x0800 && sensor.mask_bits == 1310 &&
        std::isfinite(sensor.position[0]) && std::isfinite(sensor.position[1]) &&
        std::isfinite(sensor.half_extents[0]) &&
        std::isfinite(sensor.half_extents[1]) &&
        sensor.half_extents[0] > 0.0f && sensor.half_extents[1] > 0.0f;
}

bool source_filter_allows(const physical::Filter& a,
                          const physical::Filter& b) noexcept {
    if (a.present != 1 || b.present != 1) return false;
    if (a.group == b.group && a.group != 0) return a.group > 0;
    return (a.mask & b.category) != 0 && (a.category & b.mask) != 0;
}

void assign_error(std::string& out, const char* value) noexcept {
    try {
        out = value;
    } catch (...) {
        out.clear();
    }
}

}  // namespace

Owner::Owner() noexcept {
    callbacks_.context = this;
    callbacks_.test = &Owner::test;
    callbacks_.contact = &Owner::contact;
    callbacks_.velocity = &Owner::velocity;
}

Owner::~Owner() {
    std::string ignored;
    (void)detach(ignored);
}

Status Owner::attach(physical::NativeWorld& world,
                     const collision::BoundZone& zone,
                     const collision::PhysicalSensor& sensor,
                     const SensorServices& sensor_services,
                     const collision::Services& collision_services,
                     std::string& error) noexcept {
    error.clear();
    if (body_) {
        assign_error(error, "QuestMoveInZone sensor Owner is already attached");
        return Status::already_attached;
    }
    if (!live_zone(zone)) {
        assign_error(error, "QuestMoveInZone sensor BoundZone is no longer in the active ObjectManager");
        return Status::stale_zone;
    }
    if (!sensor_services.resolve_game_object ||
        !sensor_services.source_owners_enabled ||
        !sensor_services.source_collision_test ||
        !collision_services.virtual_24 || !collision_services.current_level ||
        !collision_services.move_in_zone_constant ||
        !collision_services.raise_current_level_event || !valid_sensor(sensor)) {
        assign_error(error, "QuestMoveInZone sensor requires a live BoundZone, source callbacks, and source filter geometry");
        return Status::invalid_argument;
    }
    auto* backend = world.backend();
    if (!backend) {
        assign_error(error, "QuestMoveInZone sensor requires the active shared NativeWorld");
        return Status::world_unavailable;
    }

    zone_ = zone;
    sensor_services_ = sensor_services;
    collision_services_ = collision_services;
    last_test_status_ = Status::complete;
    last_test_allowed_ = false;
    last_contact_status_ = Status::complete;
    last_test_error_.clear();
    last_contact_error_.clear();
    last_contact_result_ = {};

    b2Body* body = nullptr;
    try {
        b2BodyDef definition;
        definition.position.Set(sensor.position[0], sensor.position[1]);
        definition.allowSleep = true;
        definition.isSleeping = true;
        definition.fixedRotation = true;
        definition.isBullet = false;
        definition.userData = &callbacks_;
        body = world.create(&definition);
        if (!body) {
            assign_error(error, "QuestMoveInZone shared-world body allocation failed");
            return Status::body_creation_failed;
        }

        b2PolygonDef shape;
        shape.SetAsBox(sensor.half_extents[0], sensor.half_extents[1]);
        shape.userData = &callbacks_;
        shape.isSensor = true;
        shape.friction = 0.0f;
        shape.restitution = 0.0f;
        shape.density = 0.0f;
        shape.filter.groupIndex = sensor.group_index;
        shape.filter.categoryBits = sensor.category_bits;
        shape.filter.maskBits = sensor.mask_bits;
        if (!body->CreateShape(&shape)) {
            world.destroy(body);
            assign_error(error, "QuestMoveInZone sensor shape allocation failed");
            return Status::body_creation_failed;
        }
        body->SetMassFromShapes();
    } catch (const std::exception&) {
        if (body && world.backend() == backend) {
            try { world.destroy(body); } catch (...) {}
        }
        assign_error(error, "QuestMoveInZone sensor creation threw");
        return Status::body_creation_failed;
    } catch (...) {
        if (body && world.backend() == backend) {
            try { world.destroy(body); } catch (...) {}
        }
        assign_error(error, "QuestMoveInZone sensor creation threw");
        return Status::body_creation_failed;
    }
    world_ = &world;
    backend_ = backend;
    body_ = body;
    return Status::complete;
}

Status Owner::detach(std::string& error) noexcept {
    error.clear();
    if (!body_) {
        world_ = nullptr;
        backend_ = nullptr;
        zone_ = {};
        sensor_services_ = {};
        collision_services_ = {};
        return Status::complete;
    }
    if (!world_ || world_->backend() != backend_) {
        // NativeWorld::clear already destroyed the whole Box2D world and all
        // bodies. Do not dereference the now-retired body pointer.
        body_ = nullptr;
        world_ = nullptr;
        backend_ = nullptr;
        zone_ = {};
        sensor_services_ = {};
        collision_services_ = {};
        return Status::world_retired;
    }
    try {
        world_->destroy(body_);
    } catch (...) {
        assign_error(error, "QuestMoveInZone sensor body could not be detached");
        return Status::world_unavailable;
    }
    world_ = nullptr;
    backend_ = nullptr;
    zone_ = {};
    sensor_services_ = {};
    collision_services_ = {};
    return Status::complete;
}

unsigned Owner::test(void* raw, void* other_context,
                     const physical::Filter* own_filter,
                     const physical::Filter* other_filter) noexcept {
    auto* self = static_cast<Owner*>(raw);
    if (!self) return 0;
    self->last_test_allowed_ = false;
    if (!own_filter || !other_filter || !live_zone(self->zone_)) return 0;

    objects::GameObject* peer = nullptr;
    std::string error;
    try {
        if (!other_context || !self->sensor_services_.resolve_game_object(
                self->sensor_services_.context, other_context, peer, error) ||
            !peer || !peer->identity ||
            self->zone_.object_manager->find_by_identity(peer->identity) != peer) {
            self->record_test_failure(Status::peer_resolution_failed,
                error.empty() ? "POZone collision peer did not resolve to a live canonical GameObject"
                              : error.c_str());
            return 0;
        }
        bool owners_enabled = false;
        if (!self->sensor_services_.source_owners_enabled(
                self->sensor_services_.context, self->zone_, *peer,
                owners_enabled, error)) {
            self->record_test_failure(Status::source_callback_failed,
                error.empty() ? "PhysicalObject owner-enabled source gate failed"
                              : error.c_str());
            return 0;
        }
        if (!owners_enabled) {
            self->last_test_status_ = Status::complete;
            self->last_test_error_.clear();
            return 0;
        }
        if (!source_filter_allows(*own_filter, *other_filter)) {
            self->last_test_status_ = Status::complete;
            self->last_test_error_.clear();
            return 0;
        }
        if (!self->sensor_services_.source_collision_test(
                self->sensor_services_.context, self->zone_, *peer, error)) {
            self->record_test_failure(Status::source_callback_failed,
                error.empty() ? "POZone source collision-test virtual +0xc8 failed"
                              : error.c_str());
            return 0;
        }
    } catch (...) {
        self->record_test_failure(Status::source_callback_failed,
                                  "POZone source collision-test callback threw");
        return 0;
    }
    self->last_test_status_ = Status::complete;
    self->last_test_allowed_ = true;
    self->last_test_error_.clear();
    return 1;
}

void Owner::contact(void* raw, physical::ContactEvent event,
                    void* other_context, const float*, unsigned) noexcept {
    auto* self = static_cast<Owner*>(raw);
    if (!self || event != physical::ContactEvent::add) return;

    objects::GameObject* peer = nullptr;
    try {
        std::string error;
        if (!other_context || !self->sensor_services_.resolve_game_object ||
            !self->sensor_services_.resolve_game_object(
                self->sensor_services_.context, other_context, peer, error) ||
            !peer || !peer->identity || !live_zone(self->zone_) ||
            self->zone_.object_manager->find_by_identity(peer->identity) != peer) {
            self->record_contact_failure(Status::peer_resolution_failed,
                error.empty() ? "POZone contact peer did not resolve to a live canonical GameObject"
                              : error.c_str());
            return;
        }
        const auto result = collision::on_collision_begins(
            self->zone_, peer, self->collision_services_,
            self->last_contact_result_, error);
        if (result == collision::Status::source_callback_failed ||
            result == collision::Status::constant_unavailable ||
            result == collision::Status::event_raise_failed ||
            result == collision::Status::stale_object ||
            result == collision::Status::invalid_argument) {
            self->record_contact_failure(
                result == collision::Status::event_raise_failed
                    ? Status::quest_event_failed
                    : Status::source_callback_failed,
                error.empty() ? collision::status_name(result) : error.c_str());
            return;
        }
        self->last_contact_status_ = Status::complete;
        self->last_contact_error_.clear();
    } catch (...) {
        self->record_contact_failure(Status::quest_event_failed,
                                     "POZone contact event callback threw");
    }
}

void Owner::velocity(void*, float* output) noexcept {
    if (output) output[0] = output[1] = 0.0f;
}

void Owner::record_test_failure(Status status, const char* error) noexcept {
    last_test_status_ = status;
    assign_error(last_test_error_, error);
}

void Owner::record_contact_failure(Status status, const char* error) noexcept {
    last_contact_status_ = status;
    assign_error(last_contact_error_, error);
}

}  // namespace dh2::level_world::quest_move_in_zone_sensor_owner_v1
