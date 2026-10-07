#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {
// Actual nullable singleton snapshot, pinned across BOTH Door LoadSound calls.
// Neither a new sound owner nor a bool readiness/side registry.
struct StartupSoundManagerBorrowV87 {
 std::shared_ptr<void> receiver;std::uintptr_t identity{};
 std::function<bool(std::int32_t,std::string&)> load_sound;
 bool valid_nullable()const noexcept{return receiver?identity&&bool(load_sound):!identity&&!load_sound;}
 explicit operator bool()const noexcept{return bool(receiver);}
};
}
