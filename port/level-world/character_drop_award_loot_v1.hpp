#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::character_drop_award_loot_v1 {

struct StagedItem {
    std::uintptr_t item_identity{};
    std::int32_t pick_up_type{};
};

struct Services {
    void* context{};
    bool (*scatter)(void*, std::uintptr_t item_identity,
                    std::array<float, 3>& spawn_position,
                    std::array<float, 3>& destination,
                    std::string& error){};
    bool (*spawn)(void*, std::uintptr_t item_identity,
                  const std::array<float, 3>& spawn_position,
                  const std::array<float, 3>& destination,
                  std::uintptr_t& world_item_identity,
                  std::string& error){};
    // Reaches the existing ItemObject::_DoAutoPickupHack/Interact path. It is
    // called synchronously after Spawn, before the next item's scatter draws.
    bool (*auto_pickup)(void*, std::uintptr_t world_item_identity,
                        std::uintptr_t killer_character,
                        std::string& error){};
};

struct Request {
    const StagedItem* items{};
    std::size_t item_count{};
    std::uintptr_t killer_character{};
    std::int32_t automatic_pickup_type{};
};

struct Result {
    std::size_t item_count{};
    std::size_t items_completed{};
    std::size_t current_index{};
    std::uintptr_t last_world_item_identity{};
    std::uint32_t scatter_attempted{}, scatter_completed{};
    std::uint32_t spawn_attempted{}, spawn_completed{};
    std::uint32_t auto_pickup_attempted{}, auto_pickup_completed{};
};

enum class Status : std::uint8_t {
    complete, invalid_argument, busy, consumed, provider_failed
};

// Source ItemObject::DropAndAwardLoot at 0x3ec8a0 processes the temporary
// loot inventory in order: _GetRandomDropPos at 0x3ec668, ItemManager::Spawn
// at 0x3eacd0, then _DoAutoPickupHack at 0x3ec474 for PickUpType.Automatic.
// This owner handles only that ordered tail over already-generated V4 Items.
class Runtime {
    Request request_{};
    Services services_{};
    Result snapshot_{};
    bool busy_{}, consumed_{};
public:
    Runtime(Request, Services);
    Runtime(const Runtime&) = delete;
    Runtime& operator=(const Runtime&) = delete;

    Status run(Result*, std::string& error);
    const Result& snapshot() const noexcept { return snapshot_; }
};

} // namespace dh2::character_drop_award_loot_v1
