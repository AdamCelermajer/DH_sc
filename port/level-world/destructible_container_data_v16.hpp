#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::world {
// Actual Structs::DestructibleContainer44 row. Preserve every serialized word,
// including unconsumed resistance/effect properties, rather than omit them.
struct DestructibleContainerRowV16 {
 std::array<std::uint32_t,7> prefix4_1c{};std::uint8_t keep_physics20{};
 std::uint32_t raw24{},loot28{},script_length2c{};std::string script30;
 std::array<std::uint32_t,4> tail34_40{};
 std::int32_t sound()const noexcept{return static_cast<std::int32_t>(prefix4_1c[6]);}
 std::int32_t loot()const noexcept{return static_cast<std::int32_t>(loot28);}
 std::int32_t visual()const noexcept{return static_cast<std::int32_t>(tail34_40[2]);}
};
class DestructibleContainerTableV16 {
 std::vector<std::string> names_;std::vector<DestructibleContainerRowV16> rows_;
public:
 bool load(const std::uint8_t*,std::size_t,const std::uint8_t*,std::size_t,std::string&);
 std::size_t size()const noexcept{return rows_.size();}
 const std::vector<std::string>& names()const noexcept{return names_;}
 const DestructibleContainerRowV16* row(std::int32_t id)const noexcept{return id>=0&&std::size_t(id)<rows_.size()?&rows_[id]:nullptr;}
 std::int32_t data_id(const std::string& name)const noexcept;
};
}
