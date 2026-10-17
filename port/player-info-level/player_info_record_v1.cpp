#include "player_info_record_v1.hpp"
#include <new>

namespace dh2::player_info_record_v1 {
namespace n = netstruct_members_v1;
namespace {
constexpr std::uint32_t offsets[] = {
    0x288,0x2b0,0x2e8,0x310,0x338,0x360,0x388,0x3b0,0x3d8,0x400,
    0x428,0x450,0x478,0x4a0,0x4c8,0x4e8,0x508,0x528,
    0x548,0x570,0x598,0x5c0,0x5e8,0x610,0x638};
constexpr unsigned declaration[] = {0,3,4,5,1,6,2,7,8,9,10,11,12,13,
                                    14,15,16,17,18,19,20,21,22,23,24};
constexpr std::uint32_t setters[] = {0x310,0x338,0x360,0x388,0x428,0x478};
constexpr std::uint32_t player_vtable = 0x963938;
struct Busy {bool& flag;explicit Busy(bool& f):flag(f){flag=true;}~Busy(){flag=false;}};
Status temporary_scalar(Record& r,Member& destination,std::int32_t value,std::int32_t residue) {
    Member temporary;temporary.header.value=residue;
    temporary.header.reserved[0]=static_cast<std::uint8_t>(residue);
    auto status=n::construct_scalar(temporary,destination.kind,destination.header.size_bits,value,r.base.serial);
    if(status!=Status::complete) return status;
    return n::assign(destination,temporary);
}
Status name(Record& r,std::string_view value) {
    Member temporary;
    auto status=n::construct_string(temporary,value,r.base.serial);
    return status==Status::complete?n::assign(r.members[1],temporary):status;
}
Status reset_members(Record& r,const ResetResidues& residues) {
    auto status=cnet_player_info_v1::reset(r.base);
    if(status!=Status::complete) return status;
    r.local_66c=1;r.character_660=0;
    status=name(r,{});if(status!=Status::complete) return status;
    for(unsigned i=0;i<4;++i) {
        status=temporary_scalar(r,*r.at(setters[i]),-1,residues.setters[i]);
        if(status!=Status::complete) return status;
    }
    const std::array<std::uint8_t,36> absent=[] {std::array<std::uint8_t,36> a{};a.fill(255);return a;}();
    for(unsigned i=7;i<=9;++i) {
        const auto size=i==7?36:i==8?3:30;
        status=n::set_buffer(r.members[i],{absent.data(),size});
        if(status!=Status::complete) return status;
    }
    for(unsigned i=4;i<6;++i) {
        status=temporary_scalar(r,*r.at(setters[i]),0,residues.setters[i]);
        if(status!=Status::complete) return status;
    }
    // Reset reuses the same temporary slot in all seven array iterations.
    for(unsigned i=18;i<25;++i) {
        status=temporary_scalar(r,r.members[i],0,i==18?residues.first_array:0);
        if(status!=Status::complete) return status;
    }
    r.number_678=-1;
    status=temporary_scalar(r,r.members[0],0,residues.controller_type);
    if(status!=Status::complete) return status;
    r.save_slot_664=-1;r.controller_668=-1;r.internal_id_670=-1;
    r.member_674=-1;r.group_number_67c=-1;
    status=temporary_scalar(r,r.members[13],0,residues.field_4a0);
    if(status!=Status::complete) return status;
    r.field_684=0;r.loading_info_680=0;
    for(unsigned i=14;i<18;++i) {
        status=temporary_scalar(r,r.members[i],0,residues.flags[i-14]);
        if(status!=Status::complete) return status;
    }
    return Status::complete;
}
void plain_fields(Record& destination,const Record& source) {
    destination.character_660=source.character_660;destination.save_slot_664=source.save_slot_664;
    destination.controller_668=source.controller_668;destination.local_66c=source.local_66c;
    destination.internal_id_670=source.internal_id_670;destination.member_674=source.member_674;
    destination.number_678=source.number_678;destination.group_number_67c=source.group_number_67c;
    destination.loading_info_680=source.loading_info_680;destination.field_684=source.field_684;
}
}
Member* Record::at(std::uint32_t offset) {
    if(auto* member=base.at(offset)) return member;
    for(unsigned i=0;i<25;++i) if(offsets[i]==offset) return &members[i];
    return nullptr;
}
const Member* Record::at(std::uint32_t offset) const {
    if(auto* member=base.at(offset)) return member;
    for(unsigned i=0;i<25;++i) if(offsets[i]==offset) return &members[i];
    return nullptr;
}
Status construct(Record& r,std::uint64_t* serial,n::Memory memory,Services services,const ResetResidues& residues) {
    if(!serial) return Status::invalid_argument;
    if(r.constructed || r.busy || r.base.constructed) return Status::invalid_state;
    Busy busy(r.busy);r.services=services;
    auto status=cnet_player_info_v1::construct_c2(r.base,serial);
    if(status!=Status::complete) return status;
    r.base.network.source_vtable=player_vtable;
    for(unsigned i=0;i<25;++i) {
        auto& m=r.members[i];
        if(i==1) status=n::construct_string(m,{},serial);
        else if(i>=7 && i<=9) status=n::construct_bytes(m,i==7?36:i==8?3:30,{},serial,memory);
        else {
            const auto kind=i>=14 && i<=17?n::Kind::boolean:n::Kind::integer;
            const auto bits=i==0?8:i>=14 && i<=17?1:i>=10 && i<=13?32:16;
            status=n::construct_scalar(m,kind,bits,i==6?-1:0,serial);
        }
        if(status!=Status::complete) return status;
    }
    if(!services.ensure_factory) return Status::missing_provider;
    try {status=services.ensure_factory(services.context);}catch(...){return Status::provider_failed;}
    if(status!=Status::complete) return status;
    for(auto i:declaration) {
        status=n::declare_member(r.base.network,r.members[i]);
        if(status!=Status::complete) return status;
    }
    r.constructed=true;return reset_members(r,residues);
}
Status reset(Record& r,const ResetResidues& residues) {
    if(!r.constructed || r.busy) return Status::invalid_state;
    Busy busy(r.busy);return reset_members(r,residues);
}
Status set_character_scalar(Record& r,std::uint32_t offset,std::int32_t value,std::int32_t residue) {
    if(!r.constructed || r.busy) return Status::invalid_state;
    for(auto accepted:setters) if(accepted==offset) {
        Busy busy(r.busy);return temporary_scalar(r,*r.at(offset),value,residue);
    }
    return Status::invalid_argument;
}
Status set_character_name(Record& r,std::string_view value) {
    if(!r.constructed || r.busy) return Status::invalid_state;
    Busy busy(r.busy);return name(r,value);
}
Status copy_construct(Record& destination,const Record& source) {
    if(&destination==&source || destination.constructed || destination.base.constructed || destination.busy || !source.constructed || source.busy) return Status::invalid_state;
    Busy busy(destination.busy);destination.services=source.services;
    auto status=cnet_player_info_v1::copy_construct(destination.base,source.base);
    if(status!=Status::complete) return status;
    destination.base.network.source_vtable=player_vtable;
    for(unsigned i=0;i<25;++i) {
        status=n::copy_construct(destination.members[i],source.members[i]);
        if(status!=Status::complete) return status;
    }
    plain_fields(destination,source);destination.constructed=true;return Status::complete;
}
Status assign(Record& destination,const Record& source) {
    if(!destination.constructed || !source.constructed || destination.busy || source.busy) return Status::invalid_state;
    if(destination.base.serial!=source.base.serial) return Status::invalid_argument;
    Busy busy(destination.busy);
    auto status=cnet_player_info_v1::assign(destination.base,source.base);
    if(status!=Status::complete) return status;
    for(unsigned i=0;i<25;++i) {
        status=n::assign(destination.members[i],source.members[i]);
        if(status!=Status::complete) return status;
    }
    plain_fields(destination,source);return Status::complete;
}
Status destroy(Record& r) {
    if(r.busy) return Status::invalid_state;
    if(!r.constructed && !r.base.network.constructed) return Status::complete;
    Busy busy(r.busy);r.base.network.source_vtable=player_vtable;
    if(r.loading_info_680) {
        if(!r.services.delete_loading_info) return Status::missing_provider;
        Status status;
        try {status=r.services.delete_loading_info(r.services.context,r.loading_info_680);}catch(...){return Status::provider_failed;}
        if(status!=Status::complete) return status;
    }
    r.loading_info_680=0;
    // Int16 D1 is empty. Keep its source vtable metadata unchanged.
    for(unsigned i=18;i<25;++i) r.members[i].constructed=false;
    for(unsigned i=10;i<18;++i) if(r.members[i].constructed) n::transition_to_base(r.members[i]);
    for(int i=9;i>=7;--i) if(r.members[i].constructed) {
        auto& m=r.members[i];m.header.vtable=0x963868;
        const auto status=n::retire_payload(m);if(status!=Status::complete) return status;
        n::transition_to_base(m);m.constructed=false;
    }
    if(r.members[1].constructed) r.members[1].header.vtable=0x963790;
    for(int i=6;i>=2;--i) if(r.members[i].constructed) n::transition_to_base(r.members[i]);
    if(r.members[1].constructed) {
        const auto status=n::retire_payload(r.members[1]);if(status!=Status::complete) return status;
    }
    for(unsigned i=0;i<2;++i) if(r.members[i].constructed) n::transition_to_base(r.members[i]);
    for(auto& member:r.members) member.constructed=false;
    const auto status=cnet_player_info_v1::destroy(r.base);r.constructed=false;return status;
}
Record::~Record() {destroy(*this);}
Status Factory::ensure(void* context) {
    auto& factory=*static_cast<Factory*>(context);
    try {
        const std::lock_guard<std::mutex> lock(factory.guard_);
        if(!factory.create_) {
            factory.create_=create_native;factory.delete_=delete_native;
        }
    } catch(...) {return Status::provider_failed;}
    return Status::complete;
}
Status Factory::delete_loading(void* context,std::uintptr_t value) {
    auto& factory=*static_cast<Factory*>(context);
    if(!factory.loading_delete_) return Status::missing_provider;
    return factory.loading_delete_(factory.loading_context_,value);
}
Services Factory::services() {return {this,ensure,delete_loading};}
Status Factory::construct_record(Record& record) {
    return construct(record,serial_,memory_,services(),residues_);
}
Status Factory::create_native(Factory& factory,Record** output) {
    if(!output || !factory.serial_) return Status::invalid_argument;
    if(factory.active_) return Status::invalid_state;
    if(!factory.memory_.allocate || !factory.memory_.release) return Status::missing_provider;
    Busy busy(factory.active_);void* storage;
    try {storage=factory.memory_.allocate(factory.memory_.context,sizeof(Record),0);}
    catch(...) {return Status::provider_failed;}
    if(!storage) return Status::allocation_failed;
    Record* record;
    try {record=new(storage) Record;}
    catch(...) {
        try {factory.memory_.release(factory.memory_.context,storage);}catch(...){return Status::provider_failed;}
        return Status::allocation_failed;
    }
    const auto status=factory.construct_record(*record);
    if(status!=Status::complete) {
        // Defined native failure policy. Original allocator OOM/exception
        // behavior is outside the ARM equivalence proof.
        const auto retired=destroy(*record);if(retired!=Status::complete) return retired;
        record->~Record();
        try {factory.memory_.release(factory.memory_.context,storage);}catch(...){return Status::provider_failed;}
        return status;
    }
    *output=record;return Status::complete;
}
Status Factory::delete_native(Factory& factory,Record* record) {
    if(!record) return Status::complete; // PlayerInfo::Delete null branch.
    if(factory.active_) return Status::invalid_state;
    if(!factory.memory_.release) return Status::missing_provider;
    if(record->services.context!=&factory) return Status::invalid_argument;
    Busy busy(factory.active_);
    const auto status=destroy(*record);if(status!=Status::complete) return status;
    record->~Record();
    try {factory.memory_.release(factory.memory_.context,record);}catch(...){return Status::provider_failed;}
    return Status::complete;
}
Status Factory::create_record(Record** output) {
    // Direct PlayerInfo::Create also installs the static entries through C1.
    return create_native(*this,output);
}
Status Factory::delete_record(Record* record) {return delete_native(*this,record);}
bool Factory::registered() {
    const std::lock_guard<std::mutex> lock(guard_);
    return create_==create_native && delete_==delete_native;
}
} // namespace dh2::player_info_record_v1
