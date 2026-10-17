#include "swf_menu_launch_v1.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include <cmath>
#include <vector>
#include <exception>
namespace dh2::ui {
namespace {
bool integer(const SwfMenuLaunchServicesV1& s,double value,std::int32_t& out,std::string& e){
 if(!s.eabi_integer){e="Source EABI integer provider unavailable";return false;}
 return s.eabi_integer(s.context,value,out,e);
}
}
bool swf_menu_create_save_slot_v1(const gameswf::fn_call& fn,const SwfMenuLaunchServicesV1& s,std::string& e){
 e.clear();if(fn.nargs!=2)return true;
 if(!fn.env||!fn.result){e="Malformed create-save-slot AS call";return false;}
 try {
  const std::string name=fn.arg(0).to_tu_string().c_str();
  const std::string character=fn.arg(1).to_tu_string().c_str();
  if(!s.create){e="Source CreateSaveSlot provider unavailable";return false;}
  std::int32_t slot=-1;bool publish=false;
  if(!s.create(s.context,name,character,slot,publish,e))return false;
  if(publish)fn.result->set_double(double(slot));
  return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
bool swf_menu_assign_save_slot_v1(const gameswf::fn_call& fn,const SwfMenuLaunchServicesV1& s,std::string& e){
 e.clear();if(fn.nargs<2||!fn.env){e="Malformed assign-save-slot AS call";return false;}
 try {
  std::int32_t slot=0,ordinal=0;
  if(!integer(s,fn.arg(0).to_number(),slot,e))return false;
  const double second=fn.arg(1).to_number();
  if(slot<0)return true;
  if(!integer(s,second,ordinal,e))return false;
  if(ordinal<0)return true;
  if(!s.assign){e="Source AssignSaveSlotToPlayer provider unavailable";return false;}
  return s.assign(s.context,slot,ordinal,e);
 }catch(const std::exception& ex){e=ex.what();return false;}
}
bool swf_menu_parsed_string_v1(const gameswf::fn_call& fn,Localization& strings,
 const LocalizationServices& localization,const HudTextServicesV1& text,
 const SwfMenuLaunchServicesV1& services,std::string& e){
 e.clear();if(fn.nargs!=2)return true;
 if(!fn.env||!fn.result){e="Malformed parsed-string AS call";return false;}
 if(!fn.arg(0).is_string()||!fn.arg(1).is_object())return true;
 try {
  const std::string symbol=fn.arg(0).to_xstring();
  gameswf::gc_ptr<gameswf::as_array> array=gameswf::cast_to<gameswf::as_array>(fn.arg(1).to_object());
  if(!array){e="Parsed-string object is not an AS array";return false;}
  LocalizationResult raw;if(!strings.raw_symbol(symbol,localization,raw,e))return false;
  const int capacity=array->size();
  if(capacity<0||capacity>65536){e="Parsed-string array outside bounds";return false;}
  std::vector<gameswf::as_value> retained;retained.reserve(std::size_t(capacity));
  std::vector<HudTextVariantV1> values;values.reserve(std::size_t(capacity));
  for(int i=0;i<array->size();++i){
   if(i>=capacity){e="Parsed-string array grew beyond source allocation";return false;}
   gameswf::as_value value;array->get_member(std::to_string(i).c_str(),&value);
   retained.push_back(value);auto& pin=retained.back();HudTextVariantV1 variant;
   if(pin.is_string())variant.text=pin.to_xstring();
   else if(pin.is_number()){
    if(!integer(services,pin.to_number(),variant.integer,e))return false;
    variant.number=static_cast<float>(pin.to_number());variant.text=pin.to_string();
   }
   values.push_back(variant);
  }
  std::string parsed;bool changed=false;
  if(!hud_text_parse_ex_v1(raw.found?raw.text.c_str():nullptr,values.data(),values.size(),text,parsed,changed,e))return false;
  fn.result->set_string(parsed.c_str());return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
bool swf_menu_preview_save_slot_v1(const gameswf::fn_call& fn,const SwfMenuLaunchServicesV1& s,std::string& e){
 e.clear();if(fn.nargs<1||!fn.env){e="Malformed preview-save-slot AS call";return false;}
 try {
  std::int32_t slot=0;if(!integer(s,fn.arg(0).to_number(),slot,e))return false;
  const bool force=fn.nargs>1?fn.arg(1).to_bool():false;
  if(!s.change_preview_slot){e="Source main-menu preview provider unavailable";return false;}
  return s.change_preview_slot(s.context,slot,force,e);
 }catch(const std::exception& ex){e=ex.what();return false;}
}
bool swf_menu_start_game_continuation_v1(const gameswf::fn_call& fn,const SwfMenuLaunchServicesV1& s,std::string& e){
 e.clear();if(!fn.env){e="Malformed NativeStartGame AS call";return false;}
 try {
  const bool numeric=fn.nargs==1&&fn.arg(0).is_number();
  std::int32_t requested=0;
  if(numeric){
   const double value=fn.arg(0).to_number();
   if(!std::isfinite(value)){e="Nonfinite NativeStartGame difficulty";return false;}
   if(!integer(s,value,requested,e))return false;
  }
  if(!s.request_start_game){e="NativeStartGame continuation unavailable";return false;}
  return s.request_start_game(s.context,numeric,requested,e);
 }catch(const std::exception& ex){e=ex.what();return false;}
}
}
