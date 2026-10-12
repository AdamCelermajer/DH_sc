#pragma once

#include "crypt_generated_mvp_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::world {

// Renderer-independent projection of original MVP AnimatedDecor GameObjects
// for every static MLX level. Every candidate keeps its authored activate and
// deactivate condition text; this projection does not evaluate conditions or
// execute scripts.
struct SourceAnimatedDecorV1 {
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  std::string name;
  std::string xrefobject;
  std::string dae_path;
  std::string source_path;
  std::string start_animation;
  std::string activate_condition;
  std::string deactivate_condition;
  float world_transform[9]{};  // position[3], rotation_degrees[3], scale[3]
};

// Imports the level XML and one ordered MVP per Module (the same contract as
// compile_generated_crypt_animated_decor_v1), then returns the MVP GameObjects
// whose gametype is exactly "AnimatedDecor" in source order. Rows with an
// unsupported identity, BDAE or placement are counted in deferred_count and
// skipped; they are not rendered. A missing startanim is returned empty
// (AnimatedDecor::InitPost substitutes "idle").
bool compile_source_animated_decor_v1(
    const std::uint8_t* level_xml, std::size_t level_size,
    const char* level_name, const char* level_source_path,
    const GeneratedCryptMvpFileV1* mvps, std::size_t mvp_count,
    std::vector<SourceAnimatedDecorV1>& output, std::size_t& deferred_count,
    std::string& error);

}  // namespace dh2::world
