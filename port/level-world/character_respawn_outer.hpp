#pragma once

#include "character_group_respawn.hpp"

#include <cstdint>

namespace dh2::character_respawn {

constexpr std::int32_t respawn_time_property_id = 11;

// Logical facts read by Character::GetRespawnDelay/CanRespawn. The suppression
// byte is Character+0x1481, set by Character::InitSpawned. Keep its raw byte:
// the original branches on zero/nonzero, not on a normalized bool.
struct Facts {
    std::int32_t character_state_id;
    std::uint8_t init_spawned_suppression;
    std::uint8_t reserved[3];
};

struct Services {
    void* property_context;
    // Resolve the live CharacterProperties field, returning the raw signed
    // property value. Character::CanRespawn/GetRespawnDelay request property
    // ID 11; the caller owns the actual Character property view.
    std::int32_t (*read_raw_property)(void*, std::int32_t property_id,
                                      std::int32_t* raw_value);
    // Reuse the already reconstructed GroupInfo::CanRespawn leaf verbatim.
    dh2::character_group::Services group;
};

struct DelayResult {
    std::int32_t raw_respawn_time;
    std::int32_t delay_ms;
    std::uint32_t property_read;
};

struct PredicateResult {
    std::uint32_t can_respawn;
    std::int32_t raw_respawn_time;
    std::uint32_t property_read;
    std::uint32_t group_predicate_called;
    std::uint32_t group_members_queried;
    std::int32_t group_status_after;
    std::uint32_t group_status_observed;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    property_read_failed = 2,
    group_input_invalid = 3,
    group_member_query_failed = 4,
};

// Mirrors GetRespawnDelay: suppression returns zero before property access;
// otherwise the signed property is arithmetically shifted right by 8 and
// multiplied by 1000 with ARM32 low-32-bit MUL wrap semantics.
Status get_delay(const Facts* facts, const Services* services,
                 DelayResult* result);

// Mirrors Character::CanRespawn outer gates and delegates the optional group
// branch to the existing GroupInfo source kernel. A null group returns true
// after the byte/property gates. A rejected gate never reads the group.
// Facts, Services, outer output, and group storage must not overlap. A consumed
// member range must not overlap these outer arguments or wrap the address space.
// Consumed group/member storage and invoked service contexts must remain live
// through return.
// Port errors preserve the outer output; callback effects are not rolled back.
Status can_respawn(const Facts* facts, const Services* services,
                   dh2::character_group::GroupState* optional_group,
                   PredicateResult* result);

}  // namespace dh2::character_respawn
