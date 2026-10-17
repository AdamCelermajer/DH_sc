#include "crypt_generated_dact_v1.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <new>
#include <set>
#include <string_view>
#include <utility>
#include <vector>

namespace dh2::random_level {
namespace {

using Status = CryptGeneratedDactStatusV1;
constexpr std::size_t kHeaderBytes = 16;
constexpr std::size_t kRecordBytes = 256;
constexpr std::uint32_t kMaximumRows = 10000;
constexpr std::uint32_t kMaximumRooms = 512;

Status reject(CryptGeneratedDactDiagnosticV1* diagnostic, Status status,
              const char* message, std::uint32_t module = 0xffffffffU,
              std::uint32_t record = 0xffffffffU) {
  if (diagnostic) {
    *diagnostic = {};
    diagnostic->status = status;
    diagnostic->module_index = module;
    diagnostic->source_record = record;
    std::strncpy(diagnostic->message, message,
                 sizeof(diagnostic->message) - 1);
  }
  return status;
}

void write_u32(std::uint8_t* out, std::uint32_t value) {
  out[0] = static_cast<std::uint8_t>(value);
  out[1] = static_cast<std::uint8_t>(value >> 8);
  out[2] = static_cast<std::uint8_t>(value >> 16);
  out[3] = static_cast<std::uint8_t>(value >> 24);
}

void write_f32(std::uint8_t* out, float value) {
  std::uint32_t bits = 0;
  std::memcpy(&bits, &value, sizeof(bits));
  write_u32(out, bits);
}

bool write_ascii(std::uint8_t* out, std::size_t width, const char* text) {
  if (!text) return false;
  const auto length = std::strlen(text);
  if (!length || length >= width) return false;
  for (std::size_t i = 0; i < length; ++i) {
    const auto c = static_cast<unsigned char>(text[i]);
    if (c < 32 || c > 126) return false;
  }
  std::memcpy(out, text, length);
  return true;
}

bool finite_transform(const float* values, const float limit,
                      bool positive = false) {
  for (unsigned axis = 0; axis < 3; ++axis) {
    if (!std::isfinite(values[axis]) || std::abs(values[axis]) > limit ||
        (positive && values[axis] <= 0.0f))
      return false;
  }
  return true;
}

bool identity_module_transform(const world::Object& module) {
  for (unsigned axis = 0; axis < 3; ++axis)
    if (module.local.rotation_degrees[axis] != 0.0f ||
        module.local.scale[axis] != 1.0f)
      return false;
  return true;
}

bool non_whitespace(const char* value) {
  if (!value) return false;
  for (; *value; ++value)
    if (*value != ' ' && *value != '\t' && *value != '\r' && *value != '\n')
      return true;
  return false;
}

bool condition_is_absent(const char* value) {
  if (!non_whitespace(value)) return true;
  while (*value == ' ' || *value == '\t' || *value == '\r' || *value == '\n')
    ++value;
  std::string_view text(value);
  while (!text.empty() &&
         (text.back() == ' ' || text.back() == '\t' || text.back() == '\r' ||
          text.back() == '\n'))
    text.remove_suffix(1);
  if (text.size() != 7) return false;
  constexpr std::string_view kInvalid = "invalid";
  for (std::size_t i = 0; i < text.size(); ++i) {
    char c = text[i];
    if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    if (c != kInvalid[i]) return false;
  }
  return true;
}

bool field_name_contains(const char* name, std::string_view needle) {
  if (!name) return false;
  for (const char* begin = name; *begin; ++begin) {
    std::size_t i = 0;
    for (; i < needle.size() && begin[i]; ++i) {
      char left = begin[i];
      char right = needle[i];
      if (left >= 'A' && left <= 'Z') left = static_cast<char>(left - 'A' + 'a');
      if (right >= 'A' && right <= 'Z') right = static_cast<char>(right - 'A' + 'a');
      if (left != right) break;
    }
    if (i == needle.size()) return true;
  }
  return false;
}

bool has_active_condition(const world::Object& object) {
  for (std::uint32_t i = 0; i < object.field_count; ++i) {
    const auto& field = object.fields[i];
    // These two export fields are condition metadata, not condition hooks.
    // `condition_desc_pydata=ConditionTable` is a type label, and
    // `condition_desc=Invalid` is the empty/default marker.
    if (field_name_contains(field.name, "condition_desc")) continue;
    if (field_name_contains(field.name, "cond") &&
        !condition_is_absent(field.value))
      return true;
  }
  return false;
}

bool has_script(const world::Object& object) {
  for (std::uint32_t i = 0; i < object.field_count; ++i) {
    const auto& field = object.fields[i];
    if (field_name_contains(field.name, "script") && non_whitespace(field.value))
      return true;
  }
  return false;
}

bool is_known_factory_template(const char* name) {
  if (!name) return false;
  constexpr std::array<std::string_view, 4> known = {
      "MonsterCommonType1", "MonsterChampionType1", "NPC", "Faery"};
  return std::find(known.begin(), known.end(), name) != known.end();
}

bool parse_spawn_policy(const world::Object& object, bool& gated) {
  const auto* auto_spawn = dh2_world_field(&object, "auto_spawn");
  const auto* ai_state = dh2_world_field(&object, "ai_state");
  const auto* spawn_prob = dh2_world_field(&object, "spawn_prob");
  if (spawn_prob && *spawn_prob && std::strcmp(spawn_prob, "100") != 0)
    return false;

  const bool is_disabled = auto_spawn && std::strcmp(auto_spawn, "0") == 0;
  if (auto_spawn && std::strcmp(auto_spawn, "0") != 0 &&
      std::strcmp(auto_spawn, "1") != 0)
    return false;
  if (is_disabled) {
    if (!ai_state || std::strcmp(ai_state, "Limbus") != 0) return false;
    gated = true;
    return true;
  }
  if (ai_state && *ai_state && std::strcmp(ai_state, "Idle") != 0)
    return false;
  gated = false;
  return true;
}

struct ProjectedActor {
  CryptGeneratedDactSourceV1 source;
  float position[3] = {};
  float rotation[3] = {};
  float scale[3] = {};
  bool gated = false;
};

}  // namespace

CryptGeneratedDactStatusV1 crypt_compile_generated_dact_v1(
    const world::SourceLevel& source,
    const data::CharacterTable& characters,
    const data::Dictionary& models,
    CryptGeneratedDactV1& output,
    CryptGeneratedDactDiagnosticV1* diagnostic,
    const std::vector<CryptGeneratedDactRetainedSourceV1>* retained_sources) noexcept {
  output = {};
  if (diagnostic) *diagnostic = {};
  if (!source.modules || !source.module_count ||
      source.module_count > kMaximumRooms ||
      (source.entity_count && !source.entities)) {
    return reject(diagnostic, Status::argument,
                  "Generated source level has no usable module/entity table");
  }
  for (std::uint32_t i = 0; i < source.module_count; ++i) {
    if (!source.modules[i].mgp_loaded || !source.modules[i].cache_mgp ||
        !identity_module_transform(source.modules[i].record)) {
      return reject(diagnostic, Status::incomplete_module_gameplay,
                    "Every generated module needs loaded MGP data and translation-only placement",
                    i);
    }
  }

  try {
    std::vector<ProjectedActor> actors;
    std::vector<CryptGeneratedFaeryV1> faery_descriptors;
    std::vector<CryptGeneratedDactSkippedV1> skipped;
    std::vector<std::uint32_t> next_source_record(source.module_count, 0);
    std::uint32_t last_module = 0;
    bool saw_mgp = false;
    bool has_gate = false;
    std::set<std::pair<std::uint32_t, std::string>> unique_names;

    for (std::uint32_t i = 0; i < source.entity_count; ++i) {
      const auto& object = source.entities[i];
      if (object.kind != world::RecordKind::mgp) continue;
      const auto module_index = object.module_index;
      if (module_index >= source.module_count || !object.source_path ||
          std::strcmp(object.source_path, source.modules[module_index].cache_mgp) ||
          (saw_mgp && module_index < last_module) ||
          object.source_record != next_source_record[module_index]) {
        return reject(diagnostic, Status::invalid_source_order,
                      "Imported MGP objects lost module or in-file source order",
                      module_index, object.source_record);
      }
      saw_mgp = true;
      last_module = module_index;
      ++next_source_record[module_index];

      if (retained_sources && object.gametype &&
          std::strcmp(object.gametype, "Character") == 0) {
        const auto retained = std::find_if(retained_sources->begin(),
            retained_sources->end(), [&](const auto& value) {
              return value.module_index == module_index &&
                  value.source_record == object.source_record &&
                  value.name == (object.name ? object.name : "");
            });
        if (retained == retained_sources->end()) {
          skipped.push_back({module_index, object.source_record,
              CryptGeneratedDactSkipReasonV1::object_manager_duplicate,
              object.name ? object.name : ""});
          continue;
        }
      }

      if (!object.gametype || std::strcmp(object.gametype, "Character")) {
        skipped.push_back({module_index, object.source_record,
                           CryptGeneratedDactSkipReasonV1::non_character,
                           object.name ? object.name : ""});
        continue;
      }

      const auto* template_name = dh2_world_field(&object, "_templateName");
      if (!template_name || !*template_name) {
        return reject(diagnostic, Status::unsupported_type,
                      "Character record has no source template discriminator",
                      module_index, object.source_record);
      }
      if (std::strcmp(template_name, "Monster")) {
        if (!is_known_factory_template(template_name)) {
          return reject(diagnostic, Status::unsupported_type,
                        "Character template is outside the reviewed DACT subset",
                        module_index, object.source_record);
        }
        if (std::strcmp(template_name, "Faery") == 0) {
          const auto* character = dh2_world_field(&object, "charpropsname");
          if (!object.name || !*object.name || !character || !*character) {
            return reject(diagnostic, Status::character_data,
                          "Faery Character lacks source identity or CharacterTable link",
                          module_index, object.source_record);
          }
          const auto* ai_id = data::property(characters, character, "AI");
          const auto* animation_id =
              data::property(characters, character, "AnimTable");
          const auto* model_id =
              data::property(characters, character, "ModelFile");
          const auto* faery_list_id =
              data::property(characters, character, "FaeryList");
          const auto* scale_x = data::property(characters, character, "Scale_X");
          const auto* scale_y = data::property(characters, character, "Scale_Y");
          const auto* scale_z = data::property(characters, character, "Scale_Z");
          if (!ai_id || *ai_id < 0 || !animation_id || *animation_id < 0 ||
              !model_id || *model_id < 0 ||
              static_cast<std::size_t>(*model_id) >= models.values.size() ||
              !faery_list_id ||
              !scale_x || !scale_y || !scale_z) {
            return reject(diagnostic, Status::character_data,
                          "Faery CharacterTable AI, animation, model, FaeryList, or scale reference is unresolved",
                          module_index, object.source_record);
          }
          if (models.values[static_cast<std::size_t>(*model_id)].empty()) {
            return reject(diagnostic, Status::character_data,
                          "Faery CharacterTable model dictionary reference is empty",
                          module_index, object.source_record);
          }

          CryptGeneratedFaeryV1 faery;
          faery.module_index = module_index;
          faery.source_record = object.source_record;
          faery.name = object.name;
          faery.object_type = object.gametype;
          faery.template_name = template_name;
          faery.character = character;
          faery.ai_table_id = *ai_id;
          faery.animation_table_id = *animation_id;
          faery.character_model_dictionary_index = *model_id;
          faery.character_model_path = models.values[static_cast<std::size_t>(*model_id)];
          faery.faery_list_table_id = *faery_list_id;
          const std::int32_t authored_scale[3] = {
              *scale_x, *scale_y, *scale_z};
          float position[3] = {};
          float rotation[3] = {};
          float scale[3] = {};
          for (unsigned axis = 0; axis < 3; ++axis) {
            position[axis] = object.world_position[axis];
            rotation[axis] = object.local.rotation_degrees[axis] +
                source.modules[module_index].record.local.rotation_degrees[axis];
            scale[axis] = static_cast<float>(
                static_cast<double>(object.local.scale[axis]) *
                authored_scale[axis] / 100.0);
            faery.authored_world_transform[axis] = position[axis];
            faery.authored_world_transform[3 + axis] = rotation[axis];
            faery.authored_world_transform[6 + axis] = scale[axis];
          }
          if (!finite_transform(position, 10000000.0f) ||
              !finite_transform(rotation, 3600.0f) ||
              !finite_transform(scale, 100.0f, true)) {
            return reject(diagnostic, Status::unsupported_transform,
                          "Faery Character transform is outside runtime limits",
                          module_index, object.source_record);
          }
          // Kept outside DACT actor enumeration; the upper layer adds the
          // source ObjectManager handle without creating an enemy instance.
          faery_descriptors.push_back(std::move(faery));
        }
        skipped.push_back({module_index, object.source_record,
                           CryptGeneratedDactSkipReasonV1::known_factory_template,
                           object.name ? object.name : ""});
        continue;
      }

      const auto* character = dh2_world_field(&object, "charpropsname");
      if (!character || !*character || !object.name || !*object.name) {
        return reject(diagnostic, Status::unsupported_type,
                      "Direct Monster lacks a name or CharacterTable link",
                      module_index, object.source_record);
      }
      if (has_active_condition(object)) {
        skipped.push_back({module_index, object.source_record,
                           CryptGeneratedDactSkipReasonV1::conditional_object,
                           object.name});
        continue;
      }
      if (has_script(object)) {
        skipped.push_back({module_index, object.source_record,
                           CryptGeneratedDactSkipReasonV1::scripted_object,
                           object.name});
        continue;
      }

      // Character::SafeGetCharPropsId prefers the runtime char_template row
      // over charpropsname. DACT v1 stores only the direct CharacterTable
      // link, so accepting this source route would silently lose the template
      // selection and its caller-owned RNG draw.
      const auto* character_template = dh2_world_field(&object, "char_template");
      if (character_template && *character_template) {
        return reject(diagnostic, Status::unsupported_type,
                      "Direct Monster char_template route requires source selection",
                      module_index, object.source_record);
      }

      bool gated = false;
      if (!parse_spawn_policy(object, gated)) {
        return reject(diagnostic, Status::unsupported_spawn_policy,
                      "Direct Monster has unsupported spawn probability/state policy",
                      module_index, object.source_record);
      }
      if (!unique_names.emplace(module_index, object.name).second) {
        return reject(diagnostic, Status::duplicate_identity,
                      "DACT v1/v2 requires one actor name per generated room",
                      module_index, object.source_record);
      }

      const auto* model_id = data::property(characters, character, "ModelFile");
      const auto* scale_x = data::property(characters, character, "Scale_X");
      const auto* scale_y = data::property(characters, character, "Scale_Y");
      const auto* scale_z = data::property(characters, character, "Scale_Z");
      if (!model_id || *model_id < 0 ||
          static_cast<std::size_t>(*model_id) >= models.values.size() ||
          !scale_x || !scale_y || !scale_z) {
        return reject(diagnostic, Status::character_data,
                      "CharacterTable model or authored scale is unresolved",
                      module_index, object.source_record);
      }
      const auto& model_path = models.values[static_cast<std::size_t>(*model_id)];
      const auto slash = model_path.find_last_of("/\\");
      const auto model_name = model_path.substr(
          slash == std::string::npos ? 0 : slash + 1);
      if (model_name.empty() || model_name == "." || model_name == ".." ||
          model_name.find("..") != std::string::npos ||
          model_name.find_first_of("/\\") != std::string::npos) {
        return reject(diagnostic, Status::character_data,
                      "CharacterTable model path has no safe DACT basename",
                      module_index, object.source_record);
      }

      ProjectedActor actor;
      actor.source.module_index = module_index;
      actor.source.source_record = object.source_record;
      actor.source.dact_record = static_cast<std::uint32_t>(actors.size());
      actor.source.name = object.name;
      actor.source.character = character;
      actor.source.model = model_name;
      actor.gated = gated;
      has_gate = has_gate || gated;
      const std::int32_t authored_scale[3] = {*scale_x, *scale_y, *scale_z};
      for (unsigned axis = 0; axis < 3; ++axis) {
        actor.position[axis] = object.world_position[axis];
        actor.rotation[axis] = object.local.rotation_degrees[axis] +
            source.modules[module_index].record.local.rotation_degrees[axis];
        actor.scale[axis] = static_cast<float>(
            static_cast<double>(object.local.scale[axis]) * authored_scale[axis] /
            100.0);
      }
      if (!finite_transform(actor.position, 10000000.0f) ||
          !finite_transform(actor.rotation, 3600.0f) ||
          !finite_transform(actor.scale, 100.0f, true)) {
        return reject(diagnostic, Status::unsupported_transform,
                      "Direct Monster transform is outside the DACT runtime limits",
                      module_index, object.source_record);
      }
      if (actors.size() == kMaximumRows) {
        return reject(diagnostic, Status::limit,
                      "Generated DACT actor count exceeds the runtime limit",
                      module_index, object.source_record);
      }
      actors.push_back(std::move(actor));
    }

    if (actors.empty()) {
      return reject(diagnostic, Status::unsupported_type,
                    "Generated MGP set contains no DACT-supported direct Monsters");
    }
    const auto dact_version = has_gate ? 2U : 1U;
    CryptGeneratedDactV1 candidate;
    candidate.bytes.assign(kHeaderBytes + actors.size() * kRecordBytes, 0);
    std::memcpy(candidate.bytes.data(), "DACT", 4);
    write_u32(candidate.bytes.data() + 4, dact_version);
    write_u32(candidate.bytes.data() + 8,
              static_cast<std::uint32_t>(actors.size()));
    write_u32(candidate.bytes.data() + 12, 0);

    for (std::size_t i = 0; i < actors.size(); ++i) {
      const auto& actor = actors[i];
      auto* record = candidate.bytes.data() + kHeaderBytes + i * kRecordBytes;
      write_u32(record, 1);
      write_u32(record + 4, actor.source.module_index);
      if (!write_ascii(record + 8, 64, actor.source.name.c_str()) ||
          !write_ascii(record + 72, 64, actor.source.character.c_str()) ||
          !write_ascii(record + 136, 64, actor.source.model.c_str())) {
        return reject(diagnostic, Status::character_data,
                      "Direct Monster identity is not printable ASCII or exceeds DACT fields",
                      actor.source.module_index, actor.source.source_record);
      }
      for (unsigned axis = 0; axis < 3; ++axis) {
        write_f32(record + 200 + axis * 4, actor.position[axis]);
        write_f32(record + 212 + axis * 4, actor.rotation[axis]);
        write_f32(record + 224 + axis * 4, actor.scale[axis]);
      }
      if (dact_version == 2) write_u32(record + 236, actor.gated ? 1U : 0U);
      candidate.source_order.push_back(actor.source);
    }
    candidate.faeries = std::move(faery_descriptors);
    candidate.skipped = std::move(skipped);
    output = std::move(candidate);
    return Status::ok;
  } catch (const std::bad_alloc&) {
    output = {};
    return reject(diagnostic, Status::allocation,
                  "Generated DACT allocation failed");
  } catch (...) {
    output = {};
    return reject(diagnostic, Status::allocation,
                  "Generated DACT compilation failed unexpectedly");
  }
}

}  // namespace dh2::random_level
