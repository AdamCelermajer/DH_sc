#pragma once
#include "../../../original_scene.hpp"
namespace dh::foundation::frontend {
// Bind embedded original COMMON selected pass and unlit/lit preamble. Does
// not guess lighting parameters for other shader families or add a tint.
bool apply_preview_scene_materials(const std::vector<std::uint8_t>&,
    OriginalScene&,std::string&);
}
