#pragma once
#include "physical_world.hpp"
#include "native_body.hpp"
#include <string>
namespace dh2::physical {
struct NativePhysicalFilterBorrowV1 {
 NativeWorld* world{};NativeBody* body{};b2Shape* const* primary{};
 b2Shape* const* secondary{};const b2FilterData* saved{};std::uint8_t* disabled{};
};
// Complete enableFilter46ebe4/disableFilter46eb70. Borrow original saved
// halfwords20/22/24, actual shape18/1c and sole disabled byte26.
bool native_physical_filter_v1(const NativePhysicalFilterBorrowV1&,bool enabled,std::string&);
}
