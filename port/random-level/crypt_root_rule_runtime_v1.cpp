#include "crypt_root_rule_runtime_v1.hpp"

namespace dh2::random_level {

RootRuleRuntimeResultV1 run_root_rule_candidates_v1(
    const std::vector<CryptListEntryV1>& source_ordered_candidates,
    const RootRuleRuntimeCallbacksV1& callbacks) {
  RootRuleRuntimeResultV1 result;
  if (!callbacks.lookup_block || !callbacks.place_root_and_step) {
    result.status = RootRuleRuntimeStatusV1::invalid_callbacks;
    return result;
  }
  if (source_ordered_candidates.empty()) {
    result.status = RootRuleRuntimeStatusV1::no_candidates;
    return result;
  }

  // RootRule::Impl::Generate does not shuffle again here: candidates passed
  // from build_rule_plan_v1 are already in native order.
  const RootPlacementOriginV1 origin{};
  for (std::size_t i = 0; i < source_ordered_candidates.size(); ++i) {
    const auto& candidate = source_ordered_candidates[i];
    ++result.lookup_count;
    const auto lookup = callbacks.lookup_block(callbacks.context, candidate.name);
    switch (lookup.status) {
      case RootBlockLookupStatusV1::missing:
        ++result.lookup_miss_count;
        continue;
      case RootBlockLookupStatusV1::error:
        result.status = RootRuleRuntimeStatusV1::callback_error;
        result.callback_failure_candidate_index = i;
        return result;
      case RootBlockLookupStatusV1::found:
        if (!lookup.block) {
          result.status = RootRuleRuntimeStatusV1::callback_error;
          result.callback_failure_candidate_index = i;
          return result;
        }
        break;
      default:
        result.status = RootRuleRuntimeStatusV1::callback_error;
        result.callback_failure_candidate_index = i;
        return result;
    }

    ++result.placement_step_count;
    const auto attempt = callbacks.place_root_and_step(
        callbacks.context, lookup.block, candidate, origin);
    switch (attempt) {
      case RootPlacementStepStatusV1::success:
        result.status = RootRuleRuntimeStatusV1::success;
        result.successful_candidate_index = i;
        return result;
      case RootPlacementStepStatusV1::candidate_failed:
        break;
      case RootPlacementStepStatusV1::error:
      default:
        result.status = RootRuleRuntimeStatusV1::callback_error;
        result.callback_failure_candidate_index = i;
        return result;
    }
  }

  result.status = RootRuleRuntimeStatusV1::exhausted;
  return result;
}

}  // namespace dh2::random_level
