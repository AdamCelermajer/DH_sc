#pragma once
#include "character_state_owner_behavior.hpp"
#include "character_state_owner_extensions.hpp"
#include "character_world_runtime_v1.hpp"
#include "character_dot_attack.hpp"
#include "character_idle_events.hpp"
namespace dh2::character {
// Source constructor fields absent from State56. These are actor/FSM storage,
// not replacement flags, HP, life or an alternate FSM. Use this as the sole
// field backing if integrated; old CombatActorState combo/push values then
// remain compatibility projections and must be synchronized at source writes.
struct CharacterConstructorCombatFieldsV1 {
 std::uint16_t combo=0; // Character C1 3a9700: halfword +14d0 = zero.
 std::uint8_t invulnerable=0; // C1 3a9740: byte +14f0 = zero.
 std::uint8_t push_death=0; // FSM C1 3c1b28: word+3c=0 includes byte+3f.
 std::int32_t network_id=-1; // ObjectBase C1 33f220/33f2d8: word+110=-1.
};
extern "C" int dh2_character_constructor_combat_fields_v1(CharacterConstructorCombatFieldsV1*);
// Whole CF_SetCombatants field producer. Levels are genuine resolved property19
// results supplied by the property owner, and context is the SAME shared owner.
extern "C" int dh2_world_combat_context_v1(DotCombatContext32*,std::uintptr_t,std::uintptr_t,
 std::int32_t attacker_level,std::int32_t target_level,std::int32_t element,std::uint8_t offhand,std::uint8_t magic);
struct WorldNpcStateServicesV1 {
 const Facts* facts{};const Services* bodies{};
 const StateOwnerBehaviorPredicate8* predicates{};
 StateOwnerServices16 remaining_methods{};
 NativeFsmServices16 outer{};
 StateOwnerUpdateServices16 remaining_updates{};
 // Complete genuine PreSpawn dependency only if this source family is reached.
 PreSpawnState48* pre_spawn{};const PreSpawnServices16* spawn_services{};
 StateOwnerDebugDiagnostics* diagnostics{};
 // Legacy body-oracle fixtures may isolate diagnostics. Production source
 // initialization sets this flag; missing actual World Debug/files then fails.
 std::uint32_t diagnostics_required{};
 // Refresh live producer borrows at the reached virtual body, after its
 // source early gates have been identified by the owning adapter.
 void* prepare_context{};
 int(*prepare_method)(void*,const StateOwnerRequest48&){};
 int(*prepare_update)(void*){};
};
class CharacterWorldNpcStateOwnerV1 {
 CharacterStateOwner owner_;
 CharacterConstructorCombatFieldsV1 combat_{};
 WorldNpcStateServicesV1 services_;
 StateOwnerExtensions48 extensions_{};
 StateOwnerBehaviorContext40 behavior_{};
 StateOwnerServices16 methods_{};
 StateOwnerServices16 bound_methods_{};
 StateOwnerFrameContext56 frame_{};
 std::string error_;
 bool coherent()noexcept;
 int completed(int,const char*);
 static int method(void*,StateOwnerMachine40*,const StateOwnerRequest48*,StateOwnerResponse8*);
public:
 // The NPC owns ONE actual registered StateInfo/FSM graph. Recovered source
 // constructor flags/current/null-current are kept until genuine level init.
 CharacterWorldNpcStateOwnerV1(std::uintptr_t,WorldNpcStateServicesV1);
 CharacterWorldNpcStateOwnerV1(const CharacterWorldNpcStateOwnerV1&)=delete;
 CharacterStateOwner& owner()noexcept{return owner_;}
 State& state()noexcept{return owner_.state();}
 NativeFsm24& native_fsm()noexcept{return owner_.native_fsm();}
 CharacterConstructorCombatFieldsV1& combat_fields()noexcept{return combat_;}
 // Only genuine source preset CString may select initial level state. No
 // conversion from renderer animation/state labels or inferred alive status.
 int initialize_level(const char* source_preset);
 //Original GetPreSetAIState selects from full std::string equality.
 int initialize_level_preset_v95(std::int32_t preset);
 int transition(std::int32_t next,std::int32_t event,std::uintptr_t payload);
 int event(std::int32_t event,std::uintptr_t payload);
 int update();
 const std::string& error()const noexcept{return error_;}
};
}
