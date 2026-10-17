#include "character_faery_record_physical_v1.hpp"

#include <memory>

namespace dh2::character_faery_record_physical_v1 {
namespace {

bool canonical_record(const factory::Record& record) noexcept {
    const auto identity = record.character.identity;
    return identity && record.constructor.identity == identity &&
        record.game_object.identity == identity && record.source_handle >= 0 &&
        record.object_registered && record.character_listed;
}

bool attached_to(const factory::Record& record,
                 std::uintptr_t source_pofaerie_identity) noexcept {
    return source_pofaerie_identity &&
        record.source_pofaerie_identity == source_pofaerie_identity &&
        record.pofaerie_physical && record.pofaerie_physical->created() &&
        record.pofaerie_physical->character_identity() == record.character.identity &&
        record.pofaerie_physical->game_object_identity() == record.game_object.identity;
}

} // namespace

Status attach(factory::Record& record, std::uintptr_t source_pofaerie_identity,
              physical::NativeWorld& world,
              const physical::CharacterBodyInput& input,
              physical::WorldObject& callbacks, std::string& error) {
    error.clear();
    if (!canonical_record(record)) {
        error = "POFaerie physical attachment requires the registered canonical Character Record";
        return Status::incomplete_record;
    }
    if (!source_pofaerie_identity) {
        error = "POFaerie identity must come from the source physical-owner provider";
        return Status::source_owner_missing;
    }
    if (record.source_pofaerie_identity || record.pofaerie_physical) {
        error = "Character Record already owns a POFaerie physical component";
        return Status::already_attached;
    }
    if (input.character_type != 3 || !input.owner) {
        error = "POFaerie requires the source Character type-3 body input";
        return Status::identity_mismatch;
    }
    std::shared_ptr<physical_component::Owner> owner;
    try { owner = std::make_shared<physical_component::Owner>(); }
    catch (...) {
        error = "POFaerie physical owner allocation failed";
        return Status::owner_rejected;
    }
    const auto status = owner->create(world, record.character.identity,
        record.game_object.identity, input, callbacks);
    if (status != physical_component::Status::complete) {
        error = "Source-configured POFaerie physical component creation failed";
        return Status::owner_rejected;
    }
    if (owner->character_identity() != record.character.identity ||
        owner->game_object_identity() != record.game_object.identity) {
        owner->destroy();
        error = "POFaerie physical owner returned a mismatched Character/GameObject identity";
        return Status::identity_mismatch;
    }
    record.pofaerie_physical = std::move(owner);
    record.source_pofaerie_identity = source_pofaerie_identity;
    return Status::complete;
}

Status set_transform(factory::Record& record,
                     std::uintptr_t source_pofaerie_identity,
                     float game_x, float game_y, float angle,
                     std::string& error) {
    error.clear();
    if (!canonical_record(record)) {
        error = "POFaerie transform lost its canonical Character Record";
        return Status::incomplete_record;
    }
    if (!attached_to(record, source_pofaerie_identity)) {
        error = "POFaerie transform identity does not match this Character Record";
        return Status::identity_mismatch;
    }
    return record.pofaerie_physical->set_transform(game_x, game_y, angle) ==
            physical_component::Status::complete
        ? Status::complete : Status::owner_rejected;
}

Status observe(const factory::Record& record,
               std::uintptr_t source_pofaerie_identity,
               physical_component::Transform* output,
               std::string& error) {
    error.clear();
    if (!canonical_record(record)) {
        error = "POFaerie observation lost its canonical Character Record";
        return Status::incomplete_record;
    }
    if (!attached_to(record, source_pofaerie_identity)) {
        error = "POFaerie observation identity does not match this Character Record";
        return Status::identity_mismatch;
    }
    return record.pofaerie_physical->observe(output) ==
            physical_component::Status::complete
        ? Status::complete : Status::owner_rejected;
}

Status detach(factory::Record& record,
              std::uintptr_t source_pofaerie_identity,
              std::string& error) {
    error.clear();
    if (!record.source_pofaerie_identity && !record.pofaerie_physical)
        return Status::complete;
    if (!attached_to(record, source_pofaerie_identity)) {
        error = "POFaerie detach identity does not match this Character Record";
        return Status::identity_mismatch;
    }
    const auto status = record.pofaerie_physical->destroy();
    if (status != physical_component::Status::complete &&
        status != physical_component::Status::world_retired) {
        error = "POFaerie Box2D body could not be detached from its owning world";
        return Status::owner_rejected;
    }
    record.source_pofaerie_identity = 0;
    record.pofaerie_physical.reset();
    return Status::complete;
}

} // namespace dh2::character_faery_record_physical_v1
