#include "runtime_player_combo_chain_v1.hpp"

#include "../../content_paths.hpp"

#include <set>
#include <stdexcept>

namespace dh::foundation::combat {
namespace {
using namespace dh2::data;

void require(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

void add_policy(const AnimationTables& animations, std::int32_t id,
                OriginalSequencePolicies& policies) {
    require(id >= 0 && static_cast<std::size_t>(id) < animations.sequences.size() &&
            static_cast<std::size_t>(id) < animations.sequence_names.size(),
            "Selected player attack redirect is outside the original AnimTable");
    const auto& source = animations.sequences[static_cast<std::size_t>(id)];
    OriginalSequencePolicy policy{id, source.type, source.loop,
                                  animations.sequence_names[static_cast<std::size_t>(id)]};
    const auto inserted = policies.emplace(id, policy);
    require(inserted.second ||
            (inserted.first->second.type == policy.type &&
             inserted.first->second.loop == policy.loop &&
             inserted.first->second.name == policy.name),
            "Repeated player attack redirect has inconsistent source policy");
}

void build_sequence(const AssetCatalog& assets, const AnimationTables& animations,
                    const Dictionary& clips, const CharacterVisualConfig& same_visual,
                    const std::string& source_profile_id, const std::string& role,
                    const std::string& state,
                    std::int32_t root_id, RuntimePlayerComboChainPlanV1& output) {
    const auto& root = animations.sequences.at(static_cast<std::size_t>(root_id));
    const auto& root_name = animations.sequence_names.at(static_cast<std::size_t>(root_id));
    OriginalCombatSequencePlan sequence;
    sequence.state = state;
    sequence.name = root_name;
    sequence.variant = 0;
    sequence.id = root_id;
    sequence.type = root.type;
    sequence.loop = root.loop;
    output.visual.profileId = source_profile_id;
    output.visual.roleId = role;
    output.visual.config = same_visual;
    output.visual.stateNames.push_back(state);
    add_policy(animations, root_id, output.sequence_policies);

    std::set<std::int32_t> active;
    std::set<std::string> aliases;
    for (const auto& clip : same_visual.clips)
        require(!clip.first.empty() && aliases.insert(clip.first).second,
                "Same player visual has an empty or duplicate clip alias");
    std::size_t phase_number = 0;
    std::function<void(std::int32_t, std::vector<std::size_t>,
                       std::vector<std::int64_t>,
                       std::vector<OriginalCombatRedirect>)> visit;
    visit = [&](std::int32_t id, std::vector<std::size_t> prefix,
                std::vector<std::int64_t> indices,
                std::vector<OriginalCombatRedirect> ancestors) {
        require(prefix.size() <= 2 && active.insert(id).second,
                "Player attack redirect exceeds source depth or contains a cycle");
        add_policy(animations, id, output.sequence_policies);
        const auto& current = animations.sequences.at(static_cast<std::size_t>(id));
        require(!current.steps.empty(), "Selected player attack sequence has no authored steps");
        for (std::size_t i = 0; i < current.steps.size(); ++i) {
            const auto& step = current.steps[i];
            auto path = prefix;
            path.push_back(i);
            auto source_indices = indices;
            source_indices.push_back(static_cast<std::int64_t>(i));
            if (step.redir == 1) {
                require(prefix.size() < 2,
                        "Player attack redirect exceeds original three sequence layers");
                OriginalCombatRedirect redirect;
                redirect.index = static_cast<std::int64_t>(i);
                redirect.animationId = step.anim;
                redirect.redirect = step.redir;
                redirect.speed = step.speed;
                redirect.blendOut = step.blend_out;
                redirect.moveGO = step.move_go;
                auto child_ancestors = ancestors;
                child_ancestors.push_back(std::move(redirect));
                visit(step.anim, std::move(path), std::move(source_indices),
                      std::move(child_ancestors));
                continue;
            }
            require(step.redir == 0,
                    "Selected player attack has an unsupported original redirect mode");
            const auto* uri = animation_clip(step, clips);
            require(uri && !uri->empty(),
                    "Selected player attack leaf has no authored visual clip");
            OriginalCombatPhase phase;
            phase.sourcePath = path;
            phase.sourceIndices = std::move(source_indices);
            phase.ancestors = ancestors;
            phase.animationId = step.anim;
            phase.redirect = step.redir;
            phase.blendOut = step.blend_out;
            phase.moveGO = step.move_go;
            phase.speed = step.speed;
            phase.sourceUri = *uri;
            phase.resolvedPath = resolve_content_path(
                assets, normalize_content_uri(*uri), same_visual.model_path)
                    .lexically_relative(assets.root()).generic_string();
            phase.clipName = role + "/" + state + "/sequence-" +
                             std::to_string(root_id) + "/phase-" +
                             std::to_string(phase_number++);
            require(aliases.insert(phase.clipName).second,
                    "Generated player attack clip alias collides with the source visual");
            output.visual.config.clips.emplace_back(phase.clipName, phase.resolvedPath);
            output.visual.clipRates.emplace(phase.clipName, phase.speed);
            sequence.phases.push_back(std::move(phase));
            require(sequence.phases.size() <= 4096,
                    "Selected player attack exceeds the authored phase bound");
        }
        active.erase(id);
    };
    visit(root_id, {}, {}, {});
    require(!sequence.phases.empty(), "Selected player attack has no visual phases");
    output.visual.sequences.push_back(std::move(sequence));
}
}

bool plan_runtime_player_combo_chain_v1(
    const AssetCatalog& assets,
    const equipment_menu::RuntimePlayerLocomotionV1& equipped,
    std::int32_t stanced_list_mask,
    std::int32_t predecessor_state,
    bool delivered,
    const dh2::data::AnimationTables& animations,
    const dh2::data::Dictionary& clips,
    const std::string& source_profile_id,
    const CharacterVisualConfig& same_visual,
    const std::string& role,
    RuntimePlayerComboChainPlanV1& output,
    std::string& error) {
    try {
        require(!source_profile_id.empty() && !role.empty(),
                "Player combo plan requires source profile and feature role IDs");
        require(equipped.animation_table >= 0 &&
                static_cast<std::size_t>(equipped.animation_table) < animations.characters.size(),
                "Resolved player animation table is outside the original CharAnimTable");
        require(equipped.stance >= 0 && equipped.stance < 32,
                "Resolved player stance is outside the source stance domain");
        require(stanced_list_mask >= 0,
                "Original stanced-list mask cannot be negative");

        RuntimePlayerComboChainPlanV1 next;
        next.command_delivered = delivered;
        next.animation_table = equipped.animation_table;
        next.stance = equipped.stance;
        next.predecessor_state = predecessor_state;
        next.state = predecessor_state == 4 ? "Attack" : "AttackStatic";

        const auto* base = dh2::data::animation_state(
            animations, equipped.animation_table, next.state);
        require(base != nullptr,
                "Original player CharAnimTable has no selected attack state");
        const auto base_offset = base - animations.sequences.data();
        require(base_offset >= 0 &&
                static_cast<std::size_t>(base_offset) < animations.sequences.size(),
                "Selected player attack base escaped the loaded AnimTable");
        next.base_sequence_id = static_cast<std::int32_t>(base_offset);

        // Recovered CSAttack::OnFocus tests SL__LIST_IPHONE bit0x40 for the
        // moving Attack field and bit0x80 for AttackStatic, then adds the
        // actual GetAnimStance result to that source table sequence ID.
        const std::int32_t stance_bit = next.state == "Attack" ? 0x40 : 0x80;
        next.stance_applied =
            (static_cast<std::uint32_t>(stanced_list_mask) &
             static_cast<std::uint32_t>(stance_bit)) != 0;
        const std::int64_t selected = std::int64_t(next.base_sequence_id) +
                                      (next.stance_applied ? equipped.stance : 0);
        require(selected >= 0 &&
                static_cast<std::size_t>(selected) < animations.sequences.size() &&
                static_cast<std::size_t>(selected) < animations.sequence_names.size(),
                "Selected player attack stance sequence is outside the original AnimTable");
        next.selected_sequence_id = static_cast<std::int32_t>(selected);

        // The sequence selection used by OriginalAttackSequence names this
        // request's root as a one-entry state bank. Stance already selected
        // the exact source root ID above; it is not a visual variant index.
        next.selection.state = next.state;
        next.selection.variant = 0;
        next.selection.actor_rate = 1.0;
        build_sequence(assets, animations, clips, same_visual, source_profile_id,
                       role, next.state,
                       next.selected_sequence_id, next);
        for (const auto& policy : next.sequence_policies)
            next.requires_original_rng_start = next.requires_original_rng_start ||
                                               policy.second.type == 2;
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
}

} // namespace dh::foundation::combat
