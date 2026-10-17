#include "item_manager_pool_v1.hpp"

#include <algorithm>
#include <cstdio>
#include <new>
#include <utility>

namespace dh2::item_manager_pool_v1 {
namespace {
Category* category(Owner* owner, std::int32_t id) noexcept {
    if (!owner) return nullptr;
    const auto it = std::find_if(owner->categories.begin(), owner->categories.end(),
        [id](const Category& value) { return value.audio_visual_id == id; });
    return it == owner->categories.end() ? nullptr : &*it;
}
const Category* category(const Owner* owner, std::int32_t id) noexcept {
    if (!owner) return nullptr;
    const auto it = std::find_if(owner->categories.begin(), owner->categories.end(),
        [id](const Category& value) { return value.audio_visual_id == id; });
    return it == owner->categories.end() ? nullptr : &*it;
}
Slot* slot_for_item(Owner* owner, std::uintptr_t identity) noexcept {
    if (!owner || !identity) return nullptr;
    for (auto& group : owner->categories)
        for (auto& slot : group.slots)
            if (slot.item_identity == identity) return &slot;
    return nullptr;
}
const Slot* slot_for_item(const Owner* owner, std::uintptr_t identity) noexcept {
    if (!owner || !identity) return nullptr;
    for (const auto& group : owner->categories)
        for (const auto& slot : group.slots)
            if (slot.item_identity == identity) return &slot;
    return nullptr;
}
bool plan_matches(const Owner* owner, const SpawnPlan* plan,
                  const Category*& group, const Slot*& slot) noexcept {
    group = category(owner, plan ? plan->audio_visual_id : -1);
    if (!plan || !group || plan->slot_index >= slots_per_category ||
        plan->expected_next_slot !=
            (std::size_t(plan->slot_index) + 1) % slots_per_category ||
        plan->expected_next_slot != group->next_slot) return false;
    slot = &group->slots[plan->slot_index];
    return (slot->shell_identity == plan->expected_shell_identity ||
            (!plan->expected_shell_identity && slot->shell_identity)) &&
           (slot->item_identity == plan->evicted_item_identity ||
            (!slot->enabled && slot->item_identity == 0));
}
}

Status prepare_spawn(Owner* owner, std::int32_t audio_visual_id,
                     std::uintptr_t item_identity, SpawnPlan* output,
                     std::string& error) {
    if (!owner || audio_visual_id < 0 || !item_identity || !output) {
        error = "ItemManager::Spawn requires AudioVisualID, canonical Item identity and plan";
        return Status::invalid_argument;
    }
    if (slot_for_item(owner, item_identity)) {
        error = "ItemManager::Spawn Item is already active in its pooled ItemObject";
        return Status::identity_conflict;
    }
    auto* group = category(owner, audio_visual_id);
    if (!group || !group->precached) {
        error = "ItemManager::Spawn AudioVisual category has no five-shell PreCache owner";
        return Status::not_precached;
    }
    const auto index = group->next_slot;
    const auto& slot = group->slots[index];
    const auto next = static_cast<std::uint8_t>(
        (std::size_t(index) + 1) % slots_per_category);
    *output = {audio_visual_id, index, next,
               slot.shell_identity, slot.enabled ? slot.item_identity : 0};
    // ItemManager::Spawn advances ObjectInfo::next before DeSpawn or any
    // fallible InitAgain work. Keep this order even if the caller later fails.
    group->next_slot = next;
    error.clear();
    return Status::complete;
}

Status precache_category(
    Owner* owner, std::int32_t audio_visual_id,
    const std::array<std::uintptr_t, slots_per_category>& shell_identities,
    std::string& error) {
    if (!owner || audio_visual_id < 0) {
        error = "ItemManager::PreCache requires a valid AudioVisual row";
        return Status::invalid_argument;
    }
    for (std::size_t i = 0; i < shell_identities.size(); ++i) {
        if (!shell_identities[i]) {
            error = "ItemManager::PreCache requires five stable projection-shell identities";
            return Status::invalid_argument;
        }
        if (std::find(shell_identities.begin(), shell_identities.begin() +
                      static_cast<std::ptrdiff_t>(i), shell_identities[i]) !=
            shell_identities.begin() + static_cast<std::ptrdiff_t>(i)) {
            error = "ItemManager::PreCache shell identities must be unique within a category";
            return Status::identity_conflict;
        }
        for (const auto& existing_category : owner->categories)
            for (const auto& existing_slot : existing_category.slots)
                if (existing_slot.shell_identity == shell_identities[i] &&
                    existing_category.audio_visual_id != audio_visual_id) {
                    error = "ItemManager::PreCache shell identity is already owned by another category";
                    return Status::identity_conflict;
                }
    }
    try {
        auto* group = category(owner, audio_visual_id);
        if (!group) {
            owner->categories.push_back(Category{});
            group = &owner->categories.back();
            group->audio_visual_id = audio_visual_id;
        }
        if (group->precached) {
            for (std::size_t i = 0; i < slots_per_category; ++i)
                if (group->slots[i].shell_identity != shell_identities[i]) {
                    error = "ItemManager::PreCache attempted to replace a stable category shell";
                    return Status::identity_conflict;
                }
            error.clear();
            return Status::complete;
        }
        std::array<std::string, slots_per_category> names{};
        for (std::size_t i = 0; i < slots_per_category; ++i) {
            const auto& slot = group->slots[i];
            if (slot.item_identity || slot.enabled) {
                error = "ItemManager::PreCache cannot replace an active category slot";
                return Status::stale_plan;
            }
            char name[64]{};
            const int written = std::snprintf(name, sizeof(name),
                "ItemObject_%02u_%02u", static_cast<unsigned>(audio_visual_id),
                static_cast<unsigned>(i));
            if (written < 0 || static_cast<std::size_t>(written) >= sizeof(name)) {
                error = "ItemManager::PreCache source shell name overflow";
                return Status::invalid_argument;
            }
            names[i].assign(name, static_cast<std::size_t>(written));
        }
        for (std::size_t i = 0; i < slots_per_category; ++i) {
            auto& slot = group->slots[i];
            slot.shell_identity = shell_identities[i];
            slot.source_object_name = std::move(names[i]);
        }
        group->precached = true;
        error.clear();
        return Status::complete;
    } catch (const std::bad_alloc&) {
        error = "ItemManager::PreCache shell metadata allocation failed";
        return Status::allocation_failed;
    } catch (...) {
        error = "ItemManager::PreCache shell metadata creation failed";
        return Status::allocation_failed;
    }
}

Status bind_shell(Owner* owner, const SpawnPlan* plan,
                  std::uintptr_t shell_identity, std::string& error) {
    if (!owner || !plan || !shell_identity) {
        error = "ItemManager::PreCache requires a stable pooled ItemObject shell";
        return Status::invalid_argument;
    }
    auto* group = category(owner, plan->audio_visual_id);
    if (!group || plan->slot_index >= slots_per_category ||
        plan->expected_next_slot !=
            (std::size_t(plan->slot_index) + 1) % slots_per_category ||
        group->next_slot != plan->expected_next_slot) {
        error = "ItemManager::PreCache shell binding uses a stale slot plan";
        return Status::stale_plan;
    }
    auto& slot = group->slots[plan->slot_index];
    if (slot.shell_identity && slot.shell_identity != shell_identity) {
        error = "ItemManager::PreCache attempted to replace a stable pool shell";
        return Status::identity_conflict;
    }
    if (slot.item_identity != plan->evicted_item_identity &&
        (slot.enabled || slot.item_identity != 0)) {
        error = "ItemManager::PreCache shell binding disagrees with active Item";
        return Status::stale_plan;
    }
    slot.shell_identity = shell_identity;
    error.clear();
    return Status::complete;
}

Status de_spawn(Owner* owner, std::uintptr_t shell_identity,
                std::uintptr_t expected_item_identity, std::string& error) {
    if (!owner || !shell_identity) {
        error = "ItemManager::DeSpawn requires its pooled ItemObject identity";
        return Status::invalid_argument;
    }
    for (auto& group : owner->categories) {
        for (auto& slot : group.slots) {
            if (slot.shell_identity != shell_identity) continue;
            if (slot.item_identity != expected_item_identity) {
                error = "ItemManager::DeSpawn Item identity does not match pool slot";
                return Status::identity_conflict;
            }
            slot.enabled = false;
            slot.item_identity = 0;
            slot.pickup_lock_ms = 0;
            slot.pickup_lock_player_id = -1;
            error.clear();
            return Status::complete;
        }
    }
    error = "ItemManager::DeSpawn pooled ItemObject shell was not precached";
    return Status::invalid_argument;
}

Status bind_receiver(Owner* owner, std::uintptr_t shell_identity,
                     std::uintptr_t receiver_character_identity,
                     std::uintptr_t* effective_receiver_identity,
                     std::string& error) {
    if (!owner || !shell_identity || !effective_receiver_identity) {
        error = "ItemObject::InitAgain requires a pooled shell and receiver output";
        return Status::invalid_argument;
    }
    for (auto& group : owner->categories) {
        for (auto& slot : group.slots) {
            if (slot.shell_identity != shell_identity) continue;
            if (receiver_character_identity)
                slot.receiver_character_identity = receiver_character_identity;
            *effective_receiver_identity = slot.receiver_character_identity;
            error.clear();
            return Status::complete;
        }
    }
    error = "ItemObject::InitAgain shell was not precached by ItemManager";
    return Status::invalid_argument;
}

std::uintptr_t receiver_for_shell(const Owner* owner,
                                  std::uintptr_t shell_identity) noexcept {
    if (!owner || !shell_identity) return 0;
    for (const auto& group : owner->categories)
        for (const auto& slot : group.slots)
            if (slot.shell_identity == shell_identity)
                return slot.receiver_character_identity;
    return 0;
}

Status queue_deferred_interact(Owner* owner, std::uintptr_t shell_identity,
                               std::uintptr_t character_identity,
                               std::string& error) {
    if (!owner || !shell_identity || !character_identity) {
        error = "ItemObject::OnCollisionBegins requires its pooled shell and Character";
        return Status::invalid_argument;
    }
    for (auto& group : owner->categories) {
        for (auto& slot : group.slots) {
            if (slot.shell_identity != shell_identity) continue;
            slot.deferred_interact_character_identity = character_identity;
            error.clear();
            return Status::complete;
        }
    }
    error = "ItemObject::OnCollisionBegins shell was not precached by ItemManager";
    return Status::invalid_argument;
}

std::uintptr_t deferred_interact_for_shell(
    const Owner* owner, std::uintptr_t shell_identity) noexcept {
    if (!owner || !shell_identity) return 0;
    for (const auto& group : owner->categories)
        for (const auto& slot : group.slots)
            if (slot.shell_identity == shell_identity)
                return slot.deferred_interact_character_identity;
    return 0;
}

bool clear_deferred_interact(Owner* owner,
                             std::uintptr_t shell_identity) noexcept {
    if (!owner || !shell_identity) return false;
    for (auto& group : owner->categories) {
        for (auto& slot : group.slots) {
            if (slot.shell_identity != shell_identity) continue;
            slot.deferred_interact_character_identity = 0;
            return true;
        }
    }
    return false;
}

Status activate(Owner* owner, const SpawnPlan* plan,
                std::uintptr_t shell_identity, std::uintptr_t item_identity,
                std::uint32_t pickup_lock_ms, std::string& error) {
    if (!owner || !plan || !shell_identity || !item_identity) {
        error = "ItemManager::Spawn requires its pooled shell and canonical Item";
        return Status::invalid_argument;
    }
    const Category* const_group{};
    const Slot* const_slot{};
    if (!plan_matches(owner, plan, const_group, const_slot)) {
        error = "ItemManager::Spawn pool slot changed before InitAgain";
        return Status::stale_plan;
    }
    auto* group = category(owner, plan->audio_visual_id);
    auto& slot = group->slots[plan->slot_index];
    if (slot.shell_identity && slot.shell_identity != shell_identity) {
        error = "ItemManager::Spawn shell identity differs from pre-cached object";
        return Status::identity_conflict;
    }
    if (!slot.shell_identity) slot.shell_identity = shell_identity;
    slot.item_identity = item_identity;
    slot.pickup_lock_ms = pickup_lock_ms;
    slot.pickup_lock_player_id = -1;
    slot.enabled = true;
    error.clear();
    return Status::complete;
}

bool set_pickup_lock(Owner* owner, std::uintptr_t item_identity,
                     std::uint32_t milliseconds,
                     std::int32_t player_id) noexcept {
    auto* slot = slot_for_item(owner, item_identity);
    if (!slot || !slot->enabled) return false;
    slot->pickup_lock_ms = milliseconds;
    slot->pickup_lock_player_id = player_id;
    return true;
}
void advance_pickup_locks(Owner* owner, std::uint32_t elapsed_ms) noexcept {
    if (!owner) return;
    for (auto& group : owner->categories)
        for (auto& slot : group.slots)
            if (slot.enabled)
                slot.pickup_lock_ms = slot.pickup_lock_ms <= elapsed_ms
                    ? 0 : slot.pickup_lock_ms - elapsed_ms;
}
bool pickup_locked(const Owner* owner, std::uintptr_t item_identity) noexcept {
    const auto* slot = slot_for_item(owner, item_identity);
    return slot && slot->enabled && slot->pickup_lock_ms != 0;
}
bool pickup_locked_for_player(const Owner* owner,std::uintptr_t item_identity,
                              std::int32_t player_id) noexcept {
    const auto* slot=slot_for_item(owner,item_identity);
    return slot&&slot->enabled&&slot->pickup_lock_ms!=0&&
           slot->pickup_lock_player_id==player_id;
}
bool enabled(const Owner* owner, std::uintptr_t item_identity) noexcept {
    const auto* slot = slot_for_item(owner, item_identity);
    return slot && slot->enabled;
}
bool passes_disabled_save_gate(const Owner* owner,
                               std::uintptr_t item_identity) noexcept {
    return enabled(owner, item_identity);
}
const Category* find_category(const Owner* owner,
                              std::int32_t audio_visual_id) noexcept {
    return category(owner, audio_visual_id);
}
const std::string* source_object_name(const Owner* owner,
                                      std::int32_t audio_visual_id,
                                      std::size_t slot_index) noexcept {
    const auto* group = category(owner, audio_visual_id);
    if (!group || !group->precached || slot_index >= slots_per_category)
        return nullptr;
    return &group->slots[slot_index].source_object_name;
}

} // namespace dh2::item_manager_pool_v1
