#pragma once

#include "crypt_generated_spawnpoints_v1.hpp"
#include "crypt_template_character_projection_v1.hpp"
#include "source_handle_ledger_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::data {
struct CharacterTable;
struct Dictionary;
}

namespace dh2::world {

struct GeneratedCryptActorHandleV1 {
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  std::uint32_t dact_record = 0;
  std::int32_t source_handle = -1;
  std::string name;
};

// An authored Character descriptor with templateName Faery. This stays
// outside GeneratedCryptActorHandleV1/DACT Monster actor enumeration until its
// original owner, AI and placement lifecycles are implemented.
struct GeneratedCryptFaerySourceV1 {
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  // Source ObjectManager registration key from the ordered handle ledger;
  // this does not imply a live Character instance has been created.
  std::int32_t source_handle = -1;
  std::string name;
  // MGP gametype selects the ObjectManager constructor; template_name is a
  // separate Character property used later for AI/script selection.
  std::string object_type;
  std::string template_name;
  std::string character;
  std::string activate_condition;
  std::string deactivate_condition;
  std::int32_t ai_table_id = -1;
  std::int32_t animation_table_id = -1;
  std::int32_t character_model_dictionary_index = -1;
  std::string character_model_path;
  std::int32_t faery_list_table_id = -1;
  // Retained MGP `spawn_prob`; CheckSpawnProbability consumes the canonical
  // ObjectManager GameObject threshold rather than a Faery-local cache.
  std::int32_t spawn_probability = 100;
  float authored_world_transform[9] = {};
};

// Source-backed non-Faery Character template route. This retains the source
// ObjectManager key and MGP properties while keeping the actor out of DACT;
// resolution stays unattempted until the runtime supplies the actual catalog.
struct GeneratedCryptTemplateCharacterSourceV1 {
  std::int32_t source_handle = -1;
  crypt_template_character_projection_v1::Descriptor projection;
  bool gated_spawn = false;
  std::int16_t property_cache = -1;
  std::int16_t template_cache = -1;
};

// A Character with an authored `activate_cond` is retained independently of
// DACT. The Level/Quest owner must evaluate the named ConditionData before it
// asks the canonical Character factory to construct this source object.
// Retention alone never activates or inserts it into the live roster.
struct GeneratedCryptConditionalCharacterSourceV1 {
  // Assigned by the selected source-Level owner after generation; prevents a
  // retained MGP descriptor from activating in a later Level instance.
  std::uint32_t level_generation = 0;
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  std::int32_t source_handle = -1;
  std::string name;
  std::string editor_template_name;
  std::string character;
  std::string activate_condition;
  std::int32_t character_property_id = -1;
  float world_transform[9] = {};
};

// A source TriggerZone stays out of DACT, but its exact ObjectManager
// provenance and resolved owner transform are retained for the existing
// TriggerZone/contact runtime.
struct GeneratedCryptTriggerZoneSourceV1 {
  bool present = false;
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  std::int32_t source_handle = -1;
  std::string name;
  std::string script_name;
  std::int32_t activation_limit = 1;
  std::int32_t configured_delay_ms = 0;
  std::uint8_t has_script_move_out = 0;
  std::uint8_t has_script_all_player = 0;
  std::uint8_t has_script_all_player_move_out = 0;
  std::uint8_t has_one_player_effect = 0;
  std::uint8_t has_associated_door = 0;
  float world_position[3] = {};
  float owner_scale[3] = {};
};

// Door source-owner lookup facts for later Script_OpenDoor dispatch. This is
// only the exact ObjectManager identity and authored transform/configuration;
// it does not claim Door animation, collision, or scene playback is bound.
struct GeneratedCryptDoorSourceV1 {
  bool present = false;
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  std::int32_t source_handle = -1;
  std::string name;
  std::string data;
  std::int32_t opened = 0;
  float world_position[3] = {};
  float world_rotation[3] = {};
  float owner_scale[3] = {};
};

// Mirrors ObjectManager::GetObjectByName exact-name lookup over the retained
// first-owner Door candidates. It does not fabricate a Door instance.
const GeneratedCryptDoorSourceV1* find_generated_crypt_door_source_v1(
    const std::vector<GeneratedCryptDoorSourceV1>& sources,
    const char* exact_name) noexcept;

// Imports the generated Level and exact source MGPs, then projects the
// currently modeled direct-Monster subset to the native DACT v1/v2 format.
// Known factories, conditions and scripts are counted as deferred. Unknown
// Character templates and unsupported direct actors fail closed.
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
    std::vector<GeneratedCryptFaerySourceV1>* faery_sources = nullptr,
    std::vector<GeneratedCryptTemplateCharacterSourceV1>* template_sources = nullptr,
    const character::template_factory::Catalog* template_catalog = nullptr,
    std::vector<GeneratedCryptTriggerZoneSourceV1>* trigger_sources = nullptr,
    std::vector<GeneratedCryptDoorSourceV1>* door_sources = nullptr,
    std::vector<GeneratedCryptConditionalCharacterSourceV1>*
        conditional_character_sources = nullptr);

}  // namespace dh2::world
