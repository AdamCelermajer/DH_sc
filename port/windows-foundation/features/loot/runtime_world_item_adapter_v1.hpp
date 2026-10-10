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
    std::array<float, 3> source_position{};
    std::string inventory_instance_id;
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
