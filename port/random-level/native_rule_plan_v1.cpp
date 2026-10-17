#include "native_rule_plan_v1.hpp"

#include <algorithm>
#include <cctype>
#include <limits>
#include <map>
#include <utility>

namespace dh2::random_level {
namespace {

constexpr std::uint64_t kModulus = 0xFFFFFFFBULL;
constexpr std::uint64_t kMultiplier = 279470273ULL;

struct XmlNode {
  std::string name;
  std::map<std::string, std::string> attributes;
  std::vector<XmlNode> children;
};

class CryptXmlParser {
 public:
  explicit CryptXmlParser(std::string_view source) : source_(source) {}

  bool parse(XmlNode& root) {
    if (!skip_misc()) return false;
    if (!parse_node(root)) return false;
    if (!skip_misc()) return false;
    skip_space();
    if (position_ != source_.size()) return fail("trailing XML content");
    return true;
  }

  const std::string& error() const { return error_; }

 private:
  bool fail(std::string message) {
    if (error_.empty()) {
      error_ = std::move(message) + " at byte " + std::to_string(position_);
    }
    return false;
  }

  static bool is_space(char value) {
    return std::isspace(static_cast<unsigned char>(value)) != 0;
  }

  static bool is_name_start(char value) {
    const auto c = static_cast<unsigned char>(value);
    return std::isalpha(c) != 0 || value == '_' || value == ':';
  }

  static bool is_name_char(char value) {
    const auto c = static_cast<unsigned char>(value);
    return std::isalnum(c) != 0 || value == '_' || value == ':' || value == '-' || value == '.';
  }

  void skip_space() {
    while (position_ < source_.size() && is_space(source_[position_])) ++position_;
  }

  bool skip_misc() {
    while (true) {
      skip_space();
      if (source_.substr(position_, 4) == "<!--") {
        const auto end = source_.find("-->", position_ + 4);
        if (end == std::string_view::npos) return fail("unterminated XML comment");
        position_ = end + 3;
        continue;
      }
      if (source_.substr(position_, 2) == "<?") {
        const auto end = source_.find("?>", position_ + 2);
        if (end == std::string_view::npos) return fail("unterminated processing instruction");
        position_ = end + 2;
        continue;
      }
      return true;
    }
  }

  bool parse_name(std::string& output) {
    if (position_ >= source_.size() || !is_name_start(source_[position_])) {
      return fail("expected XML name");
    }
    const auto begin = position_++;
    while (position_ < source_.size() && is_name_char(source_[position_])) ++position_;
    output.assign(source_.substr(begin, position_ - begin));
    return true;
  }

  bool parse_attribute_value(std::string& output) {
    if (position_ >= source_.size()) return fail("expected attribute value");
    if (source_[position_] == '\'' || source_[position_] == '"') {
      const char quote = source_[position_++];
      const auto begin = position_;
      while (position_ < source_.size() && source_[position_] != quote) {
        if (source_[position_] == '&') return fail("XML entities are outside this parser slice");
        ++position_;
      }
      if (position_ >= source_.size()) return fail("unterminated attribute value");
      output.assign(source_.substr(begin, position_ - begin));
      ++position_;
      return true;
    }

    // TinyXML in the original accepts unquoted values. Its attribute parser
    // stops at whitespace, '/', or '>'; mirror that narrow behavior because
    // the pinned Crypt file contains `dontgoback=false` in this form.
    const auto begin = position_;
    while (position_ < source_.size() && !is_space(source_[position_]) &&
           source_[position_] != '/' && source_[position_] != '>') {
      if (source_[position_] == '&') return fail("XML entities are outside this parser slice");
      ++position_;
    }
    if (position_ == begin) return fail("empty attribute value");
    output.assign(source_.substr(begin, position_ - begin));
    return true;
  }

