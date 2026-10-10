#include "runtime_world_item_adapter_v1.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>
#include <utility>
#include <cstring>

namespace dh::foundation::loot {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool valid_item_identity(const dh2::data::LootTablesV2::Borrow& tables,
                         std::int32_t item_id, const dh2::data::Item* authored,
                         std::string& error) {
    if (!tables) return fail(error, "Missing same original LootTables/ItemTable snapshot");
    const auto& items = tables.items();
    if (item_id < 0 || static_cast<std::size_t>(item_id) >= items.rows.size() ||
        items.identifiers.size() != items.rows.size())
        return fail(error, "World item has no valid original ItemTable index");
    if (!authored || authored != &items.rows[static_cast<std::size_t>(item_id)])
        return fail(error, "World item does not borrow the exact original ItemTable row");
    return true;
}

bool valid_position(const std::array<float, 3>& position) noexcept {
    return std::all_of(position.begin(), position.end(),
                       [](float value) { return std::isfinite(value); });
}

struct RunningGuard {
    bool& flag;
    explicit RunningGuard(bool& value) : flag(value) { flag = true; }
    ~RunningGuard() { flag = false; }
};

std::int32_t signed32(std::uint32_t bits) noexcept {
    std::int32_t result{};
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

bool stage_source_add_gold(dh::foundation::CharacterState& staged,
                           std::int32_t amount,
                           std::optional<std::int32_t> requested_limit,
                           std::string& error) {
    constexpr auto constructor_limit = std::numeric_limits<std::int32_t>::max();
    const auto limit = requested_limit.value_or(constructor_limit);
    if (limit < 0 || staged.gold > static_cast<std::uint64_t>(constructor_limit))
        return fail(error, "Source AddGold requires a valid signed gold owner and limit");

    auto gold = static_cast<std::int32_t>(staged.gold);
    if (amount < 0) {
        const auto removal = signed32(0u - std::uint32_t(amount));
        if (gold < removal) amount = signed32(0u - std::uint32_t(gold));
    }
    if (amount > 0) {
        const auto room = signed32(std::uint32_t(limit) - std::uint32_t(gold));
        if (amount > room) amount = room < 0 ? 0 : room;
    }
    const auto next = signed32(std::uint32_t(gold) + std::uint32_t(amount));
    if (next < 0)
        return fail(error, "Source AddGold negative/wrapped Debug continuation is unavailable");
    gold = next <= limit ? next : limit;
    staged.gold = static_cast<std::uint64_t>(gold);
    error.clear();
    return true;
}
}

bool RuntimeWorldItemAdapterV1::publish_death_drop(
    const RuntimeWorldItemRecordV1& record,
    const dh::foundation::ActorState& victim,
    RuntimeWorldItemIdV1& published_item,
    std::string& error) {
    error.clear();
    published_item = invalid_runtime_world_item_v1;
    if (running_) return fail(error, "Unsupported world-item store reentry");
    RunningGuard guard(running_);
    if (victim.id != record.source_actor)
        return fail(error, "Death drop source ActorId differs from the victim");
    return publish_at_position(record,victim.transform.position,published_item,error);
}

bool RuntimeWorldItemAdapterV1::publish_source_object_drop(
    const RuntimeWorldItemRecordV1& record,
    const dh::foundation::ActorDefinition& definition,
    const dh::foundation::WorldObject& source,
    RuntimeWorldItemIdV1& published_item,
    std::string& error) {
    error.clear();
    published_item=invalid_runtime_world_item_v1;
    if(running_)return fail(error,"Unsupported world-item store reentry");
    RunningGuard guard(running_);
    if(source.id==dh::foundation::invalid_object_id||
       source.id!=record.source_actor||definition.stableId!=record.source_actor||
       source.name.empty()||source.name!=definition.name)
        return fail(error,"Source-object drop ObjectId/ActorDefinition/WorldObject identity mismatch");
    return publish_at_position(record,source.transform.position,published_item,error);
}

bool RuntimeWorldItemAdapterV1::publish_at_position(
    const RuntimeWorldItemRecordV1& record,
    const std::array<float,3>& position,
    RuntimeWorldItemIdV1& published_item,
    std::string& error) {
    if (!valid_item_identity(tables_, record.item_id, record.authored_item, error)) return false;
    if (record.loot_table < 0 || !tables_ ||
        static_cast<std::size_t>(record.loot_table) >= tables_.loots().size() ||
        !record.authored_entry || record.quantity == 0)
        return fail(error, "World item is missing its original Loot row, entry, or quantity");
    if (record.authored_item->record.words[22] == 13 && !record.resolved_gold_value)
        return fail(error, "GoldStack drop has no source-resolved ItemInstance value");
    for (float value : position)
        if (!std::isfinite(value)) return fail(error, "Death drop source position is nonfinite");
    if (next_identity_ == invalid_runtime_world_item_v1 ||
        next_identity_ == std::numeric_limits<RuntimeWorldItemIdV1>::max())
        return fail(error, "Runtime world-item identity space exhausted");

    RuntimeWorldItemEntryV1 entry;
    entry.identity = next_identity_;
    entry.source_outcome = record;
    entry.authored_item = record.authored_item;
    entry.quantity = record.quantity;
    entry.source_position = position;
    entry.inventory_instance_id = "world-drop-" + std::to_string(entry.identity);

    try {
        auto inserted = items_.emplace(entry.identity, entry);
        if (!inserted.second) return fail(error, "Runtime world-item identity collided with a live drop");
    } catch (...) {
        return fail(error, "Runtime world-item publication allocation failed");
    }
    published_item = next_identity_++;
    return true;
}

bool RuntimeWorldItemAdapterV1::spawn_world_item_thunk(
    void* raw, const RuntimeWorldItemRecordV1& record,
    const dh::foundation::ActorState& victim,
    const dh::foundation::ActorState* killer, std::string& error) {
    (void)killer; // Killer identity is already retained in source_outcome.
    if (!raw) return fail(error, "Missing runtime world-item store callback context");
    RuntimeWorldItemIdV1 published{};
    return static_cast<RuntimeWorldItemAdapterV1*>(raw)->publish_death_drop(
        record, victim, published, error);
}

bool RuntimeWorldItemAdapterV1::inspect(RuntimeWorldItemIdV1 identity,
                                        RuntimeWorldItemEntryV1& output,
                                        std::string& error) const {
    error.clear();
    auto found = items_.find(identity);
    if (identity == invalid_runtime_world_item_v1 || found == items_.end())
        return fail(error, "Unknown or already-consumed runtime world item");
    output = found->second;
    return true;
}

bool RuntimeWorldItemAdapterV1::render_items(
    std::vector<RuntimeWorldItemRenderV1>& output, std::string& error) const {
    output.clear();
    error.clear();
    if (!tables_) return fail(error, "Missing actual ItemTable for world-item rendering");
    std::vector<RuntimeWorldItemRenderV1> next;
    try {
        next.reserve(items_.size());
        const auto& table = tables_.items();
        for (const auto& pair : items_) {
            const auto& drop = pair.second;
            if (!valid_item_identity(tables_, drop.source_outcome.item_id,
                                     drop.authored_item, error)) return false;
            const auto index = static_cast<std::size_t>(drop.source_outcome.item_id);
            const auto& row = table.rows[index];
            RuntimeWorldItemRenderV1 view;
            view.identity = drop.identity;
            view.item_id = drop.source_outcome.item_id;
            view.item_identifier = table.identifiers[index];
            view.exact_icon_name = row.icon_name;
            view.source_name_text_oid = row.record.words[17];
            view.quantity = drop.quantity;
            view.position = drop.source_position;
            view.has_exact_icon = !row.icon_name.empty();
            next.push_back(std::move(view));
        }
    } catch (...) {
        return fail(error, "World-item render enumeration allocation failed");
    }
    output = std::move(next);
    return true;
}

bool RuntimeWorldItemAdapterV1::pickup(
    RuntimeWorldItemIdV1 identity,
    dh::foundation::CharacterState& character,
    RuntimeWorldItemPickupReceiptV1& receipt,
    std::string& error,
    std::optional<std::int32_t> source_gold_limit) {
    receipt = {};
    receipt.world_item = identity;
    error.clear();
    if (running_) return fail(error, "Unsupported runtime world-item store reentry");
    RunningGuard guard(running_);
    auto found = items_.find(identity);
    if (identity == invalid_runtime_world_item_v1 || found == items_.end())
        return fail(error, "Unknown or already-consumed runtime world item");
    const auto& drop = found->second;
    if (!valid_item_identity(tables_, drop.source_outcome.item_id,
                             drop.authored_item, error)) return false;
    if (drop.quantity == 0 || drop.quantity != drop.source_outcome.quantity ||
        drop.inventory_instance_id.empty() || !valid_position(drop.source_position))
        return fail(error, "Runtime world item lost its source quantity, identity, or position");

    // Stage all validation and stack arithmetic before removing the item.
    // Once this succeeds, the only commit operations are map erase and a
    // statically verified no-throw CharacterState swap.
    try {
        dh::foundation::CharacterState staged = character;
        const auto item_index = static_cast<std::size_t>(drop.source_outcome.item_id);
        if (tables_.items().rows[item_index].record.words[22] == 13) {
            if (!drop.source_outcome.resolved_gold_value)
                return fail(error, "GoldStack pickup has no source-resolved value");
            if (!stage_source_add_gold(staged, *drop.source_outcome.resolved_gold_value,
                                       source_gold_limit, error)) return false;
            receipt.inventory_instance_id = drop.inventory_instance_id;
            receipt.quantity = drop.quantity;
            static_assert(std::is_nothrow_swappable<dh::foundation::CharacterState>::value,
                          "World item erase and CharacterState commit must be no-throw");
            items_.erase(found);
            using std::swap;
            swap(character, staged);
            receipt.completed = true;
            return true;
        }
        std::string inventory_id = drop.inventory_instance_id;
        const auto id_in_use = [&](const std::string& id) {
            return std::any_of(staged.inventory.begin(), staged.inventory.end(),
                               [&](const auto& owned) { return owned.instance_id == id; });
        };
        std::uint32_t collision_suffix = 0;
        while (id_in_use(inventory_id)) {
            if (++collision_suffix > dh::foundation::character_collection_limit) {
                return fail(error, "Unable to derive a unique world-drop inventory instance identity");
            }
            inventory_id = drop.inventory_instance_id + "-" + std::to_string(collision_suffix);
        }
        dh::foundation::InventoryItem incoming{
            inventory_id,
            tables_.items().identifiers[item_index],
            drop.quantity};
        dh::foundation::inventory::Presenter inventory(staged, tables_.items());
        if (!inventory.pickup(incoming, receipt.retained_instance_id, error)) {
            receipt.retained_instance_id.clear();
            return false;
        }
        // Any string allocation for the receipt must finish before either
        // canonical store or CharacterState changes.
        receipt.inventory_instance_id = inventory_id;
        receipt.quantity = drop.quantity;
        static_assert(std::is_nothrow_swappable<dh::foundation::CharacterState>::value,
                      "World item erase and CharacterState commit must be no-throw");
        items_.erase(found);
        using std::swap;
        swap(character, staged);
        receipt.completed = true;
        return true;
    } catch (const std::exception& exception) {
        receipt = {};
        receipt.world_item = identity;
        error = std::string("World-item pickup staging failed before commit: ") + exception.what();
        return false;
    } catch (...) {
        receipt = {};
        receipt.world_item = identity;
        return fail(error, "World-item pickup staging failed before commit");
    }
}

} // namespace dh::foundation::loot
