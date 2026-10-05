#include "menu_stack_actions_v1.hpp"
#include "character_menu_queries_owner_v1.hpp"
#include <cstring>
#include <deque>
#include <stdexcept>
#include <utility>
using namespace dh2::ui;
extern "C" int dh2_menu_stack_action_v1(std::uint32_t op,const MenuStackActionCallV1*call,
 const MenuStackActionServicesV1*services,const MenuStackServicesV1*lifecycle){
 if(op>3||!call||call->reserved||(call->count&&!call->arguments))return -1;
 if(op==0&&!call->count)return -1;
 if(op==2&&(call->count!=1||(call->arguments[0].source_type!=3&&call->arguments[0].source_type!=4)))return 0;
 const bool conversion=op==0||(op==1&&call->count)||op==2;
 if(conversion&&call->arguments[0].reserved)return -1;
 if(!services||!services->instance||!lifecycle||!lifecycle->invoke||(conversion&&!services->text))return -1;
 MenuStackV1*stack=nullptr;const char*name=nullptr;
 // Push/Above capture converted name before acquiring the manager. Pop
 // acquires it first, so conversion may synchronously mutate that SAME stack.
 if(op==1||op==3){if(services->instance(services->context,&stack))return -2;if(!stack)return -2;}
 if(conversion){if(services->text(services->context,&call->arguments[0],&name))return -2;if(!name)return -2;}
 if(op==0||op==2){if(services->instance(services->context,&stack))return -2;if(!stack)return -2;}
 return dh2_menu_stack_native_v1(stack,op,call->count,conversion?call->arguments[0].source_type:0,name,lifecycle);
}
namespace dh2::ui {
MenuStackActionsV1::MenuStackActionsV1(MenuStackActionsGraphV1 graph):graph_(std::move(graph)){
 if(!graph_.owner||!graph_.instance||!graph_.lifecycle.invoke)
  throw std::invalid_argument("Menu navigation requires a retained manager and genuine lifecycle services");
}
bool MenuStackActionsV1::dispatch(const char*name,const MenuStackActionCallV1&call,
 const std::function<bool(const MenuStackActionValueV1&,std::string&,std::string&)>& text,std::string&error)const{
 error.clear();std::uint32_t op=4;
 if(name){const char*names[]={"NativePushMenu","NativePopMenu","NativePopAllAbove","NativePopAllMenus"};
  for(std::uint32_t i=0;i<4;++i)if(!std::strcmp(name,names[i])){op=i;break;}}
 if(op==4){error="Unsupported in-game menu navigation action";return false;}
 // Per-invocation strong pin and copied services/handlers survive synchronous
 // nested dispatch. Text buffers remain stable through all nested callbacks.
 const auto owner=graph_.owner;const auto instance=graph_.instance;const auto lifecycle=graph_.lifecycle;
 struct Frame {
  const std::function<bool(MenuStackOwnerV1*&,std::string&)>*instance;
  const std::function<bool(const MenuStackActionValueV1&,std::string&,std::string&)>*text;
  std::string*error;std::deque<std::string> strings;
  static int get(void*p,MenuStackV1**out){auto&f=*static_cast<Frame*>(p);MenuStackOwnerV1*owner=nullptr;
   if(!(*f.instance)(owner,*f.error)||!owner||!owner->view()){if(f.error->empty())*f.error="Required source menu manager is unavailable";return 1;}
   *out=owner->view();return 0;
  }
  static int convert(void*p,const MenuStackActionValueV1*value,const char**out){auto&f=*static_cast<Frame*>(p);
   f.strings.emplace_back();if(!*f.text||!(*f.text)(*value,f.strings.back(),*f.error))return 1;
   if(f.strings.back().find('\0')!=std::string::npos){*f.error="Malformed source menu string projection";return 1;}
   *out=f.strings.back().c_str();return 0;
  }
 } frame{&instance,&text,&error,{}};
 MenuStackActionServicesV1 services{&frame,Frame::get,Frame::convert};
 try{
  const int rc=dh2_menu_stack_action_v1(op,&call,&services,&lifecycle);
  if(!rc)return true;
  if(error.empty())error=rc==-1?"Malformed source menu navigation projection":rc==-2?"Required source menu navigation service failed":rc==-3?"Unsupported source menu navigation continuation":"Menu navigation occurrence budget exhausted";
  return false;
 }catch(const std::exception&failure){error=failure.what();return false;}
}
bool MenuStackActionsV1::dispatch(const char*name,CharacterMenuCallV1&call,std::string&error)const{
 if(call.arguments.size()>UINT32_MAX){error="Menu action argument count exceeds source width";return false;}
 std::vector<MenuStackActionValueV1> args;args.reserve(call.arguments.size());
 for(std::size_t i=0;i<call.arguments.size();++i)args.push_back({i+1,call.arguments[i].kind,0});
 const MenuStackActionCallV1 projected{args.data(),static_cast<std::uint32_t>(args.size()),0};
 auto convert=[&](const MenuStackActionValueV1&value,std::string&out,std::string&failure){
  if(!value.identity||value.identity>call.arguments.size()||!call.debug_text){failure="Required source AS to_xstring conversion is unavailable";return false;}
  return call.debug_text(call.arguments[value.identity-1],out,failure);
 };
 return dispatch(name,projected,convert,error);
}
}
static_assert(sizeof(MenuStackActionValueV1)==16&&sizeof(MenuStackActionCallV1)==16&&sizeof(MenuStackActionServicesV1)==24,"native64 action ABI");
