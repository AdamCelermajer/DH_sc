// B159: Mud Lord (SwampKing) damage calibration over the real tables.
//
// Original (IDA libDungeonHunter2.so, pseudocode lines in the local dump):
//  - swampking_OnInit (swampking_core.luac, = monster_OnInit): dynamic level = host level clamped to GetCurrentLevelRange
//    (0x37f1f0, LevelList row words 12+d max / 15+d min) or, without a range, to the row's own LevelMin/LevelMax; + LevelOffset.
//  - skill scripts: ApplyPropClass(CLASS_ID) -> Character::_ApplyPropClass 0x3babdc -> PROPS_ApplyClass 0x3df3b8 -> _LoadClass
//    0x3e2e20 on the attacker's own resolved sheet (CharProperties+2708); then SkillCombatRoll(target) = _SkillCombatRoll
//    0x3b9fbc -> F_SkillAttack 0x3b31a4 (mask = SkillTable flags | 0x8000000, element = SkillTable +20) -> _F_CalculateResult
//    0x3b2638 -> CF__CalcDamage 0x3b1fb8 -> F_ApplyResult 0x3b10b4.
//  - CF__CalcDamage type 0 with isSkill: rand[SnS_Min(174), SnS_Max(175)) + bonus damage; - (25 * defender Defense(71)) >> 8
//    (<= 0 -> 1.0); + rand[ElemMin(95), ElemMax(96)) * min(1, 1 - defender resist(74+element)/100) when the ATTACKER's element
//    property 97 is not -1 (its own live value: the SwampKing sheet has element 1).
// argv[1] = package assets (original-cache tables), argv[2] = Android data root (scripts, ProjectileTable).
#include "runtime_swampking_core_ai_v1.hpp"
#include "../enemy_ai/runtime_ranged_skill_monster_v1.hpp"
#include "../spawn/spawn_character_v1.hpp"
#include "../../content_paths.hpp"
#include "../../original_actor_properties.hpp"
#include "../../../game-data/combat_result.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::bosses;

namespace {
int checks = 0;
void check(bool ok, const std::string& what) {
    ++checks;
    if (!ok) throw std::runtime_error(what);
}
std::vector<std::uint8_t> read_bytes(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot read " + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
// Inclusive bounds of one original skill hit that is neither missed, dodged, blocked nor critical.
struct Range { std::int32_t lo, hi; };
// CF__CalcDamage 0x3b1fb8 with isSkill = 1; the attacker is the monster (no weapon: bonus damage 0, no combo, defender state != 9).
// type 0 (mask 0x20000): rand[174,175) - (25 * d71 >> 8) + elemental rand[95,96) * factor when the attacker's property 97 is set.
// type 2 (mask 0x40000): rand[174,175), * element factor + attacker 166+element, - (51 * d73 >> 8); element = SkillTable +20.
// Draws are rand(hi - lo) in [0, hi - lo), so the largest reachable value is hi - 1 (8.8 fixed point).
Range oracle(const dh2::data::PropertySheet& a, const dh2::data::PropertySheet& d, std::uint32_t mask, std::int32_t skill_element) {
    const std::int32_t smin = std::max(0, a[174]), smax = std::max(0, a[175]);
    const auto resist_factor = [&](std::int32_t element) {
        std::int32_t factor = (d[std::size_t(element + 74)] >> 8) / -100 + 1;
        return factor > 1 ? 1 : factor;
    };
    Range r{};
    if (mask & 0x20000u) {
        const std::int32_t defence = (25 * d[71]) >> 8;
        const std::int32_t element = a[97] >> 8;
        const bool elemental = element != -1 && a[96] >= 0 && a[95] >= 0;
        const std::int32_t factor = elemental ? resist_factor(element) : 1;
        const auto build = [&](std::int32_t base, std::int32_t extra) {
            std::int32_t v = base - defence;
            if (v <= 0) v = 256;
            if (elemental && factor >= 0) { v += extra * factor; if (v <= 0) v = 256; }
            return v;
        };
        r.lo = build(smin, a[95]);
        r.hi = build(smax - 1, a[96] - 1);
    } else {
        const std::int32_t defence = (51 * d[73]) >> 8;
        const auto build = [&](std::int32_t base) {
            std::int32_t v = base;
            if (skill_element != -1) {
                const std::int32_t factor = resist_factor(skill_element);
                if (factor < 0) v = 256;
                else { v = factor * (a[std::size_t(166 + skill_element)] + v); if (v <= 0) v = 256; }
            }
            v -= defence;
            return v <= 0 ? 256 : v;
        };
        r.lo = build(smin);
        r.hi = build(smax - 1);
    }
    return r;
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 3, "usage: <package assets> <android data root>");
        const AssetCatalog package(argv[1]), scripts(argv[2]);
        std::string error;
        OriginalPropertyDatabase db;
        check(load_original_property_tables(package, "original-cache/data/pydata", db, error), error);
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(db.characters, rules, error), error);
        const auto field = [&](const char* name) {
            const auto& f = db.characters.fields;
            const auto c = std::find(f.begin(), f.end(), std::string(name));
            check(c != f.end(), std::string("field ") + name);
            return std::size_t(c - f.begin());
        };
        const auto row_of = [&](const char* name) {
            const auto r = std::find(db.characters.names.begin(), db.characters.names.end(), std::string(name));
            check(r != db.characters.names.end(), std::string("row ") + name);
            return std::size_t(r - db.characters.names.begin());
        };

