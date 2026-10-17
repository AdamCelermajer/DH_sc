#include "crypt_generated_mvp_v1.hpp"

#include "../world-data/world.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <utility>

namespace dh2::world {
namespace {

struct SourceOwner {
  SourceLevel value{};
  ~SourceOwner() { dh2_world_free(&value); }
};

bool trimmed_ascii_equal(const char* text, const char* expected) {
  if (!text || !expected) return false;
  while (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')
    ++text;
  const auto* end = text + std::strlen(text);
  while (end != text &&
         (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r' ||
          end[-1] == '\n'))
    --end;
  const auto expected_size = std::strlen(expected);
  if (static_cast<std::size_t>(end - text) != expected_size) return false;
  for (std::size_t i = 0; i < expected_size; ++i) {
    const auto value = static_cast<unsigned char>(text[i]);
    const auto lower = value >= 'A' && value <= 'Z'
        ? static_cast<unsigned char>(value + ('a' - 'A')) : value;
    if (lower != static_cast<unsigned char>(expected[i])) return false;
  }
  return true;
}

bool empty_or_invalid(const char* text) {
  if (!text) return true;
  while (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')
    ++text;
  return !*text || trimmed_ascii_equal(text, "invalid");
}

bool non_whitespace(const char* text) {
  if (!text) return false;
  for (; *text; ++text)
    if (*text != ' ' && *text != '\t' && *text != '\r' && *text != '\n')
      return true;
  return false;
}

bool explicitly_true(const char* text) {
  return trimmed_ascii_equal(text, "true");
}

const source_handle_ledger_v1::Occurrence* find_occurrence(
    const source_handle_ledger_v1::Ledger& ledger,
    const Object& object, std::size_t& matches) {
  const source_handle_ledger_v1::Occurrence* found = nullptr;
  matches = 0;
  for (const auto& occurrence : ledger.occurrences) {
    if (occurrence.source.kind != RecordKind::mvp ||
        occurrence.source.module_index != object.module_index ||
        occurrence.source.record_index != object.source_record ||
        occurrence.source.name != (object.name ? object.name : "") ||
        occurrence.source.gametype != (object.gametype ? object.gametype : ""))
      continue;
    found = &occurrence;
    ++matches;
  }
  return found;
}

bool valid_transform(const GeneratedCryptAnimatedDecorV1& row) {
  for (unsigned axis = 0; axis < 3; ++axis) {
    const float values[] = {
        row.local_transform[axis], row.local_transform[3 + axis],
        row.local_transform[6 + axis], row.world_transform[axis],
        row.world_transform[3 + axis], row.world_transform[6 + axis]};
    for (const float value : values)
      if (!std::isfinite(value)) return false;
    if (std::abs(row.world_transform[axis]) > 10000000.f ||
        std::abs(row.world_transform[3 + axis]) > 3600.f ||
        row.world_transform[6 + axis] <= 0.f ||
        row.world_transform[6 + axis] > 10000.f)
      return false;
  }
  return true;
}

}  // namespace

bool compile_generated_crypt_animated_decor_v1(
    const std::uint8_t* level_xml, std::size_t level_size,
    const char* level_name, const char* level_source_path,
    const GeneratedCryptMvpFileV1* mvps, std::size_t mvp_count,
    const source_handle_ledger_v1::Ledger& handle_ledger,
    std::vector<GeneratedCryptAnimatedDecorV1>& output,
    std::size_t& deferred_count, std::string& error) {
  output.clear();
  deferred_count = 0;
  error.clear();
  if (!level_xml || !level_size || !level_name || !*level_name ||
      !level_source_path || !mvps || !mvp_count || mvp_count > 256) {
    error = "Generated Crypt MVP input is incomplete or exceeds the module limit";
    return false;
  }

  SourceOwner source;
  Diagnostic diagnostic{};
  if (dh2_world_import_level(&source.value, level_name, level_source_path,
          level_xml, level_size, &diagnostic) != Error::ok) {
    error = diagnostic.message[0] ? diagnostic.message
                                  : "Generated Crypt Level XML import failed";
    return false;
  }
  if (!source.value.module_count || mvp_count != source.value.module_count) {
    error = "Generated Crypt needs one ordered MVP for every Module";
    return false;
  }

  for (std::uint32_t module = 0; module < source.value.module_count; ++module) {
    if (!mvps[module].source_path || !mvps[module].data || !mvps[module].size) {
      error = "Generated Crypt MVP input is incomplete";
      return false;
    }
    if (dh2_world_import_module_objects(&source.value, module, RecordKind::mvp,
            mvps[module].source_path, mvps[module].data, mvps[module].size,
            &diagnostic) != Error::ok) {
      error = diagnostic.message[0] ? diagnostic.message
                                    : "Generated Crypt MVP import failed";
      return false;
    }
  }

  std::vector<GeneratedCryptAnimatedDecorV1> candidate;
  for (std::uint32_t index = 0; index < source.value.entity_count; ++index) {
    const auto& object = source.value.entities[index];
    if (object.kind != RecordKind::mvp || !object.gametype ||
        std::strcmp(object.gametype, "AnimatedDecor") != 0)
      continue;

    std::size_t occurrence_count = 0;
    const auto* occurrence = find_occurrence(handle_ledger, object,
                                              occurrence_count);
    if (!occurrence || occurrence_count != 1 || occurrence->handle < 0) {
      error = "Generated Crypt AnimatedDecor has no unique source handle occurrence";
      return false;
    }
    if (occurrence->duplicate_name) {
      ++deferred_count;
      continue;
    }

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
      error = "Generated Crypt AnimatedDecor has an unsupported source identity";
      return false;
    }
    const auto dae_length = std::strlen(canonical_dae);
    if (dae_length < 5 ||
        std::strcmp(canonical_dae + dae_length - 5, ".bdae") != 0) {
      error = "Generated Crypt AnimatedDecor does not reference a BDAE model";
      return false;
    }

    if (!empty_or_invalid(dh2_world_field(&object, "activate_cond")) ||
        !empty_or_invalid(dh2_world_field(&object, "deactivate_cond")) ||
        non_whitespace(dh2_world_field(&object, "script")) ||
        explicitly_true(dh2_world_field(&object, "facultative"))) {
      ++deferred_count;
      continue;
    }

    GeneratedCryptAnimatedDecorV1 row{};
    row.module_index = object.module_index;
    row.source_record = object.source_record;
    row.source_handle = occurrence->handle;
    row.name = object.name ? object.name : "";
    row.xrefobject = root;
    row.dae_path = canonical_dae;
    row.source_path = object.source_path ? object.source_path : "";
    row.start_animation = start_animation;
    const auto& module = source.value.modules[object.module_index].record;
    for (unsigned axis = 0; axis < 3; ++axis) {
      row.local_transform[axis] = object.local.position[axis];
      row.local_transform[3 + axis] = object.local.rotation_degrees[axis];
      row.local_transform[6 + axis] = object.local.scale[axis];
      row.world_transform[axis] = object.world_position[axis];
      row.world_transform[3 + axis] = object.local.rotation_degrees[axis] +
          module.local.rotation_degrees[axis];
      row.world_transform[6 + axis] = object.local.scale[axis] *
          module.local.scale[axis];
    }
    if (row.name.empty() || row.source_path.empty() ||
        !valid_transform(row)) {
      error = "Generated Crypt AnimatedDecor provenance or placement is invalid";
      return false;
    }
    candidate.push_back(std::move(row));
  }

  output = std::move(candidate);
  return true;
}

}  // namespace dh2::world
