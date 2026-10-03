#include "../actor_blended_playback.hpp"
#include "../../engine-animation/animation_blend.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
using namespace dh2;
namespace {
void check(bool b,const std::string& message){if(!b)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing fixture "+path);return {std::istreambuf_iterator<char>(f),{}};}
data::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
template<class T>void write(std::ofstream& f,const T& v){f.write(reinterpret_cast<const char*>(&v),sizeof(v));}
void floats(std::ofstream& f,const float* p,std::size_t n){f.write(reinterpret_cast<const char*>(p),n*4);}
void pose(std::ofstream& f,const scene::Scene& s){for(const auto& n:s.graph){floats(f,n.translation,3);floats(f,n.quaternion,4);floats(f,n.scale,3);}}
bool equal(float a,float b){return a==b||(std::isnan(a)&&std::isnan(b));}
struct Context {
 const data::AnimationTables* tables;data::AnimationRandom* random;const actor::ClipBank* bank;
 visual::SceneBinding* binding;scene::Scene* scene;float speed=1;
 unsigned authored=0,closures=0,replayed=0,transitions=0;
 bool idle_on_close=false,switch_trigger=false;std::string error;
 bool stop_loop_on_selection=false,reassert_on_selection=false;
 std::vector<std::pair<unsigned,std::string>> events;
};
void observe(void* raw,actor::BlendedPlayback& p,const actor::BlendedPlaybackEvent& event){
 auto& c=*static_cast<Context*>(raw);const auto& e=event.event;
 check(event.slot<2,"Authored observer slot identity differs");
 if(e.handoff.event_id==0x28){
  check(e.handoff.payload&&p.last_event_lag==e.handoff.lag_ms,"Authored lag/payload handoff differs");
  ++c.authored;c.replayed+=e.phase==actor::replay_event;c.events.emplace_back(event.slot,e.handoff.payload);
  if(c.switch_trigger&&std::string(e.handoff.payload)=="first"){
   c.switch_trigger=false;
   check(p.start(*c.tables,259,*c.random,*c.bank,*c.binding,*c.scene,c.speed,c.error),c.error);
   check(p.current_clip()==1023,"Synchronous event failed to select Died");++c.transitions;
  }
 }else if(e.handoff.event_id==0x24||e.handoff.event_id==0x26){
  check(!e.handoff.payload&&e.phase==actor::selection_event,"Source selection callback choreography differs");
  if(c.stop_loop_on_selection&&e.handoff.event_id==0x26){c.stop_loop_on_selection=false;p.scheduler.stop_loop();}
  if(c.reassert_on_selection&&e.handoff.event_id==0x24){
   c.reassert_on_selection=false;p.completion.pending=1;
   check(p.start(*c.tables,259,*c.random,*c.bank,*c.binding,*c.scene,c.speed,c.error),c.error);
  }
 }else if(e.handoff.event_id!=0x22){
  check((e.handoff.event_id==0x27||e.handoff.event_id==0x25||e.handoff.event_id==0x23)&&
        !e.handoff.payload&&e.phase==actor::animator_event&&p.completion.pending,
        "Source animator callback choreography differs");
 }else{
  check(e.handoff.event_id==0x22&&!e.handoff.payload&&e.phase==actor::sequence_event,"Finite closure handoff differs");
  check(!p.scheduler.active()&&p.sequence_closed==1,"Closure not marked before observer");++c.closures;
  if(c.idle_on_close){const auto prior=p.current_clip();c.idle_on_close=false;
   check(p.start(*c.tables,262,*c.random,*c.bank,*c.binding,*c.scene,c.speed,c.error),c.error);
   check(p.current_clip()==prior,"Closure selection was not deferred until callback return");++c.transitions;
  }
 }
}
struct Capture {
 std::ofstream file;unsigned count=0;
 explicit Capture(const std::string& path,const actor::BlendedPlayback& p,const scene::Scene& scene):file(path,std::ios::binary){
  check(bool(file),"Cannot write composition fixtures");file.write("BPF1",4);write(file,count);
  write(file,std::uint32_t(p.transform_set().targets().size()));write(file,std::uint32_t(scene.graph.size()));
  std::int32_t root=-1;for(std::size_t i=0;i<p.transform_set().targets().size();++i){const auto& t=p.transform_set().targets()[i];if(t.node==33&&t.type==1)root=int(i);}
  write(file,root);
  for(const auto& t:p.transform_set().targets()){write(file,t.type);write(file,t.node);write(file,t.components);}
 }
 void frame(actor::BlendedPlayback& p,std::uint32_t now,const actor::ClipBank& bank,
            visual::SceneBinding& binding,scene::Scene& scene,std::string& error,bool time=false){
  // Sampler services in the original composition oracle receive these real
  // compiled-asset buffers. Raw accessor correctness is separately audited by
  // TransformSet's original-instruction corpus; this is composition evidence.
  write(file,now);write(file,std::uint32_t(time));write(file,p.blend);
  for(const auto& slot:p.slots)write(file,slot.root_history);
  floats(file,p.aggregate.value,3);
  for(std::size_t i=0;i<p.transform_set().targets().size();++i)floats(file,p.values(i).data(),p.values(i).size());
  pose(file,scene);
  for(auto enabled:p.target_enabled)write(file,std::uint32_t(enabled));
  const auto old_history=std::array<visual::Delta,2>{p.slots[0].root_history,p.slots[1].root_history};
  const auto old_aggregate=p.aggregate;const auto old_root=binding.root;
  check(time?p.time_phase(now,bank,binding,scene,error):p.scene_phase(now,bank,binding,scene,error),error);
  for(std::size_t i=0;i<p.transform_set().targets().size();++i)floats(file,p.values(i).data(),p.values(i).size());
  float scratch[3]{};int reference=-1;
  for(std::size_t i=0;i<p.transform_set().targets().size();++i)if(p.transform_set().targets()[i].type==1&&p.transform_set().targets()[i].node==std::uint32_t(binding.animated_node()))reference=int(i);
  for(unsigned i=0;i<2;++i){const auto& slot=p.slots[i];if(reference>=0&&slot.compiled_clip>=0){auto cursor=slot.key_cursors[reference];check(p.transform_set().sample(slot.compiled_clip,reference,slot.timeline.current_ms,scratch,3,&cursor,error),error);}floats(file,scratch,3);}
  write(file,p.blend);for(const auto& slot:p.slots)write(file,slot.root_history);
  floats(file,p.aggregate.value,3);pose(file,scene);
  if(time){check(!std::memcmp(&old_history[0],&p.slots[0].root_history,28)&&!std::memcmp(&old_history[1],&p.slots[1].root_history,28)&&!std::memcmp(&old_aggregate,&p.aggregate,28)&&!std::memcmp(&old_root,&binding.root,96),"Time-only path changed root history/owner");}
  ++count;
 }
 void finish(){file.seekp(4);write(file,count);file.close();}
};
int direct(data::AnimationTables& tables,int clip,int blend,int loop=0,bool move=true){
 data::AnimationSequence sequence;sequence.type=0;sequence.loop=loop;sequence.steps.resize(1);
 sequence.steps[0].anim=clip;sequence.steps[0].blend_out=blend;sequence.steps[0].move_go=move;
 tables.sequences.push_back(sequence);return int(tables.sequences.size()-1);
}
}
int main(int argc,char** argv){try{
 if(argc!=3&&argc!=4)return 2;const bool dynamic=argc==4&&std::string(argv[3])=="--dynamic";if(argc==4&&!dynamic)return 2;const std::string assets=argv[1];std::string error;
 auto a=read(assets+"/data/animations_pyarray.bin"),b=read(assets+"/data/animations_pyarraynames.bin"),c=read(assets+"/data/animations_pystructnames.bin"),d=read(assets+"/data/animations_dictionary_pyarraynames.bin"),e=read(assets+"/data/animations_dictionary_pyarray.bin");
 data::Dictionary dictionary;data::AnimationTables tables;
 check(data::load_dictionary(bytes(d),bytes(e),dictionary,error)&&data::load_animation_tables(bytes(a),bytes(b),bytes(c),dictionary,tables,error),error);
 auto raw=read(assets+"/models/prince_modular.bdae");resources::BresView view{};scene::Scene rest;
 check(dh2_bres_open(&view,raw.data(),raw.size())==resources::BresError::ok&&scene::load(view,rest,error),error);
 check(rest.graph.size()==35,"Prince graph node count differs");std::set<int> ids{955,956,957,958,959,960,961,962,963,967,969,971,1023,1040,1041,1114,1126};
 actor::ClipBank bank;for(auto id:ids){const auto name=dictionary.values.at(id);auto image=read(assets+"/animations/"+name.substr(name.find_last_of("/\\")+1));animation::Player player;
  check(player.load(image.data(),image.size(),rest,error),error);check(!player.unbound&&!player.skipped,"Unsupported real Prince track");bank.emplace(id,std::move(player));}
 // Model/template is an explicit caller registration fixture. Key20000 is a
 // native fixture identity, not a recovered original dictionary/template ID.
 const int template_id=20000;
 if(dynamic){animation::Player player;check(player.load(raw.data(),raw.size(),rest,error),error);check(!player.track_count(),"Model default fixture unexpectedly owns animation tracks");bank.emplace(template_id,std::move(player));}
 std::vector<std::int32_t> order;if(dynamic)order.push_back(template_id);for(const auto& row:bank)if(row.first!=template_id)order.push_back(row.first);
 auto setup=[&](actor::BlendedPlayback& p,const scene::Scene& authored,const visual::SceneBinding& v){return dynamic?p.compile_dynamic(bank,order,authored,v,error,&bank.at(template_id)):p.compile(bank,authored,v,error);};
 auto live=rest;visual::SceneBinding binding;check(binding.bind(live,error),error);actor::BlendedPlayback prototype;check(setup(prototype,rest,binding),error);
 check(prototype.sequence_closed==1&&prototype.step_index()==UINT32_MAX&&prototype.step_count(tables)==0,"Source CharAnimator constructor closed/getters differ");
 if(dynamic)for(const auto& slot:prototype.slots)check(slot.clip_id==template_id&&slot.compiled_clip==0&&slot.timeline.loop==1&&slot.timeline.initialized==0,"Source constructor library0/loop1 initialization differs");
 Capture capture(argv[2],prototype,live);
 unsigned frames=0,root_samples=0,closures=0,authored=0,replayed=0,transitions=0,time_only=0,metadata_swaps=0,terminal=0,speed_checks=0,zero_weight_histories=0;
 for(float speed:{1.f,1.3f})for(int sequence:{262,280,271,243,259}){
  actor::BlendedPlayback p;data::AnimationRandom random;live=rest;binding=visual::SceneBinding{};check(binding.bind(live,error)&&setup(p,rest,binding),error);
  Context context{&tables,&random,&bank,&binding,&live};context.speed=speed;p.observer={&context,observe};
  check(p.start(tables,sequence,random,bank,binding,live,speed,error),error);
  for(unsigned frame=0;frame<250;++frame){const auto now=1000+frame*37;
   if(frame<24||frame%31==0)capture.frame(p,now,bank,binding,live,error);
   else check(p.scene_phase(now,bank,binding,live,error),error);
   ++frames;++root_samples;
   for(const auto& slot:p.slots)if(p.blend.weights[&slot-&p.slots[0]]==0.f){check(slot.root_history.timestamp==now,"Zero-weight slot history was not sampled");++zero_weight_histories;}
   for(const auto& slot:p.slots)check(slot.timeline.scale==speed*p.scheduler.clip().speed,"Current step speed not propagated to both timelines");++speed_checks;
   const auto extra=p.completion.extra_ms;check(p.animator_phase(tables,random,bank,binding,live,speed,extra,error),error);
   terminal+=!p.scheduler.active();
   for(const auto& node:live.graph)for(float f:node.world)check(std::isfinite(f),"Nonfinite blended world matrix");
  }
  if(sequence==243||sequence==259)check(context.closures==1,"Finite authored sequence closure count differs");
  closures+=context.closures;authored+=context.authored;replayed+=context.replayed;
 }
 // Genuine Idle/Walk/Run transitions: old authored Idle BlendOut100 becomes
 // incoming Move fade duration; subsequent Move BlendOut0 is retained for later.
 for(float speed:{1.f,1.3f}){
  actor::BlendedPlayback p;data::AnimationRandom random;live=rest;binding=visual::SceneBinding{};check(binding.bind(live,error)&&setup(p,rest,binding),error);
  for(int sequence:{262,280,271,262}){const auto outgoing=p.blend.current;const auto history=p.slots[outgoing].root_history;
   check(p.start(tables,sequence,random,bank,binding,live,speed,error),error);
   if(p.root_timestamp)check(history.timestamp==p.slots[outgoing].root_history.timestamp&&!std::memcmp(history.previous,p.slots[outgoing].root_history.previous,12),"NewAnim reset outgoing history at same timestamp");
   for(unsigned frame=0;frame<10;++frame){const auto now=1000+transitions*400+frame*9;capture.frame(p,now,bank,binding,live,error);++frames;
    check(p.animator_phase(tables,random,bank,binding,live,speed,p.completion.extra_ms,error),error);}
   ++transitions;
  }
 }
 // Time-only path keeps pose/root bytes and owner clock while owning separate
 // slot timeline/event progress. No additional root integration is fabricated.
 {
  actor::BlendedPlayback p;data::AnimationRandom random;live=rest;binding=visual::SceneBinding{};check(binding.bind(live,error)&&setup(p,rest,binding),error);
  check(p.start(tables,280,random,bank,binding,live,1,error)&&p.scene_phase(1000,bank,binding,live,error),error);
  const auto owner_timestamp=p.root_timestamp;
  for(unsigned i=0;i<20;++i){capture.frame(p,1010+i*13,bank,binding,live,error,true);++time_only;check(p.root_timestamp==owner_timestamp,"Time-only path advanced owner scene timestamp");}
 }
 // Accepted finite closure callback may synchronously ANIM_Set Idle. Source
 // pending selection is held until callback return, then NewAnim runs immediately.
 for(float speed:{1.f,1.3f}){
  actor::BlendedPlayback p;data::AnimationRandom random;live=rest;binding=visual::SceneBinding{};check(binding.bind(live,error)&&setup(p,rest,binding),error);
  Context context{&tables,&random,&bank,&binding,&live};context.speed=speed;context.idle_on_close=true;p.observer={&context,observe};
  check(p.start(tables,243,random,bank,binding,live,speed,error),error);
  for(unsigned i=0;i<350;++i){check(p.scene_phase(1000+i*37,bank,binding,live,error),error);++frames;check(p.animator_phase(tables,random,bank,binding,live,speed,p.completion.extra_ms,error),error);}
  check(context.closures==1&&context.transitions==1&&p.scheduler.active()&&(p.current_clip()==1040||p.current_clip()==1041),"Deferred Idle closure selection differs");closures+=context.closures;transitions+=context.transitions;authored+=context.authored;replayed+=context.replayed;
 }
 // Explicit source services reproduce two independently observed original
 // counterexamples. A queued between-phase selection cannot preempt closure;
 // a new queue written during consumed selection is cleared AFTER it returns.
 unsigned callback_tail_regressions=0;
 {
  actor::BlendedPlayback p;data::AnimationRandom random;live=rest;binding=visual::SceneBinding{};check(binding.bind(live,error)&&setup(p,rest,binding),error);
  Context context{&tables,&random,&bank,&binding,&live};p.observer={&context,observe};
  check(p.start(tables,259,random,bank,binding,live,1,error),error);const auto old=p.current_clip();p.completion.pending=1;
  check(p.start(tables,262,random,bank,binding,live,1,error)&&p.current_clip()==old,"Between-phase selection bypassed pending49");
  context.reassert_on_selection=true;check(p.animator_phase(tables,random,bank,binding,live,1,0,error),error);
  check(context.closures==1&&p.completion.pending&&(p.current_clip()==1040||p.current_clip()==1041),"Closure/consumed selection pending lifetime differs");
  check(p.animator_phase(tables,random,bank,binding,live,1,0,error),error);
  check(p.current_clip()!=1023&&!p.completion.pending,"Reasserted queue survived source clear-after-return");++callback_tail_regressions;
 }
 {
  auto fixture=tables;const auto seq=direct(fixture,1114,0,2);actor::BlendedPlayback p;data::AnimationRandom random;live=rest;binding=visual::SceneBinding{};check(binding.bind(live,error)&&setup(p,rest,binding),error);
  Context context{&fixture,&random,&bank,&binding,&live};p.observer={&context,observe};check(p.start(fixture,seq,random,bank,binding,live,1,error),error);
  p.completion.pending=1;context.stop_loop_on_selection=true;check(p.animator_phase(fixture,random,bank,binding,live,1,0,error),error);
  check(p.scheduler.frames().front().loops==1,"Captured repeat count was not restored after synchronous StopLoop");++callback_tail_regressions;
 }
 // Both managers deliver unscaled events in slot order. Synthetic immutable
 unsigned forced_stop_checks=0;
 {
  actor::BlendedPlayback p;data::AnimationRandom random;live=rest;binding=visual::SceneBinding{};check(binding.bind(live,error)&&setup(p,rest,binding),error);
  Context context{&tables,&random,&bank,&binding,&live};p.observer={&context,observe};check(p.start(tables,259,random,bank,binding,live,1,error),error);
  check(!p.completion.pending,"Initial authored selection unexpectedly completed");p.stop_loop(true);p.stop_loop(false);
  check(p.stop_requested,"False StopLoop cleared prior true flag");check(p.animator_phase(tables,random,bank,binding,live,1,0,error),error);
  check(!p.stop_requested&&!p.completion.pending&&context.closures==1&&p.sequence_closed,"StopLoop true did not force next animator update before pending gate");++forced_stop_checks;
  const auto before=p.current_timeline();p.set_step(99);p.skip_next_step();p.stop_loop(true);
  check(p.step_index()==UINT32_MAX&&p.step_count(tables)==0&&!p.stop_requested&&!std::memcmp(&before,&p.current_timeline(),sizeof(before)),"Closed animator controls altered terminal timeline");++forced_stop_checks;
 }
 // Both managers deliver unscaled events in slot order. Synthetic immutable
 // track names test retained batches; actual Prince clip pose resources remain.
 unsigned two_slot_events=0,retained_events=0;
 for(unsigned reentry=0;reentry<2;++reentry){
  auto fixture=tables;const auto seq=direct(fixture,955,0,-1);actor::BlendedPlayback p;data::AnimationRandom random;live=rest;binding=visual::SceneBinding{};check(binding.bind(live,error)&&setup(p,rest,binding),error);
  auto& track=const_cast<animation::EventView&>(p.transform_set().clip(p.transform_set().find_clip(955))->events.view());
  const std::int32_t keys[]={300,333};const char* one[]={"first","second"};const char* two[]={"third"};const animation::EventGroup groups[]={{2,one},{1,two}};track={4,2,reinterpret_cast<const std::uint8_t*>(keys),groups};
  Context context{&fixture,&random,&bank,&binding,&live};p.observer={&context,observe};
  check(p.start(fixture,seq,random,bank,binding,live,1,error)&&p.start(fixture,seq,random,bank,binding,live,1,error),error);
  p.blend.remaining=-1;p.blend.weights[0]=.25f;p.blend.weights[1]=.75f;
  check(p.scene_phase(1000,bank,binding,live,error),error);context.events.clear();context.switch_trigger=reentry!=0;
  check(p.scene_phase(1200,bank,binding,live,error),error);
  if(!reentry){check(context.events==std::vector<std::pair<unsigned,std::string>>{{0,"first"},{0,"second"},{0,"third"},{1,"first"},{1,"second"},{1,"third"}},"Both slot managers were filtered/scaled/reordered");two_slot_events=context.events.size();}
  else{
   const auto first=std::find(context.events.begin(),context.events.end(),std::make_pair(0u,std::string("first")));
   check(first!=context.events.end()&&std::find(first,context.events.end(),std::make_pair(0u,std::string("second")))!=context.events.end()&&std::find(first,context.events.end(),std::make_pair(0u,std::string("third")))!=context.events.end(),"Retained old manager dropped tail events after nested selection");
   check(context.transitions==1&&p.current_clip()==1023,"Event reentry failed live clip re-fetch");retained_events=3;transitions+=context.transitions;
  }
 }
 // ANIM_Swap changes stack metadata without choosing/replaying the active slot.
 {
  actor::BlendedPlayback p;data::AnimationRandom random;live=rest;binding=visual::SceneBinding{};check(binding.bind(live,error)&&setup(p,rest,binding),error);check(p.start(tables,243,random,bank,binding,live,1,error),error);
  const auto before=p.blend;const auto before_slot=p.slots[p.blend.current];const auto root=binding.root;
  const auto* moving=data::animation_state(tables,48,"Attack");check(moving,"Moving attack row absent");check(p.swap(tables,int(moving-tables.sequences.data()),243,random,bank,binding,live,1,error),error);
  check(!std::memcmp(&before,&p.blend,32)&&before_slot.clip_id==p.current_clip()&&!std::memcmp(&before_slot.timeline,&p.current_timeline(),56)&&!std::memcmp(&root,&binding.root,96),"Metadata swap replayed or moved active slot");++metadata_swaps;
 }
 // A bound reference node with no position union channel still resets CURRENT
 // history to zero at timestamp+1; aggregate Animate skips both histories.
 {
  auto authored=rest;authored.graph[33].id="missing-root-channel";live=authored;binding=visual::SceneBinding{};check(binding.bind(live,error),error);actor::BlendedPlayback p;data::AnimationRandom random;check(setup(p,authored,binding),error);
  p.slots[0].root_history={42,{1,2,3},{4,5,6}};p.slots[1].root_history={43,{7,8,9},{10,11,12}};p.root_timestamp=100;
  const auto outgoing=p.slots[0].root_history;check(p.start(tables,280,random,bank,binding,live,1,error),error);
  check(!std::memcmp(&outgoing,&p.slots[0].root_history,28)&&p.slots[1].root_history.timestamp==101,"Missing reference ResetDelta touched wrong history");for(float f:p.slots[1].root_history.previous)check(f==0,"Missing reference reset did not use zero point");
  const auto current=p.slots[1].root_history;check(p.scene_phase(200,bank,binding,live,error),error);check(!std::memcmp(&current,&p.slots[1].root_history,28),"Missing reference aggregate updated history");
 }
 capture.finish();
 std::cout<<"{\"validation\":\"PASS\",\"asset_bank_clips\":"<<bank.size()<<",\"dynamic_compile\":"<<(dynamic?"true":"false")<<",\"constructor_slot_checks\":"<<(dynamic?2:0)<<",\"prince_nodes\":35,\"compiled_targets\":"<<prototype.transform_set().targets().size()<<",\"scene_phase_frames\":"<<frames<<",\"composition_fixture_records\":"<<capture.count<<",\"root_samples\":"<<root_samples<<",\"zero_weight_history_samples\":"<<zero_weight_histories<<",\"both_slot_speed_checks\":"<<speed_checks<<",\"time_only_frames\":"<<time_only<<",\"authored_callbacks\":"<<authored<<",\"replay_callbacks\":"<<replayed<<",\"sequence_closures\":"<<closures<<",\"terminal_pose_frames\":"<<terminal<<",\"synchronous_transitions\":"<<transitions<<",\"two_slot_event_callbacks\":"<<two_slot_events<<",\"retained_batch_callbacks\":"<<retained_events<<",\"metadata_swaps\":"<<metadata_swaps<<",\"callback_tail_regressions\":"<<callback_tail_regressions<<",\"forced_stop_checks\":"<<forced_stop_checks<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
