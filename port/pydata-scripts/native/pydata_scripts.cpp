#include "pydata_scripts.h"

#include <stdlib.h>
#include <string.h>

namespace {

enum FieldCode : uint8_t {
    FIELD_I32 = 1,
    FIELD_BOOL = 2,
    FIELD_STRING = 3,
    FIELD_I32_ARRAY = 4
};

struct Schema {
    uint32_t id;
    const char *name;
    uint8_t field_count;
    uint8_t fields[DH2_SCRIPT_MAX_FIELDS];
};

#define S0(ID, NAME) {ID, NAME, 0, {0}}
#define S1(ID, NAME, A) {ID, NAME, 1, {A}}
#define S2(ID, NAME, A, B) {ID, NAME, 2, {A, B}}
#define S3(ID, NAME, A, B, C) {ID, NAME, 3, {A, B, C}}
#define S4(ID, NAME, A, B, C, D) {ID, NAME, 4, {A, B, C, D}}
#define S5(ID, NAME, A, B, C, D, E) {ID, NAME, 5, {A, B, C, D, E}}
#define S6(ID, NAME, A, B, C, D, E, F) {ID, NAME, 6, {A, B, C, D, E, F}}

/* Corpus commands plus the source-verified CharacterMenu tutorial commands. */
static const Schema kSchemas[] = {
    S4(0, "ExecScript", FIELD_BOOL, FIELD_I32, FIELD_I32_ARRAY, FIELD_BOOL),
    S0(1, "EnterCutSceneMode"),
    S0(2, "ExitCutSceneMode"),
    S1(3, "CONSOLE", FIELD_STRING),
    S2(5, "PlayCamera", FIELD_I32, FIELD_BOOL),
    S2(6, "SetCameraClip", FIELD_I32, FIELD_I32),
    S3(8, "SetCameraTarget", FIELD_I32, FIELD_STRING, FIELD_BOOL),
    S3(10, "StartDialog", FIELD_I32, FIELD_I32, FIELD_I32),
    S1(11, "StartDialogID", FIELD_I32),
    S0(12, "WaitDialog"),
    S4(13, "PlaySound", FIELD_I32, FIELD_BOOL, FIELD_BOOL, FIELD_I32),
    S3(14, "StopSound", FIELD_I32, FIELD_BOOL, FIELD_I32),
    S1(15, "PlayLevelMusic", FIELD_I32),
    S0(16, "EnterSafeZone"),
    S0(17, "LeaveSafeZone"),
    S4(19, "PlayAnimByName", FIELD_STRING, FIELD_BOOL, FIELD_STRING, FIELD_BOOL),
    S6(20, "PlayEffect", FIELD_I32, FIELD_I32, FIELD_I32, FIELD_I32,
       FIELD_STRING, FIELD_BOOL),
    S1(21, "StopEffect", FIELD_I32),
    S3(22, "ShowFlash", FIELD_I32, FIELD_STRING, FIELD_BOOL),
    S3(23, "HideFlash", FIELD_I32, FIELD_STRING, FIELD_BOOL),
    S1(24, "LockCharacter", FIELD_STRING),
    S1(25, "UnlockCharacter", FIELD_STRING),
    S1(26, "Wait", FIELD_I32),
    S2(27, "SetFaeryState", FIELD_I32, FIELD_I32),
    S2(29, "PutCharacterInLimbus", FIELD_BOOL, FIELD_STRING),
    S1(30, "SpawnCharacter", FIELD_STRING),
    S1(31, "PutCharacterInIdle", FIELD_STRING),
    S2(32, "MarkCharacterAsScripted", FIELD_BOOL, FIELD_STRING),
    S1(39, "StopActor", FIELD_STRING),
    S4(40, "MoveActor", FIELD_STRING, FIELD_BOOL, FIELD_STRING, FIELD_BOOL),
    S2(41, "LookActor", FIELD_STRING, FIELD_STRING),
    S1(42, "ShowActor", FIELD_STRING),
    S1(43, "HideActor", FIELD_STRING),
    S1(44, "KillActor", FIELD_STRING),
    S5(45, "PlayActorAnim", FIELD_I32, FIELD_I32, FIELD_I32, FIELD_STRING,
       FIELD_BOOL),
    S2(46, "SetActorPosition", FIELD_STRING, FIELD_STRING),
    S1(51, "UnEquipHands", FIELD_STRING),
    S1(52, "ReEquipHands", FIELD_STRING),
    S2(54, "OpenDoor", FIELD_STRING, FIELD_BOOL),
    S2(55, "CloseDoor", FIELD_STRING, FIELD_BOOL),
    S0(63, "RestartLevel"),
    S0(68, "ShowTrophies"),
    S0(69, "SaveGame"),
    S0(70, "BlockSaveGame"),
    S3(74, "EnqueueCharMenuTutorialMessage", FIELD_STRING, FIELD_STRING, FIELD_I32),
    S0(75, "SkipCharMenuTutorialMessage"),
    S0(76, "SkipAllCharMenuTutorialMessages"),
    S1(77, "LockTutorial", FIELD_I32),
    S2(78, "DoTutorial", FIELD_STRING, FIELD_I32),
    S0(79, "FlushMessages")
};

#undef S0
#undef S1
#undef S2
#undef S3
#undef S4
#undef S5
#undef S6

struct Reader {
    const uint8_t *data;
    uint32_t size;
    uint32_t position;
    uint32_t string_bytes;
    dh2_script_input_kind input_kind;
    uint32_t script_index;
    uint32_t command_index;
    dh2_script_error *error;
};

static void set_error(dh2_script_error *error,
                      dh2_script_error_code code,
                      dh2_script_input_kind input_kind,
                      uint32_t offset,
                      uint32_t script_index,
                      uint32_t command_index) {
    if (error == NULL) {
        return;
    }
    error->code = code;
    error->input_kind = input_kind;
    error->offset = offset;
    error->script_index = script_index;
    error->command_index = command_index;
}

static bool fail(Reader *reader, dh2_script_error_code code, uint32_t offset) {
    set_error(reader->error, code, reader->input_kind, offset,
              reader->script_index, reader->command_index);
    return false;
}

static bool take(Reader *reader, uint32_t count, const uint8_t **out_bytes) {
    const uint32_t start = reader->position;
    if (count > reader->size - start) {
        return fail(reader, DH2_SCRIPT_TRUNCATED, start);
    }
    if (out_bytes != NULL) {
        *out_bytes = count == 0 ? NULL : reader->data + start;
    }
    reader->position += count;
    return true;
}

static bool read_u32(Reader *reader, uint32_t *out_value) {
    const uint8_t *bytes;
    const uint32_t start = reader->position;
    if (!take(reader, 4, &bytes)) {
        return false;
    }
    *out_value = ((uint32_t)bytes[0]) |
                 ((uint32_t)bytes[1] << 8) |
                 ((uint32_t)bytes[2] << 16) |
                 ((uint32_t)bytes[3] << 24);
    (void)start;
    return true;
}

static int32_t signed_from_u32(uint32_t value) {
    if (value <= 0x7fffffffu) {
        return (int32_t)value;
    }
    return (int32_t)(-1 - (int32_t)(0xffffffffu - value));
}

static bool read_i32(Reader *reader, int32_t *out_value) {
    uint32_t raw;
    if (!read_u32(reader, &raw)) {
        return false;
    }
    *out_value = signed_from_u32(raw);
    return true;
}

static bool read_bool(Reader *reader, uint8_t *out_value) {
    const uint32_t start = reader->position;
    const uint8_t *bytes;
    if (!take(reader, 1, &bytes)) {
        return false;
    }
    if (bytes[0] > 1) {
        return fail(reader, DH2_SCRIPT_INVALID_BOOL, start);
    }
    *out_value = bytes[0];
    return true;
}

/* Returns the first invalid byte (or -1 for valid UTF-8). */
static int32_t invalid_utf8_offset(const uint8_t *s, uint32_t size) {
    uint32_t i = 0;
    while (i < size) {
        const uint8_t a = s[i];
        if (a <= 0x7f) {
            ++i;
            continue;
        }
        if (a >= 0xc2 && a <= 0xdf) {
            if (size - i < 2) return (int32_t)i;
            if ((s[i + 1] & 0xc0) != 0x80) return (int32_t)(i + 1);
            i += 2;
            continue;
        }
        if (a >= 0xe0 && a <= 0xef) {
            if (size - i < 3) return (int32_t)i;
            const uint8_t b = s[i + 1];
            const uint8_t c = s[i + 2];
            if ((b & 0xc0) != 0x80) return (int32_t)(i + 1);
            if ((c & 0xc0) != 0x80) return (int32_t)(i + 2);
            if (a == 0xe0 && b < 0xa0) return (int32_t)i;
            if (a == 0xed && b >= 0xa0) return (int32_t)i;
            i += 3;
            continue;
        }
        if (a >= 0xf0 && a <= 0xf4) {
            if (size - i < 4) return (int32_t)i;
            const uint8_t b = s[i + 1];
            const uint8_t c = s[i + 2];
            const uint8_t d = s[i + 3];
            if ((b & 0xc0) != 0x80) return (int32_t)(i + 1);
            if ((c & 0xc0) != 0x80) return (int32_t)(i + 2);
            if ((d & 0xc0) != 0x80) return (int32_t)(i + 3);
            if (a == 0xf0 && b < 0x90) return (int32_t)i;
            if (a == 0xf4 && b > 0x8f) return (int32_t)i;
            i += 4;
            continue;
        }
        return (int32_t)i;
    }
    return -1;
}

static bool read_string(Reader *reader, dh2_script_string *out_string) {
    const uint32_t length_offset = reader->position;
    uint32_t length;
    if (!read_u32(reader, &length)) {
        return false;
    }
    if (length > DH2_SCRIPT_MAX_STRING_BYTES) {
        return fail(reader, DH2_SCRIPT_LIMIT_EXCEEDED, length_offset);
    }
    if (length > DH2_SCRIPT_MAX_TOTAL_STRING_BYTES - reader->string_bytes) {
        return fail(reader, DH2_SCRIPT_LIMIT_EXCEEDED, length_offset);
    }
    const uint32_t string_offset = reader->position;
    const uint8_t *bytes;
    if (!take(reader, length, &bytes)) {
        return false;
    }
    if (length != 0) {
        const int32_t invalid = invalid_utf8_offset(bytes, length);
        if (invalid >= 0) {
            return fail(reader, DH2_SCRIPT_INVALID_UTF8,
                        string_offset + (uint32_t)invalid);
        }
    }
    reader->string_bytes += length;
    out_string->data = bytes;
    out_string->size = length;
    return true;
}

static const Schema *find_schema(uint32_t id) {
    for (uint32_t i = 0; i < (uint32_t)(sizeof(kSchemas) / sizeof(kSchemas[0])); ++i) {
        if (kSchemas[i].id == id) {
            return &kSchemas[i];
        }
    }
    return NULL;
}

static bool read_array(Reader *reader, dh2_script_i32_array *out_array) {
    const uint32_t count_offset = reader->position;
    uint32_t count;
    if (!read_u32(reader, &count)) {
        return false;
    }
    if (count > DH2_SCRIPT_MAX_ARRAY_ITEMS) {
        return fail(reader, DH2_SCRIPT_LIMIT_EXCEEDED, count_offset);
    }
    if (count > (reader->size - reader->position) / 4) {
        return fail(reader, DH2_SCRIPT_TRUNCATED, reader->position);
    }
    int32_t *items = NULL;
    if (count != 0) {
        items = (int32_t *)malloc((size_t)count * sizeof(int32_t));
        if (items == NULL) {
            return fail(reader, DH2_SCRIPT_OUT_OF_MEMORY, count_offset);
        }
    }
    out_array->data = items;
    out_array->count = count;
    for (uint32_t i = 0; i < count; ++i) {
        if (!read_i32(reader, &items[i])) {
            free(items);
            out_array->data = NULL;
            out_array->count = 0;
            return false;
        }
    }
    return true;
}

static bool read_value(Reader *reader,
                       uint8_t field_code,
                       dh2_pydata_script_value *out_value) {
    memset(out_value, 0, sizeof(*out_value));
    switch (field_code) {
        case FIELD_I32:
            out_value->kind = DH2_SCRIPT_VALUE_I32;
            return read_i32(reader, &out_value->value.i32);
        case FIELD_BOOL:
            out_value->kind = DH2_SCRIPT_VALUE_BOOL;
            return read_bool(reader, &out_value->value.boolean);
        case FIELD_STRING:
            out_value->kind = DH2_SCRIPT_VALUE_STRING;
            return read_string(reader, &out_value->value.string);
        case FIELD_I32_ARRAY:
            out_value->kind = DH2_SCRIPT_VALUE_I32_ARRAY;
            return read_array(reader, &out_value->value.i32_array);
        default:
            return fail(reader, DH2_SCRIPT_INVALID_ARGUMENT, reader->position);
    }
}

static bool parse_names(dh2_script_table *table,
                        dh2_script_error *error) {
    Reader reader;
    memset(&reader, 0, sizeof(reader));
    reader.data = table->names_storage;
    reader.size = table->names_size;
    reader.input_kind = DH2_SCRIPT_INPUT_NAMES;
    reader.error = error;

    const uint32_t count_offset = reader.position;
    uint32_t count;
    if (!read_u32(&reader, &count)) {
        return false;
    }
    if (count > DH2_SCRIPT_MAX_SCRIPTS) {
        return fail(&reader, DH2_SCRIPT_LIMIT_EXCEEDED, count_offset);
    }
    table->name_count = count;
    if (count != 0) {
        table->names = (dh2_script_string *)calloc(count, sizeof(dh2_script_string));
        if (table->names == NULL) {
            return fail(&reader, DH2_SCRIPT_OUT_OF_MEMORY, count_offset);
        }
    }
    for (uint32_t i = 0; i < count; ++i) {
        reader.script_index = i;
        if (!read_string(&reader, &table->names[i])) {
            return false;
        }
    }
    if (reader.position != reader.size) {
        return fail(&reader, DH2_SCRIPT_TRAILING_BYTES, reader.position);
    }
    return true;
}

static bool parse_programs(dh2_script_table *table,
                           dh2_script_error *error) {
    Reader reader;
    memset(&reader, 0, sizeof(reader));
    reader.data = table->programs_storage;
    reader.size = table->programs_size;
    reader.input_kind = DH2_SCRIPT_INPUT_PROGRAMS;
    reader.error = error;

    const uint32_t count_offset = reader.position;
    uint32_t script_count;
    if (!read_u32(&reader, &script_count)) {
        return false;
    }
    if (script_count > DH2_SCRIPT_MAX_SCRIPTS) {
        return fail(&reader, DH2_SCRIPT_LIMIT_EXCEEDED, count_offset);
    }
    if (script_count != table->name_count) {
        return fail(&reader, DH2_SCRIPT_COUNT_MISMATCH, count_offset);
    }
    table->script_count = script_count;
    if (script_count != 0) {
        table->scripts = (dh2_script *)calloc(script_count, sizeof(dh2_script));
        if (table->scripts == NULL) {
            return fail(&reader, DH2_SCRIPT_OUT_OF_MEMORY, count_offset);
        }
    }

    uint32_t total_commands = 0;
    for (uint32_t script_index = 0; script_index < script_count; ++script_index) {
        dh2_script *script = &table->scripts[script_index];
        reader.script_index = script_index;
        reader.command_index = 0;
        script->table_index = script_index;
        script->name = table->names[script_index];
        script->byte_offset = reader.position;
        const uint32_t command_count_offset = reader.position;
        uint32_t command_count;
        if (!read_u32(&reader, &command_count)) {
            return false;
        }
        if (command_count > DH2_SCRIPT_MAX_COMMANDS_PER_SCRIPT) {
            return fail(&reader, DH2_SCRIPT_LIMIT_EXCEEDED, command_count_offset);
        }
        if (command_count > DH2_SCRIPT_MAX_TOTAL_COMMANDS - total_commands) {
            return fail(&reader, DH2_SCRIPT_LIMIT_EXCEEDED, command_count_offset);
        }
        total_commands += command_count;
        script->command_count = command_count;
        if (command_count != 0) {
            script->commands = (dh2_script_command *)calloc(
                command_count, sizeof(dh2_script_command));
            if (script->commands == NULL) {
                return fail(&reader, DH2_SCRIPT_OUT_OF_MEMORY, command_count_offset);
            }
        }

        for (uint32_t command_index = 0; command_index < command_count; ++command_index) {
            dh2_script_command *command = &script->commands[command_index];
            reader.command_index = command_index;
            command->byte_offset = reader.position;
            uint32_t command_id;
            if (!read_u32(&reader, &command_id)) {
                return false;
            }
            const Schema *schema = find_schema(command_id);
            if (schema == NULL) {
                return fail(&reader, DH2_SCRIPT_UNKNOWN_COMMAND, command->byte_offset);
            }
            command->command_id = command_id;
            command->field_count = schema->field_count;
            for (uint32_t field_index = 0; field_index < schema->field_count; ++field_index) {
                if (!read_value(&reader, schema->fields[field_index],
                                &command->fields[field_index])) {
                    return false;
                }
            }
            command->byte_size = reader.position - command->byte_offset;
        }
        script->byte_end = reader.position;
    }
    table->total_command_count = total_commands;
    if (reader.position != reader.size) {
        reader.script_index = script_count;
        reader.command_index = 0;
        return fail(&reader, DH2_SCRIPT_TRAILING_BYTES, reader.position);
    }
    return true;
}

static uint8_t fold_ascii(uint8_t value) {
    if (value >= (uint8_t)'A' && value <= (uint8_t)'Z') {
        return (uint8_t)(value + ((uint8_t)'a' - (uint8_t)'A'));
    }
    return value;
}

static bool equal_native_case_insensitive(dh2_script_string candidate,
                                          const uint8_t *name,
                                          uint32_t name_size) {
    uint32_t i = 0;
    while (true) {
        const bool candidate_end = i >= candidate.size || candidate.data[i] == 0;
        const bool name_end = i >= name_size || name[i] == 0;
        if (candidate_end || name_end) {
            return candidate_end && name_end;
        }
        if (fold_ascii(candidate.data[i]) != fold_ascii(name[i])) {
            return false;
        }
        ++i;
    }
}

}  // namespace

