// P16 SPAWN focused tests: real CharacterTable/Charater_Templates rows, real
// actor profiles and visuals (ActorPopulation::admit_declared), and fake owner
// effects that record every call. Usage: spawn_character_v1_tests <repo-root>
#include "spawn_character_v1.hpp"

#include "../../actor_population.hpp"
#include "../../asset_catalog.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::spawn;

namespace {

void require(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    AssetCatalog assets;
    dh2::data::CharacterTable characters;
    dh2::data::CharacterTemplateTableV78 templates;
    ActorProfileLibrary profiles;
    std::string error;

    explicit Fixture(const std::filesystem::path& root) : assets(root / ".local-inputs/windows-shared-assets") {
        const std::string table_root = "original-cache/data/pydata/";
        const auto records = assets.read(table_root + "character_properties_pyarray.bin");
        const auto names = assets.read(table_root + "character_properties_pyarraynames.bin");
        const auto schema = assets.read(table_root + "character_properties_pystructnames.bin");
        require(dh2::data::load_characters({records.data(), records.size()}, {names.data(), names.size()},
                                           {schema.data(), schema.size()}, characters, error), error);
        const auto template_records = assets.read(table_root + "character_templates_pyarray.bin");
        const auto template_names = assets.read(table_root + "character_templates_pyarraynames.bin");
        require(templates.load({template_records.data(), template_records.size()},
                               {template_names.data(), template_names.size()}, error), error);
        require(profiles.load(assets, "actor-profiles-v2.xml", error), error);
    }

    SpawnServicesV1 services(std::function<bool(const std::string&)> available,
                             std::function<bool(std::int32_t, std::int32_t&)> draw,
                             std::vector<std::string>& log) {
        SpawnServicesV1 s;
        s.characters = &characters;
        s.templates = &templates;
        s.random_index = [draw](std::int32_t bound, std::int32_t& index, std::string&) {
            if (!draw(bound, index)) return false;
            return true;
        };
        s.profile_available = [available](const std::string& profile, std::string& e) {
            if (available(profile)) return true;
            e = "fixture: profile not in session";
            return false;
        };
        s.log = [&log](const std::string& line) { log.push_back(line); };
        return s;
    }
};

// Fake owners: record every mutation so tests can prove "no owner touched" on rejection.
struct Owners {
    std::vector<std::uint64_t> placed, begun, hidden;
    std::vector<SpawnClipPolicy> clips;
    std::array<float, 3> last_position{};
    float last_heading = 0;
    bool fail_begin = false;
    void attach(SpawnServicesV1& s) {
        s.place = [this](std::uint64_t id, const std::array<float, 3>& p, float h, std::string&) {
            placed.push_back(id); last_position = p; last_heading = h; return true;
        };
        s.begin = [this](std::uint64_t id, SpawnClipPolicy clip, std::string& e) {
            begun.push_back(id); clips.push_back(clip);
            if (fail_begin) { e = "fixture begin failure"; return false; }
            return true;
        };
        s.hide = [this](std::uint64_t id, std::string&) { hidden.push_back(id); return true; };
    }
};

} // namespace

