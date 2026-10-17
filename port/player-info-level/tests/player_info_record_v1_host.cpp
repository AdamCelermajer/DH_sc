#include "../player_info_record_v1.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
namespace n=dh2::netstruct_members_v1;
namespace p=dh2::player_info_record_v1;
constexpr unsigned offsets[]={0x130,0x158,0x180,0x1a8,0x1d0,0x1f8,0x220,0x248,
    0x288,0x2b0,0x2e8,0x310,0x338,0x360,0x388,0x3b0,0x3d8,0x400,
    0x428,0x450,0x478,0x4a0,0x4c8,0x4e8,0x508,0x528,0x548,0x570,0x598,0x5c0,0x5e8,0x610,0x638};
constexpr unsigned bases[]={0x10001000,0x10002000};
constexpr unsigned scalar_offsets[]={0x310,0x338,0x360,0x388,0x428,0x478};
using Event=std::array<unsigned,4>;
struct Fixture {
    std::map<void*,unsigned> allocations;
    std::vector<Event> events;
    unsigned next=1,create=0,delete_fn=0;
    bool guard=false,fail_factory=false,fail_free=false,fail_delete=false;
    p::Record* reenter=nullptr;
    n::Status nested=n::Status::complete;
    static void* allocate(void* context,std::size_t size,int tag) {
        auto& f=*static_cast<Fixture*>(context);auto* v=std::malloc(size?size:1);
        if(!v) throw std::bad_alloc();
        f.allocations[v]=f.next;f.events.push_back({1,static_cast<unsigned>(size),static_cast<unsigned>(tag),f.next++});return v;
    }
    static void release(void* context,void* v) {
        auto& f=*static_cast<Fixture*>(context);
        if(f.fail_free) throw std::runtime_error("fixture free failure");
        f.events.push_back({2,f.allocations.at(v),0,0});f.allocations.erase(v);std::free(v);
    }
    static n::Status factory(void* context) {
        auto& f=*static_cast<Fixture*>(context);
        if(f.reenter) f.nested=p::reset(*f.reenter,{});
        if(f.fail_factory) return n::Status::provider_failed;
        if(!f.guard) {
            f.events.push_back({3,1,0,0});f.create=0x374ca0;f.delete_fn=0x36d258;
            f.events.push_back({4,f.create,f.delete_fn,0});f.guard=true;f.events.push_back({5,0,0,0});
        }
        return n::Status::complete;
    }
    static n::Status deleting(void* context,std::uintptr_t value) {
        auto& f=*static_cast<Fixture*>(context);
        if(f.fail_delete) return n::Status::provider_failed;
        f.events.push_back({6,static_cast<unsigned>(value),0,0});return n::Status::complete;
    }
    n::Memory memory() {return {this,allocate,release};}
    p::Services services() {return {this,factory,deleting};}
    ~Fixture() {for(auto [v,id]:allocations){(void)id;std::free(v);}}
};
static unsigned word(const unsigned char* data,unsigned offset) {unsigned v;std::memcpy(&v,data+offset,4);return v;}
static bool read_text(std::FILE* f,std::string& text) {
    unsigned size;if(std::fread(&size,4,1,f)!=1 || size>4096) return false;
    text.resize(size);return std::fread(text.data(),1,size,f)==size;
}
static bool seed(std::FILE* in,p::Record& r,bool ready,std::uint64_t* serial,Fixture& fixture,unsigned record,std::array<unsigned char,0x688>& data) {
    if(std::fread(data.data(),data.size(),1,in)!=1) return false;
    auto* d=data.data();r.constructed=ready;r.base.constructed=ready;r.base.serial=serial;r.base.field_280=word(d,0x280);r.services=fixture.services();
    auto& net=r.base.network;net.constructed=ready;net.source_vtable=word(d,0);net.count=word(d,0x104);
    net.enabled_108=d[0x108];net.field_124=d[0x124];net.field_125=d[0x125];net.field_128=word(d,0x128);
    for(auto offset:offsets) {
        auto& m=*r.at(offset);bool flag=offset>=0x4c8&&offset<=0x528;
        bool text=offset==0x248||offset==0x2b0,bytes=offset==0x3b0||offset==0x3d8||offset==0x400;
        std::memcpy(&m.header,d+offset,text||bytes||flag?32:36);m.constructed=ready;m.serial=serial;
        m.kind=flag?n::Kind::boolean:text?n::Kind::string32:bytes?n::Kind::byte_array:
               offset>=0x1d0&&offset<=0x220?n::Kind::unsigned_integer:n::Kind::integer;
        m.memory=fixture.memory();
    }
    for(auto offset:{0x248,0x2b0}) {std::string text;if(!read_text(in,text)) return false;r.at(offset)->text=text;}
    unsigned byte_index=0;
    for(auto offset:{0x3b0,0x3d8,0x400}) {
        std::string value;if(!read_text(in,value)) return false;auto& m=*r.at(offset);
        if(ready) {
            m.bytes.size=static_cast<int>(value.size());
            if(!value.empty()) {
                m.bytes.data=static_cast<unsigned char*>(std::malloc(value.size()));
                if(!m.bytes.data) return false;
                std::memcpy(m.bytes.data,value.data(),value.size());fixture.allocations[m.bytes.data]=100+record*3+byte_index;
            }
        }
        ++byte_index;
    }
    r.character_660=word(d,0x660);r.save_slot_664=static_cast<int>(word(d,0x664));r.controller_668=static_cast<int>(word(d,0x668));r.local_66c=d[0x66c];
    r.internal_id_670=static_cast<int>(word(d,0x670));r.member_674=static_cast<int>(word(d,0x674));r.number_678=static_cast<int>(word(d,0x678));
    r.group_number_67c=static_cast<int>(word(d,0x67c));r.loading_info_680=word(d,0x680);r.field_684=word(d,0x684);return true;
}
static void pointers(p::Record& r,const unsigned char* data,p::Record& a,p::Record& b) {
    for(unsigned i=0;i<64;++i) {
        auto v=word(data,4+i*4);auto* member=reinterpret_cast<n::Member*>(static_cast<std::uintptr_t>(v));
        for(auto offset:offsets) {if(v==bases[0]+offset) member=a.at(offset);if(v==bases[1]+offset) member=b.at(offset);}
        r.base.network.members[i]=member;
    }
}
static void project(std::FILE* out,p::Record& r,p::Record& a,p::Record& b) {
    auto& net=r.base.network;unsigned words[]={net.source_vtable,net.count,net.enabled_108,net.field_124,net.field_125,net.field_128,r.base.field_280};
    std::fwrite(words,sizeof(words),1,out);
    for(auto* member:net.members) {
        auto v=static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(member));
        for(auto offset:offsets) {if(member==a.at(offset)) v=bases[0]+offset;if(member==b.at(offset)) v=bases[1]+offset;}
        std::fwrite(&v,4,1,out);
    }
    for(auto offset:offsets) {
        const auto& m=*r.at(offset);bool text=offset==0x248||offset==0x2b0,bytes=offset==0x3b0||offset==0x3d8||offset==0x400;
        std::fwrite(&m.header,text||bytes||(offset>=0x4c8&&offset<=0x528)?32:36,1,out);
        if(text) {auto size=static_cast<unsigned>(m.text.size());std::fwrite(&size,4,1,out);std::fwrite(m.text.data(),1,size,out);}
        if(bytes) {unsigned words[]={static_cast<unsigned>(m.bytes.size),m.bytes.data?1u:0u};std::fwrite(words,sizeof(words),1,out);if(m.bytes.data) std::fwrite(m.bytes.data,1,m.bytes.size,out);}
    }
    unsigned plain[]={static_cast<unsigned>(r.character_660),static_cast<unsigned>(r.save_slot_664),static_cast<unsigned>(r.controller_668),r.local_66c,
                     static_cast<unsigned>(r.internal_id_670),static_cast<unsigned>(r.member_674),static_cast<unsigned>(r.number_678),static_cast<unsigned>(r.group_number_67c),
                     static_cast<unsigned>(r.loading_info_680),r.field_684};std::fwrite(plain,sizeof(plain),1,out);
}
static unsigned policies() {
    unsigned count=0;
    {Fixture f;p::Record r;std::uint64_t serial=7;
     if(p::construct(r,nullptr,f.memory(),f.services(),{})!=n::Status::invalid_argument || serial!=7) return 0;
     ++count;}
    {Fixture f;p::Record r;std::uint64_t serial=7;
     if(p::construct(r,&serial,f.memory(),{}, {})!=n::Status::missing_provider || r.constructed || r.base.network.count!=8 || r.character_660!=0) return 0;
     if(p::destroy(r)!=n::Status::complete || !f.allocations.empty()) return 0;
     ++count;}
    {Fixture f;p::Record r;std::uint64_t serial=7;f.fail_factory=true;
     if(p::construct(r,&serial,f.memory(),f.services(),{})!=n::Status::provider_failed || r.base.network.count!=8) return 0;
     f.fail_factory=false;if(p::destroy(r)!=n::Status::complete) return 0;
     ++count;}
    {Fixture f;p::Record r;std::uint64_t serial=7;f.reenter=&r;
     if(p::construct(r,&serial,f.memory(),f.services(),{})!=n::Status::complete || f.nested!=n::Status::invalid_state || r.base.network.count!=33) return 0;
     ++count;
     auto before=serial;if(p::set_character_scalar(r,0x660,2,0)!=n::Status::invalid_argument || serial!=before) return 0;
     ++count;
     if(p::construct(r,&serial,f.memory(),f.services(),{})!=n::Status::invalid_state) return 0;
     ++count;}
    {Fixture f;p::Record r;std::uint64_t serial=7;
     if(p::construct(r,&serial,f.memory(),f.services(),{})!=n::Status::complete) return 0;
     r.loading_info_680=0x1234;auto* bytes=r.at(0x400)->bytes.data;r.services.delete_loading_info=nullptr;
     if(p::destroy(r)!=n::Status::missing_provider || r.loading_info_680!=0x1234 || r.at(0x400)->bytes.data!=bytes) return 0;
     ++count;
     r.services=f.services();f.fail_delete=true;
     if(p::destroy(r)!=n::Status::provider_failed || r.loading_info_680!=0x1234 || r.at(0x400)->bytes.data!=bytes) return 0;
     ++count;
     f.fail_delete=false;f.fail_free=true;
     if(p::destroy(r)!=n::Status::provider_failed || r.loading_info_680!=0 || r.at(0x400)->bytes.data!=bytes) return 0;
     ++count;
     f.fail_free=false;if(p::destroy(r)!=n::Status::complete || !f.allocations.empty()) return 0;
     ++count;}
    {Fixture f;p::Record a,b;std::uint64_t first=7,second=7;
     if(p::construct(a,&first,f.memory(),f.services(),{})!=n::Status::complete || p::construct(b,&second,f.memory(),f.services(),{})!=n::Status::complete) return 0;
     auto before=first;if(p::assign(a,b)!=n::Status::invalid_argument || first!=before) return 0;
     ++count;}
    {Fixture f;std::uint64_t serial=0;p::Factory factory(&serial,f.memory());p::Record* record=nullptr;
     if(factory.registered() || factory.create_record(&record)!=n::Status::complete || !record || !factory.registered() || record->base.network.count!=33) return 0;
     if(f.events.front()!=Event{1,static_cast<unsigned>(sizeof(p::Record)),0,1} || record->at(0x310)->header.value!=-1 || record->at(0x360)->header.value!=-1 || record->loading_info_680 || serial!=13) return 0;
     if(factory.delete_record(record)!=n::Status::complete || !f.allocations.empty() || factory.delete_record(nullptr)!=n::Status::complete) return 0;
     ++count;}
    {Fixture f;std::uint64_t serial=0;p::Factory factory(&serial,{});auto* unchanged=reinterpret_cast<p::Record*>(std::uintptr_t{1});
     if(factory.create_record(&unchanged)!=n::Status::missing_provider || unchanged!=reinterpret_cast<p::Record*>(std::uintptr_t{1}) || serial) return 0;
     ++count;}
    {Fixture f;std::uint64_t serial=0;auto memory=f.memory();memory.allocate=[](void*,std::size_t,int)->void*{return nullptr;};p::Factory factory(&serial,memory);p::Record* record=nullptr;
     if(factory.create_record(&record)!=n::Status::allocation_failed || record || serial || factory.registered()) return 0;
     ++count;}
    return count;
}
int main(int argc,char** argv) {
    if(argc!=3) return 2;
    auto* in=std::fopen(argv[1],"rb");auto* out=std::fopen(argv[2],"wb");if(!in||!out) return 3;
    unsigned count;if(std::fread(&count,4,1,in)!=1) return 4;
    for(unsigned index=0;index<count;++index) {
        unsigned op,value,guard;std::uint64_t serial;Fixture fixture;std::array<unsigned char,0x688> da{},db{};
        if(std::fread(&op,4,1,in)!=1 || std::fread(&value,4,1,in)!=1 || std::fread(&guard,4,1,in)!=1 || std::fread(&serial,8,1,in)!=1) return 5;
        fixture.guard=guard!=0;fixture.create=guard?0x374ca0:0;fixture.delete_fn=guard?0x36d258:0;
        p::Record a,b;if(!seed(in,a,true,&serial,fixture,0,da) || !seed(in,b,op==2||op>=4,&serial,fixture,1,db)) return 6;
        pointers(a,da.data(),a,b);pointers(b,db.data(),a,b);
        std::string name;if(!read_text(in,name)) return 7;unsigned residues[13];if(std::fread(residues,sizeof(residues),1,in)!=1) return 8;
        p::ResetResidues policy;for(unsigned i=0;i<6;++i) policy.setters[i]=static_cast<int>(residues[i]);
        policy.first_array=static_cast<int>(residues[6]);policy.controller_type=static_cast<int>(residues[7]);policy.field_4a0=static_cast<int>(residues[8]);
        for(unsigned i=0;i<4;++i) policy.flags[i]=static_cast<unsigned char>(residues[9+i]);
        n::Status status;
        switch(op) {
        case 0:case 1:status=p::construct(b,&serial,fixture.memory(),fixture.services(),policy);break;
        case 2:status=p::reset(b,policy);break;
        case 3:status=p::copy_construct(b,a);break;
        case 4:status=p::assign(b,a);break;
        case 5:status=p::assign(b,b);break;
        case 6:case 7:status=p::destroy(b);break;
        case 8:status=p::set_character_name(b,name);break;
        default:if(op>14) return 9;status=p::set_character_scalar(b,scalar_offsets[op-9],static_cast<int>(value),static_cast<int>(residues[op-9]));break;
        }
        if(status!=n::Status::complete) {std::fprintf(stderr,"case %u op %u status %u\n",index,op,static_cast<unsigned>(status));return 10;}
        std::fwrite(&serial,8,1,out);project(out,a,a,b);project(out,b,a,b);
        unsigned factory[]={fixture.guard?1u:0u,fixture.create,fixture.delete_fn};std::fwrite(factory,sizeof(factory),1,out);
        auto traces=static_cast<unsigned>(fixture.events.size());std::fwrite(&traces,4,1,out);for(auto event:fixture.events) std::fwrite(event.data(),sizeof(event),1,out);
        if((op==3||op==4) && b.base.network.members[8]!=a.base.network.members[8]) return 11;
    }
    std::fclose(in);std::fclose(out);auto checks=policies();if(checks!=14) return 12;std::printf("%u\n",checks);return 0;
}