  bool parse_node(XmlNode& node) {
    if (position_ >= source_.size() || source_[position_] != '<' ||
        source_.substr(position_, 2) == "</" || source_.substr(position_, 2) == "<!") {
      return fail("expected start element");
    }
    ++position_;
    if (!parse_name(node.name)) return false;

    while (true) {
      skip_space();
      if (source_.substr(position_, 2) == "/>" ) {
        position_ += 2;
        return true;
      }
      if (position_ < source_.size() && source_[position_] == '>') {
        ++position_;
        break;
      }
      std::string key;
      if (!parse_name(key)) return false;
      skip_space();
      if (position_ >= source_.size() || source_[position_] != '=') return fail("expected '='");
      ++position_;
      skip_space();
      std::string value;
      if (!parse_attribute_value(value)) return false;
      if (!node.attributes.emplace(std::move(key), std::move(value)).second) {
        return fail("duplicate XML attribute");
      }
    }

    while (true) {
      if (!skip_misc()) return false;
      if (position_ >= source_.size()) return fail("unterminated element " + node.name);
      if (source_.substr(position_, 2) == "</") {
        position_ += 2;
        std::string closing_name;
        if (!parse_name(closing_name)) return false;
        skip_space();
        if (position_ >= source_.size() || source_[position_] != '>') return fail("expected closing '>'");
        ++position_;
        if (closing_name != node.name) return fail("mismatched closing element");
        return true;
      }
      if (source_[position_] == '<') {
        XmlNode child;
        if (!parse_node(child)) return false;
        node.children.push_back(std::move(child));
        continue;
      }
      const auto text_begin = position_;
      const auto less = source_.find('<', position_);
      if (less == std::string_view::npos) return fail("unterminated element text");
      while (position_ < less) {
        if (!is_space(source_[position_])) return fail("non-whitespace XML text unsupported");
        ++position_;
      }
      if (position_ == text_begin) return fail("invalid XML text");
    }
  }

