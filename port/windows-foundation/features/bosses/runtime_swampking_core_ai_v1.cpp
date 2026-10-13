#include "runtime_swampking_core_ai_v1.hpp"

#include "../../content_paths.hpp"
#include "../../playable_actor_world.hpp"
#include "../generic_skills/runtime_skill_target_query_v1.hpp"
#include "../../../level-world/character_path_commands.hpp"
#include "../../../game-data/ai.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <regex>
#include <sstream>

namespace dh::foundation::bosses {
namespace {

using dh2::bosses::runtime_v1::AttackDecision;
using dh2::bosses::runtime_v1::DecisionKind;
using dh2::bosses::runtime_v1::Phase;

bool fail(std::string& error, const std::string& message) {
    error = message;
    return false;
}

bool same_lease(const std::weak_ptr<const void>& a, const std::weak_ptr<const void>& b) {
    return !a.owner_before(b) && !b.owner_before(a);
}

std::string strip_lua_comments(const std::string& text) {
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size();) {
        if (text.compare(i, 4, "--[[") == 0) {
            const auto end = text.find("]]", i + 4);
            i = end == std::string::npos ? text.size() : end + 2;
            continue;
        }
        if (text.compare(i, 2, "--") == 0) {
            const auto end = text.find('\n', i);
            i = end == std::string::npos ? text.size() : end;
            continue;
        }
        out.push_back(text[i++]);
    }
    return out;
}

// Body of a top-level `function NAME(...)` / `local function NAME(...)`: up to the first line that is exactly `end`.
bool function_body(const std::string& text, const std::string& name, std::string& body) {
    const std::regex header("(^|\\n)[ \\t]*(local[ \\t]+)?function[ \\t]+" + name + "[ \\t]*\\(");
    std::smatch match;
    if (!std::regex_search(text, match, header)) return false;
    const auto start = static_cast<std::size_t>(match.position(0) + match.length(0));
    std::size_t at = start;
    while (true) {
        const auto line = text.find('\n', at);
        if (line == std::string::npos) return false;
        std::size_t next = line + 1, finish = text.find('\n', next);
        std::string candidate = text.substr(next, finish == std::string::npos ? std::string::npos : finish - next);
        while (!candidate.empty() && (candidate.back() == '\r' || candidate.back() == ' ' || candidate.back() == '\t'))
            candidate.pop_back();
        if (candidate == "end") {
            body = text.substr(start, next - start);
            return true;
        }
        at = next;
    }
}

std::map<std::string, std::string> vf_table(const std::string& text) {
    std::map<std::string, std::string> result;
    const std::regex entry("AddToVFTable\\(\\s*\"(\\w+)\"\\s*,\\s*\"(\\w+)\"\\s*\\)");
    for (auto it = std::sregex_iterator(text.begin(), text.end(), entry); it != std::sregex_iterator(); ++it)
        result[(*it)[1].str()] = (*it)[2].str();
    return result;
}

bool numeric_constant(const std::string& text, const std::string& name, double& value) {
    const std::regex constant("local[ \\t]+" + name + "[ \\t]*=[ \\t]*(-?[0-9]+(\\.[0-9]*)?)");
    std::smatch match;
    if (!std::regex_search(text, match, constant)) return false;
    value = std::stod(match[1].str());
    return true;
}

// A StartTimerCB delay argument: a number or a numeric local constant.
bool delay_argument(const std::string& text, const std::string& token, double& value) {
    if (std::regex_match(token, std::regex("[0-9]+(\\.[0-9]*)?"))) { value = std::stod(token); return true; }
    return numeric_constant(text, token, value);
}

bool timer_delay(const std::string& text, const std::string& body, const std::string& callback, bool repeat, double& value) {
    const std::regex pattern("StartTimerCB\\(\\s*(\\w+)\\s*,\\s*" + std::string(repeat ? "true" : "false") + "\\s*,\\s*" + callback + "\\s*\\)");
    std::smatch match;
    return std::regex_search(body, match, pattern) && delay_argument(text, match[1].str(), value);
}

// Calls only: Trace("... DoSkill(1) ...") texts are string literals and are removed first.
std::vector<int> do_skill_arguments(const std::string& source) {
    std::vector<int> out;
    const std::string body = std::regex_replace(source, std::regex("\"[^\"\\n]*\""), "\"\"");
    const std::regex call("DoSkill\\(\\s*([0-9]+)\\s*\\)");
    for (auto it = std::sregex_iterator(body.begin(), body.end(), call); it != std::sregex_iterator(); ++it)
        out.push_back(std::stoi((*it)[1].str()));
    return out;
}

std::vector<std::string> string_arguments(const std::string& body, const std::string& call) {
    std::vector<std::string> out;
    const std::regex pattern(call + "\\(\\s*\"([^\"]+)\"");
    for (auto it = std::sregex_iterator(body.begin(), body.end(), pattern); it != std::sregex_iterator(); ++it)
        out.push_back((*it)[1].str());
    return out;
}

bool music_call(const std::string& body, const std::string& call, std::string& name, std::int32_t& fade) {
    const std::regex pattern(call + "\\(\\s*\"([^\"]+)\"\\s*,\\s*(true\\s*,\\s*)?(-?[0-9]+)");
    std::smatch match;
    if (!std::regex_search(body, match, pattern)) return false;
    name = match[1].str();
    fade = std::stoi(match[3].str());
    return true;
}

bool contains(const std::string& text, const char* needle) { return text.find(needle) != std::string::npos; }

const float* source_target_position(const ActorState& actor) noexcept {
    // GameObject::GetTargetPosition 0x3935dc: the cached node position only when the target node exists.
    if (actor.source_target_node180 && *actor.source_target_node180 != 0 && actor.source_target_position184)
        return actor.source_target_position184->data();
    return actor.transform.position.data();
}

float distance_squared(const float* a, const float* b) {
    const float dx = a[0] - b[0], dy = a[1] - b[1], dz = a[2] - b[2];
    return dx * dx + dy * dy + dz * dz;
}

bool read_text(const AssetCatalog& assets, const std::string& uri, std::string& text, std::string& error) {
    try {
        const auto bytes = read_content(assets, uri);
        text.assign(bytes.begin(), bytes.end());
        return true;
    } catch (const std::exception& problem) {
        error = problem.what();
        return false;
    }
}

std::int32_t signed_word(std::uint32_t word) { return static_cast<std::int32_t>(word); }

std::string trimmed_script_name(std::string script) {
    while (!script.empty() && (script.back() == '\0' || script.back() == ' ')) script.pop_back();
    const auto dot = script.rfind(".luac");
    if (dot != std::string::npos) script.erase(dot);
    const auto slash = script.find_last_of("/\\");
    if (slash != std::string::npos) script.erase(0, slash + 1);
    return script;
}

} // namespace

