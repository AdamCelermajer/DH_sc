#pragma once

#include <cstdint>

namespace dh2::object_update_dispatch {

// Logical live projection only. The source object is still owned by
// ObjectManager; these fields are the exact bytes/word read by this bounded
// inline ObjectManager::Update slice, not an ARM-layout overlay.
struct Object {
    std::uintptr_t identity;
    std::uint32_t remote_word_110;
    std::uint8_t remote_byte_118;
    std::uint8_t deletion_81;
    std::uint8_t update_gate_85;
    std::uint8_t culling_phase_86;
    std::uint8_t update_marker_88;
    std::uint8_t update_gate_8a;
    std::uint8_t reserved[2];
};

// Opaque data owned by the ObjectManager adapter for the source
// Character::UnLoadScriptProcess call. The slice preserves the call point and
// passes these values unchanged; it does not decode their container meaning.
struct ManagerContext {
    std::uintptr_t unload_arg1;
    std::uintptr_t unload_arg3;
};

enum class Operation : std::uint32_t {
    is_character,                 // captured object's virtual slot +0x24
    update_ai_pointers,           // direct Character helper, if IsCharacter
    get_online_byte,              // GetOnline() followed by returned byte +5
    is_remotely_updated,          // captured object's virtual slot +0x54
    unload_script_process,        // Character::UnLoadScriptProcess
    dispatch_virtual_update,      // captured object's virtual slot +0x2c
};

struct Request {
    Operation operation;
    std::uintptr_t object;
    std::uintptr_t subject;
    // Exact register arguments supplied only for UnLoadScriptProcess:
    // source r1, r2 (always zero), r3. Other operations receive zeroes.
    std::uintptr_t argument1;
    std::uintptr_t argument2;
    std::uintptr_t argument3;
};

struct Response { std::uint32_t raw; };

struct Services {
    void* context;
    // Mandatory when reached. For queries, raw is the source return value; for
    // direct void calls it is ignored. Nonzero provider status or an exception
    // fails the port call after retaining earlier source effects.
    std::int32_t (*invoke)(void*, Object*, const Request*, Response*);
};

enum class Route : std::uint32_t { none, skipped_offline, update_dispatched };
enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    invalid_source_fact = 2,
    service_unavailable = 3,
    service_failed = 4,
    reentrant_call = 5,
    unsupported_deletion_branch = 6,
};

struct Result {
    Route route;
    std::uint32_t service_calls;
    std::uint32_t culling_phase_writes;
    std::uint32_t update_marker_writes;
};

// Executes only the per-object inline work slice of
// ObjectManager::Update(float), entered after the manager has selected an
// active-list node, loaded its captured pointer and established that it is
// nonnull. Null-node advancement is external. It includes the
// IsCharacter/AIPointers prelude, +0x85/+0x8a gate, online/remote branch,
// culling phase reset and character cleanup, then the +0x2c update dispatch.
// The linked-list traversal, current-Level gate, nonzero deletion byte +0x81
// mark/unlink path and manager post-update handle bookkeeping remain external.
// For reached Character::Update, the provider owns its CanUpdate/culling call.
// A nonzero +0x81 on the dispatch path fails closed; it is not treated as a
// successful update or a reconstructed deletion.
//
// Object, optional ManagerContext, Services and Result storage must remain
// alive, aligned and non-overlapping through synchronous return. Providers may
// mutate the live source fields at their call boundaries. The object pointer
// and full-width identity are captured on entry; each virtual provider must
// resolve that object's current vtable slot, not cache its first callee.
// Services are copied on entry; ManagerContext values are read when unloading.
// Same-object/identity reentry is rejected. Independent nested calls require
// independent outputs; no input or output may overlap an active Result.
Status dispatch(Object*, const ManagerContext*, const Services*, Result*);

}  // namespace dh2::object_update_dispatch
