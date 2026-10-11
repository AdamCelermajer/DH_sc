#pragma once

#include "player_manager_host_level.hpp"
#include "player_offline_registry_v1.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::player_manager_update_v1 {

// Manager-owned words touched by PlayerManager::Update's offline
// _CheckOnlineTransition/ClearLoadingInfo path. This state is embedded in the
// existing NativeHostPlayer owner beside its one registry; it is not another
// PlayerManager or PlayerInfo store.
struct State {
    std::uintptr_t manager_identity{};
    std::int32_t loading_slot_6cc{};
    std::int32_t local_death_count_6c4{};
    // The source constructor writes zero, _AddCharacter increments only
    // after successful construction, and RemoveCharacter decrements. A C++
    // value-initialized zero is not a source receipt.
    std::uintptr_t counted_player_character{};
    std::int32_t global_death_state_714{};
    std::int32_t force_resend_timer{};
    std::uint8_t online_transition_719{};
    std::uint8_t local_online_transition_6ca{};
    std::uint8_t loading_flag_6d0{};
    std::uint8_t quest_sync_flag_6cb{};
    std::uint8_t quest_sync_flag_710{};
    std::uint8_t local_transition_flag_71a{};
    std::uint8_t local_transition_flag_71b{};
    std::uint8_t quest_sync_flag_711{};
    std::uint8_t manager_remove_flag_6c9{};
    std::uint32_t deleted_character_suffix_counter{};
    bool created_character_count_known{};
    bool remove_flag_6c9_known{};
    std::vector<std::uint8_t> quest_sync_buffer_6e0;
    bool busy{};
};

struct Services {
    void* context{};
    // Fresh read of GetOnline()->byte+5. This implementation supports only
    // the source offline branch; online transition/network owners stay outside
    // this adapter and are rejected.
    int (*read_online)(void*, std::uint8_t*){};
    // Character::IsDeleted byte +0x81, resolved by exact identity in the
    // still-active source level. No identity means no lookup.
    int (*read_character_deleted_81)(void*, std::uintptr_t,
                                     std::uint8_t*){};
    // GetLocalPlayer(0,false) and its reached +0x4e5 byte.
    int (*read_local_transition_ready_4e5)(void*, std::uint8_t*){};
    // Same local selection as ClearLoadingInfo. The provider returns the
    // existing manager+8 fallback when the source lookup misses.
    int (*get_local_player)(void*, player_manager_host_level::PlayerInfoProjection**){};
    // Existing offline controller-registration runtime, over this registry.
    int (*check_local_controllers)(void*, player_manager_host_level::PlayerRegistry*,
                                   std::string&){};
    // The source _ManageCharacters and _CheckLocalDeaths bodies have further
    // branch-specific owners. Providers must execute their reached source
    // continuation or fail closed; they must not silently claim a no-op.
    int (*manage_characters)(void*, std::string&){};
    int (*check_local_deaths)(void*, std::string&){};
};

// Cheap preflight for transition setup. Update still validates every owner and
// source identity when called; this only guarantees that no reached offline
// branch is represented by a missing callback.
bool services_ready(const Services&) noexcept;

// Same-owner lifecycle receipts for the supported one-player offline host.
// They mirror source PlayerManager constructor / load / Character operations;
// callers must invoke the Character events only after the complete operation.
bool manager_constructed(State&, std::uintptr_t manager_identity) noexcept;
bool load_process_started(State&, std::uintptr_t manager_identity) noexcept;
bool application_load_completed(State&, std::uintptr_t manager_identity) noexcept;
bool player_character_added(State&, std::uintptr_t manager_identity,
                            std::uintptr_t character_identity) noexcept;
bool player_character_removed(State&, std::uintptr_t manager_identity,
                              std::uintptr_t character_identity) noexcept;

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    invalid_manager,
    invalid_registry,
    service_unavailable,
    service_failed,
    online_unsupported,
    record_unavailable,
    member_unavailable,
    reentrant,
};

struct Result {
    Status status{Status::complete};
    std::uint32_t entries_examined{};
    std::uint32_t stale_character_links_cleared{};
    std::uint32_t member_writes{};
    std::uint32_t callback_count{};
};

// One synchronous PlayerManager::Update pass over the currently supported
// offline lifecycle. Every mutation is made through the existing manager and
// PlayerInfo owners. The source transition runs after Level state is cleared;
// this function does not own or reset the Level.
Status update_offline(State&, player_manager_host_level::PlayerRegistry&,
                      player_offline_registry_v1::Owner&, const Services&,
                      std::uintptr_t expected_manager,
                      std::uintptr_t supplied_manager, Result*,
                      std::string& error);

// _ManageCharacters repeats PlayerManager::ClearLoadingInfo immediately
// before RemoveAllCharacters on the offline +0x6c9==0/+0x6c4>0 branch.
// This is separate from Update's earlier _CheckOnlineTransition clear and
// intentionally touches the same manager state and local PlayerInfo record.
Status clear_loading_info_before_remove_all(
    State&, player_info_record_v1::Record&, Result*);

} // namespace dh2::player_manager_update_v1
