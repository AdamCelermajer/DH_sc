#include "character_menu_application_v4.hpp"
#include <cstring>
namespace dh2::ui {
bool CharacterMenuTutorialQueueV4::enqueue_script_command(std::int32_t id,const char* frame,const char* menu,std::string& error){
 if(!frame||!menu){error="Required actual tutorial script command string producers";return false;}
 messages_.push_back({id,frame,menu});return true;
}
bool CharacterMenuTutorialQueueV4::skip(const std::function<bool(const char*,std::string&)>& invoke,std::string& error){
 if(empty())return true;
 messages_.pop_front();
 if(!skip_function_)return true;
 if(!invoke){error="Required actual tutorial menu invocation continuation";return false;}
 // Preserve the function string across synchronous callback publication.
 const auto function=*skip_function_;return invoke(function.c_str(),error);
}
bool character_menu_application_call_v4(const char* name,CharacterMenuCallV1& call,const CharacterMenuApplicationServicesV4& services,bool& handled,std::string& error){
 handled=false;if(!name)return true;
 const bool get=!std::strcmp(name,"NativeGetCharMenuTutorialMessage"),skip=!std::strcmp(name,"NativeSkipCharMenuTutorialMessage"),multi=!std::strcmp(name,"NativeSetMultitouch"),status=!std::strcmp(name,"NativeShowStatusBar");
 if(!get&&!skip&&!multi&&!status)return true;
 handled=true;if(!services.owner){error="Required same native character-menu application owner";return false;}
 if(status){
  if(call.arguments.empty()){error="NativeShowStatusBar source-invalid missing argument";return false;}
  //43aa34 consumes actual to_bool; Application.ShowStatubBar31f668 is bx lr.
  bool original=false;if(!call.boolean||!call.boolean(call.arguments[0],original,error)){if(error.empty())error="Required actual AS to_bool source conversion";return false;}(void)original;return true;
 }
 if(multi){
  if(!services.application){error="Required actual MenuManager110 field owner";return false;}
  bool enabled=true;if(call.arguments.size()==1&&(!call.boolean||!call.boolean(call.arguments[0],enabled,error))){if(error.empty())error="Required actual AS multitouch boolean conversion";return false;}
  services.application->manager_multitouch110_v4()=static_cast<std::uint8_t>(enabled);return true;
 }
 if(!services.tutorial){error="Required actual constructor-owned tutorial message queue";return false;}
 if(skip)return services.tutorial->skip(services.invoke_tutorial,error);
 if(call.arguments.empty()){error="NativeGetCharMenuTutorialMessage source-invalid missing argument";return false;}
 // Source temp record copies FIRST before its synchronous setters. Reentrant
 // scripts may mutate the queue but cannot replace this retained message.
 const auto* first=services.tutorial->first();
 if(!first){call.result={};call.result.kind=1;call.result.boolean=false;return true;}
 const auto message=*first;const auto receiver=call.arguments[0].kind==5?call.arguments[0].object:0;
 if(!receiver||!call.member){error="Required actual tutorial AS object receiver/member service";return false;}
 if(!services.localized){error="Required same HudText tutorial string-ID producer";return false;}
 std::string localized;if(!services.localized(message.string_id,localized,error))return false;
 // Original does not branch on set_member's return value.
 CharacterMenuValueV1 value;value.kind=4;value.text=localized;
 if(!call.member(receiver,"TutorialMessage",value,error))return false;
 value.text=message.frame;if(!call.member(receiver,"TutorialFrame",value,error))return false;
 value.text=message.menu;if(!call.member(receiver,"TutorialMenu",value,error))return false;
 call.result={};call.result.kind=1;call.result.boolean=true;return true;
}
}
