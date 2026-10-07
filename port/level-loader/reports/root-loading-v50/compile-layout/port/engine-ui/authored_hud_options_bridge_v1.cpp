#include "authored_hud_options_bridge_v1.hpp"
#include <algorithm>
#include <cstring>
#include <deque>
namespace dh2::ui {
namespace {
struct Invocation {
 CharacterMenuCallV1& call;const OwnedHudSettingsV1& settings;const HudInitServices16& remaining;std::string& error;
 std::deque<std::string> text;
 static int dispatch(void* p,const HudInitRequest64* q,HudInitResponse32* out){
  auto& x=*static_cast<Invocation*>(p);using Op=HudInitOperation;
  switch(q->operation){
  case Op::argument_string:{
   if(q->index>=x.call.arguments.size()){x.error="Required actual HUD option argument";return 0;}
   const auto& v=x.call.arguments[q->index];x.text.emplace_back();
   if(v.kind==4)x.text.back()=v.text;
   else if(!x.call.text||!x.call.text(v,x.text.back(),x.error))return 0;
   out->text=x.text.back().c_str();return 1;
  }
  case Op::cast_object:
   if(q->index>=x.call.arguments.size()){x.error="Required actual HUD option object argument";return 0;}
   out->identity=x.call.arguments[q->index].kind==5?x.call.arguments[q->index].object:0;return 1;
  case Op::write_member:{
   if(!q->object||!q->name||!x.call.member){x.error="Required actual HUD option member writer";return 0;}
   CharacterMenuValueV1 v;
   if(q->type==1)v=CharacterMenuValueV1::flag(q->value!=0);
   else if(q->type==2)v=CharacterMenuValueV1::numeric(q->number);
   else if(q->type==3)v=CharacterMenuValueV1::string(q->text?q->text:"");else return 0;
   return x.call.member(q->object,q->name,v,x.error)?1:0;
  }
  case Op::result_boolean:x.call.result=CharacterMenuValueV1::flag(q->value!=0);return 1;
  case Op::result_object:x.call.result=CharacterMenuValueV1::reference(q->object);return 1;
  default:{
   const auto owned=hud_initialization_settings_v1_query(x.settings,*q,*out);
   if(owned>=0)return owned;
   if(!x.remaining.invoke){x.error="Required source HUD options language/localization/platform producer";return 0;}
   const auto result=x.remaining.invoke(x.remaining.context,q,out);
   if(result!=1&&x.error.empty())x.error="Required source HUD option service "+std::to_string(static_cast<unsigned>(q->operation));return result;
  }
  }
 }
};
}
bool authored_hud_options_bridge_v1(const char* name,CharacterMenuCallV1& call,
 const OwnedHudSettingsV1& settings,const HudInitServices16& remaining,std::string& error){
 unsigned entry{};
 if(name&&!std::strcmp(name,"NativeGetOptionParameters"))entry=3;
 else if(name&&!std::strcmp(name,"NativeUseIpodPlayer"))entry=4;
 else{error="Unsupported authored HUD option callback";return false;}
 Invocation invocation{call,settings,remaining,error,{}};
 const auto count=static_cast<unsigned>(call.arguments.size());
 const HudInitInput16 input{reinterpret_cast<std::uintptr_t>(&call),count,std::min(count,4u)};
 const HudInitServices16 services{&invocation,Invocation::dispatch};
 const auto result=dh2_ui_hud_initialization_v1(&input,entry,&services);
 if(result==0)return true;
 if(error.empty())error=result==-1?"Malformed authored HUD options source projection":"Required authored HUD options source provider";return false;
}
}