        // 1. The table facts the dynamic level needs (the user-visible "LevelMin/Max 5-6" is the row fallback only).
        const auto king_row = row_of("SwampKing");
        check(db.characters.rows[king_row][field("LevelMin")] == 5 * 256 && db.characters.rows[king_row][field("LevelMax")] == 6 * 256 &&
              db.characters.rows[king_row][field("LevelOffset")] == 0, "SwampKing row: LevelMin 5, LevelMax 6, LevelOffset 0");

        // 2. The Mud Lord at the applied level. 001_swamp returns the LevelList range 1..4 (log: current_range=1..4), so a level-2
        //    host gets level 2; a host above the range is clamped to 4.
        spawn::ProfileDerivationTablesV1 tables;
        check(spawn::load_profile_derivation_tables_v1(package, "original-cache/data/pydata", tables, error), error);
        std::int32_t level_raw = -1;
        spawn::CurrentLevelRangeV1 range;
        range.returned = true; range.minimum = 1; range.maximum = 4;
        check(spawn::spawn_level_raw_v1(db.characters, "SwampKing", 2 * 256, range, level_raw, error) && level_raw == 2 * 256, "host 2 in 1..4 -> level 2");
        check(spawn::spawn_level_raw_v1(db.characters, "SwampKing", 9 * 256, range, level_raw, error) && level_raw == 4 * 256, "host 9 clamps to 4");
        check(spawn::spawn_level_raw_v1(db.characters, "SwampKing", 2 * 256, spawn::CurrentLevelRangeV1{}, level_raw, error) && level_raw == 5 * 256,
              "without a LevelList range the row's LevelMin 5 applies");

        OriginalActorProperties king;
        OriginalActorPropertyOptions options;
        options.level_raw = 2 * 256;
        check(resolve_original_actor_properties(db.characters, db.classes, "SwampKing", options, king, error), error);
        const auto& k = king.sheets.resolved;
        check(k[97] == 256 && k[95] > 0 && k[96] > k[95], "the Mud Lord's own sheet carries element 1 with an elemental damage range");