  std::string_view source_;
  std::size_t position_ = 0;
  std::string error_;
};

const std::string* attribute(const XmlNode& node, std::string_view key) {
  const auto it = node.attributes.find(std::string(key));
  return it == node.attributes.end() ? nullptr : &it->second;
}

std::string lower_ascii(std::string value) {
  for (char& c : value) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  return value;
}

bool parse_native_atoi(std::string_view value, std::int32_t& result) {
  std::size_t position = 0;
  while (position < value.size() && std::isspace(static_cast<unsigned char>(value[position]))) ++position;
  bool negative = false;
  if (position < value.size() && (value[position] == '+' || value[position] == '-')) {
    negative = value[position] == '-';
    ++position;
  }
  const auto digit_begin = position;
  std::int64_t number = 0;
  while (position < value.size() && value[position] >= '0' && value[position] <= '9') {
    number = number * 10 + (value[position] - '0');
    if (number > static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::max()) + 1) return false;
    ++position;
  }
  if (position == digit_begin) return false;
  if (negative) number = -number;
  if (number < std::numeric_limits<std::int32_t>::min() ||
      number > std::numeric_limits<std::int32_t>::max()) return false;
  result = static_cast<std::int32_t>(number);
  return true;
}

bool parse_path_length(std::string_view value, PathOptionsV1& path, std::string& error) {
  const auto comma = value.find(',');
  const auto min_text = value.substr(0, comma);
  std::int32_t min_value = 0;
  if (!parse_native_atoi(min_text, min_value)) {
    error = "Crypt Path length has unsupported minimum";
    return false;
  }
  path.min_length = min_value;
  if (comma != std::string_view::npos) {
    if (value.find(',', comma + 1) != std::string_view::npos) {
      error = "Crypt Path length has more than one range separator";
      return false;
    }
    std::int32_t max_value = 0;
    if (!parse_native_atoi(value.substr(comma + 1), max_value)) {
      error = "Crypt Path length has unsupported maximum";
      return false;
    }
    path.max_length = max_value;
  } else {
    path.max_length = min_value;
  }
  return true;
}

bool parse_selector(std::string_view source, RuleSelectorV1& output, std::string& error) {
  output.source_text.assign(source);
  if (!source.empty() && source.front() == '#') {
    output.is_list = true;
    const auto bracket = source.find('[');
    if (bracket == std::string_view::npos) {
      output.list_name = lower_ascii(std::string(source.substr(1)));
      if (output.list_name.empty()) {
        error = "empty Crypt list selector";
        return false;
      }
      return true;
    }
    if (bracket <= 1 || source.back() != ']' || source.find('[', bracket + 1) != std::string_view::npos) {
      error = "unsupported Crypt list selector syntax";
      return false;
    }
    std::int32_t index = 0;
    if (!parse_native_atoi(source.substr(bracket + 1, source.size() - bracket - 2), index)) {
      error = "Crypt list selector index is not numeric";
      return false;
    }
    output.list_name = lower_ascii(std::string(source.substr(1, bracket - 1)));
    output.list_index = index;
    return true;
  }

  std::size_t begin = 0;
  while (true) {
    const auto comma = source.find(',', begin);
    const auto end = comma == std::string_view::npos ? source.size() : comma;
    if (end == begin) {
      error = "empty block in Crypt root selector";
      return false;
    }
    output.block_names.emplace_back(source.substr(begin, end - begin));
    if (comma == std::string_view::npos) break;
    begin = comma + 1;
  }
  return true;
}

bool parse_rule_node(const XmlNode& xml, RuleNodeV1& output, std::string& error) {
  if (xml.name == "RootRule") {
    output.kind = RuleKindV1::root;
  } else if (xml.name == "ForceBlock") {
    output.kind = RuleKindV1::force_block;
  } else if (xml.name == "Path") {
    output.kind = RuleKindV1::path;
  } else {
    error = "unsupported Crypt rule element: " + xml.name;
    return false;
  }
  const auto* name = attribute(xml, "name");
  if (!name || !parse_selector(*name, output.selector, error)) {
    if (error.empty()) error = "Crypt rule is missing name";
    return false;
  }
  if (attribute(xml, "exit") || attribute(xml, "id")) {
    error = "Crypt rule exit/id metadata is outside this slice";
    return false;
  }
  if (output.kind == RuleKindV1::path) {
    if (const auto* length = attribute(xml, "length")) {
      if (!parse_path_length(*length, output.path, error)) return false;
    }
    // Path::LoadFromXml at 0x49130c queries exact-case `dontGoBack` as an
    // integer. Invalid numeric text is ignored, preserving constructor true.
    if (const auto* dont_go_back = attribute(xml, "dontGoBack")) {
      std::int32_t value = 0;
      if (parse_native_atoi(*dont_go_back, value)) {
        output.path.dont_go_back = value != 0;
        output.path.dont_go_back_attribute_applied = true;
      }
    }
  }
  for (const auto& child : xml.children) {
    RuleNodeV1 parsed_child;
    if (!parse_rule_node(child, parsed_child, error)) return false;
    output.children.push_back(std::move(parsed_child));
  }
  return true;
}

bool parse_list(const XmlNode& xml, CryptListV1& output, std::string& error) {
  const auto* name = attribute(xml, "name");
  if (!name || name->empty()) {
    error = "Crypt list is missing name";
    return false;
  }
  output.name = lower_ascii(*name);
  if (const auto* replacement = attribute(xml, "replacement")) {
    output.replacement = *replacement == "true";
  }
  for (const auto& child : xml.children) {
    if (child.name != "elem") {
      error = "unsupported child in Crypt list: " + child.name;
      return false;
    }
    const auto* elem_name = attribute(child, "name");
    const auto* gameplay = attribute(child, "gameplay");
    const auto* visual = attribute(child, "visual");
    if (!elem_name || !gameplay || !visual) {
      error = "Crypt list element is missing name/gameplay/visual";
      return false;
    }
    output.entries.push_back({*elem_name, *gameplay, *visual});
  }
  return true;
}

bool build_plan_node(const CryptRuleDocumentV1& document,
                     const RuleNodeV1& source,
                     RulePlanNodeV1& output,
                     std::string& error) {
  output.kind = source.kind;
  output.selector = source.selector;
  output.path = source.path;
  if (source.selector.is_list) {
    const auto* list = find_list_v1(document, source.selector.list_name);
    if (!list) {
      error = "Crypt rule references missing list: " + source.selector.list_name;
      return false;
    }
    if (source.selector.list_index) {
      const auto index = *source.selector.list_index;
      if (index < 0 || static_cast<std::size_t>(index) >= list->entries.size()) {
        error = "explicit non-root Crypt list index is unsupported or out of range";
        return false;
      }
      output.allowed_list_entries.push_back(list->entries[static_cast<std::size_t>(index)]);
    } else {
      output.allowed_list_entries = list->entries;
    }
  }
  for (const auto& child : source.children) {
    RulePlanNodeV1 planned_child;
    if (!build_plan_node(document, child, planned_child, error)) return false;
    output.children.push_back(std::move(planned_child));
  }
  return true;
}

}  // namespace

std::uint32_t RandomGeneratorV1::next_int() {
  // The ARM expression adds in uint32 before widening to uint64; preserve
  // wraparound for arbitrary seeds such as UINT32_MAX.
  const std::uint32_t incremented = state_ + std::uint32_t{1};
  state_ = static_cast<std::uint32_t>((kMultiplier * incremented) % kModulus);
  return state_;
}

