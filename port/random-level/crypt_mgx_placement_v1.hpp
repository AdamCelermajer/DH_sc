#pragma once

#include "crypt_mgx_connectivity_v1.hpp"

#include <cstdint>

namespace dh2::random_level {

struct CryptMgxGridGeometryV1 {
  float unit_width = 0.0f;
  float unit_height = 0.0f;
  std::int32_t block_width = 0;
  std::int32_t block_height = 0;
};

struct CryptMgxExitCellV1 {
  std::int32_t x = 0;
  std::int32_t y = 0;
  float elevation = 0.0f;
};

struct CryptMgxTileOriginV1 {
  std::int32_t x = 0;
  std::int32_t y = 0;
  float elevation = 0.0f;
};

struct CryptMgxTilePlacementV1 {
  std::int32_t x = 0;
  std::int32_t y = 0;
  float elevation = 0.0f;
};

// Mirrors Exit::GetBlockUnitPosition and the adjacent Exit::LoadFromXml parse.
// The x/y cell result is clamped at zero but has no upper clamp; z is carried
// through as the source's third float coordinate.
bool crypt_mgx_exit_position_to_cell_v1(const CryptMgxGridGeometryV1& geometry,
                                        float position_x,
                                        float position_y,
                                        float position_z,
                                        CryptMgxExitCellV1& output) noexcept;

// Mirrors Tile::TrySpawn after the source has selected an already-compatible
// exit pair. candidate_direction is the new block's exit facing; the source
// offsets the candidate origin by the opposite facing before subtracting the
// candidate exit's local cell position. A null anchor exit omits that offset.
bool crypt_mgx_try_spawn_placement_v1(const CryptMgxTileOriginV1& anchor_tile,
                                      const CryptMgxExitCellV1* anchor_exit,
                                      const CryptMgxExitCellV1& candidate_exit,
                                      DirectionV1 candidate_direction,
                                      CryptMgxTilePlacementV1& output) noexcept;

using CryptMgxCellOccupiedV1 = bool (*)(const void* context,
                                        std::int32_t x,
                                        std::int32_t y);

// Mirrors MgxBlock::FitsInMap over the source's dynamically expanding
// Array2d. The callback must report empty for cells not currently allocated;
// the source Array2d grows those cells as zero-filled slots on access.
bool crypt_mgx_footprint_fits_v1(std::int32_t origin_x,
                                 std::int32_t origin_y,
                                 std::int32_t block_width,
                                 std::int32_t block_height,
                                 CryptMgxCellOccupiedV1 is_occupied,
                                 const void* context) noexcept;

}  // namespace dh2::random_level
