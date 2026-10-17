#pragma once

#include "player_manager_host_level.hpp"
#include <cstdint>

namespace dh2::player_locality_v1 {
using PlayerInfo = player_manager_host_level::PlayerInfoProjection;
using Registry = player_manager_host_level::PlayerRegistry;

// Borrow the canonical CMatching base identity and its actual byte +0xc.
// This is a query projection, not a second matching/network owner.
struct Matching {
    std::uintptr_t identity = 0;
    const std::uint8_t* active_c = nullptr;
};
struct MatchingLocalFields {
    std::uint8_t active_c = 0;
    std::int32_t member_3638 = -1;
    std::int32_t server_member_363c = -2;
};
// Only these reached scalar stores are projected. Full CMatching/Local
// construction and Reset's NetStruct/mutex/list effects are not implemented.
void construct_matching_local_fields(MatchingLocalFields&) noexcept;
void reset_matching_local_ids(MatchingLocalFields&) noexcept;
std::int32_t local_member_id(const MatchingLocalFields&) noexcept;
std::int32_t local_server_member_id(const MatchingLocalFields&) noexcept;

enum class Operation : std::uint32_t {
    none, online, game_state_online, acquire_matching, matching_in_room,
    acquire_net_manager, net_initialized, net_ids, internal_id_player,
    net_player_info, character_660, player_virtual_is_local, member_1a0,
    matching_member_id, matching_server_member_id
};
// All callbacks return zero after delivery. Missing reached callbacks and
// failures stop immediately, preserving their completed effects. Callbacks
// read the same canonical owners; their output is never cached across source
// reads. Online-vector spans are borrowed for one read, not retained.
struct Services {
    void* context = nullptr;
    std::int32_t (*online)(void*, std::uint8_t*) = nullptr;
    std::int32_t (*game_state_online)(void*, std::uint8_t*) = nullptr;
    std::int32_t (*acquire_matching)(void*, Matching**) = nullptr;
    std::int32_t (*matching_in_room)(void*, Matching*, std::int32_t*) = nullptr;
    std::int32_t (*acquire_net_manager)(void*, std::uintptr_t*) = nullptr;
    std::int32_t (*net_initialized)(void*, std::uintptr_t, std::int32_t*) = nullptr;
    std::int32_t (*net_ids)(void*, const Registry*, const std::int32_t**,
                            std::uint32_t*) = nullptr;
    std::int32_t (*internal_id_player)(void*, const Registry*, std::int32_t,
                                      std::uint32_t, PlayerInfo**) = nullptr;
    std::int32_t (*net_player_info)(void*, const Registry*, std::int32_t,
                                   std::uint32_t, PlayerInfo**) = nullptr;
    std::int32_t (*character_660)(void*, PlayerInfo*, std::uintptr_t*) = nullptr;
    std::int32_t (*player_virtual_is_local)(void*, PlayerInfo*, std::int32_t*) = nullptr;
    std::int32_t (*member_1a0)(void*, PlayerInfo*, std::int32_t*) = nullptr;
    std::int32_t (*matching_member_id)(void*, Matching*, std::int32_t*) = nullptr;
    std::int32_t (*matching_server_member_id)(void*, Matching*, std::int32_t*) = nullptr;
};
enum class Status : std::uint32_t {
    complete, invalid_argument, invalid_registry, service_unavailable,
    service_failed, missing_projection
};
enum class Route : std::uint32_t {none, registered_player, manager_plus_8, network_player};
struct Result {
    Status status = Status::complete;
    Route route = Route::none;
    Operation last_operation = Operation::none;
    std::uint32_t service_calls = 0, entries_examined = 0;
    PlayerInfo* player = nullptr;
    // True only for an actual hit in the canonical manager registry. A local
    // fallback result is not a registered Character association. Network
    // lookup has its own route and does not imply local-tree registration.
    bool registered = false;
    std::int32_t value = 0;
};
Status get_player_by_character(const Registry*, const Services*,
                              std::uintptr_t character, std::uint32_t lookup_flag,
                              Result*);
// IsLocalPlayer(null) is the source immediate false branch. Otherwise dispatch
// the selected PlayerInfo virtual +0x50; it must not be replaced by offline=true.
Status is_local_player(const Registry*, const Services*, std::uintptr_t character,
                       Result*);
// Source CNetPlayerInfo::IsLocal and CMatching::IsServer. A canonical inherited
// PlayerInfo virtual provider can synchronously call the former. Generic
// Matching virtual providers choose the actual selected implementation; local
// leaves above borrow its genuine fields. IsServer reads member twice freshly.
Status cnet_player_is_local(PlayerInfo*, const Services*, Result*);
Status matching_is_server(Matching*, const Services*, Result*);

enum class MatchingKind : std::uint32_t {local, bluetooth, gl_live};
struct MatchingState {
    Matching** singleton = nullptr;
    std::int32_t* provider = nullptr;
};
struct MatchingFactory {
    void* context = nullptr;
    // Original sizes/modes are passed intact. Constructor delivery is mandatory;
    // a native bounded query owner must declare its omitted NetStruct services.
    std::int32_t (*construct)(void*, MatchingKind, std::uint32_t bytes,
                              std::uint32_t mode, Matching**) = nullptr;
};
struct MatchingResult {
    Status status = Status::complete;
    std::uint32_t constructor_calls = 0;
    Matching* matching = nullptr;
};
// Borrow actual singleton/provider storage. Existing singleton returns as-is.
// Provider0 writes1 before construction. Fresh provider reads after local/BT/
// GL(false) constructors preserve source chaining and failure prefixes.
Status get_matching(const MatchingState*, const MatchingFactory*, MatchingResult*);
} // namespace dh2::player_locality_v1
