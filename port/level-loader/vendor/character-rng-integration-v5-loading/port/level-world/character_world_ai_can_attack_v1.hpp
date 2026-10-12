#pragma once
#include <cstdint>
#include <string>
namespace dh2::character {
enum class WorldAIAttackQueryV1 : std::uint32_t { IsEnemy, InventoryCanMeleeAttack, IsInMeleeRange, CharacterCanRangeAttack, IsInRange };
struct WorldAIAttackServicesV1 {
 void* context{};
 bool (*query)(void*,std::uintptr_t owner,std::uintptr_t target,WorldAIAttackQueryV1,std::int32_t&,std::string&){};
};
// Whole CharAI::AI_CanAttack 3d67f4. Null argument selects the actual AI+40
// current target borrow. These identities are never inferred from distance.
bool world_ai_can_attack_v1(std::uintptr_t owner,std::uintptr_t requested_target,
 std::uintptr_t current_target,const WorldAIAttackServicesV1&,bool&,std::string&);
}
