#include "../character_script_session.hpp"
#include "../character_design_services.hpp"
#include "../gameobject_lua_representation.hpp"
#include "../character_controller_commands.hpp"
#include "../character_path_commands.hpp"
#include "../character_timer_effects.hpp"
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <dlfcn.h>
using namespace dh2::character;
namespace {
using Raw=std::vector<std::uint8_t>;
unsigned checks=0,vm_calls=0,guards=0,source_methods=0;
void require(bool ok,unsigned line){++checks;if(!ok)throw std::runtime_error("Target session line "+std::to_string(line));}
#define check(x) require((x),__LINE__)
std::uint32_t word(std::istream& in){std::uint32_t value;in.read(reinterpret_cast<char*>(&value),4);check(bool(in));return value;}
Raw blob(std::istream& in){Raw out(word(in));if(!out.empty())in.read(reinterpret_cast<char*>(out.data()),out.size());check(bool(in));return out;}
Raw file(const char* path){std::ifstream in(path,std::ios::binary);check(bool(in));return {std::istreambuf_iterator<char>(in),{}};}
struct Inputs {
 std::array<std::array<Raw,3>,5> tables;std::vector<Raw> constants;std::vector<dh2::data::Bytes> views;GameDesignInputs256 input{};
 Inputs(const char* path){std::ifstream in(path,std::ios::binary);check(bool(in)&&word(in)==0x314f4447);for(auto& table:tables)for(auto& bytes:table)bytes=blob(in);
  auto count=word(in);for(unsigned i=0;i<count;++i){blob(in);constants.push_back(blob(in));}count=word(in);for(unsigned i=0;i<count;++i)blob(in);check(in.peek()==EOF);
  GameDesignTableInput48* target[]={&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};for(unsigned i=0;i<5;++i)*target[i]={{tables[i][0].data(),tables[i][0].size()},{tables[i][1].data(),tables[i][1].size()},{tables[i][2].data(),tables[i][2].size()}};
  for(auto& bytes:constants)views.push_back({bytes.data(),bytes.size()});input.constants=views.data();input.constant_count=views.size();
 }
};
struct Provider {
 CharacterGameDesign::Borrow* design;CharacterScriptSession* session=nullptr;dh2_script_vm* vm=nullptr;
 HostPlayer8 player{1,0};HostLevel8 level{23,0};std::vector<LevelRangeRow24> rows;
 TargetOwner16 owner{UINT64_C(0x100000001),0,0,0},other_owner{UINT64_C(0x100000002),0,0,0};
 TargetState48 target{1,&owner,0,0,0,0,0,0,0,0},other{2,&other_owner,0,0,0,0,0,0,0,0};
 TargetBindings48 bindings{&target,{this,target_service},nullptr,{0,0}};
 State state{};NativeFsm24 fsm{&state,owner.identity,1,0};
 dh2_script_object_services objects{this,type,methods,invoke};
 float command_position[3]{0,0,0},target_position[3]{1,2,3},axis[3]{0,0,1};
 ScriptCommandState48 command_state{owner.identity,UINT64_C(0x100000003),other_owner.identity,command_position,axis,0,0};
 ScriptCommandBindings40 commands{&command_state,{this,command,command_number},nullptr};
 unsigned command_calls=0,controller_calls=0,path_queries=0;bool fail_command=false;
 DebugSwitches* debug=dh2_character_debug_create();unsigned debug_opens=0,target_calls=0,scoped_calls=0;
 unsigned top_global_calls=0;bool top_global_empty=false;
 DebugFileServices24 debug_files{this,open,close};DebugLevelBinding16 debug_binding{debug,&debug_files};
 LevelServices16 level_services{&debug_binding,dh2_character_debug_level_service};
 HostContextBindings16 host{{this,host_service}};bool fail_target=false;
 explicit Provider(CharacterGameDesign::Borrow& d):design(&d){check(debug);state.current=3;state.elapsed_ms=123;
  rows.resize(d.levels()->levels.size());for(unsigned i=0;i<rows.size();++i)std::memcpy(&rows[i],d.levels()->levels[i].scalar.words+12,24);
 }
 ~Provider(){dh2_character_debug_destroy(debug);}
 static int open(void* p,const char* name,std::uintptr_t* out){auto& self=*static_cast<Provider*>(p);check(!std::strcmp(name,"DebugSwitches.savegame"));++self.debug_opens;*out=0;return 0;}
 static int close(void*,std::uintptr_t){check(false);return 1;}
 static int host_service(void* p,const HostContextRequest16* request,HostContextResponse16* out){auto& self=*static_cast<Provider*>(p);
  if(request->service==host_get_player)out->data=&self.player;else if(request->service==host_get_current_level)out->data=&self.level;
  else if(request->service==host_get_range_rows){out->data=self.rows.data();out->count=self.rows.size();}else return 1;return 0;
 }
 static int target_service(void* p,TargetState48*,const TargetRequest24* request,std::uint32_t* out){auto& self=*static_cast<Provider*>(p);++self.target_calls;
  if(request->service==target_debug_load)return dh2_character_debug_load(self.debug,&self.debug_files)<0;
  if(request->service==target_debug_query){std::uint32_t value;auto status=dh2_character_debug_get(&value,self.debug,request->text,&self.debug_files);*out=value;return status<0;}
  if(request->service==target_owner_ai_id){std::int32_t value;auto status=dh2_character_target_ai_id(&value,self.session->properties()->resolved.data(),self.design->ai()->rows.size());std::memcpy(out,&value,4);return status;}
  if(self.fail_target||request->subject!=self.other_owner.identity)return 1;
  if(request->service==target_virtual_dead){*out=0;return 0;}
  if(request->service==target_in_sight){check(self.bindings.scope&&dh2_script_callback_scope_valid(self.bindings.scope));dh2_script_value value{};check(dh2_script_vm_get_global(self.vm,"seen",&value)==-1);++guards;
   dh2_script_value object{};object.type=DH2_SCRIPT_SOURCE_OBJECT;object.identity=request->subject;
   check(dh2_script_callback_call_discard_source(self.bindings.scope,"ObservedTarget",&object,1)==-1);++guards;
   check(dh2_script_vm_call_discard_source_objects(self.vm,"ObservedTarget",&object,1)==-1);++guards;
   check(!dh2_script_callback_call_discard_source_objects(self.bindings.scope,"ObservedTarget",&object,1));++self.scoped_calls;
   const float a[3]{0,0,0},b[3]{1,0,0};return dh2_character_target_sight(out,a,b,2.f);
  }return 1;
 }
 static int type(void* p,std::uintptr_t id,const char** out){auto& self=*static_cast<Provider*>(p);if(id!=self.owner.identity&&id!=self.other_owner.identity)return 1;return dh2::gameobject_lua::dh2_gameobject_lua_type(out,dh2::gameobject_lua::character);}
 static int command_number(void*,const dh2_script_value* a,float* out){if(a->type==DH2_SCRIPT_NUMBER)*out=a->number;else if(a->type==DH2_SCRIPT_BOOLEAN)*out=float(a->boolean);else if(a->type==DH2_SCRIPT_NIL)*out=0;else return 1;return 0;}
 static int control(void* p,const CharacterControlRequest32* request,CharacterControlResponse16* out){auto& self=*static_cast<Provider*>(p);
  if(request->service==control_is_remotely_updated){TimerOwner8 projected{-1,0,0,0};std::int32_t value=0;check(dh2_character_timer_owner_query(&value,&projected,0)==1);out->word=value;return 1;}
  if(request->service==control_target_position){check(request->subject==self.other_owner.identity);std::memcpy(out->position,self.target_position,12);return 1;}
  if(request->service==control_path_to){PathToState40 state{self.owner.identity,0,0,0,0,{0,0,0},0};PathToResult16 result{};
   PathToServices16 services{&self,[](void* p,const PathToRequest32* request,std::uint32_t* returned){auto& self=*static_cast<Provider*>(p);check(request->owner==self.owner.identity&&std::memcmp(request->target,self.target_position,12)==0);++self.path_queries;*returned=0;return 0;}};
   check(dh2_character_path_to(&result,&state,request->position,&services)==0&&result.requested&&result.find_result==0);return 1; // Explicit failed-FindPath fixture, not live navigation.
  }
  if(request->service==control_stop_object){self.command_state.path_count=0;return 1;} // Explicit borrowed GameObject.Stop fixture.
  if(request->service==control_character_event){check(request->argument==0x3f);return 1;} // Explicit Character event delivery fixture.
  return 0;
 }
 static int command(void* p,ScriptCommandState48* state,const ScriptCommandRequest40* request,const float**){auto& self=*static_cast<Provider*>(p);check(state==&self.command_state&&self.commands.scope&&dh2_script_callback_scope_valid(self.commands.scope));++self.command_calls;
  if(self.fail_command)return 1;
  if(request->service==script_controller_move_object||request->service==script_controller_stop){ControllerCommandState32 controller{self.command_state.controller,self.owner.identity,0,0,0,0};CharacterControlServices16 services{&self,control};
   check(dh2_character_controller_character(&controller,request->service==script_controller_stop?controller_stop:controller_move_object,request->target,&services)==1);++self.controller_calls;
  }else if(request->service==script_controller_attack){check(request->target==self.other_owner.identity);}else return 1; // Required combat boundary remains a fixture.
  dh2_script_value ignored{};check(dh2_script_vm_get_global(self.vm,"command_seen",&ignored)==-1);++guards;
  check(!dh2_script_callback_call_discard_source(self.commands.scope,"ObservedCommand",nullptr,0));return 0;
 }
 static int methods(void* p,std::uintptr_t id,const dh2_script_object_method** out,std::uint32_t* count){const char* unused;if(type(p,id,&unused))return 1;return dh2::gameobject_lua::dh2_gameobject_lua_methods(out,count,dh2::gameobject_lua::character);}
 static int gameplay(void* raw,std::uint32_t address,dh2_script_function* function,void** context){auto& self=*static_cast<Provider*>(raw);if(address!=0x38eb68u)return 0;
  *function=[](void* raw,const dh2_script_value*,std::uint32_t,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char*,std::size_t)->int{auto& self=*static_cast<Provider*>(raw);if(!out||!returned)return -1;++self.top_global_calls;
   if(self.top_global_empty){if(capacity<1)return -1;*returned=1;out[0]={};return 0;}
   if(capacity<5)return -1;*returned=5;out[0]={};out[0].type=DH2_SCRIPT_SOURCE_OBJECT;out[0].identity=self.other_owner.identity;
   out[1]={};out[1].type=DH2_SCRIPT_NUMBER;out[1].number=12.5f;out[2]={};out[2].type=DH2_SCRIPT_NUMBER;out[2].number=90.0f;
   out[3]={};out[3].type=DH2_SCRIPT_BOOLEAN;out[3].boolean=1;out[4]={};out[4].type=DH2_SCRIPT_NUMBER;out[4].number=0.0f;return 0;};*context=&self;return 1;}
 static int invoke(void* p,const dh2_script_callback_scope* scope,std::uintptr_t id,std::uint32_t address,const dh2_script_value*,std::uint32_t,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){auto& self=*static_cast<Provider*>(p);check(dh2_script_callback_scope_valid(scope)&&capacity);++source_methods;const char* type_name;if(type(p,id,&type_name))return 1;
  if(address==0x38ebe4)return dh2::gameobject_lua::dh2_gameobject_lua_get_id(out,returned,id);
  if(address==0x3b6c7c)return dh2::gameobject_lua::dh2_gameobject_lua_get_target(out,returned,id==self.owner.identity?&self.target:&self.other);
  std::snprintf(error,size,"Fixture does not deliver source method %x",address);return 1;
 }
};
void load(dh2_script_vm* vm,const std::string& code){auto status=dh2_script_vm_load_source_file(vm,code.data(),code.size());if(status)std::cerr<<dh2_script_vm_error(vm)<<'\n';check(!status);++vm_calls;}
}
int main(int argc,char** argv){try{
 check(argc==4);Inputs raw(argv[1]);auto common=file(argv[2]),monster=file(argv[3]);CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error));auto d=design.borrow();Provider provider(d);
 auto found=std::find(d.characters()->names.begin(),d.characters()->names.end(),"Crypt_Skeleton");check(found!=d.characters()->names.end());CharacterScriptSessionInput in;in.identity=provider.owner.identity;in.name="source_target_fixture";in.source_is_character=1;
 in.properties=std::make_shared<dh2::data::PropertyState>();in.combat=std::make_shared<dh2::data::CombatActorState>();dh2::data::reset_properties(*d.rules(),*in.properties,&d.characters()->rows[found-d.characters()->names.begin()]);check(dh2::data::recalc_properties_with_class(*d.classes(),*d.rules(),*in.properties,error));
 in.common={common.data(),common.size()};in.external={monster.data(),monster.size()};in.host=&provider.host;in.level=&provider.level_services;in.target=&provider.bindings;in.state_machine=&provider.fsm;in.objects=&provider.objects;in.gameplay_context=&provider;in.gameplay_binding=Provider::gameplay;
 auto session=CharacterScriptSession::create(design.borrow(),in,error);check(session&&error.empty());provider.session=session.get();check(!session->start()&&session->error().empty());ScriptSessionView view{};check(session->owner().active(view));provider.vm=view.vm;
 unsigned supported=0;for(const auto& entry:session->registrations())if(entry.supported)++supported;check(supported==36);
 load(view.vm,"seen=0; projected=0; function ObservedTarget(object) assert(type(object)=='table' and object:GetID()==object._this); seen=seen+1 end; function ArgumentProbe(object) if object==nil then nil_argument=true else assert(type(object)=='table' and object:GetID()==object._this); argument_id=object:GetID() end; return setmetatable({}, {__index=function(_,key) assert(key=='_this'); projected=projected+1; return argument_id end}) end; assert(not HasTarget() and GetTarget()==nil); assert(type(GetID())=='userdata'); assert(GetState()==3 and GetStateTime()==123); assert(select('#',SetTarget(nil))==0); assert(not HasTarget()); local top,distance,angle,is_character,reserved=GetTargetListTop(); assert(type(top)=='table' and top:GetID()==top._this); assert(distance==12.5 and angle==90 and is_character==true and reserved==0); assert(select('#',GetTargetListTop())==5)");check(provider.top_global_calls==2);
 provider.top_global_empty=true;load(view.vm,"local top=GetTargetListTop(); assert(select('#',GetTargetListTop())==1 and top==nil)");check(provider.top_global_calls==4);provider.top_global_empty=false;
 dh2_script_value argument{};argument.type=DH2_SCRIPT_SOURCE_OBJECT;argument.identity=provider.other_owner.identity;
 check(dh2_script_vm_call_discard_source(view.vm,"ArgumentProbe",&argument,1)==-1);++guards;
 check(!dh2_script_vm_call_discard_source_objects(view.vm,"ArgumentProbe",&argument,1));
 argument.identity=0;check(!dh2_script_vm_call_discard_source_objects(view.vm,"ArgumentProbe",&argument,1));
 argument.reserved=1;check(dh2_script_vm_call_discard_source_objects(view.vm,"ArgumentProbe",&argument,1)==-1);++guards;argument.reserved=0;
 check(dh2_script_vm_call_discard_source_objects(view.vm,"ArgumentProbe",nullptr,1)==-1);++guards;
 check(dh2_script_vm_call_discard_source_objects(view.vm,"ArgumentProbe",&argument,17)==-1);++guards;
 load(view.vm,"assert(nil_argument and projected==2); function SightOne() sight=1 end; function SightTwo() sight=2 end; AddToVFTable('OnTargetInSight','SightOne')");
 check(!session->dispatch_target(4));load(view.vm,"assert(sight==1); AddToVFTable('OnTargetInSight','SightTwo')");
 check(!session->dispatch_target(4));load(view.vm,"assert(sight==2)");
 check(session->dispatch_target(0)==-1);++guards;
 provider.target.target=provider.other_owner.identity; // Explicit caller-owned target fixture, not a scene manager claim.
 load(view.vm,"saved=GetTarget(); assert(type(saved)=='table' and saved:GetID()==saved._this); assert(saved:GetTarget()==nil); assert(HasTarget()); assert(select('#',SetTarget(saved,'ignored'))==0); assert(seen==1); assert(GetTarget():GetID()==saved:GetID()); assert(select('#',ClearTarget())==0); assert(not HasTarget() and GetTarget()==nil)");
 check(!provider.target.target&&!provider.target.last_target&&!provider.bindings.scope&&provider.scoped_calls==1&&provider.debug_opens==1);
 provider.state.current=5;provider.state.elapsed_ms=0x80000000u;load(view.vm,"assert(GetState(1,'ignored')==5 and GetStateTime()==-2147483648); SetTarget(saved._this); assert(seen==2 and HasTarget())");
 provider.fail_target=true;load(view.vm,"ok,err=pcall(SetTarget,saved); assert(not ok and string.find(err,'target delivery failed')); assert(HasTarget())");check(!provider.bindings.scope&&provider.target.target==provider.other_owner.identity);++guards;provider.fail_target=false;
 // Actual source monster enemy callback commits target then reaches missing HeadTo.
 provider.target.target=0;check(session->dispatch_target(1,provider.other_owner.identity)==-2);check(session->error().find("Unsupported source global HeadTo")!=std::string::npos);
 load(view.vm,"assert(HasTarget() and seen==3)");check(!provider.bindings.scope);++guards;
 auto unbound=dh2_script_vm_create(8u*1024u*1024u);check(unbound);load(unbound,"function ArgumentProbe(object) assert(object==nil) end");
 argument.identity=0;check(!dh2_script_vm_call_discard_source_objects(unbound,"ArgumentProbe",&argument,1));
 argument.identity=provider.other_owner.identity;check(dh2_script_vm_call_discard_source_objects(unbound,"ArgumentProbe",&argument,1)!=0);++guards;dh2_script_vm_destroy(unbound);
 auto invalid=in;invalid.objects=nullptr;invalid.target=nullptr;check(bool(CharacterScriptSession::create(design.borrow(),invalid,error)));invalid.objects=&provider.objects;check(!CharacterScriptSession::create(design.borrow(),invalid,error));++guards;
 invalid=in;provider.fsm.character=7;check(!CharacterScriptSession::create(design.borrow(),invalid,error));provider.fsm.character=in.identity;++guards;
 // Providers stay alive during VM close; methods use their captured raw identity.
 load(view.vm,"finalized=false; proxy=newproxy(true); getmetatable(proxy).__gc=function() assert(GetState()==5 and GetTarget():GetID()==saved:GetID()); finalized=true end");session.reset();
 // Optional command providers execute the REAL monster enemy callback, then
 // genuine Controller/Character/PathTo kernels to a declared failed-FindPath
 // fixture. This proves command routing, not scene movement or full combat.
 in.commands=&provider.commands;auto command_session=CharacterScriptSession::create(design.borrow(),in,error);check(command_session&&error.empty());provider.session=command_session.get();check(!command_session->start());check(command_session->owner().active(view));provider.vm=view.vm;provider.target.target=0;
 unsigned command_supported=0;for(const auto& entry:command_session->registrations())if(entry.supported)++command_supported;check(command_supported==42);
 load(view.vm,"seen=0; command_seen=0; command_projected=0; function ObservedTarget(object) assert(type(object)=='table'); seen=seen+1 end; function ObservedCommand() command_seen=command_seen+1; return setmetatable({}, {__index=function(_,key) assert(key=='_this'); command_projected=command_projected+1 end}) end");
 check(!command_session->dispatch_target(1,provider.other_owner.identity));check(provider.command_calls==1&&provider.controller_calls==1&&provider.path_queries==1&&!provider.commands.scope&&!provider.bindings.scope);
 load(view.vm,"saved=GetTarget(); assert(HasTarget() and seen==1 and command_seen==1 and command_projected==1); assert(not HasPath()); assert(select('#',HeadTo(saved))==0); MoveTo(saved); Stop(); Attack(saved); assert(command_seen==5 and command_projected==5)");
 check(provider.command_calls==5&&provider.controller_calls==4&&provider.path_queries==3&&!provider.commands.scope);
 provider.command_state.path_count=1;load(view.vm,"assert(HasPath('ignored')); assert(select('#',HeadTo(nil))==0); assert(select('#',MoveTo(1,2))==0)");check(provider.command_calls==5);
 provider.fail_command=true;load(view.vm,"local ok,err=pcall(HeadTo,saved); assert(not ok and string.find(err,'Native session command delivery failed')); assert(HasTarget())");check(!provider.commands.scope&&provider.target.target==provider.other_owner.identity);++guards;provider.fail_command=false;
 invalid=in;provider.command_state.character=7;check(!CharacterScriptSession::create(design.borrow(),invalid,error));provider.command_state.character=in.identity;++guards;
 command_session.reset();
 Dl_info world{},runtime{};check(dladdr(reinterpret_cast<void*>(dh2_character_target_identity),&world)&&dladdr(reinterpret_cast<void*>(dh2_script_vm_bind_source_objects),&runtime));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"VM_calls\":"<<vm_calls<<",\"source_object_method_calls\":"<<source_methods<<",\"guards\":"<<guards<<",\"scoped_target_callbacks\":"<<provider.scoped_calls<<",\"supported_registrations\":"<<supported<<",\"command_supported_registrations\":"<<command_supported<<",\"actual_controller_character_calls\":"<<provider.controller_calls<<",\"actual_PathTo_failed_FindPath_fixture_queries\":"<<provider.path_queries<<",\"fixture_host_target_FSM\":true,\"full_enemy_AI\":false,\"world_library\":\""<<world.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\"}\n";
 }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
