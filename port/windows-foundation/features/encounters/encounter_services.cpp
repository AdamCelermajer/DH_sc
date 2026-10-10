#include "encounter_services.hpp"
#include <stdexcept>
namespace dh::foundation::encounters {
Admission::Admission(std::shared_ptr<dh2::world::NativeConditionRuntimeV69>c,AdmissionServices s):conditions_(std::move(c)),services_(std::move(s)){if(!conditions_||!services_.owner)throw std::invalid_argument("Encounter admission requires actual SAME condition arena and transport owner");}
bool Admission::initialize(dh2::world::CanonicalGameObjectBaseOwnerV1&base,unsigned offset,std::string&e){if(offset!=0x8c&&offset!=0xb0){e="Encounter condition offset outside original embedded objects";return false;}const auto*compiled=base.pointer(offset+0x1c);if(!compiled||*compiled){e="Encounter condition initialization requires actual uninitialized source cell; existing arena must be reused";return false;}return dh2::world::condition_data_init_v3(base,offset,conditions_->condition_data_services(),e);}
bool Admission::evaluate(dh2::world::CanonicalGameObjectBaseOwnerV1&base,bool disable,bool mark,bool&enabled,std::string&e){
 struct Context{Admission*self;dh2::world::CanonicalGameObjectBaseOwnerV1*base;} context{this,&base};
 dh2::world::ObjectEnableConditionBorrowV2 fields;fields.enabled8a=base.byte(0x8a);fields.minimum_difficulty_ec=base.integer(0xec);fields.disable_f1=base.byte(0xf1);fields.condition_a8=base.pointer(disable?0xcc:0xa8);fields.tested_ac=base.byte(disable?0xd0:0xac);
 dh2::world::ObjectEnableConditionServicesV2 services;services.context=&context;
 services.local_profile=[](void*p,bool&character,bool&save,std::uint8_t&sync,std::string&e){auto&self=*static_cast<Context*>(p)->self;if(!self.services_.local_save){e="Required actual local Character/Save quest-sync14 provider";return false;}return self.services_.local_save(character,save,sync,e);};
 services.current_level=[](void*p,bool&present,std::int32_t&difficulty,std::string&e){auto&self=*static_cast<Context*>(p)->self;if(!self.services_.current_level){e="Required actual current Level difficulty118 provider";return false;}return self.services_.current_level(present,difficulty,e);};
 services.condition_is_true=[](void*p,std::uintptr_t id,bool&truth,std::string&e){return static_cast<Context*>(p)->self->conditions_->evaluate(id,truth,e);};
 services.enabled_event=[](void*p,bool value,std::string&e){auto&c=*static_cast<Context*>(p);if(!c.self->services_.enabled_event){e="Required actual selected enabled virtual44/48 effect";return false;}return c.self->services_.enabled_event(*c.base,value,e);};
 return disable?dh2::world::object_test_disable_condition_v103(fields,services,mark,enabled,e):dh2::world::object_test_enable_condition_v2(fields,services,mark,enabled,e);
}
bool Admission::test_enable(dh2::world::CanonicalGameObjectBaseOwnerV1&base,bool mark,bool&enabled,std::string&e){return evaluate(base,false,mark,enabled,e);}
bool Admission::test_disable(dh2::world::CanonicalGameObjectBaseOwnerV1&base,bool mark,bool&enabled,std::string&e){return evaluate(base,true,mark,enabled,e);}
CampaignBinding::CampaignBinding(OriginalCampaignRuntime&r,OriginalCampaignWorldAdapter&w,CampaignSourceServices s):runtime_(&r),world_(&w),source_(std::move(s)){}
int CampaignBinding::common_count()const{int count=0;for(const auto&s:runtime_->scripts())if(s.scope=="common")++count;return count;}
bool CampaignBinding::global_from_level(int local,int&global,std::string&e)const{if(local<0){global=-1;e.clear();return true;}global=local+common_count();if(global<0||global>=int(runtime_->scripts().size())||runtime_->scripts()[global].scope!="level"){e="Source level script index outside original campaign array";return false;}e.clear();return true;}
bool CampaignBinding::bind(std::string&e){if(bound_||!source_.owner||!source_.admit_start||!source_.script_flags||!source_.command_skip){e="Encounter campaign requires actual start/flags/skip source providers and fresh binding";return false;}
 OriginalCampaignServices services;services.admit_start=source_.admit_start;services.random=source_.random;services.command=[this](auto phase,const auto&command,int module,bool&blocking,std::string&e){bool skip=false;if(!source_.command_skip(command,skip,e))return false;return world_->command(phase,command,module,skip,blocking,e);};runtime_->bind(std::move(services));world_->bind_runtime(*runtime_);bound_=true;e.clear();return true;}
bool CampaignBinding::trigger_script_services(dh2::world::TriggerZoneServicesV22&services,std::string&e){if(!bound_){e="Encounter campaign binding uninitialized";return false;}
 services.script_id=[this](const char*name,bool common,int&id,std::string&e){if(!name||common){e="Source Trigger script lookup requires exact noncommon name";return false;}const int global=runtime_->script_id(name,false);id=global<0?-1:global-common_count();e.clear();return true;};
 services.script_flags=[this](int local,std::uint8_t&flags,std::string&e){int global;if(!global_from_level(local,global,e))return false;if(global<0){e="Source flags queried for missing script";return false;}return source_.script_flags(global,flags,e);};
 services.script_running=[this](int local,bool&running,std::string&e){int global;if(!global_from_level(local,global,e))return false;running=global>=0&&runtime_->running(global);e.clear();return true;};
 services.start_script=[this](int local,int module,std::string&e){int global;if(!global_from_level(local,global,e))return false;if(global<0){e.clear();return true;}return runtime_->start(global,module,false,e);};e.clear();return true;
}
bool CampaignBinding::tick(std::int32_t dt,std::string&e){if(!bound_){e="Encounter campaign binding uninitialized";return false;}return runtime_->tick(dt,e);}
}
