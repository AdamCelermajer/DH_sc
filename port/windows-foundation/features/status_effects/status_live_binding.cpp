#include "status_live_binding.hpp"
namespace dh::foundation::status {
using namespace dh2::character;
DotTickBinding::DotTickBinding(DotTickBorrow b):b_(b),services_{this,invoke}{}
bool DotTickBinding::valid(std::string& error)const{
 if(!b_.state||!b_.actor||!b_.state->owner||b_.actor->identity!=b_.state->owner||
    !b_.state->properties||b_.actor->properties!=b_.state->properties||
    dh2_property_validate(b_.state->properties)){
  error="DoT tick requires SAME retained Character/PropertyView";return false;
 }
 if(!b_.combat||!b_.dot_services||!b_.dot_services->invoke||!b_.queries||
    !b_.queries->invoke||b_.queries==&services_){
  error="DoT tick requires shared combat context, full native DotServices and real owner queries";return false;
 }
 error.clear();return true;
}
int DotTickBinding::invoke(void* context,TimerEffectState32* state,
 const TimerEffectRequest40* q,std::int32_t* word,dh2::data::CombatResult* result){
 auto& self=*static_cast<DotTickBinding*>(context);
 if(self.busy_){self.error_="Synchronous DoT provider reentry rejected";self.source_status_=-1;return -1;}
 if(!self.valid(self.error_)||state!=self.b_.state||!q||!word||q->subject!=state->owner){
  if(self.error_.empty())self.error_="DoT tick callback requires SAME retained state/subject";
  self.source_status_=-1;return -1;
 }
 self.busy_=true;struct Exit{bool& flag;~Exit(){flag=false;}} exit{self.busy_};
 self.error_.clear();self.source_status_=0;
 if(q->service!=effect_dot_calculate&&q->service!=effect_dot_apply){
  if(q->service>effect_debug_query){self.error_="Unknown timer effect service";self.source_status_=-1;return -1;}
  auto status=self.b_.queries->invoke(self.b_.queries->context,state,q,word,result);
  if(status){self.error_="Required source timer owner/debug query failed";self.source_status_=-2;}
  return status;
 }
 if(!result||q->target!=state->owner||q->element< -1||q->element>4||
    q->property!=std::uint32_t(127+q->element)){
  self.error_="Malformed self/self DoT CalculateResult/ApplyResult request";self.source_status_=-1;return -1;
 }
 DotResult24 out{};
 self.source_status_=q->service==effect_dot_calculate?
  dh2_character_dot_calculate_result(&out,result,self.b_.combat,self.b_.actor,q->amount,q->element,self.b_.dot_services):
  dh2_character_dot_apply(&out,result,self.b_.actor,self.b_.dot_services);
 if(self.source_status_!=1){self.error_="Required source DoT kernel failed at service phase "+std::to_string(out.phase)+" status "+std::to_string(self.source_status_);return -1;}
 *word=0;return 0;
}
int dispatch_status_timer(StatusEffects& status,std::uintptr_t expected,
 std::uintptr_t delivered,std::int32_t event,const Timer32* timer,
 BuffResult24& buff,TimerEffectResult24& tick,std::string& error){
 if(event!=54&&event!=0x33&&event!=0x34)return 0;
 if(!expected||delivered!=expected){error="Status timer delivery requires SAME source Character";return -1;}
 auto result=event==54?status.expired(buff,timer):status.tick(tick,std::uint32_t(event));
 if(result<0)error=status.error().empty()?"Required native status timer service failed":status.error();
 else error.clear();return result;
}
int dispatch_skill_status(const SkillStatusBorrow& b,
 const skills::SkillApplyRequestV6* q,skills::SkillApplyResponseV6* r,
 dh2::data::CombatResult* result,std::string& error){
 if(!b.status||!q||!r){error="Required SAME status/application request";return -1;}
 auto status=b.status->application(*q,*r);
 if(status<0){error=b.status->error().empty()?"Required native status application failed":b.status->error();return status;}
 if(status==1){error.clear();return 0;} // SkillApplyServices uses zero delivery.
 if(!b.continuation||!b.continuation->invoke){error="Required full F_ApplyResult continuation service";return -2;}
 status=b.continuation->invoke(b.continuation->context,q,r,result);
 if(status)error="Required full F_ApplyResult continuation failed";else error.clear();
 return status;
}
StatusTimerDelivery::StatusTimerDelivery(TimerStore32& timers,const TimerServices32* other):
 timers_(timers),other_(other),services_{this,expired,other&&other->grow?grow:nullptr,0}{}
bool StatusTimerDelivery::bind(StatusEffects& status,std::string& error){
 if(status_||!timers_.owner||timers_.update_depth||status.timer_store()!=&timers_||!status.valid(error)){
  if(error.empty())error="Status timer publication requires stable source owner outside Update, once";return false;
 }
 status_=&status;error.clear();return true;
}
void StatusTimerDelivery::expired(void* context,std::uintptr_t owner,std::int32_t event,Timer32* timer){
 auto& self=*static_cast<StatusTimerDelivery*>(context);
 if(self.failure_)return; // host aborts frame after Update; first failure stays.
 if(owner!=self.timers_.owner||!self.status_){self.failure_=-1;self.error_="Required published SAME status timer owner";return;}
 BuffResult24 buff{};TimerEffectResult24 tick{};
 auto result=dispatch_status_timer(*self.status_,self.timers_.owner,owner,event,timer,buff,tick,self.error_);
 if(result<0){self.failure_=result;return;}
 if(result==0){
  if(!self.other_||!self.other_->expired){self.failure_=-2;self.error_="Required original unrelated Character timer event provider";return;}
  self.other_->expired(self.other_->context,owner,event,timer);
 }
}
int StatusTimerDelivery::grow(void* context,TimerStore32* timers,std::uint32_t capacity){
 auto& self=*static_cast<StatusTimerDelivery*>(context);
 if(timers!=&self.timers_||!self.other_||!self.other_->grow){self.failure_=-1;self.error_="Required SAME original timer allocator";return 0;}
 return self.other_->grow(self.other_->context,timers,capacity);
}
}
