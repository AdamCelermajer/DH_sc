#include "../pydata_scripts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void check(int condition, const char *label) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", label);
        ++failures;
    }
}

static uint8_t *read_file(const char *path, uint32_t *out_size) {
    FILE *file = fopen(path, "rb");
    long length;
    uint8_t *bytes;
    if (file == NULL) {
        fprintf(stderr, "cannot open %s\n", path);
        return NULL;
    }
    if (fseek(file, 0, SEEK_END) != 0 || (length = ftell(file)) < 0 ||
        (unsigned long)length > 0xfffffffful || fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        fprintf(stderr, "cannot size %s\n", path);
        return NULL;
    }
    bytes = (uint8_t *)malloc(length == 0 ? 1u : (size_t)length);
    if (bytes == NULL || fread(bytes, 1, (size_t)length, file) != (size_t)length) {
        free(bytes);
        fclose(file);
        fprintf(stderr, "cannot read %s\n", path);
        return NULL;
    }
    fclose(file);
    *out_size = (uint32_t)length;
    return bytes;
}

static int string_is(dh2_script_string value, const char *text) {
    const size_t n = strlen(text);
    return value.size == n && (n == 0 || memcmp(value.data, text, n) == 0);
}

static uint32_t find_script(const dh2_script_table *table, const char *name) {
    int32_t index = dh2_script_name_lookup(table, (const uint8_t *)name,
                                            (uint32_t)strlen(name), 0);
    return index < 0 ? 0xffffffffu : (uint32_t)index;
}

static void check_i32(const dh2_pydata_script_value *value, int32_t expected,
                      const char *label) {
    check(value->kind == DH2_SCRIPT_VALUE_I32 && value->value.i32 == expected,
          label);
}

