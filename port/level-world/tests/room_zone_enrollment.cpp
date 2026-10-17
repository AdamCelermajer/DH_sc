#include "../room_zone_enrollment.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace dh2::room_zone_enrollment;

struct Fixture {
    float min_x, min_y, max_x, max_y, x, y;
    std::uint8_t in_room, in_zone, zoning_enabled, visible;
    Address old_room, visual;
    std::uint32_t zonable_first, zonable_second;
    unsigned mutation, zonable_calls;
    int fail_operation;
    std::vector<Request> calls;
};

static std::int32_t invoke(void* context, const Request* request, std::uint32_t* result) {
    auto& f = *static_cast<Fixture*>(context);
    f.calls.push_back(*request);
    if (static_cast<int>(request->operation) == f.fail_operation) return 1;
    switch (request->operation) {
    case Operation::is_zonable:
        ++f.zonable_calls;
        if (f.zonable_calls==1 && f.mutation==1) f.x=2.0f;
        if (f.zonable_calls==1 && f.mutation==2) f.min_x=2.0f;
        if (f.zonable_calls==1 && f.mutation==5) f.old_room=0x02450000;
        if (f.zonable_calls==2 && f.mutation==3) { f.zoning_enabled=0; f.in_zone=0; }
        if (f.zonable_calls==2 && f.mutation==4) f.in_zone=0;
        *result = f.zonable_calls == 1 ? f.zonable_first : f.zonable_second;
        f.calls.back().value=*result;
        return 0;
    default:
        *result = 0;
        return 0;
    }
}

int main(int argc, char** argv) {
    if (argc != 18) return 2;
    const bool entering = argv[1][0] == 'e' && argv[1][1] == 'n';
    const bool adding = argv[1][0] == 'a';
    Fixture f{};
    f.x=std::strtof(argv[2],nullptr); f.y=std::strtof(argv[3],nullptr);
    f.min_x=std::strtof(argv[4],nullptr); f.min_y=std::strtof(argv[5],nullptr);
    f.max_x=std::strtof(argv[6],nullptr); f.max_y=std::strtof(argv[7],nullptr);
    f.old_room=static_cast<Address>(std::strtoull(argv[8],nullptr,0));
    f.in_room=static_cast<std::uint8_t>(std::strtoul(argv[9],nullptr,0));
    f.in_zone=static_cast<std::uint8_t>(std::strtoul(argv[10],nullptr,0));
    f.zoning_enabled=static_cast<std::uint8_t>(std::strtoul(argv[11],nullptr,0));
    f.visible=static_cast<std::uint8_t>(std::strtoul(argv[12],nullptr,0));
    f.visual=static_cast<Address>(std::strtoull(argv[13],nullptr,0));
    f.zonable_first=static_cast<std::uint32_t>(std::strtoul(argv[14],nullptr,0));
    f.zonable_second=static_cast<std::uint32_t>(std::strtoul(argv[15],nullptr,0));
    const auto mutate=static_cast<unsigned>(std::strtoul(argv[16],nullptr,0));
    const auto fail_op=static_cast<int>(std::strtol(argv[17],nullptr,0));
    f.mutation=mutate; f.fail_operation=fail_op;

    constexpr Address object_id=0x02110000, room_id=0x02220000;
    RoomZone zone{room_id,{&f.min_x,&f.min_y,&f.max_x,&f.max_y}};
    GameObject object{object_id,&f.x,&f.y,&f.old_room,&f.in_room,&f.in_zone,
                      &f.zoning_enabled,&f.visual,&f.visible};
    Services services{&f,&invoke}; Result result{};
    const auto status = adding ? add_initial_object(&zone,&object,&services,&result)
        : (entering ? zone_entered(&object,&services,&result)
                    : zone_exited(&object,&services,&result));
    std::cout << "{\"status\":" << static_cast<int>(status)
              << ",\"accepted\":" << (result.accepted?1:0)
              << ",\"rejection\":" << static_cast<unsigned>(result.rejection)
              << ",\"calls\":[";
    for (std::size_t i=0;i<f.calls.size();++i) {
        const auto& c=f.calls[i]; if (i) std::cout << ',';
        std::cout << '[' << static_cast<unsigned>(c.operation) << ',' << c.object << ','
                  << c.room_zone << ',' << c.auxiliary << ',' << c.value << ']';
    }
    std::cout << "],\"room\":" << f.old_room << ",\"in_room\":"
              << static_cast<unsigned>(f.in_room) << ",\"in_zone\":"
              << static_cast<unsigned>(f.in_zone) << "}\n";
}
