#include "../trigger-contact/ambush_vertical_slice.hpp"

#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {

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
    const char *path = "data/scene/005_infectedvillage.mlx";
    std::vector<uint8_t> bytes;
    Diagnostic diagnostic{};
    if (!read_bytes(cache + "/" + path, &bytes)) return false;
    if (dh2_world_import_level(level, "INFECTED_VILLAGE_01", path,
            bytes.data(), bytes.size(), &diagnostic) != Error::ok) {
        std::fprintf(stderr, "MLX import failed: %s\n", diagnostic.message);
        return false;
    }
    for (uint32_t i = 0; i < level->module_count; ++i) {
        const char *mgp_path = level->modules[i].cache_mgp;
        std::vector<uint8_t> mgp;
        if (mgp_path == nullptr || !read_bytes(cache + "/" + mgp_path, &mgp))
            return false;
        if (dh2_world_import_module_objects(level, i, RecordKind::mgp,
                mgp_path, mgp.data(), mgp.size(), &diagnostic) != Error::ok) {
            std::fprintf(stderr, "MGP import failed for %s: %s\n", mgp_path,
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
    if (dh2_script_table_decode(names.data(), static_cast<uint32_t>(names.size()),
            programs.data(), static_cast<uint32_t>(programs.size()), table,
            &error) != DH2_SCRIPT_OK) {
        std::fprintf(stderr, "script decode failed: %s at %u\n",
                     dh2_script_error_name(error.code),
                     static_cast<unsigned>(error.offset));
        return false;
    }
    return true;
}

std::string json_string(const char *value) {
    std::string result("\"");
    if (value != nullptr) {
        for (const unsigned char c : std::string(value)) {
            if (c == '"' || c == '\\') {
                result.push_back('\\');
                result.push_back(static_cast<char>(c));
            } else if (c < 0x20) {
                char escaped[7];
                std::snprintf(escaped, sizeof(escaped), "\\u%04x", c);
                result += escaped;
            } else {
                result.push_back(static_cast<char>(c));
            }
        }
    }
    result.push_back('"');
    return result;
}

const char *event_type_name(dh2_script_runtime::EventType type) {
    using namespace dh2_script_runtime;
    switch (type) {
        case EVENT_CHARACTER_SPAWN_STATE_REQUESTED:
            return "character_spawn_state_requested";
        case EVENT_OBJECT_LOOKUP_MISS:
            return "object_lookup_miss";
        default:
            return "unexpected_event";
    }
}

bool emit_projection(const std::string &cache, const std::string &common_names,
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
        std::fprintf(stderr, "could not load cache-backed level and script tables\n");
    } else if (dh2::actors::init(&registry, &level) != dh2::actors::Error::ok ||
               registry.actor_count != 26) {
        std::fprintf(stderr, "expected 26 imported source Character records\n");
        okay = false;
    }

    ObjectSeed seeds[MAX_OBJECTS]{};
    uint32_t seed_count = 0;
    Options options{};
    init_options(&options);
    options.local_player_has_character = 1;
    options.online = 0;
    if (okay && (!build_ambush_runtime_seeds(&registry, seeds, MAX_OBJECTS,
                                             &seed_count) || seed_count != 26 ||
                 init(&runtime, &common, &infected, seeds, seed_count,
                      INFECTED_VILLAGE_AMBUSH.trigger_name,
                      INFECTED_VILLAGE_AMBUSH.script_name,
                      INFECTED_VILLAGE_AMBUSH.activation_count,
                      INFECTED_VILLAGE_AMBUSH.delay_ms, &options) != ERROR_OK)) {
        std::fprintf(stderr, "could not initialize cache-backed Ambush runtime\n");
        okay = false;
    }

    if (okay && find(&registry, "_prim_tmp_infected17") != nullptr) {
        std::fprintf(stderr, "unexpected static Character for infected17\n");
        okay = false;
    }
    Aabb trigger_bounds{};
    if (okay && !make_approximate_trigger_bounds(&trigger_bounds)) {
        std::fprintf(stderr, "could not derive Ambush trigger proxy bounds\n");
        okay = false;
    }
    if (okay) {
        const PlayerAabb outside = {
            {trigger_bounds.min_x - 10.0f, trigger_bounds.min_y + 1.0f,
             trigger_bounds.min_z + 1.0f, trigger_bounds.min_x - 1.0f,
             trigger_bounds.max_y - 1.0f, trigger_bounds.max_z - 1.0f}, 1
        };
        const PlayerAabb inside = {
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
        const Status outside_status = update(&runtime, &contact, &frame);
        frame.players = &inside;
        const Status inside_status = update(&runtime, &contact, &frame);
        init_projection_state(&projection);
        if (outside_status != STATUS_NO_CONTACT || inside_status != STATUS_ACTIVATED ||
            advance_ambush_and_project(&runtime, &registry, &projection, 0) !=
                PROJECTION_OK || projection.request_count != 5 ||
            registry.request_count != 5 || registry.lookup_miss_count != 1 ||
            std::string(registry.last_lookup_miss) != "_prim_tmp_infected17") {
            std::fprintf(stderr, "Ambush did not yield its five expected registry requests\n");
            okay = false;
        }
    }

    if (okay) {
        std::puts("{\"source_actor_count\":26,\"runtime_object_count\":26,"
                  "\"activation_status\":\"activated\",\"request_count\":5,");
        std::puts("\"loaded_character_names\":[");
        for (uint32_t i = 0; i < registry.actor_count; ++i) {
            if (i != 0) std::puts(",");
            std::printf("%s", json_string(registry.actors[i].name).c_str());
        }
        std::puts("],\"requests\":[");
        for (uint32_t i = 0; i < projection.request_count; ++i) {
            const ProjectedSpawnRequest &request = projection.requests[i];
            const uint32_t event_index = request.script_event_index;
            if (event_index >= runtime.event_count || request.result == SpawnResult::invalid_request) {
                std::fprintf(stderr, "invalid projected request record\n");
                okay = false;
                break;
            }
            const Event &event = runtime.events[event_index];
            const ActorInstance *actor = find(&registry, request.actor_name);
            if ((request.result == SpawnResult::lookup_miss) != (actor == nullptr)) {
                std::fprintf(stderr, "request and source registry disagree for %s\n",
                             request.actor_name);
                okay = false;
                break;
            }
            if (i != 0) std::puts(",");
            std::printf("{\"name\":%s,\"registry_result\":%s,\"event_type\":%s,"
                        "\"event_command_id\":%u,\"source_actor\":",
                        json_string(request.actor_name).c_str(),
                        json_string(dh2::actors::spawn_result_name(request.result)).c_str(),
                        json_string(event_type_name(event.type)).c_str(), event.command_id);
            if (actor == nullptr) {
                std::printf("null");
            } else {
                std::printf("{\"module\":%s,\"source_record\":%u,\"template\":%s,"
                            "\"template_data_class\":%s,\"editor_template_name\":%s,"
                            "\"ai_state\":%s,\"auto_spawn\":%u,\"lifecycle\":%s}",
                    json_string(actor->module_name).c_str(), actor->source_record,
                    json_string(actor->character_template).c_str(),
                    json_string(actor->template_data_class).c_str(),
                    json_string(actor->editor_template_name).c_str(),
                    json_string(actor->ai_state).c_str(), actor->auto_spawn,
                    json_string(dh2::actors::lifecycle_name(actor->lifecycle)).c_str());
            }
            std::printf("}");
        }
        std::puts("],\"registry_lookup_misses\":1,");
        std::printf("\"last_lookup_miss\":%s,\"missing_name_still_unregistered\":%s,"
                    "\"actors_created\":0,\"render_objects_created\":0}\n",
                    json_string(registry.last_lookup_miss).c_str(),
                    find(&registry, "_prim_tmp_infected17") == nullptr ? "true" : "false");
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
        std::fprintf(stderr, "usage: %s CACHE COMMON_NAMES COMMON_PROGRAMS LEVEL_NAMES LEVEL_PROGRAMS\n", argv[0]);
        return 2;
    }
    const bool success = emit_projection(argv[1], argv[2], argv[3], argv[4], argv[5]);
    if (!success) return 1;
    return 0;
}
