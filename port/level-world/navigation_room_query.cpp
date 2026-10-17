#include "navigation_world.hpp"

#include <cmath>

namespace {
bool contains(const dh2::octree::Box& bounds,const float* point) noexcept {
    for(unsigned axis=0;axis<3;++axis)
        if(!(bounds.minimum[axis]<=point[axis]&&
             point[axis]<=bounds.maximum[axis]))return false;
    return true;
}
}

extern "C" int dh2_nav_world_room_at(std::uint32_t* out,
    const dh2::navigation::CollisionWorld* world,const float* point) {
    if(!out||!world||!point||
       (world->room_count&&!world->rooms))return -1;
    for(unsigned axis=0;axis<3;++axis)
        if(!std::isfinite(point[axis]))return -1;
    if(!contains(world->bounds,point))return 0;
    for(std::uint32_t room=0;room<world->room_count;++room) {
        const auto& candidate=world->rooms[room];
        // The port creates room entries from rooms with imported floor data;
        // zero-count slots represent absent room indices, not PFRoom objects.
        if(candidate.count&&!(candidate.source_flags&1u)&&
           contains(candidate.bounds,point)) {
            *out=room;
            return 1;
        }
    }
    return 0;
}
