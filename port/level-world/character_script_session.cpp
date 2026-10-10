#include "character_script_session.hpp"
#include "character_property_bindings.hpp"
#include "gameobject_lua_representation.hpp"
#include "object_identity.hpp"
#include "character_register_summon_v81.hpp"
#include "../script-runtime/script_game_bindings.h"
#include <map>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <limits>

namespace dh2::character {
namespace {
using Function=dh2_script_function;
int fail(char* out,std::size_t n,const char* message){if(out&&n)std::snprintf(out,n,"%s",message);return 1;}
int required_fail(char* out,std::size_t n,const char* message){fail(out,n,message);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
std::vector<std::uint8_t> copy(data::Bytes b){if((b.size&&!b.data)||b.size>8u*1024u*1024u)throw std::invalid_argument("Invalid session script bytes");return b.size?std::vector<std::uint8_t>(b.data,b.data+b.size):std::vector<std::uint8_t>();}
}
struct CharacterScriptSession::Impl {
 CharacterGameDesign::Borrow design;
 std::shared_ptr<data::PropertyState> properties;
 std::shared_ptr<data::CombatActorState> combat;
 std::shared_ptr<data::PropertySheet> temporary;
 std::array<float,3> position;
 std::uintptr_t identity;
 std::string name,script,message;
 std::uint32_t source_is_character;
 const HostContextBindings16* host;
 const LevelServices16* level_service;
 TargetBindings48* target;
 NativeFsm24* state_machine;
 const dh2_script_object_services* objects;
 ScriptCommandBindings40* commands;
 CharacterScriptSessionInput::MotionCapabilityServicesV1 motion_capabilities;
 CharacterScriptSessionInput::AnimationRegistrationServicesV1 animation_registration;
 void* commands_refresh_context{};int(*commands_refresh)(void*){};
 const TimerServices32* expiry;
 void* budget_context;
 int(*budget)(void*,std::uint32_t,std::int32_t*);
 dh2_script_scalar_bindings scalar;
 RegisterSummonBindingsV81 summon_v81;
 void* gameplay_context{};
 int(*gameplay_binding)(void*,std::uint32_t,dh2_script_function*,void**){};
 const dh2_script_callback_scope* skill_callback_scope{};
 struct ScopedGameplayBinding {Impl* self;Function function;void* context;};
 std::map<std::pair<std::uintptr_t,std::uint32_t>,ScopedGameplayBinding> scoped_gameplay_bindings;
 data::PropertyView property_view;
 PropertyBindings48 property_bindings{};
 LevelModel32 model{};
 LevelBindings48 level_bindings{};
 SpatialBindings40 spatial{};
 std::vector<Timer32> slots;
 TimerStore32 timers{};
 TimerServices32 timer_services{};
 dh2_script_game_bindings timer_bindings{};
 std::int32_t timer_error=0;
 std::map<std::string,std::vector<std::uint8_t>> files;
 std::vector<ScriptSessionRegistration> delivered;
 std::vector<std::string> missing;
 struct PrivateBinding {Impl* self;std::uintptr_t identity;dh2_script_vm* vm;dh2_script_aliases* aliases;bool objects_installed=false;};
 std::map<std::uintptr_t,PrivateBinding> private_bindings;
 struct UnsupportedBinding {std::string name;std::uint32_t original_callback;};
 struct MotionBinding {Impl* self;std::uint32_t address;};
 std::map<std::pair<std::uint32_t,std::uint32_t>,UnsupportedBinding> unsupported_bindings;
 std::map<std::pair<std::uintptr_t,std::uint32_t>,MotionBinding> motion_bindings;
 ScriptOwnerServices services{this,&service};
 // Must be destroyed FIRST: every other member is a bound VM dependency.
 std::unique_ptr<ScriptOwner> owner;
 Impl(CharacterGameDesign::Borrow&& d,const CharacterScriptSessionInput& in):
  design(std::move(d)),properties(in.properties),combat(in.combat),temporary(in.temporary),
  position(in.position),identity(in.identity),name(in.name),source_is_character(in.source_is_character),
  host(in.host),level_service(in.level),target(in.target),state_machine(in.state_machine),objects(in.objects),commands(in.commands),motion_capabilities(in.motion_capabilities),animation_registration(in.animation_registration),expiry(in.timer_services),budget_context(in.budget_context),budget(in.budget),scalar(in.scalar),
  property_view(data::property_view(*design.rules(),*properties)),slots(in.timer_capacity){
  commands_refresh_context=in.commands_refresh_context;commands_refresh=in.commands_refresh;
  gameplay_context=in.gameplay_context;gameplay_binding=in.gameplay_binding;
  summon_v81={in.summon_cache_v81,design.characters(),in.register_summon_v81};
  const auto* ai=data::ai_props(*design.ai(),properties->resolved[1]);if(!ai)throw std::invalid_argument("Missing source AI row");script=ai->script;
  files.emplace("data/scripts/ai/_commons.luac",copy(in.common));
  if(!script.empty()&&script.rfind("__",0)!=0){
   std::string path="data/scripts/ai/"+script;
   const char* suffix=std::strstr(script.c_str(),".lua");
   if(!suffix)path+=".luac";else if(std::strncmp(suffix,".luac",5))path+='c';
   files.emplace(std::move(path),copy(in.external));
  }
  for(const auto& file:in.include_files){if(file.filename.empty()||file.bytes.size()>8u*1024u*1024u||!files.emplace(file.filename,file.bytes).second)throw std::invalid_argument("Duplicate/invalid session cache file");}
  property_bindings={{properties->resolved.data(),224,0},{temporary?temporary->data():nullptr,temporary?224u:0u,0},nullptr,nullptr};
  std::string error;if(!design.level_model(model,property_view,error))throw std::invalid_argument(error);
  level_bindings={&model,level_service?*level_service:LevelServices16{},{0,0,0}};
  spatial={position.data(),this,nullptr,nullptr,0};
  timers={slots.data(),0,static_cast<std::uint32_t>(slots.size()),identity,0,0};
  timer_services={this,expiry&&expiry->expired?&expire:nullptr,&grow,0};
  timer_bindings={this,identity,&start_timer,&stop_timer,0};
  owner.reset(new ScriptOwner(identity,in.vm_memory_limit,in.integers));
  owner->lifecycle().delayed=ai->delayed_load;
 }
 ~Impl(){owner.reset();}
 static int grow(void* opaque,TimerStore32* store,std::uint32_t requested){
  auto& t=*static_cast<Impl*>(opaque);if(store!=&t.timers||store->update_depth||requested>1048576)return 0;
  try{auto n=std::max(requested,store->capacity?store->capacity*2:20u);t.slots.resize(n);store->slots=t.slots.data();store->capacity=n;return 1;}catch(const std::bad_alloc&){return 0;}
 }
 static void expire(void* opaque,std::uintptr_t id,std::int32_t event,Timer32* timer){auto& t=*static_cast<Impl*>(opaque);t.expiry->expired(t.expiry->context,id,event,timer);}
 static std::int32_t start_timer(void* opaque,std::uintptr_t id,std::uint32_t duration,std::int32_t repeat,std::int32_t event,std::uintptr_t ref){
  auto& t=*static_cast<Impl*>(opaque);if(id!=t.identity){t.timer_error=-1;return -1;}
  auto result=dh2_character_timer_start(&t.timers,duration,repeat,event,ref,&t.timer_services);if(result<0){t.timer_error=result;return -1;}return result;
 }
 static void stop_timer(void* opaque,std::uintptr_t id,std::uint32_t timer){auto& t=*static_cast<Impl*>(opaque);if(id!=t.identity){t.timer_error=-1;return;}auto result=dh2_character_timer_stop(&t.timers,timer);if(result<0)t.timer_error=result;}
 static int timer_callback(void* opaque,const dh2_script_value* a,std::uint32_t n,dh2_script_value* out,std::uint32_t cap,std::uint32_t* count,char* error,std::size_t size,bool start){
  auto& t=*static_cast<Impl*>(opaque);t.timer_error=0;auto result=(start?dh2_script_game_start_timer:dh2_script_game_stop_timer)(&t.timer_bindings,a,n,out,cap,count,error,size);
  if(!result&&t.timer_error){if(count)*count=0;return fail(error,size,"Native session timer delivery failed");}return result;
 }
 static int start_callback(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return timer_callback(p,a,n,o,c,r,e,z,true);}
 static int stop_callback(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return timer_callback(p,a,n,o,c,r,e,z,false);}
 static int aliases(void* opaque,const dh2_script_value* a,std::uint32_t n,dh2_script_value*,std::uint32_t,std::uint32_t* count,char* error,std::size_t size,unsigned op){
  auto& b=*static_cast<PrivateBinding*>(opaque);if(!count||(n&&!a))return fail(error,size,"Malformed session alias call");*count=0;
  auto result=op==0?dh2_script_alias_add_values(b.aliases,a,n):op==1?dh2_script_alias_push(b.aliases):dh2_script_alias_pop(b.aliases);
  return result?fail(error,size,"Native session alias delivery failed"):0;
 }
 static int alias_add(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return aliases(p,a,n,o,c,r,e,z,0);}
 static int alias_push(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return aliases(p,a,n,o,c,r,e,z,1);}
 static int alias_pop(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return aliases(p,a,n,o,c,r,e,z,2);}
 static int include(void* opaque,const dh2_script_include_scope* scope,const char* name,char* error,std::size_t size){
  auto& b=*static_cast<PrivateBinding*>(opaque);auto* owner=b.self->owner.get();if(!owner)return fail(error,size,"Closing session Include unsupported");
  auto result=owner->include(b.identity,name,b.self->services,scope);return result?fail(error,size,"Native session Include delivery failed"):0;
 }
 struct HostCapture {Impl* self;dh2_script_value* out;std::uint32_t count,capacity;};
 static int host_capture(void* opaque,const HostContextRequest16* req,HostContextResponse16* response){
  auto& c=*static_cast<HostCapture*>(opaque);
  if(req->service!=host_push_integer)return c.self->host->services.invoke(c.self->host->services.context,req,response);
  if(c.count>=c.capacity)return 1;
  auto& v=c.out[c.count++];v={};v.type=DH2_SCRIPT_NUMBER;v.number=static_cast<float>(req->value);return 0;
 }
 static int host_callback(void* opaque,const dh2_script_value* a,std::uint32_t n,dh2_script_value* out,std::uint32_t cap,std::uint32_t* count,char* error,std::size_t size,std::uint32_t op){
  auto& t=*static_cast<Impl*>(opaque);if(!count)return fail(error,size,"Malformed host output");*count=0;
  if(!t.host||!t.host->services.invoke||!out||cap<(op==host_level_range?2u:1u))return fail(error,size,"Required native host context unavailable");
  HostCapture capture{&t,out,0,cap};HostContextServices16 source{&capture,&host_capture};auto result=dh2_character_host_context_query(op,a,n,&source);
  if(result<0)return fail(error,size,"Native session host delivery failed");
  *count=capture.count;return 0;
 }
 static int host_level(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return host_callback(p,a,n,o,c,r,e,z,host_player_level);}
 static int host_difficulty(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return host_callback(p,a,n,o,c,r,e,z,host_player_difficulty);}
 static int host_range(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return host_callback(p,a,n,o,c,r,e,z,host_level_range);}
 static int get_id(void* opaque,const dh2_script_value*,std::uint32_t,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
  auto& t=*static_cast<Impl*>(opaque);if(!returned||!out||!capacity)return fail(error,size,"Malformed GetID output");*returned=0;
  return dh2::gameobject_lua::dh2_gameobject_lua_get_id(out,returned,t.identity)?fail(error,size,"Native GetID delivery failed"):0;
 }
 static int get_target(void* opaque,const dh2_script_value*,std::uint32_t,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
  auto& t=*static_cast<Impl*>(opaque);if(!returned||!out||!capacity||!t.target)return fail(error,size,"Malformed GetTarget output");*returned=0;
  return dh2::gameobject_lua::dh2_gameobject_lua_get_target(out,returned,t.target->state)?fail(error,size,"Native GetTarget delivery failed"):0;
 }
 static int target_scoped(void* opaque,const dh2_script_callback_scope* scope,const dh2_script_value* args,std::uint32_t count,char* error,std::size_t size,bool clear){
  auto& t=*static_cast<Impl*>(opaque);if(!t.target||!dh2_script_callback_scope_valid(scope))return fail(error,size,"Malformed target callback capability");
  struct Restore {TargetBindings48* target;const dh2_script_callback_scope* old;~Restore(){target->scope=old;}} restore{t.target,t.target->scope};t.target->scope=scope;
  auto result=clear?dh2_character_clear_target(t.target->state,&t.target->services):dh2_character_target_set_values(t.target,args,count);
  return result?fail(error,size,"Native session target delivery failed"):0;
 }
 static int set_target(void* p,const dh2_script_callback_scope* s,const dh2_script_value* a,std::uint32_t n,char* e,std::size_t z){return target_scoped(p,s,a,n,e,z,false);}
 static int clear_target(void* p,const dh2_script_callback_scope* s,const dh2_script_value* a,std::uint32_t n,char* e,std::size_t z){return target_scoped(p,s,a,n,e,z,true);}
 static int command_scoped(void* opaque,const dh2_script_callback_scope* scope,const dh2_script_value* args,std::uint32_t count,char* error,std::size_t size,std::uint32_t op){
  auto& t=*static_cast<Impl*>(opaque);if(!t.commands||!dh2_script_callback_scope_valid(scope))return fail(error,size,"Malformed command callback capability");
  if(t.commands_refresh&&t.commands_refresh(t.commands_refresh_context))return fail(error,size,"Required live NPC command projection");
  struct Restore{ScriptCommandBindings40* commands;const dh2_script_callback_scope* old;~Restore(){commands->scope=old;}} restore{t.commands,t.commands->scope};t.commands->scope=scope;
  std::uint32_t returned=0;auto result=dh2_character_script_command(t.commands->state,op,args,count,&t.commands->services,nullptr,0,&returned);
  if(result!=1){if(error&&size)std::snprintf(error,size,"Native session command delivery failed %d",result);return 1;}return 0;
 }
 static int command_stop(void* p,const dh2_script_callback_scope* s,const dh2_script_value* a,std::uint32_t n,char* e,std::size_t z){return command_scoped(p,s,a,n,e,z,script_stop);}
 static int command_head(void* p,const dh2_script_callback_scope* s,const dh2_script_value* a,std::uint32_t n,char* e,std::size_t z){return command_scoped(p,s,a,n,e,z,script_head_to);}
 static int command_move(void* p,const dh2_script_callback_scope* s,const dh2_script_value* a,std::uint32_t n,char* e,std::size_t z){return command_scoped(p,s,a,n,e,z,script_move_to);}
 static int command_attack(void* p,const dh2_script_callback_scope* s,const dh2_script_value* a,std::uint32_t n,char* e,std::size_t z){return command_scoped(p,s,a,n,e,z,script_attack);}
 static int command_flee(void* p,const dh2_script_callback_scope* s,const dh2_script_value* a,std::uint32_t n,char* e,std::size_t z){return command_scoped(p,s,a,n,e,z,script_flee);}
 static int command_has_path(void* opaque,const dh2_script_value* args,std::uint32_t count,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
  auto& t=*static_cast<Impl*>(opaque);
  if(t.commands_refresh&&t.commands_refresh(t.commands_refresh_context))return fail(error,size,"Required live NPC HasPath projection");
  return t.commands&&dh2_character_script_command(t.commands->state,script_has_path,args,count,&t.commands->services,out,capacity,returned)==1?0:fail(error,size,"Native session HasPath delivery failed");
 }
 struct TargetDispatch {Impl* self;ScriptSessionView view;const dh2_script_callback_scope* scope;};
 static int target_dispatch(void* opaque,const dh2::object_identity::TargetCall32* call){
  auto& delivery=*static_cast<TargetDispatch*>(opaque);
  if(!call||call->receiver!=delivery.view.identity||!call->callback||call->argument_count>1||(call->argument_count&&call->value_type!=DH2_SCRIPT_SOURCE_OBJECT))return 1;
  dh2_script_value argument{};argument.type=DH2_SCRIPT_SOURCE_OBJECT;argument.identity=call->argument;
  const auto name=dh2_script_alias_resolve(delivery.view.aliases,call->callback);
  const auto status=delivery.scope?dh2_script_callback_call_discard_source_objects(delivery.scope,name,&argument,call->argument_count):dh2_script_vm_call_discard_source_objects(delivery.view.vm,name,&argument,call->argument_count);
  if(status)delivery.self->message=delivery.scope?dh2_script_callback_error(delivery.scope):dh2_script_vm_error(delivery.view.vm);
  return status?1:0;
 }
 static int unsupported(void* opaque,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t* count,char* error,std::size_t size){
  const auto& b=*static_cast<const UnsupportedBinding*>(opaque);if(count)*count=0;
  if(error&&size)std::snprintf(error,size,"Unsupported source global %s (original callback 0x%08x)",b.name.c_str(),b.original_callback);
  return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }
 static int motion_capability(void* opaque,const dh2_script_value* args,std::uint32_t count,
  dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
  const auto& binding=*static_cast<MotionBinding*>(opaque);auto& self=*binding.self;
  if(!returned||(count&&!args))return fail(error,size,"Malformed GameObject motion capability call");
  *returned=0;
  const bool setter=binding.address==0x390900u||binding.address==0x39088cu;
  std::uint32_t value=0;
  if(setter){
   // The source setters do nothing when arg0 is absent or not a Lua boolean.
   if(!count||args[0].type!=DH2_SCRIPT_BOOLEAN)return 0;
   if(args[0].reserved||args[0].boolean>1)return fail(error,size,"Malformed source GameObject motion boolean");
   value=args[0].boolean;
  }else if(!out||!capacity)return fail(error,size,"GameObject motion query result unavailable");
  if(!self.motion_capabilities.invoke||self.motion_capabilities.invoke(
     self.motion_capabilities.context,binding.address,&value)||value>1)
   return required_fail(error,size,"Required same GameObject PFObject motion capability unavailable");
  if(!setter){out[0]={};out[0].type=DH2_SCRIPT_BOOLEAN;out[0].boolean=value;*returned=1;}
  return 0;
 }
 static int register_animation(void* opaque,const dh2_script_value* args,std::uint32_t count,
  dh2_script_value*,std::uint32_t,std::uint32_t* returned,char* error,std::size_t size){
  auto& self=*static_cast<Impl*>(opaque);
  if(!returned||(count&&!args))return fail(error,size,"Malformed RegisterAnim call");
  *returned=0;
  // Character::_RegisterAnim accepts only a present numeric first argument.
  if(!count||args[0].type!=DH2_SCRIPT_NUMBER)return 0;
  const float number=args[0].number;
  if(!std::isfinite(number)||double(number)<double(std::numeric_limits<std::int32_t>::min())||
     double(number)>double(std::numeric_limits<std::int32_t>::max()))
   return fail(error,size,"RegisterAnim source number is outside signed id range");
  if(!self.animation_registration.register_dictionary||self.animation_registration.register_dictionary(
      self.animation_registration.context,static_cast<std::int32_t>(number)))
   return required_fail(error,size,"Required same Character CharAnimator AddAnimDictToSet unavailable");
  return 0;
 }
 static int design_query(void* context,const dh2_script_value* args,std::uint32_t count,
  dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size,Function callback){
  // A native lookup delivery error is not an authored member miss (-1 in a
  // successful numeric result) or the source arity/type no-effect guard.
  // Keep it fatal to the owner even when source Lua catches the diagnostic.
  return callback(context,args,count,out,capacity,returned,error,size)?DH2_SCRIPT_REQUIRED_SERVICE_FAILURE:0;
 }
 static int design_constant(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return design_query(p,a,n,o,c,r,e,z,&dh2_script_design_get_constant);}
 static int design_struct(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return design_query(p,a,n,o,c,r,e,z,&dh2_script_design_get_struct);}
 static int design_oid(void* p,const dh2_script_value* a,std::uint32_t n,dh2_script_value* o,std::uint32_t c,std::uint32_t* r,char* e,std::size_t z){return design_query(p,a,n,o,c,r,e,z,&dh2_script_design_get_oid);}
 static int get_prop_hp(void* opaque,const dh2_script_value* args,std::uint32_t count,
  dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
  auto& self=*static_cast<Impl*>(opaque);
  if(!returned||(count&&!args)||!out||capacity<3)return fail(error,size,"Malformed Character.GetPropHP output");
  *returned=0;
  const std::int32_t hp=self.properties->resolved[36],maximum=self.properties->resolved[38];
  std::int32_t current_integer=0,maximum_integer=0,percent=0;
  if(maximum){
   maximum_integer=maximum>>8;
   // The native function divides by (Max_HP >> 8), even when Max_HP itself
   // is nonzero. Preserve the source fault instead of inventing a percentage.
   if(!maximum_integer)return fail(error,size,"Character.GetPropHP source integer divide by zero");
   current_integer=hp>>8;
   const std::uint32_t product_bits=std::uint32_t(hp)*100u;
   std::int32_t product;std::memcpy(&product,&product_bits,sizeof(product));
   if(product==std::numeric_limits<std::int32_t>::min()&&maximum_integer==-1)
    return fail(error,size,"Character.GetPropHP source signed division overflow");
   percent=(product/maximum_integer)>>8;
  }
  out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=static_cast<float>(current_integer);
  out[1]={};out[1].type=DH2_SCRIPT_NUMBER;out[1].number=static_cast<float>(maximum_integer);
  out[2]={};out[2].type=DH2_SCRIPT_NUMBER;out[2].number=static_cast<float>(percent);
  *returned=3;return 0;
 }
 static int gameplay_scoped(void* opaque,const dh2_script_callback_scope* scope,
  const dh2_script_value* args,std::uint32_t count,dh2_script_value* results,std::uint32_t capacity,
  std::uint32_t* returned,char* error,std::size_t size){
  auto& binding=*static_cast<ScopedGameplayBinding*>(opaque);auto& self=*binding.self;
  if(!binding.function||!dh2_script_callback_scope_valid(scope))return fail(error,size,"Malformed NPC skill native capability");
  struct Restore {Impl& self;const dh2_script_callback_scope* old;~Restore(){self.skill_callback_scope=old;}} restore{self,self.skill_callback_scope};
  self.skill_callback_scope=scope;
  return binding.function(binding.context,args,count,results,capacity,returned,error,size);
 }
 bool binding(const ScriptOwnerRequest& r){
  const auto& b=*r.binding;auto& c=private_bindings[r.session->identity];
  if(!c.vm)c={this,r.session->identity,r.session->vm,r.session->aliases};
  else if(c.vm!=r.session->vm||c.aliases!=r.session->aliases)throw std::runtime_error("Private session receiver changed");
  Function function=nullptr;void* context=nullptr;
  if(gameplay_binding){const int selected=gameplay_binding(gameplay_context,b.original_callback,&function,&context);
   if(selected<0||selected>1)throw std::runtime_error("NPC gameplay source binding delivery failed");
   if(selected>0){if(!function)throw std::runtime_error("NPC gameplay binding lacks actual native callback");
    if(b.original_callback==0x38eb68||b.original_callback==0x39193c){
     if(!objects)throw std::runtime_error("NPC target-list top lacks the same World source-object provider");
     if(!c.objects_installed){if(dh2_script_vm_set_source_objects(c.vm,objects))throw std::runtime_error("NPC target-list source objects registration failed");c.objects_installed=true;}
     if(dh2_script_vm_bind_source_objects(c.vm,b.name,function,context))throw std::runtime_error("NPC target-list source object registration failed");
    }else if(b.original_callback==0x3b8bd8||b.original_callback==0x3b9fbc||b.original_callback==0x3b99d0||b.original_callback==0x3ba298){
     if(objects&&!c.objects_installed){if(dh2_script_vm_set_source_objects(c.vm,objects))throw std::runtime_error("NPC skill source objects registration failed");c.objects_installed=true;}
     auto& scoped=scoped_gameplay_bindings[{c.identity,b.original_callback}];scoped={this,function,context};
     if(dh2_script_vm_bind_source_scoped_values(c.vm,b.name,&gameplay_scoped,&scoped))throw std::runtime_error("NPC skill scoped source registration failed");
    }else if(dh2_script_vm_bind_source_values(c.vm,b.name,function,context))throw std::runtime_error("NPC gameplay source registration failed");
    return true;
   }
  }
  if(commands){
   dh2_script_scoped_function command=nullptr;
   if(b.original_callback==0x3b56b4)command=&command_stop;
   else if(b.original_callback==0x3ba71c)command=&command_head;
   else if(b.original_callback==0x3bada8)command=&command_move;
   else if(b.original_callback==0x3b9f44)command=&command_attack;
   else if(b.original_callback==0x3b91a0)command=&command_flee;
   if(command){if(dh2_script_vm_bind_source_scoped(c.vm,b.name,command,this))throw std::runtime_error("Scoped command registration failed");return true;}
  }
  if(motion_capabilities.invoke&&
     (b.original_callback==0x390900u||b.original_callback==0x39088cu||
      b.original_callback==0x38ea94u||b.original_callback==0x38ea74u)){
   auto& native=motion_bindings[{c.identity,b.original_callback}];native={this,b.original_callback};
   if(dh2_script_vm_bind_source_values(c.vm,b.name,&motion_capability,&native))
    throw std::runtime_error("GameObject PFObject motion binding failed");
   return true;
  }
  if(animation_registration.register_dictionary&&b.original_callback==0x3b70d8u&&
     (!std::strcmp(b.name,"RegisterAnim")||!std::strcmp(b.name,"RegisterActorAnim"))){
   if(dh2_script_vm_bind_source_values(c.vm,b.name,&register_animation,this))
    throw std::runtime_error("Character CharAnimator RegisterAnim binding failed");
   return true;
  }
  if(!std::strcmp(b.name,"Include")&&b.original_callback==0x37efe4){if(dh2_script_vm_bind_source_include(c.vm,&include,&c))throw std::runtime_error("Include registration failed");return true;}
  if(!std::strcmp(b.name,"SetInt")&&b.original_callback==0x37de5c)return true; // Installed by genuine native-integer ScriptOwner before this delivery.
  if(!std::strcmp(b.name,"GetInt")&&b.original_callback==0x37ec14)return true;
  if((!std::strcmp(b.name,"RegisterAIState")&&b.original_callback==0x3da144u)||(!std::strcmp(b.name,"ChangeAIState")&&b.original_callback==0x3d9710u))return true; //SAME private Session installed this exact source entry before delivery.
  if(target&&b.original_callback==0x3b6c7c&&objects){
   if(!c.objects_installed){if(dh2_script_vm_set_source_objects(c.vm,objects))throw std::runtime_error("Source object provider registration failed");c.objects_installed=true;}
   if(dh2_script_vm_bind_source_objects(c.vm,b.name,&get_target,this))throw std::runtime_error("Source GetTarget registration failed");
   return true;
  }
  if(target&&(b.original_callback==0x3b8f38||b.original_callback==0x3b5690)){
   if(dh2_script_vm_bind_source_scoped(c.vm,b.name,b.original_callback==0x3b8f38?&set_target:&clear_target,this))throw std::runtime_error("Scoped target registration failed");
   return true;
  }
  if(b.original_callback==0x37ee80)function=&dh2_script_game_trace;
  else if((target||state_machine)&&b.original_callback==0x38ebe4){function=&get_id;context=this;}
  else if(target&&b.original_callback==0x3b6f50){function=&dh2_character_target_has_lua;context=target;}
  else if(state_machine&&b.original_callback==0x3b6d78){function=&dh2_character_native_fsm_script_get_state;context=state_machine;}
  else if(state_machine&&b.original_callback==0x3b6d6c){function=&dh2_character_native_fsm_script_get_time;context=state_machine;}
  else if(commands&&b.original_callback==0x38e98c){function=&command_has_path;context=this;}
  else if(b.original_callback==0x37ec70){function=&alias_add;context=&c;}
  else if(b.original_callback==0x37be00){function=&alias_push;context=&c;}
  else if(b.original_callback==0x37dc44){function=&alias_pop;context=&c;}
  else if(b.original_callback==0x37f354){function=&design_constant;context=const_cast<dh2_script_design_bindings*>(design.design());}
  else if(b.original_callback==0x37f4a8){function=&design_struct;context=const_cast<dh2_script_design_bindings*>(design.design());}
  else if(b.original_callback==0x37f5fc){function=&design_oid;context=const_cast<dh2_script_design_bindings*>(design.design());}
  else if(b.original_callback==0x37cc00){function=&host_level;context=this;}
  else if(b.original_callback==0x37cb8c){function=&host_difficulty;context=this;}
  else if(b.original_callback==0x37f1f0){function=&host_range;context=this;}
  else if(b.original_callback==0x3b9d8c){function=&dh2_character_get_prop;context=&property_bindings;}
  else if(b.original_callback==0x3b6cd8u&&(!std::strcmp(b.name,"GetPropHP")||!std::strcmp(b.name,"GetHP"))){function=&get_prop_hp;context=this;}
  else if(b.original_callback==0x3b73a4){function=&dh2_character_set_level_lua;context=&level_bindings;}
  else if(b.original_callback==0x38e700){function=&dh2_character_get_position;context=&spatial;}
  else if(b.original_callback==0x3b7590){function=&start_callback;context=this;}
  else if(b.original_callback==0x3b7064){function=&stop_callback;context=this;}
  else if(b.original_callback==0x392714){function=&dh2_character_register_summon_v81;context=&summon_v81;}
  const std::uint32_t addresses[]={0x37ebc4,0x37ee84,0x37e1a4,0x37e068,0x37f814,0x37e9ec,0x37e814,0x37f750};
  const Function callbacks[]={dh2_script_scalar_to_fixed,dh2_script_scalar_from_fixed,dh2_script_scalar_mul_fixed,dh2_script_scalar_div_fixed,dh2_script_scalar_bit_not,dh2_script_scalar_bit_and,dh2_script_scalar_bit_or,dh2_script_scalar_bit_xor};
  for(unsigned i=0;i<8;++i)if(b.original_callback==addresses[i]){function=callbacks[i];context=&scalar;}
  if(!function){
   auto& unavailable=unsupported_bindings[{r.argument0,r.argument1}];unavailable={b.name,b.original_callback};
   if(dh2_script_vm_bind_source_values(c.vm,b.name,&unsupported,&unavailable))throw std::runtime_error("Unsupported source registration failed");
   return false;
  }
  if(dh2_script_vm_bind_source_values(c.vm,b.name,function,context))throw std::runtime_error("Session registration failed");
  return true;
 }
 static int service(void* opaque,ScriptOwner& selected,const ScriptOwnerRequest& r,ScriptOwnerResponse& out){
  auto& t=*static_cast<Impl*>(opaque);try{
   if(r.reserved)return 1;
   switch(r.service){
    case owner_register_binding:{if(!r.binding||!r.session||r.binding->method)return 1;auto supported=t.binding(r);t.delivered.push_back({r.argument0,r.argument1,r.binding->original_callback,r.binding->name,supported,true});if(!supported&&std::find(t.missing.begin(),t.missing.end(),r.binding->name)==t.missing.end())t.missing.emplace_back(r.binding->name);return 0;}
    case owner_cached_file:{auto found=t.files.find(r.filename?r.filename:"");out.word=found!=t.files.end();if(out.word){out.bytes=found->second.data();out.size=found->second.size();}return 0;}
    case owner_is_character:if(r.subject!=t.identity)return 1;out.word=t.source_is_character;return 0;
    case owner_budget:{if(!t.budget)return 1;std::int32_t available;if(t.budget(t.budget_context,r.argument0,&available))return 1;std::memcpy(&out.word,&available,4);return 0;}
    case owner_is_dead:if(r.subject!=t.identity)return 1;out.word=t.combat->dead;return 0;
    case owner_timer_stop:if(r.subject!=t.identity||dh2_character_timer_stop(&t.timers,r.argument0)<0)return 1;return 0;
    case owner_design_tick:{const char* name=r.argument0==0x33?"AI_Tick":r.argument0==0x34?"DoT_Tick":nullptr;if(!name)return 1;std::int32_t tick;auto* d=t.design.design();if(d->lookup(d->context,0,"CharacterDesign",name,&tick))return 1;std::memcpy(&out.word,&tick,4);return 0;}
    case owner_timer_start:{if(r.subject!=t.identity||(r.argument1!=0x33&&r.argument1!=0x34))return 1;auto id=dh2_character_timer_start(&t.timers,r.argument0,-1,static_cast<std::int32_t>(r.argument1),0,&t.timer_services);if(id<0)return 1;out.word=static_cast<std::uint32_t>(id);return 0;}
    case owner_ai_terminate:t.message="Script termination/replacement backend unavailable";return 1;
    default:t.message="Selected script service backend unavailable";return 1;
   }
  }catch(const std::exception& e){t.message=e.what();return 1;}
  (void)selected;
 }
};
CharacterScriptSession::CharacterScriptSession(std::unique_ptr<Impl> impl):impl_(std::move(impl)){}
CharacterScriptSession::~CharacterScriptSession()=default;
std::unique_ptr<CharacterScriptSession> CharacterScriptSession::create(CharacterGameDesign::Borrow&& design,const CharacterScriptSessionInput& in,std::string& error){
 error.clear();if(!design||!in.identity||!in.properties||!in.combat||in.source_is_character>1||in.combat->dead>1||in.scalar.reserved||in.integers.reserved||!in.timer_capacity||in.timer_capacity>1048576||in.vm_memory_limit<65536||in.vm_memory_limit>1073741824||(in.timer_services&&in.timer_services->reserved)){error="Malformed character script session input";return nullptr;}
 std::uint32_t present=0;std::int32_t current=0;
 if((in.target&&(in.target->scope||in.target->reserved[0]||in.target->reserved[1]||!in.target->services.invoke||dh2_character_has_target(&present,in.target->state)))
    ||(in.state_machine&&(in.state_machine->character!=in.identity||dh2_character_native_fsm_get_integer(&current,in.state_machine,0)!=1))
    ||(in.objects&&(!in.target||!in.objects->type_name||!in.objects->methods||!in.objects->invoke))
    ||(in.commands&&(in.commands->scope||!in.commands->state||in.commands->state->character!=in.identity||in.commands->state->reserved||!in.commands->services.invoke))){error="Malformed character target/FSM/command providers";return nullptr;}
 try{return std::unique_ptr<CharacterScriptSession>(new CharacterScriptSession(std::make_unique<Impl>(std::move(design),in)));}catch(const std::exception& e){error=e.what();return nullptr;}
}
int CharacterScriptSession::start(){return advance();}
int CharacterScriptSession::load_file(std::uintptr_t script,const char* name,bool& loaded){
 auto& t=*impl_;t.message.clear();return t.owner->load_file(script,name,t.services,loaded);
}
int CharacterScriptSession::init_vcb(std::uintptr_t script){return impl_->owner->init_vcb(script,impl_->services);}
int CharacterScriptSession::advance(){auto& t=*impl_;t.message.clear();auto& state=t.owner->lifecycle();if(state.load_step==0&&(state.pending||state.active)){t.message="Script replacement unsupported";return -2;}auto result=t.owner->advance({static_cast<std::uint32_t>(t.script.size()),0,t.script.c_str(),t.name.c_str()},t.services);if(result&&!t.owner->error().empty())t.message=t.owner->error();return result;}
bool CharacterScriptSession::view(ScriptSessionView& out)const noexcept{return impl_->owner->active(out)||impl_->owner->pending(out);}
ScriptOwner& CharacterScriptSession::owner()noexcept{return *impl_->owner;}
const ScriptOwner& CharacterScriptSession::owner()const noexcept{return *impl_->owner;}
TimerStore32& CharacterScriptSession::timers()noexcept{return impl_->timers;}
const TimerStore32& CharacterScriptSession::timers()const noexcept{return impl_->timers;}
const TimerServices32& CharacterScriptSession::native_timer_services() const noexcept{return impl_->timer_services;}
int CharacterScriptSession::update_timers(std::uint32_t dt,std::uint32_t blocked){auto& t=*impl_;if(!t.expiry||!t.expiry->expired||t.expiry->reserved){t.message="Genuine timer expiry provider unavailable";return -1;}t.timer_services.expired=&Impl::expire;return dh2_character_timers_update(&t.timers,dt,blocked,&t.timer_services);}
int CharacterScriptSession::dispatch_target(std::uint32_t event,std::uintptr_t enemy,const dh2_script_callback_scope* scope){
 auto& t=*impl_;t.message.clear();ScriptSessionView view{};
 if(!t.owner->active(view)||view.kind!=script_external||(scope&&(scope->vm!=view.vm||!dh2_script_callback_scope_valid(scope)))){t.message="Malformed external target delivery";return -1;}
 Impl::TargetDispatch delivery{&t,view,scope};const dh2::object_identity::TargetScript16 script{view.identity,view.callback_flags,0};const dh2::object_identity::TargetServices16 services{&delivery,&Impl::target_dispatch};
 auto result=dh2::object_identity::dh2_object_target_event(&script,event,enemy,&services);
 if(result&&!t.message.size())t.message="Native external target delivery failed";
 return result==1?-1:result?-2:0;
}
std::shared_ptr<data::PropertyState> CharacterScriptSession::properties()const noexcept{return impl_->properties;}
std::shared_ptr<data::CombatActorState> CharacterScriptSession::combat_state()const noexcept{return impl_->combat;}
data::PropertyView& CharacterScriptSession::property_view()noexcept{return impl_->property_view;}
const float* CharacterScriptSession::position()const noexcept{return impl_->position.data();}
void CharacterScriptSession::set_position(const std::array<float,3>& position)noexcept{impl_->position=position;}
const std::string& CharacterScriptSession::script_name()const noexcept{return impl_->script;}
const std::vector<ScriptSessionRegistration>& CharacterScriptSession::registrations()const noexcept{return impl_->delivered;}
const std::vector<std::string>& CharacterScriptSession::missing_bindings()const noexcept{return impl_->missing;}
const std::string& CharacterScriptSession::error()const noexcept{return impl_->message.empty()?impl_->owner->error():impl_->message;}
const dh2_script_callback_scope* CharacterScriptSession::current_skill_callback_scope()const noexcept{
 const auto* scope=impl_->skill_callback_scope;return scope&&dh2_script_callback_scope_valid(scope)?scope:nullptr;
}
}
