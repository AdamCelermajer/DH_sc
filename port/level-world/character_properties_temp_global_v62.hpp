#pragma once
#include "../game-data/properties.hpp"
#include <memory>
namespace dh2::character {
// One original CharProperties::s_temp9a2c78 payload for the whole process.
// ELF BSS is zero; global init3df5c4 writes only the legacy Structs header.
// This is neither CharacterTable defaults nor a per-player scratch sheet.
const std::shared_ptr<data::PropertySheet>& character_properties_temp_global_v62();
}
