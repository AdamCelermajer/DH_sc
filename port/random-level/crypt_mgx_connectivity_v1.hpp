#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::random_level {

enum class DirectionV1 { north, east, south, west };

struct CryptMgxExitV1 {
  std::string object_name;
  std::string link_type;
  DirectionV1 direction = DirectionV1::north;
  float position_x = 0.0f;
  float position_y = 0.0f;
  float position_z = 0.0f;
  std::int32_t cell_x = 0;
  std::int32_t cell_y = 0;
  bool has_raw_position = false;
  bool has_cell_position = false;
};

struct CryptMgxModuleV1 {
  std::string name;
  // Retained for connectivity clients using the original v1 parser API. These
  // XML-double values are not the game's runtime grid precision; use the f32
  // source fields below for exit-to-cell calculations.
  double unit_width = 0.0;
  double unit_height = 0.0;
  std::vector<CryptMgxExitV1> exits;
  float source_unit_width = 0.0f;
  float source_unit_height = 0.0f;
  std::int32_t block_width = 0;
  std::int32_t block_height = 0;
  bool has_grid_geometry = false;
};

struct CryptMgxParseResultV1 {
  bool ok = false;
  CryptMgxModuleV1 module;
  std::string error;
  explicit operator bool() const { return ok; }
};

struct CryptExitAdjacencyV1 {
  std::size_t first_module = 0;
  std::size_t first_exit = 0;
  std::size_t second_module = 0;
  std::size_t second_exit = 0;
};

struct CryptExitPlacementCandidateV1 {
  std::size_t first_module = 0;
  std::size_t first_exit = 0;
  std::size_t second_module = 0;
  std::size_t second_exit = 0;
  // Candidate tile origin relative to the first module origin, as returned by
  // Tile::TrySpawn for this already-compatible pair and a zero first origin.
  std::int32_t second_origin_x = 0;
  std::int32_t second_origin_y = 0;
  float second_origin_z = 0.0f;
};

// Parses the bounded Module/GameObject form used by the original Crypt MGX
// exports. The original native loader keeps GameObjects whose gametype is
// "link" (case-insensitive) and loads their linktype and direction.
CryptMgxParseResultV1 parse_crypt_mgx_v1(std::string_view module_name,
                                         std::string_view xml);

// IDA-derived placement rule: link tokens must compare equal and directions
// must be opposite. This is only topological compatibility; Tile::TrySpawn
// also performs a separate geometric fit check against the candidate MGP.
bool crypt_exits_compatible_v1(const CryptMgxExitV1& first,
                               const CryptMgxExitV1& second) noexcept;

// Enumerates candidate exit pairs for each unordered module-definition pair
// (including two separate instances of the same definition). It does not
// place tiles, apply transforms, check geometry, or backtrack.
std::vector<CryptExitAdjacencyV1> enumerate_crypt_exit_adjacencies_v1(
    const std::vector<CryptMgxModuleV1>& modules);

// Keeps the same link-token/opposite-direction candidate set while attaching
// the geometric origin translation required by Tile::TrySpawn. Candidates are
// omitted when source grid/exit positions are unavailable or the translation
// cannot be represented safely. This does not test map occupancy or recurse.
std::vector<CryptExitPlacementCandidateV1> enumerate_crypt_exit_placement_candidates_v1(
    const std::vector<CryptMgxModuleV1>& modules);

const char* direction_name_v1(DirectionV1 direction) noexcept;

}  // namespace dh2::random_level
