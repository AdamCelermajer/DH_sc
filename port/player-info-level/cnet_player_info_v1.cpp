#include "cnet_player_info_v1.hpp"
namespace dh2::cnet_player_info_v1 {
namespace {
constexpr std::uint32_t offsets[]={0x130,0x158,0x180,0x1a8,0x1d0,0x1f8,0x220,0x248};
constexpr std::uint32_t bits[]={17,8,17,16,8,8,8};
struct Busy {bool& flag;explicit Busy(bool& f):flag(f){flag=true;}~Busy(){flag=false;}};
Status reset_members(Record& r) {
    auto status=netstruct_members_v1::set_int(r.members[2],-1);
    if(status!=Status::complete) return status;
    status=netstruct_members_v1::set_int(r.members[3],-1);
    return status==Status::complete?netstruct_members_v1::set_uint(r.members[4],0):status;
}
}
Member* Record::at(std::uint32_t offset) {
    for(unsigned i=0;i<8;++i) if(offsets[i]==offset) return &members[i];
    return nullptr;
}
const Member* Record::at(std::uint32_t offset) const {
    for(unsigned i=0;i<8;++i) if(offsets[i]==offset) return &members[i];
    return nullptr;
}
Status construct(Record& r,std::uint64_t* serial) {
    if(!serial) return Status::invalid_argument;
    if(r.constructed || r.busy) return Status::invalid_state;
    Busy busy(r.busy);r.serial=serial;
    auto status=netstruct_members_v1::construct(r.network);if(status!=Status::complete) return status;
    r.network.source_vtable=0x992370;
    for(unsigned i=0;i<7;++i) {
        const auto kind=i<4?netstruct_members_v1::Kind::integer:netstruct_members_v1::Kind::unsigned_integer;
        status=netstruct_members_v1::construct_scalar(r.members[i],kind,bits[i],0,serial);
        if(status!=Status::complete) return status;
    }
    status=netstruct_members_v1::construct_string(r.members[7],{},serial);
    if(status!=Status::complete) return status;
    r.field_280=0;
    for(auto& m:r.members) {
        status=netstruct_members_v1::declare_member(r.network,m);if(status!=Status::complete) return status;
    }
    r.constructed=true;return reset_members(r);
}
Status construct_c2(Record& r,std::uint64_t* serial) {return construct(r,serial);}
Status reset(Record& r) {
    if(!r.constructed || r.busy) return Status::invalid_state;
    Busy busy(r.busy);return reset_members(r);
}
Status copy_construct(Record& dest,const Record& source) {
    if(&dest==&source || dest.constructed || dest.busy || !source.constructed || source.busy) return Status::invalid_state;
    Busy busy(dest.busy);dest.serial=source.serial;
    auto status=netstruct_members_v1::copy_construct(dest.network,source.network);
    if(status!=Status::complete) return status;
    dest.network.source_vtable=0x992370;
    for(unsigned i=0;i<8;++i) {
        status=netstruct_members_v1::copy_construct(dest.members[i],source.members[i]);
        if(status!=Status::complete) return status;
    }
    dest.field_280=source.field_280;dest.constructed=true;return Status::complete;
}
Status assign(Record& dest,const Record& source) {
    if(!dest.constructed || !source.constructed || dest.busy || source.busy) return Status::invalid_state;
    Busy busy(dest.busy);
    auto status=netstruct_members_v1::assign(dest.network,source.network);
    if(status!=Status::complete) return status;
    for(unsigned i=0;i<8;++i) {
        status=netstruct_members_v1::assign(dest.members[i],source.members[i]);
        if(status!=Status::complete) return status;
    }
    dest.field_280=source.field_280;return Status::complete;
}
Status destroy(Record& r) {
    if(r.busy) return Status::invalid_state;
    if(!r.constructed && !r.network.constructed) return Status::complete;
    Busy busy(r.busy);r.network.source_vtable=0x992370;
    auto& text=r.members[7];
    if(text.constructed) {
        text.header.vtable=0x963790;
        const auto status=netstruct_members_v1::retire_payload(text);if(status!=Status::complete) return status;
    }
    if(r.members[0].constructed) netstruct_members_v1::transition_to_base(r.members[0]);
    r.network.source_vtable=0x9924d8;
    for(int i=7;i>=1;--i) if(r.members[i].constructed) netstruct_members_v1::transition_to_base(r.members[i]);
    for(auto& m:r.members) m.constructed=false;
    const auto status=netstruct_members_v1::destroy(r.network);r.constructed=false;return status;
}
Record::~Record() {destroy(*this);}
} // namespace dh2::cnet_player_info_v1
