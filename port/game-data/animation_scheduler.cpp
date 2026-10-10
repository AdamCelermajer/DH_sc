#include "animation_scheduler.hpp"
#include <stdexcept>
#include <functional>
extern "C" unsigned dh2_animation_complete(std::int32_t type,std::uint32_t count,std::uint32_t* step,std::int32_t* loops){
 if(!count||*step>=count)return 3;
 if(type==1){++*step;if(*step<count)return 1;}
 if(*loops==0)return 2;
 if(*loops>0)--*loops;
 return 0;
}
namespace dh2::data {
void AnimationScheduler::enter(const AnimationTables& table,std::int32_t id,AnimationRandom& random,bool enabled){
 if(id<0||std::size_t(id)>=table.sequences.size()||stack.size()>=3)throw std::runtime_error("Animation scheduler reference/depth outside limit");
 const auto& sequence=table.sequences[id];if(sequence.steps.empty()||sequence.steps.size()>10000)throw std::runtime_error("Animation scheduler has invalid step count");
 const unsigned index=sequence.type==2&&enabled?dh2_animation_random(&random.seed,&random.calls,sequence.steps.size()):0;
 stack.push_back({id,sequence.loop,index});activate(table,random,enabled);
}
void AnimationScheduler::activate(const AnimationTables& table,AnimationRandom& random,bool enabled){
 const auto& frame=stack.back();const auto& sequence=table.sequences.at(frame.sequence);
 if(frame.step>=sequence.steps.size())throw std::runtime_error("Animation scheduler step outside sequence");
 const auto& step=sequence.steps[frame.step];
 if(step.redir==1){enter(table,step.anim,random,enabled);return;}
 if(step.redir!=0||step.anim<0)throw std::runtime_error("Animation scheduler has no clip");
 current=step;playing=true;
}
bool AnimationScheduler::start(const AnimationTables& table,std::int32_t id,AnimationRandom& random,std::string& error,bool enabled){
 error.clear();try{AnimationScheduler next;auto rng=random;next.enter(table,id,rng,enabled);*this=std::move(next);random=rng;return true;}catch(const std::exception& e){error=e.what();return false;}
}
bool AnimationScheduler::enter_with_services(const AnimationTables& table,std::int32_t id,std::size_t depth,
                                             AnimationRandom& random,bool enabled,const AnimationSelectionServices& services){
 if(id<0||std::size_t(id)>=table.sequences.size()||depth>=3)throw std::runtime_error("Animation scheduler reference/depth outside limit");
 const auto& sequence=table.sequences[id];
 if(sequence.steps.empty()||sequence.steps.size()>10000)throw std::runtime_error("Animation scheduler has invalid step count");
 stack.resize(depth+1);stack[depth].sequence=id;stack[depth].loops=sequence.loop;
 if(services.policy){
  if(!services.error)return false;
  if(!services.policy->context||!services.policy->trace||!services.policy->random_enabled){
   *services.error="Required original CharAnimator Debug selection policy";return false;
  }
  if(!services.policy->trace(services.policy->context,*services.error))return false;
 }
 if(services.event)services.event(services.context,*this,0x24);
 // _SetAnim retains its authored sequence pointer across event24. Its live
 // current frame may have been changed by a synchronous metadata swap.
 bool random_enabled=enabled;
 if(sequence.type==2&&services.policy&&
    !services.policy->random_enabled(services.policy->context,random_enabled,*services.error))return false;
 const unsigned index=sequence.type==2&&random_enabled?dh2_animation_random(&random.seed,&random.calls,sequence.steps.size()):0;
 return activate_with_services(table,index,random,enabled,services);
}
bool AnimationScheduler::activate_with_services(const AnimationTables& table,std::uint32_t index,
                                                AnimationRandom& random,bool enabled,const AnimationSelectionServices& services){
 if(stack.empty())throw std::runtime_error("Animation selection frame is absent");
 const auto& sequence=table.sequences.at(stack.back().sequence);
 if(index>=sequence.steps.size())throw std::runtime_error("Animation scheduler step outside sequence");
 const auto& step=sequence.steps[index]; // retained before event26
 stack.back().step=index;
 if(services.event)services.event(services.context,*this,0x26);
 if(step.redir==1)return enter_with_services(table,step.anim,stack.size(),random,enabled,services);
 if(step.redir!=0||step.anim<0)throw std::runtime_error("Animation scheduler has no clip");
 current=step;playing=true;
 return !services.prepare||services.prepare(services.context,*this);
}
bool AnimationScheduler::start_with_services(const AnimationTables& table,std::int32_t id,AnimationRandom& random,
                                            std::string& error,const AnimationSelectionServices& services,bool enabled){
 error.clear();try{return enter_with_services(table,id,0,random,enabled,services);}
 catch(const std::exception& e){error=e.what();return false;}
}
bool AnimationScheduler::complete(const AnimationTables& table,AnimationRandom& random,std::string& error,bool enabled){
 error.clear();if(!playing)return true;
 try{
  auto next=*this;auto rng=random;
  while(!next.stack.empty()){
   auto& frame=next.stack.back();const auto& sequence=table.sequences.at(frame.sequence);
   const auto action=dh2_animation_complete(sequence.type,sequence.steps.size(),&frame.step,&frame.loops);
   if(action==3)throw std::runtime_error("Animation completion has invalid state");
   if(action==1){next.activate(table,rng,enabled);break;}
   if(action==0){const auto id=frame.sequence,remaining=frame.loops;const auto depth=next.stack.size();next.stack.pop_back();next.enter(table,id,rng,enabled);next.stack[depth-1].loops=remaining<0?-1:remaining;break;}
   next.stack.pop_back();
  }
  if(next.stack.empty())next.playing=false;
  *this=std::move(next);random=rng;return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool AnimationScheduler::complete_with_services(const AnimationTables& table,AnimationRandom& random,
                                                std::string& error,const AnimationCompletionServices& services,
                                                bool enabled){
 error.clear();
 auto emit=[&](std::uint32_t event){if(services.event)services.event(services.context,*this,event);};
 auto finish=[&](){return !services.finish||services.finish(services.context,*this);};
 if(stack.empty())return finish();
 try{
  std::function<bool(std::size_t)> update=[&](std::size_t depth)->bool{
   if(depth>=stack.size())throw std::runtime_error("Animation completion stack disappeared");
   // Original Update retains the sequence pointer before event27, but reads
   // the live frame's step/loop words after synchronous callback return.
   const auto& sequence=table.sequences.at(stack[depth].sequence);
   const auto type=sequence.type;const auto count=sequence.steps.size();
   if(!count||count>10000)throw std::runtime_error("Animation completion has invalid state");
   emit(0x27);
   if(depth>=stack.size())throw std::runtime_error("Animation completion callback replaced its stack");
   const bool skip_end_event=type==1&&stack[depth].step+1!=count;
   if(!skip_end_event)emit(0x25);
   if(depth>=stack.size())throw std::runtime_error("Animation completion callback replaced its stack");
   if(type==1)++stack[depth].step;
   if(type==1&&stack[depth].step<count){
    emit(0x23);
    if(services.selection){if(!activate_with_services(table,stack.back().step,random,enabled,*services.selection))return false;}
    else {activate(table,random,enabled);if(services.prepare&&!services.prepare(services.context,*this))return false;}
    return finish();
   }
   if(stack[depth].loops==0){
    if(depth==0){
     // The original retains its exhausted root frame/selected resource. It
     // marks closed48 before event22, rather than erasing the terminal pose.
     playing=false;emit(0x22);return finish();
    }
    stack.resize(depth);
    if(!update(depth-1))return false;
    return finish(); // child's common tail after recursive parent Update
   }
   if(stack[depth].loops>0)--stack[depth].loops;
   emit(0x23);
   // Repeat differs from ordinary sequential advance: a queued ANIM_Set skips
   // old Initialize, including RNG and old PlayClip/NewAnim side effects.
   if(!services.has_pending||!services.has_pending(services.context)){
    const auto id=stack[depth].sequence,remaining=stack[depth].loops;
    if(services.initialize)services.initialize(services.context,*this,id,static_cast<std::uint32_t>(depth));
    if(services.selection){if(!enter_with_services(table,id,depth,random,enabled,*services.selection))return false;}
    else {stack.resize(depth);enter(table,id,random,enabled);if(services.prepare&&!services.prepare(services.context,*this))return false;}
    // _SetAnim/replay may write StopLoop, but source restores its captured
    // remaining repeat word AFTER that synchronous service returns.
    stack.at(depth).loops=remaining<0?-1:remaining;
   }
   return finish();
  };
  return update(stack.size()-1);
 }catch(const std::exception& e){error=e.what();return false;}
}
AnimationSwap AnimationScheduler::swap_sequences(const AnimationTables& table,std::int32_t desired,std::int32_t old,std::string& error){
 error.clear();if(desired==-1)return AnimationSwap::no_op;
 if(stack.empty()){error="Animation swap has no source stack";return AnimationSwap::rejected;}
 const auto root=stack.front().sequence;
 if(old!=-1&&root!=old)return root==desired?AnimationSwap::no_op:AnimationSwap::restart;
 try{
  auto next=stack;auto replacement=desired;
  for(std::size_t depth=0;depth<next.size();++depth){
   const auto& prior=table.sequences.at(next[depth].sequence);
   const auto& selected=table.sequences.at(replacement);
   if(prior.type!=selected.type||prior.steps.size()!=selected.steps.size())throw std::runtime_error("Animation swap source shape differs");
   next[depth].sequence=replacement;
   if(depth+1<next.size()){
    const auto& step=selected.steps.at(next[depth].step);
    if(step.redir!=1)throw std::runtime_error("Animation swap child is not a redirect");
    replacement=step.anim;
   }
  }
  stack=std::move(next);return AnimationSwap::metadata;
 }catch(const std::exception& e){error=e.what();return AnimationSwap::rejected;}
}
}
