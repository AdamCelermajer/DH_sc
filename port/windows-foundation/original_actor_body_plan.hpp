#pragma once
#include "original_actor_bounds.hpp"
#include "original_character.hpp"
#include "../game-data/ai.hpp"
#include "../game-data/properties.hpp"
namespace dh::foundation {
struct OriginalActorBodyPlanInput {
 CharacterVisualConfig visual;
 const dh2::data::PropertyState* properties{};
 const dh2::data::AiTables* ai{};
 Vec3 position{},rotation_degrees{};
 std::string source_name;
 std::uintptr_t owner_identity{};
 std::uint8_t static84{},previous_flat{};
};
struct OriginalActorBodyPlan {
 OriginalActorBoundsResult bounds;
 ActorBoundsConfig components;
 std::int32_t ai_id{},character_type{};
 bool is_player=false,physical_enabled=false,circular=false;
 unsigned selected_controller_count{};
 // Original radius only for enabled circular source bodies; no fake body/userdata.
 float radius_game_units=0;
 std::int32_t group_index{};
 std::uint32_t category_bits{},mask_bits{};
};
bool original_actor_source_is_player(std::int32_t type,const std::string&source_name) noexcept;
bool make_original_actor_body_plan(const AssetCatalog&,const OriginalActorBodyPlanInput&,
                                  OriginalActorBodyPlan&,std::string&);
}
