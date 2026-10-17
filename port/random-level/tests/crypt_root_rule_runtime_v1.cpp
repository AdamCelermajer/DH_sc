#include "../crypt_root_rule_runtime_v1.hpp"

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

using namespace dh2::random_level;

namespace {

int checks = 0;

bool check(bool condition, const char* description) {
  ++checks;
  if (!condition) std::cerr << "FAIL: " << description << '\n';
  return condition;
}

struct PlacedCall {
  std::string name;
  std::string gameplay;
  std::string visual;
  const void* block = nullptr;
  RootPlacementOriginV1 origin;
};

struct CallLog {
  std::vector<std::string> lookups;
  std::vector<PlacedCall> placements;
  std::map<std::string, int> blocks;
  std::optional<std::string> missing_name;
  std::optional<std::string> lookup_error_name;
  std::optional<std::string> found_null_name;
  std::optional<std::string> placement_error_name;
  std::optional<std::string> success_name;
};

RootBlockLookupResultV1 lookup(void* raw, std::string_view name) {
  auto& log = *static_cast<CallLog*>(raw);
  const std::string owned_name(name);
  log.lookups.push_back(owned_name);
  if (log.lookup_error_name && owned_name == *log.lookup_error_name) {
    return {RootBlockLookupStatusV1::error, nullptr};
  }
  if (log.missing_name && owned_name == *log.missing_name) {
    return {RootBlockLookupStatusV1::missing, nullptr};
  }
  if (log.found_null_name && owned_name == *log.found_null_name) {
    return {RootBlockLookupStatusV1::found, nullptr};
  }
  const auto found = log.blocks.find(owned_name);
  if (found == log.blocks.end()) return {RootBlockLookupStatusV1::missing, nullptr};
  return {RootBlockLookupStatusV1::found, &found->second};
}

RootPlacementStepStatusV1 place_root_and_step(
    void* raw, const void* block, const CryptListEntryV1& candidate,
    const RootPlacementOriginV1& origin) {
  auto& log = *static_cast<CallLog*>(raw);
  log.placements.push_back(
      {candidate.name, candidate.gameplay, candidate.visual, block, origin});
  if (log.placement_error_name && candidate.name == *log.placement_error_name) {
    return RootPlacementStepStatusV1::error;
  }
  if (log.success_name && candidate.name == *log.success_name) {
    return RootPlacementStepStatusV1::success;
  }
  return RootPlacementStepStatusV1::candidate_failed;
}

RootRuleRuntimeCallbacksV1 callbacks_for(CallLog& log) {
  return {&log, lookup, place_root_and_step};
}

const char* kRulesXml =
    "<rules target=\"host\" folder=\"test\">"
    "<list name=\"roots\">"
    "<elem name=\"crypt_a\" gameplay=\"a.mgp\" visual=\"a.mvp\"/>"
    "<elem name=\"crypt_b\" gameplay=\"b.mgp\" visual=\"b.mvp\"/>"
    "<elem name=\"crypt_c\" gameplay=\"c.mgp\" visual=\"c.mvp\"/>"
    "<elem name=\"crypt_d\" gameplay=\"d.mgp\" visual=\"d.mvp\"/>"
    "</list><RootRule name=\"#roots\"/></rules>";

}  // namespace

