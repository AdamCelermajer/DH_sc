#pragma once
#include "character_scene.hpp"
#include "character_body_config.hpp"
#include "../game-data/ai.hpp"
#include "../game-data/properties.hpp"
#include <memory>

namespace dh2::physical {
struct NpcBodyRequest {
 data::PropertyView* properties=nullptr;
 const data::AiTables* ai=nullptr;
 void* owner=nullptr;
 void* new_physical=nullptr;
 void* previous_physical=nullptr;
 float position[3]{},rotation_degrees[3]{};
 // Original ObjectBase 'static' byte and genuine caller debug predicates.
 // DACT scale is deliberately absent: InitPost overwrites owner visual scale
 // from base Scale_X/Y/Z before the GameObject clamp.
 std::uint32_t static_owner=0,collision_group_override=0,disable_physical=0;
 std::uint32_t previous_flat=0,reserved=0;
};
struct NpcBodyProjection {
 std::int32_t ai_id=0,character_type=0,base_scale[3]{},collision_scale=0;
 std::uint32_t is_player=0,marker=0;
 DecorSceneOutput visual{};
 CharacterOwnerBounds bounds{};
 CharacterBodyConfig body{};
};
// Owned BRES/model-frame pose projection. This does not create the original
// AssetManager/equipment/save lifecycle. A caller-selected cached complete
// Scene is copied verbatim; nullptr explicitly selects the decoded factory
// scene. Outer owner TRS is applied only by project, never to joint matrices.
class CharacterNpcBodyModel {
 struct Snapshot;
 std::unique_ptr<Snapshot> snapshot_;
public:
 CharacterNpcBodyModel();
 ~CharacterNpcBodyModel();
 CharacterNpcBodyModel(const CharacterNpcBodyModel&)=delete;
 CharacterNpcBodyModel& operator=(const CharacterNpcBodyModel&)=delete;
 CharacterNpcBodyModel(CharacterNpcBodyModel&&)=delete;
 CharacterNpcBodyModel& operator=(CharacterNpcBodyModel&&)=delete;
 bool initialize(data::Bytes,const scene::Scene* cached_complete_scene,std::string& error);
 bool ready() const;
 const scene::Scene* complete_scene() const;
 const DecorSceneMarker* marker() const;
 const std::vector<CharacterMeshEntry>* entries() const;
 // Bounded to original AI type4 (all four Crypt kinds). Other type/name
 // IsPlayer branches remain separate source providers. Output is atomic on
 // failure. Caller must supply the already-produced resolved sheet: the
 // original GetCharAIId/SetRelativeAABB read cached words without recalculation.
 bool project(const NpcBodyRequest&,NpcBodyProjection&,std::string& error) const;
};
}
