#include "../ambush_vertical_slice.hpp"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {

bool require(bool condition, const char *message) {
    if (condition) return true;
    std::fprintf(stderr, "FAIL: %s\n", message);
    return false;
}

bool close(float actual, float expected) {
    return std::isfinite(actual) && std::fabs(actual - expected) <= 0.001f;
}

bool read_bytes(const std::string &path, std::vector<uint8_t> *output) {
    if (output == nullptr) return false;
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;
    output->assign(std::istreambuf_iterator<char>(file),
                   std::istreambuf_iterator<char>());
    return file.good() || file.eof();
}

bool import_level(const std::string &cache, dh2::world::SourceLevel *level) {
    using namespace dh2::world;
    std::vector<uint8_t> bytes;
    Diagnostic diagnostic{};
    const char *level_path = "data/scene/005_infectedvillage.mlx";
    if (!read_bytes(cache + "/" + level_path, &bytes)) return false;
    if (dh2_world_import_level(level, "INFECTED_VILLAGE_01", level_path,
            bytes.data(), bytes.size(), &diagnostic) != Error::ok) {
        std::fprintf(stderr, "MLX import failed: %s\n", diagnostic.message);
        return false;
    }
    for (uint32_t i = 0; i < level->module_count; ++i) {
        std::vector<uint8_t> mgp;
        const char *path = level->modules[i].cache_mgp;
        if (path == nullptr || !read_bytes(cache + "/" + path, &mgp)) return false;
        if (dh2_world_import_module_objects(level, i, RecordKind::mgp, path,
                mgp.data(), mgp.size(), &diagnostic) != Error::ok) {
            std::fprintf(stderr, "MGP import failed for %s: %s\n", path,
                         diagnostic.message);
            return false;
        }
    }
    return true;
}

bool read_script_table(const char *names_path, const char *programs_path,
                       dh2_script_table *table) {
    std::vector<uint8_t> names, programs;
    if (!read_bytes(names_path, &names) || !read_bytes(programs_path, &programs))
        return false;
    dh2_script_error error{};
    const auto result = dh2_script_table_decode(names.data(),
        static_cast<uint32_t>(names.size()), programs.data(),
        static_cast<uint32_t>(programs.size()), table, &error);
    if (result != DH2_SCRIPT_OK) {
        std::fprintf(stderr, "script decode failed: %s input=%u offset=%u\n",
                     dh2_script_error_name(result),
                     static_cast<unsigned>(error.input_kind),
                     static_cast<unsigned>(error.offset));
        return false;
    }
    return true;
}

const dh2::world::Object *find_entity(const dh2::world::SourceLevel &level,
                                      const char *name,
                                      uint32_t module_index) {
    const dh2::world::Object *found = nullptr;
    for (uint32_t i = 0; i < level.entity_count; ++i) {
        if (level.entities[i].module_index != module_index ||
            std::string(level.entities[i].name) != name) continue;
        if (found != nullptr) return nullptr;
        found = &level.entities[i];
    }
    return found;
}

bool check_source_trigger(const dh2::world::SourceLevel &level) {
    const auto *trigger = find_entity(level, "_prim_TriggerZone_ambush", 0);
    if (!require(trigger != nullptr, "cache import contains one Ambush Zone")) return false;
    return require(trigger->module_index == 0 &&
                   std::string(trigger->gametype) == "TriggerZone" &&
                   std::string(dh2_world_field(trigger, "script")) == "Ambush" &&
                   std::string(dh2_world_field(trigger, "triggercount")) == "1" &&
                   std::string(dh2_world_field(trigger, "triggerdelay")) == "0" &&
                   close(trigger->local.position[0], 5993.8f) &&
                   close(trigger->local.position[1], -1889.17f) &&
                   close(trigger->local.position[2], 1313.39f) &&
                   close(trigger->local.rotation_degrees[0], 0.0f) &&
                   close(trigger->local.rotation_degrees[1], 0.0f) &&
                   close(trigger->local.rotation_degrees[2], 0.0f) &&
                   close(trigger->local.scale[0], 1.92682f) &&
                   close(trigger->local.scale[1], 1.92682f) &&
                   close(trigger->local.scale[2], 1.0f) &&
                   close(trigger->world_position[0], 2545.3f) &&
                   close(trigger->world_position[1], 1110.83f) &&
                   close(trigger->world_position[2], 1313.39f),
                   "imported Ambush Zone fields and translated center match cache");
}

