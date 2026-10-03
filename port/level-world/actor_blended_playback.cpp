#include "actor_blended_playback.hpp"
#include "../engine-animation/animation_blend.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>

namespace dh2::actor {
namespace {
float multiply(float a,float b){volatile float result=a*b;return result;}
float add(float a,float b){volatile float result=a+b;return result;}
struct EndingContext {BlendedPlayback* playback;std::uint32_t slot;};
void ending(void* context,timeline::State* state){
 const auto& c=*static_cast<EndingContext*>(context);
 // The timeline callback can run before final current_ms conversion. Preserve
 // the source extra here; an outgoing slot is not an actor completion.
 if(c.slot==c.playback->blend.current)dh2_timeline_notify(&c.playback->applicator_completion,state);
}
struct DispatchContext {
 BlendedPlayback* playback;BlendedEventObserver observer;
 std::int32_t clip;std::uint32_t slot,timestamp,phase;
};
void triggered(const animation::TriggeredEvent* input,void* context){
 auto& c=*static_cast<DispatchContext*>(context);EventHandoff handoff{};
 if(dh2_actor_event_handoff(&handoff,&c.playback->last_event_lag,input))return;
 const BlendedPlaybackEvent event{{handoff,c.clip,c.timestamp,c.phase,0},c.slot};
 if(c.observer.invoke)c.observer.invoke(c.observer.context,*c.playback,event);
}
struct ReplayContext {
 BlendedPlayback* playback;const ClipBank* bank;visual::SceneBinding* visual;
 scene::Scene* scene;std::string* error;bool success=true;
};
void replay(void* context,std::uint32_t event,timeline::State*,const timeline::ReplayResult* result){
 auto& c=*static_cast<ReplayContext*>(context);
 if(event!=timeline::new_animation)return;
 c.playback->displacement=result->displacement!=0;
 if(result->restart){
  ++c.playback->restarts;
  c.success=c.playback->replay_phase(result->timestamp,*c.bank,*c.visual,*c.scene,*c.error);
 }
}
void check_completion(BlendedPlayback& p){
 if(p.applicator_completion.pending){
  p.completion=p.applicator_completion;
  // CheckCallback clears the applicator flag after the character callback.
  // Character notification only raises pending; selection belongs after Step.
  p.applicator_completion.pending=0;
 }
}
}
bool BlendedPlayback::compile(const ClipBank& bank,const scene::Scene& authored,
                              const visual::SceneBinding& visual,std::string& error){
 std::vector<std::int32_t> order;for(const auto& item:bank)order.push_back(item.first);
 return compile(bank,order,authored,visual,error);
}
bool BlendedPlayback::compile(const ClipBank& bank,const std::vector<std::int32_t>& order,
                              const scene::Scene& authored,const visual::SceneBinding& visual,
                              std::string& error){
 if(scheduler.active()||observer.invoke){error="Blended resource compile requires detached playback";return false;}
 const auto node=visual.animated_node();
 if(node<0||std::size_t(node)>=authored.graph.size()){error="Blended reference node is absent";return false;}
 std::vector<animation::TransformClipInput> inputs;
 if(order.size()!=bank.size()){error="Blended registration order is incomplete";return false;}
 std::vector<std::int32_t> registered;
 for(auto id:order){const auto found=bank.find(id);
  if(found==bank.end()||std::find(registered.begin(),registered.end(),id)!=registered.end()){
   error="Blended registration identity rejected";return false;
  }
  registered.push_back(id);inputs.push_back({id,&found->second});
 }
 animation::TransformSet next;
 if(!next.compile(inputs,authored,error))return false;
 return bind_compiled(std::move(next),bank,authored,visual,false,error);
}
bool BlendedPlayback::compile_dynamic(const ClipBank& bank,const std::vector<std::int32_t>& order,
                                      const scene::Scene& scene_bindings,const visual::SceneBinding& visual,
                                      std::string& error,const animation::Player* default_library,
                                      animation::TransformMismatchBehavior mismatch){
 if(compiled_bank||scheduler.active()||observer.invoke){error="Dynamic compile requires fresh detached playback";return false;}
 if(order.empty()||order.size()!=bank.size()){error="Dynamic registration order is absent or incomplete";return false;}
 std::vector<animation::TransformClipInput> inputs;std::vector<std::int32_t> registered;
 for(auto id:order){const auto found=bank.find(id);
  if(found==bank.end()||std::find(registered.begin(),registered.end(),id)!=registered.end()){
   error="Dynamic registration identity rejected";return false;
  }
  registered.push_back(id);inputs.push_back({id,&found->second});
 }
 animation::TransformSet next;if(!next.compile_dynamic(inputs,scene_bindings,error,default_library,mismatch))return false;
 return bind_compiled(std::move(next),bank,scene_bindings,visual,true,error);
}
bool BlendedPlayback::compile_dynamic(const ClipBank& bank,const animation::RegistrationSet& registration,
                                      const scene::Scene& scene_bindings,const visual::SceneBinding& visual,
                                      std::string& error,animation::TransformMismatchBehavior mismatch){
 error.clear();
 if(compiled_bank||scheduler.active()||observer.invoke){error="Occurrence compile requires fresh detached playback";return false;}
 try{
  const auto& occurrences=registration.occurrences();const auto& entries=registration.entries();
  if(occurrences.empty()||occurrences.size()>1024||bank.empty()||
     !registration.default_player()||!registration.default_identity()){
   error="Occurrence registration/default binding is absent or incomplete";return false;
  }
  for(std::size_t i=0;i<occurrences.size();++i){const auto& item=occurrences[i];const auto found=bank.find(item.dictionary_id);
   const bool bank_player=std::any_of(bank.begin(),bank.end(),[&](const auto& resource){return &resource.second==item.player;});
   if(item.engine_index!=std::int32_t(i)||!item.resource_identity||!bank_player||
      (found!=bank.end()&&item.player!=&found->second)){
    error="Occurrence canonical Player binding rejected";return false;
   }
   for(std::size_t j=0;j<i;++j){const auto& prior=occurrences[j];
    if((prior.player==item.player&&prior.resource_identity!=item.resource_identity)||
       (prior.resource_identity==item.resource_identity&&prior.player!=item.player)){
     error="Occurrence canonical resource identity differs";return false;
    }
   }
  }
  for(const auto& resource:bank)if(std::none_of(occurrences.begin(),occurrences.end(),[&](const auto& item){return item.player==&resource.second;})){
   error="Occurrence bank contains an unregistered Player";return false;
  }
  std::int32_t prior_id=-1;
  for(const auto& entry:entries){
   const auto first_id=std::find_if(occurrences.begin(),occurrences.end(),[&](const auto& item){return item.dictionary_id==entry.dictionary_id;});
   const auto first_resource=std::find_if(occurrences.begin(),occurrences.end(),[&](const auto& item){return item.resource_identity==entry.resource_identity;});
   if(entry.dictionary_id<=prior_id||first_id==occurrences.end()||first_id->resource_identity!=entry.resource_identity||
      first_resource==occurrences.end()||entry.engine_index!=first_resource->engine_index||
      registration.lookup(entry.dictionary_id)!=entry.engine_index){
    error="Occurrence game map requires refreshed first resource indices";return false;
   }
   prior_id=entry.dictionary_id;
  }
  const auto default_entry=std::find_if(occurrences.begin(),occurrences.end(),[&](const auto& item){
   return item.player==registration.default_player()&&item.resource_identity==registration.default_identity();});
  if(default_entry==occurrences.end()){error="Occurrence default is not a canonical registered Player";return false;}
  animation::TransformSet next;
  if(!next.compile_dynamic(registration.compiled_inputs(),scene_bindings,error,registration.default_player(),mismatch))return false;
  BlendedPlayback candidate;candidate.game_registration=entries;
  for(const auto& item:occurrences)candidate.engine_dictionary_ids.push_back(item.dictionary_id);
  if(!candidate.bind_compiled(std::move(next),bank,scene_bindings,visual,true,error,true))return false;
  *this=std::move(candidate);return true;
 }catch(const std::exception& failure){error=failure.what();return false;}
}
std::int32_t BlendedPlayback::engine_index(std::int32_t id)const{
 if(engine_dictionary_ids.empty())return compiled.find_clip(id);
 const auto found=std::lower_bound(game_registration.begin(),game_registration.end(),id,
  [](const auto& entry,std::int32_t key){return entry.dictionary_id<key;});
 return found!=game_registration.end()&&found->dictionary_id==id?found->engine_index:-1;
}
std::int32_t BlendedPlayback::dictionary_id(std::int32_t index)const{
 if(index<0)return -1;
 if(!engine_dictionary_ids.empty())return std::size_t(index)<engine_dictionary_ids.size()?engine_dictionary_ids[index]:-1;
 const auto* clip=compiled.clip(index);return clip?clip->id:-1;
}
bool BlendedPlayback::bind_compiled(animation::TransformSet&& next,const ClipBank& bank,
                                    const scene::Scene& authored,const visual::SceneBinding& visual,
                                    bool initial_library_zero,std::string& error,bool occurrence_mapping){
 const auto node=visual.animated_node();
 if(node<0||std::size_t(node)>=authored.graph.size()){error="Blended reference node is absent";return false;}
 std::vector<TargetValues> buffers;std::int32_t reference=-1;
 for(std::size_t i=0;i<next.targets().size();++i){
  const auto& target=next.targets()[i];
  if((target.type!=1&&target.type!=5&&target.type!=10)||
     target.components!=(target.type==5?4u:3u)){
   error="Blended target requires an unreconstructed applicator";return false;
  }
  buffers.push_back({std::vector<float>(target.components*2,0.f)});
  if(target.type==1&&target.node==std::uint32_t(node))reference=static_cast<std::int32_t>(i);
 }
 std::vector<std::string> identities;for(const auto& n:authored.graph)identities.push_back(n.id);
 if(!occurrence_mapping){game_registration.clear();engine_dictionary_ids.clear();}
 compiled=std::move(next);compiled_bank=&bank;target_values=std::move(buffers);
 node_identities=std::move(identities);root_target=reference;
 target_enabled.assign(compiled.targets().size(),1);
 for(auto& slot:slots)slot.key_cursors.assign(compiled.targets().size(),0);
 if(initial_library_zero){
  const auto* first=compiled.clip(0);if(!first){error="Dynamic library0 is absent";return false;}
  for(auto& slot:slots){
   slot.timeline={};slot.timeline.scale=1;slot.timeline.library_present=1;slot.timeline.loop=1;
   if(dh2_timeline_clip(&slot.timeline,0,first->start,first->end))return false;
   slot.clip_id=dictionary_id(0);slot.compiled_clip=0;slot.event_cursor={};slot.generation=0;slot.root_history={};
  }
 }
 return true;
}
bool BlendedPlayback::ready(const ClipBank& bank,const visual::SceneBinding& visual,
                            const scene::Scene& scene,std::string& error)const{
 if(compiled_bank!=&bank||blend.current>=2||blend.previous>=2||
    target_enabled.size()!=compiled.targets().size()||scene.graph.size()!=node_identities.size()||
    visual.animated_node()<0||std::size_t(visual.animated_node())>=scene.graph.size()){
  error="Blended playback resource binding is absent or inconsistent";return false;
 }
 for(std::size_t i=0;i<node_identities.size();++i)if(scene.graph[i].id!=node_identities[i]){
  error="Blended scene node identity differs";return false;
 }
 return true;
}
bool BlendedPlayback::set_speed(float speed,std::string& error){
 if(!std::isfinite(speed)||speed<0){error="Animation global speed rejected";return false;}
 const float product=multiply(speed,scheduler.clip().speed);
 if(!std::isfinite(product)){error="Animation clip speed rejected";return false;}
 // BlendedAnimSetController::SetScale -> AnimatorBlender::SetScale visits ALL
 // timelines, including zero-weight outgoing slots.
 for(auto& slot:slots)if(dh2_timeline_scale(&slot.timeline,product)){
  error="Blended timeline scale rejected";return false;
 }
 global_speed=speed;return true;
}
bool BlendedPlayback::apply_selection(const ClipBank& bank,visual::SceneBinding& visual,
                                     scene::Scene& scene,std::string& error){
 if(!ready(bank,visual,scene,error))return false;
 // Blend precedes resource lookup, including its failure side effect.
 if(dh2_blender_begin(&blend,scheduler.clip().blend_out)){error="Blended slot transition rejected";return false;}
 auto& slot=slots[blend.current];const auto previous=slot.compiled_clip;
 const auto mapped=scheduler.clip().anim;const auto index=engine_index(mapped);
 if(index<0){error="Authored blended clip is not compiled";return false;}
 const auto* clip=compiled.clip(index);
 if(!clip||dh2_timeline_clip(&slot.timeline,index,clip->start,clip->end)){
  error="Authored blended timeline clip rejected";return false;
 }
 sequence_closed=0; // valid visual/clip branch, after selection24/26 services
 slot.clip_id=mapped;slot.compiled_clip=index;slot.event_cursor={};++slot.generation;
 // Source PlayClip tests incoming IsEnded (v44), not its loop flag. Selection
 // above cleared ended; same-ID replay jumps even if that slot previously looped.
 if(previous==index&&!slot.timeline.ended){const auto word=std::uint32_t(slot.timeline.start_ms)+std::uint32_t(applicator_completion.extra_ms);
  std::int32_t ms;std::memcpy(&ms,&word,4);if(dh2_timeline_jump(&slot.timeline,ms))return false;
 }
 // Keep the historical standalone replay helper intact, but bypass its old
 // same-ID/loop fixture gate after executing the source IsEnded branch here.
 const timeline::ReplayFacts facts{-1,mapped,applicator_completion.extra_ms,0,
                                  unsigned(scheduler.clip().move_go),1,root_timestamp,0};
 ReplayContext context{this,&bank,&visual,&scene,&error};
 const timeline::ReplayServices services{&context,replay};timeline::ReplayResult result{};
 if(dh2_timeline_replay(&result,&slot.timeline,&facts,&services)!=1||!context.success)return false;
 // Read LIVE speed and scheduler step after synchronous NewAnim/event reentry.
 return set_speed(global_speed,error);
}
void BlendedPlayback::scheduler_event(std::uint32_t id){
 EventHandoff handoff{id,0,nullptr};
 if(id==0x22&&dh2_actor_sequence_close(&handoff,&sequence_closed)!=1)return;
 const auto phase=id==0x22?std::uint32_t(sequence_event):
                  (id==0x24||id==0x26)?std::uint32_t(selection_event):std::uint32_t(animator_event);
 const BlendedPlaybackEvent event{{handoff,current_clip(),root_timestamp,phase,0},blend.current};
 const auto callback=observer;if(callback.invoke)callback.invoke(callback.context,*this,event);
}
bool BlendedPlayback::start(const data::AnimationTables& tables,int sequence,data::AnimationRandom& random,
                            const ClipBank& bank,visual::SceneBinding& visual,scene::Scene& scene,
                            float speed,std::string& error){
 if(!std::isfinite(speed)||speed<0){error="Animation global speed rejected";return false;}
 if(completion.pending){
  if(sequence<0||std::size_t(sequence)>=tables.sequences.size()){error="Pending animation selection rejected";return false;}
  pending_sequence=sequence;pending_speed=speed;return true;
 }
 if(!ready(bank,visual,scene,error))return false;
 completion={};global_speed=speed;
 ReplayContext context{this,&bank,&visual,&scene,&error};
 const data::AnimationSelectionServices services{
  &context,
  [](void* raw,data::AnimationScheduler&,std::uint32_t id){static_cast<ReplayContext*>(raw)->playback->scheduler_event(id);},
  [](void* raw,data::AnimationScheduler&){auto& c=*static_cast<ReplayContext*>(raw);return c.playback->apply_selection(*c.bank,*c.visual,*c.scene,*c.error);}
 };
 return scheduler.start_with_services(tables,sequence,random,error,services);
}
bool BlendedPlayback::swap(const data::AnimationTables& tables,int desired,int old,data::AnimationRandom& random,
                           const ClipBank& bank,visual::SceneBinding& visual,scene::Scene& scene,
                           float speed,std::string& error){
 const auto result=scheduler.swap_sequences(tables,desired,old,error);
 if(result==data::AnimationSwap::rejected)return false;
 if(result==data::AnimationSwap::restart)return start(tables,desired,random,bank,visual,scene,speed,error);
 return true;
}
std::uint32_t BlendedPlayback::step_count(const data::AnimationTables& tables)const{
 if(sequence_closed||scheduler.frames().empty())return 0;
 const auto id=scheduler.frames().back().sequence;
 return id<0||std::size_t(id)>=tables.sequences.size()?0:std::uint32_t(tables.sequences[id].steps.size());
}
void BlendedPlayback::stop_loop(bool complete_next_update){
 if(sequence_closed)return;
 scheduler.stop_loop();if(complete_next_update)stop_requested=true;
}
bool BlendedPlayback::advance_slot(std::uint32_t index,std::uint32_t absolute,std::uint32_t phase,std::string& error){
 auto& slot=slots[index];const auto* retained=compiled.clip(slot.compiled_clip);
 if(!retained)return true; // a constructed, unselected slot owns an empty manager
 const auto previous=slot.timeline.current_ms;const auto generation=slot.generation;
 const auto clip=slot.clip_id;const auto old_view=retained->events.view();
 const auto start=slot.timeline.start_ms,end=slot.timeline.end_ms;
 std::int32_t signed_time;std::memcpy(&signed_time,&absolute,4);
 EndingContext ending_context{this,index};const timeline::Services services{&ending_context,ending};
 if(dh2_timeline_update(&slot.timeline,signed_time,&services)){error="Blended scene timeline rejected";return false;}
 if(observer.invoke){
  auto cursor=slot.event_cursor;DispatchContext context{this,observer,clip,index,absolute,phase};
  if(!dh2_events_update(&old_view,&cursor,previous,slot.timeline.current_ms,start,end,triggered,&context)){
   error="Blended authored event track rejected";return false;
  }
  // An old manager finishes its retained batch. It must not replace a newly
  // installed cursor after a nested source PlayClip selects this same slot.
  if(slot.generation==generation)slot.event_cursor=cursor;
 }
 return true;
}
bool BlendedPlayback::reset_current(std::uint32_t absolute,std::string& error){
 auto& slot=slots[blend.current];float point[3]{};
 const auto* clip=compiled.clip(slot.compiled_clip);if(!clip)return true;
 if(root_target>=0&&!compiled.sample(slot.compiled_clip,root_target,clip->start,point,3,
                     &slot.key_cursors[root_target],error))return false;
 return dh2_visual_reset_delta(&slot.root_history,absolute+1,point)==0;
}
bool BlendedPlayback::animate(std::uint32_t absolute,bool reset,std::uint32_t phase,
                              const ClipBank& bank,visual::SceneBinding& visual,scene::Scene& scene,
                              std::string& error){
 if(!ready(bank,visual,scene,error)||current_clip()<0){error="Blended character clip is absent";return false;}
 if(reset&&!reset_current(absolute,error))return false;
 if(dh2_blender_update_weights(&blend,absolute)){error="Blended scene fade rejected";return false;}
 for(std::uint32_t index=0;index<2;++index){
  // Re-read each live gate AFTER any preceding slot's synchronous callbacks.
  if(blend.weights[index]==0.f)continue;
  if(!advance_slot(index,absolute,phase,error))return false;
  auto& slot=slots[index];if(slot.compiled_clip<0)continue;
  for(std::size_t target=0;target<compiled.targets().size();++target){
   const auto& descriptor=compiled.targets()[target];
   if(!target_enabled[target]||descriptor.node==UINT32_MAX)continue;
   auto* out=target_values[target].values.data()+index*descriptor.components;
   if(!compiled.sample(slot.compiled_clip,target,slot.timeline.current_ms,out,descriptor.components,
                       &slot.key_cursors[target],error))return false;
  }
 }
 if(dh2_blender_normalize(&blend)){error="Blended weights rejected";return false;}
 for(std::size_t target=0;target<compiled.targets().size();++target){
  const auto& descriptor=compiled.targets()[target];
  if(!target_enabled[target]||descriptor.node==UINT32_MAX)continue;
  if(descriptor.node>=scene.graph.size()){error="Blended target node is absent";return false;}
  float value[4]{};const auto* input=target_values[target].values.data();
  const auto result=descriptor.type==5?dh2_animation_blend_quaternion(value,input,blend.weights,2):
                                     dh2_animation_blend_vector3(value,input,blend.weights,2);
  if(result){error="Typed blended contribution rejected";return false;}
  auto& node=scene.graph[descriptor.node];
  auto* out=descriptor.type==1?node.translation:descriptor.type==5?node.quaternion:node.scale;
  std::copy(value,value+descriptor.components,out);
 }
 float scratch[3]{},sum[3]{};
 for(std::uint32_t index=0;root_target>=0&&index<2;++index){
  auto& slot=slots[index];
  // One scratch survives the ALL-slot loop. A null/default-less record retains
  // bytes produced by the preceding slot, even when this slot's weight is zero.
  if(root_target>=0&&slot.compiled_clip>=0&&
     !compiled.sample(slot.compiled_clip,root_target,slot.timeline.current_ms,scratch,3,
                      &slot.key_cursors[root_target],error))return false;
  if(dh2_visual_calculate_delta(&slot.root_history,absolute,scratch))return false;
  const auto weight=blend.weights[index];
  sum[1]=add(sum[1],multiply(weight,slot.root_history.value[1]));
  sum[2]=add(sum[2],multiply(weight,slot.root_history.value[2]));
  sum[0]=add(sum[0],multiply(weight,slot.root_history.value[0]));
 }
 aggregate.timestamp=absolute;std::copy(sum,sum+3,aggregate.value);
 const auto reference=visual.animated_node();
 std::copy(scene.graph[reference].translation,scene.graph[reference].translation+3,visual.root.animated);
 if(displacement){const visual::Displacement request{&visual.root,sum,1,0};
  if(dh2_visual_displace(&request)<0){error="Blended root displacement rejected";return false;}
 }
 if(!visual.update_world(scene,error))return false;
 check_completion(*this);blend.last_time=absolute;return true;
}
bool BlendedPlayback::scene_phase(std::uint32_t absolute,const ClipBank& bank,
                                  visual::SceneBinding& visual,scene::Scene& scene,std::string& error){
 if(!animate(absolute,false,scene_event,bank,visual,scene,error))return false;
 root_timestamp=absolute;return true;
}
bool BlendedPlayback::replay_phase(std::uint32_t absolute,const ClipBank& bank,
                                   visual::SceneBinding& visual,scene::Scene& scene,std::string& error){
 return animate(absolute,true,replay_event,bank,visual,scene,error);
}
bool BlendedPlayback::time_phase(std::uint32_t absolute,const ClipBank& bank,
                                 visual::SceneBinding& visual,scene::Scene& scene,std::string& error){
 if(!ready(bank,visual,scene,error)||current_clip()<0)return false;
 if(dh2_blender_update_weights(&blend,absolute))return false;
 for(std::uint32_t i=0;i<2;++i)if(blend.weights[i]!=0.f&&!advance_slot(i,absolute,scene_event,error))return false;
 if(dh2_blender_normalize(&blend))return false;
 check_completion(*this);blend.last_time=absolute;return true;
}
bool BlendedPlayback::animator_phase(const data::AnimationTables& tables,data::AnimationRandom& random,
                                     const ClipBank& bank,visual::SceneBinding& visual,scene::Scene& scene,
                                     float speed,std::int32_t extra,std::string& error){
 if(stop_requested){stop_requested=false;completion.pending=1;}
 if(!completion.pending)return set_speed(speed,error);
 if(!std::isfinite(speed)||speed<0){error="Animation global speed rejected";return false;}
 global_speed=speed;completion.extra_ms=extra;++completions;
 struct Context {BlendedPlayback* p;const data::AnimationTables* tables;data::AnimationRandom* random;
                 const ClipBank* bank;visual::SceneBinding* visual;scene::Scene* scene;std::string* error;};
 Context context{this,&tables,&random,&bank,&visual,&scene,&error};
 const data::AnimationSelectionServices selection{
  &context,
  [](void* raw,data::AnimationScheduler&,std::uint32_t id){static_cast<Context*>(raw)->p->scheduler_event(id);},
  [](void* raw,data::AnimationScheduler&){auto& c=*static_cast<Context*>(raw);return c.p->apply_selection(*c.bank,*c.visual,*c.scene,*c.error);}
 };
 const data::AnimationCompletionServices services{
  &context,
  [](void* raw,data::AnimationScheduler&,std::uint32_t id){
   static_cast<Context*>(raw)->p->scheduler_event(id);
  },
  [](void* raw){return static_cast<Context*>(raw)->p->pending_sequence!=-1;},
  [](void* raw,data::AnimationScheduler&){auto& c=*static_cast<Context*>(raw);
   return c.p->apply_selection(*c.bank,*c.visual,*c.scene,*c.error);},
  [](void* raw,data::AnimationScheduler&){auto& c=*static_cast<Context*>(raw);auto& p=*c.p;
   // This tail also runs inside recursive parent Update, before child return.
   p.completion.pending=0;
   if(p.pending_sequence==-1)return true;
   const auto sequence=p.pending_sequence;const auto selected_speed=p.pending_speed;
   const auto success=p.start(*c.tables,sequence,*c.random,*c.bank,*c.visual,*c.scene,selected_speed,*c.error);
   // Source clears seq50 AFTER ANIM_Set/NewAnim returns. A replay callback
   // that reasserts pending and queues another sequence is cleared here too.
   p.pending_sequence=-1;return success;
  },nullptr,&selection
 };
 return scheduler.complete_with_services(tables,random,error,services);
}
}
