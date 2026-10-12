#pragma once
#include <cstdint>

namespace dh2::data {
// Caller-owned native storage. Character keys preserve the original ordering,
// but are 64-bit identities, not serialized ARM32 addresses. Entries are sorted
// by key. IEEE float bits are retained, including zero, negatives and NaNs.
struct AggroEntry {
 std::uint64_t character;
 std::uint32_t threat_bits,reserved;
};
struct AggroTable {
 AggroEntry* entries;
 std::uint32_t count,capacity;
};
enum AggroFacts : std::uint32_t {
 aggro_owner_player=1,aggro_owner_dead=2,aggro_target_dead=4,
 aggro_target_targets_owner=8
};
enum AggroRequests : std::uint32_t {
 aggro_notify_target=1,aggro_notify_target_cleared=2,
 aggro_clear_target=4,aggro_stop_controller=8
};
enum AggroOperation : std::uint32_t { aggro_set=0,aggro_add=1,aggro_clear=2 };
struct AggroRequest {
 AggroTable* outgoing;
 AggroTable* target_incoming;
 std::uint64_t owner,target;
 std::uint32_t amount_bits,facts;
};
struct AggroChange {
 std::uint32_t returned_bits,requests,inserted,removed;
};
struct AggroQuery {
 std::uint32_t threat_bits,count,has_aggro,is_aggroed;
 std::uint64_t highest;
};
}

// Set/Add/ClearAggro and the reciprocal incoming relation. Requests expose
// OnAggro/OnDeAggro and target/controller services; those backends are separate.
// Caller facts replace virtual actor queries; full acquisition/AI/FSM is pending.
// Returns 0 on success, 1 on invalid storage/input, 2 on insufficient capacity.
// Failure leaves both tables and the output unchanged.
extern "C" unsigned dh2_aggro_apply(dh2::data::AggroChange*,const dh2::data::AggroRequest*,unsigned operation);
// IsAggroed tests the incoming relation count at original CharAI +0xa4;
// HasAggro tests the outgoing count at +0x8c. The caller supplies its own
// incoming count here (distinct from the target's incoming table in Apply).
extern "C" unsigned dh2_aggro_query(dh2::data::AggroQuery*,const dh2::data::AggroTable*,std::uint64_t target,std::uint32_t incoming_count);
