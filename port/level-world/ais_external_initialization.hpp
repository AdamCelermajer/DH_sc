#pragma once
#include <cstddef>
#include <cstdint>

namespace dh2::ais_external_initialization {
struct TreeHeader {std::uint8_t color;std::uintptr_t parent,left,right;std::uint32_t count;};
// Owned native projections, not an ARM32 byte overlay. Binder/path-storage
// identities identify stable subobjects owned by this AIS's native backend.
struct State {
    std::uintptr_t identity,dispatch_table,binder_identity,path_storage_identity;
    std::uintptr_t owner_98;
    TreeHeader state_registry_9c;
    std::uintptr_t current_state_b4;
    std::uint32_t flags_b8,counter_bc,word_c0;
};
struct Tables {std::uintptr_t char_ai_script,ais_external;};
enum class Operation : std::uint32_t {lua_construct,bind_state_functions,character_create_bindings,path_assign};
struct Request {
    Operation operation;
    std::uintptr_t subject,binder;
    std::uint32_t argument;
    const char* text;
    std::size_t text_bytes;
};
struct Services {
    void* context;
    // Zero success. lua_construct is the real LuaScript C2(bool) resource
    // backend for fixed AIS, honoring the unchanged skip_bind bool, including
    // original base binding when false; bind_state_functions is the two later
    // CharAIScriptBindFunction additions RegisterAIState then ChangeAIState.
    // character_create_bindings invokes the fixed argument Character's virtual
    // +0xc, installing its genuine functions/context in this AIS's Binder+10.
    // path_assign changes this AIS's actual owned string+68, not global paths.
    // No backend's full body is claimed by this caller module.
    std::int32_t (*invoke)(void*,State*,const Request*);
};
struct Result {std::uint32_t service_calls,last_operation;std::uintptr_t returned_identity;};
enum class Status : std::int32_t {complete,invalid_argument,service_unavailable,service_failed};

// Complete CharAIScript108B/AISExternal68B constructor callers, for either ABI
// ctor alias, plus SetCharacter192B valid-nonnull gameplay branch. Construct
// delegates Lua resources first, clears owner/state registry/b4 and optionally
// binds state functions when skip_bind==false. External then resets c0/b8,
// installs its dispatch table and resets bc. Native table identities are used;
// no original ELF vptr is called by production code. SetCharacter preserves
// owner-store→Character bindings→AI path assignment order. Null owner violates
// the original diagnostic/assert precondition and rejects before effects here.
Status construct_char_ai_script(State*,bool skip_bind,const Tables*,const Services*,Result*);
Status construct_external(State*,bool skip_bind,const Tables*,const Services*,Result*);
Status set_character(State*,std::uintptr_t character,const Services*,Result*);

// One owning thread retains fixed AIS/subobject identities, class-table facts,
// captured Character, backend/VM/context and retired backing through return.
// Providers may mutate scalar projections; original later stores overwrite only
// their corresponding fields. Stable identities/tables must not change; control
// storage and borrowed backing must remain live at their disjoint addresses.
// Same State/output reentry
// is forbidden; independent objects may nest. Services are captured once;
// failure/exception stops preserving prior provider and source effects without
// rollback, extra initialization or cleanup. Resource teardown is owner policy.
} // namespace dh2::ais_external_initialization
