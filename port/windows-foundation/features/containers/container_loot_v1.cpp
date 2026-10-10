#include "container_loot_v1.hpp"

#include "../../../game-data/loot_item_selection_v8.hpp"
#include "../../../game-data/loot_power_creation_v7.hpp"

#include <cmath>
#include <cstring>
#include <limits>

namespace dh::foundation::containers {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
} // namespace

bool ContainerLootV1::bind(const ContainerLootInputsV1& inputs, std::string& error) {
    error.clear();
    bound_ = false;
    loot_.clear();
    gold_unpriced_ = 0;
    if (!inputs.tables || !inputs.powers || !inputs.store || !inputs.entry.invoke ||
        !inputs.with_rng)
        return fail(error, "Container loot requires original tables/powers, the world-item store, the entry services and a session RNG loan");
    if (!inputs.store->uses_loot_snapshot(inputs.tables))
        return fail(error, "Container loot store does not retain the supplied source LootTables snapshot");
    inputs_ = inputs;
    bound_ = true;
    return true;
}

bool ContainerLootV1::open(const ActorDefinition& definition, const WorldObject& object,
                           ActorId opener, std::uint64_t lifecycle, std::int32_t table,
                           ContainerLootOutcomeV1& outcome, std::string& error) {
    outcome = {};
    error.clear();
    if (!bound_) return fail(error, "Container loot is not bound to the current session");
    const std::size_t gold_before = gold_unpriced_;
    interactions::SourceContainerLootServicesV1 services;
    services.tables = inputs_.tables;
    services.powers = inputs_.powers;
    services.entry = inputs_.entry;
    services.context = this;
    services.with_gameplay_rng = &ContainerLootV1::with_rng;
    services.drop_item_with_rng = &ContainerLootV1::drop_item;
    const bool ok = loot_.drop(definition.stableId, lifecycle, definition, object, opener, table,
                               -1, false, services, outcome.receipt, error);
    outcome.gold_unpriced = gold_unpriced_ - gold_before;
    return ok;
}

bool ContainerLootV1::with_rng(void* raw, const interactions::SourceContainerLootRngOperationV1& operation,
                               std::string& error) {
    auto* self = static_cast<ContainerLootV1*>(raw);
    if (!self || !self->bound_ || !self->inputs_.with_rng)
        return fail(error, "Container loot lost its session RNG loan");
    return self->inputs_.with_rng(self->inputs_.rng_context, operation, error);
}

bool ContainerLootV1::drop_item(void* raw, const interactions::SourceContainerDropItemV1& item,
                                dh2::data::LootRandom8V2& random, std::string& error) {
    auto* self = static_cast<ContainerLootV1*>(raw);
    if (!self || !self->bound_) return fail(error, "Container loot lost its bound store");
    if (item.source_state || !item.source_object || !item.source_definition ||
        item.source_actor != item.source_definition->stableId ||
        item.source_object->id != item.source_actor ||
        item.source_object->name != item.source_definition->name ||
        item.source_position != item.source_object->transform.position)
        return fail(error, "Container loot requires the exact neutral source definition/object/transform");
    if (item.opener_actor == invalid_actor_id)
        return fail(error, "Container loot opener is not a current actor");
    if (!item.selected.item || !item.selected.entry || !item.selected.quantity ||
        item.selected.id < 0 ||
        static_cast<std::size_t>(item.selected.id) >= self->inputs_.tables.items().rows.size() ||
        &self->inputs_.tables.items().rows[static_cast<std::size_t>(item.selected.id)] !=
            item.selected.item)
        return fail(error, "Container loot lacks the exact borrowed ItemTable row/entry/quantity");

    loot::RuntimeWorldItemRecordV1 record;
    record.source_actor = item.source_actor;
    record.killer_actor = item.opener_actor;
    record.loot_table = item.loot_table;
    record.item_id = static_cast<std::int16_t>(item.selected.id);
    record.quantity = static_cast<std::uint8_t>(item.selected.quantity);
    record.authored_item = item.selected.item;
    record.authored_entry = item.selected.entry;
    if (record.authored_item->record.words[22] == 13) {
        // GoldStack value (AddLoot value bonus). The original kernel draws the value
        // from the same loot RNG. Without an opener bonus the draw is still consumed
        // (so later source draws stay aligned) and nothing is published.
        std::int32_t bonus256 = 0;
        if (self->inputs_.opener_bonus256 &&
            self->inputs_.opener_bonus256(self->inputs_.bonus_context, item.opener_actor,
                                          bonus256, error)) {
            std::int32_t value = 0;
            if (dh2_loot_item_value_v7(&value, &random, &record.authored_item->record, nullptr, 0,
                                       bonus256) != 0)
                return fail(error, "Original GoldStack value kernel rejected the source item");
            record.resolved_gold_value = value;
        } else {
            error.clear();
            const auto low = record.authored_item->record.words[27];
            const auto high = record.authored_item->record.words[28];
            const auto range = std::uint32_t(high) + 1u - std::uint32_t(low);
            std::int32_t signed_range = 0;
            std::memcpy(&signed_range, &range, sizeof(signed_range));
            std::int32_t discarded = 0;
            if (dh2_loot_v2_random(&random, signed_range, &discarded) != 0)
                return fail(error, "Source GoldStack value RNG range is invalid");
            ++self->gold_unpriced_;
            return true;
        }
    }
    loot::RuntimeWorldItemIdV1 published = loot::invalid_runtime_world_item_v1;
    return self->inputs_.store->publish_source_object_drop(record, *item.source_definition,
                                                          *item.source_object, published, error);
}

} // namespace dh::foundation::containers
