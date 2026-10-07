#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>
namespace dh2::world {
struct ModuleFogBorrowV67 {
 std::uintptr_t identity{};
 const float* position160{};std::array<float,3>* color3f0{};
};
// Actual source manager Module68 list, and libc lrand48() process stream. Both
// are supplied by genuine owners; this code allocates no scene/map/Random.
bool source_init_module_fog_v67(const std::vector<ModuleFogBorrowV67>&,
 const std::vector<std::array<float,3>>&,std::int32_t,
 const std::function<bool(std::int32_t&,std::string&)>& actual_libc_lrand48,std::string&);
bool source_dynamic_module_fog_v67(const std::vector<ModuleFogBorrowV67>&,
 const float position[3],float radius,std::array<float,3>& output,std::string&);
}
