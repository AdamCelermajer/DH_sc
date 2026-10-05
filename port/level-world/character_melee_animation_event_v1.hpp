#pragma once
#include "character_world_skill_combat_v6.hpp"
#include "character_world_ai_can_attack_v1.hpp"
namespace dh2::character::skills {
// Ordered CharAI::_OnAnimEvent (3d4434). All values are borrowed from the
// active animator/AI/Character, including the otherwise-unused step count.
enum MeleeAnimationOperationV1 : unsigned {
 melee_event_step_index=1,melee_event_step_count,melee_event_relay,
 melee_event_object_animation,melee_event_mesh_fx,melee_event_sound_fx,
 melee_event_state,melee_event_can_range,melee_event_projectile,
 melee_event_attack,melee_event_skill,melee_event_spell,melee_event_interact
};
struct MeleeAnimationRequestV1 {
 unsigned operation{};const char* text{};int index{},step{};bool offhand{};
};
struct MeleeAnimationResponseV1 {int value{},projectile{};};
struct MeleeAnimationServicesV1 {
 void* context{};
 int(*invoke)(void*,const MeleeAnimationRequestV1*,MeleeAnimationResponseV1*){};
 const SkillAttackNativeServicesV6* debug{};
};
struct MeleeAnimationOutputV1 {unsigned calls{},phase{};int step{},state{},status{};};
extern "C" int dh2_character_melee_animation_event_v1(MeleeAnimationOutputV1*,
 const char*,const MeleeAnimationServicesV1*);
// F_MeleeAttack 3b3368 uses the actual common CF/random and inventory owners.
// Default AIS OnAttack always supplies critical=false. Critical temporary
// class application remains an explicit source provider when requested.
struct MeleeCriticalClassServicesV1 {void* context{};int(*apply)(void*,data::PropertyView*){};};
extern "C" int dh2_character_melee_calculate_v1(data::CombatResult*,
 DotCombatContext32*,data::CombatRandom*,const SkillAttackActorV6*,
 const SkillAttackActorV6*,const data::FreshInventoryOwnedV4*,bool offhand,
 bool critical,const SkillAttackNativeServicesV6*,const MeleeCriticalClassServicesV1*);
struct WorldMeleeAttackBorrowV1 {
 std::uintptr_t owner{};const std::uintptr_t* target{};
 // Character+408 is distinct from CharAI+40. Borrow its actual producer.
 const std::uintptr_t* look_at{};
 const std::uintptr_t* active_ais{};
 const std::uint32_t* on_attack_address{};
 const data::FreshInventoryOwnedV4* inventory{};
};
struct WorldMeleeAttackBackendsV1 {
 void* context{};
 int(*activate)(void*,std::uintptr_t object,std::uintptr_t owner){};
 int(*remaining_ais)(void*,std::uintptr_t ais,int index,int step,bool offhand){};
};
// Whole CharAI::OnAttack -> AISPlayer/default OnAttack. The source virtual
// receiver and real target conversion are checked before calculation. The
// existing combat owner applies to the same registered actors and life stores.
class CharacterWorldMeleeAttackV1 {
 CharacterWorldSkillCombatV6& combat_;CharacterWorldRuntimeV1& world_;
 DotCombatContext32& context_;data::CombatRandom& random_;
 WorldMeleeAttackBorrowV1 borrow_;WorldAIAttackServicesV1 queries_;
 WorldMeleeAttackBackendsV1 backends_;std::string error_;
 data::CombatResult result_{};SkillApplyOutputV6 application_{};
public:
 CharacterWorldMeleeAttackV1(CharacterWorldSkillCombatV6&,CharacterWorldRuntimeV1&,
  DotCombatContext32&,data::CombatRandom&,WorldMeleeAttackBorrowV1,
  WorldAIAttackServicesV1,WorldMeleeAttackBackendsV1);
 int attack(int index,int step,bool offhand);
 const std::string& error()const noexcept{return error_;}
 const data::CombatResult& result()const noexcept{return result_;}
 const SkillApplyOutputV6& application()const noexcept{return application_;}
};
}