bool parse_swampking_core_ai_script_v1(const std::string& source, SwampkingCoreAiScriptV1& output, std::string& error) {
    const auto text = strip_lua_comments(source);
    const auto vf = vf_table(text);
    for (const char* event : {"OnInitFinal", "OnEnemySpotted", "OnTargetInRangedRange", "OnTargetInCloseRange", "OnUpdate",
                              "OnTargetDied", "OnDied", "OnEndOfAnim"})
        if (!vf.count(event)) return fail(error, std::string("AI script registers no ") + event + " callback");
    std::map<std::string, std::string> body;
    const auto need = [&](const std::string& name) {
        std::string b;
        if (!function_body(text, name, b)) return false;
        body[name] = b;
        return true;
    };
    for (const auto& name : {vf.at("OnEnemySpotted"), vf.at("OnTargetInRangedRange"), vf.at("OnTargetInCloseRange"),
                             vf.at("OnUpdate"), vf.at("OnDied"), vf.at("OnEndOfAnim")})
        if (!need(name)) return fail(error, "AI script callback body absent: " + name);
    for (const char* name : {"isAttackReady", "GoToDivePattern", "DiveAttackDecision", "MoveToPosition", "CheckForRandomSpawnPoint",
                             "CheckForClosestSpawnPoint", "searchForEfficientTarget", "SetSwampkingInIdleState",
                             "SetSwampkingInDiveState", "SetSwampkingInEmergeState", "SetSwampkingInIntimidateState",
                             "TargetAgain", "Do_MeleeAttackPhase1", "Do_MeleeAttackPhase2", "Do_RangedAttackPhase1",
                             "Do_RangedAttackPhase2"})
        if (!need(name)) return fail(error, std::string("AI script function absent (not the dive-boss shape): ") + name);
    SwampkingCoreAiScriptV1 next;
    double value = 0;
    const auto constant = [&](const char* name, double& out) { if (!numeric_constant(text, name, out)) return false; return true; };
    if (!constant("ATTACK_DELAY", next.attack_delay_ms) || !constant("GOTODIVE_DELAY", next.goto_dive_delay_ms) ||
        !constant("INDIVEPATTERN_DELAY", next.in_dive_pattern_delay_ms))
        return fail(error, "AI script timer constants absent");
    if (!constant("phase2_initPct", value)) return fail(error, "phase2_initPct absent");
    next.phase2_pct = static_cast<int>(value);
    if (!constant("normalMeleeAttack1_prob", value)) return fail(error, "normalMeleeAttack1_prob absent");
    next.close.phase1_normal_prob = static_cast<int>(value);
    if (!constant("firstMeleeAttack2_prob", value)) return fail(error, "firstMeleeAttack2_prob absent");
    next.close.phase2_first_prob = static_cast<int>(value);
    {
        std::smatch m;
        if (!std::regex_search(text, m, std::regex("local\\s+normalMeleeAttack2_prob\\s*=\\s*\\{\\s*([0-9]+)\\s*,\\s*([0-9]+)\\s*,\\s*([0-9]+)\\s*\\}")))
            return fail(error, "normalMeleeAttack2_prob table absent");
        next.close.phase2_prob_a = std::stoi(m[1].str());
        next.close.phase2_prob_b = std::stoi(m[2].str());
        if (!std::regex_search(text, m, std::regex("local\\s+sk_spawn_points\\s*=\\s*\\{([^}]*)\\}"))) return fail(error, "sk_spawn_points absent");
        const std::string list = m[1].str();
        const std::regex item("\"([^\"]+)\"");
        for (auto it = std::sregex_iterator(list.begin(), list.end(), item); it != std::sregex_iterator(); ++it)
            next.spawn_points.push_back((*it)[1].str());
        if (next.spawn_points.empty()) return fail(error, "sk_spawn_points is empty");
        const auto& update = body.at(vf.at("OnUpdate"));
        if (!std::regex_search(update, m, std::regex("HPPercentCurr\\s*<=\\s*([0-9]+)\\s+and\\s+\\(not\\s+weakAnim_flag\\)")))
            return fail(error, "OnUpdate weak threshold absent");
        next.weak_pct = std::stoi(m[1].str());
        if (!contains(update, "HPPercentCurr <= phase2_initPct")) return fail(error, "OnUpdate phase-2 switch absent");
        double attack = 0, dive = 0;
        if (!timer_delay(text, update, "isAttackReady", true, attack) || !timer_delay(text, update, "GoToDivePattern", true, dive) ||
            attack != next.attack_delay_ms || dive != next.goto_dive_delay_ms)
            return fail(error, "OnUpdate does not start the attack/dive timers from ATTACK_DELAY/GOTODIVE_DELAY");
        if (!contains(update, "GetPyCst(\"AIStates\", \"Idle\")")) return fail(error, "OnUpdate Idle admission absent");
    }
    if (!timer_delay(text, body.at("GoToDivePattern"), "GoToDivePattern", false, next.dive_retry_ms))
        return fail(error, "GoToDivePattern retry timer absent");
    {
        const auto& end = body.at(vf.at("OnEndOfAnim"));
        if (!timer_delay(text, end, "MoveToPosition", false, next.move_delay_ms) ||
            !timer_delay(text, end, "SetSwampkingInEmergeState", false, next.emerge_delay_ms) ||
            !timer_delay(text, end, "TargetAgain", false, next.retarget_delay_ms) ||
            !timer_delay(text, end, "DiveAttackDecision", false, next.decision_delay_ms))
            return fail(error, "OnEndOfAnim dive/emerge timers absent");
        std::smatch m;
        if (!std::regex_search(end, m, std::regex("StartTimerCB\\(\\s*([0-9]+)\\s*,\\s*false\\s*,\\s*function\\(\\)\\s*DoSkill\\(\\s*([0-9]+)\\s*\\)")))
            return fail(error, "OnEndOfAnim intimidate DoSkill timer absent");
        next.skill2_delay_ms = std::stod(m[1].str());
        next.intimidate_skill = std::stoi(m[2].str());
    }
    {
        const auto& decision = body.at("DiveAttackDecision");
        std::smatch m;
        if (!std::regex_search(decision, m, std::regex("diveCounter\\s*<\\s*([0-9]+)"))) return fail(error, "DiveAttackDecision limit absent");
        next.dives_before_reset = std::stoi(m[1].str());
        double pattern = 0, back = 0;
        if (!timer_delay(text, decision, "SetSwampkingInDiveState", true, pattern) || pattern != next.in_dive_pattern_delay_ms ||
            !timer_delay(text, decision, "GoToDivePattern", true, back) || back != next.goto_dive_delay_ms)
            return fail(error, "DiveAttackDecision timers absent");
    }
    const auto anim_of = [&](const std::string& b, std::string& out) {
        std::smatch m;
        if (!std::regex_search(b, m, std::regex("PlayAnim\\(\\s*GetPyOID\\(\\s*\"AnimDict\"\\s*,\\s*\"(\\w+)\"\\s*\\)"))) return false;
        out = m[1].str();
        return true;
    };
    {
        const auto& dive = body.at("SetSwampkingInDiveState");
        if (!anim_of(dive, next.dive_anim) || !contains(dive, "SetIsTargetable(false)")) return fail(error, "dive state animation absent");
        const auto sounds = string_arguments(dive, "PlaySound");
        if (sounds.size() != 1) return fail(error, "dive state sound absent");
        next.dive_sound = sounds.front();
        const auto& intimidate = body.at("SetSwampkingInIntimidateState");
        if (!anim_of(intimidate, next.intimidate_anim) || !std::regex_search(intimidate, std::regex("PlayAnim\\([^\\n]*nil\\s*,\\s*true\\s*\\)")))
            return fail(error, "intimidate animation (return to Idle) absent");
        next.intimidate_sounds = string_arguments(intimidate, "PlaySound");
        if (next.intimidate_sounds.empty()) return fail(error, "intimidate sounds absent");
    }
    {
        const auto emerge = do_skill_arguments(body.at("SetSwampkingInEmergeState"));
        const auto spit = do_skill_arguments(body.at("Do_RangedAttackPhase2"));
        const auto close = do_skill_arguments(body.at(vf.at("OnTargetInCloseRange")));
        const auto melee1 = do_skill_arguments(body.at("Do_MeleeAttackPhase1"));
        const auto melee2 = do_skill_arguments(body.at("Do_MeleeAttackPhase2"));
        if (emerge.size() != 1 || spit.size() != 1 || close.size() != 1 || melee1.size() != 3 || melee2.size() != 4)
            return fail(error, "DoSkill branch layout differs from the dive-boss shape");
        next.emerge_skill = emerge[0];
        next.spit_skill = spit[0];
        next.close.phase1_first_slot = close[0];
        next.close.phase1_normal_slot = melee1[1];
        next.close.phase1_other_slot = melee1[2];
        next.close.phase2_first_low_slot = melee2[0];
        next.close.phase2_first_high_slot = melee2[1];
        next.close.phase2_a_slot = melee2[2];
        next.close.phase2_b_slot = melee2[3];
        if (!contains(body.at("Do_RangedAttackPhase1"), "SetSwampkingInIntimidateState(2)")) return fail(error, "phase-1 ranged intimidate absent");
    }
    if (!music_call(body.at(vf.at("OnEnemySpotted")), "PlayMusic", next.spotted_music, next.spotted_music_fade_ms))
        return fail(error, "OnEnemySpotted music absent");
    if (!music_call(body.at(vf.at("OnDied")), "StopSound", next.died_stop_music, next.died_stop_fade_ms) ||
        !music_call(body.at(vf.at("OnDied")), "PlaySound", next.died_music, next.died_music_fade_ms))
        return fail(error, "OnDied music absent");
    if (!numeric_constant(text, "DIVE_DELAY", value) || value != next.emerge_delay_ms) return fail(error, "DIVE_DELAY absent");
    output = std::move(next);
    error.clear();
    return true;
}

bool parse_boss_skill_script_v1(const std::string& source, BossSkillScriptV1& output, std::string& error) {
    const auto text = strip_lua_comments(source);
    BossSkillScriptV1 next;
    std::smatch m;
    if (!std::regex_search(text, m, std::regex("local\\s+CLASS_ID\\s*=\\s*GetPyOID\\(\\s*\"ClassTable\"\\s*,\\s*\"(\\w+)\"")))
        return fail(error, "skill script has no CLASS_ID");
    next.class_token = m[1].str();
    double range = 0;
    if (numeric_constant(text, "RANGE", range)) next.range = static_cast<float>(range);
    else if (std::regex_search(text, m, std::regex("local\\s+RANGE\\s*=\\s*FromFixed\\(\\s*GetProp\\(\\s*GetPyStruct\\(\\s*\"CharacterProperties\"\\s*,\\s*\"(\\w+)\"")))
        next.range_field = m[1].str();
    else return fail(error, "skill script has no RANGE");
    const bool frontal = contains(text, "\"AITargetList_Sorting\", \"FrontalFirst\"") && contains(text, "\"AITargetList_CharacterFilter\", \"Enemy\"") &&
                         contains(text, "TargetListSearch(RANGE)");
    if (!frontal) return fail(error, "skill script has no FrontalFirst enemy pre-search");
    std::string pre, skill;
    if (!function_body(text, "OnPreSkill_", pre) || !function_body(text, "OnSkill_", skill)) return fail(error, "skill script has no Pre/Skill callback");
    if (contains(skill, "SpawnSkillProjectile(PROJECTILE_ID)")) {
        next.shape = BossSkillShapeV1::projectile_aoe;
        std::string delayed;
        if (!function_body(text, "OnDelayedSkill_", delayed)) return fail(error, "projectile skill has no delayed callback");
        if (!std::regex_search(text, m, std::regex("local\\s+PROJECTILE_ID\\s*=\\s*GetProp\\(\\s*GetPyStruct\\(\\s*\"CharacterProperties\"\\s*,\\s*\"(\\w+)\"")))
            return fail(error, "projectile skill has no PROJECTILE_ID field");
        next.projectile_field = m[1].str();
        if (!std::regex_search(text, m, std::regex("local\\s+AOE_CLASS_ID\\s*=\\s*GetPyOID\\(\\s*\"ClassTable\"\\s*,\\s*\"(\\w+)\"")))
            return fail(error, "projectile skill has no AOE_CLASS_ID");
        next.aoe_class_token = m[1].str();
        if (!numeric_constant(text, "AOE_RANGE", range)) return fail(error, "projectile skill has no AOE_RANGE");
        next.aoe_range = static_cast<float>(range);
        if (!std::regex_search(text, m, std::regex("local\\s+TIMERTRAP_ID\\s*=\\s*GetPyOID\\(\\s*\"TimerTraps\"\\s*,\\s*\"(\\w+)\"")))
            return fail(error, "projectile skill has no TIMERTRAP_ID");
        next.timer_trap = m[1].str();
        if (!std::regex_search(text, m, std::regex("local\\s+TIMERTRAP_DAM_ID\\s*=\\s*GetPyOID\\(\\s*\"GameObjectDamager\"\\s*,\\s*\"(\\w+)\"")))
            return fail(error, "projectile skill has no TIMERTRAP_DAM_ID");
        next.timer_trap_damager = m[1].str();
        if (!contains(delayed, "SkillCombatRoll(target)") || !contains(delayed, "SummonTimerTrap(TIMERTRAP_ID") ||
            !contains(delayed, "TargetListSearch(AOE_RANGE, 360") || !contains(delayed, "ApplyPropClass(AOE_CLASS_ID)"))
            return fail(error, "projectile skill delayed callback differs from the authored shape");
    } else if (contains(skill, "repeat") && contains(skill, "SkillCombatRoll(target)") && contains(skill, "PlayFX(")) {
        next.shape = BossSkillShapeV1::target_list_aoe;
        const auto pre_sounds = string_arguments(pre, "PlaySound");
        if (pre_sounds.size() == 1) next.pre_sound = pre_sounds.front();
        if (!std::regex_search(skill, m, std::regex("PlayFX\\(\\s*GetPyOID\\(\\s*\"AnimatedEffectTable\"\\s*,\\s*\"(\\w+)\"")))
            return fail(error, "AOE skill has no FX");
        next.skill_fx = m[1].str();
        const auto sounds = string_arguments(skill, "PlaySound");
        if (sounds.size() == 1) next.skill_sound = sounds.front();
    } else if (contains(skill, "if(target)") && contains(skill, "SkillCombatRoll(target)") && !contains(skill, "repeat") &&
               contains(pre, "if (dist > RANGE)")) {
        next.shape = BossSkillShapeV1::single_target;
    } else {
        return fail(error, "skill script shape is not single-target, target-list AOE or projectile AOE");
    }
    output = std::move(next);
    error.clear();
    return true;
}

