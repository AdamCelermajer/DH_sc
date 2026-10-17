#include "../gameplay_object_callbacks.hpp"
#include "../../actor-runtime/actor_registry.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {

bool check(bool condition, const char* message) {
    if (condition) return true;
    std::fprintf(stderr, "FAIL: %s\n", message);
    return false;
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
    if (!read_bytes(cache, "data/scene/005_infectedvillage.mlx", &bytes))
        return false;
    auto result = dh2_world_import_level(level, "INFECTED_VILLAGE_01",
        "data/scene/005_infectedvillage.mlx", bytes.data(), bytes.size(),
        &diagnostic);
    if (result != dh2::world::Error::ok) return false;

    for (std::uint32_t i = 0; i < level->module_count; ++i) {
        std::vector<std::uint8_t> mgp;
        const char* path = level->modules[i].cache_mgp;
        if (!path || !read_bytes(cache, path, &mgp)) return false;
        result = dh2_world_import_module_objects(level, i,
            dh2::world::RecordKind::mgp, path, mgp.data(), mgp.size(),
            &diagnostic);
        if (result != dh2::world::Error::ok) return false;
    }
    return true;
}

bool check_cache_projections(const std::string& cache) {
    using namespace dh2::actors;
    using namespace dh2::gameplay::callbacks;
    dh2::world::SourceLevel level{};
    Registry registry{};
    bool ok = import_infected_village(cache, &level) &&
              init(&registry, &level) == Error::ok;
    if (!check(ok, "import the cache-authored Infected Village actors")) {
        destroy(&registry);
        dh2_world_free(&level);
        return false;
    }

    const char* names[] = {
        "_prim_tmp_infected05", "_prim_tmp_infected07",
        "_prim_tmp_infected06", "_prim_tmp_infected16"
    };
    const float expected_positions[][3] = {
        {2171.37f, 1283.11f, 1325.43f},
        {2153.85f, 991.88f, 1325.43f},
        {2871.59f, 983.05f, 1325.43f},
        {2562.42f, 651.98f, 1325.43f}
    };
    for (std::size_t i = 0; ok && i < 4; ++i) {
        const auto* actor = find(&registry, names[i]);
        const GameObjectView actor_view = actor
            ? GameObjectView{actor->name, actor->world_position}
            : GameObjectView{nullptr, nullptr};
        const char* name = nullptr;
        float position[3] = {-999.0f, -999.0f, -999.0f};
        if (!check(actor != nullptr, "find each statically authored Ambush Character") ||
            !check(get_name(actor ? &actor_view : nullptr, &name),
                   "project the actor name") ||
            !check(name == (actor ? actor->name : nullptr),
                   "name projection borrows the registry's source-owned string") ||
            !check(get_position(actor ? &actor_view : nullptr, position),
                   "project the actor's imported world position")) {
            ok = false;
            break;
        }
        for (unsigned axis = 0; axis < 3; ++axis) {
            if (!check(std::fabs(position[axis] - expected_positions[i][axis]) < 0.002f,
                       "GameObject position equals the cache/MGP world position")) {
                ok = false;
                break;
            }
        }
    }
    destroy(&registry);
    dh2_world_free(&level);
    return ok;
}

bool check_character_projections() {
    using namespace dh2::gameplay::callbacks;
    std::int32_t state = -1;
    std::int32_t state_time = 0x12345678;
    std::uint16_t hits = 65535;
    const float position[] = {-1.25f, 0.0f, 42.5f};
    GameObjectView object{"source-backed actor", position};
    CharacterView character{object, &state, &state_time, &hits};

    std::int32_t actual = 123;
    const bool values_ok = get_state(&character, &actual) && actual == state;
    const bool time_ok = get_state_time(&character, &actual) && actual == state_time;
    const bool hits_ok = get_hit_count(&character, &actual) && actual == 65535;
    const bool identity_ok = get_id_identity(&character) == &character;

    // A non-finite stored coordinate passes through unchanged. The callback
    // only reads the three object fields; it does not apply a physics policy.
    const float unusual_position[] = {NAN, -INFINITY, 0.0f};
    GameObjectView unusual{"unusual", unusual_position};
    float copied[3]{};
    const bool position_passthrough = get_position(&unusual, copied) &&
        std::isnan(copied[0]) && std::isinf(copied[1]) && copied[1] < 0.0f &&
        copied[2] == 0.0f;

    std::int32_t unchanged = 77;
    CharacterView missing_state{object, nullptr, &state_time, &hits};
    const bool failed_read_retains_output =
        !get_state(&missing_state, &unchanged) && unchanged == 77;
    const bool null_output_rejected = !get_hit_count(&character, nullptr);
    return check(values_ok, "Character state getter forwards the supplied state machine ID") &&
           check(time_ok, "Character state-time getter forwards the supplied owner field") &&
           check(hits_ok, "Character hit count zero-extends the source uint16") &&
           check(identity_ok, "GameObject ID remains a pointer identity token") &&
           check(position_passthrough, "position projection copies unusual values verbatim") &&
           check(failed_read_retains_output, "missing state owner leaves output unchanged") &&
           check(null_output_rejected, "missing output pointer is rejected safely");
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: read_projection_test <cache/files>\n");
        return 2;
    }
    bool ok = check_character_projections() &&
              check_cache_projections(argv[1]);
    if (!ok) return 1;
    std::puts("read-only gameplay object projections passed; 4 cache actors checked");
    return 0;
}
