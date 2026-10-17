#include "character_ai_association.hpp"
#include <cstddef>
namespace dh2::character_ai_association {
Status associate(character_ai_initialization::State* state,std::uintptr_t character) {
    const auto at=reinterpret_cast<std::uintptr_t>(state);
    if(!state || at%alignof(character_ai_initialization::State) ||
       at>UINTPTR_MAX-sizeof(*state) || !state->identity || !character)
        return Status::invalid_argument;
    state->owner_04=character;
    return Status::complete;
}
}
