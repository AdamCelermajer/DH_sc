#include "crypt_level_generator_v1.hpp"

#include "crypt_mgx_placement_v1.hpp"
#include "crypt_path_runtime_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <string>
#include <utility>

namespace dh2::random_level {
namespace {

using GenerationStatus = CryptRuleGenerationStatusV1;

struct RuntimeTileV1 {
  const CryptModuleAssetV1* asset = nullptr;
  RuntimeTileV1* parent = nullptr;
  std::vector<RuntimeTileV1*> children;
  CryptMgxTilePlacementV1 placement;
  bool active = false;
};

struct RuntimeRuleV1 {
  const RuleNodeV1* source = nullptr;
  RuntimeRuleV1* parent = nullptr;
  CryptPathRuntimeV1 path;
  bool has_path = false;
  std::vector<std::unique_ptr<RuntimeRuleV1>> children;
};

struct GeneratedRecordStorageV1 {
  CryptGeneratedModuleV1 record;
  std::string gameplay_path;
  std::string visual_path;
};

struct GenerationContextV1 {
  const CryptRuleDocumentV1* document = nullptr;
  const CryptModuleCatalogueV1* catalogue = nullptr;
  RandomGeneratorV1* random = nullptr;
  CryptTileMapV1 tile_map;
  RuntimeRuleV1 root_rule;
  std::vector<std::unique_ptr<RuntimeTileV1>> tiles;
  RuntimeTileV1* successful_root = nullptr;
  CryptRuleGenerationResultV1 last_generation;
  bool callback_error = false;
};

CryptRuleGenerationCallbacksV1 callbacks_for(GenerationContextV1& context);

CryptRuleGenerationResultV1 generation_result(GenerationStatus status) {
  CryptRuleGenerationResultV1 result;
  result.status = status;
  return result;
}

bool identity_scale(std::string_view text) {
  if (text.empty()) return true;
  const char* cursor = text.data();
  const char* const end = text.data() + text.size();
  float values[3] = {1.0f, 1.0f, 1.0f};
  for (std::size_t index = 0; index < 3; ++index) {
    while (cursor != end && (*cursor == ' ' || *cursor == '\t')) ++cursor;
    if (cursor == end) return false;
    char* parsed_end = nullptr;
    const float value = std::strtof(cursor, &parsed_end);
    if (parsed_end == cursor || parsed_end > end || !std::isfinite(value)) {
      return false;
    }
    values[index] = value;
    cursor = parsed_end;
    while (cursor != end && (*cursor == ' ' || *cursor == '\t')) ++cursor;
    if (index != 2) {
      if (cursor == end || *cursor != ',') return false;
      ++cursor;
    }
  }
  while (cursor != end && (*cursor == ' ' || *cursor == '\t')) ++cursor;
  return cursor == end && values[0] == 1.0f && values[1] == 1.0f &&
         values[2] == 1.0f;
}

const CryptModuleAssetV1* find_asset(
    const GenerationContextV1& context, std::string_view name,
    std::string_view gameplay, std::string_view visual) {
  const CryptModuleAssetKeyV1 key{std::string(name), std::string(gameplay),
                                  std::string(visual)};
  const auto found = context.catalogue->assets_by_exact_key.find(key);
  if (found == context.catalogue->assets_by_exact_key.end() ||
      !found->second.is_complete) {
    return nullptr;
  }
  return &found->second;
}

const CryptListV1* selected_list(const GenerationContextV1& context,
                                const RuleNodeV1& rule) {
  if (!rule.selector.is_list) return nullptr;
  return find_list_v1(*context.document, rule.selector.list_name);
}

bool selected_entries(const GenerationContextV1& context,
                      const RuleNodeV1& rule,
                      std::vector<const CryptListEntryV1*>& entries) {
  const auto* list = selected_list(context, rule);
  if (!list) return false;
  if (rule.selector.list_index) {
    const auto index = *rule.selector.list_index;
    if (index < 0 || static_cast<std::size_t>(index) >= list->entries.size()) {
      return false;
    }
    entries.push_back(&list->entries[static_cast<std::size_t>(index)]);
    return true;
  }
  entries.reserve(list->entries.size());
  for (const auto& entry : list->entries) entries.push_back(&entry);
  return true;
}

bool validate_rule_tree(const GenerationContextV1& context,
                        const RuleNodeV1& node, bool is_root,
                        std::size_t depth, std::string& error) {
  if (depth > 64) {
    error = "Crypt rule nesting exceeds the bounded generator depth";
    return false;
  }
  if ((is_root && node.kind != RuleKindV1::root) ||
      (!is_root && node.kind == RuleKindV1::root)) {
    error = "Crypt rule tree has an unexpected RootRule node";
    return false;
  }
  if (node.children.size() > 5) {
    error = "source distribution table only supports up to five child rules";
    return false;
  }
  if (!is_root) {
    if (node.kind != RuleKindV1::force_block && node.kind != RuleKindV1::path) {
      error = "unsupported Crypt child rule kind";
      return false;
    }
    if (!node.selector.is_list) {
      error = "only list-backed ForceBlock/Path selectors are modeled";
      return false;
    }
    const auto* list = selected_list(context, node);
    if (!list) {
      error = "Crypt selector refers to a missing list";
      return false;
    }
    if (!list->replacement) {
      error = "consuming ListElem replacement is not modeled";
      return false;
    }
    std::vector<const CryptListEntryV1*> entries;
    if (!selected_entries(context, node, entries)) {
      error = "Crypt list selector index is out of range";
      return false;
    }
    for (const auto* entry : entries) {
      const auto* asset = find_asset(context, entry->name, entry->gameplay,
                                     entry->visual);
      if (!asset) {
        error = "catalogue is missing a selected exact asset tuple";
        return false;
      }
      if (!asset->mgx.has_grid_geometry || asset->mgx.block_width <= 0 ||
          asset->mgx.block_height <= 0 ||
          !std::isfinite(asset->mgx.source_unit_width) ||
          !std::isfinite(asset->mgx.source_unit_height) ||
          asset->mgx.source_unit_width <= 0.0f ||
          asset->mgx.source_unit_height <= 0.0f) {
        error = "selected MGX has unsupported grid geometry";
        return false;
      }
      if (!identity_scale(asset->scale)) {
        error = "DWLD v1 cannot serialize a non-identity MVX scale";
        return false;
      }
    }
    if (node.kind == RuleKindV1::path &&
        (!node.path.min_length || !node.path.max_length ||
         *node.path.min_length < 0 || *node.path.max_length < 0)) {
      error = "Path requires a bounded non-negative length range";
      return false;
    }
  }
  for (const auto& child : node.children) {
    if (!validate_rule_tree(context, child, false, depth + 1, error)) {
      return false;
    }
  }
  return true;
}

RuntimeTileV1* allocate_tile(GenerationContextV1& context,
                             const CryptModuleAssetV1* asset) {
  try {
    auto tile = std::make_unique<RuntimeTileV1>();
    tile->asset = asset;
    auto* result = tile.get();
    context.tiles.push_back(std::move(tile));
    return result;
  } catch (...) {
    return nullptr;
  }
}

bool detach_child(RuntimeTileV1* child) {
  if (!child || !child->parent) return true;
  auto& children = child->parent->children;
  const auto found = std::find(children.begin(), children.end(), child);
  if (found == children.end()) return false;
  children.erase(found);
  child->parent = nullptr;
  return true;
}

bool rollback_tile(GenerationContextV1& context, RuntimeTileV1* tile) {
  if (!tile || !tile->active) return true;
  bool ok = true;
  while (!tile->children.empty()) {
    const auto old_size = tile->children.size();
    if (!rollback_tile(context, tile->children.back())) ok = false;
    if (tile->children.size() == old_size) {
      tile->children.pop_back();
      ok = false;
    }
  }
  if (!context.tile_map.remove(tile, tile->placement.x, tile->placement.y,
                               tile->asset->mgx.block_width,
                               tile->asset->mgx.block_height)) {
    ok = false;
  }
  if (!detach_child(tile)) ok = false;
  tile->active = false;
  return ok;
}

CryptRuleCallbackStatusV1 step_exits(
    void*, const CryptRuleExecutionFrameV1& frame,
    std::vector<SourceRuleExitCandidateV1>& exits) {
  const auto* tile = static_cast<const RuntimeTileV1*>(frame.tile);
  if (!tile || !tile->asset || !tile->active) {
    return CryptRuleCallbackStatusV1::error;
  }
  exits.reserve(tile->asset->mgx.exits.size());
  for (std::size_t index = 0; index < tile->asset->mgx.exits.size(); ++index) {
    SourceRuleExitCandidateV1 candidate;
    candidate.source_exit_index = index;
    candidate.destination_block_name = tile->asset->key.name;
    exits.push_back(std::move(candidate));
  }
  return CryptRuleCallbackStatusV1::ready;
}

CryptRuleChildCreateStatusV1 create_child_rule(
    void* opaque, const CryptRuleExecutionFrameV1& parent,
    std::size_t child_index, RandomGeneratorV1& random,
    CryptRuleExecutionFrameV1& child_frame) {
  auto& context = *static_cast<GenerationContextV1*>(opaque);
  auto* parent_rule = const_cast<RuntimeRuleV1*>(
      static_cast<const RuntimeRuleV1*>(parent.rule));
  if (!parent_rule || !parent_rule->source ||
      child_index >= parent_rule->source->children.size()) {
    return CryptRuleChildCreateStatusV1::error;
  }
  const auto& source = parent_rule->source->children[child_index];
  std::unique_ptr<RuntimeRuleV1> child;
  try {
    child = std::make_unique<RuntimeRuleV1>();
  } catch (...) {
    return CryptRuleChildCreateStatusV1::error;
  }
  child->source = &source;
  child->parent = parent_rule;
  if (source.kind == RuleKindV1::path) {
    if (!source.path.min_length || !source.path.max_length) {
      return CryptRuleChildCreateStatusV1::unsupported;
    }
    const auto length = choose_crypt_path_length_v1(
        *source.path.min_length, *source.path.max_length, false, std::nullopt,
        random);
    child->path.chosen_length = length.length;
    child->path.current_step = 0;
    child->path.dont_go_back = source.path.dont_go_back;
    child->has_path = true;
  }
  auto* child_pointer = child.get();
  try {
    parent_rule->children.push_back(std::move(child));
  } catch (...) {
    return CryptRuleChildCreateStatusV1::error;
  }
  child_frame = {};
  child_frame.rule = child_pointer;
  child_frame.child_rule_count = source.children.size();
  if (child_pointer->has_path) child_frame.path_runtime = &child_pointer->path;
  (void)context;
  return CryptRuleChildCreateStatusV1::created;
}

CryptRuleBlockLookupStatusV1 resolve_block(
    void* opaque, const SourceRuleExitCandidateV1& candidate,
    const void** block) {
  if (!block) return CryptRuleBlockLookupStatusV1::error;
  const auto& context = *static_cast<const GenerationContextV1*>(opaque);
  const auto* asset = find_asset(context, candidate.elem.name,
                                 candidate.elem.gameplay,
                                 candidate.elem.visual);
  if (!asset || asset->key.name != candidate.destination_block_name) {
    return CryptRuleBlockLookupStatusV1::missing;
  }
  *block = asset;
  return CryptRuleBlockLookupStatusV1::found;
}

CryptRuleCallbackStatusV1 candidate_exits(
    void* opaque, const CryptRuleExecutionFrameV1& child,
    std::size_t anchor_exit_index,
    std::vector<SourceRuleExitCandidateV1>& exits) {
  const auto& context = *static_cast<const GenerationContextV1*>(opaque);
  const auto* rule = static_cast<const RuntimeRuleV1*>(child.rule);
  const auto* tile = static_cast<const RuntimeTileV1*>(child.tile);
  if (!rule || !rule->source || !tile || !tile->asset || !tile->active ||
      anchor_exit_index >= tile->asset->mgx.exits.size()) {
    return CryptRuleCallbackStatusV1::error;
  }
  const auto& anchor = tile->asset->mgx.exits[anchor_exit_index];
  if (!anchor.has_cell_position) return CryptRuleCallbackStatusV1::unsupported;

  std::vector<const CryptListEntryV1*> entries;
  if (!selected_entries(context, *rule->source, entries)) {
    return CryptRuleCallbackStatusV1::unsupported;
  }
  for (const auto* entry : entries) {
    const auto* asset = find_asset(context, entry->name, entry->gameplay,
                                   entry->visual);
    if (!asset) return CryptRuleCallbackStatusV1::error;
    for (std::size_t index = 0; index < asset->mgx.exits.size(); ++index) {
      const auto& target_exit = asset->mgx.exits[index];
      if (!crypt_exits_compatible_v1(anchor, target_exit)) continue;
      if (!target_exit.has_cell_position) {
        return CryptRuleCallbackStatusV1::unsupported;
      }
      SourceRuleExitCandidateV1 candidate;
      candidate.source_exit_index = index;
      candidate.destination_block_name = asset->key.name;
      candidate.elem = make_source_list_elem_v1(*entry);
      exits.push_back(std::move(candidate));
    }
  }
  return CryptRuleCallbackStatusV1::ready;
}

CryptRuleCallbackStatusV1 opposite_anchor_direction(
    void*, const CryptRuleExecutionFrameV1& child,
    std::size_t anchor_exit_index,
    CryptDirectionKeyV1& opposite_direction) {
  const auto* tile = static_cast<const RuntimeTileV1*>(child.tile);
  if (!tile || !tile->asset ||
      anchor_exit_index >= tile->asset->mgx.exits.size()) {
    return CryptRuleCallbackStatusV1::error;
  }
  const auto direction = static_cast<std::uint32_t>(
      tile->asset->mgx.exits[anchor_exit_index].direction);
  if (direction >= 4) return CryptRuleCallbackStatusV1::unsupported;
  opposite_direction = (direction + 2U) % 4U;
  return CryptRuleCallbackStatusV1::ready;
}

CryptRuleCallbackStatusV1 module_exit_directions(
    void*, const void* block,
    std::vector<CryptDirectionKeyV1>& directions) {
  const auto* asset = static_cast<const CryptModuleAssetV1*>(block);
  if (!asset) return CryptRuleCallbackStatusV1::error;
  directions.reserve(asset->mgx.exits.size());
  for (const auto& exit : asset->mgx.exits) {
    const auto direction = static_cast<std::uint32_t>(exit.direction);
    if (direction >= 4) return CryptRuleCallbackStatusV1::unsupported;
    directions.push_back(direction);
  }
  return CryptRuleCallbackStatusV1::ready;
}

CryptRuleSpawnStatusV1 try_spawn(
    void* opaque, const CryptRuleExecutionFrameV1& parent,
    std::size_t anchor_exit_index, const void* block,
    const SourceRuleExitCandidateV1& candidate,
    CryptRuleSpawnedTileV1& spawned) {
  auto& context = *static_cast<GenerationContextV1*>(opaque);
  const auto* parent_tile = static_cast<const RuntimeTileV1*>(parent.tile);
  const auto* asset = static_cast<const CryptModuleAssetV1*>(block);
  if (!parent_tile || !parent_tile->asset || !parent_tile->active || !asset ||
      anchor_exit_index >= parent_tile->asset->mgx.exits.size() ||
      candidate.source_exit_index >= asset->mgx.exits.size()) {
    return CryptRuleSpawnStatusV1::error;
  }
  const auto& anchor = parent_tile->asset->mgx.exits[anchor_exit_index];
  const auto& target = asset->mgx.exits[candidate.source_exit_index];
  if (!anchor.has_cell_position || !target.has_cell_position ||
      !crypt_exits_compatible_v1(anchor, target)) {
    return CryptRuleSpawnStatusV1::unsupported;
  }
  const CryptMgxTileOriginV1 anchor_origin{
      parent_tile->placement.x, parent_tile->placement.y,
      parent_tile->placement.elevation};
  const CryptMgxExitCellV1 anchor_cell{
      anchor.cell_x, anchor.cell_y, anchor.position_z};
  const CryptMgxExitCellV1 target_cell{
      target.cell_x, target.cell_y, target.position_z};
  CryptMgxTilePlacementV1 checked_placement;
  if (!crypt_mgx_try_spawn_placement_v1(
          anchor_origin, &anchor_cell, target_cell, target.direction,
          checked_placement)) {
    return CryptRuleSpawnStatusV1::unsupported;
  }

  auto* tile = allocate_tile(context, asset);
  if (!tile) return CryptRuleSpawnStatusV1::error;
  tile->parent = const_cast<RuntimeTileV1*>(parent_tile);
  try {
    tile->parent->children.push_back(tile);
  } catch (...) {
    tile->parent = nullptr;
    return CryptRuleSpawnStatusV1::error;
  }
  CryptMgxTilePlacementV1 placement;
  if (!context.tile_map.try_spawn(
          tile, anchor_origin, &anchor_cell, target_cell, target.direction,
          asset->mgx.block_width, asset->mgx.block_height, placement)) {
    (void)detach_child(tile);
    tile->active = false;
    return CryptRuleSpawnStatusV1::rejected;
  }
  tile->placement = placement;
  tile->active = true;
  spawned.tile = tile;
  spawned.token = tile;
  return CryptRuleSpawnStatusV1::spawned;
}

CryptRuleGenerationResultV1 recurse_child(
    void* opaque, const CryptRuleExecutionFrameV1& child,
    RandomGeneratorV1& random) {
  auto& context = *static_cast<GenerationContextV1*>(opaque);
  if (!child.rule || !child.tile) {
    return generation_result(GenerationStatus::callback_error);
  }
  const auto* rule = static_cast<const RuntimeRuleV1*>(child.rule);
  CryptRuleExecutionFrameV1 frame = child;
  frame.child_rule_count = rule->source->children.size();
  return execute_crypt_rule_step_v1(frame, callbacks_for(context), random);
}

bool unspawn(void* opaque, const CryptRuleSpawnedTileV1& spawned) {
  auto& context = *static_cast<GenerationContextV1*>(opaque);
  auto* tile = const_cast<RuntimeTileV1*>(
      static_cast<const RuntimeTileV1*>(spawned.token));
  return tile && rollback_tile(context, tile);
}

bool destroy_child_rule(void*, const CryptRuleExecutionFrameV1& child) {
  auto* rule = const_cast<RuntimeRuleV1*>(
      static_cast<const RuntimeRuleV1*>(child.rule));
  if (!rule || !rule->parent) return false;
  auto& siblings = rule->parent->children;
  const auto found = std::find_if(
      siblings.begin(), siblings.end(),
      [rule](const auto& owned) { return owned.get() == rule; });
  if (found == siblings.end()) return false;
  siblings.erase(found);
  return true;
}

bool remove_neighbors(void* opaque, const void* opaque_tile) {
  auto& context = *static_cast<GenerationContextV1*>(opaque);
  auto* tile = const_cast<RuntimeTileV1*>(
      static_cast<const RuntimeTileV1*>(opaque_tile));
  if (!tile) return false;
  bool ok = true;
  while (!tile->children.empty()) {
    const auto old_size = tile->children.size();
    if (!rollback_tile(context, tile->children.back())) ok = false;
    if (tile->children.size() == old_size) {
      tile->children.pop_back();
      ok = false;
    }
  }
  return ok;
}

CryptRuleGenerationResultV1 path_length_reached_step(
    void* opaque, const CryptRuleExecutionFrameV1& child,
    std::size_t anchor_exit_index, RandomGeneratorV1& random) {
  auto& context = *static_cast<GenerationContextV1*>(opaque);
  const auto* tile = static_cast<const RuntimeTileV1*>(child.tile);
  if (!tile || !tile->asset || anchor_exit_index >= tile->asset->mgx.exits.size()) {
    return generation_result(GenerationStatus::unsupported);
  }
  CryptRuleExecutionFrameV1 terminal = child;
  // IDA Path::Impl::OneStep (0x48fd64): at max length, a tile with exactly
  // two exits steps while excluding the other exit from the assigned pair;
  // all other exit counts pass a null exclusion to Rule::Impl::Step.
  if (tile->asset->mgx.exits.size() == 2) {
    terminal.incoming_exit_index = anchor_exit_index == 0 ? 1 : 0;
  } else {
    terminal.incoming_exit_index.reset();
  }
  const auto* rule = static_cast<const RuntimeRuleV1*>(child.rule);
  terminal.child_rule_count = rule->source->children.size();
  return execute_crypt_rule_step_v1(terminal, callbacks_for(context), random);
}

CryptRuleGenerationCallbacksV1 callbacks_for(GenerationContextV1& context) {
  CryptRuleGenerationCallbacksV1 callbacks;
  callbacks.context = &context;
  callbacks.step_exits = &step_exits;
  callbacks.create_child_rule = &create_child_rule;
  callbacks.resolve_block = &resolve_block;
  callbacks.candidate_exits = &candidate_exits;
  callbacks.opposite_anchor_direction = &opposite_anchor_direction;
  callbacks.module_exit_directions = &module_exit_directions;
  callbacks.try_spawn = &try_spawn;
  callbacks.recurse_child = &recurse_child;
  callbacks.unspawn = &unspawn;
  callbacks.destroy_child_rule = &destroy_child_rule;
  callbacks.remove_neighbors = &remove_neighbors;
  callbacks.path_length_reached_step = &path_length_reached_step;
  return callbacks;
}

RootBlockLookupResultV1 root_lookup(void* opaque, std::string_view name) {
  auto& context = *static_cast<GenerationContextV1*>(opaque);
  std::vector<const CryptListEntryV1*> entries;
  if (!selected_entries(context, context.document->root, entries)) {
    return {RootBlockLookupStatusV1::error, nullptr};
  }
  const CryptModuleAssetV1* match = nullptr;
  for (const auto* entry : entries) {
    if (entry->name != name) continue;
    const auto* asset = find_asset(context, entry->name, entry->gameplay,
                                   entry->visual);
    if (!asset) continue;
    if (match && match != asset) {
      return {RootBlockLookupStatusV1::error, nullptr};
    }
    match = asset;
  }
  if (!match) return {RootBlockLookupStatusV1::missing, nullptr};
  return {RootBlockLookupStatusV1::found, match};
}

RootPlacementStepStatusV1 place_root_and_step(
    void* opaque, const void* block, const CryptListEntryV1& candidate,
    const RootPlacementOriginV1& origin) {
  auto& context = *static_cast<GenerationContextV1*>(opaque);
  const auto* asset = find_asset(context, candidate.name, candidate.gameplay,
                                 candidate.visual);
  if (!asset || asset != block || origin.x != 0.0f || origin.y != 0.0f ||
      origin.z != 0.0f) {
    context.last_generation = generation_result(GenerationStatus::callback_error);
    return RootPlacementStepStatusV1::error;
  }
  auto* tile = allocate_tile(context, asset);
  if (!tile) {
    context.last_generation = generation_result(GenerationStatus::callback_error);
    return RootPlacementStepStatusV1::error;
  }
  if (!context.tile_map.try_place(tile, 0, 0, asset->mgx.block_width,
                                  asset->mgx.block_height)) {
    context.last_generation = generation_result(GenerationStatus::failed);
    return RootPlacementStepStatusV1::candidate_failed;
  }
  tile->placement = {0, 0, 0.0f};
  tile->active = true;

  CryptRuleExecutionFrameV1 frame;
  frame.rule = &context.root_rule;
  frame.tile = tile;
  frame.child_rule_count = context.document->root.children.size();
  const auto callbacks = callbacks_for(context);
  context.last_generation = execute_crypt_rule_step_v1(
      frame, callbacks, *context.random);
  if (context.last_generation.status == GenerationStatus::success) {
    context.successful_root = tile;
    return RootPlacementStepStatusV1::success;
  }
  if (context.last_generation.status != GenerationStatus::failed) {
    context.callback_error = true;
    (void)remove_neighbors(&context, tile);
    if (!context.tile_map.remove(tile, 0, 0, asset->mgx.block_width,
                                 asset->mgx.block_height)) {
      context.callback_error = true;
    }
    tile->active = false;
    return RootPlacementStepStatusV1::error;
  }
  if (!remove_neighbors(&context, tile) ||
      !context.tile_map.remove(tile, 0, 0, asset->mgx.block_width,
                               asset->mgx.block_height)) {
    context.callback_error = true;
    return RootPlacementStepStatusV1::error;
  }
  tile->active = false;
  return RootPlacementStepStatusV1::candidate_failed;
}

bool catalogue_matches_document(const CryptRuleDocumentV1& document,
                                const CryptModuleCatalogueV1& catalogue) {
  std::size_t position = 0;
  for (const auto& list : document.lists) {
    for (std::size_t index = 0; index < list.entries.size(); ++index) {
      if (position >= catalogue.rule_candidates.size()) return false;
      const auto& candidate = catalogue.rule_candidates[position++];
      const auto& entry = list.entries[index];
      if (candidate.list_name != list.name || candidate.list_index != index ||
          candidate.key.name != entry.name ||
          candidate.key.gameplay != entry.gameplay ||
          candidate.key.visual != entry.visual) {
        return false;
      }
    }
  }
  return position == catalogue.rule_candidates.size();
}

bool build_root_candidates(const CryptRuleDocumentV1& document,
                           RandomGeneratorV1& random,
                           std::vector<CryptListEntryV1>& candidates,
                           std::string& error) {
  const auto& selector = document.root.selector;
  if (!selector.is_list) {
    error = "root selector must be a source list for this adapter";
    return false;
  }
  const auto* list = find_list_v1(document, selector.list_name);
  if (!list) {
    error = "RootRule references a missing candidate list";
    return false;
  }
  if (!list->replacement) {
    error = "consuming RootRule list behavior is not modeled";
    return false;
  }
  if (selector.list_index && *selector.list_index < -1) {
    error = "RootRule index below -1 is unsupported";
    return false;
  }
  if (selector.list_index && *selector.list_index >= 0 &&
      static_cast<std::size_t>(*selector.list_index) < list->entries.size()) {
    candidates.push_back(list->entries[static_cast<std::size_t>(
        *selector.list_index)]);
    return true;
  }
  candidates = list->entries;
  if (!source_shuffle(candidates, random)) {
    error = "root candidate shuffle exceeds source RNG bounds";
    return false;
  }
  return true;
}

std::vector<RuntimeTileV1*> active_preorder(GenerationContextV1& context) {
  std::vector<RuntimeTileV1*> result;
  if (!context.successful_root) return result;
  std::vector<RuntimeTileV1*> stack{context.successful_root};
  while (!stack.empty()) {
    auto* tile = stack.back();
    stack.pop_back();
    if (!tile || !tile->active) continue;
    result.push_back(tile);
    for (auto child = tile->children.rbegin(); child != tile->children.rend();
         ++child) {
      stack.push_back(*child);
    }
  }
  return result;
}

bool build_layout(GenerationContextV1& context,
                  const CryptGeneratedSpawnV1* spawn,
                  CryptLevelGeneratorResultV1& result) {
  auto tiles = active_preorder(context);
  std::vector<GeneratedRecordStorageV1> storage;
  storage.reserve(tiles.size());
  result.modules.reserve(tiles.size());
  const std::string folder = context.document->folder;
  for (const auto* tile : tiles) {
    const auto& asset = *tile->asset;
    GeneratedRecordStorageV1 item;
    item.gameplay_path = folder + "/mgp/" + asset.key.gameplay;
    item.visual_path = folder + "/mvp/" + asset.key.visual;
    item.record.block_name = asset.key.name;
    item.record.catalogue_xrefobject = asset.xrefobject;
    item.record.bres_path = asset.dae;
    item.record.xrefmax = asset.xrefmax;
    item.record.fog_color = asset.fog_color;
    item.record.is_solid = asset.is_solid;
    item.record.grid_x = tile->placement.x;
    item.record.grid_y = tile->placement.y;
    item.record.elevation = tile->placement.elevation;
    item.record.unit_width = asset.mgx.source_unit_width;
    item.record.unit_height = asset.mgx.source_unit_height;
    item.record.block_width = static_cast<std::uint32_t>(
        asset.mgx.block_width);
    item.record.block_height = static_cast<std::uint32_t>(
        asset.mgx.block_height);
    result.modules.push_back({asset.key.name, asset.xrefobject,
                              asset.key.gameplay, asset.key.visual,
                              tile->placement.x, tile->placement.y,
                              tile->placement.elevation});
    storage.push_back(std::move(item));
  }
  std::vector<CryptGeneratedModuleV1> records;
  records.reserve(storage.size());
  for (auto& item : storage) {
    item.record.gameplay_path = item.gameplay_path;
    item.record.mvp_path = item.visual_path;
    records.push_back(item.record);
  }
  CryptGeneratedLayoutOptionsV1 options;
  options.rule_document = context.document;
  options.spawn = spawn;
  const auto status = crypt_serialize_generated_layout_v1(
      records.data(), records.size(), options, result.layout,
      &result.layout_diagnostic);
  if (status != CryptGeneratedLayoutStatusV1::ok) {
    result.message = result.layout_diagnostic.message;
    return false;
  }
  return true;
}

}  // namespace

static CryptLevelGeneratorResultV1 generate_crypt_level_impl_v1(
    const CryptRuleDocumentV1& document,
    const CryptModuleCatalogueV1& catalogue, std::uint32_t seed,
    const CryptGeneratedSpawnV1* spawn) {
  CryptLevelGeneratorResultV1 result;
  result.seed = seed;
  RandomGeneratorV1 random(seed);
  GenerationContextV1 context;
  context.document = &document;
  context.catalogue = &catalogue;
  context.random = &random;
  context.root_rule.source = &document.root;
  context.root_rule.has_path = false;

  if (!catalogue_matches_document(document, catalogue)) {
    result.status = CryptLevelGeneratorStatusV1::incomplete_catalogue;
    result.message = "validated catalogue does not match parsed rule lists";
    result.final_rng_state = random.state();
    return result;
  }
  if (!catalogue.catalogue_issues.empty() ||
      catalogue.unresolved_candidate_count() != 0) {
    result.status = CryptLevelGeneratorStatusV1::incomplete_catalogue;
    result.message = "one or more source list tuples lack validated assets";
    result.final_rng_state = random.state();
    return result;
  }
  std::string error;
  if (!validate_rule_tree(context, document.root, true, 0, error)) {
    result.status = CryptLevelGeneratorStatusV1::unsupported_rules;
    result.message = std::move(error);
    result.final_rng_state = random.state();
    return result;
  }
  std::vector<CryptListEntryV1> root_candidates;
  if (!build_root_candidates(document, random, root_candidates, error)) {
    result.status = CryptLevelGeneratorStatusV1::unsupported_rules;
    result.message = std::move(error);
    result.final_rng_state = random.state();
    return result;
  }
  if (root_candidates.empty()) {
    result.status = CryptLevelGeneratorStatusV1::no_solution;
    result.message = "RootRule has no candidates";
    result.final_rng_state = random.state();
    return result;
  }
  RootRuleRuntimeCallbacksV1 root_callbacks;
  root_callbacks.context = &context;
  root_callbacks.lookup_block = &root_lookup;
  root_callbacks.place_root_and_step = &place_root_and_step;
  result.root_result = execute_crypt_root_rule_v1(root_candidates,
                                                  root_callbacks);
  result.generation_result = context.last_generation;
  result.final_rng_state = random.state();
  if (result.root_result.status != RootRuleRuntimeStatusV1::success ||
      context.callback_error) {
    if (result.generation_result.status == GenerationStatus::unsupported ||
        result.generation_result.status == GenerationStatus::rng_failure) {
      result.status = CryptLevelGeneratorStatusV1::unsupported_rules;
    } else if (result.generation_result.status == GenerationStatus::failed ||
               result.root_result.status == RootRuleRuntimeStatusV1::exhausted ||
               result.root_result.status == RootRuleRuntimeStatusV1::no_candidates) {
      result.status = CryptLevelGeneratorStatusV1::no_solution;
    } else {
      result.status = CryptLevelGeneratorStatusV1::generation_error;
    }
    if (result.message.empty()) {
      result.message = context.callback_error
                           ? "source generation callback failed"
                           : "RootRule candidates were exhausted";
    }
    return result;
  }
  if (!build_layout(context, spawn, result)) {
    result.status = CryptLevelGeneratorStatusV1::serialization_error;
    return result;
  }
  result.status = CryptLevelGeneratorStatusV1::success;
  result.message = "generated " + std::to_string(result.modules.size()) +
                   " modules";
  return result;
}

CryptLevelGeneratorResultV1 generate_crypt_level_v1(
    const CryptRuleDocumentV1& document,
    const CryptModuleCatalogueV1& catalogue, std::uint32_t seed,
    const CryptGeneratedSpawnV1* spawn) {
  try {
    return generate_crypt_level_impl_v1(document, catalogue, seed, spawn);
  } catch (...) {
    CryptLevelGeneratorResultV1 result;
    result.status = CryptLevelGeneratorStatusV1::generation_error;
    result.seed = seed;
    result.final_rng_state = seed;
    result.message = "Crypt generation failed during bounded host allocation";
    return result;
  }
}

}  // namespace dh2::random_level
