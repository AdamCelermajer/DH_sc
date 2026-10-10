#include "character_world_npc_state_owner_v1.hpp"
#include <cstring>
namespace dh2::character {
extern "C" int dh2_character_constructor_combat_fields_v1(CharacterConstructorCombatFieldsV1* out){if(!out)return -1;*out={};return 0;}
extern "C" int dh2_world_combat_context_v1(DotCombatContext32* out,std::uintptr_t a,std::uintptr_t b,std::int32_t al,std::int32_t bl,std::int32_t element,std::uint8_t off,std::uint8_t magic){if(!out||!a||!b)return -1;auto raw=std::uint32_t(al)-std::uint32_t(bl);std::int32_t delta,inverse;std::memcpy(&delta,&raw,4);raw=0u-raw;std::memcpy(&inverse,&raw,4);*out={a,b,delta,inverse,element,off,magic,0,0};return 0;}
CharacterWorldNpcStateOwnerV1::CharacterWorldNpcStateOwnerV1(std::uintptr_t id,WorldNpcStateServicesV1 services):owner_(id),services_(services){
 extensions_={services_.pre_spawn,services_.spawn_services,services_.remaining_methods,services_.remaining_updates};
 behavior_={services_.facts,services_.bodies,services_.predicates,{&extensions_,dh2_character_state_owner_extensions_method}};
 if(dh2_character_state_owner_behavior_bind(&bound_methods_,&behavior_)!=1)error_="Required actual NPC state body/fact borrows";
 methods_={this,method};
 frame_={&owner_.machine(),services_.facts,services_.bodies,services_.outer,{&extensions_,dh2_character_state_owner_extensions_update}};
}
int CharacterWorldNpcStateOwnerV1::method(void* p,StateOwnerMachine40* machine,const StateOwnerRequest48* q,StateOwnerResponse8* out){
 auto& self=*static_cast<CharacterWorldNpcStateOwnerV1*>(p);if(machine!=&self.owner_.machine()||!q||!out||!self.bound_methods_.invoke)return -1;
 if(q->operation==state_owner_profile_begin||q->operation==state_owner_profile_end){if(self.services_.diagnostics)return self.services_.diagnostics->invoke(*q);if(self.services_.diagnostics_required)return -1;}
 if(q->state==3&&((q->operation==state_owner_focus&&q->source_function==0x3c3020)||(q->operation==state_owner_blur&&q->source_function==0x3c2d3c))){if(self.services_.diagnostics){if(self.services_.diagnostics->idle_behavior(q->source_function))return -1;}else if(self.services_.diagnostics_required)return -1;}
 if(q->state==17&&((q->operation==state_owner_focus&&q->source_function==0x3c688c)||(q->operation==state_owner_blur&&q->source_function==0x3c67d4))){if(self.services_.diagnostics){if(self.services_.diagnostics->pre_spawn_behavior(q->source_function))return -1;}else if(self.services_.diagnostics_required)return -1;}
 if(self.services_.prepare_method&&self.services_.prepare_method(self.services_.prepare_context,*q))return -1;
 return self.bound_methods_.invoke(self.bound_methods_.context,machine,q,out);
}
bool CharacterWorldNpcStateOwnerV1::coherent()noexcept{return services_.facts&&services_.bodies&&services_.bodies->invoke&&bound_methods_.invoke&&services_.diagnostics_required<=1&&owner_.native_fsm().character&&owner_.native_fsm().state==&owner_.state();}
int CharacterWorldNpcStateOwnerV1::completed(int status,const char* operation){if(status<0)error_=std::string("Required source NPC ")+operation+" continuation";else error_.clear();return status;}
int CharacterWorldNpcStateOwnerV1::initialize_level(const char* preset){if(!coherent()||!preset)return completed(-1,"level preset/body");std::int32_t id=0;if(dh2_character_native_fsm_preset_state(&id,preset)!=1)return completed(-1,"preset");return completed(owner_.initialize_level(id,methods_),"level init");}
int CharacterWorldNpcStateOwnerV1::initialize_level_preset_v95(std::int32_t preset){if(!coherent())return completed(-1,"level preset/body");return completed(owner_.initialize_level(preset,methods_),"level init");}
int CharacterWorldNpcStateOwnerV1::transition(std::int32_t next,std::int32_t event,std::uintptr_t payload){if(!coherent())return completed(-1,"state binding");return completed(owner_.transition(next,event,payload,methods_),"transition");}
int CharacterWorldNpcStateOwnerV1::event(std::int32_t event,std::uintptr_t payload){if(!coherent())return completed(-1,"state binding");return completed(owner_.event(event,payload,methods_),"event");}
int CharacterWorldNpcStateOwnerV1::update(){if(!coherent()||(services_.prepare_update&&services_.prepare_update(services_.prepare_context)))return completed(-1,"frame binding");return completed(dh2_character_state_owner_frame(&frame_),"frame");}
}