std::optional<std::uint32_t> RandomGeneratorV1::bounded(std::uint32_t bound) {
  if (bound == 0) return std::nullopt;
  return next_int() % bound;
}

std::int32_t RandomGeneratorV1::get_int(std::int32_t lower, std::int32_t upper) {
  if (lower >= upper) return lower;
  const auto width = static_cast<std::uint32_t>(upper) - static_cast<std::uint32_t>(lower);
  const auto offset = next_int() % width;
  return static_cast<std::int32_t>(static_cast<std::uint32_t>(lower) + offset);
}

ParseResultV1 parse_crypt_rule_v1(std::string_view xml) {
  XmlNode root_xml;
  CryptXmlParser parser(xml);
  if (!parser.parse(root_xml)) return {std::nullopt, parser.error()};
  if (root_xml.name != "rules") return {std::nullopt, "Crypt root element must be <rules>"};

  CryptRuleDocumentV1 document;
  const auto* target = attribute(root_xml, "target");
  const auto* folder = attribute(root_xml, "folder");
  if (!target || !folder) return {std::nullopt, "Crypt rules require target and folder"};
  document.target = *target;
  document.folder = *folder;
  for (const auto& root_attribute : root_xml.attributes) {
    if (root_attribute.first == "target" || root_attribute.first == "folder") continue;
    document.root_property_overrides.push_back(
        {root_attribute.first, root_attribute.second});
  }

  bool found_root = false;
  for (const auto& child : root_xml.children) {
    if (child.name == "list") {
      CryptListV1 list;
      std::string error;
      if (!parse_list(child, list, error)) return {std::nullopt, std::move(error)};
      if (find_list_v1(document, list.name)) {
        return {std::nullopt, "duplicate Crypt list name"};
      }
      document.lists.push_back(std::move(list));
    } else if (child.name == "RootRule") {
      if (found_root) return {std::nullopt, "multiple Crypt RootRule elements"};
      std::string error;
      if (!parse_rule_node(child, document.root, error)) return {std::nullopt, std::move(error)};
      found_root = true;
    } else if (child.name == "pool") {
      return {std::nullopt, "Crypt room-pool rules are outside this deterministic slice"};
    } else {
      return {std::nullopt, "unsupported child under Crypt <rules>: " + child.name};
    }
  }
  if (!found_root) return {std::nullopt, "Crypt rules are missing RootRule"};
  return {std::move(document), {}};
}

const CryptListV1* find_list_v1(const CryptRuleDocumentV1& document,
                                std::string_view name) {
  const auto normalized = lower_ascii(std::string(name));
  for (const auto& list : document.lists) {
    if (list.name == normalized) return &list;
  }
  return nullptr;
}

PlanResultV1 build_rule_plan_v1(const CryptRuleDocumentV1& document,
                                std::uint32_t seed) {
  RulePlanV1 plan;
  RandomGeneratorV1 random(seed);
  const auto& selector = document.root.selector;
  if (selector.is_list) {
    const auto* list = find_list_v1(document, selector.list_name);
    if (!list) return {std::nullopt, "Crypt RootRule references missing list"};
    const auto index = selector.list_index;
    if (index && *index < -1) {
      return {std::nullopt, "negative RootRule list index below -1 is outside the source-safe slice"};
    }
    if (index && *index >= 0 && static_cast<std::size_t>(*index) < list->entries.size()) {
      // RootRule::Impl::Generate uses the indexed element directly and draws
      // no random value for this branch (0x48e8ac).
      plan.root_candidate_order.push_back(list->entries[static_cast<std::size_t>(*index)]);
    } else {
      plan.root_candidate_order = list->entries;
      if (!source_shuffle(plan.root_candidate_order, random)) {
        return {std::nullopt, "root candidate shuffle exceeds source uint32 bound"};
      }
    }
  } else {
    plan.root_candidate_order.reserve(selector.block_names.size());
    for (const auto& name : selector.block_names) {
      plan.root_candidate_order.push_back({name, {}, {}});
    }
    if (!source_shuffle(plan.root_candidate_order, random)) {
      return {std::nullopt, "root candidate shuffle exceeds source uint32 bound"};
    }
  }
  plan.rng_state_after_root = random.state();
  std::string error;
  if (!build_plan_node(document, document.root, plan.root_constraints, error)) {
    return {std::nullopt, std::move(error)};
  }
  return {std::move(plan), {}};
}

}  // namespace dh2::random_level
