#include "npc_menu_services.hpp"
#include <cassert>
#include <iostream>
using namespace dh::foundation::interactions;
int main(){
    NpcInteractServices out;NpcMenuOwnerServices source;std::string e;
    assert(!bind_npc_menu_services(out,source,e)&&!e.empty());
    auto lease=std::make_shared<int>(0);source.manager=lease;
    assert(bind_npc_menu_services(out,source,e));std::uintptr_t id=0;std::shared_ptr<void> pin;
    assert(!out.hud_root(id,pin,e)&&!e.empty());
    assert(!out.merchant_root(id,pin,e)&&!e.empty());
    std::int32_t player=0;assert(!out.player_info_id(7,player,e)&&!e.empty());
    assert(!out.invoke_as(200,"_root","openMerchantMenu",{},e)&&!e.empty());
    dh2::ui::SwfMovie movie;NpcMenuReceiverBorrow unloaded{lease,200,&movie};
    // A genuine but unconstructed native movie must fail at its source scope.
    assert(!invoke_npc_menu_as(unloaded,"_root","openMerchantMenu",{},e)&&!e.empty());
    std::int32_t source_id=-7;
    source.player_info=[&](auto actor,NpcPlayerInfoBorrow& b,std::string&){assert(actor==7);b={lease,77,&source_id};return true;};
    source.hud_root=[&](NpcMenuReceiverBorrow& b,std::string&){b=unloaded;return true;};
    source.renderfx=[&](auto,NpcMenuReceiverBorrow& b,std::string&){b=unloaded;return true;};
    assert(bind_npc_menu_services(out,source,e));
    assert(out.hud_root(id,pin,e)&&id==200&&pin);
    assert(out.player_info_id(7,player,e)&&player==-7);source_id=12;
    assert(out.player_info_id(7,player,e)&&player==12);
    assert(!out.invoke_as(201,"_root","openMerchantMenu",{},e)&&e.find("mismatch")!=std::string::npos);
    assert(!out.invoke_as(200,"_root","openMerchantMenu",{},e));
    std::cout<<"NPC native UI borrow/live PlayerInfo and unloaded-movie failure boundaries passed\n";
}
