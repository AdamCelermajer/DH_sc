#include "../crypt_mgx_connectivity_v1.hpp"

#include <array>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace {

using dh2::random_level::CryptMgxModuleV1;
using dh2::random_level::DirectionV1;

struct ExpectedExit {
  const char* object_name;
  DirectionV1 direction;
};

struct ExpectedModule {
  const char* filename;
  std::vector<ExpectedExit> exits;
};

const std::array<ExpectedModule, 21> kExpectedModules = {{
    {"boss_room.mgx", {{"_exit_south_09", DirectionV1::south}}},
    {"cemetery_entrance.mgx", {{"_exit_north_07", DirectionV1::north}}},
    {"corner_ne.mgx", {{"_exit_east_05", DirectionV1::east},
                        {"_exit_north_03", DirectionV1::north}}},
    {"corner_nw.mgx", {{"_exit_west_06", DirectionV1::west},
                        {"_exit_north_02", DirectionV1::north}}},
    {"corner_se.mgx", {{"_exit_east_04", DirectionV1::east},
                        {"_exit_south_05", DirectionV1::south}}},
    {"corner_sw.mgx", {{"_exit_west_08", DirectionV1::west},
                        {"_exit_south_07", DirectionV1::south}}},
    {"deadend_e.mgx", {{"_exit_east_09", DirectionV1::east}}},
    {"deadend_n.mgx", {{"_exit_north_04", DirectionV1::north}}},
    {"deadend_w.mgx", {{"_exit_west_07", DirectionV1::west}}},
    {"entrance_n.mgx", {{"_exit_north_99", DirectionV1::north}}},
    {"entrance_s.mgx", {{"_exit_south_04", DirectionV1::south}}},
    {"kings_chamber_s.mgx", {{"_exit_south_03", DirectionV1::south}}},
    {"straight_a_ew.mgx", {{"_exit_east_01", DirectionV1::east},
                            {"_exit_west_01", DirectionV1::west}}},
    {"straight_b_ew.mgx", {{"_exit_east_08", DirectionV1::east},
                            {"_exit_west_04", DirectionV1::west}}},
    {"straight_b_ns.mgx", {{"_exit_north_06", DirectionV1::north},
                            {"_exit_south_08", DirectionV1::south}}},
    {"straight_c_ew.mgx", {{"_exit_east_02", DirectionV1::east},
                            {"_exit_west_02", DirectionV1::west}}},
    {"straight_c_ns.mgx", {{"_exit_south_10", DirectionV1::south}}},
    {"straight_d_ew.mgx", {{"_exit_west_03", DirectionV1::west},
                            {"_exit_east_03", DirectionV1::east}}},
    {"straight_ns.mgx", {{"_exit_north_05", DirectionV1::north},
                          {"_exit_south_06", DirectionV1::south}}},
    {"t_nse.mgx", {{"_exit_south_01", DirectionV1::south},
                   {"_exit_east_07", DirectionV1::east},
                   {"_exit_north_01", DirectionV1::north}}},
    {"t_sew.mgx", {{"_exit_west_05", DirectionV1::west},
                   {"_exit_east_06", DirectionV1::east},
                   {"_exit_south_02", DirectionV1::south}}},
}};

int checks = 0;

bool check(bool condition, const std::string& description) {
  ++checks;
  if (!condition) std::cerr << "FAIL: " << description << '\n';
  return condition;
}

bool read_file(const std::filesystem::path& path, std::string& output) {
  std::ifstream input(path, std::ios::binary);
  if (!input) return false;
  output.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
  return true;
}

std::size_t direction_index(DirectionV1 direction) {
  switch (direction) {
    case DirectionV1::north: return 0;
    case DirectionV1::east: return 1;
    case DirectionV1::south: return 2;
    case DirectionV1::west: return 3;
  }
  return 4;
}

std::size_t direction_pair_index(DirectionV1 first, DirectionV1 second) {
  return direction_index(first) * 4 + direction_index(second);
}

struct RawPosition {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
};

