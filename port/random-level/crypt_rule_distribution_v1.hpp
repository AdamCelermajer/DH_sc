#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace dh2::random_level {

class RandomGeneratorV1;

enum class RuleDistributionStatusV1 {
  ready,
  bypass_success,       // Rule::Impl::Step returns success when either count is 0.
  unsupported_shape,    // No checked-in source row for exits > child rules.
  unsupported_domain,   // Native table dimensions are bounded to counts 0..5.
};

struct SourceRuleDistributionRecordV1 {
  std::array<std::uint8_t, 6> bytes{};

  bool operator==(const SourceRuleDistributionRecordV1& other) const noexcept {
    return bytes == other.bytes;
  }
};

struct RuleDistributionCatalogV1 {
  RuleDistributionStatusV1 status = RuleDistributionStatusV1::unsupported_domain;
  // Full six-byte records in the original gDistributions order. Duplicates and
  // non-lexicographic rows are preserved; the trailing 0xff sentinel is omitted.
  std::vector<SourceRuleDistributionRecordV1> records;
};

// IDA snapshot: libDungeonHunter2.so gDistributions @ 0x8ce5c8,
// Rule::Impl::Step @ 0x48f954. Index formula is 4356*open_exits +
// 726*child_rules; each record is six bytes and the next record's first signed
// byte is the negative terminator. The table has 15 checked-in rows for
// 1 <= open_exits <= child_rules <= 5.
RuleDistributionCatalogV1 enumerate_source_rule_distributions_v1(
    std::size_t open_exit_count, std::size_t child_rule_count);

// The native code shuffles pointers to these records after table lookup. This
// helper applies the existing shared source_shuffle/RNG to the record vector.
bool shuffle_source_rule_distributions_v1(RuleDistributionCatalogV1& catalog,
                                          RandomGeneratorV1& random);

}  // namespace dh2::random_level
