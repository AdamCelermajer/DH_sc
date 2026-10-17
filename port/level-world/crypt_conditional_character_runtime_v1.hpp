#pragma once

#include "character_runtime_factory_v1.hpp"
#include "crypt_generated_source_dact_v1.hpp"
#include "source_level_owner_v1.hpp"
#include "../game-data/condition_data_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::crypt_conditional_character_runtime_v1 {

namespace factory = character_runtime_factory_v1;
namespace conditions = data::condition_data_v1;

struct Bindings {
  const conditions::Table* condition_table{};
  factory::Owner* character_factory{};
  void* quest_context{};
  conditions::QuestStateLookup quest_state{};
  const source_level_owner_v1::Owner* source_level_owner{};
  void* properties_context{};
  int (*initialize_source_properties)(void*,
      const world::GeneratedCryptConditionalCharacterSourceV1&,
      factory::Record&, std::string&){};
};

struct Result {
  factory::Record* character{};
  conditions::EvalResult condition{};
  factory::Result factory{};
};

enum class Status : std::uint8_t {
  created,
  condition_false,
  invalid_argument,
  condition_unavailable,
  stale_or_missing_source_level,
  factory_unavailable,
  factory_failed,
  property_initialization_failed,
  lifecycle_failed,
};

// Evaluates the authored ConditionData and, only when true, runs the existing
// canonical Character/ObjectManager/CharacterList factory and its explicit
// source-properties, InitPost and InitFinal stages. Missing live GSLevel,
// condition lookup, or lifecycle services fail closed before reporting a
// created actor. This owner does not manufacture a Level or Character.
Status activate(const Bindings&,
    const world::GeneratedCryptConditionalCharacterSourceV1&,
    const object_manager_runtime_owner_v1::GameObject&,
    const character::aggro_search::GameObject&,
    const factory::Services&, const factory::LifecycleServices&,
    Result*, std::string&);

}  // namespace dh2::crypt_conditional_character_runtime_v1
