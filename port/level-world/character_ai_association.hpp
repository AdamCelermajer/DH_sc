#pragma once
#include "character_ai_initialization.hpp"
namespace dh2::character_ai_association {
enum class Status : std::int32_t {complete,invalid_argument};
// Original CharAI::SetCharacter148B valid nonnull gameplay branch. The source
// Character constructors invoke this after embedded AI construction. Only +4
// changes; no AIS, alive, queue, zoning or script state is initialized here.
// Null source arguments enter assertion/diagnostic paths. The native API rejects
// those before effects and does not claim to reproduce an invalid dereference.
Status associate(character_ai_initialization::State*,std::uintptr_t character);
}
