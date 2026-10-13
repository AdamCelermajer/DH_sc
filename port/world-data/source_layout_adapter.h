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

// Exact normalized cache references retained from one imported MLX Module.
// This is an in-memory source bridge, not a runtime asset format. Call with
// output==NULL/capacity==0 to query the bounded module count, then call again
// with that capacity to receive every module in authored order.
typedef struct dh2_world_source_module_ref {
    uint32_t module_index;
    char name[256];
    char mgp_path[512];
    char mvp_path[512];
} dh2_world_source_module_ref;

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
    char char_template_pydata[128];
    char char_template[128];
    char activate_condition[128];
    char deactivate_condition[128];
    char script[128];
    char spawn_probability[32];
    char auto_spawn[16];
    char ai_state[64];
    // Character::DeclareProperties exposes these exact authored MGP names;
    // AddToGroup consumes them after Character property initialization.
    char char_group[128];
    char char_group_role[32];
    // Character::DeclareProperties maps this authored MGP float to
    // Character+0x143c (`spawn_view_radius`).
    char spawn_view_radius[32];
    uint8_t is_global;
    uint32_t deferred_reason_mask;
    uint32_t supported_direct_dact_v1;
    float local_transform[9];
    float world_transform[9];
} dh2_world_source_character_row;

// Generic authored MGP GameObject census row. It retains the exact source
// occurrence and transform/condition fields needed to construct non-Character
// owners from the same imported bytes; it never creates or activates objects.
typedef struct dh2_world_source_object_row {
    uint32_t module_index;
    // Per-Level Module constructor ID which ObjectManager::LoadFromXML
    // passes into ObjectBase+0x64 for this module's MGP rows.
    int32_t source_module_id;
    uint32_t source_record;
    char name[96];
    char gametype[64];
    char template_name[64];
    char activate_condition[128];
    char deactivate_condition[128];
    char spawn_probability[32];
    // ObjectBase::TestEnableCondition consumes these after InitPost. Empty
    // values retain constructor/default-property semantics.
    char min_difficulty[32];
    char is_demo_only[16];
    char dimensions[64];
    // Authored GameObject `data_desc` key. Type-specific source factories use
    // this to resolve their immutable table row; it is not the runtime data ID.
    char data_desc[128];
    // Door::DeclareProperties uses the authored `data` key to select its
    // immutable Doors table row; opened and door_collision are source bytes
    // retained until the canonical Door owner runs InitPost/InitFinal.
    char door_data[128];
    char door_opened[16];
    char door_collision[16];
    // Source PyArray selector paired with data_desc (for example,
    // DestructibleContainers). Some MGP classes derive this through their
    // source virtual instead of storing an explicit field.
    char data_desc_pydata[64];
    // TriggerZoneExitLevel::DeclareProperties authored fields. These stay
    // immutable on the retained MGP source census; only its +2072/+2073
    // runtime flags are published on the canonical ObjectManager row.
    char level_id[32];
    char level_name[128];
    char entrypoint_id[32];
    char fasttravel[128];
    // GameObject::DeclareProperties defaults skip_checkpoint to false; the
    // source MGP override is retained for ObjectBase +0x28 save eligibility.
    char skip_checkpoint[16];
    // ObjectBase::DeclareProperties `visible` value; an empty field preserves
    // the constructor-time PropertyMap default of true.
    char visible[16];
    // GameObject::GetTargetPosition returns the cached source position unless
    // InitPost resolved this authored node name to a visual child.
    char target_node[128];
    // Trigger::DeclareProperties (0x398a88) and TriggerZone::DeclareProperties
    // (0x39bfa4) authored values, retained verbatim; empty keeps the source
    // property default (triggercount 1, triggerdelay 0, scope/triggertype/cineID 0).
    char trigger_count[16];
    char trigger_delay[16];
    char trigger_scope[16];
    char trigger_type[16];
    char trigger_cine_id[16];
    char trigger_script[96];
    char trigger_script_move_out[96];
    char trigger_script_all_player[96];
    char trigger_script_all_player_move_out[96];
    char trigger_effect_one_player[96];
    char trigger_is_door_closed[96];
    float local_transform[9];
    float world_transform[9];
} dh2_world_source_object_row;

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

// Enumerate the exact normalized MGP/MVP paths and Module names from any
// supported authored Level MLX. Does not read files or create runtime objects.
int dh2_world_source_module_refs(const uint8_t* mlx, size_t mlx_size,
    const char* level_name, const char* source_path,
    dh2_world_source_module_ref* output, size_t output_capacity,
    size_t* output_count, char* error, size_t error_capacity);

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

// Census MGP GameObjects with an optional exact gametype filter. A null or
// empty filter includes every MGP object. With output==NULL and capacity==0,
// returns the required row count for a bounded second pass.
int dh2_world_census_source_objects(const uint8_t* mlx, size_t mlx_size,
    const char* level_name, const char* source_path,
    const dh2_world_source_mgp* mgps, size_t mgp_count,
    const char* gametype_filter, dh2_world_source_object_row* output,
    size_t output_capacity, size_t* output_count,
    char* error, size_t error_capacity);
#endif

#ifdef __cplusplus
}
#endif
