#include "../character_aggro_cleanup.hpp"

#include <cstdio>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace dh2::character_aggro_cleanup;

namespace {

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Answer {
    std::uint64_t peer;
    bool contains_owner;
};

struct Event {
    char kind;
    std::uint64_t peer;
    std::uint64_t owner;
};

struct PeerLease {
    std::uint64_t identity = 0;
    bool retained = false;
};

struct Fixture {
    PeerRef* mutable_source_peers = nullptr;
    std::uint32_t mutable_peer_count = 0;
    std::vector<Answer> answers;
    std::vector<Event> events;
    std::vector<std::uint64_t> live_peers;
    PeerLease leases[16]{};
    std::uint32_t leases_used = 0;
    bool owner_map_cleared = false;
    bool mutate_source_peers_on_clear = false;
    bool reenter_with_empty_owner = false;
    bool release_error = false;
    std::uint64_t fail_retain_for = 0;
    std::uint64_t throw_erase_for = 0;
    std::uint64_t owner = 0;
};

bool query_peer(void* context, std::uint64_t peer, void* handle,
                std::uint64_t owner) {
    auto& fixture = *static_cast<Fixture*>(context);
    require(owner == fixture.owner, "peer lookup receives source owner");
    auto* lease = static_cast<PeerLease*>(handle);
    require(lease != nullptr && lease->identity == peer && lease->retained,
            "peer lookup uses a retained live actor");
    fixture.events.push_back({'Q', peer, owner});
    for (const auto& answer : fixture.answers) {
        if (answer.peer == peer) return answer.contains_owner;
    }
    return false;
}

void erase_peer_owner(void* context, std::uint64_t peer, void* handle,
                      std::uint64_t owner) {
    auto& fixture = *static_cast<Fixture*>(context);
    auto* lease = static_cast<PeerLease*>(handle);
    require(lease != nullptr && lease->identity == peer && lease->retained,
            "incoming erase uses a retained live actor");
    require(!fixture.owner_map_cleared, "peer incoming erases precede owner map clear");
    fixture.events.push_back({'E', peer, owner});
    if (peer == fixture.throw_erase_for) throw std::runtime_error("injected erase service failure");
    for (auto& answer : fixture.answers) {
        if (answer.peer == peer) answer.contains_owner = false;
    }
}

std::int32_t retain_peer(void* context, std::uint64_t peer, void** handle) {
    auto& fixture = *static_cast<Fixture*>(context);
    require(!fixture.owner_map_cleared, "all peers are retained before owner map clear");
    fixture.events.push_back({'R', peer, fixture.owner});
    bool live = false;
    for (const auto identity : fixture.live_peers) live |= identity == peer;
    if (!live || peer == fixture.fail_retain_for ||
        fixture.leases_used >= std::size(fixture.leases)) {
        return 1;
    }
    auto& lease = fixture.leases[fixture.leases_used++];
    lease = {peer, true};
    *handle = &lease;
    return 0;
}

void clear_owner(void* context, std::uint64_t owner) {
    auto& fixture = *static_cast<Fixture*>(context);
    require(!fixture.owner_map_cleared, "owner outgoing map cleared once");
    require(owner == fixture.owner, "owner map clear receives owner identity");
    fixture.events.push_back({'O', 0, owner});
    fixture.owner_map_cleared = true;
    if (fixture.mutate_source_peers_on_clear) {
        for (std::uint32_t i = 0; i < fixture.mutable_peer_count; ++i) {
            fixture.mutable_source_peers[i].character_identity = 0x900 + i;
        }
        fixture.live_peers.clear();
    }
}

void notify_peer_deaggro(void* context, std::uint64_t peer, void* handle,
                std::uint64_t owner) {
    auto& fixture = *static_cast<Fixture*>(context);
    require(fixture.owner_map_cleared, "owner map clear precedes peer virtual calls");
    require(owner == fixture.owner, "OnDeAggro notification receives source owner");
    auto* lease = static_cast<PeerLease*>(handle);
    require(lease != nullptr && lease->identity == peer && lease->retained,
            "peer callback receives its live retained Character handle");
    fixture.events.push_back({'D', peer, owner});
    if (fixture.reenter_with_empty_owner) {
        fixture.reenter_with_empty_owner = false;
        const Facts nested{owner, 0, nullptr, 0};
        Result nested_result{90, 91, 92, 93};
        require(clear_all(&nested, nullptr, &nested_result) == Status::complete &&
                    nested_result.peers_examined == 0 &&
                    nested_result.peer_incoming_erases == 0 &&
                    nested_result.owner_map_clears == 0 &&
                    nested_result.peer_callbacks == 0,
                "callback reentry sees an empty source map");
    }
}

void release_peer(void* context, std::uint64_t peer, void* handle) noexcept {
    auto& fixture = *static_cast<Fixture*>(context);
    auto* lease = static_cast<PeerLease*>(handle);
    if (lease == nullptr || lease->identity != peer || !lease->retained) {
        fixture.release_error = true;
        return;
    }
    fixture.events.push_back({'L', peer, fixture.owner});
    lease->retained = false;
}

void expect_events(const Fixture& fixture, const std::vector<Event>& expected) {
    require(fixture.events.size() == expected.size(), "event count");
    for (std::size_t i = 0; i < expected.size(); ++i) {
        require(fixture.events[i].kind == expected[i].kind &&
                    fixture.events[i].peer == expected[i].peer &&
                    fixture.events[i].owner == expected[i].owner,
                "source event order and arguments");
    }
}

}  // namespace

