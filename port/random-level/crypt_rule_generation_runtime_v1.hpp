#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include "crypt_candidate_selection_v1.hpp"
#include "crypt_path_runtime_v1.hpp"
#include "crypt_root_rule_runtime_v1.hpp"
#include "crypt_rule_distribution_v1.hpp"
#include "crypt_tile_map_v1.hpp"

namespace dh2::random_level {

struct CryptRuleExecutionFrameV1 {
  // Opaque native/host rule implementation and tile handles. `child_rule_count`
  // is the number of child Rule::Impl objects the source Step can instantiate.
  const void* rule = nullptr;
  const void* tile = nullptr;
  std::optional<std::size_t> incoming_exit_index;
  std::size_t child_rule_count = 0;
  // Rule::Impl::Step copies the ListElem paired with the assigned open exit
  // onto the child rule before calling its OneStep. Those pairs are created
  // with ListElem() in Step, so this is normally the default empty element;
  // derived filters may inspect it while enumerating their own candidates.
  SourceListElemV1 assigned_elem;
  // Non-null only for Path::Impl. The pointed-to state belongs to this rule
  // instance and is initialized by create_child_rule, including RNG draws.
  CryptPathRuntimeV1* path_runtime = nullptr;
};

enum class CryptRuleGenerationStatusV1 {
  success,
  failed,
  unsupported,
  malformed_record,
  rng_failure,
  invalid_callbacks,
  callback_error,
};

struct CryptRuleGenerationResultV1 {
  CryptRuleGenerationStatusV1 status =
      CryptRuleGenerationStatusV1::invalid_callbacks;
  std::size_t rows_tried = 0;
  std::size_t one_step_candidates_tried = 0;
  std::size_t spawned_tiles = 0;
  std::size_t recursive_steps = 0;
  std::size_t row_rollbacks = 0;
};

enum class CryptRuleCallbackStatusV1 { ready, unsupported, error };
enum class CryptRuleChildCreateStatusV1 { created, missing, unsupported, error };
enum class CryptRuleBlockLookupStatusV1 { found, missing, unsupported, error };
enum class CryptRuleSpawnStatusV1 { spawned, rejected, unsupported, error };

struct CryptRuleSpawnedTileV1 {
  const void* tile = nullptr;
  // Opaque placement token passed unchanged to unspawn. It may contain the
  // TileMap owner and enough native state to reverse TrySpawn.
  const void* token = nullptr;
};

struct CryptRuleGenerationCallbacksV1 {
  void* context = nullptr;

  // Enumerate this tile's exits in source order. The executor removes the
  // incoming exit by source index before Rule::Impl::Step's first shuffle.
  CryptRuleCallbackStatusV1 (*step_exits)(
      void* context, const CryptRuleExecutionFrameV1& frame,
      std::vector<SourceRuleExitCandidateV1>& exits) = nullptr;

  // Mirrors NewImpl. It may consume the same RandomGeneratorV1 used by every
  // nested frame (for example Path length selection). Return missing when the
  // native implementation yields null; the current row then fails.
  CryptRuleChildCreateStatusV1 (*create_child_rule)(
      void* context, const CryptRuleExecutionFrameV1& parent,
      std::size_t child_rule_index, RandomGeneratorV1& random,
      CryptRuleExecutionFrameV1& child) = nullptr;

  // Resolve one complete candidate tuple after the child derived-rule callback
  // has enumerated it. The implementation should use its ListElem's exact
  // (name, gameplay, visual) lookup semantics and may also use the paired exit
  // metadata to identify the source block/MGX.
  CryptRuleBlockLookupStatusV1 (*resolve_block)(
      void* context, const SourceRuleExitCandidateV1& candidate,
      const void** block) = nullptr;

  // Return the child rule's derived-filtered (Exit*, ListElem) candidates in
  // source order. Enumeration is against the current parent tile/rule; each
  // candidate's own ListElem is resolved separately before Path checks or
  // TrySpawn. The executor then applies Rule::Impl::FilterExits' `_start`
  // removal and random shuffle.
  CryptRuleCallbackStatusV1 (*candidate_exits)(
      void* context, const CryptRuleExecutionFrameV1& child,
      std::size_t anchor_exit_index,
      std::vector<SourceRuleExitCandidateV1>& exits) = nullptr;

  // Path::Impl obtains the opposite direction for the anchor exit before its
  // first candidate. This must return the native Direction::sDirections key.
  CryptRuleCallbackStatusV1 (*opposite_anchor_direction)(
      void* context, const CryptRuleExecutionFrameV1& child,
      std::size_t anchor_exit_index,
      CryptDirectionKeyV1& opposite_direction) = nullptr;

  // For Path::Impl, enumerate every direction on the candidate module (not
  // just the filtered exit vector); the same count controls the one-exit
  // terminal branch. TileMap placement remains in this callback and uses MGX
  // width/height occupancy, as in Tile::TrySpawn -> FitsInMap.
  CryptRuleCallbackStatusV1 (*module_exit_directions)(
      void* context, const void* block,
      std::vector<CryptDirectionKeyV1>& directions) = nullptr;

  CryptRuleSpawnStatusV1 (*try_spawn)(
      void* context, const CryptRuleExecutionFrameV1& parent,
      std::size_t anchor_exit_index, const void* block,
      const SourceRuleExitCandidateV1& candidate,
      CryptRuleSpawnedTileV1& spawned) = nullptr;

  // This is the recursive Rule::Impl::Step call. It receives the same shared
  // RNG and a child frame with the spawned tile and candidate exit as incoming.
  CryptRuleGenerationResultV1 (*recurse_child)(
      void* context, const CryptRuleExecutionFrameV1& child,
      RandomGeneratorV1& random) = nullptr;

  // Failed OneStep candidates immediately call Tile::Unspawn. Failed Step rows
  // destroy child impls in reverse creation order, then RemoveNeighbors.
  bool (*unspawn)(void* context, const CryptRuleSpawnedTileV1& spawned) = nullptr;
  bool (*destroy_child_rule)(
      void* context, const CryptRuleExecutionFrameV1& child) = nullptr;
  bool (*remove_neighbors)(void* context, const void* tile) = nullptr;

  // Path::Impl calls Rule::Impl::Step on the current tile when its chosen
  // length is reached, using a source-specific incoming-exit choice. Supply
  // that exact branch here; omission returns `unsupported` instead of guessing.
  CryptRuleGenerationResultV1 (*path_length_reached_step)(
      void* context, const CryptRuleExecutionFrameV1& child,
      std::size_t anchor_exit_index, RandomGeneratorV1& random) = nullptr;
};

// One recursive Rule::Impl::Step / Rule::Impl::OneStep slice. The callbacks are
// intentionally explicit around binary-dependent object and tile operations;
// this kernel owns source ordering, shared-RNG shuffles, path counter rollback,
// and failed-row cleanup. `step_exits` must expose the current tile's raw exits
// in block order. Unsupported callback branches stop without fabricating a
// successful generation result.
CryptRuleGenerationResultV1 execute_crypt_rule_step_v1(
    const CryptRuleExecutionFrameV1& frame,
    const CryptRuleGenerationCallbacksV1& callbacks,
    RandomGeneratorV1& random);

// Root selection stays delegated to the already source-backed RootRule helper;
// its place_root_and_step callback can place the root and call the executor.
RootRuleRuntimeResultV1 execute_crypt_root_rule_v1(
    const std::vector<CryptListEntryV1>& source_ordered_candidates,
    const RootRuleRuntimeCallbacksV1& callbacks);

}  // namespace dh2::random_level