        // 3. The four skills (rows of SwampKingSkills) and their formula sheets.
        const auto skill_records = read_content(package, "data/pydata/skills_pyarray.bin");
        const auto skill_names = read_content(package, "data/pydata/skills_pyarraynames.bin");
        const auto skill_schema = read_content(package, "data/pydata/skills_pystructnames.bin");
        dh2::data::SkillTables skills;
        check(skills.load({skill_records.data(), skill_records.size()}, {skill_names.data(), skill_names.size()},
                          {skill_schema.data(), skill_schema.size()}, error), error);
        const auto records = read_bytes(std::string(argv[2]) + "/data/pydata/projectiles_pyarray.bin");
        const auto names = read_bytes(std::string(argv[2]) + "/data/pydata/projectiles_pyarraynames.bin");
        const auto schema = read_bytes(std::string(argv[2]) + "/data/pydata/projectiles_pystructnames.bin");
        const auto dictionary = read_bytes(std::string(argv[2]) + "/data/pydata/projectiles_dictionary_pyarray.bin");
        std::vector<enemy_ai::SourceProjectileRowV1> projectiles;
        check(enemy_ai::load_source_projectile_table_v1({records.data(), records.size()}, {names.data(), names.size()},
                                                        {schema.data(), schema.size()}, {dictionary.data(), dictionary.size()}, projectiles, error), error);
        SwampkingCoreSourcesV1 sources{&scripts, &tables, skills.borrow(), &projectiles, &db.classes};
        SwampkingCoreDefinitionV1 def;
        bool supported = false;
        std::string reason;
        check(derive_swampking_core_v1(sources, "SwampKing", supported, def, reason) && supported, "SwampKing definition: " + reason);

        // The hero of the real runs: KnightPlayerBase at level 2 (169.195 HP) without gear, and the same with Defense 20.
        OriginalActorProperties hero;
        OriginalActorPropertyOptions hero_options;
        hero_options.level_raw = 2 * 256;
        check(resolve_original_actor_properties(db.characters, db.classes, "KnightPlayerBase", hero_options, hero, error), error);
        check(hero.max_health > 169.19f && hero.max_health < 169.2f, "hero level 2 has 169.195 HP");
        auto armoured = hero.sheets.resolved;
        armoured[71] = 20 * 256;
        auto resistant = hero.sheets.resolved;
        for (std::size_t e = 74; e < 80; ++e) resistant[e] = 100 * 256; // 100% resist: factor (100 / -100 + 1) = 0

