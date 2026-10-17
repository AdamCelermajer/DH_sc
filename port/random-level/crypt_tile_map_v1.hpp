#pragma once

#include "crypt_mgx_placement_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace dh2::random_level {

// Host model of Array2d<Tile*> plus the map-facing part of Tile::TrySpawn.
// The two-dimensional storage is indexed as rows[y][x], like the native
// nested deque. A null owner means an empty source tile pointer.
struct CryptTileMapBoundsV1 {
  std::int32_t origin_x = 0;
  std::int32_t origin_y = 0;
  std::size_t width = 0;
  std::size_t height = 0;
};

using CryptTileCellVisitV1 = void (*)(void* context,
                                      std::int32_t x,
                                      std::int32_t y);

class CryptTileMapV1 {
 public:
  // Source Array2d::operator() grows zero-filled storage when a cell is read.
  // This query has the same growth side effect.
  const void* cell(std::int32_t x, std::int32_t y);

  // Inspection only: coordinates outside allocated source bounds read empty
  // without growing the Array2d.
  const void* peek(std::int32_t x, std::int32_t y) const noexcept;

  CryptTileMapBoundsV1 bounds() const noexcept;

  // Models TrySpawn -> FitsInMap -> Spawn -> Tile::PlaceTile -> PlaceInMap.
  // The candidate placement uses the shared IDA-derived Tile::TrySpawn helper.
  // On collision or invalid placement, no owner cells are written.
  bool try_spawn(const void* owner,
                 const CryptMgxTileOriginV1& anchor_tile,
                 const CryptMgxExitCellV1* anchor_exit,
                 const CryptMgxExitCellV1& candidate_exit,
                 DirectionV1 candidate_direction,
                 std::int32_t block_width,
                 std::int32_t block_height,
                 CryptMgxTilePlacementV1& placement,
                 CryptTileCellVisitV1 visit_added = nullptr,
                 void* visit_context = nullptr);

  // TrySpawn first asks FitsInMap (x-major/y-minor). The subsequent native
  // PlaceInMap writes y-major/x-minor. The optional visitor exposes that
  // source order to focused host tests.
  bool try_place(const void* owner,
                 std::int32_t origin_x,
                 std::int32_t origin_y,
                 std::int32_t block_width,
                 std::int32_t block_height,
                 CryptTileCellVisitV1 visit_added = nullptr,
                 void* visit_context = nullptr);

  // Mirrors RemoveFromMap: scan y-major/x-minor, clear only cells whose
  // owner matches, and continue after a mismatch (the native path asserts).
  // Returns false if any visited cell belonged to another tile.
  bool remove(const void* owner,
              std::int32_t origin_x,
              std::int32_t origin_y,
              std::int32_t block_width,
              std::int32_t block_height,
              CryptTileCellVisitV1 visit_removed = nullptr,
              void* visit_context = nullptr);

 private:
  static bool occupied_callback(const void* context,
                                std::int32_t x,
                                std::int32_t y);
  void ensure_position(std::int32_t x, std::int32_t y);
  void ensure_x(std::int32_t x);
  void ensure_y(std::int32_t y);
  const void*& mutable_cell(std::int32_t x, std::int32_t y);

  // The native constructor/Clear establish zero origins and zero extents.
  // EnsurePosition then grows toward each accessed coordinate, preserving
  // negative coordinates and zero-filled gaps.
  std::int32_t origin_x_ = 0;
  std::int32_t origin_y_ = 0;
  std::size_t width_ = 0;
  std::size_t height_ = 0;
  std::vector<std::vector<const void*>> rows_;
};

}  // namespace dh2::random_level
