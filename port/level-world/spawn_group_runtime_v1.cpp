#include "spawn_group_runtime_v1.hpp"

#include <algorithm>
#include <cstdio>
#include <limits>
#include <string>
#include <utility>

namespace dh2::spawn_group_runtime_v1 {
namespace {

std::uint32_t spawn_unique_id = 0;

bool random_below(const Services& services, std::uint32_t bound,
                  std::uint32_t* value) noexcept {
    if (!bound || !services.random_below(services.context, bound, value))
        return false;
    return *value < bound;
}

bool eligible(const GroupDefinition& definition, const Spot& spot) noexcept {
    if (!definition.active_spot_only || !spot.interactive ||
        !spot.zoning_enabled)
        return true;
    return spot.in_zone;
}

bool select_weighted(const std::vector<SpawnInfo>& entries,
                     const Services& services,
                     const SpawnInfo** selected) noexcept {
    std::int64_t total = 0;
    for (const auto& entry : entries) {
        if (entry.probability < 0) return false;
        total += entry.probability;
    }
    if (total <= 0 || total > std::numeric_limits<std::int32_t>::max())
        return false;

    std::uint32_t draw = 0;
    if (!random_below(services, static_cast<std::uint32_t>(total), &draw))
        return false;
    auto roll = static_cast<std::int32_t>(draw);
    for (const auto& entry : entries) {
        roll -= entry.probability;
        if (roll < 0) {
            *selected = &entry;
            return true;
        }
    }
    return false;
}

std::string next_name() {
    char name[32]{};
    std::snprintf(name, sizeof(name), "Spawn_%05u", ++spawn_unique_id);
    return name;
}

} // namespace

void Owner::define_group(std::int32_t group_id, GroupDefinition definition) {
    groups_[group_id] = GroupState{std::move(definition), 0, {}};
}

bool Owner::add_spot(std::int32_t group_id, const Spot& spot) {
    const auto it = groups_.find(group_id);
    if (it == groups_.end()) return false;
    it->second.spots.push_back(spot);
    return true;
}

void Owner::remove_spot(std::uintptr_t spot_handle) {
    for (auto it = groups_.begin(); it != groups_.end();) {
        auto& spots = it->second.spots;
        spots.erase(std::remove_if(spots.begin(), spots.end(),
                                   [spot_handle](const Spot& spot) {
                                       return spot.handle == spot_handle;
                                   }),
                    spots.end());
        if (spots.empty()) it = groups_.erase(it);
        else ++it;
    }
}

Status Owner::update(std::int32_t dt_ms, const Services* services) {
    if (!services || !services->random_below || !services->create_character ||
        !services->init_spawned || !services->place_object)
        return Status::invalid_argument;

    for (auto& [group_id, state] : groups_) {
        (void)group_id;
        state.timer_ms = static_cast<std::int32_t>(
            static_cast<std::uint32_t>(state.timer_ms) -
            static_cast<std::uint32_t>(dt_ms));
        if (state.timer_ms > 0) continue;

        const auto& definition = state.definition;
        state.timer_ms = definition.delay_ms;

        const SpawnInfo* selected = nullptr;
        if (!select_weighted(definition.entries, *services, &selected) ||
            !selected || selected->quantity <= 0)
            continue;

        std::vector<Spot> candidates;
        candidates.reserve(state.spots.size());
        for (const auto& spot : state.spots)
            if (eligible(definition, spot)) candidates.push_back(spot);

        if (candidates.empty()) {
            state.timer_ms = 1000;
            continue;
        }

        for (std::int32_t count = 0;
             count < selected->quantity && !candidates.empty(); ++count) {
            std::uint32_t index = 0;
            if (!random_below(*services,
                              static_cast<std::uint32_t>(candidates.size()),
                              &index))
                break;

            const Spot spot = candidates[index];
            candidates.erase(candidates.begin() + index);
            const auto name = next_name();
            const auto actor = services->create_character(
                services->context, nullptr, "Character", name.c_str(), true);
            if (!actor) continue;

            services->init_spawned(services->context, actor,
                                   selected->character_id, spot.position);
            services->place_object(services->context, spot.handle, actor);
        }
    }
    return Status::complete;
}

std::int32_t Owner::timer_ms(std::int32_t group_id) const noexcept {
    const auto it = groups_.find(group_id);
    return it == groups_.end() ? 0 : it->second.timer_ms;
}

} // namespace dh2::spawn_group_runtime_v1
