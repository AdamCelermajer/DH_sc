#include "../crypt_candidate_selection_v1.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace dh2::random_level;

namespace {

int failures = 0;

void check(bool condition, const char* name) {
  if (!condition) {
    std::cerr << "FAIL: " << name << '\n';
    ++failures;
  }
}

SourceRuleExitCandidateV1 exit_candidate(std::size_t id, std::string name) {
  return {id, std::move(name), SourceListElemV1{}};
}

SourceListElemV1 elem(std::string name, std::string gameplay,
                      std::string visual, std::int32_t chances,
                      std::uint64_t owner = 0) {
  return {owner, std::move(name), std::move(gameplay), std::move(visual),
          chances};
}

RuleDistributionCatalogV1 catalog(
    std::initializer_list<std::array<std::uint8_t, 6>> records) {
  RuleDistributionCatalogV1 result;
  result.status = RuleDistributionStatusV1::ready;
  for (const auto& bytes : records) {
    result.records.push_back({bytes});
  }
  return result;
}

bool same_exit_order(const std::vector<SourceRuleExitCandidateV1>& left,
                     const std::vector<SourceRuleExitCandidateV1>& right) {
  if (left.size() != right.size()) return false;
  for (std::size_t i = 0; i < left.size(); ++i) {
    if (left[i].source_exit_index != right[i].source_exit_index) return false;
  }
  return true;
}

}  // namespace

