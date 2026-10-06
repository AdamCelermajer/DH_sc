#include "authored_menu_deadzones_v3.hpp"
#include "authored_menu_application_fields_v3.hpp"
#include "authored_menu_touchscreen_v3.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::ui;
int main(){
 AuthoredMenuTouchScreenV3 touchscreen(854,480);std::string touch_error;
 assert(touchscreen.empty()&&touchscreen.orientation()==0&&touchscreen.scale()==1.f);
 assert(touchscreen.reset({},touch_error)&&touchscreen.process({},touch_error));
 touchscreen.active_source_bytes()[1]=1;touchscreen.active_source_bytes()[7]=1;
 std::vector<int> released;
 assert(touchscreen.reset([&](auto x,auto y,auto id,std::string&){
  assert(x==-1&&y==-1);released.push_back(id);touchscreen.active_source_bytes()[7]=0;return true;
 },touch_error));
 assert((released==std::vector<int>{1,7})); // source copied list survives reentry
 touchscreen.source_tail()=1;
 assert(!touchscreen.process({},touch_error)&&!touchscreen.empty());
 assert(touchscreen.process([](auto& same,std::string&){same.source_head()=same.source_tail();return true;},touch_error));
 assert(touchscreen.empty()); // declared queued-process fixture, not source kernel proof
 AuthoredMenuApplicationFieldsV3 application;MenuStackGlobalsV1* globals=nullptr;
 std::string required;
 assert(!application.globals({},globals,required)&&!globals);
 assert(application.globals([](bool& actual,std::string&){actual=true;return true;},globals,required));
 assert(globals->last_open_menu==0&&globals->in_game_menu==0&&globals->use_native_drm==1);
 assert(!application.application_ec().has_value());
 assert(application.register_listener(101,required));
 assert(!application.unregister_listener(101,{},required)&&application.listeners().at(101));
 assert(application.unregister_listener(101,[](std::string&){return true;},required));
 assert(!application.listeners().at(101));
 assert(application.unregister_listener(202,[](std::string&){return true;},required));
 assert(application.listeners().size()==1);
 AuthoredMenuCharacterBorrowV3 root{1,"",true,true,false,{}};
 AuthoredMenuCharacterBorrowV3 hidden{2,"hidden",false,true,true,{}};
 AuthoredMenuCharacterBorrowV3 child{3,"prefix_deadzone_button",true,false,false,{}};
 AuthoredMenuCharacterBorrowV3 unnamed{4,"",true,false,false,{}};
 hidden.children={&child};root.children={&hidden,&unnamed};
 std::vector<AuthoredMenuCharacterBorrowV3*> found;std::string error;
 assert(authored_menu_collect_characters_v3(&root,"deadzone_",0,found,error));
 assert(found.size()==1&&found[0]==&child); // substring, not prefix
 found.clear();assert(authored_menu_collect_characters_v3(&root,"deadzone_",1,found,error));assert(found.empty());
 assert(authored_menu_collect_characters_v3(&root,nullptr,2,found,error));assert(found.empty()); // root focus blocks subtree
 assert(authored_menu_collect_characters_v3(&root,nullptr,4,found,error));assert(found.size()==2);
 std::uint8_t registered=0;int debug=0,bounds=0;
 AuthoredMenuDeadZoneServicesV3 services;
 services.debug=[&](std::string&){++debug;return true;};
 services.bounds=[&](auto&,auto& rectangle,std::string&){++bounds;rectangle=authored_menu_absolute_rectangle_v3({-20,20,-40,40},100,-100);return true;};
 std::vector<AuthoredMenuDeadZoneV3> zones;
 assert(authored_menu_register_deadzones_v3(registered,&root,services,zones,error));
 assert(registered==1&&debug==1&&bounds==1&&zones.size()==1);
 assert(zones[0].xmin==4&&zones[0].xmax==6&&zones[0].ymin==-7&&zones[0].ymax==-3);
 assert(authored_menu_register_deadzones_v3(registered,&root,{},zones,error));
 assert(debug==1&&bounds==1);
 registered=0;assert(!authored_menu_register_deadzones_v3(registered,&root,{},zones,error));
 assert(registered==1); // required failure retains source prefix and no retry
 std::cout<<"authored menu deadzones: source DFS/gates/rectangles/failure prefix PASS\n";
}
