#include "character_faery_selection_owner_v1.hpp"

#include <limits>
#include <utility>

namespace dh2::character_faery_selection_owner_v1 {
namespace {
bool restore(PlayerSave& player_save, std::uint32_t difficulty,
            std::int32_t previous, const save::Services& skill_services,
            std::string& error) {
    if (previous < 0 || previous >= 5 ||
        !player_save.set_current_faery(std::uint32_t(previous), difficulty,
                                       error))
        return false;
    if (!skill_services.update_all_skills ||
        !skill_services.update_all_skills(skill_services.context, error))
        return false;
    error.clear();
    return true;
}
} // namespace

Status change(factory::Record& player_record, factory::Record& faery_record,
              PlayerSave& player_save,
              std::uint32_t difficulty, std::uint32_t faery_id,
              const save::Services& skill_services,
              const Services& services, Result* output,
              std::string& error) {
    error.clear();
    if (output) *output = {};
    auto& character_visual = faery_record.faery_visual;
    const auto faery_identity = faery_record.character.identity;
    const auto player_character = player_record.character.identity;
    if (!output || !player_character ||
        player_record.constructor.identity != player_character ||
        !faery_identity || faery_record.constructor.identity != faery_identity ||
        player_record.faery_character_420 != faery_identity ||
        !character_visual.character ||
        character_visual.character != faery_identity ||
        faery_identity == player_character || difficulty >= 3 ||
        !visual::ready(character_visual, faery_identity) ||
        faery_id >= 5 || !player_save.faeries_initialized()[difficulty] ||
        !services.resolve_asset || !services.prepare_placement ||
        !services.commit_placement || !services.rollback_placement ||
        !services.discard_placement || character_visual.generation >
            std::numeric_limits<std::uint64_t>::max() - 2) {
        error = "Faery selection requires one live Character facade, canonical Save, and complete Level providers";
        return Status::invalid_argument;
    }
    output->selected_faery = faery_id;
    output->previous_faery = player_save.current_faery(difficulty);
    if (output->previous_faery < 0 || output->previous_faery >= 5 ||
        character_visual.asset->faery_id !=
            static_cast<std::uint32_t>(output->previous_faery)) {
        error = "Canonical Save current-faery value and live Character visual do not agree";
        return Status::invalid_argument;
    }

    std::shared_ptr<const visual::Asset> candidate;
    try {
        if (services.resolve_asset(services.context, faery_id, &candidate,
                                   error) != 0 || !candidate) {
            if (error.empty()) error = "Faery renderer asset resolution failed";
            return Status::asset_unavailable;
        }
    } catch (...) {
        error = "Faery renderer asset resolver threw";
        return Status::asset_unavailable;
    }
    if (candidate->faery_id != faery_id ||
        candidate->model_dictionary_id < 0 || candidate->idle_sequence < 0 ||
        candidate->model_file.empty() || !candidate->renderer_data) {
        error = "Faery renderer asset has no verified model/clip resource owner";
        return Status::asset_unavailable;
    }
    output->asset_prepared = true;
    std::shared_ptr<visual::CharacterPose> candidate_pose;
    try {
        candidate_pose = candidate->renderer_data->create_character_pose();
        if (!candidate_pose || !candidate_pose->sample(
                candidate->idle_sequence, 0, error)) {
            if (error.empty()) error = "Faery BDAE pose instance preparation failed";
            return Status::asset_unavailable;
        }
    } catch (...) {
        error = "Faery BDAE pose preparation threw";
        return Status::asset_unavailable;
    }

    try {
        if (services.prepare_placement(services.context, player_character,
                character_visual.character, faery_id,
                &output->placement_plan, error) != 0 ||
            !output->placement_plan) {
            if (output->placement_plan)
                services.discard_placement(services.context,
                                            output->placement_plan);
            output->placement_plan = 0;
            if (error.empty()) error = "Faery Level placement preflight failed";
            return Status::placement_unavailable;
        }
    } catch (...) {
        if (output->placement_plan)
            services.discard_placement(services.context,
                                        output->placement_plan);
        output->placement_plan = 0;
        error = "Faery Level placement preflight threw";
        return Status::placement_unavailable;
    }
    output->placement_prepared = true;

    save::Result save_result{};
    const auto save_status = save::change(&player_save, difficulty, faery_id,
                                          skill_services, &save_result, error);
    output->save_changed = save_result.save_changed;
    output->skills_updated = save_result.skills_updated;
    if (save_status != save::Status::complete) {
        if (save_result.save_changed) {
            output->rollback_attempted = true;
            std::string rollback_error;
            output->rollback_complete = restore(player_save, difficulty,
                output->previous_faery, skill_services, rollback_error);
            if (!output->rollback_complete) {
                error = "Faery Save/skill update failed and canonical rollback failed: " + rollback_error;
                services.discard_placement(services.context,
                                            output->placement_plan);
                output->placement_plan = 0;
                return Status::rollback_failed;
            }
        }
        services.discard_placement(services.context, output->placement_plan);
        output->placement_plan = 0;
        return Status::save_or_skill_failed;
    }

    const auto previous_visual = character_visual;
    character_visual.asset = std::move(candidate);
    character_visual.pose = std::move(candidate_pose);
    character_visual.clip_sequence = character_visual.asset->idle_sequence;
    character_visual.clip_time_ms = 0;
    ++character_visual.generation;
    if (!visual::ready(character_visual, previous_visual.character)) {
        character_visual = previous_visual;
        output->rollback_attempted = true;
        std::string rollback_error;
        output->rollback_complete = restore(player_save, difficulty,
            output->previous_faery, skill_services, rollback_error);
        services.discard_placement(services.context, output->placement_plan);
        output->placement_plan = 0;
        if (!output->rollback_complete) {
            error = "Faery visual validation failed and canonical rollback failed: " + rollback_error;
            return Status::rollback_failed;
        }
        error = "Prepared Faery visual facade failed its Character identity check";
        return Status::visual_rejected;
    }
    output->visual_swapped = true;

    int placement_result = 1;
    try {
        placement_result = services.commit_placement(services.context,
            output->placement_plan, error);
    } catch (...) {
        error = "Faery Level placement commit threw";
    }
    if (placement_result != 0) {
        output->rollback_attempted = true;
        std::string rollback_error;
        const bool placement_restored = services.rollback_placement(
            services.context, output->placement_plan, rollback_error) == 0;
        character_visual = previous_visual;
        character_visual.generation = previous_visual.generation + 2;
        std::string save_rollback_error;
        const bool save_restored = restore(player_save, difficulty,
            output->previous_faery, skill_services, save_rollback_error);
        output->rollback_complete = placement_restored && save_restored;
        services.discard_placement(services.context, output->placement_plan);
        output->placement_plan = 0;
        if (!output->rollback_complete) {
            error = "Faery placement failed and rollback was incomplete; placement=" +
                rollback_error + "; Save/skills=" + save_rollback_error;
            return Status::rollback_failed;
        }
        if (error.empty()) error = "Faery Level placement commit failed; Save and visual were restored";
        return Status::placement_failed;
    }

    output->placement_committed = true;
    services.discard_placement(services.context, output->placement_plan);
    output->placement_plan = 0;
    error.clear();
    return Status::complete;
}

} // namespace dh2::character_faery_selection_owner_v1