static int run_corpus(const char *common_names_path,
                      const char *common_programs_path,
                      const char *swamp_names_path,
                      const char *swamp_programs_path) {
    uint32_t common_names_size = 0, common_programs_size = 0;
    uint32_t swamp_names_size = 0, swamp_programs_size = 0;
    uint8_t *common_names_data = read_file(common_names_path, &common_names_size);
    uint8_t *common_programs_data = read_file(common_programs_path,
                                               &common_programs_size);
    uint8_t *swamp_names_data = read_file(swamp_names_path, &swamp_names_size);
    uint8_t *swamp_programs_data = read_file(swamp_programs_path,
                                              &swamp_programs_size);
    dh2_script_table common = {0};
    dh2_script_table swamp = {0};
    dh2_script_error error = {0};
    dh2_script_error_code code;
    int common_ok = 0;
    int swamp_ok = 0;
    uint32_t i;
    uint32_t common_command_sum = 0;
    uint32_t swamp_command_sum = 0;

    if (common_names_data == NULL || common_programs_data == NULL ||
        swamp_names_data == NULL || swamp_programs_data == NULL) {
        free(common_names_data);
        free(common_programs_data);
        free(swamp_names_data);
        free(swamp_programs_data);
        return 1;
    }

    code = dh2_script_table_decode(common_names_data, common_names_size,
                                   common_programs_data, common_programs_size,
                                   &common, &error);
    check(code == DH2_SCRIPT_OK, "decode common tables");
    common_ok = code == DH2_SCRIPT_OK;
    if (code != DH2_SCRIPT_OK) {
        fprintf(stderr, "common decode: %s input=%u offset=%u script=%u command=%u\n",
                dh2_script_error_name(error.code), (unsigned)error.input_kind,
                (unsigned)error.offset, (unsigned)error.script_index,
                (unsigned)error.command_index);
    }
    code = dh2_script_table_decode(swamp_names_data, swamp_names_size,
                                   swamp_programs_data, swamp_programs_size,
                                   &swamp, &error);
    check(code == DH2_SCRIPT_OK, "decode SWAMP tables");
    swamp_ok = code == DH2_SCRIPT_OK;
    if (code != DH2_SCRIPT_OK) {
        fprintf(stderr, "SWAMP decode: %s input=%u offset=%u script=%u command=%u\n",
                dh2_script_error_name(error.code), (unsigned)error.input_kind,
                (unsigned)error.offset, (unsigned)error.script_index,
                (unsigned)error.command_index);
    }

    /* The returned names, strings, and vectors must outlive caller buffers. */
    memset(common_names_data, 0xa5, common_names_size);
    memset(common_programs_data, 0xa5, common_programs_size);
    memset(swamp_names_data, 0xa5, swamp_names_size);
    memset(swamp_programs_data, 0xa5, swamp_programs_size);
    free(common_names_data);
    free(common_programs_data);
    free(swamp_names_data);
    free(swamp_programs_data);

    check(common.name_count == 15 && common.script_count == 15,
          "common corpus has 15 named scripts and programs");
    check(swamp.name_count == 55 && swamp.script_count == 55,
          "SWAMP corpus has 55 named scripts and programs");
    for (i = 0; i < common.script_count; ++i) {
        common_command_sum += common.scripts[i].command_count;
    }
    for (i = 0; i < swamp.script_count; ++i) {
        swamp_command_sum += swamp.scripts[i].command_count;
    }
    check(common_command_sum == 81 && common.total_command_count == 81,
          "all 81 common commands parsed");
    check(swamp_command_sum == 789 && swamp.total_command_count == 789,
          "all 789 SWAMP commands parsed");

    if (common_ok && common.script_count > 1) {
        check(string_is(common.scripts[1].name, "BeginScriptedCutScene"),
              "common script names share the owned result");
    }
    check(dh2_script_resolve_id(&common, &swamp,
                                (const uint8_t *)"lIzArDmAn_InTrO", 15, 0) == 32,
          "level-only native lookup adds common table base");
    check(dh2_script_resolve_id(&common, &swamp,
                                (const uint8_t *)"BeginScriptedCutScene", 21, 1) == 1,
          "global lookup resolves common table first");
    check(dh2_script_resolve_id(&common, &swamp,
                                (const uint8_t *)"LizardMan_Intro", 15, 1) == 32,
          "global lookup resolves level table after common");
    check(dh2_script_command_name(26) != NULL &&
          strcmp(dh2_script_command_name(26), "Wait") == 0,
          "known command names are available");
    check(dh2_script_command_name(4) == NULL, "unknown command has no name");

    if (swamp_ok) {
        const uint32_t index = find_script(&swamp, "LizardMan_Intro");
        check(index == 17, "LizardMan_Intro table index");
        if (index < swamp.script_count) {
            const dh2_script *script = &swamp.scripts[index];
            static const uint32_t expected_offsets[12] = {
                2430, 2444, 2455, 2493, 2501, 2535,
                2543, 2577, 2585, 2596, 2620, 2634
            };
            static const uint32_t expected_sizes[12] = {
                14, 11, 38, 8, 34, 8, 34, 8, 11, 24, 14, 22
            };
            static const uint32_t expected_ids[12] = {
                0, 24, 8, 26, 30, 26, 30, 26, 25, 8, 0, 78
            };
            check(script->byte_offset == 2426 && script->byte_end == 2656,
                  "LizardMan_Intro record offsets");
            check(script->command_count == 12, "LizardMan_Intro has 12 commands");
            for (i = 0; i < 12 && i < script->command_count; ++i) {
                check(script->commands[i].byte_offset == expected_offsets[i],
                      "LizardMan_Intro command byte offset");
                check(script->commands[i].byte_size == expected_sizes[i],
                      "LizardMan_Intro command byte size");
                check(script->commands[i].command_id == expected_ids[i],
                      "LizardMan_Intro command ID");
            }
            check(script->commands[0].fields[0].kind == DH2_SCRIPT_VALUE_BOOL &&
                  script->commands[0].fields[0].value.boolean == 1 &&
                  script->commands[0].fields[1].value.i32 == 1 &&
                  script->commands[0].fields[2].value.i32_array.count == 0 &&
                  script->commands[0].fields[3].value.boolean == 1,
                  "LizardMan_Intro first ExecScript payload");
            check(string_is(script->commands[1].fields[0].value.string, "All"),
                  "LizardMan_Intro LockCharacter payload");
            check(script->commands[2].fields[0].value.i32 == 1000 &&
                  string_is(script->commands[2].fields[1].value.string,
                            "_prim_Waypoint_NewCamSpot") &&
                  script->commands[2].fields[2].value.boolean == 0,
                  "LizardMan_Intro SetCameraTarget payload");
            check(script->commands[3].fields[0].value.i32 == 500 &&
                  script->commands[5].fields[0].value.i32 == 1500 &&
                  script->commands[7].fields[0].value.i32 == 2000,
                  "LizardMan_Intro wait payloads");
            check(string_is(script->commands[4].fields[0].value.string,
                            "_prim_Monster_LizManIntro1") &&
                  string_is(script->commands[6].fields[0].value.string,
                            "_prim_Monster_LizManIntro2"),
                  "LizardMan_Intro spawn payloads");
        }
    }

    if (swamp_ok) {
        const uint32_t index = find_script(&swamp, "enterLocation_StartRoom");
        check(index == 45, "enterLocation_StartRoom table index");
        if (index < swamp.script_count) {
            const dh2_script *script = &swamp.scripts[index];
            check(script->byte_offset == 17424 && script->byte_end == 17444,
                  "enterLocation_StartRoom record offsets");
            check(script->command_count == 1, "StartRoom has one command");
            if (script->command_count == 1) {
                const dh2_script_command *command = &script->commands[0];
                check(command->byte_offset == 17428 && command->byte_size == 16,
                      "StartRoom StartDialog command offset and size");
                check(command->command_id == 10 && command->field_count == 3,
                      "StartRoom StartDialog schema");
                check_i32(&command->fields[0], -1, "StartRoom dialog argument 0");
                check_i32(&command->fields[1], 3, "StartRoom dialog argument 1");
                check_i32(&command->fields[2], 1703960,
                          "StartRoom dialog argument 2");
            }
        }
    }

    dh2_script_table_destroy(&common);
    dh2_script_table_destroy(&swamp);
    return failures == 0 ? 0 : 1;
}