std::string swampking_core_skill_state_v1(int slot) { return "SkillTree" + std::to_string(slot); }
std::string swampking_core_playanim_clip_v1(std::int32_t anim_dict_id) { return "ai:" + std::to_string(anim_dict_id); }

bool derive_swampking_core_v1(const SwampkingCoreSourcesV1& sources, const std::string& profile, bool& supported,
                              SwampkingCoreDefinitionV1& output, std::string& reason) {
    supported = false;
    reason.clear();
    if (!sources.assets || !sources.tables) { reason = "sources unavailable"; return true; }
    const auto& characters = sources.tables->properties.characters;
    const auto row = std::find(characters.names.begin(), characters.names.end(), profile);
    if (row == characters.names.end()) { reason = "CharacterTable row absent"; return true; }
    const auto index = static_cast<std::size_t>(row - characters.names.begin());
    const auto field = [&](const char* name) -> std::int32_t {
        const auto column = std::find(characters.fields.begin(), characters.fields.end(), std::string(name));
        return column == characters.fields.end() ? std::numeric_limits<std::int32_t>::min()
                                                  : characters.rows[index][static_cast<std::size_t>(column - characters.fields.begin())];
    };
    const auto* ai = dh2::data::ai_props(sources.tables->ai, field("AI"));
    if (!ai) { reason = "AI row absent"; return true; }
    SwampkingCoreDefinitionV1 next;
    next.profile = profile;
    next.ai_script = ai->script;
    std::string text, problem;
    if (!read_text(*sources.assets, "data/scripts/ai/" + next.ai_script + ".luac", text, problem)) {
        reason = "authored AI script unavailable: " + problem; return true;
    }
    if (!parse_swampking_core_ai_script_v1(text, next.ai, problem)) { reason = "AI script " + next.ai_script + ": " + problem; return true; }
    next.range_min = static_cast<float>(field("RangeMinDistance") >> 8);
    next.range_max = static_cast<float>(field("RangeMaxDistance") >> 8);
    if (field("RangeProjectileID") < 0 || !(next.range_max > next.range_min)) { reason = "CanRangeAttack is false or the range is invalid"; return true; }
    const auto tree = field("SkillTree");
    if (!sources.skills || tree < 0 || static_cast<std::size_t>(tree) >= sources.skills.lists().size()) { reason = "SkillTree list unavailable"; return true; }
    const auto& list = sources.skills.lists()[static_cast<std::size_t>(tree)];
    int highest = std::max({next.ai.spit_skill, next.ai.emerge_skill, next.ai.intimidate_skill, next.ai.close.phase1_first_slot,
                            next.ai.close.phase1_normal_slot, next.ai.close.phase1_other_slot, next.ai.close.phase2_a_slot,
                            next.ai.close.phase2_b_slot});
    if (highest < 0 || static_cast<std::size_t>(highest) >= list.size()) { reason = "DoSkill index outside the SkillTree list"; return true; }
    if (!sources.classes) { reason = "ClassTables unavailable"; return true; }
    const auto class_id = [&](const std::string& token) -> std::int32_t {
        const auto found = std::find(sources.classes->names.begin(), sources.classes->names.end(), token);
        return found == sources.classes->names.end() ? -1 : static_cast<std::int32_t>(found - sources.classes->names.begin());
    };
    for (std::size_t slot = 0; slot < list.size(); ++slot) {
        BossSkillDefinitionV1 skill;
        skill.slot = static_cast<int>(slot);
        skill.skill_table_id = list[slot];
        if (skill.skill_table_id < 0 || static_cast<std::size_t>(skill.skill_table_id) >= sources.skills.skills().size()) {
            reason = "SkillTable row unavailable"; return true;
        }
        const auto& record = sources.skills.skills()[static_cast<std::size_t>(skill.skill_table_id)];
        skill.skill_name = sources.skills.skill_names()[static_cast<std::size_t>(skill.skill_table_id)];
        skill.skill_script = trimmed_script_name(record.script);
        skill.animation_sequence_id = signed_word(record.scalar.words[1]);
        skill.element = signed_word(record.scalar.words[5]);
        skill.skill_flags = record.scalar.words[7];
        if (skill.animation_sequence_id < 0) { reason = "SkillTable Anim root is negative: " + skill.skill_name; return true; }
        if (!read_text(*sources.assets, "data/scripts/skills/" + skill.skill_script + ".luac", text, problem)) {
            reason = "authored skill script unavailable: " + problem; return true;
        }
        if (!parse_boss_skill_script_v1(text, skill.script, problem)) { reason = "skill script " + skill.skill_script + ": " + problem; return true; }
        skill.range = skill.script.range;
        if (!skill.script.range_field.empty()) {
            const auto value = field(skill.script.range_field.c_str());
            if (value == std::numeric_limits<std::int32_t>::min()) { reason = "skill RANGE field absent"; return true; }
            skill.range = static_cast<float>(value) / 256.0f;
        }
        skill.class_id = class_id(skill.script.class_token);
        if (skill.class_id < 0) { reason = "skill CLASS_ID absent from ClassTables: " + skill.script.class_token; return true; }
        if (skill.script.shape == BossSkillShapeV1::projectile_aoe) {
            skill.aoe_class_id = class_id(skill.script.aoe_class_token);
            if (skill.aoe_class_id < 0) { reason = "skill AOE_CLASS_ID absent from ClassTables"; return true; }
            const auto projectile = field(skill.script.projectile_field.c_str());
            if (!sources.projectiles || projectile < 0 || static_cast<std::size_t>(projectile) >= sources.projectiles->size()) {
                reason = "ProjectileTable unavailable"; return true;
            }
            const auto& projectile_row = (*sources.projectiles)[static_cast<std::size_t>(projectile)];
            if (projectile_row.laser || projectile_row.timer_ms <= 0 || !(projectile_row.velocity > 0)) {
                reason = "projectile row needs an unsupported laser/model-duration motion"; return true;
            }
            skill.projectile = projectile_row;
        }
        next.skills.push_back(std::move(skill));
    }
    const auto& clips = sources.tables->clips;
    const auto dict = [&](const std::string& name, std::int32_t& id, std::string& path) {
        const auto found = std::find(clips.names.begin(), clips.names.end(), name);
        if (found == clips.names.end()) return false;
        id = static_cast<std::int32_t>(found - clips.names.begin());
        path = clips.values[static_cast<std::size_t>(id)];
        return !path.empty();
    };
    if (!dict(next.ai.dive_anim, next.dive_anim_id, next.dive_anim_path) ||
        !dict(next.ai.intimidate_anim, next.intimidate_anim_id, next.intimidate_anim_path)) {
        reason = "PlayAnim AnimDict entries absent"; return true;
    }
    output = std::move(next);
    supported = true;
    return true;
}

bool append_swampking_core_states_v1(const spawn::ProfileDerivationTablesV1& tables, const SwampkingCoreDefinitionV1& def,
                                     OriginalMeleeActor& actor, std::string& error) {
    for (const auto& skill : def.skills)
        if (!spawn::append_sequence_state_v1(tables, swampking_core_skill_state_v1(skill.slot), {skill.animation_sequence_id}, actor, error))
            return false;
    error.clear();
    return true;
}

RuntimeSwampkingCoreAiV1::RuntimeSwampkingCoreAiV1(SwampkingCoreServicesV1 services,
                                                   std::map<ActorId, SwampkingCoreDefinitionV1> definitions)
    : services_(std::move(services)), definitions_(std::move(definitions)) {}

