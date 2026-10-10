#include "npc_source_services.hpp"
#include <cassert>
#include <iostream>
using namespace dh::foundation::interactions;
struct Listener {
    int calls=0;
    static bool event(void* raw,const dh2::events::EventBorrowV12& event,dh2::events::EventManagerOwnerV12& manager,
     std::int32_t& result,std::string& e){
        auto& self=*static_cast<Listener*>(raw);assert(manager.identity()==300);
        dh2::loader::GameEventQuestBorrowV75 q;
        assert(dh2::loader::project_scoped_game_quest_event_v75(event,q,e));
        assert(q.character8==7&&q.id18==-7&&*q.quantity14==-1&&!*q.from_network11&&!*q.pending_network10);
        *q.pending_network10=1;assert(*q.character8_cell==7&&*q.id18_cell==-7);
        ++self.calls;result=0;return true;
    }
};
int main(){
    auto world=std::make_shared<int>(0);dh2::events::EventManagerOwnerV12 manager(300);Listener listener;
    bool inserted=false;std::string e;
    assert(manager.attach(23,{400,&listener,Listener::event,world},0,inserted,e)&&inserted);
    NpcInteractServices services;assert(bind_npc_quest_raise(services,[&](auto id,NpcLevelEventBorrow& out,std::string&){assert(id==100||id==0);out={world,100,&manager};return true;},e));
    NpcTalkEvent event;event.objective_type=23;event.actor=7;event.room=11;event.data_id=-7;
    assert(services.raise_async(100,event,e));assert(listener.calls==1&&manager.pending_count()==0);
    assert(!services.raise_async(0,event,e)&&!e.empty());
    std::cout<<"NPC TalkToNPC source payload delivered through SAME native EventManager/projection; no alternate queue\n";
}
