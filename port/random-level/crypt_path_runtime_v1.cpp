#include "crypt_path_runtime_v1.hpp"

#include <algorithm>

namespace dh2::random_level {

CryptPathLengthSelectionV1 choose_crypt_path_length_v1(
    std::int32_t min_length, std::int32_t max_length,
    bool room_pool_name_present,
    std::optional<std::int32_t> matching_room_pool_length,
    RandomGeneratorV1& random) {
  if (room_pool_name_present) {
    if (matching_room_pool_length) {
      return {*matching_room_pool_length,
              CryptPathLengthSourceV1::room_pool_value};
    }
    // Rule::Impl initializes this field to 1 before Path::Impl searches pools.
    return {1, CryptPathLengthSourceV1::room_pool_lookup_miss};
  }

  return {random.get_int(min_length, max_length),
          CryptPathLengthSourceV1::random_range};
}

void CryptPathRuntimeV1::begin_step(
    CryptDirectionKeyV1 opposite_incoming_direction) {
  if (current_step == 0 && dont_go_back) {
    direction = opposite_incoming_direction;
  }
}

bool CryptPathRuntimeV1::length_reached() const {
  return current_step >= chosen_length;
}

bool CryptPathRuntimeV1::is_dead_end(std::int32_t child_rule_count) const {
  return current_step + 1 == chosen_length && child_rule_count == 0;
}

bool CryptPathRuntimeV1::candidate_direction_allows_spawn(
    const std::vector<CryptDirectionKeyV1>& candidate_module_exit_directions,
    CryptDirectionKeyV1 opposite_incoming_direction) const {
  if (direction == kCryptNoDirectionV1) return true;

  const bool module_has_saved_direction =
      std::find(candidate_module_exit_directions.begin(),
                candidate_module_exit_directions.end(), direction) !=
      candidate_module_exit_directions.end();
  if (!module_has_saved_direction) return true;

  return opposite_incoming_direction == direction;
}

CryptPathChildAttemptV1 CryptPathRuntimeV1::begin_child_attempt(
    std::size_t candidate_module_exit_count) {
  if (candidate_module_exit_count == 1) {
    return CryptPathChildAttemptV1::terminal_one_exit;
  }
  ++current_step;
  return CryptPathChildAttemptV1::recurse;
}

void CryptPathRuntimeV1::rollback_failed_child_attempt(
    CryptPathChildAttemptV1 attempt) {
  if (attempt == CryptPathChildAttemptV1::recurse) --current_step;
}

}  // namespace dh2::random_level
