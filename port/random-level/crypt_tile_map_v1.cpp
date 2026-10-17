#include "crypt_tile_map_v1.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace dh2::random_level {
namespace {

bool source_range_is_representable(std::int32_t origin_x,
                                   std::int32_t origin_y,
                                   std::int32_t block_width,
                                   std::int32_t block_height) noexcept {
  if (block_width <= 0 || block_height <= 0) return true;
  const std::int64_t end_x =
      static_cast<std::int64_t>(origin_x) + block_width;
  const std::int64_t end_y =
      static_cast<std::int64_t>(origin_y) + block_height;
  return end_x <= std::numeric_limits<std::int32_t>::max() &&
         end_y <= std::numeric_limits<std::int32_t>::max();
}

}  // namespace

const void* CryptTileMapV1::cell(std::int32_t x, std::int32_t y) {
  return mutable_cell(x, y);
}

const void* CryptTileMapV1::peek(std::int32_t x,
                                 std::int32_t y) const noexcept {
  const std::int64_t column =
      static_cast<std::int64_t>(x) - static_cast<std::int64_t>(origin_x_);
  const std::int64_t row =
      static_cast<std::int64_t>(y) - static_cast<std::int64_t>(origin_y_);
  if (column < 0 || row < 0 ||
      static_cast<std::uint64_t>(column) >= width_ ||
      static_cast<std::uint64_t>(row) >= height_) {
    return nullptr;
  }
  return rows_[static_cast<std::size_t>(row)]
             [static_cast<std::size_t>(column)];
}

CryptTileMapBoundsV1 CryptTileMapV1::bounds() const noexcept {
  return {origin_x_, origin_y_, width_, height_};
}

bool CryptTileMapV1::try_spawn(
    const void* owner,
    const CryptMgxTileOriginV1& anchor_tile,
    const CryptMgxExitCellV1* anchor_exit,
    const CryptMgxExitCellV1& candidate_exit,
    DirectionV1 candidate_direction,
    std::int32_t block_width,
    std::int32_t block_height,
    CryptMgxTilePlacementV1& placement,
    CryptTileCellVisitV1 visit_added,
    void* visit_context) {
  CryptMgxTilePlacementV1 candidate_placement;
  if (!owner ||
      !crypt_mgx_try_spawn_placement_v1(anchor_tile, anchor_exit,
                                        candidate_exit, candidate_direction,
                                        candidate_placement) ||
      !try_place(owner, candidate_placement.x, candidate_placement.y,
                 block_width, block_height, visit_added, visit_context)) {
    return false;
  }
  placement = candidate_placement;
  return true;
}

bool CryptTileMapV1::try_place(const void* owner,
                              std::int32_t origin_x,
                              std::int32_t origin_y,
                              std::int32_t block_width,
                              std::int32_t block_height,
                              CryptTileCellVisitV1 visit_added,
                              void* visit_context) {
  if (!owner ||
      !crypt_mgx_footprint_fits_v1(
          origin_x, origin_y, block_width, block_height,
          &CryptTileMapV1::occupied_callback, this)) {
    return false;
  }

  // IDA: MgxBlock::PlaceInMap (0x48ba58) traverses row/y offsets first, then
  // x offsets. FitsInMap has already established that every visited cell is
  // empty, so each assignment is a single Tile* write.
  for (std::int64_t y_offset = 0; y_offset < block_height; ++y_offset) {
    const auto y = static_cast<std::int32_t>(
        static_cast<std::int64_t>(origin_y) + y_offset);
    for (std::int64_t x_offset = 0; x_offset < block_width; ++x_offset) {
      const auto x = static_cast<std::int32_t>(
          static_cast<std::int64_t>(origin_x) + x_offset);
      const void*& target = mutable_cell(x, y);
      if (target) return false;
      target = owner;
      if (visit_added) visit_added(visit_context, x, y);
    }
  }
  return true;
}

