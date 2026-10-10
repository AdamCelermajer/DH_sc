#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "../world-data/record_kind.hpp"

namespace dh2::world::source_handle_ledger_v1 {

enum class ModuleFileKind : std::uint8_t { mgp, mvp };

// Files are passed in ObjectManager load order. Each source Module must appear
// once with one MGP and one MVP; the caller controls their within-module order.
struct OrderedModuleFile {
    std::uint32_t module_index{};
    ModuleFileKind kind{};
    const char* source_path{};
    const std::uint8_t* data{};
    std::size_t size{};
};

struct Provenance {
    std::uint32_t module_index{};
    std::string module_name;
    std::uint32_t record_index{};
    RecordKind kind{};
    std::string name;
    std::string gametype;
};

// One attempted source registration. Duplicate names resolve to the existing
// key; ObjectManager::Add destroys the new candidate and keeps the old object.
struct Occurrence {
    std::int32_t handle{};
    bool duplicate_name{};
    Provenance source;
};

// Retained ObjectManager map value for one handle. Duplicate candidates do not
// replace the first object; lookup_count includes both successful and duplicate
// name lookups.
struct Entry {
    std::int32_t handle{};
    std::size_t lookup_count{};
    Provenance retained_source;
};

struct Ledger {
    // The counter value in ObjectManager before the first generated root is
    // registered. The caller must supply this from the active source lifecycle.
    std::uint32_t first_source_handle{};
    // Explicitly supplied count of source handles consumed by root records
    // before Module contents load. build() registers LevelConfig/Module roots
    // in source order and verifies this offset against their unique names.
    std::uint32_t initial_top_level_handle_offset{};
    std::int64_t next_handle_after{};
    std::vector<Occurrence> occurrences;
    // Entries are in first-registration order, equivalent to ascending keys.
    std::vector<Entry> entries;
};

enum class Status : std::uint8_t {
    ok,
    invalid_argument,
    invalid_level,
    invalid_top_level_offset,
    invalid_file_order,
    module_import_failed,
    object_limit,
    handle_overflow,
    allocation_failed,
};

// Imports a generated Level and its ordered module files through world-data,
// then reproduces ObjectManager's successful name-to-map-key registrations.
// Root records (LevelConfig followed by Modules) are registered first from the
// caller-supplied first_source_handle. initial_top_level_handle_offset is the
// expected count of unique root names; it is checked, never silently inferred.
// SourceLevel currently rejects other root object types. The output is cleared
// on failure; source XML/assets are never retained.
Status build(const std::uint8_t* level_xml, std::size_t level_size,
             const char* level_name, const char* level_source_path,
             const OrderedModuleFile* ordered_files, std::size_t file_count,
             std::uint32_t first_source_handle,
             std::uint32_t initial_top_level_handle_offset,
             Ledger& output, std::string& error);

}  // namespace dh2::world::source_handle_ledger_v1
