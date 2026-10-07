#pragma once
#include <cstddef>
#include <cstdint>
namespace dh2::character {
struct CharacterDeferredQueue;
// The caller pins every borrowed Character projection through entries/callbacks.
// identity is immutable; controller is the live Character+378 projection.
struct DeferredQueueOwner16 {std::uintptr_t identity,controller;};
// Native logical iterator corresponding to an original RB node. Generation0
// denotes the source header/end iterator. A deleted node's token stays stale.
struct DeferredQueueToken24 {CharacterDeferredQueue* queue;std::int32_t key;std::uint32_t reserved;std::uint64_t generation;};
struct DeferredQueueRow16 {std::int32_t key;std::uint32_t reserved;std::uintptr_t owner;};
enum DeferredQueueOperation:std::uint32_t {deferred_queue_kill,deferred_queue_unload_ai};
struct DeferredQueueRequest32 {
 std::uint32_t operation,final;
 std::uintptr_t owner,controller;
 std::int32_t key;std::uint32_t reserved;
};
using DeferredQueueInvoke=int(*)(void*,CharacterDeferredQueue*,const DeferredQueueRequest32*);
// Providers must return normally (no C++ exceptions/longjmp across the call).
struct DeferredQueueServices24 {void* context;DeferredQueueInvoke invoke;std::uint32_t available,reserved;};
static_assert(sizeof(DeferredQueueOwner16)==16&&sizeof(DeferredQueueToken24)==24&&sizeof(DeferredQueueRow16)==16);
static_assert(sizeof(DeferredQueueRequest32)==32&&sizeof(DeferredQueueServices24)==24);
}
// One caller-owned shared s_concurrentAI projection, not one queue per actor.
// Native std::map is a port representation; logical identity/order/ownership are
// source-derived. It never owns/deletes Characters, controllers, AI or scripts.
extern "C" dh2::character::CharacterDeferredQueue* dh2_character_deferred_queue_create();
// 2 if borrowed by an active callback; leaves queue intact. Caller must not
// destroy dangling/freed handles. Valid empty/populated owners are destroyed0.
extern "C" int dh2_character_deferred_queue_destroy(dh2::character::CharacterDeferredQueue*);
//Whole ClearConcurrentAI3a799c: erase SAME map nodes without AI callbacks.
extern "C" int dh2_character_deferred_queue_clear_v108(dh2::character::CharacterDeferredQueue*);
// Signed bit-preserving time key, original existing-key mapped owner overwrite.
// 0 complete,1 malformed,3 allocation failure. Output iterator is optional.
extern "C" int dh2_character_deferred_queue_assign(dh2::character::CharacterDeferredQueue*,
 std::int32_t,const dh2::character::DeferredQueueOwner16*,dh2::character::DeferredQueueToken24*);
// Returns source ordered first, or generation0/end; no imported manager clock.
extern "C" int dh2_character_deferred_queue_first(dh2::character::CharacterDeferredQueue*,dh2::character::DeferredQueueToken24*);
extern "C" int dh2_character_deferred_queue_find(dh2::character::CharacterDeferredQueue*,std::int32_t,dh2::character::DeferredQueueToken24*);
// Caller provides disjoint live output storage and a live opaque queue handle.
// Output unchanged if capacity insufficient/malformed. Rows are sorted signed
// time keys; owner field is the actual Character identity, not native address.
extern "C" int dh2_character_deferred_queue_snapshot(const dh2::character::CharacterDeferredQueue*,
 dh2::character::DeferredQueueRow16*,std::uint32_t capacity,std::uint32_t* count);
// Original UnLoadScriptProcess3a7b24. Explicit valid token removes that exact
// node; end token searches the FIRST matching passed owner identity. Erase and
// count decrement precede required AI_UnLoadScript(owner,rawfinal), including
// when no matching node exists. End-search writes token before removal; it then
// becomes stale. Caller retains all borrowed projections through delivery.
extern "C" int dh2_character_deferred_queue_unload(dh2::character::CharacterDeferredQueue*,
 const dh2::character::DeferredQueueOwner16*,dh2::character::DeferredQueueToken24*,std::uint32_t raw_final,
 const dh2::character::DeferredQueueServices24*);
// Source Character.Update eviction once when size>8 (or >24 for levelword29).
// Capture smallest node→Cmd_Kill(NULL,true) using its live controller→reload
// mapped owner from SAME node→UnLoadScriptProcess(token,true). Synchronous
// mutation/reentry is permitted; erased/reinserted captured token fails1.
// 0 complete,1 malformed borrowed topology/token,2 required provider unavailable,
// 3 delivery failure. Already delivered effects are retained, never rolled back.
extern "C" int dh2_character_deferred_queue_evict(dh2::character::CharacterDeferredQueue*,
 std::uint32_t current_level_word,const dh2::character::DeferredQueueServices24*);