static void put_u32(uint8_t *out, uint32_t value) {
    out[0] = (uint8_t)value;
    out[1] = (uint8_t)(value >> 8);
    out[2] = (uint8_t)(value >> 16);
    out[3] = (uint8_t)(value >> 24);
}

static int test_char_menu_tutorial_schema(void) {
    const uint8_t names[] = {1,0,0,0,1,0,0,0,'T'};
    uint8_t programs[64] = {0};
    uint32_t at=0;
    dh2_script_table table={0};
    dh2_script_error error={0};
    put_u32(programs+at,1);at+=4; /* one script */
    put_u32(programs+at,3);at+=4; /* three tutorial commands */
    put_u32(programs+at,74);at+=4;
    put_u32(programs+at,6);at+=4;memcpy(programs+at,"Skills",6);at+=6;
    put_u32(programs+at,14);at+=4;memcpy(programs+at,"menu_Character",14);at+=14;
    put_u32(programs+at,341);at+=4;
    put_u32(programs+at,75);at+=4;
    put_u32(programs+at,76);at+=4;
    const dh2_script_error_code code=dh2_script_table_decode(
        names,sizeof(names),programs,at,&table,&error);
    check(code==DH2_SCRIPT_OK&&table.script_count==1&&
          table.scripts[0].command_count==3,
          "native decoder accepts tutorial enqueue/skip command IDs 74-76");
    if(code==DH2_SCRIPT_OK){
        const dh2_script_command* enqueue=&table.scripts[0].commands[0];
        check(enqueue->command_id==74&&enqueue->field_count==3&&
              enqueue->fields[0].kind==DH2_SCRIPT_VALUE_STRING&&
              string_is(enqueue->fields[0].value.string,"Skills")&&
              string_is(enqueue->fields[1].value.string,"menu_Character")&&
              enqueue->fields[2].kind==DH2_SCRIPT_VALUE_I32&&
              enqueue->fields[2].value.i32==341,
              "native decoder retains frame/menu/string-ID source layout");
        check(table.scripts[0].commands[1].command_id==75&&
              table.scripts[0].commands[2].command_id==76,
              "native decoder distinguishes one/all tutorial skip commands");
    }
    dh2_script_table_destroy(&table);
    return failures==0?0:1;
}

