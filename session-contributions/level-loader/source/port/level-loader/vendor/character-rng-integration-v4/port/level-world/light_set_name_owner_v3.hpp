#pragma once
#include <array>
#include <cstdint>
#include <string>
namespace dh2::world {
// Sole source LightSetManager name fields: C1/C2 construct four CString rows
// then copy the actual Names array at 40da14..80 / 40dcec..58. This is the
// reached name-query owner, not complete light/filter/driver initialization.
// A later full manager must adopt/borrow these fields, not allocate a shadow.
class LightSetNameOwnerV3 {
 std::array<std::string,4> names_;
public:
 LightSetNameOwnerV3();
 std::int32_t get_id(const std::string&)const noexcept;
 const std::array<std::string,4>& source_names()const noexcept{return names_;}
 std::array<std::string,4>& source_names()noexcept{return names_;}
};
}
