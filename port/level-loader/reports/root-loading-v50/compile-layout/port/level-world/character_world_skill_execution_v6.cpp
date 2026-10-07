#include "character_world_skill_execution_v6.hpp"
#include <cstdio>
namespace dh2::character::skills {
CharacterWorldSkillExecutionV6::CharacterWorldSkillExecutionV6(CharacterWorldRuntimeV1&w,const data::AiTables&ai,CharacterSkillNativeReadOnlyBindingsV6&previous,const data::FreshInventoryOwnedV4&gear,NativeFsm24&fsm,data::SkillTables::Borrow tables,DotCombatContext32&cf,data::CombatRandom&rng,DebugSwitches&debug,const DebugFileServices24&files,trophies::TrophyNativeBindingsV1*trophy,WorldSkillCombatBackendsV6 backends):world_(w),switches_(debug),debug_files_(files),trophies_(trophy),backends_(backends),debug_{this,debug_service}{
 auto wrapped=backends_;wrapped.hit={this,hit_service};wrapped.application={this,application_service,&debug_};
 combat_=std::make_unique<CharacterWorldSkillCombatV6>(world_,ai,debug_,wrapped);
 native_=std::make_unique<CharacterSkillNativeBindingsV6>(previous,gear,fsm,std::move(tables),combat_->native_world(),cf,rng,debug_,combat_->application_services());
}
int CharacterWorldSkillExecutionV6::debug_service(void*p,const SkillAttackNativeRequestV6*q,std::uintptr_t*out){if(!q||!out)return -1;auto&w=*static_cast<CharacterWorldSkillExecutionV6*>(p);*out=0;
 if(q->service==skill_attack_debug_load_v6)return dh2_character_debug_load(&w.switches_,&w.debug_files_)==1?0:-1;
 if(q->service==skill_attack_string_construct_v6){if(!q->name)return -1;*out=++w.string_serial_;w.strings_[*out]=q->name;return 0;}
 auto token=w.strings_.find(q->subject);if(token==w.strings_.end())return -1;
 if(q->service==skill_attack_debug_get_v6){std::uint32_t value;if(dh2_character_debug_get(&value,&w.switches_,token->second.c_str(),&w.debug_files_)!=1)return -1;*out=value;return 0;}
 if(q->service==skill_attack_string_destroy_v6){w.strings_.erase(token);return 0;}return -1;
}
int CharacterWorldSkillExecutionV6::hit_service(void*p,HitActor32*a,const HitRequest32*q,std::uintptr_t*out){auto&w=*static_cast<CharacterWorldSkillExecutionV6*>(p);if(!q||!out)return -1;
 if(q->service==hit_trophy_manager_v6||q->service==hit_trophy_index_v6||q->service==hit_unlock_v6){if(!w.trophies_){w.error_="Required same TrophyManager owner";return -1;}return w.trophies_->hit(q,out);}
 if(!w.backends_.hit.invoke){w.error_="Required Hit provider "+std::to_string(q->service);return -1;}return w.backends_.hit.invoke(w.backends_.hit.context,a,q,out);
}
int CharacterWorldSkillExecutionV6::application_service(void*p,const SkillApplyRequestV6*q,SkillApplyResponseV6*out,data::CombatResult*result){auto&w=*static_cast<CharacterWorldSkillExecutionV6*>(p);if(!q||!out)return -1;const auto status=w.backends_.application.invoke?w.backends_.application.invoke(w.backends_.application.context,q,out,result):-1;if(status)w.error_="Required SkillApply provider "+std::to_string(q->service);return status;}
int CharacterWorldSkillExecutionV6::refresh_entry(void*p,WorldSkillCombatBorrowV6*out){auto&e=*static_cast<Entry*>(p);auto&r=e.registration;if(!out||!r.refresh||r.refresh(r.context,&e.borrow))return -1;auto&b=e.borrow;
 if(b.identity!=r.identity||!b.properties||!b.life||!b.machine||!b.fields||!b.facts||b.facts->properties!=b.properties->resolved||b.facts->state!=b.machine->current||b.facts->combo_hits!=b.fields->combo)return -1;
 if(b.inventory&&(b.inventory->character()!=b.identity||b.inventory->properties()->resolved.data()!=b.properties->resolved))return -1;
 e.attack={b.identity,b.properties,b.facts};e.lifecycle_before=b.life->lifecycle;e.hit={b.identity,b.properties,b.controller?*b.controller:0,b.life->lifecycle,0};
 e.application={b.identity,b.properties,b.buffs,b.controller?&e.hit:nullptr,nullptr,nullptr,b.dot_ids,b.dot_fx_ids,&b.fields->combo,&b.fields->invulnerable,&b.fields->push_death,&b.fields->network_id};
 *out={&e.attack,&e.application,b.life,b.inventory,b.outgoing,b.incoming};return 0;
}
int CharacterWorldSkillExecutionV6::add(const WorldSkillExecutionRegistrationV6&r){if(!r.identity||!r.refresh)return -1;for(const auto&e:actors_)if(e.registration.identity==r.identity)return -1;actors_.push_back({r});auto&e=actors_.back();const auto status=combat_->add({r.identity,&e,refresh_entry});if(status){actors_.pop_back();error_="Required same-world source combat descriptor: "+combat_->error();}return status;}
int CharacterWorldSkillExecutionV6::remove(std::uintptr_t id){for(auto i=actors_.begin();i!=actors_.end();++i)if(i->registration.identity==id){auto status=combat_->remove(id);if(status)return status;actors_.erase(i);return 0;}return -1;}
int CharacterWorldSkillExecutionV6::attach(CharacterScriptSessionV3&s){auto status=native_->attach(s);if(status)error_=native_->error();return status;}
int CharacterWorldSkillExecutionV6::binding(void*p,std::uint32_t address,dh2_script_function*function,void**context){if(!p||!function||!context)return -1;auto&w=*static_cast<CharacterWorldSkillExecutionV6*>(p);dh2_script_function f=nullptr;void*c=nullptr;auto status=CharacterSkillNativeBindingsV6::binding(w.native_.get(),address,&f,&c);if(status!=1)return status;if(address!=0x3b9fbc){*function=f;*context=c;return 1;}auto&b=w.bindings_[address];b={f,c,&w};*function=invoke;*context=&b;return 1;}
void CharacterWorldSkillExecutionV6::publish_compatibility_fields()noexcept{for(auto&e:actors_){auto&b=e.borrow;if(!b.fields||!b.life)continue;b.life->combo_hits=b.fields->combo;b.life->push_death=b.fields->push_death;if(b.controller&&e.hit.lifecycle!=e.lifecycle_before){b.life->lifecycle=e.hit.lifecycle;e.lifecycle_before=e.hit.lifecycle;}}}
int CharacterWorldSkillExecutionV6::invoke(void*p,const dh2_script_value*a,std::uint32_t n,dh2_script_value*out,std::uint32_t cap,std::uint32_t*written,char*error,std::size_t size){auto&b=*static_cast<Binding*>(p);auto&w=*b.owner;w.error_.clear();auto status=b.function(b.context,a,n,out,cap,written,error,size);w.publish_compatibility_fields();if(status){w.error_="Required source SkillV6 execution: "+w.error_+"; "+w.native_->error()+"; "+w.combat_->error();if(error&&size)std::snprintf(error,size,"%s",w.error_.c_str());}return status;}
}
