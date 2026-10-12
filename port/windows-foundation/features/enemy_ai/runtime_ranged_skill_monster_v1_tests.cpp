// B154: ranged-skill monster (Bogwitch) unit tests over the real authored scripts and pydata tables.
// argv[1] = package assets (original-cache tables, compiled level banks), argv[2] = Android data root holding the
// authored AI/skill scripts and the ProjectileTable (assets-extra/android: data/scripts/..., data/pydata/projectiles_*).
#include "runtime_ranged_skill_monster_v1.hpp"
#include "../campaign_host/actor_clip_manifest.hpp"
#include "../../content_paths.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_campaign_runtime.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;

namespace {
int checks = 0;
void check(bool ok, const std::string& what) {
    ++checks;
    if (!ok) throw std::runtime_error(what);
}
std::string read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot read " + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
std::vector<std::uint8_t> read_bytes(const std::string& path) {
    const auto text = read_file(path);
    return {text.begin(), text.end()};
}
bool near(float a, float b, float tolerance = 1e-3f) { return std::fabs(a - b) <= tolerance; }

void classification(const std::string& package) {
    const AssetCatalog assets(package);
    std::string error;
    OriginalCampaignRuntime cave, swamp, darkwood;
    check(cave.load_compiled_level(assets, "data/scene/002_swamp_witchcave.rule.xml", error), "witch cave bank: " + error);
    const auto cave_actors = campaign_host::collect_script_gameplay_actors(cave);
    check(cave_actors.count("_prim_Monster_04_56") == 1,
          "B154: the witch (spawned by Intro_Witch, retired only by SkipCutscene_Bugfix, idled by WitchTeleport and killed by "
          "Return_Cinematic by name) is a gameplay actor");
    check(cave_actors.count("_prim_Monster_09") == 1, "the cave chest ambush lizards stay gameplay actors");
    check(swamp.load_compiled_level(assets, "data/scene/001_swamp.mlx", error), "swamp bank: " + error);
    const auto swamp_actors = campaign_host::collect_script_gameplay_actors(swamp);
    check(swamp_actors.count("_prim_ActorTroll") == 0, "the opening troll (spawned and retired by Swamp_Intro) stays a prop");
    check(swamp_actors.count("_prim_Monster_FakeMoth_01") == 1, "the moth intro actors stay gameplay actors");
    check(darkwood.load_compiled_level(assets, "data/scene/003_darkwood.mlx", error), "darkwood bank: " + error);
    const auto darkwood_actors = campaign_host::collect_script_gameplay_actors(darkwood);
    check(darkwood_actors.count("_prim_tmp_questtype1_LostMerch2") == 0,
          "the lost merchant spawned by PostAccept and retired by Success (no named Idle/Kill) stays a prop");
    check(darkwood_actors.count("_prim_tmp_questtype1_MerchSuccess") == 1, "a never-retired spawned merchant is a gameplay actor");
}

void scripts(const std::string& extra) {
    RangedSkillAiScriptV1 ai;
    std::string error;
    check(parse_ranged_skill_ai_script_v1(read_file(extra + "/data/scripts/ai/witch.luac"), ai, error), "witch.luac: " + error);
    check(ai.close_range_skill == 0 && ai.close_range_stops && ai.close_range_looks_at_target, "OnTargetInCloseRange: Stop; LookAtTarget; DoSkill(0)");
    check(ai.ranged_range_approaches && ai.out_of_range_clears_target && ai.target_died_clears_target, "ranged/out/died callbacks");
    check(ai.enemy_spotted_searches_player, "OnEnemySpotted searches the Player");
    check(ai.teleport.present && ai.teleport.repeat && near(float(ai.teleport.delay_ms), 10000.0f), "teleport timer MOVE_DELAY 10000 ms repeating");
    check(ai.teleport.fx_name == "spell_dh2_prince_mage_stone_escape_magefx" && ai.teleport.sound_name == "sfx_teleport", "teleport FX/sound");
    check(ai.teleport.candidates.size() == 4, "four relative teleport candidates");
    const std::array<std::array<float, 3>, 4> expected{{{0, 900, 0}, {0, -900, 0}, {900, 0, 0}, {-900, 0, 0}}};
    for (unsigned i = 0; i != 4; ++i)
        check(near(ai.teleport.candidates[i][0], expected[i][0]) && near(ai.teleport.candidates[i][1], expected[i][1]) &&
              near(ai.teleport.candidates[i][2], expected[i][2]), "teleport candidate order forward/back/right/left at MOVE_RANGE 900");
    check(ai.spotted_music == "SwampKingMusic" && ai.spotted_music_fade_ms == 500, "spotted music");
    check(ai.died_music == "SwampHubAmbientMusic" && ai.died_music_fade_ms == 1000 && ai.died_stops_timer, "death music and timer stop");

    RangedSkillAiScriptV1 rejected;
    check(!parse_ranged_skill_ai_script_v1(read_file(extra + "/data/scripts/ai/monster.luac"), rejected, error),
          "monster.luac (conditional DoSkill/Attack) is not a ranged-skill AI script");
    check(!parse_ranged_skill_ai_script_v1("function X_OnUpdate()\nend\nAddToVFTable(\"OnUpdate\", \"X_OnUpdate\");", rejected, error),
          "a script without range callbacks is rejected");

    RangedSkillScriptV1 skill;
    check(parse_ranged_skill_script_v1(read_file(extra + "/data/scripts/skills/witch_ranged_00.luac"), skill, error), "witch_ranged_00.luac: " + error);
    check(skill.class_token == "Swamp_WitchRangedAttack" && skill.range_field == "RangeMaxDistance" && skill.projectile_field == "RangeProjectileID",
          "skill constants");
    check(skill.pre_frontal_enemy_search && skill.skill_spawns_projectile && skill.delayed_combat_roll && skill.delayed_check_character,
          "skill callbacks");
}

std::vector<SourceProjectileRowV1> projectiles(const std::string& extra) {
    const auto records = read_bytes(extra + "/data/pydata/projectiles_pyarray.bin");
    const auto names = read_bytes(extra + "/data/pydata/projectiles_pyarraynames.bin");
    const auto schema = read_bytes(extra + "/data/pydata/projectiles_pystructnames.bin");
    const auto dictionary = read_bytes(extra + "/data/pydata/projectiles_dictionary_pyarray.bin");
    std::vector<SourceProjectileRowV1> rows;
    std::string error;
    check(load_source_projectile_table_v1({records.data(), records.size()}, {names.data(), names.size()},
                                          {schema.data(), schema.size()}, {dictionary.data(), dictionary.size()}, rows, error),
          "ProjectileTable: " + error);
    check(rows.size() == 41, "41 projectile rows");
    const auto& witch = rows[39];
    check(witch.name == "bogwitch" && witch.model_uri == "data/3D/projectiles/projectile_dh2_bogwitch.bdae", "row 39 bogwitch model");
    check(near(witch.velocity, 20) && near(witch.velocity_damping, 0) && witch.timer_ms == 4000 && near(witch.max_distance, -1),
          "bogwitch motion: velocity 20, no damping, 4000 ms, unbounded distance");
    check(witch.impact_fx == 76 && witch.impact_sound == 613 && witch.disappear_on_hit && !witch.friendly_fire && !witch.target_lock &&
          witch.magic && witch.time_between_hit_ms == 1000, "bogwitch impact/filters");
    auto truncated = records;
    truncated.pop_back();
    std::vector<SourceProjectileRowV1> bad;
    check(!load_source_projectile_table_v1({truncated.data(), truncated.size()}, {names.data(), names.size()},
                                           {schema.data(), schema.size()}, {dictionary.data(), dictionary.size()}, bad, error),
          "a truncated ProjectileTable is rejected");
    return rows;
}

void definition(const std::string& package, const std::string& extra, const std::vector<SourceProjectileRowV1>& rows) {
    std::string error;
    // The tables come from the package; the scripts from the Android data root (not packaged yet).
    const AssetCatalog package_assets(package), script_assets(extra);
    spawn::ProfileDerivationTablesV1 tables;
    check(spawn::load_profile_derivation_tables_v1(package_assets, "original-cache/data/pydata", tables, error), "derivation tables: " + error);
    OriginalPropertyDatabase properties;
    check(load_original_property_tables(package_assets, "original-cache/data/pydata", properties, error), "property tables: " + error);
    const auto skill_records = read_content(package_assets, "data/pydata/skills_pyarray.bin");
    const auto skill_names = read_content(package_assets, "data/pydata/skills_pyarraynames.bin");
    const auto skill_schema = read_content(package_assets, "data/pydata/skills_pystructnames.bin");
    dh2::data::SkillTables skills;
    check(skills.load({skill_records.data(), skill_records.size()}, {skill_names.data(), skill_names.size()},
                      {skill_schema.data(), skill_schema.size()}, error), "skill tables: " + error);
    RangedSkillMonsterSourcesV1 sources{&script_assets, &tables, skills.borrow(), &rows, &properties.classes};
    RangedSkillMonsterDefinitionV1 witch;
    bool supported = false;
    std::string reason;
    check(derive_ranged_skill_monster_v1(sources, "Witch", supported, witch, reason) && supported, "Witch is a ranged-skill monster: " + reason);
    check(witch.ai_script == "witch" && witch.skill_script == "witch_ranged_00" && witch.skill_name == "Witch_Projectile" &&
          witch.skill_table_id == 122, "Witch DoSkill(0) -> SkillTree WitchSkills[0] = Witch_Projectile (122)");
    check(witch.animation_sequence_id == 756, "Witch_Projectile Anim = 756 Witch_Attack1H");
    check(near(witch.range_min, 600) && near(witch.range_max, 5000) && near(witch.pre_search_range, 5000) && witch.projectile_row == 39,
          "CanRangeAttack 600..5000, pre-search RangeMaxDistance, projectile 39");
    check(witch.class_id >= 0 && properties.classes.names[std::size_t(witch.class_id)] == "Swamp_WitchRangedAttack", "class id");
    RangedSkillMonsterDefinitionV1 other;
    check(derive_ranged_skill_monster_v1(sources, "Swamp_LizadMan_Type1", supported, other, reason) && !supported,
          "a melee lizard is not a ranged-skill monster");
    check(derive_ranged_skill_monster_v1(sources, "Darkwood_RangedBandit_lvl_4", supported, other, reason) && !supported,
          "a monster.luac ranged bandit keeps its existing owner (not this AI shape): " + reason);

    CombatSessionProfile policy;
    check(derive_ranged_skill_monster_policy_v1(tables, "Witch", policy, error), "witch policy: " + error);
    check(policy.animationOnly && policy.receiveDamage && policy.reaction && policy.reaction->state == "Injured" && policy.death &&
          policy.death->state == "Died" && policy.initialIdle.state == "Idle" && policy.damageMarkerNames.empty() &&
          !policy.sequenceAction && policy.propertyOptions.refill_vitals, "incoming-only policy with the AnimTable Injured/Died poses");

    // Formula sheet: SnS_Level 0 + Swamp_WitchRangedAttack (SnS_CleanUp, Min 15+1.6L, Max 18+1.8L, Push chance 50).
    dh2::data::PropertyRules rules;
    check(dh2::data::load_property_rules(properties.characters, rules, error), "property rules: " + error);
    const auto field = [&](const char* name) {
        const auto& f = properties.characters.fields;
        const auto c = std::find(f.begin(), f.end(), std::string(name));
        check(c != f.end(), std::string("field ") + name);
        return std::size_t(c - f.begin());
    };
    dh2::data::PropertySheet live = rules.defaults, sheet{};
    check(ranged_skill_formula_sheet_v1(properties.characters, properties.classes, rules, live, witch.class_id, 0, sheet, error),
          "formula sheet: " + error);
    check(sheet[field("SnS_MinDamage")] == 15 * 256 && sheet[field("SnS_MaxDamage")] == 18 * 256, "SnS_Min/MaxDamage at level 0");
    check(sheet[field("SnS_Push_Target_Chance")] == 50 * 256, "SnS_Push_Target_Chance 50");
    check(sheet[field("SnS_Level")] == 0, "SnS_Level 0 (monster skill level)");
    check(!ranged_skill_formula_sheet_v1(properties.characters, properties.classes, rules, live, -1, 0, sheet, error), "bad class rejected");
}

void geometry() {
    check(ranged_range_event_v1(599.f * 599.f, 600, 5000) == RangedRangeEventV1::close_range, "below RangeMin is close range");
    check(ranged_range_event_v1(600.f * 600.f, 600, 5000) == RangedRangeEventV1::ranged_range, "RangeMin itself is ranged range");
    check(ranged_range_event_v1(5000.f * 5000.f, 600, 5000) == RangedRangeEventV1::ranged_range, "RangeMax itself is ranged range");
    check(ranged_range_event_v1(5001.f * 5001.f, 600, 5000) == RangedRangeEventV1::out_of_range, "beyond RangeMax is out of range");
    const auto look = source_look_vector_v1(0.0f);
    check(near(look[0], 0) && near(look[1], -1), "heading 0 looks along -Y");
    const auto forward = relative_point_v1({100, 200, 30}, 0.0f, {0, 900, 0});
    check(near(forward[0], 100) && near(forward[1], -700) && near(forward[2], 30), "forward relative point");
    const auto right = relative_point_v1({0, 0, 0}, 0.0f, {900, 0, 0});
    check(near(right[0], 900) && near(right[1], 0), "right = cross(Z, look)");

    SourceProjectileRowV1 row;
    row.velocity = 20;
    row.timer_ms = 4000;
    row.max_distance = -1;
    RangedProjectileV1 p;
    p.forward = {1, 0, 0};
    p.speed = row.velocity;
    p.remaining_ms = row.timer_ms;
    check(!advance_ranged_projectile_v1(p, row, 100) && near(p.position[0], 200), "20 Box2D units/s = 2000 game units/s");
    check(ranged_projectile_contact_v1(p, {250, 0, 500}, 60) && !ranged_projectile_contact_v1(p, {300, 0, 0}, 60), "XY body contact");
    check(advance_ranged_projectile_v1(p, row, 3900), "expires after its 4000 ms Timer");
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 3, "usage: runtime_ranged_skill_monster_v1_tests <package assets> <android data root>");
        classification(argv[1]);
        scripts(argv[2]);
        const auto rows = projectiles(argv[2]);
        definition(argv[1], argv[2], rows);
        geometry();
        std::cout << "runtime_ranged_skill_monster_v1 tests passed (" << checks << " checks)\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << "runtime_ranged_skill_monster_v1 test failed: " << failure.what() << '\n';
        return 1;
    }
}
