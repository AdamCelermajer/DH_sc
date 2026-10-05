#include "character_combat_hit_fx_v2.hpp"
namespace dh2::character::skills {
bool character_combat_hit_fx_v2(CharacterWorldRuntimeV1& world,data::EffectsTables::Borrow tables,
 fx::CharacterMeshFxOwnerV2* owner,const data::CombatResult& result,std::uintptr_t target,
 CombatHitFxServicesV1 services,std::string& error){
 error.clear();WorldTargetActorBorrowV1 actor{};
 if(!tables||world.actor(target,&actor)||!actor.character||!actor.character->resolved||!actor.life){error="Required actual target Character blood-effects borrow";return false;}
 std::int32_t id;
 if(result.weapon_category!=-1)id=std::int32_t(std::uint32_t(result.weapon_category)+124u);
 else{
  if(tables.characters().empty()){error="Required CharacterEffects source fallback row0";return false;}
  const auto cached=actor.character->resolved[7];
  const auto index=cached>=0&&std::size_t(cached)<tables.characters().size()?std::size_t(cached):0u;
  const auto& row=tables.characters()[index];id=actor.life->dead?row.blood_death:row.blood;
 }
 // GetTargetPosition and source rotation are evaluated before manager call,
 // even for invalid set IDs. Preserve that prefix and require their producers.
 float rotation[3]{};
 if(!actor.target_node||(*actor.target_node&&!actor.target_enabled)){error="Required source target-position node/enable borrow";return false;}
 const float* position=dh2_world_target_position_v1(actor.position,actor.cached_target_position,
  *actor.target_node,actor.target_enabled?*actor.target_enabled:0);
 if(!position){error="Required source hit target-position backing";return false;}
 if(!services.rotation||services.rotation(services.context,target,rotation)){error="Required source Character hit FX rotation+16c";return false;}
 if(id<0||std::size_t(id)>=tables.sets().size())return true;
 if(!owner){error="Required source VisualFXManager for positive hit set "+std::to_string(id);return false;}
 std::uintptr_t created=0;return owner->play_set(id,position,rotation,0,&created,error);
}
}
