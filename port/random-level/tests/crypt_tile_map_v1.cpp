#include "../crypt_tile_map_v1.hpp"

#include <cstdint>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

namespace {

using dh2::random_level::CryptMgxExitCellV1;
using dh2::random_level::CryptMgxTileOriginV1;
using dh2::random_level::CryptMgxTilePlacementV1;
using dh2::random_level::CryptTileCellVisitV1;
using dh2::random_level::CryptTileMapBoundsV1;
using dh2::random_level::CryptTileMapV1;
using dh2::random_level::DirectionV1;

struct VisitLog {
  std::vector<std::pair<std::int32_t, std::int32_t>> cells;
};

void record_cell(void* context, std::int32_t x, std::int32_t y) {
  static_cast<VisitLog*>(context)->cells.emplace_back(x, y);
}

bool expect(bool condition, const char* message, int& checks) {
  ++checks;
  if (!condition) std::cerr << "FAIL: " << message << '\n';
  return condition;
}

bool expect_cells(
    const VisitLog& log,
    const std::vector<std::pair<std::int32_t, std::int32_t>>& expected,
    const char* message,
    int& checks) {
  return expect(log.cells == expected, message, checks);
}

}  // namespace

int main() {
  int checks = 0;
  bool ok = true;
  CryptTileMapV1 map;
  int owner_a = 1;
  int owner_b = 2;
  int owner_c = 3;
  int owner_d = 4;

  CryptTileMapBoundsV1 initial = map.bounds();
  ok &= expect(initial.origin_x == 0 && initial.origin_y == 0 &&
                   initial.width == 0 && initial.height == 0,
               "Array2d starts at zero origin with empty extents", checks);
  ok &= expect(map.peek(-2, 7) == nullptr && map.bounds().width == 0,
               "peek does not grow an unallocated map", checks);

  // IDA-derived TrySpawn computes (-1,0) for an east-facing candidate from
  // this anchor; the resulting 2x2 block exercises negative x and row order.
  const CryptMgxTileOriginV1 anchor{0, 0, 10.0f};
  const CryptMgxExitCellV1 candidate_exit{0, 0, 2.0f};
  CryptMgxTilePlacementV1 placement;
  VisitLog added_a;
  CryptTileCellVisitV1 visitor = &record_cell;
  ok &= expect(map.try_spawn(&owner_a, anchor, nullptr, candidate_exit,
                             DirectionV1::east, 2, 2, placement, visitor,
                             &added_a),
               "TrySpawn places a free candidate", checks);
  ok &= expect(placement.x == -1 && placement.y == 0 &&
                   placement.elevation == 8.0f,
               "placement reuses IDA-derived TrySpawn geometry", checks);
  ok &= expect_cells(added_a, {{-1, 0}, {0, 0}, {-1, 1}, {0, 1}},
                      "PlaceInMap writes y-major then x-major", checks);
  const CryptTileMapBoundsV1 after_a = map.bounds();
  ok &= expect(after_a.origin_x == -1 && after_a.origin_y == 0 &&
                   after_a.width == 2 && after_a.height == 2,
               "Array2d grows to cover negative x and positive y", checks);
  ok &= expect(map.peek(-1, 0) == &owner_a && map.peek(0, 0) == &owner_a &&
                   map.peek(-1, 1) == &owner_a && map.peek(0, 1) == &owner_a,
               "each footprint cell stores the owning Tile pointer", checks);

  const auto bounds_before_collision = map.bounds();
  ok &= expect(!map.try_place(&owner_b, 0, 0, 2, 2),
               "overlapping footprint is rejected", checks);
  ok &= expect(map.peek(1, 0) == nullptr && map.peek(1, 1) == nullptr &&
                   map.bounds().origin_x == bounds_before_collision.origin_x &&
                   map.bounds().width == bounds_before_collision.width,
               "rejected overlap writes no cells", checks);

  VisitLog added_b;
  ok &= expect(map.try_place(&owner_b, 2, 0, 1, 2, visitor, &added_b),
               "a disjoint footprint can be placed", checks);
  ok &= expect_cells(added_b, {{2, 0}, {2, 1}},
                      "one-column add retains source row order", checks);
  VisitLog added_c;
  ok &= expect(map.try_place(&owner_c, 3, -1, 1, 1, visitor, &added_c),
               "map expands at negative y and positive x", checks);
  ok &= expect(map.peek(-1, 0) == &owner_a && map.peek(2, 1) == &owner_b &&
                   map.peek(3, -1) == &owner_c && map.peek(1, 0) == nullptr,
               "expansion preserves existing owners and null gaps", checks);
  const auto expanded = map.bounds();
  ok &= expect(expanded.origin_x == -1 && expanded.origin_y == -1 &&
                   expanded.width == 5 && expanded.height == 3,
               "bounds include gaps introduced by Array2d growth", checks);

  VisitLog removed_a;
  ok &= expect(map.remove(&owner_a, -1, 0, 2, 2, visitor, &removed_a),
               "Unspawn removes a matching footprint", checks);
  ok &= expect_cells(removed_a, {{-1, 0}, {0, 0}, {-1, 1}, {0, 1}},
                      "RemoveFromMap scans y-major then x-major", checks);
  ok &= expect(map.peek(-1, 0) == nullptr && map.peek(0, 1) == nullptr &&
                   map.peek(2, 0) == &owner_b,
               "removal clears only the selected owner's cells", checks);
  ok &= expect(map.bounds().origin_x == expanded.origin_x &&
                   map.bounds().origin_y == expanded.origin_y &&
                   map.bounds().width == expanded.width &&
                   map.bounds().height == expanded.height,
               "removal retains the Array2d allocation bounds", checks);

  VisitLog removed_b;
  ok &= expect(map.remove(&owner_b, 2, 0, 1, 2, visitor, &removed_b),
               "Unspawn removes a one-column tile", checks);
  ok &= expect_cells(removed_b, {{2, 0}, {2, 1}},
                      "one-column remove retains source row order", checks);

  ok &= expect(map.try_place(&owner_d, 4, -1, 1, 1),
               "a neighboring tile can occupy a later rollback cell", checks);
  VisitLog removed_mismatch;
  ok &= expect(!map.remove(&owner_d, 3, -1, 2, 1, visitor,
                           &removed_mismatch),
               "wrong-owner removal reports native assert condition", checks);
  ok &= expect_cells(removed_mismatch, {{4, -1}},
                      "remove continues and clears later exact matches", checks);
  ok &= expect(map.peek(3, -1) == &owner_c && map.peek(4, -1) == nullptr,
               "mismatched cells stay occupied while matching cells clear",
               checks);

  const auto before_empty_block = map.bounds();
  ok &= expect(map.try_place(&owner_d,
                             std::numeric_limits<std::int32_t>::max(), 12, 0,
                             3),
               "zero-width block follows native no-cell success", checks);
  ok &= expect(map.bounds().origin_x == before_empty_block.origin_x &&
                   map.bounds().origin_y == before_empty_block.origin_y &&
                   map.bounds().width == before_empty_block.width &&
                   map.bounds().height == before_empty_block.height,
               "zero-width block does not expand the map", checks);

  if (!ok) return 1;
  std::cout << "PASS (" << checks
            << " tile-map occupancy, growth, ordering, collision, and rollback checks)\n";
  return 0;
}