int main(int argc, char** argv) {
    try {
        require(argc == 2, "Supply repository root");
        const std::filesystem::path root(argv[1]);
        Fixture f(root);
        const std::string lizard = "Swamp_LizadMan_Type1";
        const std::string minions = "Swamp_Moth_Minions";     // CharacterTable row, no actor profile in XML
        const std::string moth = "Swamp_Moth_Type1";

        // --- parse (--spawn-test) ---
        SpawnTestRequestV1 parsed;
        require(parse_spawn_test_v1("Swamp_LizadMan_Type1@-6752.641,938.285,250@33", parsed, f.error), f.error);
        require(parsed.name == lizard && parsed.frame == 33 && std::fabs(parsed.position[0] + 6752.641f) < 1e-2f &&
                parsed.position[2] == 250.0f, "parse valid spawn test");
        for (const char* bad : {"NoFrame@1,2,3", "X@1,2@5", "X@1,2,3@-1", "X@nan,2,3@5", "X@1,2,3x@5", "@1,2,3@5", "X@1,2,3@5junk"}) {
            SpawnTestRequestV1 rejected;
            require(!parse_spawn_test_v1(bad, rejected, f.error), std::string("malformed --spawn-test accepted: ") + bad);
        }

        // --- candidate profiles: direct row and weighted template ---
        std::vector<std::string> candidates;
        require(spawn_candidate_profiles_v1(lizard, f.characters, f.templates, candidates, f.error) &&
                candidates == std::vector<std::string>{lizard}, "direct row candidate");
        std::string template_name;
        std::vector<std::string> members;
        for (const auto& name : f.templates.names()) {
            if (spawn_candidate_profiles_v1(name, f.characters, f.templates, members, f.error) && members.size() > 1) {
                template_name = name;
                break;
            }
        }
        require(!template_name.empty(), "no multi-member template in the shipped table");
        require(!spawn_candidate_profiles_v1("NoSuchRowOrTemplate", f.characters, f.templates, members, f.error),
                "unknown name must be rejected");

        // --- level scaling from row data (lizard LevelMin 1.0, LevelMax 3.0) ---
        std::int32_t level = -2;
        require(spawn_level_raw_v1(f.characters, lizard, 256, level, f.error) && level == 256, "lizard host 1.0 -> 1.0");
        require(spawn_level_raw_v1(f.characters, lizard, 768, level, f.error) && level == 768, "lizard host 3.0 -> 3.0");
        require(spawn_level_raw_v1(f.characters, lizard, 1280, level, f.error) && level == 768, "lizard host 5.0 clamps to 3.0");
        require(spawn_level_raw_v1(f.characters, minions, 1024, level, f.error) && level == 512, "minions LevelMax 2.0 clamp");
        std::string unscaled;
        for (std::size_t i = 0; i < f.characters.names.size(); ++i)
            if (*dh2::data::property(f.characters, f.characters.names[i], "LevelMax") == -1) { unscaled = f.characters.names[i]; break; }
        require(!unscaled.empty(), "a row without LevelMax must exist");
        require(spawn_level_raw_v1(f.characters, unscaled, 768, level, f.error) && level == -1, "unscaled row keeps base level");

        // --- pool: unique IDs, capacity, release, declarations ---
        SpawnPoolV1 pool;
        require(pool.reserve(lizard, 2, 256, f.error), f.error);
        require(pool.reserve(moth, 1, 256, f.error), f.error);
        require(!pool.reserve("", 1, 0, f.error) && !pool.reserve(moth, 0, 0, f.error), "invalid reservation rejected");
        const auto decls = pool.declarations("original-cache/data/scene/001_swamp.mlx");
        require(decls.size() == 3, "declarations per slot");
        std::set<std::uint64_t> ids;
        for (const auto& d : decls) {
            ids.insert(d.stableId);
            require(d.stableId >= pool_stable_id_base && d.properties.at("ai_state") == "Limbus" &&
                    d.properties.at("auto_spawn") == "0", "declaration carries intro-style hidden facts");
        }
        require(ids.size() == 3, "pool stable IDs must be unique");
        std::uint64_t a = 0, b = 0, c = 0;
        require(pool.acquire(lizard, 77, a, f.error) && pool.acquire(lizard, 77, b, f.error), "two lizard slots");
        require(a != b && pool.busy_count() == 2, "distinct live slots");
        require(!pool.acquire(lizard, 77, c, f.error) && f.error.find("No free spawn slot") != std::string::npos,
                "third lizard spawn must be refused, not duplicated");
        require(pool.release(a, f.error) && !pool.release(a, f.error), "release once, second release refused");
        require(pool.acquire(lizard, 0, c, f.error) && c == a, "freed slot is reusable");
        require(!pool.release(12345, f.error), "unknown slot release refused");

        // --- real population admission (same loader family as authored actors) ---
        ActorPopulation population;
        ActorCustomization customization;
        customization.allow_missing_animation_targets = true;
        customization.use_authored_modular_defaults = true;
        const auto* lizard_profile = f.profiles.find(lizard);
        require(lizard_profile, "lizard profile present");
        require(population.admit_declared(f.assets, decls[0], *lizard_profile, PopulationDecision::deferred, customization, f.error), f.error);
        require(population.actors().size() == 1 && !population.actors()[0].enabled && population.actors()[0].visual.loaded(),
                "deferred pool slot admitted with a loaded visual and disabled");
        require(!population.admit_declared(f.assets, decls[0], *lizard_profile, PopulationDecision::deferred, customization, f.error),
                "duplicate declaration rejected");
        require(!population.admit_declared(f.assets, decls[1], *lizard_profile, PopulationDecision::exclude, customization, f.error),
                "exclude is not an admission");

        // --- spawn with fake owners ---
        std::vector<std::string> log;
        Owners owners;
        bool profile_lizard = true;
        std::int32_t fixed_draw = 0;
        auto draw = [&](std::int32_t bound, std::int32_t& index) { index = fixed_draw % bound; return true; };
        SpawnServicesV1 s = f.services([&](const std::string& p) { return p == lizard || (p == moth && profile_lizard); }, draw, log);
        owners.attach(s);
        SpawnPoolV1 fresh;
        require(fresh.reserve(lizard, 2, 256, f.error) && fresh.reserve(minions, 1, 512, f.error), f.error);

        SpawnRequestV1 request;
        request.name = lizard;
        request.position = {-6752.641f, 938.285f, 250.0f};
        request.heading_radians = 0.25f;
        request.summoner = 0;
        request.host_level_raw = 768;
        SpawnResultV1 result;
        require(spawn_character_v1(fresh, request, s, result, f.error) && result.spawned, "lizard spawn accepted");
        require(owners.placed.size() == 1 && owners.begun.size() == 1 && owners.clips[0] == SpawnClipPolicy::source_spawn_state,
                "lizard spawn places once and starts the source Spawn clip once");
        require(owners.last_position[0] == request.position[0] && owners.last_heading == 0.25f, "placement uses request position/heading");
        require(result.plan.level_raw == 768 && result.plan.profile_id == lizard && result.plan.template_row < 0, "plan facts");
        require(log.size() == 1 && log[0].rfind("SPAWN ok template=Swamp_LizadMan_Type1 profile=Swamp_LizadMan_Type1", 0) == 0 &&
                log[0].find("transient=1") != std::string::npos, "one ok log line per spawn");

        // Minions: row exists, profile not admitted -> rejected before any owner is touched.
        log.clear();
        owners.placed.clear();
        SpawnRequestV1 minion_request = request;
        minion_request.name = minions;
        SpawnResultV1 rejected;
        require(!spawn_character_v1(fresh, minion_request, s, rejected, f.error) &&
                rejected.line.find("profile not admitted: Swamp_Moth_Minions") != std::string::npos &&
                owners.placed.empty() && log.size() == 1, "unadmitted profile rejected with zero owner mutation");

        // Pool exhausted -> refused, nothing placed.
        log.clear();
        SpawnResultV1 second, third, fourth;
        require(spawn_character_v1(fresh, request, s, second, f.error), "second lizard");
        require(!spawn_character_v1(fresh, request, s, third, f.error) && third.line.find("No free spawn slot") != std::string::npos &&
                owners.placed.size() == 1, "no free slot: refused, no duplicate, no placement");
        require(log.size() == 2, "one line per attempt");

        // Non-finite input rejected before any owner call.
        SpawnRequestV1 nan_request = request;
        nan_request.position[1] = std::nanf("");
        owners.placed.clear();
        require(!spawn_character_v1(fresh, nan_request, s, fourth, f.error) && owners.placed.empty(), "nonfinite position rejected first");

        // Begin failure: owner was touched, the actor stays busy and failed (no reuse, no despawn).
        owners.fail_begin = true;
        SpawnPoolV1 failing;
        require(failing.reserve(lizard, 1, 256, f.error), f.error);
        SpawnResultV1 failed_result;
        require(!spawn_character_v1(failing, request, s, failed_result, f.error) &&
                failed_result.line.find("failed-state") != std::string::npos, "begin failure reported as failed-state");
        const auto failed_id = failed_result.actor;
        require(failing.busy_count() == 1 && failing.slot(failed_id)->failed, "failed actor keeps its slot busy");
        std::string dead_error;
        require(!despawn_character_v1(failing, failed_id, s, dead_error), "failed actor cannot be despawned blindly");
        owners.fail_begin = false;

        // Template: one weighted draw from the shared RNG; the member profile must be admitted.
        Owners template_owners;
        SpawnPoolV1 template_pool;
        std::vector<std::string> template_profiles;
        require(spawn_candidate_profiles_v1(template_name, f.characters, f.templates, template_profiles, f.error), f.error);
        for (const auto& p : template_profiles) require(template_pool.reserve(p, 1, 256, f.error), f.error);
        std::int32_t bound_seen = 0;
        SpawnServicesV1 template_services = f.services([](const std::string&) { return true; },
            [&](std::int32_t bound, std::int32_t& index) { bound_seen = bound; index = 0; return true; }, log);
        template_owners.attach(template_services);
        SpawnRequestV1 template_request = request;
        template_request.name = template_name;
        SpawnResultV1 template_result;
        require(spawn_character_v1(template_pool, template_request, template_services, template_result, f.error) && template_result.spawned,
                "template spawn draws once and spawns its member");
        const auto& members_raw = f.templates.rows()[static_cast<std::size_t>(f.templates.find(template_name.c_str()))].selected_ids;
        require(bound_seen == static_cast<std::int32_t>(members_raw.size()), "RNG bound equals weighted member count");
        require(template_result.plan.template_row >= 0 &&
                std::find(template_profiles.begin(), template_profiles.end(), template_result.plan.profile_id) != template_profiles.end(),
                "template member resolves to its CharacterTable row");

        // Despawn returns the slot; a second despawn is refused.
        std::string derr;
        require(despawn_character_v1(fresh, result.actor, s, derr) && owners.hidden.size() == 1 && fresh.busy_count() == 1,
                "despawn hides and frees the slot");
        require(!despawn_character_v1(fresh, result.actor, s, derr), "double despawn refused");

        // Pool reuse: the freed slot is handed out again (same actor ID, no duplicate, busy count restored).
        log.clear();
        SpawnResultV1 reused;
        require(spawn_character_v1(fresh, request, s, reused, f.error) && reused.actor == result.actor &&
                fresh.busy_count() == 2, "despawned slot is reused by the next spawn of its profile");
        require(despawn_character_v1(fresh, reused.actor, s, derr) && despawn_character_v1(fresh, result.actor == reused.actor ? second.actor : result.actor, s, derr) &&
                fresh.busy_count() == 0, "both slots released after despawn");

        // Named request parser (--despawn-test / --spawn-declared): NAME@FRAME only.
        SpawnNamedRequestV1 named;
        require(parse_spawn_named_v1("_prim_Monster_LizManIntro1@60", named, f.error) &&
                named.name == "_prim_Monster_LizManIntro1" && named.frame == 60, "parse NAME@FRAME");
        for (const char* bad : {"NoFrame", "@60", "X@", "X@-1", "X@6junk", "X@1.5"}) {
            SpawnNamedRequestV1 rejected;
            require(!parse_spawn_named_v1(bad, rejected, f.error), std::string("malformed NAME@FRAME accepted: ") + bad);
        }

        // Declared (authored) spawn: admitted only from the hidden PreSpawn17 state, one begin, no pool slot touched.
        Owners declared_owners;
        SpawnServicesV1 declared = f.services([](const std::string&) { return true; },
            [](std::int32_t, std::int32_t& index) { index = 0; return true; }, log);
        declared_owners.attach(declared);
        std::string declared_line, declared_error;
        require(!spawn_declared_v1("_prim_Monster_LizManIntro1", 9001, 3, declared, declared_line, declared_error) &&
                declared_owners.begun.empty() && declared_line.find("not in PreSpawn17") != std::string::npos,
                "declared spawn refused from Idle3 with zero owner mutation");
        require(spawn_declared_v1("_prim_Monster_LizManIntro1", 9001, 17, declared, declared_line, declared_error) &&
                declared_owners.begun == std::vector<std::uint64_t>{9001} &&
                declared_owners.clips == std::vector<SpawnClipPolicy>{SpawnClipPolicy::source_spawn_state} &&
                declared_line == "SPAWN declared ok name=_prim_Monster_LizManIntro1 actor=9001 clip=source_spawn_state",
                "declared spawn from PreSpawn17 begins the source Spawn clip once");
        declared_owners.fail_begin = true;
        require(!spawn_declared_v1("_prim_Monster_LizManIntro2", 9002, 17, declared, declared_line, declared_error) &&
                declared_line.find("failed-state") != std::string::npos, "declared begin failure is reported");

        std::cout << "PASS parse=7 candidates=direct+template level=row-scaled pool=unique+capacity admit=deferred+visual"
                  << " spawn=place+clip+one-line rejections=unadmitted,nofree,nonfinite failed-state=kept-busy"
                  << " template=" << template_name << " members=" << template_profiles.size() << " despawn=freed reuse=same-slot"
                  << " named=parsed declared=17-only\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "spawn_character_v1 FAIL: " << exception.what() << '\n';
        return 1;
    }
}
