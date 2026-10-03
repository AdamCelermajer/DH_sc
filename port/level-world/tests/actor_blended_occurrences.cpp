// Reuse the existing audited frame capture format; production remains linked
// through genuine shared libraries, never included into this executable.
#define main dh2_old_blended_fixture_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-function"
#include "actor_blended_playback.cpp"
#pragma GCC diagnostic pop
#undef main
#include <climits>

namespace {
struct BankReader {
 std::vector<std::uint8_t> bytes;std::size_t at=0;
 unsigned word(){check(at+4<=bytes.size(),"Short occurrence fixture");unsigned v;std::memcpy(&v,bytes.data()+at,4);at+=4;return v;}
 std::string string(){auto n=word();check(n<=bytes.size()-at,"Short occurrence path");std::string s(reinterpret_cast<const char*>(bytes.data()+at),n);at+=n;return s;}
};
struct BankResource {int id,start,end;std::string path;};
struct Events {unsigned authored=0,selections=0,closures=0;std::array<unsigned,2> authored_slots{};};
void occurrence_event(void* raw,actor::BlendedPlayback& p,const actor::BlendedPlaybackEvent& e){
 auto& count=*static_cast<Events*>(raw);check(e.slot<2,"Wrong source slot metadata");
 if(e.event.handoff.event_id==0x28){check(e.event.handoff.payload&&e.event.clip==955&&p.last_event_lag==e.event.handoff.lag_ms,"Authored dictionary/lag metadata differs");++count.authored;++count.authored_slots[e.slot];}
 if(e.event.handoff.event_id==0x24||e.event.handoff.event_id==0x26)++count.selections;
 if(e.event.handoff.event_id==0x22)++count.closures;
}
}
int main(int argc,char**argv){try{
 check(argc==5,"usage: actor_blended_occurrences bank-fixture assets playclip-fixtures frame-output");std::string error;const std::string assets=argv[2];
 BankReader r{read(argv[1])};check(r.word()==0x314b4250,"Wrong bank fixture");const auto resource_count=r.word(),occurrence_count=r.word(),projection_count=r.word(),default_id=r.word();
 check(resource_count==116&&occurrence_count==158&&projection_count==17&&default_id==1111,"Wrong source registration cardinalities");
 const auto model=r.string();std::vector<BankResource> resources;
 for(unsigned i=0;i<resource_count;++i){BankResource v;v.id=static_cast<int>(r.word());v.start=static_cast<int>(r.word());v.end=static_cast<int>(r.word());v.path=r.string();resources.push_back(v);}
 std::vector<int> requests;for(unsigned i=0;i<occurrence_count;++i)requests.push_back(static_cast<int>(r.word()));
 std::vector<std::pair<int,int>> projections;for(unsigned i=0;i<projection_count;++i){auto id=r.word();auto index=r.word();projections.emplace_back(id,index);}check(r.at==r.bytes.size(),"Trailing bank fixture");
 const auto raw=read(assets+"/"+model);resources::BresView view{};scene::Scene rest;
 check(dh2_bres_open(&view,raw.data(),raw.size())==resources::BresError::ok&&scene::load(view,rest,error),error);
 actor::ClipBank bank;for(const auto& v:resources){auto bytes=read(assets+"/"+v.path);auto& player=bank[v.id];check(player.load(bytes.data(),bytes.size(),rest,error,animation::MissingTargets::ignore),error);}
 auto registration=[&](bool alias=false){animation::RegistrationSet result;for(auto id:requests)check(result.append(id,std::uint64_t(id),&bank.at(id),error),error);
  if(alias)check(result.append(20001,1040,&bank.at(1040),error),error);
  check(result.set_default(1111,&bank.at(1111),error),error);result.refresh_indices();return result;};
 auto live=rest;visual::SceneBinding binding;check(binding.bind(live,error),error);actor::BlendedPlayback p;
 {auto source=registration();check(p.compile_dynamic(bank,source,rest,binding,error),error);} // copied map/default metadata outlives registration
 check(p.transform_set().clip_count()==158&&p.transform_set().targets().size()==83,"Lost source occurrences/target order");
 unsigned constructor=0;for(const auto& slot:p.slots){check(slot.clip_id==1111&&slot.compiled_clip==0&&slot.timeline.clip_index==0&&slot.timeline.loop==1&&!slot.timeline.initialized&&slot.timeline.start_ms==0&&slot.timeline.end_ms==3599,"Source constructor/default binding differs");for(auto c:slot.key_cursors)check(c==0,"Constructor invented key cursor");++constructor;}
 unsigned bounds=0;for(unsigned i=0;i<requests.size();++i){const auto found=std::find_if(resources.begin(),resources.end(),[&](const auto& v){return v.id==requests[i];});const auto* clip=p.transform_set().clip(i);check(clip&&clip->id==int(i)&&p.dictionary_id(i)==requests[i]&&clip->start==found->start&&clip->end==found->end,"Occurrence bounds/dictionary metadata differs");++bounds;}
 check(p.engine_index(1138)==155&&p.transform_set().clip(155)->start==INT_MAX&&p.transform_set().clip(155)->end==INT_MIN,"Zero-track source registration was normalized/filtered");
 check(p.engine_index(-1)==-1&&p.engine_index(INT_MAX)==-1&&p.dictionary_id(-1)==-1&&p.dictionary_id(158)==-1,"Missing identity lookup accepted");
 Capture capture(argv[4],p,live);data::AnimationTables tables;data::AnimationRandom random;unsigned mapped=0,frames=0,root_histories=0;
 for(const auto& expected:projections){const auto sequence=direct(tables,expected.first,0);check(p.start(tables,sequence,random,bank,binding,live,1,error),error);
  check(p.current_clip()==expected.first&&p.current_engine_clip()==expected.second&&p.current_timeline().clip_index==expected.second&&p.engine_index(expected.first)==expected.second,"Source dictionary→first engine selection differs");++mapped;
  for(unsigned frame=0;frame<3;++frame){const auto now=1000+mapped*150+frame*37;capture.frame(p,now,bank,binding,live,error);++frames;for(const auto& slot:p.slots){check(slot.root_history.timestamp==now,"Root history sampled wrong engine/slot");++root_histories;}for(const auto& node:live.graph)for(float f:node.world)check(std::isfinite(f),"Nonfinite source graph pose");}
 }
 // Both incoming and outgoing managers use owned event names and dictionary
 // metadata, after the temporary original registration has already died.
 p=actor::BlendedPlayback{};live=rest;binding=visual::SceneBinding{};check(binding.bind(live,error),error);
 {auto source=registration();check(p.compile_dynamic(bank,source,rest,binding,error),error);}
 Events events;p.observer={&events,occurrence_event};const auto attack=direct(tables,955,1000);
 const auto event_start=1000u;
 check(p.start(tables,attack,random,bank,binding,live,1,error),error);
 for(unsigned i=1;i<=40;++i){capture.frame(p,event_start+i*37,bank,binding,live,error);check(p.animator_phase(tables,random,bank,binding,live,1,p.completion.extra_ms,error),error);++frames;}
 check(p.start(tables,attack,random,bank,binding,live,1,error),error);
 for(unsigned i=41;i<=80;++i){capture.frame(p,event_start+i*37,bank,binding,live,error);check(p.animator_phase(tables,random,bank,binding,live,1,p.completion.extra_ms,error),error);++frames;}
 check(events.authored&&events.authored_slots[0]&&events.authored_slots[1],"Both-slot authored events disappeared: "+std::to_string(events.authored_slots[0])+","+std::to_string(events.authored_slots[1])+" tracks="+std::to_string(bank.at(955).events.view().count));capture.finish();
 // Synthetic alias uses exactly one canonical bank Player/identity and a
 // different game map key. The actual source producer map/append gold is
 // replayed independently; PlayClip gold supplies this engine comparison.
 actor::BlendedPlayback alias;auto alias_live=rest;visual::SceneBinding alias_binding;check(alias_binding.bind(alias_live,error),error);
 {auto source=registration(true);check(alias.compile_dynamic(bank,source,rest,alias_binding,error),error);}
 check(alias.transform_set().clip_count()==159&&alias.engine_index(20001)==2&&alias.dictionary_id(158)==20001,"Alias lost occurrence or first resource index");
 BankReader source_play{read(argv[3])};check(source_play.word()==0x314f5042,"Wrong original PlayClip gold");const auto comparisons=source_play.word();
 for(unsigned i=0;i<comparisons;++i){const auto previous=source_play.word(),index=source_play.word(),dictionary=source_play.word(),ended=source_play.word(),extra=source_play.word(),expected=source_play.word();
  alias.blend.current=0;alias.blend.previous=0;alias.blend.duration=0;alias.blend.remaining=0;alias.blend.weights[0]=1;alias.blend.weights[1]=0;
  alias.slots[1].compiled_clip=previous;alias.slots[1].clip_id=previous==2?1040:previous==7?1041:1111;alias.slots[1].timeline.ended=ended;alias.slots[1].timeline.loop=1;alias.completion={};std::memcpy(&alias.applicator_completion.extra_ms,&extra,4);
  const auto sequence=direct(tables,dictionary,0);check(alias.start(tables,sequence,random,bank,alias_binding,alias_live,1,error),error);
  check(alias.current_engine_clip()==int(index)&&alias.current_clip()==int(dictionary),"Alias selected dictionary as engine index");
  const auto& time=alias.current_timeline();check(std::uint32_t(time.current_ms)-std::uint32_t(time.start_ms)==expected&&!time.initialized&&!time.ended,"Same-engine extra differs from original PlayClip");
 }
 check(source_play.at==source_play.bytes.size(),"Trailing original PlayClip gold");
 unsigned guards=0;
 auto reject=[&](const animation::RegistrationSet& source,const actor::ClipBank& supplied,const visual::SceneBinding& supplied_binding){actor::BlendedPlayback fresh;fresh.completions=17;fresh.root_timestamp=19;const auto blend=fresh.blend;const auto first=fresh.slots[0].timeline;
  check(!fresh.compile_dynamic(supplied,source,rest,supplied_binding,error)&&fresh.transform_set().clip_count()==0&&fresh.engine_index(1111)==-1&&fresh.completions==17&&fresh.root_timestamp==19&&!std::memcmp(&fresh.blend,&blend,32)&&!std::memcmp(&fresh.slots[0].timeline,&first,56),"Malformed occurrence compile partially committed");++guards;};
 {animation::RegistrationSet empty;reject(empty,bank,binding);}
 {animation::RegistrationSet missing;for(auto id:requests)check(missing.append(id,id,&bank.at(id),error),error);reject(missing,bank,binding);}
 {auto source=registration();check(source.set_default(1040,&bank.at(1111),error),error);reject(source,bank,binding);}
 {auto source=registration();animation::Player detached;check(source.set_default(1111,&detached,error),error);reject(source,bank,binding);}
 {animation::RegistrationSet wrong;for(auto id:requests)check(wrong.append(id,id,id==1111?&bank.at(1040):&bank.at(id),error),error);check(wrong.set_default(1111,&bank.at(1111),error),error);wrong.refresh_indices();reject(wrong,bank,binding);}
 {auto source=registration();check(source.append(1040,50000,&bank.at(1040),error),error);source.refresh_indices();reject(source,bank,binding);}
 {auto source=registration();check(source.append(20001,1040,&bank.at(1041),error),error);source.refresh_indices();reject(source,bank,binding);}
 {auto source=registration();check(source.append(20001,1040,&bank.at(1040),error),error);reject(source,bank,binding);} // stale alias map
 {auto source=registration();visual::SceneBinding absent;reject(source,bank,absent);}
 {auto source=registration();const auto prior=alias.current_timeline();const auto count=alias.transform_set().clip_count();check(!alias.compile_dynamic(bank,source,rest,alias_binding,error)&&alias.transform_set().clip_count()==count&&!std::memcmp(&prior,&alias.current_timeline(),56),"Bound coordinator recompile mutated state");++guards;}
 {auto source=registration();actor::BlendedPlayback fresh;check(!fresh.compile_dynamic(bank,source,rest,binding,error,static_cast<animation::TransformMismatchBehavior>(2))&&fresh.transform_set().clip_count()==0,"Invalid compiler policy partially committed");++guards;}
 // Missing game lookup keeps source Blend-before-map-failure side effects.
 const auto previous_slot=alias.blend.current;const auto missing=direct(tables,30000,0);check(!alias.start(tables,missing,random,bank,alias_binding,alias_live,1,error)&&alias.blend.current!=previous_slot,"Missing map silently accepted or lost source Blend ordering");
 std::cout<<"{\"validation\":\"PASS\",\"resources\":116,\"occurrences\":158,\"constructor_slot_checks\":"<<constructor<<",\"original_bounds_checks\":"<<bounds<<",\"source_mapped_selections\":"<<mapped<<",\"scene_frames\":"<<frames<<",\"original_playclip_comparisons\":"<<comparisons<<",\"root_history_checks\":"<<root_histories<<",\"authored_callbacks\":"<<events.authored<<",\"slot0_authored\":"<<events.authored_slots[0]<<",\"slot1_authored\":"<<events.authored_slots[1]<<",\"selection_callbacks\":"<<events.selections<<",\"alias_occurrences\":159,\"alias_first_engine\":2,\"zero_track_engine\":155,\"atomic_rejections\":"<<guards<<",\"missing_lookup_source_blend_check\":1,\"registration_destroyed_before_playback\":true,\"sanitizer_findings\":0}\n";return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}

