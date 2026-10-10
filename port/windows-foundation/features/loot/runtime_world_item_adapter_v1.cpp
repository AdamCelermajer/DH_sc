#include "runtime_world_item_adapter_v1.hpp"
#include "world_drop_rules_v1.hpp"
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
    entry.destination = position;
    entry.visual_row = record.authored_item->record.words[item_word_audio_visual_v1];
    entry.spawn_sequence = next_sequence_;

    try {
        // ItemManager::Spawn: five ItemObjects per AudioVisual category, reused
        // round-robin; the slot being reused is de-spawned (its item is gone).
        if (entry.visual_row >= 0) {
            for (;;) {
                std::size_t live = 0;
                auto oldest = items_.end();
                for (auto it = items_.begin(); it != items_.end(); ++it) {
                    if (it->second.visual_row != entry.visual_row) continue;
                    ++live;
                    if (oldest == items_.end() ||
                        it->second.spawn_sequence < oldest->second.spawn_sequence) oldest = it;
                }
                if (live < world_item_pool_slots_per_category_v1) break;
                items_.erase(oldest);
                ++pool_evictions_;
            }
        }
        auto inserted = items_.emplace(entry.identity, entry);
        if (!inserted.second) return fail(error, "Runtime world-item identity collided with a live drop");
    } catch (...) {
        return fail(error, "Runtime world-item publication allocation failed");
    }
    ++next_sequence_;
    published_item = next_identity_++;
    notify_sound_(WorldItemSoundEventV1::drop, items_.at(published_item));
    return true;
}

bool scatter_destination_v1(dh2::data::LootRandom8V2& rng,
                            const std::array<float, 3>& victim,
                            const std::array<float, 3>* killer,
                            std::array<float, 3>& out, std::string& error) {
    error.clear();
    std::int32_t draw{};
    if (!killer) {
        out = victim;
        if (dh2_loot_v2_random(&rng, 500, &draw)) return fail(error, "Scatter Random failed");
        out[0] = float(draw - 250) + out[0];
        if (dh2_loot_v2_random(&rng, 500, &draw)) return fail(error, "Scatter Random failed");
        out[1] = out[1] + float(draw - 250);
        return true;
    }
    float dir[3]{(*killer)[0] - victim[0], (*killer)[1] - victim[1], (*killer)[2] - victim[2]};
    // Point3D<float>::normalize34d0b0 divides unconditionally (IEEE zero/NaN kept);
    // a coincident killer would produce NaN, which is rejected below.
    const float length = std::sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
    for (float& v : dir) v /= length;
    if (dh2_loot_v2_random(&rng, 200, &draw)) return fail(error, "Scatter Random failed");
    const float along = float(draw + 150);
    if (dh2_loot_v2_random(&rng, 300, &draw)) return fail(error, "Scatter Random failed");
    const float across = float(draw - 150);
    constexpr float k[3]{0.f, 0.f, 1.f}; // Vec3f_K
    const float cx = dir[1] * k[2] - dir[2] * k[1];
    const float cy = dir[2] * k[0] - dir[0] * k[2];
    const float cz = dir[0] * k[1] - dir[1] * k[0];
    out = {(across * cx + dir[0] * along) + victim[0],
           (across * cy + dir[1] * along) + victim[1],
           (across * cz + dir[2] * along) + victim[2]};
    if (!valid_position(out)) return fail(error, "Scatter produced a nonfinite landing point");
    return true;
}

std::int32_t item_power_font_palette_row_v1(std::size_t power_count) noexcept {
    switch (power_count) {
    case 0: return 6;
    case 1: return 4;
    case 2: return 1;
    case 3: return 5;
    case 4: return 3;
    default: return 2;
    }
}

bool RuntimeWorldItemAdapterV1::publish_death_drop_scattered(
    const RuntimeWorldItemRecordV1& record, const dh::foundation::ActorState& victim,
    const std::array<float, 3>* killer_position, dh2::data::LootRandom8V2* rng,
    RuntimeWorldItemIdV1& published_item, std::string& error) {
    if (!publish_death_drop(record, victim, published_item, error)) return false;
    if (!rng) return true;
    std::array<float, 3> landing{};
    if (!scatter_destination_v1(*rng, victim.transform.position, killer_position, landing, error)) {
        // Keep the published item at the victim position (no reward is lost).
        error.clear();
        return true;
    }
    items_.at(published_item).destination = landing;
    return true;
}

