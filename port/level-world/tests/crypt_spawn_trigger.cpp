#include "../crypt_spawn_trigger.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace dh2_crypt_spawn_trigger;
using namespace dh2_script_runtime;

namespace {

int failures = 0;

void check(bool condition, const char *label) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", label);
        ++failures;
    }
}

bool close(float actual, float expected, float epsilon = 0.01f) {
    return std::isfinite(actual) && std::fabs(actual - expected) <= epsilon;
}

uint8_t *read_file(const char *path, uint32_t *size) {
    FILE *file = std::fopen(path, "rb");
    long length;
    uint8_t *bytes;
    if (file == NULL || std::fseek(file, 0, SEEK_END) != 0 ||
        (length = std::ftell(file)) < 0 ||
        static_cast<unsigned long>(length) > 0xfffffffful ||
        std::fseek(file, 0, SEEK_SET) != 0) {
        if (file != NULL) std::fclose(file);
        return NULL;
    }
    bytes = static_cast<uint8_t *>(std::malloc(
        length == 0 ? 1u : static_cast<size_t>(length)));
    if (bytes == NULL || std::fread(bytes, 1, static_cast<size_t>(length), file) !=
        static_cast<size_t>(length)) {
        std::free(bytes);
        std::fclose(file);
        return NULL;
    }
    std::fclose(file);
    *size = static_cast<uint32_t>(length);
    return bytes;
}

bool load_table(const char *names_path, const char *programs_path,
                dh2_script_table *table, const char *label) {
    uint32_t names_size = 0;
    uint32_t programs_size = 0;
    uint8_t *names = read_file(names_path, &names_size);
    uint8_t *programs = read_file(programs_path, &programs_size);
    dh2_script_error error = {};
    if (names == NULL || programs == NULL) {
        std::fprintf(stderr, "cannot read %s PyData table\n", label);
        std::free(names);
        std::free(programs);
        return false;
    }
    const dh2_script_error_code result = dh2_script_table_decode(
        names, names_size, programs, programs_size, table, &error);
    std::free(names);
    std::free(programs);
    if (result != DH2_SCRIPT_OK) {
        std::fprintf(stderr, "%s PyData decode failed: %s input=%u offset=%u\n",
                     label, dh2_script_error_name(result),
                     static_cast<unsigned>(error.input_kind),
                     static_cast<unsigned>(error.offset));
        return false;
    }
    return true;
}

uint32_t count_event(const Runtime &runtime, EventType type) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < runtime.event_count; ++i) {
        if (runtime.events[i].type == type) ++count;
    }
    return count;
}

Frame make_frame(const dh2_zone_contact::Vec3 &world_position,
                 const dh2_zone_contact::Vec3 &scale,
                 const PlayerAabb *players, uint32_t player_count) {
    Frame frame = {};
    frame.owner_world_position = world_position;
    frame.owner_scale = scale;
    frame.players = players;
    frame.player_count = player_count;
    frame.enabled = 1;
    return frame;
}

void test_bounds() {
    const dh2_zone_contact::Vec3 world_position = {
        -1405.23f, 19500.262f, 608.062f
    };
    const dh2_zone_contact::Vec3 scale = {-1.0f, 0.0f, 0.0f};
    Aabb bounds = {};
    check(make_world_bounds(&GHOST_AMBUSH_01, &world_position,
                            &GHOST_AMBUSH_01.authored_object_scale, &bounds),
          "build exact absolute bounds from Zone dimensions, scale, and owner position");
    check(close(bounds.min_x, -1911.319f) && close(bounds.max_x, -899.141f) &&
          close(bounds.min_y, 19356.985f) && close(bounds.max_y, 19643.539f) &&
          close(bounds.min_z, 508.062f) && close(bounds.max_z, 708.062f),
          "Crypt trigger absolute AABB keeps scaled half extents and translated position");
    check(!make_world_bounds(&GHOST_AMBUSH_01, &world_position, &scale, &bounds),
          "negative resolved scale is rejected instead of reversing AABB faces");
}

