#include "spawn_character_v1.hpp"

#include "../enemy_ai/runtime_monster_level_policy_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <sstream>
#include <stdexcept>

namespace dh::foundation::spawn {
namespace {

const char* clip_name(SpawnClipPolicy clip) {
    return clip == SpawnClipPolicy::source_spawn_state ? "source_spawn_state" : "none";
}

std::string format_position(const std::array<float, 3>& p) {
    char buffer[96];
    std::snprintf(buffer, sizeof buffer, "%.3f,%.3f,%.3f", p[0], p[1], p[2]);
    return buffer;
}

bool finite_position(const std::array<float, 3>& p) {
    return std::isfinite(p[0]) && std::isfinite(p[1]) && std::isfinite(p[2]);
}

bool row_index(const dh2::data::CharacterTable& characters, const std::string& name, std::size_t& index) {
    const auto found = std::find(characters.names.begin(), characters.names.end(), name);
    if (found == characters.names.end()) return false;
    index = static_cast<std::size_t>(found - characters.names.begin());
    return index < characters.rows.size();
}

std::int32_t field(const dh2::data::CharacterTable& characters, const std::string& row, const char* name) {
    const auto* value = dh2::data::property(characters, row, name);
    return value ? *value : 0;
}

std::string reject_line(const SpawnRequestV1& request, const std::string& reason) {
    return "SPAWN rejected template=" + request.name + " summoner=" + std::to_string(request.summoner) +
           " reason=" + reason;
}

} // namespace

bool parse_spawn_test_v1(const std::string& text, SpawnTestRequestV1& output, std::string& error) {
    const auto first = text.find('@');
    const auto second = first == std::string::npos ? std::string::npos : text.find('@', first + 1);
    if (first == std::string::npos || second == std::string::npos || first == 0) {
        error = "--spawn-test expects TEMPLATE@X,Y,Z@FRAME";
        return false;
    }
    SpawnTestRequestV1 parsed;
    parsed.name = text.substr(0, first);
    const auto coords = text.substr(first + 1, second - first - 1);
    const auto frame = text.substr(second + 1);
    std::istringstream stream(coords);
    char c1 = 0, c2 = 0;
    if (!(stream >> parsed.position[0] >> c1 >> parsed.position[1] >> c2 >> parsed.position[2]) ||
        c1 != ',' || c2 != ',' || !(stream >> std::ws).eof() || !finite_position(parsed.position)) {
        error = "--spawn-test coordinates must be three finite numbers X,Y,Z";
        return false;
    }
    try {
        std::size_t used = 0;
        parsed.frame = std::stoll(frame, &used);
        if (used != frame.size() || parsed.frame < 0) throw std::invalid_argument("frame");
    } catch (const std::exception&) {
        error = "--spawn-test frame must be a nonnegative integer";
        return false;
    }
    output = parsed;
    error.clear();
    return true;
}

bool spawn_candidate_profiles_v1(const std::string& name, const dh2::data::CharacterTable& characters,
                                 const dh2::data::CharacterTemplateTableV78& templates,
                                 std::vector<std::string>& profiles, std::string& error) {
    profiles.clear();
    std::size_t row = 0;
    if (row_index(characters, name, row)) {
        profiles.push_back(name);
        error.clear();
        return true;
    }
    const auto template_row = templates.find(name.c_str());
    if (template_row < 0) {
        error = "Unknown CharacterTable row or Charater_Templates row: " + name;
        return false;
    }
    const std::int16_t* members = nullptr;
    std::uint32_t count = 0;
    if (!templates.preset(name, members, count, error)) return false;
    if (!count || !members) {
        error = "Charater_Templates row has no members: " + name;
        return false;
    }
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto member = members[i];
        if (member < 0 || static_cast<std::size_t>(member) >= characters.names.size()) {
            error = "Charater_Templates member is outside the CharacterTable: " + name;
            return false;
        }
        const auto& member_name = characters.names[static_cast<std::size_t>(member)];
        if (std::find(profiles.begin(), profiles.end(), member_name) == profiles.end()) profiles.push_back(member_name);
    }
    error.clear();
    return true;
}

