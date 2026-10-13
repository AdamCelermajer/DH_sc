// v21.8 skills-a: unit checks for the data-driven source skill use profile table.
// The values are transcribed from the original skill Lua (see runtime_skill_use_profile_v1.hpp).
// argv[1] = windows-shared-assets root (original-cache/data/pydata name tables).
#include "runtime_skill_use_profile_v1.hpp"
#include "runtime_skill_use_profile_rogue_v1.hpp"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

bool near(float a, float b) { return std::fabs(a - b) < 0.0001f; }

// Authored names are stored as raw strings in the pyarraynames tables; a verbatim
// occurrence proves the row is present in the original data set.
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
        check(argc == 2, "Usage: runtime_skill_use_profile_v1_tests <windows-shared-assets>");
        const std::string root = argv[1];
        const auto skill_names = read_file(root + "/original-cache/data/pydata/skills_pyarraynames.bin");
        const auto class_names = read_file(root + "/original-cache/data/pydata/character_classes_pyarraynames.bin");

        const std::size_t table_size = sizeof(kRuntimeSkillUseProfilesV1) / sizeof(kRuntimeSkillUseProfilesV1[0]);
        check(table_size == 11, "Use profile table admitted-row count changed");
        for (const auto& profile : kRuntimeSkillUseProfilesV1) {
            check(contains_name(skill_names, profile.skill_name),
                  std::string("Skill name is absent from the authored SkillTable names: ") + profile.skill_name);
            check(contains_name(class_names, profile.class_token),
                  std::string("Class token is absent from the authored ClassTable names: ") + profile.class_token);
        }

        // Admission is exact on (name, script); unlisted rows stay rejected.
        check(find_skill_use_profile_v1("ViciousStrike", "prince_warrior_bashdown") == nullptr,
              "Use profile lookup must require the exact (name, script) pair");
        check(find_skill_use_profile_v1("Eclipse", "prince_illusionist_eclipse") == nullptr,
              "Subclass rows are outside this owner");

        // prince_warrior_bashdown.luac: RANGE 160; OnSkill_ uses the OnPre-selected target only.
        const auto* bashdown = find_skill_use_profile_v1("BashDown", "prince_warrior_bashdown");
        check(bashdown && near(bashdown->pre_range, 160.0f) && bashdown->pre_look_at &&
              bashdown->use_targets == SkillUseTargetsV1::pre_selected_single && !bashdown->sets_cooldown &&
              bashdown->post_clear_target,
              "BashDown Lua transcription changed");
        // prince_warrior_ground_slam.luac: no OnPre query; OnSkill_ range TempProp1, ClosestFirst, LookAt; cooldown.
        const auto* ground_slam = find_skill_use_profile_v1("GroundSlam", "prince_warrior_ground_slam");
        check(ground_slam && near(ground_slam->pre_range, 0.0f) && ground_slam->use_range_from_temp_prop1 &&
              ground_slam->use_sort == SkillTargetSortV1::closest_first && ground_slam->use_look_at &&
              ground_slam->sets_cooldown && ground_slam->post_clear_target,
              "GroundSlam Lua transcription changed");
        // prince_warrior_charge.luac: OnSkill_ NoSort RANGE 300 ANGLE 120; SetSkillCooldown; shield class variant.
        const auto* charge = find_skill_use_profile_v1("Charge", "prince_warrior_charge");
        check(charge && near(charge->use_range, 300.0f) && near(charge->use_cone_radians, 2.0943951f) &&
              charge->use_sort == SkillTargetSortV1::source_order && charge->sets_cooldown &&
              charge->shield_variant && !charge->post_clear_target, "Charge Lua transcription changed");
        // prince_rogue_jump_kick.luac: INIT_RANGE 250; OnSkill_ RANGE 160, ANGLE 270; ClearTarget Post.
        const auto* jump_kick = find_skill_use_profile_v1("JumpKick", "prince_rogue_jump_kick");
        check(jump_kick && near(jump_kick->pre_range, 250.0f) && near(jump_kick->use_range, 160.0f) &&
              near(jump_kick->use_cone_radians, 4.712389f) && jump_kick->post_clear_target &&
              !jump_kick->sets_cooldown, "JumpKick Lua transcription changed");
        // prince_rogue_vicious_strike.luac: SEARCH_RANGE 300 + LookAt; OnSkill_ RANGE 160, ANGLE 180;
        // character-only SkillCombatRoll; CLASS = _Stun; Post ClearTarget; no SetSkillCooldown.
        const auto* vicious = find_skill_use_profile_v1("ViciousStrike", "prince_rogue_vicious_strike");
        check(vicious && near(vicious->pre_range, 300.0f) && near(vicious->use_range, 160.0f) &&
              near(vicious->use_cone_radians, 3.1415927f) && vicious->pre_look_at &&
              vicious->use_targets == SkillUseTargetsV1::requery_at_use && !vicious->rolls_objects &&
              vicious->post_clear_target && !vicious->sets_cooldown && !vicious->shield_variant &&
              std::string(vicious->class_token) == "Skill_Rogue_ViciousStrike_Stun",
              "ViciousStrike Lua transcription changed");
        // prince_rogue_quickness.luac: no OnPre query or cooldown; the Use call is CreateBuff/ApplyBuff of
        // Skill_Rogue_Quickness_buff with FX spell_dh2_prince_rogue_quickness; TIME_BASE 4000 / TIME_INC 1000 fallback.
        const auto* quickness = find_skill_use_profile_v1("Quickness", "prince_rogue_quickness");
        check(quickness && quickness->buff && near(quickness->pre_range, 0.0f) && !quickness->sets_cooldown &&
              quickness->post_clear_target && std::string(quickness->class_token) == "Skill_Rogue_Quickness" &&
              std::string(quickness->buff->buff_class) == "Skill_Rogue_Quickness_buff" &&
              std::string(quickness->buff->fx_set) == "spell_dh2_prince_rogue_quickness" &&
              near(quickness->buff->fallback_base_ms, 4000.0f) && near(quickness->buff->fallback_inc_ms, 1000.0f),
              "Quickness Lua transcription changed");
        check(contains_name(class_names, quickness->buff->buff_class), "Quickness buff class is absent from the authored ClassTable names");
        for (const auto& profile : kRuntimeSkillUseProfilesV1)
            check((profile.buff != nullptr) == (std::string(profile.skill_name) == "Quickness"), "Only Quickness has a buff arm");
        // prince_rogue_roundhouse.luac: RANGE 220 FrontalFirst + LookAt; OnSkill_ pops the OnPre list (no search).
        const auto* roundhouse = find_skill_use_profile_v1("Roundhouse", "prince_rogue_roundhouse");
        check(roundhouse && near(roundhouse->pre_range, 220.0f) && roundhouse->pre_look_at &&
              roundhouse->use_targets == SkillUseTargetsV1::pre_list_all && roundhouse->rolls_objects &&
              roundhouse->post_clear_target && !roundhouse->sets_cooldown,
              "Roundhouse Lua transcription changed");
        // prince_mage_coldray.luac: RANGE 650, ANGLE 90 (MaxAngleSearch), FrontalFirst + LookAt; every popped target
        // (objects too) gets SkillCombatRoll; no SetSkillCooldown; Post ClearTarget.
        const auto* cold_ray = find_skill_use_profile_v1("ColdRay", "prince_mage_coldray");
        check(cold_ray && near(cold_ray->pre_range, 650.0f) && cold_ray->pre_look_at &&
              near(cold_ray->use_range, 650.0f) && near(cold_ray->use_cone_radians, 1.5707964f) &&
              cold_ray->use_targets == SkillUseTargetsV1::requery_at_use && cold_ray->rolls_objects &&
              cold_ray->post_clear_target && !cold_ray->sets_cooldown && !cold_ray->shield_variant &&
              std::string(cold_ray->class_token) == "Skill_Mage_ColdRay",
              "ColdRay Lua transcription changed");
        // prince_mage_chain_lightning.luac: OnPre RANGE 1000 FrontalFirst + LookAt; OnSkill_ pops the Pre list (first
        // Character) and SpawnSkillProjectile; no cooldown; Post ClearTarget; host-owned Use.
        const auto* chain = find_skill_use_profile_v1("ChainLightning", "prince_mage_chain_lightning");
        check(chain && near(chain->pre_range, 1000.0f) && chain->pre_look_at && chain->external_owner &&
              chain->use_targets == SkillUseTargetsV1::pre_list_all && !chain->sets_cooldown &&
              chain->post_clear_target && std::string(chain->class_token) == "Skill_Chain_Lightning",
              "ChainLightning Lua transcription changed");
        // prince_mage_cyclone.luac: OnPre LOOK_AT_RANGE 1500 ANGLE 360 + LookAt; Use = CreateTornado; Post ClearTarget.
        const auto* cyclone = find_skill_use_profile_v1("Cyclone", "prince_mage_cyclone");
        check(cyclone && near(cyclone->pre_range, 1500.0f) && cyclone->pre_look_at && cyclone->external_owner &&
              cyclone->post_clear_target && !cyclone->sets_cooldown &&
              std::string(cyclone->class_token) == "Skill_Cyclone", "Cyclone Lua transcription changed");
        // prince_mage_stone_escape.luac: no OnPre query, SetSkillCooldown commented out, no Post.
        const auto* stone = find_skill_use_profile_v1("StoneEscape", "prince_mage_stone_escape");
        check(stone && near(stone->pre_range, 0.0f) && !stone->pre_look_at && stone->external_owner &&
              !stone->post_clear_target && !stone->sets_cooldown &&
              std::string(stone->class_token) == "Skill_Mage_StoneEscape", "StoneEscape Lua transcription changed");
        check(!cold_ray->external_owner, "ColdRay is a plain SkillCombatRoll loop");

        // v21.8 skills-rogue: Assassin-tree rows live in their own table (runtime_skill_use_profile_rogue_v1.hpp).
        const std::size_t rogue_size = sizeof(kRuntimeSkillUseProfilesRogueV1) / sizeof(kRuntimeSkillUseProfilesRogueV1[0]);
        check(rogue_size == 2, "Rogue-tree use profile row count changed");
        for (const auto& profile : kRuntimeSkillUseProfilesRogueV1) {
            check(contains_name(skill_names, profile.skill_name),
                  std::string("Rogue-tree skill name is absent from the authored SkillTable names: ") + profile.skill_name);
            check(contains_name(class_names, profile.class_token),
                  std::string("Rogue-tree class token is absent from the authored ClassTable names: ") + profile.class_token);
            check(profile.buff && contains_name(class_names, profile.buff->buff_class),
                  std::string("Rogue-tree buff class is absent from the authored ClassTable names: ") + profile.skill_name);
        }
        // Rogue rows are admitted only through their exact pair and are not in the base table.
        check(find_skill_use_profile_v1("Blindness", "prince_assassin_blindness") == nullptr,
              "Blindness must not be in the base table");
        check(find_skill_use_profile_rogue_v1("Blindness", "prince_assassin_blindness") != nullptr,
              "Blindness row missing from the Rogue table");
        check(find_skill_use_profile_rogue_v1("Blindness", "prince_assassin_poison_sting") == nullptr,
              "Rogue table lookup must require the exact (name, script) pair");
        // prince_assassin_blindness.luac: BUFF_TIME = SnS_Duration (cell 176); no OnPre query, no cooldown; no Post.
        const auto* blindness = find_skill_use_profile_rogue_v1("Blindness", "prince_assassin_blindness");
        check(blindness && blindness->buff && near(blindness->pre_range, 0.0f) && !blindness->sets_cooldown &&
              !blindness->post_clear_target && std::string(blindness->class_token) == "Skill_Assassin_Blindness" &&
              std::string(blindness->buff->buff_class) == "Skill_Assassin_Blindness" &&
              std::string(blindness->buff->fx_set) == "spell_dh2_assassin_blindness_persist" &&
              blindness->buff->duration_prop_index == 176 && !blindness->rolls_objects,
              "Blindness Lua transcription changed");
        // prince_assassin_poison_sting.luac: BUFF_TIME = Dot_Duration (cell 125), 4000 ms fallback; Post ClearTarget.
        const auto* poison = find_skill_use_profile_rogue_v1("PoisonSting", "prince_assassin_poison_sting");
        check(poison && poison->buff && near(poison->pre_range, 0.0f) && !poison->sets_cooldown &&
              poison->post_clear_target && std::string(poison->class_token) == "Skill_Assassin_PoisonSting" &&
              std::string(poison->buff->buff_class) == "Skill_Assassin_PoisonSting" &&
              std::string(poison->buff->fx_set) == "skill_dh2_assassin_poison_sting_persist" &&
              poison->buff->duration_prop_index == 125 && near(poison->buff->fallback_base_ms, 4000.0f),
              "PoisonSting Lua transcription changed");
        // Not admitted: Assassination (kill branch), GrapplingHook and the Archer actives (projectile).
        check(find_skill_use_profile_rogue_v1("Assassination", "prince_assassin_assassination") == nullptr,
              "Assassination must stay out until its kill branch has a Session primitive");
        check(find_skill_use_profile_rogue_v1("GrapplingHook", "prince_assassin_grapling_hook") == nullptr,
              "GrapplingHook is BLOCKED-PROJECTILE");
        check(find_skill_use_profile_rogue_v1("Volley", "prince_archer_volley") == nullptr,
              "Archer projectile skills are BLOCKED-PROJECTILE");

        // v21.8 es-runtime: per-volley plan (Lua skill_inc cycle). Without a plan every Use is the row's own volley.
        const RuntimeSkillUseProfileV1* plain = find_skill_use_profile_v1("ViciousStrike", "prince_rogue_vicious_strike");
        check(plain != nullptr && select_volley_v1(*plain, 0) == nullptr && select_volley_v1(*plain, 7) == nullptr,
              "A profile without a volley plan selected a volley");
        const RuntimeSkillVolleyV1 plan[] = {{"Skill_Berserk_Berserk_Stun", false},
                                             {"Skill_Berserk_Berserk_Damage", true},
                                             {"Skill_Berserk_Berserk_KnockBack", true}};
        RuntimeSkillUseProfileV1 planned = *plain;
        planned.volleys = plan;
        planned.volley_count = 3;
        for (std::uint32_t cursor = 0; cursor < 7; ++cursor) {
            const auto* selected = select_volley_v1(planned, cursor);
            const auto& expected = plan[cursor % 3];
            check(selected == &plan[cursor % 3] && std::string(selected->class_token) == expected.class_token &&
                  selected->top_target_only == expected.top_target_only,
                  "Volley cycle does not wrap every three Uses at cursor " + std::to_string(cursor));
        }
        check(select_volley_v1(planned, 3) == &plan[0] && select_volley_v1(planned, 5) == &plan[2],
              "Volley cycle does not wrap at its length");
        planned.volley_count = 0;
        check(select_volley_v1(planned, 1) == nullptr, "A zero-length volley plan selected a volley");
    } catch (const std::exception& error) {
        std::fprintf(stderr, "FAIL runtime_skill_use_profile_v1: %s\n", error.what());
        return 1;
    }
    std::puts("PASS runtime_skill_use_profile_v1: eleven authored base rows (six hit skills, Quickness buff, ColdRay, ChainLightning, Cyclone, StoneEscape) plus two Rogue-tree buff rows (Blindness, PoisonSting), exact admission, Lua-transcribed values");
    return 0;
}
