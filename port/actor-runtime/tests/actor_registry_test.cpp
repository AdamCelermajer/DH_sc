#include "../actor_registry.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {

bool require(bool condition, const char* message) {
    if (condition) return true;
    std::fprintf(stderr, "FAIL: %s\n", message);
    return false;
}

bool close(float actual, float expected) {
    return std::isfinite(actual) && std::fabs(actual - expected) <= 0.002f;
}

bool read_bytes(const std::string& cache, const char* relative,
                std::vector<std::uint8_t>* output) {
    if (!output) return false;
    std::ifstream file(cache + "/" + relative, std::ios::binary);
    if (!file) return false;
    output->assign(std::istreambuf_iterator<char>(file),
                   std::istreambuf_iterator<char>());
    return file.good() || file.eof();
}

bool import_infected_village(const std::string& cache,
                             dh2::world::SourceLevel* level) {
    std::vector<std::uint8_t> bytes;
    dh2::world::Diagnostic diagnostic{};
    if (!read_bytes(cache, "data/scene/005_infectedvillage.mlx", &bytes)) {
        std::fprintf(stderr, "cannot read Infected Village MLX under %s\n", cache.c_str());
        return false;
    }
    auto result = dh2_world_import_level(level, "INFECTED_VILLAGE_01",
        "data/scene/005_infectedvillage.mlx", bytes.data(), bytes.size(), &diagnostic);
    if (result != dh2::world::Error::ok) {
        std::fprintf(stderr, "MLX import failed: %s\n", diagnostic.message);
        return false;
    }
    if (level->module_count != 2) {
        std::fprintf(stderr, "expected two modules, got %u\n", level->module_count);
        return false;
    }
    for (std::uint32_t i = 0; i < level->module_count; ++i) {
        std::vector<std::uint8_t> mgp;
        const char* path = level->modules[i].cache_mgp;
        if (!path || !read_bytes(cache, path, &mgp)) {
            std::fprintf(stderr, "cannot read module %u MGP %s\n", i,
                         path ? path : "<missing path>");
            return false;
        }
        result = dh2_world_import_module_objects(level, i,
            dh2::world::RecordKind::mgp, path, mgp.data(), mgp.size(), &diagnostic);
        if (result != dh2::world::Error::ok) {
            std::fprintf(stderr, "MGP import failed for %s: %s\n", path,
                         diagnostic.message);
            return false;
        }
    }
    return true;
}

struct ExpectedActor {
    const char* name;
    std::uint32_t source_record;
    float position[3];
    float rotation_z;
};

const ExpectedActor expected_ambush[] = {
    {"_prim_tmp_infected05", 6, {2171.37f, 1283.11f, 1325.43f}, 78.9326f},
    {"_prim_tmp_infected07", 8, {2153.85f, 991.88f, 1325.43f}, 96.4741f},
    {"_prim_tmp_infected06", 7, {2871.59f, 983.05f, 1325.43f}, -101.756f},
    {"_prim_tmp_infected16", 19, {2562.42f, 651.98f, 1325.43f}, 174.741f}
};

bool verify_imported_actors(const dh2::world::SourceLevel& level,
                            const dh2::actors::Registry& registry) {
    using namespace dh2::actors;
    if (!require(registry.actor_count == 26, "cache Character record count changed")) return false;
    std::uint32_t translation_checks = 0;
    for (std::uint32_t i = 0; i < registry.actor_count; ++i) {
        const auto& actor = registry.actors[i];
        const auto& module = level.modules[actor.module_index];
        for (int axis = 0; axis < 3; ++axis) {
            if (!require(close(actor.world_position[axis],
                               actor.local_position[axis] + module.record.world_position[axis]),
                         "world translation does not equal local position plus module origin"))
                return false;
        }
        ++translation_checks;
    }
    const char* template_class = "Charater_Templates";
    for (const auto& expected : expected_ambush) {
        const auto* actor = find(&registry, expected.name);
        if (!require(actor != nullptr, "confirmed Ambush Character missing")) return false;
        if (!require(actor->source_record == expected.source_record,
                     "Ambush source record index changed") ||
            !require(actor->module_index == 0 &&
                     std::string(actor->level_name) == "INFECTED_VILLAGE_01" &&
                     std::string(actor->module_name) == "_module_infectedvillage_01_001" &&
                     std::string(actor->source_path) ==
                         "data/3d/modules/infectedvillage/mgp/infected01.mgp" &&
                     actor->source_end > actor->source_begin,
                     "owned source identity changed") ||
            !require(actor->has_character_template &&
                     std::string(actor->character_template) == "InfectedVillage_CommonType1",
                     "Character template field changed") ||
            !require(actor->has_template_data_class &&
                     std::string(actor->template_data_class) == template_class,
                     "template data class changed") ||
            !require(actor->has_editor_template_name &&
                     std::string(actor->editor_template_name) == "MonsterCommonType1",
                     "editor template name changed") ||
            !require(actor->has_ai_state && std::string(actor->ai_state) == "Limbus",
                     "initial AI state changed") ||
            !require(actor->has_auto_spawn && !actor->auto_spawn,
                     "authored auto_spawn=0 not preserved") ||
            !require(close(actor->world_position[0], expected.position[0]) &&
                     close(actor->world_position[1], expected.position[1]) &&
                     close(actor->world_position[2], expected.position[2]),
                     "Ambush actor world position changed") ||
            !require(close(actor->local_rotation_degrees[2], expected.rotation_z),
                     "Ambush actor local rotation changed")) return false;
    }
    if (translation_checks != registry.actor_count) return false;
    return true;
}

