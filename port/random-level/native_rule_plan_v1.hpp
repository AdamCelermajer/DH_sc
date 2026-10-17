#pragma once

#include <cstdint>
#include <cstddef>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace dh2::random_level {

// Source evidence: libDungeonHunter2.so, rnd::RandomGenerator::NextInt at
// 0x483a94 and operator()(unsigned int) at 0x483af8.
class RandomGeneratorV1 {
 public:
  explicit RandomGeneratorV1(std::uint32_t seed) : state_(seed) {}

  std::uint32_t next_int();
  std::optional<std::uint32_t> bounded(std::uint32_t bound);
  std::int32_t get_int(std::int32_t lower, std::int32_t upper);
  std::uint32_t state() const { return state_; }

 private:
  std::uint32_t state_;
};

// Matches the custom std::random_shuffle instantiation at 0x48c31c: for i in
// [1,n), draw below i+1 and swap values[i] with values[draw].
template <typename T>
bool source_shuffle(std::vector<T>& values, RandomGeneratorV1& random) {
  for (std::size_t i = 1; i < values.size(); ++i) {
    if (i + 1 > static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max())) return false;
    const auto selected = random.bounded(static_cast<std::uint32_t>(i + 1));
    if (!selected) return false;
    using std::swap;
    swap(values[i], values[*selected]);
  }
  return true;
}

struct CryptListEntryV1 {
  std::string name;
  std::string gameplay;
  std::string visual;
};

struct CryptListV1 {
  std::string name;
  // Native ListRule reads the `replacement` attribute. It does not read the
  // `random` attribute present in this Crypt XML; the native default is true.
  bool replacement = true;
  std::vector<CryptListEntryV1> entries;
};

enum class RuleKindV1 { root, force_block, path };

struct RuleSelectorV1 {
  std::string source_text;
  bool is_list = false;
  std::string list_name;
  std::optional<std::int32_t> list_index;
  std::vector<std::string> block_names;
};

struct PathOptionsV1 {
  std::optional<std::int32_t> min_length;
  std::optional<std::int32_t> max_length;
  // Path constructor sets this true. The Crypt file's lowercase
  // `dontgoback="false"` is not the native case-sensitive numeric
  // `dontGoBack` attribute, so it leaves the default unchanged.
  bool dont_go_back = true;
  bool dont_go_back_attribute_applied = false;
};

struct RuleNodeV1 {
  RuleKindV1 kind = RuleKindV1::root;
  RuleSelectorV1 selector;
  PathOptionsV1 path;
  std::vector<RuleNodeV1> children;
};

struct CryptRulePropertyV1 {
  std::string name;
  std::string value;
};

struct CryptRuleDocumentV1 {
  std::string target;
  std::string folder;
  // Root attributes other than target/folder that may override declared
  // LevelConfig properties. Native PropertyMap ignores undeclared attributes.
  std::vector<CryptRulePropertyV1> root_property_overrides;
  std::vector<CryptListV1> lists;
  RuleNodeV1 root;
};

struct ParseResultV1 {
  std::optional<CryptRuleDocumentV1> document;
  std::string error;
  explicit operator bool() const { return document.has_value(); }
};

ParseResultV1 parse_crypt_rule_v1(std::string_view xml);

const CryptListV1* find_list_v1(const CryptRuleDocumentV1& document,
                                std::string_view name);

struct RulePlanNodeV1 {
  RuleKindV1 kind = RuleKindV1::root;
  RuleSelectorV1 selector;
  // This is an allowed domain for native exit filtering, not a chosen module.
  // The choice requires MGX exits, MGP fit and native recursion/backtracking.
  std::vector<CryptListEntryV1> allowed_list_entries;
  PathOptionsV1 path;
  std::vector<RulePlanNodeV1> children;
};

struct RulePlanV1 {
  std::vector<CryptListEntryV1> root_candidate_order;
  RulePlanNodeV1 root_constraints;
  std::uint32_t rng_state_after_root = 0;
  // This slice ends before Tile::NewRoot/Tile::TrySpawn and emits no <Level>.
  bool generated_level = false;
};

struct PlanResultV1 {
  std::optional<RulePlanV1> plan;
  std::string error;
  explicit operator bool() const { return plan.has_value(); }
};

PlanResultV1 build_rule_plan_v1(const CryptRuleDocumentV1& document,
                                std::uint32_t seed);

}  // namespace dh2::random_level
