#include "character_menu_as_bridge_v1.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_environment.h"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include <cmath>
#include <cstring>
#include <map>
#include <stdexcept>
namespace dh2::ui {
namespace {
using Value=CharacterMenuValueV1;
struct Frame {
 const gameswf::fn_call& source;
 gameswf::gc_ptr<gameswf::player> player;
 std::map<std::uintptr_t,gameswf::gc_ptr<gameswf::as_object>> objects;
 CharacterMenuCallV1 call;
 explicit Frame(const gameswf::fn_call& fn):source(fn),player(fn.env->get_player()){}
 Value project(const gameswf::as_value& value){
  Value out;
  if(value.is_undefined())return out;
  if(value.is_bool()){out.kind=1;out.boolean=value.to_bool();return out;}
  if(value.is_string()){out.kind=4;out.text=value.to_string();return out;}
  if(value.is_object()){
   auto* object=value.to_object();out.kind=5;
   out.object=reinterpret_cast<std::uintptr_t>(object);
   if(object){
    if(object->get_player()!=player.get_ptr())throw std::invalid_argument("Character menu AS object belongs to another player");
    objects.emplace(out.object,object);
   }
   return out;
  }
  // Do not copy a bound property as_value: its copy constructor executes the
  // getter. Conversion below reads the original argument only when reached.
  if(value.is_property()){out.kind=6;return out;}
  out.kind=2;out.number=value.to_number();return out; // includes numeric NaN
 }
 gameswf::as_object* object(std::uintptr_t identity,std::string& error)const{
  const auto found=objects.find(identity);
  if(!identity||found==objects.end()){error="Character menu AS receiver was not borrowed by this callback";return nullptr;}
  return found->second.get_ptr();
 }
 bool convert(const Value& value,gameswf::as_value& out,std::string& error)const{
  switch(value.kind){
   case 0:out.set_undefined();return true;
   case 1:out.set_bool(value.boolean);return true;
   case 2:out.set_double(value.number);return true;
   case 4:out.set_string(value.text.c_str());return true;
   case 5:{
    if(!value.object){out.set_null();return true;}
    auto* receiver=object(value.object,error);if(!receiver)return false;
    out.set_as_object(receiver);return true;
   }
   default:error="Character menu cannot construct an unrecovered AS value tag";return false;
  }
 }
 const gameswf::as_value* argument(const Value& value)const{
  for(std::size_t i=0;i<call.arguments.size();++i)
   if(&value==&call.arguments[i])return &source.arg(static_cast<int>(i));
  return nullptr;
 }
 void bind(){
  call.arguments.reserve(static_cast<std::size_t>(source.nargs));
  for(int i=0;i<source.nargs;++i)call.arguments.push_back(project(source.arg(i)));
  call.result=project(*source.result);
  call.create_object=[this](std::uintptr_t& identity,std::string&){
   gameswf::gc_ptr<gameswf::as_object> created=new gameswf::as_object(player.get_ptr());
   identity=reinterpret_cast<std::uintptr_t>(created.get_ptr());
   objects.emplace(identity,created);return true;
  };
  call.member=[this](std::uintptr_t identity,const char* name,const Value& value,std::string& error){
   auto* receiver=object(identity,error);if(!receiver||!name)return false;
   gameswf::as_value native;if(!convert(value,native,error))return false;
   // Source callers ignore SetMember's boolean. Read-only setters and actual
   // watchers execute their normal semantics; rejection is not delivery loss.
   receiver->set_member(name,native);return true;
  };
  call.array=[this](std::uintptr_t identity,std::string& error){
   auto* receiver=object(identity,error);if(!receiver)return false;
   if(!receiver->is(gameswf::as_array::m_class_id)){error="Character menu requires the actual AS array receiver";return false;}
   return true;
  };
  call.append=[this](std::uintptr_t identity,const Value& value,std::string& error){
   if(!call.array(identity,error))return false;
   gameswf::as_value native;if(!convert(value,native,error))return false;
   static_cast<gameswf::as_array*>(object(identity,error))->push(native);return true;
  };
  call.number=[this](const Value& value,double& out,std::string& error){
   if(const auto* original=argument(value)){out=original->to_number();return true;}
   gameswf::as_value native;if(!convert(value,native,error))return false;
   out=native.to_number();return true;
  };
   call.boolean=[this](const Value& value,bool& out,std::string& error){
   if(const auto* original=argument(value)){out=original->to_bool();return true;}
   gameswf::as_value native;if(!convert(value,native,error))return false;
    out=native.to_bool();return true;
   };
   call.text=[this](const Value& value,std::string& out,std::string& error){
    if(const auto* original=argument(value)){out=original->to_string();return true;}
    gameswf::as_value native;if(!convert(value,native,error))return false;
    out=native.to_string();return true;
   };
   call.debug_text=[this](const Value& value,std::string& out,std::string& error){
    if(const auto* original=argument(value)){out=original->to_xstring();return true;}
    gameswf::as_value native;if(!convert(value,native,error))return false;
    out=native.to_xstring();return true;
   };
 }
};
bool identical(const Value& a,const Value& b){
 return a.kind==b.kind&&a.boolean==b.boolean&&a.text==b.text&&a.object==b.object&&
  std::memcmp(&a.number,&b.number,sizeof(double))==0;
}
}
CharacterMenuAsBridgeV1::CharacterMenuAsBridgeV1(std::shared_ptr<void> owner,Dispatch dispatch):
 game_owner_(std::move(owner)),dispatch_(std::move(dispatch)){
 if(!game_owner_||!dispatch_)throw std::invalid_argument("Character menu requires the retained game dispatcher");
}
bool CharacterMenuAsBridgeV1::dispatch(const char* name,const gameswf::fn_call& fn,std::string& error)const{
 error.clear();
 if(!name||!*name||!fn.env||!fn.env->get_player()||!fn.result||fn.nargs<0){error="Malformed character menu AS callback";return false;}
 try{
  // Separate strong game/player/object pins protect synchronous setter or
  // query reentry. Every borrow ends with this call, preventing graph cycles.
  const auto game=game_owner_;const auto handler=dispatch_;
  Frame frame(fn);frame.bind();const auto previous=frame.call.result;
  const bool delivered=handler(name,frame.call,error);
  if(!identical(previous,frame.call.result)){
   gameswf::as_value result;
   if(!frame.convert(frame.call.result,result,error))return false;
   *fn.result=result;
  }
  if(!delivered&&error.empty())error="Required character menu native callback failed";
  return delivered;
 }catch(const std::exception& failure){error=failure.what();return false;}
}
}
