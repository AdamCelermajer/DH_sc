#pragma once

#include "runtime_death_rewards_v1.hpp"
#include "../inventory/inventory_feature.hpp"
#include "../../actor_definitions.hpp"
#include "../../world.hpp"
#include <array>
#include <map>
#include <optional>

namespace dh::foundation::loot {

using RuntimeWorldItemIdV1 = std::uint64_t;
inline constexpr RuntimeWorldItemIdV1 invalid_runtime_world_item_v1 = 0;

// One item owned by the current gameplay instance. Original Loot outcome and
// ItemTable row identities remain borrowed from the retained source snapshot.
struct RuntimeWorldItemEntryV1 {
    RuntimeWorldItemIdV1 identity{invalid_runtime_world_item_v1};
    RuntimeWorldItemRecordV1 source_outcome;
    const dh2::data::Item* authored_item{};
    std::uint32_t quantity{};
    // CURRENT world position (starts at the dropper position and travels to
    // `destination`; the name is kept for source compatibility with existing
    // consumers/tests).
    std::array<float, 3> source_position{};
    std::string inventory_instance_id;

    // P14 DROPS additions (all appended; see world_drop_rules_v1.hpp).
    std::array<float, 3> destination{};     // ItemObject landing point (_GetRandomDropPos)
    std::int32_t visual_row{-1};            // ItemTable AudioVisualID = ItemManager pool category
    std::uint64_t spawn_sequence{};         // ItemManager round-robin order inside a category
    std::uint32_t age_ms{};
    ActorId owner_actor{invalid_actor_id};  // ItemObject+480 owner player (drop-protection window)
    std::int32_t owner_protect_ms{};        // ItemObject+476 timer, >0 rejects the owner's Interact
    bool from_inventory{};                  // published by drop_item_to_world (no Loot row)
};

// Narrow presentation data for root renderers. An icon is exposed only when
// the exact original ItemTable IconName is nonempty. The row's word17 is the
// actual source text OID, not a guessed display string.
struct RuntimeWorldItemRenderV1 {
    RuntimeWorldItemIdV1 identity{invalid_runtime_world_item_v1};
    std::int32_t item_id{-1};
    std::string item_identifier;
    std::string exact_icon_name;
    std::int32_t source_name_text_oid{};
    std::uint32_t quantity{};
    std::array<float, 3> position{};
    bool has_exact_icon{};
};

struct RuntimeWorldItemPickupReceiptV1 {
    RuntimeWorldItemIdV1 world_item{invalid_runtime_world_item_v1};
    std::string inventory_instance_id;
    std::string retained_instance_id;
    std::uint32_t quantity{};
    bool completed{};
};

// Same-session generic WorldItemStore and CharacterState pickup adapter. It
// owns one authoritative map for world items, retains the actual LootTables
// snapshot that backs every row, and uses no RNG or detached inventory owner.
class RuntimeWorldItemAdapterV1 {
    dh2::data::LootTablesV2::Borrow tables_;
    RuntimeWorldItemIdV1 next_identity_{1};
    std::map<RuntimeWorldItemIdV1, RuntimeWorldItemEntryV1> items_;
    bool running_{};
    std::uint64_t next_sequence_{1};
    std::uint64_t pool_evictions_{};

public:
    explicit RuntimeWorldItemAdapterV1(dh2::data::LootTablesV2::Borrow tables)
        : tables_(std::move(tables)) {}

    bool publish_death_drop(const RuntimeWorldItemRecordV1&,
                            const dh::foundation::ActorState& victim,
                            RuntimeWorldItemIdV1& published_item,
                            std::string& error);

    // Publishes an authored neutral-object drop into this same store. The
    // source position is borrowed from the actual WorldObject transform; no
    // CharacterState or fabricated combat properties are introduced.
    bool publish_source_object_drop(const RuntimeWorldItemRecordV1&,
                                    const dh::foundation::ActorDefinition&,
                                    const dh::foundation::WorldObject&,
                                    RuntimeWorldItemIdV1& published_item,
                                    std::string& error);

    // ItemObject::DropAndAwardLoot placement: the landing point comes from
    // _GetRandomDropPos over the SAME loot RNG (killer position optional).
    // The plain publish_death_drop() keeps the item at the victim position.
    bool publish_death_drop_scattered(const RuntimeWorldItemRecordV1&,
                                      const dh::foundation::ActorState& victim,
                                      const std::array<float, 3>* killer_position,
                                      dh2::data::LootRandom8V2* rng,
                                      RuntimeWorldItemIdV1& published_item,
                                      std::string& error);

    // Publishes an item that already left a CharacterState (ItemObject::
    // DropInventory path). No Loot row exists: loot_table stays -1.
    bool publish_inventory_drop(const dh::foundation::InventoryItem&,
                                const std::array<float, 3>& position,
                                dh::foundation::ActorId owner_actor,
                                std::int32_t owner_protect_ms,
                                RuntimeWorldItemIdV1& published_item,
                                std::string& error);

    // Moves every item toward its destination (source ItemObject speed word
    // 6.0) and ages the owner-protection timers. dt in whole milliseconds.
    void advance(std::uint32_t dt_ms) noexcept;
    // Drops every world item (session reload). Never grants rewards.
    void clear() noexcept { items_.clear(); }
    const std::map<RuntimeWorldItemIdV1, RuntimeWorldItemEntryV1>& entries() const noexcept { return items_; }
    // Items removed by the 5-slot-per-category ItemManager pool recycling.
    std::uint64_t pool_evictions() const noexcept { return pool_evictions_; }
    const dh2::data::LootTablesV2::Borrow& tables() const noexcept { return tables_; }

    // Function-pointer-compatible bridge for RuntimeDeathRewardServicesV1.
    static bool spawn_world_item_thunk(
        void*, const RuntimeWorldItemRecordV1&,
        const dh::foundation::ActorState& victim,
        const dh::foundation::ActorState* killer, std::string& error);

private:
    bool publish_at_position(const RuntimeWorldItemRecordV1&,
                             const std::array<float,3>&,
                             RuntimeWorldItemIdV1& published_item,
                             std::string& error);

public:

    bool inspect(RuntimeWorldItemIdV1,
                 RuntimeWorldItemEntryV1& output,
                 std::string& error) const;
    bool render_items(std::vector<RuntimeWorldItemRenderV1>& output,
                      std::string& error) const;
    bool pickup(RuntimeWorldItemIdV1,
                dh::foundation::CharacterState& character,
                RuntimeWorldItemPickupReceiptV1& receipt,
                std::string& error,
                std::optional<std::int32_t> source_gold_limit = std::nullopt);

    std::size_t size() const noexcept { return items_.size(); }
    bool uses_loot_snapshot(const dh2::data::LootTablesV2::Borrow& source) const noexcept {
        return tables_ && source && &tables_.items() == &source.items() &&
               &tables_.loots() == &source.loots() &&
               &tables_.loot_names() == &source.loot_names();
    }
};

} // namespace dh::foundation::loot
