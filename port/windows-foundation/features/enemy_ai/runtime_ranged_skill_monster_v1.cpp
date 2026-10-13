#include "runtime_ranged_skill_monster_v1.hpp"

#include "../../content_paths.hpp"
#include "../../playable_actor_world.hpp"
#include "../generic_skills/runtime_skill_target_query_v1.hpp"
#include "../../../level-world/character_path_commands.hpp"
#include "../../../game-data/ai.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <regex>
#include <sstream>

namespace dh::foundation::enemy_ai {
namespace {

bool fail(std::string& error, const std::string& message) {
    error = message;
    return false;
}

bool same_lease(const std::weak_ptr<const void>& a, const std::weak_ptr<const void>& b) {
    return !a.owner_before(b) && !b.owner_before(a);
}

// Lua block comments (--[[ ... ]]) and line comments (-- ...). The authored AI/skill scripts keep no "--" in strings.
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

// One relative MoveTo argument: a number, a constant name or a negated constant name.
bool relative_argument(const std::string& text, std::string token, float& value) {
    token = std::regex_replace(token, std::regex("^\\s+|\\s+$"), "");
    bool negative = false;
    if (!token.empty() && token.front() == '-') { negative = true; token.erase(0, 1); }
    double number = 0;
    if (std::regex_match(token, std::regex("[0-9]+(\\.[0-9]*)?"))) number = std::stod(token);
    else if (!std::regex_match(token, std::regex("\\w+")) || !numeric_constant(text, token, number)) return false;
    value = static_cast<float>(negative ? -number : number);
    return true;
}

bool contains(const std::string& text, const char* needle) { return text.find(needle) != std::string::npos; }

bool string_call_argument(const std::string& text, const char* call, std::string& value, std::int32_t* number = nullptr) {
    const std::regex pattern(std::string(call) + "\\(\\s*\"([^\"]+)\"(\\s*,\\s*(-?[0-9]+))?");
    std::smatch match;
    if (!std::regex_search(text, match, pattern)) return false;
    value = match[1].str();
    if (number) *number = match[3].matched ? std::stoi(match[3].str()) : 0;
    return true;
}

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

std::int32_t signed_word(std::uint32_t word) { std::int32_t v; std::memcpy(&v, &word, 4); return v; }

std::string trimmed_script_name(std::string name) {
    const auto nul = name.find('\0');
    if (nul != std::string::npos) name.resize(nul);
    return name;
}

bool read_text(const AssetCatalog& assets, const std::string& uri, std::string& text, std::string& error) {
    try {
        const auto bytes = read_content(assets, uri);
        text.assign(bytes.begin(), bytes.end());
        return true;
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
}

struct Cursor {
    const std::uint8_t* data;
    std::size_t size, at = 0;
    bool u8(std::uint8_t& v) { if (at + 1 > size) return false; v = data[at++]; return true; }
    bool u32(std::uint32_t& v) { if (at + 4 > size) return false; std::memcpy(&v, data + at, 4); at += 4; return true; }
    bool text(std::string& v) {
        std::uint32_t n = 0;
        if (!u32(n) || at + n > size) return false;
        v.assign(reinterpret_cast<const char*>(data + at), n);
        at += n;
        return true;
    }
};

} // namespace

bool parse_ranged_skill_ai_script_v1(const std::string& raw, RangedSkillAiScriptV1& output, std::string& error) {
    const auto text = strip_lua_comments(raw);
    const auto table = vf_table(text);
    const auto body_of = [&](const char* event, std::string& body) {
        const auto found = table.find(event);
        return found != table.end() && function_body(text, found->second, body);
    };
    RangedSkillAiScriptV1 next;
    std::string close, ranged, out, spotted, target_died, died;
    if (!body_of("OnTargetInCloseRange", close)) return fail(error, "AI script has no OnTargetInCloseRange callback");
    std::smatch match;
    if (!std::regex_search(close, match, std::regex("DoSkill\\(\\s*([0-9]+)\\s*\\)")))
        return fail(error, "OnTargetInCloseRange does not issue DoSkill(n)");
    next.close_range_skill = std::stoi(match[1].str());
    // A conditional DoSkill (monster.luac rolls/flees first) is not this shape.
    if (contains(close, "if") || contains(close, "Attack(")) return fail(error, "OnTargetInCloseRange DoSkill is conditional");
    next.close_range_stops = contains(close, "Stop()");
    next.close_range_looks_at_target = contains(close, "LookAtTarget()");
    if (!body_of("OnTargetInRangedRange", ranged) || !contains(ranged, "MoveTo(GetTarget())"))
        return fail(error, "OnTargetInRangedRange does not MoveTo its target");
    if (contains(ranged, "DoSkill")) return fail(error, "OnTargetInRangedRange also uses a skill");
    next.ranged_range_approaches = true;
    if (body_of("OnTargetOutOfRange", out)) next.out_of_range_clears_target = contains(out, "ClearTarget()");
    if (body_of("OnTargetDied", target_died)) next.target_died_clears_target = contains(target_died, "ClearTarget()");
    if (body_of("OnEnemySpotted", spotted)) {
        std::string search;
        if (contains(spotted, "Do_SearchVictims()") && function_body(text, "Do_SearchVictims", search) &&
            contains(search, "\"AITargetList_CharacterFilter\", \"Player\""))
            next.enemy_spotted_searches_player = true;
        std::int32_t fade = 0;
        if (string_call_argument(spotted, "PlayMusic", next.spotted_music, &fade)) next.spotted_music_fade_ms = fade;
        if (std::regex_search(spotted, match, std::regex("StartTimerCB\\(\\s*(\\w+)\\s*,\\s*(true|false)\\s*,\\s*(\\w+)\\s*\\)"))) {
            double delay = 0;
            std::string callback;
            if (!numeric_constant(text, match[1].str(), delay) || delay <= 0)
                return fail(error, "Teleport timer delay constant is not a positive number");
            next.teleport.delay_ms = delay;
            next.teleport.repeat = match[2].str() == "true";
            if (!function_body(text, match[3].str(), callback)) return fail(error, "Teleport timer callback body is absent");
            std::smatch fx;
            if (std::regex_search(callback, fx, std::regex("PlayFX\\(\\s*(\\w+)"))) {
                std::smatch id;
                if (!std::regex_search(text, id, std::regex("local[ \\t]+" + fx[1].str() +
                        "[ \\t]*=[ \\t]*GetPyOID\\(\\s*\"AnimatedEffectTable\"\\s*,\\s*\"([^\"]+)\"\\s*\\)")))
                    return fail(error, "Teleport FX constant is not an AnimatedEffectTable OID");
                next.teleport.fx_name = id[1].str();
            }
            string_call_argument(callback, "PlaySound", next.teleport.sound_name);
            std::size_t warps = 0;
            const std::regex warp_call("WarpTo\\(([^)]*)\\)"), move_call("MoveTo\\(([^)]*)\\)");
            for (auto it = std::sregex_iterator(callback.begin(), callback.end(), warp_call);
                 it != std::sregex_iterator(); ++it) ++warps;
            for (auto it = std::sregex_iterator(callback.begin(), callback.end(), move_call);
                 it != std::sregex_iterator(); ++it) {
                std::vector<std::string> args;
                std::stringstream stream((*it)[1].str());
                for (std::string part; std::getline(stream, part, ',');) args.push_back(part);
                if (args.size() != 4 || std::regex_replace(args[3], std::regex("\\s+"), "") != "true")
                    return fail(error, "Teleport MoveTo is not a relative (x, y, z, true) request");
                std::array<float, 3> offset{};
                for (unsigned i = 0; i != 3; ++i)
                    if (!relative_argument(text, args[i], offset[i]))
                        return fail(error, "Teleport MoveTo offset is not a number/constant");
                next.teleport.candidates.push_back(offset);
            }
            if (next.teleport.candidates.empty() || warps != next.teleport.candidates.size() || !contains(callback, "HasPath()"))
                return fail(error, "Teleport callback does not pair each relative MoveTo/HasPath with a WarpTo");
            next.teleport.present = true;
        }
    }
    if (body_of("OnDied", died)) {
        next.died_stops_timer = contains(died, "StopTimerCB");
        std::int32_t fade = 0;
        if (string_call_argument(died, "PlayMusic", next.died_music, &fade)) next.died_music_fade_ms = fade;
    }
    output = std::move(next);
    error.clear();
    return true;
}

bool parse_ranged_skill_script_v1(const std::string& raw, RangedSkillScriptV1& output, std::string& error) {
    const auto text = strip_lua_comments(raw);
    RangedSkillScriptV1 next;
    std::smatch match;
    const auto local_prop = [&](const char* name, bool fixed, std::string& field) {
        const std::string inner = "GetProp\\(\\s*GetPyStruct\\(\\s*\"CharacterProperties\"\\s*,\\s*\"(\\w+)\"\\s*\\)\\s*\\)";
        const std::string value = fixed ? "FromFixed\\(\\s*" + inner + "\\s*\\)" : inner;
        std::smatch m;
        if (!std::regex_search(text, m, std::regex("local[ \\t]+" + std::string(name) + "[ \\t]*=[ \\t]*" + value))) return false;
        field = m[1].str();
        return true;
    };
    if (!local_prop("RANGE", true, next.range_field)) return fail(error, "Skill script RANGE is not a FromFixed CharacterProperties field");
    if (!local_prop("PROJECTILE_ID", false, next.projectile_field)) return fail(error, "Skill script PROJECTILE_ID is not a CharacterProperties field");
    if (!std::regex_search(text, match, std::regex("local[ \\t]+CLASS_ID[ \\t]*=[ \\t]*GetPyOID\\(\\s*\"ClassTable\"\\s*,\\s*\"(\\w+)\"\\s*\\)")))
        return fail(error, "Skill script CLASS_ID is not a ClassTable OID");
    next.class_token = match[1].str();
    std::map<std::string, std::string> callbacks;
    const auto register_at = text.find("RegisterSkillEx(");
    if (register_at == std::string::npos) return fail(error, "Skill script registers no RegisterSkillEx table");
    const auto register_end = text.find("})", register_at);
    const auto block = text.substr(register_at, register_end == std::string::npos ? std::string::npos : register_end - register_at);
    const std::regex assignment("(\\w+)\\s*=\\s*(\\w+)");
    for (auto it = std::sregex_iterator(block.begin(), block.end(), assignment); it != std::sregex_iterator(); ++it)
        callbacks[(*it)[1].str()] = (*it)[2].str();
    const auto body_of = [&](const char* key, std::string& body) {
        const auto found = callbacks.find(key);
        return found != callbacks.end() && function_body(text, found->second, body);
    };
    std::string pre, skill, delayed, delayed_check;
    if (body_of("pre", pre))
        next.pre_frontal_enemy_search = contains(pre, "\"AITargetList_CharacterFilter\", \"Enemy\"") &&
            contains(pre, "\"AITargetList_Sorting\", \"FrontalFirst\"") && contains(pre, "TargetListSearch(RANGE)") &&
            contains(pre, "LookAt(target)");
    if (body_of("skill", skill))
        next.skill_spawns_projectile = contains(skill, "SpawnSkillProjectile(PROJECTILE_ID)");
    if (body_of("delayed", delayed))
        next.delayed_combat_roll = contains(delayed, "ApplyPropClass(CLASS_ID)") && contains(delayed, "SkillCombatRoll(target)") &&
            contains(delayed, "\"SnS_Level\"");
    if (body_of("delayed_check", delayed_check))
        next.delayed_check_character = contains(delayed_check, "IsCharacter()");
    if (!next.pre_frontal_enemy_search || !next.skill_spawns_projectile || !next.delayed_combat_roll || !next.delayed_check_character)
        return fail(error, "Skill script is not a FrontalFirst pre-search / projectile / delayed SkillCombatRoll skill");
    output = std::move(next);
    error.clear();
    return true;
}

bool load_source_projectile_table_v1(dh2::data::Bytes records, dh2::data::Bytes names, dh2::data::Bytes schema,
                                     dh2::data::Bytes dictionary, std::vector<SourceProjectileRowV1>& output,
                                     std::string& error) {
    static const char* const fields[] = {"Dedicated", "DisappearOnHit", "ExpireFX", "ExpireSFX", "FriendlyFire", "ImpactFX",
        "ImpactSFX", "IsLaser", "IsMagic", "MaxDistance", "MaxRotation", "Model", "ObjectImpactFX", "ObjectImpactSFX",
        "StayOnFloor", "TargetLock", "TimeBetweenHit", "Timer", "Velocity", "VelocityDamping"};
    static const char shape[] = "bbiibiibbiiiiibbiiii";
    Cursor s{schema.data, schema.size};
    std::uint32_t field_count = 0;
    if (!s.u32(field_count) || field_count != 20) return fail(error, "ProjectileTable schema field count differs");
    for (unsigned i = 0; i != 20; ++i) {
        std::string name;
        if (!s.text(name) || name != fields[i]) return fail(error, "ProjectileTable schema field order differs");
    }
    Cursor n{names.data, names.size}, d{dictionary.data, dictionary.size}, r{records.data, records.size};
    std::uint32_t rows = 0, name_count = 0, models = 0;
    if (!r.u32(rows) || !n.u32(name_count) || name_count != rows || rows > 4096)
        return fail(error, "ProjectileTable row/name counts differ");
    std::vector<std::string> model_uris;
    if (!d.u32(models) || models > 4096) return fail(error, "ProjectileDict is malformed");
    for (std::uint32_t i = 0; i != models; ++i) {
        std::string uri;
        if (!d.text(uri)) return fail(error, "ProjectileDict entry is truncated");
        model_uris.push_back(uri);
    }
    std::vector<SourceProjectileRowV1> next(rows);
    for (std::uint32_t row = 0; row != rows; ++row) {
        auto& p = next[row];
        if (!n.text(p.name)) return fail(error, "ProjectileTable names are truncated");
        std::uint32_t words[20]{};
        for (unsigned i = 0; i != 20; ++i) {
            if (shape[i] == 'b') { std::uint8_t b = 0; if (!r.u8(b)) return fail(error, "ProjectileTable row is truncated"); words[i] = b; }
            else if (!r.u32(words[i])) return fail(error, "ProjectileTable row is truncated");
        }
        const auto f = [&](unsigned i) { float v; std::memcpy(&v, &words[i], 4); return v; };
        p.dedicated = words[0] != 0; p.disappear_on_hit = words[1] != 0;
        p.expire_fx = signed_word(words[2]); p.expire_sound = signed_word(words[3]);
        p.friendly_fire = words[4] != 0; p.impact_fx = signed_word(words[5]); p.impact_sound = signed_word(words[6]);
        p.laser = words[7] != 0; p.magic = words[8] != 0; p.max_distance = f(9); p.max_rotation = f(10);
        p.model = signed_word(words[11]); p.object_impact_fx = signed_word(words[12]); p.object_impact_sound = signed_word(words[13]);
        p.stay_on_floor = words[14] != 0; p.target_lock = words[15] != 0; p.time_between_hit_ms = signed_word(words[16]);
        p.timer_ms = signed_word(words[17]); p.velocity = f(18); p.velocity_damping = f(19);
        if (p.model >= 0) {
            if (static_cast<std::size_t>(p.model) >= model_uris.size()) return fail(error, "ProjectileTable model is outside ProjectileDict");
            p.model_uri = model_uris[static_cast<std::size_t>(p.model)];
        }
    }
    if (r.at != r.size) return fail(error, "ProjectileTable has trailing bytes");
    output = std::move(next);
    error.clear();
    return true;
}

bool derive_ranged_skill_monster_v1(const RangedSkillMonsterSourcesV1& sources, const std::string& profile,
                                    bool& supported, RangedSkillMonsterDefinitionV1& output, std::string& reason) {
    supported = false;
    reason.clear();
    if (!sources.assets || !sources.tables) { reason = "ranged-skill sources unavailable"; return true; }
    const auto& characters = sources.tables->properties.characters;
    const auto row = std::find(characters.names.begin(), characters.names.end(), profile);
    if (row == characters.names.end()) { reason = "CharacterTable row absent"; return true; }
    const auto index = static_cast<std::size_t>(row - characters.names.begin());
    const auto field = [&](const char* name) -> std::int32_t {
        const auto column = std::find(characters.fields.begin(), characters.fields.end(), std::string(name));
        return column == characters.fields.end() ? std::numeric_limits<std::int32_t>::min()
                                                  : characters.rows[index][static_cast<std::size_t>(column - characters.fields.begin())];
    };
    RangedSkillMonsterDefinitionV1 next;
    next.profile = profile;
    next.projectile_row = field("RangeProjectileID");
    if (next.projectile_row < 0) { reason = "CanRangeAttack is false (RangeProjectileID -1)"; return true; }
    const auto* ai = dh2::data::ai_props(sources.tables->ai, field("AI"));
    if (!ai) { reason = "AI row absent"; return true; }
    next.ai_script = ai->script;
    std::string text, problem;
    if (!read_text(*sources.assets, "data/scripts/ai/" + next.ai_script + ".luac", text, problem)) {
        reason = "authored AI script unavailable: " + problem; return true;
    }
    if (!parse_ranged_skill_ai_script_v1(text, next.ai, problem)) { reason = "AI script " + next.ai_script + ": " + problem; return true; }
    next.range_min = static_cast<float>(field("RangeMinDistance") >> 8);
    next.range_max = static_cast<float>(field("RangeMaxDistance") >> 8);
    if (!(next.range_max > next.range_min) || next.range_min < 0) { reason = "RangeMin/MaxDistance invalid"; return true; }
    const auto tree = field("SkillTree");
    if (!sources.skills || tree < 0 || static_cast<std::size_t>(tree) >= sources.skills.lists().size()) {
        reason = "SkillTree list unavailable"; return true;
    }
    const auto& list = sources.skills.lists()[static_cast<std::size_t>(tree)];
    if (next.ai.close_range_skill < 0 || static_cast<std::size_t>(next.ai.close_range_skill) >= list.size()) {
        reason = "DoSkill index outside the SkillTree list (Character::_DoSkill rejects it)"; return true;
    }
    next.skill_table_id = list[static_cast<std::size_t>(next.ai.close_range_skill)];
    if (next.skill_table_id < 0 || static_cast<std::size_t>(next.skill_table_id) >= sources.skills.skills().size()) {
        reason = "SkillTable row unavailable"; return true;
    }
    const auto& skill = sources.skills.skills()[static_cast<std::size_t>(next.skill_table_id)];
    next.skill_name = sources.skills.skill_names()[static_cast<std::size_t>(next.skill_table_id)];
    next.skill_script = trimmed_script_name(skill.script);
    // Same SkillTable scalar words the player skill owner reads (runtime_skill_animation_bank_v1 / coordinator):
    // words[1] Anim root sequence, words[5] ElementalType, words[7] Flags.
    next.animation_sequence_id = signed_word(skill.scalar.words[1]);
    next.element = signed_word(skill.scalar.words[5]);
    next.skill_flags = skill.scalar.words[7];
    if (next.animation_sequence_id < 0) { reason = "SkillTable Anim root is negative"; return true; }
    if (!read_text(*sources.assets, "data/scripts/skills/" + next.skill_script + ".luac", text, problem)) {
        reason = "authored skill script unavailable: " + problem; return true;
    }
    if (!parse_ranged_skill_script_v1(text, next.skill, problem)) { reason = "skill script " + next.skill_script + ": " + problem; return true; }
    if (next.skill.projectile_field != "RangeProjectileID") { reason = "skill projectile field is not RangeProjectileID"; return true; }
    const auto range = field(next.skill.range_field.c_str());
    if (range == std::numeric_limits<std::int32_t>::min()) { reason = "skill RANGE field absent"; return true; }
    next.pre_search_range = static_cast<float>(range) / 256.0f;
    if (!sources.projectiles || static_cast<std::size_t>(next.projectile_row) >= sources.projectiles->size()) {
        reason = "ProjectileTable unavailable"; return true;
    }
    next.projectile = (*sources.projectiles)[static_cast<std::size_t>(next.projectile_row)];
    if (next.projectile.laser || next.projectile.timer_ms <= 0 || !(next.projectile.velocity > 0) ||
        !std::isfinite(next.projectile.velocity_damping) || next.projectile.velocity_damping < 0) {
        reason = "projectile row needs an unsupported laser/model-duration motion"; return true;
    }
    if (!sources.classes) { reason = "ClassTables unavailable"; return true; }
    const auto cls = std::find(sources.classes->names.begin(), sources.classes->names.end(), next.skill.class_token);
    if (cls == sources.classes->names.end()) { reason = "skill CLASS_ID absent from ClassTables"; return true; }
    next.class_id = static_cast<std::int32_t>(cls - sources.classes->names.begin());
    output = std::move(next);
    supported = true;
    return true;
}

bool derive_ranged_skill_monster_policy_v1(const spawn::ProfileDerivationTablesV1& tables, const std::string& profile,
                                           CombatSessionProfile& output, std::string& error) {
    const auto& characters = tables.properties.characters;
    const auto row = std::find(characters.names.begin(), characters.names.end(), profile);
    if (row == characters.names.end()) return fail(error, "CharacterTable row is absent: " + profile);
    const auto column = std::find(characters.fields.begin(), characters.fields.end(), std::string("AnimTable"));
    if (column == characters.fields.end()) return fail(error, "CharacterTable has no AnimTable field");
    const auto table = characters.rows[static_cast<std::size_t>(row - characters.names.begin())]
                                      [static_cast<std::size_t>(column - characters.fields.begin())];
    const auto& animations = tables.animations;
    if (table < 0 || static_cast<std::size_t>(table) >= animations.characters.size())
        return fail(error, "AnimTable index outside CharAnim table: " + profile);
    for (const char* state : {"Idle", "Injured", "Died"}) {
        const auto found = std::find(animations.state_names.begin(), animations.state_names.end(), std::string(state));
        if (found == animations.state_names.end()) return fail(error, std::string("CharAnim state absent: ") + state);
        const auto& values = animations.characters[static_cast<std::size_t>(table)]
                                 .fields[static_cast<std::size_t>(found - animations.state_names.begin())];
        if (std::none_of(values.begin(), values.end(), [](std::int32_t id) { return id >= 0; }))
            return fail(error, std::string("AnimTable has no ") + state + " sequence: " + profile);
    }
    CombatSessionProfile policy;
    policy.animationOnly = true;
    policy.receiveDamage = true;
    policy.retainedPhaseClock = true;
    policy.initialIdle = CombatSessionChoice{"Idle", 0, {0}};
    policy.reaction = CombatSessionChoice{"Injured", 0, {0}};
    policy.death = CombatSessionChoice{"Died", 0, {0}};
    policy.motionRoot = "auto";
    policy.customization.allow_missing_animation_targets = true;
    policy.propertyOptions.refill_vitals = true; // a new monster, same as derive_enemy_combat_policy_v1
    output = std::move(policy);
    error.clear();
    return true;
}

RangedRangeEventV1 ranged_range_event_v1(float d2, float range_min, float range_max) {
    if (range_min * range_min > d2) return RangedRangeEventV1::close_range;           // AI_IsInCloseRange
    if (range_min * range_min <= d2 && range_max * range_max >= d2) return RangedRangeEventV1::ranged_range; // AI_IsInRange
    return RangedRangeEventV1::out_of_range;
}

std::array<float, 3> source_look_vector_v1(float heading) { return {std::sin(heading), -std::cos(heading), 0.0f}; }

std::array<float, 3> relative_point_v1(const std::array<float, 3>& p, float heading, const std::array<float, 3>& o) {
    const auto look = source_look_vector_v1(heading);
    // cross(K=(0,0,1), look) = (-look.y, look.x, 0)
    const std::array<float, 3> right{-look[1], look[0], 0.0f};
    return {p[0] + o[0] * right[0] + o[1] * look[0], p[1] + o[0] * right[1] + o[1] * look[1], p[2] + o[2]};
}

bool ranged_skill_formula_sheet_v1(const dh2::data::CharacterTable& characters, const dh2::data::ClassTables& classes,
                                   const dh2::data::PropertyRules&, const dh2::data::PropertySheet& live,
                                   std::int32_t class_id, std::int32_t skill_level, dh2::data::PropertySheet& output,
                                   std::string& error) {
    // Original Character::_ApplyPropClass 0x3babdc -> CharProperties::PROPS_ApplyClass 0x3df3b8 (no bool argument) ->
    // _LoadClass 0x3e2e20 on the attacker's OWN resolved sheet (CharProperties+2708): the class rows (SnS_CleanUp, then the
    // SnS_Min/MaxDamage... writes) overwrite only their destination properties in place. Every other property that
    // CF__CalcDamage 0x3b1fb8 / CF__CalcMissOrDodge 0x3b1df0 read through PROPS_Get/_GetProperty(+4084) (attack rating 50,
    // element 97 with its 95/96 range, bonus damage 83+, 92-94, 132/133...) stays the attacker's live value. Mode-1 source
    // reads use RecalcProperty (the live level), which apply_class reproduces with the live snapshot as its read sheet.
    // So the formula sheet starts from the live sheet, not from the property defaults.
    const auto column = std::find(characters.fields.begin(), characters.fields.end(), std::string("SnS_Level"));
    if (column == characters.fields.end()) return fail(error, "CharacterTable has no SnS_Level field");
    const auto index = static_cast<std::size_t>(column - characters.fields.begin());
    if (class_id < 0 || static_cast<std::size_t>(class_id) >= classes.names.size())
        return fail(error, "Skill CLASS_ID is outside ClassTables");
    if (skill_level < -32768 || skill_level > 32767) return fail(error, "Skill level outside the source fixed range");
    auto next = live;
    next[index] = static_cast<std::int32_t>(static_cast<std::uint32_t>(skill_level) << 8);
    auto live_with_level = live;
    live_with_level[index] = next[index];
    if (!dh2::data::apply_class(classes, class_id, next, error, &live_with_level)) {
        if (error.empty()) error = "Skill ClassTables row application failed";
        return false;
    }
    output = next;
    error.clear();
    return true;
}

bool advance_ranged_projectile_v1(RangedProjectileV1& p, const SourceProjectileRowV1& row, std::int32_t dt_ms) {
    if (dt_ms > 0) {
        // Box2D speed units -> game units (x100), as runtime_session_projectile_v1 (Projectile::Update).
        const float seconds = static_cast<float>(dt_ms) * 0.001f;
        p.position[0] += p.speed * p.forward[0] * 100.0f * seconds;
        p.position[1] += p.speed * p.forward[1] * 100.0f * seconds;
        p.speed -= row.velocity_damping * seconds;
        p.remaining_ms -= dt_ms;
        p.age_ms += dt_ms;
    }
    bool expired = p.remaining_ms <= 0 || p.speed <= 0;
    if (!expired && row.max_distance >= 0) {
        const float dx = p.position[0] - p.origin[0], dy = p.position[1] - p.origin[1], dz = p.position[2] - p.origin[2];
        expired = dx * dx + dy * dy + dz * dz >= row.max_distance * row.max_distance;
    }
    return expired;
}

bool ranged_projectile_contact_v1(const RangedProjectileV1& p, const std::array<float, 3>& centre, float radius) {
    const float dx = p.position[0] - centre[0], dy = p.position[1] - centre[1];
    return dx * dx + dy * dy <= radius * radius;
}

RuntimeRangedSkillMonsterV1::RuntimeRangedSkillMonsterV1(RangedSkillMonsterServicesV1 services,
                                                         std::map<ActorId, RangedSkillMonsterDefinitionV1> definitions)
    : services_(std::move(services)), definitions_(std::move(definitions)) {}

void RuntimeRangedSkillMonsterV1::log(std::string line) {
    if (log_.size() < 512) log_.push_back(std::move(line));
}

std::vector<std::string> RuntimeRangedSkillMonsterV1::take_log() {
    std::vector<std::string> out;
    out.swap(log_);
    return out;
}

std::vector<RangedProjectileV1> RuntimeRangedSkillMonsterV1::projectiles() const { return projectiles_; }

bool RuntimeRangedSkillMonsterV1::active_cast(ActorId id, std::uint64_t generation, std::int32_t& skill_table_id) const noexcept {
    const auto runtime = actors_.find(id);
    const auto definition = definitions_.find(id);
    if (runtime == actors_.end() || definition == definitions_.end() || !generation ||
        runtime->second.cast.generation != generation) return false;
    skill_table_id = definition->second.skill_table_id;
    return true;
}

bool RuntimeRangedSkillMonsterV1::look_at(CombatSession& session, ActorId owner_id, ActorId target_id, std::string& error) {
    auto* owner = session.world() ? session.world()->find_actor(owner_id) : nullptr;
    const auto* target = session.world() ? session.world()->find_actor(target_id) : nullptr;
    if (!owner || !target) return true;
    // GameObject::LookAt(Point) on the target position (same kernel as the player skill target LookAt).
    dh2::character::LookAtState16 look{{owner->transform.position[0], owner->transform.position[1], owner->transform.position[2]},
                                       owner->transform.rotation[2]};
    if (dh2_character_look_at_point(&look, source_target_position(*target)))
        return fail(error, "Original GameObject LookAt(Point) kernel rejected the target");
    owner->transform.rotation[2] = look.heading_angle;
    return true;
}

bool RuntimeRangedSkillMonsterV1::pre_search(CombatSession& session, ActorId id, const RangedSkillMonsterDefinitionV1& def,
                                             ActorId& selected, std::string& error) {
    selected = invalid_actor_id;
    if (!services_.search_population) return fail(error, "Ranged skill pre-search requires the source population order");
    std::vector<ActorId> order;
    std::vector<std::pair<ActorId, bool>> facts;
    if (!services_.search_population(order, facts, error)) return false;
    std::vector<generic_skills::SkillTargetSourceFactsV1> source;
    source.reserve(facts.size());
    for (const auto& fact : facts) source.push_back({fact.first, true, false, false, true, fact.second});
    generic_skills::SkillActorTargetQueryV1 query;
    // OnPreSkill_: Enemy filter, no objects, FrontalFirst, TargetListSearch(RANGE) (full radius).
    if (!generic_skills::query_source_character_targets_v1(session, id, order, source, true, def.pre_search_range,
            3.1415927410125732421875f, generic_skills::SkillTargetSortV1::frontal_first, query, error))
        return false;
    if (query.status == generic_skills::SkillActorTargetQueryStatusV1::selected_character) selected = query.selected;
    return true;
}

bool RuntimeRangedSkillMonsterV1::begin_skill(CombatSession& session, ActorId id, const RangedSkillMonsterDefinitionV1& def,
                                              ActorRuntime& runtime, std::string& error) {
    if (runtime.cast.active) return true; // SM_IsUsingSkill: AI_IsSkillUsable rejects; no OnPreSkill
    const auto* plan = session.actor_source_plan(id);
    const auto* policies = session.source_sequence_policies();
    const OriginalCombatSequencePlan* sequence = nullptr;
    if (plan) for (const auto& candidate : plan->sequences) if (candidate.id == def.animation_sequence_id) { sequence = &candidate; break; }
    if (!plan || !policies || !sequence) {
        ++report_.cast_rejected;
        log("cast rejected actor=" + std::to_string(id) + ": skill Anim root " + std::to_string(def.animation_sequence_id) +
            " is not in the actor's retained source bank");
        return true;
    }
    // OnPreSkill_: FrontalFirst enemy search; the Lua local `target` keeps its previous value when nothing is found.
    ActorId selected = invalid_actor_id;
    if (!pre_search(session, id, def, selected, error)) return false;
    if (selected != invalid_actor_id) runtime.cast.target = selected;
    if (runtime.cast.target != invalid_actor_id && !look_at(session, id, runtime.cast.target, error)) return false;
    const auto generation = next_generation_++;
    OriginalAttackSelection selection;
    selection.state = sequence->state;
    selection.variant = sequence->variant;
    // A type-2 root chooses one child group at random in the source; the port's derived monster attack pins the first
    // authored group (derive_enemy_combat_policy_v1), and so does this cast.
    if (sequence->type == 2) selection.group_path = {0};
    CombatSessionStateAnimationServices services;
    services.event = [this, &session, id, generation, defp = &def](ActorId actor, const RetainedAnimationEvent& event, std::string& e) {
        if (actor != id) return fail(e, "Ranged skill event reached a foreign actor");
        return skill_event(session, id, *defp, generation, event.name, e);
    };
    services.finished = [this, id, generation](ActorId actor, std::string& e) {
        if (actor != id) return fail(e, "Ranged skill completion reached a foreign actor");
        auto found = actors_.find(id);
        if (found != actors_.end() && found->second.cast.generation == generation) found->second.cast.active = false;
        e.clear();
        return true;
    };
    services.departed = [this, id, generation](ActorId actor, std::int32_t from, std::int32_t to, std::string& e) {
        if (actor != id) return fail(e, "Ranged skill departure reached a foreign actor");
        auto found = actors_.find(id);
        if (found != actors_.end() && found->second.cast.generation == generation && found->second.cast.active) {
            found->second.cast.active = false;
            ++report_.interrupted_casts;
            log("cast interrupted actor=" + std::to_string(id) + " from=" + std::to_string(from) + " to=" + std::to_string(to) +
                " used=" + std::to_string(found->second.cast.used));
        }
        e.clear();
        return true;
    };
    // A cast is transient (no cooldown, no persisted owner state); a checkpoint does not carry it.
    services.checkpoint = [](std::string& e) { e.clear(); return true; };
    runtime.cast.active = true;
    runtime.cast.used = false;
    runtime.cast.generation = generation;
    const CombatSessionSourceSequencePolicy policy{6, 0x6341u, generation};
    std::string problem;
    if (!session.play_actor_source_sequence(id, *plan, *policies, selection, std::move(services), policy, problem)) {
        runtime.cast.active = false;
        ++report_.cast_rejected;
        log("cast rejected actor=" + std::to_string(id) + ": " + problem);
        return true;
    }
    ++report_.casts;
    log("cast begin actor=" + std::to_string(id) + " skill=" + def.skill_name + " sequence=" + std::to_string(def.animation_sequence_id) +
        " generation=" + std::to_string(generation) + " target=" + std::to_string(runtime.cast.target));
    return true;
}

bool RuntimeRangedSkillMonsterV1::skill_event(CombatSession& session, ActorId id, const RangedSkillMonsterDefinitionV1& def,
                                              std::uint64_t generation, const std::string& name, std::string& error) {
    if (name != "do_skill") { error.clear(); return true; }
    auto found = actors_.find(id);
    if (found == actors_.end() || found->second.cast.generation != generation || found->second.cast.used) { error.clear(); return true; }
    auto& cast = found->second.cast;
    cast.used = true;
    // OnSkill_: LookAt(target); SpawnSkillProjectile(PROJECTILE_ID).
    if (cast.target != invalid_actor_id && !look_at(session, id, cast.target, error)) return false;
    const auto* owner = session.world()->find_actor(id);
    if (!owner) { error.clear(); return true; }
    RangedProjectileV1 projectile;
    projectile.id = next_projectile_++;
    projectile.owner = id;
    const float* origin = source_target_position(*owner);
    projectile.origin = {origin[0], origin[1], origin[2]};
    projectile.position = projectile.origin;
    projectile.forward = source_look_vector_v1(owner->transform.rotation[2]);
    projectile.speed = def.projectile.velocity;
    projectile.remaining_ms = def.projectile.timer_ms;
    projectile.model_uri = def.projectile.model_uri;
    projectiles_.push_back(projectile);
    ++report_.projectiles;
    log("projectile spawn actor=" + std::to_string(id) + " id=" + std::to_string(projectile.id) + " row=" + def.projectile.name +
        " at=" + std::to_string(origin[0]) + "," + std::to_string(origin[1]) + "," + std::to_string(origin[2]) +
        " heading=" + std::to_string(owner->transform.rotation[2]));
    error.clear();
    return true;
}

bool RuntimeRangedSkillMonsterV1::teleport(CombatSession& session, ActorId id, const RangedSkillMonsterDefinitionV1& def,
                                           ActorRuntime& runtime, std::string& error) {
    auto* owner = session.world()->find_actor(id);
    if (!owner) { error.clear(); return true; }
    // DoTeleport: Stop(); PlayFX(old); PlaySound; first relative MoveTo with a path -> WarpTo; PlayFX(new).
    if (runtime.approaching && services_.stop) {
        std::string stopError;
        if (!services_.stop(session, *owner, stopError)) log("teleport stop not applied: " + stopError);
        runtime.approaching = false;
    }
    owner = session.world()->find_actor(id);
    if (!owner) { error.clear(); return true; }
    const std::array<float, 3> before = owner->transform.position;
    const std::int32_t fx = !def.ai.teleport.fx_name.empty() && services_.fx_id ? services_.fx_id(def.ai.teleport.fx_name) : -1;
    std::string presentation;
    if (fx >= 0 && services_.play_fx && !services_.play_fx(fx, before, presentation)) log("teleport FX not played: " + presentation);
    if (!def.ai.teleport.sound_name.empty() && services_.play_sound && !services_.play_sound(def.ai.teleport.sound_name, presentation))
        log("teleport sound not played: " + presentation);
    bool moved = false;
    for (const auto& offset : def.ai.teleport.candidates) {
        const auto goal = relative_point_v1(before, owner->transform.rotation[2], offset);
        bool found = false;
        if (!services_.path_exists || !services_.warp) break;
        if (!services_.path_exists(session, *owner, goal, found, error)) return false;
        if (!found) continue;
        if (!services_.warp(session, *owner, goal, error)) return false;
        moved = true;
        break;
    }
    owner = session.world()->find_actor(id);
    if (!owner) { error.clear(); return true; }
    const auto after = owner->transform.position;
    if (fx >= 0 && services_.play_fx && !services_.play_fx(fx, after, presentation)) log("teleport FX not played: " + presentation);
    if (moved) ++report_.teleports; else ++report_.teleports_without_path;
    log("teleport actor=" + std::to_string(id) + " moved=" + std::to_string(moved) + " from=" + std::to_string(before[0]) + "," +
        std::to_string(before[1]) + " to=" + std::to_string(after[0]) + "," + std::to_string(after[1]) + " fx=" + std::to_string(fx));
    error.clear();
    return true;
}

bool RuntimeRangedSkillMonsterV1::update_projectiles(CombatSession& session, std::int32_t dt_ms, std::string& error) {
    auto* world = session.world();
    const auto* tables = session.original_ai_tables();
    for (auto it = projectiles_.begin(); it != projectiles_.end();) {
        auto& p = *it;
        const auto def = definitions_.find(p.owner);
        if (def == definitions_.end()) { it = projectiles_.erase(it); continue; }
        const auto& row = def->second.projectile;
        const bool expired = advance_ranged_projectile_v1(p, row, dt_ms);
        const auto* owner = world->find_actor(p.owner);
        ActorId hit = invalid_actor_id;
        for (const auto& pair : world->actors()) {
            if (pair.first == p.owner || !owner) continue;
            const auto& candidate = pair.second;
            if (!candidate.alive() || candidate.action == CharacterAction::dead) continue;
            const auto* candidate_traits = world->traits(pair.first);
            const auto* owner_traits = world->traits(p.owner);
            if (!candidate_traits || !owner_traits || !candidate_traits->targetable) continue;
            // Projectile::OnCollision: FriendlyFire 0 admits only enemies of the owner (AI_IsEnemy).
            if (!row.friendly_fire && !(tables && dh2::data::ai_enemy(*tables, owner->faction_id, candidate.faction_id,
                                                                      owner_traits->is_player, candidate_traits->is_player)))
                continue;
            const float radius = services_.body_radius ? services_.body_radius(pair.first) : 0.0f;
            const std::array<float, 3> centre = candidate.transform.position;
            if (radius > 0 && ranged_projectile_contact_v1(p, centre, radius)) { hit = pair.first; break; }
        }
        if (hit != invalid_actor_id) {
            ++report_.hits;
            const auto& d = def->second;
            std::string presentation;
            auto* attacker = world->find_actor(p.owner);
            auto* victim = world->find_actor(hit);
            // Projectile::HandleImpactFX 0x3e4eec: a Character peer gets the impact FX at its GetTargetPosition.
            if (victim) {
                const float* at = source_target_position(*victim);
                if (row.impact_fx >= 0 && services_.play_fx && !services_.play_fx(row.impact_fx, {at[0], at[1], at[2]}, presentation))
                    log("impact FX not played: " + presentation);
            }
            if (row.impact_sound >= 0 && services_.play_sound_id && !services_.play_sound_id(row.impact_sound, presentation))
                log("impact sound not played: " + presentation);
            const auto* props = world->combat_properties(p.owner);
            if (attacker && victim && props && world->eligible_target(*attacker, *victim) && services_.characters &&
                services_.classes && services_.property_rules) {
                // delayed_check: target is a character; delayed: SnS_Level = 0 (monster skill level), ApplyPropClass,
                // SkillCombatRoll(target).
                dh2::data::PropertySheet sheet{};
                if (!ranged_skill_formula_sheet_v1(*services_.characters, *services_.classes, *services_.property_rules,
                        props->sheets.resolved, d.class_id, 0, sheet, error)) return false;
                CombatSessionSourceHit source;
                source.attacker = p.owner;
                source.target = hit;
                source.binding_lease = session.actor_binding_lease();
                source.generation = p.id;
                source.event_index = 0;
                source.mask = d.skill_flags | 0x08000000u; // native SkillCombatRoll SkillAttack bit (coordinator)
                source.source_id = d.skill_name;
                source.marker_name = "do_skill";
                source.category = props->facts.main_damage_class;
                source.element = d.element;
                source.direct_amount = 0;
                source.attacker_formula_sheet = &sheet;
                DamageEvent receipt;
                std::string problem;
                if (!session.apply_source_result(source, receipt, problem)) {
                    log("projectile hit not applied id=" + std::to_string(p.id) + ": " + problem);
                } else {
                    log("projectile hit id=" + std::to_string(p.id) + " owner=" + std::to_string(p.owner) + " target=" + std::to_string(hit) +
                        " applied=" + std::to_string(receipt.applied) + " removed=" + std::to_string(receipt.health_removed) + " outcomes=" + std::to_string(receipt.source_outcomes.value_or(0)) +
                        " hp=" + std::to_string(victim->health) + "/" + std::to_string(victim->max_health));
                }
            } else {
                log("projectile hit id=" + std::to_string(p.id) + " target=" + std::to_string(hit) +
                    " without damage: owner/target no longer eligible (owner dead or target untargetable)");
            }
            if (row.disappear_on_hit) { it = projectiles_.erase(it); continue; }
        }
        if (expired) {
            ++report_.misses_expired;
            std::string presentation;
            if (row.expire_fx >= 0 && services_.play_fx && !services_.play_fx(row.expire_fx, p.position, presentation))
                log("expire FX not played: " + presentation);
            if (row.expire_sound >= 0 && services_.play_sound_id && !services_.play_sound_id(row.expire_sound, presentation))
                log("expire sound not played: " + presentation);
            log("projectile expired id=" + std::to_string(p.id));
            it = projectiles_.erase(it);
            continue;
        }
        ++it;
    }
    error.clear();
    return true;
}

bool RuntimeRangedSkillMonsterV1::tick_actor(CombatSession& session, ActorId id, const RangedSkillMonsterDefinitionV1& def,
                                             ActorRuntime& runtime, double dt, std::string& error) {
    auto* world = session.world();
    auto* owner = world->find_actor(id);
    if (!owner) return true;
    if (!owner->alive() || owner->action == CharacterAction::dead) {
        if (!runtime.dead_seen) {
            runtime.dead_seen = true;
            runtime.cast.active = false;
            // OnDied: PlayMusic(...); StopTimerCB(teleport).
            if (def.ai.died_stops_timer) runtime.timer_active = false;
            std::string presentation;
            if (!def.ai.died_music.empty() && services_.play_music && !services_.play_music(def.ai.died_music, def.ai.died_music_fade_ms, presentation))
                log("death music not played: " + presentation);
            log("died actor=" + std::to_string(id) + " teleportTimer=" + std::to_string(runtime.timer_active));
        }
        return true;
    }
    runtime.dead_seen = false;
    const auto* traits = world->traits(id);
    const auto* props = world->combat_properties(id);
    const auto* tables = session.original_ai_tables();
    // _UpdateTarget returns while awaiting spawn / in Limbus; the session publishes that as untargetable.
    if (!traits || !traits->targetable || !props || !tables) return true;
    const auto* ai = dh2::data::ai_props(*tables, props->sheets.resolved[1]);
    if (!ai) return true;
    // Teleport timer (StartTimerCB(MOVE_DELAY, true, DoTeleport)).
    if (runtime.timer_active && def.ai.teleport.present) {
        runtime.timer_ms += dt * 1000.0;
        if (runtime.timer_ms >= def.ai.teleport.delay_ms) {
            runtime.timer_ms -= def.ai.teleport.delay_ms;
            if (!def.ai.teleport.repeat) runtime.timer_active = false;
            if (!teleport(session, id, def, runtime, error)) return false;
            owner = world->find_actor(id);
            if (!owner) return true;
        }
    }
    const auto inside_view = [&](const ActorState& a, const ActorState& b, float radius) {
        const auto* b_props = world->combat_properties(b.id);
        const auto* b_ai = b_props ? dh2::data::ai_props(*tables, b_props->sheets.resolved[1]) : nullptr;
        if (!b_ai) return false;
        dh2::data::AiRangeRequest request{};
        for (unsigned i = 0; i != 3; ++i) { request.owner[i] = a.transform.position[i]; request.target[i] = b.transform.position[i]; }
        request.owner_melee = ai->melee_radius;
        request.target_melee = b_ai->melee_radius;
        request.view = radius;
        dh2::data::AiRangeResult result{};
        return dh2_ai_range(&result, &request) == 0 && result.sight != 0;
    };
    if (runtime.target != invalid_actor_id) {
        auto* target = world->find_actor(runtime.target);
        if (!target || !target->alive() || target->action == CharacterAction::dead) {
            // OnTargetDied: Stop(); ClearTarget(); StopTimerCB.
            if (runtime.approaching && services_.stop && !services_.stop(session, *owner, error)) return false;
            runtime.approaching = false;
            runtime.target = invalid_actor_id;
            owner->target_id = invalid_actor_id;
            runtime.timer_active = false;
            ++report_.target_losses;
            log("target died actor=" + std::to_string(id));
            return true;
        }
        if (!world->eligible_target(*owner, *target)) return true; // untargetable: no range events this frame
        bool visible = inside_view(*owner, *target, ai->view_radius);
        if (visible && services_.can_see && !services_.can_see(session, *owner, *target, visible, error)) return false;
        if (!visible) return true; // AI_IsInSight false: OnTargetOutOfSight has no witch callback; target kept
        const auto event = ranged_range_event_v1(distance_squared(source_target_position(*owner), source_target_position(*target)),
                                                 def.range_min, def.range_max);
        if (event == RangedRangeEventV1::close_range) {
            // OnTargetInCloseRange: Stop(); LookAtTarget(); DoSkill(n).
            if (runtime.approaching && services_.stop && !session.owns_pose(id)) {
                if (!services_.stop(session, *owner, error)) return false;
            }
            runtime.approaching = false;
            if (!runtime.cast.active && def.ai.close_range_looks_at_target && !look_at(session, id, runtime.target, error)) return false;
            return begin_skill(session, id, def, runtime, error);
        }
        if (event == RangedRangeEventV1::ranged_range) {
            // OnTargetInRangedRange: MoveTo(GetTarget()) while Idle (not casting/hurt) without a path.
            if (runtime.cast.active || session.owns_pose(id)) return true;
            if (!services_.approach) return true;
            if (!services_.approach(session, *owner, *target, error)) return false;
            if (!runtime.approaching) { ++report_.approaches; log("approach actor=" + std::to_string(id)); }
            runtime.approaching = true;
            return true;
        }
        // OnTargetOutOfRange: Stop(); ClearAggro; ClearTarget().
        if (def.ai.out_of_range_clears_target) {
            if (runtime.approaching && services_.stop && !services_.stop(session, *owner, error)) return false;
            runtime.approaching = false;
            runtime.target = invalid_actor_id;
            owner->target_id = invalid_actor_id;
            ++report_.target_losses;
            log("target out of range actor=" + std::to_string(id));
        }
        return true;
    }
    // No target: OnEnemySpotted for an eligible enemy inside the authored no-aggro view radius (the same search radius
    // as the port's monster controller), then Do_SearchVictims picks the closest Player.
    ActorId spotted = invalid_actor_id;
    for (const auto& pair : world->actors()) {
        if (pair.first == id || !world->eligible_target(*owner, pair.second)) continue;
        if (!inside_view(*owner, pair.second, ai->view_radius_no_aggro)) continue;
        bool visible = true;
        if (services_.can_see && !services_.can_see(session, *owner, pair.second, visible, error)) return false;
        if (visible) { spotted = pair.first; break; }
    }
    if (spotted == invalid_actor_id) return true;
    if (!runtime.spotted) {
        runtime.spotted = true;
        ++report_.spotted;
        std::string presentation;
        if (!def.ai.spotted_music.empty() && services_.play_music &&
            !services_.play_music(def.ai.spotted_music, def.ai.spotted_music_fade_ms, presentation))
            log("spotted music not played: " + presentation);
    }
    ActorId chosen = invalid_actor_id;
    if (def.ai.enemy_spotted_searches_player) {
        float best = std::numeric_limits<float>::max();
        for (const auto& pair : world->actors()) {
            const auto* t = world->traits(pair.first);
            if (!t || !t->is_player || !world->eligible_target(*owner, pair.second)) continue;
            const float d2 = distance_squared(owner->transform.position.data(), pair.second.transform.position.data());
            if (d2 < best) { best = d2; chosen = pair.first; }
        }
    }
    if (chosen != invalid_actor_id) {
        runtime.target = chosen;
        owner->target_id = chosen;
        if (!look_at(session, id, chosen, error)) return false;
    }
    if (def.ai.teleport.present && !runtime.timer_active) { runtime.timer_active = true; runtime.timer_ms = 0; }
    log("enemy spotted actor=" + std::to_string(id) + " spotted=" + std::to_string(spotted) + " target=" + std::to_string(chosen) +
        " teleportTimer=" + std::to_string(runtime.timer_active));
    return true;
}

bool RuntimeRangedSkillMonsterV1::update(CombatSession& session, double dt, bool ai, std::string& error) {
    error.clear();
    if (!std::isfinite(dt) || dt < 0) return fail(error, "Ranged skill monster dt must be finite and nonnegative");
    auto* world = session.world();
    if (!world || !session.original_ai_tables()) return fail(error, "Ranged skill monsters require the live session world/AI tables");
    const auto lease = session.actor_binding_lease();
    if (!lease_known_ || !same_lease(lease_, lease)) {
        actors_.clear();
        projectiles_.clear();
        lease_ = lease;
        lease_known_ = true;
    }
    for (const auto& pair : definitions_) {
        if (!ai || !world->find_actor(pair.first)) continue;
        if (!tick_actor(session, pair.first, pair.second, actors_[pair.first], dt, error)) return false;
    }
    const double total_ms = dt * 1000.0 + fraction_ms_;
    const auto dt_ms = static_cast<std::int32_t>(total_ms);
    fraction_ms_ = total_ms - dt_ms;
    return update_projectiles(session, dt_ms, error);
}

} // namespace dh::foundation::enemy_ai
