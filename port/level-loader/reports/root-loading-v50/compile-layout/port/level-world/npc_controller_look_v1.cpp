#include "npc_controller_look_v1.hpp"
namespace dh2::character {
int npc_controller_look_v1(const NpcControllerLookBorrowV1& b,std::uintptr_t target){
 if(!b.world||!b.identity||!b.source_heading_178)return -1;
 skills::WorldTargetActorBorrowV1 actor{};
 if(b.world->actor(b.identity,&actor)||actor.identity!=b.identity||
    actor.heading_angle!=b.source_heading_178||
    actor.controller_heading_angle!=b.source_heading_178)return -1;
 // Complete original controller gates and LookAt(object/point) owner kernels;
 // null target completes its genuine void branch without changing heading.
 return b.world->targets().look_at(b.identity,target);
}
}
