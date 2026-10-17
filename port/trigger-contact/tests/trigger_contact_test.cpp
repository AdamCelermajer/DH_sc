#include "../trigger_contact.hpp"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace dh2_script_runtime;
using namespace dh2_trigger_contact;

static int failures = 0;

static void check(bool condition, const char *label) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", label);
        ++failures;
    }
}

static bool close(float actual, float expected) {
    return isfinite(actual) && fabsf(actual - expected) <= 0.001f;
}

static uint8_t *read_file(const char *path, uint32_t *out_size) {
    FILE *file = fopen(path, "rb");
    long length;
    uint8_t *data;
    if (file == NULL || fseek(file, 0, SEEK_END) != 0 ||
        (length = ftell(file)) < 0 || (unsigned long)length > 0xfffffffful ||
        fseek(file, 0, SEEK_SET) != 0) {
        if (file != NULL) fclose(file);
        fprintf(stderr, "cannot open/size %s\n", path);
        return NULL;
    }
    data = (uint8_t *)malloc(length == 0 ? 1u : (size_t)length);
    if (data == NULL || fread(data, 1, (size_t)length, file) != (size_t)length) {
        free(data);
        fclose(file);
        fprintf(stderr, "cannot read %s\n", path);
        return NULL;
    }
    fclose(file);
    *out_size = (uint32_t)length;
    return data;
}

static bool load_table(const char *names_path, const char *programs_path,
                       dh2_script_table *table, const char *label) {
    uint32_t names_size = 0;
    uint32_t programs_size = 0;
    uint8_t *names = read_file(names_path, &names_size);
    uint8_t *programs = read_file(programs_path, &programs_size);
    dh2_script_error error = {};
    if (names == NULL || programs == NULL) {
        free(names);
        free(programs);
        return false;
    }
    const dh2_script_error_code result = dh2_script_table_decode(
        names, names_size, programs, programs_size, table, &error);
    free(names);
    free(programs);
    if (result != DH2_SCRIPT_OK) {
        fprintf(stderr, "%s decode failed: %s input=%u offset=%u\n", label,
                dh2_script_error_name(result), (unsigned)error.input_kind,
                (unsigned)error.offset);
        return false;
    }
    return true;
}

static uint32_t count_event(const Runtime &runtime, EventType type) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < runtime.event_count; ++i) {
        if (runtime.events[i].type == type) ++count;
    }
    return count;
}

static bool init_runtime(Runtime *runtime,
                         const dh2_script_table *common,
                         const dh2_script_table *level,
                         int32_t trigger_count) {
    Options options;
    init_options(&options);
    options.local_player_has_character = 1;
    options.online = 0;
    options.game_difficulty = 0;
    return init(runtime, common, level, NULL, 0,
                INFECTED_VILLAGE_AMBUSH.trigger_name,
                INFECTED_VILLAGE_AMBUSH.script_name,
                trigger_count, INFECTED_VILLAGE_AMBUSH.delay_ms,
                &options) == ERROR_OK;
}

static Frame make_frame(const PlayerAabb *players, uint32_t player_count) {
    Frame frame = {};
    frame.trigger_bounds = {-1.0f, -1.0f, -1.0f,
                             1.0f,  1.0f,  1.0f};
    frame.players = players;
    frame.player_count = player_count;
    frame.enabled = 1;
    return frame;
}

