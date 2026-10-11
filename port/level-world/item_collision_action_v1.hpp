#pragma once

#include <cstdint>
#include <string>

#include "item_manager_pool_v1.hpp"

namespace dh2::item_collision_action_v1 {

enum class Action : std::uint8_t {
    none,
    deferred_interact,
    show_tooltip,
    hide_tooltip_and_clear_character
};

struct BeginRequest {
    bool physical_item_interactive{};
    bool item_has_been_looted{};
    std::uintptr_t character_identity{};
    std::int32_t interaction_type{-1};
    bool character_moving{};
    bool item_is_character_target{};
    bool character_is_local_player{};
};

struct BeginResult {
    Action action{Action::none};
    std::uintptr_t deferred_character_identity{};
};

// Source: POItem::onCollisionBegins (0x4702a8) requires ItemObject::IsInteractive
// (0x3ec014); ItemObject::OnCollisionBegins (0x3ec048) rechecks HasBeenLooted
// and its GetInteractionType (0x3ebeb4). Type -1 schedules Character in
// GameObject +0x2e4 when moving. GameObject::Update (0x38cbe8) passes it to
// virtual ItemObject::Interact (0x3ed144), then clears the field after that
// call, even if the call mutates or re-enters the pool.
item_manager_pool_v1::Status on_collision_begins(
    const BeginRequest&, item_manager_pool_v1::Owner*,
    std::uintptr_t shell_identity, BeginResult*, std::string& error);

Action on_collision_ends(bool character_present,
                         bool item_is_character_target) noexcept;

using DeferredInteract = bool (*)(void* context,
                                  std::uintptr_t character_identity,
                                  std::string& error);

struct UpdateResult {
    bool invoked{};
    bool callback_completed{};
    std::uintptr_t character_identity{};
};

// Mirrors GameObject::Update's load -> virtual callback -> unconditional
// field clear order. Any re-entrant write during Interact is overwritten by
// the source clear that follows the callback.
item_manager_pool_v1::Status update_deferred_interact(
    item_manager_pool_v1::Owner*, std::uintptr_t shell_identity,
    DeferredInteract, void* context, UpdateResult*, std::string& error);

} // namespace dh2::item_collision_action_v1
