#include "character_menu_application_v4.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::ui;
int main(){
 CharacterMenuTutorialQueueV4 queue;AuthoredMenuApplicationFieldsV3 fields;
 CharacterMenuApplicationServicesV4 services;services.owner=std::make_shared<int>(1);services.tutorial=&queue;services.application=&fields;
 assert(queue.empty()&&!queue.skip_function()&&fields.manager_multitouch110_v4()==0);
 CharacterMenuCallV1 call;call.arguments.resize(1);call.arguments[0].kind=5;call.arguments[0].object=17;
 bool handled=false;std::string error;
 assert(character_menu_application_call_v4("NativeGetCharMenuTutorialMessage",call,services,handled,error)&&handled&&call.result.kind==1&&!call.result.boolean);
 assert(character_menu_application_call_v4("NativeSkipCharMenuTutorialMessage",call,services,handled,error)&&queue.empty());
 unsigned converted=0;bool value=false;call.boolean=[&](auto&,bool& out,std::string&){++converted;out=value;return true;};
 for(unsigned count=0;count<4;++count)for(bool b:{false,true}){value=b;call.arguments.resize(count);const auto previous=converted;
  assert(character_menu_application_call_v4("NativeSetMultitouch",call,services,handled,error));
  assert(fields.manager_multitouch110_v4()==(count==1?unsigned(b):1u));assert(converted==previous+unsigned(count==1));
 }
 call.arguments.resize(1);const auto prior=converted;assert(character_menu_application_call_v4("NativeShowStatusBar",call,services,handled,error)&&converted==prior+1);
 auto previous_result=call.result;assert(!character_menu_application_call_v4("NativeOther",call,services,handled,error)||!handled);assert(call.result.kind==previous_result.kind);
 assert(!queue.enqueue_script_command(7,nullptr,"menu_A",error)&&queue.empty());
 assert(queue.enqueue_script_command(7,"frame_A","menu_A",error));
 call.arguments[0].kind=5;call.arguments[0].object=17;
 assert(!character_menu_application_call_v4("NativeGetCharMenuTutorialMessage",call,services,handled,error)&&queue.size()==1);
 std::vector<std::string> writes;
 services.localized=[&](int id,std::string& text,std::string&){assert(id==7);text="authored localized text";return true;};
 call.member=[&](auto object,const char* name,const auto& val,std::string&){assert(object==17&&val.kind==4);writes.push_back(std::string(name)+"="+val.text);
  if(writes.size()==1){assert(queue.skip({},error));assert(queue.enqueue_script_command(9,"frame_B","menu_B",error));}return true;};
 assert(character_menu_application_call_v4("NativeGetCharMenuTutorialMessage",call,services,handled,error)&&call.result.boolean);
 assert((writes==std::vector<std::string>{"TutorialMessage=authored localized text","TutorialFrame=frame_A","TutorialMenu=menu_A"}));assert(queue.first()->string_id==9);
 queue.publish_source_skip_function("actual_fixture_function");assert(!queue.skip({},error)&&queue.empty()); // retained pop prefix
 assert(queue.enqueue_script_command(10,"actual_frame","actual_menu",error));
 unsigned invoked=0;assert(queue.skip([&](const char* name,std::string&){++invoked;assert(std::string(name)=="actual_fixture_function"&&queue.empty());return true;},error)&&invoked==1);
 std::cout<<"character menu application SAME queue/fields/conversions; positive declared localized/member services and reentry prefixes PASS\n";
}
