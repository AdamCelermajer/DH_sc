#pragma once
#include "../game-data/combat_result.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
namespace dh2::character {
struct CombatSoundListV1 {const std::int32_t* ids{};std::uint32_t count{};};
// Borrowed original CharSounds row (stride40): words4/8 death,12/16 hit,
// 20/24 impact1,28/32 impact2; bytes36/37 select attacker impact lists.
struct CombatSoundRowV1 {CombatSoundListV1 death,hit,impact1,impact2;std::uint8_t use_impact1{},use_impact2{};};
struct CombatSoundPlayV1 {
 std::uintptr_t manager{},target{};std::int32_t sound_id{};
 std::array<float,3> position{};bool source_bool{};std::int32_t source_integer{};
 float source_float0{},source_float1{};
};
struct CombatSoundServicesV1 {
 void* context{};
 int(*row)(void*,std::uintptr_t,const CombatSoundRowV1**){};
 // Calls actual Load/String/Get/Destroy for MP_MinimalRandoms in source order.
 int(*minimal_randoms)(void*,std::uint32_t*){};
 int(*dead)(void*,std::uintptr_t,bool*){};
 int(*manager)(void*,std::uintptr_t*){};
 // Source Random::GetRandom(count,false), sharing existing gameplay seed/count.
 int(*random)(void*,std::uint32_t,std::uint32_t*){};
 int(*position)(void*,std::uintptr_t,std::array<float,3>*){};
 // Must deliver source Vox Play3D request, including its subsequent gates.
 int(*play)(void*,const CombatSoundPlayV1*){};
};
struct CombatSoundOutputV1 {std::uint32_t phase{},played{},calls{};};
// Full source selection 3afee0 (character_attacker=true), or GameObject
// overload3afd38 (false). 0 completed source branch, -1 malformed, -2 required
// provider. Prefix deliveries/RNG effects are retained after failure.
int character_combat_sound_v1(CombatSoundOutputV1*,const data::CombatResult*,
 std::uintptr_t attacker,std::uintptr_t target,bool character_attacker,
 const CombatSoundServicesV1*);
}
