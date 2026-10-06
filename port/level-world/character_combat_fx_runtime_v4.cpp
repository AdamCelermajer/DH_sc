#include "character_combat_fx_runtime_v4.hpp"
namespace dh2::character::skills {
CharacterCombatFxRuntimeV4::CharacterCombatFxRuntimeV4(CharacterWorldRuntimeV1& world,data::EffectsTables::Borrow tables,const scene::Scene& scene,DebugSwitches& debug,const DebugFileServices24& files,CombatFxRuntimeServicesV2 services):world_(world),tables_(std::move(tables)),scene_(scene),debug_(debug),files_(files),services_(services){
 modules_=dh2_fx_debug_modules_create(&debug_,&files_);
 if(modules_)manager_=std::make_unique<fx::CharacterMeshFxOwnerV4>(tables_,scene_,services_.assets,fx::MeshFxServicesV1{this,service},services_.particles);
}
CharacterCombatFxRuntimeV4::~CharacterCombatFxRuntimeV4(){manager_.reset();dh2_fx_debug_modules_destroy(modules_);}
bool CharacterCombatFxRuntimeV4::service(void* raw,fx::MeshFxRequestV1& q,std::string& error){
 auto& self=*static_cast<CharacterCombatFxRuntimeV4*>(raw);
 if(q.live_scene!=&self.scene_){error="Combat FX Scene identity mismatch";return false;}
 using O=fx::MeshFxOperationV1;
 if(q.operation==O::debug_load){if(dh2_character_debug_load(&self.debug_,&self.files_)==1)return true;error="Required actual Debug Load for CombatFX";return false;}
 if(q.operation==O::module_enabled){if(dh2_fx_debug_module_get(&q.result,self.modules_,q.text)==1)return true;error="Required actual AnimatedFX Debug module";return false;}
 if(q.operation==O::set_switch||q.operation==O::instance_switch){if(dh2_character_debug_get(&q.result,&self.debug_,q.text,&self.files_)==1)return true;error="Required actual source FX tracing switch";return false;}
 if(self.services_.remaining.invoke)return self.services_.remaining.invoke(self.services_.remaining.context,q,error);
 error="Required source CombatFX anchor/floor service "+std::to_string(unsigned(q.operation));return false;
}
int CharacterCombatFxRuntimeV4::initialize(){error_.clear();if(!manager_){error_="Source CombatFX manager allocation failed";return -1;}return manager_->precache_libraries(error_)?0:-2;}
int CharacterCombatFxRuntimeV4::application(const SkillApplyRequestV6& q,const data::CombatResult& result){
 if(q.service!=skill_apply_hit_fx_v6)return 0;
 if(!manager_){error_="Required retained source CombatFX manager";return -2;}
 return character_combat_hit_fx_v4(world_,tables_,manager_.get(),result,q.target,services_.transform,error_)?1:-2;
}
int CharacterCombatFxRuntimeV4::animation_event(const char* name,std::uintptr_t owner){
 error_.clear();WorldTargetActorBorrowV1 actor{};
 if(!manager_||world_.actor(owner,&actor)||!actor.target_node||(*actor.target_node&&!actor.target_enabled)){error_="Required authored FX event same-world owner position";return -2;}
 const auto* position=dh2_world_target_position_v1(actor.position,actor.cached_target_position,*actor.target_node,actor.target_enabled?*actor.target_enabled:0);
 if(!position){error_="Required authored FX source target-position backing";return -2;}
 return manager_->animation_event(name,position,error_)?0:-2;
}
int CharacterCombatFxRuntimeV4::frame(std::int32_t absolute,std::int32_t dt){
 if(!manager_){error_="Required retained source CombatFX manager";return -2;}
 // Scene timeline sampling precedes VisualFXManager state update/return pool.
 return manager_->scene_frame(absolute,dt,error_)&&manager_->manager_frame(dt,error_)?0:-2;
}
int CharacterCombatFxRuntimeV4::draw_parts(std::vector<skinning::VisualDrawPartV6>& out){
 if(!manager_){error_="Required retained source CombatFX manager";return -2;}
 return manager_->draw_parts(out,error_)?0:-2;
}
}
