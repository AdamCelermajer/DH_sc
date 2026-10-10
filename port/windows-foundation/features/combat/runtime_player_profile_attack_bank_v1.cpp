#include "runtime_player_profile_attack_bank_v1.hpp"

#include "../../content_paths.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <stdexcept>

namespace dh::foundation::combat {
namespace {
using namespace dh2::data;

void require(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

std::int32_t profile_table_id(const ActorProfile& profile) {
    std::size_t consumed = 0;
    const auto value = std::stoi(profile.animation_table, &consumed);
    require(consumed == profile.animation_table.size() && value >= 0,
            "Player source profile has an invalid animation table ID");
    return value;
}

void merge_sequence_policies(const OriginalSequencePolicies& source,
                             OriginalSequencePolicies& destination) {
    for (const auto& item : source) {
        const auto inserted = destination.emplace(item.first, item.second);
        require(inserted.second ||
                (inserted.first->second.type == item.second.type &&
                 inserted.first->second.loop == item.second.loop &&
                 inserted.first->second.name == item.second.name),
                "Player attack bank has conflicting source sequence policies");
    }
}

void merge_bank(OriginalCombatVisualPlan& destination,
                OriginalCombatVisualPlan source) {
    require(destination.profileId == source.profileId &&
            destination.config.model_path == source.config.model_path,
            "Player moving/static attack banks do not share the same source profile/model");
    require(destination.roleId == source.roleId,
            "Player moving/static attack banks have different role namespaces");

    for (const auto& clip : source.config.clips) {
        const auto found = std::find_if(destination.config.clips.begin(),
            destination.config.clips.end(), [&](const auto& existing) {
                return existing.first == clip.first;
            });
        if (found == destination.config.clips.end()) {
            destination.config.clips.push_back(clip);
        } else {
            require(found->second == clip.second,
                    "Player attack bank alias resolves to different profile clips");
        }
    }
    for (const auto& rate : source.clipRates) {
        const auto inserted = destination.clipRates.emplace(rate.first, rate.second);
        require(inserted.second || std::abs(inserted.first->second - rate.second) < 1e-9,
                "Player attack bank alias has conflicting authored rates");
    }
    for (auto& sequence : source.sequences) {
        const auto found = std::find_if(destination.sequences.begin(),
            destination.sequences.end(), [&](const OriginalCombatSequencePlan& existing) {
                return existing.state == sequence.state && existing.variant == sequence.variant;
            });
        require(found == destination.sequences.end(),
                "Player attack bank contains a duplicate state/variant");
        destination.sequences.push_back(std::move(sequence));
    }
    for (auto& state : source.stateNames)
        if (std::find(destination.stateNames.begin(), destination.stateNames.end(), state) ==
            destination.stateNames.end()) destination.stateNames.push_back(std::move(state));
}

} // namespace

bool plan_runtime_player_profile_attack_bank_v1(
    const AssetCatalog& assets,
    const ActorProfile& source_profile,
    const equipment_menu::RuntimePlayerLocomotionV1& equipped,
    std::int32_t source_stanced_list_mask,
    const AnimationTables& animations,
    const Dictionary& clip_dictionary,
    const OriginalCombatVisualPlan& same_profile_visual,
    const std::string& role,
    RuntimePlayerProfileAttackBankPlanV1& output,
    std::string& error) {
    try {
        require(!source_profile.id.empty() && !role.empty(),
                "Player source attack bank requires profile and role IDs");
        const auto animation_table = profile_table_id(source_profile);
        require(equipped.animation_table == animation_table,
                "Resolved player equipment stance belongs to a different source animation table");
        require(equipped.stance >= 0 && equipped.stance < 32,
                "Resolved player stance is outside the source stance domain");
        require(source_stanced_list_mask >= 0,
                "Original stanced-list mask cannot be negative");
        require(same_profile_visual.profileId == source_profile.id,
                "Player source attack bank requires the same profile visual plan");
        const auto expected_model = resolve_content_path(
            assets, normalize_content_uri(source_profile.model_uri))
                .lexically_relative(assets.root()).generic_string();
        require(!same_profile_visual.config.model_path.empty() &&
                std::filesystem::path(same_profile_visual.config.model_path).lexically_normal() ==
                std::filesystem::path(expected_model).lexically_normal(),
                "Player source attack bank visual model differs from its profile model");
        require(animation_table >= 0 &&
                static_cast<std::size_t>(animation_table) < animations.characters.size(),
                "Player source profile animation table is outside the loaded CharAnimTable");

        RuntimePlayerProfileAttackBankPlanV1 next;
        next.source_profile_id = source_profile.id;
        next.animation_table = animation_table;
        next.stance = equipped.stance;

        // Use the same source planner as delivered request planning. The
        // predecessor is explicit for each bank; no input request is created.
        RuntimePlayerComboChainPlanV1 moving, stationary;
        if (!plan_runtime_player_combo_chain_v1(
                assets, equipped, source_stanced_list_mask, 4, true,
                animations, clip_dictionary, source_profile.id,
                same_profile_visual.config, role, moving, error))
            throw std::runtime_error(error);
        if (!plan_runtime_player_combo_chain_v1(
                assets, equipped, source_stanced_list_mask, 3, true,
                animations, clip_dictionary, source_profile.id,
                same_profile_visual.config, role, stationary, error))
            throw std::runtime_error(error);

        require(moving.state == "Attack" && stationary.state == "AttackStatic",
                "Player source attack planner returned an unexpected state bank");
        next.moving_base_sequence_id = moving.base_sequence_id;
        next.moving_sequence_id = moving.selected_sequence_id;
        next.static_base_sequence_id = stationary.base_sequence_id;
        next.static_sequence_id = stationary.selected_sequence_id;
        next.moving_stance_applied = moving.stance_applied;
        next.static_stance_applied = stationary.stance_applied;
        next.moving_selection = moving.selection;
        next.static_selection = stationary.selection;
        next.requires_original_rng_start =
            moving.requires_original_rng_start || stationary.requires_original_rng_start;
        next.bank = std::move(moving.visual);
        merge_bank(next.bank, std::move(stationary.visual));
        merge_sequence_policies(moving.sequence_policies, next.sequence_policies);
        merge_sequence_policies(stationary.sequence_policies, next.sequence_policies);

        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
}

} // namespace dh::foundation::combat