void test_runtime(const dh2_script_table *common,
                  const dh2_script_table *level) {
    Options options = {};
    init_options(&options);
    options.local_player_has_character = 1;
    Runtime runtime = {};
    State state = {};
    dh2_crypt_spawn_trigger::init_state(&state);
    check(init(&runtime, common, level, NULL, 0,
               GHOST_AMBUSH_01.trigger_name, GHOST_AMBUSH_01.script_name,
               GHOST_AMBUSH_01.activation_limit,
               GHOST_AMBUSH_01.configured_delay_ms, &options) == ERROR_OK,
          "initialize runtime with original GhostAmbush01 table and trigger");
    check(runtime.trigger_script_id == 17,
          "GhostAmbush01 resolves to original global ID 17 (local 2)");

    /* x07_crypt_backup.mlx places its authored final-room module at
     * (0,19200,0), with unit scale and zero rotation. The corresponding MGP
     * TriggerZone local position is added by the fixture validator. */
    const dh2_zone_contact::Vec3 owner_world_position = {
        -1405.23f, 19500.262f, 608.062f
    };
    const dh2_zone_contact::Vec3 owner_scale = {5.06089f, 1.43277f, 1.0f};
    const Aabb ghost_zone = {
        -1911.319f, 19356.985f, 508.062f,
        -899.141f, 19643.539f, 708.062f
    };
    const PlayerAabb outside = {
        {ghost_zone.max_x + 0.01f, 19400.0f, 550.0f,
         ghost_zone.max_x + 20.0f, 19420.0f, 600.0f}, 1
    };
    PlayerAabb player = outside;
    Frame frame = make_frame(owner_world_position, owner_scale, &player, 1);
    check(update(&runtime, &state, &frame) ==
              dh2_trigger_contact::STATUS_NO_CONTACT &&
          runtime.trigger_activations == 0 &&
          count_event(runtime, EVENT_TRIGGER_STARTED) == 0,
          "outside player does not start GhostAmbush01");

    /* A null Character entry is skipped by GetNumPlayerTouching even if the
     * caller's stale bounds happen to overlap. */
    player.has_character = 0;
    player.bounds = {-1500.0f, 19400.0f, 550.0f,
                     -1490.0f, 19410.0f, 560.0f};
    check(update(&runtime, &state, &frame) ==
              dh2_trigger_contact::STATUS_NO_CONTACT,
          "null Character entry does not count as contact");

    /* TriggerZone's actual path uses GameObject::IsTouching on updated
     * absolute AABBs. It does not invoke Zone::IsInside/PhysicalObject. */
    player.has_character = 1;
    player.bounds = {-1500.0f, 19400.0f, 550.0f,
                     -1490.0f, 19410.0f, 560.0f};
    check(update(&runtime, &state, &frame) ==
              dh2_trigger_contact::STATUS_ACTIVATED &&
          runtime.trigger_activations == 1 &&
          count_event(runtime, EVENT_TRIGGER_STARTED) == 1,
          "first character AABB contact activates the exact Crypt source script");
    check(update(&runtime, &state, &frame) ==
              dh2_trigger_contact::STATUS_BLOCKED_ACTIVATION_COUNT &&
          runtime.trigger_activations == 1 &&
          count_event(runtime, EVENT_TRIGGER_STARTED) == 1,
          "one-shot count gate preempts further contact sampling after activation");

    player.bounds = {ghost_zone.max_x, 19400.0f, 550.0f,
                     ghost_zone.max_x + 5.0f, 19410.0f, 560.0f};
    check(update(&runtime, &state, &frame) ==
              dh2_trigger_contact::STATUS_BLOCKED_ACTIVATION_COUNT &&
          runtime.trigger_activations == 1 && state.qualifying_contact,
          "one-shot native activation count preserves the edge latch before contact sampling");

    dh2_zone_contact::PhysicalObjectView no_physical_object = {false, 0.0f};
    dh2_zone_contact::Aabb zone_bounds = {
        {ghost_zone.min_x, ghost_zone.min_y, ghost_zone.min_z},
        {ghost_zone.max_x, ghost_zone.max_y, ghost_zone.max_z}
    };
    dh2_zone_contact::Vec3 actor_position = {
        owner_world_position.x, owner_world_position.y, owner_world_position.z
    };
    const dh2_zone_contact::ContactResult zone_result =
        dh2_zone_contact::project_is_inside(
            &no_physical_object, false, false, &zone_bounds,
            &owner_world_position, &actor_position, NULL, NULL);
    check(!zone_result.inside &&
          zone_result.path == dh2_zone_contact::kNoPhysicalObject,
          "separate Zone::IsInside projection still rejects absent PhysicalObject");
}

}  // namespace

int main(int argc, char **argv) {
    dh2_script_table common = {};
    dh2_script_table crypt = {};
    if (argc != 5) {
        std::fprintf(stderr,
                     "usage: %s common.names common.programs crypt.names crypt.programs\n",
                     argv[0]);
        return 2;
    }
    check(load_table(argv[1], argv[2], &common, "common"),
          "decode original common script table");
    check(load_table(argv[3], argv[4], &crypt, "007_crypt_01"),
          "decode original Crypt level script table");
    if (failures == 0) {
        test_bounds();
        test_runtime(&common, &crypt);
    }
    dh2_script_table_destroy(&common);
    dh2_script_table_destroy(&crypt);
    if (failures != 0) {
        std::fprintf(stderr, "%d Crypt trigger assertion(s) failed\n", failures);
        return 1;
    }
    std::puts("Crypt GhostAmbush01 trigger source/runtime checks passed");
    return 0;
}
