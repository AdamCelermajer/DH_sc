#include "crypt_generated_source_dact_v1.hpp"

#include "../random-level/crypt_generated_dact_v1.hpp"
#include "../world-data/world.hpp"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <utility>
#include <vector>

namespace dh2::world {
namespace {

struct SourceOwner {
  SourceLevel value{};
  ~SourceOwner() { dh2_world_free(&value); }
};

bool parse_i32(const char* text, std::int32_t* value) {
  if (!text || !*text || !value) return false;
  errno = 0;
  char* end = nullptr;
  const auto parsed = std::strtol(text, &end, 10);
  if (errno || end == text || *end ||
      parsed < std::numeric_limits<std::int32_t>::min() ||
      parsed > std::numeric_limits<std::int32_t>::max()) return false;
  *value = static_cast<std::int32_t>(parsed);
  return true;
}

bool has_active_condition(const Object& object) {
  const auto* condition = dh2_world_field(&object, "activate_cond");
  if (!condition || !*condition) return false;
  constexpr char invalid[] = "invalid";
  std::size_t index = 0;
  while (invalid[index] && condition[index] &&
         std::tolower(static_cast<unsigned char>(condition[index])) ==
             invalid[index])
    ++index;
  return invalid[index] != '\0' || condition[index] != '\0';
}

}  // namespace

const GeneratedCryptDoorSourceV1* find_generated_crypt_door_source_v1(
    const std::vector<GeneratedCryptDoorSourceV1>& sources,
    const char* exact_name) noexcept {
  if (!exact_name || !*exact_name) return nullptr;
  for (const auto& source : sources) {
    if (source.present && source.source_handle >= 0 &&
        source.name == exact_name) return &source;
  }
  return nullptr;
}

bool compile_generated_crypt_dact_v1(
    const std::uint8_t* level_xml, std::size_t level_size,
    const char* level_name, const char* level_source_path,
    const GeneratedMgpView* mgps, std::size_t mgp_count,
    const dh2::data::CharacterTable& characters,
    const dh2::data::Dictionary& models,
    const source_handle_ledger_v1::Ledger& handle_ledger,
    std::vector<GeneratedCryptActorHandleV1>& actor_sources,
    std::vector<std::uint8_t>& output, std::size_t& actor_count,
    std::size_t& deferred_count, std::string& error,
    std::vector<GeneratedCryptFaerySourceV1>* faery_sources,
    std::vector<GeneratedCryptTemplateCharacterSourceV1>* template_sources,
    const character::template_factory::Catalog* template_catalog,
    std::vector<GeneratedCryptTriggerZoneSourceV1>* trigger_sources,
    std::vector<GeneratedCryptDoorSourceV1>* door_sources,
    std::vector<GeneratedCryptConditionalCharacterSourceV1>*
        conditional_character_sources) {
  actor_sources.clear();
  if (faery_sources) faery_sources->clear();
  if (template_sources) template_sources->clear();
  if (trigger_sources) trigger_sources->clear();
  if (door_sources) door_sources->clear();
  if (conditional_character_sources) conditional_character_sources->clear();
  output.clear();
  actor_count = 0;
  deferred_count = 0;
  error.clear();
  if (!level_xml || !level_size || !level_name || !*level_name ||
      !level_source_path || !mgps) {
    error = "Generated Crypt DACT input is incomplete";
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
  if (!source.value.module_count || source.value.module_count > 256 ||
      mgp_count != source.value.module_count) {
    error = "Generated Crypt needs one ordered MGP for every Module";
    return false;
  }

  for (std::uint32_t i = 0; i < source.value.module_count; ++i) {
    if (!mgps[i].source_path || !mgps[i].data || !mgps[i].size) {
      error = "Generated Crypt MGP input is incomplete";
      return false;
    }
    const auto status = dh2_world_import_module_objects(&source.value, i,
        RecordKind::mgp, mgps[i].source_path, mgps[i].data, mgps[i].size,
        &diagnostic);
    if (status != Error::ok) {
      error = diagnostic.message[0] ? diagnostic.message
                                    : "Generated Crypt MGP import failed";
      return false;
    }
  }

  // Preserve Door ObjectManager keys for exact Script_OpenDoor name lookup.
  // This is a source index only; the Door animation/update owner remains
  // unimplemented and must not be synthesized from these facts.
  if (door_sources) {
    for (std::uint32_t index = 0; index < source.value.entity_count; ++index) {
      const auto& object = source.value.entities[index];
      if (object.kind != RecordKind::mgp || !object.gametype ||
          std::strcmp(object.gametype, "Door") != 0) continue;
      if (!object.name || !*object.name ||
          object.module_index >= source.value.module_count) {
        error = "Generated Crypt Door is missing source identity/provenance";
        return false;
      }
      const auto occurrence = std::find_if(handle_ledger.occurrences.begin(),
          handle_ledger.occurrences.end(), [&](const auto& value) {
            return value.source.kind == RecordKind::mgp &&
                value.source.module_index == object.module_index &&
                value.source.record_index == object.source_record &&
                value.source.name == object.name && value.source.gametype == "Door";
          });
      if (occurrence == handle_ledger.occurrences.end() ||
          occurrence->handle < 0) {
        error = "Generated Crypt Door has no ObjectManager source handle";
        return false;
      }
      if (occurrence->duplicate_name) continue;
      const auto* template_name = dh2_world_field(&object, "_templateName");
      const auto* data_name = dh2_world_field(&object, "data");
      const auto* opened = dh2_world_field(&object, "opened");
      std::int32_t opened_value = 0;
      if (!template_name || std::strcmp(template_name, "Door") != 0 ||
          !data_name || !*data_name || !opened ||
          !parse_i32(opened, &opened_value) ||
          (opened_value != 0 && opened_value != 1)) {
        error = "Generated Crypt Door configuration is outside the recovered source contract";
        return false;
      }
      GeneratedCryptDoorSourceV1 retained;
      retained.present = true;
      retained.module_index = object.module_index;
      retained.source_record = object.source_record;
      retained.source_handle = occurrence->handle;
      retained.name = object.name;
      retained.data = data_name;
      retained.opened = opened_value;
      for (unsigned axis = 0; axis < 3; ++axis) {
        retained.world_position[axis] = object.world_position[axis];
        retained.world_rotation[axis] = object.local.rotation_degrees[axis];
        retained.owner_scale[axis] = object.local.scale[axis];
        if (!std::isfinite(retained.world_position[axis]) ||
            !std::isfinite(retained.world_rotation[axis]) ||
            !std::isfinite(retained.owner_scale[axis]) ||
            retained.owner_scale[axis] <= 0.0f) {
          error = "Generated Crypt resolved Door transform is invalid";
          return false;
        }
      }
      door_sources->push_back(std::move(retained));
    }
  }

  // Retain authored TriggerZones as source GameObject descriptors; they are
  // never DACT Characters. The ObjectManager ledger decides which duplicate
  // name owns the one live source object.
  if (trigger_sources) {
    for (std::uint32_t index = 0; index < source.value.entity_count; ++index) {
      const auto& object = source.value.entities[index];
      if (object.kind != RecordKind::mgp || !object.gametype ||
          std::strcmp(object.gametype, "TriggerZone") != 0) continue;
      if (!object.name || !*object.name || object.module_index >= source.value.module_count) {
        error = "Generated Crypt TriggerZone is missing source identity/provenance";
        return false;
      }
      const auto occurrence = std::find_if(handle_ledger.occurrences.begin(),
          handle_ledger.occurrences.end(), [&](const auto& value) {
            return value.source.kind == RecordKind::mgp &&
                value.source.module_index == object.module_index &&
                value.source.record_index == object.source_record &&
                value.source.name == object.name &&
                value.source.gametype == "TriggerZone";
          });
      if (occurrence == handle_ledger.occurrences.end() ||
          occurrence->handle < 0) {
        error = "Generated Crypt TriggerZone has no ObjectManager source handle";
        return false;
      }
      // ObjectManager::Add keeps the first source object with a duplicate name
      // and destroys later candidates. Repeated selected modules therefore
      // share the first trigger owner and must not produce another contact VM.
      if (occurrence->duplicate_name) continue;
      const auto* template_name = dh2_world_field(&object, "_templateName");
      const auto* script = dh2_world_field(&object, "script");
      const auto* count = dh2_world_field(&object, "triggercount");
      const auto* delay = dh2_world_field(&object, "triggerdelay");
      const auto* move_out = dh2_world_field(&object, "script_move_out");
      const auto* all_player = dh2_world_field(&object, "script_all_player");
      const auto* all_move_out =
          dh2_world_field(&object, "script_all_player_move_out");
      const auto* one_player = dh2_world_field(&object, "effect_one_player");
      std::int32_t activation_limit = 0, configured_delay_ms = 0;
      if (!template_name || std::strcmp(template_name, "TriggerZone") != 0 ||
          !script || !count || !parse_i32(count, &activation_limit) ||
          (activation_limit != -1 && activation_limit < 0) ||
          !delay || !parse_i32(delay, &configured_delay_ms) ||
          configured_delay_ms < 0 ||
          dh2_world_field(&object, "dimensions") ||
          (dh2_world_field(&object, "door") &&
           *dh2_world_field(&object, "door")) ||
          (dh2_world_field(&object, "door_name") &&
           *dh2_world_field(&object, "door_name")) ||
          (dh2_world_field(&object, "associated_door") &&
           *dh2_world_field(&object, "associated_door"))) {
        error = "Generated Crypt TriggerZone configuration is outside the recovered source contract";
        return false;
      }
      GeneratedCryptTriggerZoneSourceV1 retained;
      retained.present = true;
      retained.module_index = object.module_index;
      retained.source_record = object.source_record;
      retained.source_handle = occurrence->handle;
      retained.name = object.name;
      retained.script_name = script;
      retained.activation_limit = activation_limit;
      retained.configured_delay_ms = configured_delay_ms;
      retained.has_script_move_out = move_out && *move_out;
      retained.has_script_all_player = all_player && *all_player;
      retained.has_script_all_player_move_out = all_move_out && *all_move_out;
      retained.has_one_player_effect = one_player && *one_player;
      retained.has_associated_door =
          (dh2_world_field(&object, "door") && *dh2_world_field(&object, "door")) ||
          (dh2_world_field(&object, "door_name") && *dh2_world_field(&object, "door_name")) ||
          (dh2_world_field(&object, "associated_door") && *dh2_world_field(&object, "associated_door"));
      for (unsigned axis = 0; axis < 3; ++axis) {
        retained.world_position[axis] = object.world_position[axis];
        retained.owner_scale[axis] = object.local.scale[axis];
        if (!std::isfinite(retained.world_position[axis]) ||
            !std::isfinite(retained.owner_scale[axis]) ||
            retained.owner_scale[axis] <= 0.0f) {
          error = "Generated Crypt resolved TriggerZone transform is invalid";
          return false;
        }
      }
      trigger_sources->push_back(std::move(retained));
    }
  }

  std::vector<dh2::random_level::CryptGeneratedDactRetainedSourceV1>
      retained_characters;
  retained_characters.reserve(source.value.entity_count);
  for (std::uint32_t index = 0; index < source.value.entity_count; ++index) {
    const auto& object = source.value.entities[index];
    if (object.kind != RecordKind::mgp || !object.gametype ||
        std::strcmp(object.gametype, "Character") != 0) continue;
    const auto found = std::find_if(handle_ledger.occurrences.begin(),
        handle_ledger.occurrences.end(), [&](const auto& occurrence) {
          return occurrence.source.kind == RecordKind::mgp &&
              occurrence.source.module_index == object.module_index &&
              occurrence.source.record_index == object.source_record &&
              occurrence.source.name == (object.name ? object.name : "") &&
              occurrence.source.gametype == "Character";
        });
    if (found == handle_ledger.occurrences.end()) {
      error = "Generated Crypt Character is absent from the source handle ledger";
      return false;
    }
    const auto duplicate_count = std::count_if(handle_ledger.occurrences.begin(),
        handle_ledger.occurrences.end(), [&](const auto& occurrence) {
          return occurrence.source.kind == RecordKind::mgp &&
              occurrence.source.module_index == object.module_index &&
              occurrence.source.record_index == object.source_record &&
              occurrence.source.name == (object.name ? object.name : "") &&
              occurrence.source.gametype == "Character";
        });
    if (duplicate_count != 1) {
      error = "Generated Crypt Character maps to multiple source registrations";
      return false;
    }
    if (!found->duplicate_name) {
      retained_characters.push_back({object.module_index, object.source_record,
                                     object.name ? object.name : ""});
    }
  }

  dh2::random_level::CryptGeneratedDactV1 projected;
  dh2::random_level::CryptGeneratedDactDiagnosticV1 projection_diagnostic{};
  const auto status = dh2::random_level::crypt_compile_generated_dact_v1(
      source.value, characters, models, projected, &projection_diagnostic,
      &retained_characters);
  if (status != dh2::random_level::CryptGeneratedDactStatusV1::ok) {
    error = projection_diagnostic.message[0]
        ? projection_diagnostic.message
        : "Generated Crypt actor projection failed";
    return false;
  }
  actor_sources.reserve(projected.source_order.size());
  for (std::size_t index = 0; index < projected.source_order.size(); ++index) {
    const auto& source_actor = projected.source_order[index];
    const auto found = std::find_if(handle_ledger.occurrences.begin(),
        handle_ledger.occurrences.end(), [&](const auto& occurrence) {
          return occurrence.source.kind == RecordKind::mgp &&
              occurrence.source.module_index == source_actor.module_index &&
              occurrence.source.record_index == source_actor.source_record &&
              occurrence.source.name == source_actor.name &&
              occurrence.source.gametype == "Character";
        });
    if (found == handle_ledger.occurrences.end() || found->duplicate_name ||
        found->handle < 0) {
      actor_sources.clear();
      error = "Generated Crypt DACT actor does not own its source handle";
      return false;
    }
    actor_sources.push_back({source_actor.module_index,
        source_actor.source_record, static_cast<std::uint32_t>(index),
        found->handle, source_actor.name});
  }
  if (faery_sources) {
    faery_sources->reserve(projected.faeries.size());
    for (const auto& source_faery : projected.faeries) {
      const auto found = std::find_if(handle_ledger.occurrences.begin(),
          handle_ledger.occurrences.end(), [&](const auto& occurrence) {
            return occurrence.source.kind == RecordKind::mgp &&
                occurrence.source.module_index == source_faery.module_index &&
                occurrence.source.record_index == source_faery.source_record &&
                occurrence.source.name == source_faery.name &&
                occurrence.source.gametype == "Character";
          });
      if (found == handle_ledger.occurrences.end() || found->duplicate_name ||
          found->handle < 0) {
        faery_sources->clear();
        actor_sources.clear();
        error = "Generated Crypt Faery does not own its source ObjectManager handle";
        return false;
      }
      GeneratedCryptFaerySourceV1 descriptor;
      descriptor.module_index = source_faery.module_index;
      descriptor.source_record = source_faery.source_record;
      descriptor.source_handle = found->handle;
      descriptor.name = source_faery.name;
      descriptor.object_type = source_faery.object_type;
      descriptor.template_name = source_faery.template_name;
      descriptor.character = source_faery.character;
      descriptor.ai_table_id = source_faery.ai_table_id;
      descriptor.animation_table_id = source_faery.animation_table_id;
      descriptor.character_model_dictionary_index =
          source_faery.character_model_dictionary_index;
      descriptor.character_model_path = source_faery.character_model_path;
      descriptor.faery_list_table_id = source_faery.faery_list_table_id;
      std::copy(source_faery.authored_world_transform,
          source_faery.authored_world_transform + 9,
          descriptor.authored_world_transform);
      faery_sources->push_back(std::move(descriptor));
    }
  }
  if (template_sources) {
    for (std::uint32_t index = 0; index < source.value.entity_count; ++index) {
      const auto& object = source.value.entities[index];
      if (object.kind != RecordKind::mgp || !object.gametype ||
          std::strcmp(object.gametype, "Character") != 0)
        continue;
      const auto* runtime_template = dh2_world_field(&object, "char_template");
      const auto* template_data =
          dh2_world_field(&object, "char_template_pydata");
      const auto* editor_template = dh2_world_field(&object, "_templateName");
      if (!runtime_template || !*runtime_template || !template_data ||
          !*template_data || !editor_template || !*editor_template ||
          std::strcmp(editor_template, "Faery") == 0)
        continue;

      const auto found = std::find_if(handle_ledger.occurrences.begin(),
          handle_ledger.occurrences.end(), [&](const auto& occurrence) {
            return occurrence.source.kind == RecordKind::mgp &&
                occurrence.source.module_index == object.module_index &&
                occurrence.source.record_index == object.source_record &&
                occurrence.source.name == (object.name ? object.name : "") &&
                occurrence.source.gametype == "Character";
          });
      if (found == handle_ledger.occurrences.end() || found->handle < 0) {
        template_sources->clear();
        actor_sources.clear();
        if (faery_sources) faery_sources->clear();
        error = "Generated Crypt template Character does not own its source ObjectManager handle";
        return false;
      }
      if (found->duplicate_name) continue;

      GeneratedCryptTemplateCharacterSourceV1 retained;
      std::string projection_error;
      const auto projection_status = template_catalog
          ? crypt_template_character_projection_v1::project(object,
                *template_catalog,
                character::template_factory::selection_required,
                &retained.projection, projection_error)
          : crypt_template_character_projection_v1::retain_source(
                object, &retained.projection, projection_error);
      if (projection_status !=
          crypt_template_character_projection_v1::Status::complete) {
        template_sources->clear();
        actor_sources.clear();
        if (faery_sources) faery_sources->clear();
        error = projection_error.empty()
            ? "Generated Crypt template Character source projection failed"
            : projection_error;
        return false;
      }
      retained.source_handle = found->handle;
      const auto* auto_spawn = dh2_world_field(&object, "auto_spawn");
      const auto* ai_state = dh2_world_field(&object, "ai_state");
      const auto* spawn_probability = dh2_world_field(&object, "spawn_prob");
      if ((auto_spawn && std::strcmp(auto_spawn, "0") != 0 &&
           std::strcmp(auto_spawn, "1") != 0) ||
          (spawn_probability && *spawn_probability &&
           std::strcmp(spawn_probability, "100") != 0)) {
        template_sources->clear();
        actor_sources.clear();
        if (faery_sources) faery_sources->clear();
        error = "Generated Crypt template Character has unsupported spawn policy";
        return false;
      }
      retained.gated_spawn = auto_spawn && std::strcmp(auto_spawn, "0") == 0;
      if ((retained.gated_spawn &&
           (!ai_state || std::strcmp(ai_state, "Limbus") != 0)) ||
          (!retained.gated_spawn && ai_state && *ai_state &&
           std::strcmp(ai_state, "Idle") != 0)) {
        template_sources->clear();
        actor_sources.clear();
        if (faery_sources) faery_sources->clear();
        error = "Generated Crypt template Character spawn state is unsupported";
        return false;
      }
      template_sources->push_back(std::move(retained));
    }
  }
  if (conditional_character_sources) {
    for (std::uint32_t index = 0; index < source.value.entity_count; ++index) {
      const auto& object = source.value.entities[index];
      if (object.kind != RecordKind::mgp || !object.gametype ||
          std::strcmp(object.gametype, "Character") != 0 ||
          !has_active_condition(object))
        continue;
      const auto* editor_template = dh2_world_field(&object, "_templateName");
      const auto* character = dh2_world_field(&object, "charpropsname");
      const auto* condition = dh2_world_field(&object, "activate_cond");
      if (!object.name || !*object.name || !editor_template ||
          !*editor_template || !character || !*character || !condition ||
          !*condition) {
        conditional_character_sources->clear();
        error = "Conditional Crypt Character is missing its source factory, property, or condition";
        return false;
      }
      const auto row = std::find(characters.names.begin(), characters.names.end(),
                                 character);
      if (row == characters.names.end()) {
        conditional_character_sources->clear();
        error = "Conditional Crypt Character property is absent from CharacterTable";
        return false;
      }
      const auto found = std::find_if(handle_ledger.occurrences.begin(),
          handle_ledger.occurrences.end(), [&](const auto& occurrence) {
            return occurrence.source.kind == RecordKind::mgp &&
                occurrence.source.module_index == object.module_index &&
                occurrence.source.record_index == object.source_record &&
                occurrence.source.name == object.name &&
                occurrence.source.gametype == "Character";
          });
      if (found == handle_ledger.occurrences.end() || found->duplicate_name ||
          found->handle < 0) {
        conditional_character_sources->clear();
        error = "Conditional Crypt Character does not own a unique source handle";
        return false;
      }
      GeneratedCryptConditionalCharacterSourceV1 retained;
      retained.module_index = object.module_index;
      retained.source_record = object.source_record;
      retained.source_handle = found->handle;
      retained.name = object.name;
      retained.editor_template_name = editor_template;
      retained.character = character;
      retained.activate_condition = condition;
      retained.character_property_id = static_cast<std::int32_t>(
          std::distance(characters.names.begin(), row));
      for (unsigned axis = 0; axis < 3; ++axis) {
        retained.world_transform[axis] = object.world_position[axis];
        retained.world_transform[3 + axis] = object.local.rotation_degrees[axis];
        retained.world_transform[6 + axis] = object.local.scale[axis];
      }
      conditional_character_sources->push_back(std::move(retained));
    }
  }
  actor_count = actor_sources.size();
  deferred_count = projected.skipped.size();
  output = std::move(projected.bytes);
  return true;
}

}  // namespace dh2::world
