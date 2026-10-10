#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
namespace dh2::data { struct CharacterTable; struct Dictionary; }
extern "C" {
#endif

// MGP bytes supplied for one MLX Module. The array passed to the source
// SpawnPoint compiler is in MLX Module order; source_path must identify that
// module's referenced cache path (the importer canonicalizes iPhone aliases).
typedef struct dh2_world_source_mgp {
    const char* source_path;
    const uint8_t* data;
    size_t size;
} dh2_world_source_mgp;

// The same borrowed cache-file view for the module's MVP visual records.
typedef struct dh2_world_source_mvp {
    const char* source_path;
    const uint8_t* data;
    size_t size;
} dh2_world_source_mvp;

// Internal adapter result. This is an in-memory bridge to the typed
// level-world API, not an asset or runtime serialization format.
typedef struct dh2_world_source_static_decor {
    uint32_t module_index;
    uint32_t source_record;
    char name[64];
    char xrefobject[64];
    char dae_path[128];
    char source_path[64];
    float local_transform[9];
    float world_transform[9];
} dh2_world_source_static_decor;

// Source identity retained beside each supported static DACT row. DACT v1
// itself omits the MGP record index, so this bridge preserves it at compile
// time instead of asking runtime callers to infer it from an actor name.
typedef struct dh2_world_source_static_character {
    uint32_t module_index;
    uint32_t source_record;
    char name[64];
    char character[64];
} dh2_world_source_static_character;

// Import the original Level MLX with the existing source-world parser and
// serialize only its translation-only Module layout into runtime DWLD v1.
int dh2_world_compile_static_layout(const uint8_t* mlx, size_t mlx_size,
    const char* level_name, const char* source_path,
    uint8_t* output, size_t output_capacity, size_t* output_size,
    char* error, size_t error_capacity);

// Import the MLX and its referenced MGPs through the source-world importer,
// then serialize only the three unconditional SWAMP entrypoints (IDs 0, 3,
// and 13) in the existing SPWN v1 format. The known conditional ID 1 and 4
// variants remain deferred. The v1 row carries module index and object name;
// source file path and GameObject record index remain available on the
// imported source records but are not representable in SPWN v1.
int dh2_world_compile_static_spawnpoints(const uint8_t* mlx, size_t mlx_size,
    const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, size_t mgp_count,
    uint8_t* output, size_t output_capacity, size_t* output_size,
    char* error, size_t error_capacity);

// Import the original Level and its module MVPs through SourceLevel, then
// return the reviewed ten unconditional SWAMP static Decor rows in source
// order. Results are caller-owned in-memory records, not a serialized blob.
int dh2_world_compile_static_mvp(const uint8_t* mlx, size_t mlx_size,
    const char* level_name, const char* source_path,
    const dh2_world_source_mvp* mvps, size_t mvp_count,
    dh2_world_source_static_decor* output, size_t output_capacity,
    size_t* output_count, char* error, size_t error_capacity);

#ifdef __cplusplus
// Compile the five reviewed direct SWAMP Monster rows into the current DACT
// v1 layout. Character model and scale data are borrowed immutable owners.
int dh2_world_compile_static_dact(const uint8_t* mlx, size_t mlx_size,
    const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, size_t mgp_count,
    const dh2::data::CharacterTable* characters,
    const dh2::data::Dictionary* models,
    uint8_t* output, size_t output_capacity, size_t* output_size,
    char* error, size_t error_capacity);
int dh2_world_compile_static_dact_with_sources(const uint8_t* mlx, size_t mlx_size,
    const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, size_t mgp_count,
    const dh2::data::CharacterTable* characters,
    const dh2::data::Dictionary* models,
    uint8_t* output, size_t output_capacity, size_t* output_size,
    dh2_world_source_static_character* source_rows, size_t source_capacity,
    size_t* source_count, char* error, size_t error_capacity);
#endif

#ifdef __cplusplus
}
#endif