void RuntimeSwampkingCoreAiV1::log(std::string line) {
    if (log_.size() < 1024) log_.push_back(std::move(line));
}

std::vector<std::string> RuntimeSwampkingCoreAiV1::take_log() {
    std::vector<std::string> out;
    out.swap(log_);
    return out;
}

std::vector<enemy_ai::RangedProjectileV1> RuntimeSwampkingCoreAiV1::projectiles() const {
    std::vector<enemy_ai::RangedProjectileV1> out;
    for (const auto& p : projectiles_) out.push_back(p.motion);
    return out;
}

bool RuntimeSwampkingCoreAiV1::untargetable(ActorId id) const noexcept {
    const auto found = actors_.find(id);
    return found != actors_.end() && !found->second.targetable;
}

bool RuntimeSwampkingCoreAiV1::active_cast(ActorId id, std::uint64_t generation, std::int32_t& skill_table_id) const noexcept {
    const auto runtime = actors_.find(id);
    const auto definition = definitions_.find(id);
    if (runtime == actors_.end() || definition == definitions_.end() || !generation || runtime->second.cast.generation != generation ||
        runtime->second.cast.slot < 0 || static_cast<std::size_t>(runtime->second.cast.slot) >= definition->second.skills.size())
        return false;
    skill_table_id = definition->second.skills[static_cast<std::size_t>(runtime->second.cast.slot)].skill_table_id;
    return true;
}

bool RuntimeSwampkingCoreAiV1::view(ActorId id, ActorView& out) const {
    const auto found = actors_.find(id);
    if (found == actors_.end()) return false;
    const auto& r = found->second;
    out.state = r.sk;
    out.phase = r.phase;
    out.allowed = r.allowed;
    out.attack_ready = r.attack_flag;
    out.weak = r.weak;
    out.can_dive_now = r.can_dive_now;
    out.targetable = r.targetable;
    out.dive_counter = r.dive_counter;
    out.timers = std::count_if(r.timers.begin(), r.timers.end(), [](const Timer& t) { return t.id >= 0; });
    return true;
}

// CharTimers: TMR_Start takes the first inactive slot (CharTimers::_findTimerSlot 0x3db... lowest index, else push_back) and
// returns its index; TMR_Stop deactivates whatever occupies the slot. _commons StartTimerCB/StopTimerCB keep a
// TimerEvents[slot] table; OnTimer runs the callback, then clears a non-looping entry (even when the callback re-armed the
// same slot). Timer entries with id < 0 are free slots.
int RuntimeSwampkingCoreAiV1::start_timer(ActorRuntime& r, double ms, bool repeat, TimerAction action) {
    std::size_t slot = 0;
    while (slot < r.timers.size() && r.timers[slot].id >= 0) ++slot;
    Timer timer;
    timer.id = static_cast<int>(slot);
    timer.remaining_ms = ms;
    timer.period_ms = ms;
    timer.repeat = repeat;
    timer.fresh = true;
    timer.callback = true; // started during this frame's callbacks: first decremented on the next CharTimers::Update
    timer.action = action;
    if (slot == r.timers.size()) r.timers.push_back(timer);
    else r.timers[slot] = timer;
    return timer.id;
}

void RuntimeSwampkingCoreAiV1::stop_timer(ActorRuntime& r, std::optional<int>& handle) {
    if (handle && *handle >= 0 && static_cast<std::size_t>(*handle) < r.timers.size()) r.timers[static_cast<std::size_t>(*handle)].id = -1;
}

void RuntimeSwampkingCoreAiV1::cleanup(ActorRuntime& r) {
    // swampking_OnEndGameCleanUp: the named handles only (MoveToPosition's dive_timer1 and TargetAgain stay armed).
    for (auto* handle : {&r.timer_attack, &r.timer_diving, &r.timer_divepattern, &r.emerge_timer, &r.dive_timer, &r.skill2_timer}) {
        stop_timer(r, *handle);
        handle->reset();
    }
}

bool RuntimeSwampkingCoreAiV1::run_timers(CombatSession& session, ActorId id, const SwampkingCoreDefinitionV1& def,
                                          ActorRuntime& r, double dt, std::string& error) {
    const double dt_ms = dt * 1000.0;
    for (std::size_t slot = 0; slot < r.timers.size(); ++slot) {
        if (r.timers[slot].id < 0 || r.timers[slot].fresh) continue;
        r.timers[slot].remaining_ms -= dt_ms;
        int guard = 0;
        while (r.timers[slot].id >= 0 && r.timers[slot].remaining_ms <= 0.0 && guard++ < 16) {
            const auto action = r.timers[slot].action;
            const bool loop = r.timers[slot].repeat, callback = r.timers[slot].callback;
            if (loop) r.timers[slot].remaining_ms += r.timers[slot].period_ms;
            else r.timers[slot].id = -1; // the slot is free before the event is raised
            if (!callback) continue;     // TimerEvents[slot] was cleared: OnTimer finds no callback
            if (!fire(session, id, def, r, action, error)) return false;
            // OnTimer: a non-looping entry is cleared after its callback, even if the callback re-armed this same slot (the
            // engine timer keeps its slot and fires without a callback).
            if (!loop && r.timers[slot].id >= 0 && r.timers[slot].callback) {
                log("timer slot " + std::to_string(slot) + " re-armed inside its own one-shot callback: its callback is cleared (_commons OnTimer)");
                r.timers[slot].callback = false;
            }
        }
    }
    for (auto& timer : r.timers) timer.fresh = false;
    return true;
}

bool RuntimeSwampkingCoreAiV1::look_at(CombatSession& session, ActorId owner_id, ActorId target_id, std::string& error) {
    auto* owner = session.world() ? session.world()->find_actor(owner_id) : nullptr;
    const auto* target = session.world() ? session.world()->find_actor(target_id) : nullptr;
    if (!owner || !target) return true;
    dh2::character::LookAtState16 look{{owner->transform.position[0], owner->transform.position[1], owner->transform.position[2]},
                                       owner->transform.rotation[2]};
    if (dh2_character_look_at_point(&look, source_target_position(*target)))
        return fail(error, "Original GameObject LookAt(Point) kernel rejected the target");
    owner->transform.rotation[2] = look.heading_angle;
    return true;
}

bool RuntimeSwampkingCoreAiV1::frontal_search(CombatSession& session, ActorId id, float range, std::vector<ActorId>& list,
                                              std::string& error) {
    list.clear();
    if (!services_.search_population) return fail(error, "Boss skill pre-search requires the source population order");
    std::vector<ActorId> order;
    std::vector<std::pair<ActorId, bool>> facts;
    if (!services_.search_population(order, facts, error)) return false;
    std::vector<generic_skills::SkillTargetSourceFactsV1> source;
    source.reserve(facts.size());
    for (const auto& fact : facts) source.push_back({fact.first, true, false, false, true, fact.second});
    generic_skills::SkillActorTargetQueryV1 query;
    // Enemy filter, NONE objects, FrontalFirst, TargetListSearch(RANGE) (full radius).
    if (!generic_skills::query_source_character_targets_v1(session, id, order, source, true, range, 3.1415927410125732421875f,
            generic_skills::SkillTargetSortV1::frontal_first, query, error))
        return false;
    list = query.ordered_character_targets;
    return true;
}

bool RuntimeSwampkingCoreAiV1::search_efficient_target(CombatSession& session, ActorId id, const SwampkingCoreDefinitionV1& def,
                                                       ActorRuntime& r, std::string& error) {
    // searchForEfficientTarget: ClearAggro/ClearTarget; Player filter, NONE objects, ClosestFirst, TargetListSearch(RANGED_RANGE);
    // the first Player becomes the target (SetTarget, AddAggro, LookAt).
    auto* world = session.world();
    auto* owner = world->find_actor(id);
    r.target = invalid_actor_id;
    if (owner) owner->target_id = invalid_actor_id;
    if (!owner) return true;
    ActorId best = invalid_actor_id;
    float best_d2 = std::numeric_limits<float>::max();
    for (const auto& pair : world->actors()) {
        const auto* traits = world->traits(pair.first);
        if (!traits || !traits->is_player || !world->eligible_target(*owner, pair.second)) continue;
        const float d2 = distance_squared(owner->transform.position.data(), pair.second.transform.position.data());
        if (d2 <= def.range_max * def.range_max && d2 < best_d2) { best_d2 = d2; best = pair.first; }
    }
    if (best == invalid_actor_id) return true;
    r.target = best;
    owner->target_id = best;
    return look_at(session, id, best, error);
}

bool RuntimeSwampkingCoreAiV1::set_position(CombatSession& session, ActorId id, const std::string& waypoint, std::string& error) {
    // GameObject::_SetPosition 0x3928f0 by name: teleport, no state change; an unknown name does nothing.
    if (!services_.waypoint || !services_.warp) return fail(error, "Boss SetActorPosition requires the waypoint and position owners");
    std::array<float, 3> at{};
    bool found = false;
    if (!services_.waypoint(waypoint, at, found, error)) return false;
    if (!found) { log("SetActorPosition waypoint not found: " + waypoint); error.clear(); return true; }
    if (!services_.warp(id, at, error)) return false;
    ++report_.relocations;
    const auto* owner = session.world()->find_actor(id);
    log("SetActorPosition actor=" + std::to_string(id) + " waypoint=" + waypoint + " at=" + std::to_string(at[0]) + "," + std::to_string(at[1]) +
        (owner ? " now=" + std::to_string(owner->transform.position[0]) + "," + std::to_string(owner->transform.position[1]) : std::string()));
    return true;
}

