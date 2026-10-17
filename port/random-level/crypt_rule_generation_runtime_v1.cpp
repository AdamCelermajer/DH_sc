#include "crypt_rule_generation_runtime_v1.hpp"

#include <algorithm>

namespace dh2::random_level {
namespace {

using Result = CryptRuleGenerationResultV1;
using Status = CryptRuleGenerationStatusV1;

Result make_result(Status status) {
  Result result;
  result.status = status;
  return result;
}

void add_counts(Result& target, const Result& source) {
  target.rows_tried += source.rows_tried;
  target.one_step_candidates_tried += source.one_step_candidates_tried;
  target.spawned_tiles += source.spawned_tiles;
  target.recursive_steps += source.recursive_steps;
  target.row_rollbacks += source.row_rollbacks;
}

Status callback_status(CryptRuleCallbackStatusV1 status) {
  switch (status) {
    case CryptRuleCallbackStatusV1::ready:
      return Status::success;
    case CryptRuleCallbackStatusV1::unsupported:
      return Status::unsupported;
    case CryptRuleCallbackStatusV1::error:
    default:
      return Status::callback_error;
  }
}

Result execute_one_step(const CryptRuleExecutionFrameV1& child_rule,
                        const CryptRuleExecutionFrameV1& parent,
                        const SourceRuleAssignmentV1& assignment,
                        const CryptRuleGenerationCallbacksV1& callbacks,
                        RandomGeneratorV1& random) {
  if (child_rule.path_runtime &&
      child_rule.path_runtime->length_reached()) {
    if (!callbacks.path_length_reached_step) {
      return make_result(Status::unsupported);
    }
    auto terminal = callbacks.path_length_reached_step(
        callbacks.context, child_rule, assignment.source_exit_index, random);
    ++terminal.recursive_steps;
    return terminal;
  }

  CryptDirectionKeyV1 opposite_direction = kCryptNoDirectionV1;
  if (child_rule.path_runtime) {
    if (!callbacks.opposite_anchor_direction ||
        !callbacks.module_exit_directions) {
      return make_result(Status::unsupported);
    }
    const auto direction_status = callbacks.opposite_anchor_direction(
        callbacks.context, child_rule, assignment.source_exit_index,
        opposite_direction);
    if (direction_status != CryptRuleCallbackStatusV1::ready) {
      return make_result(callback_status(direction_status));
    }
    child_rule.path_runtime->begin_step(opposite_direction);
  }

  std::vector<SourceRuleExitCandidateV1> candidates;
  const auto candidates_status = callbacks.candidate_exits(
      callbacks.context, child_rule, assignment.source_exit_index, candidates);
  if (candidates_status != CryptRuleCallbackStatusV1::ready) {
    return make_result(callback_status(candidates_status));
  }

  // This is the common tail of Rule::Impl::FilterExits: remove `_start`
  // destinations, then consume the same shared RNG to shuffle the vector.
  if (!filter_and_shuffle_rule_impl_exits_v1(candidates, random)) {
    return make_result(Status::rng_failure);
  }
  if (candidates.empty()) return make_result(Status::failed);

  Result result = make_result(Status::failed);
  for (const auto& candidate : candidates) {
    ++result.one_step_candidates_tried;

    // OneStep's derived FilterExits returns candidate (Exit*, ListElem) pairs.
    // Resolve the tuple for this candidate only; different candidates can
    // refer to MGXs with different exits, which Path::Impl inspects separately.
    const void* block = nullptr;
    switch (callbacks.resolve_block(callbacks.context, candidate,
                                    &block)) {
      case CryptRuleBlockLookupStatusV1::missing:
        // The native candidate contains a concrete Exit/Block pair. If the
        // adapter cannot resolve that source tuple, continuing would invent a
        // candidate order or successful fallback that the source cannot take.
        return make_result(Status::malformed_record);
      case CryptRuleBlockLookupStatusV1::unsupported:
        return make_result(Status::unsupported);
      case CryptRuleBlockLookupStatusV1::error:
        return make_result(Status::callback_error);
      case CryptRuleBlockLookupStatusV1::found:
        if (!block) return make_result(Status::callback_error);
        break;
      default:
        return make_result(Status::callback_error);
    }

    std::vector<CryptDirectionKeyV1> module_directions;
    if (child_rule.path_runtime) {
      const auto directions_status = callbacks.module_exit_directions(
          callbacks.context, block, module_directions);
      if (directions_status != CryptRuleCallbackStatusV1::ready) {
        return make_result(callback_status(directions_status));
      }
      if (!child_rule.path_runtime->candidate_direction_allows_spawn(
              module_directions, opposite_direction)) {
        continue;
      }
    }

    CryptRuleSpawnedTileV1 spawned;
    switch (callbacks.try_spawn(callbacks.context, parent,
                                assignment.source_exit_index, block,
                                candidate, spawned)) {
      case CryptRuleSpawnStatusV1::rejected:
        continue;
      case CryptRuleSpawnStatusV1::unsupported:
        return make_result(Status::unsupported);
      case CryptRuleSpawnStatusV1::error:
        return make_result(Status::callback_error);
      case CryptRuleSpawnStatusV1::spawned:
        if (!spawned.tile) return make_result(Status::callback_error);
        break;
      default:
        return make_result(Status::callback_error);
    }
    ++result.spawned_tiles;

    if (child_rule.path_runtime) {
      const auto attempt = child_rule.path_runtime->begin_child_attempt(
          module_directions.size());
      if (attempt == CryptPathChildAttemptV1::terminal_one_exit) {
        result.status = Status::success;
        return result;
      }
    }

    CryptRuleExecutionFrameV1 spawned_frame = child_rule;
    spawned_frame.tile = spawned.tile;
    spawned_frame.incoming_exit_index = candidate.source_exit_index;
    ++result.recursive_steps;
    auto nested = callbacks.recurse_child(callbacks.context, spawned_frame,
                                          random);
    add_counts(result, nested);
    if (nested.status == Status::success) {
      result.status = Status::success;
      return result;
    }

    if (child_rule.path_runtime) {
      child_rule.path_runtime->rollback_failed_child_attempt(
          CryptPathChildAttemptV1::recurse);
    }
    if (!callbacks.unspawn(callbacks.context, spawned)) {
      result.status = Status::callback_error;
      return result;
    }

    if (nested.status != Status::failed) {
      result.status = nested.status;
      return result;
    }
  }
  return result;
}

bool cleanup_failed_row(const CryptRuleExecutionFrameV1& parent,
                        const std::vector<CryptRuleExecutionFrameV1>& children,
                        const CryptRuleGenerationCallbacksV1& callbacks) {
  bool cleanup_ok = true;
  for (auto child = children.rbegin(); child != children.rend(); ++child) {
    if (!callbacks.destroy_child_rule(callbacks.context, *child)) {
      cleanup_ok = false;
    }
  }
  if (!callbacks.remove_neighbors(callbacks.context, parent.tile)) {
    cleanup_ok = false;
  }
  return cleanup_ok;
}

}  // namespace

CryptRuleGenerationResultV1 execute_crypt_rule_step_v1(
    const CryptRuleExecutionFrameV1& frame,
    const CryptRuleGenerationCallbacksV1& callbacks,
    RandomGeneratorV1& random) {
  if (!frame.rule || !frame.tile || !callbacks.step_exits ||
      !callbacks.create_child_rule || !callbacks.resolve_block ||
      !callbacks.candidate_exits || !callbacks.try_spawn ||
      !callbacks.recurse_child || !callbacks.unspawn ||
      !callbacks.destroy_child_rule || !callbacks.remove_neighbors) {
    return make_result(Status::invalid_callbacks);
  }

  std::vector<SourceRuleExitCandidateV1> exits;
  const auto exits_status =
      callbacks.step_exits(callbacks.context, frame, exits);
  if (exits_status != CryptRuleCallbackStatusV1::ready) {
    return make_result(callback_status(exits_status));
  }
  // Rule::Impl::Step creates (exit, ListElem()) pairs from raw tile exits. Any
  // candidate-facing tuple belongs to the child's derived FilterExits later.
  for (auto& exit : exits) exit.elem = SourceListElemV1{};
  if (frame.incoming_exit_index) {
    exits.erase(std::remove_if(exits.begin(), exits.end(), [&](const auto& exit) {
                  return exit.source_exit_index ==
                         *frame.incoming_exit_index;
                }),
                exits.end());
  }

  const auto catalog = enumerate_source_rule_distributions_v1(
      exits.size(), frame.child_rule_count);
  const auto prepared = prepare_source_rule_step_candidates_v1(
      std::move(exits), frame.child_rule_count, catalog, random);
  switch (prepared.status) {
    case SourceRuleStepCandidateStatusV1::bypass_success:
      return make_result(Status::success);
    case SourceRuleStepCandidateStatusV1::rng_failure:
      return make_result(Status::rng_failure);
    case SourceRuleStepCandidateStatusV1::unsupported_catalog:
      return make_result(Status::unsupported);
    case SourceRuleStepCandidateStatusV1::malformed_record:
      return make_result(Status::malformed_record);
    case SourceRuleStepCandidateStatusV1::ready:
      break;
    default:
      return make_result(Status::unsupported);
  }

  Result result = make_result(Status::failed);
  for (const auto& row : prepared.rows) {
    ++result.rows_tried;
    std::vector<CryptRuleExecutionFrameV1> row_children;
    row_children.reserve(row.assignments.size());
    Status row_failure = Status::failed;
    bool row_succeeded = true;

    for (const auto& assignment : row.assignments) {
      CryptRuleExecutionFrameV1 child;
      switch (callbacks.create_child_rule(callbacks.context, frame,
                                          assignment.child_rule_index, random,
                                          child)) {
        case CryptRuleChildCreateStatusV1::missing:
          row_succeeded = false;
          row_failure = Status::failed;
          break;
        case CryptRuleChildCreateStatusV1::unsupported:
          row_succeeded = false;
          row_failure = Status::unsupported;
          break;
        case CryptRuleChildCreateStatusV1::error:
          row_succeeded = false;
          row_failure = Status::callback_error;
          break;
        case CryptRuleChildCreateStatusV1::created:
          if (!child.rule) {
            row_succeeded = false;
            row_failure = Status::callback_error;
            break;
          }
          child.tile = frame.tile;
          child.incoming_exit_index = frame.incoming_exit_index;
          // Step constructs each open-exit pair with ListElem() before
          // assigning the pair to the child Rule::Impl.
          child.assigned_elem = assignment.elem;
          row_children.push_back(child);
          break;
        default:
          row_succeeded = false;
          row_failure = Status::callback_error;
          break;
      }
      if (!row_succeeded) break;

      auto child_result = execute_one_step(child, frame, assignment, callbacks,
                                           random);
      add_counts(result, child_result);
      if (child_result.status != Status::success) {
        row_succeeded = false;
        row_failure = child_result.status;
        break;
      }
    }

    if (row_succeeded) {
      result.status = Status::success;
      return result;
    }

    ++result.row_rollbacks;
    if (!cleanup_failed_row(frame, row_children, callbacks)) {
      result.status = Status::callback_error;
      return result;
    }
    if (row_failure != Status::failed) {
      result.status = row_failure;
      return result;
    }
  }
  return result;
}

RootRuleRuntimeResultV1 execute_crypt_root_rule_v1(
    const std::vector<CryptListEntryV1>& source_ordered_candidates,
    const RootRuleRuntimeCallbacksV1& callbacks) {
  return run_root_rule_candidates_v1(source_ordered_candidates, callbacks);
}

}  // namespace dh2::random_level