int main() {
    try {
        constexpr std::uint64_t owner = 0x100;
        PeerRef peers[] = {{0x40, 0}, {0x10, 0}, {0x30, 0}, {0x20, 0}};
        Fixture fixture;
        fixture.mutable_source_peers = peers;
        fixture.mutable_peer_count = 4;
        fixture.owner = owner;
        fixture.answers = {{0x40, true}, {0x10, false},
                           {0x30, true}, {0x20, true}};
        fixture.live_peers = {0x40, 0x10, 0x30, 0x20};
        fixture.mutate_source_peers_on_clear = true;
        fixture.reenter_with_empty_owner = true;
        Services services{&fixture, query_peer, erase_peer_owner, retain_peer,
                          clear_owner, notify_peer_deaggro, release_peer};
        Facts facts{owner, 4, peers, 4};
        Result result{71, 72, 73, 74};

        auto status = clear_all(&facts, &services, &result);
        require(status == Status::complete && result.peers_examined == 4 &&
                    result.peer_incoming_erases == 3 &&
                    result.owner_map_clears == 1 && result.peer_callbacks == 4 &&
                    !fixture.release_error,
                "source snapshot and complete peer dispatch");
        expect_events(fixture, {
            {'R', 0x40, owner}, {'R', 0x10, owner}, {'R', 0x30, owner}, {'R', 0x20, owner},
            {'Q', 0x40, owner}, {'E', 0x40, owner},
            {'Q', 0x10, owner},
            {'Q', 0x30, owner}, {'E', 0x30, owner},
            {'Q', 0x20, owner}, {'E', 0x20, owner},
            {'O', 0, owner},
            {'D', 0x40, owner}, {'D', 0x10, owner}, {'D', 0x30, owner}, {'D', 0x20, owner},
            {'L', 0x20, owner}, {'L', 0x30, owner}, {'L', 0x10, owner}, {'L', 0x40, owner}});

        // Empty owner map is a source no-op; it does not require runtime services.
        const Facts empty_facts{owner, 0, nullptr, 0};
        Result empty_result{1, 2, 3, 4};
        status = clear_all(&empty_facts, nullptr, &empty_result);
        require(status == Status::complete && empty_result.peers_examined == 0 &&
                    empty_result.peer_incoming_erases == 0 &&
                    empty_result.owner_map_clears == 0 && empty_result.peer_callbacks == 0,
                "empty source map is a no-op");

        // A candidate which lacks the mirrored incoming relation is still
        // snapshotted and receives OnDeAggro after the owner map is cleared.
        PeerRef unmirrored_peer[] = {{0x81, 0}};
        Fixture unmirrored;
        unmirrored.owner = owner;
        unmirrored.answers = {{0x81, false}};
        unmirrored.live_peers = {0x81};
        Services unmirrored_services{&unmirrored, query_peer, erase_peer_owner,
                                     retain_peer, clear_owner, notify_peer_deaggro, release_peer};
        Facts unmirrored_facts{owner, 1, unmirrored_peer, 1};
        Result unmirrored_result{};
        status = clear_all(&unmirrored_facts, &unmirrored_services, &unmirrored_result);
        require(status == Status::complete &&
                    unmirrored_result.peer_incoming_erases == 0 &&
                    unmirrored_result.owner_map_clears == 1 &&
                    unmirrored_result.peer_callbacks == 1,
                "unmirrored outgoing peer is still dispatched");
        expect_events(unmirrored, {{'R', 0x81, owner}, {'Q', 0x81, owner},
                                   {'O', 0, owner}, {'D', 0x81, owner},
                                   {'L', 0x81, owner}});

        // Retain failure occurs before any incoming erase or owner clear;
        // previously retained peers are released before returning.
        PeerRef lifetime_peers[] = {{0xb1, 0}, {0xb2, 0}};
        Fixture lifetime_failure;
        lifetime_failure.owner = owner;
        lifetime_failure.answers = {{0xb1, true}, {0xb2, true}};
        lifetime_failure.live_peers = {0xb1, 0xb2};
        lifetime_failure.fail_retain_for = 0xb2;
        Services lifetime_services{&lifetime_failure, query_peer, erase_peer_owner,
                                   retain_peer, clear_owner, notify_peer_deaggro, release_peer};
        Facts lifetime_facts{owner, 2, lifetime_peers, 2};
        Result lifetime_result{31, 32, 33, 34};
        status = clear_all(&lifetime_facts, &lifetime_services, &lifetime_result);
        require(status == Status::peer_lifetime_failed &&
                    !lifetime_failure.owner_map_cleared &&
                    !lifetime_failure.leases[0].retained &&
                    !lifetime_failure.release_error &&
                    lifetime_result.peers_examined == 31 &&
                    lifetime_result.peer_incoming_erases == 32 &&
                    lifetime_result.owner_map_clears == 33 &&
                    lifetime_result.peer_callbacks == 34,
                "failed peer hold is mutation-free and releases prior holds");
        expect_events(lifetime_failure, {{'R', 0xb1, owner}, {'R', 0xb2, owner},
                                        {'L', 0xb1, owner}});

        // Once the source begins erasing mirrored entries, a throwing adapter
        // cannot promise rollback. RAII releases every hold as the exception
        // propagates; the first completed erase remains visible.
        PeerRef throwing_peers[] = {{0xc1, 0}, {0xc2, 0}};
        Fixture throwing;
        throwing.owner = owner;
        throwing.answers = {{0xc1, true}, {0xc2, true}};
        throwing.live_peers = {0xc1, 0xc2};
        throwing.throw_erase_for = 0xc2;
        Services throwing_services{&throwing, query_peer, erase_peer_owner,
                                  retain_peer, clear_owner, notify_peer_deaggro, release_peer};
        Facts throwing_facts{owner, 2, throwing_peers, 2};
        Result throwing_result{};
        bool propagated = false;
        try {
            (void)clear_all(&throwing_facts, &throwing_services, &throwing_result);
        } catch (const std::runtime_error&) {
            propagated = true;
        }
        require(propagated && !throwing.owner_map_cleared &&
                    !throwing.answers[0].contains_owner &&
                    throwing.answers[1].contains_owner &&
                    !throwing.leases[0].retained && !throwing.leases[1].retained &&
                    !throwing.release_error,
                "callback exception releases holds and does not claim state rollback");
        expect_events(throwing, {
            {'R', 0xc1, owner}, {'R', 0xc2, owner},
            {'Q', 0xc1, owner}, {'E', 0xc1, owner},
            {'Q', 0xc2, owner}, {'E', 0xc2, owner},
            {'L', 0xc2, owner}, {'L', 0xc1, owner}});

        // The original Set/Clear pair oracle accepts self relations. The all-
        // aggro prepass erases the owner's separate incoming entry, clears its
        // outgoing map, then sends the owner its own OnDeAggro notification.
        PeerRef self_peer[] = {{owner, 0}};
        Fixture self;
        self.owner = owner;
        self.answers = {{owner, true}};
        self.live_peers = {owner};
        self.mutable_source_peers = self_peer;
        self.mutable_peer_count = 1;
        self.mutate_source_peers_on_clear = true;
        Services self_services{&self, query_peer, erase_peer_owner, retain_peer,
                               clear_owner, notify_peer_deaggro, release_peer};
        Facts self_facts{owner, 1, self_peer, 1};
        Result self_result{};
        status = clear_all(&self_facts, &self_services, &self_result);
        require(status == Status::complete && self_result.peers_examined == 1 &&
                    self_result.peer_incoming_erases == 1 &&
                    self_result.owner_map_clears == 1 &&
                    self_result.peer_callbacks == 1 && !self.answers[0].contains_owner &&
                    !self.leases[0].retained && !self.release_error,
                "source self relation uses separate maps and a retained OnDeAggro callback");
        expect_events(self, {{'R', owner, owner}, {'Q', owner, owner},
                             {'E', owner, owner}, {'O', 0, owner},
                             {'D', owner, owner}, {'L', owner, owner}});

        // Duplicate identities cannot come from the source map and are rejected
        // before retaining actors or mutating any aggro relation.
        PeerRef duplicate_peers[] = {{0xa1, 0}, {0xa1, 0}};
        Facts duplicate_facts{owner, 2, duplicate_peers, 2};
        Result duplicate_result{21, 22, 23, 24};
        status = clear_all(&duplicate_facts, &services, &duplicate_result);
        require(status == Status::invalid_argument &&
                    duplicate_result.peers_examined == 21 &&
                    duplicate_result.peer_incoming_erases == 22 &&
                    duplicate_result.owner_map_clears == 23 &&
                    duplicate_result.peer_callbacks == 24,
                "duplicate source peer rejects atomically");

        std::printf("{\"clear_all_aggro_cases\":7,\"ordered_peer_callbacks\":4,\"on_deaggro_virtual_notifications\":true,\"self_peer_supported\":true,\"self_peer_callbacks\":1,\"incoming_erase_precedes_owner_clear\":true,\"source_reentry_snapshot\":true,\"retained_peers_not_freed\":true,\"callback_failure_rollback_claimed\":false,\"mismatches\":0}\n");
        return 0;
    } catch (const std::exception& failure) {
        std::fprintf(stderr, "character aggro cleanup: %s\n", failure.what());
        return 1;
    }
}