static void test_contact_cases(const dh2_script_table *common,
                               const dh2_script_table *level) {
    const PlayerAabb outside = {
        {1.01f, -0.5f, -0.5f, 2.0f, 0.5f, 0.5f}, 1
    };
    const PlayerAabb boundary = {
        {1.0f, -0.5f, -0.5f, 2.0f, 0.5f, 0.5f}, 1
    };

    Runtime runtime = {};
    State state = {};
    Frame frame = make_frame(&outside, 1);
    check(init_runtime(&runtime, common, level, 1),
          "initialize source-backed one-shot Ambush runtime");
    init_state(&state);
    check(update(&runtime, &state, &frame) == STATUS_NO_CONTACT &&
          runtime.trigger_activations == 0 &&
          count_event(runtime, EVENT_TRIGGER_STARTED) == 0,
          "player AABB outside the trigger does not start Ambush");

    Runtime boundary_runtime = {};
    State boundary_state = {};
    frame = make_frame(&boundary, 1);
    check(init_runtime(&boundary_runtime, common, level, 2),
          "initialize boundary fixture runtime");
    init_state(&boundary_state);
    check(overlaps_closed(&frame.trigger_bounds, &boundary.bounds),
          "AABBs that meet at one face overlap inclusively");
    check(update(&boundary_runtime, &boundary_state, &frame) == STATUS_ACTIVATED &&
          boundary_runtime.trigger_activations == 1 &&
          count_event(boundary_runtime, EVENT_TRIGGER_STARTED) == 1,
          "inclusive boundary contact requests Ambush once");

    check(update(&boundary_runtime, &boundary_state, &frame) ==
              STATUS_SUSTAINED_CONTACT &&
          boundary_runtime.trigger_activations == 1 &&
          count_event(boundary_runtime, EVENT_TRIGGER_STARTED) == 1,
          "sustained overlap does not create a second rising edge");

    Runtime reentry_runtime = {};
    State reentry_state = {};
    check(init_runtime(&reentry_runtime, common, level, 1),
          "initialize leave/re-enter fixture runtime");
    init_state(&reentry_state);
    frame = make_frame(&boundary, 1);
    check(update(&reentry_runtime, &reentry_state, &frame) == STATUS_ACTIVATED,
          "leave/re-enter fixture activates on its first contact");
    frame = make_frame(&outside, 1);
    check(update(&reentry_runtime, &reentry_state, &frame) ==
              STATUS_BLOCKED_ACTIVATION_COUNT && reentry_state.qualifying_contact,
          "native count gate returns before contact sampling after count=1");
    frame = make_frame(&boundary, 1);
    check(update(&reentry_runtime, &reentry_state, &frame) ==
              STATUS_BLOCKED_ACTIVATION_COUNT &&
          reentry_runtime.trigger_activations == 1 &&
          count_event(reentry_runtime, EVENT_TRIGGER_STARTED) == 1,
          "re-entry remains blocked and cannot start Ambush a second time");

    Runtime running_runtime = {};
    State running_state = {};
    check(init_runtime(&running_runtime, common, level, 2),
          "initialize already-running fixture runtime");
    check(enter_trigger(&running_runtime),
          "seed an active Ambush script from an independent contact");
    init_state(&running_state);
    frame = make_frame(&boundary, 1);
    check(update(&running_runtime, &running_state, &frame) ==
              STATUS_SCRIPT_ALREADY_RUNNING &&
          running_runtime.trigger_activations == 2 &&
          count_event(running_runtime, EVENT_TRIGGER_SCRIPT_ALREADY_RUNNING) == 1 &&
          count_event(running_runtime, EVENT_SCRIPT_STARTED) == 1,
          "running Ambush suppresses duplicate start but still consumes trigger count");

    Runtime two_player_runtime = {};
    State two_player_state = {};
    const PlayerAabb players[2] = {outside, boundary};
    frame = make_frame(players, 2);
    check(init_runtime(&two_player_runtime, common, level, 1),
          "initialize two-player ordinary-script fixture");
    init_state(&two_player_state);
    check(update(&two_player_runtime, &two_player_state, &frame) ==
              STATUS_ACTIVATED && two_player_runtime.trigger_activations == 1,
          "ordinary Ambush script qualifies when only the second of two players touches");
}

