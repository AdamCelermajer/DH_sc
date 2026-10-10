#include "source_status_owners.hpp"
#include <cstring>
namespace dh::foundation::status {
using namespace dh2::character;
namespace {
bool overlaps(const void* a,std::size_t n,const void* b,std::size_t m){
 auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return x<=y?y-x<n:x-y<m;
}
struct InitFailure {int status;};
}
int construct_source_property_owner(const SourcePropertyConstructionBorrow& b,
 BuffOwner*& published,std::string& error){
 error.clear();
 if(published||!b.character||!b.sheets||!b.properties||!b.process_temporary||
    !b.fsm||!b.fsm->state||b.fsm->character!=b.character||
    b.bindings.properties!=b.properties||!b.bindings.timers||
    b.bindings.timers->owner!=b.character||b.bindings.timers->update_depth||
    b.properties->groups||b.properties->group_count||dh2_property_validate(b.properties)||
    b.properties->base!=b.sheets->base.data()||b.properties->saved!=b.sheets->saved.data()||
    b.properties->gear!=b.sheets->gear.data()||b.properties->resolved!=b.sheets->resolved.data()||
    overlaps(b.process_temporary,sizeof(*b.process_temporary),b.sheets,sizeof(*b.sheets))||
    overlaps(b.process_temporary,sizeof(*b.process_temporary),b.properties->defaults,896)||
    overlaps(b.sheets,sizeof(*b.sheets),b.properties->defaults,896)){
  error="Fresh source CharProperties construction requires SAME retained Character/FSM/inline sheets/timers and process temporary";return -1;
 }
 // Original C2 initializes its empty buff map before ResetAllProperties.
 auto* owner=dh2_character_buffs_create(&b.bindings);
 if(!owner){error="Required source CharProperties buff map allocation/bindings failed";return -2;}
 published=owner;
 // ResetAllProperties3defc4: base, saved, gears, cached. Then C2's separate
 // _ResetProperties3def34 call resets the SAME process temporary sheet.
 for(auto* sheet:{&b.sheets->base,&b.sheets->saved,&b.sheets->gear,&b.sheets->resolved})
  std::memcpy(sheet->data(),b.properties->defaults,896);
 std::memcpy(b.process_temporary->data(),b.properties->defaults,896);
 return 1;
}
SourceStatusInitBinding::SourceStatusInitBinding(SourceStatusInitBorrow b):b_(b),services_{this,invoke}{}
bool SourceStatusInitBinding::valid(){
 if(!b_.character||!b_.properties||dh2_property_validate(b_.properties)||
    !b_.fsm||!b_.fsm->state||b_.fsm->character!=b_.character||
    !b_.lifecycle||b_.lifecycle->owner!=b_.character||!b_.life||
    !b_.timers||b_.timers->owner!=b_.character||!b_.timer_services||
    b_.timer_services->reserved||b_.timers->update_depth){
  error_="Source CharAI OnInit requires SAME retained Character/property/FSM/lifecycle/timer owners outside Update";return false;
 }
 return true;
}
int SourceStatusInitBinding::on_init(){
 if(busy_){error_="Synchronous source CharAI OnInit reentry rejected";return -1;}
 error_.clear();if(!valid())return -1;
 busy_=true;struct Exit{bool& busy;~Exit(){busy=false;}} exit{busy_};
 try{
  auto status=dh2_character_script_lifecycle(b_.lifecycle,script_on_init,0,&services_);
  if(status<0)error_="Malformed source CharAI lifecycle state";
  return status;
 }catch(const InitFailure& failure){return failure.status;}
}
void SourceStatusInitBinding::invoke(void* context,ScriptLifecycleState64* state,
 const ScriptLifecycleRequest32* q,ScriptLifecycleResponse16* out){
 auto& self=*static_cast<SourceStatusInitBinding*>(context);
 auto fail=[&](int status,const char* text){self.error_=text;throw InitFailure{status};};
 if(!q||!out||state!=self.b_.lifecycle||!self.valid())fail(-1,"Source CharAI callback changed retained owner identity");
 switch(q->service){
 case script_owner_is_dead:{
  if(q->subject!=self.b_.character)fail(-1,"Source CharAI IsDead targets foreign Character");
  std::int32_t dead{};if(dh2_character_timer_owner_query(&dead,self.b_.life,1)!=1)fail(-2,"Required actual Character IsDead fields failed");
  std::memcpy(&out->word,&dead,4);return;
 }
 case script_design_tick:
  if(dh2_character_design_tick(&out->word,self.b_.design,q->argument0)!=1)fail(-2,"Required actual CharacterDesign AI_Tick/DoT_Tick service failed");
  return;
 case script_timer_stop:
  if(q->subject!=self.b_.character)fail(-1,"Source CharAI StopTimer targets foreign Character");
  if(dh2_character_timer_stop(self.b_.timers,q->argument0)<0)fail(-2,"Required original CharAI StopTimer failed");
  return;
 case script_timer_start:{
  if(q->subject!=self.b_.character||q->payload||(q->argument1!=0x33&&q->argument1!=0x34))fail(-1,"Malformed source CharAI StartTimer owner/event");
  const auto id=dh2_character_timer_start(self.b_.timers,q->argument0,-1,std::int32_t(q->argument1),0,self.b_.timer_services);
  // Source UINT_MAX StartTimer failure is a genuine stored return. Port
  // storage/growth errors -2/-3 are explicit incomplete native providers.
  if(id< -1)fail(-2,"Required original CharAI timer allocation failed");
  std::memcpy(&out->word,&id,4);return;
 }
 case script_ais_init:
  if(!self.b_.ais||self.b_.ais(self.b_.context,state,q,out))fail(-2,"Required original pending AIS OnInit service failed");
  return;
 default:fail(-2,"Unrecovered source CharAI initialization service");
 }
}
}
