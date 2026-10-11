#include "item_collision_action_v1.hpp"

#include <utility>

namespace dh2::item_collision_action_v1 {

item_manager_pool_v1::Status on_collision_begins(
    const BeginRequest& request, item_manager_pool_v1::Owner* pool,
    std::uintptr_t shell_identity, BeginResult* result, std::string& error) {
    if (!result) {
        error = "ItemObject::OnCollisionBegins requires an action result";
        return item_manager_pool_v1::Status::invalid_argument;
    }
    *result = {};
    if (!request.physical_item_interactive ||
        request.item_has_been_looted || !request.character_identity) {
        error.clear();
        return item_manager_pool_v1::Status::complete;
    }

    if (request.interaction_type == -1) {
        if (!request.character_moving) {
            error.clear();
            return item_manager_pool_v1::Status::complete;
        }
        const auto status = item_manager_pool_v1::queue_deferred_interact(
            pool, shell_identity, request.character_identity, error);
        if (status == item_manager_pool_v1::Status::complete) {
            result->action = Action::deferred_interact;
            result->deferred_character_identity = request.character_identity;
        }
        return status;
    }

    if (request.item_is_character_target && request.character_is_local_player)
        result->action = Action::show_tooltip;
    error.clear();
    return item_manager_pool_v1::Status::complete;
}

Action on_collision_ends(bool character_present,
                         bool item_is_character_target) noexcept {
    return character_present && !item_is_character_target
        ? Action::hide_tooltip_and_clear_character : Action::none;
}

item_manager_pool_v1::Status update_deferred_interact(
    item_manager_pool_v1::Owner* pool, std::uintptr_t shell_identity,
    DeferredInteract interact, void* context, UpdateResult* result,
    std::string& error) {
    if (!pool || !shell_identity || !result || !interact) {
        error = "GameObject::Update requires its pooled shell, result, and Interact service";
        return item_manager_pool_v1::Status::invalid_argument;
    }
    *result = {};
    const auto character_identity =
        item_manager_pool_v1::deferred_interact_for_shell(pool, shell_identity);
    if (!character_identity) {
        error.clear();
        return item_manager_pool_v1::Status::complete;
    }

    result->invoked = true;
    result->character_identity = character_identity;
    std::string callback_error;
    try {
        result->callback_completed =
            interact(context, character_identity, callback_error);
    } catch (...) {
        callback_error = "ItemObject::Interact service threw during deferred update";
    }

    // Source GameObject::Update clears +0x2e4 after the virtual call, not
    // before it. This deliberately removes any value re-queued by a callback.
    if (!item_manager_pool_v1::clear_deferred_interact(pool, shell_identity)) {
        error = "GameObject::Update lost its pooled shell before the source field clear";
        return item_manager_pool_v1::Status::invalid_argument;
    }
    error = std::move(callback_error);
    return item_manager_pool_v1::Status::complete;
}

} // namespace dh2::item_collision_action_v1
