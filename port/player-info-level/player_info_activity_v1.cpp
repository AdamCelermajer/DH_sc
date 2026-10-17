#include "player_info_activity_v1.hpp"

namespace dh2::player_info_activity_v1 {
namespace n = netstruct_members_v1;
namespace {
struct Busy {bool& flag;explicit Busy(bool& f):flag(f){flag=true;}~Busy(){flag=false;}};
Status activity(const cnet_player_info_v1::Record& r, bool* output) {
    // Source short-circuits each signed identity before reaching the next.
    for(auto offset : {0x158u, 0x180u, 0x1a8u}) {
        const auto& member = *r.at(offset);
        if(!member.constructed || member.busy || member.kind != n::Kind::integer)
            return Status::invalid_state;
        if(member.header.value < 0) {*output = false;return Status::complete;}
    }
    const auto& member = *r.at(activity_member);
    if(!member.constructed || member.busy || member.kind != n::Kind::unsigned_integer)
        return Status::invalid_state;
    *output = member.header.value == 3;
    return Status::complete;
}
}
Status cnet_is_active(const cnet_player_info_v1::Record& r, bool* active) {
    if(!active) return Status::invalid_argument;
    if(!r.constructed || r.busy) return Status::invalid_state;
    return activity(r,active);
}
Status player_is_active(player_info_record_v1::Record& r, const Services& services, bool* active) {
    if(!active) return Status::invalid_argument;
    if(!r.constructed || r.busy || !r.base.constructed || r.base.busy) return Status::invalid_state;
    if(!services.online) return Status::missing_provider;
    Busy busy(r.busy);
    std::uint8_t online = 0;
    try {
        if(services.online(services.context,&online)) return Status::provider_failed;
    } catch(...) {return Status::provider_failed;}
    if(!online) {*active = true;return Status::complete;}
    return activity(r.base,active);
}
Status set_state(cnet_player_info_v1::Record& r, std::int32_t value, std::int32_t residue) {
    if(!r.constructed || r.busy) return Status::invalid_state;
    if(!r.serial) return Status::invalid_argument;
    auto& destination = *r.at(state_member);
    if(!destination.constructed || destination.busy || destination.kind != n::Kind::unsigned_integer ||
       destination.header.size_bits != 8) return Status::invalid_state;
    if(destination.serial != r.serial) return Status::invalid_argument;
    Busy busy(r.busy);
    n::Member temporary;
    temporary.header.value = residue;
    const auto status = n::construct_scalar(temporary,n::Kind::unsigned_integer,8,value,r.serial);
    return status == Status::complete ? n::assign(destination,temporary) : status;
}
Status set_state(player_info_record_v1::Record& r, std::int32_t value, std::int32_t residue) {
    if(!r.constructed || r.busy) return Status::invalid_state;
    Busy busy(r.busy);
    return set_state(r.base,value,residue);
}
} // namespace dh2::player_info_activity_v1
