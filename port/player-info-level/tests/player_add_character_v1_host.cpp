#include "../player_add_character_v1.hpp"
#include <array>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace a=dh2::player_add_character_v1;
using Row=std::array<std::int64_t,25>;
using Event=std::array<std::int64_t,3>;
constexpr std::uintptr_t MANAGER=0x10001000,PLAYER=0x10003000,FRESH=0x10004000,HOST=0x10005000;
constexpr std::uintptr_t CHARACTER=0x10008000,HOST_CHARACTER=0x1000b000,ROOM=0x1000e000,LEVEL=0x10010000;
struct Fixture {
    Row row;std::uint64_t serial=0;
    dh2::netstruct_members_v1::Memory memory{nullptr,
        [](void*,std::size_t size,int)->void*{return std::malloc(size);},
        [](void*,void* p){std::free(p);}};
    dh2::player_info_record_v1::Factory factory{&serial,memory};
    a::Record record,fresh,host;
    a::Player player{PLAYER,7,&record.at(0x310)->header},fresh_player{FRESH,8,&fresh.at(0x310)->header},hosting{HOST,9,&host.at(0x310)->header};
    a::Player* entries[3]{&player,&fresh_player,&hosting};
    a::Registry registry{MANAGER,entries,3,&player};
    std::int32_t member=-99,id=-88;std::array<std::uint32_t,3> position{{101,102,103}},host_position{{901,902,903}};
    std::uintptr_t room=0;std::uint8_t no_room=0,state=0;std::uint32_t count=0;
    int local_reads=0,online_reads=0,count_reads=0;
    std::vector<Event> trace;
    dh2::player_locality_v1::Services queries{};a::Services services{};
    int fail_at=-1,throw_at=-1,attempts=0;
    a::Runtime* reenter=nullptr;a::Status nested=a::Status::complete;
    static std::int64_t signed_word(std::uint32_t n){return static_cast<std::int32_t>(n);}
    int note(int op,std::int64_t x=0,std::int64_t y=0) {
        trace.push_back({op,x,y});int index=attempts++;
        if(reenter){a::Result out;nested=reenter->add_character(7,{},&out);}
        if(index==throw_at)throw std::runtime_error("declared provider failure");
        return index==fail_at?1:0;
    }
    static Fixture& f(void* p){return *static_cast<Fixture*>(p);}
    static std::uint8_t slot(int index,int seed){return static_cast<std::uint8_t>((seed+index*83)%256);}
    static std::uint8_t learned(int index,int seed){return static_cast<std::uint8_t>((seed+index*19)%256);}
    explicit Fixture(Row input):row(input),count(static_cast<std::uint32_t>(row[21])) {
        for(a::Record* r:{&record,&fresh,&host})
            if(factory.construct_record(*r)!=dh2::netstruct_members_v1::Status::complete)throw std::runtime_error("full record constructor");
        record.at(0x360)->header.value=static_cast<int>(row[1]);record.character_660=row[2]?CHARACTER:0;
        record.save_slot_664=static_cast<int>(row[22]);record.member_674=17;record.internal_id_670=-42;
        record.at(0x4c8)->header.reserved[0]=static_cast<std::uint8_t>(row[14]);
        fresh.character_660=row[20]?HOST_CHARACTER:CHARACTER;host.character_660=row[17]?HOST_CHARACTER:0;
        room=row[18]?ROOM:0;
        auto bytes=[&](unsigned offset,int length,bool slots) {
            auto* m=record.at(offset);if(m->bytes.data)memory.release(nullptr,m->bytes.data);
            m->bytes.size=length;m->bytes.data=length?static_cast<std::uint8_t*>(memory.allocate(nullptr,static_cast<std::size_t>(length),2)):nullptr;
            for(int i=0;i<length;++i)m->bytes.data[i]=slots?slot(i,static_cast<int>(row[13])):learned(i,static_cast<int>(row[13]));
        };
        bytes(0x3d8,static_cast<int>(row[11]),true);bytes(0x400,static_cast<int>(row[12]),false);
        queries.context=this;
        queries.internal_id_player=[](void* c,const a::Registry* r,int key,unsigned flag,a::Player** out){auto& v=f(c);if(r!=&v.registry||key!=v.row[0])return 1;int rc=v.note(0,key,flag);if(rc)return rc;*out=flag?&v.fresh_player:&v.player;return 0;};
        queries.online=[](void* c,std::uint8_t* out){auto& v=f(c);*out=static_cast<std::uint8_t>(v.row[v.online_reads++?4:3]);return v.note(7,*out);};
        queries.player_virtual_is_local=[](void* c,a::Player* p,int* out){auto& v=f(c);if(p!=&v.player)return 1;*out=(v.row[5]>>v.local_reads++)&1;return v.note(4,*out);};
        services.context=this;services.queries=&queries;
        services.record=[](void* c,a::Player* p,a::Record** out){auto& v=f(c);*out=p==&v.player?&v.record:p==&v.fresh_player?&v.fresh:p==&v.hosting?&v.host:nullptr;return *out?0:1;};
        services.spawn=[](void* c,const char* type,const char* name,bool one,bool two,std::uintptr_t* out){auto& v=f(c);char expected[64];std::snprintf(expected,sizeof(expected),"PlayerCharacter_%d",static_cast<int>(v.row[0]));if(std::strcmp(type,"Character")||std::strcmp(name,expected)||!one||!two)return 1;int rc=v.note(1,v.row[0],3);if(rc)return rc;*out=0x10012000;return 0;};
        services.resolve_character=[](void* c,std::uintptr_t h,std::uintptr_t* out){auto& v=f(c);if(h!=0x10012000)return 1;*out=v.row[7]?CHARACTER:0;return v.note(2,*out!=0);};
        services.initialize_save=[](void* c,std::uintptr_t p){if(p!=CHARACTER)return 1;return f(c).note(3);};
        services.save_slot=[](void* c,std::uintptr_t p,unsigned slot){if(p!=CHARACTER)return 1;return f(c).note(5,signed_word(slot));};
        services.save_class=[](void* c,std::uintptr_t p,int cl){if(p!=CHARACTER)return 1;return f(c).note(6,cl);};
        services.character_fields=[](void* c,std::uintptr_t p,a::CharacterFields* out){auto& v=f(c);if(p==CHARACTER)*out={p,&v.member,&v.id,{&v.position[0],&v.position[1],&v.position[2]},&v.room,&v.no_room};else if(p==HOST_CHARACTER)*out={p,nullptr,nullptr,{&v.host_position[0],&v.host_position[1],&v.host_position[2]},&v.room,nullptr};else return 1;return 0;};
        services.is_host=[](void* c,a::Player* p,int* out){auto& v=f(c);if(p!=&v.player)return 1;*out=static_cast<int>(v.row[16]);return v.note(8,*out);};
        services.hosting_player=[](void* c,const a::Registry* r,a::Player** out){auto& v=f(c);if(r!=&v.registry)return 1;*out=&v.hosting;return v.note(9);};
        services.set_position=[](void* c,std::uintptr_t d,std::uintptr_t s,bool b){if(d!=CHARACTER||s!=HOST_CHARACTER||!b)return 1;return f(c).note(10,1);};
        services.set_rotation=[](void* c,std::uintptr_t d,std::uintptr_t s){if(d!=CHARACTER||s!=HOST_CHARACTER)return 1;return f(c).note(11);};
        services.set_initial_position=[](void* c,std::uintptr_t d,std::uintptr_t s){if(d!=CHARACTER||s!=HOST_CHARACTER)return 1;return f(c).note(12);};
        services.room_add=[](void* c,std::uintptr_t room,std::uintptr_t p,int* out){auto& v=f(c);if(room!=ROOM||p!=CHARACTER)return 1;*out=static_cast<int>(v.row[19]);return v.note(13,*out);};
        services.add_no_room=[](void* c,std::uintptr_t p){if(p!=CHARACTER)return 1;return f(c).note(14);};
        services.zone_entered=[](void* c,std::uintptr_t p){if(p!=CHARACTER)return 1;return f(c).note(15);};
        services.init_all=[](void* c,std::uintptr_t p){if(p!=CHARACTER)return 1;return f(c).note(16);};
        services.is_active=[](void* c,a::Player* p,int* out){auto& v=f(c);if(p!=&v.player)return 1;*out=static_cast<int>(v.row[6]);return v.note(17,*out);};
        services.init_camera=[](void* c,std::uintptr_t p){if(p!=CHARACTER)return 1;return f(c).note(18);};
        services.set_idle=[](void* c,std::uintptr_t p,bool b){if(p!=CHARACTER||b)return 1;return f(c).note(19);};
        services.save_skill_slot=[](void* c,std::uintptr_t p,int slot,unsigned skill){if(p!=CHARACTER)return 1;return f(c).note(21,slot,signed_word(skill));};
        services.save_skill_count=[](void* c,std::uintptr_t p,bool* present,unsigned* count){auto& v=f(c);if(p!=CHARACTER)return 1;*present=v.row[9]!=0;*count=static_cast<unsigned>(v.row[10]+v.count_reads++*v.row[24]);return 0;};
        services.save_skill_level=[](void* c,std::uintptr_t p,unsigned skill,int level){if(p!=CHARACTER)return 1;return f(c).note(22,skill,level);};
        services.set_state=[](void* c,std::uintptr_t p,std::uint8_t state){auto& v=f(c);if(p!=CHARACTER)return 1;int rc=v.note(23,state);if(rc)return rc;v.state=state;return 0;};
        services.current_level=[](void* c,std::uintptr_t* out){auto& v=f(c);*out=v.row[8]?LEVEL:0;return v.note(24,*out!=0);};
        services.quick_save=[](void* c,std::uintptr_t l,bool b){if(l!=LEVEL||b)return 1;return f(c).note(25);};
        services.remove_character=[](void* c,const a::Registry* r,std::uintptr_t p){auto& v=f(c);if(r!=&v.registry||p!=CHARACTER)return 1;return v.note(28);};
        services.attach_controller=[](void* c,const a::Registry* r,int id){auto& v=f(c);if(r!=&v.registry)return 1;return v.note(26,id);};
        services.attach_light=[](void* c,const a::Registry* r,int id){auto& v=f(c);if(r!=&v.registry)return 1;return v.note(27,id);};
        services.debug_mode=[](void* c,int* out){auto& v=f(c);*out=static_cast<int>(v.row[23]);return v.note(29,*out);};
        services.source_assertion=[](void* c,a::Assertion which){return f(c).note(30,which==a::Assertion::spawn_failed?0:1);};
    }
    a::Result run(){a::Runtime runtime(registry,count,services);a::Result out;a::StackResidues stack;
        stack.skill_slots.fill(0xf9);stack.skill_levels.fill(0xfa);
        auto status=runtime.add_character(static_cast<int>(row[0]),stack,&out);
        if(status!=out.status)throw std::runtime_error("result status");
        return out;}
    void print(){std::cout<<"{\"character\":"<<(record.character_660?1:0)<<",\"member\":"<<member<<",\"id\":"<<id<<",\"position\":["<<position[0]<<','<<position[1]<<','<<position[2]<<"],\"no_room\":"<<unsigned(no_room)<<",\"state\":"<<unsigned(state)<<",\"count\":"<<count<<",\"trace\":[";
        bool comma=false;for(auto e:trace){if(comma)std::cout<<',';comma=true;std::cout<<'['<<e[0]<<','<<e[1]<<','<<e[2]<<']';}std::cout<<"]}";}
};
unsigned failure_checks() {
    Row row{{7,2,0,1,1,15,1,1,1,1,10,3,30,7,1,0,0,1,1,0,0,0,-1,0,0}};
    Fixture baseline(row);auto complete=baseline.run();if(complete.status!=a::Status::complete)throw std::runtime_error("baseline");
    unsigned checks=0;int deliveries=baseline.attempts;
    auto event_index=[&](int op,int occurrence=0){for(unsigned i=0;i<baseline.trace.size();++i)if(baseline.trace[i][0]==op&&occurrence--==0)return static_cast<int>(i);throw std::runtime_error("missing baseline event");};
    for(bool throwing:{false,true})for(int at=0;at<deliveries;++at) {
        Fixture f(row);if(throwing)f.throw_at=at;else f.fail_at=at;auto out=f.run();
        if(out.status!=a::Status::service_failed || f.attempts!=at+1 ||
           f.trace!=std::vector<Event>(baseline.trace.begin(),baseline.trace.begin()+at+1))throw std::runtime_error("failure prefix");
        if((f.record.character_660!=0)!=(at>=event_index(3)) ||
           (f.member==17)!=(at>=event_index(7)) || (f.id==-42)!=(at>=event_index(7)) ||
           (f.position[0]==901)!=(at>=event_index(13)) ||
           (f.no_room==1)!=(at>=event_index(15)) ||
           (f.state==1)!=(at>event_index(23)) ||
           (f.count==1)!=(at>=event_index(7,1)))throw std::runtime_error("reached stores retained");
        ++checks;
    }
    {Fixture f(row);f.services.init_all=nullptr;auto out=f.run();if(out.status!=a::Status::service_unavailable||f.record.character_660!=CHARACTER||out.last_operation!=a::Operation::init_all||f.count)throw std::runtime_error("missing InitAll prefix");++checks;}
    {Fixture f(row);f.services.initialize_save=nullptr;auto out=f.run();if(out.status!=a::Status::service_unavailable||f.record.character_660!=CHARACTER||out.association_writes!=1)throw std::runtime_error("source660 before Save delivery");++checks;}
    {Fixture f(row);a::Runtime runtime(f.registry,f.count,f.services);f.reenter=&runtime;a::Result out;runtime.add_character(7,{},&out);if(f.nested!=a::Status::reentrant||out.status!=a::Status::complete)throw std::runtime_error("reentry");++checks;}
    {Fixture f(row);f.registry.entries=nullptr;auto out=f.run();if(out.status!=a::Status::invalid_registry||f.attempts)throw std::runtime_error("invalid registry");++checks;}
    {Fixture f(row);auto* m=f.record.at(0x3d8);m->bytes.size=4;auto out=f.run();m->bytes.size=3;if(out.status!=a::Status::unsafe_buffer||out.last_operation!=a::Operation::get_slots||f.count)throw std::runtime_error("overlong slots");++checks;}
    {Fixture f(row);f.services.record=nullptr;auto out=f.run();if(out.status!=a::Status::service_unavailable||f.record.character_660)throw std::runtime_error("missing canonical record");++checks;}
    {Fixture f(row);a::Runtime runtime(f.registry,f.count,f.services);auto* alias=reinterpret_cast<a::Result*>(&f.record);
        auto status=runtime.add_character(7,{},alias);if(status!=a::Status::invalid_argument||!f.record.constructed||f.record.character_660||f.attempts!=1)throw std::runtime_error("record output alias");++checks;}
    {Fixture f(row);a::Runtime runtime(f.registry,f.count,f.services);auto* alias=reinterpret_cast<a::Result*>(&f.registry);
        auto status=runtime.add_character(7,{},alias);if(status!=a::Status::invalid_argument||f.registry.manager_identity!=MANAGER||f.attempts)throw std::runtime_error("registry output alias");++checks;}
    {Fixture f(row);f.services.character_fields=[](void* c,std::uintptr_t p,a::CharacterFields* out){auto& v=Fixture::f(c);*out={p,&v.member,&v.member,{},{},nullptr};return 0;};
        a::Runtime runtime(f.registry,f.count,f.services);a::Result out;auto status=runtime.add_character(7,{},&out);if(status!=a::Status::invalid_argument||f.member!=-99||f.record.character_660!=CHARACTER)throw std::runtime_error("Character word alias");++checks;}
    {Fixture f(row);f.player.character_level_member=&f.fresh.at(0x310)->header;auto out=f.run();if(out.status!=a::Status::missing_projection||f.record.character_660)throw std::runtime_error("canonical projection mismatch");++checks;}
    return checks;
}
int main(int argc,char** argv) {
    try {if(argc!=2)return 2;std::ifstream input(argv[1]);Row row;bool comma=false;
        std::cout<<"{\"results\":[";while(input>>row[0]){for(unsigned i=1;i<row.size();++i)input>>row[i];Fixture f(row);auto result=f.run();
            if(result.status!=a::Status::complete&&result.status!=a::Status::source_fault)throw std::runtime_error("unexpected caller status");
            if(comma)std::cout<<',';
            comma=true;f.print();}
        std::cout<<"],\"failure_checks\":"<<failure_checks()<<"}\n";
    } catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
