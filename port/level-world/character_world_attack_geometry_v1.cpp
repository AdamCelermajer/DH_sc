#include "character_world_attack_geometry_v1.hpp"
#include <cstring>
namespace dh2::character::skills {
struct CharacterWorldAttackGeometryV1::Projection {
 CombatProperties896 properties{};
 CombatInventory16 inventory{};
 CombatEquipSet8 sets[2]{};
 CombatItemInstance4 items[2]{};
 const CombatItemInstance4* refs[2]{};
 std::vector<CombatItemRecord164> rows;
 const CombatItemRecord164* records{};std::uint32_t count{};
};
CharacterWorldAttackGeometryV1::CharacterWorldAttackGeometryV1(
 CharacterWorldRuntimeV1& w,const data::AiTables& a,DebugSwitches& d,
 const DebugFileServices24& f,WorldAttackGeometryServicesV1 s):
 world_(w),ai_(a),debug_(d),files_(f),services_(s){}
bool CharacterWorldAttackGeometryV1::inventory(std::uintptr_t id,Projection& p,std::string& error){
 WorldTargetActorBorrowV1 actor{};
 if(world_.actor(id,&actor)||!actor.character||!actor.character->resolved){error="Required registered Character resolved attack properties";return false;}
 std::memcpy(p.properties.words,actor.character->resolved,sizeof p.properties.words);
 WorldAttackInventoryBorrowV1 b{};
 if(!services_.inventory||services_.inventory(services_.context,id,&b)){
  error="Required same Character source inventory attack borrow";return false;
 }
 if(b.owned){
  if(b.owned->character()!=id||!b.owned->properties()||b.owned->properties()->resolved.data()!=actor.character->resolved){error="Attack inventory/property authority mismatch";return false;}
  const auto& actual=b.owned->equipment();
  for(unsigned i=0;i<2;++i){
   if(actual[i][1]){if(!actual[i][1]->item){error="Actual main-hand item absent";return false;}p.items[i].item_id=actual[i][1]->item->id;p.refs[i]=&p.items[i];}
   p.sets[i].main_hand=p.refs[i]?&p.refs[i]:nullptr;
  }
  p.inventory={p.sets,2,b.owned->current_equipment()};
  const auto& rows=b.owned->table().rows;
  p.rows.resize(rows.size());for(std::size_t i=0;i<rows.size();++i)std::memcpy(p.rows[i].words,rows[i].record.words,sizeof p.rows[i].words);
  p.records=p.rows.data();p.count=std::uint32_t(p.rows.size());
 }else{
  if(!b.projection||b.resolved!=actor.character->resolved){error="Required source-owned NPC inventory projection and same properties";return false;}
  p.inventory=*b.projection;p.records=b.rows;p.count=b.row_count;
 }
 return true;
}
bool CharacterWorldAttackGeometryV1::radius(std::uintptr_t id,float& result,std::string& error){
 Projection p;if(!inventory(id,p,error))return false;
 std::vector<float> radii;radii.reserve(ai_.rows.size());for(const auto& row:ai_.rows)radii.push_back(row.melee_radius);
 if(dh2_attack_melee_radius(&result,&p.properties,&p.inventory,p.records,p.count,radii.data(),std::uint32_t(radii.size()))){error="Malformed actual melee radius projection";return false;}return true;
}
bool CharacterWorldAttackGeometryV1::position(std::uintptr_t id,const float*& result,std::string& error){
 WorldTargetActorBorrowV1 a{};
 if(world_.actor(id,&a)||!a.target_node||!a.position||(*a.target_node&&!a.target_enabled)){
  error="Required source GetTargetPosition fields";return false;
 }
 result=dh2_world_target_position_v1(a.position,a.cached_target_position,*a.target_node,*a.target_node?*a.target_enabled:0);
 if(!result){error="Required selected target-node position cache";return false;}return true;
}
bool CharacterWorldAttackGeometryV1::debug(std::string& error){
 std::uint32_t value{};
 if(dh2_character_debug_load(&debug_,&files_)!=1||dh2_character_debug_get(&value,&debug_,"IsTracingCharAITarget",&files_)!=1){error="Required source melee DebugSwitches query";return false;}
 // Source performs a second, differently cased query when tracing is active.
 // The original diagnostic expression has no gameplay effect.
 if(value&&(dh2_character_debug_load(&debug_,&files_)!=1||dh2_character_debug_get(&value,&debug_,"isTracingCharAITarget",&files_)!=1)){error="Required source melee tracing query";return false;}return true;
}
bool CharacterWorldAttackGeometryV1::query(void* p,std::uintptr_t owner,std::uintptr_t target,WorldAIAttackQueryV1 op,std::int32_t& result,std::string& error){return static_cast<CharacterWorldAttackGeometryV1*>(p)->read(owner,target,op,result,error);}
bool CharacterWorldAttackGeometryV1::read(std::uintptr_t owner,std::uintptr_t target,WorldAIAttackQueryV1 op,std::int32_t& result,std::string& error){
 error.clear();result=0;
 // Original AI_IsInRange resolves its explicit/current target before asking
 // Character.CanRangeAttack. Null target never reaches inventory/property I/O.
 if(op==WorldAIAttackQueryV1::IsInRange&&!target)return true;
 if(op==WorldAIAttackQueryV1::IsEnemy){std::uintptr_t value{};if(world_.relationship(owner,target,true,&value)){error=world_.error();return false;}result=std::int32_t(value);return true;}
 if(op==WorldAIAttackQueryV1::InventoryCanMeleeAttack||op==WorldAIAttackQueryV1::CharacterCanRangeAttack||op==WorldAIAttackQueryV1::IsInRange){
  // Character's cached projectile shortcut reads no inventory at all.
  WorldTargetActorBorrowV1 a{};if(world_.actor(owner,&a)||!a.character||!a.character->resolved){error="Required actual Character attack property borrow";return false;}
  Projection p;std::memcpy(p.properties.words,a.character->resolved,sizeof p.properties.words);
  const bool needs_inventory=op==WorldAIAttackQueryV1::InventoryCanMeleeAttack||p.properties.words[32]==-1;
  if(needs_inventory&&!inventory(owner,p,error))return false;
  if(op==WorldAIAttackQueryV1::InventoryCanMeleeAttack){std::int32_t radius{};result=dh2_attack_equipment_melee_radius(&radius,&p.inventory,p.records,p.count);}
  else{std::int32_t parameters[3]{};result=dh2_attack_range_parameters(parameters,&p.properties,needs_inventory?&p.inventory:nullptr,p.records,p.count);
   if(result>0&&op==WorldAIAttackQueryV1::IsInRange){if(!target){result=0;return true;}const float *x{},*y{};if(!position(owner,x,error)||!position(target,y,error))return false;result=dh2_attack_ranged_distance(x,y,parameters);}}
  if(result<0){error="Malformed actual equipment/range projection";return false;}return true;
 }
 if(op!=WorldAIAttackQueryV1::IsInMeleeRange){error="Unknown attack geometry query";return false;}
 if(!target)return true;
 WorldTargetActorBorrowV1 a{};if(world_.actor(target,&a)){error="Required actual melee target registration";return false;}
 bool character=false;target_providers::Handle16 local{},*shared{};target_providers::Registry24* registry{};
 if(world_.get_handle(target,&local)||world_.handle_borrow(target,&shared,&registry)){error="Required original target handle";return false;}
 std::uintptr_t resolved{};auto query_services=world_.targets().query_services();
 if(target_providers::dh2_target_handle_character(&resolved,&local,shared,registry,&query_services)){error="Required original handle AsCharacter";return false;}character=resolved!=0;
 std::int32_t kind{};
 if(character&&(!services_.object_kind||services_.object_kind(services_.context,target,&kind))){error="Required target ObjectBase source kind word";return false;}
 std::int32_t interaction{};
 if(character&&kind==0){target_search::Request24 q{target_search::interaction_type,0,target,owner};target_search::Response16 response{};auto services=world_.targets().search_services();if(services.invoke(services.context,&q,&response)){error=world_.targets().error();return false;}interaction=std::int32_t(response.word);}
 if(!character||kind!=0||interaction!=8){
  if(!services_.interaction_range||services_.interaction_range(services_.context,owner,target,&result)){error="Required source AI_IsInInteractionRange continuation";return false;}return true;
 }
 const float *x{},*y{};float radii[2]{};
 if(!position(owner,x,error)||!position(target,y,error)||!radius(owner,radii[0],error)||!radius(target,radii[1],error)||!debug(error))return false;
 result=dh2_attack_melee_distance(x,y,radii);
 if(result<0){error="Malformed selected source melee position span";return false;}return true;
}
}
