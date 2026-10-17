#include "actor_playback.hpp"
#include <cmath>
#include <cstring>
extern "C" int dh2_actor_event_handoff(dh2::actor::EventHandoff* out,std::int32_t* lag,const dh2::animation::TriggeredEvent* event){
 if(!out||!lag||!event||!event->name)return -1;
 const auto word0=event->lag_ms;const auto* word4=event->name;
 *lag=word0;*out={0x28,word0,word4};return 0;
}
extern "C" int dh2_actor_sequence_close(dh2::actor::EventHandoff* out,std::uint32_t* closed){
 if(!out||!closed||*closed>1)return -1;
 if(*closed)return 0;
 *closed=1;*out={0x22,0,nullptr};return 1;
}
#ifndef DH2_ACTOR_PLAYBACK_KERNEL_ONLY
namespace dh2::actor {
namespace {
void ending(void* context,timeline::State* state){
 auto& playback=*static_cast<Playback*>(context);
 dh2_timeline_notify(&playback.completion,state);
}
struct ReplayContext {
 Playback* playback;const ClipBank* bank;
 visual::SceneBinding* visual;scene::Scene* scene;std::string* error;bool success=true;
};
void replay(void* context,std::uint32_t event,timeline::State*,const timeline::ReplayResult* result){
 auto& c=*static_cast<ReplayContext*>(context);
 if(event!=timeline::new_animation)return;
 c.playback->displacement=result->displacement!=0;
 if(result->restart){
  // NewAnim synchronously calls root onAnimate at its last scene timestamp.
  ++c.playback->restarts;
  c.success=c.playback->replay_phase(result->timestamp,*c.bank,*c.visual,*c.scene,*c.error);
 }
}
struct DispatchContext {Playback* playback;EventObserver observer;std::int32_t clip;std::uint32_t timestamp,phase;};
void triggered(const animation::TriggeredEvent* input,void* context){
 auto& c=*static_cast<DispatchContext*>(context);auto& p=*c.playback;EventHandoff handoff{};
 if(dh2_actor_event_handoff(&handoff,&p.last_event_lag,input))return;
 // This is the retained manager's callback, including after a nested selection.
 const PlaybackEvent event{handoff,c.clip,c.timestamp,c.phase,0};
 if(c.observer.invoke)c.observer.invoke(c.observer.context,p,event);
}
struct CompletionContext {
 Playback* playback;const data::AnimationTables* tables;data::AnimationRandom* random;
 const ClipBank* bank;visual::SceneBinding* visual;scene::Scene* scene;
 float speed;std::string* error;
};
}
void Playback::scheduler_event(std::uint32_t id){
 EventHandoff handoff{id,0,nullptr};
 const auto phase=(id==0x24||id==0x26)?std::uint32_t(playback_selection_event):
                  id==0x22?std::uint32_t(sequence_event):std::uint32_t(playback_animator_event);
 if(id==0x22){
  if(dh2_actor_sequence_close(&handoff,&sequence_closed)!=1)return;
  handoff.payload=reinterpret_cast<const char*>(&sequence_closed);
 }
 const PlaybackEvent event{handoff,clip_id,root_timestamp,phase,0};
 const auto callback=observer;if(callback.invoke)callback.invoke(callback.context,*this,event);
}
bool Playback::set_speed(float speed,std::string& error){
 if(!std::isfinite(speed)||speed<0){error="Animation global speed rejected";return false;}
 const float product=speed*scheduler.clip().speed;
 if(!std::isfinite(product)||dh2_timeline_scale(&timeline,product)){
  error="Animation clip speed rejected";return false;
 }
 return true;
}
bool Playback::apply_selection(const ClipBank& bank,visual::SceneBinding& visual,
                               scene::Scene& scene,float speed,std::string& error){
 const int mapped=scheduler.clip().anim;
 auto clip=bank.find(mapped);if(clip==bank.end()){error="Authored playback clip is not bundled";return false;}
 const int previous=clip_id;
 if(previous!=mapped&&dh2_timeline_clip(&timeline,mapped,clip->second.start,clip->second.end)){
  error="Authored timeline clip rejected";return false;
 }
 // CharAnimator::_SetAnimStep clears its finite-close byte after choosing a
 // live controller clip. Keep the owner latch aligned when a sequence advances.
 sequence_closed=0;
 clip_id=mapped;
 event_cursor={};const auto generation=++event_generation;
 const timeline::ReplayFacts facts{previous,mapped,completion.extra_ms,0,
                                  unsigned(scheduler.clip().move_go),1,root_timestamp,0};
 ReplayContext context{this,&bank,&visual,&scene,&error};
 const timeline::ReplayServices services{&context,replay};timeline::ReplayResult result{};
 if(dh2_timeline_replay(&result,&timeline,&facts,&services)!=1||!context.success)return false;
 // Source caller reads live animator speed fields after synchronous callbacks.
 // A nested selection has already installed its own requested speed.
 return generation!=event_generation||set_speed(speed,error);
}
bool Playback::start(const data::AnimationTables& tables,int sequence,data::AnimationRandom& random,
                     const ClipBank& bank,visual::SceneBinding& visual,scene::Scene& scene,
                     float speed,std::string& error){
 if(observer.invoke&&processing_completion){
  if(sequence<0||std::size_t(sequence)>=tables.sequences.size()||!std::isfinite(speed)||speed<0){error="Pending animation selection rejected";return false;}
  pending_sequence=sequence;pending_speed=speed;return true;
 }
 completion={};
 sequence_closed=0;pending_sequence=-1;
 CompletionContext context{this,&tables,&random,&bank,&visual,&scene,speed,&error};
 const data::AnimationSelectionServices services{
  &context,
  [](void* raw,data::AnimationScheduler&,std::uint32_t id){
   static_cast<CompletionContext*>(raw)->playback->scheduler_event(id);
  },
  [](void* raw,data::AnimationScheduler&){
   auto& c=*static_cast<CompletionContext*>(raw);
   return c.playback->apply_selection(*c.bank,*c.visual,*c.scene,c.speed,*c.error);
  },nullptr
 };
 return scheduler.start_with_services(tables,sequence,random,error,services);
}
bool Playback::swap(const data::AnimationTables& tables,int desired,int old,data::AnimationRandom& random,
                    const ClipBank& bank,visual::SceneBinding& visual,scene::Scene& scene,
                    float speed,std::string& error){
 const auto result=scheduler.swap_sequences(tables,desired,old,error);
 if(result==data::AnimationSwap::rejected)return false;
 if(result==data::AnimationSwap::restart)return start(tables,desired,random,bank,visual,scene,speed,error);
 return true;
}
bool Playback::animate(std::uint32_t absolute,bool reset,std::uint32_t phase,const ClipBank& bank,
                       visual::SceneBinding& visual,scene::Scene& scene,std::string& error){
 const auto clip=bank.find(clip_id);if(clip==bank.end()){error="Character playback clip is absent";return false;}
 // ResetDelta samples a value at clip start without applying a pose. Keep that
 // history before callbacks, using a detached binding/scene sample; public root
 // and live scene remain unchanged until actual animation values are applied.
 if(reset&&observer.invoke){
  auto baseline=visual;auto authored=scene;const auto prior_root=visual.root;
  if(!baseline.sample(authored,clip->second,clip->second.start,absolute+1,true,false,error))return false;
  baseline.root=prior_root;visual=std::move(baseline);reset=false;
 }
 const auto previous=timeline.current_ms;
 std::int32_t signed_time;std::memcpy(&signed_time,&absolute,4);
 const timeline::Services services{this,ending};
 if(dh2_timeline_update(&timeline,signed_time,&services)){error="Scene timeline rejected";return false;}
 if(observer.invoke){
  const auto generation=event_generation;auto cursor=event_cursor;
  DispatchContext context{this,observer,clip_id,absolute,phase};
  if(!dh2_events_update(&clip->second.events.view(),&cursor,previous,timeline.current_ms,
                       timeline.start_ms,timeline.end_ms,triggered,&context)){error="Authored event track rejected";return false;}
  // Original manager is retained through synchronous dispatch. A replacement
  // manager owns a fresh cursor; old dispatch completion cannot overwrite it.
  if(generation==event_generation)event_cursor=cursor;
 }
 const auto active=bank.find(clip_id);if(active==bank.end()){error="Selected playback clip is absent";return false;}
 if(!visual.sample(scene,active->second,timeline.current_ms,absolute,reset,displacement,error))return false;
 return true;
}
bool Playback::scene_phase(std::uint32_t absolute,const ClipBank& bank,
                           visual::SceneBinding& visual,scene::Scene& scene,std::string& error){
 // A finished finite sequence retains its timeline and terminal pose. The
 // original scene animator continues to sample it while its owner remains.
 if(!animate(absolute,false,scene_event,bank,visual,scene,error))return false;
 root_timestamp=absolute;return true;
}
bool Playback::replay_phase(std::uint32_t absolute,const ClipBank& bank,
                            visual::SceneBinding& visual,scene::Scene& scene,std::string& error){
 return animate(absolute,true,replay_event,bank,visual,scene,error);
}
bool Playback::animator_phase(const data::AnimationTables& tables,data::AnimationRandom& random,
                              const ClipBank& bank,visual::SceneBinding& visual,scene::Scene& scene,
                              float speed,std::int32_t extra,std::string& error){
 if(!completion.pending)return set_speed(speed,error);
 completion.extra_ms=extra;++completions;
 CompletionContext context{this,&tables,&random,&bank,&visual,&scene,speed,&error};
 const data::AnimationSelectionServices selection{
  &context,
  [](void* raw,data::AnimationScheduler&,std::uint32_t id){
   static_cast<CompletionContext*>(raw)->playback->scheduler_event(id);
  },
  [](void* raw,data::AnimationScheduler&){
   auto& c=*static_cast<CompletionContext*>(raw);
   return c.playback->apply_selection(*c.bank,*c.visual,*c.scene,c.speed,*c.error);
  },nullptr
 };
 const data::AnimationCompletionServices services{
  &context,selection.event,
  [](void* raw){return static_cast<CompletionContext*>(raw)->playback->pending_sequence!=-1;},
  selection.prepare,
  [](void* raw,data::AnimationScheduler&){
   auto& c=*static_cast<CompletionContext*>(raw);auto& p=*c.playback;
   p.completion.pending=0;
   if(p.pending_sequence==-1)return true;
   const auto sequence=p.pending_sequence;const auto selected_speed=p.pending_speed;
   p.pending_sequence=-1;
   const bool was_processing=p.processing_completion;p.processing_completion=false;
   const bool success=p.start(*c.tables,sequence,*c.random,*c.bank,*c.visual,
                              *c.scene,selected_speed,*c.error);
   p.processing_completion=was_processing;return success;
  },nullptr,&selection
 };
 const bool was_processing=processing_completion;processing_completion=true;
 bool success=false;
 try{success=scheduler.complete_with_services(tables,random,error,services);}
 catch(...){processing_completion=was_processing;throw;}
 processing_completion=was_processing;return success;
}
}
#endif
