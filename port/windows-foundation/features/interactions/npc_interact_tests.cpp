#include "npc_interact.hpp"
#include <cassert>
#include <iostream>
using namespace dh::foundation::interactions;
int main(){
    auto lease=std::make_shared<int>(0);std::int32_t room=11;std::int16_t data=-7;std::uint8_t talk=0;
    const std::string name="Original NPC";NpcInteractBorrow b{lease,42,&room,&data,&talk,&name};
    NpcInteractServices s;s.world=lease;bool interacting=false,merchant=true,cleaner=false;int count=0;
    std::vector<std::string> order;std::vector<std::int32_t> row{120,227};std::string e;
    s.is_interacting=[&](auto id,bool& v,std::string&){assert(id==42);order.push_back("is");v=interacting;return true;};
    s.current_level=[&](auto& v,std::string&){order.push_back("level");v=100;return true;};
    s.constant=[&](auto type,auto value,auto& v,std::string&){assert(std::string(type)=="v2QuestObjectiveType"&&std::string(value)=="TalkToNPC");order.push_back("constant");room=12;data=1;v=23;return true;};
    s.raise_async=[&](auto level,const NpcTalkEvent& event,std::string&){assert(level==100&&event.room==11&&event.data_id==-7&&event.actor==7&&event.objective_type==23&&event.source_index==-1&&!event.byte18&&!event.byte19);order.push_back("quest");return true;};
    s.set_interact_state=[&](auto id,auto kind,bool yes,auto actor,bool no,std::string&){assert(id==42&&kind==3&&yes&&actor==7&&!no);order.push_back("fsm");return true;};
    s.raise_character_event=[&](auto id,auto event,auto actor,std::string&){assert(id==42&&event==5&&actor==7);order.push_back("event5");return true;};
    s.is_merchant=[&](auto,bool& v,std::string&){order.push_back("merchant");v=merchant;return true;};
    s.has_loot=[&](auto,bool& v,std::string&){v=true;return true;};
    s.handle_as_character=[&](auto id,auto& v,std::string&){assert(id==7);v=7;return true;};
    s.get_loot=[&](auto id,auto& v,std::string&){assert(id==42);order.push_back("loot");v=4;return true;};
    s.merchant_table=[&](auto id,MerchantRowBorrow& v,std::string&){assert(id==4);order.push_back("table");v={lease,&row};return true;};
    s.num_items=[&](auto id,auto& v,std::string&){assert(id==42);order.push_back("num");v=count;return true;};
    s.add_merchant_loot=[&](auto id,auto loot,std::string&){assert(id==42);order.push_back("add"+std::to_string(loot));return true;};
    s.hud_root=[&](auto& id,auto& pin,std::string&){order.push_back("hud");id=200;pin=lease;return true;};
    s.merchant_root=[&](auto& id,auto& pin,std::string&){order.push_back("menu");id=201;pin=lease;return true;};
    s.player_info_id=[&](auto actor,auto& id,std::string&){assert(actor==7);id=10;order.push_back("player");return true;};
    s.invoke_as=[&](auto id,auto path,auto method,const std::vector<NpcAsValue>& values,std::string&){assert(std::string(path)=="_root");order.push_back(method);
        if(std::string(method)=="AdditionnalInfosForMerchant"){assert(id==201&&values.size()==2&&values[0].string=="Original NPC"&&values[1].number==12);}
        else {assert(id==200&&values.size()==1&&values[0].number==10);}return true;};
    s.is_cleaner=[&](auto,bool& v,std::string&){order.push_back("cleaner");v=cleaner;return true;};
    assert(npc_interact(b,7,s,e));
    const std::vector<std::string> expected={"is","level","constant","quest","fsm","event5","merchant","loot","table","num","add120","add227","hud","player","openMerchantMenu","menu","AdditionnalInfosForMerchant"};
    assert(order==expected);
    // Already interacting omits a duplicate quest event but still invokes FSM.
    order.clear();interacting=true;talk=1;assert(npc_interact(b,7,s,e));
    assert((order==std::vector<std::string>{"is","fsm"}));
    order.clear();talk=0;merchant=false;cleaner=true;assert(npc_interact(b,7,s,e));
    assert((order==std::vector<std::string>{"is","fsm","event5","merchant","cleaner","hud","player","openCleanerMenu"}));
    // Missing UI fails after actual FSM/event prefix; no invented menu success.
    order.clear();s.invoke_as={};assert(!npc_interact(b,7,s,e)&&!e.empty());
    assert((order==std::vector<std::string>{"is","fsm","event5","merchant","cleaner","hud","player"}));
    order.clear();interacting=false;s.raise_async={};assert(!npc_interact(b,7,s,e));
    assert((order==std::vector<std::string>{"is","level","constant"}));
    std::cout<<"NPC source Interact ordering, quest snapshots, merchant/cleaner UI and partial failures passed\n";
}
