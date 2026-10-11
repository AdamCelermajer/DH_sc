#include "character_faery_source_physical_owner_v1.hpp"

#include "character_faery_record_physical_v1.hpp"
#include "character_faery_visual_facade_v1.hpp"

#include <algorithm>
#include <cmath>
#include <memory>

namespace dh2::character_faery_source_physical_owner_v1 {
namespace component = character_physical_component_v1;
namespace record_physical = character_faery_record_physical_v1;

namespace {

bool canonical_record(const factory::Record& record) noexcept {
    const auto identity = record.character.identity;
    return identity && record.constructor.identity == identity &&
        record.game_object.identity == identity && record.source_handle >= 0 &&
        record.object_registered && record.character_listed;
}

bool valid_bounds(const physical::CharacterOwnerBounds& bounds) noexcept {
    for (float value : bounds.absolute_box)
        if (!std::isfinite(value)) return false;
    return bounds.absolute_box[0] <= bounds.absolute_box[3] &&
        bounds.absolute_box[1] <= bounds.absolute_box[4] &&
        bounds.absolute_box[2] <= bounds.absolute_box[5];
}

// Composite allocation: the Record's existing shared_ptr<component::Owner>
// aliases the component subobject while retaining this whole allocation. The
// WorldObject view therefore stays at a stable address until Record detach.
class POFaerieOwner final {
public:
    physical::WorldObject callbacks{};
    component::Owner body{};

    POFaerieOwner() noexcept {
        callbacks.context = this;
        callbacks.test = &test;
        callbacks.contact = &contact;
        callbacks.velocity = &velocity;
    }

    ~POFaerieOwner() {
        // The body stores `&callbacks` in Box2D user data. Retire it before
        // destroying this context, including any failed later attach step.
        if (body.created()) body.destroy();
    }

    static unsigned test(void*, void*, const physical::Filter*,
                         const physical::Filter*) noexcept {
        // Character::InitPhysicalObject's Faery path passes category 0x100
        // with mask 0. The source type-3 fixture cannot collide with peers.
        return 0;
    }

    static void contact(void*, physical::ContactEvent, void*, const float*,
                        unsigned) noexcept {
        // Unreachable for the source type-3 filter (mask == 0).
    }

