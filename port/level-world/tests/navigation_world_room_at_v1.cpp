#include "../navigation_world.hpp"

#include <cassert>
#include <cstdint>

int main() {
    using namespace dh2::navigation;
    const dh2::selector::Floor floor{};
    const CollisionFloor floors[]{{&floor,{0,0}}};
    const std::uint32_t floor_ids[]{0};
    CollisionRoom rooms[]{
        {{{0,0,0},{10,10,10}},floor_ids,1,0,0},
        {{{20,20,20},{30,30,30}},floor_ids,1,0,0},
    };
    const CollisionWorld world{rooms,floors,2,1,{{0,0,0},{30,30,30}}};
    std::uint32_t room=~0u;
    const float first_room[]{5,5,5};
    assert(dh2_nav_world_room_at(&room,&world,first_room)==1&&room==0);
    rooms[0].source_flags=1;
    assert(dh2_nav_world_room_at(&room,&world,first_room)==0);
    rooms[0].source_flags=0;
    const float outside_world[]{40,40,40};
    assert(dh2_nav_world_room_at(&room,&world,outside_world)==0);
    const float invalid_point[]{0,0,__builtin_nanf("")};
    assert(dh2_nav_world_room_at(&room,&world,invalid_point)==-1);
}
