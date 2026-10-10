#pragma once

#include "../../combat_session.hpp"
#include "../../../level-world/character_object_interest_v106.hpp"

namespace dh::foundation {

enum class SessionSourceObjectInterestStatusV1 : unsigned char {
    published,
    source_object_not_a_session_actor
};

// Runs the recovered Character.UpdateObjectOfInterest owner over the exact
// borrowed source fields/services supplied by the current Character owner.
// The resolver must map actual source GameObject identities to this Session's
// ActorIds; no current/selected combat target is used as an OOI fallback.
bool update_session_source_object_interest_v1(
    CombatSession&, ActorId session_owner, std::uint32_t actual_dt,
    dh2::character::CharacterInterestFieldsV106&,
    const dh2::character::CharacterInterestServicesV106&,
    const std::function<bool(std::uintptr_t source_object, ActorId& session_actor,
                             bool& is_character, std::string&)>&,
    SessionSourceObjectInterestStatusV1&, std::string& error);

} // namespace dh::foundation
