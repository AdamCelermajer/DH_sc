#include "../crypt_generated_dact_v1.hpp"
#include "../../game-data/condition_data_v1.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using namespace dh2;

std::vector<std::uint8_t> read(const std::string& path) {
  std::ifstream file(path, std::ios::binary);
  if (!file) throw std::runtime_error("Unable to open: " + path);
  return {std::istreambuf_iterator<char>(file), {}};
}

void require(bool condition, const char* message) {
  if (!condition) throw std::runtime_error(message);
}

std::uint32_t word(const std::uint8_t* p) {
  return static_cast<std::uint32_t>(p[0]) |
      (static_cast<std::uint32_t>(p[1]) << 8) |
      (static_cast<std::uint32_t>(p[2]) << 16) |
      (static_cast<std::uint32_t>(p[3]) << 24);
}

void load_tables(const std::string& assets, data::CharacterTable& characters,
                 data::Dictionary& models) {
  const auto properties = read(assets + "/data/character_properties_pyarray.bin");
  const auto character_names = read(assets + "/data/character_properties_pyarraynames.bin");
  const auto character_fields = read(assets + "/data/character_properties_pystructnames.bin");
  const auto model_names = read(assets + "/data/character_models_dictionary_pyarraynames.bin");
  const auto model_values = read(assets + "/data/character_models_dictionary_pyarray.bin");
  std::string error;
  require(data::load_characters({properties.data(), properties.size()},
                                {character_names.data(), character_names.size()},
                                {character_fields.data(), character_fields.size()},
                                characters, error), error.c_str());
  require(data::load_dictionary({model_names.data(), model_names.size()},
                                {model_values.data(), model_values.size()},
                                models, error), error.c_str());
}

struct TestQuestState {std::int32_t quest_id=0,state=0;};
bool test_quest_state(void* context,std::int32_t quest_id,std::int32_t* state){
  const auto* value=static_cast<const TestQuestState*>(context);
  if(!value||!state||value->quest_id!=quest_id)return false;
  *state=value->state;return true;
}

void test_crypt_condition_data(const std::string& assets){
  using namespace data::condition_data_v1;
  const auto base=assets+"/original-cache/data/pydata/v2conditions_";
  const auto packed=read(base+"pyarray.bin");
  const auto names=read(base+"pyarraynames.bin");
  const auto schema=read(base+"pystructnames.bin");
  const auto constants=read(base+"pycst.bin");
  Table table;std::string error;
  require(load({packed.data(),std::uint32_t(packed.size())},
               {names.data(),std::uint32_t(names.size())},
               {schema.data(),std::uint32_t(schema.size())},
               {constants.data(),std::uint32_t(constants.size())},table,error),
          error.c_str());
  require(table.rows.size()==118,"Original condition-table row count differs");
  const auto* before=find(table,"IsBefore_Gothicus2Survivors");
  const auto* after=find(table,"IsAfter_Gothicus2Survivors");
  const auto* rene=find(table,"RENE_FOLLOW");
  require(before&&before->source_type==0&&before->predicates.size()==1&&
              before->predicates[0].operation==1&&before->predicates[0].quest_id==27&&
              before->predicates[0].required_state==12,
          "Source IsBefore_Gothicus2Survivors predicate changed");
  require(after&&after->source_type==0&&after->predicates.size()==1&&
              after->predicates[0].operation==2&&after->predicates[0].quest_id==27&&
              after->predicates[0].required_state==6,
          "Source IsAfter_Gothicus2Survivors predicate changed");
  require(rene&&rene->source_type==0&&rene->predicates.size()==1&&
              rene->predicates[0].operation==1&&rene->predicates[0].quest_id==8&&
              rene->predicates[0].required_state==13,
          "Source RENE_FOLLOW predicate changed");
  EvalResult result;TestQuestState quest{27,11};
  require(evaluate(*before,false,test_quest_state,&quest,&result)==Status::complete&&result.value,
          "Source lower-than quest condition should pass below threshold");
  quest.state=12;
  require(evaluate(*before,false,test_quest_state,&quest,&result)==Status::complete&&!result.value,
          "Source lower-than quest condition should fail at threshold");
  quest.state=7;
  require(evaluate(*after,false,test_quest_state,&quest,&result)==Status::complete&&result.value,
          "Source higher-than quest condition should pass above threshold");
  quest.state=6;
  require(evaluate(*after,false,test_quest_state,&quest,&result)==Status::complete&&!result.value,
          "Source higher-than quest condition should fail at threshold");
  quest={8,12};
  require(evaluate(*rene,false,test_quest_state,&quest,&result)==Status::complete&&result.value,
          "Source RENE_FOLLOW lower-than condition should pass below threshold");
  require(evaluate(*rene,false,nullptr,nullptr,&result)==Status::service_unavailable,
          "Condition evaluation did not fail closed without canonical quest lookup");
}