        struct Expect { const char* class_name; std::int32_t min_base, max_base, min_scale, max_scale; };
        // Class rows (independent of the engine): 174 = base + scale * level, 175 likewise (fixed point 8.8, level 2).
        const Expect expectations[] = {
            {"SwampKingSkill5PCT", 14080, 16640, 281, 307},
            {"SwampKingSkill30PCT", 15360, 17920, 281, 307},
            {"SwampKingSkillPoisonAttack", 11520, 14080, 153, 204},
            {"SwampKingSkillPoisonAttackAOE", 7680, 8960, 153, 204},
            {"SwampKingSkillAOEPushback", 11520, 12800, 204, 230},
        };
        for (const auto& e : expectations) {
            const auto cls = std::find(db.classes.names.begin(), db.classes.names.end(), std::string(e.class_name));
            check(cls != db.classes.names.end(), e.class_name);
            dh2::data::PropertySheet sheet{};
            check(enemy_ai::ranged_skill_formula_sheet_v1(db.characters, db.classes, rules, k, std::int32_t(cls - db.classes.names.begin()), 0, sheet, error), error);
            check(sheet[174] == e.min_base + 2 * e.min_scale && sheet[175] == e.max_base + 2 * e.max_scale,
                  std::string(e.class_name) + ": SnS_Min/MaxDamage = base + scale * level 2");
            // ApplyPropClass is in place: the monster's own element / elemental range / attack rating survive.
            check(sheet[97] == k[97] && sheet[95] == k[95] && sheet[96] == k[96] && sheet[50] == k[50] && sheet[19] == k[19],
                  std::string(e.class_name) + ": the attacker's own element, elemental range, attack rating and level are kept");

            // 4. Run the real result engine (dh2_combat_result = _F_CalculateResult) for every DoSkill slot that uses this class.
            for (const auto& skill : def.skills) {
                if (skill.class_id != std::int32_t(cls - db.classes.names.begin())) continue;
                const std::uint32_t mask = skill.skill_flags | 0x08000000u;
                for (const auto* defender_sheet : {&hero.sheets.resolved, &armoured, &resistant}) {
                    const auto expected = oracle(sheet, *defender_sheet, mask, skill.element);
                    dh2::data::CombatantView attacker{sheet.data(), -1, -1, 0, 0, 0, -1, 0};
                    dh2::data::CombatantView defender{defender_sheet->data(), -1, -1, 0, 0, 0, 3, 0};
                    std::int32_t lowest = INT32_MAX, highest = INT32_MIN;
                    int landed = 0, pushes = 0;
                    for (std::uint32_t seed = 1; seed <= 6000; ++seed) {
                        dh2::data::CombatRandom random{seed * 7919u + 13u, 0};
                        dh2::data::CombatResult result;
                        const dh2::data::CombatResultRequest request{&attacker, &defender, &random, mask, -1, skill.element, 0};
                        check(dh2_combat_result(&result, &request) == 0, "result engine");
                        if (result.outcomes & 0x80u) ++pushes;
                        if (result.outcomes & 3u) continue;      // missed / dodged: no damage
                        if (result.outcomes & (4u | 8u)) continue; // blocked / critical change the formula
                        ++landed;
                        lowest = std::min(lowest, result.amount);
                        highest = std::max(highest, result.amount);
                    }
                    check(landed > 100, std::string(skill.skill_name) + ": hits land");
                    check(lowest >= expected.lo && highest <= expected.hi,
                          std::string(skill.skill_name) + ": every landed hit is inside the CF__CalcDamage range [" + std::to_string(expected.lo / 256.0) +
                          "," + std::to_string(expected.hi / 256.0) + "], saw [" + std::to_string(lowest / 256.0) + "," + std::to_string(highest / 256.0) + "]");
                    // 6000 draws reach both ends of the range to within 2% of its width.
                    const auto width = expected.hi - expected.lo;
                    check(lowest - expected.lo <= width / 50 + 1 && expected.hi - highest <= width / 50 + 1,
                          std::string(skill.skill_name) + ": the draws cover the whole predicted range");
                    // Outcome bit 0x80 (CF__CalcPush) is what F_ApplyResult turns into SM_SetKnockBackState: rare for the bites
                    // (SnS_Push_Target_Chance 108), common for the emerge AOE (158) which also carries the great-knockback flag.
                    if (defender_sheet == &hero.sheets.resolved) {
                        const bool emerge = skill.script.shape == BossSkillShapeV1::target_list_aoe && skill.skill_flags & 0x100000u;
                        check(emerge ? pushes > 6000 / 3 && (skill.skill_flags & 0x100000u) != 0 : pushes < 6000 / 8,
                              std::string(skill.skill_name) + ": push outcome rate " + std::to_string(pushes) + "/6000");
                    }
                    if (std::getenv("DH_PRINT_BOSS_DAMAGE"))
                        std::cout << skill.skill_name << " class=" << e.class_name << " flags=0x" << std::hex << skill.skill_flags << std::dec
                                  << " element=" << skill.element << " defender=" << (defender_sheet == &hero.sheets.resolved ? "hero" : defender_sheet == &armoured ? "armoured" : "resist")
                                  << " damage " << expected.lo / 256.0 << ".." << expected.hi / 256.0 << " landed=" << landed << "/6000 push-outcomes=" << pushes << '\n';
                }
            }
        }
        std::cout << "runtime_swampking_damage_calibration_v1 tests passed (" << checks << " checks)\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << "runtime_swampking_damage_calibration_v1 test failed: " << failure.what() << '\n';
        return 1;
    }
}
