#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::character {
struct CharacterIsPlayerFieldsV41 {
 std::shared_ptr<const void> receiver_lease;
 std::uintptr_t identity{};
 const std::string* live_archetype{};
};
struct CharacterIsPlayerServicesV41 {
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> actual_get_char_type;
};
// Whole original IsPlayer3a49f0 body. GetCharType remains an actual borrowed
// service. Archetype is read only for type0, from the SAME live receiver.
bool character_is_player_v41(const CharacterIsPlayerFieldsV41&,
 const CharacterIsPlayerServicesV41&,bool&,std::string&);
}
