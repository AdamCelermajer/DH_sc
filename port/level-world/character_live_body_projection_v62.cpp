#include "character_live_body_projection_v62.hpp"
#include "loot_source_fields_v47.hpp"
#include <algorithm>
#include <cmath>
namespace dh2::character {
bool character_live_body_projection_v62(RetainedCharacterActorV1& actor,const world::RetainedGameObjectVisualV1& visual,const physical::NpcBodyRequest& q,physical::NpcBodyProjection& out,std::string& error){
 error.clear();if(!actor.object||!actor.source_bounds_ready||!visual.ready()||actor.source_visual()!=reinterpret_cast<std::uintptr_t>(&visual)||!q.properties||dh2_property_validate(q.properties)||q.properties->base!=actor.object->properties->base.data()||q.properties->resolved!=actor.object->properties->resolved.data()||!q.ai||q.ai->rows.size()<=8||q.owner!=reinterpret_cast<void*>(actor.object->identity)||!q.new_physical||q.reserved||q.static_owner>255||q.collision_group_override>1||q.disable_physical>1){error="Required same Character live visual/bounds/property/body projection";return false;}
 physical::NpcBodyProjection candidate{};auto id=q.properties->resolved[1];if(id<0||std::size_t(id)>=q.ai->rows.size())id=8;candidate.ai_id=id;candidate.character_type=q.ai->rows[id].type;
 bool player{};if(!loot_character_is_player_v47(*q.ai,*q.properties,actor.source_name(),player,error))return false;candidate.is_player=player?1u:0u;
 std::array<float,3> scale;if(!actor.source_scale(scale,error))return false;
 std::copy_n(q.properties->base+12,3,candidate.base_scale);candidate.collision_scale=q.properties->resolved[16];candidate.marker=visual.marker().found;
 std::copy_n(visual.mesh_box().data(),6,candidate.visual.mesh_box);
 std::copy_n(actor.runtime.subobjects.local_bounds,6,candidate.bounds.relative_box);std::copy_n(actor.runtime.subobjects.absolute_bounds,6,candidate.bounds.absolute_box);candidate.bounds.flat=actor.source_bounds_flat;
 std::copy(scale.begin(),scale.end(),candidate.visual.effective_scale);std::copy_n(actor.runtime.rotation.rotation,3,candidate.visual.rotation_radians);
 for(float v:candidate.bounds.absolute_box)if(!std::isfinite(v)){error="Nonfinite source Character bounds";return false;}
 physical::CharacterBodyInput body{};body.owner=q.owner;body.new_physical=q.new_physical;body.previous_physical=q.previous_physical;body.character_type=candidate.character_type;body.is_player=player?1u:0u;body.special_owner_byte=q.static_owner;body.collision_group_override=q.collision_group_override;body.disable_physical=q.disable_physical;
 const auto* bounds=candidate.bounds.absolute_box;body.absolute_bounds[0]=bounds[0];body.absolute_bounds[1]=bounds[1];body.absolute_bounds[2]=bounds[3];body.absolute_bounds[3]=bounds[4];std::copy_n(actor.object->position.data(),2,body.position);
 if(dh2_character_body_config(&candidate.body,&body)){error="Source Character body definition failed";return false;}out=candidate;return true;
}
}

