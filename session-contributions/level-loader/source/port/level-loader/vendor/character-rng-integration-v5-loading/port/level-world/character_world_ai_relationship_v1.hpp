#pragma once
#include "character_target_providers.hpp"
#include "../game-data/ai.hpp"
namespace dh2::character::skills {
enum WorldAiServiceV1:std::uint32_t {world_ai_handle=1,world_ai_object,world_ai_kind,world_ai_faction,world_ai_player,world_ai_interactive,world_ai_interaction_type,world_ai_assert};
struct WorldAiRequestV1 {std::uint32_t service,argument;std::uintptr_t subject,other;target_providers::Handle16* handle;};
struct WorldAiResponseV1 {std::uintptr_t identity;std::int32_t value;std::uint32_t reserved;target_providers::Handle16 handle;};
struct WorldAiServicesV1 {void* context;int(*invoke)(void*,const WorldAiRequestV1*,WorldAiResponseV1*);};
struct WorldAiFactionRowV1 {const data::AiFactionEntry* entries;std::uint32_t count,reserved;};
struct WorldAiRelationshipV1 {
 std::uintptr_t character;const std::uintptr_t* current_target;
 const WorldAiFactionRowV1* rows;std::uint32_t count,reserved;
 const std::int32_t* assert_mode;
};
struct WorldAiOutputV1 {std::uint32_t result,phase,calls,assert_line;};
// Whole CharAI AI_IsFriend/AI_IsEnemy ordering. enemy 0/1, nullable target
// resolves through actual current target. 0 complete, -1 malformed, -2 required
// service, -3 original fatal assertion/unsafe foreign table continuation.
extern "C" int dh2_world_ai_relationship_v1(WorldAiOutputV1*,const WorldAiRelationshipV1*,std::uint32_t enemy,std::uintptr_t target,const WorldAiServicesV1*);
// Whole Character.GetCharAIFactionId: live resolved word0, invalid -> faction10.
extern "C" std::int32_t dh2_world_ai_faction_v1(const std::int32_t* resolved,std::uint32_t faction_count);
}