bool registry_transactions_and_requests(const dh2::world::SourceLevel& level,
                                        dh2::actors::Registry* registry) {
    using namespace dh2::actors;
    if (!require(init(registry, &level) == Error::ok, "registry init failed")) return false;

    const char* request_order[] = {
        "_prim_tmp_infected05", "_prim_tmp_infected07", "_prim_tmp_infected17",
        "_prim_tmp_infected06", "_prim_tmp_infected16"
    };
    const SpawnResult expected[] = {
        SpawnResult::requested, SpawnResult::requested, SpawnResult::lookup_miss,
        SpawnResult::requested, SpawnResult::requested
    };
    for (std::uint32_t i = 0; i < 5; ++i) {
        if (!require(request_spawn(registry, request_order[i]) == expected[i],
                     "Ambush SpawnCharacter lookup result changed")) return false;
    }
    const auto* infected05 = find(registry, "_prim_tmp_infected05");
    if (!require(infected05 && infected05->lifecycle == Lifecycle::spawn_requested &&
                 infected05->spawn_transition_count == 1,
                 "first request did not set one lifecycle projection")) return false;
    if (!require(request_spawn(registry, "_prim_tmp_infected05") ==
                 SpawnResult::already_requested,
                 "repeated spawn request is not idempotent")) return false;
    infected05 = find(registry, "_prim_tmp_infected05");
    if (!require(infected05 && infected05->spawn_transition_count == 1,
                 "repeated request incremented lifecycle transition")) return false;
    if (!require(registry->lookup_miss_count == 1 &&
                 std::string(registry->last_lookup_miss) == "_prim_tmp_infected17",
                 "missing Ambush name was not reported")) return false;

    // A successful replacement copies a fresh actor set and resets only the
    // port-owned lifecycle projection and request diagnostics.
    if (!require(init(registry, &level) == Error::ok,
                 "successful registry replacement failed")) return false;
    infected05 = find(registry, "_prim_tmp_infected05");
    if (!require(registry->actor_count == 26 && infected05 &&
                 infected05->lifecycle == Lifecycle::registered &&
                 infected05->spawn_transition_count == 0 &&
                 registry->request_count == 0 && registry->lookup_miss_count == 0,
                 "successful replacement did not reset owned lifecycle state")) return false;

    if (!require(request_spawn(registry, "_prim_tmp_infected05") ==
                 SpawnResult::requested,
                 "could not seed lifecycle state for failure transaction checks")) return false;

    const auto original_count = registry->actor_count;
    const std::string original_first_name = registry->actors[0].name;
    const dh2::world::Object* sample = nullptr;
    for (std::uint32_t i = 0; i < level.entity_count; ++i) {
        if (level.entities[i].gametype &&
            std::string(level.entities[i].gametype) == "Character") {
            sample = &level.entities[i];
            break;
        }
    }
    if (!require(sample != nullptr, "source fixture has no Character")) return false;

    // Duplicate names are rejected without destroying the previous registry.
    dh2::world::Object duplicate_objects[2] = {*sample, *sample};
    auto duplicate_level = level;
    duplicate_level.entities = duplicate_objects;
    duplicate_level.entity_count = 2;
    if (!require(init(registry, &duplicate_level) == Error::duplicate_name,
                 "duplicate Character names were not rejected")) return false;
    if (!require(registry->actor_count == original_count &&
                 std::string(registry->actors[0].name) == original_first_name &&
                 find(registry, "_prim_tmp_infected05")->lifecycle ==
                     Lifecycle::spawn_requested && registry->request_count == 1 &&
                 registry->lookup_miss_count == 0,
                 "duplicate-name rejection changed prior registry")) return false;

    // Invalid transforms are rejected transactionally.
    dh2::world::Object invalid_object = *sample;
    invalid_object.local.rotation_degrees[1] = NAN;
    auto invalid_level = level;
    invalid_level.entities = &invalid_object;
    invalid_level.entity_count = 1;
    if (!require(init(registry, &invalid_level) == Error::invalid_source_record,
                 "non-finite source transform was accepted")) return false;
    if (!require(registry->actor_count == original_count &&
                 std::string(registry->actors[0].name) == original_first_name &&
                 find(registry, "_prim_tmp_infected05")->lifecycle ==
                     Lifecycle::spawn_requested && registry->request_count == 1 &&
                 registry->lookup_miss_count == 0,
                 "invalid-transform rejection changed prior registry")) return false;

    // Capacity is counted from Character records before candidate allocation
    // or duplicate checks, so this synthetic over-bound view cannot overread.
    dh2::world::Object too_many[MAX_ACTORS + 1]{};
    for (std::uint32_t i = 0; i < MAX_ACTORS + 1; ++i) too_many[i] = *sample;
    auto capacity_level = level;
    capacity_level.entities = too_many;
    capacity_level.entity_count = MAX_ACTORS + 1;
    if (!require(init(registry, &capacity_level) == Error::capacity,
                 "actor capacity bound was not enforced")) return false;
    if (!require(registry->actor_count == original_count &&
                 std::string(registry->actors[0].name) == original_first_name &&
                 find(registry, "_prim_tmp_infected05")->lifecycle ==
                     Lifecycle::spawn_requested && registry->request_count == 1 &&
                 registry->lookup_miss_count == 0,
                 "capacity rejection changed prior registry")) return false;

    if (!require(init(registry, &level) == Error::ok &&
                 find(registry, "_prim_tmp_infected05")->lifecycle ==
                     Lifecycle::registered,
                 "registry could not be replaced after failed candidates")) return false;

    return true;
}