bool RuntimeSwampkingCoreAiV1::play_anim(CombatSession& session, ActorId id, const SwampkingCoreDefinitionV1& def, ActorRuntime& r,
                                         bool dive, std::string& error) {
    // Character::_PlayAnim 0x3ba388 -> SM_SetAnimState: state 14 (CSAnim) without admission; the current skill is left.
    // The Session admits a skill departure only to Idle3 (or Dead12) under its Runtime transition handler; the CSAnim14 entry then
    // follows through play_actor_clip's own accepted source_program transition (Idle3 -> 14). Never ask for 14 directly.
    if (r.cast.active) {
        bool departed = false;
        if (!session.cancel_actor_source_sequence(id, session.actor_binding_lease(), r.cast.generation, 3, departed, error)) return false;
        r.cast.active = false;
    }
    if (!services_.play_clip) return fail(error, "Boss PlayAnim requires the scripted clip owner");
    const auto dict = dive ? def.dive_anim_id : def.intimidate_anim_id;
    const auto& path = dive ? def.dive_anim_path : def.intimidate_anim_path;
    std::int32_t duration = 0;
    std::string problem;
    if (!services_.play_clip(id, swampking_core_playanim_clip_v1(dict), path, duration, problem)) {
        log("PlayAnim not played actor=" + std::to_string(id) + " clip=" + std::to_string(dict) + ": " + problem);
        duration = 0;
    }
    r.clip.active = true;
    r.clip.remaining_ms = duration;
    r.clip.return_to_idle = !dive; // PlayAnim(..., nil, true): CSAnim::OnEvent 0x3c1a14 returns to Idle on event 34
    r.in_anim_state = true;
    log(std::string("PlayAnim actor=") + std::to_string(id) + " " + (dive ? def.ai.dive_anim : def.ai.intimidate_anim) +
        " duration_ms=" + std::to_string(duration));
    return true;
}

bool RuntimeSwampkingCoreAiV1::dive_state(CombatSession& session, ActorId id, const SwampkingCoreDefinitionV1& def, ActorRuntime& r,
                                          std::string& error) {
    // SetSwampkingInDiveState. The repeating dive-pattern timer (INDIVEPATTERN) calls it with no idle guard in the script; a
    // dead (or retired) boss has no CSAnim to enter, so the host drops the call there (the original CharTimers die with it).
    if (r.sk == ScriptState::dive) return true;
    {
        const auto* self = session.world()->find_actor(id);
        if (!self || !self->alive() || self->action == CharacterAction::dead) return true;
    }
    r.sk = ScriptState::dive;
    // ClearAggro(target): the aggro table only; the target stays.
    r.dive_anim = true;
    r.targetable = false;
    if (!play_anim(session, id, def, r, true, error)) return false;
    std::string presentation;
    if (services_.play_sound && !services_.play_sound(def.ai.dive_sound, presentation)) log("dive sound not played: " + presentation);
    ++report_.dives;
    log("dive actor=" + std::to_string(id) + " phase=" + std::to_string(r.phase) + " counter=" + std::to_string(r.dive_counter));
    return true;
}

bool RuntimeSwampkingCoreAiV1::idle_state(CombatSession& session, ActorId id, const SwampkingCoreDefinitionV1& def, ActorRuntime& r,
                                          std::string& error) {
    // SetSwampkingInIdleState.
    if (r.sk == ScriptState::dive) {
        const ActorId target = r.target;
        std::string spot;
        if (r.phase == 1) {
            // CheckForClosestSpawnPoint(target): the spawn point nearest to the target (first one without a target).
            spot = def.ai.spawn_points.front();
            const auto* t = target != invalid_actor_id ? session.world()->find_actor(target) : nullptr;
            if (t && services_.waypoint) {
                float best = std::numeric_limits<float>::max();
                for (const auto& name : def.ai.spawn_points) {
                    std::array<float, 3> at{};
                    bool found = false;
                    if (!services_.waypoint(name, at, found, error)) return false;
                    if (!found) continue;
                    const float d2 = distance_squared(at.data(), t->transform.position.data());
                    if (d2 < best) { best = d2; spot = name; }
                }
            }
        } else {
            spot = def.ai.spawn_points.front();
        }
        if (!set_position(session, id, spot, error)) return false;
        r.attack_flag = false;
        if (!do_skill(session, id, def, r, def.ai.spit_skill, error)) return false;
    }
    r.sk = ScriptState::idle;
    r.targetable = true;
    return true;
}

bool RuntimeSwampkingCoreAiV1::fire(CombatSession& session, ActorId id, const SwampkingCoreDefinitionV1& def, ActorRuntime& r,
                                    TimerAction action, std::string& error) {
    auto* owner = session.world()->find_actor(id);
    if (!owner) return true;
    const auto native_state = [&]() -> std::int32_t {
        if (!owner->alive() || owner->action == CharacterAction::dead) return 12;
        if (r.cast.active) return 6;
        if (r.in_anim_state) return 14;
        return session.original_actor_state(id);
    };
    switch (action) {
    case TimerAction::attack_ready: {
        // isAttackReady: LookAtTarget when targeted; ready = ST_IDLE and not Skill and not canDiveNow.
        if (r.target != invalid_actor_id && !look_at(session, id, r.target, error)) return false;
        r.attack_flag = r.sk == ScriptState::idle && native_state() != 6 && !r.can_dive_now;
        return true;
    }
    case TimerAction::go_to_dive: {
        r.attack_flag = false;
        if (native_state() != 3) {
            start_timer(r, def.ai.dive_retry_ms, false, TimerAction::go_to_dive);
            log("dive postponed actor=" + std::to_string(id) + " state=" + std::to_string(native_state()));
            return true;
        }
        r.can_dive_now = true;
        return dive_state(session, id, def, r, error);
    }
    case TimerAction::dive_state: return dive_state(session, id, def, r, error);
    case TimerAction::move_to_position: {
        // MoveToPosition -> CheckForRandomSpawnPoint: GetRand(1, n); weak -> the first spawn point.
        const auto n = static_cast<std::int32_t>(def.ai.spawn_points.size());
        const auto pick = services_.random ? services_.random(1, n) : 1;
        const auto& spot = r.weak ? def.ai.spawn_points.front()
                                  : def.ai.spawn_points[static_cast<std::size_t>(std::clamp(pick, 1, n) - 1)];
        return set_position(session, id, spot, error);
    }
    case TimerAction::emerge: {
        // SetSwampkingInEmergeState: ST_EMERGE; DoSkill(3); emergeAnim_flag.
        if (r.sk == ScriptState::emerge) return true;
        r.sk = ScriptState::emerge;
        ++report_.emerges;
        log("emerge actor=" + std::to_string(id));
        if (!do_skill(session, id, def, r, def.ai.emerge_skill, error)) return false;
        r.emerge_anim = true;
        return true;
    }
    case TimerAction::target_again: r.targetable = true; return true;
    case TimerAction::dive_decision: {
        // DiveAttackDecision.
        if (r.dive_counter < def.ai.dives_before_reset) {
            ++r.dive_counter;
            stop_timer(r, r.timer_diving);
            r.timer_divepattern = start_timer(r, def.ai.in_dive_pattern_delay_ms, true, TimerAction::dive_state);
            r.attack_flag = false;
            return do_skill(session, id, def, r, def.ai.spit_skill, error); // Do_RangedAttackPhase2
        }
        r.dive_counter = 0;
        r.can_dive_now = false;
        stop_timer(r, r.timer_divepattern);
        r.timer_divepattern.reset();
        r.timer_diving = start_timer(r, def.ai.goto_dive_delay_ms, true, TimerAction::go_to_dive);
        return idle_state(session, id, def, r, error);
    }
    case TimerAction::skill2: return do_skill(session, id, def, r, def.ai.intimidate_skill, error);
    }
    return true;
}

bool RuntimeSwampkingCoreAiV1::on_end_of_anim(CombatSession& session, ActorId id, const SwampkingCoreDefinitionV1& def,
                                              ActorRuntime& r, std::string& error) {
    // swampking_OnEndOfAnim (events 34/35 of any sequence; the flags select the branch).
    if (r.sk == ScriptState::dive && r.dive_anim) {
        r.dive_anim = false;
        if (!search_efficient_target(session, id, def, r, error)) return false;
        start_timer(r, def.ai.move_delay_ms, false, TimerAction::move_to_position);
        r.emerge_timer = start_timer(r, def.ai.emerge_delay_ms, false, TimerAction::emerge);
        start_timer(r, def.ai.retarget_delay_ms, false, TimerAction::target_again);
    }
    if (r.sk == ScriptState::emerge && r.emerge_anim) {
        r.emerge_anim = false;
        r.dive_timer = start_timer(r, def.ai.decision_delay_ms, false, TimerAction::dive_decision);
    }
    if (r.sk == ScriptState::intimidate && r.intimidate_flag > 0) {
        if (r.intimidate_flag == 1) {
            if (!idle_state(session, id, def, r, error)) return false;
        } else if (r.intimidate_flag == 2) {
            if (!search_efficient_target(session, id, def, r, error)) return false;
            r.skill2_timer = start_timer(r, def.ai.skill2_delay_ms, false, TimerAction::skill2);
            r.sk = ScriptState::idle;
        }
        r.intimidate_flag = 0;
    }
    return true;
}

