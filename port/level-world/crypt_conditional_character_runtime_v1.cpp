#include "crypt_conditional_character_runtime_v1.hpp"

namespace dh2::crypt_conditional_character_runtime_v1 {

Status activate(const Bindings& bindings,
    const world::GeneratedCryptConditionalCharacterSourceV1& source,
    const object_manager_runtime_owner_v1::GameObject& game_object_seed,
    const character::aggro_search::GameObject& aggro_object_seed,
    const factory::Services& factory_services,
    const factory::LifecycleServices& lifecycle,
    Result* output, std::string& error) {
  error.clear();
  if (output) *output = {};
  if (!output || source.source_handle < 0 || source.name.empty() ||
      source.character.empty() || source.character_property_id < 0 ||
      source.activate_condition.empty()) {
    error = "Conditional Crypt Character source or result is incomplete";
    return Status::invalid_argument;
  }
  if (!bindings.source_level_owner) {
    error = "Conditional Crypt Character has no source Level owner";
    return Status::stale_or_missing_source_level;
  }
  const auto level = bindings.source_level_owner->snapshot();
  if (level.phase != source_level_owner_v1::Phase::active ||
      !level.renderer_projection || !level.source_gslevel ||
      !level.source_level || !level.source_level_savegame ||
      !level.level_fields || !level.quest_owner || !level.player_character ||
      level.source_level_state != 38 || !source.level_generation ||
      source.level_generation != level.generation) {
    error = "Conditional Crypt Character has no verified current source Level chain";
    return Status::stale_or_missing_source_level;
  }
  if (!bindings.condition_table || !bindings.quest_state) {
    error = "Conditional Crypt Character condition or canonical quest lookup is unavailable";
    return Status::condition_unavailable;
  }
  if (!bindings.character_factory ||
      !bindings.initialize_source_properties ||
      !lifecycle.init_post || !lifecycle.init_final) {
    error = "Conditional Crypt Character canonical factory lifecycle is incomplete";
    return Status::factory_unavailable;
  }
  const bool supported_template =
      source.editor_template_name == "Monster" ||
      source.editor_template_name == "NPC";
  const bool supported_condition =
      source.activate_condition == "RENE_FOLLOW" ||
      source.activate_condition == "IsBefore_Gothicus2Survivors" ||
      source.activate_condition == "IsAfter_Gothicus2Survivors";
  if (!supported_template || !supported_condition) {
    error = "Conditional Crypt Character factory or condition is unsupported";
    return Status::invalid_argument;
  }

  const auto* definition = conditions::find(*bindings.condition_table,
      source.activate_condition.c_str());
  if (!definition) {
    error = "Conditional Crypt Character ConditionData row is missing";
    return Status::condition_unavailable;
  }
  const auto condition_status = conditions::evaluate(*definition, false,
      bindings.quest_state, bindings.quest_context, &output->condition);
  if (condition_status != conditions::Status::complete) {
    error = "Conditional Crypt Character quest condition could not be evaluated";
    return Status::condition_unavailable;
  }
  if (!output->condition.value) {
    error.clear();
    return Status::condition_false;
  }

  factory::Record* record = nullptr;
  const auto created = bindings.character_factory->create(source.source_handle,
      game_object_seed, aggro_object_seed, factory_services, &record,
      &output->factory, error);
  if (created != factory::Status::complete || !record) {
    if (error.empty()) error = "Canonical Character factory rejected conditional source";
    return Status::factory_failed;
  }

  int properties_status = -1;
  try {
    properties_status = bindings.initialize_source_properties(
        bindings.properties_context, source, *record, error);
  } catch (...) {
    error = "Conditional Character source-property provider threw";
  }
  if (properties_status != 0) {
    if (error.empty()) error = "Conditional Character source properties failed";
    factory::Result retired{};
    std::string cleanup_error;
    (void)bindings.character_factory->retire(source.source_handle, &retired,
                                               cleanup_error);
    return Status::property_initialization_failed;
  }

  auto factory_status = bindings.character_factory->mark_source_properties_ready(
      source.source_handle, &output->factory, error);
  if (factory_status == factory::Status::complete)
    factory_status = bindings.character_factory->run_init_post(source.source_handle,
        lifecycle, &output->factory, error);
  if (factory_status == factory::Status::complete)
    factory_status = bindings.character_factory->run_init_final(source.source_handle,
        lifecycle, &output->factory, error);
  if (factory_status != factory::Status::complete) {
    factory::Result retired{};
    std::string cleanup_error;
    (void)bindings.character_factory->retire(source.source_handle, &retired,
                                               cleanup_error);
    if (error.empty()) error = "Conditional Character source lifecycle failed";
    return Status::lifecycle_failed;
  }
  output->character = record;
  error.clear();
  return Status::created;
}

}  // namespace dh2::crypt_conditional_character_runtime_v1
