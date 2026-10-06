#pragma once
#include "player_manager_owner_v1.hpp"
#include "character_skill_combat_v6.hpp"
namespace dh2::player {
// This bridge retains the original manager map and scalar PlayerInfo projection.
// Embedded network-property storage remains owned by the supplied constructor;
// no network or whole AddCharacter call is substituted by adoption.
struct PlayerManagerCombatServicesV2 {
 void* context{};
 bool(*invoke)(void*,const PlayerManagerRequestV1&,PlayerManagerResponseV1&,std::string&){};
};
struct DevelopmentPlayerLaunchV2 {
 std::int32_t internal{},controller{},local_index{};bool local{true};
 std::uintptr_t character{};const std::int16_t* same_base_id{};
};
class PlayerManagerCombatRuntimeV2 {
 PlayerManagerCombatServicesV2 services_;
 PlayerManagerOwnerV1 manager_;
 std::string error_;
 static bool service(void*,const PlayerManagerRequestV1&,PlayerManagerResponseV1&,std::string&);
public:
 explicit PlayerManagerCombatRuntimeV2(PlayerManagerCombatServicesV2 services);
 bool initialize();
 bool initialize_development_scalar_projection();
 bool adopt_created_character(const DevelopmentPlayerLaunchV2&);
 // Explicit development launch transport, separate from original AddCharacter.
 // Source AddPlayer initializes record identity/local ordinals; the caller then
 // publishes the SAME already-created Character at original store3722ec.
 bool adopt_created_character(std::int32_t internal,std::int32_t controller,
  std::int32_t local_index,bool local,std::uintptr_t character,
  const std::int16_t* same_base_id);
 int hit(const character::HitRequest32&,std::uintptr_t*);
 int application(const character::skills::SkillApplyRequestV6&,
  character::skills::SkillApplyResponseV6*);
 PlayerManagerOwnerV1& manager() noexcept{return manager_;}
 const std::string& error()const noexcept{return error_;}
private:
 bool development_scalar_projection_{};
};
}
