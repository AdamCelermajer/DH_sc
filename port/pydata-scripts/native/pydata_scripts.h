#ifndef DH2_PYDATA_SCRIPTS_H
#define DH2_PYDATA_SCRIPTS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Data-only decoder for the recovered common and SWAMP PyData script tables.
 * String views and script names point into storage owned by dh2_script_table.
 * ExecScript integer arrays are separately owned by the table as well.
 * Initialize a result with {0}, decode once, and always destroy it, including
 * after a failed decode. The decoder never dispatches or runs commands.
 */

enum {
    DH2_SCRIPT_MAX_SCRIPTS = 4096,
    DH2_SCRIPT_MAX_COMMANDS_PER_SCRIPT = 4096,
    DH2_SCRIPT_MAX_TOTAL_COMMANDS = 100000,
    DH2_SCRIPT_MAX_STRING_BYTES = 1u << 20,
    DH2_SCRIPT_MAX_TOTAL_STRING_BYTES = 16u << 20,
    DH2_SCRIPT_MAX_ARRAY_ITEMS = 1u << 16,
    DH2_SCRIPT_MAX_TABLE_BYTES = 64u << 20,
    DH2_SCRIPT_MAX_FIELDS = 8
};

typedef enum dh2_script_error_code {
    DH2_SCRIPT_OK = 0,
    DH2_SCRIPT_INVALID_ARGUMENT = 1,
    DH2_SCRIPT_LIMIT_EXCEEDED = 2,
    DH2_SCRIPT_TRUNCATED = 3,
    DH2_SCRIPT_INVALID_UTF8 = 4,
    DH2_SCRIPT_INVALID_BOOL = 5,
    DH2_SCRIPT_UNKNOWN_COMMAND = 6,
    DH2_SCRIPT_COUNT_MISMATCH = 7,
    DH2_SCRIPT_TRAILING_BYTES = 8,
    DH2_SCRIPT_OUT_OF_MEMORY = 9
} dh2_script_error_code;

typedef enum dh2_script_input_kind {
    DH2_SCRIPT_INPUT_NAMES = 1,
    DH2_SCRIPT_INPUT_PROGRAMS = 2
} dh2_script_input_kind;

typedef struct dh2_script_error {
    dh2_script_error_code code;
    dh2_script_input_kind input_kind;
    uint32_t offset;
    uint32_t script_index;
    uint32_t command_index;
} dh2_script_error;

typedef struct dh2_script_string {
    const uint8_t *data;
    uint32_t size;
} dh2_script_string;

typedef struct dh2_script_i32_array {
    const int32_t *data;
    uint32_t count;
} dh2_script_i32_array;

typedef enum dh2_script_value_kind {
    DH2_SCRIPT_VALUE_I32 = 1,
    DH2_SCRIPT_VALUE_BOOL = 2,
    DH2_SCRIPT_VALUE_STRING = 3,
    DH2_SCRIPT_VALUE_I32_ARRAY = 4,
    DH2_SCRIPT_VALUE_F32 = 5
} dh2_script_value_kind;

/* Data decoder values are distinct from the Lua runtime's dh2_script_value. */
typedef struct dh2_pydata_script_value {
    dh2_script_value_kind kind;
    union {
        int32_t i32;
        float f32;
        uint8_t boolean;
        dh2_script_string string;
        dh2_script_i32_array i32_array;
    } value;
} dh2_pydata_script_value;

typedef struct dh2_script_command {
    uint32_t command_id;
    uint32_t byte_offset;
    uint32_t byte_size;
    uint32_t field_count;
    dh2_pydata_script_value fields[DH2_SCRIPT_MAX_FIELDS];
} dh2_script_command;

typedef struct dh2_script {
    uint32_t table_index;
    uint32_t byte_offset;
    uint32_t byte_end;
    uint32_t command_count;
    dh2_script_string name;
    dh2_script_command *commands;
} dh2_script;

typedef struct dh2_script_table {
    uint8_t *names_storage;
    uint32_t names_size;
    uint8_t *programs_storage;
    uint32_t programs_size;
    uint32_t name_count;
    uint32_t script_count;
    uint32_t total_command_count;
    dh2_script_string *names;
    dh2_script *scripts;
} dh2_script_table;

/* The output must be a fresh, zero-initialized result. On every failure it is
 * reset to an empty result and error contains the input and absolute byte
 * offset of the first detected problem. Both inputs are copied and owned. */
dh2_script_error_code dh2_script_table_decode(
    const uint8_t *names_data,
    uint32_t names_size,
    const uint8_t *programs_data,
    uint32_t programs_size,
    dh2_script_table *out_table,
    dh2_script_error *out_error);

void dh2_script_table_destroy(dh2_script_table *table);

/* Search a single table at/after start_index. Returns the table-local index or
 * -1. Comparison follows the native case-insensitive byte comparison in the
 * default C locale (ASCII letters fold; other UTF-8 bytes compare exactly). */
int32_t dh2_script_name_lookup(
    const dh2_script_table *table,
    const uint8_t *name,
    uint32_t name_size,
    uint32_t start_index);

/* Mirrors ScriptManager::GetIDFromName(name, search_from_global): when true,
 * search common first, then level; when false, search only the level table and
 * return its absolute ID after the common table. */
int32_t dh2_script_resolve_id(
    const dh2_script_table *common_table,
    const dh2_script_table *level_table,
    const uint8_t *name,
    uint32_t name_size,
    uint8_t search_from_global);

const char *dh2_script_command_name(uint32_t command_id);
const char *dh2_script_error_name(dh2_script_error_code code);

#ifdef __cplusplus
}
#endif

#endif
