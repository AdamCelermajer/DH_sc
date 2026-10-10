#include "../character_script_session.hpp"
#include "../character_design_services.hpp"
#include "../../android-native/app/src/main/cpp/source_process_arrays_v101.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <dlfcn.h>
using namespace dh2::character;
namespace {
using Raw=std::vector<std::uint8_t>;
unsigned checks=0,vm_checks=0,guards=0,finalizers=0;
 unsigned stage17_register_anim_calls=0,stage17_flying_calls=0,stage17_required_failure_prefixes=0;
void require(bool value,unsigned line){++checks;if(!value)throw std::runtime_error("Session line "+std::to_string(line)+" check "+std::to_string(checks));}
#define check(value) require((value),__LINE__)
std::uint32_t word(std::istream& f){std::uint32_t v;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
Raw blob(std::istream& f){Raw raw(word(f));if(!raw.empty())f.read(reinterpret_cast<char*>(raw.data()),raw.size());check(bool(f));return raw;}
Raw file(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
std::string text(std::istream& f){auto b=blob(f);return {b.begin(),b.end()};}
struct Inputs {
 std::array<std::array<Raw,3>,5> tables;
 std::vector<Raw> constants;std::vector<dh2::data::Bytes> views;
 GameDesignInputs256 input{};
 explicit Inputs(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f)&&word(f)==0x314f4447);for(auto& t:tables)for(auto& r:t)r=blob(f);auto n=word(f);for(unsigned i=0;i<n;++i){text(f);constants.push_back(blob(f));}n=word(f);for(unsigned i=0;i<n;++i)text(f);check(f.peek()==EOF);
  GameDesignTableInput48* t[]={&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};for(unsigned i=0;i<5;++i)*t[i]={{tables[i][0].data(),tables[i][0].size()},{tables[i][1].data(),tables[i][1].size()},{tables[i][2].data(),tables[i][2].size()}};
  for(const auto& c:constants)views.push_back({c.data(),c.size()});input.constants=views.data();input.constant_count=views.size();
 }
};
struct ActualProcessArrays {
 std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> owner{std::make_shared<dh2::android_ui::SourceProcessArraysV101>()};
 static void append_word(Raw& out,std::uint32_t value){for(unsigned i=0;i<4;++i)out.push_back(static_cast<std::uint8_t>(value>>(8*i)));}
 static int lookup(void* raw,std::uint32_t kind,const char* group,const char* name,std::int32_t* out){
  if(!raw||kind!=1||!group||!name||!out)return 1;
  try{std::int32_t value=-1;std::string error;
   if(!static_cast<dh2::android_ui::SourceProcessArraysV101*>(raw)->get_member_id(group,name,value,error))return 1;
   *out=value;return 0;
  }catch(...){return 1;}
 }
 bool load(const Raw& animation_rows,const Raw& animation_names,std::string& error){
  auto read=[&](const std::string& uri,Raw& out,std::string& failure){
   if(uri=="data/pydata/animations_dictionary_pyarray.bin"){out=animation_rows;failure.clear();return true;}
   if(uri=="data/pydata/animations_dictionary_pyarraynames.bin"){out=animation_names;failure.clear();return true;}
   out.clear();bool registered=false;
   for(const auto& entry:dh2::android_ui::process_array_registration_v101)if(uri==std::string("data/pydata/")+entry.file){
    registered=true;append_word(out,0);
   }
   if(!registered){failure="Test process stream lacks the exact registered source file";return false;}
   failure.clear();return true;
  };
  bool complete=false;unsigned calls=0;
  while(!complete&&calls++<72)if(!owner->load_stage(read,complete,error))return false;
  if(!complete){error="Process PyData owner did not reach the original complete stage";return false;}
  error.clear();return owner->ready();
 }
 dh2_script_design_bindings bindings(){return {owner.get(),&lookup,0};}
};
struct Placement {std::string name,row;std::array<float,3> position;};
std::vector<Placement> placements(const Raw& bytes){std::uint32_t n=0;check(bytes.size()>=16&&!std::memcmp(bytes.data(),"DACT\1\0\0\0",8));std::memcpy(&n,bytes.data()+8,4);check(bytes.size()==16+n*256);std::vector<Placement> out;
 for(unsigned i=0;i<n;++i){const auto* p=bytes.data()+16+i*256;std::uint32_t kind=0;std::memcpy(&kind,p,4);if(kind!=1)continue;Placement value;value.name=reinterpret_cast<const char*>(p+8);value.row=reinterpret_cast<const char*>(p+72);std::memcpy(value.position.data(),p+200,12);out.push_back(value);}return out;
}
struct Host {
 HostPlayer8 player{};HostLevel8 level{};std::vector<LevelRangeRow24> rows;
 unsigned reads=0;
 static int invoke(void* context,const HostContextRequest16* r,HostContextResponse16* out){auto& t=*static_cast<Host*>(context);check(r&&!r->reserved);++t.reads;
  if(r->service==host_get_player)out->data=&t.player;
  else if(r->service==host_get_current_level)out->data=&t.level;
  else if(r->service==host_get_range_rows){out->data=t.rows.data();out->count=t.rows.size();}
  else check(false); // Session captures pushes; underlying host never fabricates them.
  return 0;
 }
};
struct MissingSave {
 unsigned opened=0;
 static int open(void* p,const char* name,std::uintptr_t* out){auto& t=*static_cast<MissingSave*>(p);check(!std::strcmp(name,"DebugSwitches.savegame"));++t.opened;*out=0;return 0;}
 static int close(void*,std::uintptr_t){check(false);return 1;}
};
void load(dh2_script_vm* vm,const std::string& code){check(!dh2_script_vm_load_source_file(vm,code.data(),code.size()));++vm_checks;}
dh2_script_value get(dh2_script_vm* vm,const char* name){dh2_script_value out{};check(!dh2_script_vm_get_global(vm,name,&out));++vm_checks;return out;}
ScriptSessionView active(CharacterScriptSession& s){ScriptSessionView v{};check(s.owner().active(v)&&s.view(v));return v;}
std::int32_t lookup(const CharacterGameDesign::Borrow& d,unsigned kind,const char* group,const char* member){std::int32_t out;check(!d.design()->lookup(d.design()->context,kind,group,member,&out));return out;}
struct CloseContext {std::array<float,3> expected;dh2_script_aliases* aliases;};
int finalizer(void* p,const dh2_script_value* a,unsigned n,dh2_script_value*,unsigned,unsigned* count,char*,std::size_t){
 auto& c=*static_cast<CloseContext*>(p);auto& expected=c.expected;check(n==8&&a[0].number==expected[0]&&a[1].number==expected[1]&&a[2].number==expected[2]);check(a[3].number==263&&a[4].number==0&&a[5].boolean&&a[6].number==45&&!a[7].boolean);check(!dh2_script_alias_contains(c.aliases,"BeforeClose")&&dh2_script_alias_contains(c.aliases,"AfterClose"));++finalizers;*count=0;return 0;
}
int busy(void* context,const dh2_script_value*,unsigned,dh2_script_value*,unsigned,unsigned* returned,char*,std::size_t){auto& s=*static_cast<CharacterScriptSession*>(context);auto v=active(s);dh2_script_value out{};check(dh2_script_vm_get_global(v.vm,"not_allowed",&out)==-1&&dh2_script_vm_load_source_file(v.vm,"x=1",3)==-1);auto& state=s.owner().lifecycle();auto old=state.load_step;state.load_step=1;check(s.advance()==-2);state.load_step=old;guards+=3;*returned=0;return 0;}
struct Expiry {
 CharacterScriptSession* session=nullptr;unsigned calls=0;
 static void expired(void* p,std::uintptr_t id,std::int32_t event,Timer32* timer){auto& t=*static_cast<Expiry*>(p);check(t.session&&id==t.session->timers().owner&&(event==0x33||event==0x34));++t.calls;check(dh2_character_timer_stop(&t.session->timers(),timer->id)==1);}
};
struct Stage17Motion {
 std::uint32_t flags=0;unsigned writes=0,reads=0;bool reject=false;
 static int invoke(void* raw,std::uint32_t address,std::uint32_t* value){auto& t=*static_cast<Stage17Motion*>(raw);if(!value||*value>1)return -1;
  if(t.reject)return -1;
  if(address==0x390900u){t.flags=(t.flags&~1u)|*value;++t.writes;return 0;}
  if(address==0x39088cu){t.flags=(t.flags&~2u)|(*value<<1);++t.writes;return 0;}
  if(address==0x38ea94u){*value=t.flags&1u;++t.reads;return 0;}
  if(address==0x38ea74u){*value=(t.flags>>1)&1u;++t.reads;return 0;}return -1;
 }
};
struct Stage17NativeFixture {
 struct AudioBinding {Stage17NativeFixture* self;std::uint32_t address;};
 std::array<AudioBinding,3> audio{{{this,0x37e420u},{this,0x37e578u},{this,0x37e730u}}};
 Stage17Motion motion;std::vector<std::int32_t> animation_ids;unsigned stop_calls=0,other_audio_calls=0;unsigned reject_animation_call=0;
 static int audio_call(void* raw,const dh2_script_value* args,std::uint32_t count,dh2_script_value*,std::uint32_t,std::uint32_t* returned,char* error,std::size_t size){
  auto& binding=*static_cast<AudioBinding*>(raw);auto& self=*binding.self;if(!returned||(count&&!args))return 1;*returned=0;
  if(binding.address==0x37e730u){if(count!=2||args[0].type!=DH2_SCRIPT_STRING||std::string(args[0].text,args[0].text_bytes)!="SwampKingMusic"||args[1].type!=DH2_SCRIPT_NUMBER||args[1].number!=2000){if(error&&size)std::snprintf(error,size,"Unexpected Stage17 StopSound arguments");return 1;}++self.stop_calls;return 0;}
  ++self.other_audio_calls;return 0;
 }
 static int select(void* raw,std::uint32_t address,dh2_script_function* function,void** context){auto& self=*static_cast<Stage17NativeFixture*>(raw);if(!function||!context)return -1;
  for(auto& entry:self.audio)if(entry.address==address){*function=&audio_call;*context=&entry;return 1;}return 0;
 }
 static int animation(void* raw,std::int32_t id){auto& self=*static_cast<Stage17NativeFixture*>(raw);self.animation_ids.push_back(id);return self.reject_animation_call==self.animation_ids.size()?-1:0;}
};
void run_stage17_init(CharacterGameDesign& design,CharacterGameDesign& process_design,const Raw& common,const Raw& script,
 HostContextBindings16& host,const LevelServices16& level){
 auto d=process_design.borrow();
 std::int32_t ai_id=-1;for(std::size_t i=0;i<d.ai()->rows.size();++i)if(d.ai()->rows[i].script=="swampking_core"){ai_id=std::int32_t(i);break;}check(ai_id>=0);
 std::shared_ptr<dh2::data::PropertyState> props;std::string actor_name;
 for(std::size_t i=0;i<d.characters()->rows.size();++i){auto candidate=std::make_shared<dh2::data::PropertyState>();std::string error;
  dh2::data::reset_properties(*d.rules(),*candidate,&d.characters()->rows[i]);
  if(!dh2::data::recalc_properties_with_class(*d.classes(),*d.rules(),*candidate,error))continue;
  if(candidate->resolved[1]==ai_id){props=std::move(candidate);actor_name=d.characters()->names[i];break;}}
 check(bool(props));auto combat=std::make_shared<dh2::data::CombatActorState>();Stage17NativeFixture native;
 CharacterScriptSessionInput in;in.identity=UINT64_C(0x1717171700000001);in.name=actor_name;in.properties=props;in.combat=combat;in.source_is_character=1;
 in.common={common.data(),common.size()};in.external={script.data(),script.size()};in.host=&host;in.level=&level;
 in.motion_capabilities={&native.motion,&Stage17Motion::invoke};in.animation_registration={&native,&Stage17NativeFixture::animation};
 in.gameplay_context=&native;in.gameplay_binding=&Stage17NativeFixture::select;
 // Record the current six-table CharacterGameDesign limitation separately.
 // The authentic callback reaches AnimDict before its first RegisterAnim.
 std::string error;auto missing_dictionary=CharacterScriptSession::create(design.borrow(),in,error);check(bool(missing_dictionary)&&!missing_dictionary->start());
 auto missing_view=active(*missing_dictionary);const auto missing_epoch=dh2_script_vm_required_failure_epoch(missing_view.vm);
 check(missing_dictionary->owner().source_initial_virtual_v86(missing_view.identity,16)==-2);
 check(dh2_script_vm_required_failure_epoch(missing_view.vm)==missing_epoch+1);
 check(native.stop_calls==1&&native.motion.writes==1&&native.animation_ids.empty());
 check(std::strstr(dh2_script_vm_error(missing_view.vm),"source design lookup delivery failed"));missing_dictionary.reset();
 native.stop_calls=0;native.motion={};
 // This VM resolves the exact registered AnimDict through the retained
 // process-array owner and its decoded source names; no names are supplied to
 // the animation callback or CharacterGameDesign as a detached table.
 auto session=CharacterScriptSession::create(std::move(d),in,error);
 check(bool(session)&&error.empty());const int status=session->start();
 if(status||!session->error().empty())throw std::runtime_error("Actual swampking_core InitFinal failed after callback prefix: "+session->error());
 auto view=active(*session);
 // The load process invokes OnInit at slot8. The reached Stage17 virtual
 // slot16 is a separate source call and must not be inferred from start().
 check(native.stop_calls==0&&native.motion.writes==0&&native.animation_ids.empty());
 load(view.vm,"stage17_process_anim_dict_id=GetPyOID('AnimDict','swampking_idle'); stage17_missing_anim_dict_id=GetPyOID('AnimDict','__not_a_real_animdict_member__')");
 check(get(view.vm,"stage17_process_anim_dict_id").number==1328);
 check(get(view.vm,"stage17_missing_anim_dict_id").number==-1);
 check(!session->owner().source_initial_virtual_v86(view.identity,16));
 load(view.vm,"stage17_hp_current,stage17_hp_max,stage17_hp_percent=GetPropHP()");
 const auto hp=props->resolved[36],maximum=props->resolved[38];
 check(get(view.vm,"stage17_hp_current").number==float(maximum?(hp>>8):0));
 check(get(view.vm,"stage17_hp_max").number==float(maximum?(maximum>>8):0));
 if(maximum&&(maximum>>8))check(get(view.vm,"stage17_hp_percent").number==float((std::int32_t((std::uint32_t(hp)*100u)) / (maximum>>8))>>8));
 else check(get(view.vm,"stage17_hp_percent").number==0);
 check(native.stop_calls==1&&native.motion.flags==1&&native.motion.writes==1);
 check(native.animation_ids.size()==7&&std::all_of(native.animation_ids.begin(),native.animation_ids.end(),[](std::int32_t id){return id>=0;}));
 check(std::find(native.animation_ids.begin(),native.animation_ids.end(),1328)!=native.animation_ids.end());
 stage17_register_anim_calls=static_cast<unsigned>(native.animation_ids.size());stage17_flying_calls=native.motion.writes;
 // Required producer failures retain the committed callback prefix and fail
 // the native virtual even though Lua pcall can ordinarily swallow errors.
 for(unsigned mode=0;mode<2;++mode){
  Stage17NativeFixture failed;failed.motion.reject=mode==0;failed.reject_animation_call=mode==1?3:0;
  auto failing_input=in;failing_input.motion_capabilities={&failed.motion,&Stage17Motion::invoke};
  failing_input.animation_registration={&failed,&Stage17NativeFixture::animation};failing_input.gameplay_context=&failed;
  auto failing=CharacterScriptSession::create(process_design.borrow(),failing_input,error);check(bool(failing)&&!failing->start());
  auto failed_view=active(*failing);const auto epoch=dh2_script_vm_required_failure_epoch(failed_view.vm);
  check(failing->owner().source_initial_virtual_v86(failed_view.identity,16)==-2);
  check(dh2_script_vm_required_failure_epoch(failed_view.vm)==epoch+1);
  check(failed.stop_calls==1);
  check(failed.motion.writes==unsigned(mode==1));
  check(failed.animation_ids.size()==unsigned(mode==1?3:0));
  check(failing->owner().error().find("Required native service failed during initial virtual")!=std::string::npos);
  ++stage17_required_failure_prefixes;
  if(mode==0){
   load(failed_view.vm,"function stage17_swallow_required() local ok=pcall(MarkAsFlying,true); assert(not ok); stage17_after_pcall=true end; AddToVFTable('OnInitFinal','stage17_swallow_required')");
   const auto caught_epoch=dh2_script_vm_required_failure_epoch(failed_view.vm);
   check(failing->owner().source_initial_virtual_v86(failed_view.identity,16)==-2);
   check(dh2_script_vm_required_failure_epoch(failed_view.vm)==caught_epoch+1&&get(failed_view.vm,"stage17_after_pcall").boolean);
   check(failed.stop_calls==1&&!failed.motion.writes&&failed.animation_ids.empty());++stage17_required_failure_prefixes;
  }
 }
}
std::string origin(void* address){Dl_info d{};check(dladdr(address,&d));return d.dli_fname;}
}
int main(int argc,char** argv){try{
 check(argc==8);Inputs raw(argv[1]);auto common=file(argv[2]),monster=file(argv[3]),dact=file(argv[4]),stage17_script=file(argv[5]),animation_rows=file(argv[6]),animation_names=file(argv[7]);auto authored=placements(dact);check(authored.size()==11);
 ActualProcessArrays process_arrays;std::string error;check(process_arrays.load(animation_rows,animation_names,error));
 std::int32_t actual_idle=-1;check(process_arrays.owner->get_member_id("AnimDict","swampking_idle",actual_idle,error)&&actual_idle==1328);
 auto design=std::make_unique<CharacterGameDesign>();check(design->initialize(raw.input,error));
 auto process_design=std::make_unique<CharacterGameDesign>();auto process_bindings=process_arrays.bindings();
 check(process_design->initialize(raw.input,process_bindings,process_arrays.owner,error));auto d=process_design->borrow();
 Host h;h.rows.resize(d.levels()->levels.size());for(unsigned i=0;i<h.rows.size();++i)std::memcpy(&h.rows[i],d.levels()->levels[i].scalar.words+12,24);
 auto crypt=std::find(d.levels()->level_names.begin(),d.levels()->level_names.end(),"GOTHICUS_CRYPT_01");check(crypt!=d.levels()->level_names.end());h.level={std::int32_t(crypt-d.levels()->level_names.begin()),0};
 auto player=std::make_shared<dh2::data::PropertyState>();dh2::data::reset_properties(*d.rules(),*player,&d.characters()->rows[263]);check(dh2::data::recalc_properties_with_class(*d.classes(),*d.rules(),*player,error));auto pv=dh2::data::property_view(*d.rules(),*player);check(!dh2_character_host_context_sync_level(&h.player,&pv));
 HostContextBindings16 host{{&h,&Host::invoke}};
 MissingSave missing;DebugFileServices24 files{&missing,&MissingSave::open,&MissingSave::close};std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)> debug(dh2_character_debug_create(),&dh2_character_debug_destroy);check(bool(debug));DebugLevelBinding16 debug_binding{debug.get(),&files};LevelServices16 level{&debug_binding,&dh2_character_debug_level_service};
 run_stage17_init(*design,*process_design,common,stage17_script,host,level);
 std::vector<std::unique_ptr<CharacterScriptSession>> sessions;std::vector<const float*> stable_positions;
 unsigned registrations=0,supported=0,unsupported=0;std::int32_t expected_buff=lookup(d,1,"ClassTable","Buff_Speed");const auto tick=lookup(d,0,"CharacterDesign","AI_Tick"),dot=lookup(d,0,"CharacterDesign","DoT_Tick");
 for(unsigned index=0;index<authored.size();++index){const auto& actor=authored[index];auto at=std::find(d.characters()->names.begin(),d.characters()->names.end(),actor.row);check(at!=d.characters()->names.end());
  CharacterScriptSessionInput in;in.identity=UINT64_C(0xabcd012300000000)+index+1;in.name=actor.name;in.properties=std::make_shared<dh2::data::PropertyState>();in.combat=std::make_shared<dh2::data::CombatActorState>();in.position=actor.position;in.source_is_character=1;in.common={common.data(),common.size()};in.external={monster.data(),monster.size()};in.host=&host;in.level=&level;
  const std::string alpha="Include('_beta');SetInt('alpha',GetInt('beta')+1);BusyGuard()",beta="SetInt('beta',41)";
  in.include_files={{"data/scripts/ai/_alpha.luac",Raw(alpha.begin(),alpha.end())},{"data/scripts/ai/_beta.luac",Raw(beta.begin(),beta.end())}};
  dh2::data::reset_properties(*d.rules(),*in.properties,&d.characters()->rows[at-d.characters()->names.begin()]);check(dh2::data::recalc_properties_with_class(*d.classes(),*d.rules(),*in.properties,error));
  auto before=in.properties;auto shared_combat=in.combat;auto ai=dh2::data::ai_props(*d.ai(),in.properties->resolved[1]);check(ai&&ai->script=="monster");
  auto s=CharacterScriptSession::create(design->borrow(),in,error);check(s&&error.empty()&&s->script_name()=="monster");check(s->start()==0&&s->owner().last_source_load_status()==0&&s->error().empty());auto v=active(*s);check(v.kind==4&&v.loaded_files==2&&v.callback_flags&&s->owner().lifecycle().load_step==7);check(dh2_script_vm_stack_size(v.vm)==5);
  check(get(v.vm,"saved_X").number==actor.position[0]&&get(v.vm,"saved_Y").number==actor.position[1]&&get(v.vm,"m_flee_flag").boolean&&get(v.vm,"BUFF_ID").number==float(expected_buff));
  check(s->properties()==before&&s->combat_state()==shared_combat&&s->position()[2]==actor.position[2]);check(s->timers().count==2&&s->timers().slots[0].event==0x33&&s->timers().slots[0].duration_ms==std::uint32_t(tick)&&s->timers().slots[1].event==0x34&&s->timers().slots[1].duration_ms==std::uint32_t(dot));
  for(unsigned j=0;j<2;++j)check(s->timers().slots[j].active&&s->timers().slots[j].repeat==-1&&!s->timers().slots[j].user_ref);
  check(s->registrations().size()==170);unsigned ri=0;for(unsigned phase=1;phase<=2;++phase)for(unsigned i=0;i<dh2_character_script_binding_count(phase);++i){ScriptBinding24 b{};check(!dh2_character_script_binding(&b,phase,i));if(b.method)continue;auto& r=s->registrations().at(ri++);check(r.phase==phase&&r.index==i&&r.name==b.name&&r.original_callback==b.original_callback&&r.installed);check(get(v.vm,b.name).type==DH2_SCRIPT_FUNCTION);r.supported?++supported:++unsupported;}registrations+=ri;
  check(!s->missing_bindings().empty());load(v.vm,"unsupported_ok,unsupported_error=pcall(Attack);int_before=GetInt('independent');SetInt('independent',17);int_after=GetInt('independent');default_ok,default_error=pcall(GetProp,19,true)");check(!get(v.vm,"unsupported_ok").boolean&&std::strstr(get(v.vm,"unsupported_error").text,"Unsupported source global Attack"));check(get(v.vm,"int_before").number==0&&get(v.vm,"int_after").number==17&&!get(v.vm,"default_ok").boolean);
  Timer32 before_timers[2];std::memcpy(before_timers,s->timers().slots,64);check(s->update_timers(1000,0)==-1&&!std::memcmp(before_timers,s->timers().slots,64));++guards;
  load(v.vm,"SetInt('independent',"+std::to_string(index+101)+")");stable_positions.push_back(s->position());sessions.push_back(std::move(s));
 }
 check(missing.opened==1);for(unsigned i=0;i<sessions.size();++i){auto v=active(*sessions[i]);check(sessions[i]->position()==stable_positions[i]);load(v.vm,"own=GetInt('independent')");check(get(v.vm,"own").number==float(i+101));}
 auto& first=*sessions[0];auto firstvm=active(first).vm;
 // All script bytes and initial caller value objects can die after create.
 std::fill(common.begin(),common.end(),0);std::fill(monster.begin(),monster.end(),0);for(auto& t:raw.tables)for(auto& b:t)std::fill(b.begin(),b.end(),0);for(auto& c:raw.constants)std::fill(c.begin(),c.end(),0);
 check(!dh2_script_vm_bind_source_values(firstvm,"BusyGuard",&busy,&first));
 load(firstvm,"Include('_commons');Include('_alpha');assert(GetInt('alpha')==42 and GetInt('beta')==41);SetInt('beta',99);Include('_alpha');assert(GetInt('beta')==99);timer_id=StartTimer(17,1);StopTimer(timer_id)");check(first.timers().count==3&&!first.timers().slots[2].active&&first.timers().slots[2].event==0x35);
 // Genuine scoped Include uses persistent owner/cache services while busy.
 // The per-session common is already loaded, so its exact bytes remain copied.
 check(active(first).loaded_files==4);
 auto new_position=std::array<float,3>{712.5f,-900.25f,33.5f};first.set_position(new_position);load(firstvm,"px,py,pz=GetPosition()");check(get(firstvm,"px").number==new_position[0]&&get(firstvm,"py").number==new_position[1]&&get(firstvm,"pz").number==new_position[2]);
 // The snapshot survives its original owner's destruction via the retained Borrow.
 d={};design.reset();CloseContext close{new_position,active(first).aliases};check(!dh2_script_vm_bind_source_values(firstvm,"ObserveClose",&finalizer,&close));
 load(firstvm,"AddToVFTable('BeforeClose','Old');PushVFTable();SetInt('fromgc',99);proxy=newproxy(true);getmetatable(proxy).__gc=function() local was=GetInt('fromgc');SetInt('fromgc',45);local x,y,z=GetPosition();AddToVFTable('AfterClose','New');local supported=pcall(Attack);ObserveClose(x,y,z,GetPyOID('CharacterTable','KnightPlayerBase'),was,GetInt('fromgc')==45,GetInt('fromgc'),supported) end");
 sessions.erase(sessions.begin());check(finalizers==1);sessions.clear();debug.reset();
 // Dedicated fresh input guards are atomic; invalid construction consumes no VM.
 Inputs clean(argv[1]);CharacterGameDesign guard_design;check(guard_design.initialize(clean.input,error));auto b=guard_design.borrow();CharacterScriptSessionInput invalid;invalid.identity=1;invalid.properties=std::make_shared<dh2::data::PropertyState>();invalid.combat=std::make_shared<dh2::data::CombatActorState>();dh2::data::reset_properties(*b.rules(),*invalid.properties,&b.characters()->rows[263]);
 for(unsigned mode=0;mode<6;++mode){auto in=invalid;if(mode==0)in.identity=0;if(mode==1)in.properties.reset();if(mode==2)in.combat.reset();if(mode==3)in.source_is_character=2;if(mode==4)in.timer_capacity=0;if(mode==5)in.vm_memory_limit=65535;check(!CharacterScriptSession::create(guard_design.borrow(),in,error)&&!error.empty());++guards;}
 // Actual expiry behavior belongs to the caller; this explicit stopping fixture
 // proves storage and callback delivery only, not the missing full AI handler.
 Expiry expiry;TimerServices32 expiry_provider{&expiry,&Expiry::expired,nullptr,0};auto common2=file(argv[2]),monster2=file(argv[3]);auto at=std::find(b.characters()->names.begin(),b.characters()->names.end(),authored[0].row);dh2::data::reset_properties(*b.rules(),*invalid.properties,&b.characters()->rows[at-b.characters()->names.begin()]);check(dh2::data::recalc_properties_with_class(*b.classes(),*b.rules(),*invalid.properties,error));
 invalid.name=authored[0].name;invalid.position=authored[0].position;invalid.source_is_character=1;invalid.common={common2.data(),common2.size()};invalid.external={monster2.data(),monster2.size()};invalid.host=nullptr;invalid.timer_services=&expiry_provider;
 // No Host backend: this is deliberately a failure-prefix case, not Init success.
 auto partial=CharacterScriptSession::create(guard_design.borrow(),invalid,error);check(partial&&partial->start()==0&&partial->owner().last_source_load_status()==0&&!partial->error().empty());expiry.session=partial.get();check(partial->timers().count==2&&partial->update_timers(std::max(std::uint32_t(tick),std::uint32_t(dot)),0)==1&&expiry.calls==2);partial.reset();
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_swampking_core_oninit\":true,\"actual_swampking_core_virtual16\":true,\"missing_AnimDict_failure_reproduced\":true,\"process_AnimDict_owner_resolved_swampking_idle\":true,\"process_AnimDict_swampking_idle_id\":1328,\"stage17_required_failure_prefixes\":"<<stage17_required_failure_prefixes<<",\"actual_swampking_core_get_prophp\":true,\"actual_swampking_core_mark_as_flying_calls\":"<<stage17_flying_calls<<",\"actual_swampking_core_register_anim_calls\":"<<stage17_register_anim_calls<<",\"actual_Crypt_monster_initializations\":11,\"source_registrations\":"<<registrations<<",\"supported_registrations\":"<<supported<<",\"explicit_failing_registrations\":"<<unsupported<<",\"actual_VM_checks\":"<<vm_checks<<",\"atomic_guards\":"<<guards<<",\"actual_finalizers\":"<<finalizers<<",\"expiry_fixture_callbacks\":"<<expiry.calls<<",\"DebugSwitches_missing_file_fixture_deliveries\":"<<missing.opened<<",\"host_fixture_projection_reads\":"<<h.reads<<",\"source_timer_durations\":["<<tick<<","<<dot<<"],\"runtime_library\":\""<<origin(reinterpret_cast<void*>(dh2_script_vm_create_empty))<<"\",\"world_library\":\""<<origin(reinterpret_cast<void*>(dh2_character_set_level))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(dh2_class_recalc_base))<<"\",\"whole_session_original_instruction_differential\":false,\"full_AI_timer_expiry\":false,\"source_Application_session_selection\":false,\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
