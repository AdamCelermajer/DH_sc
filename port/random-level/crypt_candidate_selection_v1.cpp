#include "crypt_candidate_selection_v1.hpp"

#include <algorithm>
#include <cctype>
#include <numeric>

namespace dh2::random_level {
namespace {

bool ascii_iequals(std::string_view left, std::string_view right) {
  if (left.size() != right.size()) return false;
  for (std::size_t i = 0; i < left.size(); ++i) {
    const auto l = static_cast<unsigned char>(left[i]);
    const auto r = static_cast<unsigned char>(right[i]);
    if (std::tolower(l) != std::tolower(r)) return false;
  }
  return true;
}

}  // namespace

SourceListElemV1 make_source_list_elem_v1(const CryptListEntryV1& entry,
                                         std::uint64_t owner_token) {
  return {owner_token, entry.name, entry.gameplay, entry.visual, 100};
}

bool source_block_search_matches_v1(const SourceListElemV1& entry,
                                    std::string_view name,
                                    std::string_view gameplay,
                                    std::string_view visual) {
  return entry.name == name && entry.gameplay == gameplay &&
         entry.visual == visual;
}

SourceListFindResultV1 find_source_list_elem_v1(
    std::vector<SourceListElemV1>& entries, bool replacement, bool consume,
    std::string_view name, std::string_view gameplay, std::string_view visual) {
  SourceListFindResultV1 result;
  for (std::size_t i = 0; i < entries.size(); ++i) {
    if (!source_block_search_matches_v1(entries[i], name, gameplay, visual)) {
      continue;
    }
    result.found = true;
    result.source_index = i;
    result.elem = entries[i];
    if (consume && !replacement) entries.erase(entries.begin() + i);
    return result;
  }
  return result;
}

std::vector<SourceRuleExitCandidateV1> match_source_list_names_v1(
    const std::vector<SourceRuleExitCandidateV1>& exits,
    const std::vector<SourceListElemV1>& entries,
    std::optional<std::size_t> fixed_entry_index) {
  std::vector<SourceRuleExitCandidateV1> result;
  if (fixed_entry_index && *fixed_entry_index >= entries.size()) return result;

  for (const auto& exit : exits) {
    const auto first = fixed_entry_index.value_or(0);
    const auto end = fixed_entry_index ? first + 1 : entries.size();
    for (std::size_t i = first; i < end; ++i) {
      if (exit.destination_block_name != entries[i].name) continue;
      auto candidate = exit;
      candidate.elem = entries[i];
      result.push_back(std::move(candidate));
    }
  }
  return result;
}

std::vector<SourceRuleExitCandidateV1> match_source_block_names_v1(
    const std::vector<SourceRuleExitCandidateV1>& exits,
    const std::vector<std::string>& block_names) {
  std::vector<SourceRuleExitCandidateV1> result;
  for (const auto& exit : exits) {
    for (const auto& name : block_names) {
      if (!ascii_iequals(exit.destination_block_name, name)) continue;
      auto candidate = exit;
      candidate.elem = SourceListElemV1{};
      result.push_back(std::move(candidate));
    }
  }
  return result;
}

bool filter_and_shuffle_rule_impl_exits_v1(
    std::vector<SourceRuleExitCandidateV1>& candidates,
    RandomGeneratorV1& random) {
  candidates.erase(
      std::remove_if(candidates.begin(), candidates.end(), [](const auto& candidate) {
        return candidate.destination_block_name.find("_start") !=
               std::string::npos;
      }),
      candidates.end());
  return source_shuffle(candidates, random);
}

SourceRuleStepCandidatesV1 prepare_source_rule_step_candidates_v1(
    std::vector<SourceRuleExitCandidateV1> exits,
    std::size_t child_rule_count, const RuleDistributionCatalogV1& catalog,
    RandomGeneratorV1& random) {
  SourceRuleStepCandidatesV1 result;

  // Rule::Impl::Step shuffles all non-incoming block exits before checking
  // whether the current rule has children or a usable distribution row.
  if (!source_shuffle(exits, random)) {
    result.status = SourceRuleStepCandidateStatusV1::rng_failure;
    return result;
  }
  result.shuffled_exits = std::move(exits);
  if (result.shuffled_exits.empty() || child_rule_count == 0) {
    result.status = SourceRuleStepCandidateStatusV1::bypass_success;
    return result;
  }
  if (catalog.status != RuleDistributionStatusV1::ready ||
      catalog.records.empty() || result.shuffled_exits.size() > 6) {
    result.status = SourceRuleStepCandidateStatusV1::unsupported_catalog;
    return result;
  }

  result.shuffled_record_indices.resize(catalog.records.size());
  std::iota(result.shuffled_record_indices.begin(),
            result.shuffled_record_indices.end(), std::size_t{0});
  if (!source_shuffle(result.shuffled_record_indices, random)) {
    result.status = SourceRuleStepCandidateStatusV1::rng_failure;
    return result;
  }

  result.rows.reserve(result.shuffled_record_indices.size());
  for (const std::size_t record_index : result.shuffled_record_indices) {
    const auto& record = catalog.records[record_index];
    SourceRuleAssignmentRowV1 row;
    row.source_record_index = record_index;
    row.source_record = record.bytes;
    row.assignments.reserve(result.shuffled_exits.size());
    for (std::size_t exit_index = 0;
         exit_index < result.shuffled_exits.size(); ++exit_index) {
      const auto child_index = record.bytes[exit_index];
      // The native table is read as signed char and then offset into the
      // child-impl array. A negative or out-of-domain table byte is not a
      // supported assignment in this bounded host model.
      if (child_index > 0x7f || child_index >= child_rule_count) {
        result.rows.clear();
        result.status = SourceRuleStepCandidateStatusV1::malformed_record;
        return result;
      }
      const auto& exit = result.shuffled_exits[exit_index];
      row.assignments.push_back(
          {exit.source_exit_index, child_index, exit.elem});
    }
    result.rows.push_back(std::move(row));
  }

  result.status = SourceRuleStepCandidateStatusV1::ready;
  return result;
}

}  // namespace dh2::random_level
