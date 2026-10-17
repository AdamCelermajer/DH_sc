#pragma once

#include "character_ai_skill_script_constructor.hpp"
#include "character_faery_selection.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::character_ai_set_skills_and_spells {

enum class List : std::uint32_t { none = 0, skill = 1, faery = 2 };
enum class Operation : std::uint32_t {
    debug_load = 0,
    debug_get_switch = 1,
    capture_script_path = 2,
    set_script_path = 3,
    get_skill_list = 4,
    get_skill = 5,
    get_faery_list = 6,
    get_faery_list_id = 7,
    reserve = 8,
    arguments_construct = 9,
    arguments_push_string = 10,
    arguments_push_integer = 11,
    arguments_set_string = 12,
    arguments_set_number = 13,
    arguments_destroy = 14,
    load_script = 15,
    call_script = 16,
    allocate_skill_script = 17,
    append_skill_script = 18,
    init_vcb = 19,
    release_script_path = 20,
    release_skill_script_allocation = 21,
};

// Source vectors are the two CharAI std::vector<CharAISkillScript*> members.
// Their backing memory is owned by the native adapter. Reserve and append are
// explicit services; no successful no-op vector mutation is accepted.
struct ScriptVector {
    std::uintptr_t identity;
    std::uintptr_t* begin;
    std::uintptr_t* end;
    std::uintptr_t* capacity;
};

// The Skill row is read by Character::GetCharSkill. It preserves the source
// gate at +0x24 and the script/name pointer at +0x28, which is reused for the
// DeclareSkill argument, Load, and CharAISkillScript constructor.
struct SkillRow {
    std::uintptr_t script_gate;
    std::uintptr_t script_name;
};

// Faery records are selected by character_faery_selection::select. This view
// binds its exact table and typed PyDataConstants service to the source owner.
struct FaeryBinding {
    character_faery_selection::Character character;
    const character_faery_selection::Globals* globals;
    const character_faery_selection::Services* services;
    // Optional host-width companion for native selectors whose source-shaped
    // FaeryRow retains 32-bit words. `source_rows` must be the exact row array
    // selected by Globals; values are keyed by FaeryTable row index. This lets
    // native callers pass the owned std::string::c_str() pointer without
    // narrowing it through words[6]. Null keeps the original 32-bit oracle
    // fixture path unchanged.
    struct FullWidthScriptNames {
        const character_faery_selection::FaeryRow* source_rows;
        const std::uintptr_t* values;
        std::size_t count;
    };
    const FullWidthScriptNames* full_width_script_names = nullptr;
};

struct State {
    std::uintptr_t ai;
    std::uintptr_t owner;
    std::uint32_t assert_level;
    ScriptVector skills;
    ScriptVector faeries;
    FaeryBinding faery_binding;
};

struct Request {
    Operation operation;
    List list;
    std::uint32_t slot;
    std::uintptr_t receiver;
    std::uintptr_t script_name;
    std::uintptr_t display_name;
    std::uintptr_t arguments;
    std::uint32_t integer;
    std::uint32_t number_bits;
    std::uint32_t allocation_bytes;
    std::uint32_t allocation_hint;
    const char* text;
    std::size_t text_size;
    const char* saved_path;
    std::size_t saved_path_size;
};
struct Response {
    std::uintptr_t identity;
    std::uint32_t word;
    std::uint32_t count;
    std::uint32_t loaded;
    const char* path;
    std::size_t path_size;
    SkillRow skill;
};

struct Services {
    void* context;
    // Synchronous typed source providers. `invoke` must implement the exact
    // requested external operation or fail; Load's false response is a normal
    // source result. All borrowed actors, strings, rows and vector storage stay
    // alive for this whole call. Callback mutation effects are never rolled
    // back. The constructor child is composed through its maintained kernel.
    std::int32_t (*invoke)(void*, State*, const Request*, Response*);
    const character_ai_skill_script_constructor::Services* constructor_services;
    const character_ai_skill_script_constructor::Globals* constructor_globals;
};

struct Result {
    std::uint32_t service_calls;
    std::uint32_t skill_slots;
    std::uint32_t faery_slots;
    std::uint32_t script_allocations;
    std::uint32_t null_appends;
    std::uint32_t declarations;
    std::uint32_t faery_selections;
    std::uint32_t last_operation;
    std::uint32_t init_vcb_called;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    invalid_source_fact = 2,
    service_unavailable = 3,
    service_failed = 4,
    child_kernel_failed = 5,
    fatal_source_assertion = 6,
};

// Complete bounded control-flow reconstruction of CharAI::SetSkillsAndSpells
// (1916 bytes, ELF 0x3ce044). It populates each currently-empty destination
// vector, restores the captured AIS script path, runs the second real debug
// query, then dispatches the source active-AIS +0xcc virtual (InitVCB). The
// selector and CharAISkillScript constructor use their maintained child
// kernels. VM loading/calls, debug persistence, source row getters, vector
// allocation, AIS vcall and String/Arguments storage remain typed providers.
// Skill declarations/constructors receive the current source loop slot.
// Faery declarations pass -1.0f and their child constructor receives
// UINT32_MAX, exactly as emitted by the original caller.
// This is not a successful stub for any missing service and is not Android
// wired by itself.
Status prepare(State*, const Services*, Result*);

} // namespace dh2::character_ai_set_skills_and_spells