std::vector<std::array<std::uint8_t, 256>> actor_rows(
    const std::vector<std::uint8_t>& dact) {
  require(dact.size() >= 16 && std::memcmp(dact.data(), "DACT", 4) == 0,
          "DACT header rejected");
  const auto count = word(dact.data() + 8);
  require(dact.size() == 16 + static_cast<std::uint64_t>(count) * 256,
          "DACT record size rejected");
  std::vector<std::array<std::uint8_t, 256>> result;
  for (std::uint32_t i = 0; i < count; ++i) {
    const auto* row = dact.data() + 16 + i * 256;
    if (word(row) != 1) continue;
    std::array<std::uint8_t, 256> copy{};
    std::memcpy(copy.data(), row, copy.size());
    result.push_back(copy);
  }
  return result;
}

const world::Object* object_at(const world::SourceLevel& source,
                               std::uint32_t module,
                               std::uint32_t source_record) {
  for (std::uint32_t i = 0; i < source.entity_count; ++i) {
    const auto& object = source.entities[i];
    if (object.kind == world::RecordKind::mgp &&
        object.module_index == module && object.source_record == source_record)
      return &object;
  }
  return nullptr;
}

bool has_skip(const random_level::CryptGeneratedDactV1& result,
              std::uint32_t module, std::uint32_t record,
              random_level::CryptGeneratedDactSkipReasonV1 reason) {
  for (const auto& skipped : result.skipped)
    if (skipped.module_index == module && skipped.source_record == record &&
        skipped.reason == reason)
      return true;
  return false;
}

void set_field(world::Object& object, const char* field_name,
               const char* value) {
  for (std::uint32_t i = 0; i < object.field_count; ++i) {
    if (std::strcmp(object.fields[i].name, field_name) == 0) {
      object.fields[i].value = const_cast<char*>(value);
      return;
    }
  }
  throw std::runtime_error("Expected source field missing for test mutation");
}

void add_field(world::Object& object, const char* field_name,
               const char* value) {
  auto* grown = static_cast<world::Field*>(std::realloc(
      object.fields, (object.field_count + 1) * sizeof(world::Field)));
  require(grown != nullptr, "Test source field allocation failed");
  object.fields = grown;
  const auto key_length = std::strlen(field_name) + 1;
  const auto value_length = std::strlen(value) + 1;
  auto* key = static_cast<char*>(std::malloc(key_length));
  auto* text = static_cast<char*>(std::malloc(value_length));
  require(key && text, "Test source field text allocation failed");
  std::memcpy(key, field_name, key_length);
  std::memcpy(text, value, value_length);
  object.fields[object.field_count++] = {key, text};
}

void remove_last_field(world::Object& object) {
  require(object.field_count > 0, "No test source field to remove");
  auto& field = object.fields[--object.field_count];
  std::free(field.name);
  std::free(field.value);
  field = {};
}

