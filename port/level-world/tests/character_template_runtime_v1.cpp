#include "../character_template_runtime_v1.hpp"
#include "../source_random_lifecycle_v1.hpp"

#include "../../game-data/data.hpp"
#include "../character_template_catalog_v1.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using Bytes = std::vector<std::uint8_t>;

void require(bool condition, const char* message) {
  if (!condition) throw std::runtime_error(message);
}

Bytes read(const std::filesystem::path& path) {
  std::ifstream input(path, std::ios::binary);
  if (!input) throw std::runtime_error("Unable to open: " + path.string());
  return {std::istreambuf_iterator<char>(input), {}};
}

void run(const std::filesystem::path& assets) {
  const auto load = [&](const char* path) { return read(assets / "data" / path); };
  const auto rows = load("character_properties_pyarray.bin");
  const auto names = load("character_properties_pyarraynames.bin");
  const auto fields = load("character_properties_pystructnames.bin");
  const auto class_names = load("character_classes_pyarraynames.bin");
  const auto template_rows = load("character_templates_pyarray.bin");
  const auto template_names = load("character_templates_pyarraynames.bin");
  const auto model_names = load("character_models_dictionary_pyarraynames.bin");
  const auto model_values = load("character_models_dictionary_pyarray.bin");

  dh2::data::CharacterTable characters;
  dh2::data::Dictionary models;
  dh2::character::template_factory::Catalog catalog;
  std::string error;
  require(dh2::data::load_characters({rows.data(), rows.size()},
      {names.data(), names.size()}, {fields.data(), fields.size()}, characters,
      error), error.c_str());
  require(dh2::data::load_dictionary({model_names.data(), model_names.size()},
      {model_values.data(), model_values.size()}, models, error), error.c_str());
  require(dh2::character::template_catalog_v1::load(template_rows.data(),
      template_rows.size(), template_names.data(), template_names.size(),
      class_names.data(), class_names.size(), characters, catalog, error),
      error.c_str());

  const auto template_id = std::find_if(catalog.templates.begin(),
      catalog.templates.end(), [](const auto& entry) {
        return entry.name == "GothicusCrypt_Ghosts";
      });
  require(template_id != catalog.templates.end(), "Crypt ghost template is absent");
  require(template_id->property_ids == std::vector<std::int32_t>({35, 35, 35, 35, 37}),
      "Crypt ghost template authored slots changed");
  require(std::count(template_id->property_ids.begin(), template_id->property_ids.end(), 35) == 4 &&
      std::count(template_id->property_ids.begin(), template_id->property_ids.end(), 37) == 1,
      "duplicate Crypt slots no longer preserve the source 4:1 selection weight");
  const auto template_index = static_cast<std::int16_t>(
      template_id - catalog.templates.begin());

  std::int16_t property_cache = -1;
  std::int16_t template_cache = -1;
  auto& actual = dh2::random_lifecycle::process_state();
  dh2::random_lifecycle::seed_from_gsinit_update(0x12345678u);
  auto expected = actual;
  expected.seeds[0] = (expected.seeds[0] * 59051u + 177149u) % 0xdaf26bu;
  ++expected.counters[0];
  const auto expected_slot = static_cast<std::int32_t>(expected.seeds[0] % 5u);
  dh2::character_template_runtime_v1::Bindings bindings{
      &property_cache, &template_cache, "Charater_Templates",
      "GothicusCrypt_Ghosts", &catalog, &characters, &models, &actual};
  dh2::character_template_runtime_v1::Result result;
  auto status = dh2::character_template_runtime_v1::resolve(bindings, &result, error);
  require(status == dh2::character_template_runtime_v1::Status::selected,
      "uncached template did not resolve");
  require(result.selected_slot == expected_slot && result.random_draws == 1,
      "selection did not use exactly one ordinary process-RNG draw");
  require(result.property_id == template_id->property_ids[static_cast<std::size_t>(expected_slot)] &&
      property_cache == result.property_id && template_cache == template_index,
      "selected source property or retained cache IDs differ");
  require(actual.seeds[0] == expected.seeds[0] &&
      actual.counters[0] == expected.counters[0] &&
      actual.seeds[1] == expected.seeds[1] &&
      actual.counters[1] == expected.counters[1],
      "template selection did not mutate exactly one ordinary process-RNG counter");

  actual.seeds[0] = 0xabcdef01u;
  actual.counters[0] = 99u;
  status = dh2::character_template_runtime_v1::resolve(bindings, &result, error);
  require(status == dh2::character_template_runtime_v1::Status::cached &&
      result.property_id == property_cache && result.random_draws == 0 &&
      actual.seeds[0] == 0xabcdef01u && actual.counters[0] == 99u,
      "retained selection drew again on the cached path");

  const auto mixed_template = std::find_if(catalog.templates.begin(),
      catalog.templates.end(), [](const auto& entry) {
        return entry.name == "GothicusCrypt_CommonType1";
      });
  require(mixed_template != catalog.templates.end() &&
      mixed_template->property_ids == std::vector<std::int32_t>(
          {41, 41, 41, 41, 42, 42, 35, 35, 37, 33, 34}),
      "Crypt common template authored alternatives changed");
  const auto mixed_template_index = static_cast<std::int16_t>(
      mixed_template - catalog.templates.begin());
  const auto model = std::find(characters.fields.begin(), characters.fields.end(), "ModelFile");
  require(model != characters.fields.end(), "ModelFile field is absent");
  const auto model_index = static_cast<std::size_t>(model - characters.fields.begin());
  const auto animation = std::find(characters.fields.begin(),
      characters.fields.end(), "AnimTable");
  require(animation != characters.fields.end(), "AnimTable field is absent");
  const auto animation_index = static_cast<std::size_t>(animation - characters.fields.begin());
  require(characters.rows[41][model_index] != characters.rows[34][model_index] &&
      characters.rows[41][animation_index] != characters.rows[34][animation_index],
      "Crypt common template no longer spans distinct model and animation resources");

  // This source-authored Crypt template mixes dogs, ghosts, and slimes. Slot 10
  // deterministically selects the slime so its own model/AnimTable pair must
  // survive selection and flow to the caller's per-Character loader.
  std::int16_t mixed_property = -1;
  std::int16_t mixed_template_cache = -1;
  dh2_random_state mixed_random{{9u, 456u}, {3u, 4u}};
  bindings.property_cache = &mixed_property;
  bindings.template_cache = &mixed_template_cache;
  bindings.template_name = "GothicusCrypt_CommonType1";
  bindings.random = &mixed_random;
  const auto selected_model = characters.rows[34][model_index];
  const auto selected_animation = characters.rows[34][animation_index];
  auto mixed_characters = characters;
  mixed_characters.rows[41][model_index] = -1;  // valid nonselected Dog alternative is not preloaded
  bindings.characters = &mixed_characters;
  status = dh2::character_template_runtime_v1::resolve(bindings, &result, error);
  require(status == dh2::character_template_runtime_v1::Status::selected &&
      result.selected_slot == 10 && result.property_id == 34 &&
      result.random_draws == 1 && mixed_property == 34 &&
      mixed_template_cache == mixed_template_index &&
      mixed_characters.rows[static_cast<std::size_t>(result.property_id)][model_index] == selected_model &&
      mixed_characters.rows[static_cast<std::size_t>(result.property_id)][animation_index] == selected_animation &&
      selected_model >= 0 && static_cast<std::size_t>(selected_model) < models.values.size() &&
      !models.values[static_cast<std::size_t>(selected_model)].empty() && selected_animation == 64,
      "source-selected Crypt alternative did not retain its own valid model/animation resources");
  require(mixed_random.seeds[0] == 708608u && mixed_random.counters[0] == 4u &&
      mixed_random.seeds[1] == 456u && mixed_random.counters[1] == 4u,
      "mixed Crypt template did not consume exactly one ordinary RNG draw");

  // A selected row with a missing model still fails. Its source property cache
  // and already-consumed random draw remain retained, matching SafeGetCharPropsId.
  auto missing_model = models;
  missing_model.values[static_cast<std::size_t>(selected_model)].clear();
  std::int16_t retained_bad_property = -1;
  std::int16_t retained_bad_template = -1;
  dh2_random_state bad_random{{9u, 456u}, {3u, 4u}};
  bindings.property_cache = &retained_bad_property;
  bindings.template_cache = &retained_bad_template;
  bindings.models = &missing_model;
  bindings.random = &bad_random;
  status = dh2::character_template_runtime_v1::resolve(bindings, &result, error);
  require(status == dh2::character_template_runtime_v1::Status::incompatible_alternatives &&
      error.find("Selected template Character") != std::string::npos &&
      retained_bad_property == 34 && retained_bad_template == mixed_template_index &&
      result.property_id == 34 && result.selected_slot == 10 && result.random_draws == 1 &&
      bad_random.seeds[0] == 708608u && bad_random.counters[0] == 4u &&
      bad_random.seeds[1] == 456u && bad_random.counters[1] == 4u,
      "missing selected model did not fail after retaining the source selection/cache");
}
}  // namespace

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: character_template_runtime_v1 assets-root\n";
    return 2;
  }
  try {
    run(argv[1]);
    std::cout << "Crypt template selection preserves weighted draws and selects per-Character model/animation resources; missing selected resources fail\n";
    return 0;
  } catch (const std::exception& exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }
}
