#include "player_local_selection_v1.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dh2::player_local_selection_v1;
namespace {
using Row=std::array<std::int64_t,21>;
using Trace=std::vector<std::array<std::int64_t,3>>;
unsigned checks=0;
void require(bool b,const char* s) {++checks;if(!b)throw std::runtime_error(s);}
struct Fixture {
    Row row;
    std::array<dh2::character_level_member::IntMember,4> levels{};
    std::array<PlayerInfo,4> players{};
    std::array<PlayerInfo*,3> entries{};
    Registry registry{};
    std::array<std::uint8_t,4> controlled{};
    std::array<std::uintptr_t,4> characters{};
    std::array<std::int32_t,4> internal_words{},slots{{-1,-1,-1,-1}};
    std::array<std::int32_t,3> ids{{2,7,19}},second_ids{{7,19,2}};
    const std::int32_t* span=ids.data();std::uint32_t span_count=3;
    std::int32_t last_slot=11;
    SavegameManager save{0x1001f000,&last_slot};
    std::uintptr_t application=0x1001d000;
    std::uint8_t active=0;
    dh2::player_locality_v1::Matching matching{0x10008000,&active};
    dh2::player_locality_v1::Services queries{};
    dh2::player_manager_host_level::Services host{};
    Services services{};Trace trace;
    unsigned online_calls=0,internal_calls=0,attempts=0;
    int fail_at=-1,throw_at=-1;
    explicit Fixture(const Row& input):row(input) {
        players[0]={0x10001008,-1,&levels[0]};internal_words[0]=-1;
        for(unsigned i=0;i<3;++i) {
            players[i+1]={0x10003000+i*0x1000,ids[i],&levels[i+1]};entries[i]=&players[i+1];
            controlled[i+1]=static_cast<std::uint8_t>(row[7+i]);
            characters[i+1]=static_cast<std::uintptr_t>(row[10+i]);
            internal_words[i+1]=static_cast<std::int32_t>(row[13+i]);
        }
        registry={0x10001000,entries.data(),static_cast<std::uint32_t>(row[18]),&players[0]};
        queries.context=this;
        queries.online=[](void* c,std::uint8_t* v) {auto& f=*static_cast<Fixture*>(c);*v=static_cast<std::uint8_t>(f.row[3]);if(f.online_calls&&f.row[19])*v=f.row[19]==1?0:1;++f.online_calls;return f.note(Operation::online,0,*v);};
        queries.game_state_online=[](void* c,std::uint8_t* v) {auto& f=*static_cast<Fixture*>(c);*v=static_cast<std::uint8_t>(f.row[4]);return f.note(Operation::game_state_online,0,*v);};
        queries.acquire_matching=[](void* c,dh2::player_locality_v1::Matching** v) {auto& f=*static_cast<Fixture*>(c);*v=&f.matching;return f.note(Operation::acquire_matching,0,(*v)->identity);};
        queries.matching_in_room=[](void* c,dh2::player_locality_v1::Matching* m,std::int32_t* v) {auto& f=*static_cast<Fixture*>(c);*v=static_cast<std::int32_t>(f.row[5]);return f.note(Operation::matching_in_room,m->identity,*v);};
        queries.acquire_net_manager=[](void* c,std::uintptr_t* v) {auto& f=*static_cast<Fixture*>(c);*v=0x10018000;return f.note(Operation::acquire_net_manager,0,*v);};
        queries.net_initialized=[](void* c,std::uintptr_t n,std::int32_t* v) {auto& f=*static_cast<Fixture*>(c);*v=static_cast<std::int32_t>(f.row[6]);return f.note(Operation::net_initialized,n,*v);};
        queries.character_660=[](void* c,PlayerInfo* p,std::uintptr_t* v) {auto& f=*static_cast<Fixture*>(c);*v=f.characters[f.index(p)];return f.note(Operation::character_660,p->identity,*v);};
        // The real shared public selector runs; this provider does not re-create
        // the lookup algorithm or own alternative PlayerInfo records.
        queries.internal_id_player=[](void* c,const Registry* r,std::int32_t id,std::uint32_t flag,PlayerInfo** v) {
            auto& f=*static_cast<Fixture*>(c);auto status=f.note(Operation::internal_id_player,id,flag);if(status)return status;
            dh2::player_manager_host_level::Result result{};
            auto selected=dh2::player_manager_host_level::get_player_by_internal_id(r,&f.host,id,flag,v,&result);
            if(++f.internal_calls==1&&f.row[17]) {
                if(f.row[17]==1)f.ids[0]=19;
                else if(f.row[17]==2)f.span=f.second_ids.data();
                else f.span_count=0;
            }
            return selected==dh2::player_manager_host_level::Status::complete?0:1;
        };
        host.context=this;host.read_online_byte_5=queries.online;host.read_online_game_state_byte_24=queries.game_state_online;
        // Historical host-level name denotes the source virtual+64 IsInRoom.
        host.matching_is_host=[](void* c,std::int32_t* v) {auto& f=*static_cast<Fixture*>(c);dh2::player_locality_v1::Matching* m=nullptr;
            if(f.queries.acquire_matching(c,&m))return 1;
            return f.queries.matching_in_room(c,m,v);};
        host.get_net_player_manager=queries.acquire_net_manager;host.net_player_manager_is_initialized=queries.net_initialized;
        host.get_net_player_info=[](void* c,std::uintptr_t,std::int32_t id,std::uint32_t flag,PlayerInfo** v) {auto& f=*static_cast<Fixture*>(c);*v=f.key_player(id);return f.note_raw(16,id,flag);};
        services.queries=&queries;services.context=this;
        services.local_controller_66c=[](void* c,PlayerInfo* p,std::uint8_t* v) {auto& f=*static_cast<Fixture*>(c);*v=f.controlled[f.index(p)];return f.note(Operation::local_controller_66c,p->identity,*v);};
        services.internal_id_670=[](void* c,PlayerInfo* p,std::int32_t* v) {auto& f=*static_cast<Fixture*>(c);*v=f.internal_words[f.index(p)];return f.note(Operation::internal_id_670,p->identity,*v);};
        services.local_ids_6b4=[](void* c,const Registry* r,const std::int32_t** v,std::uint32_t* n) {auto& f=*static_cast<Fixture*>(c);if(r!=&f.registry)throw std::runtime_error("foreign manager");*v=f.span;*n=f.span_count;return f.note(Operation::local_ids_6b4,0,*n);};
        services.application=[](void* c,std::uintptr_t* v) {auto& f=*static_cast<Fixture*>(c);*v=f.application;return f.note(Operation::application,0,*v);};
        services.savegame_manager_4c=[](void* c,std::uintptr_t a,SavegameManager** v) {auto& f=*static_cast<Fixture*>(c);require(a==0x1001d000,"captured Application for save field");*v=&f.save;auto status=f.note(Operation::savegame_manager_4c,a,(*v)->identity);if(f.row[20])f.application=0x1001e000;return status;};
        services.player_manager_40=[](void* c,std::uintptr_t a,const Registry** v) {auto& f=*static_cast<Fixture*>(c);require(a==0x1001d000,"captured Application retained after global changes");require(f.last_slot==f.row[16],"first slot store precedes manager read");*v=&f.registry;return f.note(Operation::player_manager_40,a,(*v)->manager_identity);};
        services.save_slot_664=[](void* c,PlayerInfo* p,std::int32_t** v) {auto& f=*static_cast<Fixture*>(c);*v=&f.slots[f.index(p)];return f.note(Operation::save_slot_664,p->identity,0);};
    }
    unsigned index(PlayerInfo* p) {for(unsigned i=0;i<4;++i)if(p==&players[i])return i;throw std::runtime_error("foreign player");}
    PlayerInfo* key_player(std::int32_t id) {for(unsigned i=1;i<4;++i)if(players[i].internal_id==id)return &players[i];return &players[0];}
    int note_raw(std::int64_t op,std::int64_t a,std::int64_t b) {
        trace.push_back({op,a,b});auto current=static_cast<int>(attempts++);
        if(current==throw_at)throw std::runtime_error("declared provider failure");
        return current==fail_at?1:0;
    }
    int note(Operation op,std::int64_t a,std::int64_t b) {return note_raw(static_cast<std::int64_t>(op),a,b);}
    Result run() {
        Result result;Status status;
        if(row[0]==0)status=get_internal_id_by_local_id(&registry,&services,static_cast<std::int32_t>(row[1]),static_cast<std::uint32_t>(row[2]),&result);
        else if(row[0]==1)status=get_local_player(&registry,&services,static_cast<std::int32_t>(row[1]),static_cast<std::uint32_t>(row[2]),&result);
        else if(row[0]==2)status=assign_save_slot_to_player(&services,static_cast<std::int32_t>(row[16]),static_cast<std::int32_t>(row[1]),&result);
        else {
            result.internal_id=static_cast<std::int32_t>(row[1]);
            auto delivered=queries.internal_id_player(this,&registry,result.internal_id,static_cast<std::uint32_t>(row[2]),&result.player);
            status=delivered?Status::service_failed:Status::complete;result.status=status;
        }
        if(status==Status::invalid_argument||status==Status::invalid_registry)result.status=status;
        require(status==result.status,"reported status");return result;
    }
};
Row base(unsigned mode=0) {return Row{{mode,0,0,0,1,1,1,1,0,1,0,22,44,2,7,19,5,0,3,0,0}};}
void failures(unsigned& failure_cases,unsigned& guards) {
    for(unsigned scenario=0;scenario<7;++scenario) {
        auto row=base(scenario<3?scenario:scenario<5?scenario-3:2);
        if(scenario>=3)row[3]=1;
        if(scenario==4)row[2]=1;
        if(scenario==6)row[19]=1;
        Fixture baseline(row);auto good=baseline.run();require(good.status==Status::complete,"baseline");
        for(unsigned i=0;i<baseline.attempts;++i)for(bool throwing:{false,true}) {
            Fixture f(row);if(throwing)f.throw_at=static_cast<int>(i);else f.fail_at=static_cast<int>(i);
            auto r=f.run();require(r.status==Status::service_failed,"failure explicit");
            require(f.trace.size()==i+1&&std::equal(f.trace.begin(),f.trace.end(),baseline.trace.begin()),"exact reached service prefix");
            if(row[0]==2) {
                require(f.last_slot==(i>=2?row[16]:11),"first store retained at later failure");
                require(r.last_slot_writes==(i>=2?1u:0u)&&r.player_slot_writes==0,"reached write receipt");
                require(f.slots==std::array<std::int32_t,4>{{-1,-1,-1,-1}},"no invented second store");
            }
            ++failure_cases;
        }
    }
    {Fixture f(base(1));f.services.internal_id_670=nullptr;auto r=f.run();require(r.status==Status::service_unavailable&&f.trace.size()==2,"missing ID field preserves reads");++guards;}
    {Fixture f(base(2));f.services.player_manager_40=nullptr;auto r=f.run();require(r.status==Status::service_unavailable&&f.last_slot==5&&f.slots[0]==-1,"mandatory manager after first store");++guards;}
    {Fixture f(base(2));f.services.queries=nullptr;auto r=f.run();require(r.status==Status::service_unavailable&&f.last_slot==5&&f.trace.size()==3,"query graph absent after first store");++guards;}
    {Fixture f(base(2));f.services.save_slot_664=nullptr;auto r=f.run();require(r.status==Status::service_unavailable&&r.player==&f.players[1]&&f.last_slot==5&&r.player_slot_writes==0,"missing second storage preserves selected prefix");++guards;}
    {Fixture f(base());f.registry.entries=nullptr;Result r;r.internal_id=777;require(get_internal_id_by_local_id(&f.registry,&f.services,0,0,&r)==Status::invalid_registry&&r.internal_id==777&&f.trace.empty(),"invalid registry no output");++guards;}
    {Fixture f(base());f.entries[0]=f.entries[1];Result r;require(get_local_player(&f.registry,&f.services,0,0,&r)==Status::invalid_registry&&f.trace.empty(),"canonical ordering guard");++guards;}
    {Fixture f(base());auto* r=reinterpret_cast<Result*>(&f.registry);require(get_local_player(&f.registry,&f.services,0,0,r)==Status::invalid_argument&&f.trace.empty(),"result registry alias");++guards;}
    {Fixture f(base(1));f.row[1]=1;auto r=f.run();require(r.internal_id==19&&r.player==&f.players[3],"local ordinal differs from map key");++guards;}
    {Fixture f(base(1));f.internal_words[1]=88;auto r=f.run();require(r.internal_id==88&&r.player==&f.players[0],"actual word distinct from key, source fallback");++guards;}
    {Fixture f(base(2));f.row[1]=-1;auto r=f.run();require(r.player==&f.players[0]&&f.slots[0]==5&&!f.characters[0],"assignment fallback does not register");++guards;}
    {Fixture f(base(2));f.row[20]=1;auto r=f.run();require(r.application==0x1001d000&&f.application==0x1001e000&&f.last_slot==5&&f.slots[1]==5,"captured owner survives global changes");++guards;}
    {Fixture f(base());f.row[3]=1;f.span_count=4097;auto r=f.run();require(r.status==Status::missing_projection&&r.last_operation==Operation::local_ids_6b4,"network span bound after reached providers");++guards;}
    {Fixture f(base());f.row[3]=1;f.span=nullptr;auto r=f.run();require(r.status==Status::missing_projection,"nonempty missing span");++guards;}
    {Fixture f(base(2));f.save.last_slot_8=nullptr;auto r=f.run();require(r.status==Status::missing_projection&&f.last_slot==11&&f.trace.size()==2,"missing true SavegameManager word");++guards;}
    {Fixture f(base(2));f.services.save_slot_664=[](void* c,PlayerInfo*,std::int32_t** v){auto& x=*static_cast<Fixture*>(c);*v=x.save.last_slot_8;return 0;};auto r=f.run();require(r.status==Status::invalid_argument&&f.last_slot==5&&r.player_slot_writes==0,"distinct original slot owners");++guards;}
    {Fixture f(base());PlayerInfo* selected=&f.players[2];dh2::player_manager_host_level::Result r;f.host.read_online_byte_5=nullptr;require(dh2::player_manager_host_level::get_player_by_internal_id(&f.registry,&f.host,2,0,&selected,&r)==dh2::player_manager_host_level::Status::service_unavailable&&selected==&f.players[2],"public pointer unchanged on missing source service");++guards;}
    {Fixture f(base());PlayerInfo* selected=nullptr;dh2::player_manager_host_level::Result r;f.levels[1].value=919;require(dh2::player_manager_host_level::get_player_by_internal_id(&f.registry,&f.host,2,0,&selected,&r)==dh2::player_manager_host_level::Status::complete&&selected==&f.players[1]&&r.character_level_330==0,"pointer query does not read Level");++guards;}
    {Fixture f(base());PlayerInfo* selected=nullptr;dh2::player_manager_host_level::Result r;require(dh2::player_manager_host_level::get_player_by_internal_id(&f.registry,nullptr,-1,7,&selected,&r)==dh2::player_manager_host_level::Status::complete&&selected==&f.players[0]&&r.service_calls==0,"source minus-one branch needs no services");++guards;}
}
void print_trace(const Trace& trace) {std::cout<<'[';bool comma=false;for(const auto& x:trace){if(comma)std::cout<<',';comma=true;std::cout<<'['<<x[0]<<','<<x[1]<<','<<x[2]<<']';}std::cout<<']';}
}
int main(int argc,char** argv) {
    try {
        if(argc!=2)throw std::runtime_error("input rows required");
        std::ifstream input(argv[1]);if(!input)throw std::runtime_error("input open");
        std::vector<Row> rows;Row row{};while(input>>row[0]){for(unsigned i=1;i<row.size();++i)if(!(input>>row[i]))throw std::runtime_error("partial row");rows.push_back(row);}
        unsigned failure_cases=0,guards=0;failures(failure_cases,guards);
        std::cout<<"{\"validation\":\"PASS\",\"failure_prefix_cases\":"<<failure_cases<<",\"guard_cases\":"<<guards<<",\"results\":[";
        bool comma=false;for(const auto& x:rows){Fixture f(x);auto r=f.run();require(r.status==Status::complete,"comparison complete");if(comma)std::cout<<',';comma=true;
            std::cout<<"{\"id\":"<<r.internal_id<<",\"player\":"<<(r.player?r.player->identity:0)<<",\"last_slot\":"<<f.last_slot<<",\"slots\":[";
            for(unsigned i=0;i<4;++i){if(i)std::cout<<',';std::cout<<f.slots[i];}
            std::cout<<"],\"last_writes\":"<<r.last_slot_writes<<",\"player_writes\":"<<r.player_slot_writes<<",\"trace\":";print_trace(f.trace);std::cout<<'}';}
        std::cout<<"],\"checks\":"<<checks<<",\"native_registration\":false}"<<std::endl;return 0;
    } catch(const std::exception& e) {std::cerr<<e.what()<<std::endl;return 1;}
}
