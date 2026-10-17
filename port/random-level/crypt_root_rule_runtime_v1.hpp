#pragma once

#include <cstddef>
#include <optional>
#include <string_view>
#include <vector>

#include "native_rule_plan_v1.hpp"

namespace dh2::random_level {

enum class RootBlockLookupStatusV1 { found, missing, error };

struct RootBlockLookupResultV1 {
  RootBlockLookupStatusV1 status = RootBlockLookupStatusV1::error;
  const void* block = nullptr;
};

enum class RootPlacementStepStatusV1 { success, candidate_failed, error };

struct RootPlacementOriginV1 {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
};

struct RootRuleRuntimeCallbacksV1 {
  void* context = nullptr;
  RootBlockLookupResultV1 (*lookup_block)(void* context,
                                          std::string_view name) = nullptr;
  RootPlacementStepStatusV1 (*place_root_and_step)(
      void* context, const void* block, const CryptListEntryV1& candidate,
      const RootPlacementOriginV1& origin) = nullptr;
};

enum class RootRuleRuntimeStatusV1 {
  success,
  no_candidates,
  exhausted,
  invalid_callbacks,
  callback_error,
};

struct RootRuleRuntimeResultV1 {
  RootRuleRuntimeStatusV1 status = RootRuleRuntimeStatusV1::invalid_callbacks;
  std::size_t lookup_count = 0;
  std::size_t lookup_miss_count = 0;
  std::size_t placement_step_count = 0;
  std::optional<std::size_t> successful_candidate_index;
  std::optional<std::size_t> callback_failure_candidate_index;
};

// Source boundary: RootRule::Impl::Generate @ 0x48e8ac resolves each candidate
// with Rule::GetBlock, skips lookup misses, then attempts PlaceRootTile in list
// order until it succeeds. PlaceRootTile @ 0x48c374 creates the root Tile,
// places it at (0,0,0), and invokes the rule Step callback. The plan has already
// applied the native candidate shuffle; this wrapper consumes no RNG.
RootRuleRuntimeResultV1 run_root_rule_candidates_v1(
    const std::vector<CryptListEntryV1>& source_ordered_candidates,
    const RootRuleRuntimeCallbacksV1& callbacks);

}  // namespace dh2::random_level