bool spawn_level_raw_v1(const dh2::data::CharacterTable& characters, const std::string& row_name,
                        std::int32_t host_level_raw, std::int32_t& level_raw, std::string& error) {
    level_raw = -1;
    std::size_t row = 0;
    if (!row_index(characters, row_name, row)) {
        error = "Level scaling row is absent: " + row_name;
        return false;
    }
    const auto max_raw = field(characters, row_name, "LevelMax");
    if (max_raw <= -1) { // Source monster.luac tests monster_LevelMax > -1; -1 is the absent sentinel.
        error.clear();
        return true; // Not a level-scaled row: the profile's base level stands.
    }
    // Source fixed point is 256 per level; monster.luac passes float32 values.
    enemy_ai::RuntimeMonsterLevelInputsV1 inputs;
    inputs.source_reads_complete = true;
    inputs.monster_level_max = static_cast<float>(max_raw) / 256.0f;
    inputs.monster_level_min = static_cast<float>(field(characters, row_name, "LevelMin")) / 256.0f;
    inputs.monster_level_offset = static_cast<float>(field(characters, row_name, "LevelOffset")) / 256.0f;
    inputs.host_player_level = static_cast<float>(host_level_raw) / 256.0f;
    inputs.current_range_values_returned = false;
    enemy_ai::RuntimeMonsterLevelDecisionV1 decision;
    if (!enemy_ai::resolve_runtime_monster_level_v1(inputs, decision, error)) return false;
    if (!decision.calls_set_level) {
        error.clear();
        return true;
    }
    level_raw = static_cast<std::int32_t>(std::lround(decision.level_passed_to_to_fixed * 256.0f));
    error.clear();
    return true;
}

bool SpawnPoolV1::reserve(const std::string& profile_id, std::uint32_t count, std::int32_t level_raw, std::string& error) {
    if (profile_id.empty() || count == 0) {
        error = "Spawn pool reservation needs a profile and a positive count";
        return false;
    }
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto stable_id = pool_stable_id_base + slots_.size();
        SpawnPoolSlotV1 slot;
        slot.stable_id = stable_id;
        slot.profile_id = profile_id;
        slot.level_raw = level_raw;
        slots_.push_back(std::move(slot));
    }
    error.clear();
    return true;
}

std::vector<ActorDefinition> SpawnPoolV1::declarations(const std::string& level_uri) const {
    std::vector<ActorDefinition> output;
    output.reserve(slots_.size());
    for (std::size_t i = 0; i < slots_.size(); ++i) {
        const auto& slot = slots_[i];
        ActorDefinition definition;
        definition.stableId = slot.stable_id;
        definition.sourceId = "spawn-pool/" + slot.profile_id + "/" + std::to_string(i);
        definition.sourcePath = level_uri;
        definition.moduleName = "spawn-pool";
        definition.name = "spawn_pool_" + slot.profile_id + "_" + std::to_string(i);
        definition.gametype = "Character";
        definition.role = "spawn-pool";
        // Same declaration facts an authored intro actor carries: created
        // hidden in the Limbus/PreSpawn state until a spawn admits it.
        definition.properties["charpropsname"] = slot.profile_id;
        definition.properties["ai_state"] = "Limbus";
        definition.properties["ai_state_visible"] = "0";
        definition.properties["auto_spawn"] = "0";
        definition.placement = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        output.push_back(std::move(definition));
    }
    return output;
}

bool SpawnPoolV1::owns(std::uint64_t stable_id) const noexcept {
    return slot(stable_id) != nullptr;
}

const SpawnPoolSlotV1* SpawnPoolV1::slot(std::uint64_t stable_id) const noexcept {
    for (const auto& candidate : slots_)
        if (candidate.stable_id == stable_id) return &candidate;
    return nullptr;
}

bool SpawnPoolV1::acquire(const std::string& profile_id, std::uint64_t summoner, std::uint64_t& stable_id, std::string& error) {
    for (auto& slot : slots_) {
        if (slot.busy || slot.profile_id != profile_id) continue;
        slot.busy = true;
        slot.summoner = summoner;
        slot.failed = false;
        stable_id = slot.stable_id;
        error.clear();
        return true;
    }
    error = "No free spawn slot for profile " + profile_id;
    return false;
}

bool SpawnPoolV1::release(std::uint64_t stable_id, std::string& error) {
    for (auto& slot : slots_) {
        if (slot.stable_id != stable_id) continue;
        if (!slot.busy) { error = "Spawn slot is not busy"; return false; }
        if (slot.failed) { error = "Failed spawn slot is not released by despawn"; return false; }
        slot.busy = false;
        slot.summoner = 0;
        error.clear();
        return true;
    }
    error = "Unknown spawn slot";
    return false;
}

void SpawnPoolV1::mark_failed(std::uint64_t stable_id) noexcept {
    for (auto& slot : slots_)
        if (slot.stable_id == stable_id) slot.failed = true;
}

std::size_t SpawnPoolV1::busy_count() const noexcept {
    return static_cast<std::size_t>(std::count_if(slots_.begin(), slots_.end(), [](const auto& s) { return s.busy; }));
}

