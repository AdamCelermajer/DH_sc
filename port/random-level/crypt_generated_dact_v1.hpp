#pragma once

#include "../game-data/data.hpp"
#include "../world-data/world.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::random_level {

enum class CryptGeneratedDactStatusV1 : std::uint32_t {
  ok,
  argument,
  incomplete_module_gameplay,
  invalid_source_order,
  unsupported_type,
  unsupported_spawn_policy,
  character_data,
  unsupported_transform,
  duplicate_identity,
  limit,
  allocation
};

enum class CryptGeneratedDactSkipReasonV1 : std::uint32_t {
  non_character,
  known_factory_template,
  conditional_object,
  scripted_object,
  object_manager_duplicate
};

struct CryptGeneratedDactRetainedSourceV1 {
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  std::string name;
};

struct CryptGeneratedDactSourceV1 {
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  std::uint32_t dact_record = 0;
  std::string name;
  std::string character;
  std::string model;
};

// Source-faithful metadata for an authored Character with templateName Faery. This is
// deliberately separate from DACT records: it is not a Monster/enemy actor
// and does not imply that its source factory or AI has been activated.
struct CryptGeneratedFaeryV1 {
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  std::string name;
  // MGP `gametype` selects ObjectManager::objectCreationMap; distinct from
  // `template_name` (`_templateName=Faery`).
  std::string object_type;
  std::string template_name;
  std::string character;
  // CharacterTable property values. The authored Character model is only a
  // hint: Character::GetCharModelName replaces it with the parent's selected
  // FaeryTable model when this Character is attached to a parent.
  std::int32_t ai_table_id = -1;
  std::int32_t animation_table_id = -1;
  std::int32_t character_model_dictionary_index = -1;
  std::string character_model_path;
  // Raw CharacterTable value. Character::GetCharFaeryListId selects list row
  // zero when this authored value is negative or outside the loaded table.
  std::int32_t faery_list_table_id = -1;
  // Authored MGP transform before Character initialization; Faery InitFinal
  // later places it relative to its parent. Module origin is translation-only.
  float authored_world_transform[9] = {};
};

struct CryptGeneratedDactSkippedV1 {
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  CryptGeneratedDactSkipReasonV1 reason =
      CryptGeneratedDactSkipReasonV1::non_character;
  std::string name;
};

struct CryptGeneratedDactV1 {
  // Current native object loader format: DACT v1 for unconditional rows or
  // DACT v2 when source-authored Limbus/auto_spawn=0 actors are retained.
  std::vector<std::uint8_t> bytes;
  // In-memory provenance. DACT itself stores room/module and object name, but
  // has no source_record or stable numeric ObjectManager-handle field.
  std::vector<CryptGeneratedDactSourceV1> source_order;
  // Typed source descriptors only. These are not serialized into DACT.
  std::vector<CryptGeneratedFaeryV1> faeries;
  std::vector<CryptGeneratedDactSkippedV1> skipped;
};

struct CryptGeneratedDactDiagnosticV1 {
  CryptGeneratedDactStatusV1 status = CryptGeneratedDactStatusV1::ok;
  std::uint32_t module_index = 0xffffffffU;
  std::uint32_t source_record = 0xffffffffU;
  char message[192] = {};
};

// Project source-ordered MGP GameObjects from an already imported generated
// SourceLevel into the runtime DACT actor subset. Only direct Monster records
// with resolved CharacterTable data are emitted. The known Character template
// Faery is retained as separate metadata and remains absent from DACT.
// Other known non-direct factories, conditions and scripts are returned as
// skipped provenance; unknown Character templates and invalid direct actors
// fail the whole compile.
// Module placement must already satisfy world-data's translation-only rule.
CryptGeneratedDactStatusV1 crypt_compile_generated_dact_v1(
    const world::SourceLevel& source,
    const data::CharacterTable& characters,
    const data::Dictionary& models,
    CryptGeneratedDactV1& output,
    CryptGeneratedDactDiagnosticV1* diagnostic = nullptr,
    const std::vector<CryptGeneratedDactRetainedSourceV1>* retained_sources =
        nullptr) noexcept;

}  // namespace dh2::random_level
