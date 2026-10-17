#pragma once

#include <cstdint>

namespace dh2::world_object_character_identity_v1 {

// Stable sidecar for one Character-backed physical owner. The opaque body
// token prevents a stale projection from resolving after that owner's body
// has been replaced. It is not an ObjectManager or Character owner.
struct Projection {
    const void* body_token = nullptr;
    std::uintptr_t game_object_identity = 0;
};

enum class Status : int {
    complete, invalid_argument, already_bound, unbound, body_mismatch,
    non_character_peer, owner_identity_mismatch,
};

struct PersistEvent {
    std::uint32_t character_event = 0;
    std::uintptr_t peer_game_object_identity = 0;
};

inline Status bind(Projection* projection, const void* body_token,
                   std::uintptr_t game_object_identity) noexcept {
    if (!projection || !body_token || !game_object_identity)
        return Status::invalid_argument;
    if (projection->body_token || projection->game_object_identity)
        return Status::already_bound;
    projection->body_token = body_token;
    projection->game_object_identity = game_object_identity;
    return Status::complete;
}

inline Status resolve(const Projection* projection, const void* body_token,
                      std::uintptr_t* game_object_identity) noexcept {
    if (!projection || !body_token || !game_object_identity)
        return Status::invalid_argument;
    if (!projection->body_token || !projection->game_object_identity)
        return Status::unbound;
    if (projection->body_token != body_token) return Status::body_mismatch;
    *game_object_identity = projection->game_object_identity;
    return Status::complete;
}

inline Status retire(Projection* projection, const void* body_token) noexcept {
    if (!projection || !body_token) return Status::invalid_argument;
    if (!projection->body_token || !projection->game_object_identity)
        return Status::unbound;
    if (projection->body_token != body_token) return Status::body_mismatch;
    *projection = {};
    return Status::complete;
}

// POCharacter::onCollisionPersists (0x46ff6c) is the only physical contact
// path currently routed for the local Player: after its Character/peer gates,
// it raises 0x39 for the instigator and 0x3a for the other participant. This
// projection accepts only the local Player owner and a Character-backed peer;
// callers must not use it for Add/Remove/Result or non-Character contacts.
inline Status project_player_persist(const Projection* owner,
                                     const void* owner_body_token,
                                     std::uintptr_t expected_player_identity,
                                     const Projection* peer,
                                     const void* peer_body_token,
                                     bool instigator,
                                     PersistEvent* out) noexcept {
    if (!owner || !owner_body_token || !expected_player_identity || !peer ||
        !peer_body_token || !out) return Status::invalid_argument;
    std::uintptr_t owner_identity = 0;
    auto status = resolve(owner, owner_body_token, &owner_identity);
    if (status != Status::complete) return status;
    if (owner_identity != expected_player_identity)
        return Status::owner_identity_mismatch;
    std::uintptr_t peer_identity = 0;
    status = resolve(peer, peer_body_token, &peer_identity);
    if (status == Status::unbound) return Status::non_character_peer;
    if (status != Status::complete) return status;
    if (peer_identity == owner_identity) return Status::non_character_peer;
    *out = {instigator ? 0x39u : 0x3au, peer_identity};
    return Status::complete;
}

} // namespace dh2::world_object_character_identity_v1