bool spawn_character_v1(SpawnPoolV1& pool, const SpawnRequestV1& request,
                        const SpawnServicesV1& services, SpawnResultV1& result, std::string& error) {
    result = SpawnResultV1{};
    const auto reject = [&](const std::string& reason) {
        result.line = reject_line(request, reason);
        result.error = reason;
        if (services.log) services.log(result.line);
        error = reason;
        return false;
    };
    if (request.name.empty()) return reject("empty template name");
    if (!finite_position(request.position) || !std::isfinite(request.heading_radians)) return reject("nonfinite position or heading");
    if (request.host_level_raw <= 0) return reject("host level must be positive");
    if (!services.characters || !services.templates || !services.random_index || !services.profile_available ||
        !services.place || !services.begin) return reject("spawn services are incomplete");

    // 1. Resolve. A template draw uses the caller's shared RNG exactly once.
    std::size_t direct_row = 0;
    if (row_index(*services.characters, request.name, direct_row)) {
        result.plan.profile_id = request.name;
    } else {
        // Charater_Templates row: one weighted draw from the caller's shared RNG.
        const auto template_row = services.templates->find(request.name.c_str());
        if (template_row < 0) return reject("unknown CharacterTable row or Charater_Templates row");
        const std::int16_t* members = nullptr;
        std::uint32_t count = 0;
        if (!services.templates->preset(request.name, members, count, error)) return reject(error);
        std::int32_t selected = -1;
        if (!count || !members || !services.random_index(static_cast<std::int32_t>(count), selected, error) ||
            selected < 0 || static_cast<std::uint32_t>(selected) >= count)
            return reject("shared RNG draw for template failed");
        const auto member = members[selected];
        if (member < 0 || static_cast<std::size_t>(member) >= services.characters->names.size())
            return reject("template member outside CharacterTable");
        result.plan.template_row = template_row;
        result.plan.profile_id = services.characters->names[static_cast<std::size_t>(member)];
    }
    std::size_t row_pos = 0;
    if (!row_index(*services.characters, result.plan.profile_id, row_pos)) return reject("selected row is absent");
    result.plan.character_row = static_cast<std::int32_t>(row_pos);
    result.plan.faction = static_cast<std::int32_t>(field(*services.characters, result.plan.profile_id, "AIFaction"));
    std::string level_error;
    if (!spawn_level_raw_v1(*services.characters, result.plan.profile_id, request.host_level_raw, result.plan.level_raw, level_error))
        return reject(level_error);

    // 2. Admission of the profile in this session (visual, policy, population).
    std::string available_error;
    if (!services.profile_available(result.plan.profile_id, available_error))
        return reject("profile not admitted: " + result.plan.profile_id + (available_error.empty() ? "" : " (" + available_error + ")"));

    // 3. Free slot. Busy slots are never reused.
    std::uint64_t actor = 0;
    if (!pool.acquire(result.plan.profile_id, request.summoner, actor, error))
        return reject(error);
    result.actor = actor;

    // 4. Place, then start the clip policy. Failure before begin frees the slot.
    if (!services.place(actor, request.position, request.heading_radians, error)) {
        std::string release_error;
        pool.release(actor, release_error);
        return reject("placement failed: " + error);
    }
    std::string begin_error;
    if (!services.begin(actor, request.clip, begin_error)) {
        // The owner was touched: keep the slot busy and mark it failed.
        pool.mark_failed(actor);
        result.line = "SPAWN failed-state template=" + request.name + " profile=" + result.plan.profile_id +
                      " actor=" + std::to_string(actor) + " reason=" + begin_error;
        result.error = begin_error;
        if (services.log) services.log(result.line);
        error = begin_error;
        return false;
    }

    result.spawned = true;
    result.line = "SPAWN ok template=" + request.name + " profile=" + result.plan.profile_id +
                  " row=" + std::to_string(result.plan.character_row) +
                  " actor=" + std::to_string(actor) +
                  " pos=" + format_position(request.position) +
                  " heading=" + std::to_string(request.heading_radians) +
                  " level=" + std::to_string(result.plan.level_raw) +
                  " clip=" + clip_name(request.clip) +
                  " summoner=" + std::to_string(request.summoner) +
                  " transient=1 busy=" + std::to_string(pool.busy_count()) + "/" + std::to_string(pool.slots().size());
    if (services.log) services.log(result.line);
    error.clear();
    return true;
}

bool despawn_character_v1(SpawnPoolV1& pool, std::uint64_t actor, const SpawnServicesV1& services, std::string& error) {
    const auto* slot = pool.slot(actor);
    if (!slot || !slot->busy) { error = "Actor is not a live spawn"; return false; }
    if (slot->failed) { error = "Failed spawn actor state is unknown; not despawned"; return false; }
    if (!services.hide) { error = "Despawn services are incomplete"; return false; }
    if (!services.hide(actor, error)) return false;
    if (!pool.release(actor, error)) return false;
    if (services.log) services.log("SPAWN despawn actor=" + std::to_string(actor) + " busy=" + std::to_string(pool.busy_count()) + "/" + std::to_string(pool.slots().size()));
    return true;
}

} // namespace dh::foundation::spawn
