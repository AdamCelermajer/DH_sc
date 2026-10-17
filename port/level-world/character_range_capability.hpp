#pragma once
#include "character_combat_queries.hpp"
#include <cstdint>

namespace dh2::character_range_capability {
using Properties=character::CombatProperties896;
using InventoryView=character::CombatInventory16;
using EquipSet=character::CombatEquipSet8;
using Instance=character::CombatItemInstance4;
using Item=character::CombatItemRecord164;
struct Character;
enum class InventoryBinding : std::uint32_t { base_inventory,character_inventory };
struct Inventory {
    std::uintptr_t identity;
    InventoryView* view;
    InventoryBinding binding;
    Character* character_owner;
};
struct Character { std::uintptr_t identity;Properties* properties;Inventory* inventory; };
struct ItemTable { Item* rows;std::uint32_t count; };
struct Services {
    void* context;
    // The actual GetItem leaf captures instance.item_id BEFORE global table
    // resolution. Zero success; return borrowed current ItemTable*. Captured ID
    // remains fixed if this callback changes instance, equipment or global rows.
    // Each original GetItem performs a fresh capture; no cached item result.
    std::int32_t (*capture_items)(void*,const Instance*,std::uint32_t captured_id,const ItemTable**);
};
struct Result {
    std::uint32_t value,calls,item_queries,stores,last_slot,last_item_id;
};
enum class Status : std::int32_t {
    complete,invalid_argument,service_unavailable,service_failed,invalid_source_fact,
    unsupported_virtual,
};
Status character_parameters(Character*,std::uint32_t* minimum,std::uint32_t* maximum,
    std::uint32_t* projectile,const Services*,Result*);
Status inventory_parameters(Inventory*,std::uint32_t* minimum,std::uint32_t* maximum,
    std::uint32_t* projectile,const Services*,Result*);
Status character_can_range(Character*,const Services*,Result*);
Status has_ranged_weapon(Inventory*,const Services*,Result*);

// Character overload stores property30 ASR8, property31 ASR8, then fresh
// property32. Inventory overload dispatches genuine virtual8, rereads current
// equip-set AFTER its predicate, gets a fresh row and stores words38/39/40.
// Outputs may be exactly aliased to each other and writable scalar property/item
// words. They must not alias wrapper/binding/view/ref/instance pointer metadata,
// Services or Result. All source reads/stores remain sequential; no memcpy batch.
// Source current-set getter uses signed byte2e; view.current_set must be in
// [-128,127]. Source immutable identities/bindings and metadata mappings are
// external. Base inventory/Character embedded inventory virtual8 routes are
// reconstructed; other derived vtables report unsupported_virtual.
// One owning thread retains all wrappers, props/inventory/ref/instances, rows,
// retired backing, services/context and outputs through return. Table captures
// may synchronously change live scalar/equipment/table bindings. Reentry into
// this module for the same wrappers, metadata overwrite or borrowed destruction
// is forbidden; independent owners and other source modules may nest. Failures
// preserve prior source stores/effects; no rollback or added cleanup. Inventory
// construction, equipment mutation policy and property resolution are external.
} // namespace dh2::character_range_capability