std::vector<RawPosition> read_source_link_positions(const std::string& xml) {
  std::vector<RawPosition> positions;
  std::size_t cursor = 0;
  while ((cursor = xml.find("<GameObject", cursor)) != std::string::npos) {
    const auto tag_end = xml.find("/>", cursor);
    if (tag_end == std::string::npos) break;
    const std::string_view tag(xml.data() + cursor, tag_end + 2 - cursor);
    cursor = tag_end + 2;
    if (tag.find("gametype=\"link\"") == std::string_view::npos &&
        tag.find("gametype='link'") == std::string_view::npos) {
      continue;
    }
    const auto position_key = tag.find("position=");
    if (position_key == std::string_view::npos || position_key + 9 >= tag.size()) continue;
    const char quote = tag[position_key + 9];
    const auto value_begin = position_key + 10;
    const auto value_end = tag.find(quote, value_begin);
    if (value_end == std::string_view::npos) continue;
    const auto value = tag.substr(value_begin, value_end - value_begin);
    RawPosition point;
    float* components[] = {&point.x, &point.y, &point.z};
    std::size_t part_start = 0;
    for (float* component : components) {
      while (part_start < value.size() && value[part_start] == ',') ++part_start;
      if (part_start == value.size()) break;
      auto part_end = value.find(',', part_start);
      if (part_end == std::string_view::npos) part_end = value.size();
      const std::string token(value.substr(part_start, part_end - part_start));
      *component = static_cast<float>(std::strtod(token.c_str(), nullptr));
      part_start = part_end;
    }
    positions.push_back(point);
  }
  return positions;
}

std::pair<std::int32_t, std::int32_t> direction_step(DirectionV1 direction) {
  switch (direction) {
    case DirectionV1::north: return {0, -1};
    case DirectionV1::east: return {1, 0};
    case DirectionV1::south: return {0, 1};
    case DirectionV1::west: return {-1, 0};
  }
  return {0, 0};
}

}  // namespace

