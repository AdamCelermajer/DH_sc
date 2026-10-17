#pragma once

#include <cstdint>

namespace dh2::character_ai_skill_script_constructor {

enum class DispatchTable : std::uint32_t { none = 0, char_ai_skill_script = 1 };
enum class Operation : std::uint32_t {
    arguments_construct = 0,
    arguments_push_string = 1,
    arguments_push_integer = 2,
    report_assertion = 3,
};
enum class Assertion : std::uint32_t { character_nonnull = 0, name_nonnull = 1 };
struct State {
    // Stable object identity used by the real allocator/provider. This is not
    // an original ELF address; the adapter stores a semantic dispatch tag.
    std::uintptr_t identity;
    DispatchTable dispatch_table;
    std::uintptr_t character;
    const char* script_name;
    std::uint32_t skill_index_14;
    std::int32_t last_skill_id_18;
};
struct Request {
    Operation operation;
    std::uintptr_t arguments_identity; // source subobject: object identity + 0x0c
    const char* text;
    std::uint32_t integer;
    Assertion assertion;
    std::uint32_t source_line;
};
struct Services {
    void* context;
    // All three operations are real typed services. A missing or failed child
    // operation stops construction at that point and preserves earlier field
    // stores/callback effects; there is no successful empty Arguments object.
    std::int32_t (*invoke)(void*, State*, const Request*);
    std::int32_t (*report_assertion)(void*, State*, const Request*);
};
struct Globals { std::uint32_t assert_level; };
struct Result {
    DispatchTable dispatch_table;
    std::uint32_t service_calls;
    std::uint32_t assertions;
    std::uint32_t last_operation;
    std::uintptr_t returned_identity;
};
enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    invalid_source_fact = 2,
    service_unavailable = 3,
    service_failed = 4,
    fatal_source_assertion = 5,
};

// Reconstructs the 336-byte CharAISkillScript(Character*, char const*, uint)
// constructor at ELF 0x3cde2c. It writes the object field projection, creates
// the embedded Arguments at +0x0c, sets index +0x14 and last-ID +0x18, then
// forwards source-owned pushString/name and pushInteger/index calls. The real
// Lua argument storage, vtable pointer and allocator stay with the integration
// adapter; this is not a raw host overlay or a fabricated VM object.
Status construct(State*, std::uintptr_t character, const char* script_name,
                 std::uint32_t skill_index, Globals*, const Services*, Result*);

} // namespace dh2::character_ai_skill_script_constructor
