#include "runtime_player_incoming_animation_bank_v1.hpp"

#include "../../content_paths.hpp"

#include <algorithm>
#include <charconv>
#include <functional>
#include <set>
#include <stdexcept>

namespace dh::foundation::combat {
namespace {
using namespace dh2::data;

void require(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

std::int32_t profile_table_id(const ActorProfile& profile) {
    require(!profile.animation_table.empty(),
            "Incoming animation bank profile has no source animationTable");
    std::int32_t value = -1;
    const auto parsed = std::from_chars(profile.animation_table.data(),
        profile.animation_table.data() + profile.animation_table.size(), value);
    require(parsed.ec == std::errc{} &&
            parsed.ptr == profile.animation_table.data() + profile.animation_table.size() && value >= 0,
            "Incoming animation bank profile animationTable is not an exact nonnegative ID");
    return value;
}

std::int32_t state_root(const AnimationTables& animations, std::int32_t table,
                        const char* state) {
    const auto* sequence = animation_state(animations, table, state);
    require(sequence != nullptr,
            std::string("Source player CharAnimTable is missing ") + state + " root");
    const auto offset = sequence - animations.sequences.data();
    require(offset >= 0 && static_cast<std::size_t>(offset) < animations.sequences.size(),
            "Source player animation root escaped the loaded AnimTable");
    return static_cast<std::int32_t>(offset);
}

void add_policy(RuntimePlayerIncomingAnimationBankV1& bank,
                const AnimationTables& animations, std::int32_t id) {
    require(id >= 0 && static_cast<std::size_t>(id) < animations.sequences.size() &&
            static_cast<std::size_t>(id) < animations.sequence_names.size(),
            "Incoming sequence reference is outside the actual AnimTable");
    const auto& sequence = animations.sequences[static_cast<std::size_t>(id)];
    OriginalSequencePolicy policy{id, sequence.type, sequence.loop,
        animations.sequence_names[static_cast<std::size_t>(id)]};
    const auto inserted = bank.sequence_policies.emplace(id, policy);
    require(inserted.second || (inserted.first->second.type == policy.type &&
            inserted.first->second.loop == policy.loop &&
            inserted.first->second.name == policy.name),
            "Shared incoming sequence has inconsistent source Type/Loop metadata");
}

void build_state(const AssetCatalog& assets, const ActorProfile& profile,
                 const AnimationTables& animations, const Dictionary& clips,
                 const std::string& role, const std::string& state, std::int32_t root_id,
                 RuntimePlayerIncomingAnimationBankV1& bank,
                 std::set<std::string>& aliases) {
    const auto& root = animations.sequences.at(static_cast<std::size_t>(root_id));
    OriginalCombatSequencePlan plan;
    plan.state = state;
    plan.name = animations.sequence_names.at(static_cast<std::size_t>(root_id));
    plan.id = root_id;
    plan.type = root.type;
    plan.loop = root.loop;
    bank.visual.stateNames.push_back(state);

    std::set<std::int32_t> active;
    std::function<void(std::int32_t, std::vector<std::size_t>,
                       std::vector<std::int64_t>,
                       std::vector<OriginalCombatRedirect>)> visit;
    visit = [&](std::int32_t id, std::vector<std::size_t> prefix,
                std::vector<std::int64_t> source_indices,
                std::vector<OriginalCombatRedirect> ancestors) {
        require(prefix.size() < 3 && active.insert(id).second,
                "Incoming animation redirect exceeds source depth or contains a cycle");
        add_policy(bank, animations, id);
        const auto& sequence = animations.sequences.at(static_cast<std::size_t>(id));
        require(!sequence.steps.empty(), "Incoming source sequence has no authored steps");
        for (std::size_t i = 0; i < sequence.steps.size(); ++i) {
            const auto& step = sequence.steps[i];
            auto path = prefix;
            path.push_back(i);
            auto indices = source_indices;
            indices.push_back(static_cast<std::int64_t>(i));
            require(bank.steps.emplace(std::make_pair(state, path), step).second,
                    "Duplicate source incoming AnimationStep path");
            if (step.redir == 1) {
                auto next_ancestors = ancestors;
                next_ancestors.push_back({static_cast<std::int64_t>(i), step.anim,
                    step.redir, step.speed, step.blend_out, step.move_go, {}});
                visit(step.anim, std::move(path), std::move(indices),
                      std::move(next_ancestors));
                continue;
            }
            require(step.redir == 0 && step.anim >= 0,
                    "Incoming source animation leaf is symbolic or unsupported");
            const auto* uri = animation_clip(step, clips);
            require(uri && !uri->empty(), "Incoming source clip is absent from AnimDict");

            OriginalCombatPhase phase;
            phase.sourcePath = path;
            phase.sourceIndices = std::move(indices);
            phase.ancestors = ancestors;
            phase.animationId = step.anim;
            phase.redirect = step.redir;
            phase.blendOut = step.blend_out;
            phase.moveGO = step.move_go;
            phase.speed = step.speed;
            phase.sourceUri = *uri;
            phase.resolvedPath = resolve_content_path(assets, normalize_content_uri(*uri),
                profile.character_uri).lexically_relative(assets.root()).generic_string();
            phase.clipName = role + "/" + state + "/root-" +
                std::to_string(root_id) + "/phase-" + std::to_string(plan.phases.size());
            require(aliases.insert(phase.clipName).second,
                    "Generated incoming animation alias collides with actor visual");
            bank.visual.config.clips.emplace_back(phase.clipName, phase.resolvedPath);
            bank.visual.clipRates.emplace(phase.clipName, phase.speed);
            plan.phases.push_back(std::move(phase));
        }
        active.erase(id);
    };

    visit(root_id, {}, {}, {});
    require(!plan.phases.empty(), "Incoming animation root has no visual clip phases");
    bank.visual.sequences.push_back(std::move(plan));
}

const OriginalCombatSequencePlan& require_state(
        const RuntimePlayerIncomingAnimationBankV1& bank, const char* state) {
    const auto* plan = bank.visual.sequence(state, 0);
    require(plan != nullptr, std::string("Incoming bank is missing ") + state);
    return *plan;
}
} // namespace

bool build_runtime_player_incoming_animation_bank_v1(
    const AssetCatalog& assets, const ActorProfile& profile,
    const CharacterVisualConfig& same_actor_visual, const AnimationTables& animations,
    const Dictionary& clips, const std::string& role,
    RuntimePlayerIncomingAnimationBankV1& output, std::string& error) {
    try {
        require(!profile.id.empty() && !role.empty(),
                "Incoming animation bank requires source profile and feature role IDs");
        require(!same_actor_visual.model_path.empty(),
                "Incoming animation bank requires this actor's source model path");
        const auto table = profile_table_id(profile);
        require(static_cast<std::size_t>(table) < animations.characters.size(),
                "Profile animationTable is outside the loaded source CharAnimTable");
        require(!profile.animation_table_name.empty() &&
                static_cast<std::size_t>(table) < animations.character_names.size() &&
                profile.animation_table_name == animations.character_names[static_cast<std::size_t>(table)],
                "Profile animationTable ID/name do not identify the same original CharAnimTable row");
        const auto source_model = resolve_content_path(assets, profile.model_uri,
                                                       profile.character_uri);
        require(assets.resolve(same_actor_visual.model_path) == source_model,
                "Incoming visual model does not match the source actor profile");
        for (const auto& state : profile.states) {
            for (std::size_t i = 0; i < state.second.size(); ++i) {
                const auto alias = state.first + (i ? "#" + std::to_string(i + 1) : "");
                const auto configured = std::find_if(same_actor_visual.clips.begin(),
                    same_actor_visual.clips.end(), [&](const auto& clip) {
                        return clip.first == alias;
                    });
                require(configured != same_actor_visual.clips.end(),
                        "Incoming visual is missing a clip declared by the exact source profile");
                const auto source_path = resolve_content_path(assets, state.second[i].uri,
                                                               profile.character_uri);
                require(assets.resolve(configured->second) == source_path,
                        "Incoming visual profile clip differs from its authored source profile");
            }
        }

        RuntimePlayerIncomingAnimationBankV1 next;
        next.profile_id = profile.id;
        next.animation_table = table;
        next.injury_override_sequence_id = state_root(animations, table, "Injured");
        next.death_override_sequence_id = state_root(animations, table, "Died");
        next.visual.profileId = profile.id;
        next.visual.roleId = role;
        next.visual.config = same_actor_visual;
        std::set<std::string> aliases;
        for (const auto& clip : same_actor_visual.clips)
            require(!clip.first.empty() && aliases.insert(clip.first).second,
                    "Same actor visual contains an empty or duplicate clip alias");

        build_state(assets, profile, animations, clips, role, "Injured",
                    next.injury_override_sequence_id, next, aliases);
        build_state(assets, profile, animations, clips, role, "Died",
                    next.death_override_sequence_id, next, aliases);

        require(require_state(next, "Injured").type == 2,
                "Source player Injured root is not the authored caller-selected Type2 group");
        require(require_state(next, "Died").type == 0,
                "Source player Died root is not the authored Type0 sequence");
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
}

bool make_runtime_player_injury_selection_v1(
    const RuntimePlayerIncomingAnimationBankV1& bank,
    std::size_t caller_selected_step, OriginalAttackSelection& output,
    std::string& error) {
    try {
        const auto& injury = require_state(bank, "Injured");
        require(injury.type == 2 && injury.loop == 0,
                "Source player injury selection requires the authored finite Type2 root");
        require(caller_selected_step < injury.phases.size(),
                "Caller-selected injury step is outside the authored source choices");
        require(injury.phases[caller_selected_step].sourcePath.size() == 1 &&
                injury.phases[caller_selected_step].sourcePath[0] == caller_selected_step,
                "Injury root is not a flat source choice group");
        OriginalAttackSelection next;
        next.state = "Injured";
        next.variant = 0;
        next.choices.emplace(std::vector<std::size_t>{}, caller_selected_step);
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
}

bool make_runtime_player_death_selection_v1(
    const RuntimePlayerIncomingAnimationBankV1& bank,
    OriginalAttackSelection& output, std::string& error) {
    try {
        const auto& died = require_state(bank, "Died");
        require(died.type == 0 && died.loop == 0,
                "Source player death selection requires the ordinary finite Type0 Died root");
        OriginalAttackSelection next;
        next.state = "Died";
        next.variant = 0;
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
}

} // namespace dh::foundation::combat
