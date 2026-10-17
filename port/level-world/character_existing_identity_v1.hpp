#pragma once

#include "character_constructor_owner_v1.hpp"

#include <array>
#include <cstdint>
#include <string>

namespace dh2::character_existing_identity_v1 {

namespace ctor = character_constructor_owner_v1;
using Identity = ctor::Identity;

// Constructor-owned state for a Character whose stable GameObject identity
// and manager/CharacterList projections are owned elsewhere. This does not
// allocate a factory Record or a second roster entry. Component construction
// and associations are supplied by the existing canonical per-actor owners.
struct Owner {
    Identity identity{};
    ctor::Owner constructor{};
    std::array<std::uint32_t, 3> quick_save_position_1468{};
    std::array<std::uint32_t, 3> revive_position_1474{};
    bool constructor_save_vectors_ready{};
    bool source_component_adoption_complete{};
};

enum class Status : std::uint8_t {
    complete, invalid_argument, service_unavailable, constructor_failed,
};

struct Result {
    ctor::Result constructor{};
    bool constructor_save_vectors_ready{};
    bool source_component_adoption_complete{};
};

// Adopt source Character components in the recovered constructor order on the
// caller-owned identity. This is a port component-adoption receipt, not proof
// of full raw Character constructor parity. Character C2 initializes both
// serialized Point3D fields to zero; this owner retains those exact raw words
// beside the same identity.
Status construct(Owner*, Identity, const ctor::Services*, Result*,
                 std::string& error);

} // namespace dh2::character_existing_identity_v1
