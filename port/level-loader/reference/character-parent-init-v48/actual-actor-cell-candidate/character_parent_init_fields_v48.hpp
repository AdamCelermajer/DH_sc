#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <string>
namespace dh2::character {
// Typed views only. Actor embeds each source scalar once; these views do not
// allocate a CanonicalBase, property sheet, visual, condition or save object.
struct CharacterConditionDataFieldsV48 {
 std::uint32_t source_offset{};
 const std::string* name{};
 std::uintptr_t* compiled{};
 std::uint8_t* tested{};
};
struct CharacterParentInitFieldsV48 {
 std::shared_ptr<const void> receiver_lease;
 std::uintptr_t identity{};
 std::array<CharacterConditionDataFieldsV48,2> conditions;
 const std::string* min_difficulty_d4{};
 std::int32_t* min_difficulty_ec{};
 const std::int32_t* probability274{};
 float* scale120{};float* position160{};float* rotation16c{};float* heading178{};
 float* relative144{};float* absolute12c{};float* destination1a8{};
 std::uintptr_t* physical2dc{};
 const std::uintptr_t* attached2e0{};
 std::uintptr_t* visual2d8{};
 const std::uint8_t* facultative60{};
 const std::uint8_t* optimization10c{};
 const std::uint8_t* skip_checkpoint15c{};
 std::uint8_t* checkpoint28{};
 const std::string* idle_sound358{};
 std::int16_t* idle_sound370{}; // actual original C2 and InitPost STRH
};
}
