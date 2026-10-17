#include "player_locality_v1.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

using namespace dh2::player_locality_v1;
namespace {
using Row=std::array<std::int64_t,19>;
using Trace=std::vector<std::array<std::int64_t,3>>;
constexpr auto absent=std::numeric_limits<std::int32_t>::min();
unsigned checks=0;
void require(bool b,const char* s) {++checks;if(!b)throw std::runtime_error(s);}
struct Fixture {
    Row row;
    std::array<dh2::character_level_member::IntMember,4> levels{};
    std::array<PlayerInfo,4> players{};
    std::array<PlayerInfo*,3> entries{};
    Registry registry{};
    std::array<std::uintptr_t,4> characters{};
    std::int32_t info_member=0;
    MatchingLocalFields fields{};
    Matching matching{},second{};
    Matching* singleton=nullptr;
    std::int32_t provider=0;
    Services services{};
    Trace trace;
    std::array<std::int32_t,3> ids{{2,7,19}};
    std::uint32_t id_count=3;
    unsigned acquired=0,members=0,attempts=0;
    int fail_at=-1,throw_at=-1,mutate_factory_provider=-1;
    explicit Fixture(const Row& input):row(input) {
        players[0]={0x10001008,-1,&levels[0]};
        for(unsigned i=0;i<3;++i) {players[i+1]={0x10003000+i*0x1000,ids[i],&levels[i+1]};entries[i]=&players[i+1];}
        registry={0x10001000,entries.data(),3,&players[0]};
        if(row[18]==3)registry.entry_count=0;
        for(unsigned i=0;i<3;++i)characters[i+1]=static_cast<std::uintptr_t>(row[7+i]);
        info_member=static_cast<std::int32_t>(row[10]);
        fields.active_c=static_cast<std::uint8_t>(row[11]);
        matching={0x10008000,&fields.active_c};second={0x10010000,&fields.active_c};
        services.context=this;
        services.online=[](void* c,std::uint8_t* v) {auto& f=*static_cast<Fixture*>(c);*v=static_cast<std::uint8_t>(f.row[3]);return f.note(Operation::online,0,*v);};
        services.game_state_online=[](void* c,std::uint8_t* v) {auto& f=*static_cast<Fixture*>(c);*v=static_cast<std::uint8_t>(f.row[4]);return f.note(Operation::game_state_online,0,*v);};
        services.acquire_matching=[](void* c,Matching** v) {auto& f=*static_cast<Fixture*>(c);*v=f.acquired++?&f.second:&f.matching;
            if(f.acquired==2 && f.row[17]!=absent)f.info_member=static_cast<std::int32_t>(f.row[17]);
            return f.note(Operation::acquire_matching,0,(*v)->identity);};
        services.matching_in_room=[](void* c,Matching* m,std::int32_t* v) {auto& f=*static_cast<Fixture*>(c);*v=static_cast<std::int32_t>(f.row[5]);return f.note(Operation::matching_in_room,m->identity,*v);};
        services.acquire_net_manager=[](void* c,std::uintptr_t* v) {auto& f=*static_cast<Fixture*>(c);*v=0x10018000;return f.note(Operation::acquire_net_manager,0,*v);};
        services.net_initialized=[](void* c,std::uintptr_t m,std::int32_t* v) {auto& f=*static_cast<Fixture*>(c);*v=static_cast<std::int32_t>(f.row[6]);return f.note(Operation::net_initialized,m,*v);};
        services.net_ids=[](void* c,const Registry*,const std::int32_t** v,std::uint32_t* n) {auto& f=*static_cast<Fixture*>(c);*v=f.ids.data();*n=f.id_count;return f.note(Operation::net_ids,0,*n);};
        services.internal_id_player=[](void* c,const Registry*,std::int32_t id,std::uint32_t flag,PlayerInfo** v) {auto& f=*static_cast<Fixture*>(c);*v=f.player_id(id);auto status=f.note(Operation::internal_id_player,id,flag);
            if(f.row[18]==1)f.ids[0]=19;
            if(f.row[18]==2)f.id_count=0;
            return status;};
        services.net_player_info=[](void* c,const Registry*,std::int32_t id,std::uint32_t flag,PlayerInfo** v) {auto& f=*static_cast<Fixture*>(c);*v=f.player_id(id);return f.note(Operation::net_player_info,id,flag);};
        services.character_660=[](void* c,PlayerInfo* p,std::uintptr_t* v) {auto& f=*static_cast<Fixture*>(c);*v=f.characters[f.index(p)];return f.note(Operation::character_660,p->identity,*v);};
        services.member_1a0=[](void* c,PlayerInfo* p,std::int32_t* v) {auto& f=*static_cast<Fixture*>(c);*v=f.info_member;return f.note(Operation::member_1a0,p->identity,*v);};
        services.matching_member_id=[](void* c,Matching* m,std::int32_t* v) {auto& f=*static_cast<Fixture*>(c);f.fields.member_3638=static_cast<std::int32_t>(m==&f.second?f.row[15]:f.row[f.members++?13:12]);*v=local_member_id(f.fields);return f.note(Operation::matching_member_id,m->identity,*v);};
        services.matching_server_member_id=[](void* c,Matching* m,std::int32_t* v) {auto& f=*static_cast<Fixture*>(c);f.fields.server_member_363c=static_cast<std::int32_t>(f.row[14]);*v=local_server_member_id(f.fields);
            if(f.row[16]!=absent)f.info_member=static_cast<std::int32_t>(f.row[16]);
            return f.note(Operation::matching_server_member_id,m->identity,*v);};
        services.player_virtual_is_local=[](void* c,PlayerInfo* p,std::int32_t* v) {auto& f=*static_cast<Fixture*>(c);
            auto status=f.note(Operation::player_virtual_is_local,p->identity,0);if(status)return status;
            Result r;auto s=cnet_player_is_local(p,&f.services,&r);*v=r.value;return s==Status::complete?0:-1;};
    }
    unsigned index(PlayerInfo* p) {for(unsigned i=0;i<4;++i)if(p==&players[i])return i;throw std::runtime_error("foreign PlayerInfo");}
    PlayerInfo* player_id(std::int32_t id) {for(unsigned i=1;i<4;++i)if(players[i].internal_id==id)return &players[i];return &players[0];}
    int note(Operation op,std::int64_t a,std::int64_t b) {
        trace.push_back({static_cast<std::int64_t>(op),a,b});auto index=static_cast<int>(attempts++);
        if(index==throw_at)throw std::runtime_error("declared provider failure");
        return index==fail_at?-1:0;
    }
    Result run() {
        Result r;Status status;
        if(row[0]==0)status=get_player_by_character(&registry,&services,static_cast<std::uintptr_t>(row[1]),static_cast<std::uint32_t>(row[2]),&r);
        else if(row[0]==1)status=is_local_player(&registry,&services,static_cast<std::uintptr_t>(row[1]),&r);
        else if(row[0]==2)status=cnet_player_is_local(&players[0],&services,&r);
        else status=matching_is_server(&matching,&services,&r);
        require(status==r.status,"reported status");return r;
    }
};
Row base(unsigned mode=2) {return Row{{mode,99,0,0,0,0,0,22,99,44,-1,0,-1,7,7,-1,absent,absent,0}};}
void print_trace(const Trace& t) {std::cout<<'[';bool comma=false;for(auto& r:t){if(comma)std::cout<<',';comma=true;std::cout<<'['<<r[0]<<','<<r[1]<<','<<r[2]<<']';}std::cout<<']';}
void guards_and_failures(unsigned& failure_cases,unsigned& guard_cases,unsigned& factory_cases) {
    for(unsigned scenario=0;scenario<6;++scenario) {
        unsigned mode=scenario<4?scenario:scenario-4;
        auto b=base(mode);b[11]=1;b[12]=0;b[10]=3;
        if(scenario>=4)b[3]=b[4]=b[5]=b[6]=1;
        Fixture normal(b);auto success=normal.run();require(success.status==Status::complete,"baseline complete");
        for(unsigned i=0;i<normal.attempts;++i)for(bool throwing:{false,true}) {
            Fixture f(b);if(throwing)f.throw_at=static_cast<int>(i);else f.fail_at=static_cast<int>(i);
            auto r=f.run();require(r.status==Status::service_failed,"failure explicit");
            require(f.trace.size()==i+1,"failure reached trace prefix");
            require(std::equal(f.trace.begin(),f.trace.end(),normal.trace.begin()),"failure exact prefix");++failure_cases;
        }
    }
    {Fixture f(base(1));f.registry.entry_count=0;auto r=f.run();require(r.status==Status::complete&&r.value==1&&!r.registered&&r.route==Route::manager_plus_8,"constructor fallback local != registered");++guard_cases;}
    {Result r;require(is_local_player(nullptr,nullptr,0,&r)==Status::complete&&r.value==0&&r.service_calls==0,"null source branch needs no services");++guard_cases;}
    {Fixture f(base(0));f.registry.entries=nullptr;Result r;r.value=777;require(get_player_by_character(&f.registry,&f.services,99,0,&r)==Status::invalid_registry&&r.value==777&&f.trace.empty(),"invalid registry unchanged");++guard_cases;}
    {Fixture f(base(0));f.entries[0]=f.entries[1];Result r;require(get_player_by_character(&f.registry,&f.services,99,0,&r)==Status::invalid_registry&&f.trace.empty(),"unordered keys");++guard_cases;}
    {Fixture f(base(0));f.services.character_660=nullptr;auto r=f.run();require(r.status==Status::service_unavailable&&f.trace.size()==1,"missing character provider stops prefix");++guard_cases;}
    {Fixture f(base(1));f.services.player_virtual_is_local=nullptr;auto r=f.run();require(r.status==Status::service_unavailable&&r.registered,"mandatory virtual no fabricated locality");++guard_cases;}
    {Fixture f(base(2));f.services.acquire_matching=nullptr;auto r=f.run();require(r.status==Status::service_unavailable&&f.trace.empty(),"mandatory matching");++guard_cases;}
    {Fixture f(base(3));f.fields.active_c=1;f.services.matching_member_id=nullptr;auto r=f.run();require(r.status==Status::service_unavailable&&f.trace.empty(),"active member provider mandatory");++guard_cases;}
    {Fixture f(base(0));auto* r=reinterpret_cast<Result*>(&f.registry);require(get_player_by_character(&f.registry,&f.services,99,0,r)==Status::invalid_argument&&f.trace.empty(),"borrowed registry alias");++guard_cases;}
    {MatchingLocalFields f{3,8,9};reset_matching_local_ids(f);require(f.active_c==3&&f.member_3638==-1&&f.server_member_363c==-2,"reset IDs keeps active");construct_matching_local_fields(f);require(f.active_c==0&&local_member_id(f)==-1&&local_server_member_id(f)==-2,"ctor reached fields");++guard_cases;}
    for(std::int32_t provider:{0,1,2,3,4,9}) {
        Fixture f(base());f.provider=provider;MatchingState s{&f.singleton,&f.provider};MatchingFactory factory{&f,[](void* c,MatchingKind kind,std::uint32_t bytes,std::uint32_t mode,Matching** out){auto& f=*static_cast<Fixture*>(c);f.trace.push_back({static_cast<std::int64_t>(kind),bytes,mode});*out=&f.matching;return 0;}};MatchingResult r;
        require(get_matching(&s,&factory,&r)==Status::complete,"factory source branch");
        require(r.constructor_calls==(provider==9?0u:1u),"factory call count");require(f.provider==(provider==0?1:provider),"provider prefix");
        if(provider!=9)require(r.matching==&f.matching,"factory same identity");
        ++factory_cases;
        MatchingResult again;auto old=f.trace.size();require(get_matching(&s,&factory,&again)==Status::complete&&f.trace.size()==old,"singleton retained no recreate");
    }
    {Fixture f(base());MatchingState s{&f.singleton,&f.provider};MatchingResult r;require(get_matching(&s,nullptr,&r)==Status::service_unavailable&&f.provider==1&&!f.singleton,"missing constructor retains provider prefix");++factory_cases;}
    {Fixture f(base());f.singleton=&f.matching;MatchingState s{&f.singleton,nullptr};MatchingResult r;require(get_matching(&s,nullptr,&r)==Status::complete&&r.matching==&f.matching&&r.constructor_calls==0,"existing singleton requires no provider or factory");++factory_cases;}
    {Fixture f(base());MatchingState s{&f.singleton,&f.provider};MatchingFactory factory{&f,[](void* c,MatchingKind kind,std::uint32_t,std::uint32_t,Matching** out){auto& f=*static_cast<Fixture*>(c);*out=&f.matching;f.provider=kind==MatchingKind::local?2:kind==MatchingKind::bluetooth?3:4;return 0;}};MatchingResult r;require(get_matching(&s,&factory,&r)==Status::complete&&r.constructor_calls==4,"fresh factory provider chaining");++factory_cases;}
    {Fixture f(base());MatchingState s{&f.singleton,&f.provider};MatchingFactory factory{&f,[](void*,MatchingKind,std::uint32_t,std::uint32_t,Matching**){return -1;}};MatchingResult r;require(get_matching(&s,&factory,&r)==Status::service_failed&&f.provider==1&&!f.singleton&&r.constructor_calls==1,"factory failure keeps source prefix");++factory_cases;}
}
} // namespace
int main(int argc,char** argv) {
    try {
        if(argc!=2)throw std::runtime_error("input rows required");
        std::ifstream input(argv[1]);if(!input)throw std::runtime_error("input open");
        std::vector<Row> rows;Row row{};while(input>>row[0]){for(unsigned i=1;i<row.size();++i)if(!(input>>row[i]))throw std::runtime_error("partial input");rows.push_back(row);}
        unsigned failures=0,guards=0,factories=0;guards_and_failures(failures,guards,factories);
        std::cout<<"{\"validation\":\"PASS\",\"failure_prefix_cases\":"<<failures<<",\"guard_cases\":"<<guards<<",\"factory_cases\":"<<factories<<",\"results\":[";
        bool comma=false;for(auto& r:rows) {
            Fixture f(r);std::int32_t value=0;std::uintptr_t player=0;unsigned route=0;bool registered=false;
            if(r[0]<4) {auto result=f.run();require(result.status==Status::complete,"comparison complete");value=result.value;player=result.player?result.player->identity:0;route=static_cast<unsigned>(result.route);registered=result.registered;}
            else if(r[0]==4) {
                f.provider=static_cast<std::int32_t>(r[1]);if(r[2])f.singleton=&f.matching;MatchingState state{&f.singleton,&f.provider};
                MatchingFactory factory{&f,[](void* c,MatchingKind kind,std::uint32_t bytes,std::uint32_t mode,Matching** out){auto& f=*static_cast<Fixture*>(c);f.trace.push_back({static_cast<std::int64_t>(kind),bytes,mode});*out=&f.matching;
                    if(f.row[3])f.provider=kind==MatchingKind::local?2:kind==MatchingKind::bluetooth?3:4;
                    return 0;}};
                MatchingResult result;require(get_matching(&state,&factory,&result)==Status::complete,"matching comparison");value=f.provider;player=result.matching?result.matching->identity:0;route=result.constructor_calls;
            } else {
                f.fields={static_cast<std::uint8_t>(r[11]),17,33};
                if(r[0]==5)construct_matching_local_fields(f.fields);else reset_matching_local_ids(f.fields);
                f.trace.push_back({f.fields.active_c,f.fields.member_3638,f.fields.server_member_363c});
            }
            if(comma)std::cout<<',';
            comma=true;
            std::cout<<"{\"value\":"<<value<<",\"player\":"<<player<<",\"route\":"<<route<<",\"registered\":"<<(registered?"true":"false")<<",\"trace\":";print_trace(f.trace);std::cout<<'}';}
        std::cout<<"],\"checks\":"<<checks<<",\"native_association\":false}"<<std::endl;return 0;
    } catch(const std::exception& e) {std::cerr<<e.what()<<std::endl;return 1;}
}