static int test_start_dialog_id_schema(void) {
    const uint8_t names[] = {1,0,0,0,1,0,0,0,'T'};
    uint8_t programs[32] = {0};
    uint32_t at=0;
    dh2_script_table table={0};
    dh2_script_error error={0};
    put_u32(programs+at,1);at+=4;
    put_u32(programs+at,1);at+=4;
    put_u32(programs+at,11);at+=4;
    put_u32(programs+at,0);at+=4;
    const dh2_script_error_code code=dh2_script_table_decode(
        names,sizeof(names),programs,at,&table,&error);
    check(code==DH2_SCRIPT_OK&&table.script_count==1&&
          table.scripts[0].command_count==1,
          "native decoder accepts the source StartDialogID schema");
    if(code==DH2_SCRIPT_OK){
        const dh2_script_command* command=&table.scripts[0].commands[0];
        check(command->command_id==11&&command->field_count==1&&
              command->fields[0].kind==DH2_SCRIPT_VALUE_I32&&
              command->fields[0].value.i32==0,
              "StartDialogID retains its one signed dialog-index field");
    }
    dh2_script_table_destroy(&table);
    return failures==0?0:1;
}

static int expect_error(const char *label,
                        const uint8_t *names, uint32_t names_size,
                        const uint8_t *programs, uint32_t programs_size,
                        dh2_script_error_code expected_code,
                        dh2_script_input_kind expected_input,
                        uint32_t expected_offset) {
    dh2_script_table table = {0};
    dh2_script_error error = {0};
    dh2_script_error_code code = dh2_script_table_decode(
        names, names_size, programs, programs_size, &table, &error);
    int ok = code == expected_code && error.code == expected_code &&
             error.input_kind == expected_input && error.offset == expected_offset &&
             table.names_storage == NULL && table.programs_storage == NULL &&
             table.names == NULL && table.scripts == NULL &&
             table.name_count == 0 && table.script_count == 0;
    if (!ok) {
        fprintf(stderr, "FAIL: %s: returned=%s error=%s input=%u offset=%u\n",
                label, dh2_script_error_name(code), dh2_script_error_name(error.code),
                (unsigned)error.input_kind, (unsigned)error.offset);
        ++failures;
    }
    dh2_script_table_destroy(&table);
    return ok;
}

