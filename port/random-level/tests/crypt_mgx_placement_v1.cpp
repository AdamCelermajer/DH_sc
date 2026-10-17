#include "../crypt_mgx_placement_v1.hpp"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

using namespace dh2::random_level;

namespace {

int checks = 0;
int failures = 0;

void check(bool condition, const char* label) {
  ++checks;
  if (!condition) {
    ++failures;
    std::cerr << "FAIL: " << label << '\n';
  }
}

struct Occupancy {
  std::vector<std::pair<std::int32_t, std::int32_t>> cells;
  std::vector<std::pair<std::int32_t, std::int32_t>> lookups;
};

bool is_occupied(const void* context, std::int32_t x, std::int32_t y) {
  auto& occupancy = *static_cast<Occupancy*>(const_cast<void*>(context));
  occupancy.lookups.emplace_back(x, y);
  for (const auto& cell : occupancy.cells) {
    if (cell.first == x && cell.second == y) return true;
  }
  return false;
}

bool same_cell(const CryptMgxExitCellV1& actual,
               std::int32_t x,
               std::int32_t y) {
  return actual.x == x && actual.y == y && actual.elevation == 0.0f;
}

bool same_placement(const CryptMgxTilePlacementV1& actual,
                    std::int32_t x,
                    std::int32_t y,
                    float elevation) {
  return actual.x == x && actual.y == y && actual.elevation == elevation;
}

void test_mgx_coordinate_transform() {
  const CryptMgxGridGeometryV1 geometry{2.0f, 4.0f, 4, 2};
  CryptMgxExitCellV1 cell{77, 88, 9.0f};

  check(crypt_mgx_exit_position_to_cell_v1(geometry, -4.0f, 4.0f, 0.0f, cell) &&
            same_cell(cell, 0, 0),
        "left/top outer edges clamp to zero");
  check(crypt_mgx_exit_position_to_cell_v1(geometry, -2.0f, 0.0f, 0.0f, cell) &&
            same_cell(cell, 0, 0),
        "exact interior x boundary maps to the preceding cell");
  check(crypt_mgx_exit_position_to_cell_v1(geometry, -1.9f, -1.0f, 0.0f, cell) &&
            same_cell(cell, 1, 1),
        "fractional positions use ceil-minus-one in both axes");
  check(crypt_mgx_exit_position_to_cell_v1(geometry, 4.0f, -4.0f, 0.0f, cell) &&
            same_cell(cell, 3, 1),
        "exact right and lower boundaries map to the preceding cell");
  check(crypt_mgx_exit_position_to_cell_v1(geometry, 6.0f, -8.0f, 0.0f, cell) &&
            same_cell(cell, 4, 2),
        "the original transform has no upper clamp");

  const CryptMgxGridGeometryV1 crypt_geometry{4800.0f, 4800.0f, 1, 1};
  check(crypt_mgx_exit_position_to_cell_v1(
            crypt_geometry, -0.902344f, 2227.2f, 428.8f, cell) &&
            cell.x == 0 && cell.y == 0 &&
            std::fabs(cell.elevation - 428.8f) < 0.001f,
        "cached Crypt north exit maps to cell zero and preserves its z");

  const CryptMgxExitCellV1 before = cell;
  const CryptMgxGridGeometryV1 zero_unit{0.0f, 4.0f, 4, 2};
  check(!crypt_mgx_exit_position_to_cell_v1(zero_unit, 0.0f, 0.0f, 0.0f, cell) &&
            cell.x == before.x && cell.y == before.y &&
            cell.elevation == before.elevation,
        "invalid unit size rejects without changing output");
}

void test_try_spawn_transform() {
  const CryptMgxTileOriginV1 tile{10, -5, 100.0f};
  const CryptMgxExitCellV1 anchor{2, 3, 4.0f};
  const CryptMgxExitCellV1 candidate{5, 7, 1.0f};
  CryptMgxTilePlacementV1 placed{-90, -91, -92.0f};

  check(crypt_mgx_try_spawn_placement_v1(tile, &anchor, candidate,
                                         DirectionV1::north, placed) &&
            same_placement(placed, 7, -8, 103.0f),
        "north exit uses the opposite south cell step");
  check(crypt_mgx_try_spawn_placement_v1(tile, &anchor, candidate,
                                         DirectionV1::east, placed) &&
            same_placement(placed, 6, -9, 103.0f),
        "east exit uses the opposite west cell step");
  check(crypt_mgx_try_spawn_placement_v1(tile, &anchor, candidate,
                                         DirectionV1::south, placed) &&
            same_placement(placed, 7, -10, 103.0f),
        "south exit uses the opposite north cell step");
  check(crypt_mgx_try_spawn_placement_v1(tile, &anchor, candidate,
                                         DirectionV1::west, placed) &&
            same_placement(placed, 8, -9, 103.0f),
        "west exit uses the opposite east cell step");

  check(crypt_mgx_try_spawn_placement_v1(tile, nullptr, candidate,
                                         DirectionV1::east, placed) &&
            same_placement(placed, 4, -12, 99.0f),
        "null anchor exit omits its cell/elevation offset");

  const CryptMgxTilePlacementV1 before = placed;
  check(!crypt_mgx_try_spawn_placement_v1(
            tile, &anchor, candidate, static_cast<DirectionV1>(99), placed) &&
            same_placement(placed, before.x, before.y, before.elevation),
        "invalid direction rejects without changing output");
  const CryptMgxTileOriginV1 overflow_tile{
      std::numeric_limits<std::int32_t>::max(), 0, 0.0f};
  check(!crypt_mgx_try_spawn_placement_v1(overflow_tile, nullptr, candidate,
                                          DirectionV1::west, placed),
        "coordinate overflow is rejected safely");
}

void test_footprint_fit() {
  Occupancy occupancy;
  check(crypt_mgx_footprint_fits_v1(-1, 4, 2, 2, is_occupied, &occupancy),
        "empty footprint fits across negative map coordinates");
  check(occupancy.lookups ==
            std::vector<std::pair<std::int32_t, std::int32_t>>{
                {-1, 4}, {-1, 5}, {0, 4}, {0, 5}},
        "fit visits every half-open footprint cell in source loop order");

  occupancy.cells = {{0, 5}};
  occupancy.lookups.clear();
  check(!crypt_mgx_footprint_fits_v1(-1, 4, 2, 2, is_occupied, &occupancy),
        "any occupied cell rejects the candidate footprint");
  check(occupancy.lookups.back() == std::make_pair<std::int32_t, std::int32_t>(0, 5),
        "fit stops on the first occupied cell");

  occupancy.cells = {{100, 100}};
  occupancy.lookups.clear();
  check(crypt_mgx_footprint_fits_v1(-1, 4, 2, 2, is_occupied, &occupancy),
        "occupied cells outside the candidate footprint do not reject it");
  check(crypt_mgx_footprint_fits_v1(0, 0, 0, 3, nullptr, nullptr) &&
            crypt_mgx_footprint_fits_v1(0, 0, 3, -1, nullptr, nullptr),
        "non-positive dimensions match the source's zero-iteration success");
  check(!crypt_mgx_footprint_fits_v1(0, 0, 1, 1, nullptr, nullptr),
        "positive footprints require an occupancy lookup");
  check(!crypt_mgx_footprint_fits_v1(
            std::numeric_limits<std::int32_t>::max(), 0, 2, 1, is_occupied,
            &occupancy),
        "footprint end overflow is rejected safely");
}

}  // namespace

int main() {
  test_mgx_coordinate_transform();
  test_try_spawn_transform();
  test_footprint_fit();
  std::cout << checks - failures << '/' << checks << " placement checks passed\n";
  return failures == 0 ? 0 : 1;
}
