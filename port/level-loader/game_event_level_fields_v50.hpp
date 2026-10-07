#pragma once
#include <memory>
#include <cstdint>
namespace dh2::loader {
class GameEventManagerV50;
// Production pointers must borrow the SAME completed source Level C1 and
// actual Level-owned manager lease. Fixtures identify their storage explicitly.
struct GameEventLevelFieldsV50 {
 std::shared_ptr<void> level_owner;std::uintptr_t identity{};
 std::uint32_t* state130{};std::uintptr_t* field194{};
 std::shared_ptr<GameEventManagerV50>* owner194{};
};
}
