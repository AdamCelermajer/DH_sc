#include "source_animated_decor_v1.hpp"

#include "../world-data/world.hpp"

#include <cmath>
#include <cstring>
#include <utility>

namespace dh2::world {
namespace {

struct SourceOwner {
  SourceLevel value{};
  ~SourceOwner() { dh2_world_free(&value); }
};

bool valid_row(const SourceAnimatedDecorV1& row) {
  for (unsigned axis = 0; axis < 3; ++axis) {
    for (unsigned slot : {0u, 3u, 6u})
      if (!std::isfinite(row.world_transform[slot + axis])) return false;
    if (std::abs(row.world_transform[axis]) > 10000000.f ||
        std::abs(row.world_transform[3 + axis]) > 3600.f ||
        row.world_transform[6 + axis] <= 0.f ||
        row.world_transform[6 + axis] > 10000.f)
      return false;
  }
  return true;
}

}  // namespace

bool compile_source_animated_decor_v1(
    const std::uint8_t* level_xml, std::size_t level_size,
    const char* level_name, const char* level_source_path,
    const GeneratedCryptMvpFileV1* mvps, std::size_t mvp_count,
    std::vector<SourceAnimatedDecorV1>& output, std::size_t& deferred_count,
    std::string& error) {
  output.clear();
  deferred_count = 0;
  error.clear();
  if (!level_xml || !level_size || !level_name || !*level_name ||
      !level_source_path || !mvps || !mvp_count || mvp_count > 256) {
    error = "Source AnimatedDecor input is incomplete or exceeds the module limit";
    return false;
  }

  SourceOwner source;
  Diagnostic diagnostic{};
  if (dh2_world_import_level(&source.value, level_name, level_source_path,
          level_xml, level_size, &diagnostic) != Error::ok) {
    error = diagnostic.message[0] ? diagnostic.message
                                  : "Source AnimatedDecor Level XML import failed";
    return false;
  }
  if (!source.value.module_count || mvp_count != source.value.module_count) {
    error = "Source AnimatedDecor needs one ordered MVP for every Module";
    return false;
  }
  for (std::uint32_t module = 0; module < source.value.module_count; ++module) {
    if (!mvps[module].source_path || !mvps[module].data || !mvps[module].size) {
      error = "Source AnimatedDecor MVP input is incomplete";
      return false;
    }
    if (dh2_world_import_module_objects(&source.value, module, RecordKind::mvp,
            mvps[module].source_path, mvps[module].data, mvps[module].size,
            &diagnostic) != Error::ok) {
      error = diagnostic.message[0] ? diagnostic.message
                                    : "Source AnimatedDecor MVP import failed";
      return false;
    }
  }

  std::vector<SourceAnimatedDecorV1> candidate;
  for (std::uint32_t index = 0; index < source.value.entity_count; ++index) {
    const auto& object = source.value.entities[index];
    if (object.kind != RecordKind::mvp || !object.gametype ||
        std::strcmp(object.gametype, "AnimatedDecor") != 0)
      continue;

    const auto* type = dh2_world_field(&object, "type");
    const auto* template_name = dh2_world_field(&object, "_templateName");
    const auto* root = dh2_world_field(&object, "xrefobject");
    const auto* dae = dh2_world_field(&object, "dae");
    const auto* start_animation = dh2_world_field(&object, "startanim");
    char canonical_dae[256]{};
    if (!type || std::strcmp(type, "Block") || !template_name ||
        std::strcmp(template_name, "AnimatedDecor") || !root || !*root ||
        !dae || dh2_world_cache_path(canonical_dae, sizeof(canonical_dae),
            dae, &diagnostic) != Error::ok || !start_animation ||
        !*start_animation) {
      ++deferred_count;
      continue;
    }
    const auto dae_length = std::strlen(canonical_dae);
    if (dae_length < 5 ||
        std::strcmp(canonical_dae + dae_length - 5, ".bdae") != 0) {
      ++deferred_count;
      continue;
    }

    SourceAnimatedDecorV1 row{};
    row.module_index = object.module_index;
    row.source_record = object.source_record;
    row.name = object.name ? object.name : "";
    row.xrefobject = root;
    row.dae_path = canonical_dae;
    row.source_path = object.source_path ? object.source_path : "";
    row.start_animation = start_animation;
    const auto* activate = dh2_world_field(&object, "activate_cond");
    row.activate_condition = activate ? activate : "";
    const auto& module = source.value.modules[object.module_index].record;
    for (unsigned axis = 0; axis < 3; ++axis) {
      row.world_transform[axis] = object.world_position[axis];
      row.world_transform[3 + axis] = object.local.rotation_degrees[axis] +
          module.local.rotation_degrees[axis];
      row.world_transform[6 + axis] = object.local.scale[axis] *
          module.local.scale[axis];
    }
    if (row.name.empty() || row.source_path.empty() || !valid_row(row)) {
      ++deferred_count;
      continue;
    }
    candidate.push_back(std::move(row));
  }

  output = std::move(candidate);
  return true;
}

}  // namespace dh2::world