bool RuntimeWorldItemAdapterV1::publish_inventory_drop(
    const dh::foundation::InventoryItem& item, const std::array<float, 3>& position,
    dh::foundation::ActorId owner_actor, std::int32_t owner_protect_ms,
    RuntimeWorldItemIdV1& published_item, std::string& error) {
    error.clear();
    published_item = invalid_runtime_world_item_v1;
    if (running_) return fail(error, "Unsupported world-item store reentry");
    RunningGuard guard(running_);
    if (!tables_) return fail(error, "Missing same original LootTables/ItemTable snapshot");
    if (item.quantity == 0 || item.quantity > 255)
        return fail(error, "Dropped world item quantity must be 1..255");
    if (!valid_position(position)) return fail(error, "Dropped item position is nonfinite");
    const auto& table = tables_.items();
    std::size_t index = table.identifiers.size();
    for (std::size_t i = 0; i < table.identifiers.size(); ++i)
        if (table.identifiers[i] == item.definition_id) { index = i; break; }
    if (index == table.identifiers.size() || index >= table.rows.size())
        return fail(error, "Dropped item has no original ItemTable row");
    if (next_identity_ == invalid_runtime_world_item_v1 ||
        next_identity_ == std::numeric_limits<RuntimeWorldItemIdV1>::max())
        return fail(error, "Runtime world-item identity space exhausted");

    RuntimeWorldItemEntryV1 entry;
    entry.identity = next_identity_;
    entry.authored_item = &table.rows[index];
    entry.source_outcome.source_actor = owner_actor;
    entry.source_outcome.killer_actor = invalid_actor_id;
    entry.source_outcome.loot_table = -1;
    entry.source_outcome.item_id = static_cast<std::int16_t>(index);
    entry.source_outcome.quantity = static_cast<std::uint8_t>(item.quantity);
    entry.source_outcome.authored_item = entry.authored_item;
    entry.quantity = item.quantity;
    entry.source_position = position;
    entry.destination = position;
    entry.inventory_instance_id = item.instance_id.empty()
        ? "world-drop-" + std::to_string(entry.identity) : item.instance_id;
    entry.visual_row = entry.authored_item->record.words[item_word_audio_visual_v1];
    entry.spawn_sequence = next_sequence_;
    entry.owner_actor = owner_actor;
    entry.owner_protect_ms = owner_protect_ms;
    entry.from_inventory = true;
    try {
        if (entry.visual_row >= 0) {
            for (;;) {
                std::size_t live = 0;
                auto oldest = items_.end();
                for (auto it = items_.begin(); it != items_.end(); ++it) {
                    if (it->second.visual_row != entry.visual_row) continue;
                    ++live;
                    if (oldest == items_.end() ||
                        it->second.spawn_sequence < oldest->second.spawn_sequence) oldest = it;
                }
                if (live < world_item_pool_slots_per_category_v1) break;
                items_.erase(oldest);
                ++pool_evictions_;
            }
        }
        if (!items_.emplace(entry.identity, entry).second)
            return fail(error, "Runtime world-item identity collided with a live drop");
    } catch (...) {
        return fail(error, "Runtime world-item publication allocation failed");
    }
    ++next_sequence_;
    published_item = next_identity_++;
    notify_sound_(WorldItemSoundEventV1::drop, items_.at(published_item));
    return true;
}

void RuntimeWorldItemAdapterV1::advance(std::uint32_t dt_ms) noexcept {
    const float step_per_ms = source_item_speed_word_v1 * assumed_item_ticks_per_second_v1 / 1000.0f;
    for (auto& pair : items_) {
        auto& item = pair.second;
        item.age_ms += dt_ms;
        if (item.owner_protect_ms > 0)
            item.owner_protect_ms = item.owner_protect_ms > std::int32_t(dt_ms)
                ? item.owner_protect_ms - std::int32_t(dt_ms) : 0;
        float delta[3]{item.destination[0] - item.source_position[0],
                       item.destination[1] - item.source_position[1],
                       item.destination[2] - item.source_position[2]};
        const float distance = std::sqrt(delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2]);
        if (!(distance > 0.0f)) continue;
        const float step = step_per_ms * float(dt_ms);
        if (step >= distance) item.source_position = item.destination;
        else for (int i = 0; i < 3; ++i) item.source_position[i] += delta[i] / distance * step;
    }
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
            const RuntimeWorldItemEntryV1 picked = found->second;
            items_.erase(found);
            using std::swap;
            swap(character, staged);
            receipt.completed = true;
            notify_sound_(WorldItemSoundEventV1::pickup, picked);
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
        const RuntimeWorldItemEntryV1 picked = found->second;
        items_.erase(found);
        using std::swap;
        swap(character, staged);
        receipt.completed = true;
        notify_sound_(WorldItemSoundEventV1::pickup, picked);
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
