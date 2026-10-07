#include "script_manager_owner_v52.hpp"
#include <exception>
namespace dh2::loader {
namespace {
std::int32_t wrap(std::int32_t value,std::int32_t delta){
 const auto bits=std::uint32_t(value)+std::uint32_t(delta);std::int32_t out;std::memcpy(&out,&bits,4);return out;
}
bool trace(const ScriptSchedulerServicesV96& s,const char* name,bool& value,std::string& e){
 if(!s.owner||!s.debug_load||!s.debug_switch){e="Required actual ScriptManager Debug owner";return false;}
 return s.debug_load(e)&&s.debug_switch(name,value,e);
}
}
bool ScriptManagerOwnerV52::bind_scheduler_v96(ScriptSchedulerServicesV96 services,std::string& e){
 if(busy_||execution_depth_v96_||!services.owner)return fail("Cannot bind ScriptManager scheduler during delivery or without owner",e);
 scheduler_=std::move(services);e.clear();return true;
}
bool ScriptManagerOwnerV52::is_script_running_v96(std::int32_t id,bool& out,std::string& e)const{
 if(id<0||std::size_t(id)>=contexts_.size()){e="IsScriptRunning outside actual source context array";return false;}
 out=contexts_[id].state8!=2;e.clear();return true; //455bec; no shadow running set.
}
bool ScriptManagerOwnerV52::stop_script_v96(std::int32_t id,bool,std::string& e){
 if(id<0||std::size_t(id)>=contexts_.size())return fail("StopScript outside actual source context array",e);
 //455bb8 ignores the boolean; does not call command Finish, alter field4,
 //decrement manager4 or stop a child script. Preserve exactly those stores.
 contexts_[id].state8=2;contexts_[id].command0=-1;e.clear();return true;
}
bool ScriptManagerOwnerV52::skip_script_v96(std::int32_t id,bool received,std::string& e){
 if(diagnostics_.failed){e=diagnostics_.error;return false;}
 if(!scheduler_.owner||!scheduler_.flush_dialogs||!scheduler_.flush_dialogs(e))return fail(e.empty()?"Required source dialog-message flush before SkipScript":e,e);
 bool online{};if(!scheduler_.online||!scheduler_.online(online,e))return fail(e.empty()?"Required actual GetOnline for SkipScript":e,e);
 if(online&&!received){
  if(!scheduler_.send_script_message||!scheduler_.send_script_message(true,id,-1,e))return fail(e.empty()?"Required actual network skip-script message":e,e);
 }
 bool ignored{};if(!trace(scheduler_,"isTracingScriptCmd",ignored,e))return fail(e,e);
 if(fields_.current0==-1)fields_.current0=id; //460544..460550, including original id semantics.
 e.clear();return true;
}
bool ScriptManagerOwnerV52::start_script_v96(std::int32_t id,std::int32_t module,bool received,std::string& e){
 if(diagnostics_.failed){e=diagnostics_.error;return false;}
 //4605f0..460620 genuine guard; invalid IDs perform no upstream calls.
 if(id<0||std::size_t(id)>=contexts_.size()){e.clear();return true;}
 bool dead{};if(!scheduler_.owner||!scheduler_.all_players_dead||!scheduler_.all_players_dead(dead,e))return fail(e.empty()?"Required actual PM AllPlayersDead":e,e);
 bool online{};if(dead){
  if(!scheduler_.online||!scheduler_.online(online,e))return fail(e.empty()?"Required actual GetOnline at dead-player guard":e,e);
  if(!online){e.clear();return true;}
  std::int32_t count{};if(!scheduler_.player_count714||!scheduler_.player_count714(count,e))return fail(e.empty()?"Required actual PM player count714":e,e);
  if(count<=1){e.clear();return true;}
 }
 bool skip_all{};if(!trace(scheduler_,"SkipAllScripts",skip_all,e))return fail(e,e);
 if(skip_all&&fields_.current0==-1&&!skip_script_v96(id,false,e))return false;
 if(!scheduler_.online||!scheduler_.online(online,e))return fail(e.empty()?"Required actual GetOnline before StartScript":e,e);
 if(online){
  if(received){bool running{};if(!is_script_running_v96(id,running,e))return fail(e,e);if(running){e.clear();return true;}}
  else if(!commands_[id].skip4){
   if(!scheduler_.send_script_message||!scheduler_.send_script_message(false,id,module,e))return fail(e.empty()?"Required actual network start-script message":e,e);
  }
 }
 bool ignored{};if(!trace(scheduler_,"isTracingScriptCmd",ignored,e))return fail(e,e);
 //460760: original context0 is MODULE, field4 is command INDEX.
 // Historical member names remain to avoid changing the parsed owner layout.
 contexts_[id].field4=0;contexts_[id].state8=0;contexts_[id].command0=module;
 fields_.field4=wrap(fields_.field4,1);e.clear();return true;
}
bool ScriptManagerOwnerV52::execute_script_v96(std::int32_t id,bool& running,std::string& e){
 running=false;if(diagnostics_.failed){e=diagnostics_.error;return false;}
 if(id<0||std::size_t(id)>=contexts_.size()||std::size_t(id)>=commands_.size())return fail("ExecuteScript outside SAME source context/command arrays",e);
 //45c19c..45c1b0 happens even when script is stopped.
 const auto* name=name_from_id(id);
 if(!name||!scheduler_.publish_current_name||!scheduler_.publish_current_name(name,e))return fail(e.empty()?"Required actual current-script debug-name store":e,e);
 ++execution_depth_v96_;struct Depth{std::uint32_t& n;~Depth(){--n;}}depth{execution_depth_v96_};
 try{for(;;){
  auto& context=contexts_[id];auto& script=commands_[id];
  if(context.state8==2){e.clear();return true;}
  if(context.state8!=0&&context.state8!=1)return fail("ExecuteScript unsupported source context state",e);
  const auto index=context.field4;
  if(!script.storage8||index>=script.storage8->size()||index>=std::uint32_t(script.count0))return fail("ExecuteScript original unchecked command index outside native domain",e);
  // Copy the native borrow only, not data/context storage, through callbacks.
  auto command=(*script.storage8)[index].command;
  if(!command.actual_owner||!command.kind8)return fail("ExecuteScript requires SAME actual command receiver",e);
  if(context.state8==0){
   if(!command.execute||!command.execute(fields_.current0!=-1,context.command0,e))return fail(e.empty()?"Required source command Execute virtual8":e,e);
   contexts_[id].state8=1; //original unconditional postcallback store.
  }
  //45c20c rereads command index/kind; nested ExecScript can mutate contexts.
  auto current_index=contexts_[id].field4;
  if(current_index>=script.storage8->size())return fail("Source callback changed command index outside retained array",e);
  command=(*script.storage8)[current_index].command;
  std::int32_t exec_kind{};
  if(!scheduler_.constant||!scheduler_.constant("ScriptCmdID","ExecScript",exec_kind,e))return fail(e.empty()?"Required genuine ScriptCmdID.ExecScript":e,e);
  bool blocking=false;
  if(fields_.current0==-1||*command.kind8==exec_kind){
   if(!command.blocking||!command.blocking(blocking,e))return fail(e.empty()?"Required source IsBlocking virtualc":e,e);
  }
  if(blocking){
   //Original calls Update and returns1 even if Update unblocks/stops script;
   //completion is observed by the next scheduler invocation, never twice.
   current_index=contexts_[id].field4;
   if(current_index>=script.storage8->size())return fail("Blocking callback changed source index outside retained array",e);
   command=(*script.storage8)[current_index].command;
   if(!command.update||!command.update(e))return fail(e.empty()?"Required source Update virtual4":e,e);
   running=true;return true;
  }
  contexts_[id].state8=0;contexts_[id].field4+=1;
  if(contexts_[id].field4>=std::uint32_t(script.count0)){
   contexts_[id].state8=2;fields_.field4=wrap(fields_.field4,-1);
   bool ignored{};if(!trace(scheduler_,"isTracingScriptCmd",ignored,e))return fail(e,e);
   //Source reloads state after Debug delivery, allowing a real restart.
  }
 }}catch(const std::exception& ex){return fail(ex.what(),e);}catch(...){return fail("ScriptManager execution service threw",e);}
}
bool ScriptManagerOwnerV52::execute_all_scripts_v96(bool& any,std::string& e){
 any=false;if(busy_||execution_depth_v96_)return fail("ExecuteAllScripts reentered during source delivery",e);
 if(diagnostics_.failed){e=diagnostics_.error;return false;}
 struct Busy{bool& b;Busy(bool& v):b(v){b=true;}~Busy(){b=false;}}guard(busy_);
 const auto count=commands_.size(); //45c390 captured loop bound.
 for(std::size_t id=0;id<count;++id){bool running{};if(!execute_script_v96(std::int32_t(id),running,e))return false;any=any||running;}
 if(fields_.field4==0)stop_skipping_v96();
 if(!any){fields_.field4=0;stop_skipping_v96();}
 e.clear();return true;
}
}