int main(int argc, char** argv) {
  using namespace dh2::random_level;
  if (argc != 2) {
    std::cerr << "usage: crypt_mgx_connectivity_v1 <cache/data/3d/modules/crypt/mgx>\n";
    return 2;
  }

  bool ok = true;
  const std::filesystem::path directory(argv[1]);
  std::vector<CryptMgxModuleV1> modules;
  std::map<std::string, std::size_t> direction_counts;
  std::size_t exit_count = 0;
  std::size_t raw_position_count = 0;
  std::size_t standard_elevation_count = 0;
  std::size_t entrance_elevation_count = 0;
  std::size_t geometry_count = 0;

  for (const auto& expected : kExpectedModules) {
    const auto path = directory / expected.filename;
    std::string xml;
    ok &= check(read_file(path, xml), std::string("read cached ") + expected.filename);
    if (xml.empty()) continue;
    const auto parsed = parse_crypt_mgx_v1(expected.filename, xml);
    ok &= check(static_cast<bool>(parsed), std::string("parse cached ") + expected.filename);
    if (!parsed) {
      std::cerr << expected.filename << ": " << parsed.error << '\n';
      continue;
    }
    auto module = parsed.module;
    ok &= check(module.unit_width == 4800.0 && module.unit_height == 4800.0,
                std::string("Crypt module dimensions in ") + expected.filename);
    ok &= check(module.source_unit_width == 4800.0f &&
                    module.source_unit_height == 4800.0f && module.block_width == 1 &&
                    module.block_height == 1 && module.has_grid_geometry,
                std::string("source f32 block-grid geometry in ") + expected.filename);
    if (module.has_grid_geometry) ++geometry_count;
    ok &= check(module.exits.size() == expected.exits.size(),
                std::string("source exit count in ") + expected.filename);
    const auto source_positions = read_source_link_positions(xml);
    ok &= check(source_positions.size() == expected.exits.size(),
                std::string("independently read source position count in ") +
                    expected.filename);
    const auto compare_count = module.exits.size() < expected.exits.size()
                                   ? module.exits.size()
                                   : expected.exits.size();
    for (std::size_t i = 0; i < compare_count; ++i) {
      const auto& actual = module.exits[i];
      ok &= check(actual.object_name == expected.exits[i].object_name,
                  std::string("source exit name in ") + expected.filename);
      ok &= check(actual.direction == expected.exits[i].direction,
                  std::string("source direction in ") + expected.filename);
      ok &= check(actual.link_type == "Crypt",
                  std::string("source link token in ") + expected.filename);
      ok &= check(actual.has_raw_position && actual.has_cell_position,
                  std::string("source position and computed cell retained in ") +
                      expected.filename);
      if (actual.has_raw_position) {
        ++raw_position_count;
        ok &= check(std::isfinite(actual.position_x) &&
                        std::isfinite(actual.position_y) &&
                        std::isfinite(actual.position_z),
                    std::string("finite raw position in ") + expected.filename);
        if (i < source_positions.size()) {
          ok &= check(actual.position_x == source_positions[i].x &&
                          actual.position_y == source_positions[i].y &&
                          actual.position_z == source_positions[i].z,
                      std::string("raw f32 position matches cached XML in ") +
                          expected.filename);
        }
        if (actual.position_z == 428.8f) {
          ++standard_elevation_count;
        } else if (actual.position_z == 416.395f) {
          ++entrance_elevation_count;
        }
      }
      ok &= check(actual.cell_x == 0 && actual.cell_y == 0,
                  std::string("cached Crypt exit maps to source grid cell (0,0) in ") +
                      expected.filename);
      ++direction_counts[direction_name_v1(actual.direction)];
      ++exit_count;
    }
    modules.push_back(std::move(module));
  }

  ok &= check(modules.size() == kExpectedModules.size(), "all 21 cached Crypt MGX definitions load");
  ok &= check(exit_count == 35, "all 35 source link exits load");
  ok &= check(geometry_count == 21 && raw_position_count == 35,
              "source geometry and all 35 raw exit positions are available");
  ok &= check(standard_elevation_count == 34 && entrance_elevation_count == 1,
              "source z retains 34 exits at 428.8 and the entrance exit at 416.395");
  ok &= check(direction_counts["north"] == 8 && direction_counts["east"] == 9 &&
                  direction_counts["south"] == 10 && direction_counts["west"] == 8,
              "Crypt source exit direction inventory");

  const auto adjacency = enumerate_crypt_exit_adjacencies_v1(modules);
  std::array<std::size_t, 16> directional_pairs{};
  std::map<std::pair<std::size_t, std::size_t>, bool> module_pairs;
  for (const auto& edge : adjacency) {
    const auto& first = modules[edge.first_module].exits[edge.first_exit];
    const auto& second = modules[edge.second_module].exits[edge.second_exit];
    ++directional_pairs[direction_pair_index(first.direction, second.direction)];
    module_pairs[{edge.first_module, edge.second_module}] = true;
  }
  ok &= check(adjacency.size() == 160,
              "160 link-token/equal-opposite-direction candidate exit pairs across unordered definitions");
  ok &= check(module_pairs.size() == 132, "132 unordered module-definition pairs have an exit candidate");
  ok &= check(directional_pairs[direction_pair_index(DirectionV1::north, DirectionV1::south)] == 51 &&
                  directional_pairs[direction_pair_index(DirectionV1::south, DirectionV1::north)] == 32 &&
                  directional_pairs[direction_pair_index(DirectionV1::east, DirectionV1::west)] == 37 &&
                  directional_pairs[direction_pair_index(DirectionV1::west, DirectionV1::east)] == 40,
              "candidate exit pairs follow opposite-direction source rule");

  const auto placement_candidates = enumerate_crypt_exit_placement_candidates_v1(modules);
  ok &= check(placement_candidates.size() == adjacency.size(),
              "all topological pairs with cached geometry receive source placement deltas");
  std::map<std::tuple<std::size_t, std::size_t, std::size_t, std::size_t>, int>
      topology_pair_counts;
  std::map<std::tuple<std::size_t, std::size_t, std::size_t, std::size_t>, int>
      placement_pair_counts;
  for (const auto& edge : adjacency) {
    ++topology_pair_counts[{edge.first_module, edge.first_exit,
                            edge.second_module, edge.second_exit}];
  }
  bool every_placement_is_aligned = true;
  for (const auto& candidate : placement_candidates) {
    const auto key = std::make_tuple(candidate.first_module, candidate.first_exit,
                                     candidate.second_module, candidate.second_exit);
    ++placement_pair_counts[key];
    const auto& anchor = modules[candidate.first_module].exits[candidate.first_exit];
    const auto& second = modules[candidate.second_module].exits[candidate.second_exit];
    const auto step = direction_step(anchor.direction);
    const bool aligned =
        candidate.second_origin_x + second.cell_x == anchor.cell_x + step.first &&
        candidate.second_origin_y + second.cell_y == anchor.cell_y + step.second &&
        std::fabs((candidate.second_origin_z + second.position_z) -
                  anchor.position_z) < 0.001f;
    every_placement_is_aligned &= aligned && crypt_exits_compatible_v1(anchor, second);
  }
  ok &= check(placement_pair_counts == topology_pair_counts,
              "geometric candidate catalog preserves exactly the topology pair set");
  ok &= check(every_placement_is_aligned,
              "every candidate exit lands one cell along the anchor and at matching z");

  CryptMgxModuleV1 synthetic_anchor{"synthetic_anchor", 4800.0, 4800.0, {}};
  CryptMgxModuleV1 synthetic_candidate{"synthetic_candidate", 4800.0, 4800.0, {}};
  synthetic_anchor.has_grid_geometry = true;
  synthetic_candidate.has_grid_geometry = true;
  synthetic_anchor.exits.push_back(
      {"anchor_e", "Crypt", DirectionV1::east});
  synthetic_candidate.exits.push_back(
      {"candidate_w", "Crypt", DirectionV1::west});
  auto& synthetic_anchor_exit = synthetic_anchor.exits.front();
  synthetic_anchor_exit.cell_x = 1;
  synthetic_anchor_exit.cell_y = 2;
  synthetic_anchor_exit.position_z = 428.8f;
  synthetic_anchor_exit.has_cell_position = true;
  synthetic_anchor_exit.has_raw_position = true;
  auto& synthetic_candidate_exit = synthetic_candidate.exits.front();
  synthetic_candidate_exit.cell_x = 4;
  synthetic_candidate_exit.cell_y = 5;
  synthetic_candidate_exit.position_z = 416.395f;
  synthetic_candidate_exit.has_cell_position = true;
  synthetic_candidate_exit.has_raw_position = true;
  const auto synthetic_placements = enumerate_crypt_exit_placement_candidates_v1(
      {synthetic_anchor, synthetic_candidate});
  ok &= check(synthetic_placements.size() == 1 &&
                  synthetic_placements.front().second_origin_x == -2 &&
                  synthetic_placements.front().second_origin_y == -3 &&
                  std::fabs(synthetic_placements.front().second_origin_z - 12.405f) <
                      0.001f,
              "nonzero exit cells and unequal heights produce the source translation");
  if (synthetic_placements.size() == 1) {
    const auto& placement = synthetic_placements.front();
    ok &= check(placement.second_origin_x + synthetic_candidate_exit.cell_x ==
                        synthetic_anchor_exit.cell_x + 1 &&
                    placement.second_origin_y + synthetic_candidate_exit.cell_y ==
                        synthetic_anchor_exit.cell_y &&
                    std::fabs((placement.second_origin_z +
                               synthetic_candidate_exit.position_z) -
                              synthetic_anchor_exit.position_z) < 0.001f,
                "synthetic pair is adjacent in cells and coincident in world height");
  }
  synthetic_anchor_exit.has_cell_position = false;
  ok &= check(enumerate_crypt_exit_adjacencies_v1(
                  {synthetic_anchor, synthetic_candidate}).size() == 1 &&
                  enumerate_crypt_exit_placement_candidates_v1(
                      {synthetic_anchor, synthetic_candidate}).empty(),
              "missing computed cells cannot create a geometric match");

  const CryptMgxExitV1 east{"east", "Crypt", DirectionV1::east};
  const CryptMgxExitV1 west{"west", "Crypt", DirectionV1::west};
  const CryptMgxExitV1 north{"north", "Crypt", DirectionV1::north};
  const CryptMgxExitV1 different_token{"west", "CryptOther", DirectionV1::west};
  const CryptMgxExitV1 different_case_token{"west", "crypt", DirectionV1::west};
  ok &= check(crypt_exits_compatible_v1(east, west), "equal link token and opposite east/west connect");
  ok &= check(!crypt_exits_compatible_v1(east, north), "non-opposite directions do not connect");
  ok &= check(!crypt_exits_compatible_v1(east, different_token), "different link tokens do not connect");
  ok &= check(!crypt_exits_compatible_v1(east, different_case_token),
              "source link tokens use byte-exact comparison");

  constexpr std::string_view sample =
      "<?xml version=\"1.0\"?><Module unit_width=\"4800\" unit_height=\"4800\">"
      "<!-- generated -->"
      "<GameObject name=\"x\" gametype=\"LiNk\" direction=\"east\" linktype=\"Crypt\"/>"
      "<GameObject gametype=\"decoration\"/>"
      "</Module><!-- exporter trailer -->";
  const auto sample_result = parse_crypt_mgx_v1("sample", sample);
  ok &= check(sample_result && sample_result.module.exits.size() == 1,
              "native case-insensitive gametype=link selection and ignored non-link object");
  ok &= check(sample_result && !sample_result.module.has_grid_geometry &&
                  !sample_result.module.exits.empty() &&
                  !sample_result.module.exits[0].has_raw_position &&
                  enumerate_crypt_exit_placement_candidates_v1(
                      {sample_result.module}).empty(),
              "legacy topology-only XML remains parseable without fabricated geometry");

  constexpr std::string_view precision_sample =
      "<Module unit_width=\"4800.00006\" unit_height=\"4800.00006\" "
      "block_width=\"1.0\" block_height=\"1.0\">"
      "<GameObject name=\"e\" gametype=\"link\" linktype=\"Crypt\" "
      "direction=\"north\" position=\"-0.902344,2227.2,428.8\"/>"
      "</Module>";
  const auto precision_result = parse_crypt_mgx_v1("precision", precision_sample);
  ok &= check(precision_result && precision_result.module.has_grid_geometry &&
                  precision_result.module.block_width == 1 &&
                  precision_result.module.block_height == 1,
              "native QueryIntAttribute accepts the cached 1.0 block dimension spelling");
  if (precision_result && !precision_result.module.exits.empty()) {
    const auto& exit = precision_result.module.exits.front();
    ok &= check(precision_result.module.unit_width == 4800.00006 &&
                    precision_result.module.source_unit_width == 4800.0f &&
                    static_cast<double>(precision_result.module.source_unit_width) !=
                        precision_result.module.unit_width,
                "legacy XML double remains while source f32 unit precision is retained");
    ok &= check(exit.position_x == -0.902344f && exit.position_y == 2227.2f &&
                    exit.position_z == 428.8f && exit.has_cell_position &&
                    exit.cell_x == 0 && exit.cell_y == 0,
                "cached Crypt z=428.8 position retains f32 values and maps to cell zero");
  }
  const auto malformed = parse_crypt_mgx_v1(
      "bad", "<Module unit_width=\"4800\" unit_height=\"4800\">"
            "<GameObject name=\"x\" gametype=\"link\" linktype=\"Crypt\"/>"
            "</Module>");
  ok &= check(!malformed, "malformed link exits fail closed when direction is missing");

  std::cout << "{\"validation\":\"" << (ok ? "PASS" : "FAIL")
            << "\",\"checks\":" << checks << ",\"modules\":" << modules.size()
            << ",\"exits\":" << exit_count << ",\"candidate_exit_pairs\":" << adjacency.size()
            << ",\"candidate_module_pairs\":" << module_pairs.size()
            << ",\"stage\":\"mgx-candidate-placement\"}\n";
  return ok ? 0 : 1;
}
