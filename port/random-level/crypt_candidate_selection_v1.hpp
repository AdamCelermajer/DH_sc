#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "crypt_rule_distribution_v1.hpp"
#include "native_rule_plan_v1.hpp"

namespace dh2::random_level {

// Bounded copy of rnd::ListElem (IDA: ListElem constructors at 0x48e080,
// 0x48e128, copy at 0x48e280, assignment at 0x48c0bc, LoadFromXml at
// 0x48c51c). The owner is opaque in the host projection; copies preserve it.
// Native XML defaults chances to 100. Candidate filtering only carries this
// value; no weighting behavior is inferred here.
struct SourceListElemV1 {
  std::uint64_t owner_token = 0;
  std::string name;
  std::string gameplay;
  std::string visual;
  std::int32_t chances = 100;
};

SourceListElemV1 make_source_list_elem_v1(const CryptListEntryV1& entry,
                                         std::uint64_t owner_token = 0);

// IDA: rnd::BlockSearch::operator() @ 0x48c638 compares name, gameplay, and
// visual byte-for-byte, including their lengths.
bool source_block_search_matches_v1(const SourceListElemV1& entry,
                                    std::string_view name,
                                    std::string_view gameplay,
                                    std::string_view visual);

struct SourceListFindResultV1 {
  bool found = false;
  std::size_t source_index = 0;
  SourceListElemV1 elem;
};

// IDA: rnd::ListRule::Find @ 0x4905d0 returns the first exact triple match.
// If consume=true, it erases that element only when replacement is false.
SourceListFindResultV1 find_source_list_elem_v1(
    std::vector<SourceListElemV1>& entries, bool replacement, bool consume,
    std::string_view name, std::string_view gameplay, std::string_view visual);

struct SourceRuleExitCandidateV1 {
  std::size_t source_exit_index = 0;
  std::string destination_block_name;
  SourceListElemV1 elem;
};

// ForceBlock/Path list-backed branches iterate exits first, then ListElem
// entries; they compare destination block name with ListElem.name exactly and
// copy the complete matching ListElem. A fixed index models mIndex != -1;
// nullopt models the source-order scan of all entries. Duplicate matches are
// retained.
std::vector<SourceRuleExitCandidateV1> match_source_list_names_v1(
    const std::vector<SourceRuleExitCandidateV1>& exits,
    const std::vector<SourceListElemV1>& entries,
    std::optional<std::size_t> fixed_entry_index = std::nullopt);

// ForceBlock's no-ListRule branch compares configured names case-insensitively
// and constructs a default ListElem for each match. This ASCII fold matches
// the shipped Crypt identifiers; non-ASCII locale behavior is outside scope.
std::vector<SourceRuleExitCandidateV1> match_source_block_names_v1(
    const std::vector<SourceRuleExitCandidateV1>& exits,
    const std::vector<std::string>& block_names);

// IDA: Rule::Impl::FilterExits @ 0x48e690 removes any candidate whose target
// block name contains case-sensitive "_start", then random-shuffles the
// remaining pair vector. This is the derived-filter output stage; callers
// should pass its source-ordered candidates before this common base filter.
bool filter_and_shuffle_rule_impl_exits_v1(
    std::vector<SourceRuleExitCandidateV1>& candidates,
    RandomGeneratorV1& random);

enum class SourceRuleStepCandidateStatusV1 {
  ready,
  bypass_success,       // Rule::Impl::Step returns success if either count is 0.
  unsupported_catalog,
  malformed_record,
  rng_failure,
};

struct SourceRuleAssignmentV1 {
  std::size_t source_exit_index = 0;
  std::size_t child_rule_index = 0;
  SourceListElemV1 elem;
};

struct SourceRuleAssignmentRowV1 {
  std::size_t source_record_index = 0;
  std::array<std::uint8_t, 6> source_record{};
  // The native loop assigns each record byte to the exit at the same position
  // in the already-shuffled exit vector.
  std::vector<SourceRuleAssignmentV1> assignments;
};

struct SourceRuleStepCandidatesV1 {
  SourceRuleStepCandidateStatusV1 status =
      SourceRuleStepCandidateStatusV1::unsupported_catalog;
  // Rule::Impl::Step shuffles its open exits first, then shuffles pointers to
  // source table rows. These vectors expose both source orders for auditing.
  std::vector<SourceRuleExitCandidateV1> shuffled_exits;
  std::vector<std::size_t> shuffled_record_indices;
  std::vector<SourceRuleAssignmentRowV1> rows;
};

// Bounded Rule::Impl::Step candidate stage (IDA @ 0x48f954). Input exits must
// already have the incoming exit removed, in native block order, and catalog
// must have been read for exactly (exits.size(), child_rule_count). This
// function performs the source exit shuffle followed by the source row shuffle
// and exposes the byte-to-exit assignment. It does not model child NewImpl,
// TrySpawn, recursion, or Unspawn.
SourceRuleStepCandidatesV1 prepare_source_rule_step_candidates_v1(
    std::vector<SourceRuleExitCandidateV1> exits,
    std::size_t child_rule_count,
    const RuleDistributionCatalogV1& catalog,
    RandomGeneratorV1& random);

}  // namespace dh2::random_level
