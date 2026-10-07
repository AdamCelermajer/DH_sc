#include "source_campaign_script_tutorial_v118.hpp"
#include "model_renderer.hpp"
#include <script_command_receivers_v59.hpp>
#include <cstring>
namespace model_renderer {
bool execute_source_campaign_script_tutorial_v118(const SourceCampaignCandidateBorrowV55& candidate,
 const dh2::loader::CheckedCommandBorrowV59& command,bool skip,std::int32_t module,
 bool& handled,std::string& error){
 (void)skip;(void)module;handled=false;
 if(!command.kind8||!command.actual_data){error="Required actual tutorial command/Data";return false;}
 const auto kind=*command.kind8;if(kind<71||kind>76)return true;handled=true;
 auto word=[&](std::uint32_t offset,std::int32_t& value){
  const auto* field=command.actual_data->scalar(offset);
  if(!field||field->width!=4){error="Required SAME tutorial command Data word";return false;}
  std::memcpy(&value,&field->bits,sizeof(value));return true;
 };
 if(kind==71){
  std::int32_t text{},duration{};
  if(!word(12,text))return false;
  if(text<0){error.clear();return true;} //46013d4 does not enqueue negative IDs.
  if(!word(8,duration))return false;
  return enqueue_source_tutorial_v118(candidate.actual_world,text,duration,error);
 }
 if(kind==72||kind==73)return skip_source_tutorial_v118(candidate.actual_world,kind==73,error);
 std::int32_t text{-1};const char* frame{};const char* menu{};
 if(kind==74){
  if(!word(24,text))return false;
  frame=command.actual_data->cstring(12);menu=command.actual_data->cstring(20);
  if(!frame||!menu){error="Required actual CharMenuTutorial frame/menu strings";return false;}
 }
 return character_tutorial_operation_v118(candidate.actual_world,kind,text,frame,menu,error);
}
}
