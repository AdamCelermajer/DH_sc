// Included inside model_renderer's anonymous namespace after attack_inventory.
// Required headers: character_combat_follower_v1.hpp,
// combat_flash_inputs_v1.hpp, player_equipment_queries_v1.hpp.
namespace renderer_combat_text_detail_v1 {
using namespace dh2::character::skills;
int actor(void* raw,std::uintptr_t id,WorldTargetActorBorrowV1& out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 return t.targets?t.targets->actor(id,&out):-1;
}
int follower(void* raw,std::uintptr_t id,bool* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);WorldTargetActorBorrowV1 a{};
 if(actor(raw,id,a)||!t.world||!t.world->tables.ai())return -1;
 return character_combat_follower_v1(out,a,*t.world->tables.ai())==1?0:-1;
}
int position(void* raw,std::uintptr_t id,float out[3]){
 WorldTargetActorBorrowV1 a{};if(!out||actor(raw,id,a)||!a.position)return -1;
 // Character::GetPosition, not AI's GetTargetPosition/cache.
 std::copy(a.position,a.position+3,out);return 0;
}
int height(void* raw,std::uintptr_t id,float* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);auto it=t.world_actors.find(id);
 if(!out||it==t.world_actors.end()||!it->second.object)return -1;
 const float* box=nullptr;
 if(id==t.fsm.character)box=prince_runtime.subobjects.local_bounds;
 else if(it->second.npc&&it->second.npc->animation&&it->second.npc->source_bounds_ready)box=it->second.npc->runtime.subobjects.local_bounds;
 if(!box)return -1;*out=box[5]-box[2];return 0;
}
int property(void* raw,std::uintptr_t id,int index,std::int32_t* out){
 WorldTargetActorBorrowV1 a{};
 if(!out||index<0||index>=224||actor(raw,id,a)||!a.character||!a.character->resolved)return -1;
 *out=a.character->resolved[index];return 0;
}
int dual(void* raw,std::uintptr_t id,bool* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);if(!out)return -1;
 WorldAttackInventoryBorrowV1 b{};
 if(PlayerSkillsRuntime::attack_inventory(raw,id,&b))return -1;
 if(b.owned){
  if(id==t.fsm.character&&(!player_equipment||b.owned!=player_equipment->inventory()))return -1;
  WorldTargetActorBorrowV1 a{};
  if(actor(raw,id,a)||!a.character||!a.character->resolved)return -1;
  dh2::player::EquipmentQueries12V1 q{};
  if(!dh2::player::owned_equipment_queries_v4(q,*b.owned,id,a.character->resolved,t.error))return -1;
  *out=(q.flags&dh2::player::query_dual)!=0;return 0;
 }
 // This exact owner represents the source NPC constructor's two empty sets.
 // Reject a changed projection instead of assuming every NPC is non-dual.
 if(!b.projection||!b.resolved||b.projection->count!=2||!b.projection->sets||
    b.projection->current_set<0||b.projection->current_set>=2)return -1;
 for(unsigned i=0;i<2;++i)if(b.projection->sets[i].main_hand)return -1;
 dh2::player::EquipmentQueries12V1 q{};
 if(dh2_equipment_queries_v1(&q,nullptr,nullptr,b.resolved[203]))return -1;
 *out=(q.flags&dh2::player::query_dual)!=0;return 0;
}
int player(void* raw,std::uintptr_t id,bool* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);if(!out||!t.targets)return -1;
 auto s=t.targets->targets().query_services();std::uintptr_t value{};
 const dh2::target_providers::Request24 q{dh2::target_providers::virtual_player,0,id,0};
 if(!s.invoke||s.invoke(s.context,&q,&value))return -1;
 *out=value!=0;return 0;
}
int constant(void* raw,const char* group,const char* key,std::int32_t* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 const auto* d=t.world?t.world->tables.design():nullptr;
 return d&&d->lookup?d->lookup(d->context,0,group,key,out):-1;
}
int localized(void*,std::int32_t id,const char** out){
 return combat_text_sink.localized?combat_text_sink.localized(combat_text_sink.context,id,out):-1;
}
int enqueue(void*,const CombatTextRequestV1* request){
 return combat_text_sink.enqueue?combat_text_sink.enqueue(combat_text_sink.context,request):-1;
}
}
void apply_combat_text_v1(const dh2::data::CombatResult& result,
 std::uintptr_t attacker,std::uintptr_t target){
 using namespace renderer_combat_text_detail_v1;
 if(!player_skills_runtime)throw std::runtime_error("Required retained World combat text owner");
 auto& t=*player_skills_runtime;
 const CombatTextServicesV1 services{&t,follower,renderer_combat_text_detail_v1::position,height,property,dual,renderer_combat_text_detail_v1::player,constant,localized,enqueue};
 const int status=character_combat_text_v1(result,attacker,target,services);
 if(status!=1)throw std::runtime_error("Required original scrolling combat text provider ("+
  std::to_string(status)+")"+(t.error.empty()?"":"; "+t.error));
}
bool fill_combat_text_frame_v1(CombatTextFrameV1& out,std::string& error){
 if(!player_skills_runtime||!player_skills_runtime->world){error="Required retained World Flash inputs";return false;}
 auto& w=*player_skills_runtime->world;bool disabled{};
 const int status=dh2::ui::combat_flash_debug_gate_v1(&disabled,w.debug.get(),&w.debug_files);
 if(status!=1){error="Required original Flash Debug gate ("+std::to_string(status)+")";return false;}
 out={current_application_dt,w.source_level_load_phase,disabled,current_application_tick};return true;
}
