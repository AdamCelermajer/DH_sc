#include "../crypt_generated_layout_v1.hpp"
#include "../native_rule_plan_v1.hpp"
#include "../../world-data/world.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace dh2::random_level;

namespace {

unsigned checks = 0;
unsigned failures = 0;

void check(bool condition, const char* label) {
  ++checks;
  if (!condition) {
    ++failures;
    std::cerr << "FAIL: " << label << '\n';
  }
}

std::uint32_t read_u32_le(const std::uint8_t* p) {
  return static_cast<std::uint32_t>(p[0]) |
      (static_cast<std::uint32_t>(p[1]) << 8) |
      (static_cast<std::uint32_t>(p[2]) << 16) |
      (static_cast<std::uint32_t>(p[3]) << 24);
}

float read_f32_le(const std::uint8_t* p) {
  const auto bits = read_u32_le(p);
  float result = 0.0f;
  std::memcpy(&result, &bits, sizeof(result));
  return result;
}

CryptGeneratedModuleV1 module(std::string_view name,
                              std::string_view root,
                              std::int32_t x, std::int32_t y,
                              float elevation,
                              std::uint32_t width = 1,
                              std::uint32_t height = 1) {
  CryptGeneratedModuleV1 result;
  result.block_name = name;
  result.catalogue_xrefobject = root;
  result.bres_path = "data/3d/modules/crypt/crypt.bdae";
  result.gameplay_path = "data/3d/modules/crypt/mgp/crypt_entrance_01.mgp";
  result.mvp_path = "data/3d/modules/crypt/mvp/crypt_entrance_01.mvp";
  result.xrefmax = "data/3d/modules/crypt/crypt.max";
  result.fog_color = "1.0,0.36,0.49";
  result.is_solid = "1";
  result.grid_x = x;
  result.grid_y = y;
  result.elevation = elevation;
  result.unit_width = 4800.0f;
  result.unit_height = 4800.0f;
  result.block_width = width;
  result.block_height = height;
  return result;
}

void test_source_xml_and_dwld_order() {
  CryptGeneratedModuleV1 modules[] = {
      module("crypt&entrance", "_module_crypt_entrance", 0, 0, 2.0f),
      module("crypt_hall", "_module_crypt_hall", 2, 3, 37.25f, 2, 3),
  };
  const CryptGeneratedXmlAttributeV1 config_properties[] = {
      {"music", "CryptOneAmbientMusic"},
      {"scriptFile", "data/pydata/scripts/007_crypt_01.pyscript"},
  };
  CryptGeneratedLayoutOptionsV1 options;
  options.level_config_name = "_prim_LevelConfig";
  options.level_config_properties = config_properties;
  options.level_config_property_count = 2;
  CryptGeneratedLayoutV1 output;
  CryptGeneratedLayoutDiagnosticV1 diagnostic{};

  const auto status = crypt_serialize_generated_layout_v1(
      modules, 2, options, output, &diagnostic);
  check(status == CryptGeneratedLayoutStatusV1::ok &&
            diagnostic.status == CryptGeneratedLayoutStatusV1::ok,
        "two placed Crypt modules serialize");
  check(output.source_level_xml.find("<Level>") != std::string::npos &&
            output.source_level_xml.find("name=\"_prim_LevelConfig\"") != std::string::npos &&
            output.source_level_xml.find("gametype=\"LevelConfig\"") != std::string::npos,
        "XML starts with a source-loader LevelConfig GameObject");
  const auto config_record_end = output.source_level_xml.find("/>");
  const std::string_view config_record(output.source_level_xml.data(),
      config_record_end == std::string::npos ? 0 : config_record_end);
  check(config_record_end != std::string::npos &&
            config_record.find(" type=\"") == std::string_view::npos &&
            config_record.find(" position=\"") == std::string_view::npos &&
            config_record.find(" rotation=\"") == std::string_view::npos &&
            config_record.find(" scale=\"") == std::string_view::npos,
        "LevelConfig output contains only source PropertyMap fields");
  check(output.source_level_xml.find("name=\"crypt&amp;entrance_0\"") != std::string::npos &&
            output.source_level_xml.find("gametype=\"Module\"") != std::string::npos &&
            output.source_level_xml.find("name=\"crypt_hall_1\"") != std::string::npos,
        "native preorder suffixes and XML escaping are preserved");
  check(output.source_level_xml.find(
            "position=\"0.000000,-0.000000,2.000000\"") != std::string::npos &&
            output.source_level_xml.find(
            "position=\"12000.000000,-19200.000000,37.250000\"") != std::string::npos,
        "native grid-centre position math is formatted to six decimals");
  check(output.source_level_xml.find("mgp=\"data/3d/modules/crypt/mgp/") != std::string::npos &&
            output.source_level_xml.find("mvp=\"data/3d/modules/crypt/mvp/") != std::string::npos &&
            output.source_level_xml.find("xrefobject=\"_module_crypt_hall\"") != std::string::npos,
        "native module asset and catalogue properties are emitted");

  const auto& dwld = output.dwld_v1;
  check(dwld.size() == 24 + 2 * 128 && std::memcmp(dwld.data(), "DWLD", 4) == 0 &&
            read_u32_le(dwld.data() + 4) == 1 && read_u32_le(dwld.data() + 8) == 2,
        "DWLD v1 header and exact record length match the module list");
  check(read_u32_le(dwld.data() + 12) == 0 &&
            read_u32_le(dwld.data() + 16) == 0 &&
            read_u32_le(dwld.data() + 20) == 0 && !output.spawn_selected,
        "unselected spawn remains explicitly unset in the descriptor");
  const auto* first = dwld.data() + 24;
  const auto* second = first + 128;
  check(std::strcmp(reinterpret_cast<const char*>(first),
                    "_module_crypt_entrance-node") == 0 &&
            std::signbit(read_f32_le(first + 116)) &&
            read_f32_le(first + 112) == 0.0f &&
            read_f32_le(first + 120) == 2.0f,
        "first DWLD row keeps preorder root and source negative-zero coordinate");
  check(std::strcmp(reinterpret_cast<const char*>(second),
                    "_module_crypt_hall-node") == 0 &&
            read_f32_le(second + 112) == 12000.0f &&
            read_f32_le(second + 116) == -19200.0f &&
            read_f32_le(second + 120) == 37.25f &&
            read_u32_le(second + 124) == 0,
        "second DWLD row uses the same placement order and zero reserved field");

  dh2::world::SourceLevel imported{};
  dh2::world::Diagnostic source_diagnostic{};
  const auto import_status = dh2_world_import_level(
      &imported, "generated_crypt", "data/scene/generated_crypt.mlx",
      reinterpret_cast<const std::uint8_t*>(output.source_level_xml.data()),
      output.source_level_xml.size(), &source_diagnostic);
  check(import_status == dh2::world::Error::ok && imported.module_count == 2 &&
            imported.modules[0].record.module_index == 0 &&
            imported.modules[1].record.module_index == 1 &&
            imported.modules[1].record.local.position[0] == 12000.0f &&
            imported.modules[1].record.local.position[1] == -19200.0f,
        "project MLX importer accepts the generated flat stream in source order");
  dh2_world_free(&imported);

  const CryptGeneratedSpawnV1 spawn{{123.5f, -456.0f, 7.25f}};
  options.spawn = &spawn;
  CryptGeneratedLayoutV1 with_spawn;
  check(crypt_serialize_generated_layout_v1(modules, 2, options, with_spawn,
                                            &diagnostic) ==
            CryptGeneratedLayoutStatusV1::ok && with_spawn.spawn_selected &&
            read_f32_le(with_spawn.dwld_v1.data() + 12) == 123.5f &&
            read_f32_le(with_spawn.dwld_v1.data() + 16) == -456.0f &&
            read_f32_le(with_spawn.dwld_v1.data() + 20) == 7.25f,
        "caller-selected spawn is written only when provided explicitly");
}

void test_level_config_defaults_and_rule_overrides() {
  auto placed = module("crypt_entrance", "_module_crypt_entrance", 0, 0, 0.0f);
  CryptRuleDocumentV1 document;
  document.root_property_overrides = {
      {"ambientColor", "0.5,0.5,0.5"},
      {"camera_znear", "900"},
      {"camera_zfar", "5000"},
      {"music", "CryptOneAmbientMusic"},
      {"fixed_light_set", "data/3D/Light/crypt_es1_1.lightset_xml"},
      {"undeclaredLevelConfigProperty", "must-not-be-emitted"},
      {"name", "crypt_rules_config"},
  };
  CryptGeneratedLayoutOptionsV1 options;
  options.rule_document = &document;
  CryptGeneratedLayoutV1 output;
  CryptGeneratedLayoutDiagnosticV1 diagnostic{};

  check(crypt_serialize_generated_layout_v1(&placed, 1, options, output,
                                            &diagnostic) ==
            CryptGeneratedLayoutStatusV1::ok,
        "rule-root LevelConfig properties serialize");
  check(output.source_level_xml.find("name=\"crypt_rules_config\"") !=
            std::string::npos &&
            output.source_level_xml.find("camera_znear=\"900\"") !=
                std::string::npos &&
            output.source_level_xml.find("camera_zfar=\"5000\"") !=
                std::string::npos &&
            output.source_level_xml.find("ambientColor=\"0.5,0.5,0.5\"") !=
                std::string::npos &&
            output.source_level_xml.find("music=\"CryptOneAmbientMusic\"") !=
                std::string::npos &&
            output.source_level_xml.find(
                "fixed_light_set=\"data/3D/Light/crypt_es1_1.lightset_xml\"") !=
                std::string::npos,
        "Crypt rules override native LevelConfig defaults");
  check(output.source_level_xml.find("camera_animset=\"Default\"") !=
            std::string::npos &&
            output.source_level_xml.find("fog_start=\"1\"") !=
                std::string::npos &&
            output.source_level_xml.find("static=\"false\"") !=
                std::string::npos &&
            output.source_level_xml.find("rim_light_file=\"char_rim_light.tga\"") !=
                std::string::npos,
        "unmodified LevelConfig/ObjectBase fields use IDA defaults");
  check(output.source_level_xml.find("undeclaredLevelConfigProperty") ==
            std::string::npos,
        "undeclared root XML fields are ignored like native PropertyMap");
}

void test_rejections_are_transactional() {
  auto valid = module("crypt_room", "_module_crypt_room", 1, 2, 3.0f);
  CryptGeneratedLayoutOptionsV1 options;
  CryptGeneratedLayoutV1 output;
  output.source_level_xml = "previous XML";
  output.dwld_v1 = {1, 2, 3};
  output.spawn_selected = true;
  CryptGeneratedLayoutDiagnosticV1 diagnostic{};

  auto invalid_scale = valid;
  invalid_scale.scale[1] = 2.0f;
  check(crypt_serialize_generated_layout_v1(&invalid_scale, 1, options, output,
                                            &diagnostic) ==
            CryptGeneratedLayoutStatusV1::unsupported_transform &&
            diagnostic.module_index == 0 && output.source_level_xml == "previous XML" &&
            output.dwld_v1 == std::vector<std::uint8_t>({1, 2, 3}) &&
            output.spawn_selected,
        "DWLD-incompatible scale rejects without replacing prior output");

  auto invalid_dimension = valid;
  invalid_dimension.block_height = 0;
  check(crypt_serialize_generated_layout_v1(&invalid_dimension, 1, options, output,
                                            &diagnostic) ==
            CryptGeneratedLayoutStatusV1::argument,
        "zero footprint is rejected before position computation");

  auto non_finite = valid;
  non_finite.elevation = std::numeric_limits<float>::infinity();
  check(crypt_serialize_generated_layout_v1(&non_finite, 1, options, output,
                                            &diagnostic) ==
            CryptGeneratedLayoutStatusV1::non_finite,
        "non-finite elevation is rejected");

  auto too_long_path = valid;
  const std::string overlong_path(96, 'x');
  too_long_path.gameplay_path = overlong_path;
  check(crypt_serialize_generated_layout_v1(&too_long_path, 1, options, output,
                                            &diagnostic) ==
            CryptGeneratedLayoutStatusV1::limit,
        "native fixed gameplay-path buffer truncation is rejected");

  auto too_long_name = valid;
  const std::string overlong_name(86, 'x');
  too_long_name.block_name = overlong_name;
  check(crypt_serialize_generated_layout_v1(&too_long_name, 1, options, output,
                                            &diagnostic) ==
            CryptGeneratedLayoutStatusV1::limit,
        "native fixed module-name buffer truncation is rejected");

  auto invalid_text = valid;
  invalid_text.block_name = std::string_view("room\x01", 5);
  check(crypt_serialize_generated_layout_v1(&invalid_text, 1, options, output,
                                            &diagnostic) ==
            CryptGeneratedLayoutStatusV1::invalid_text,
        "XML control characters are rejected");

  std::vector<CryptGeneratedModuleV1> excess(257, valid);
  check(crypt_serialize_generated_layout_v1(excess.data(), excess.size(), options,
                                            output, &diagnostic) ==
            CryptGeneratedLayoutStatusV1::limit,
        "module count above the source loader limit is rejected");
}

}  // namespace

int main() {
  test_source_xml_and_dwld_order();
  test_level_config_defaults_and_rule_overrides();
  test_rejections_are_transactional();
  std::cout << checks - failures << '/' << checks
            << " generated layout checks passed\n";
  return failures == 0 ? 0 : 1;
}
