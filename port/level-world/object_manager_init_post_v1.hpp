#pragma once

#include "object_manager_runtime_owner_v1.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::object_manager_init_post_v1 {

using Address = dh2::object_manager_runtime_owner_v1::Address;
using GameObject = dh2::object_manager_runtime_owner_v1::GameObject;
using ObjectOwner = dh2::object_manager_runtime_owner_v1::Owner;

struct ModuleRef {
    Address identity{};
};

enum class Phase : std::uint8_t {
    unstarted,
    load_modules,
    object_init_post,
    build_post_init_lists,
    done,
    failed,
};

enum class Status : std::uint8_t {
    ok,
    progress,
    complete,
    invalid_argument,
    invalid_state,
    unsupported_provider,
    provider_failed,
};

enum class ProviderBoundary : std::uint8_t {
    none,
    load_module,
    object_init_post,
    resolve_init_post_row,
    test_enable_condition,
    publish_condition_tested,
    clear_post_init_lists,
    is_room_zone,
    append_room_zone,
    room_zone_init_object_list,
    has_additional_init_list,
    append_additional_init_object,
    active_list_condition_gate,
    read_active_list_flags,
    append_active_object,
};

const char* provider_boundary_name(ProviderBoundary boundary) noexcept;
const char* phase_name(Phase phase) noexcept;

// Services are borrowed native-provider boundaries. The port owns sequencing
// and the object cursor; it does not own modules, queues, virtual methods, or
// RoomZone initialization.
struct Services {
    void* context{};
    // One source Module::LoadModule call; the provider owns its ordered
    // _ChooseXmls path lists and all Level::LoadFile loops for that Module.
    bool (*load_module)(void*, Address module_identity){};
    bool (*object_init_post)(void*, GameObject&){};
    bool (*test_enable_condition)(void*, GameObject&, bool include_enabled){};
    bool (*clear_post_init_lists)(void*){};
    bool (*is_room_zone)(void*, const GameObject&, bool* result){};
    bool (*append_room_zone)(void*, GameObject&){};
    bool (*room_zone_init_object_list)(void*, GameObject&){};
    bool (*has_additional_init_list)(void*, const GameObject&, bool* result){};
    bool (*append_additional_init_object)(void*, GameObject&){};
    // Reads the source +0xac/+0xa8 gate and +0xd0/+0xcc fields. The owner
    // applies gate && !flag_d0 && flag_cc before queueing at ObjectManager+0x44.
    bool (*active_list_condition_gate)(void*, const GameObject&, bool* result){};
    bool (*read_active_list_flags)(void*, const GameObject&,
                                   bool* flag_d0, bool* flag_cc){};
    bool (*append_active_object)(void*, GameObject&){};
};

// Resumable owner for ObjectManager::InitPost (ELF 0x34552c). It borrows the
// selected module order and the existing source-handle map; it does not create
// a second Level, Module, or GameObject registry. A caller must keep both
// borrowed inputs alive and stable until Phase::done or Phase::failed.
struct State {
    Phase phase{Phase::unstarted};
    ObjectOwner* objects{};
    const ModuleRef* modules{};
    std::size_t module_count{};
    std::size_t next_module{};
    ObjectOwner::Cursor object_cursor{};
    std::size_t module_load_calls{};
    std::size_t object_init_calls{};
    std::size_t post_init_object_calls{};
    Phase failed_phase{Phase::unstarted};
    ProviderBoundary failed_provider{ProviderBoundary::none};
    Address failed_identity{};
    Address failed_source_handle{};
};

Status begin(State* state, ObjectOwner* objects,
             const ModuleRef* modules, std::size_t module_count) noexcept;

// Performs at most one module or object unit. Phase transitions that have no
// source callback may consume a call without advancing a source iterator.
// Missing providers leave the current cursor/phase unchanged; provider failure
// makes the state terminal because the source callback may have side effects.
Status step(State* state, const Services* services) noexcept;

} // namespace dh2::object_manager_init_post_v1
