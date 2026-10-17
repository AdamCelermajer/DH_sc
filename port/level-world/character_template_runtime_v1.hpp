#pragma once

#include "character_template_factory.hpp"
#include "../game-data/data.hpp"

extern "C" {
#include "../random/random.h"
}

#include <cstdint>
#include <string>

namespace dh2::character_template_runtime_v1 {

enum class Status : std::uint8_t {
    selected,
    cached,
    no_alternatives,
    invalid_argument,
    invalid_source,
    template_not_found,
    ambiguous_template,
    incompatible_alternatives,
    random_unavailable,
    selection_failed,
};

struct Bindings {
    std::int16_t* property_cache = nullptr; // source Character +0x13c8
    std::int16_t* template_cache = nullptr; // source Character +0x13ca
    const char* template_data_class = nullptr;
    const char* template_name = nullptr;
    const character::template_factory::Catalog* catalog = nullptr;
    const data::CharacterTable* characters = nullptr;
    const data::Dictionary* models = nullptr;
    dh2_random_state* random = nullptr; // borrow process_state(); ordinary stream only
};

struct Result {
    std::int32_t property_id = -1;
    std::int32_t template_id = -1;
    std::int32_t selected_slot = -1;
    std::uint32_t random_draws = 0;
};

// Resolves an uncached non-player char_template route. The source draw selects
// one ordered alternative before the actor factory loads that Character's own
// ModelFile and AnimTable; alternatives need not share either resource. The
// selected row fields must be consumed from the returned property_id after this
// call. A successful property cache prevents all future draws.
Status resolve(const Bindings&, Result*, std::string& error) noexcept;

const char* status_name(Status) noexcept;

} // namespace dh2::character_template_runtime_v1
