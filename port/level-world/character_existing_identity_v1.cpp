#include "character_existing_identity_v1.hpp"

namespace dh2::character_existing_identity_v1 {

Status construct(Owner* owner, Identity identity, const ctor::Services* services,
                 Result* result, std::string& error) {
    error.clear();
    if (!owner || !identity || !services || !result ||
        owner->constructor_save_vectors_ready || owner->source_component_adoption_complete ||
        owner->constructor.constructor_complete) {
        error = "Existing-identity Character constructor owner arguments or state are invalid";
        return Status::invalid_argument;
    }
    *result = {};
    if (owner->identity && owner->identity != identity) {
        error = "Existing-identity Character owner cannot change identity";
        return Status::invalid_argument;
    }
    owner->identity = identity;
    const auto status = ctor::construct(&owner->constructor, identity, services,
                                        &result->constructor, error);
    if (status != ctor::Status::complete) {
        owner->identity = 0;
        return status == ctor::Status::service_unavailable
            ? Status::service_unavailable : Status::constructor_failed;
    }

    // Character C2 writes both three-word saved positions to zero. Keep raw
    // source words; they are later overwritten only by their exact producers.
    owner->quick_save_position_1468 = {};
    owner->revive_position_1474 = {};
    owner->constructor_save_vectors_ready = true;
    owner->source_component_adoption_complete = true;
    result->constructor_save_vectors_ready = true;
    result->source_component_adoption_complete = true;
    return Status::complete;
}

} // namespace dh2::character_existing_identity_v1
