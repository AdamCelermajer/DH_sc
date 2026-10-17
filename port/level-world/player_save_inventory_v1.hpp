#pragma once

#include "../game-data/fresh_inventory_owned_v4.hpp"
#include "../game-data/item_power_tables_v5.hpp"
#include "../game-data/player_savegame_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::player_save_inventory_v1 {

struct MutableBytes {
    std::uint8_t* data{};
    std::size_t size{};
};

struct ItemRef {
    const data::ItemInstanceV1* item{};
    const data::OwnedItemSlotV4* slot{};
};

// A synchronous, read-only projection for focused byte-format tests. The
// inventory adapter below builds this from the canonical V4 Item graph.
struct GearView {
    std::int32_t gold{};
    std::int32_t selected_set{};
    const ItemRef* items{};
    std::size_t item_count{};
    const std::vector<std::string>* item_names{};
    const std::vector<std::string>* power_names{};
};

struct Bindings {
    const data::PlayerSavegameV1* save{};
    const data::FreshInventoryOwnedV4* inventory{};
    data::ItemPowerTablesV5::Borrow powers;
};

enum class Stage : std::uint32_t {
    not_started,
    gold,
    selection,
    item_count,
    item_name,
    item_slot_first,
    item_slot_second,
    item_quantity,
    item_value,
    item_identified,
    power_count,
    power_name,
    complete,
};

struct Result {
    Stage stage{Stage::not_started};
    std::uint32_t source_caller{};
    std::size_t written{};
    std::uint64_t stream_writes{};
    std::uint32_t declared_items{};
    std::uint32_t completed_items{};
    std::uint32_t declared_powers{};
    std::uint32_t completed_powers{};
};

enum class Status { complete, invalid_argument, failed };

// Writes the GEAR payload into a caller-owned bounded buffer. A reached
// capacity/source failure retains its written prefix and stage. It does not
// write an outer section tag or mutate Save, Inventory, Items or properties.
Status save(const GearView&, MutableBytes, Result*, std::string& error);
Status save(const Bindings&, MutableBytes, Result*, std::string& error);

}  // namespace dh2::player_save_inventory_v1