    static void velocity(void*, float* output) noexcept {
        if (output) output[0] = output[1] = 0.0f;
        // POCharacter pins its body to zero mass in its constructor.
    }
};

} // namespace

Status attach(factory::Record& record, physical::NativeWorld& world,
              const physical::CharacterOwnerBounds& source_bounds,
              std::string& error) {
    error.clear();
    if (!canonical_record(record)) {
        error = "POFaerie body requires the registered canonical Character Record";
        return Status::incomplete_record;
    }
    if (!valid_bounds(source_bounds)) {
        error = "POFaerie body requires finite ordered source Character bounds";
        return Status::invalid_source_bounds;
    }
    if (record.pofaerie_physical || record.source_pofaerie_identity) {
        error = "Character Record already owns a POFaerie physical component";
        return Status::already_attached;
    }
    if (!world.backend()) {
        error = "POFaerie body requires the active shared actor physics world";
        return Status::world_unavailable;
    }

    std::shared_ptr<POFaerieOwner> owner;
    try {
        owner = std::make_shared<POFaerieOwner>();
    } catch (...) {
        error = "POFaerie physical owner allocation failed";
        return Status::body_creation_failed;
    }

    physical::CharacterBodyInput input{};
    input.owner = &record;
    input.new_physical = owner.get();
    input.character_type = 3;
    input.absolute_bounds[0] = source_bounds.absolute_box[0];
    input.absolute_bounds[1] = source_bounds.absolute_box[1];
    input.absolute_bounds[2] = source_bounds.absolute_box[3];
    input.absolute_bounds[3] = source_bounds.absolute_box[4];
    input.position[0] = record.game_object.world_x;
    input.position[1] = record.game_object.world_y;

    const auto result = owner->body.create(
        world, record.character.identity, record.game_object.identity, input,
        owner->callbacks);
    if (result != component::Status::complete) {
        error = "Source-configured POFaerie type-3 body creation failed";
        return Status::body_creation_failed;
    }

    // The heap owner's address is the stable identity of this semantic
    // POFaerie allocation. It is not an emulated ARM32 address or a counter.
    const auto identity = reinterpret_cast<std::uintptr_t>(owner.get());
    if (!identity || owner->body.callbacks() != &owner->callbacks ||
        owner->body.character_identity() != record.character.identity ||
        owner->body.game_object_identity() != record.game_object.identity) {
        owner->body.destroy();
        error = "POFaerie owner did not retain the canonical Record identities";
        return Status::body_creation_failed;
    }

    record.pofaerie_physical =
        std::shared_ptr<component::Owner>(owner, &owner->body);
    record.source_pofaerie_identity = identity;
    return Status::complete;
}

Status initialize_from_revive(factory::Record& record,
              physical::NativeWorld& world,
              const physical::CharacterOwnerBounds& source_bounds,
              std::string& error) {
    error.clear();
    if (!canonical_record(record)) {
        error = "POFaerie InitPhysicalObject requires the registered canonical Character Record";
        return Status::incomplete_record;
    }

    const auto facade_address = reinterpret_cast<std::uintptr_t>(
        &record.faery_visual);
    auto* live_visual = record.game_object.live_fields.visual_object_2d8;
    if (!character_faery_visual_facade_v1::ready(
            record.faery_visual, record.character.identity) ||
        record.game_object.visual_object_2d8 != facade_address ||
        (live_visual && *live_visual != facade_address)) {
        error = "POFaerie InitPhysicalObject requires the same-Record synced Faery visual facade";
        return Status::visual_unavailable;
    }

    if (record.pofaerie_physical || record.source_pofaerie_identity) {
        if (!record.pofaerie_physical || !record.source_pofaerie_identity) {
            error = "POFaerie Record has a partial physical owner binding";
            return Status::already_attached;
        }
        auto* owner = reinterpret_cast<POFaerieOwner*>(
            record.source_pofaerie_identity);
        if (record.pofaerie_physical.get() != &owner->body ||
            !owner->body.created() ||
            owner->body.callbacks() != &owner->callbacks ||
            owner->body.character_identity() != record.character.identity ||
            owner->body.game_object_identity() != record.game_object.identity) {
            error = "POFaerie Revive encountered a different or retired physical owner";
            return Status::already_attached;
        }
        if (!valid_bounds(source_bounds)) {
            error = "POFaerie Revive requires finite ordered source Character bounds";
            return Status::invalid_source_bounds;
        }
        const float source_radius = std::max(
            source_bounds.absolute_box[3] - source_bounds.absolute_box[0],
            source_bounds.absolute_box[4] - source_bounds.absolute_box[1]) *
            0.005f;
        if (!std::isfinite(source_radius)) {
            error = "POFaerie Revive source body radius is not finite";
            return Status::invalid_source_bounds;
        }
        const bool requires_replacement = !owner->body.belongs_to(world) ||
            std::fabs(owner->body.config().radius - source_radius) > 0.0001f;
        if (requires_replacement) {
            // InitPhysicalObject constructs the selected source body and
            // SetPhysicalObject retires the old physical owner. Reuse the
            // same canonical Record slot and callback owner, destroying the
            // prior Box2D body before creating its replacement so this port
            // never has two Faery bodies live in actor_world.
            const auto destroyed = owner->body.destroy();
            if (destroyed != component::Status::complete &&
                destroyed != component::Status::world_retired) {
                error = "POFaerie Revive could not retire the previous native body";
                return Status::body_creation_failed;
            }
            record.pofaerie_physical.reset();
            record.source_pofaerie_identity = 0;
            return attach(record, world, source_bounds, error);
        }
        const auto moved = owner->body.set_position(record.game_object.world_x,
                                                   record.game_object.world_y);
        if (moved != component::Status::complete) {
            error = "POFaerie Revive could not synchronize the existing native body position";
            return Status::body_creation_failed;
        }
        return Status::already_complete;
    }

    return attach(record, world, source_bounds, error);
}

Status detach(factory::Record& record, std::string& error) {
    error.clear();
    if (!record.source_pofaerie_identity && !record.pofaerie_physical)
        return Status::complete;
    if (!canonical_record(record)) {
        error = "POFaerie detach lost its canonical Character Record";
        return Status::incomplete_record;
    }
    const auto result = record_physical::detach(
        record, record.source_pofaerie_identity, error);
    if (result != record_physical::Status::complete)
        return Status::body_creation_failed;
    return Status::complete;
}

} // namespace dh2::character_faery_source_physical_owner_v1
