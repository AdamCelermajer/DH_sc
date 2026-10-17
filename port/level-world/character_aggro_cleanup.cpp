#include "character_aggro_cleanup.hpp"

#include <algorithm>
#include <cstddef>
#include <new>
#include <vector>

namespace dh2::character_aggro_cleanup {
namespace {

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a <= b ? b - a < left_size : a - b < right_size;
}

bool valid_inputs(const Facts* facts, const Services* services,
                  const Result* result) {
    if (facts == nullptr || result == nullptr || facts->owner_identity == 0 ||
        facts->outgoing_count > 1'048'576 ||
        facts->peer_count != facts->outgoing_count ||
        (facts->peer_count != 0 && facts->outgoing_peers_in_source_order == nullptr) ||
        overlaps(facts, sizeof(*facts), result, sizeof(*result)) ||
        (services != nullptr &&
         (overlaps(facts, sizeof(*facts), services, sizeof(*services)) ||
          overlaps(result, sizeof(*result), services, sizeof(*services))))) {
        return false;
    }
    if (facts->peer_count == 0) return true;
    const auto peer_bytes = static_cast<std::size_t>(facts->peer_count) * sizeof(PeerRef);
    if (overlaps(facts->outgoing_peers_in_source_order, peer_bytes,
                 facts, sizeof(*facts)) ||
        overlaps(facts->outgoing_peers_in_source_order, peer_bytes,
                 result, sizeof(*result)) ||
        (services != nullptr &&
         overlaps(facts->outgoing_peers_in_source_order, peer_bytes,
                  services, sizeof(*services)))) {
        return false;
    }
    for (std::uint32_t i = 0; i < facts->peer_count; ++i) {
        const auto identity = facts->outgoing_peers_in_source_order[i].character_identity;
        if (identity == 0 ||
            facts->outgoing_peers_in_source_order[i].reserved != 0) {
            return false;
        }
    }
    return true;
}

struct HeldPeer {
    std::uint64_t identity;
    void* handle;
};

struct PeerLeaseOwner {
    Services services;
    std::vector<HeldPeer>& peers;

    ~PeerLeaseOwner() noexcept {
        for (auto it = peers.rbegin(); it != peers.rend(); ++it) {
            services.release_peer(services.context, it->identity, it->handle);
        }
    }
};

}  // namespace

Status clear_all(const Facts* facts, const Services* services, Result* result) {
    if (!valid_inputs(facts, services, result)) return Status::invalid_argument;

    Result completed{};
    completed.peers_examined = facts->peer_count;
    if (facts->peer_count == 0) {
        *result = completed;
        return Status::complete;
    }
    if (services == nullptr || services->peer_incoming_contains_owner == nullptr ||
        services->erase_peer_incoming_owner == nullptr ||
        services->retain_peer == nullptr || services->clear_owner_outgoing == nullptr ||
        services->notify_peer_deaggro == nullptr ||
        services->release_peer == nullptr) {
        return Status::invalid_argument;
    }

    const Services callbacks = *services;
    const std::uint64_t owner_identity = facts->owner_identity;
    const std::uint32_t outgoing_count = facts->outgoing_count;
    std::vector<std::uint64_t> peer_snapshot;
    std::vector<std::uint64_t> identity_check;
    std::vector<HeldPeer> held_peers;
    try {
        peer_snapshot.reserve(facts->peer_count);
        identity_check.reserve(facts->peer_count);
        held_peers.reserve(facts->peer_count);
        for (std::uint32_t i = 0; i < facts->peer_count; ++i) {
            const auto identity = facts->outgoing_peers_in_source_order[i].character_identity;
            peer_snapshot.push_back(identity);
            identity_check.push_back(identity);
        }
    } catch (const std::bad_alloc&) {
        return Status::snapshot_allocation_failed;
    }
    std::sort(identity_check.begin(), identity_check.end());
    if (std::adjacent_find(identity_check.begin(), identity_check.end()) !=
        identity_check.end()) {
        return Status::invalid_argument;
    }

    PeerLeaseOwner leases{callbacks, held_peers};
    for (const auto peer_identity : peer_snapshot) {
        void* handle = nullptr;
        std::int32_t retained = 0;
        try {
            retained = callbacks.retain_peer(callbacks.context, peer_identity, &handle);
        } catch (...) {
            if (handle != nullptr) {
                callbacks.release_peer(callbacks.context, peer_identity, handle);
            }
            throw;
        }
        if (retained != 0) {
            if (handle != nullptr) {
                // A provider that returns a failed status with a non-null
                // handle has already acquired a hold; release it immediately.
                callbacks.release_peer(callbacks.context, peer_identity, handle);
            }
            return Status::peer_lifetime_failed;
        }
        if (handle == nullptr) return Status::peer_lifetime_failed;
        held_peers.push_back({peer_identity, handle});
    }

    // The original loops each outgoing peer in source map order, erases the
    // mirrored entry from that peer's incoming tree if present, and appends
    // every peer to its temporary callback vector. Holds are acquired first
    // as port-only safety machinery; this avoids dispatching a freed actor if
    // later callbacks mutate the native actor registry.
    for (const auto& peer : held_peers) {
        if (callbacks.peer_incoming_contains_owner(
                callbacks.context, peer.identity, peer.handle, owner_identity)) {
            callbacks.erase_peer_incoming_owner(
                callbacks.context, peer.identity, peer.handle, owner_identity);
            ++completed.peer_incoming_erases;
        }
    }

    // Source clears this owner's outgoing tree after every peer-side incoming
    // erase and before dispatching any peer's virtual OnDeAggro(owner).
    callbacks.clear_owner_outgoing(callbacks.context, owner_identity);
    completed.owner_map_clears = outgoing_count != 0 ? 1u : 0u;
    for (const auto& peer : held_peers) {
        callbacks.notify_peer_deaggro(
            callbacks.context, peer.identity, peer.handle, owner_identity);
        ++completed.peer_callbacks;
    }

    *result = completed;
    return Status::complete;
}

}  // namespace dh2::character_aggro_cleanup
