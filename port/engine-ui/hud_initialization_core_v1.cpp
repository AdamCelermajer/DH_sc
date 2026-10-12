#include "hud_initialization_core_v1.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include <array>
#include <cstring>
namespace dh2::ui {
namespace {
struct Invocation {
 const gameswf::fn_call& fn;const HudInitServices16& backend;std::string& error;
 std::array<std::vector<HudInitVariant12V1>,2> arguments;
 std::array<HudInitArguments16V1,2> views{};
 static int dispatch(void*p,const HudInitRequest64*q,HudInitResponse32*out){
  auto&x=*static_cast<Invocation*>(p);using Op=HudInitOperation;
  auto argument=[&]() -> const gameswf::as_value* {if(q->index>=static_cast<unsigned>(x.fn.nargs)){x.error="HUD native source argument projection unavailable";return nullptr;}return &x.fn.arg(static_cast<int>(q->index));};
  switch(q->operation){
   case Op::argument_type:{auto*v=argument();if(!v)return 0;out->value=v->is_function()?4:v->is_object()?5:!std::strcmp(v->type_of(),"number")?2:v->is_string()?3:v->is_bool()?1:v->is_property()?6:0;out->identity=reinterpret_cast<std::uintptr_t>(v->to_object());return 1;}
   case Op::argument_is_number:{auto*v=argument();if(!v)return 0;out->value=v->is_number();return 1;}
   case Op::argument_number:{auto*v=argument();if(!v)return 0;out->number=v->to_number();return 1;}
   case Op::argument_boolean:{auto*v=argument();if(!v)return 0;out->value=v->to_bool();return 1;}
   case Op::argument_string:{auto*v=argument();if(!v)return 0;out->text=v->to_string();return 1;}
   case Op::cast_object:case Op::cast_array:{auto*v=argument();if(!v)return 0;auto*object=v->is_object()?v->to_object():nullptr;if(q->operation==Op::cast_array)object=gameswf::cast_to<gameswf::as_array>(object);else object=gameswf::cast_to<gameswf::as_object>(object);out->identity=reinterpret_cast<std::uintptr_t>(object);return 1;}
   case Op::arguments_create:if(q->index>=2)return 0;x.arguments[q->index].clear();out->identity=reinterpret_cast<std::uintptr_t>(&x.arguments[q->index]);return 1;
   case Op::arguments_append:{auto i=q->subject==reinterpret_cast<std::uintptr_t>(&x.arguments[0])?0:q->subject==reinterpret_cast<std::uintptr_t>(&x.arguments[1])?1:-1;if(i<0||x.arguments[i].size()>=65536)return 0;x.arguments[i].push_back({static_cast<float>(q->number),q->value,0});return 1;}
   case Op::parse_text:{auto i=q->subject==reinterpret_cast<std::uintptr_t>(&x.arguments[0])?0:q->subject==reinterpret_cast<std::uintptr_t>(&x.arguments[1])?1:-1;if(i<0)return 0;x.views[i]={x.arguments[i].data(),static_cast<std::uint32_t>(x.arguments[i].size()),0};auto request=*q;request.subject=reinterpret_cast<std::uintptr_t>(&x.views[i]);if(!x.backend.invoke){x.error="Required HUD localization parseEx provider unavailable";return 0;}return x.backend.invoke(x.backend.context,&request,out);}
   case Op::write_member:{auto*object=reinterpret_cast<gameswf::as_object*>(q->object);if(!object||!q->name){x.error="Required HUD source AS output object unavailable";return 0;}gameswf::as_value v;if(q->type==1)v.set_bool(q->value!=0);else if(q->type==2)v.set_double(q->number);else if(q->type==3&&q->text)v.set_string(q->text);else{return 0;}object->set_member(q->name,v);return 1;} // source ignores setter acceptance
   case Op::array_push:{auto*object=reinterpret_cast<gameswf::as_object*>(q->object);auto*array=gameswf::cast_to<gameswf::as_array>(object);if(!array){x.error="Required HUD source array unavailable";return 0;}array->push(gameswf::as_value(q->number));return 1;}
   case Op::result_boolean:if(!x.fn.result)return 0;x.fn.result->set_bool(q->value!=0);return 1;
   case Op::result_number:if(!x.fn.result)return 0;x.fn.result->set_double(q->number);return 1;
   case Op::result_object:if(!x.fn.result)return 0;x.fn.result->set_as_object(reinterpret_cast<gameswf::as_object*>(q->object));return 1;
   default:if(!x.backend.invoke){x.error="Required HUD initialization game provider unavailable";return 0;}return x.backend.invoke(x.backend.context,q,out);
  }
 }
};
}
bool hud_initialization_native_v1(SwfAsGraph&graph,const char*name,const gameswf::fn_call&fn,const HudInitServices16&services,std::string&error){
 SwfAsValue scope;if(!graph.global_value(scope,error))return false;
 if(!name||!fn.env||fn.nargs<0){error="Malformed HUD native callback";return false;}
 unsigned entry;if(!std::strcmp(name,"NativeSkillGetEquipedSkillsIDs"))entry=0;else if(!std::strcmp(name,"NativeGetSkillDetails"))entry=1;else if(!std::strcmp(name,"NativeHUDGetActiveFaery"))entry=2;else if(!std::strcmp(name,"NativeGetOptionParameters"))entry=3;else if(!std::strcmp(name,"NativeUseIpodPlayer"))entry=4;else{error="Unsupported HUD initialization native callback";return false;}
 Invocation invocation{fn,services,error,{},{}};HudInitInput16 input{reinterpret_cast<std::uintptr_t>(&fn),static_cast<unsigned>(fn.nargs),static_cast<unsigned>(std::min(fn.nargs,4))};HudInitServices16 bound{&invocation,Invocation::dispatch};
 auto rc=dh2_ui_hud_initialization_v1(&input,entry,&bound);if(rc==0)return true;if(error.empty())error=rc==-1?"Malformed HUD initialization source projection":"Required HUD initialization service failed";return false;
}
}