bool test_vertical_slice(const std::string &cache,
                         const std::string &common_names,
                         const std::string &common_programs,
                         const std::string &level_names,
                         const std::string &level_programs) {
    using namespace dh2::actors;
    using namespace dh2_script_runtime;
    using namespace dh2_trigger_contact;

    dh2::world::SourceLevel level{};
    Registry registry{};
    dh2_script_table common{}, infected{};
    Runtime runtime{};
    ProjectionState projection{};
    bool okay = import_level(cache, &level) &&
        read_script_table(common_names.c_str(), common_programs.c_str(), &common) &&
        read_script_table(level_names.c_str(), level_programs.c_str(), &infected);
    if (!okay) {
        require(false, "load cache world records and checked script tables");
    } else {
        okay = [&]() -> bool {
            if (!require(check_source_trigger(level), "validate original trigger record")) return false;
            if (!require(dh2::actors::init(&registry, &level) == dh2::actors::Error::ok &&
                         registry.actor_count == 26,
                         "import the existing 26-record Character registry")) return false;
            if (!require(dh2::actors::find(&registry, "_prim_tmp_infected17") == nullptr,
                         "unresolved infected17 has no source Character allocation")) return false;

            Aabb trigger_bounds{};
            if (!require(make_approximate_trigger_bounds(&trigger_bounds),
                         "derive approximate Zone bounds from inherited source dimensions")) return false;
            if (!require(close(trigger_bounds.min_x, 2352.618f) &&
                         close(trigger_bounds.min_y, 918.148f) &&
                         close(trigger_bounds.min_z, 1213.39f) &&
                         close(trigger_bounds.max_x, 2737.982f) &&
                         close(trigger_bounds.max_y, 1303.512f) &&
                         close(trigger_bounds.max_z, 1413.39f),
                         "Zone::InitPost local bounds translated into the documented approximate proxy")) return false;

            ObjectSeed seeds[MAX_OBJECTS]{};
            uint32_t seed_count = 0;
            if (!require(build_ambush_runtime_seeds(&registry, seeds, MAX_OBJECTS,
                                                    &seed_count) && seed_count == 26,
                         "seed checked script scheduler from source Character rows")) return false;
            Options options{};
            init_options(&options);
            options.local_player_has_character = 1;
            options.online = 0;
            if (!require(init(&runtime, &common, &infected, seeds, seed_count,
                              INFECTED_VILLAGE_AMBUSH.trigger_name,
                              INFECTED_VILLAGE_AMBUSH.script_name,
                              INFECTED_VILLAGE_AMBUSH.activation_count,
                              INFECTED_VILLAGE_AMBUSH.delay_ms, &options) == ERROR_OK,
                         "initialize Ambush with its authored one-shot count")) return false;

            const PlayerAabb outside = {
                {trigger_bounds.min_x - 10.0f, trigger_bounds.min_y + 1.0f,
                 trigger_bounds.min_z + 1.0f, trigger_bounds.min_x - 1.0f,
                 trigger_bounds.max_y - 1.0f, trigger_bounds.max_z - 1.0f}, 1
            };
            const PlayerAabb caller_inside = {
                {trigger_bounds.min_x + 1.0f, trigger_bounds.min_y + 1.0f,
                 trigger_bounds.min_z + 1.0f, trigger_bounds.max_x - 1.0f,
                 trigger_bounds.max_y - 1.0f, trigger_bounds.max_z - 1.0f}, 1
            };
            Frame frame{};
            frame.trigger_bounds = trigger_bounds;
            frame.enabled = 1;
            frame.players = &outside;
            frame.player_count = 1;
            State contact{};
            init_state(&contact);
            if (!require(update(&runtime, &contact, &frame) == STATUS_NO_CONTACT &&
                         runtime.trigger_activations == 0,
                         "outside caller-supplied Character AABB leaves Ambush inactive")) return false;

            frame.players = &caller_inside;
            if (!require(update(&runtime, &contact, &frame) == STATUS_ACTIVATED &&
                         runtime.trigger_activations == 1,
                         "outside-to-overlap transition starts the one-shot Ambush script")) return false;
            init_projection_state(&projection);
            if (!require(advance_ambush_and_project(&runtime, &registry, &projection, 0) ==
                             PROJECTION_OK,
                         "advance checked Ambush and project its requests")) return false;

            {
                const char *names[MAX_AMBUSH_PROJECTED_REQUESTS] = {
                    "_prim_tmp_infected05", "_prim_tmp_infected07", "_prim_tmp_infected17",
                    "_prim_tmp_infected06", "_prim_tmp_infected16"
                };
                const SpawnResult results[MAX_AMBUSH_PROJECTED_REQUESTS] = {
                    SpawnResult::requested, SpawnResult::requested, SpawnResult::lookup_miss,
                    SpawnResult::requested, SpawnResult::requested
                };
                bool exact = projection.request_count == MAX_AMBUSH_PROJECTED_REQUESTS;
                for (uint32_t i = 0; exact && i < MAX_AMBUSH_PROJECTED_REQUESTS; ++i) {
                    const uint32_t event_index = projection.requests[i].script_event_index;
                    const EventType expected_type = results[i] == SpawnResult::lookup_miss
                        ? EVENT_OBJECT_LOOKUP_MISS : EVENT_CHARACTER_SPAWN_STATE_REQUESTED;
                    exact = event_index < runtime.event_count &&
                            std::string(projection.requests[i].actor_name) == names[i] &&
                            projection.requests[i].result == results[i] &&
                            runtime.events[event_index].type == expected_type &&
                            runtime.events[event_index].command_id == 30 &&
                            runtime.events[event_index].detail == std::string(names[i]);
                }
                if (!require(exact, "five scheduler requests retain exact source order and results")) return false;
            }
            if (!require(registry.actor_count == 26 && registry.request_count == 5 &&
                         registry.lookup_miss_count == 1 &&
                         std::string(registry.last_lookup_miss) == "_prim_tmp_infected17" &&
                         dh2::actors::find(&registry, "_prim_tmp_infected17") == nullptr,
                         "missing infected17 stays an unresolved registry miss")) return false;

            {
                const char *present[] = {"_prim_tmp_infected05", "_prim_tmp_infected07",
                                         "_prim_tmp_infected06", "_prim_tmp_infected16"};
                for (const char *name : present) {
                    const ActorInstance *actor = dh2::actors::find(&registry, name);
                    if (!require(actor != nullptr &&
                                 actor->lifecycle == Lifecycle::spawn_requested &&
                                 actor->spawn_transition_count == 1,
                                 "source Character receives one registry spawn-state request"))
                        return false;
                }
            }

            if (!require(level.entity_count > 0 && registry.actor_count == 26 &&
                         runtime.object_count == seed_count,
                         "projection changes no source record count and allocates no actor/render object")) return false;

            /* Count=1 is authored on this Zone and TriggerZone rejects before
             * contact sampling after activation. Re-entry after an exit is
             * not asserted as native behavior. */
            frame.players = &outside;
            if (!require(update(&runtime, &contact, &frame) == STATUS_BLOCKED_ACTIVATION_COUNT,
                         "authored activation count blocks later outside frames")) return false;
            frame.players = &caller_inside;
            if (!require(update(&runtime, &contact, &frame) == STATUS_BLOCKED_ACTIVATION_COUNT &&
                         runtime.trigger_activations == 1 &&
                         projection.request_count == MAX_AMBUSH_PROJECTED_REQUESTS,
                         "authored one-shot gate prevents a second activation or duplicate requests")) return false;
            return true;
        }();
    }

    dh2_script_table_destroy(&common);
    dh2_script_table_destroy(&infected);
    dh2::actors::destroy(&registry);
    dh2_world_free(&level);
    return okay;
}

}  // namespace

int main(int argc, char **argv) {
    if (argc != 6) {
        std::fprintf(stderr, "usage: %s CACHE COMMON_NAMES COMMON_PROGRAMS "
                     "LEVEL_NAMES LEVEL_PROGRAMS\n", argv[0]);
        return 2;
    }
    const bool okay = test_vertical_slice(argv[1], argv[2], argv[3], argv[4], argv[5]);
    if (!okay) return 1;
    std::puts("verified cache-backed Ambush trigger-to-actor host slice");
    return 0;
}
