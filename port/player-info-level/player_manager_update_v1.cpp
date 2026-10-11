#include "player_manager_update_v1.hpp"

#include "netstruct_members_v1.hpp"
#include "player_info_record_v1.hpp"

namespace dh2::player_manager_update_v1 {
namespace {
namespace h = player_manager_host_level;
namespace p = player_info_record_v1;
namespace n = netstruct_members_v1;
using Record = player_offline_registry_v1::Owner;

struct Busy {
    bool& value;
    explicit Busy(bool& target) : value(target) { value = true; }
    ~Busy() { value = false; }
};

Status set_bool(p::Record& record, std::uint32_t offset, std::uint8_t value) {
    if (!record.constructed || record.busy || !record.base.serial)
        return Status::member_unavailable;
    auto* destination = record.at(offset);
    if (!destination || !destination->constructed || destination->busy ||
        destination->kind != n::Kind::boolean || destination->header.size_bits != 1 ||
        destination->serial != record.base.serial)
        return Status::member_unavailable;

    n::Member temporary;
    const auto constructed = n::construct_scalar(temporary, n::Kind::boolean, 1,
                                                  value, record.base.serial);
    if (constructed != n::Status::complete) return Status::member_unavailable;
    return n::assign(*destination, temporary) == n::Status::complete
        ? Status::complete : Status::member_unavailable;
}

Status clear_loading_info(p::Record& record, Result& result) {
    // PlayerInfo::ClearLoadingInfo calls IsActive first. In the supported
    // offline branch IsActive is unconditionally true.
    for (const auto offset : {0x4c8u, 0x4e8u, 0x508u, 0x528u}) {
        const auto status = set_bool(record, offset, 0);
        if (status != Status::complete) return status;
        ++result.member_writes;
    }
    const auto timer = p::set_character_scalar(record, p::character_death_timer,
                                                -1, 0);
    if (timer != n::Status::complete) return Status::member_unavailable;
    ++result.member_writes;
    return Status::complete;
}

bool fail(Result* output, Status status, const char* message, std::string& error) {
    if (output) output->status = status;
    // Keep a provider's own reason after the wrapper's step name.
    error = error.empty() ? std::string(message) : std::string(message) + ": " + error;
    return false;
}
}

bool services_ready(const Services& services) noexcept {
    return services.read_online &&
           services.read_character_deleted_81 &&
           services.read_local_transition_ready_4e5 &&
           services.get_local_player &&
           services.check_local_controllers &&
           services.manage_characters &&
           services.check_local_deaths;
}

bool manager_constructed(State& state, std::uintptr_t manager_identity) noexcept {
    if (!manager_identity || state.busy ||
        (state.manager_identity && state.manager_identity != manager_identity) ||
        state.created_character_count_known || state.remove_flag_6c9_known)
        return false;
    state.manager_identity = manager_identity;
    state.local_death_count_6c4 = 0;
    state.manager_remove_flag_6c9 = 0;
    state.counted_player_character = 0;
    state.deleted_character_suffix_counter = 0;
    state.created_character_count_known = true;
    state.remove_flag_6c9_known = true;
    return true;
}

bool load_process_started(State& state, std::uintptr_t manager_identity) noexcept {
    if (!manager_identity || state.manager_identity != manager_identity || state.busy ||
        !state.created_character_count_known)
        return false;
    state.manager_remove_flag_6c9 = 1;
    state.remove_flag_6c9_known = true;
    return true;
}

bool application_load_completed(State& state,
                                std::uintptr_t manager_identity) noexcept {
    if (!manager_identity || state.manager_identity != manager_identity || state.busy ||
        !state.created_character_count_known)
        return false;
    state.manager_remove_flag_6c9 = 0;
    state.remove_flag_6c9_known = true;
    return true;
}

bool player_character_added(State& state, std::uintptr_t manager_identity,
                            std::uintptr_t character_identity) noexcept {
    if (!manager_identity || !character_identity ||
        state.manager_identity != manager_identity || state.busy ||
        !state.created_character_count_known)
        return false;
    if (state.counted_player_character == character_identity)
        return true;
    if (state.counted_player_character || state.local_death_count_6c4 != 0)
        return false;
    state.counted_player_character = character_identity;
    ++state.local_death_count_6c4;
    return true;
}

bool player_character_removed(State& state, std::uintptr_t manager_identity,
                              std::uintptr_t character_identity) noexcept {
    if (!manager_identity || !character_identity ||
        state.manager_identity != manager_identity ||
        !state.created_character_count_known ||
        state.counted_player_character != character_identity ||
        state.local_death_count_6c4 != 1)
        return false;
    state.counted_player_character = 0;
    state.local_death_count_6c4 = 0;
    return true;
}

Status clear_loading_info_before_remove_all(State& state,
        player_info_record_v1::Record& player, Result* output) {
    Result local{};
    auto& result = output ? *output : local;
    result = {};
    if (!state.manager_identity || !state.created_character_count_known ||
        !state.remove_flag_6c9_known || state.manager_remove_flag_6c9 != 0 ||
        state.local_death_count_6c4 <= 0 || !player.constructed || player.busy)
        return result.status = Status::invalid_manager;

    // Exact repeated PlayerManager::ClearLoadingInfo manager writes. The
    // online transition bytes +0x719/+0x6ca and quest-sync buffer belong to
    // the preceding _CheckOnlineTransition path and are not rewritten here.
    state.loading_slot_6cc = -1;
    state.loading_flag_6d0 = 0;
    state.quest_sync_flag_6cb = 0;
    state.quest_sync_flag_710 = 0;
    state.local_transition_flag_71a = 0;
    state.global_death_state_714 = 0;
    state.quest_sync_flag_711 = 0;
    const auto status = clear_loading_info(player, result);
    if (status != Status::complete)
        return result.status = status;
    result.status = Status::complete;
    return result.status;
}

Status update_offline(State& state, h::PlayerRegistry& registry,
                      player_offline_registry_v1::Owner& records,
                      const Services& services, std::uintptr_t expected_manager,
                      std::uintptr_t supplied_manager, Result* output,
                      std::string& error) {
    Result local{};
    auto& result = output ? *output : local;
    result = {};
    if (!expected_manager || !supplied_manager)
        return fail(output, Status::invalid_argument,
                    "PlayerManager::Update identity is null", error), result.status;
    if (state.manager_identity != expected_manager ||
        registry.manager_identity != expected_manager ||
        supplied_manager != expected_manager)
        return fail(output, Status::invalid_manager,
                    "PlayerManager::Update identity is not the canonical host registry", error), result.status;
    if (!state.created_character_count_known || !state.remove_flag_6c9_known)
        return fail(output, Status::service_unavailable,
                    "PlayerManager::Update source +0x6c4/+0x6c9 lifecycle facts are not established", error), result.status;
    if (registry.entry_count && !registry.entries)
        return fail(output, Status::invalid_registry,
                    "PlayerManager::Update registry entries are unavailable", error), result.status;
    if (state.busy)
        return fail(output, Status::reentrant,
                    "PlayerManager::Update reentered its canonical owner", error), result.status;
    if (!services.read_online)
        return fail(output, Status::service_unavailable,
                    "PlayerManager::Update offline mode provider is unavailable", error), result.status;

    Busy busy(state.busy);
    std::uint8_t online = 0;
    ++result.callback_count;
    try {
        if (services.read_online(services.context, &online))
            return fail(output, Status::service_failed,
                        "PlayerManager::Update online mode read failed", error), result.status;
    } catch (...) {
        return fail(output, Status::service_failed,
                    "PlayerManager::Update online mode read threw", error), result.status;
    }
    if (online)
        return fail(output, Status::online_unsupported,
                    "PlayerManager::Update has no canonical online manager owner", error), result.status;

    // The first source loop clears only the same PlayerInfo's +0x660 when its
    // exact Character's +0x81 byte is set.
    for (std::uint32_t i = 0; i < registry.entry_count; ++i) {
        auto* projection = registry.entries[i];
        if (!projection)
            return fail(output, Status::invalid_registry,
                        "PlayerManager::Update found a null registry projection", error), result.status;
        ++result.entries_examined;
        auto* record = records.record(projection);
        if (!record || !record->constructed)
            return fail(output, Status::record_unavailable,
                        "PlayerManager::Update cannot resolve the canonical PlayerInfo record", error), result.status;
        const auto character = record->character_660;
        if (!character) continue;
        if (!services.read_character_deleted_81)
            return fail(output, Status::service_unavailable,
                        "PlayerManager::Update Character +0x81 provider is unavailable", error), result.status;
        std::uint8_t deleted = 0;
        ++result.callback_count;
        try {
            if (services.read_character_deleted_81(services.context, character, &deleted))
                return fail(output, Status::service_failed,
                            "PlayerManager::Update Character +0x81 read failed", error), result.status;
        } catch (...) {
            return fail(output, Status::service_failed,
                        "PlayerManager::Update Character +0x81 read threw", error), result.status;
        }
        if (deleted) {
            record->character_660 = 0;
            ++result.stale_character_links_cleared;
        }
    }

    // PlayerManager::Update's pre-transition +0x71b gate. The canonical host
    // currently starts clear; query the reached local byte only when set.
    if (state.local_transition_flag_71b) {
        if (!services.read_local_transition_ready_4e5)
            return fail(output, Status::service_unavailable,
                        "PlayerManager +0x71b local-ready provider is unavailable", error), result.status;
        std::uint8_t ready = 0;
        ++result.callback_count;
        try {
            if (services.read_local_transition_ready_4e5(services.context, &ready))
                return fail(output, Status::service_failed,
                            "PlayerManager +0x71b local-ready read failed", error), result.status;
        } catch (...) {
            return fail(output, Status::service_failed,
                        "PlayerManager +0x71b local-ready read threw", error), result.status;
        }
        if (ready) state.local_transition_flag_71b = 0;
    }

    // _CheckOnlineTransition's offline tail clears these manager words, then
    // ClearLoadingInfo updates exactly the local/fallback PlayerInfo, then
    // ClearQuestSyncBuffer clears its one manager-owned byte buffer.
    state.online_transition_719 = 0;
    state.local_online_transition_6ca = 0;
    state.loading_slot_6cc = -1;
    state.loading_flag_6d0 = 0;
    state.quest_sync_flag_6cb = 0;
    state.quest_sync_flag_710 = 0;
    state.local_transition_flag_71a = 0;
    if (!services.get_local_player)
        return fail(output, Status::service_unavailable,
                    "PlayerManager::ClearLoadingInfo local PlayerInfo provider is unavailable", error), result.status;
    h::PlayerInfoProjection* local_player = nullptr;
    ++result.callback_count;
    try {
        if (services.get_local_player(services.context, &local_player) || !local_player)
            return fail(output, Status::service_failed,
                        "PlayerManager::ClearLoadingInfo local PlayerInfo lookup failed", error), result.status;
    } catch (...) {
        return fail(output, Status::service_failed,
                    "PlayerManager::ClearLoadingInfo local PlayerInfo lookup threw", error), result.status;
    }
    auto* local_record = records.record(local_player);
    if (!local_record || !local_record->constructed)
        return fail(output, Status::record_unavailable,
                    "PlayerManager::ClearLoadingInfo cannot resolve the canonical local record", error), result.status;
    const auto clear_status = clear_loading_info(*local_record, result);
    if (clear_status != Status::complete)
        return fail(output, clear_status,
                    "PlayerManager::ClearLoadingInfo member update failed", error), result.status;
    state.global_death_state_714 = 0;
    state.quest_sync_flag_711 = 0;
    state.quest_sync_buffer_6e0.clear();

    // Local controllers are the existing offline registration kernel; remote
    // controllers and global deaths are skipped by their GetOnline guards.
    if (!services.check_local_controllers)
        return fail(output, Status::service_unavailable,
                    "PlayerManager::_CheckLocalControllers provider is unavailable", error), result.status;
    ++result.callback_count;
    try {
        if (services.check_local_controllers(services.context, &registry, error))
            return fail(output, Status::service_failed,
                        "PlayerManager::_CheckLocalControllers failed", error), result.status;
    } catch (...) {
        return fail(output, Status::service_failed,
                    "PlayerManager::_CheckLocalControllers threw", error), result.status;
    }

    if (!services.manage_characters)
        return fail(output, Status::service_unavailable,
                    "PlayerManager::_ManageCharacters provider is unavailable", error), result.status;
    ++result.callback_count;
    try {
        if (services.manage_characters(services.context, error))
            return fail(output, Status::service_failed,
                        "PlayerManager::_ManageCharacters failed", error), result.status;
    } catch (...) {
        return fail(output, Status::service_failed,
                    "PlayerManager::_ManageCharacters threw", error), result.status;
    }

    if (!services.check_local_deaths)
        return fail(output, Status::service_unavailable,
                    "PlayerManager::_CheckLocalDeaths provider is unavailable", error), result.status;
    ++result.callback_count;
    try {
        if (services.check_local_deaths(services.context, error))
            return fail(output, Status::service_failed,
                        "PlayerManager::_CheckLocalDeaths failed", error), result.status;
    } catch (...) {
        return fail(output, Status::service_failed,
                    "PlayerManager::_CheckLocalDeaths threw", error), result.status;
    }

    // PlayerStatManager::Update is a source-verified BX LR. Clear exact error
    // residue and report the completed supported offline pass.
    error.clear();
    result.status = Status::complete;
    return Status::complete;
}

} // namespace dh2::player_manager_update_v1
