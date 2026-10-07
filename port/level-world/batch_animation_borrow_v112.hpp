#pragma once
#include <functional>
#include <memory>
#include <string>
#include <cstdint>
namespace dh2::world {
//A retained reference to the existing animator/pose graph. The batch owns no
//new Scene, playback controller or animation clock.
struct BatchAnimationBorrowV112 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 std::function<bool(std::uint32_t,std::string&)> phase;
};
}
