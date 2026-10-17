#pragma once

#include "character_template_factory.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

namespace dh2::data {
struct CharacterTable;
}

namespace dh2::character::template_catalog_v1 {

// Decode Arrays::Charater_Templates pyarray and its names sidecar. The
// CharacterTable and ClassTable inputs must already be loaded from their
// matching source arrays; no row IDs or names are synthesized here.
bool load(const std::uint8_t* records, std::size_t records_size,
          const std::uint8_t* names, std::size_t names_size,
          const std::uint8_t* class_names, std::size_t class_names_size,
          const data::CharacterTable& characters,
          template_factory::Catalog& output, std::string& error) noexcept;

}  // namespace dh2::character::template_catalog_v1
