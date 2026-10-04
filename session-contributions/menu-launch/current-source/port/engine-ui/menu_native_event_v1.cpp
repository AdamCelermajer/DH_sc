#include "menu_native_event_v1.hpp"
#include <cstring>
namespace dh2::ui {
namespace {
bool missing(std::string& e,const char* operation){e=std::string("Required menu native service unavailable: ")+operation;return false;}
}
bool MenuNativeEventV1::post(const MenuNativeEventServicesV1& s,std::string& e){
    // Original 0x4231b4 and 0x423098. Empty owned vector is the constructor
    // branch; it does not stand in for an unavailable dead-zone provider.
    if(!render_bound)return true;
    if(!s.raw_position)return missing(e,"raw event position");
    int x=0,y=0;if(!s.raw_position(s.context,x,y,e))return false;
    for(const auto& r:dead_zones){
        if(float(x)<r[0]||float(x)>r[1]||float(y)<r[2]||float(y)>r[3])continue;
        if(!s.consume)return missing(e,"consume event");
        return s.consume(s.context,e);
    }
    return true;
}
bool MenuNativeEventV1::base(SwfEvent48& event,const MenuNativeEventServicesV1& s,std::string& e){
    if(!event.name)return missing(e,"event name");
    // Original 0x423214. DragAndDrop runs before rollover/release policy.
    if(drag_bound){if(!s.drag_event)return missing(e,"DragAndDrop::OnEvent");if(!s.drag_event(s.context,event,e))return false;}
    if(rollover_enabled&&event.kind==8&&std::strstr(event.name,"btn")==event.name){
        if(!s.can_mouse)return missing(e,"character CanHandleMouseEvent");
        bool allowed=false;if(!s.can_mouse(s.context,event.character,allowed,e))return false;
        if(allowed){if(!s.focus)return missing(e,"RenderFX::SetFocus");if(!s.focus(s.context,event.character,e))return false;}
    }
    if(event.kind==6&&std::strstr(event.name,"btn_MENU_TWITTER")){
        if(!s.browser)return missing(e,"nativeOpenBrowser");
        if(!s.browser(s.context,"http://ingameads.gameloft.com/redir/?from=D2HP&op=TBFV&game=D2HP&t=twitter",e))return false;
    }
    return post(s,e);
}
bool MenuNativeEventV1::main(SwfEvent48& event,const MenuNativeEventServicesV1& s,std::string& e){
    if(!event.name)return missing(e,"event name");
    // Original 0x42bb48 uses strstr and passes a remapped savegame language.
    if(event.kind==6){
        const bool live=std::strstr(event.name,"btn_GLLive")!=nullptr;
        if(live||std::strstr(event.name,"btn_MENU_MORE_GAMES")){
            if(!s.language)return missing(e,"SavegameManager::getLanguage");
            int language=0;if(!s.language(s.context,language,e))return false;
            if(language>=4&&language<=6)++language;else if(language==7)language=4;
            if(!s.online)return missing(e,live?"nativeOpenGLive":"nativeOpenIGP");
            if(!s.online(s.context,live,language,e))return false;
        }
    }
    return base(event,s,e);
}
}
