#pragma once

#include "../../asset_catalog.hpp"
#include "../../original_character.hpp"

namespace dh::foundation::doors {

// Replays the source Door::InitPost -> VisualObject::ApplyMeshBox producer on
// the actual retained object Scene. Marker geometry uses the verified
// _colbox_ group path; the no-marker branch uses the existing source
// CalcMeshBox static/skinned providers. It then applies source GameObject
// ApplyMeshBox XY padding and owner placement. Output is an absolute game-unit
// box. Render/indexed bounds are never consulted.
bool source_door_apply_mesh_box_v1(const AssetCatalog& assets,
                                   const CharacterVisual& visual,
                                   const float position[3],
                                   const float rotation_degrees[3],
                                   const float scale[3],
                                   float output_absolute6[6],
                                   std::string& error);

} // namespace dh::foundation::doors
