#include "game_object_visual_attachment_v1.hpp"

#include <limits>

namespace dh2::game_object_visual_attachment_v1 {
namespace {
struct Range { std::uintptr_t begin{}, end{}; };

template<class T> bool range(const T* value, Range& out) noexcept {
    const auto address = reinterpret_cast<std::uintptr_t>(value);
    if (!value || address % alignof(T) ||
        address > std::numeric_limits<std::uintptr_t>::max() - sizeof(T))
        return false;
    out = {address, address + sizeof(T)};
    return true;
}

bool overlaps(Range left, Range right) noexcept {
    return left.begin < right.end && right.begin < left.end;
}

bool destroy(const Services& services, std::uintptr_t identity,
             Result& result, std::string& error) {
    if (!identity) return true;
    if (!services.destroy_visual) {
        error = "VisualObject replacement reached a missing vtable+4 destructor provider";
        return false;
    }
    ++result.destructor_calls;
    try {
        if (services.destroy_visual(services.context, identity) == 0) return true;
    } catch (...) {
        error = "VisualObject destructor provider threw";
        return false;
    }
    if (error.empty()) error = "VisualObject destructor provider failed";
    return false;
}
} // namespace

Status set_visual_object(object_manager_runtime_owner_v1::GameObject* object,
                         Candidate candidate, const Services* services,
                         Result* output, std::string& error) {
    error.clear();
    Range object_range{}, visual_range{};
    if (!object || !object->identity || !services || !range(output, visual_range) ||
        !range(object, object_range) || overlaps(object_range, visual_range)) {
        error = "VisualObject attachment requires a live GameObject and separate result";
        return Status::invalid_argument;
    }
    *output = {};
    // ObjectManager's canonical projection may borrow the renderer-owned
    // source GameObject+0x2d8 field. Mutate that field when bound; otherwise
    // retain the projection's own field as the host-owned fallback.
    auto* visual_field = object->live_fields.visual_object_2d8
        ? object->live_fields.visual_object_2d8
        : &object->visual_object_2d8;
    Range visual_field_range{};
    if (!range(visual_field, visual_field_range) ||
        overlaps(visual_field_range, visual_range)) {
        error = "GameObject visual pointer field is invalid or aliases the result";
        return Status::invalid_argument;
    }
    output->previous_visual = *visual_field;
    output->current_visual = *visual_field;

    if (candidate.visual) {
        Range candidate_range{};
        if (!range(candidate.visual, candidate_range) ||
            overlaps(candidate_range, object_range) ||
            overlaps(candidate_range, visual_range) ||
            !candidate.visual->identity ||
            candidate.visual->owner_identity != object->identity) {
            error = "VisualObject candidate is invalid or belongs to another GameObject";
            return Status::invalid_visual_owner;
        }
        if (candidate.root_scene_node) {
            Range root_range{};
            if (!range(candidate.root_scene_node, root_range) ||
                overlaps(root_range, object_range) ||
                overlaps(root_range, candidate_range) ||
                overlaps(root_range, visual_range) ||
                !candidate.root_scene_node->identity ||
                (candidate.root_scene_node->owner_game_object_204 &&
                 candidate.root_scene_node->owner_game_object_204 !=
                    object->identity)) {
                error = "VisualObject root SceneNode is invalid or linked to another GameObject";
                return Status::invalid_visual_owner;
            }
            auto* root_owner = candidate.root_scene_node->owner_game_object_204_live
                ? candidate.root_scene_node->owner_game_object_204_live
                : &candidate.root_scene_node->owner_game_object_204;
            Range root_owner_range{};
            if (!range(root_owner, root_owner_range) ||
                overlaps(root_owner_range, visual_range) ||
                (*root_owner && *root_owner != object->identity)) {
                error = "VisualObject root SceneNode owner field is invalid or linked to another GameObject";
                return Status::invalid_visual_owner;
            }
        }
    }

    const auto next = candidate.visual ? candidate.visual->identity : 0;
    const bool replacing = next != output->previous_visual;
    if (candidate.visual && !candidate.root_scene_node) {
        if (!replacing) {
            error = "Attached VisualObject is missing its source root SceneNode";
            return Status::invalid_visual_owner;
        }
        output->rejected_missing_scene_node = true;
        if (!destroy(*services, next, *output, error))
            return services->destroy_visual
                ? Status::service_failed : Status::service_unavailable;
        error = "New VisualObject constructor produced no source SceneNode; previous attachment retained";
        return Status::missing_scene_node;
    }

    if (replacing && output->previous_visual) {
        if (!destroy(*services, output->previous_visual, *output, error))
            return services->destroy_visual
                ? Status::service_failed : Status::service_unavailable;
        *visual_field = 0;
        output->current_visual = 0;
        ++output->game_object_stores;
    }
    if (replacing) {
        *visual_field = next;
        output->current_visual = next;
        ++output->game_object_stores;
    }
    if (candidate.visual) {
        auto* root_owner = candidate.root_scene_node->owner_game_object_204_live
            ? candidate.root_scene_node->owner_game_object_204_live
            : &candidate.root_scene_node->owner_game_object_204;
        *root_owner = object->identity;
        ++output->root_owner_stores;
    }
    return Status::complete;
}

} // namespace dh2::game_object_visual_attachment_v1