int main() {
  const CryptListEntryV1 parsed{"crypt_hall", "walk", "stone"};
  const auto parsed_elem = make_source_list_elem_v1(parsed, 17);
  check(parsed_elem.owner_token == 17 && parsed_elem.chances == 100 &&
            parsed_elem.name == "crypt_hall" &&
            parsed_elem.gameplay == "walk" && parsed_elem.visual == "stone",
        "parser entry maps to native defaults and keeps owner token");

  std::vector<SourceListElemV1> list = {
      elem("crypt_hall", "walk", "stone", 35, 8),
      elem("crypt_hall", "walk", "stone", 70, 9),
      elem("crypt_hall", "run", "stone", 10, 10),
  };
  auto first = find_source_list_elem_v1(list, false, true,
                                        "crypt_hall", "walk", "stone");
  check(first.found && first.source_index == 0 && first.elem.owner_token == 8 &&
            first.elem.chances == 35 && list.size() == 2 &&
            list.front().owner_token == 9,
        "ListRule Find returns first exact triple and consumes only nonreplacement");
  const auto remaining = find_source_list_elem_v1(
      list, true, true, "crypt_hall", "walk", "stone");
  check(remaining.found && remaining.elem.chances == 70 && list.size() == 2,
        "replacement ListRule Find does not consume");
  const auto miss = find_source_list_elem_v1(
      list, true, false, "crypt_hall", "WALK", "stone");
  check(!miss.found, "ListRule triple comparison is case-sensitive");

  const std::vector<SourceRuleExitCandidateV1> exits = {
      exit_candidate(4, "crypt_hall"), exit_candidate(5, "crypt_hall"),
      exit_candidate(6, "CRYPT_HALL")};
  const std::vector<SourceListElemV1> entries = {
      elem("crypt_hall", "walk", "stone", 20, 1),
      elem("crypt_hall", "walk", "stone", 40, 2),
      elem("crypt_hall", "walk", "stone", 60, 3),
  };
  const auto all_matches = match_source_list_names_v1(exits, entries);
  check(all_matches.size() == 6 &&
            all_matches[0].source_exit_index == 4 &&
            all_matches[0].elem.owner_token == 1 &&
            all_matches[1].elem.owner_token == 2 &&
            all_matches[2].source_exit_index == 4 &&
            all_matches[3].source_exit_index == 5 &&
            all_matches[4].elem.owner_token == 2,
        "ForceBlock/Path preserve exit-outer, list-inner order and duplicates");
  const auto fixed_match = match_source_list_names_v1(exits, entries, 1);
  check(fixed_match.size() == 2 && fixed_match[0].elem.owner_token == 2 &&
            fixed_match[1].source_exit_index == 5,
        "fixed ListElem index preserves source exit order");
  const auto no_match = match_source_list_names_v1(exits, entries, 99);
  check(no_match.empty(), "invalid fixed index yields no candidate");

  const std::vector<std::string> configured_names = {"crypt_hall", "CRYPT_HALL"};
  const auto no_list_matches = match_source_block_names_v1(exits, configured_names);
  check(no_list_matches.size() == 6 && no_list_matches[0].source_exit_index == 4 &&
            no_list_matches[1].source_exit_index == 4 &&
            no_list_matches[0].elem.chances == 100 &&
            no_list_matches[0].elem.name.empty(),
        "no-ListRule names compare case-insensitively and produce default ListElem");

  std::vector<SourceRuleExitCandidateV1> filter_input = {
      exit_candidate(0, "hall"), exit_candidate(1, "crypt_start_room"),
      exit_candidate(2, "pre_start_mid"), exit_candidate(3, "crypt_Start"),
      exit_candidate(4, "_start")};
  auto expected_filtered = filter_input;
  expected_filtered.erase(
      std::remove_if(expected_filtered.begin(), expected_filtered.end(),
                     [](const auto& candidate) {
                       return candidate.destination_block_name.find("_start") !=
                              std::string::npos;
                     }),
      expected_filtered.end());
  RandomGeneratorV1 filter_rng(1234);
  RandomGeneratorV1 expected_filter_rng(1234);
  check(source_shuffle(expected_filtered, expected_filter_rng),
        "fixture source shuffle succeeds");
  check(filter_and_shuffle_rule_impl_exits_v1(filter_input, filter_rng) &&
            same_exit_order(filter_input, expected_filtered) &&
            filter_rng.state() == expected_filter_rng.state() &&
            filter_input.size() == 2,
        "Rule FilterExits removes case-sensitive _start substring then shuffles");

  const auto distributions = catalog({{{0, 1, 0, 0, 0, 0}},
                                      {{0, 1, 0, 0, 0, 0}},
                                      {{1, 0, 0, 0, 0, 0}}});
  std::vector<SourceRuleExitCandidateV1> step_exits = {
      exit_candidate(20, "left"), exit_candidate(21, "right")};
  step_exits[0].elem = elem("", "", "", 23, 1);
  step_exits[1].elem = elem("", "", "", 87, 2);
  auto expected_exits = step_exits;
  std::vector<std::size_t> expected_record_order = {0, 1, 2};
  RandomGeneratorV1 step_rng(20261007);
  RandomGeneratorV1 expected_rng(20261007);
  check(source_shuffle(expected_exits, expected_rng) &&
            source_shuffle(expected_record_order, expected_rng),
        "fixture Rule::Step shuffles succeed");
  const auto selection = prepare_source_rule_step_candidates_v1(
      step_exits, 2, distributions, step_rng);
  bool mappings_match = selection.rows.size() == expected_record_order.size();
  if (mappings_match) {
    for (std::size_t row_index = 0; row_index < selection.rows.size(); ++row_index) {
      const auto record_index = expected_record_order[row_index];
      mappings_match = mappings_match &&
          selection.rows[row_index].source_record_index == record_index &&
          selection.rows[row_index].assignments.size() == expected_exits.size();
      for (std::size_t exit_index = 0; mappings_match &&
           exit_index < expected_exits.size(); ++exit_index) {
        const auto& expected_exit = expected_exits[exit_index];
        const auto& actual = selection.rows[row_index].assignments[exit_index];
        mappings_match = actual.source_exit_index == expected_exit.source_exit_index &&
            actual.child_rule_index == distributions.records[record_index].bytes[exit_index] &&
            actual.elem.chances == expected_exit.elem.chances &&
            actual.elem.owner_token == expected_exit.elem.owner_token;
      }
    }
  }
  check(selection.status == SourceRuleStepCandidateStatusV1::ready &&
            same_exit_order(selection.shuffled_exits, expected_exits) &&
            selection.shuffled_record_indices == expected_record_order &&
            mappings_match && step_rng.state() == expected_rng.state(),
        "Rule::Step shuffles exits then row pointers and maps bytes by exit slot");
  std::size_t duplicate_row_pairs = 0;
  for (std::size_t i = 0; i < selection.rows.size(); ++i) {
    for (std::size_t j = i + 1; j < selection.rows.size(); ++j) {
      if (selection.rows[i].source_record == selection.rows[j].source_record) {
        ++duplicate_row_pairs;
      }
    }
  }
  check(selection.rows.size() == 3 && duplicate_row_pairs == 1,
        "duplicate source distribution rows remain separate candidates");

  RandomGeneratorV1 bypass_rng(51);
  RandomGeneratorV1 bypass_expected_rng(51);
  auto bypass_exits = step_exits;
  check(source_shuffle(bypass_exits, bypass_expected_rng),
        "fixture bypass shuffle succeeds");
  const auto bypass = prepare_source_rule_step_candidates_v1(
      step_exits, 0, distributions, bypass_rng);
  check(bypass.status == SourceRuleStepCandidateStatusV1::bypass_success &&
            same_exit_order(bypass.shuffled_exits, bypass_exits) &&
            bypass.rows.empty() && bypass_rng.state() == bypass_expected_rng.state(),
        "zero child rules still consumes the source exit shuffle before bypass");

  const auto malformed_catalog = catalog({{{0xff, 0, 0, 0, 0, 0}}});
  RandomGeneratorV1 malformed_rng(8);
  const auto malformed = prepare_source_rule_step_candidates_v1(
      {exit_candidate(30, "a"), exit_candidate(31, "b")}, 2,
      malformed_catalog, malformed_rng);
  check(malformed.status == SourceRuleStepCandidateStatusV1::malformed_record,
        "negative signed-char distribution values are rejected as malformed");

  if (failures) return 1;
  std::cout << "PASS: 16 source-backed candidate-selection checks\n";
  return 0;
}
