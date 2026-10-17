#include "crypt_mgx_placement_v1.hpp"

#include <cmath>
#include <limits>

namespace dh2::random_level {
namespace {

bool to_source_int(float value, std::int32_t& output) noexcept {
  if (!std::isfinite(value) ||
      static_cast<double>(value) <
          static_cast<double>(std::numeric_limits<std::int32_t>::min()) ||
      static_cast<double>(value) >
          static_cast<double>(std::numeric_limits<std::int32_t>::max())) {
    return false;
  }
  output = static_cast<std::int32_t>(value);
  return true;
}

bool checked_add(std::int32_t first,
                 std::int32_t second,
                 std::int64_t& output) noexcept {
  output = static_cast<std::int64_t>(first) + static_cast<std::int64_t>(second);
  return output >= std::numeric_limits<std::int32_t>::min() &&
         output <= std::numeric_limits<std::int32_t>::max();
}

bool checked_sub(std::int64_t first,
                 std::int32_t second,
                 std::int64_t& output) noexcept {
  output = first - static_cast<std::int64_t>(second);
  return output >= std::numeric_limits<std::int32_t>::min() &&
         output <= std::numeric_limits<std::int32_t>::max();
}

}  // namespace

bool crypt_mgx_exit_position_to_cell_v1(const CryptMgxGridGeometryV1& geometry,
                                        float position_x,
                                        float position_y,
                                        float position_z,
                                        CryptMgxExitCellV1& output) noexcept {
  if (!std::isfinite(position_x) || !std::isfinite(position_y) ||
      !std::isfinite(position_z) ||
      !std::isfinite(geometry.unit_width) ||
      !std::isfinite(geometry.unit_height) || geometry.unit_width <= 0.0f ||
      geometry.unit_height <= 0.0f || geometry.block_width ==
          std::numeric_limits<std::int32_t>::min()) {
    return false;
  }

  // IDA: Exit::GetBlockUnitPosition (0x489ddc). Preserve the original float
  // operation order around ceilf and clamp only the resulting cell indices.
  const float negative_half_width =
      (static_cast<float>(-geometry.block_width) * geometry.unit_width) * 0.5f;
  const float half_height =
      (static_cast<float>(geometry.block_height) * geometry.unit_height) * 0.5f;
  const float x_cell_float =
      std::ceil((position_x - negative_half_width) / geometry.unit_width) - 1.0f;
  const float y_cell_float =
      std::ceil((half_height - position_y) / geometry.unit_height) - 1.0f;

  std::int32_t x_cell = 0;
  std::int32_t y_cell = 0;
  if (!to_source_int(x_cell_float, x_cell) ||
      !to_source_int(y_cell_float, y_cell)) {
    return false;
  }

  output.x = x_cell < 0 ? 0 : x_cell;
  output.y = y_cell < 0 ? 0 : y_cell;
  // Exit::LoadFromXml parses the same position string as Point3D<float> and
  // stores its third component unchanged as the exit elevation.
  output.elevation = position_z;
  return true;
}

bool crypt_mgx_try_spawn_placement_v1(const CryptMgxTileOriginV1& anchor_tile,
                                      const CryptMgxExitCellV1* anchor_exit,
                                      const CryptMgxExitCellV1& candidate_exit,
                                      DirectionV1 candidate_direction,
                                      CryptMgxTilePlacementV1& output) noexcept {
  std::int32_t opposite_dx = 0;
  std::int32_t opposite_dy = 0;
  switch (candidate_direction) {
    // Direction::sDirections order is north, east, south, west. These are the
    // (dx,dy) values of DIRECTION_OPPOSITES[candidate_direction].
    case DirectionV1::north: opposite_dy = 1; break;
    case DirectionV1::east: opposite_dx = -1; break;
    case DirectionV1::south: opposite_dy = -1; break;
    case DirectionV1::west: opposite_dx = 1; break;
    default: return false;
  }

  // IDA: Tile::TrySpawn (0x491ab0):
  // new.x/y = opposite(candidate.dir).xy + tile.x/y - candidateExit.x/y;
  // add anchorExit.x/y only when the source anchor pointer is non-null.
  std::int64_t x = 0;
  std::int64_t y = 0;
  if (!checked_add(opposite_dx, anchor_tile.x, x) ||
      !checked_add(opposite_dy, anchor_tile.y, y) ||
      !checked_sub(x, candidate_exit.x, x) ||
      !checked_sub(y, candidate_exit.y, y)) {
    return false;
  }

  if (anchor_exit &&
      (!checked_add(static_cast<std::int32_t>(x), anchor_exit->x, x) ||
       !checked_add(static_cast<std::int32_t>(y), anchor_exit->y, y))) {
    return false;
  }

  const float elevation =
      (anchor_tile.elevation + (anchor_exit ? anchor_exit->elevation : 0.0f)) -
      candidate_exit.elevation;
  if (!std::isfinite(elevation)) return false;

  output.x = static_cast<std::int32_t>(x);
  output.y = static_cast<std::int32_t>(y);
  output.elevation = elevation;
  return true;
}

bool crypt_mgx_footprint_fits_v1(std::int32_t origin_x,
                                 std::int32_t origin_y,
                                 std::int32_t block_width,
                                 std::int32_t block_height,
                                 CryptMgxCellOccupiedV1 is_occupied,
                                 const void* context) noexcept {
  // MgxBlock::FitsInMap returns true without visiting cells when either
  // dimension is non-positive.
  if (block_width <= 0 || block_height <= 0) return true;
  if (!is_occupied) return false;

  const std::int64_t end_x = static_cast<std::int64_t>(origin_x) + block_width;
  const std::int64_t end_y = static_cast<std::int64_t>(origin_y) + block_height;
  if (end_x > std::numeric_limits<std::int32_t>::max() ||
      end_y > std::numeric_limits<std::int32_t>::max()) {
    return false;
  }

  // IDA: MgxBlock::FitsInMap (0x48b85c) scans x in the outer loop and y in
  // the inner loop; Array2d::operator() (0x48b7fc) expands for negative or
  // previously unseen coordinates, so there is no fixed map-edge rejection.
  for (std::int64_t x = origin_x; x < end_x; ++x) {
    for (std::int64_t y = origin_y; y < end_y; ++y) {
      if (is_occupied(context, static_cast<std::int32_t>(x),
                      static_cast<std::int32_t>(y))) {
        return false;
      }
    }
  }
  return true;
}

}  // namespace dh2::random_level