extern "C" dh2_script_error_code dh2_script_table_decode(
    const uint8_t *names_data,
    uint32_t names_size,
    const uint8_t *programs_data,
    uint32_t programs_size,
    dh2_script_table *out_table,
    dh2_script_error *out_error) {
    if (out_table == NULL) {
        set_error(out_error, DH2_SCRIPT_INVALID_ARGUMENT, DH2_SCRIPT_INPUT_NAMES,
                  0, 0, 0);
        return DH2_SCRIPT_INVALID_ARGUMENT;
    }
    memset(out_table, 0, sizeof(*out_table));
    if (out_error != NULL) {
        memset(out_error, 0, sizeof(*out_error));
    }
    if ((names_size != 0 && names_data == NULL) ||
        (programs_size != 0 && programs_data == NULL)) {
        set_error(out_error, DH2_SCRIPT_INVALID_ARGUMENT,
                  names_size != 0 && names_data == NULL
                      ? DH2_SCRIPT_INPUT_NAMES : DH2_SCRIPT_INPUT_PROGRAMS,
                  0, 0, 0);
        return DH2_SCRIPT_INVALID_ARGUMENT;
    }
    if (names_size > DH2_SCRIPT_MAX_TABLE_BYTES) {
        set_error(out_error, DH2_SCRIPT_LIMIT_EXCEEDED, DH2_SCRIPT_INPUT_NAMES,
                  0, 0, 0);
        return DH2_SCRIPT_LIMIT_EXCEEDED;
    }
    if (programs_size > DH2_SCRIPT_MAX_TABLE_BYTES) {
        set_error(out_error, DH2_SCRIPT_LIMIT_EXCEEDED, DH2_SCRIPT_INPUT_PROGRAMS,
                  0, 0, 0);
        return DH2_SCRIPT_LIMIT_EXCEEDED;
    }

    dh2_script_table result;
    memset(&result, 0, sizeof(result));
    result.names_size = names_size;
    result.programs_size = programs_size;
    if (names_size != 0) {
        result.names_storage = (uint8_t *)malloc(names_size);
        if (result.names_storage == NULL) {
            set_error(out_error, DH2_SCRIPT_OUT_OF_MEMORY, DH2_SCRIPT_INPUT_NAMES,
                      0, 0, 0);
            return DH2_SCRIPT_OUT_OF_MEMORY;
        }
        memcpy(result.names_storage, names_data, names_size);
    }
    if (programs_size != 0) {
        result.programs_storage = (uint8_t *)malloc(programs_size);
        if (result.programs_storage == NULL) {
            dh2_script_table_destroy(&result);
            set_error(out_error, DH2_SCRIPT_OUT_OF_MEMORY, DH2_SCRIPT_INPUT_PROGRAMS,
                      0, 0, 0);
            return DH2_SCRIPT_OUT_OF_MEMORY;
        }
        memcpy(result.programs_storage, programs_data, programs_size);
    }

    dh2_script_error local_error;
    memset(&local_error, 0, sizeof(local_error));
    dh2_script_error *error_target = out_error != NULL ? out_error : &local_error;
    if (!parse_names(&result, error_target) || !parse_programs(&result, error_target)) {
        const dh2_script_error_code code = error_target->code;
        dh2_script_table_destroy(&result);
        return code;
    }
    *out_table = result;
    return DH2_SCRIPT_OK;
}

