#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::item_manager_pool_v1 {

constexpr std::size_t slots_per_category = 5;

struct Slot {
    std::uintptr_t shell_identity{};
    std::uintptr_t item_identity{};
    // ItemObject::InitAgain only overwrites +0x3bc when its Character
    // argument is nonnull. ItemManager::DeSpawn does not clear this field.
    std::uintptr_t receiver_character_identity{};
    // GameObject::Update consumes ItemObject +0x2e4 as the Character argument
    // to virtual ItemObject::Interact, then clears it. DeSpawn leaves it alone.
    std::uintptr_t deferred_interact_character_identity{};
    std::uint32_t pickup_lock_ms{};
    // ItemObject+0x3c0 is compared to PlayerInfo+0x678 by Interact.
    std::int32_t pickup_lock_player_id{-1};
    std::string source_object_name;
    bool enabled{};
};

struct Category {
    std::int32_t audio_visual_id{-1};
    std::array<Slot, slots_per_category> slots{};
    std::uint8_t next_slot{};
    bool precached{};
};

struct Owner {
    std::vector<Category> categories;
};

struct SpawnPlan {
    std::int32_t audio_visual_id{-1};
    std::uint8_t slot_index{};
    std::uint8_t expected_next_slot{};
    std::uintptr_t expected_shell_identity{};
    std::uintptr_t evicted_item_identity{};
};

enum class Status : std::uint8_t {
    complete, invalid_argument, not_precached, identity_conflict, stale_plan,
    allocation_failed
};

// ItemManager::PreCache makes five ItemObject shells per AudioVisualID.
// Spawn uses the Item row's +0x54 AudioVisualID and rotates those five slots.
// These identities are port-owned projection shells keyed by exact source
// PreCache names; they are not original ObjectBase instances or storage.
Status precache_category(Owner*, std::int32_t audio_visual_id,
                         const std::array<std::uintptr_t, slots_per_category>&,
                         std::string& error);
// Spawn reserves/advances the source round-robin cursor before DeSpawn,
// SetPosition/SetDestination and InitAgain. This returns that reserved slot;
// the V4 owner can then retire its prior Item before binding the new one.
Status prepare_spawn(Owner*, std::int32_t audio_visual_id,
                     std::uintptr_t item_identity, SpawnPlan*,
                     std::string& error);
Status bind_shell(Owner*, const SpawnPlan*, std::uintptr_t shell_identity,
                  std::string& error);
Status de_spawn(Owner*, std::uintptr_t shell_identity,
                std::uintptr_t expected_item_identity,
                std::string& error);
// Preserve the pooled ItemObject receiver across DeSpawn/reuse when InitAgain
// receives nullptr. A nonnull Character replaces the retained receiver.
Status bind_receiver(Owner*, std::uintptr_t shell_identity,
                     std::uintptr_t receiver_character_identity,
                     std::uintptr_t* effective_receiver_identity,
                     std::string& error);
std::uintptr_t receiver_for_shell(const Owner*,
                                  std::uintptr_t shell_identity) noexcept;
// Mirrors the independent GameObject +0x2e4 deferred-action field. DeSpawn
// and shell reuse preserve it; the source Update call consumes and clears it.
Status queue_deferred_interact(Owner*, std::uintptr_t shell_identity,
                               std::uintptr_t character_identity,
                               std::string& error);
std::uintptr_t deferred_interact_for_shell(const Owner*,
                                           std::uintptr_t shell_identity) noexcept;
bool clear_deferred_interact(Owner*, std::uintptr_t shell_identity) noexcept;
Status activate(Owner*, const SpawnPlan*, std::uintptr_t shell_identity,
                std::uintptr_t item_identity, std::uint32_t pickup_lock_ms,
                std::string& error);

bool set_pickup_lock(Owner*, std::uintptr_t item_identity,
                     std::uint32_t milliseconds,
                     std::int32_t player_id=-1) noexcept;
void advance_pickup_locks(Owner*, std::uint32_t elapsed_ms) noexcept;
bool pickup_locked(const Owner*, std::uintptr_t item_identity) noexcept;
bool pickup_locked_for_player(const Owner*,std::uintptr_t item_identity,
                              std::int32_t player_id) noexcept;
bool enabled(const Owner*, std::uintptr_t item_identity) noexcept;
// This is specifically the source GameObject::Disabled gate in
// ObjectManager::IsObjectSerializable, not the full ObjectManager predicate.
bool passes_disabled_save_gate(const Owner*, std::uintptr_t item_identity) noexcept;
const Category* find_category(const Owner*, std::int32_t audio_visual_id) noexcept;
const std::string* source_object_name(const Owner*, std::int32_t audio_visual_id,
                                      std::size_t slot_index) noexcept;

} // namespace dh2::item_manager_pool_v1