bool RuntimeSwampkingCoreAiV1::roll(CombatSession& session, ActorId owner_id, ActorId target_id, const BossSkillDefinitionV1& skill,
                                    std::int32_t class_id, std::uint64_t generation, std::uint32_t event_index, std::string& error) {
    auto* world = session.world();
    auto* attacker = world->find_actor(owner_id);
    auto* victim = world->find_actor(target_id);
    const auto* props = world->combat_properties(owner_id);
    if (!attacker || !victim || !props || !world->eligible_target(*attacker, *victim)) {
        log("skill roll skipped " + skill.skill_name + " target=" + std::to_string(target_id) + ": attacker/target not eligible");
        return true;
    }
    if (!services_.characters || !services_.classes || !services_.property_rules)
        return fail(error, "Boss SkillCombatRoll requires the CharacterTable, ClassTables and property rules");
    // ApplyPropClass(CLASS_ID) on the skill sheet with SnS_Level 0 (a monster keeps the script's initial skill level, B154).
    dh2::data::PropertySheet sheet{};
    if (!enemy_ai::ranged_skill_formula_sheet_v1(*services_.characters, *services_.classes, *services_.property_rules,
            props->sheets.resolved, class_id, 0, sheet, error)) return false;
    CombatSessionSourceHit source;
    source.attacker = owner_id;
    source.target = target_id;
    source.binding_lease = session.actor_binding_lease();
    source.generation = generation;
    source.event_index = event_index;
    source.mask = skill.skill_flags | 0x08000000u;
    source.source_id = skill.skill_name;
    source.marker_name = "do_skill";
    source.category = props->facts.main_damage_class;
    source.element = skill.element;
    source.direct_amount = 0;
    source.attacker_formula_sheet = &sheet;
    DamageEvent receipt;
    std::string problem;
    if (!session.apply_source_result(source, receipt, problem)) {
        log("skill roll not applied " + skill.skill_name + ": " + problem);
        return true;
    }
    ++report_.hits;
    log("skill roll " + skill.skill_name + " target=" + std::to_string(target_id) + " applied=" + std::to_string(receipt.applied) +
        " removed=" + std::to_string(receipt.health_removed) + " outcomes=" + std::to_string(receipt.source_outcomes.value_or(0)) +
        " hp=" + std::to_string(victim->health) + "/" + std::to_string(victim->max_health));
    return true;
}

bool RuntimeSwampkingCoreAiV1::do_skill(CombatSession& session, ActorId id, const SwampkingCoreDefinitionV1& def, ActorRuntime& r,
                                        int slot, std::string& error) {
    auto* world = session.world();
    auto* owner = world->find_actor(id);
    if (!owner || slot < 0 || static_cast<std::size_t>(slot) >= def.skills.size()) return true; // _DoSkill rejects a bad index
    const auto& skill = def.skills[static_cast<std::size_t>(slot)];
    // AI_IsSkillUsable 0x3d8358 / SM_SetSkillState: rejected while using a skill or in Cast/Scared/Stunned/KnockedBack/Dead/Spawn/
    // Limbus/PreSpawn; accepted from Idle, Move, Attack, Injured, Interact, Anim14 and Reviving.
    const std::int32_t state = r.cast.active ? 6 : (r.in_anim_state ? 14 : session.original_actor_state(id));
    if (!owner->alive() || r.cast.active || state == 0 || state == 1 || state == 7 || state == 8 || state == 9 || state == 10 ||
        state == 12 || state == 17) {
        ++report_.cast_rejected;
        log("DoSkill(" + std::to_string(slot) + ") rejected actor=" + std::to_string(id) + " state=" + std::to_string(state));
        return true;
    }
    const auto* plan = session.actor_source_plan(id);
    const auto* policies = session.source_sequence_policies();
    const OriginalCombatSequencePlan* sequence = nullptr;
    if (plan) for (const auto& candidate : plan->sequences)
        if (candidate.id == skill.animation_sequence_id && candidate.state == swampking_core_skill_state_v1(slot)) { sequence = &candidate; break; }
    if (!plan || !policies || !sequence) {
        ++report_.cast_rejected;
        log("DoSkill(" + std::to_string(slot) + ") rejected: Anim root " + std::to_string(skill.animation_sequence_id) + " not in the actor bank");
        return true;
    }
    // Leaving CSAnim (14, a finished dive) for Skill: the port publishes the scripted clip state through the Idle owner first.
    if (r.in_anim_state) {
        r.clip.active = false;
        if (services_.put_idle && !services_.put_idle(id, error)) return false;
        r.in_anim_state = false;
    }
    // OnPreSkill_.
    Cast cast;
    cast.slot = slot;
    auto& local = r.skill_local_target[slot];
    std::string presentation;
    if (skill.script.shape == BossSkillShapeV1::target_list_aoe) {
        if (!skill.script.pre_sound.empty() && services_.play_sound && !services_.play_sound(skill.script.pre_sound, presentation))
            log("skill pre sound not played: " + presentation);
        if (!frontal_search(session, id, skill.range, cast.list, error)) return false;
        local = cast.list.empty() ? invalid_actor_id : cast.list.front();
        if (local != invalid_actor_id && !look_at(session, id, local, error)) return false;
    } else {
        if (r.target != invalid_actor_id && world->find_actor(r.target)) {
            local = r.target;
            if (!look_at(session, id, local, error)) return false;
        }
        const auto* t = local != invalid_actor_id ? world->find_actor(local) : nullptr;
        if (!t) {
            log("skill " + skill.skill_name + " OnPreSkill has no target (Lua error in the source): no search");
        } else {
            const float d = std::sqrt(distance_squared(owner->transform.position.data(), t->transform.position.data()));
            if (d > skill.range) {
                if (!frontal_search(session, id, skill.range, cast.list, error)) return false;
                if (!cast.list.empty()) {
                    local = cast.list.front();
                    if (!look_at(session, id, local, error)) return false;
                } else if (skill.script.shape == BossSkillShapeV1::single_target) {
                    local = invalid_actor_id; // target = nil
                }
            }
        }
    }
    cast.target = local;
    const auto generation = next_generation_++;
    cast.generation = generation;
    OriginalAttackSelection selection;
    selection.state = sequence->state;
    selection.variant = sequence->variant;
    if (sequence->type == 2) selection.group_path = {0};
    CombatSessionStateAnimationServices services;
    services.event = [this, &session, id, generation, defp = &def](ActorId actor, const RetainedAnimationEvent& event, std::string& e) {
        if (actor != id) return fail(e, "Boss skill event reached a foreign actor");
        return skill_event(session, id, *defp, generation, event.name, e);
    };
    services.finished = [this, &session, id, generation, defp = &def](ActorId actor, std::string& e) {
        if (actor != id) return fail(e, "Boss skill completion reached a foreign actor");
        auto found = actors_.find(id);
        if (found == actors_.end() || found->second.cast.generation != generation) { e.clear(); return true; }
        found->second.cast.active = false;
        // The sequence end raises event 34: Lua OnEndOfAnim (the emerge branch).
        return on_end_of_anim(session, id, *defp, found->second, e);
    };
    services.departed = [this, &session, id, generation, defp = &def](ActorId actor, std::int32_t from, std::int32_t to, std::string& e) {
        if (actor != id) return fail(e, "Boss skill departure reached a foreign actor");
        auto found = actors_.find(id);
        if (found == actors_.end() || found->second.cast.generation != generation || !found->second.cast.active) { e.clear(); return true; }
        found->second.cast.active = false;
        log("skill interrupted actor=" + std::to_string(id) + " from=" + std::to_string(from) + " to=" + std::to_string(to));
        // The interrupting animation's end raises OnEndOfAnim in the source; the port's Injured clip is zero-length (authored
        // swampking_hurt_01/02 carry no key), so it is raised now. Death (12) stops the AI.
        if (to == 12) { e.clear(); return true; }
        return on_end_of_anim(session, id, *defp, found->second, e);
    };
    services.checkpoint = [](std::string& e) { e.clear(); return true; };
    r.cast = cast;
    r.cast.active = true;
    const CombatSessionSourceSequencePolicy policy{6, 0x6341u, generation};
    std::string problem;
    if (!session.play_actor_source_sequence(id, *plan, *policies, selection, std::move(services), policy, problem)) {
        r.cast.active = false;
        ++report_.cast_rejected;
        log("DoSkill(" + std::to_string(slot) + ") rejected actor=" + std::to_string(id) + ": " + problem);
        return true;
    }
    ++report_.casts;
    log("DoSkill(" + std::to_string(slot) + ") actor=" + std::to_string(id) + " skill=" + skill.skill_name + " sequence=" +
        std::to_string(skill.animation_sequence_id) + " target=" + std::to_string(cast.target) + " list=" + std::to_string(cast.list.size()) +
        " phase=" + std::to_string(r.phase));
    return true;
}

