#pragma once

#include "player_save_section_writers_v1.hpp"
#include "properties.hpp"

namespace dh2::data::player_property_save_writer_v1 {

// Implements PlayerSavegame::__SaveProperties over the live resolved
// Character property sheet. The caller must pass the view belonging to
// save.character(); PropertyView itself has no owner identity to cross-check.
// Values are serialized in place; no additional property store is retained.
player_save_section_writers_v1::Status write_properties_v1(
    const PlayerSavegameV1& save, const PropertyView& properties,
    const player_save_section_writers_v1::WriteServicesV1& stream,
    std::string& error);

}  // namespace dh2::data::player_property_save_writer_v1
