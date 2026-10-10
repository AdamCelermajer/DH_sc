#pragma once

#include "character_state.hpp"
#include <filesystem>
#include <string>

namespace dh::foundation {

// Saves are bounded and independent of the original engine runtime. Writers
// emit CharacterState schema v2. Readers accept v1 and promote it in memory;
// fields absent from v1 remain default/unknown until an explicit save writes
// v2. Failure leaves the destination file (save) or supplied state (load)
// unchanged.
bool save_character(const std::filesystem::path& path,
                    const CharacterState& state, std::string& error);
bool load_character(const std::filesystem::path& path,
                    CharacterState& state, std::string& error);

} // namespace dh::foundation