void test_fail_closed(world::SourceLevel& source,
                      const data::CharacterTable& characters,
                      const data::Dictionary& models) {
  random_level::CryptGeneratedDactV1 output;
  random_level::CryptGeneratedDactDiagnosticV1 diagnostic{};
  auto* direct = const_cast<world::Object*>(object_at(source, 3, 3));
  require(direct != nullptr, "Synthetic script test actor missing");
  const auto* old_ai = dh2_world_field(direct, "ai_state");
  set_field(*direct, "ai_state", "Attack");
  // Unsupported actor states fail the transaction and expose no partial DACT.
  auto status = random_level::crypt_compile_generated_dact_v1(
      source, characters, models, output, &diagnostic);
  require(status == random_level::CryptGeneratedDactStatusV1::unsupported_spawn_policy,
          "Unsupported source AI state was accepted");
  require(output.bytes.empty(), "Failed DACT compile exposed partial bytes");
  set_field(*direct, "ai_state", old_ai ? old_ai : "");

  add_field(*direct, "script", "unsafe.lua");
  status = random_level::crypt_compile_generated_dact_v1(
      source, characters, models, output, &diagnostic);
  require(status == random_level::CryptGeneratedDactStatusV1::ok &&
              output.source_order.size() == 12 &&
              has_skip(output, 3, 3,
                       random_level::CryptGeneratedDactSkipReasonV1::scripted_object),
          "Scripted direct Monster was not omitted from DACT");
  remove_last_field(*direct);

  const auto* old_template = dh2_world_field(direct, "_templateName");
  set_field(*direct, "_templateName", "UnreviewedActorType");
  status = random_level::crypt_compile_generated_dact_v1(
      source, characters, models, output, &diagnostic);
  require(status == random_level::CryptGeneratedDactStatusV1::unsupported_type &&
              output.bytes.empty(),
          "Unknown Character factory type was guessed into DACT");
  set_field(*direct, "_templateName", old_template);

  // Runtime char_template selects Charater_Templates rows and requires the
  // original RNG draw. A direct DACT row carries only charpropsname, so reject
  // instead of silently projecting the wrong CharacterTable row.
  const auto* old_char_template = dh2_world_field(direct, "char_template");
  if (old_char_template)
    set_field(*direct, "char_template", "GothicusCrypt_Ghosts");
  else
    add_field(*direct, "char_template", "GothicusCrypt_Ghosts");
  status = random_level::crypt_compile_generated_dact_v1(
      source, characters, models, output, &diagnostic);
  require(status == random_level::CryptGeneratedDactStatusV1::unsupported_type &&
              output.bytes.empty(),
          "Direct Monster runtime template was silently projected as charpropsname");
  if (old_char_template)
    set_field(*direct, "char_template", old_char_template);
  else
    remove_last_field(*direct);

  // An active condition on a direct Monster is reported and omitted.
  auto* conditional = const_cast<world::Object*>(object_at(source, 7, 18));
  require(conditional != nullptr, "Conditional Crypt actor missing");
  const auto* old_condition = dh2_world_field(conditional, "activate_cond");
  set_field(*conditional, "activate_cond", "invalid");
  status = random_level::crypt_compile_generated_dact_v1(
      source, characters, models, output, &diagnostic);
  require(status == random_level::CryptGeneratedDactStatusV1::ok,
          "Removing a condition from a supported Monster failed");
  require(!has_skip(output, 7, 18,
                    random_level::CryptGeneratedDactSkipReasonV1::conditional_object),
          "Inactive condition marker was treated as executable");
  set_field(*conditional, "activate_cond", old_condition);

  // Conditions and scripts are never turned into unconditional DACT rows.
  // The shipped Crypt condition remains present and omitted in the baseline.
  status = random_level::crypt_compile_generated_dact_v1(
      source, characters, models, output, &diagnostic);
  require(status == random_level::CryptGeneratedDactStatusV1::ok &&
              has_skip(output, 7, 18,
                       random_level::CryptGeneratedDactSkipReasonV1::conditional_object),
          "Source conditional Monster was not explicitly deferred");

  auto* module = source.modules;
  const auto old_scale = module[0].record.local.scale[0];
  module[0].record.local.scale[0] = 2.0f;
  status = random_level::crypt_compile_generated_dact_v1(
      source, characters, models, output, &diagnostic);
  require(status == random_level::CryptGeneratedDactStatusV1::incomplete_module_gameplay &&
              output.bytes.empty(),
          "Scaled module placement was not rejected");
  module[0].record.local.scale[0] = old_scale;
  const auto old_rotation = module[0].record.local.rotation_degrees[2];
  module[0].record.local.rotation_degrees[2] = 90.0f;
  status = random_level::crypt_compile_generated_dact_v1(
      source, characters, models, output, &diagnostic);
  require(status == random_level::CryptGeneratedDactStatusV1::incomplete_module_gameplay &&
              output.bytes.empty(),
          "Rotated module placement was not rejected");
  module[0].record.local.rotation_degrees[2] = old_rotation;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: crypt_generated_dact_v1 cache-root android-assets-root\n";
    return 2;
  }
  try {
    const std::string cache_root = argv[1];
    const std::string assets = argv[2];
    test_crypt_condition_data(assets);
    const auto worlds = assets + "/worlds";
    const auto mlx = read(worlds + "/x07_crypt_backup.mlx");
    world::SourceLevel source{};
    world::Diagnostic source_diagnostic{};
    require(dh2_world_import_level(&source, "CRYPT",
                "data/scene/x07_crypt_backup.mlx", mlx.data(), mlx.size(),
                &source_diagnostic) == world::Error::ok,
            source_diagnostic.message);

    for (std::uint32_t i = 0; i < source.module_count; ++i) {
      const auto& module = source.modules[i];
      const auto mgp = read(cache_root + "/" + module.cache_mgp);
      require(dh2_world_import_module_objects(&source, i,
                  world::RecordKind::mgp, module.cache_mgp, mgp.data(),
                  mgp.size(), &source_diagnostic) == world::Error::ok,
              source_diagnostic.message);
    }

    data::CharacterTable characters;
    data::Dictionary models;
    load_tables(assets, characters, models);

    random_level::CryptGeneratedDactV1 generated;
    random_level::CryptGeneratedDactDiagnosticV1 diagnostic{};
    auto status = random_level::crypt_compile_generated_dact_v1(
        source, characters, models, generated, &diagnostic);
    require(status == random_level::CryptGeneratedDactStatusV1::ok,
            diagnostic.message);
    require(generated.source_order.size() == 13,
            "Fixed Crypt direct-Monster count differs from source evidence");
    const auto authored_faery = std::find_if(
        generated.faeries.begin(), generated.faeries.end(), [](const auto& row) {
          return row.name == "_prim_Faery";
        });
    require(authored_faery != generated.faeries.end() &&
                authored_faery->object_type == "Character" &&
                authored_faery->character == "DefaultFairy" &&
                authored_faery->ai_table_id == 20 &&
                authored_faery->animation_table_id == 23 &&
                authored_faery->character_model_dictionary_index == 34 &&
                authored_faery->faery_list_table_id == -1,
            "Authored DefaultFairy FaeryList fallback source value was rejected or rewritten");
    const auto skipped_count = [&](random_level::CryptGeneratedDactSkipReasonV1 reason) {
      return static_cast<std::size_t>(std::count_if(
          generated.skipped.begin(), generated.skipped.end(),
          [reason](const auto& row) { return row.reason == reason; }));
    };
    require(source.entity_count == 82 && generated.skipped.size() == 69 &&
                skipped_count(random_level::CryptGeneratedDactSkipReasonV1::non_character) == 50 &&
                skipped_count(random_level::CryptGeneratedDactSkipReasonV1::known_factory_template) == 17 &&
                skipped_count(random_level::CryptGeneratedDactSkipReasonV1::conditional_object) == 2,
            "Fixed Crypt MGP projection/deferred record accounting changed");
    // Reconcile every source MGP Character against the only categories this
    // loader exposes. A caller cannot treat the DACT subset as a complete
    // current-Level roster while conditional Character owners remain absent.
    using SourceKey = std::pair<std::uint32_t, std::uint32_t>;
    std::set<SourceKey> source_characters;
    std::set<SourceKey> reconciled_characters;
    std::size_t runtime_template_candidates = 0;
    std::size_t unresolved_factory_candidates = 0;
    std::size_t conditional_candidates = 0;
    const auto record_source = [&](const SourceKey& key) {
      require(reconciled_characters.insert(key).second,
              "Crypt Character source row was reconciled more than once");
    };
    for (std::uint32_t index = 0; index < source.entity_count; ++index) {
      const auto& object = source.entities[index];
      if (object.kind != world::RecordKind::mgp || !object.gametype ||
          std::strcmp(object.gametype, "Character") != 0) continue;
      const SourceKey key{object.module_index, object.source_record};
      require(source_characters.insert(key).second,
              "Crypt source Character row identity is duplicated");
      if (std::any_of(generated.source_order.begin(), generated.source_order.end(),
          [&](const auto& row) { return SourceKey{row.module_index, row.source_record} == key; })) {
        record_source(key);
        continue;
      }
      if (std::any_of(generated.faeries.begin(), generated.faeries.end(),
          [&](const auto& row) { return SourceKey{row.module_index, row.source_record} == key; })) {
        record_source(key);
        continue;
      }
      const auto skipped = std::find_if(generated.skipped.begin(), generated.skipped.end(),
          [&](const auto& row) { return SourceKey{row.module_index, row.source_record} == key; });
      require(skipped != generated.skipped.end(),
              "Crypt Character source row has no projection or deferred reason");
      if (skipped->reason == random_level::CryptGeneratedDactSkipReasonV1::known_factory_template) {
        if (dh2_world_field(&object, "char_template") &&
            dh2_world_field(&object, "char_template_pydata"))
          ++runtime_template_candidates;
        else
          ++unresolved_factory_candidates;
      }
      else if (skipped->reason == random_level::CryptGeneratedDactSkipReasonV1::conditional_object)
        ++conditional_candidates;
      else
        throw std::runtime_error("Crypt Character source row has an unreviewed deferred category");
      record_source(key);
    }
    require(source_characters.size() == 32 &&
                reconciled_characters.size() == source_characters.size() &&
                runtime_template_candidates == 15 &&
                unresolved_factory_candidates == 1 && conditional_candidates == 2,
            "Crypt source Character reconciliation no longer exposes the incomplete conditional-owner gap");
    require(generated.bytes.size() == 16 + 13 * 256 &&
                word(generated.bytes.data() + 4) == 2 &&
                word(generated.bytes.data() + 8) == 13,
            "Generated DACT v2 header/size rejected");

    std::vector<random_level::CryptGeneratedDactRetainedSourceV1> retained;
    for (std::uint32_t i = 0; i < source.entity_count; ++i) {
      const auto& object = source.entities[i];
      if (object.kind != world::RecordKind::mgp || !object.gametype ||
          std::strcmp(object.gametype, "Character") != 0) continue;
      if (object.module_index == 3 && object.source_record == 3) continue;
      retained.push_back({object.module_index, object.source_record, object.name});
    }
    random_level::CryptGeneratedDactV1 retained_projection;
    status = random_level::crypt_compile_generated_dact_v1(
        source, characters, models, retained_projection, &diagnostic, &retained);
    require(status == random_level::CryptGeneratedDactStatusV1::ok &&
                retained_projection.source_order.size() == 12 &&
                word(retained_projection.bytes.data() + 8) == 12 &&
                has_skip(retained_projection, 3, 3,
                    random_level::CryptGeneratedDactSkipReasonV1::object_manager_duplicate),
            "ObjectManager-discarded Character remained in generated DACT");
    require(std::none_of(retained_projection.source_order.begin(),
                retained_projection.source_order.end(), [](const auto& row) {
                  return row.module_index == 3 && row.source_record == 3;
                }),
            "Discarded Monster provenance remained in DACT order");

    constexpr std::array<std::pair<unsigned, unsigned>, 13> expected_source = {{
        {0, 4}, {0, 5}, {0, 9}, {0, 10}, {0, 11}, {0, 12},
        {3, 3}, {3, 4}, {3, 6},
        {7, 12}, {7, 13}, {7, 20}, {7, 21},
    }};
    for (std::size_t i = 0; i < expected_source.size(); ++i) {
      const auto& row = generated.source_order[i];
      require(row.module_index == expected_source[i].first &&
                  row.source_record == expected_source[i].second &&
                  row.dact_record == i,
              "Generated DACT rows lost source module/file order");
      const auto* original = object_at(source, row.module_index, row.source_record);
      require(original && row.name == original->name,
              "Generated DACT row lost its source name identity");
    }
    require(has_skip(generated, 7, 18,
                     random_level::CryptGeneratedDactSkipReasonV1::conditional_object) &&
                has_skip(generated, 7, 19,
                     random_level::CryptGeneratedDactSkipReasonV1::conditional_object),
            "Condition-gated Crypt boss actors were not deferred");
    const auto* big_skeleton = object_at(source, 7, 18);
    const auto* big_skeleton_second = object_at(source, 7, 19);
    const auto* big_skeleton_condition = big_skeleton
        ? dh2_world_field(big_skeleton, "activate_cond") : nullptr;
    const auto* big_skeleton_second_condition = big_skeleton_second
        ? dh2_world_field(big_skeleton_second, "activate_cond") : nullptr;
    require(big_skeleton && big_skeleton->name &&
                std::strcmp(big_skeleton->name, "_prim_Monster_BigSkel") == 0 &&
                big_skeleton_condition &&
                std::strcmp(big_skeleton_condition, "IsBefore_Gothicus2Survivors") == 0 &&
                big_skeleton_second && big_skeleton_second->name &&
                std::strcmp(big_skeleton_second->name, "_prim_Monster_BigSkel01") == 0 &&
                big_skeleton_second_condition &&
                std::strcmp(big_skeleton_second_condition,
                    "IsBefore_Gothicus2Survivors") == 0,
            "Deferred Crypt roster rows no longer identify the two conditional Big Skeletons");
    const auto* wandering_priest = object_at(source, 0, 2);
    const auto* wandering_priest_condition = wandering_priest
        ? dh2_world_field(wandering_priest, "activate_cond") : nullptr;
    require(wandering_priest && wandering_priest->name &&
                std::strcmp(wandering_priest->name, "_prim_NPC_WanderingPriest") == 0 &&
                wandering_priest_condition &&
                std::strcmp(wandering_priest_condition, "RENE_FOLLOW") == 0 &&
                dh2_world_field(wandering_priest, "charpropsname") &&
                !dh2_world_field(wandering_priest, "char_template"),
            "Conditional Crypt NPC no longer identifies its missing native factory binding");

    const auto expected = read(worlds + "/crypt01.dact");
    const auto expected_actors = actor_rows(expected);
    require(expected_actors.size() == generated.source_order.size(),
            "Current fixed Crypt DACT actor subset count differs");
    for (std::size_t i = 0; i < expected_actors.size(); ++i) {
      require(std::memcmp(generated.bytes.data() + 16 + i * 256,
                          expected_actors[i].data(), 256) == 0,
              "Generated source-MGP actor row differs from fixed Crypt DACT");
    }

    test_fail_closed(source, characters, models);
    dh2_world_free(&source);
    std::cout << "generated Crypt roster reconciliation: 32 source Characters = 13 DACT + 15 runtime templates + 1 Faery + 1 conditional NPC (RENE_FOLLOW, no char_template) + 2 conditional Big Skeletons (IsBefore_Gothicus2Survivors); all rows classified; proof blocked on NPC/condition owners; DACT byte-exact; guards pass\n";
    return 0;
  } catch (const std::exception& exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }
}
