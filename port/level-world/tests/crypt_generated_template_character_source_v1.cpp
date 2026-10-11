#include "../crypt_generated_source_dact_v1.hpp"
#include "../crypt_spawn_trigger.hpp"
#include "../../game-data/data.hpp"
#include "../character_template_catalog_v1.hpp"
#include "../crypt_conditional_character_runtime_v1.hpp"
#include "../../world-data/world.hpp"

#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using Bytes = std::vector<std::uint8_t>;

struct ActivationFixture {
  std::vector<dh2::character::aggro_search::Character*> roster;
  std::map<std::int32_t, std::int32_t> quest_states;
  std::array<int, dh2::character_runtime_factory_v1::component_count> components{};
  std::int32_t fail_component_id = -1;
  std::uint32_t rolled_back_components{};
};

int make_component(void* context, dh2::character_constructor_owner_v1::Component id,
    dh2::character_constructor_owner_v1::Identity,
    dh2::character_runtime_factory_v1::ComponentStorage::Slot* slot,
    std::string& error) {
  auto& fixture = *static_cast<ActivationFixture*>(context);
  if (fixture.fail_component_id == static_cast<std::int32_t>(id)) {
    fixture.fail_component_id = -1;
    error = "injected Character component failure after first component";
    return 1;
  }
  slot->canonical_owner = &fixture.components[static_cast<std::size_t>(id)];
  return 0;
}
int associate_component(void*, dh2::character_constructor_owner_v1::Association,
    dh2::character_constructor_owner_v1::Identity,
    const dh2::character_runtime_factory_v1::ComponentStorage&, std::string&) { return 0; }
int register_state(void*, dh2::character_constructor_owner_v1::Identity,
    std::uint32_t, std::string&) { return 0; }
void rollback_component(void* context, dh2::character_constructor_owner_v1::Action action,
    std::uint32_t, dh2::character_constructor_owner_v1::Identity,
    dh2::character_runtime_factory_v1::ComponentStorage&) noexcept {
  if (action == dh2::character_constructor_owner_v1::Action::component)
    ++static_cast<ActivationFixture*>(context)->rolled_back_components;
}
int roster_add(void* context, dh2::character::aggro_search::Character* character,
    bool duplicate, bool* appended) {
  auto& roster = static_cast<ActivationFixture*>(context)->roster;
  if (duplicate || !character || !appended) return 1;
  roster.push_back(character); *appended = true; return 0;
}
int roster_remove(void* context, dh2::character::aggro_search::Character* character,
    std::size_t* removed) {
  auto& roster = static_cast<ActivationFixture*>(context)->roster;
  if (!removed) return 1;
  const auto before = roster.size();
  roster.erase(std::remove(roster.begin(), roster.end(), character), roster.end());
  *removed = before - roster.size(); return 0;
}
bool lookup_quest_state(void* context, std::int32_t quest_id, std::int32_t* state) {
  const auto& states = static_cast<ActivationFixture*>(context)->quest_states;
  const auto found = states.find(quest_id);
  if (!state || found == states.end()) return false;
  *state = found->second; return true;
}
int initialize_properties(void*,
    const dh2::world::GeneratedCryptConditionalCharacterSourceV1& source,
    dh2::character_runtime_factory_v1::Record& record, std::string& error) {
  if (source.character_property_id < 0 || record.source_handle != source.source_handle) {
    error = "source Character property owner mismatch"; return 1;
  }
  return 0;
}
int lifecycle_stage(void*, dh2::character_runtime_factory_v1::Record&, std::string&) { return 0; }

void require(bool condition, const char* message) {
  if (!condition) throw std::runtime_error(message);
}

Bytes read(const std::filesystem::path& path) {
  std::ifstream input(path, std::ios::binary);
  if (!input) throw std::runtime_error("Unable to open: " + path.string());
  return {std::istreambuf_iterator<char>(input), {}};
}

constexpr char kLevel[] =
    "<Level><GameObject name=\"config\" gametype=\"LevelConfig\"/>"
    "<GameObject name=\"crypt_room\" gametype=\"Module\" "
    "dae=\"data/3d/modules/crypt/crypt.bdae\" "
    "mgp=\"data/3d/modules/crypt/mgp/crypt_test.mgp\" "
    "mvp=\"data/3d/modules/crypt/mvp/crypt_test.mvp\" "
    "xrefobject=\"_crypt_room\" position=\"10,20,30\" "
    "rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "<GameObject name=\"crypt_room_03\" gametype=\"Module\" "
    "dae=\"data/3d/modules/crypt/crypt.bdae\" "
    "mgp=\"data/3d/modules/crypt/mgp/crypt_test_03.mgp\" "
    "mvp=\"data/3d/modules/crypt/mvp/crypt_test_03.mvp\" "
    "xrefobject=\"_crypt_room_03\" position=\"100,200,300\" "
    "rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "<GameObject name=\"crypt_hall\" gametype=\"Module\" "
    "dae=\"data/3d/modules/crypt/crypt.bdae\" "
    "mgp=\"data/3d/modules/crypt/mgp/crypt_hall.mgp\" "
    "mvp=\"data/3d/modules/crypt/mvp/crypt_hall.mvp\" "
    "xrefobject=\"_crypt_hall\" position=\"-10,-20,-30\" "
    "rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "</Level>";

