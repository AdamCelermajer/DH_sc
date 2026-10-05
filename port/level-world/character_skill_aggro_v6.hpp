#pragma once
#include "../game-data/aggro.hpp"
#include <cstdint>
namespace dh2::character::skills {
enum SkillAggroServiceV6:std::uint32_t {skill_aggro_owner_player_v6=1,skill_aggro_owner_dead_v6,skill_aggro_target_dead_v6,skill_aggro_target_on_aggro_v6};
struct SkillAggroRequestV6 {std::uint32_t service,reserved;std::uintptr_t subject,other;};
struct SkillAggroServicesV6 {void* context;int(*invoke)(void*,const SkillAggroRequestV6*,std::uint32_t*);};
struct SkillAggroOwnerV6 {std::uintptr_t identity;data::AggroTable* outgoing;};
struct SkillAggroTargetV6 {std::uintptr_t identity;data::AggroTable* incoming;};
struct SkillAggroOutputV6 {std::uint32_t returned_bits,calls,phase,notify_delivered;std::int32_t status;std::uint32_t reserved;};
// Source AI_SetAggro/AI_AddAggro over SAME world AI relation storage. Add
// captures the previous value before live virtual queries; Set calls target
// OnAggro BEFORE either source map is written when the entry was absent.
// This callback requires its actual AIS backend, never generic success.
// -2 retains the source query/notification prefix, with no invented relation.
extern "C" int dh2_character_skill_aggro_v6(SkillAggroOutputV6*,SkillAggroOwnerV6*,
 SkillAggroTargetV6*,std::uint32_t amount_bits,std::uint32_t add,const SkillAggroServicesV6*);
}
