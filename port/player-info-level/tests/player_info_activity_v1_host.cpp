#include "../player_info_activity_v1.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
namespace n=dh2::netstruct_members_v1;
namespace p=dh2::player_info_record_v1;
namespace a=dh2::player_info_activity_v1;
constexpr unsigned offsets[]={0x130,0x158,0x180,0x1a8,0x1d0,0x1f8,0x220,0x248,
    0x288,0x2b0,0x2e8,0x310,0x338,0x360,0x388,0x3b0,0x3d8,0x400,
    0x428,0x450,0x478,0x4a0,0x4c8,0x4e8,0x508,0x528,0x548,0x570,0x598,0x5c0,0x5e8,0x610,0x638};
constexpr unsigned base=0x10001000;
static unsigned word(const unsigned char* data,unsigned offset) {unsigned v;std::memcpy(&v,data+offset,4);return v;}
struct Fixture {
    p::Record* record=nullptr;
    unsigned calls=0,mutation=0;
    std::uint8_t online=0;
    bool fail=false,throws=false,reenter=false;
    n::Status nested=n::Status::complete;
    static void* allocate(void*,std::size_t size,int) {return std::malloc(size?size:1);}
    static void release(void*,void* pointer) {std::free(pointer);}
    static int query(void* context,std::uint8_t* output) {
        auto& f=*static_cast<Fixture*>(context);++f.calls;
        if(f.reenter) {bool result=false;f.nested=a::player_is_active(*f.record,f.services(),&result);}
        if(f.mutation==1) f.record->at(0x158)->header.value=-1;
        if(f.mutation==2) {
            for(auto offset:{0x158u,0x180u,0x1a8u}) f.record->at(offset)->header.value=0;
            f.record->at(0x1d0)->header.value=3;
        }
        if(f.throws) throw std::runtime_error("activity provider failure");
        if(f.fail) return 1;
        *output=f.online;return 0;
    }
    n::Memory memory() {return {this,allocate,release};}
    a::Services services() {return {this,query};}
};
static void seed(p::Record& r,const unsigned char* data) {
    r.base.field_280=word(data,0x280);auto& net=r.base.network;
    net.source_vtable=word(data,0);net.count=word(data,0x104);net.enabled_108=data[0x108];
    net.field_124=data[0x124];net.field_125=data[0x125];net.field_128=word(data,0x128);
    for(auto offset:offsets) {
        const bool small=offset==0x248||offset==0x2b0||offset==0x3b0||offset==0x3d8||offset==0x400||
                         (offset>=0x4c8&&offset<=0x528);
        std::memcpy(&r.at(offset)->header,data+offset,small?32:36);
    }
    for(unsigned i=0;i<64;++i) {
        const auto value=word(data,4+i*4);
        auto* pointer=reinterpret_cast<n::Member*>(static_cast<std::uintptr_t>(value));
        for(auto offset:offsets) if(value==base+offset) pointer=r.at(offset);
        net.members[i]=pointer;
    }
    r.character_660=word(data,0x660);r.save_slot_664=static_cast<int>(word(data,0x664));
    r.controller_668=static_cast<int>(word(data,0x668));r.local_66c=data[0x66c];
    r.internal_id_670=static_cast<int>(word(data,0x670));r.member_674=static_cast<int>(word(data,0x674));
    r.number_678=static_cast<int>(word(data,0x678));r.group_number_67c=static_cast<int>(word(data,0x67c));
    r.loading_info_680=word(data,0x680);r.field_684=word(data,0x684);
}
static void project(std::FILE* out,p::Record& r) {
    auto& net=r.base.network;
    unsigned words[]={net.source_vtable,net.count,net.enabled_108,net.field_124,net.field_125,net.field_128,r.base.field_280};
    std::fwrite(words,sizeof(words),1,out);
    for(auto* member:net.members) {
        auto value=static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(member));
        for(auto offset:offsets) if(member==r.at(offset)) value=base+offset;
        std::fwrite(&value,4,1,out);
    }
    for(auto offset:offsets) {
        const bool small=offset==0x248||offset==0x2b0||offset==0x3b0||offset==0x3d8||offset==0x400||
                         (offset>=0x4c8&&offset<=0x528);
        std::fwrite(&r.at(offset)->header,small?32:36,1,out);
    }
    unsigned plain[]={static_cast<unsigned>(r.character_660),static_cast<unsigned>(r.save_slot_664),static_cast<unsigned>(r.controller_668),r.local_66c,
                     static_cast<unsigned>(r.internal_id_670),static_cast<unsigned>(r.member_674),static_cast<unsigned>(r.number_678),
                     static_cast<unsigned>(r.group_number_67c),static_cast<unsigned>(r.loading_info_680),r.field_684};
    std::fwrite(plain,sizeof(plain),1,out);
}
static unsigned policies() {
    unsigned checks=0;Fixture f;std::uint64_t serial=0;
    p::Factory factory(&serial,f.memory());p::Record r;f.record=&r;
    bool output=true;
    if(a::player_is_active(r,f.services(),&output)!=n::Status::invalid_state||f.calls||!output) return 0;
    ++checks;
    if(a::cnet_is_active(r.base,&output)!=n::Status::invalid_state||!output) return 0;
    ++checks;
    if(a::set_state(r,3,0)!=n::Status::invalid_state||serial) return 0;
    ++checks;
    if(factory.construct_record(r)!=n::Status::complete) return 0;
    const auto before=serial;
    if(a::cnet_is_active(r.base,nullptr)!=n::Status::invalid_argument||serial!=before) return 0;
    ++checks;
    if(a::player_is_active(r,{},&output)!=n::Status::missing_provider||f.calls||!output) return 0;
    ++checks;
    f.fail=true;
    if(a::player_is_active(r,f.services(),&output)!=n::Status::provider_failed||!output||r.busy) return 0;
    ++checks;f.fail=false;f.throws=true;
    if(a::player_is_active(r,f.services(),&output)!=n::Status::provider_failed||!output||r.busy) return 0;
    ++checks;f.throws=false;f.reenter=true;
    if(a::player_is_active(r,f.services(),&output)!=n::Status::complete||!output||f.nested!=n::Status::invalid_state||r.busy) return 0;
    ++checks;f.reenter=false;
    auto& last=*r.at(a::activity_member);last.constructed=false;f.online=0;
    if(a::player_is_active(r,f.services(),&output)!=n::Status::complete||!output) return 0;
    ++checks;f.online=1;
    r.at(0x158)->header.value=-1;
    if(a::player_is_active(r,f.services(),&output)!=n::Status::complete||output) return 0;
    ++checks;
    for(auto offset:{0x158u,0x180u,0x1a8u}) r.at(offset)->header.value=0;
    output=true;
    if(a::player_is_active(r,f.services(),&output)!=n::Status::invalid_state||!output) return 0;
    ++checks;last.constructed=true;
    auto& destination=*r.at(a::state_member);destination.busy=true;
    if(a::set_state(r,3,0)!=n::Status::invalid_state||serial!=before||r.busy||r.base.busy) return 0;
    ++checks;destination.busy=false;
    auto* shared=destination.serial;std::uint64_t other=serial;destination.serial=&other;
    if(a::set_state(r,3,0)!=n::Status::invalid_argument||serial!=before||other!=before) return 0;
    ++checks;destination.serial=shared;
    auto* serial_pointer=r.base.serial;r.base.serial=nullptr;
    if(a::set_state(r,3,0)!=n::Status::invalid_argument||serial!=before) return 0;
    ++checks;r.base.serial=serial_pointer;
    destination.kind=n::Kind::integer;
    if(a::set_state(r,3,0)!=n::Status::invalid_state||serial!=before) return 0;
    ++checks;destination.kind=n::Kind::unsigned_integer;
    r.busy=true;
    if(a::set_state(r,3,0)!=n::Status::invalid_state||serial!=before) return 0;
    ++checks;r.busy=false;
    r.base.busy=true;
    if(a::set_state(r.base,3,0)!=n::Status::invalid_state||serial!=before) return 0;
    ++checks;r.base.busy=false;
    const auto activity_before=last.header.value;
    if(a::set_state(r,3,3)!=n::Status::complete||serial!=before+1||last.header.value!=activity_before) return 0;
    ++checks;
    if(a::set_state(r.base,3,0)!=n::Status::complete||serial!=before+2||last.header.value!=activity_before) return 0;
    ++checks;
    if(a::set_state(r.base,3,3)!=n::Status::complete||serial!=before+2) return 0;
    ++checks;
    return checks;
}
int main(int argc,char** argv) {
    if(argc!=3) return 2;
    const auto policy_checks=policies();if(policy_checks!=20) return 3;
    auto* in=std::fopen(argv[1],"rb");auto* out=std::fopen(argv[2],"wb");if(!in||!out) return 4;
    unsigned count;if(std::fread(&count,4,1,in)!=1) return 5;
    for(unsigned i=0;i<count;++i) {
        unsigned args[5];std::uint64_t serial;unsigned char data[0x688];
        if(std::fread(args,sizeof(args),1,in)!=1||std::fread(&serial,8,1,in)!=1||std::fread(data,sizeof(data),1,in)!=1) return 6;
        Fixture f;p::Factory factory(&serial,f.memory());p::Record r;f.record=&r;
        const auto before=serial;if(factory.construct_record(r)!=n::Status::complete) return 7;
        seed(r,data);serial=before;f.online=static_cast<std::uint8_t>(args[3]);f.mutation=args[4];
        bool active=false;n::Status status;
        switch(args[0]) {
        case 0:status=a::cnet_is_active(r.base,&active);break;
        case 1:status=a::player_is_active(r,f.services(),&active);break;
        case 2:status=a::set_state(r.base,static_cast<int>(args[1]),static_cast<int>(args[2]));break;
        case 3:status=a::set_state(r,static_cast<int>(args[1]),static_cast<int>(args[2]));break;
        default:return 8;
        }
        if(status!=n::Status::complete) return 9;
        unsigned result[]={active?1u:0u,f.calls};
        std::fwrite(&serial,8,1,out);std::fwrite(result,sizeof(result),1,out);project(out,r);
    }
    std::fclose(in);std::fclose(out);std::printf("%u\n",policy_checks);return 0;
}