bool RuntimeSwampkingCoreAiV1::skill_event(CombatSession& session, ActorId id, const SwampkingCoreDefinitionV1& def,
                                           std::uint64_t generation, const std::string& name, std::string& error) {
    if (name != "do_skill") { error.clear(); return true; }
    auto found = actors_.find(id);
    if (found == actors_.end() || found->second.cast.generation != generation || found->second.cast.used) { error.clear(); return true; }
    auto& r = found->second;
    r.cast.used = true;
    const auto& skill = def.skills[static_cast<std::size_t>(r.cast.slot)];
    std::string presentation;
    switch (skill.script.shape) {
    case BossSkillShapeV1::single_target:
        if (r.cast.target != invalid_actor_id) return roll(session, id, r.cast.target, skill, skill.class_id, generation, 0, error);
        return true;
    case BossSkillShapeV1::target_list_aoe: {
        if (r.cast.target != invalid_actor_id) {
            std::uint32_t index = 0;
            for (const auto target : r.cast.list) {
                if (!roll(session, id, target, skill, skill.class_id, generation, index++, error)) return false;
                ++report_.aoe_hits;
            }
        }
        auto* owner = session.world()->find_actor(id);
        const std::int32_t fx = services_.fx_id ? services_.fx_id(skill.script.skill_fx) : -1;
        if (owner && fx >= 0 && services_.play_fx && !services_.play_fx(fx, owner->transform.position, presentation))
            log("skill FX not played: " + presentation);
        if (fx < 0) log("skill FX unavailable: " + skill.script.skill_fx);
        if (!skill.script.skill_sound.empty() && services_.play_sound && !services_.play_sound(skill.script.skill_sound, presentation))
            log("skill sound not played (" + skill.script.skill_sound + "): " + presentation);
        return true;
    }
    case BossSkillShapeV1::projectile_aoe: {
        if (r.cast.target != invalid_actor_id && !look_at(session, id, r.cast.target, error)) return false;
        const auto* owner = session.world()->find_actor(id);
        if (!owner || !skill.projectile) return true;
        Projectile p;
        p.slot = skill.slot;
        p.generation = generation;
        p.motion.id = next_projectile_++;
        p.motion.owner = id;
        const float* origin = source_target_position(*owner);
        p.motion.origin = {origin[0], origin[1], origin[2]};
        p.motion.position = p.motion.origin;
        p.motion.forward = enemy_ai::source_look_vector_v1(owner->transform.rotation[2]);
        p.motion.speed = skill.projectile->velocity;
        p.motion.remaining_ms = skill.projectile->timer_ms;
        p.motion.model_uri = skill.projectile->model_uri;
        projectiles_.push_back(p);
        ++report_.projectiles;
        log("projectile spawn actor=" + std::to_string(id) + " id=" + std::to_string(p.motion.id) + " row=" + skill.projectile->name);
        return true;
    }
    }
    return true;
}

bool RuntimeSwampkingCoreAiV1::update_projectiles(CombatSession& session, std::int32_t dt_ms, std::string& error) {
    auto* world = session.world();
    const auto* tables = session.original_ai_tables();
    for (auto it = projectiles_.begin(); it != projectiles_.end();) {
        auto& p = *it;
        const auto def = definitions_.find(p.motion.owner);
        if (def == definitions_.end() || p.slot < 0 || static_cast<std::size_t>(p.slot) >= def->second.skills.size() ||
            !def->second.skills[static_cast<std::size_t>(p.slot)].projectile) { it = projectiles_.erase(it); continue; }
        const auto& skill = def->second.skills[static_cast<std::size_t>(p.slot)];
        const auto& row = *skill.projectile;
        const bool expired = enemy_ai::advance_ranged_projectile_v1(p.motion, row, dt_ms);
        const auto* owner = world->find_actor(p.motion.owner);
        ActorId hit = invalid_actor_id;
        for (const auto& pair : world->actors()) {
            if (pair.first == p.motion.owner || !owner) continue;
            const auto& candidate = pair.second;
            if (!candidate.alive() || candidate.action == CharacterAction::dead) continue;
            const auto* ct = world->traits(pair.first);
            const auto* ot = world->traits(p.motion.owner);
            if (!ct || !ot || !ct->targetable) continue;
            if (!row.friendly_fire && !(tables && dh2::data::ai_enemy(*tables, owner->faction_id, candidate.faction_id, ot->is_player, ct->is_player)))
                continue;
            const float radius = services_.body_radius ? services_.body_radius(pair.first) : 0.0f;
            if (radius > 0 && enemy_ai::ranged_projectile_contact_v1(p.motion, candidate.transform.position, radius)) { hit = pair.first; break; }
        }
        if (hit != invalid_actor_id) {
            ++report_.projectile_hits;
            std::string presentation;
            const auto* victim = world->find_actor(hit);
            std::array<float, 3> at{};
            if (victim) {
                const float* tp = source_target_position(*victim);
                at = {tp[0], tp[1], tp[2]};
                if (row.impact_fx >= 0 && services_.play_fx && !services_.play_fx(row.impact_fx, at, presentation)) log("impact FX not played: " + presentation);
            }
            if (row.impact_sound >= 0 && services_.play_sound_id && !services_.play_sound_id(row.impact_sound, presentation))
                log("impact sound not played: " + presentation);
            // OnDelayedSkill_(target): ApplyPropClass(CLASS_ID); SkillCombatRoll(target); SummonTimerTrap at target:GetPosition();
            // then the AOE: Enemy filter, NoSort, TargetListSearch(AOE_RANGE, 360, x, y, z), ApplyPropClass(AOE_CLASS_ID), roll
            // every other listed enemy.
            if (!roll(session, p.motion.owner, hit, skill, skill.class_id, p.generation, 0, error)) return false;
            const auto* v = world->find_actor(hit);
            const std::array<float, 3> centre = v ? v->transform.position : at;
            if (services_.summon_timer_trap) {
                if (!services_.summon_timer_trap(skill.script.timer_trap, skill.script.timer_trap_damager, p.motion.owner, centre, presentation))
                    log("SummonTimerTrap " + skill.script.timer_trap + " not created: " + presentation);
                else ++report_.traps;
            } else {
                log("SummonTimerTrap " + skill.script.timer_trap + " has no TimerTrap owner in this build");
            }
            std::uint32_t index = 1;
            const auto* o = world->find_actor(p.motion.owner);
            for (const auto& pair : world->actors()) {
                if (pair.first == hit || pair.first == p.motion.owner || !o) continue;
                if (!world->eligible_target(*o, pair.second)) continue;
                if (distance_squared(pair.second.transform.position.data(), centre.data()) > skill.script.aoe_range * skill.script.aoe_range) continue;
                if (!roll(session, p.motion.owner, pair.first, skill, skill.aoe_class_id, p.generation, index++, error)) return false;
                ++report_.aoe_hits;
            }
            if (row.disappear_on_hit) { it = projectiles_.erase(it); continue; }
        }
        if (expired) {
            std::string presentation;
            if (row.expire_fx >= 0 && services_.play_fx && !services_.play_fx(row.expire_fx, p.motion.position, presentation))
                log("expire FX not played: " + presentation);
            log("projectile expired id=" + std::to_string(p.motion.id));
            it = projectiles_.erase(it);
            continue;
        }
        ++it;
    }
    error.clear();
    return true;
}

