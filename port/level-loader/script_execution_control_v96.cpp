#include "script_execution_control_v96.hpp"
namespace dh2::loader {
namespace {
bool missing(const char* what,std::string& e){e=std::string("Required actual script execution ")+what;return false;}
bool trace(const ScriptExecutionControlServicesV96& s,std::string& e){
 bool ignored{};return s.debug_load&&s.debug_switch?s.debug_load(e)&&s.debug_switch("isTracingScriptCmd",ignored,e):missing("Debug source leaves",e);
}
bool scalar(const CheckedCommandBorrowV59& c,unsigned offset,unsigned width,std::uint32_t& value,std::string& e){
 auto* actual=c.actual_data->scalar(offset);if(!actual||actual->width!=width)return missing("SAME source Data scalar",e);
 value=actual->bits;return true;
}
std::int32_t signed_bits(std::uint32_t bits){std::int32_t out;std::memcpy(&out,&bits,4);return out;}
}
ScriptCommandBehaviorV59 script_execution_control_v96(ScriptCommandBehaviorV59 existing,ScriptExecutionControlServicesV96 source){
 auto prior_execute=existing.execute;auto prior_blocking=existing.blocking;
 existing.actual_owner=source.application;
 existing.execute=[source,prior_execute](const auto& command,bool skip,std::int32_t module,auto& e){
  if(!source.application.lock())return missing("live Application",e);
  const auto kind=*command.kind8;
  if(kind==26){
   //Wait45c438 captures original Data before Debug, then stores elapsed0 and
   //SAME Data+8 duration. Skip flag is intentionally ignored by original.
   auto data=command.actual_data;
   if(!trace(source,e))return false;
   const auto* duration=data->scalar(8);
   if(!duration||duration->width!=4)return missing("Wait duration Data+8",e);
   return command.actual_receiver->write_operand_word(16,4,0,e)&&
    command.actual_receiver->write_operand_word(20,4,duration->bits,e);
  }
  if(kind==9){return trace(source,e);} //EmptyImpl45c560 is Debug, not just BXLR.
  if(kind==68){e.clear();return true;} //ShowTrophies455904 is literal BXLR.
  if(kind==0){
   //ExecScript4607f4 ignores skip; selects/records child before Start(true).
   std::uint32_t count{};if(!scalar(command,16,4,count,e))return false;
   std::uint32_t child{};
   if(!count){if(!scalar(command,12,4,child,e))return false;}
   else{
    auto* choices=command.actual_data->array(20);
    if(!choices||choices->size()!=count)return missing("ExecScript actual int array",e);
    data::LootRandom8V2* random{};std::shared_ptr<void> lease;
    if(!source.random||!source.random(random,lease,e)||!random||!lease)return missing("same process Random stream",e);
    std::int32_t index{};
    if(count>std::uint32_t(INT32_MAX)||dh2_loot_v2_random(random,std::int32_t(count),&index)!=0||
       index<0||std::uint32_t(index)>=count)return missing("ExecScript source random selection",e);
    child=(*choices)[std::size_t(index)];
   }
   if(!trace(source,e))return false;
   std::uint32_t common{};if(!scalar(command,8,1,common,e))return false;
   auto manager=source.manager.lock();if(!manager)return missing("same ScriptManager",e);
   if(!common)child+=std::uint32_t(manager->fields().common8);
   if(!command.actual_receiver->write_operand_word(16,4,child,e))return false;
   return manager->start_script_v96(signed_bits(child),module,true,e);
  }
  if(prior_execute)return prior_execute(command,skip,module,e);
  return missing("connected original command Execute body",e);
 };
 existing.blocking=[source,prior_blocking](const auto& command,bool& blocking,auto& e){
  if(*command.kind8==0){
   std::uint32_t wait{};if(!scalar(command,24,1,wait,e))return false;
   if(!wait){blocking=false;return true;}
   std::uint32_t child{};if(!command.actual_receiver->read_operand_word(16,4,child,e))return false;
   auto manager=source.manager.lock();return manager?manager->is_script_running_v96(signed_bits(child),blocking,e):missing("same child script manager",e);
  }
  if(prior_blocking)return prior_blocking(command,blocking,e);
  return missing("connected original dynamic IsBlocking body",e);
 };
 existing.update=[source](const auto& command,auto& e){
  if(*command.kind8!=26)return missing("Wait-only native Update receiver",e);
  std::uint32_t elapsed{},dt{};
  //4597cc captures elapsed before actual Application.GetDt delivery.
  if(!command.actual_receiver->read_operand_word(16,4,elapsed,e)||
     !source.application_dt||!source.application_dt(dt,e))return false;
  return command.actual_receiver->write_operand_word(16,4,elapsed+dt,e);
 };
 return existing;
}
bool bind_parsed_script_execution_v96(ScriptManagerOwnerV52& manager,ScriptCommandBehaviorV59 behavior,std::string& e){
 for(const auto& script:manager.commands()){
  if(!script.storage8)continue;
  for(const auto& slot:*script.storage8){
   if(slot.storage_released)continue;
   if(!slot.command.actual_owner||!slot.command.canonical_receiver_v96||!slot.command.retained_data||!*slot.command.retained_data||
      slot.command.identity!=reinterpret_cast<std::uintptr_t>(slot.command.actual_owner.get()))return missing("parsed canonical command lease",e);
   auto actual=slot.command.canonical_receiver_v96;
   if(actual.get()!=slot.command.actual_owner.get())return missing("SAME typed canonical command receiver",e);
   CheckedCommandBorrowV59 checked;if(!actual->checked_data_borrow(checked,e)||
     checked.actual_data!=*slot.command.retained_data)return false;
   if(!actual->bind_execution_v96(behavior,e))return false;
  }
 }
 e.clear();return true;
}
}
