// v21.8 skills-mage-sub: unit checks for the Illusionist/Necromancer use profile rows.
// The values are transcribed from the original skill Lua (see runtime_skill_use_profile_magesub_v1.hpp).
// argv[1] = windows-shared-assets root (original-cache/data/pydata name tables).
#include "runtime_skill_use_profile_magesub_v1.hpp"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

bool near(float a, float b) { return std::fabs(a - b) < 0.0001f; }

// Authored names are stored as raw strings in the pyarraynames tables; a verbatim occurrence proves the row exists.
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
        check(argc == 2, "Usage: runtime_skill_use_profile_magesub_v1_tests <windows-shared-assets>");
        const std::string root = argv[1];
        const auto skill_names = read_file(root + "/original-cache/data/pydata/skills_pyarraynames.bin");
        const auto class_names = read_file(root + "/original-cache/data/pydata/character_classes_pyarraynames.bin");

        const std::size_t table_size = sizeof(kRuntimeSkillUseProfilesMageSubV1) / sizeof(kRuntimeSkillUseProfilesMageSubV1[0]);
        // DeathClaw, Figments, Eclipse, TimeControl, Terror (v21.8 w2-illusionist adds Eclipse, TimeControl and Terror).
        check(table_size == 5, "Mage-subclass use profile admitted-row count changed");
        for (const auto& profile : kRuntimeSkillUseProfilesMageSubV1) {
            check(contains_name(skill_names, profile.skill_name),
                  std::string("Skill name is absent from the authored SkillTable names: ") + profile.skill_name);
            check(contains_name(class_names, profile.class_token),
                  std::string("Class token is absent from the authored ClassTable names: ") + profile.class_token);
        }

        // Admission is exact on (name, script); the base table keeps the Knight/Rogue rows only.
        check(find_skill_use_profile_magesub_v1("DeathClaw", "prince_necromancer_deathclaw") != nullptr,
              "DeathClaw must be admitted");
        check(find_skill_use_profile_magesub_v1("Figments", "prince_illusionist_figments") != nullptr,
              "Figments must be admitted");
        check(find_skill_use_profile_magesub_v1("DeathClaw", "prince_illusionist_figments") == nullptr,
              "Use profile lookup must require the exact (name, script) pair");
        // v21.8 w2-illusionist: Terror is the conversion variant of the per-target enemy buff arm (3 slots, FactionBuff).
        {
            const auto* terror = find_skill_use_profile_magesub_v1("Terror", "prince_illusionist_terror");
            check(terror && terror->enemy_buff && terror->enemy_buff->conversion_slots == 3 &&
                  std::string(terror->enemy_buff->buff_class) == "FactionBuff", "Terror must be admitted as a 3-slot FactionBuff conversion");
        }
        // v21.8 w2-illusionist: Eclipse and TimeControl are admitted by the per-target enemy buff arm (see below).
        const auto* eclipse = find_skill_use_profile_magesub_v1("Eclipse", "prince_illusionist_eclipse");
        const auto* timecontrol = find_skill_use_profile_magesub_v1("TimeControl", "prince_illusionist_timecontrol");
        check(eclipse != nullptr && timecontrol != nullptr, "Eclipse and TimeControl must be admitted");
        check(find_skill_use_profile_v1("Eclipse", "prince_illusionist_eclipse") == nullptr,
              "Eclipse must be admitted by the magesub table, not the shared table");
        if (eclipse) {
            check(eclipse->enemy_buff && std::string(eclipse->enemy_buff->buff_class) == "Skill_Illusionist_Eclipse" &&
                      eclipse->enemy_buff->duration_prop_index == 176 && eclipse->enemy_buff->duration_base_ms == 3000.0f &&
                      eclipse->enemy_buff->duration_inc_ms == 1000.0f && !eclipse->enemy_buff->skip_boss,
                  "Eclipse enemy buff: SnS_Duration with 3000+1000*L fallback, bosses NOT skipped");
            check(!eclipse->rolls_combat && eclipse->buff == nullptr && !eclipse->sets_cooldown,
                  "Eclipse: no roll, no caster buff, no cooldown");
            check(eclipse->target_fx_each_name && std::string(eclipse->target_fx_each_name) == "spell_dh2_prince_mage_illusionist_eclipse" &&
                      eclipse->target_fx_each_non_player_only,
                  "Eclipse: PlayFX per non-player Character target");
        }
        if (timecontrol) {
            check(timecontrol->enemy_buff && std::string(timecontrol->enemy_buff->buff_class) == "Buff_Illusionist_TimeControl" &&
                      timecontrol->enemy_buff->duration_prop_index < 0 && timecontrol->enemy_buff->duration_base_ms == 4000.0f &&
                      timecontrol->enemy_buff->duration_inc_ms == 1000.0f && timecontrol->enemy_buff->skip_boss &&
                      timecontrol->enemy_buff->skip_miniboss,
                  "TimeControl enemy buff: 4000+1000*L hard-coded duration, bosses and minibosses skipped");
            check(timecontrol->sets_cooldown && !timecontrol->rolls_combat,
                  "TimeControl: SetSkillCooldown in OnPre, no roll");
            check(timecontrol->use_sound_name && std::string(timecontrol->use_sound_name) == "sfx_skill_mage_ilu_hold_time_hit",
                  "TimeControl PlaySound is the _hit cue");
        }
        // v21.8 w2-illusionist Figments arms: stun 1500..4000 non-boss, FX buff with SnS_Dot_Duration (prop 181).
        if (const auto* figments_row = find_skill_use_profile_magesub_v1("Figments", "prince_illusionist_figments")) {
            check(figments_row->enemy_stun && figments_row->enemy_stun->min_ms == 1500.0f &&
                      figments_row->enemy_stun->max_ms == 4000.0f && figments_row->enemy_stun->skip_boss_and_miniboss,
                  "Figments stun: 1500..4000 ms, bosses and minibosses skipped");
            check(figments_row->enemy_buff && figments_row->enemy_buff->duration_prop_index == 181 &&
                      !figments_row->enemy_buff->skip_boss,
                  "Figments buff: SnS_Dot_Duration (181), applied to every target");
        }
        for (const auto& [name, script] : std::initializer_list<std::pair<const char*, const char*>>{
                 {"FireSkullRain", "prince_necromancer_fireskullrain"},
                 {"Pestilence", "prince_necromancer_pestilence"}, {"Rise", "prince_necromancer_rise"},
                 {"ColdRay", "prince_mage_coldray"}, {"Cyclone", "prince_mage_cyclone"}}) {
            check(find_skill_use_profile_magesub_v1(name, script) == nullptr,
                  std::string("Row must stay rejected: ") + name);
            // ColdRay and Cyclone are admitted by the shared (mage) table since v21.8-mage; the rest must stay out of it.
            const bool in_shared_table = std::string(name) == "ColdRay" || std::string(name) == "Cyclone";
            check((find_skill_use_profile_v1(name, script) != nullptr) == in_shared_table,
                  std::string("Base table must not admit: ") + name);
        }

        // prince_necromancer_deathclaw.luac: OnPre RANGE 680 / ANGLE 135 FrontalFirst + LookAt; SetSkillCooldown(1500)
        // (Lua constant); OnSkill_ re-search 680/135 with objects rolled; Post ClearTarget; no buff.
        const auto* death_claw = find_skill_use_profile_magesub_v1("DeathClaw", "prince_necromancer_deathclaw");
        check(death_claw && near(death_claw->pre_range, 680.0f) && near(death_claw->pre_cone_radians, 3.1415927f) &&
              death_claw->pre_sort == SkillTargetSortV1::frontal_first && death_claw->pre_look_at &&
              death_claw->sets_cooldown && near(death_claw->fixed_cooldown_ms, 1500.0f) &&
              death_claw->use_targets == SkillUseTargetsV1::requery_at_use && near(death_claw->use_range, 680.0f) &&
              !death_claw->use_range_from_temp_prop1 && near(death_claw->use_cone_radians, native_search_max_angle_v1(135.0f)) &&
              death_claw->use_sort == SkillTargetSortV1::frontal_first && !death_claw->use_look_at &&
              death_claw->rolls_objects && death_claw->post_clear_target && !death_claw->shield_variant &&
              death_claw->buff == nullptr && std::string(death_claw->class_token) == "Skill_Necromancer_DeathClaw",
              "DeathClaw Lua transcription changed");
        // prince_illusionist_figments.luac: no OnPre query; cooldown from the class projection (fixed 0);
        // OnSkill_ search 1000 / full angle, ClosestFirst, enemy characters only (object filter NONE); no Post.
        const auto* figments = find_skill_use_profile_magesub_v1("Figments", "prince_illusionist_figments");
        check(figments && near(figments->pre_range, 0.0f) && figments->sets_cooldown &&
              near(figments->fixed_cooldown_ms, 0.0f) && figments->use_targets == SkillUseTargetsV1::requery_at_use &&
              near(figments->use_range, 1000.0f) && near(figments->use_cone_radians, 3.1415927f) &&
              figments->use_sort == SkillTargetSortV1::closest_first && !figments->rolls_objects &&
              !figments->post_clear_target && figments->buff == nullptr &&
              std::string(figments->class_token) == "Skill_Illusionist_Figments",
              "Figments Lua transcription changed");

        // The base rows must keep a zero fixed cooldown (projection is used for them).
        for (const auto& profile : kRuntimeSkillUseProfilesV1)
            check(near(profile.fixed_cooldown_ms, 0.0f), "Base rows must not carry a fixed cooldown");
    } catch (const std::exception& error) {
        std::fprintf(stderr, "FAIL runtime_skill_use_profile_magesub_v1: %s\n", error.what());
        return 1;
    }
    std::puts("PASS runtime_skill_use_profile_magesub_v1: two authored rows (DeathClaw, Figments), exact admission, "
              "not-admitted rows rejected, Lua-transcribed values");
    return 0;
}