extern "C" void dh2_script_table_destroy(dh2_script_table *table) {
    if (table == NULL) {
        return;
    }
    if (table->scripts != NULL) {
        for (uint32_t script_index = 0; script_index < table->script_count; ++script_index) {
            dh2_script *script = &table->scripts[script_index];
            if (script->commands != NULL) {
                for (uint32_t command_index = 0;
                     command_index < script->command_count;
                     ++command_index) {
                    dh2_script_command *command = &script->commands[command_index];
                    for (uint32_t field_index = 0;
                         field_index < command->field_count;
                         ++field_index) {
                        dh2_pydata_script_value *field = &command->fields[field_index];
                        if (field->kind == DH2_SCRIPT_VALUE_I32_ARRAY) {
                            free((void *)field->value.i32_array.data);
                        }
                    }
                }
                free(script->commands);
            }
        }
        free(table->scripts);
    }
    free(table->names);
    free(table->names_storage);
    free(table->programs_storage);
    memset(table, 0, sizeof(*table));
}

extern "C" int32_t dh2_script_name_lookup(
    const dh2_script_table *table,
    const uint8_t *name,
    uint32_t name_size,
    uint32_t start_index) {
    if (table == NULL || (name_size != 0 && name == NULL) ||
        start_index > table->name_count) {
        return -1;
    }
    for (uint32_t i = start_index; i < table->name_count; ++i) {
        if (equal_native_case_insensitive(table->names[i], name, name_size)) {
            return (int32_t)i;
        }
    }
    return -1;
}