bool CryptTileMapV1::remove(const void* owner,
                            std::int32_t origin_x,
                            std::int32_t origin_y,
                            std::int32_t block_width,
                            std::int32_t block_height,
                            CryptTileCellVisitV1 visit_removed,
                            void* visit_context) {
  if (!owner) return false;
  if (block_width <= 0 || block_height <= 0) return true;
  if (!source_range_is_representable(origin_x, origin_y, block_width,
                                     block_height)) {
    return false;
  }

  // IDA: MgxBlock::RemoveFromMap (0x48b8f0) uses the same y-major/x-minor
  // order as PlaceInMap, clears only exact Tile* matches, and asserts on a
  // mismatch while continuing the footprint scan.
  bool all_matched = true;
  for (std::int64_t y_offset = 0; y_offset < block_height; ++y_offset) {
    const auto y = static_cast<std::int32_t>(
        static_cast<std::int64_t>(origin_y) + y_offset);
    for (std::int64_t x_offset = 0; x_offset < block_width; ++x_offset) {
      const auto x = static_cast<std::int32_t>(
          static_cast<std::int64_t>(origin_x) + x_offset);
      const void*& target = mutable_cell(x, y);
      if (target == owner) {
        target = nullptr;
        if (visit_removed) visit_removed(visit_context, x, y);
      } else {
        all_matched = false;
      }
    }
  }
  return all_matched;
}

bool CryptTileMapV1::occupied_callback(const void* context,
                                      std::int32_t x,
                                      std::int32_t y) {
  auto* map = const_cast<CryptTileMapV1*>(
      static_cast<const CryptTileMapV1*>(context));
  return map->cell(x, y) != nullptr;
}

void CryptTileMapV1::ensure_position(std::int32_t x, std::int32_t y) {
  // Array2d::operator() calls EnsurePosition(x,y); that function grows the
  // column axis first and the row axis second.
  ensure_x(x);
  ensure_y(y);
}

void CryptTileMapV1::ensure_x(std::int32_t x) {
  const std::int64_t old_begin = origin_x_;
  const std::int64_t old_end = old_begin + static_cast<std::int64_t>(width_);
  const std::int64_t new_begin = std::min(old_begin, static_cast<std::int64_t>(x));
  const std::int64_t new_end =
      std::max(old_end, static_cast<std::int64_t>(x) + 1);
  const std::uint64_t new_width =
      static_cast<std::uint64_t>(new_end - new_begin);
  const auto max_row_size = std::vector<const void*>().max_size();
  if (new_width > max_row_size) {
    throw std::length_error("CryptTileMapV1 x extent exceeds vector limits");
  }
  if (new_width == width_) return;

  std::vector<std::vector<const void*>> expanded_rows;
  expanded_rows.reserve(rows_.size());
  const auto shift = static_cast<std::size_t>(old_begin - new_begin);
  for (const auto& row : rows_) {
    std::vector<const void*> expanded(static_cast<std::size_t>(new_width),
                                       nullptr);
    std::copy(row.begin(), row.end(), expanded.begin() + shift);
    expanded_rows.push_back(std::move(expanded));
  }
  rows_.swap(expanded_rows);
  origin_x_ = static_cast<std::int32_t>(new_begin);
  width_ = static_cast<std::size_t>(new_width);
}

void CryptTileMapV1::ensure_y(std::int32_t y) {
  const std::int64_t old_begin = origin_y_;
  const std::int64_t old_end = old_begin + static_cast<std::int64_t>(height_);
  const std::int64_t new_begin = std::min(old_begin, static_cast<std::int64_t>(y));
  const std::int64_t new_end =
      std::max(old_end, static_cast<std::int64_t>(y) + 1);
  const std::uint64_t new_height =
      static_cast<std::uint64_t>(new_end - new_begin);
  if (new_height > rows_.max_size()) {
    throw std::length_error("CryptTileMapV1 y extent exceeds vector limits");
  }
  if (new_height == height_) return;

  std::vector<std::vector<const void*>> expanded_rows(
      static_cast<std::size_t>(new_height),
      std::vector<const void*>(width_, nullptr));
  const auto shift = static_cast<std::size_t>(old_begin - new_begin);
  for (std::size_t row = 0; row < rows_.size(); ++row) {
    expanded_rows[shift + row] = std::move(rows_[row]);
  }
  rows_.swap(expanded_rows);
  origin_y_ = static_cast<std::int32_t>(new_begin);
  height_ = static_cast<std::size_t>(new_height);
}

const void*& CryptTileMapV1::mutable_cell(std::int32_t x, std::int32_t y) {
  ensure_position(x, y);
  const auto column = static_cast<std::size_t>(
      static_cast<std::int64_t>(x) - static_cast<std::int64_t>(origin_x_));
  const auto row = static_cast<std::size_t>(
      static_cast<std::int64_t>(y) - static_cast<std::int64_t>(origin_y_));
  return rows_[row][column];
}

}  // namespace dh2::random_level