static int run_malformed(void) {
    static const uint8_t no_bytes[] = {0};
    static const uint8_t truncated_names_header[] = {0, 0};
    static const uint8_t truncated_name_body[] = {1, 0, 0, 0, 1, 0, 0, 0};
    static const uint8_t one_name[] = {1, 0, 0, 0, 1, 0, 0, 0, 'x'};
    static const uint8_t unknown_id[] = {
        1, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0
    };
    static const uint8_t truncated_wait[] = {
        1, 0, 0, 0, 1, 0, 0, 0, 26, 0, 0, 0
    };
    static const uint8_t mismatch_programs[] = {2, 0, 0, 0};
    static const uint8_t trailing_names[] = {
        0, 0, 0, 0, 0xaa
    };
    static const uint8_t trailing_programs[] = {
        0, 0, 0, 0, 0xaa
    };
    static const uint8_t invalid_bool_camera_target[] = {
        1, 0, 0, 0, 1, 0, 0, 0, 8, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 2
    };
    static const uint8_t invalid_utf8_name[] = {
        1, 0, 0, 0, 1, 0, 0, 0, 0xff
    };
    uint8_t oversized_script_count[4];
    uint8_t oversized_array[21];
    uint8_t one_byte = 0;

    expect_error("truncated name-table header", truncated_names_header,
                 sizeof(truncated_names_header), no_bytes, 0,
                 DH2_SCRIPT_TRUNCATED, DH2_SCRIPT_INPUT_NAMES, 0);
    expect_error("truncated name bytes", truncated_name_body,
                 sizeof(truncated_name_body), no_bytes, 0,
                 DH2_SCRIPT_TRUNCATED, DH2_SCRIPT_INPUT_NAMES, 8);
    expect_error("unknown command ID", one_name, sizeof(one_name),
                 unknown_id, sizeof(unknown_id), DH2_SCRIPT_UNKNOWN_COMMAND,
                 DH2_SCRIPT_INPUT_PROGRAMS, 8);
    expect_error("truncated Wait argument", one_name, sizeof(one_name),
                 truncated_wait, sizeof(truncated_wait), DH2_SCRIPT_TRUNCATED,
                 DH2_SCRIPT_INPUT_PROGRAMS, 12);
    expect_error("name/program count mismatch", one_name, sizeof(one_name),
                 mismatch_programs, sizeof(mismatch_programs),
                 DH2_SCRIPT_COUNT_MISMATCH, DH2_SCRIPT_INPUT_PROGRAMS, 0);
    expect_error("trailing name bytes", trailing_names, sizeof(trailing_names),
                 no_bytes, 0, DH2_SCRIPT_TRAILING_BYTES,
                 DH2_SCRIPT_INPUT_NAMES, 4);
    expect_error("trailing program bytes", trailing_names, 4,
                 trailing_programs, sizeof(trailing_programs),
                 DH2_SCRIPT_TRAILING_BYTES, DH2_SCRIPT_INPUT_PROGRAMS, 4);
    expect_error("invalid bool byte", one_name, sizeof(one_name),
                 invalid_bool_camera_target, sizeof(invalid_bool_camera_target),
                 DH2_SCRIPT_INVALID_BOOL, DH2_SCRIPT_INPUT_PROGRAMS, 20);
    expect_error("invalid UTF-8 script name", invalid_utf8_name,
                 sizeof(invalid_utf8_name), no_bytes, 0,
                 DH2_SCRIPT_INVALID_UTF8, DH2_SCRIPT_INPUT_NAMES, 8);

    put_u32(oversized_script_count, DH2_SCRIPT_MAX_SCRIPTS + 1u);
    expect_error("script count limit", oversized_script_count,
                 sizeof(oversized_script_count), no_bytes, 0,
                 DH2_SCRIPT_LIMIT_EXCEEDED, DH2_SCRIPT_INPUT_NAMES, 0);

    memset(oversized_array, 0, sizeof(oversized_array));
    put_u32(oversized_array, 1);       /* script count */
    put_u32(oversized_array + 4, 1);   /* command count */
    put_u32(oversized_array + 8, 0);   /* ExecScript */
    oversized_array[12] = 0;           /* bool */
    put_u32(oversized_array + 13, 1);  /* script ID */
    put_u32(oversized_array + 17, DH2_SCRIPT_MAX_ARRAY_ITEMS + 1u);
    expect_error("ExecScript array limit", one_name, sizeof(one_name),
                 oversized_array, sizeof(oversized_array),
                 DH2_SCRIPT_LIMIT_EXCEEDED, DH2_SCRIPT_INPUT_PROGRAMS, 17);

    {
        dh2_script_table table = {0};
        dh2_script_error error = {0};
        const dh2_script_error_code code = dh2_script_table_decode(
            &one_byte, DH2_SCRIPT_MAX_TABLE_BYTES + 1u, no_bytes, 0,
            &table, &error);
        check(code == DH2_SCRIPT_LIMIT_EXCEEDED &&
              error.input_kind == DH2_SCRIPT_INPUT_NAMES && error.offset == 0,
              "table byte-size limit checked before reading/copying");
        dh2_script_table_destroy(&table);
    }
    {
        dh2_script_table table = {0};
        const dh2_script_error_code code = dh2_script_table_decode(
            truncated_names_header, sizeof(truncated_names_header),
            no_bytes, 0, &table, NULL);
        check(code == DH2_SCRIPT_TRUNCATED,
              "failure code remains available when error detail is omitted");
        dh2_script_table_destroy(&table);
    }
    return failures == 0 ? 0 : 1;
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--malformed") == 0) {
        if (test_start_dialog_id_schema() != 0) return 1;
        if (test_char_menu_tutorial_schema() != 0) return 1;
        if (run_malformed() != 0) return 1;
        printf("native tutorial schema and malformed-input checks passed\n");
        return 0;
    }
    if (argc != 5) {
        fprintf(stderr, "usage: %s COMMON_NAMES COMMON_PROGRAMS SWAMP_NAMES SWAMP_PROGRAMS\n",
                argv[0]);
        return 2;
    }
    if (run_corpus(argv[1], argv[2], argv[3], argv[4]) != 0) return 1;
    if (test_start_dialog_id_schema() != 0) return 1;
    if (test_char_menu_tutorial_schema() != 0) return 1;
    if (run_malformed() != 0) return 1;
    printf("native corpus and malformed-input checks passed\n");
    return 0;
}
