#include "../app/src/main/cpp/front_inspection_avatar_services_v1.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
void check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
struct DrawOwner {
    std::int32_t drawn_slot{-1};
    unsigned destroys{},setups{},cameras{};
    static bool destroy(void* p,std::string&){auto& self=*static_cast<DrawOwner*>(p);++self.destroys;self.drawn_slot=-1;return true;}
    static bool setup(void* p,std::int32_t slot,std::string&){auto& self=*static_cast<DrawOwner*>(p);++self.setups;self.drawn_slot=slot;return true;}
    static bool camera(void* p,std::string&){++static_cast<DrawOwner*>(p)->cameras;return true;}
    dh2::ui::MenuAvatarPreviewServicesV1 services(){return {this,destroy,setup,camera};}
};
}
int main(){try{
    using namespace dh2;
    ui::MenuAvatarPreviewStateV1 state;
    DrawOwner canonical,inspection;
    auto retained=std::make_shared<int>(1);
    auto services=canonical.services();std::string error;
    check(ui::change_menu_avatar_preview_v1(state,0,false,services,error),"initial canonical slot");
    // Reproduce metadata reload, persistent persona marker, and creation's
    // enable-persona attempt after the process adapter has been published.
    for(unsigned pass=0;pass<3;++pass){
        check(!android_ui::install_front_inspection_avatar_services_v1(services,retained,inspection.services()),"inspection replaced retained process provider");
        check(services.context==&canonical,"canonical receiver changed");
    }
    for(const auto slot:{2,3,0,-1}){
        check(ui::change_menu_avatar_preview_v1(state,slot,false,services,error),"native slot transition");
        check(state.slot==slot&&canonical.drawn_slot==slot,"UI slot and rendered receiver diverged");
    }
    check(inspection.setups==0&&inspection.destroys==0&&inspection.cameras==0,"inspection consumed live process callbacks");
    check(canonical.setups==5&&canonical.destroys==4&&canonical.cameras==5,"original transition effects changed");
    // An explicit inspection-only session retains its existing behavior.
    services={};
    check(android_ui::install_front_inspection_avatar_services_v1(services,{},inspection.services()),"inspection provider unavailable without process");
    check(services.context==&inspection,"wrong inspection receiver");
    // Show that the old unconditional metadata assignment reproduced the
    // actual failure: the UI advances to Mage slot 2 while draw stays slot 0.
    state.slot=0;canonical.drawn_slot=0;services=inspection.services();
    check(ui::change_menu_avatar_preview_v1(state,2,false,services,error),"old routing reproduction");
    check(state.slot==2&&canonical.drawn_slot==0&&inspection.drawn_slot==2,"old stale-character reproduction failed");
    std::cout<<"PASS retained process preview routing across metadata, marker, creation and slot changes; old stale draw reproduced\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
