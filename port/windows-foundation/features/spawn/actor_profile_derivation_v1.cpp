#include "actor_profile_derivation_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../original_attack_sequence.hpp"
#include "../../../game-data/animation_tables.hpp"

#include <algorithm>
#include <filesystem>
#include <cmath>
#include <cstdio>
#include <stdexcept>

namespace dh::foundation::spawn {
namespace {

const char* const kProfileStates[] = {"Template", "Idle", "Walk", "Run", "Attack", "Died", "Limbus"};
const char* const kMeleeStates[] = {"Attack", "AttackStatic", "Died", "Idle", "Injured", "PreSpawn", "Run", "Spawn", "Walk"};
// Source scale fields carry the original-character-scale encoding (authored profiles agree).
const char* const kDespawnState = "Despawn";

bool has_despawn_state(const ProfileDerivationTablesV1& tables) {
    const auto& names = tables.animations.state_names;
    return std::find(names.begin(), names.end(), std::string(kDespawnState)) != names.end();
}
const char* const kScaleFields[] = {"Scale_X", "Scale_Y", "Scale_Z"};

std::size_t character_index(const ProfileDerivationTablesV1& tables, const std::string& row, std::string& error) {
    const auto& names = tables.properties.characters.names;
    const auto found = std::find(names.begin(), names.end(), row);
    if (found == names.end()) {
        error = "CharacterTable row is absent: " + row;
        return names.size();
    }
    return static_cast<std::size_t>(found - names.begin());
}

std::int32_t raw_field(const ProfileDerivationTablesV1& tables, std::size_t index, const char* field) {
    const auto& fields = tables.properties.characters.fields;
    const auto column = std::find(fields.begin(), fields.end(), std::string(field));
    if (column == fields.end()) return -1;
    return tables.properties.characters.rows[index][static_cast<std::size_t>(column - fields.begin())];
}

// CharAnim sequence ids for one state of one AnimTable row (empty when absent).
bool state_sequence_ids(const ProfileDerivationTablesV1& tables, std::int32_t animation_table,
                        const std::string& state, std::vector<std::int32_t>& ids, std::string& error) {
    ids.clear();
    const auto& animations = tables.animations;
    if (animation_table < 0 || static_cast<std::size_t>(animation_table) >= animations.characters.size()) {
        error = "AnimTable index outside CharAnim table";
        return false;
    }
    const auto state_index = std::find(animations.state_names.begin(), animations.state_names.end(), state);
    if (state_index == animations.state_names.end()) {
        error = "CharAnim state is absent: " + state;
        return false;
    }
    const auto& values = animations.characters[static_cast<std::size_t>(animation_table)]
                             .fields[static_cast<std::size_t>(state_index - animations.state_names.begin())];
    for (const auto id : values)
        if (id >= 0) ids.push_back(id);
    return true;
}

// Ordered unique leaf clip URIs reachable from a sequence (redirects are followed).
bool flatten_clips(const ProfileDerivationTablesV1& tables, std::int32_t sequence, std::vector<std::string>& out,
                   std::vector<std::int32_t>& path, std::string& error) {
    const auto& sequences = tables.animations.sequences;
    if (sequence < 0 || static_cast<std::size_t>(sequence) >= sequences.size() || path.size() >= 32 ||
        std::find(path.begin(), path.end(), sequence) != path.end()) {
        error = "Invalid original sequence reference or redirect cycle";
        return false;
    }
    path.push_back(sequence);
    for (const auto& step : sequences[static_cast<std::size_t>(sequence)].steps) {
        if (step.redir) {
            if (!flatten_clips(tables, step.anim, out, path, error)) return false;
        } else if (step.anim >= 0) {
            const auto* uri = dh2::data::animation_clip(step, tables.clips);
            if (!uri) {
                error = "AnimDict entry is absent for animation " + std::to_string(step.anim);
                return false;
            }
            if (std::find(out.begin(), out.end(), *uri) == out.end()) out.push_back(*uri);
        }
    }
    path.pop_back();
    return true;
}

bool state_clips(const ProfileDerivationTablesV1& tables, std::int32_t animation_table, const std::string& state,
                 std::vector<std::string>& out, std::string& error) {
    out.clear();
    std::vector<std::int32_t> ids;
    if (!state_sequence_ids(tables, animation_table, state, ids, error)) return false;
    for (const auto id : ids) {
        std::vector<std::int32_t> path;
        if (!flatten_clips(tables, id, out, path, error)) return false;
    }
    return true;
}

// Same recursion as the authored melee export: a redirect step nests the referenced sequence.
bool emit_steps(const ProfileDerivationTablesV1& tables, std::int32_t sequence, std::vector<OriginalMeleeStep>& out,
                std::vector<std::int32_t>& path, std::string& error) {
    const auto& sequences = tables.animations.sequences;
    if (sequence < 0 || static_cast<std::size_t>(sequence) >= sequences.size() || path.size() >= 32 ||
        std::find(path.begin(), path.end(), sequence) != path.end()) {
        error = "Invalid original sequence redirect";
        return false;
    }
    path.push_back(sequence);
    const auto& steps = sequences[static_cast<std::size_t>(sequence)].steps;
    for (std::size_t order = 0; order < steps.size(); ++order) {
        const auto& step = steps[order];
        OriginalMeleeStep node;
        node.index = static_cast<std::int64_t>(order);
        node.animationId = step.anim;
        node.redirect = step.redir;
        node.blendOut = step.blend_out;
        node.moveGO = step.move_go ? 1 : 0;
        node.speed = static_cast<double>(step.speed);
        if (step.redir) {
            if (!emit_steps(tables, step.anim, node.children, path, error)) return false;
        } else if (step.anim >= 0) {
            const auto* uri = dh2::data::animation_clip(step, tables.clips);
            if (!uri) {
                error = "AnimDict entry is absent for animation " + std::to_string(step.anim);
                return false;
            }
            node.uri = *uri;
        }
        out.push_back(std::move(node));
    }
    path.pop_back();
    return true;
}

std::string format_float(double value) {
    char buffer[64];
    std::snprintf(buffer, sizeof buffer, "%.1f", value);
    return buffer;
}

} // namespace

bool load_profile_derivation_tables_v1(const AssetCatalog& assets, const std::string& pydata_root,
                                       ProfileDerivationTablesV1& output, std::string& error) {
    try {
        // Reads go through the runtime content path (data/pydata/...), which is where the package
        // stores the animation dictionary names; pydata_root is kept for the caller's provenance only.
        (void)pydata_root;
        ProfileDerivationTablesV1 next;
        const auto read = [&](const char* name) { return read_content(assets, std::string("data/pydata/") + name); };
        const auto bytes = [](const auto& data) { return dh2::data::Bytes{data.data(), data.size()}; };
        const auto characters = read("character_properties_pyarray.bin");
        const auto character_names = read("character_properties_pyarraynames.bin");
        const auto character_fields = read("character_properties_pystructnames.bin");
        if (!dh2::data::load_characters(bytes(characters), bytes(character_names), bytes(character_fields),
                                        next.properties.characters, error)) return false;
        const auto dictionary_names = read("animations_dictionary_pyarraynames.bin");
        const auto dictionary_values = read("animations_dictionary_pyarray.bin");
        const auto records = read("animations_pyarray.bin");
        const auto animation_names = read("animations_pyarraynames.bin");
        const auto fields = read("animations_pystructnames.bin");
        if (!dh2::data::load_dictionary(bytes(dictionary_names), bytes(dictionary_values), next.clips, error)) return false;
        if (!dh2::data::load_animation_tables(bytes(records), bytes(animation_names), bytes(fields), next.clips,
                                              next.animations, error)) return false;
        const auto model_names = read("character_models_dictionary_pyarraynames.bin");
        const auto model_values = read("character_models_dictionary_pyarray.bin");
        if (!dh2::data::load_dictionary(bytes(model_names), bytes(model_values), next.models, error)) return false;
        const auto ai = read("ai_pyarray.bin"), ai_names = read("ai_pyarraynames.bin"), ai_fields = read("ai_pystructnames.bin");
        const auto factions = read("ai_factions_pyarray.bin"), faction_names = read("ai_factions_pyarraynames.bin"),
                   faction_fields = read("ai_factions_pystructnames.bin");
        if (!dh2::data::load_ai(bytes(ai), bytes(ai_names), bytes(ai_fields), bytes(factions), bytes(faction_names),
                                bytes(faction_fields), next.ai, error)) return false;
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

const std::vector<std::string>& profile_states_v1() {
    static const std::vector<std::string> states(std::begin(kProfileStates), std::end(kProfileStates));
    return states;
}

const std::vector<std::string>& melee_states_v1() {
    static const std::vector<std::string> states(std::begin(kMeleeStates), std::end(kMeleeStates));
    return states;
}

bool derive_actor_profile_v1(const ProfileDerivationTablesV1& tables, const std::string& row,
                             ActorProfile& output, std::string& error) {
    const auto index = character_index(tables, row, error);
    if (index == tables.properties.characters.names.size()) return false;
    const auto animation_table = raw_field(tables, index, "AnimTable");
    const auto model_index = raw_field(tables, index, "ModelFile");
    if (animation_table < 0 || static_cast<std::size_t>(animation_table) >= tables.animations.character_names.size()) {
        error = "CharacterTable AnimTable is outside the AnimTable: " + row;
        return false;
    }
    if (model_index < 0 || static_cast<std::size_t>(model_index) >= tables.models.values.size()) {
        error = "CharacterTable ModelFile is outside the model dictionary: " + row;
        return false;
    }
    ActorProfile profile;
    profile.id = row;
    profile.character_uri = row;
    profile.model_uri = tables.models.values[static_cast<std::size_t>(model_index)];
    profile.property_row = std::to_string(index);
    profile.animation_table = std::to_string(animation_table);
    profile.animation_table_name = tables.animations.character_names[static_cast<std::size_t>(animation_table)];
    // P16 DESPAWN: Despawn is published only when the AnimTable carries Despawn clips (authored XML omits it).
    std::vector<std::string> profile_states = profile_states_v1();
    if (has_despawn_state(tables)) {
        std::vector<std::string> despawn;
        if (!state_clips(tables, static_cast<std::int32_t>(animation_table), kDespawnState, despawn, error)) return false;
        if (!despawn.empty()) profile_states.push_back(kDespawnState);
    }
    for (const auto& state : profile_states) {
        std::vector<std::string> clips;
        if (!state_clips(tables, static_cast<std::int32_t>(animation_table), state, clips, error)) return false;
        std::vector<ActorClip> entries;
        for (const auto& uri : clips) entries.push_back(ActorClip{uri, 1.0, false});
        if (state == "Template" && !clips.empty()) profile.template_clip_uri = clips.front();
        profile.states.emplace(state, std::move(entries));
    }
    for (std::size_t field = 0; field < tables.properties.characters.fields.size(); ++field) {
        const auto& name = tables.properties.characters.fields[field];
        const bool scale = std::find(std::begin(kScaleFields), std::end(kScaleFields), name) != std::end(kScaleFields);
        profile.raw_properties.emplace(name, ActorRawProperty{
            tables.properties.characters.rows[index][field],
            scale ? "original-character-scale" : "original-table-raw-integer-uninterpreted"});
    }
    output = std::move(profile);
    error.clear();
    return true;
}

bool derive_melee_actor_v1(const ProfileDerivationTablesV1& tables, const std::string& row,
                           OriginalMeleeActor& output, std::string& error) {
    const auto index = character_index(tables, row, error);
    if (index == tables.properties.characters.names.size()) return false;
    const auto animation_table = raw_field(tables, index, "AnimTable");
    const auto ai_id = raw_field(tables, index, "AI");
    const auto model_index = raw_field(tables, index, "ModelFile");
    if (ai_id < 0 || static_cast<std::size_t>(ai_id) >= tables.ai.names.size()) {
        error = "CharacterTable AI is outside the AI table: " + row;
        return false;
    }
    if (model_index < 0 || static_cast<std::size_t>(model_index) >= tables.models.values.size()) {
        error = "CharacterTable ModelFile is outside the model dictionary: " + row;
        return false;
    }
    OriginalMeleeActor actor;
    actor.id = row;
    actor.propertyRow = static_cast<std::int64_t>(index);
    actor.factionId = raw_field(tables, index, "AIFaction");
    actor.aiId = ai_id;
    actor.aiName = tables.ai.names[static_cast<std::size_t>(ai_id)];
    actor.model = tables.models.values[static_cast<std::size_t>(model_index)];
    std::vector<std::string> profile_clips;
    if (!state_clips(tables, animation_table, "Template", profile_clips, error)) return false;
    actor.templateClip = profile_clips.empty() ? std::string() : profile_clips.front();
    if (const auto* props = ai_props(tables.ai, static_cast<std::int32_t>(ai_id))) {
        actor.aiProperties = {
            {"AttackDelay", std::to_string(props->attack_delay)},
            {"CombatBeat", std::to_string(props->combat_beat)},
            {"CombatMusic", std::to_string(props->combat_music)},
            {"DelayedLoad", std::to_string(props->delayed_load)},
            {"Flags", std::to_string(props->flags)},
            {"InteractRadius", format_float(props->interact_radius)},
            {"LeashDistance", format_float(props->leash_distance)},
            {"MeleeRadius", format_float(props->melee_radius)},
            {"OnAggroSFX", std::to_string(props->on_aggro_sfx)},
            {"Script", props->script},
            {"SelfFX", std::to_string(props->self_fx)},
            {"Trophy", std::to_string(props->trophy)},
            {"Type", std::to_string(props->type)},
            {"ViewRadius", format_float(props->view_radius)},
            {"ViewRadiusNoAggro", format_float(props->view_radius_no_aggro)},
        };
    } else {
        error = "AI row has no decoded properties: " + tables.ai.names[static_cast<std::size_t>(ai_id)];
        return false;
    }
    // P16 DESPAWN: the Despawn sequences join the melee states when the AnimTable carries them.
    std::vector<std::string> melee_states = melee_states_v1();
    if (has_despawn_state(tables)) {
        std::vector<std::int32_t> despawn;
        if (!state_sequence_ids(tables, animation_table, kDespawnState, despawn, error)) return false;
        if (!despawn.empty()) melee_states.push_back(kDespawnState);
    }
    for (const auto& state : melee_states) {
        std::vector<std::int32_t> ids;
        if (!state_sequence_ids(tables, animation_table, state, ids, error)) return false;
        std::vector<OriginalMeleeSequence> sequences;
        for (const auto id : ids) {
            if (id < 0 || static_cast<std::size_t>(id) >= tables.animations.sequences.size() ||
                static_cast<std::size_t>(id) >= tables.animations.sequence_names.size()) {
                error = "State sequence is outside the AnimTable: " + state;
                return false;
            }
            const auto& source = tables.animations.sequences[static_cast<std::size_t>(id)];
            OriginalMeleeSequence sequence;
            sequence.id = id;
            sequence.name = tables.animations.sequence_names[static_cast<std::size_t>(id)];
            sequence.loop = source.loop;
            sequence.type = source.type;
            std::vector<std::int32_t> path;
            if (!emit_steps(tables, id, sequence.steps, path, error)) return false;
            sequences.push_back(std::move(sequence));
        }
        actor.states.emplace(state, std::move(sequences));
    }
    output = std::move(actor);
    error.clear();
    return true;
}

bool derive_enemy_combat_policy_v1(const ProfileDerivationTablesV1& tables, const std::string& row,
                                   CombatSessionProfile& output, std::string& error) {
    const auto index = character_index(tables, row, error);
    if (index == tables.properties.characters.names.size()) return false;
    CombatSessionProfile policy;
    policy.initialIdle = CombatSessionChoice{"Idle", 0, {0}};
    policy.reaction = CombatSessionChoice{"Injured", 0, {0}};
    policy.death = CombatSessionChoice{"Died", 0, {0}};
    OriginalAttackSelection attack;
    attack.state = "Attack";
    attack.variant = 0;
    attack.group_path = {0};
    policy.sequenceAction = attack;
    policy.damageMarkerNames = {"attack_mainhand"};
    policy.retainedPhaseClock = true;
    policy.motionRoot = "auto";
    policy.customization.allow_missing_animation_targets = true;
    policy.propertyOptions.refill_vitals = true;
    output = std::move(policy);
    error.clear();
    return true;
}

} // namespace dh::foundation::spawn
