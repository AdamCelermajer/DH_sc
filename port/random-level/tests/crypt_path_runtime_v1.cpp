#include "../crypt_path_runtime_v1.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

namespace {

int checks = 0;
int failures = 0;

void check(bool condition, const char* label) {
  ++checks;
  if (!condition) {
    ++failures;
    std::cerr << "FAIL: " << label << '\n';
  }
}

}  // namespace

int main() {
  using namespace dh2::random_level;

  RandomGeneratorV1 range_rng(17);
  RandomGeneratorV1 expected_rng(17);
  const auto expected_length = expected_rng.get_int(2, 3);
  const auto range = choose_crypt_path_length_v1(2, 3, false, std::nullopt,
                                                  range_rng);
  check(range.length == expected_length, "range uses source GetInt result");
  check(range.length == 2, "upper bound is exclusive for Crypt length 2,3");
  check(range_rng.state() == expected_rng.state(),
        "nonempty length range consumes exactly one RNG draw");
  check(range.source == CryptPathLengthSourceV1::random_range,
        "range selection records random source");

  RandomGeneratorV1 fixed_rng(23);
  const auto fixed = choose_crypt_path_length_v1(2, 8, true, 6, fixed_rng);
  check(fixed.length == 6, "RoomPool length is fixed pool value");
  check(fixed_rng.state() == 23, "matching RoomPool consumes no RNG");
  check(fixed.source == CryptPathLengthSourceV1::room_pool_value,
        "fixed selection records RoomPool source");

  RandomGeneratorV1 missing_pool_rng(29);
  const auto missing_pool = choose_crypt_path_length_v1(
      2, 8, true, std::nullopt, missing_pool_rng);
  check(missing_pool.length == 1,
        "unmatched named RoomPool leaves Rule::Impl default length");
  check(missing_pool_rng.state() == 29,
        "unmatched named RoomPool consumes no RNG");
  check(missing_pool.source == CryptPathLengthSourceV1::room_pool_lookup_miss,
        "unmatched RoomPool is distinguishable");

  RandomGeneratorV1 equal_rng(31);
  const auto equal = choose_crypt_path_length_v1(4, 4, false, std::nullopt,
                                                  equal_rng);
  check(equal.length == 4, "equal bounds return lower bound");
  check(equal_rng.state() == 31, "equal bounds consume no RNG");

  CryptPathRuntimeV1 default_path;
  default_path.begin_step(0x11u);
  check(default_path.dont_go_back, "Path default dontGoBack is true");
  check(default_path.direction == 0x11u,
        "first step saves opposite incoming direction");
  default_path.current_step = 1;
  default_path.begin_step(0x22u);
  check(default_path.direction == 0x11u,
        "saved direction is not replaced after first step");

  CryptPathRuntimeV1 free_direction_path;
  free_direction_path.dont_go_back = false;
  free_direction_path.begin_step(0x33u);
  check(free_direction_path.direction == kCryptNoDirectionV1,
        "dontGoBack false leaves direction sentinel");

  const std::vector<CryptDirectionKeyV1> no_matching_exit = {0x21u, 0x22u};
  const std::vector<CryptDirectionKeyV1> matching_exit = {0x11u, 0x22u};
  check(free_direction_path.candidate_direction_allows_spawn(
            no_matching_exit, 0x44u),
        "direction sentinel allows candidate");
  check(default_path.candidate_direction_allows_spawn(no_matching_exit, 0x44u),
        "module without saved-direction exit falls through to TrySpawn");
  check(!default_path.candidate_direction_allows_spawn(matching_exit, 0x44u),
        "matching module direction with different incoming direction rejects");
  check(default_path.candidate_direction_allows_spawn(matching_exit, 0x11u),
        "matching module and incoming directions allow TrySpawn");
  check(default_path.candidate_direction_allows_spawn({}, 0x44u),
        "module with no exits falls through to TrySpawn");

  CryptPathRuntimeV1 terminal_path;
  terminal_path.chosen_length = 4;
  terminal_path.current_step = 3;
  check(terminal_path.is_dead_end(0),
        "IsDeadEnd matches final next step with no child rules");
  check(!terminal_path.is_dead_end(1),
        "child rules prevent IsDeadEnd result");
  terminal_path.current_step = 2;
  check(!terminal_path.is_dead_end(0),
        "nonfinal step is not a dead end");
  check(!terminal_path.length_reached(), "length is not reached below limit");
  terminal_path.current_step = 4;
  check(terminal_path.length_reached(), "length gate accepts count at limit");
  terminal_path.current_step = 5;
  check(terminal_path.length_reached(), "length gate uses greater-than-or-equal");

  CryptPathRuntimeV1 attempts;
  const auto recurse = attempts.begin_child_attempt(2);
  check(recurse == CryptPathChildAttemptV1::recurse,
        "multi-exit candidate invokes recursive callback");
  check(attempts.current_step == 1,
        "recursive candidate increments count before callback");
  attempts.rollback_failed_child_attempt(recurse);
  check(attempts.current_step == 0,
        "failed recursive callback rolls count back");
  const auto terminal = attempts.begin_child_attempt(1);
  check(terminal == CryptPathChildAttemptV1::terminal_one_exit,
        "one-exit module terminates without recursive callback");
  check(attempts.current_step == 0,
        "one-exit terminal does not increment count");
  attempts.rollback_failed_child_attempt(terminal);
  check(attempts.current_step == 0,
        "terminal candidate has no recursive count to roll back");

  if (failures) {
    std::cerr << failures << " of " << checks << " checks failed\n";
    return 1;
  }
  std::cout << checks << " Crypt Path runtime checks passed\n";
  return 0;
}
