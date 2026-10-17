#include "character_physics_position.hpp"

#include "move_state.hpp"

#include <cstddef>

namespace dh2::character_physics_position {
namespace {
struct Range {
    std::uintptr_t begin;
    std::uintptr_t end;
};

bool range(const void* pointer, std::size_t size, std::size_t alignment,
           Range& out) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignment || begin > UINTPTR_MAX - size)
        return false;
    out = {begin, begin + size};
    return true;
}

bool overlap(Range a, Range b) {
    return a.begin < b.end && b.begin < a.end;
}
}  // namespace

Status query(const CharacterView* character, Result* result) {
    Range view_range{}, result_range{}, flags_range{};
    if (!range(character, sizeof(*character), alignof(CharacterView), view_range) ||
        !range(result, sizeof(*result), alignof(Result), result_range) ||
        overlap(view_range, result_range) || !character->identity ||
        !range(character->flags_520, sizeof(*character->flags_520),
               alignof(std::uint32_t), flags_range) ||
        overlap(view_range, flags_range) || overlap(result_range, flags_range))
        return Status::invalid_argument;

    dh2::move::Policy decoded{};
    if (dh2_move_policy(&decoded, character->flags_520) != 0)
        return Status::policy_failure;
    *result = {character->identity, decoded.position_from_physics};
    return Status::complete;
}

}  // namespace dh2::character_physics_position

extern "C" int dh2_character_is_updating_position_from_physics(
    const dh2::character_physics_position::CharacterView* character,
    dh2::character_physics_position::Result* result) {
    return static_cast<int>(dh2::character_physics_position::query(character,
                                                                    result));
}
