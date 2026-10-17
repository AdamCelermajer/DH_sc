#include "crypt_generated_layout_v1.hpp"

#include "native_rule_plan_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <limits>
#include <locale>
#include <new>
#include <sstream>
#include <string>
#include <utility>

namespace dh2::random_level {
namespace {

constexpr std::size_t kMaxModules = 256;
constexpr std::size_t kMaxXmlBytes = 8U * 1024U * 1024U;
constexpr std::size_t kMaxAttributeBytes = 4096;
constexpr std::size_t kMaxCachePathBytes = 1024;
constexpr std::size_t kMaxNativeModulePathBytes = 95;
constexpr std::size_t kNativeModuleNameBytes = 88;
constexpr std::size_t kDwldHeaderBytes = 24;
constexpr std::size_t kDwldRecordBytes = 128;
constexpr float kMaxWorldCoordinate = 10000000.0f;

using Status = CryptGeneratedLayoutStatusV1;

Status reject(CryptGeneratedLayoutDiagnosticV1* diagnostic, Status status,
              const char* message, std::uint32_t module_index = 0xffffffffU) {
  if (diagnostic) {
    *diagnostic = {};
    diagnostic->status = status;
    diagnostic->module_index = module_index;
    std::snprintf(diagnostic->message, sizeof(diagnostic->message), "%s",
                  message ? message : "Generated layout rejected");
  }
  return status;
}

Status success(CryptGeneratedLayoutDiagnosticV1* diagnostic) {
  if (diagnostic) *diagnostic = {};
  return Status::ok;
}

bool valid_xml_text(std::string_view value) {
  std::size_t at = 0;
  while (at < value.size()) {
    const auto first = static_cast<unsigned char>(value[at++]);
    std::uint32_t codepoint = first;
    unsigned continuation = 0;
    if (first < 0x80) {
      // XML 1.0 permits tab, line feed and carriage return among controls.
      if (!((first >= 0x20) || first == 0x09 || first == 0x0a || first == 0x0d))
        return false;
    } else if (first >= 0xc2 && first <= 0xdf) {
      codepoint = first & 0x1f;
      continuation = 1;
    } else if (first >= 0xe0 && first <= 0xef) {
      codepoint = first & 0x0f;
      continuation = 2;
    } else if (first >= 0xf0 && first <= 0xf4) {
      codepoint = first & 0x07;
      continuation = 3;
    } else {
      return false;
    }
    if (continuation) {
      if (continuation > value.size() - at) return false;
      for (unsigned i = 0; i < continuation; ++i) {
        const auto byte = static_cast<unsigned char>(value[at++]);
        if ((byte & 0xc0) != 0x80) return false;
        codepoint = (codepoint << 6) | (byte & 0x3f);
      }
      if ((continuation == 1 && codepoint < 0x80) ||
          (continuation == 2 && codepoint < 0x800) ||
          (continuation == 3 && codepoint < 0x10000) ||
          codepoint > 0x10ffff ||
          (codepoint >= 0xd800 && codepoint <= 0xdfff))
        return false;
    }
    if (!((codepoint >= 0x20 && codepoint <= 0xd7ff) ||
          (codepoint >= 0xe000 && codepoint <= 0xfffd) ||
          (codepoint >= 0x10000 && codepoint <= 0x10ffff) ||
          codepoint == 0x09 || codepoint == 0x0a || codepoint == 0x0d))
      return false;
  }
  return true;
}

bool valid_attribute_name(std::string_view name) {
  if (name.empty()) return false;
  const auto first = static_cast<unsigned char>(name.front());
  if (!((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z') ||
        first == '_'))
    return false;
  for (std::size_t i = 1; i < name.size(); ++i) {
    const auto c = static_cast<unsigned char>(name[i]);
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') || c == '_' || c == '-' || c == '.'))
      return false;
  }
  return true;
}

bool valid_cache_path(std::string_view value) {
  if (value.empty() || value.size() > kMaxCachePathBytes ||
      value.size() < 6 || value.substr(0, 5) != "data/")
    return false;
  std::size_t segment_start = 0;
  for (std::size_t i = 0; i <= value.size(); ++i) {
    if (i != value.size() && value[i] != '/') {
      const auto c = static_cast<unsigned char>(value[i]);
      if (c < 0x20 || c >= 0x7f || c == ':' || c == '?' || c == '#') return false;
      continue;
    }
    const auto length = i - segment_start;
    if (!length || (length == 1 && value[segment_start] == '.') ||
        (length == 2 && value[segment_start] == '.' &&
         value[segment_start + 1] == '.'))
      return false;
    segment_start = i + 1;
  }
  return true;
}

bool append_bounded(std::string& output, std::string_view text) {
  if (text.size() > kMaxXmlBytes - output.size()) return false;
  if (text.empty()) return true;
  output.append(text.data(), text.size());
  return true;
}

bool append_escaped(std::string& output, std::string_view text) {
  std::size_t begin = 0;
  for (std::size_t i = 0; i < text.size(); ++i) {
    std::string_view replacement;
    switch (text[i]) {
      case '&': replacement = "&amp;"; break;
      case '<': replacement = "&lt;"; break;
      case '>': replacement = "&gt;"; break;
      case '"': replacement = "&quot;"; break;
      case '\'': replacement = "&apos;"; break;
      // XML parsers normalize literal whitespace in attributes. Character
      // references preserve the original PropertyMap string value instead.
      case '\t': replacement = "&#x9;"; break;
      case '\n': replacement = "&#xA;"; break;
      case '\r': replacement = "&#xD;"; break;
      default: continue;
    }
    if (!append_bounded(output, text.substr(begin, i - begin)) ||
        !append_bounded(output, replacement))
      return false;
    begin = i + 1;
  }
  return append_bounded(output, text.substr(begin));
}

bool append_attribute(std::string& output, std::string_view name,
                      std::string_view value) {
  return append_bounded(output, " ") && append_bounded(output, name) &&
         append_bounded(output, "=\"") && append_escaped(output, value) &&
         append_bounded(output, "\"");
}

bool format_float(float value, std::string& output) {
  if (!std::isfinite(value)) return false;
  std::ostringstream stream;
  stream.imbue(std::locale::classic());
  stream << std::fixed << std::setprecision(6) << value;
  output = stream.str();
  return !output.empty();
}

bool format_vector(float x, float y, float z, std::string& output) {
  std::string a, b, c;
  if (!format_float(x, a) || !format_float(y, b) || !format_float(z, c))
    return false;
  output.clear();
  output.reserve(a.size() + b.size() + c.size() + 2);
  output.append(a).push_back(',');
  output.append(b).push_back(',');
  output.append(c);
  return true;
}

void write_u32_le(std::uint8_t* output, std::uint32_t value) {
  output[0] = static_cast<std::uint8_t>(value);
  output[1] = static_cast<std::uint8_t>(value >> 8);
  output[2] = static_cast<std::uint8_t>(value >> 16);
  output[3] = static_cast<std::uint8_t>(value >> 24);
}

bool write_f32_le(std::uint8_t* output, float value) {
  if (!std::isfinite(value)) return false;
  std::uint32_t bits = 0;
  static_assert(sizeof(bits) == sizeof(value), "DWLD requires 32-bit floats");
  std::memcpy(&bits, &value, sizeof(bits));
  write_u32_le(output, bits);
  return true;
}

struct ConfigPropertyDefault {
  std::string_view name;
  std::string_view value;
  bool emit_default = true;
};

// Values recovered from LevelConfig::DeclareProperties (0x3f4b9c) and its
// ObjectBase property declarations (0x33f014). Name/gametype are emitted with
// the required identity fields below. `static` uses the object's existing
// member value, so it is only emitted when an explicit source override exists.
constexpr ConfigPropertyDefault kConfigDefaults[] = {
    {"activate_cond", ""},
    {"ambiant_music", ""},
    {"ambientColor", "255.000000,255.000000,255.000000"},
    {"camera_animset", "Default"},
    {"camera_file", ""},
    {"camera_name", ""},
    {"camera_zfar", "10000"},
    {"camera_znear", "600"},
    {"clear_color", "0.000000,0.000000,0.000000"},
    {"combat_music", ""},
    {"combat_music_enabled", "true"},
    {"deactivate_cond", ""},
    {"dfog_blend_dist", "100.000000"},
    {"dfog_change_min_dist", "200.000000"},
    {"dfog_colors", ""},
    {"fix_index_buffer_size", "655360"},
    {"fix_quantize", "false"},
    {"fix_vertex_buffer_size", "5242880"},
    {"fixed_light_set", ""},
    {"fog_color", "255.000000,255.000000,255.000000"},
    {"fog_direction_mask", "0.000000,0.000000,1.000000"},
    {"fog_end", "0"},
    {"fog_start", "1"},
    {"gametype", "LevelConfig"},
    {"isGlobal", "false"},
    {"isUnderlay", "false"},
    {"is_demo_only", "false"},
    {"is_tutorial_only", "false"},
    {"light_set", ""},
    {"min_difficulty", ""},
    {"mp_script", ""},
    {"music", ""},
    {"name", "level_config"},
    {"prog_index_buffer_size", "655360"},
    {"prog_quantize", "false"},
    {"prog_vertex_buffer_size", "5242880"},
    {"rim_light_file", "char_rim_light.tga"},
    {"roomname", ""},
    {"safezone_music", ""},
    {"scriptFile", ""},
    {"shadow_color", "0.000000,0.000000,0.000000"},
    {"shadow_opacity", "0.000000"},
    {"skybox", ""},
    {"static", "false"},
    {"sun_position", "-1000000.000000,-1000000.000000,-1000000.000000"},
    {"visible", "true"},
};

std::string_view find_override(const CryptGeneratedLayoutOptionsV1& options,
                               std::string_view name,
                               bool& found) {
  if (options.level_config_properties) {
    for (std::size_t i = 0; i < options.level_config_property_count; ++i) {
      if (options.level_config_properties[i].name == name) {
        found = true;
        return options.level_config_properties[i].value;
      }
    }
  }
  if (options.rule_document) {
    for (const auto& property : options.rule_document->root_property_overrides) {
      if (property.name == name) {
        found = true;
        return property.value;
      }
    }
  }
  found = false;
  return {};
}

std::string_view config_identity(const CryptGeneratedLayoutOptionsV1& options,
                                 std::string_view name,
                                 std::string_view fallback) {
  bool found = false;
  const auto value = find_override(options, name, found);
  return found ? value : fallback;
}

bool in_runtime_range(float value) {
  return std::isfinite(value) && std::abs(value) <= kMaxWorldCoordinate;
}

Status validate_module(const CryptGeneratedModuleV1& module,
                       std::uint32_t index,
                       CryptGeneratedLayoutDiagnosticV1* diagnostic) {
  const auto fail = [&](Status status, const char* message) {
    return reject(diagnostic, status, message, index);
  };
  if (module.block_name.empty() || module.catalogue_xrefobject.empty() ||
      module.gameplay_path.empty() || module.mvp_path.empty() ||
      module.bres_path.empty())
    return fail(Status::argument, "Module identity or asset path is empty");
  if (!valid_xml_text(module.block_name) ||
      !valid_xml_text(module.catalogue_xrefobject) ||
      !valid_xml_text(module.bres_path) ||
      !valid_xml_text(module.gameplay_path) || !valid_xml_text(module.mvp_path) ||
      !valid_xml_text(module.xrefmax) || !valid_xml_text(module.fog_color) ||
      !valid_xml_text(module.is_solid))
    return fail(Status::invalid_text, "Module contains invalid XML UTF-8/text");
  const auto generated_name_size = module.block_name.size() + 1 +
      (index < 10 ? 1 : index < 100 ? 2 : 3);
  if (generated_name_size >= kNativeModuleNameBytes)
    return fail(Status::limit, "Native module instance name would truncate");
  if (module.catalogue_xrefobject.size() + 5 > 111)
    return fail(Status::limit, "DWLD catalogue node ID exceeds its 112-byte field");
  if (module.bres_path.size() > kMaxCachePathBytes ||
      module.gameplay_path.size() > kMaxNativeModulePathBytes ||
      module.mvp_path.size() > kMaxNativeModulePathBytes ||
      module.xrefmax.size() > kMaxAttributeBytes ||
      module.fog_color.size() > kMaxAttributeBytes ||
      module.is_solid.size() > kMaxAttributeBytes)
    return fail(Status::limit, "Native module property would truncate");
  if (!valid_cache_path(module.bres_path) ||
      !valid_cache_path(module.gameplay_path) ||
      !valid_cache_path(module.mvp_path) ||
      (!module.xrefmax.empty() && !valid_cache_path(module.xrefmax)))
    return fail(Status::invalid_path, "Module asset path is not a safe data/ cache path");
  if (!module.block_width || !module.block_height)
    return fail(Status::argument, "Module footprint dimensions must be positive");
  if (module.block_width > static_cast<std::uint32_t>(
                               std::numeric_limits<std::int32_t>::max()) ||
      module.block_height > static_cast<std::uint32_t>(
                                std::numeric_limits<std::int32_t>::max()))
    return fail(Status::limit, "Module footprint exceeds native signed dimensions");
  if (!std::isfinite(module.unit_width) || !std::isfinite(module.unit_height) ||
      module.unit_width <= 0.0f || module.unit_height <= 0.0f ||
      !std::isfinite(module.elevation))
    return fail(Status::non_finite, "Grid units/elevation are invalid");
  for (float scale : module.scale) {
    if (!std::isfinite(scale))
      return fail(Status::non_finite, "Module scale is non-finite");
    if (scale != 1.0f)
      return fail(Status::unsupported_transform,
                  "DWLD v1 cannot represent a non-identity module scale");
  }
  const float half_width = static_cast<float>(module.block_width - 1U) * 0.5f;
  const float half_height = static_cast<float>(module.block_height - 1U) * 0.5f;
  const float x = (static_cast<float>(module.grid_x) + half_width) * module.unit_width;
  const float y = (static_cast<float>(module.grid_y) + half_height) * -module.unit_height;
  if (!in_runtime_range(x) || !in_runtime_range(y) ||
      !in_runtime_range(module.elevation))
    return fail(Status::non_finite, "Generated module position exceeds runtime bounds");
  return Status::ok;
}

bool append_config_properties(std::string& xml,
                              const CryptGeneratedLayoutOptionsV1& options) {
  if (options.level_config_property_count > 122 ||
      (options.level_config_property_count && !options.level_config_properties))
    return false;
  for (std::size_t i = 0; i < options.level_config_property_count; ++i) {
    const auto& property = options.level_config_properties[i];
    if (!valid_attribute_name(property.name) ||
        property.value.size() > kMaxAttributeBytes || !valid_xml_text(property.value))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (options.level_config_properties[j].name == property.name) return false;
  }

  for (const auto& property : kConfigDefaults) {
    bool found = false;
    auto value = find_override(options, property.name, found);
    if (!found && property.name == "name") {
      value = options.level_config_name;
      found = true;
    }
    if (!found && !property.emit_default) continue;
    if (!append_attribute(xml, property.name, found ? value : property.value))
      return false;
  }
  return true;
}

}  // namespace

CryptGeneratedLayoutStatusV1 crypt_serialize_generated_layout_v1(
    const CryptGeneratedModuleV1* modules, std::size_t module_count,
    const CryptGeneratedLayoutOptionsV1& options,
    CryptGeneratedLayoutV1& output,
    CryptGeneratedLayoutDiagnosticV1* diagnostic) noexcept {
  if (diagnostic) *diagnostic = {};
  if (!modules || !module_count || module_count > kMaxModules)
    return reject(diagnostic, module_count > kMaxModules ? Status::limit : Status::argument,
                  "Generated module list is empty or exceeds the source/runtime limit");
  if (options.level_config_property_count > 122 ||
      (options.level_config_property_count && !options.level_config_properties))
    return reject(diagnostic, Status::argument,
                  "LevelConfig identity or property list is invalid");
  const auto config_name = config_identity(options, "name", options.level_config_name);
  const auto config_gametype = config_identity(options, "gametype", "LevelConfig");
  if (config_name.empty() || config_name.size() > kMaxAttributeBytes ||
      config_gametype.empty() || config_gametype.size() > kMaxAttributeBytes ||
      !valid_xml_text(config_name) || !valid_xml_text(config_gametype))
    return reject(diagnostic, Status::argument,
                  "LevelConfig name/gametype override is invalid");
  if (options.spawn) {
    for (float coordinate : options.spawn->position)
      if (!in_runtime_range(coordinate))
        return reject(diagnostic, Status::non_finite,
                      "Explicit SpawnPoint position exceeds runtime bounds");
  }

  for (std::size_t i = 0; i < module_count; ++i) {
    const auto status = validate_module(modules[i], static_cast<std::uint32_t>(i),
                                        diagnostic);
    if (status != Status::ok) return status;
  }

  try {
    CryptGeneratedLayoutV1 candidate;
    candidate.dwld_v1.assign(kDwldHeaderBytes + module_count * kDwldRecordBytes, 0);
    auto& descriptor = candidate.dwld_v1;
    std::memcpy(descriptor.data(), "DWLD", 4);
    write_u32_le(descriptor.data() + 4, 1);
    write_u32_le(descriptor.data() + 8, static_cast<std::uint32_t>(module_count));
    if (options.spawn) {
      for (unsigned axis = 0; axis < 3; ++axis)
        if (!write_f32_le(descriptor.data() + 12 + axis * 4,
                          options.spawn->position[axis]))
          return reject(diagnostic, Status::non_finite,
                        "Explicit SpawnPoint position is non-finite");
      candidate.spawn_selected = true;
    }

    std::string& xml = candidate.source_level_xml;
    if (!append_bounded(xml, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Level>\n"))
      return reject(diagnostic, Status::limit, "Generated level XML exceeds 8 MiB");
    if (!append_bounded(xml, "  <GameObject") ||
        !append_config_properties(xml, options) ||
        !append_bounded(xml, "/>\n"))
      return reject(diagnostic, Status::invalid_text,
                    "LevelConfig XML properties are invalid or exceed limits");

    std::string generated_name;
    std::string position;
    for (std::size_t i = 0; i < module_count; ++i) {
      const auto& module = modules[i];
      generated_name.assign(module.block_name.data(), module.block_name.size());
      generated_name.push_back('_');
      generated_name.append(std::to_string(i));
      const float x = (static_cast<float>(module.grid_x) +
          static_cast<float>(module.block_width - 1U) * 0.5f) * module.unit_width;
      const float y = (static_cast<float>(module.grid_y) +
          static_cast<float>(module.block_height - 1U) * 0.5f) * -module.unit_height;
      if (!format_vector(x, y, module.elevation, position))
        return reject(diagnostic, Status::non_finite,
                      "Module position could not be formatted",
                      static_cast<std::uint32_t>(i));

      if (!append_bounded(xml, "  <GameObject") ||
          !append_attribute(xml, "type", "obj") ||
          !append_attribute(xml, "name", generated_name) ||
          !append_attribute(xml, "position", position) ||
          !append_attribute(xml, "scale", "1.000000,1.000000,1.000000") ||
          !append_attribute(xml, "rotation", "0.000000,0.000000,0.000000") ||
          !append_attribute(xml, "gametype", "Module") ||
          (!module.xrefmax.empty() && !append_attribute(xml, "xrefmax", module.xrefmax)) ||
          !append_attribute(xml, "xrefobject", module.catalogue_xrefobject) ||
          (!module.fog_color.empty() && !append_attribute(xml, "fog_color", module.fog_color)) ||
          (!module.is_solid.empty() && !append_attribute(xml, "is_solid", module.is_solid)) ||
          !append_attribute(xml, "dae", module.bres_path) ||
          !append_attribute(xml, "mgp", module.gameplay_path) ||
          !append_attribute(xml, "mvp", module.mvp_path) ||
          !append_bounded(xml, "/>\n"))
        return reject(diagnostic, Status::limit,
                      "Module XML attributes exceed the source limit",
                      static_cast<std::uint32_t>(i));

      auto* record = descriptor.data() + kDwldHeaderBytes + i * kDwldRecordBytes;
      std::memcpy(record, module.catalogue_xrefobject.data(),
                  module.catalogue_xrefobject.size());
      std::memcpy(record + module.catalogue_xrefobject.size(), "-node", 5);
      // The zero-initialized vector supplies the string terminator and padding.
      if (!write_f32_le(record + 112, x) || !write_f32_le(record + 116, y) ||
          !write_f32_le(record + 120, module.elevation))
        return reject(diagnostic, Status::non_finite,
                      "DWLD module position is non-finite",
                      static_cast<std::uint32_t>(i));
      write_u32_le(record + 124, 0);
    }
    if (!append_bounded(xml, "</Level>\n"))
      return reject(diagnostic, Status::limit, "Generated level XML exceeds 8 MiB");
    output = std::move(candidate);
  } catch (const std::bad_alloc&) {
    return reject(diagnostic, Status::allocation, "Generated layout allocation failed");
  } catch (...) {
    return reject(diagnostic, Status::allocation, "Generated layout construction failed");
  }
  return success(diagnostic);
}

}  // namespace dh2::random_level
