#pragma once

#include <cstdint>

namespace dh2::character_faery_selection {

// Field projections consumed by Character::GetCharFaery. FaeryListRow is a
// normalized host/native view (its borrowed pointer is host-width); offsets
// map to source fields +4 and +8 in the 12-byte ARM row. FaeryRow retains the
// source's 9-word/36-byte record because Type is read at +0x20.
struct FaeryListRow {
    std::uint32_t name_id;
    std::int32_t list_size;
    const std::int32_t* members;
};
struct FaeryRow { std::uint32_t words[9]; };
static_assert(sizeof(FaeryRow) == 36, "source Faery row layout");

// Global table slots stay live during the call. GetCharFaeryListId reads the
// list count before GetCharFaery performs either PyDataConstants query. The
// list row itself is retained, while its `members` pointer and FaeryTable base
// are read at the same later points as the original instructions.
struct Tables {
    const FaeryListRow* list_rows;
    std::uint32_t list_count;
    const FaeryRow* faery_rows;
    std::uint32_t faery_count;
};
struct Character { std::uintptr_t identity; std::int32_t faery_list_id_106c; };

enum class Operation : std::uint32_t { faery_types_count = 0, assertion = 1 };
enum class Assertion : std::uint32_t {
    faery_id_in_range = 0,
    list_size_matches_types = 1,
    table_type_matches_index = 2,
};
struct Request {
    Operation operation;
    Assertion assertion;
    std::uint32_t line;
    const char* category;
    const char* key;
};
struct Response { std::int32_t word; };
struct Services {
    void* context;
    // get_constant performs one fresh source PyDataConstants lookup for the
    // exact FaeryTypes/COUNT pair. report_assertion models the source's
    // nonfatal gAssertLevel==1 diagnostic only; gAssertLevel==2 is a source
    // fatal boundary and is reported by the kernel without a null write.
    std::int32_t (*get_constant)(void*, Character*, const Request*, Response*);
    std::int32_t (*report_assertion)(void*, Character*, const Request*);
};
struct Globals {
    const Tables* tables;
    std::uint32_t assert_level;
};
struct Result {
    const FaeryRow* row;
    // Preserve the authored Character property for callers that need the
    // serialized/source value. selected_list_id is the effective table row
    // returned by GetCharFaeryListId (row 0 for negative or out-of-range IDs).
    std::int32_t authored_list_id;
    std::uint32_t selected_list_id;
    std::uint32_t constant_queries;
    std::uint32_t assertions;
    std::uint32_t type_matches;
    std::uint32_t last_operation;
};
enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    invalid_source_fact = 2,
    service_unavailable = 3,
    service_failed = 4,
    fatal_source_assertion = 5,
};

// Complete bounded table-selection body corresponding to
// Character::GetCharFaery(int) const at ELF 0x3aeac0. The original source logs
// consistency assertions and continues at assert level 1; level 2 faults.
// Invalid table storage that would cause an out-of-bounds source dereference is
// rejected as an explicit port guard. Callers retain the Character, Globals,
// Tables, and backing rows for the full synchronous call.
Status select(Character*, std::int32_t faery_id, const Globals*,
              const Services*, Result*);

} // namespace dh2::character_faery_selection