int main() {
  bool ok = true;
  const auto parsed = parse_crypt_rule_v1(kRulesXml);
  ok &= check(static_cast<bool>(parsed), "parse four-entry root fixture");
  if (!parsed) {
    std::cerr << parsed.error << '\n';
    return 1;
  }
  const auto planned = build_rule_plan_v1(*parsed.document, 20261007u);
  ok &= check(static_cast<bool>(planned), "build root candidate plan");
  if (!planned) {
    std::cerr << planned.error << '\n';
    return 1;
  }
  const auto candidates = planned.plan->root_candidate_order;
  ok &= check(candidates.size() == 4, "plan has all four candidates");

  CallLog success_log;
  for (const auto& candidate : candidates) success_log.blocks.emplace(candidate.name, 1);
  success_log.missing_name = candidates[0].name;
  success_log.success_name = candidates[2].name;
  success_log.placement_error_name = "never";
  const auto success = run_root_rule_candidates_v1(candidates,
                                                   callbacks_for(success_log));
  ok &= check(success.status == RootRuleRuntimeStatusV1::success &&
                  success.successful_candidate_index == 2 &&
                  success.lookup_count == 3 && success.lookup_miss_count == 1 &&
                  success.placement_step_count == 2,
              "skip first lookup miss, continue failed placement, stop at first success");
  ok &= check(success_log.lookups.size() == 3 &&
                  success_log.lookups[0] == candidates[0].name &&
                  success_log.lookups[1] == candidates[1].name &&
                  success_log.lookups[2] == candidates[2].name &&
                  success_log.placements.size() == 2 &&
                  success_log.placements[0].name == candidates[1].name &&
                  success_log.placements[1].name == candidates[2].name,
              "lookup and placement callbacks preserve plan order");
  ok &= check(success_log.placements.size() == 2 &&
                  success_log.placements[0].block ==
                      &success_log.blocks.at(candidates[1].name) &&
                  success_log.placements[1].block ==
                      &success_log.blocks.at(candidates[2].name) &&
                  success_log.placements[1].gameplay == candidates[2].gameplay &&
                  success_log.placements[1].visual == candidates[2].visual,
              "found block handle and candidate metadata reach placement callback");
  ok &= check(success_log.placements.size() == 2 &&
                  success_log.placements[0].origin.x == 0.0f &&
                  success_log.placements[0].origin.y == 0.0f &&
                  success_log.placements[0].origin.z == 0.0f &&
                  success_log.placements[1].origin.x == 0.0f &&
                  success_log.placements[1].origin.y == 0.0f &&
                  success_log.placements[1].origin.z == 0.0f,
              "PlaceRootTile origin is passed as exactly (0,0,0)");
  ok &= check(planned.plan->root_candidate_order.size() == candidates.size() &&
                  planned.plan->root_candidate_order[0].name == candidates[0].name &&
                  planned.plan->root_candidate_order[3].name == candidates[3].name,
              "runtime does not reorder the plan candidate list");

  CallLog exhausted_log;
  for (const auto& candidate : candidates) exhausted_log.blocks.emplace(candidate.name, 1);
  exhausted_log.placement_error_name = "never";
  const auto exhausted = run_root_rule_candidates_v1(
      candidates, callbacks_for(exhausted_log));
  ok &= check(exhausted.status == RootRuleRuntimeStatusV1::exhausted &&
                  exhausted.lookup_count == candidates.size() &&
                  exhausted.placement_step_count == candidates.size() &&
                  !exhausted.successful_candidate_index,
              "ordinary placement failure tries every candidate then exhausts");

  CallLog lookup_error_log;
  for (const auto& candidate : candidates) lookup_error_log.blocks.emplace(candidate.name, 1);
  lookup_error_log.lookup_error_name = candidates[1].name;
  const auto lookup_error = run_root_rule_candidates_v1(
      candidates, callbacks_for(lookup_error_log));
  ok &= check(lookup_error.status == RootRuleRuntimeStatusV1::callback_error &&
                  lookup_error.callback_failure_candidate_index == 1 &&
                  lookup_error.lookup_count == 2 &&
                  lookup_error.placement_step_count == 1,
              "lookup callback error stops the candidate loop");

  CallLog null_handle_log;
  null_handle_log.found_null_name = candidates[0].name;
  const auto null_handle = run_root_rule_candidates_v1(
      candidates, callbacks_for(null_handle_log));
  ok &= check(null_handle.status == RootRuleRuntimeStatusV1::callback_error &&
                  null_handle.callback_failure_candidate_index == 0 &&
                  null_handle.placement_step_count == 0,
              "found-with-null block handle is rejected before placement callback");

  CallLog placement_error_log;
  for (const auto& candidate : candidates) placement_error_log.blocks.emplace(candidate.name, 1);
  placement_error_log.placement_error_name = candidates[1].name;
  const auto placement_error = run_root_rule_candidates_v1(
      candidates, callbacks_for(placement_error_log));
  ok &= check(placement_error.status == RootRuleRuntimeStatusV1::callback_error &&
                  placement_error.callback_failure_candidate_index == 1 &&
                  placement_error.placement_step_count == 2,
              "placement callback error stops instead of masquerading as candidate failure");

  const auto empty_candidates = run_root_rule_candidates_v1(
      {}, callbacks_for(exhausted_log));
  ok &= check(empty_candidates.status == RootRuleRuntimeStatusV1::no_candidates &&
                  empty_candidates.lookup_count == 0,
              "empty plan returns no-candidates without callbacks");
  const RootRuleRuntimeCallbacksV1 invalid_callbacks{&exhausted_log, nullptr,
                                                      place_root_and_step};
  const auto invalid = run_root_rule_candidates_v1(candidates, invalid_callbacks);
  ok &= check(invalid.status == RootRuleRuntimeStatusV1::invalid_callbacks &&
                  invalid.lookup_count == 0,
              "missing required callback is rejected without side effects");

  const std::vector<CryptListEntryV1> empty_name_candidate = {{"", "", ""}};
  CallLog empty_name_log;
  empty_name_log.missing_name = "";
  const auto empty_name = run_root_rule_candidates_v1(
      empty_name_candidate, callbacks_for(empty_name_log));
  ok &= check(empty_name.status == RootRuleRuntimeStatusV1::exhausted &&
                  empty_name_log.lookups.size() == 1 &&
                  empty_name_log.lookups[0].empty() &&
                  empty_name.placement_step_count == 0,
              "empty block name follows source lookup-miss path");

  std::cout << "PASS: " << checks << " RootRule runtime checks\n";
  return ok ? 0 : 1;
}
