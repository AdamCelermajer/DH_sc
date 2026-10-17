#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "native_rule_plan_v1.hpp"

namespace dh2::random_level {

// `Direction` is copied as a 32-bit value from Direction::sDirections by the
// native Path::Impl. Four is the source sentinel meaning “no saved direction”.
using CryptDirectionKeyV1 = std::uint32_t;
inline constexpr CryptDirectionKeyV1 kCryptNoDirectionV1 = 4;

enum class CryptPathLengthSourceV1 {
  random_range,
  room_pool_value,
  room_pool_lookup_miss,
};

struct CryptPathLengthSelectionV1 {
  std::int32_t length = 1;
  CryptPathLengthSourceV1 source = CryptPathLengthSourceV1::random_range;
};

// Mirrors Path::Impl construction. A named RoomPool avoids RNG use; if the
// name was present but no pool matched, Rule::Impl's initialized length of 1
// is left intact. Without a RoomPool name, GetInt(min,max) uses an exclusive
// upper bound and consumes one source RNG value only when min < max.
CryptPathLengthSelectionV1 choose_crypt_path_length_v1(
    std::int32_t min_length, std::int32_t max_length,
    bool room_pool_name_present,
    std::optional<std::int32_t> matching_room_pool_length,
    RandomGeneratorV1& random);

enum class CryptPathChildAttemptV1 {
  terminal_one_exit,
  recurse,
};

// State owned by one Path::Impl. Candidate enumeration, Tile::TrySpawn,
// recursion, Unspawn, and the source ordered vector<pair<Exit,ListElem>> remain
// caller-owned. This object never shuffles candidates or changes ListElem.
struct CryptPathRuntimeV1 {
  std::int32_t chosen_length = 1;
  std::int32_t current_step = 0;
  bool dont_go_back = true;
  CryptDirectionKeyV1 direction = kCryptNoDirectionV1;

  // Call after the native max-length early return, with the source's
  // Direction::sDirections value for opposite(incoming_exit.direction).
  void begin_step(CryptDirectionKeyV1 opposite_incoming_direction);

  bool length_reached() const;
  bool is_dead_end(std::int32_t child_rule_count) const;

  // Exact OneStep directional branch: a candidate is rejected only when its
  // module contains the saved direction AND the current opposite-incoming
  // direction differs from it. No matching module exit falls through to spawn.
  bool candidate_direction_allows_spawn(
      const std::vector<CryptDirectionKeyV1>& candidate_module_exit_directions,
      CryptDirectionKeyV1 opposite_incoming_direction) const;

  // Native Path::OneStep does not recurse into a one-exit module. Otherwise it
  // increments before the callback; caller rolls back only a failed recurse.
  CryptPathChildAttemptV1 begin_child_attempt(
      std::size_t candidate_module_exit_count);
  void rollback_failed_child_attempt(CryptPathChildAttemptV1 attempt);
};

}  // namespace dh2::random_level
