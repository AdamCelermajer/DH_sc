#pragma once
#include <cstddef>
#include <cstdint>

// Owned, renderer-independent records from the original MLX/MGP/MVP files.
// These port types are not the original ARM object layout. They describe data;
// importing an object does not activate it or execute its conditions/scripts.
namespace dh2::world {
enum class Error : std::uint32_t {
    ok, argument, allocation, limit, xml, unsupported_xml, duplicate_field,
    missing_field, number, path, record, already_loaded, unsupported_transform,
    scene, missing_node, ambiguous_node
};
enum class RecordKind : std::uint32_t { level, mgp, mvp };
constexpr std::uint32_t no_module = 0xffffffffU;
struct Diagnostic {
    Error error;
    std::uint32_t byte_offset, line, column;
    char message[160];
};
struct Transform {
    float position[3], rotation_degrees[3], scale[3];
};
struct Field { char* name; char* value; };
struct Object {
    char* source_path;                   // Canonical path relative to cache root.
    std::uint32_t source_record;          // Zero-based GameObject order in file.
    std::uint32_t source_begin, source_end; // Original byte span [begin, end).
    std::uint32_t module_index;
    RecordKind kind;
    Field* fields;
    std::uint32_t field_count;
    const char* name;                    // Aliases an owned field value.
    const char* gametype;
    Transform local;                    // Original exported values, degrees.
    float world_position[3];            // Local + module origin for MGP/MVP.
};
struct Module {
    Object record;
    char* cache_dae;
    char* cache_mgp;
    char* cache_mvp;
    char* catalogue_node_id;             // Original xrefobject + "-node".
    bool mgp_loaded, mvp_loaded;
};
struct Level {
    char* name;                          // Caller-selected LevelList key.
    char* source_path;
    Object config;
    Module* modules;
    std::uint32_t module_count;
    Object* entities;
    std::uint32_t entity_count;
};
}

extern "C" {
// Initialize Level with {}. On success this replaces its previous contents;
// failures leave it unchanged. All input text is copied. No file I/O occurs.
dh2::world::Error dh2_world_import_level(dh2::world::Level*, const char* level_name,
    const char* source_path, const std::uint8_t* bytes, std::size_t size,
    dh2::world::Diagnostic*);
// A module's two files may each be imported once. Failures leave Level intact.
// File path must match the selected module's checked MGP/MVP cache reference.
// This first implementation supports translation-only module placement.
dh2::world::Error dh2_world_import_module_objects(dh2::world::Level*,
    std::uint32_t module_index, dh2::world::RecordKind, const char* source_path,
    const std::uint8_t* bytes, std::size_t size, dh2::world::Diagnostic*);
void dh2_world_free(dh2::world::Level*);
const char* dh2_world_field(const dh2::world::Object*, const char* name);
// Explicit supplied-cache alias: ASCII lower case, '\\' -> '/', and
// data/iphone/... -> data/... . Rejects absolute paths, traversal, empty
// segments and colon. This is a port policy, not a recovered filesystem ABI.
dh2::world::Error dh2_world_cache_path(char* output, std::size_t capacity,
    const char* original, dh2::world::Diagnostic*);
}
