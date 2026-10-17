#include "../crypt_rule_distribution_v1.hpp"
#include "../native_rule_plan_v1.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct ExpectedRow {
  std::size_t open_exits;
  std::size_t child_rules;
  std::size_t record_count;
  std::uint64_t fnv1a;
};

constexpr ExpectedRow kExpectedRows[] = {
  {1, 1, 1, 0xd7e4fcfa299d713dULL},
  {1, 2, 2, 0x77d14ca72f1e9e5cULL},
  {1, 3, 3, 0x3bd1ba7748cbf6eeULL},
  {1, 4, 4, 0xbfc3f0c3d48248c5ULL},
  {1, 5, 5, 0x15e357aa3ff878f9ULL},
  {2, 2, 2, 0x0044b314e9d7e8c3ULL},
  {2, 3, 6, 0xa168fec889c9f695ULL},
  {2, 4, 9, 0x54051488db7785b7ULL},
  {2, 5, 16, 0x28f44ce4a7a3d31cULL},
  {3, 3, 6, 0x18c38d400b6e9b49ULL},
  {3, 4, 24, 0x7cf6d186d5857e4dULL},
  {3, 5, 60, 0x53e931292c218fb7ULL},
  {4, 4, 24, 0x6a3bc542219a80b5ULL},
  {4, 5, 120, 0x49ba495d13275b1bULL},
  {5, 5, 120, 0xade118ccda0afd9dULL},
};

int checks = 0;

bool check(bool condition, const std::string& label) {
  ++checks;
  if (!condition) std::cerr << "FAIL: " << label << '\n';
  return condition;
}

std::uint64_t hash_records(
    const std::vector<dh2::random_level::SourceRuleDistributionRecordV1>& records) {
  std::uint64_t hash = 14695981039346656037ULL;
  for (const auto& record : records) {
    for (const std::uint8_t byte : record.bytes) {
      hash ^= byte;
      hash *= 1099511628211ULL;
    }
  }
  return hash;
}

std::vector<std::vector<std::uint8_t>> active_prefix(
    const dh2::random_level::RuleDistributionCatalogV1& catalog,
    std::size_t count) {
  std::vector<std::vector<std::uint8_t>> result;
  for (const auto& record : catalog.records) {
    result.emplace_back(record.bytes.begin(), record.bytes.begin() + count);
  }
  return result;
}

}  // namespace

int main() {
  using dh2::random_level::RandomGeneratorV1;
  using dh2::random_level::RuleDistributionStatusV1;
  using dh2::random_level::enumerate_source_rule_distributions_v1;
  using dh2::random_level::shuffle_source_rule_distributions_v1;
  using dh2::random_level::source_shuffle;

  bool ok = true;
  for (const auto& expected : kExpectedRows) {
    const auto catalog =
        enumerate_source_rule_distributions_v1(expected.open_exits, expected.child_rules);
    ok &= check(catalog.status == RuleDistributionStatusV1::ready,
                "IDA row is exposed as ready");
    ok &= check(catalog.records.size() == expected.record_count,
                "IDA row record count");
    ok &= check(hash_records(catalog.records) == expected.fnv1a,
                "all six raw bytes match the IDA-derived row hash");
  }

  const auto two_from_four = enumerate_source_rule_distributions_v1(2, 4);
  const std::vector<std::vector<std::uint8_t>> expected_two_from_four = {
      {0, 1}, {0, 2}, {0, 3}, {1, 0}, {1, 2}, {1, 3}, {2, 0}, {2, 1}, {2, 3}};
  ok &= check(active_prefix(two_from_four, 2) == expected_two_from_four,
              "IDA row (2,4) preserves its nine-record subset and order");

  const auto two_from_five = enumerate_source_rule_distributions_v1(2, 5);
  const std::vector<std::vector<std::uint8_t>> expected_two_from_five = {
      {0, 1}, {0, 2}, {0, 3}, {0, 4},
      {1, 0}, {1, 2}, {1, 3}, {1, 4},
      {2, 0}, {2, 1}, {2, 3}, {2, 4},
      {4, 0}, {4, 1}, {4, 2}, {4, 3}};
  ok &= check(active_prefix(two_from_five, 2) == expected_two_from_five,
              "IDA row (2,5) preserves omitted leading-index group");

  const auto three_from_five = enumerate_source_rule_distributions_v1(3, 5);
  ok &= check(three_from_five.records[16].bytes[0] == 1 &&
                  three_from_five.records[16].bytes[1] == 2 &&
                  three_from_five.records[16].bytes[2] == 1,
              "IDA row (3,5) preserves repeated child-rule byte");
  const auto four_from_five = enumerate_source_rule_distributions_v1(4, 5);
  ok &= check(four_from_five.records[36] == four_from_five.records[42],
              "IDA row (4,5) preserves duplicate records");
  const auto five_from_five = enumerate_source_rule_distributions_v1(5, 5);
  ok &= check(five_from_five.records[36] == five_from_five.records[42],
              "IDA row (5,5) preserves duplicate records");

  ok &= check(enumerate_source_rule_distributions_v1(0, 3).status ==
                  RuleDistributionStatusV1::bypass_success,
              "zero open exits use native success bypass");
  ok &= check(enumerate_source_rule_distributions_v1(3, 0).status ==
                  RuleDistributionStatusV1::bypass_success,
              "zero child rules use native success bypass");
  ok &= check(enumerate_source_rule_distributions_v1(3, 2).status ==
                  RuleDistributionStatusV1::unsupported_shape,
              "unsnapshotted invalid shape is rejected");
  ok &= check(enumerate_source_rule_distributions_v1(6, 6).status ==
                  RuleDistributionStatusV1::unsupported_domain,
              "counts outside native table dimensions are rejected");

  auto shuffled = enumerate_source_rule_distributions_v1(2, 4);
  auto expected_shuffle = shuffled;
  RandomGeneratorV1 actual_rng(42);
  RandomGeneratorV1 expected_rng(42);
  ok &= check(shuffle_source_rule_distributions_v1(shuffled, actual_rng),
              "distribution shuffle succeeds");
  ok &= check(source_shuffle(expected_shuffle.records, expected_rng),
              "shared source shuffle reference succeeds");
  ok &= check(shuffled.records == expected_shuffle.records &&
                  actual_rng.state() == expected_rng.state(),
              "distribution shuffle reuses shared RNG and exact draw order");

  std::cout << "crypt_rule_distribution_v1 checks=" << checks
            << " status=" << (ok ? "PASS" : "FAIL") << '\n';
  return ok ? 0 : 1;
}
