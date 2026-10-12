#pragma once
#include "character_target_providers.hpp"
#include <cstdint>
namespace dh2::object_identity {
using Handle16 = target_providers::Handle16;
// Borrowed projection of ObjectBase +81/+82/+85; not an object owner.
struct Flags8 {std::uint8_t disabled,delete_delay,updating,reserved[5];};
// Logical projection of the source deletion list in insertion order. Identities
// may be zero. Caller owns the list and all underlying object lifetimes.
struct DeletionQueue16 {std::uintptr_t* values;std::uint32_t count,capacity;};
enum TargetEvent : std::uint32_t {enemy_spotted=1,target_died,target_out_of_sight,target_in_sight,target_out_of_range,target_in_ranged_range,target_in_close_range,target_in_melee_range};
struct TargetScript16 {std::uintptr_t identity;std::uint32_t available_callbacks,reserved;};
struct TargetCall32 {std::uintptr_t receiver;const char* callback;std::uintptr_t argument;std::uint32_t argument_count,value_type;};
struct TargetServices16 {void* context;int(*invoke)(void*,const TargetCall32*);};
static_assert(sizeof(Flags8)==8&&sizeof(DeletionQueue16)==16);
static_assert(sizeof(TargetScript16)==16&&sizeof(TargetCall32)==32&&sizeof(TargetServices16)==16);
// 0 success; 1 malformed/overlap or full queue, no mutation. Null shared means
// null source object. A nonnull object with a missing handle is not supported.
// This stamps/copies a handle; it does not resolve or validate cached pointers.
extern "C" int dh2_object_handle_from_pointer(Handle16* result,Handle16* shared,std::uint32_t manager_frame);
extern "C" int dh2_object_mark_deleted(Flags8*);
extern "C" int dh2_object_set_updating(Flags8*,std::uint32_t raw_byte);
extern "C" int dh2_object_queue_deletion(DeletionQueue16*,std::uintptr_t identity);
// AISExternal wrapper choreography only. Availability is actual +b8 mask,
// not range/hostility/sight acceptance. Call is synchronous; observer may
// reenter, but must preserve borrowed state/services/actor/VM lifetime.
// Source enemy argument Value7 becomes the genuine object table (null=>nil).
// 0 complete/skipped;1 malformed before call;2 failed provider after prefix.
extern "C" int dh2_object_target_event(const TargetScript16*,std::uint32_t event,std::uintptr_t enemy,const TargetServices16*);
}
