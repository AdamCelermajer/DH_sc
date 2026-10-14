// v21.8 skills-knight: unit checks for the Knight-tree (Berserker/Paladin) use profile rows.
// The values are transcribed from the original skill Lua (see runtime_skill_use_profile_knight_v1.hpp).
// argv[1] = windows-shared-assets root (original-cache/data/pydata name tables).
#include "runtime_skill_use_profile_registry_v1.hpp"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

bool near(float a, float b) { return std::fabs(a - b) < 0.0001f; }

bool contains_name(const std::string& blob, const std::string& name) {
    return blob.find(name) != std::string::npos;
}

std::string read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot read authored name table: " + path);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Usage: runtime_skill_use_profile_knight_v1_tests <windows-shared-assets>");
        const std::string root = argv[1];
        const auto skill_names = read_file(root + "/original-cache/data/pydata/skills_pyarraynames.bin");
        const auto class_names = read_file(root + "/original-cache/data/pydata/character_classes_pyarraynames.bin");

        // Seven admitted Knight-tree rows (Berserk, WhirlingDrift, DivineJudgement, three auras, Intimidation).
        const std::size_t knight_count = sizeof(kRuntimeSkillUseProfilesKnightV1) / sizeof(kRuntimeSkillUseProfilesKnightV1[0]);
        check(knight_count == 7, "Knight use profile row count changed");
        for (const auto& profile : kRuntimeSkillUseProfilesKnightV1) {
            check(contains_name(skill_names, profile.skill_name),
                  std::string("Skill name is absent from the authored SkillTable names: ") + profile.skill_name);
            check(contains_name(class_names, profile.class_token),
                  std::string("Class token is absent from the authored ClassTable names: ") + profile.class_token);
            if (profile.buff) {
                check(contains_name(class_names, profile.buff->buff_class),
                      std::string("Buff class is absent from the authored ClassTable names: ") + profile.buff->buff_class);
            }
            for (std::size_t i = 0; i < profile.volley_count; ++i) {
                check(contains_name(class_names, profile.volleys[i].class_token),
                      std::string("Volley class is absent from the authored ClassTable names: ") + profile.volleys[i].class_token);
            }
        }

        // Exact (name, script) admission through the registry; the base-table rows stay admitted too.
        check(find_skill_use_profile_registered_v1("Intimidation", "prince_paladin_offensive_aura") == nullptr,
              "Lookup must require the exact (name, script) pair");
        check(find_skill_use_profile_registered_v1("OffensiveAura", "prince_berseeker_warcry") == nullptr,
              "Lookup must require the exact (name, script) pair");
        check(find_skill_use_profile_registered_v1("Charge", "prince_warrior_charge") != nullptr,
              "Shared base table rows must stay admitted through the registry");
        check(find_skill_use_profile_registered_v1("Quickness", "prince_rogue_quickness") != nullptr,
              "Quickness must stay admitted through the registry");

        // prince_berseeker_whirling_drift.luac: RANGE 360, NoSort, OnSkill_ pops the OnPre list, Post ClearTarget,
        // no cooldown, no buff, combat rolls on the list.
        const auto* whirl = find_skill_use_profile_registered_v1("WhirlingDrift", "prince_berseeker_whirling_drift");
        check(whirl && near(whirl->pre_range, 360.0f) && whirl->pre_sort == SkillTargetSortV1::source_order &&
              !whirl->pre_look_at && !whirl->sets_cooldown &&
              whirl->use_targets == SkillUseTargetsV1::pre_list_all && whirl->post_clear_target &&
              whirl->rolls_combat && whirl->buff == nullptr,
              "WhirlingDrift profile does not match prince_berseeker_whirling_drift.luac");

        // prince_paladin_divine_judgement.luac: OnPre RANGE 600 ClosestFirst + SetSkillCooldown; OnSkill_ requery
        // TargetListSearch(600, 360) ClosestFirst; Post ClearTarget.
        const auto* divine = find_skill_use_profile_registered_v1("DivineJudgement", "prince_paladin_divine_judgement");
        check(divine && near(divine->pre_range, 600.0f) && divine->pre_sort == SkillTargetSortV1::closest_first &&
              divine->sets_cooldown && divine->use_targets == SkillUseTargetsV1::requery_at_use &&
              near(divine->use_range, 600.0f) && !divine->use_range_from_temp_prop1 &&
              divine->use_sort == SkillTargetSortV1::closest_first && divine->post_clear_target &&
              divine->rolls_combat && divine->buff == nullptr,
              "DivineJudgement profile does not match prince_paladin_divine_judgement.luac");

        // Paladin auras: no OnPre query, no combat roll, buff with BUFF_TIME 12000 and the BUFF_PERSIST FX set.
        struct AuraExpect { const char* skill; const char* script; const char* buff; const char* fx; };
        const AuraExpect auras[] = {
            {"OffensiveAura", "prince_paladin_offensive_aura", "Skill_Paladin_OffensiveAura",
             "skill_dh2_prince_warrior_paladin_offensive_aura_persist"},
            {"GuardingAura", "prince_paladin_guarding_aura", "Skill_Paladin_GuardingAura",
             "skill_dh2_prince_warrior_paladin_guarding_aura_persist"},
            {"PurifyingAura", "prince_paladin_purifying_aura", "Skill_Paladin_PurifyingAura",
             "skill_dh2_prince_warrior_paladin_purifying_aura_persist"},
        };
        for (const auto& expect : auras) {
            const auto* aura = find_skill_use_profile_registered_v1(expect.skill, expect.script);
            check(aura != nullptr, std::string("Aura row missing: ") + expect.skill);
            check(aura->pre_range == 0.0f && !aura->rolls_combat && !aura->sets_cooldown && aura->post_clear_target,
                  std::string("Aura row flags differ from the Lua: ") + expect.skill);
            check(aura->buff != nullptr && std::string(aura->buff->buff_class) == expect.buff &&
                  std::string(aura->buff->fx_set) == expect.fx && near(aura->buff->fixed_duration_ms, 12000.0f),
                  std::string("Aura buff arm differs from the Lua (BUFF_ID/BUFF_PERSIST/BUFF_TIME): ") + expect.skill);
        }

        // w2-paladin: BUFF_FX (skill_ prefix, played by PlayFX at Use on the caster) per aura; the ally FX2 (spell_ prefix)
        // is only played for allies, which the single-player port does not have.
        const struct { const char* skill; const char* script; const char* self_fx; float heal; } self_fx_expect[] = {
            {"OffensiveAura", "prince_paladin_offensive_aura", "skill_dh2_prince_warrior_paladin_offensive_aura", 0.0f},
            {"GuardingAura", "prince_paladin_guarding_aura", "skill_dh2_prince_warrior_paladin_guarding_aura", 0.0f},
            {"PurifyingAura", "prince_paladin_purifying_aura", "skill_dh2_prince_warrior_paladin_purifying_aura", 15.0f},
        };
        for (const auto& expect : self_fx_expect) {
            const auto* aura = find_skill_use_profile_registered_v1(expect.skill, expect.script);
            check(aura && aura->self_fx_name && std::string(aura->self_fx_name) == expect.self_fx,
                  std::string("Aura self BUFF_FX differs from the Lua: ") + expect.skill);
            check(aura && near(aura->self_heal_per_rank, expect.heal),
                  std::string("Aura heal differs from the Lua HEAL_AMOUNT: ") + expect.skill);
        }
        // Purifying Heal(): HP + rank * 15 capped at HP_Max (prince_paladin_purifying_aura.luac Heal()).
        check(near(self_heal_target_hp_v1(100.0f, 200.0f, 15.0f, 2), 130.0f), "heal: 100 + 2*15 = 130");
        check(near(self_heal_target_hp_v1(190.0f, 200.0f, 15.0f, 3), 200.0f), "heal caps at HP_Max");
        check(near(self_heal_target_hp_v1(200.0f, 200.0f, 15.0f, 1), 200.0f), "heal at full HP stays at HP_Max");
        check(near(self_heal_target_hp_v1(50.0f, 200.0f, 15.0f, 0), 50.0f), "rank 0 heals nothing");

        // prince_berseeker_warcry.luac: OnPre RANGE 600 ClosestFirst + LookAt, OnSkill_ acts on the OnPre target only;
        // no SkillCombatRoll in the Lua, so the row must not roll combat.
        // prince_berseeker_berserk.luac: OnPre TargetListSearch(200) FrontalFirst + LookAt(top), HP cost, UseMana; OnSkill_
        // re-searches (200, 360) and runs three volleys: stun (all Characters), damage (top), knockback (top); Post ClearTarget.
        const auto* berserk = find_skill_use_profile_registered_v1("Berserk", "prince_berseeker_berserk");
        check(berserk && near(berserk->pre_range, 200.0f) && berserk->pre_look_at &&
              berserk->pre_sort == SkillTargetSortV1::frontal_first && !berserk->sets_cooldown &&
              berserk->use_targets == SkillUseTargetsV1::requery_at_use && near(berserk->use_range, 200.0f) &&
              berserk->use_sort == SkillTargetSortV1::frontal_first && !berserk->rolls_objects &&
              berserk->post_clear_target && berserk->rolls_combat && berserk->buff == nullptr &&
              std::string(berserk->class_token) == "Skill_Berserk_Berserk_Damage",
              "Berserk profile does not match prince_berseeker_berserk.luac (OnPre/OnSkill_ range, sort, post)");
        check(berserk && berserk->volley_count == 3 && berserk->volleys &&
              std::string(berserk->volleys[0].class_token) == "Skill_Berserk_Berserk_Stun" && !berserk->volleys[0].top_target_only &&
              std::string(berserk->volleys[1].class_token) == "Skill_Berserk_Berserk_Damage" && berserk->volleys[1].top_target_only &&
              std::string(berserk->volleys[2].class_token) == "Skill_Berserk_Berserk_KnockBack" && berserk->volleys[2].top_target_only,
              "Berserk volley plan differs from the skill_inc cycle (stun all, damage top, knockback top)");
        check(berserk && berserk->self_hp_cost && near(berserk->self_hp_cost->gate_percent, 10.0f) &&
              near(berserk->self_hp_cost->divisor, 10.0f),
              "Berserk OnPre HP cost differs from `if HPPercentCurr > 10 then HP = health - health/10`");

        // prince_berseeker_warcry.luac: OnPre RANGE 600 ClosestFirst + LookAt; OnSkill_ pops the OnPre list (no re-search),
        // GetRand(1,4) scare/stun per non-boss Character, fear then stun sound; no SkillCombatRoll, no Post.
        const auto* warcry = find_skill_use_profile_registered_v1("Intimidation", "prince_berseeker_warcry");
        check(warcry && near(warcry->pre_range, 600.0f) && warcry->pre_look_at &&
              warcry->pre_sort == SkillTargetSortV1::closest_first &&
              warcry->use_targets == SkillUseTargetsV1::pre_list_all && !warcry->rolls_combat &&
              !warcry->post_clear_target && warcry->buff == nullptr,
              "Intimidation profile does not match prince_berseeker_warcry.luac (OnPre list scope, no roll)");
        check(warcry && warcry->status_arm && warcry->status_arm->faces == 4u && warcry->status_arm->stun_face == 3u &&
              std::string(warcry->status_arm->scare_sound) == "sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear" &&
              std::string(warcry->status_arm->stun_sound) == "sfx_skill_warrior_bsk_bloodmoon_howl_hit_stun" &&
              near(warcry->status_arm->fallback_ms, 2000.0f),
              "Intimidation status arm differs from GetRand(1,4), stun on 3, fallback 2000 ms, fear/stun cues");

        std::puts("PASS runtime_skill_use_profile_knight_v1: seven Knight-tree rows, exact admission, Berserk volley/HP-cost arms, Intimidation list scope and status arm");
    } catch (const std::exception& error) {
        std::fprintf(stderr, "FAIL runtime_skill_use_profile_knight_v1: %s\n", error.what());
        return 1;
    }
    return 0;
}
