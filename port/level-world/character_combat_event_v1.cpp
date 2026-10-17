#include "character_combat_event_v1.hpp"

#include <cstring>
#include <vector>

namespace dh2::character::combat_event_v1 {
Status route(const Request* request, Result* output) noexcept {
    if (!request || !output || !request->authored_name ||
        request->event.can_range > 1)
        return Status::invalid_argument;

    Result candidate{};
    auto event = request->event;
    if (event.state == 5) {
        std::int32_t parameters[3] = {0, 0, -1};
        const int ranged = dh2_attack_range_parameters(
            parameters, request->properties, request->inventory,
            request->item_rows, request->item_count);
        if (ranged < 0) return Status::range_query_failed;
        candidate.can_range = ranged;
        candidate.range_min = parameters[0];
        candidate.range_max = parameters[1];
        candidate.projectile = ranged ? parameters[2] : -1;
        event.can_range = static_cast<std::uint32_t>(ranged);
        event.projectile = candidate.projectile;
    }

    if (dh2_combat_event_route(&candidate.action, &event,
                               request->authored_name) != 0)
        return Status::route_failed;
    *output = candidate;
    return Status::complete;
}

Status route_snapshot(const std::int32_t resolved[224],
                      std::int32_t selected_set,
                      const std::int32_t main_hand_ids[2],
                      const bool main_hand_present[2],
                      const data::ItemTable* source_table,
                      const data::CombatEventContext* source_event,
                      const char* authored_name, Result* output) noexcept {
    if (!source_event || !authored_name || !output)
        return Status::invalid_argument;

    Result candidate{};
    auto event = *source_event;
    if (event.state == 5) {
        if (!resolved) return Status::invalid_argument;
        CombatProperties896 properties{};
        std::memcpy(properties.words, resolved, sizeof(properties.words));
        std::int32_t parameters[3] = {0, 0, -1};
        int ranged = -1;
        if (properties.words[32] != -1) {
            ranged = dh2_attack_range_parameters(parameters, &properties,
                                                  nullptr, nullptr, 0);
        } else {
            if (selected_set < 0 || selected_set >= 2 || !main_hand_ids ||
                !main_hand_present || !source_table ||
                source_table->rows.empty() || source_table->rows.size() > 65536)
                return Status::range_query_failed;
            try {
                const auto item_count = static_cast<std::uint32_t>(
                    source_table->rows.size());
                std::vector<CombatItemRecord164> rows(item_count);
                for (std::uint32_t i = 0; i < item_count; ++i)
                    std::memcpy(rows[i].words,
                                source_table->rows[i].record.words,
                                sizeof(rows[i].words));

                CombatItemInstance4 instances[2]{};
                const CombatItemInstance4* references[2]{};
                CombatEquipSet8 sets[2]{};
                for (std::size_t set_index = 0; set_index < 2; ++set_index) {
                    if (!main_hand_present[set_index]) continue;
                    const auto id = main_hand_ids[set_index];
                    if (id < 0 || static_cast<std::uint32_t>(id) >= item_count)
                        return Status::range_query_failed;
                    instances[set_index].item_id = id;
                    references[set_index] = &instances[set_index];
                    sets[set_index].main_hand = &references[set_index];
                }
                CombatInventory16 inventory{sets, 2, selected_set};
                ranged = dh2_attack_range_parameters(
                    parameters, &properties, &inventory, rows.data(), item_count);
            } catch (...) {
                return Status::range_query_failed;
            }
        }
        if (ranged < 0) return Status::range_query_failed;
        candidate.can_range = ranged;
        candidate.range_min = parameters[0];
        candidate.range_max = parameters[1];
        candidate.projectile = ranged ? parameters[2] : -1;
        event.can_range = static_cast<std::uint32_t>(ranged);
        event.projectile = candidate.projectile;
    }

    if (dh2_combat_event_route(&candidate.action, &event,
                               authored_name) != 0)
        return Status::route_failed;
    *output = candidate;
    return Status::complete;
}

Status route_character(const data::PropertyState* source_properties,
                       data::FreshInventoryOwnedV4* source_inventory,
                       const data::CombatEventContext* source_event,
                       const char* authored_name, Result* output) noexcept {
    if (!source_event || !authored_name || !output)
        return Status::invalid_argument;
    if (source_event->state != 5)
        return route_snapshot(source_properties ? source_properties->resolved.data() : nullptr,
                              0, nullptr, nullptr, nullptr,
                              source_event, authored_name, output);
    if (!source_properties || !source_inventory ||
        source_inventory->properties() != source_properties)
        return Status::invalid_argument;

    const auto& table = source_inventory->table();
    const auto& equipment = source_inventory->equipment();
    std::int32_t ids[2]{};
    bool present[2]{};
    for (std::size_t set_index = 0; set_index < 2; ++set_index) {
        const auto* slot = equipment[set_index][1];
        if (!slot) continue;
        if (!slot->item) return Status::range_query_failed;
        ids[set_index] = slot->item->id;
        present[set_index] = true;
    }
    return route_snapshot(source_properties->resolved.data(),
                          source_inventory->current_equipment(), ids, present,
                          &table,
                          source_event, authored_name, output);
}
}
