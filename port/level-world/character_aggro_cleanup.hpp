#pragma once

#include <cstdint>

namespace dh2::character_aggro_cleanup {

// Borrowed projection of one Character* key in CharAI's outgoing aggro map.
// The caller supplies peers in the exact source map iteration order; this
// kernel deliberately does not sort synthetic host identities.
struct PeerRef {
    std::uint64_t character_identity;
    std::uint32_t reserved;
};

struct Facts {
    std::uint64_t owner_identity;
    std::uint32_t outgoing_count;
    const PeerRef* outgoing_peers_in_source_order;
    std::uint32_t peer_count;
};

struct Services {
    void* context;
    // Reads whether peer.CharAI's incoming tree contains owner Character.
    // It is a total, read-only lookup over a retained live peer.
    bool (*peer_incoming_contains_owner)(
        void*, std::uint64_t peer_identity, void* lifetime_handle,
        std::uint64_t owner_identity);
    // Erases the owner's incoming entry from this peer's incoming tree when
    // the lookup above found it. This is the source's per-peer prepass erase.
    void (*erase_peer_incoming_owner)(
        void*, std::uint64_t peer_identity, void* lifetime_handle,
        std::uint64_t owner_identity);
    // Port lifetime adapter: acquire a stable strong hold for every outgoing
    // Character before owner mutation. A successful call returns a non-null
    // handle that remains valid until release_peer is called.
    std::int32_t (*retain_peer)(void*, std::uint64_t peer_identity,
                                void** lifetime_handle);
    // Implements the source map clear on this owner. Called once after the
    // incoming-erase pass and before OnDeAggro notifications, when nonempty.
    void (*clear_owner_outgoing)(void*, std::uint64_t owner_identity);
    // Implements peer.CharAI::OnDeAggro(owner), the actual vptr slot +0x3c.
    // This is a notification to the peer's selected AI consumer; it must not
    // be replaced by AI_ClearAggro or additional pair/target/controller clears.
    void (*notify_peer_deaggro)(
        void*, std::uint64_t peer_identity, void* lifetime_handle,
        std::uint64_t owner_identity);
    // Releases the runtime hold after the complete peer snapshot dispatch.
    void (*release_peer)(void*, std::uint64_t peer_identity,
                         void* lifetime_handle) noexcept;
};

struct Result {
    std::uint32_t peers_examined;
    std::uint32_t peer_incoming_erases;
    std::uint32_t owner_map_clears;
    std::uint32_t peer_callbacks;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    snapshot_allocation_failed = 2,
    peer_lifetime_failed = 3,
};

// Reconstructs the source-owned ordering of CharAI::AI_ClearAllAggro:
// retain all outgoing peers in map order; for each peer, erase the mirrored
// owner entry if present, then append the peer unconditionally to the source
// snapshot; clear this owner's outgoing map once; then invoke every peer's
// OnDeAggro(owner) in that frozen order. A self peer is valid: the owner's
// outgoing and incoming maps remain distinct source stores. Native map storage,
// the selected AI notification consumer, and actor ownership are supplied by
// the adapter. This method adds no pair target/controller cleanup.
Status clear_all(const Facts* facts, const Services* services, Result* result);

}  // namespace dh2::character_aggro_cleanup
