#include "swf_event_dispatch.hpp"
#include <cstring>
extern "C" int dh2_ui_swf_send_event(dh2::ui::SwfEvent48* event,std::uint32_t* selection,
 const dh2::ui::SwfEventServices24* services){
 if(!event||!selection||!services||!event->name)return -1;
 if(!services->native_event)return -2;
 if(!services->native_event(services->context,event))return -2;
 if(event->consumed)return 0;
 constexpr const char* methods[]={"on_focus_in","on_focus_out","on_clicked",nullptr,"onPress",nullptr,
  "onRelease","onReleaseOutside","onRollOver","onRollOut","onDragOver","onDragOut"};
 const auto kind=event->kind;
 if(kind>11||!methods[kind])return 0;
 if(!services->as_method||!services->as_method(services->context,event->character,methods[kind]))return -2;
 if(kind!=6)return 0;
 // Branch selection precedes the AS call; source reloads the event name after
 // that synchronous call. Global stores are direct and cannot invent reentry.
 const auto* name=event->name;
 if(!name)return -1;
 if(std::strcmp(name,"btnDelete")==0)*selection=0x13;
 name=event->name;
 if(std::strcmp(name,"btn_GAMEPLAYMENUS_ACCEPT")==0||std::strcmp(name,"btn_GAMEPLAYMENUS_REFUSE")==0)*selection=1;
 name=event->name;
 if(std::strcmp(name,"btn_Legend")==0)*selection=0x14;
 name=event->name;
 if(std::strcmp(name,"btn_deadzone")==0)*selection=0x0c;
 return 0;
}
