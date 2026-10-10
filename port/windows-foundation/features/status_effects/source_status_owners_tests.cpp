#include "source_status_owners.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh::foundation::status;
using namespace dh2::character;
static void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
// Source-field fixtures over caller-retained storage. No live Character
// publication or full Character C1 execution is claimed by this test.
struct Retained {
 std::uintptr_t identity=reinterpret_cast<std::uintptr_t>(this);
 dh2::data::PropertyRules rules;dh2::data::PropertyState sheets;
 dh2::data::PropertyView properties=dh2::data::property_view(rules,sheets);
 dh2::data::PropertySheet process_temporary{};
 State state;NativeFsm24 fsm{&state,identity,0,0};
 Timer32 slots[20]{};TimerStore32 timers{slots,0,20,identity,0,0};
 TimerServices32 clock{};BuffServices16 buffs{this,buff};BuffOwner* owner{};
 ScriptLifecycleState64 lifecycle{};TimerOwner8 life{-1,0,0,0};
 dh2_script_design_bindings design{this,constant,0};
 std::vector<std::string> trace;bool fail_dot=false;unsigned buff_calls=0;
 Retained(){lifecycle.owner=identity;lifecycle.timer33=lifecycle.timer34=-1;
  for(unsigned i=0;i<224;++i)rules.defaults[i]=std::int32_t(100+i);
 }
 ~Retained(){dh2_character_timers_stop_all(&timers);dh2_character_buffs_destroy(owner);}
 static int buff(void* p,dh2::data::PropertyView*,const BuffRequest32*,std::uintptr_t*){++static_cast<Retained*>(p)->buff_calls;return 0;}
 static int constant(void* p,std::uint32_t kind,const char* group,const char* key,std::int32_t* value){
  auto& f=*static_cast<Retained*>(p);
  check(kind==0&&std::string(group)=="CharacterDesign","actual source constants group/kind");
  f.trace.emplace_back(key);
  if(std::string(key)=="AI_Tick"){
   // Source stop-old33 happened BEFORE first constant lookup, while old34
   // remains active. These are exact independent source instruction ordering.
   if(f.lifecycle.timer33>=0)check(!f.slots[f.lifecycle.timer33].active,"source old33 stop precedes design lookup");
   if(f.lifecycle.timer34>=0)check(f.slots[f.lifecycle.timer34].active,"source old34 survives first design lookup");
   *value=3701;
  }else{
   check(std::string(key)=="DoT_Tick","source DoT constant key");
   check(f.lifecycle.timer33>=0&&f.slots[f.lifecycle.timer33].active&&f.slots[f.lifecycle.timer33].event==0x33,"source new33 publication precedes second constant lookup");
   if(f.lifecycle.timer34>=0)check(!f.slots[f.lifecycle.timer34].active,"source old34 stop precedes DoT lookup");
   if(f.fail_dot)return -1;
   *value=1903;
  }
  return 0;
 }
 static int ais(void* p,ScriptLifecycleState64* state,const ScriptLifecycleRequest32* q,ScriptLifecycleResponse16*){
  auto& f=*static_cast<Retained*>(p);f.trace.emplace_back("AIS_Init");
  check(state==&f.lifecycle&&q->service==script_ais_init&&q->subject==f.lifecycle.pending,"SAME source pending AIS receiver");
  if(!f.life.dead)check(f.slots[f.lifecycle.timer33].active&&f.slots[f.lifecycle.timer34].active,"AIS source virtual runs after both timer publications");
  return 0;
 }
 SourceStatusInitBorrow init(){return {identity,&properties,&fsm,&lifecycle,&life,&timers,&clock,&design,this,ais};}
 SourcePropertyConstructionBorrow construction(){return {identity,&sheets,&properties,&process_temporary,&fsm,{&properties,&timers,&clock,&buffs}};}
 void old_timers(){lifecycle.timer33=dh2_character_timer_start(&timers,20,-1,0x33,0,&clock);lifecycle.timer34=dh2_character_timer_start(&timers,30,-1,0x34,0,&clock);check(lifecycle.timer33==0&&lifecycle.timer34==1,"source old timer setup");}
};
int main(){try{
 std::string error;Retained f;auto* base=f.properties.base;auto* cached=f.properties.resolved;
 check(construct_source_property_owner(f.construction(),f.owner,error)==1&&f.owner,"genuine source buff map constructor");
 for(unsigned i=0;i<224;++i)check(f.sheets.base[i]==f.rules.defaults[i]&&f.sheets.saved[i]==f.rules.defaults[i]&&f.sheets.gear[i]==f.rules.defaults[i]&&f.sheets.resolved[i]==f.rules.defaults[i]&&f.process_temporary[i]==f.rules.defaults[i],"source four-sheet plus shared temporary reset");
 check(f.properties.base==base&&f.properties.resolved==cached&&!f.buff_calls&&!dh2_character_buffs_count(f.owner),"retained source sheet identity; no fabricated class/recalc service");
 f.sheets.saved[0]=999;check(construct_source_property_owner(f.construction(),f.owner,error)==-1&&f.sheets.saved[0]==999,"already published owner is never reset/adopted as fresh");
 Retained foreign;auto bad=foreign.construction();bad.fsm=&f.fsm;check(construct_source_property_owner(bad,foreign.owner,error)==-1&&!foreign.owner,"foreign actual FSM rejected atomically");
 bad=foreign.construction();bad.process_temporary=&foreign.sheets.saved;check(construct_source_property_owner(bad,foreign.owner,error)==-1,"private actor sheet cannot replace process temporary");
 f.old_timers();f.lifecycle.pending=0x123456;SourceStatusInitBinding init(f.init());
 check(init.on_init()==1&&f.trace==std::vector<std::string>{"AI_Tick","DoT_Tick","AIS_Init"},"source OnInit timer/AIS order");
 check(f.timers.count==2&&f.lifecycle.timer33==0&&f.lifecycle.timer34==1&&f.slots[0].duration_ms==3701&&f.slots[1].duration_ms==1903&&f.slots[0].repeat==-1&&f.slots[1].repeat==-1&&!f.slots[0].user_ref&&!f.slots[1].user_ref,"source design durations/repeat/ref; no new clock");
 Retained failed;failed.old_timers();failed.fail_dot=true;SourceStatusInitBinding partial(failed.init());
 check(partial.on_init()==-2&&failed.trace==std::vector<std::string>{"AI_Tick","DoT_Tick"}&&failed.slots[0].active&&!failed.slots[1].active&&failed.lifecycle.timer33==0&&failed.lifecycle.timer34==1&&!partial.error().empty(),"reached design failure retains real stop/start/lifecycle prefix");
 Retained dead;dead.life.dead=1;dead.lifecycle.pending=0x778899;auto db=dead.init();db.design=nullptr;SourceStatusInitBinding skip(db);
 check(skip.on_init()==1&&dead.timers.count==0&&dead.trace==std::vector<std::string>{"AIS_Init"},"source dead gate skips design/timers but still invokes pending AIS");
 Retained missing;missing.lifecycle.pending=0x11;auto mb=missing.init();mb.ais=nullptr;SourceStatusInitBinding boundary(mb);
 check(boundary.on_init()==-2&&missing.timers.count==2&&!boundary.error().empty(),"missing original AIS stops after real timer publication");
 std::cout<<"PASS source CharProperties C2 retained sheets/global temporary/map; source CharAI OnInit owner/order/design/old-timer/dead/AIS/failure-prefix checks\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
