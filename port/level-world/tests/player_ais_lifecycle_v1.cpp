// Borrow the actual catalogue/VM/Debug fixture without changing its own gate.
#define main player_skill_session_existing_main
#include "player_skill_session_v1.cpp"
#undef main
#include "../player_ais_lifecycle_v1.hpp"
#include "../character_ai_association.hpp"
#include "../../game-data/combat_application.hpp"

namespace l=dh2::player_ais_lifecycle_v1;
namespace c=dh2::character;
namespace {
const char* update_overlay=R"lua(RegisterSkill(function() update_calls=(update_calls or 0)+1 end,
 function() return true end,function() end,function() end,function() end))lua";
const char* update_failure=R"lua(RegisterSkill(function() pcall(function() GetHostPlayer() end) end,
 function() return true end,function() end,function() end,function() end))lua";
struct LifecycleFixture {
 Fixture f;
 dh2::character_ai_initialization::State ai{};
 l::PlayerFields fields{};l::Tables tables{0x710000001ull,0x720000001ull,0x730000001ull};
 d::AiTables ai_tables;dh2_pycst_view design{};
 d::PlayerSavegameV1 save;
 c::Coordinator coordinator{CHAR};
 dh2::character_level_runtime::Runtime vitals;
 dh2::character_level_runtime::Storage storage{};
 std::unique_ptr<dh2::player_skill_update_session_v1::Runtime> updates;
 std::unique_ptr<dh2::player_skill_use_session_v1::Runtime> uses;
 s::Session* session_slot=nullptr;p::Owner* preparation_slot=nullptr;
 dh2::player_skill_update_session_v1::Runtime* update_slot=nullptr;
 dh2::player_skill_use_session_v1::Runtime* use_slot=nullptr;
 std::uint32_t dead=0;std::int32_t machine=3;
 std::unique_ptr<l::Runtime> runtime;
 unsigned backend_calls=0,constructor_calls=0,configure_calls=0,queue_calls=0;
 bool fail_constructor=false,fail_configure=false,reenter=false;
 l::Status reentry_status=l::Status::complete;
 std::vector<std::string> trace;
 LifecycleFixture(const std::filesystem::path& cache,const std::filesystem::path& temp,Catalogue& cat,const char* name)
  :f(cache,temp,cat,name,false){
  const auto load=[&](const char* stem,const char* suffix){return read(cache/"data/pydata"/(std::string(stem)+suffix+".bin"));};
  auto a=load("ai","_pyarray"),b=load("ai","_pyarraynames"),cs=load("ai","_pystructnames"),
   fa=load("ai_factions","_pyarray"),fn=load("ai_factions","_pyarraynames"),fs=load("ai_factions","_pystructnames");
  std::string error;check(d::load_ai(bytes(a),bytes(b),bytes(cs),bytes(fa),bytes(fn),bytes(fs),ai_tables,error),error.c_str());
  const auto* row=d::ai_props(ai_tables,f.properties.resolved[1]);check(row&&row->script=="__player__","actual Player AI selector differs");
  ai.identity=0x300000001ull;
  const dh2::character_ai_initialization::Services queue{this,[](void* raw,dh2::character_ai_initialization::State* state,std::uintptr_t id){
   auto& s=*static_cast<LifecycleFixture*>(raw);check(state==&s.ai&&id==s.ai.identity,"CharAI constructor queue identity differs");++s.queue_calls;return 0;}};
  dh2::character_ai_initialization::Result constructed{};
  check(dh2::character_ai_initialization::construct(&ai,0x740000001ull,&queue,&constructed)==dh2::character_ai_initialization::Status::complete,"CharAI source construction failed");
  check(dh2::character_ai_association::associate(&ai,CHAR)==dh2::character_ai_association::Status::complete,"CharAI association failed");
  fields.script.identity=AIS;fields.script.dispatch_table=0xdead000001ull;
  fields.script.binder_identity=reinterpret_cast<std::uintptr_t>(&f.session);
  fields.script.path_storage_identity=reinterpret_cast<std::uintptr_t>(&f);
  fields.script.owner_98=0xdead000002ull;fields.script.counter_bc=91;fields.script.word_c0=92;
  fields.vector_begin=11;fields.vector_end=12;fields.vector_capacity=13;fields.skill_d0=93;fields.skill_d4=94;
  save.set_character(CHAR);check(save.initialize_skills(cat.tables->skills(),f.properties.resolved[28],error),error.c_str());save.initialize_faeries();
  check(!dh2_pycst_open(&design,cat.design.data(),std::uint32_t(cat.design.size())),"actual design constants rejected");
  coordinator.bind({this,[](void*){return c::Facts{};},{this,[](void*,c::State*,const c::Request*){}},nullptr,nullptr});
  storage={CHAR,reinterpret_cast<std::uintptr_t>(&f.properties),f.properties.base.data(),&f.view,nullptr,0,nullptr,0,&f.debug.globals(),&f.debug.services()};
  for(const auto& skill:cat.tables->skills().skills)if(skill.script_length&&!skill.script.empty())f.overlay(("data/scripts/skills/"+skill.script+".luac").c_str(),update_overlay);
  for(const auto& faery:cat.tables->faeries().faeries)if(faery.spell_script_length&&!faery.spell_script.empty())f.overlay(("data/scripts/skills/"+faery.spell_script+".luac").c_str(),update_overlay);
  l::Bindings bindings{&ai,&fields,&f.ais,&tables,row,"Player",&session_slot,&preparation_slot,&update_slot,&use_slot,&save,&coordinator,&dead,&design,&vitals,&storage,{this,backend}};
  runtime=std::make_unique<l::Runtime>(bindings);
 }
 ~LifecycleFixture(){runtime.reset();coordinator.stop_timers();uses.reset();updates.reset();f.session.reset();f.owner.reset();}
 static int backend(void* raw,const l::Request* request,std::string& error){
  auto& s=*static_cast<LifecycleFixture*>(raw);++s.backend_calls;
  check(request&&request->character==CHAR&&request->ais==AIS,"backend native identities changed");
  if(s.reenter){s.reenter=false;l::Result nested{};nested.source_return=91;std::string unchanged="sentinel";
   s.reentry_status=s.runtime->initialize(1,&nested,unchanged);check(s.reentry_status==l::Status::busy&&
    s.runtime->load(&nested,unchanged)==l::Status::busy&&s.runtime->initialize_process(1,&nested,unchanged)==l::Status::busy&&
    nested.source_return==91&&unchanged=="sentinel","lifecycle reentry changed output");}
  if(request->operation==l::Operation::construct_vm){
   ++s.constructor_calls;s.trace.emplace_back("construct");
   check(request->allocation_bytes==0xd8&&request->skip_bind==1&&s.ai.pointer_28==0&&!s.ai.alternate_ais_20&&
    s.f.session->stage()==s::Stage::created,"factory/VM source prefix differs");
   if(s.fail_constructor){error="constructor fixture required failure";return 1;}
   // Transfer the fixture's genuinely-created, unbound SAME Session into the
   // lifecycle. No second VM is created at this caller boundary.
   s.session_slot=s.f.session.get();return 0;
  }
  ++s.configure_calls;s.trace.emplace_back("configure");
  check(s.ai.pointer_28==7&&s.ai.active_ais_1c==AIS&&s.ai.alternate_ais_20==AIS,"configure preceded actual publication");
  if(s.fail_configure){error="configure fixture required failure";return 1;}
  s.f.owner=p::Owner::create(s.f.catalogue.tables,{CHAR,s.ai.active_ais_1c,&s.f.view,0},s.f.session->preparation_services(),error);
  if(!s.f.owner)return 1;
  p::source::Result prepared{};if(s.f.owner->prepare(&prepared)!=p::source::Status::complete)return 1;
  s.preparation_slot=s.f.owner.get();
  s.updates=std::make_unique<dh2::player_skill_update_session_v1::Runtime>(*s.f.session,*s.f.owner,s.ai.identity,CHAR,s.machine);
  s.uses=std::make_unique<dh2::player_skill_use_session_v1::Runtime>(*s.f.session,*s.f.owner,CHAR);
  s.update_slot=s.updates.get();s.use_slot=s.uses.get();return 0;
 }
 void failing_updates(){for(const auto& skill:f.catalogue.tables->skills().skills)if(skill.script_length&&!skill.script.empty())f.overlay(("data/scripts/skills/"+skill.script+".luac").c_str(),update_failure);}
};
void run(const std::filesystem::path& cache,const std::filesystem::path& temp){
 Catalogue catalogue(cache);unsigned successes=0,failures=0,guards=0,split_cases=0;std::vector<std::uint32_t> constructor_words;
 for(const auto* name:{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"}){
  LifecycleFixture f(cache,temp/name,catalogue,name);auto* vm=f.f.session->vm();
  f.reenter=true;l::Result result{};std::string error;
  check(f.runtime->initialize(1,&result,error)==l::Status::complete&&result.source_return==1,error.c_str());
  check(f.ai.pointer_28==7&&f.ai.active_ais_1c==AIS&&f.ai.alternate_ais_20==AIS&&f.queue_calls==1&&
   f.ai.owner_04==CHAR&&f.fields.script.owner_98==CHAR&&f.fields.script.dispatch_table==f.tables.ais_player_iphone&&
   !f.fields.vector_begin&&!f.fields.vector_end&&!f.fields.vector_capacity&&!f.fields.skill_d0&&!f.fields.skill_d4,
   "source factory/phase7/active publication differs");
  const auto& tree=f.fields.script.state_registry_9c;
  check(!tree.color&&!tree.parent&&!tree.count&&tree.left==reinterpret_cast<std::uintptr_t>(&tree)&&tree.right==tree.left,"source empty AIS registry differs");
  constructor_words={f.f.ais.flags_b8,f.fields.script.counter_bc,f.fields.script.word_c0,std::uint32_t(f.fields.script.current_state_b4),std::uint32_t(tree.parent),tree.count,
   std::uint32_t(f.fields.vector_begin),std::uint32_t(f.fields.vector_end),std::uint32_t(f.fields.vector_capacity),f.fields.skill_d0,f.fields.skill_d4};
  check(f.f.session->vm()==vm&&f.session_slot==f.f.session.get()&&f.constructor_calls==1&&f.configure_calls==1&&
   result.init_phase_mask==31&&result.vitals.init.hp_completed&&result.vitals.init.mp_completed&&result.update.callbacks==13&&
   result.vitals.hp.added&&result.vitals.mp.added&&
   std::uint32_t(f.f.properties.resolved[36])==result.vitals.hp.current+result.vitals.hp.positive_amount&&
   std::uint32_t(f.f.properties.resolved[41])==result.vitals.mp.current+result.vitals.mp.positive_amount,"Player ordered same-owner InitProcess differs");
  check(f.f.owner->slots(p::source::List::skill).size()==16&&f.f.owner->slots(p::source::List::faery).size()==5&&
   f.f.session->statistics().init_vcb_calls==1&&f.f.session->statistics().ais_bindings==35&&
   f.f.session->stage()==s::Stage::character_bound&&f.f.session->contains_path("data/scripts/ai/_commons.luac"),"source native preparation/VCB binding differs");
  const auto& timers=f.coordinator.timers();check(timers.count==2&&f.ai.word_10==0&&f.ai.word_14==1,"source timer order differs");
  for(unsigned i=0;i<2;++i){const auto& timer=timers.slots[i];dh2_pycst_result tick{};const char* key=i?"DoT_Tick":"AI_Tick";
   check(!dh2_pycst_get(&f.design,"CharacterDesign",15,key,std::strlen(key),&tick)&&tick.found&&timer.id==i&&timer.active&&
    timer.event==std::int32_t(0x33+i)&&timer.repeat==-1&&!timer.user_ref&&timer.duration_ms==std::uint32_t(tick.value),"Coordinator source AI/DoT timer values differ");}
  const auto calls=f.backend_calls;auto* owner=f.f.owner.get();f.f.properties.resolved[36]-=256;
  check(f.runtime->initialize(1,&result,error)==l::Status::complete&&!result.source_return&&!result.service_calls&&
   f.backend_calls==calls&&f.f.owner.get()==owner&&f.f.session->vm()==vm&&f.f.properties.resolved[36]!=f.f.properties.resolved[38],"active source guard replayed init/healed owner");
  check(f.reentry_status==l::Status::busy,"source provider reentry did not reach guard");++successes;++guards;
 }
 {LifecycleFixture f(cache,temp/"split-load-init",catalogue,"KnightPlayerBase");auto* vm=f.f.session->vm();
  const auto before=f.f.properties;f.reenter=true;l::Result loaded{},finished{},guarded{};std::string error;
  check(f.runtime->load(&loaded,error)==l::Status::complete&&loaded.source_return==1&&loaded.init_phase_mask==0&&
   f.ai.pointer_28==7&&f.ai.active_ais_1c==AIS&&f.ai.alternate_ais_20==AIS&&f.constructor_calls==1&&!f.configure_calls&&
   !f.preparation_slot&&!f.update_slot&&!f.use_slot&&!f.f.session->statistics().init_vcb_calls&&
   f.f.properties.base==before.base&&f.f.properties.saved==before.saved&&f.f.properties.resolved==before.resolved&&
   f.coordinator.timers().count==2&&f.f.session->vm()==vm,"load ran InitProcess or failed source publication");
  const auto calls=f.backend_calls;const auto statistics=f.f.session->statistics();
  check(f.runtime->load(&guarded,error)==l::Status::complete&&guarded.source_return==1&&!guarded.service_calls&&!guarded.init_phase_mask&&
   f.runtime->initialize(1,&guarded,error)==l::Status::complete&&!guarded.source_return&&!guarded.service_calls&&
   f.backend_calls==calls&&!f.configure_calls&&f.f.properties.resolved==before.resolved&&
   f.f.session->statistics().load_calls==statistics.load_calls,"loaded source guards replayed constructor or ran InitProcess");
  // A real caller can change the same live save between source load and finish.
  // This uses the existing saved-row writer, not an initial-grant/profile body.
  check(f.save.set_skill_level(0,2,error),error.c_str());
  check(f.runtime->initialize_process(0,&finished,error)==l::Status::complete&&finished.source_return==1&&
   finished.init_phase_mask==15&&finished.vitals.init.hp_completed==1&&finished.vitals.init.mp_completed==1&&
   finished.vitals.hp.current==std::uint32_t(before.resolved[36])&&finished.vitals.mp.current==std::uint32_t(before.resolved[41])&&
   finished.update.callbacks==13&&f.constructor_calls==1&&f.configure_calls==1&&f.f.session->statistics().init_vcb_calls==1&&
   f.save.skill_level(0)==2&&f.f.session->vm()==vm,"split InitProcess duplicated or lost live save/VM effects");
  const auto hp=f.f.properties.resolved[36],mp=f.f.properties.resolved[41];auto* owner=f.f.owner.get();
  check(f.runtime->load(&guarded,error)==l::Status::complete&&!guarded.service_calls&&
   f.runtime->initialize(1,&guarded,error)==l::Status::complete&&!guarded.source_return&&!guarded.service_calls&&
   f.f.properties.resolved[36]==hp&&f.f.properties.resolved[41]==mp&&f.f.owner.get()==owner&&
   f.constructor_calls==1&&f.configure_calls==1&&f.f.session->statistics().init_vcb_calls==1,"split compatibility guard duplicated heal/VM/preparation");
  check(f.reentry_status==l::Status::busy,"split load did not share busy guard");++successes;++split_cases;++guards;}
 {LifecycleFixture f(cache,temp/"constructor-failure",catalogue,"KnightPlayerBase");f.fail_constructor=true;l::Result result{};std::string error;
  check(f.runtime->initialize(1,&result,error)==l::Status::failed&&!f.ai.pointer_28&&!f.ai.active_ais_1c&&!f.ai.alternate_ais_20&&
   f.fields.script.owner_98==0xdead000002ull&&f.f.ais.flags_b8==0xffffffff&&!f.coordinator.timers().count,"failed Lua construction fabricated later fields/phase");
  auto before=result;const auto calls=f.backend_calls;error="sentinel";
  check(f.runtime->initialize(1,&result,error)==l::Status::failed&&!std::memcmp(&result,&before,sizeof(result))&&error=="sentinel"&&calls==f.backend_calls,"failed lifecycle automatically replayed source prefix");++failures;}
 {LifecycleFixture f(cache,temp/"split-configure-failure",catalogue,"KnightPlayerBase");l::Result result{};std::string error;
  check(f.runtime->load(&result,error)==l::Status::complete&&!result.init_phase_mask,"split failure fixture load initialized skills");
  f.fail_configure=true;check(f.runtime->initialize_process(1,&result,error)==l::Status::failed&&
   result.init_phase_mask==3&&f.ai.pointer_28==7&&f.ai.active_ais_1c==AIS&&f.ai.alternate_ais_20==AIS&&
   f.constructor_calls==1&&f.configure_calls==1&&!f.preparation_slot,"split failure rolled back publication or ran update/post");
  const auto before=result;const auto calls=f.backend_calls;error="sentinel";
  check(f.runtime->load(&result,error)==l::Status::failed&&f.runtime->initialize_process(1,&result,error)==l::Status::failed&&
   f.runtime->initialize(1,&result,error)==l::Status::failed&&!std::memcmp(&result,&before,sizeof(result))&&
   error=="sentinel"&&f.backend_calls==calls,"split methods did not share retained failure latch");++failures;++split_cases;}
 {LifecycleFixture f(cache,temp/"configure-failure",catalogue,"KnightPlayerBase");f.fail_configure=true;l::Result result{};std::string error;
  check(f.runtime->initialize(1,&result,error)==l::Status::failed&&f.ai.pointer_28==7&&f.ai.active_ais_1c==AIS&&
   f.ai.alternate_ais_20==AIS&&result.init_phase_mask==3&&f.coordinator.timers().count==2&&!f.preparation_slot,"failed configure erased source phase7 or ran update/post");++failures;}
 {LifecycleFixture f(cache,temp/"update-failure",catalogue,"KnightPlayerBase");f.failing_updates();l::Result result{};std::string error;
  check(f.runtime->initialize(1,&result,error)==l::Status::failed&&f.ai.pointer_28==7&&f.ai.active_ais_1c==AIS&&
   f.preparation_slot&&result.init_phase_mask==7&&result.update.last_lua_status==-5&&f.f.session->statistics().required_failures==1,"caught required update failure fabricated Post/Final or rolled back publication");++failures;}
 {LifecycleFixture f(cache,temp/"dead",catalogue,"KnightPlayerBase");f.dead=1;l::Result result{};std::string error;
  check(f.runtime->initialize(0,&result,error)==l::Status::complete&&result.init_phase_mask==15&&!f.coordinator.timers().count&&
   f.ai.word_10==UINT32_MAX&&f.ai.word_14==UINT32_MAX&&f.ai.pointer_28==7,"dead source OnInit gate scheduled timers or skipped publication");++successes;}
 {LifecycleFixture f(cache,temp/"guards",catalogue,"KnightPlayerBase");l::Result result{};result.source_return=44;std::string error="sentinel";
  check(f.runtime->initialize(2,&result,error)==l::Status::invalid_argument&&result.source_return==44&&error=="sentinel"&&
   f.runtime->initialize(1,reinterpret_cast<l::Result*>(&f.ai),error)==l::Status::invalid_argument&&f.ai.owner_04==CHAR&&!f.constructor_calls,"invalid/aliased lifecycle controls changed owner");++guards;}
 {LifecycleFixture f(cache,temp/"declaration-output-alias",catalogue,"KnightPlayerBase");l::Result result{};result.source_return=44;
  auto& authored=const_cast<dh2::data::AiProps*>(dh2::data::ai_props(f.ai_tables,f.f.properties.resolved[1]))->script;
  check(f.runtime->initialize(1,&result,authored)==l::Status::invalid_argument&&authored=="__player__"&&result.source_return==44&&
   !f.ai.pointer_28&&!f.ai.active_ais_1c&&!f.constructor_calls,"error output alias mutated borrowed authored declaration");++guards;}
 {LifecycleFixture f(cache,temp/"split-guards",catalogue,"KnightPlayerBase");l::Result result{};result.source_return=44;std::string error="sentinel";
  auto& authored=const_cast<d::AiProps*>(d::ai_props(f.ai_tables,f.f.properties.resolved[1]))->script;
  check(f.runtime->load(&result,authored)==l::Status::invalid_argument&&authored=="__player__"&&
   f.runtime->initialize_process(2,&result,error)==l::Status::invalid_argument&&
   f.runtime->initialize_process(1,reinterpret_cast<l::Result*>(&f.ai),error)==l::Status::invalid_argument&&
   result.source_return==44&&error=="sentinel"&&!f.ai.pointer_28&&!f.constructor_calls,"split API guard changed borrowed controls");++guards;}
 std::cout<<"{\"validation\":\"PASS\",\"real_player_classes\":3,\"completed_lifecycles\":"<<successes<<",\"failure_prefix_cases\":"<<failures<<",\"guard_cases\":"<<guards<<",\"split_phase_cases\":"<<split_cases<<",\"constructor_scalar_words\":[";
 for(std::size_t i=0;i<constructor_words.size();++i)std::cout<<(i?",":"")<<constructor_words[i];
 std::cout<<"],\"constructor_tree_empty\":true,\"source_phase7_published\":true,\"single_retained_vm\":true,\"actual_coordinator_timers\":true,\"selected_native_build\":false,\"live_gameplay\":false}\n";
}
}
int main(int argc,char** argv){try{check(argc==3,"cache and output directory required");run(argv[1],argv[2]);return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
