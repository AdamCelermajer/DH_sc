#include "session_source_object_interest_v1.hpp"

namespace dh::foundation {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
}

bool update_session_source_object_interest_v1(
    CombatSession& session, ActorId owner, std::uint32_t actual_dt,
    dh2::character::CharacterInterestFieldsV106& fields,
    const dh2::character::CharacterInterestServicesV106& services,
    const std::function<bool(std::uintptr_t, ActorId&, bool&, std::string&)>& resolve,
    SessionSourceObjectInterestStatusV1& status, std::string& error) {
    error.clear();
    status = SessionSourceObjectInterestStatusV1::published;
    const auto* world = session.world();
    const auto* session_actor = session.actor(owner);
    if (!world || !session_actor || session_actor != world->find_actor(owner))
        return fail(error, "Source OOI owner is not the same live Session actor");
    if (!resolve)
        return fail(error, "Actual source GameObject-to-Session actor resolver is unavailable");

    ActorId resolved_owner = invalid_actor_id;
    bool owner_is_character = false;
    if (!resolve(fields.identity, resolved_owner, owner_is_character, error)) {
        if (error.empty()) error = "Actual source Character owner identity resolution failed";
        return false;
    }
    if (resolved_owner != owner || !owner_is_character)
        return fail(error, "Source OOI fields do not belong to this Session Character");

    if (!dh2::character::source_character_update_interest_v106(fields, actual_dt, services, error))
        return false;

    if (!*fields.object14a4) {
        if (!session.publish_source_object_interest(owner, invalid_actor_id,
                *fields.type14a8, error)) return false;
        error.clear();
        return true;
    }

    ActorId target = invalid_actor_id;
    bool target_is_character = false;
    if (!resolve(*fields.object14a4, target, target_is_character, error)) {
        if (error.empty()) error = "Actual source OOI identity resolution failed";
        return false;
    }
    if (!target_is_character || target == invalid_actor_id ||
        !session.actor(target) || session.actor(target) != world->find_actor(target)) {
        // Source OOI can be a non-Character GameObject. The current Session
        // marker projection can represent only same-world Character ActorIds;
        // keep OOI unknown rather than truncating or aliasing the source ID.
        status = SessionSourceObjectInterestStatusV1::source_object_not_a_session_actor;
        error.clear();
        return true;
    }
    if (!session.publish_source_object_interest(owner, target, *fields.type14a8, error))
        return false;
    error.clear();
    return true;
}

} // namespace dh::foundation