bool RuntimeSwampkingCoreAiV1::tick_actor(CombatSession& session, ActorId id, const SwampkingCoreDefinitionV1& def,
                                          ActorRuntime& r, double dt, bool ai, std::string& error) {
    auto* world = session.world();
    auto* owner = world->find_actor(id);
    if (!owner) return true;
    std::string presentation;
    if (!owner->alive() || owner->action == CharacterAction::dead) {
        if (!r.dead_seen) {
            // swampking_OnDied: StopSound(music); PlaySound(hub music); swampking_OnEndGameCleanUp.
            r.dead_seen = true;
            r.cast.active = false;
            r.clip.active = false;
            r.target = invalid_actor_id;
            if (services_.stop_music && !services_.stop_music(def.ai.died_stop_music, def.ai.died_stop_fade_ms, presentation))
                log("death music stop not applied: " + presentation);
            if (services_.play_music && !services_.play_music(def.ai.died_music, def.ai.died_music_fade_ms, presentation))
                log("death music not played: " + presentation);
            cleanup(r);
            ++report_.deaths;
            log("died actor=" + std::to_string(id));
        }
        return true;
    }
    r.dead_seen = false;
    // Character::CanUpdate 0x3a52a4 / ObjectBase::TestCullingBeforeUpdate 0x33de90: a culled character skips Character::Update
    // until it has once been inside the camera frustum (latch 0 -> 1 -> 2). The port has no per-object frustum latch for
    // this owner; it latches once the local player comes within the actor's AI ViewRadius (approximation, see the report).
    if (!r.awake) {
        const auto* player = world->find_actor(session.player_id());
        const auto* props0 = world->combat_properties(id);
        const auto* tables0 = session.original_ai_tables();
        const auto* ai0 = props0 && tables0 ? dh2::data::ai_props(*tables0, props0->sheets.resolved[1]) : nullptr;
        if (!player || !ai0 ||
            distance_squared(player->transform.position.data(), owner->transform.position.data()) > ai0->view_radius * ai0->view_radius)
            return true;
        r.awake = true;
        log("awake actor=" + std::to_string(id) + " (player inside ViewRadius " + std::to_string(ai0->view_radius) + ")");
    }
    // CharTimers::Update runs inside Character::Update before CharAI::Update, also while CharAI is blocked.
    if (!run_timers(session, id, def, r, dt, error)) return false;
    owner = world->find_actor(id);
    if (!owner) return true;
    // Scripted PlayAnim clip end: event 34 -> OnEndOfAnim; CSAnim returns to Idle only for PlayAnim(..., true).
    if (r.clip.active) {
        r.clip.remaining_ms -= dt * 1000.0;
        if (r.clip.remaining_ms <= 0.0) {
            r.clip.active = false;
            if (r.clip.return_to_idle) {
                if (services_.put_idle && !services_.put_idle(id, error)) return false;
                r.in_anim_state = false;
            }
            if (!on_end_of_anim(session, id, def, r, error)) return false;
        }
    }
    if (!ai) return true;
    // swampking_OnUpdate.
    const float hp = owner->max_health > 0 ? owner->health / owner->max_health * 100.0f : 0.0f;
    if (hp <= def.ai.weak_pct && !r.weak) { r.weak = true; ++report_.weak; log("weak actor=" + std::to_string(id) + " hp%=" + std::to_string(hp)); }
    const auto next_phase = dh2::bosses::runtime_v1::phase_after_update(r.phase == 1 ? Phase::phase1 : Phase::phase2, hp, def.ai.phase2_pct);
    if (r.phase == 1 && next_phase == Phase::phase2) {
        r.phase = 2;
        r.attack_flag = false;
        ++report_.phase2;
        log("phase 2 actor=" + std::to_string(id) + " hp%=" + std::to_string(hp));
    }
    if (r.target != invalid_actor_id) {
        const auto* t = world->find_actor(r.target);
        if (!t || !t->alive() || t->action == CharacterAction::dead) {
            // OnUpdate: StopSound/PlayMusic, stop the attack/dive timers; OnTargetDied: Stop, ClearTarget, musicStarted=false.
            if (services_.stop_music && !services_.stop_music(def.ai.spotted_music, def.ai.died_stop_fade_ms, presentation)) log("music stop: " + presentation);
            if (services_.play_music && !services_.play_music(def.ai.died_music, def.ai.died_music_fade_ms, presentation)) log("music: " + presentation);
            if (r.timer_attack) {
                stop_timer(r, r.timer_attack);
                stop_timer(r, r.timer_diving);
                r.timer_attack.reset();
                r.timer_diving.reset();
            }
            r.target = invalid_actor_id;
            owner->target_id = invalid_actor_id;
            r.music_started = false;
            log("target died actor=" + std::to_string(id));
        }
    }
    if (!r.allowed) {
        if (!r.in_anim_state && !r.cast.active && session.original_actor_state(id) == 3) {
            r.allowed = true;
            r.timer_attack = start_timer(r, def.ai.attack_delay_ms, true, TimerAction::attack_ready);
            r.timer_diving = start_timer(r, def.ai.goto_dive_delay_ms, true, TimerAction::go_to_dive);
            log("allowed actor=" + std::to_string(id) + " (native Idle): attack and dive timers started");
        }
        return true; // the range/spotted callbacks require `allowed`
    }
    const auto* props = world->combat_properties(id);
    const auto* tables = session.original_ai_tables();
    const auto* ai_row = props && tables ? dh2::data::ai_props(*tables, props->sheets.resolved[1]) : nullptr;
    if (!ai_row) return true;
    const auto in_sight = [&](const ActorState& t, float radius, bool& visible) {
        const auto* t_props = world->combat_properties(t.id);
        const auto* t_ai = t_props ? dh2::data::ai_props(*tables, t_props->sheets.resolved[1]) : nullptr;
        visible = false;
        if (!t_ai) return true;
        dh2::data::AiRangeRequest request{};
        for (unsigned i = 0; i != 3; ++i) { request.owner[i] = owner->transform.position[i]; request.target[i] = t.transform.position[i]; }
        request.owner_melee = ai_row->melee_radius;
        request.target_melee = t_ai->melee_radius;
        request.view = radius;
        dh2::data::AiRangeResult result{};
        visible = dh2_ai_range(&result, &request) == 0 && result.sight != 0;
        if (visible && services_.can_see && !services_.can_see(session, *owner, t, visible, error)) return false;
        return true;
    };
    if (r.target == invalid_actor_id) {
        // _UpdateAggro: OnEnemySpotted about every 500 ms for an enemy inside ViewRadius while there is no target.
        r.spot_ms += dt * 1000.0;
        if (r.spot_ms < 500.0) return true;
        r.spot_ms = 0.0;
        for (const auto& pair : world->actors()) {
            if (pair.first == id || !world->eligible_target(*owner, pair.second)) continue;
            bool visible = false;
            if (!in_sight(pair.second, ai_row->view_radius, visible)) return false;
            if (!visible) continue;
            // swampking_OnEnemySpotted.
            if (!r.music_started) {
                r.music_started = true;
                if (services_.play_music && !services_.play_music(def.ai.spotted_music, def.ai.spotted_music_fade_ms, presentation))
                    log("spotted music not played: " + presentation);
            }
            r.target = pair.first;
            owner->target_id = pair.first;
            ++report_.spotted;
            log("enemy spotted actor=" + std::to_string(id) + " target=" + std::to_string(pair.first));
            break;
        }
        return true;
    }
    const auto* target = world->find_actor(r.target);
    if (!target || !world->eligible_target(*owner, *target)) return true;
    bool visible = false;
    if (!in_sight(*target, ai_row->view_radius, visible)) return false;
    if (!visible) return true; // OnTargetOutOfSight: phase 1 sets an unused delay
    const float range_d2 = distance_squared(source_target_position(*owner), source_target_position(*target));
    const auto event = enemy_ai::ranged_range_event_v1(range_d2, def.range_min, def.range_max);
    r.range_log_ms += dt * 1000.0;
    if (r.range_log_ms >= 2000.0) {
        r.range_log_ms = 0.0;
        const float* a = source_target_position(*owner);
        log("range actor=" + std::to_string(id) + " d=" + std::to_string(std::sqrt(range_d2)) + " event=" + std::to_string(static_cast<int>(event)) +
            " ownerTarget=" + std::to_string(a[0]) + "," + std::to_string(a[1]) + "," + std::to_string(a[2]) + " attackReady=" + std::to_string(r.attack_flag));
    }
    if (event == enemy_ai::RangedRangeEventV1::close_range) {
        // swampking_OnTargetInCloseRange.
        if (r.phase == 1) {
            if (r.first_melee && r.attack_flag) {
                if (!do_skill(session, id, def, r, def.ai.close.phase1_first_slot, error)) return false;
                r.first_melee = false;
                r.attack_flag = false;
            } else if (r.attack_flag) {
                r.attack_flag = false; // Do_MeleeAttackPhase1
                const int roll_value = services_.random ? services_.random(1, 100) : 1;
                const auto decision = dh2::bosses::runtime_v1::close_attack(Phase::phase1, false, roll_value, def.ai.close);
                if (decision.kind == DecisionKind::skill_slot && !do_skill(session, id, def, r, decision.skill_slot, error)) return false;
            }
        } else if (r.attack_flag) {
            r.attack_flag = false; // Do_MeleeAttackPhase2
            const bool first = r.first_melee;
            if (first) r.first_melee = false;
            const int roll_value = services_.random ? services_.random(1, 100) : 1;
            const auto decision = dh2::bosses::runtime_v1::close_attack(Phase::phase2, first, roll_value, def.ai.close);
            if (decision.kind == DecisionKind::skill_slot && !do_skill(session, id, def, r, decision.skill_slot, error)) return false;
            if (decision.kind == DecisionKind::no_skill_dispatched) log("phase-2 melee roll " + std::to_string(roll_value) + " dispatches no skill (source gap)");
        }
        return true;
    }
    if (event == enemy_ai::RangedRangeEventV1::ranged_range) {
        // swampking_OnTargetInRangedRange.
        r.first_melee = true;
        if (!r.attack_flag) return true;
        r.attack_flag = false;
        if (r.phase == 1) {
            if (services_.random) (void)services_.random(1, 100); // Do_RangedAttackPhase1 draws an unused roll
            // SetSwampkingInIntimidateState(2).
            if (r.sk != ScriptState::intimidate) {
                r.sk = ScriptState::intimidate;
                r.intimidate_flag = 2;
                if (!play_anim(session, id, def, r, false, error)) return false;
                const auto n = static_cast<std::int32_t>(def.ai.intimidate_sounds.size());
                const auto pick = services_.random ? services_.random(1, 3) : 1;
                if (pick >= 1 && pick <= n && services_.play_sound &&
                    !services_.play_sound(def.ai.intimidate_sounds[static_cast<std::size_t>(pick - 1)], presentation))
                    log("intimidate sound not played: " + presentation);
                ++report_.intimidates;
            }
        } else {
            if (!do_skill(session, id, def, r, def.ai.spit_skill, error)) return false; // Do_RangedAttackPhase2
        }
        return true;
    }
    return true; // OnTargetOutOfRange: no callback in this script
}

bool RuntimeSwampkingCoreAiV1::update(CombatSession& session, double dt, bool ai, std::string& error) {
    error.clear();
    if (!std::isfinite(dt) || dt < 0) return fail(error, "Boss AI dt must be finite and nonnegative");
    auto* world = session.world();
    if (!world || !session.original_ai_tables()) return fail(error, "Boss AI requires the live session world/AI tables");
    const auto lease = session.actor_binding_lease();
    if (!lease_known_ || !same_lease(lease_, lease)) {
        // A restore replaces every actor: the script state starts again (swampking_OnInit -> InitVars).
        actors_.clear();
        projectiles_.clear();
        lease_ = lease;
        lease_known_ = true;
        targetability_changed_ = true; // the script state (and so SetIsTargetable) starts again; hosts re-read the permission
        for (const auto& pair : definitions_) world->set_selection_suppressed(pair.first, false);
    }
    for (const auto& pair : definitions_) {
        if (!world->find_actor(pair.first)) continue;
        auto& runtime = actors_[pair.first];
        const bool before = runtime.targetable;
        if (!tick_actor(session, pair.first, pair.second, runtime, dt, ai, error)) return false;
        // SetIsTargetable (Character+1045) is read by IsInteractive, i.e. target selection only: it never gates damage.
        world->set_selection_suppressed(pair.first, !runtime.targetable);
        if (runtime.targetable != before) {
            targetability_changed_ = true;
            log("SetIsTargetable(" + std::string(runtime.targetable ? "true" : "false") + ") actor=" + std::to_string(pair.first));
        }
    }
    const double total_ms = dt * 1000.0 + fraction_ms_;
    const auto dt_ms = static_cast<std::int32_t>(total_ms);
    fraction_ms_ = total_ms - dt_ms;
    return update_projectiles(session, dt_ms, error);
}

} // namespace dh::foundation::bosses
