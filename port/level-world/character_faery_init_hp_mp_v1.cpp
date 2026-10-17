#include "character_faery_init_hp_mp_v1.hpp"

#include "character_runtime_factory_v1.hpp"

#include <new>

namespace dh2::character_faery_init_hp_mp_v1 {
namespace {

bool canonical_record(const character_runtime_factory_v1::Record& record,
                      std::uintptr_t& identity) noexcept {
    identity = record.constructor.identity;
    return identity != 0 && record.character.identity == identity &&
           record.game_object.identity == identity &&
           record.object_registered && record.character_listed &&
           record.source_properties_ready();
}

void copy_result(character_level_runtime::Result* out,
                 const character_level_runtime::Result& value) noexcept {
    if (out) *out = value;
}

} // namespace

Status Owner::initialize(character_runtime_factory_v1::Record& record,
                         const character_level_runtime::Storage& storage,
                         character_level_runtime::Result* out,
                         std::string& error) {
    error.clear();
    if (out) *out = {};
    std::uintptr_t identity = 0;
    if (!canonical_record(record, identity) || storage.character != identity ||
        !storage.properties || !storage.base || !storage.view ||
        storage.view->base != storage.base) {
        error = "Faery InitHpMp requires its registered canonical Character and live property view";
        return Status::invalid_record;
    }

    if (bound_) {
        if (character_ != identity || properties_ != storage.properties ||
            base_ != storage.base || view_ != storage.view) {
            error = "Faery InitHpMp storage no longer belongs to the retained Character owner";
            return Status::owner_mismatch;
        }
        if (failure_latched_) {
            copy_result(out, last_result_);
            error = "Faery InitHpMp previously failed after entering source regeneration";
            return Status::prior_failure;
        }
    } else {
        character_ = identity;
        properties_ = storage.properties;
        base_ = storage.base;
        view_ = storage.view;
        bound_ = true;
    }

    last_result_ = {};
    const auto result = runtime_.initialize_hp_mp(&storage, &last_result_);
    copy_result(out, last_result_);
    if (result == character_level_runtime::Status::complete) {
        // Source Revive and CharAI::InitScriptProcess may each invoke
        // _InitHpMp for this same Character. Each such call is a distinct
        // source operation, so successful completion does not suppress a
        // later invocation.
        complete_ = true;
        return Status::complete;
    }
    if (result == character_level_runtime::Status::invalid_argument) {
        error = "Faery InitHpMp storage failed the source runtime's live-sheet validation";
        return Status::invalid_storage;
    }
    if (result == character_level_runtime::Status::busy) {
        error = "Faery InitHpMp runtime is already active";
        return Status::busy;
    }

    // The original caller is ordered HP then MP and is non-atomic. A failure
    // may follow a successful HP mutation, so never replay this startup stage.
    failure_latched_ = true;
    error = "Faery InitHpMp source regeneration provider failed";
    return Status::provider_failed;
}

Status initialize(character_runtime_factory_v1::Record& record,
                  const character_level_runtime::Storage& storage,
                  character_level_runtime::Result* result,
                  std::string& error) {
    if (!record.faery_init_hp_mp) {
        try {
            record.faery_init_hp_mp =
                std::make_unique<Owner>();
        } catch (const std::bad_alloc&) {
            error = "could not retain Faery InitHpMp lifecycle owner on Character Record";
            return Status::allocation_failed;
        }
    }
    return record.faery_init_hp_mp->initialize(record, storage, result, error);
}

} // namespace dh2::character_faery_init_hp_mp_v1
