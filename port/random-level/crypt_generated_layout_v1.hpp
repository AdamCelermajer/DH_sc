#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::random_level {

struct CryptRuleDocumentV1;

// One already-placed tile, in native recursive preorder. Paths are full
// cache-relative paths (dae, mgp, and mvp respectively). The catalogue root
// is the raw xrefobject value; DWLD serialization appends "-node".
struct CryptGeneratedModuleV1 {
  std::string_view block_name;
  std::string_view catalogue_xrefobject;
  std::string_view bres_path;
  std::string_view gameplay_path;
  std::string_view mvp_path;
  std::string_view xrefmax;
  std::string_view fog_color;
  std::string_view is_solid;
  std::int32_t grid_x = 0;
  std::int32_t grid_y = 0;
  float elevation = 0.0f;
  float unit_width = 0.0f;
  float unit_height = 0.0f;
  std::uint32_t block_width = 0;
  std::uint32_t block_height = 0;
  float scale[3] = {1.0f, 1.0f, 1.0f};
};

struct CryptGeneratedXmlAttributeV1 {
  std::string_view name;
  std::string_view value;
};

struct CryptGeneratedSpawnV1 {
  float position[3] = {0.0f, 0.0f, 0.0f};
};

struct CryptGeneratedLayoutOptionsV1 {
  // When supplied, declared LevelConfig overrides from the <rules> root are
  // applied after the source defaults, as RandomGenerator::LoadRuleFile does.
  const CryptRuleDocumentV1* rule_document = nullptr;
  // Direct caller properties apply last. The serializer supplies source
  // LevelConfig defaults; name is a convenient fallback override.
  std::string_view level_config_name = "level_config";
  const CryptGeneratedXmlAttributeV1* level_config_properties = nullptr;
  std::size_t level_config_property_count = 0;
  // Null means the caller has not selected a SpawnPoint. DWLD v1 has no
  // entrypoint identity, so zeroed coordinates must not imply a selection.
  const CryptGeneratedSpawnV1* spawn = nullptr;
};

struct CryptGeneratedLayoutV1 {
  std::string source_level_xml;
  std::vector<std::uint8_t> dwld_v1;
  bool spawn_selected = false;
};

enum class CryptGeneratedLayoutStatusV1 : std::uint32_t {
  ok,
  argument,
  limit,
  invalid_text,
  invalid_path,
  non_finite,
  unsupported_transform,
  allocation
};

struct CryptGeneratedLayoutDiagnosticV1 {
  CryptGeneratedLayoutStatusV1 status = CryptGeneratedLayoutStatusV1::ok;
  std::uint32_t module_index = 0xffffffffU;
  char message[160] = {};
};

// Serialize one generated Crypt placement list into two parallel outputs:
// (1) the source-shaped flat Level stream (LevelConfig GameObject first, then
// Module GameObjects in the supplied preorder), and (2) the current DWLD v1
// runtime module table from those same records. IDA confirms the native writer
// uses PropertyMap::SavePropertiesToXML's default "GameObject" element for
// both kinds; "Module" is the gametype value, not a nested XML element name.
// Module rotation is identity; non-identity scale is rejected because DWLD v1
// cannot encode it.
//
// The XML preserves the generator-owned MVX fields listed above when supplied.
// Its LevelConfig contains only source property-map fields, using declared
// defaults plus rule-root overrides. This function serializes data only: it
// does not choose a SpawnPoint, resolve
// assets, execute generator recursion, or activate a level.
//
// On failure, output is unchanged. On success, diagnostic is cleared. The
// returned spawn_selected bit records whether the DWLD header was explicitly
// populated; a structurally valid descriptor without it is not gameplay-ready.
CryptGeneratedLayoutStatusV1 crypt_serialize_generated_layout_v1(
    const CryptGeneratedModuleV1* modules, std::size_t module_count,
    const CryptGeneratedLayoutOptionsV1& options,
    CryptGeneratedLayoutV1& output,
    CryptGeneratedLayoutDiagnosticV1* diagnostic = nullptr) noexcept;

}  // namespace dh2::random_level
