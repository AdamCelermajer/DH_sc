#include "../native_rule_plan_v1.hpp"

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

namespace {

int checks = 0;

bool check(bool condition, const char* description) {
  ++checks;
  if (!condition) std::cerr << "FAIL: " << description << '\n';
  return condition;
}

const dh2::random_level::RulePlanNodeV1* child_at(
    const dh2::random_level::RulePlanNodeV1& node, std::size_t index) {
  return index < node.children.size() ? &node.children[index] : nullptr;
}

}  // namespace

int main(int argc, char** argv) {
  using namespace dh2::random_level;
  if (argc != 2) {
    std::cerr << "usage: native_rule_plan_v1 <007_crypt_01.rule.xml>\n";
    return 2;
  }

  RandomGeneratorV1 zero_seed(0);
  constexpr std::array<std::uint32_t, 8> expected_stream = {
      279470273u, 1475680373u, 3271658247u, 2499713547u,
      3591385125u, 3000472915u, 634820695u, 2247711126u};
  bool ok = true;
  for (const auto value : expected_stream) {
    ok &= check(zero_seed.next_int() == value, "IDA PRNG stream for seed 0");
  }

  RandomGeneratorV1 wrap_seed(UINT32_MAX);
  ok &= check(wrap_seed.next_int() == 0, "uint32 seed increment wraps before widening");

  RandomGeneratorV1 range_seed(17);
  const auto same_range = range_seed.get_int(4, 4);
  ok &= check(same_range == 4 && range_seed.state() == 17,
              "empty GetInt range returns lower bound without consuming RNG");
  ok &= check(!range_seed.bounded(0) && range_seed.state() == 17,
              "zero bound is reported unsupported without consuming RNG");

  RandomGeneratorV1 shuffle_random(0x00C0FFEEu);
  std::vector<int> shuffled = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  constexpr std::array<int, 10> expected_shuffle = {2, 0, 3, 1, 6, 5, 7, 8, 9, 4};
  ok &= check(source_shuffle(shuffled, shuffle_random), "source shuffle succeeds");
  for (std::size_t i = 0; i < expected_shuffle.size(); ++i) {
    ok &= check(shuffled[i] == expected_shuffle[i], "IDA shuffle golden permutation");
  }
  ok &= check(shuffle_random.state() == 3042378398u,
              "one generator draw per Fisher-Yates position");

  std::ifstream input(argv[1], std::ios::binary);
  if (!input) {
    std::cerr << "FAIL: cannot read pinned Crypt rule " << argv[1] << '\n';
    return 2;
  }
  const std::string xml((std::istreambuf_iterator<char>(input)), {});
  const auto parsed = parse_crypt_rule_v1(xml);
  ok &= check(static_cast<bool>(parsed), "parse actual 007_crypt_01.rule.xml");
  if (!parsed) {
    std::cerr << "parser: " << parsed.error << '\n';
    return 1;
  }

  const auto& document = *parsed.document;
  ok &= check(document.target == "iphone", "Crypt rules target");
  ok &= check(document.folder == "data/3d/modules/crypt", "Crypt module folder");
  const auto root_override = [&document](std::string_view name) -> std::string_view {
    for (const auto& property : document.root_property_overrides)
      if (property.name == name) return property.value;
    return {};
  };
  ok &= check(root_override("ambientColor") == "0.5,0.5,0.5" &&
                  root_override("camera_znear") == "900" &&
                  root_override("camera_zfar") == "5000" &&
                  root_override("music") == "CryptOneAmbientMusic" &&
                  root_override("fixed_light_set") ==
                      "data/3D/Light/crypt_es1_1.lightset_xml",
              "rules-root LevelConfig overrides survive parsing");
  ok &= check(root_override("target").empty() && root_override("folder").empty(),
              "generator target/folder are kept outside the property override set");
  ok &= check(document.lists.size() == 6, "all six Crypt candidate lists parsed");
  const auto* objective_rooms = find_list_v1(document, "objective_rooms");
  const auto* path_list = find_list_v1(document, "path");
  const auto* deadends = find_list_v1(document, "deadends");
  const auto* hallway = find_list_v1(document, "hallway");
  const auto* tmodules = find_list_v1(document, "tmodules");
  const auto* finalroom = find_list_v1(document, "finalroom");
  ok &= check(objective_rooms && objective_rooms->entries.size() == 4,
              "Crypt objective_rooms source list");
  ok &= check(path_list && path_list->entries.size() == 10,
              "Crypt path source list");
  ok &= check(deadends && deadends->entries.size() == 3,
              "Crypt deadends source list");
  ok &= check(hallway && hallway->entries.size() == 1 &&
                  hallway->entries[0].gameplay == "crypt_straight_ns_01.mgp",
              "Crypt hallway source module");
  ok &= check(tmodules && tmodules->entries.size() == 1,
              "Crypt tmodules source list");
  ok &= check(finalroom && finalroom->entries.size() == 3,
              "Crypt finalroom source list");
  for (const auto* list : {objective_rooms, path_list, deadends, hallway, tmodules, finalroom}) {
    ok &= check(list && list->replacement,
                "Crypt random attribute does not override native replacement default");
  }

  RandomGeneratorV1 actual_list_rng(0x00C0FFEEu);
  auto objective_permutation = objective_rooms->entries;
  ok &= check(source_shuffle(objective_permutation, actual_list_rng),
              "source shuffle over actual Crypt objective-room list");
  constexpr std::array<const char*, 4> expected_objectives = {
      "straight_c_ns", "kings_chamber_s", "cemetery_entrance", "entrance_s"};
  for (std::size_t i = 0; i < expected_objectives.size(); ++i) {
    ok &= check(objective_permutation[i].name == expected_objectives[i],
                "Crypt list order follows IDA shuffle primitive");
  }
  const auto plan_result = build_rule_plan_v1(document, 0x00C0FFEEu);
  ok &= check(static_cast<bool>(plan_result), "build source-backed Crypt rule plan");
  if (!plan_result) {
    std::cerr << "plan: " << plan_result.error << '\n';
    return 1;
  }
  const auto& plan = *plan_result.plan;
  ok &= check(!plan.generated_level, "plan does not claim tile/map generation");
  ok &= check(plan.root_candidate_order.size() == 1,
              "Crypt RootRule explicit selector yields one candidate");
  if (plan.root_candidate_order.size() == 1) {
    ok &= check(plan.root_candidate_order[0].name == "cemetery_entrance",
                "Crypt RootRule index 3 candidate");
    ok &= check(plan.root_candidate_order[0].gameplay == "crypt_cemetery_entrance_01.mgp",
                "Crypt RootRule gameplay module");
    ok &= check(plan.root_candidate_order[0].visual == "crypt_cemetery_entrance_01.mvp",
                "Crypt RootRule visual module");
  }
  ok &= check(plan.rng_state_after_root == 0x00C0FFEEu,
              "explicit Crypt root index consumes no random values");

  const auto* root_force = child_at(plan.root_constraints, 0);
  const auto* outer_path = root_force ? child_at(*root_force, 0) : nullptr;
  const auto* selected_tmodule = outer_path ? child_at(*outer_path, 0) : nullptr;
  const auto* inner_path = selected_tmodule ? child_at(*selected_tmodule, 0) : nullptr;
  const auto* planned_final = inner_path ? child_at(*inner_path, 0) : nullptr;
  const auto* planned_deadends = selected_tmodule ? child_at(*selected_tmodule, 1) : nullptr;
  ok &= check(root_force && root_force->allowed_list_entries.size() == 1,
              "hallway ForceBlock exposes the actual candidate domain");
  ok &= check(outer_path && outer_path->path.min_length == 2 &&
                  outer_path->path.max_length == 3,
              "first Crypt Path length range");
  ok &= check(outer_path && outer_path->path.dont_go_back &&
                  !outer_path->path.dont_go_back_attribute_applied,
              "lowercase textual dontgoback is ignored by native numeric exact-case query");
  ok &= check(selected_tmodule && selected_tmodule->allowed_list_entries.size() == 1 &&
                  selected_tmodule->allowed_list_entries[0].name == "t_sew",
              "Crypt tmodules explicit index 0");
  ok &= check(inner_path && inner_path->path.min_length == 1 &&
                  inner_path->path.max_length == 2 && inner_path->path.dont_go_back,
              "second Crypt Path range and source default");
  ok &= check(planned_final && planned_final->allowed_list_entries.size() == 3,
              "finalroom remains a three-module allowed domain");
  ok &= check(planned_deadends && planned_deadends->allowed_list_entries.size() == 3,
              "deadends remains a three-module allowed domain");

  std::cout << "{\"validation\":\"" << (ok ? "PASS" : "FAIL")
            << "\",\"checks\":" << checks
            << ",\"source\":\"007_crypt_01.rule.xml\",\"stage\":\"root-and-rule-plan\"}\n";
  return ok ? 0 : 1;
}
