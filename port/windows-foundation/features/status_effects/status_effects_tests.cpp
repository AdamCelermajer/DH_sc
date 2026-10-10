#include "status_effects.hpp"
#include "status_live_binding.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation::status;
using namespace dh2::character;
static void check(bool v,const char* msg){if(!v)throw std::runtime_error(msg);}
static std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),"source ClassDict missing");return {std::istreambuf_iterator<char>(f),{}};}
struct Host {
 dh::foundation::ActorState actor;
 dh2::data::PropertyRules rules;
 dh2::data::PropertyState state;
 dh2::data::PropertyView properties;
 Timer32 slots[16]{};TimerStore32 timers{slots,0,16,0x123456789,0,0};
 TimerServices32 clock{this,expiry,nullptr,0};BuffServices16 services{this,service};
 BuffOwner* owner{};StatusEffects* adapter{};int expiries=0,reentries=0,releases=0;
 bool probe=false,fail=false;
 Host(){actor.id=1;properties=dh2::data::property_view(rules,state);BuffBindings32 b{&properties,&timers,&clock,&services};owner=dh2_character_buffs_create(&b);check(owner,"same native owner creation");}
 ~Host(){dh2_character_timers_stop_all(&timers);dh2_character_buffs_destroy(owner);}
 static int service(void* p,dh2::data::PropertyView* view,const BuffRequest32* q,std::uintptr_t*){
  auto& h=*static_cast<Host*>(p);check(view==&h.properties&&q->character==h.timers.owner,"same native service owner");
  if(h.probe){BuffResult24 r{};check(h.adapter->remove_all(r)==-1,"adapter rejects service reentry");++h.reentries;}
  if(h.fail)return 0;
  if(q->service==buff_recalculate){for(int i=0;i<224;++i){std::int32_t value;check(!dh2_property_resolve(view,i,&value),"genuine property resolve");view->resolved[i]=value;}return 1;}
  if(q->service==buff_fx_release){++h.releases;return 1;}
  return 0; // Actual visual creation is required; never fake loaded FX.
 }
 static void expiry(void* p,std::uintptr_t owner,std::int32_t event,Timer32* t){auto& h=*static_cast<Host*>(p);check(owner==h.timers.owner&&event==54,"original timer54 envelope");BuffResult24 r{};check(h.adapter->expired(r,t)==1,"source expiry removes buff");++h.expiries;}
};
// Explicit boundary fixtures, not a live engine provider. The tests execute
// real DoT calculation/application kernels and stop at absent full HitFor.
struct TickFixture {
 TimerEffectState32 state{};DotActor32 actor{};DotCombatContext32 combat{};
 TimerEffectServices16 queries{this,query};DotServices16 dots{this,dot};
 int outer_debug=0,inner_debug=0,hit_attempts=0,continuations=0;
 static int query(void* p,TimerEffectState32*,const TimerEffectRequest40* q,std::int32_t* word,dh2::data::CombatResult*){
  auto& f=*static_cast<TickFixture*>(p);*word=0;
  if(q->service==effect_debug_query){check(std::string(q->name)=="isTracingChar_Attack","source outer DOT debug name");++f.outer_debug;}
  return 0;
 }
 static int dot(void* p,DotActor32* a,const DotRequest40* q,DotResponse8* r,dh2::data::CombatResult* result){
  auto& f=*static_cast<TickFixture*>(p);check(a==&f.actor&&q->subject==f.state.owner,"same actor for source DoT services");*r={};
  if(q->service==dot_debug_query)++f.inner_debug;
  if(q->service==dot_hit_for){check(q->target==a->identity&&result->mask==0x20080000u,"genuine self/self direct DOT result");++f.hit_attempts;return 1;}
  return 0;
 }
 static int continuation(void* p,const dh2::character::skills::SkillApplyRequestV6*,dh2::character::skills::SkillApplyResponseV6*,dh2::data::CombatResult*){
  ++static_cast<TickFixture*>(p)->continuations;return -7;
 }
};
int main(int argc,char** argv){try{
 const std::string dir=argc>1?argv[1]:".local-inputs/windows-shared-assets/original-cache/data/pydata";
 auto records=read(dir+"/character_classes_pyarray.bin"),names=read(dir+"/character_classes_pyarraynames.bin"),schema=read(dir+"/character_classes_pystructnames.bin");
 dh2::data::ClassTables tables;std::string error;check(dh2::data::load_classes({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},tables,error),error.c_str());
 auto find=[&](const char* n){auto it=std::find(tables.names.begin(),tables.names.end(),n);check(it!=tables.names.end(),"real authored buff name");return std::int32_t(it-tables.names.begin());};
 auto id=find("Buff_Speed");auto slow=find("Debuff_Slow");auto dotid=find("AUTO_DOT_01_FIRE");
 Host h;Borrow b{&h.actor,h.timers.owner,&h.properties,h.owner,&h.timers};StatusEffects adapter(b);h.adapter=&adapter;
 check(adapter.valid(error),"live borrow valid");BuffResult24 r{};
 check(adapter.add(r,id,100,1,2,-1,"actual_speed")==1&&r.instance,"native timed buff");auto first=r.instance;
 check(adapter.add(r,id,200,1,3,-1,"actual_speed")==1&&r.instance==first,"stronger duplicate refresh keeps source instance");
 check(dh2_character_buffs_count(h.owner)==1,"duplicate capacity one");
 check(dh2_character_timers_update(&h.timers,199,0,&h.clock)==1&&h.expiries==0,"genuine source duration boundary");
 check(dh2_character_timers_update(&h.timers,1,0,&h.clock)==1&&h.expiries==1&&dh2_character_buffs_count(h.owner)==0,"genuine timer expiry cleanup");
 check(adapter.expired(r,&h.slots[0])==-1,"duplicate expiry has no stale buff dereference");
 Timer32 foreign{};check(adapter.expired(r,&foreign)==-1,"foreign expiry rejected");
 check(adapter.add(r,slow,500,1,0,-1,"debuff_slow")==1,"real slow class ID add");
 h.actor.action=dh::foundation::CharacterAction::idle;check(dh2_character_buffs_count(h.owner)==1,"interrupt leaves duration owned by source timer");
 h.probe=true;check(adapter.remove_all(r)==1&&h.reentries>0,"death cleanup rejects callback reentry");h.probe=false;
 check(dh2_character_buffs_count(h.owner)==0&&!h.slots[0].active,"death cleanup stops actual buff timer");
 check(adapter.remove_all(r)==1,"end cleanup repeat source behavior");
 check(adapter.dot(r,100,4,0)==-2,"missing actual FX dictionary fails");
 TimerEffectResult24 tick{};check(adapter.tick(tick,34)==-2,"missing full CalculateResult/FApplyResult fails");
 dh2::character::skills::SkillApplyRequestV6 q{};dh2::character::skills::SkillApplyResponseV6 out{};
 q.service=dh2::character::skills::skill_apply_stun_v6;q.subject=q.target=h.timers.owner;
 check(adapter.application(q,out)==-2,"missing original FSM services fails");
 q.service=999;check(adapter.application(q,out)==0,"unknown service never accepted");
 h.fail=true;check(adapter.remove_all(r)==-2,"reached property service failure explicit");h.fail=false;
 auto wrong=b;wrong.character++;StatusEffects invalid(wrong);check(!invalid.valid(error),"foreign timer character rejected");
 TickFixture fixture;fixture.state={h.timers.owner,&h.properties,0,0};fixture.actor={h.timers.owner,&h.properties,-1,0,0,0,0};
 DotTickBorrow ticks{&fixture.state,&fixture.actor,&fixture.combat,&fixture.dots,&fixture.queries};DotTickBinding live(ticks);
 check(live.valid(error),"same retained DoT binding valid");
 auto tickborrow=b;tickborrow.ticking=&fixture.state;tickborrow.tick_services=&live.services();StatusEffects ticking(tickborrow);
 h.state.resolved[126]=256;
 check(ticking.tick(tick,0x34)==-2&&fixture.hit_attempts==1&&fixture.actor.combo_hits==1,"real DoT kernel reaches missing full HitFor, retains source prefix");
 check(fixture.outer_debug==1&&fixture.inner_debug>=1&&fixture.combat.attacker==h.timers.owner&&fixture.combat.defender==h.timers.owner,"no duplicate outer debug; same shared combat owner");
 check(live.source_status()==-2&&!live.error().empty(),"explicit full-apply diagnostic");
 auto foreign_tick=ticks;DotActor32 other=fixture.actor;other.identity++;foreign_tick.actor=&other;DotTickBinding rejected(foreign_tick);check(!rejected.valid(error),"foreign DoT actor rejected");
 check(dispatch_status_timer(ticking,h.timers.owner,h.timers.owner+1,54,&h.slots[0],r,tick,error)==-1,"foreign timer envelope rejected before mutation");
 check(dispatch_status_timer(ticking,h.timers.owner,h.timers.owner,99,nullptr,r,tick,error)==0,"unrelated timer service unhandled");
 using namespace dh2::character::skills;
 SkillApplyServicesV6 tail{&fixture,TickFixture::continuation,nullptr};SkillStatusBorrow application{&adapter,&tail};
 q.service=skill_apply_sound_v6;check(dispatch_skill_status(application,&q,&out,nullptr,error)==-7&&fixture.continuations==1,"full application sound continuation failure preserved");
 q.service=skill_apply_stun_v6;check(dispatch_skill_status(application,&q,&out,nullptr,error)==-2&&fixture.continuations==1,"required native FSM cannot fall through to fake acceptance");
 application.continuation=nullptr;q.service=skill_apply_sound_v6;check(dispatch_skill_status(application,&q,&out,nullptr,error)==-2,"missing application tail fails");
 StatusTimerDelivery delivery(h.timers);check(delivery.bind(ticking,error),"source timer callback publication");
 check(!delivery.bind(ticking,error),"timer callback publication once");
 auto timerid=dh2_character_timer_start(&h.timers,1,0,0x34,0,&delivery.services());check(timerid>=0,"source timer creation retained");
 check(dh2_character_timers_update(&h.timers,1,0,&delivery.services())==1&&delivery.failure()==-2&&!delivery.error().empty(),"native void timer callback retains reached failure for host Update caller");
 std::cout<<"PASS original ClassDict Buff_Speed="<<id<<" Debuff_Slow="<<slow<<" AUTO_DOT_01_FIRE="<<dotid<<" duplicate/duration/expiry/reentry/interrupt/death/end/required-service/live-DOT/shared-owner/application-tail/void-timer-failure checks\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