std::string actor_xml(const std::string& name, unsigned index) {
  return "<GameObject name=\"" + name + "\" gametype=\"Character\" "
      "position=\"" + std::to_string(index) + ",2,3\" "
      "rotation=\"0,0,0\" scale=\"1,1,1\" "
      "_templateName=\"MonsterCommonType1\" "
      "char_template_pydata=\"Charater_Templates\" "
      "char_template=\"GothicusCrypt_Ghosts\" "
      "auto_spawn=\"0\" ai_state=\"Limbus\" spawn_prob=\"100\"/>";
}

void run(const std::filesystem::path& assets) {
  const auto read_asset = [&](const char* relative) {
    return read(assets / relative);
  };
  const auto records = read_asset("data/character_properties_pyarray.bin");
  const auto names = read_asset("data/character_properties_pyarraynames.bin");
  const auto fields = read_asset("data/character_properties_pystructnames.bin");
  const auto model_names =
      read_asset("data/character_models_dictionary_pyarraynames.bin");
  const auto model_values =
      read_asset("data/character_models_dictionary_pyarray.bin");
  const auto class_names = read_asset("data/character_classes_pyarraynames.bin");
  const auto template_data = read_asset("data/character_templates_pyarray.bin");
  const auto template_names = read_asset("data/character_templates_pyarraynames.bin");
  dh2::data::CharacterTable characters;
  dh2::data::Dictionary models;
  dh2::character::template_factory::Catalog catalog;
  std::string error;
  require(dh2::data::load_characters({records.data(), records.size()},
      {names.data(), names.size()}, {fields.data(), fields.size()}, characters,
      error), error.c_str());
  require(dh2::data::load_dictionary({model_names.data(), model_names.size()},
      {model_values.data(), model_values.size()}, models, error), error.c_str());
  require(dh2::character::template_catalog_v1::load(template_data.data(),
      template_data.size(), template_names.data(), template_names.size(),
      class_names.data(), class_names.size(), characters, catalog, error),
      error.c_str());
  require(dh2::data::property(characters, "Crypt_Ghost", "ModelFile") &&
      dh2::data::property(characters, "Crypt_Ghost", "Scale_X"),
      "Packaged Crypt_Ghost CharacterTable row is incomplete");

  std::string mgp = "<Module>";
  std::array<std::string, 4> names_in_source = {
      "GhostAmbusher_0", "GhostAmbusher_1", "GhostAmbusher_2",
      "GhostAmbusher_3"};
  for (std::size_t index = 0; index < names_in_source.size(); ++index)
    mgp += actor_xml(names_in_source[index], static_cast<unsigned>(index));
  mgp += actor_xml(names_in_source[0], 4);
  mgp += "<GameObject name=\"direct_ghost\" gametype=\"Character\" "
      "position=\"0,0,0\" rotation=\"0,0,0\" scale=\"1,1,1\" "
      "_templateName=\"Monster\" charpropsname=\"Crypt_Ghost\" "
      "ai_state=\"Idle\" auto_spawn=\"1\" spawn_prob=\"100\"/>"
      "<GameObject name=\"_prim_TriggerZone_GhostAmbush01\" "
      "gametype=\"TriggerZone\" position=\"-1405.23,300.262,608.062\" "
      "rotation=\"0,0,0\" scale=\"5.06089,1.43277,1.0\" "
      "_templateName=\"TriggerZone\" script=\"GhostAmbush01\" "
      "triggercount=\"1\" triggerdelay=\"0\" script_move_out=\"\" "
      "script_all_player=\"\" script_all_player_move_out=\"\" "
      "effect_one_player=\"\"/>"
      "<GameObject name=\"_prim_TriggerZone_GhostAmbush01\" "
      "gametype=\"TriggerZone\" position=\"9999,9999,9999\" "
      "rotation=\"0,0,0\" scale=\"1,1,1\" "
      "_templateName=\"TriggerZone\" script=\"GhostAmbush01\" "
      "triggercount=\"1\" triggerdelay=\"0\" script_move_out=\"\" "
      "script_all_player=\"\" script_all_player_move_out=\"\" "
      "effect_one_player=\"\"/>"
      "<GameObject name=\"_prim_Door_FirstGate\" gametype=\"Door\" "
      "position=\"5,6,7\" rotation=\"0,0,180\" scale=\"1.3,1.3,1.3\" "
      "_templateName=\"Door\" data=\"Gate_BIG\" opened=\"0\"/>"
      "<GameObject name=\"_prim_NPC_WanderingPriest\" gametype=\"Character\" "
      "position=\"9,8,7\" rotation=\"0,90,0\" scale=\"1,1,1\" "
      "_templateName=\"NPC\" charpropsname=\"WanderingPriest\" "
      "activate_cond=\"RENE_FOLLOW\"/>"
      "<GameObject name=\"_prim_BigSkeleton_0\" gametype=\"Character\" "
      "position=\"1,2,3\" rotation=\"0,0,0\" scale=\"1,1,1\" "
      "_templateName=\"Monster\" charpropsname=\"Crypt_Ghost\" "
      "activate_cond=\"IsBefore_Gothicus2Survivors\"/>"
      "<GameObject name=\"_prim_BigSkeleton_1\" gametype=\"Character\" "
      "position=\"4,5,6\" rotation=\"0,0,0\" scale=\"1,1,1\" "
      "_templateName=\"Monster\" charpropsname=\"Crypt_Ghost\" "
      "activate_cond=\"IsBefore_Gothicus2Survivors\"/>"
      "<GameObject name=\"tail_ghost\" gametype=\"Character\" "
      "position=\"7,8,9\" rotation=\"0,0,0\" scale=\"1,1,1\" "
      "_templateName=\"Monster\" charpropsname=\"Crypt_Ghost\"/>"
      "</Module>";
  const std::string mgp_03 =
      "<Module>"
      "<GameObject name=\"_prim_TriggerZone_GhostAmbush03\" "
      "gametype=\"TriggerZone\" position=\"1385.11,295.688,573.477\" "
      "rotation=\"0,0,0\" scale=\"3.41928,1.51349,1.0\" "
      "_templateName=\"TriggerZone\" script=\"GhostAmbush03\" "
      "triggercount=\"1\" triggerdelay=\"0\" script_move_out=\"\" "
      "script_all_player=\"\" script_all_player_move_out=\"\" "
      "effect_one_player=\"\"/>"
      "<GameObject name=\"_prim_TriggerZone_GhostAmbush04\" "
      "gametype=\"TriggerZone\" position=\"-1392.52,295.688,573.477\" "
      "rotation=\"0,0,0\" scale=\"3.41928,1.51349,1.0\" "
      "_templateName=\"TriggerZone\" script=\"GhostAmbush04\" "
      "triggercount=\"1\" triggerdelay=\"0\" script_move_out=\"\" "
      "script_all_player=\"\" script_all_player_move_out=\"\" "
      "effect_one_player=\"\"/>"
      "<GameObject name=\"_prim_Door_corner_ne_001\" gametype=\"Door\" "
      "position=\"3.2309,2500,10\" rotation=\"0,0,180\" "
      "scale=\"1.29032,1.29032,1.29032\" _templateName=\"Door\" "
      "data=\"Gate_BIG\" opened=\"0\"/>"
      "<GameObject name=\"_prim_Door_corner_ne_001\" gametype=\"Door\" "
      "position=\"9999,9999,9999\" rotation=\"0,0,0\" scale=\"1,1,1\" "
      "_templateName=\"Door\" data=\"Gate_BIG\" opened=\"0\"/>"
      "</Module>";
  const std::string mgp_hall =
      "<Module>"
      "<GameObject name=\"_prim_TriggerZone_hallambush\" "
      "gametype=\"TriggerZone\" position=\"-3.28113,-693.084,0.0\" "
      "rotation=\"0,0,0\" scale=\"5.18841,1.77438,3.51033\" "
      "_templateName=\"TriggerZone\" script=\"GhostAmbushHallway\" "
      "triggercount=\"1\" triggerdelay=\"0\" script_move_out=\"\" "
      "script_all_player=\"\" script_all_player_move_out=\"\" "
      "effect_one_player=\"\"/>"
      "</Module>";

  const dh2::world::GeneratedMgpView mgps[] = {
      {"data/3d/modules/crypt/mgp/crypt_test.mgp",
       reinterpret_cast<const std::uint8_t*>(mgp.data()), mgp.size()},
      {"data/3d/modules/crypt/mgp/crypt_test_03.mgp",
       reinterpret_cast<const std::uint8_t*>(mgp_03.data()), mgp_03.size()},
      {"data/3d/modules/crypt/mgp/crypt_hall.mgp",
       reinterpret_cast<const std::uint8_t*>(mgp_hall.data()), mgp_hall.size()}};
  dh2::world::source_handle_ledger_v1::Ledger ledger;
  for (std::uint32_t index = 0; index < names_in_source.size() + 4; ++index) {
    if (index == names_in_source.size() + 2) {
      dh2::world::source_handle_ledger_v1::Occurrence occurrence;
      occurrence.handle = 506;
      occurrence.source.module_index = 0;
      occurrence.source.module_name = "crypt_room";
      occurrence.source.record_index = 6;
      occurrence.source.kind = dh2::world::RecordKind::mgp;
      occurrence.source.name = "_prim_TriggerZone_GhostAmbush01";
      occurrence.source.gametype = "TriggerZone";
      ledger.occurrences.push_back(std::move(occurrence));
      continue;
    }
    if (index == names_in_source.size() + 3) {
      dh2::world::source_handle_ledger_v1::Occurrence occurrence;
      occurrence.handle = 506;
      occurrence.duplicate_name = true;
      occurrence.source.module_index = 0;
      occurrence.source.module_name = "crypt_room";
      occurrence.source.record_index = 7;
      occurrence.source.kind = dh2::world::RecordKind::mgp;
      occurrence.source.name = "_prim_TriggerZone_GhostAmbush01";
      occurrence.source.gametype = "TriggerZone";
      ledger.occurrences.push_back(std::move(occurrence));
      continue;
    }
    const auto name = index < names_in_source.size()
        ? names_in_source[index]
        : index == names_in_source.size()
            ? names_in_source[0] : std::string("direct_ghost");
    dh2::world::source_handle_ledger_v1::Occurrence occurrence;
    occurrence.handle = index == names_in_source.size()
        ? 500 : static_cast<std::int32_t>(500 + index);
    occurrence.duplicate_name = index == names_in_source.size();
    occurrence.source.module_index = 0;
    occurrence.source.module_name = "crypt_room";
    occurrence.source.record_index = index;
    occurrence.source.kind = dh2::world::RecordKind::mgp;
    occurrence.source.name = name;
    occurrence.source.gametype = "Character";
    ledger.occurrences.push_back(std::move(occurrence));
  }
  const auto add_trigger_owner = [&](std::uint32_t module,
      std::uint32_t record, std::int32_t handle, const char* module_name,
      const char* name) {
    dh2::world::source_handle_ledger_v1::Occurrence occurrence;
    occurrence.handle = handle;
    occurrence.source.module_index = module;
    occurrence.source.module_name = module_name;
    occurrence.source.record_index = record;
    occurrence.source.kind = dh2::world::RecordKind::mgp;
    occurrence.source.name = name;
    occurrence.source.gametype = "TriggerZone";
    ledger.occurrences.push_back(std::move(occurrence));
  };
  add_trigger_owner(1, 0, 507, "crypt_room_03",
      "_prim_TriggerZone_GhostAmbush03");
  add_trigger_owner(1, 1, 508, "crypt_room_03",
      "_prim_TriggerZone_GhostAmbush04");
  add_trigger_owner(2, 0, 509, "crypt_hall",
      "_prim_TriggerZone_hallambush");
  const auto add_door_owner = [&](std::uint32_t module,
      std::uint32_t record, std::int32_t handle, const char* module_name,
      const char* name, bool duplicate = false) {
    dh2::world::source_handle_ledger_v1::Occurrence occurrence;
    occurrence.handle = handle;
    occurrence.duplicate_name = duplicate;
    occurrence.source.module_index = module;
    occurrence.source.module_name = module_name;
    occurrence.source.record_index = record;
    occurrence.source.kind = dh2::world::RecordKind::mgp;
    occurrence.source.name = name;
    occurrence.source.gametype = "Door";
    ledger.occurrences.push_back(std::move(occurrence));
  };
  add_door_owner(0, 8, 600, "crypt_room", "_prim_Door_FirstGate");
  add_door_owner(1, 2, 601, "crypt_room_03",
      "_prim_Door_corner_ne_001");
  add_door_owner(1, 3, 601, "crypt_room_03",
      "_prim_Door_corner_ne_001", true);
  const auto add_character_owner = [&](std::uint32_t record,
      std::int32_t handle, const char* name) {
    dh2::world::source_handle_ledger_v1::Occurrence occurrence;
    occurrence.handle = handle;
    occurrence.source.module_index = 0;
    occurrence.source.module_name = "crypt_room";
    occurrence.source.record_index = record;
    occurrence.source.kind = dh2::world::RecordKind::mgp;
    occurrence.source.name = name;
    occurrence.source.gametype = "Character";
    ledger.occurrences.push_back(std::move(occurrence));
  };
  add_character_owner(9, 700, "_prim_NPC_WanderingPriest");
  add_character_owner(10, 701, "_prim_BigSkeleton_0");
  add_character_owner(11, 702, "_prim_BigSkeleton_1");
  add_character_owner(12, 703, "tail_ghost");

  std::vector<dh2::world::GeneratedCryptActorHandleV1> actors;
  std::vector<dh2::world::GeneratedCryptTemplateCharacterSourceV1> templates;
  std::vector<dh2::world::GeneratedCryptTriggerZoneSourceV1> triggers;
  std::vector<dh2::world::GeneratedCryptDoorSourceV1> doors;
  std::vector<dh2::world::GeneratedCryptConditionalCharacterSourceV1>
      conditional_characters;
  std::vector<std::uint8_t> dact;
  std::size_t actor_count = 0;
  std::size_t deferred_count = 0;
  require(dh2::world::compile_generated_crypt_dact_v1(
      reinterpret_cast<const std::uint8_t*>(kLevel), sizeof(kLevel) - 1,
      "CRYPT_TEST", "data/scene/crypt_test.mlx", mgps, 3, characters, models,
      ledger, actors, dact, actor_count, deferred_count, error, nullptr,
      &templates, &catalog, &triggers, &doors, &conditional_characters),
      error.c_str());
  require(actor_count == 2 && actors.size() == 2 && templates.size() == 4 &&
      deferred_count == 16 && conditional_characters.size() == 3,
      "Template Characters must remain outside DACT while retaining descriptors");
  require(conditional_characters[0].source_handle == 700 &&
      conditional_characters[0].source_record == 9 &&
      conditional_characters[0].name == "_prim_NPC_WanderingPriest" &&
      conditional_characters[0].editor_template_name == "NPC" &&
      conditional_characters[0].character == "WanderingPriest" &&
      conditional_characters[0].activate_condition == "RENE_FOLLOW" &&
      conditional_characters[0].character_property_id >= 0 &&
      conditional_characters[1].source_handle == 701 &&
      conditional_characters[1].activate_condition ==
          "IsBefore_Gothicus2Survivors" &&
      conditional_characters[2].source_handle == 702 &&
      conditional_characters[2].activate_condition ==
          "IsBefore_Gothicus2Survivors",
      "Conditional Crypt Character factory/quest routes were not retained in source order");

  const auto condition_assets = [&](const char* suffix) {
    return read(assets / "original-cache/data/pydata" /
        (std::string("v2conditions_") + suffix + ".bin"));
  };
  auto condition_bytes = condition_assets("pyarray");
  auto condition_names = condition_assets("pyarraynames");
  auto condition_schema = condition_assets("pystructnames");
  auto condition_constants = condition_assets("pycst");
  dh2::data::condition_data_v1::Table condition_table;
  require(dh2::data::condition_data_v1::load(
      {condition_bytes.data(), static_cast<std::uint32_t>(condition_bytes.size())},
      {condition_names.data(), static_cast<std::uint32_t>(condition_names.size())},
      {condition_schema.data(), static_cast<std::uint32_t>(condition_schema.size())},
      {condition_constants.data(), static_cast<std::uint32_t>(condition_constants.size())},
      condition_table, error), error.c_str());

  const auto activate_source = [&](std::size_t source_index,
      std::map<std::int32_t, std::int32_t> states, bool expect_created,
      const char* condition_override = nullptr, bool level_valid = true,
      bool factory_failure_retry = false, bool stale_descriptor = false) {
    ActivationFixture fixture;
    fixture.quest_states = std::move(states);
    if (factory_failure_retry)
      fixture.fail_component_id = static_cast<std::int32_t>(
          dh2::character_constructor_owner_v1::Component::controllable);
    dh2::object_manager_runtime_owner_v1::Owner object_manager;
    const dh2::character_runtime_factory_v1::RosterServices roster_services{
        &fixture, &roster_add, &roster_remove};
    dh2::character_runtime_factory_v1::Owner factory(object_manager, roster_services);
    dh2::crypt_conditional_character_runtime_v1::Bindings bindings;
    bindings.condition_table = &condition_table;
    bindings.character_factory = &factory;
    bindings.quest_context = &fixture;
    bindings.quest_state = &lookup_quest_state;
    dh2::source_level_owner_v1::Owner source_level_owner;
    require(source_level_owner.begin_load() &&
        source_level_owner.request_level(13, 0, 0) &&
        source_level_owner.begin_source_load() &&
        source_level_owner.publish_projection(&fixture),
        "Source Level owner rejected the generated Crypt projection");
    if (level_valid) {
      dh2::source_level_owner_v1::NativeLevelBinding level_binding;
      level_binding.level_fields = &fixture;
      level_binding.quest_owner = &factory;
      level_binding.player_savegame = 0x300;
      level_binding.player_character = 0x400;
      level_binding.level_row = 13;
      level_binding.entry_point = 0;
      level_binding.difficulty = 0;
      level_binding.level_state = 38;
      level_binding.source_save_slot = 0;
      level_binding.character_table_size = 64;
      require(source_level_owner.bind_native_level(level_binding),
          "Source Level owner rejected a complete native Level binding");
    }
    bindings.source_level_owner = &source_level_owner;
    bindings.initialize_source_properties = &initialize_properties;
    dh2::character_runtime_factory_v1::Services services{
        &fixture, &make_component, &associate_component, &register_state,
        &rollback_component};
    dh2::character_runtime_factory_v1::LifecycleServices lifecycle{
        &fixture, &lifecycle_stage, &lifecycle_stage};
    dh2::object_manager_runtime_owner_v1::GameObject object_seed{};
    dh2::character::aggro_search::GameObject aggro_seed{};
    dh2::crypt_conditional_character_runtime_v1::Result result;
    auto source = conditional_characters[source_index];
    if (condition_override) source.activate_condition = condition_override;
    source.level_generation = source_level_owner.snapshot().generation +
        (stale_descriptor ? 1u : 0u);
    auto status = dh2::crypt_conditional_character_runtime_v1::activate(
        bindings, source, object_seed, aggro_seed,
        services, lifecycle, &result, error);
    if (factory_failure_retry) {
      require(status == dh2::crypt_conditional_character_runtime_v1::Status::factory_failed &&
          factory.size() == 0 && fixture.roster.empty() &&
          fixture.rolled_back_components == 1,
          "Failed Character construction did not roll back its partial owner transaction");
      status = dh2::crypt_conditional_character_runtime_v1::activate(
          bindings, source, object_seed, aggro_seed, services, lifecycle,
          &result, error);
    }
    require((status == dh2::crypt_conditional_character_runtime_v1::Status::created) ==
            expect_created, "Conditional Character activation disagreed with source quest guard");
    require(expect_created ? result.character != nullptr && fixture.roster.size() == 1 &&
                factory.find(source.source_handle) != nullptr &&
                result.character->init_final_complete()
            : result.character == nullptr && fixture.roster.empty() && factory.size() == 0,
        "Conditional Character did not use the canonical factory/lifecycle transaction");
  };
  activate_source(0, {{8, 12}}, true);   // RENE_FOLLOW: 12 < 13
  activate_source(0, {{8, 13}}, false);  // threshold is exclusive
  activate_source(1, {{27, 11}}, true);  // IsBefore: 11 < 12
  activate_source(1, {{27, 12}}, false);
  activate_source(2, {{27, 11}}, true);
  activate_source(2, {{27, 7}}, true, "IsAfter_Gothicus2Survivors");
  activate_source(2, {{27, 6}}, false, "IsAfter_Gothicus2Survivors");
  activate_source(0, {{8, 12}}, false, nullptr, false);
  activate_source(0, {{8, 12}}, true, nullptr, true, true);
  activate_source(0, {{8, 12}}, false, nullptr, true, false, true);
  for (std::size_t index = 0; index < templates.size(); ++index) {
    const auto& item = templates[index];
    const auto& projection = item.projection;
    require(item.source_handle == static_cast<std::int32_t>(500 + index) &&
        projection.module_index == 0 && projection.source_record == index &&
        projection.object_name == names_in_source[index] &&
        projection.object_type == "Character" &&
        projection.property_source.editor_template_name == "MonsterCommonType1" &&
        projection.property_source.template_data_class == "Charater_Templates" &&
        projection.property_source.template_name == "GothicusCrypt_Ghosts" &&
        projection.property_source.explicit_property_name.empty() &&
        projection.property_resolution_attempted &&
        projection.property_resolution.status ==
            dh2::character::template_factory::Status::selection_required &&
        projection.property_resolution.authored_alternatives ==
            std::vector<std::int32_t>({35, 35, 35, 35, 37}) &&
        projection.property_resolution.property_id == -1,
        "Template descriptor changed source order/handle or lost catalog-derived weighted slots");
  }
  require(actors[0].name == "direct_ghost" && actors[0].source_record == 5 &&
      actors[1].name == "tail_ghost" && actors[1].source_record == 12,
      "Conditional rows changed the retained DACT source order");
  require(triggers.size() == 4 && triggers[0].present &&
      triggers[0].source_handle == 506 &&
      triggers[0].module_index == 0 && triggers[0].source_record == 6 &&
      triggers[0].name == "_prim_TriggerZone_GhostAmbush01" &&
      triggers[0].script_name == "GhostAmbush01" &&
      triggers[0].activation_limit == 1 &&
      std::fabs(triggers[0].world_position[0] - (-1395.23f)) < 0.01f &&
      std::fabs(triggers[0].world_position[1] - 320.262f) < 0.01f &&
      std::fabs(triggers[0].world_position[2] - 638.062f) < 0.01f &&
      std::fabs(triggers[0].owner_scale[0] - 5.06089f) < 0.001f &&
      std::fabs(triggers[0].owner_scale[1] - 1.43277f) < 0.001f &&
      std::fabs(triggers[0].owner_scale[2] - 1.0f) < 0.001f,
      "GhostAmbush01 retains its first ObjectManager owner and ignores a later duplicate placement");
  const auto check_trigger = [&](std::size_t index, std::int32_t handle,
      std::uint32_t module, std::uint32_t record, const char* name,
      const char* script, const float position[3], const float scale[3]) {
    const auto& item = triggers[index];
    require(item.present && item.source_handle == handle &&
        item.module_index == module && item.source_record == record &&
        item.name == name && item.script_name == script &&
        item.activation_limit == 1,
        "Authored spawn trigger lost MGP source order, exact handle, or script identity");
    for (unsigned axis = 0; axis < 3; ++axis)
      require(std::fabs(item.world_position[axis] - position[axis]) < 0.01f &&
          std::fabs(item.owner_scale[axis] - scale[axis]) < 0.001f,
          "Authored spawn trigger lost module-resolved owner transform");
  };
  const float ambush03_position[] = {1485.11f, 495.688f, 873.477f};
  const float ambush03_scale[] = {3.41928f, 1.51349f, 1.0f};
  const float ambush04_position[] = {-1292.52f, 495.688f, 873.477f};
  const float hallway_position[] = {-13.28113f, -713.084f, -30.0f};
  const float hallway_scale[] = {5.18841f, 1.77438f, 3.51033f};
  check_trigger(1, 507, 1, 0, "_prim_TriggerZone_GhostAmbush03",
      "GhostAmbush03", ambush03_position, ambush03_scale);
  check_trigger(2, 508, 1, 1, "_prim_TriggerZone_GhostAmbush04",
      "GhostAmbush04", ambush04_position, ambush03_scale);
  check_trigger(3, 509, 2, 0, "_prim_TriggerZone_hallambush",
      "GhostAmbushHallway", hallway_position, hallway_scale);
  require(doors.size() == 2 && doors[0].present &&
      doors[0].source_handle == 600 && doors[0].module_index == 0 &&
      doors[0].source_record == 8 && doors[0].name == "_prim_Door_FirstGate" &&
      doors[0].data == "Gate_BIG" && doors[0].opened == 0 &&
      std::fabs(doors[0].world_position[0] - 15.0f) < 0.01f &&
      std::fabs(doors[0].world_position[1] - 26.0f) < 0.01f &&
      std::fabs(doors[0].world_position[2] - 37.0f) < 0.01f &&
      std::fabs(doors[0].world_rotation[2] - 180.0f) < 0.01f &&
      std::fabs(doors[0].owner_scale[0] - 1.3f) < 0.001f &&
      doors[1].present && doors[1].source_handle == 601 &&
      doors[1].module_index == 1 && doors[1].source_record == 2 &&
      doors[1].name == "_prim_Door_corner_ne_001" &&
      doors[1].data == "Gate_BIG" && doors[1].opened == 0 &&
      std::fabs(doors[1].world_position[0] - 103.2309f) < 0.01f &&
      std::fabs(doors[1].world_position[1] - 2700.0f) < 0.01f &&
      std::fabs(doors[1].world_position[2] - 310.0f) < 0.01f &&
      std::fabs(doors[1].world_rotation[2] - 180.0f) < 0.01f &&
      std::fabs(doors[1].owner_scale[0] - 1.29032f) < 0.001f,
      "OpenDoor exact-name targets retain first ObjectManager handles and source transforms");
  require(dh2::world::find_generated_crypt_door_source_v1(
          doors, "_prim_Door_FirstGate") == &doors[0] &&
      dh2::world::find_generated_crypt_door_source_v1(
          doors, "_prim_Door_corner_ne_001") == &doors[1] &&
      !dh2::world::find_generated_crypt_door_source_v1(
          doors, "_prim_Door_firstgate") &&
      !dh2::world::find_generated_crypt_door_source_v1(doors, "_missing") &&
      !dh2::world::find_generated_crypt_door_source_v1(doors, ""),
      "OpenDoor source lookup is exact-name and fails closed for missing or differently-cased targets");

  const auto common_names = read_asset("scripts/scripts_pyscriptnames.bin");
  const auto common_programs = read_asset("scripts/scripts_pyscripts.bin");
  const auto crypt_names = read_asset("scripts/007_crypt_01_pyscriptnames.bin");
  const auto crypt_programs = read_asset("scripts/007_crypt_01_pyscripts.bin");
  dh2_script_table common{}, crypt{};
  dh2_script_error script_error{};
  require(dh2_script_table_decode(common_names.data(),
      static_cast<std::uint32_t>(common_names.size()), common_programs.data(),
      static_cast<std::uint32_t>(common_programs.size()), &common,
      &script_error) == DH2_SCRIPT_OK, "Decode packaged common script table");
  require(dh2_script_table_decode(crypt_names.data(),
      static_cast<std::uint32_t>(crypt_names.size()), crypt_programs.data(),
      static_cast<std::uint32_t>(crypt_programs.size()), &crypt,
      &script_error) == DH2_SCRIPT_OK, "Decode packaged Crypt script table");
  dh2_script_runtime::Options options{};
  dh2_script_runtime::init_options(&options);
  options.local_player_has_character = 1;
  dh2_script_runtime::Runtime runtime{};
  require(dh2_script_runtime::init(&runtime, &common, &crypt, nullptr, 0,
      triggers[0].name.c_str(), triggers[0].script_name.c_str(), 1, 0,
      &options) == dh2_script_runtime::ERROR_OK,
      "Initialize one shared Crypt ScriptManager owner");
  dh2_crypt_spawn_trigger::PlayerAabb player{{-100000.0f, -100000.0f,
      -100000.0f, 100000.0f, 100000.0f, 100000.0f}, 1};
  std::array<dh2_crypt_spawn_trigger::State, 4> trigger_states{};
  for (std::size_t index = 0; index < triggers.size(); ++index) {
    const auto& source = triggers[index];
    dh2_crypt_spawn_trigger::SourceFacts facts{};
    facts.trigger_name = source.name.c_str();
    facts.script_name = source.script_name.c_str();
    facts.module_gameplay_file = mgps[source.module_index].source_path;
    facts.inherited_zone_dimensions = {200.0f, 200.0f, 200.0f};
    facts.activation_limit = source.activation_limit;
    facts.configured_delay_ms = source.configured_delay_ms;
    dh2_crypt_spawn_trigger::Frame frame{};
    frame.owner_world_position = {source.world_position[0],
        source.world_position[1], source.world_position[2]};
    frame.owner_scale = {source.owner_scale[0], source.owner_scale[1],
        source.owner_scale[2]};
    frame.players = &player;
    frame.player_count = 1;
    frame.enabled = 1;
    dh2_crypt_spawn_trigger::init_state(&trigger_states[index]);
    const auto event_begin = runtime.event_count;
    const auto status = dh2_crypt_spawn_trigger::update(&runtime,
        &trigger_states[index], &facts, &frame);
    require(status == dh2_trigger_contact::STATUS_ACTIVATED &&
        trigger_states[index].trigger_activations == 1 &&
        trigger_states[index].trigger_fired,
        "Each authored trigger must consume its own one-shot state through the shared runtime");
    const dh2_script_runtime::Event* started = nullptr;
    for (std::uint32_t event_index = event_begin;
         event_index < runtime.event_count; ++event_index) {
      if (runtime.events[event_index].type ==
          dh2_script_runtime::EVENT_TRIGGER_STARTED)
        started = &runtime.events[event_index];
    }
    const auto expected_id = dh2_script_resolve_id(&common, &crypt,
        reinterpret_cast<const std::uint8_t*>(source.script_name.data()),
        static_cast<std::uint32_t>(source.script_name.size()), 0);
    require(started && started->detail == source.name &&
        started->value == expected_id,
        "Shared runtime must start each selected script in MGP source order");
    require(dh2_crypt_spawn_trigger::update(&runtime,
        &trigger_states[index], &facts, &frame) ==
            dh2_trigger_contact::STATUS_BLOCKED_ACTIVATION_COUNT,
        "Each authored trigger must keep its independent one-shot gate");
  }
  dh2_script_table_destroy(&crypt);
  dh2_script_table_destroy(&common);
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: crypt_generated_template_character_source_v1 assets-root\n";
    return 2;
  }
  try {
    run(argv[1]);
  std::cout << "generated Crypt sources retain conditional NPC/Big Skeleton routes; RENE_FOLLOW/Before/After guards invoke the canonical Character factory only with a verified Level; source cache projections pass\n";
    return 0;
  } catch (const std::exception& exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }
}
