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

// Full SWAMP MGP Character census row. The source handle is joined by the
// level-world wrapper from the same ObjectManager ledger and exact
// (module_index, source_record) tuple; this world-data record never invents a
// handle or activates the object. Transform arrays are position, rotation
// degrees, scale; world transform follows the translation-only Module owner.
typedef struct dh2_world_source_character_row {
    uint32_t module_index;
    uint32_t source_record;
    char name[96];
    char character[96];
    char template_name[64];
    char activate_condition[128];
    char deactivate_condition[128];
    char script[128];
    char spawn_probability[32];
    char auto_spawn[16];
    char ai_state[64];
    uint32_t deferred_reason_mask;
    uint32_t supported_direct_dact_v1;
    float local_transform[9];
    float world_transform[9];
} dh2_world_source_character_row;

enum dh2_world_source_character_deferred_reason {
    DH2_SOURCE_CHARACTER_DEFER_NONE = 0,
    DH2_SOURCE_CHARACTER_DEFER_ACTIVATE_CONDITION = 1u << 0,
    DH2_SOURCE_CHARACTER_DEFER_DEACTIVATE_CONDITION = 1u << 1,
    DH2_SOURCE_CHARACTER_DEFER_SCRIPT = 1u << 2,
    DH2_SOURCE_CHARACTER_DEFER_AUTOSPAWN_DISABLED = 1u << 3,
    DH2_SOURCE_CHARACTER_DEFER_NON_IDLE_AI = 1u << 4,
    DH2_SOURCE_CHARACTER_DEFER_UNSUPPORTED_TEMPLATE = 1u << 5,
    DH2_SOURCE_CHARACTER_DEFER_SPAWN_PROBABILITY = 1u << 6,
    DH2_SOURCE_CHARACTER_DEFER_UNSUPPORTED_DIRECT_ROW = 1u << 7,
    DH2_SOURCE_CHARACTER_DEFER_DUPLICATE_OBJECT_MANAGER_NAME = 1u << 8
};

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

// Census every MGP GameObject whose gametype is Character, retaining source
// identity, authored conditions/scripts and transforms. With output==NULL and
// capacity==0, returns the required row count for a bounded second pass.
// `supported_direct_dact_v1` identifies only the five currently compiled
// unconditional SWAMP Monsters. Every other row carries an explicit reason
// mask; this is enumeration only and never evaluates or dispatches it.
int dh2_world_census_source_characters(const uint8_t* mlx, size_t mlx_size,
    const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, size_t mgp_count,
    dh2_world_source_character_row* output, size_t output_capacity,
    size_t* output_count, char* error, size_t error_capacity);
#endif

#ifdef __cplusplus
}
#endif
