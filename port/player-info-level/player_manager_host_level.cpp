#include "player_manager_host_level.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>

namespace dh2::player_manager_host_level {
namespace {

bool valid_registry(const PlayerRegistry* registry) noexcept {
    if (!registry || registry->manager_identity == 0 || !registry->manager_plus_8 ||
        registry->manager_plus_8->identity == 0 ||
        !registry->manager_plus_8->character_level_member ||
        registry->entry_count > 4096 ||
        (registry->entry_count != 0 && !registry->entries)) {
        return false;
    }
    std::int32_t previous = 0;
    bool have_previous = false;
    for (std::uint32_t i = 0; i < registry->entry_count; ++i) {
        const auto* entry = registry->entries[i];
        if (!entry || entry->identity == 0 || !entry->character_level_member ||
            (have_previous && entry->internal_id <= previous)) {
            return false;
        }
        previous = entry->internal_id;
        have_previous = true;
    }
    return true;
}

bool ranges_overlap(const void* left, std::size_t left_size,
                    const void* right, std::size_t right_size) noexcept {
    if (!left || !right || left_size == 0 || right_size == 0) return false;
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    if (left_size > std::numeric_limits<std::uintptr_t>::max() - a ||
        right_size > std::numeric_limits<std::uintptr_t>::max() - b) return true;
    return a < b + right_size && b < a + left_size;
}

bool result_overlaps_registry(const Result* output,
                              const PlayerRegistry* registry,
                              const Services* services) noexcept {
    if (ranges_overlap(output, sizeof(*output), registry, sizeof(*registry)) ||
        ranges_overlap(output, sizeof(*output), services, sizeof(*services)) ||
        ranges_overlap(output, sizeof(*output), registry->manager_plus_8,
                       sizeof(*registry->manager_plus_8)) ||
        ranges_overlap(output, sizeof(*output),
                       registry->manager_plus_8->character_level_member,
                       sizeof(*registry->manager_plus_8->character_level_member)) ||
        ranges_overlap(output, sizeof(*output), registry->entries,
                       registry->entry_count * sizeof(*registry->entries))) {
        return true;
    }
    for (std::uint32_t i = 0; i < registry->entry_count; ++i) {
        const auto* player = registry->entries[i];
        if (ranges_overlap(output, sizeof(*output), player, sizeof(*player)) ||
            ranges_overlap(output, sizeof(*output), player->character_level_member,
                           sizeof(*player->character_level_member))) {
            return true;
        }
    }
    return false;
}

template <typename Function, typename... Args>
bool call(const Services* services, std::uint32_t& calls,
          Function function, Args... args) noexcept {
    if (!function) return false;
    ++calls;
    try {return function(services->context, args...) == 0;} catch (...) {return false;}
}

enum class SelectStatus : std::uint32_t {
    selected,
    service_unavailable,
    service_failed,
    no_player,
};

struct Selection {
    PlayerInfoProjection* player = nullptr;
    Route route = Route::none;
    std::uint32_t local_examined = 0;
    SelectStatus status = SelectStatus::selected;
};

bool select_local(const PlayerRegistry* registry, std::int32_t internal_id,
                  Selection& selection) noexcept {
    std::uint32_t low = 0;
    std::uint32_t high = registry->entry_count;
    while (low < high) {
        const auto middle = low + (high - low) / 2;
        ++selection.local_examined;
        const auto key = registry->entries[middle]->internal_id;
        if (key < internal_id) low = middle + 1;
        else high = middle;
    }
    if (low < registry->entry_count &&
        registry->entries[low]->internal_id == internal_id) {
        selection.player = registry->entries[low];
        selection.route = Route::local_id_tree;
    } else {
        // GetPlayerByInternalID returns PlayerManager+8 on a tree miss.
        selection.player = registry->manager_plus_8;
        selection.route = Route::manager_plus_8_fallback;
    }
    return selection.player != nullptr;
}

SelectStatus select_internal_id(const PlayerRegistry* registry,
                                 const Services* services,
                                 std::int32_t internal_id,
                                 std::uint32_t lookup_flag,
                                 std::uint32_t& service_calls,
                                Selection& selection) noexcept {
    if (internal_id == -1) {
        selection.player = registry->manager_plus_8;
        selection.route = Route::manager_plus_8_fallback;
        return SelectStatus::selected;
    }

    // The source GetPlayerByInternalID performs its own fresh online/network
    // checks after GetHostingPlayer has already selected an ID.
    std::uint8_t online = 0;
    if (!services->read_online_byte_5) return SelectStatus::service_unavailable;
    if (!call(services, service_calls, services->read_online_byte_5, &online))
        return SelectStatus::service_failed;
    if (online != 0) {
        std::uint8_t game_state_online = 0;
        if (!services->read_online_game_state_byte_24) return SelectStatus::service_unavailable;
        if (!call(services, service_calls, services->read_online_game_state_byte_24,
                  &game_state_online)) return SelectStatus::service_failed;
        if (game_state_online != 0) {
            std::int32_t is_host = 0;
            if (!services->matching_is_host) return SelectStatus::service_unavailable;
            if (!call(services, service_calls, services->matching_is_host, &is_host))
                return SelectStatus::service_failed;
            if (is_host != 0) {
                std::uintptr_t net_manager = 0;
                if (!services->get_net_player_manager) return SelectStatus::service_unavailable;
                if (!call(services, service_calls, services->get_net_player_manager,
                          &net_manager)) return SelectStatus::service_failed;
                if (net_manager == 0) return SelectStatus::service_failed;
                std::int32_t initialized = 0;
                if (!services->net_player_manager_is_initialized)
                    return SelectStatus::service_unavailable;
                if (!call(services, service_calls,
                          services->net_player_manager_is_initialized,
                          net_manager, &initialized)) return SelectStatus::service_failed;
                if (initialized != 0) {
                    PlayerInfoProjection* player = nullptr;
                    if (!services->get_net_player_info) return SelectStatus::service_unavailable;
                    if (!call(services, service_calls, services->get_net_player_info,
                               net_manager, internal_id, lookup_flag, &player))
                        return SelectStatus::service_failed;
                    if (!player) return SelectStatus::no_player;
                    selection.player = player;
                    selection.route = Route::net_player_info;
                    return SelectStatus::selected;
                }
            }
        }
    }

    select_local(registry, internal_id, selection);
    return SelectStatus::selected;
}

void finish_result(Result* output, const Result& value) noexcept {
    *output = value;
}

}  // namespace

Status get_hosting_level(const PlayerRegistry* registry,
                         const Services* services,
                         Result* output) {
    if (!services || !output) return Status::invalid_argument;
    if (!valid_registry(registry)) return Status::invalid_registry;
    if (result_overlaps_registry(output, registry, services))
        return Status::invalid_argument;

    Result report{};
    report.status = Status::service_unavailable;
    report.route = Route::none;
    report.requested_internal_id = 0;

    std::int32_t internal_id = 0;
    std::uint8_t online = 0;
    if (!services->read_online_byte_5) {
        finish_result(output, report);
        return report.status;
    }
    if (!call(services, report.service_calls, services->read_online_byte_5, &online)) {
        report.status = Status::service_failed;
        finish_result(output, report);
        return report.status;
    }
    if (online != 0) {
        std::uint8_t game_state_online = 0;
        if (!services->read_online_game_state_byte_24) {
            finish_result(output, report);
            return report.status;
        }
        if (!call(services, report.service_calls,
                  services->read_online_game_state_byte_24, &game_state_online)) {
            report.status = Status::service_failed;
            finish_result(output, report);
            return report.status;
        }
        if (game_state_online != 0) {
            std::int32_t is_host = 0;
            if (!services->matching_is_host) {
                finish_result(output, report);
                return report.status;
            }
            if (!call(services, report.service_calls, services->matching_is_host, &is_host)) {
                report.status = Status::service_failed;
                finish_result(output, report);
                return report.status;
            }
            if (is_host != 0) {
                std::uintptr_t net_manager = 0;
                if (!services->get_net_player_manager) {
                    finish_result(output, report);
                    return report.status;
                }
                if (!call(services, report.service_calls, services->get_net_player_manager,
                          &net_manager) || net_manager == 0) {
                    report.status = Status::service_failed;
                    finish_result(output, report);
                    return report.status;
                }
                std::int32_t initialized = 0;
                if (!services->net_player_manager_is_initialized) {
                    finish_result(output, report);
                    return report.status;
                }
                if (!call(services, report.service_calls,
                          services->net_player_manager_is_initialized,
                          net_manager, &initialized)) {
                    report.status = Status::service_failed;
                    finish_result(output, report);
                    return report.status;
                }
                if (initialized != 0) {
                    // Source reacquires the singleton instead of reusing the
                    // manager used for IsInitialized.
                    net_manager = 0;
                    if (!services->get_net_player_manager) {
                        finish_result(output, report);
                        return report.status;
                    }
                    if (!call(services, report.service_calls, services->get_net_player_manager,
                              &net_manager) || net_manager == 0) {
                        report.status = Status::service_failed;
                        finish_result(output, report);
                        return report.status;
                    }
                    if (!services->read_net_host_internal_id) {
                        finish_result(output, report);
                        return report.status;
                    }
                    if (!call(services, report.service_calls,
                              services->read_net_host_internal_id, net_manager, &internal_id)) {
                        report.status = Status::service_failed;
                        finish_result(output, report);
                        return report.status;
                    }
                }
            }
        }
    }

    report.requested_internal_id = internal_id;
    Selection selected;
    const auto selection_status = select_internal_id(registry, services, internal_id, 0,
                                                      report.service_calls, selected);
    report.local_entries_examined = selected.local_examined;
    if (selection_status != SelectStatus::selected) {
        report.status = selection_status == SelectStatus::service_unavailable
                            ? Status::service_unavailable
                            : selection_status == SelectStatus::no_player
                                  ? Status::no_player_projection
                                  : Status::service_failed;
        finish_result(output, report);
        return report.status;
    }
    if (!selected.player) {
        report.status = Status::no_player_projection;
        finish_result(output, report);
        return report.status;
    }
    if (!selected.player->character_level_member) {
        report.status = Status::no_level_member;
        finish_result(output, report);
        return report.status;
    }
    report.route = selected.route;
    report.player_identity = selected.player->identity;
    report.character_level_330 = selected.player->character_level_member->value;
    report.status = Status::complete;
    finish_result(output, report);
    return report.status;
}

Status get_player_by_internal_id(const PlayerRegistry* registry,
                                const Services* services,
                                std::int32_t internal_id,
                                std::uint32_t lookup_flag,
                                PlayerInfoProjection** output_player,
                                Result* output) {
    if ((!services && internal_id!=-1) || !output_player || !output) return Status::invalid_argument;
    if (!valid_registry(registry)) return Status::invalid_registry;
    if (result_overlaps_registry(output, registry, services) ||
        ranges_overlap(output_player,sizeof(*output_player),output,sizeof(*output)) ||
        ranges_overlap(output_player,sizeof(*output_player),registry,sizeof(*registry)) ||
        ranges_overlap(output_player,sizeof(*output_player),services,sizeof(*services)) ||
        ranges_overlap(output_player,sizeof(*output_player),registry->entries,
                       registry->entry_count*sizeof(*registry->entries))) return Status::invalid_argument;
    auto cell_aliases_player=[&](const PlayerInfoProjection* player) {
        return ranges_overlap(output_player,sizeof(*output_player),player,sizeof(*player)) ||
            ranges_overlap(output_player,sizeof(*output_player),player->character_level_member,
                           sizeof(*player->character_level_member));
    };
    if (cell_aliases_player(registry->manager_plus_8)) return Status::invalid_argument;
    for (std::uint32_t i=0;i<registry->entry_count;++i)
        if (cell_aliases_player(registry->entries[i])) return Status::invalid_argument;
    Result report{};report.requested_internal_id=internal_id;
    Selection selected;SelectStatus status=SelectStatus::service_failed;
    try {status=select_internal_id(registry,services,internal_id,lookup_flag,
                                  report.service_calls,selected);} catch (...) {}
    report.local_entries_examined=selected.local_examined;
    report.status=status==SelectStatus::selected?Status::complete:
        status==SelectStatus::service_unavailable?Status::service_unavailable:
        status==SelectStatus::no_player?Status::no_player_projection:Status::service_failed;
    if (report.status==Status::complete) {
        if (!selected.player || !selected.player->identity) report.status=Status::no_player_projection;
        else {
            if (ranges_overlap(output,sizeof(*output),selected.player,sizeof(*selected.player)) ||
                ranges_overlap(output,sizeof(*output),selected.player->character_level_member,
                               selected.player->character_level_member?sizeof(*selected.player->character_level_member):0) ||
                cell_aliases_player(selected.player)) return Status::invalid_argument;
            report.route=selected.route;report.player_identity=selected.player->identity;
            *output_player=selected.player;
        }
    }
    *output=report;return report.status;
}

ReconcileStatus reconcile_character_level(const ReconcileState* state,
                                           const ReconcileServices* services,
                                           ReconcileResult* output) {
    if (!state || !services || !output || !state->player ||
        state->player->identity == 0) {
        return ReconcileStatus::invalid_argument;
    }
    if (ranges_overlap(output, sizeof(*output), state, sizeof(*state)) ||
        ranges_overlap(output, sizeof(*output), services, sizeof(*services)) ||
        ranges_overlap(output, sizeof(*output), state->player,
                       sizeof(*state->player)) ||
        ranges_overlap(output, sizeof(*output),
                       state->player->character_level_member,
                       state->player->character_level_member
                           ? sizeof(*state->player->character_level_member) : 0)) {
        return ReconcileStatus::invalid_argument;
    }
    if (state->character_identity == 0 || state->character_properties_identity == 0) {
        ReconcileResult skipped{};
        skipped.status = ReconcileStatus::skipped_unbound_character;
        skipped.level_before = state->player->character_level_member
                                  ? state->player->character_level_member->value : 0;
        skipped.level_after = skipped.level_before;
        *output = skipped;
        return skipped.status;
    }
    if (!state->player->character_level_member) return ReconcileStatus::no_level_member;
    if (!services->get_property_int) return ReconcileStatus::service_unavailable;

    ReconcileResult report{};
    report.status = ReconcileStatus::service_unavailable;
    auto& member = *state->player->character_level_member;
    report.level_before = member.value;
    report.level_after = member.value;

    std::int32_t property_value = 0;
    ++report.property_reads;
    if (services->get_property_int(services->context,
                                   state->character_properties_identity,
                                   19, 0, &property_value) != 0) {
        report.status = ReconcileStatus::service_failed;
        *output = report;
        return report.status;
    }
    report.first_property_value = property_value;
    if (property_value != member.value) {
        if (!services->set_character_level) {
            report.status = ReconcileStatus::service_unavailable;
            *output = report;
            return report.status;
        }
        // The original caller performs a second GetInt instead of forwarding
        // the value used for the comparison.
        ++report.property_reads;
        if (services->get_property_int(services->context,
                                       state->character_properties_identity,
                                       19, 0, &property_value) != 0) {
            report.status = ReconcileStatus::service_failed;
            report.level_after = member.value;
            *output = report;
            return report.status;
        }
        report.setter_argument = property_value;
        ++report.setter_calls;
        if (services->set_character_level(services->context, state->player,
                                          property_value) != 0) {
            report.status = ReconcileStatus::service_failed;
            report.level_after = member.value;
            *output = report;
            return report.status;
        }
    }
    report.level_after = member.value;
    report.status = ReconcileStatus::complete;
    *output = report;
    return report.status;
}

}  // namespace dh2::player_manager_host_level
