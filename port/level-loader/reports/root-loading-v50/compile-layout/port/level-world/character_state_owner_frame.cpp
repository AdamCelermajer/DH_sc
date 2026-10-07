#include "character_state_owner_frame.hpp"
namespace {
using namespace dh2::character;
// Factory/vtable probe registration-plan.json, indexed by genuine state ID.
constexpr std::uint32_t update_sources[20]={0x3bffe4u,0x3bfff0u,0x3bfff8u,0x3c0e80u,0x3c1184u,0x3c14f8u,0x3c0018u,0x3c0020u,0x3c4788u,0x3c549cu,0x3c0038u,0x3c0040u,0x3c004cu,0x3c0080u,0x3c1a3cu,0x3c0058u,0x3c0064u,0x3c0070u,0x3c0e90u,0x3c0ea0u};
std::uint32_t bounded_source(std::int32_t id){switch(id){case 3:return 0x3c0e80;case 4:return 0x3c1184;case 5:return 0x3c14f8;case 12:return 0x3c004c;default:return 0;}}
bool valid(StateOwnerFrameContext56* c){
 if(!c||!c->machine||!c->facts||!c->bodies||!c->bodies->invoke||!c->outer.invoke||!c->other_updates.invoke)return false;
 const auto m=c->machine;const auto f=m->fsm;const auto facts=c->facts;
 if(!f||!f->state||!f->character||f->reserved||f->current_present>1||m->reserved||m->state_count>20||(!m->states&&m->state_count)||m->current_index < -1||m->current_index>=static_cast<std::int32_t>(m->state_count))return false;
 if((m->current_index>=0)!=bool(f->current_present))return false;
 if(m->current_index>=0&&f->state->current!=m->states[m->current_index].id)return false;
 if(facts->reserved||facts->is_player>1||facts->is_at_destination>1||facts->following_path>1||facts->has_ranged_weapon>1)return false;
 for(std::uint32_t i=0;i<m->state_count;++i){const auto& row=m->states[i];if(row.reserved||row.id<0||row.id>=20||(i&&m->states[i-1].id>=row.id)||row.update!=update_sources[row.id])return false;}
 if(f->current_present&&bounded_source(f->state->current)){const auto s=f->state;if(s->idle_suppressed>255||s->stop_attack_allowed>255||s->heading_active>255||s->controller_locked>255||s->dead_alternate>255||s->body_present>1||s->move_type>2)return false;}
 return true;
}
struct Dispatch {StateOwnerFrameContext56* context;NativeFsm24* fsm;State* state;};
int invoke(void* p,NativeFsm24* f,const NativeFsmRequest32* r,std::uint32_t* out){
 auto& d=*static_cast<Dispatch*>(p);auto c=d.context;
 // The source retains its machine/state receiver across service calls.
 if(!valid(c)||c->machine->fsm!=d.fsm||f!=d.fsm||f->state!=d.state)return 1;
 if(r->service!=fsm_current_update)return c->outer.invoke(c->outer.context,f,r,out);
 auto m=c->machine;
 if(!f->current_present||r->argument0!=static_cast<std::uint32_t>(f->state->current)||r->subject!=f->character||r->argument1||r->force||r->payload)return 1;
 // Capture the actual current method before the synchronous virtual call.
 const auto row=m->states[m->current_index];
 if(bounded_source(row.id))return dh2_character_native_fsm_bounded_tail(f,c->facts,c->bodies)==1?0:1;
 const StateOwnerUpdateRequest24 request{row.update,row.id,f->character,f->state->elapsed_ms,0};
 return c->other_updates.invoke(c->other_updates.context,m,&request);
}
}
extern "C" int dh2_character_state_owner_frame(StateOwnerFrameContext56* c){
 if(!valid(c))return -1;
 Dispatch dispatch{c,c->machine->fsm,c->machine->fsm->state};
 const NativeFsmServices16 services{&dispatch,invoke};
 return dh2_character_native_fsm_update(dispatch.fsm,&services);
}
