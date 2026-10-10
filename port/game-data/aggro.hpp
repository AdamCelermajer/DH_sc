#pragma once
#include <cstdint>
#include <vector>

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
// Optional source CharAI callback seam. A new relation notifies its target
// before either map is inserted; a removed relation notifies after both maps
// are erased. These callbacks are projections of CharAI's virtual +0x38/+0x3c
// slots, not additional relation stores.
struct AggroNotifications {
 void* context;
 void (*target_on_aggro)(void*,std::uint64_t target,std::uint64_t owner) noexcept;
 void (*target_on_deaggro)(void*,std::uint64_t target,std::uint64_t owner) noexcept;
};
struct AggroChange {
 std::uint32_t returned_bits,requests,inserted,removed;
};
struct AggroQuery {
 std::uint32_t threat_bits,count,has_aggro,is_aggroed;
 std::uint64_t highest;
};

// Canonical portable storage for the two CharAI maps attached to one
// Character. Combat mutates these buffers; Character/CharAI adapters borrow
// the same tables for source cleanup. CharAI TreeHeader::count is only a
// synchronized scalar projection of these maps, never another map store.
struct AggroOwner {
 std::vector<AggroEntry> outgoing,incoming;
 std::uint32_t out_count=0,in_count=0;
 void initialize(std::uint32_t capacity){outgoing.resize(capacity);incoming.resize(capacity);out_count=in_count=0;}
 AggroTable outgoing_table() noexcept {return {outgoing.empty()?nullptr:outgoing.data(),out_count,std::uint32_t(outgoing.size())};}
 AggroTable incoming_table() noexcept {return {incoming.empty()?nullptr:incoming.data(),in_count,std::uint32_t(incoming.size())};}
};
}

// Set/Add/ClearAggro and the reciprocal incoming relation. Requests expose
// OnAggro/OnDeAggro and target/controller services; those backends are separate.
// Caller facts replace virtual actor queries; full acquisition/AI/FSM is pending.
// Returns 0 on success, 1 on invalid storage/input, 2 on insufficient capacity.
// Failure leaves both tables and the output unchanged.
extern "C" unsigned dh2_aggro_apply(dh2::data::AggroChange*,const dh2::data::AggroRequest*,unsigned operation);
// Same canonical mutation and result as dh2_aggro_apply, with source-ordered
// edge callbacks. Existing relations and absent clears do not notify.
extern "C" unsigned dh2_aggro_apply_notifying(dh2::data::AggroChange*,const dh2::data::AggroRequest*,unsigned operation,const dh2::data::AggroNotifications*);
// IsAggroed tests the incoming relation count at original CharAI +0xa4;
// HasAggro tests the outgoing count at +0x8c. The caller supplies its own
// incoming count here (distinct from the target's incoming table in Apply).
extern "C" unsigned dh2_aggro_query(dh2::data::AggroQuery*,const dh2::data::AggroTable*,std::uint64_t target,std::uint32_t incoming_count);
