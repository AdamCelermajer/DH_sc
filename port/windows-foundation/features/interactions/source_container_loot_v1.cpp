#include "source_container_loot_v1.hpp"

#include "../../../game-data/loot_table_selection_v8.hpp"

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
}

bool SourceContainerLootV1::drop(
    ActorId source_actor, std::uint64_t binding_lifecycle,
    const ActorDefinition& definition, const ActorState& source_state,
    ActorId opener_actor, std::int32_t table, std::int32_t fixed_powers,
    bool source_flag, const SourceContainerLootServicesV1& services,
    SourceContainerLootReceiptV1& output, std::string& error) {
    return drop_impl(source_actor,binding_lifecycle,definition,&source_state,nullptr,
        opener_actor,table,fixed_powers,source_flag,services,output,error);
}

bool SourceContainerLootV1::drop(
    ActorId source_actor, std::uint64_t binding_lifecycle,
    const ActorDefinition& definition, const WorldObject& source_object,
    ActorId opener_actor, std::int32_t table, std::int32_t fixed_powers,
    bool source_flag, const SourceContainerLootServicesV1& services,
    SourceContainerLootReceiptV1& output, std::string& error) {
    return drop_impl(source_actor,binding_lifecycle,definition,nullptr,&source_object,
        opener_actor,table,fixed_powers,source_flag,services,output,error);
}

bool SourceContainerLootV1::drop_impl(
    ActorId source_actor, std::uint64_t binding_lifecycle,
    const ActorDefinition& definition, const ActorState* source_state,
    const WorldObject* source_object, ActorId opener_actor,
    std::int32_t table, std::int32_t fixed_powers, bool source_flag,
    const SourceContainerLootServicesV1& services,
    SourceContainerLootReceiptV1& output, std::string& error) {
    output = {};
    error.clear();
    if (source_actor == invalid_actor_id || opener_actor == invalid_actor_id ||
        !binding_lifecycle)
        return fail(error, "Container loot requires source/opener ActorId and binding lifecycle");
    if (definition.stableId != source_actor || (!source_state && !source_object))
        return fail(error, "Container loot requires same-ID authored definition and WorldObject or ActorState");
    if (source_state && (source_state->id != source_actor ||
        source_state->definition_id != definition.name))
        return fail(error, "Container loot source ActorId/ActorDefinition/ActorState mismatch");
    if (source_object && (source_object->id != source_actor ||
        source_object->name != definition.name))
        return fail(error, "Container loot source ObjectId/ActorDefinition/WorldObject mismatch");

    const Key key{source_actor, binding_lifecycle};
    if (receipts_.find(key) != receipts_.end())
        return fail(error, "Container loot already attempted for same ActorId lifecycle");
    output.source_actor = source_actor;
    output.binding_lifecycle = binding_lifecycle;
    output.state = SourceContainerLootStateV1::attempted;
    auto inserted = receipts_.emplace(key, output).first;
    auto& receipt = inserted->second;

    // The original source receiver treats an absent table as an empty drop.
    // Do not query RNG or synthesize a fallback table in that case.
    if (table < 0) {
        receipt.state = SourceContainerLootStateV1::completed;
        output = receipt;
        return true;
    }
    if (!services.tables || !services.powers ||
        (!services.gameplay_rng && !services.with_gameplay_rng) ||
        (!services.drop_item && !services.drop_item_with_rng)) {
        receipt.state = SourceContainerLootStateV1::failed;
        output = receipt;
        return fail(error, "Container loot requires original tables/powers/shared RNG/drop sink");
    }
    if (static_cast<std::size_t>(table) >= services.tables.loots().size()) {
        receipt.state = SourceContainerLootStateV1::failed;
        output = receipt;
        return fail(error, "Container loot table index outside original source tables");
    }

    const auto select_and_publish = [&](dh2::data::LootRandom8V2& random,
                                        std::string& operation_error) {
        dh2::data::LootTableSelectionV8 table_selection(
            services.tables, services.powers, random, services.entry);
        std::vector<const dh2::data::LootEntry32V2*> selected_entries;
        if (!table_selection.select(table, selected_entries, operation_error)) return false;
        dh2::data::LootItemSelectionV8 item_selection(
            services.tables, random, services.entry);
        std::vector<dh2::data::LootItemInfoV8> selected_items;
        if (!item_selection.expand(selected_entries, false, selected_items, operation_error)) return false;
        receipt.selected_items = selected_items.size();
        for (const auto& selected : selected_items) {
            SourceContainerDropItemV1 item;
            item.source_actor = source_actor;
            item.opener_actor = opener_actor;
            item.loot_table = table;
            item.fixed_powers = fixed_powers;
            item.source_flag = source_flag;
            item.source_definition = &definition;
            item.source_state = source_state;
            item.source_object = source_object;
            item.source_position = source_object ? source_object->transform.position :
                source_state->transform.position;
            item.selected = selected;
            const bool published = services.drop_item_with_rng ?
                services.drop_item_with_rng(services.context,item,random,operation_error) :
                services.drop_item(services.context,item,operation_error);
            if (!published) return false;
            ++receipt.delivered_items;
        }
        return true;
    };
    const bool selected_ok = services.with_gameplay_rng ?
        services.with_gameplay_rng(services.context,select_and_publish,error) :
        select_and_publish(*services.gameplay_rng,error);
    if(!selected_ok){
        receipt.state = SourceContainerLootStateV1::failed;
        output = receipt;
        return false;
    }
    receipt.state = SourceContainerLootStateV1::completed;
    output = receipt;
    return true;
}

} // namespace dh::foundation::interactions