static void test_gates(const dh2_script_table *common,
                       const dh2_script_table *level) {
    const PlayerAabb touching = {
        {0.0f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f}, 1
    };
    Runtime runtime = {};
    State state = {};
    Frame frame = make_frame(&touching, 1);

    check(init_runtime(&runtime, common, level, 1), "initialize gate fixture");
    init_state(&state);
    frame.local_player_marked_scripted = 1;
    check(update(&runtime, &state, &frame) == STATUS_BLOCKED_SCRIPTED_PLAYER &&
          !state.qualifying_contact && runtime.trigger_activations == 0,
          "native scripted-player gate prevents contact sampling");
    frame.local_player_marked_scripted = 0;
    frame.enabled = 0;
    check(update(&runtime, &state, &frame) == STATUS_BLOCKED_DISABLED,
          "native enabled flag gate blocks a disabled trigger");
    frame.enabled = 1;
    frame.delay_timer_ms = 1;
    check(update(&runtime, &state, &frame) == STATUS_BLOCKED_DELAY,
          "positive native delay timer blocks activation");
    frame.delay_timer_ms = 0;
    frame.has_associated_door = 1;
    frame.associated_door_state = 3;
    check(update(&runtime, &state, &frame) == STATUS_BLOCKED_DOOR,
          "associated door state 3 blocks activation");
    frame.associated_door_state = 1;
    check(update(&runtime, &state, &frame) == STATUS_BLOCKED_DOOR,
          "associated door state 1 blocks activation");
    frame.associated_door_state = 2;
    check(update(&runtime, &state, &frame) == STATUS_ACTIVATED,
          "other associated door states pass the recovered door gate");

    Runtime online_runtime = {};
    State online_state = {};
    frame = make_frame(&touching, 1);
    frame.online = 1;
    check(init_runtime(&online_runtime, common, level, 1),
          "initialize online-context fixture");
    init_state(&online_state);
    check(update(&online_runtime, &online_state, &frame) ==
              STATUS_UNSUPPORTED_ONLINE && online_runtime.trigger_activations == 0,
          "online path is rejected explicitly because its virtual gate is unresolved");
}

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "usage: %s COMMON_NAMES COMMON_PROGRAMS "
                "LEVEL_NAMES LEVEL_PROGRAMS\n", argv[0]);
        return 2;
    }

    const SourceMetadata &metadata = INFECTED_VILLAGE_AMBUSH;
    check(strcmp(metadata.level_name, "005_infectedvillage") == 0 &&
          strcmp(metadata.trigger_name, "_prim_TriggerZone_ambush") == 0 &&
          strcmp(metadata.script_name, "Ambush") == 0,
          "adapter identifies the cache-backed Infected Village trigger");
    check(metadata.module_position[0] == -3448.5f &&
          metadata.world_position[0] == 2545.3f &&
          metadata.world_position[1] == 1110.83f &&
          metadata.world_position[2] == 1313.39f,
          "source metadata includes MLX plus MGP translation");
    check(metadata.scale[0] == 1.92682f && metadata.scale[1] == 1.92682f &&
          metadata.scale[2] == 1.0f && metadata.activation_count == 1 &&
          metadata.delay_ms == 0 && metadata.zone_dimensions_are_inherited,
          "authored scale and inherited dimensions are marked without replacing caller AABBs");
    check(metadata.zone_dimensions[0] == 200.0f &&
          metadata.zone_dimensions[1] == 200.0f &&
          metadata.zone_dimensions[2] == 200.0f,
          "native Zone default dimensions are carried into the bounds adapter");
    Aabb approximate_bounds = {};
    check(make_approximate_trigger_bounds(&approximate_bounds) &&
          close(approximate_bounds.min_x, 2352.618f) &&
          close(approximate_bounds.min_y, 918.148f) &&
          close(approximate_bounds.min_z, 1213.39f) &&
          close(approximate_bounds.max_x, 2737.982f) &&
          close(approximate_bounds.max_y, 1303.512f) &&
          close(approximate_bounds.max_z, 1413.39f),
          "world-space proxy consumes Zone::InitPost local AABB math then applies translation");

    dh2_script_table common = {};
    dh2_script_table level = {};
    const bool common_ok = load_table(argv[1], argv[2], &common, "common");
    const bool level_ok = load_table(argv[3], argv[4], &level, "Infected Village");
    check(common_ok && level_ok, "decode the original common and level script tables");
    if (common_ok && level_ok) {
        check(dh2_script_resolve_id(&common, &level,
                  (const uint8_t *)"Ambush", 6, 0) ==
                  (int32_t)common.script_count,
              "Ambush resolves to the first Infected Village script");
        test_contact_cases(&common, &level);
        test_gates(&common, &level);
    }

    dh2_script_table_destroy(&common);
    dh2_script_table_destroy(&level);
    if (failures != 0) {
        fprintf(stderr, "%d trigger-contact check(s) failed\n", failures);
        return 1;
    }
    puts("verified Infected Village Ambush trigger contact adapter");
    return 0;
}
