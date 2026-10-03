#include "../actor_playback.hpp"
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
void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing fixture "+path);return {std::istreambuf_iterator<char>(f),{}};}
data::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
struct Reader {const std::vector<std::uint8_t>& b;std::size_t p=0;template<class T>T take(){check(p+sizeof(T)<=b.size(),"Short event corpus");T v;std::memcpy(&v,b.data()+p,sizeof(T));p+=sizeof(T);return v;}std::string name(){auto n=take<std::uint32_t>();check(n&&p+n<=b.size()&&!b[p+n-1],"Corpus string rejected");std::string s(reinterpret_cast<const char*>(b.data()+p),n-1);p+=n;return s;}};
using Events=std::vector<std::pair<std::int32_t,std::string>>;
void collect(const animation::TriggeredEvent* e,void* context){static_cast<Events*>(context)->emplace_back(e->lag_ms,e->name);}
struct Context {
 const data::AnimationTables* tables;data::AnimationRandom* random;const actor::ClipBank* bank;visual::SceneBinding* binding;scene::Scene* live;
 Events events;unsigned authored=0,closures=0,replayed=0,switches=0;bool switch_trigger=false,closure_idle=false,drop_observer=false;const char* switch_name="attack_mainhand";float speed=1;float before[3]{};std::uint32_t prior_time=0;std::string error;
};
void observe(void* raw,actor::Playback& p,const actor::PlaybackEvent& e){
 auto& c=*static_cast<Context*>(raw);
 if(e.handoff.event_id==0x28){
  check(e.handoff.payload&&p.last_event_lag==e.handoff.lag_ms,"Triggered identity/lag handoff differs");
  check(e.phase==actor::scene_event||e.phase==actor::replay_event,"Trigger phase differs");
  ++c.authored;c.replayed+=e.phase==actor::replay_event;c.events.emplace_back(e.handoff.lag_ms,e.handoff.payload);
  if(e.phase==actor::scene_event){check(p.root_timestamp==c.prior_time,"Event saw prematurely finalized root timestamp");for(unsigned i=0;i<3;++i)check(c.binding->root.position[i]==c.before[i],"Root moved before event callback");}
  if(c.switch_trigger&&std::string(e.handoff.payload)==c.switch_name){
   c.switch_trigger=false;const auto origin=e.clip;
   check(p.start(*c.tables,259,*c.random,*c.bank,*c.binding,*c.live,c.speed,c.error),c.error);
   check(origin!=p.clip_id&&p.clip_id==1023,"Synchronous trigger failed to select Died");++c.switches;if(c.drop_observer)p.observer={};
  }
 }else{
  check(e.handoff.event_id==0x22&&!e.handoff.payload&&e.phase==actor::sequence_event,"Sequence closure identity differs");
  check(!p.scheduler.active()&&p.sequence_closed==1,"Closure not marked before observer");++c.closures;
  if(c.closure_idle){const int old=p.clip_id;check(p.start(*c.tables,262,*c.random,*c.bank,*c.binding,*c.live,c.speed,c.error),c.error);check(p.clip_id==old,"Closure ANIM_Set was not held until callback return");++c.switches;c.closure_idle=false;}
 }
}
}
int main(int argc,char** argv){try{
 if(argc!=3)return 2;const std::string assets=argv[1];std::string error;
 auto a=read(assets+"/data/animations_pyarray.bin"),b=read(assets+"/data/animations_pyarraynames.bin"),c=read(assets+"/data/animations_pystructnames.bin"),d=read(assets+"/data/animations_dictionary_pyarraynames.bin"),e=read(assets+"/data/animations_dictionary_pyarray.bin");
 data::Dictionary dictionary;data::AnimationTables tables;check(data::load_dictionary(bytes(d),bytes(e),dictionary,error)&&data::load_animation_tables(bytes(a),bytes(b),bytes(c),dictionary,tables,error),error);
 auto raw=read(assets+"/models/prince_modular.bdae");resources::BresView view{};scene::Scene rest;check(dh2_bres_open(&view,raw.data(),raw.size())==resources::BresError::ok&&scene::load(view,rest,error),error);check(rest.graph.size()==35,"Prince node count differs");
 std::set<int> ids;std::function<void(int)> collect_ids=[&](int seq){for(const auto& step:tables.sequences.at(seq).steps)if(step.redir)collect_ids(step.anim);else ids.insert(step.anim);};
 collect_ids(243);collect_ids(259);collect_ids(262);const auto* moving_sequence=data::animation_state(tables,48,"Attack");check(moving_sequence,"Authored moving attack absent");const int moving=int(moving_sequence-tables.sequences.data());collect_ids(moving);
 for(int id:{955,956,957,958,959,960,961,962,963,967,969,971,1023})ids.insert(id);
 actor::ClipBank bank;for(int id:ids){const auto path=dictionary.values.at(id);auto image=read(assets+"/animations/"+path.substr(path.find_last_of("/\\")+1));animation::Player p;check(p.load(image.data(),image.size(),rest,error),error);check(!p.unbound&&!p.skipped,"Unsupported real Prince track");bank.emplace(id,std::move(p));}
 auto gold=read(argv[2]);Reader reader{gold};check(reader.take<std::uint32_t>()==0x31474541,"AEG1 magic differs");auto windows=reader.take<unsigned>(),swaps=reader.take<unsigned>(),lags=reader.take<unsigned>();unsigned callback_comparisons=0;
 for(unsigned i=0;i<windows;++i){const auto clip=reader.take<int>(),previous=reader.take<int>(),current=reader.take<int>(),start=reader.take<int>(),end=reader.take<int>(),last=reader.take<int>(),after=reader.take<int>(),n=reader.take<int>();Events expected;
  for(int k=0;k<n;++k){auto lag=reader.take<int>();expected.emplace_back(lag,reader.name());}
  animation::EventCursor cursor{last};Events actual;check(dh2_events_update(&bank.at(clip).events.view(),&cursor,previous,current,start,end,collect,&actual)==1,"Authored event update rejected");check(actual==expected&&cursor.last_entry==after,"Original event corpus differs");
  for(const auto& event:actual){actor::EventHandoff out{};int stored=0;animation::TriggeredEvent in{event.first,event.second.c_str()};check(dh2_actor_event_handoff(&out,&stored,&in)==0&&out.event_id==0x28&&out.payload==in.name&&stored==in.lag_ms,"Original Character handoff differs");++callback_comparisons;}
 }
 for(unsigned i=0;i<swaps;++i){int desired=reader.take<int>(),old=reader.take<int>(),depth=reader.take<int>(),root=reader.take<int>();int loops[3],steps[3],expected[3];for(auto& v:loops)v=reader.take<int>();for(auto& v:steps)v=reader.take<int>();for(auto& v:expected)v=reader.take<int>();auto action=reader.take<int>();
  data::AnimationTables fixture;fixture.sequences.resize(9);for(int id=0;id<9;++id){auto& s=fixture.sequences[id];s.type=1;s.steps.resize(3);for(auto& step:s.steps){step.redir=id%3<depth;step.anim=step.redir?id+1:955;}}
  data::AnimationScheduler scheduler;data::AnimationRandom random;check(scheduler.start(fixture,root,random,error),error);auto& frames=const_cast<std::vector<data::AnimationFrame>&>(scheduler.frames());check(frames.size()==unsigned(depth+1),"Swap source fixture depth differs");for(unsigned k=0;k<frames.size();++k){frames[k].loops=loops[k];frames[k].step=steps[k];}
  auto prior=scheduler.clip();auto result=scheduler.swap_sequences(fixture,desired,old,error);check(int(result)==action,"Original ANIM_Swap decision differs");for(unsigned k=0;k<frames.size();++k)check(frames[k].sequence==expected[k]&&frames[k].loops==loops[k]&&frames[k].step==unsigned(steps[k]),"Original ANIM_Swap stack differs");check(scheduler.clip().anim==prior.anim&&scheduler.clip().speed==prior.speed,"Metadata swap reselected active clip");
 }
 for(unsigned i=0;i<lags;++i){auto lag=reader.take<int>();actor::EventHandoff out{};int stored=0;animation::TriggeredEvent input{lag,"attack_mainhand"};check(dh2_actor_event_handoff(&out,&stored,&input)==0&&out.event_id==0x28&&out.lag_ms==lag&&stored==lag&&out.payload==input.name,"Original lag corpus differs");}
 check(reader.p==gold.size(),"Trailing event corpus bytes");
 unsigned scene_frames=0,authored=0,closures=0,replayed=0,transitions=0,terminal=0,root_samples=0,metadata_swaps=0;
 for(float speed:{1.f,1.3f})for(unsigned mode=0;mode<4;++mode){actor::Playback p;auto live=rest;visual::SceneBinding binding;check(binding.bind(live,error),error);data::AnimationRandom random;
  Context ctx{&tables,&random,&bank,&binding,&live};ctx.speed=speed;ctx.switch_trigger=mode==2;ctx.closure_idle=mode==1;p.observer={&ctx,observe};const int seq=mode==0?259:243;check(p.start(tables,seq,random,bank,binding,live,speed,error),error);
  for(unsigned frame=0;frame<500;++frame){ctx.events.clear();ctx.prior_time=p.root_timestamp;std::copy(binding.root.position,binding.root.position+3,ctx.before);check(p.scene_phase(1000+frame*37,bank,binding,live,error),error);++scene_frames;
   auto independently=rest;check(bank.at(p.clip_id).sample(independently,p.timeline.current_ms,error),error);for(unsigned axis=0;axis<3;++axis)check(binding.root.animated[axis]==independently.graph[33].translation[axis],"Callback-selected clip was not sampled");++root_samples;
   if(mode==3&&frame==1){auto before=p.timeline;auto cursor=p.event_cursor;auto root_state=binding.root;const int clip=p.clip_id;auto rng=random;check(p.swap(tables,moving,243,random,bank,binding,live,speed,error),error);check(p.clip_id==clip&&!std::memcmp(&before,&p.timeline,sizeof(before))&&p.event_cursor.last_entry==cursor.last_entry&&!std::memcmp(&root_state,&binding.root,sizeof(root_state))&&random.seed==rng.seed&&random.calls==rng.calls,"Attack moving metadata swap replayed active clip");++metadata_swaps;}
   const auto captured=p.completion.extra_ms;check(p.animator_phase(tables,random,bank,binding,live,speed,captured,error),error);
   if(!p.scheduler.active()){check(p.clip_id>=0&&p.sequence_closed==1,"Finite pose was discarded");++terminal;}
   for(const auto& node:live.graph)for(float component:node.world)check(std::isfinite(component),"Nonfinite callback scene matrix");
  }
  check(ctx.closures==1,"Finite sequence closure emitted more or less than once");if(mode==2)check(ctx.switches==1&&p.clip_id==1023,"Attack authored event transition missing");if(mode==1)check(ctx.switches==1&&p.scheduler.active()&&(p.clip_id==1040||p.clip_id==1041),"Deferred closure Idle transition missing");
  authored+=ctx.authored;closures+=ctx.closures;replayed+=ctx.replayed;transitions+=ctx.switches;
 }
 unsigned retained_batch=0,overshoot_replays=0;
 {
  // Immutable synthetic track, using actual Prince pose/clip ownership. The
  // original updateTime probe proves all three old-manager callbacks survive
  // synchronous active-manager replacement; no authored name is invented for
  // a production gameplay event.
  auto& event_view=const_cast<animation::EventView&>(bank.at(955).events.view());const auto saved=event_view;
  const std::int32_t keys[]={366,466};const char* first[]={"first","second"};const char* second[]={"third"};const animation::EventGroup groups[]={{2,first},{1,second}};
  event_view={4,2,reinterpret_cast<const std::uint8_t*>(keys),groups};
  data::AnimationTables fixture=tables;const int direct=int(fixture.sequences.size());data::AnimationSequence sequence;sequence.type=0;sequence.steps.resize(1);sequence.steps[0].anim=955;fixture.sequences.push_back(sequence);
  actor::Playback p;data::AnimationRandom random;auto live=rest;visual::SceneBinding binding;check(binding.bind(live,error),error);Context ctx{&fixture,&random,&bank,&binding,&live};ctx.switch_trigger=true;ctx.switch_name="first";ctx.drop_observer=true;p.observer={&ctx,observe};check(p.start(fixture,direct,random,bank,binding,live,1,error),error);check(p.scene_phase(1000,bank,binding,live,error),error);ctx.prior_time=p.root_timestamp;std::copy(binding.root.position,binding.root.position+3,ctx.before);check(p.scene_phase(1500,bank,binding,live,error),error);
  check(ctx.events==Events({{234,"first"},{234,"second"},{134,"third"}}),"Retained old-manager order/lag differs from original animator probe");check(p.event_cursor.last_entry==-1&&p.clip_id==1023,"Retired manager overwrote new clip cursor");auto independent=rest;check(bank.at(1023).sample(independent,p.timeline.current_ms,error),error);for(unsigned i=0;i<3;++i)check(binding.root.animated[i]==independent.graph[33].translation[i],"Outer pose used retired clip");retained_batch=ctx.events.size();event_view=saved;
 }
 for(float speed:{1.f,1.3f}){
  data::AnimationTables fixture=tables;const int direct=int(fixture.sequences.size());data::AnimationSequence sequence;sequence.type=0;sequence.loop=-1;sequence.steps.resize(1);sequence.steps[0].anim=955;sequence.steps[0].move_go=true;fixture.sequences.push_back(sequence);
  actor::Playback p;data::AnimationRandom random;auto live=rest;visual::SceneBinding binding;check(binding.bind(live,error),error);Context ctx{&fixture,&random,&bank,&binding,&live};ctx.speed=speed;p.observer={&ctx,observe};check(p.start(fixture,direct,random,bank,binding,live,speed,error),error);
  check(p.scene_phase(1000,bank,binding,live,error),error);ctx.prior_time=1000;std::copy(binding.root.position,binding.root.position+3,ctx.before);check(p.scene_phase(1000+bank.at(955).end+500,bank,binding,live,error),error);check(p.completion.pending&&p.completion.extra_ms>0,"Overshoot fixture did not capture applicator extra");const auto captured=p.completion.extra_ms;const auto before=p.restarts;ctx.events.clear();check(p.animator_phase(fixture,random,bank,binding,live,speed,captured,error),error);animation::EventCursor fresh;Events replay_expected;check(dh2_events_update(&bank.at(955).events.view(),&fresh,bank.at(955).start+captured,p.timeline.current_ms,p.timeline.start_ms,p.timeline.end_ms,collect,&replay_expected),"Replay source event interval rejected");check(p.restarts==before+1&&p.event_cursor.last_entry==fresh.last_entry&&ctx.events==replay_expected,"Same-clip replay did not use fresh manager interval");replayed+=ctx.replayed;++overshoot_replays;
  // Explicit source timeline-loop caller fixture. NewAnim at an older root
  // timestamp dispatches through the same event manager; no second clock.
  p=actor::Playback{};binding=visual::SceneBinding{};live=rest;check(binding.bind(live,error),error);ctx.events.clear();ctx.authored=ctx.replayed=0;p.observer={&ctx,observe};check(p.start(fixture,direct,random,bank,binding,live,1,error),error);ctx.prior_time=0;std::fill(ctx.before,ctx.before+3,0);check(p.scene_phase(1000,bank,binding,live,error),error);ctx.prior_time=1000;std::copy(binding.root.position,binding.root.position+3,ctx.before);check(p.scene_phase(1030,bank,binding,live,error),error);p.timeline.loop=1;check(p.replay_phase(1000,bank,binding,live,error),error);check(ctx.replayed==1,"Reentrant NewAnim source-loop callback missing");replayed+=ctx.replayed;
 }
 actor::EventHandoff out{99,44,"unchanged"};unsigned closed=0;check(dh2_actor_sequence_close(&out,&closed)==1&&closed==1&&out.event_id==0x22&&!out.payload,"Closure kernel first dispatch differs");out.event_id=99;check(dh2_actor_sequence_close(&out,&closed)==0&&out.event_id==99,"Closure duplicate mutated output");closed=2;check(dh2_actor_sequence_close(&out,&closed)==-1&&closed==2&&out.event_id==99,"Malformed closure was not atomic");check(dh2_actor_event_handoff(&out,nullptr,nullptr)==-1&&out.event_id==99,"Malformed handoff mutated output");
 std::cout<<"{\"original_event_windows\":"<<windows<<",\"original_lag_cases\":"<<lags<<",\"original_swap_cases\":"<<swaps<<",\"original_callbacks\":"<<callback_comparisons<<",\"scene_phase_frames\":"<<scene_frames<<",\"root_samples\":"<<root_samples<<",\"authored_callbacks\":"<<authored<<",\"sequence_closures\":"<<closures<<",\"replay_callbacks\":"<<replayed<<",\"synchronous_transitions\":"<<transitions<<",\"terminal_pose_frames\":"<<terminal<<",\"metadata_swaps\":"<<metadata_swaps<<",\"retained_batch_callbacks\":"<<retained_batch<<",\"overshoot_replays\":"<<overshoot_replays<<",\"rejection_checks\":2,\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