extern "C" int32_t dh2_script_resolve_id(
    const dh2_script_table *common_table,
    const dh2_script_table *level_table,
    const uint8_t *name,
    uint32_t name_size,
    uint8_t search_from_global) {
    if ((common_table == NULL) || (level_table == NULL) ||
        (name_size != 0 && name == NULL)) {
        return -1;
    }
    if (search_from_global != 0) {
        const int32_t common_id = dh2_script_name_lookup(
            common_table, name, name_size, 0);
        if (common_id >= 0) {
            return common_id;
        }
    }
    const int32_t level_id = dh2_script_name_lookup(level_table, name, name_size, 0);
    if (level_id < 0) {
        return -1;
    }
    return (int32_t)common_table->name_count + level_id;
}

extern "C" const char *dh2_script_command_name(uint32_t command_id) {
    const Schema *schema = find_schema(command_id);
    return schema == NULL ? NULL : schema->name;
}

extern "C" const char *dh2_script_error_name(dh2_script_error_code code) {
    switch (code) {
        case DH2_SCRIPT_OK: return "ok";
        case DH2_SCRIPT_INVALID_ARGUMENT: return "invalid argument";
        case DH2_SCRIPT_LIMIT_EXCEEDED: return "limit exceeded";
        case DH2_SCRIPT_TRUNCATED: return "truncated input";
        case DH2_SCRIPT_INVALID_UTF8: return "invalid UTF-8";
        case DH2_SCRIPT_INVALID_BOOL: return "invalid bool";
        case DH2_SCRIPT_UNKNOWN_COMMAND: return "unknown command";
        case DH2_SCRIPT_COUNT_MISMATCH: return "count mismatch";
        case DH2_SCRIPT_TRAILING_BYTES: return "trailing bytes";
        case DH2_SCRIPT_OUT_OF_MEMORY: return "out of memory";
        default: return "unknown error";
    }
}
