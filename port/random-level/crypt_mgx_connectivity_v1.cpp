#include "crypt_mgx_connectivity_v1.hpp"
#include "crypt_mgx_placement_v1.hpp"

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <map>
#include <utility>

namespace dh2::random_level {
namespace {

constexpr std::size_t kMaxXmlBytes = 1024 * 1024;
constexpr std::size_t kMaxAttributesPerTag = 64;
constexpr std::size_t kMaxLinkExitsPerModule = 64;

struct XmlTag {
  std::string name;
  std::map<std::string, std::string> attributes;
  bool closing = false;
  bool self_closing = false;
};

bool is_space(char value) {
  return value == ' ' || value == '\t' || value == '\r' || value == '\n';
}

bool is_name_start(char value) {
  return (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z') ||
         value == '_' || value == ':';
}

bool is_name_char(char value) {
  return is_name_start(value) || (value >= '0' && value <= '9') || value == '-' ||
         value == '.';
}

char ascii_lower(char value) {
  return value >= 'A' && value <= 'Z' ? static_cast<char>(value + ('a' - 'A')) : value;
}

bool equals_ascii_case_insensitive(std::string_view first, std::string_view second) {
  if (first.size() != second.size()) return false;
  for (std::size_t i = 0; i < first.size(); ++i) {
    if (ascii_lower(first[i]) != ascii_lower(second[i])) return false;
  }
  return true;
}

bool compute_source_cell_position(const CryptMgxModuleV1& module,
                                  CryptMgxExitV1& exit) noexcept {
  if (!module.has_grid_geometry || !exit.has_raw_position) return false;
  const CryptMgxGridGeometryV1 geometry{
      module.source_unit_width, module.source_unit_height,
      module.block_width, module.block_height};
  CryptMgxExitCellV1 cell;
  if (!crypt_mgx_exit_position_to_cell_v1(
          geometry, exit.position_x, exit.position_y, exit.position_z, cell)) {
    return false;
  }
  exit.cell_x = cell.x;
  exit.cell_y = cell.y;
  exit.has_cell_position = true;
  return true;
}

bool parse_source_position(std::string_view source,
                           CryptMgxExitV1& exit) {
  float* const components[] = {&exit.position_x, &exit.position_y, &exit.position_z};
  std::size_t cursor = 0;
  for (float* component : components) {
    // The original StrToObj uses strtok with ',' and strtod. strtok collapses
    // adjacent delimiters, and missing tail components retain their zeros.
    while (cursor < source.size() && source[cursor] == ',') ++cursor;
    if (cursor == source.size()) break;
    const auto end = source.find(',', cursor);
    const auto length = (end == std::string_view::npos ? source.size() : end) - cursor;
    const std::string token(source.substr(cursor, length));
    const double parsed = std::strtod(token.c_str(), nullptr);
    const float value = static_cast<float>(parsed);
    if (!std::isfinite(value)) return false;
    *component = value;
    cursor = end == std::string_view::npos ? source.size() : end;
  }
  exit.has_raw_position = true;
  return true;
}

class MgxXmlParser {
 public:
  explicit MgxXmlParser(std::string_view source) : source_(source) {}

  bool parse(CryptMgxModuleV1& module) {
    if (source_.size() > kMaxXmlBytes) return fail("MGX exceeds parser size bound");
    if (!skip_misc()) return false;

    XmlTag root;
    if (!read_tag(root)) return false;
    if (root.closing || root.self_closing || root.name != "Module") {
      return fail("MGX root must be a non-empty <Module>");
    }
    if (!read_positive_double(root, "unit_width", module.unit_width) ||
        !read_positive_double(root, "unit_height", module.unit_height)) {
      return false;
    }
    module.source_unit_width = static_cast<float>(module.unit_width);
    module.source_unit_height = static_cast<float>(module.unit_height);
    if (!std::isfinite(module.source_unit_width) ||
        !std::isfinite(module.source_unit_height) ||
        module.source_unit_width <= 0.0f || module.source_unit_height <= 0.0f) {
      return fail("MGX unit dimensions exceed source float range");
    }
    if (!read_grid_geometry(root, module)) return false;

    bool saw_closing_root = false;
    while (!saw_closing_root) {
      if (!skip_misc()) return false;
      if (position_ == source_.size()) return fail("unterminated <Module>");

      XmlTag tag;
      if (!read_tag(tag)) return false;
      if (tag.closing) {
        if (tag.name != "Module") return fail("unexpected MGX closing tag");
        saw_closing_root = true;
        continue;
      }
      if (tag.name != "GameObject") return fail("unsupported MGX child element");
      if (!tag.self_closing) return fail("MGX GameObject must be self-closing in this parser slice");

      const auto* game_type = attribute(tag, "gametype");
      if (!game_type) return fail("MGX GameObject is missing gametype");
      if (!equals_ascii_case_insensitive(*game_type, "link")) continue;

      const auto* object_name = attribute(tag, "name");
      const auto* link_type = attribute(tag, "linktype");
      const auto* direction = attribute(tag, "direction");
      if (!object_name || object_name->empty() || !link_type || link_type->empty() || !direction) {
        return fail("MGX link GameObject requires name, linktype, and direction");
      }
      DirectionV1 parsed_direction;
      if (!parse_direction(*direction, parsed_direction)) {
        return fail("unsupported MGX direction token");
      }
      if (module.exits.size() >= kMaxLinkExitsPerModule) {
        return fail("MGX link-exit count exceeds parser bound");
      }
      CryptMgxExitV1 parsed_exit{*object_name, *link_type, parsed_direction};
      if (const auto* position = attribute(tag, "position")) {
        if (!parse_source_position(*position, parsed_exit)) {
          return fail("invalid source-float MGX link position");
        }
        if (module.has_grid_geometry &&
            !compute_source_cell_position(module, parsed_exit)) {
          return fail("MGX link position cannot be mapped to source grid cells");
        }
      }
      module.exits.push_back(std::move(parsed_exit));
    }

    if (!skip_misc()) return false;
    if (position_ != source_.size()) return fail("trailing MGX content");
    return true;
  }

  const std::string& error() const { return error_; }

 private:
  bool fail(std::string message) {
    if (error_.empty()) error_ = std::move(message) + " at byte " + std::to_string(position_);
    return false;
  }

  void skip_space() {
    while (position_ < source_.size() && is_space(source_[position_])) ++position_;
  }

  bool skip_misc() {
    while (true) {
      skip_space();
      if (source_.substr(position_, 4) == "<!--") {
        const auto end = source_.find("-->", position_ + 4);
        if (end == std::string_view::npos) return fail("unterminated MGX comment");
        position_ = end + 3;
        continue;
      }
      if (source_.substr(position_, 2) == "<?") {
        const auto end = source_.find("?>", position_ + 2);
        if (end == std::string_view::npos) return fail("unterminated MGX processing instruction");
        position_ = end + 2;
        continue;
      }
      return true;
    }
  }

  bool parse_name(std::string& output) {
    if (position_ >= source_.size() || !is_name_start(source_[position_])) {
      return fail("expected MGX XML name");
    }
    const auto begin = position_++;
    while (position_ < source_.size() && is_name_char(source_[position_])) ++position_;
    output.assign(source_.substr(begin, position_ - begin));
    return true;
  }

  bool parse_attribute_value(std::string& output) {
    if (position_ >= source_.size() ||
        (source_[position_] != '\'' && source_[position_] != '"')) {
      return fail("expected quoted MGX attribute value");
    }
    const char quote = source_[position_++];
    const auto begin = position_;
    while (position_ < source_.size() && source_[position_] != quote) {
      if (source_[position_] == '&') return fail("MGX entities are outside parser scope");
      ++position_;
    }
    if (position_ >= source_.size()) return fail("unterminated MGX attribute value");
    output.assign(source_.substr(begin, position_ - begin));
    ++position_;
    return true;
  }

  bool read_tag(XmlTag& output) {
    if (position_ >= source_.size() || source_[position_] != '<') {
      return fail("expected MGX tag");
    }
    ++position_;
    if (position_ < source_.size() && source_[position_] == '/') {
      output.closing = true;
      ++position_;
      if (!parse_name(output.name)) return false;
      skip_space();
      if (position_ >= source_.size() || source_[position_] != '>') {
        return fail("malformed MGX closing tag");
      }
      ++position_;
      return true;
    }
    if (position_ >= source_.size() || source_[position_] == '!') {
      return fail("unsupported MGX declaration");
    }
    if (!parse_name(output.name)) return false;

    while (true) {
      skip_space();
      if (source_.substr(position_, 2) == "/>" ) {
        output.self_closing = true;
        position_ += 2;
        return true;
      }
      if (position_ < source_.size() && source_[position_] == '>') {
        ++position_;
        return true;
      }
      if (output.attributes.size() >= kMaxAttributesPerTag) {
        return fail("MGX attribute count exceeds parser bound");
      }
      std::string key;
      if (!parse_name(key)) return false;
      skip_space();
      if (position_ >= source_.size() || source_[position_] != '=') {
        return fail("expected '=' in MGX attribute");
      }
      ++position_;
      skip_space();
      std::string value;
      if (!parse_attribute_value(value)) return false;
      if (!output.attributes.emplace(std::move(key), std::move(value)).second) {
        return fail("duplicate MGX attribute");
      }
    }
  }

  static const std::string* attribute(const XmlTag& tag, const char* key) {
    const auto found = tag.attributes.find(key);
    return found == tag.attributes.end() ? nullptr : &found->second;
  }

  bool read_positive_double(const XmlTag& tag, const char* key, double& output) {
    const auto* source = attribute(tag, key);
    if (!source || source->empty()) return fail(std::string("MGX Module missing ") + key);
    errno = 0;
    char* end = nullptr;
    const double value = std::strtod(source->c_str(), &end);
    if (errno != 0 || end == source->c_str() || *end != '\0' || !std::isfinite(value) ||
        value <= 0.0) {
      return fail(std::string("invalid positive MGX Module ") + key);
    }
    output = value;
    return true;
  }

  bool read_grid_geometry(const XmlTag& tag, CryptMgxModuleV1& module) {
    const auto* block_width = attribute(tag, "block_width");
    const auto* block_height = attribute(tag, "block_height");
    if (!block_width && !block_height) return true;
    if (!block_width || !block_height) {
      return fail("MGX grid geometry requires both block_width and block_height");
    }

    // IDA: MgxBlock::LoadFromXml calls TiXmlElement::QueryIntAttribute
    // (0x5157f0), whose QueryIntValue uses sscanf("%d") (0x515798). That
    // accepts the cached export spelling "1.0" as integer 1.
    int parsed_width = 0;
    int parsed_height = 0;
    if (std::sscanf(block_width->c_str(), "%d", &parsed_width) != 1 ||
        std::sscanf(block_height->c_str(), "%d", &parsed_height) != 1) {
      return fail("invalid MGX integer block dimensions");
    }
    module.block_width = static_cast<std::int32_t>(parsed_width);
    module.block_height = static_cast<std::int32_t>(parsed_height);
    module.has_grid_geometry = true;
    return true;
  }

  static bool parse_direction(std::string_view source, DirectionV1& output) {
    if (source == "north") {
      output = DirectionV1::north;
    } else if (source == "east") {
      output = DirectionV1::east;
    } else if (source == "south") {
      output = DirectionV1::south;
    } else if (source == "west") {
      output = DirectionV1::west;
    } else {
      return false;
    }
    return true;
  }

  std::string_view source_;
  std::size_t position_ = 0;
  std::string error_;
};

bool opposite(DirectionV1 first, DirectionV1 second) noexcept {
  switch (first) {
    case DirectionV1::north: return second == DirectionV1::south;
    case DirectionV1::east: return second == DirectionV1::west;
    case DirectionV1::south: return second == DirectionV1::north;
    case DirectionV1::west: return second == DirectionV1::east;
  }
  return false;
}

bool derive_pair_origin(const CryptMgxExitV1& anchor,
                        const CryptMgxExitV1& candidate,
                        std::int32_t& origin_x,
                        std::int32_t& origin_y,
                        float& origin_z) noexcept {
  if (!crypt_exits_compatible_v1(anchor, candidate) ||
      !anchor.has_cell_position || !candidate.has_cell_position) {
    return false;
  }
  const CryptMgxExitCellV1 anchor_cell{
      anchor.cell_x, anchor.cell_y, anchor.position_z};
  const CryptMgxExitCellV1 candidate_cell{
      candidate.cell_x, candidate.cell_y, candidate.position_z};
  const CryptMgxTileOriginV1 anchor_tile{};
  CryptMgxTilePlacementV1 placement;
  if (!crypt_mgx_try_spawn_placement_v1(anchor_tile, &anchor_cell,
                                        candidate_cell, candidate.direction,
                                        placement)) {
    return false;
  }
  origin_x = placement.x;
  origin_y = placement.y;
  origin_z = placement.elevation;
  return true;
}

}  // namespace

CryptMgxParseResultV1 parse_crypt_mgx_v1(std::string_view module_name,
                                         std::string_view xml) {
  if (module_name.empty()) return {false, {}, "MGX module name is empty"};
  CryptMgxModuleV1 module;
  module.name.assign(module_name);
  MgxXmlParser parser(xml);
  if (!parser.parse(module)) return {false, {}, parser.error()};
  return {true, std::move(module), {}};
}

bool crypt_exits_compatible_v1(const CryptMgxExitV1& first,
                               const CryptMgxExitV1& second) noexcept {
  return first.link_type == second.link_type && opposite(first.direction, second.direction);
}

std::vector<CryptExitAdjacencyV1> enumerate_crypt_exit_adjacencies_v1(
    const std::vector<CryptMgxModuleV1>& modules) {
  std::vector<CryptExitAdjacencyV1> result;
  for (std::size_t first_module = 0; first_module < modules.size(); ++first_module) {
    for (std::size_t second_module = first_module; second_module < modules.size(); ++second_module) {
      const auto& first = modules[first_module];
      const auto& second = modules[second_module];
      for (std::size_t first_exit = 0; first_exit < first.exits.size(); ++first_exit) {
        for (std::size_t second_exit = 0; second_exit < second.exits.size(); ++second_exit) {
          if (crypt_exits_compatible_v1(first.exits[first_exit], second.exits[second_exit])) {
            result.push_back({first_module, first_exit, second_module, second_exit});
          }
        }
      }
    }
  }
  return result;
}

std::vector<CryptExitPlacementCandidateV1> enumerate_crypt_exit_placement_candidates_v1(
    const std::vector<CryptMgxModuleV1>& modules) {
  std::vector<CryptExitPlacementCandidateV1> result;
  const auto topology = enumerate_crypt_exit_adjacencies_v1(modules);
  result.reserve(topology.size());
  for (const auto& edge : topology) {
    const auto& anchor = modules[edge.first_module].exits[edge.first_exit];
    const auto& candidate = modules[edge.second_module].exits[edge.second_exit];
    CryptExitPlacementCandidateV1 placement;
    if (!derive_pair_origin(anchor, candidate, placement.second_origin_x,
                            placement.second_origin_y, placement.second_origin_z)) {
      continue;
    }
    placement.first_module = edge.first_module;
    placement.first_exit = edge.first_exit;
    placement.second_module = edge.second_module;
    placement.second_exit = edge.second_exit;
    result.push_back(placement);
  }
  return result;
}

const char* direction_name_v1(DirectionV1 direction) noexcept {
  switch (direction) {
    case DirectionV1::north: return "north";
    case DirectionV1::east: return "east";
    case DirectionV1::south: return "south";
    case DirectionV1::west: return "west";
  }
  return "unknown";
}

}  // namespace dh2::random_level