void print_json_string(const char* text) {
    std::putchar('"');
    for (const unsigned char* p =
             reinterpret_cast<const unsigned char*>(text); *p; ++p) {
        if (*p == '"' || *p == '\\') {
            std::putchar('\\');
            std::putchar(*p);
        } else if (*p < 0x20) {
            std::printf("\\u%04x", static_cast<unsigned>(*p));
        } else {
            std::putchar(*p);
        }
    }
    std::putchar('"');
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: actor_registry_test CACHE_FILES\n");
        return 2;
    }
    dh2::world::SourceLevel level{};
    if (!import_infected_village(argv[1], &level)) {
        dh2_world_free(&level);
        return 1;
    }
    dh2::actors::Registry registry{};
    if (dh2::actors::init(&registry, &level) != dh2::actors::Error::ok ||
        !verify_imported_actors(level, registry) ||
        !registry_transactions_and_requests(level, &registry)) {
        dh2::actors::destroy(&registry);
        dh2_world_free(&level);
        return 1;
    }

    std::printf("{\"component\":\"port-owned actor registry\",\"actor_count\":%u,"
                "\"translation_checks\":%u,\"ambush_actors\":[",
                registry.actor_count, registry.actor_count);
    for (std::uint32_t i = 0; i < 4; ++i) {
        const auto* actor = dh2::actors::find(&registry, expected_ambush[i].name);
        if (i) std::putchar(',');
        std::printf("{\"name\":"); print_json_string(actor->name);
        std::printf(",\"source_record\":%u,\"template\":",
                    actor->source_record);
        print_json_string(actor->character_template);
        std::printf(",\"world_position\":[%.4f,%.4f,%.4f],"
                    "\"rotation_z\":%.4f,\"ai_state\":",
                    actor->world_position[0], actor->world_position[1],
                    actor->world_position[2], actor->local_rotation_degrees[2]);
        print_json_string(actor->ai_state);
        std::printf(",\"auto_spawn\":%s,\"spawn_result\":",
                    actor->auto_spawn ? "true" : "false");
        print_json_string(dh2::actors::spawn_result_name(
            dh2::actors::request_spawn(&registry, actor->name)));
        std::putchar('}');
    }
    const auto miss = dh2::actors::request_spawn(&registry, "_prim_tmp_infected17");
    std::printf("],\"missing_actor\":{\"name\":\"_prim_tmp_infected17\","
                "\"result\":");
    print_json_string(dh2::actors::spawn_result_name(miss));
    std::printf(",\"lookup_miss_count\":%u,\"last_lookup_miss\":",
                registry.lookup_miss_count);
    print_json_string(registry.last_lookup_miss);
    std::printf("},\"checks\":{\"transactional_replacement\":true,"
                "\"duplicate_names\":true,\"capacity\":true,"
                "\"invalid_data\":true,\"spawn_idempotence\":true,"
                "\"cache_transforms\":true}}\n");

    // Registry owns copies; freeing the imported source must not invalidate it.
    dh2_world_free(&level);
    const bool owns_records =
        dh2::actors::find(&registry, "_prim_tmp_infected05") != nullptr &&
        dh2::actors::find(&registry, "_prim_tmp_infected05")->source_path[0] != '\0';
    dh2::actors::destroy(&registry);
    if (!owns_records) {
        std::fprintf(stderr, "FAIL: registry retained borrowed source strings\n");
        return 1;
    }
    return 0;
}
