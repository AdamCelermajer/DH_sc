#include "../character_enemy_retention.hpp"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace r=dh2::character_enemy_retention;
namespace t=dh2::target_search;
constexpr std::uintptr_t A=0x10014000,B=0x10018000,AI=A+0x3c8,P=0x10020000,Q=0x10024000,M=0x10030000,N=0x10031000;
std::uint32_t bits(float x) { std::uint32_t w;std::memcpy(&w,&x,4);return w; }
float number(std::uint32_t w) { float x;std::memcpy(&x,&w,4);return x; }
struct Call { unsigned op,kind;std::uintptr_t subject,peer;unsigned word,extra; };
struct Fixture {
    r::Owner a{},b{};r::Object p{},q{},nonchar{};
    dh2::data::AggroEntry entries[3]{};dh2::data::AggroTable map{entries,0,3};
    r::State state{AI,&a};r::AiRow rows[9]{},replacement[9]{};
    r::AiTable table{rows,9};r::PlayerInfo info{P};
    t::Entry16 end{},e[3]{};t::Room16 room_end{},room{};r::Registry registry{&room_end};
    r::Level level{&registry};r::Application app{&level,M};
    r::Point origin{{0,0,0}},pointP{{0,-2,0}},pointQ{{0,-1,0}};
    r::Target heap[8]{};r::List list{};r::Result result{};
    std::vector<Call> calls,search_calls;
    unsigned dead=0,enemy=1,player=1,ownerPlayer=0,mutation=0;
    unsigned mask=3,design=bits(7),fail=0,throwAt=0,callsSeen=0;
    bool failSearch=false;
    r::Services services{this,invoke,{this,searchInvoke}};
    Fixture() {
        a.object.identity=A;b.object.identity=B;p.identity=P;q.identity=Q;nonchar.identity=0x10028000;
        for(auto* o:{&a.object,&b.object,&p,&q,&nonchar}) {o->visible=1;o->character_word1314=10;}
        a.outgoing=&map;b.outgoing=&map;rows[8].words[16]=bits(10);replacement[8].words[16]=bits(1);
        room_end.next=&room;room.next=&room_end;room.objects=&end;
        end.next=&e[0];e[0]={&e[1],&q};e[1]={&end,&p};
    }
    static int invoke(void* context,r::State* state,const r::Request* req,r::Response* out) {
        auto& f=*static_cast<Fixture*>(context); const auto& x=*req;
        f.calls.push_back({unsigned(x.operation),unsigned(x.kind),x.subject,x.peer,x.word,x.extra});
        ++f.callsSeen;
        if(f.throwAt==f.callsSeen)throw std::runtime_error("provider");
        if(f.fail==f.callsSeen)return 7;
        switch(x.operation) {
        case r::Operation::application:out->view=&f.app;break;
        case r::Operation::ai_table:
            out->view=&f.table;if(f.mutation==2)state->owner=&f.b;break;
        case r::Operation::char_ai_id:
            out->word=8;
            if(f.mutation==3) {f.table.rows=f.replacement;f.rows[8].words[16]=bits(4);state->owner=&f.b;}
            break;
        case r::Operation::get_local_player:
            assert(x.word==0 && x.extra==1);out->view=&f.info;
            if(f.mutation==5)state->owner=&f.b;
            if(f.mutation==6)f.info.character_660=Q;
            break;
        case r::Operation::is_enemy:
            out->identity=f.enemy;
            if(f.mutation==7)state->owner=&f.b;
            break;
        case r::Operation::design_30:
            out->word=f.design;if(f.mutation==8)state->owner=&f.b;break;
        case r::Operation::clear_aggro:
            if(f.mutation==9)state->owner=&f.b;
            break;
        case r::Operation::set_target:assert(x.peer==0 && x.word==0);break;
        case r::Operation::sync_last_target:break;
        case r::Operation::add_aggro:break;
        }
        return 0;
    }
    static int searchInvoke(void* context,r::List* list,const r::SearchRequest* req,r::SearchResponse* out) {
        auto& f=*static_cast<Fixture*>(context);const auto& x=*req;
        f.search_calls.push_back({unsigned(x.operation),0,x.subject,x.other,0,0});
        if(f.failSearch)return 4;
        switch(x.operation) {
        case r::SearchOperation::is_character:
            out->identity=1;if(f.mutation==1)f.state.owner=&f.b;break;
        case r::SearchOperation::look_vector:out->point={{0,-1,0}};break;
        case r::SearchOperation::target_position:
            out->view=x.subject==P?&f.pointP:x.subject==Q?&f.pointQ:&f.origin;
            if(x.subject==P && f.mutation==11)f.origin.coordinates[1]=-2;
            if(f.mutation==14)out->view=&f.result;
            break;
        case r::SearchOperation::diagnostic_switch:
            out->identity=0xffffffff;if(f.mutation==4){f.app.player_manager_40=N;f.state.owner=&f.b;}
            break;
        case r::SearchOperation::melee_radius:out->number=0;break;
        case r::SearchOperation::interaction_radius:out->number=0;break;
        case r::SearchOperation::resolve_character:
            out->view=x.subject==P?&f.p:x.subject==Q?&f.q:x.subject==A?&f.a.object:nullptr;break;
        case r::SearchOperation::is_zonable:out->identity=1;break;
        case r::SearchOperation::is_interactive:
            out->identity=x.subject==P?bool(f.mask&1):x.subject==Q?bool(f.mask&2):1;break;
        case r::SearchOperation::is_dead:
            out->identity=f.dead;
            if(f.mutation==12)list->reference_character=&f.b.object;
            break;
        case r::SearchOperation::is_enemy:out->identity=f.enemy;break;
        case r::SearchOperation::is_player:
            out->identity=x.subject==P?f.player:(x.subject==A||x.subject==B)?f.ownerPlayer:0;
            if(x.subject==P && f.mutation==13)list->reference_character=&f.b.object;
            break;
        case r::SearchOperation::angle:
            assert(x.first && x.second);out->number=-0.25f;break;
        }
        return 0;
    }
    r::Status run() {return r::update(&state,Q,heap,8,&services,&result);}
    void known(std::uintptr_t key) {entries[0]={key,0,0};map.count=1;}
};
void dumpCalls(const std::vector<Call>& calls) {
    std::cout<<'[';bool first=true;for(const auto& c:calls) {if(!first)std::cout<<',';first=false;
        std::cout<<'['<<c.op<<','<<c.kind<<','<<c.subject<<','<<c.peer<<','<<c.word<<','<<c.extra<<']';}std::cout<<']';
}
void dumpRetention(Fixture& f,r::Status status) {
    const auto& x=f.result;
    std::cout<<"{\"status\":"<<int(status)<<",\"decision\":"<<unsigned(x.decision)<<",\"owner\":"<<f.state.owner->object.identity
      <<",\"player\":"<<x.player<<",\"known\":"<<x.known_player<<",\"found\":"<<x.found_player
      <<",\"pops\":"<<x.unmatched_pops<<",\"count\":"<<x.search_count<<",\"radius\":"<<x.radius_word
      <<",\"added\":"<<x.added_word<<",\"calls\":";dumpCalls(f.calls);std::cout<<"}\n";
}
int main(int argc,char** argv) {
    if(argc>1) {
        Fixture f;
        if(std::string(argv[1])=="retain") {
            assert(argc==9);f.mask=std::strtoul(argv[2],nullptr,0);
            auto key=std::strtoull(argv[3],nullptr,0);if(key!=UINT64_MAX)f.known(key);
            f.info.character_660=std::strtoull(argv[4],nullptr,0);f.enemy=std::strtoul(argv[5],nullptr,0);
            f.mutation=std::strtoul(argv[6],nullptr,0);f.design=std::strtoul(argv[7],nullptr,0);
            f.rows[8].words[16]=std::strtoul(argv[8],nullptr,0);
            // Retention relation return is independent from search classification.
            const auto saved=f.enemy;f.enemy=1;
            struct Bridge {Fixture* f;unsigned retention;};Bridge bridge{&f,saved};
            f.services.context=&bridge;
            f.services.invoke=[](void* c,r::State* s,const r::Request* q,r::Response* v)->int {
                auto& b=*static_cast<Bridge*>(c);auto rc=Fixture::invoke(b.f,s,q,v);
                if(q->operation==r::Operation::is_enemy)v->identity=b.retention;
                return rc;
            };
            dumpRetention(f,f.run());return 0;
        }
        if(std::string(argv[1])=="gate") {
            assert(argc==9);f.a.object.character_word1314=std::strtoul(argv[2],nullptr,0);
            f.p.character_word1310=std::strtoul(argv[3],nullptr,0);
            f.dead=std::strtoul(argv[4],nullptr,0);f.enemy=std::strtoul(argv[5],nullptr,0);
            f.player=std::strtoul(argv[6],nullptr,0);f.ownerPlayer=std::strtoul(argv[7],nullptr,0);f.mutation=std::strtoul(argv[8],nullptr,0);
            assert(r::init(&f.list,f.heap,8,&f.a.object,&f.services.search)==r::Status::complete);
            f.search_calls.clear();unsigned accepted=999;
            const auto status=r::character_valid(&f.list,&f.p,&f.services.search,&accepted);
            std::cout<<"{\"status\":"<<int(status)<<",\"value\":"<<accepted<<",\"calls\":";
            dumpCalls(f.search_calls);std::cout<<"}\n";return 0;
        }
        return 2;
    }
    unsigned cases=0;
    {Fixture f;assert(f.run()==r::Status::complete && f.result.decision==r::Decision::added_player_aggro && f.result.unmatched_pops==1);++cases;}
    {Fixture f;f.mask=0;assert(f.run()==r::Status::complete && f.result.decision==r::Decision::cleared_empty_search && f.calls.size()==6);++cases;}
    {Fixture f;f.known(P);assert(f.run()==r::Status::complete && f.result.decision==r::Decision::retained_known_player);++cases;}
    {Fixture f;f.known(P);f.mask=2;assert(f.run()==r::Status::complete && f.result.decision==r::Decision::retained_known_player && !f.result.found_player);++cases;}
    {Fixture f;f.mask=2;assert(f.run()==r::Status::complete && f.result.decision==r::Decision::player_not_found);++cases;}
    for(unsigned i=1;i<=9;++i) {Fixture f;f.mutation=i;if(i==9)f.mask=0;assert(f.run()==r::Status::complete);++cases;}
    for(unsigned i=1;i<=7;++i) {Fixture f;f.fail=i;assert(f.run()==r::Status::service_failed && f.callsSeen==i);++cases;}
    {Fixture f;f.throwAt=7;assert(f.run()==r::Status::service_failed && f.result.added_word==bits(7));++cases;}
    {Fixture f;f.failSearch=true;assert(f.run()==r::Status::service_failed && f.calls.empty());++cases;}
    {Fixture f;const auto before=f.state;r::Result* alias=reinterpret_cast<r::Result*>(&f.state);
        assert(r::update(&f.state,Q,f.heap,8,&f.services,alias)==r::Status::invalid_argument && f.calls.empty() && f.state.ai==before.ai);++cases;}
    {Fixture f;assert(r::update(&f.state,Q,f.heap,0,&f.services,&f.result)==r::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;f.map.count=2;f.entries[0]={P,0,0};f.entries[1]={P,0,0};assert(f.run()==r::Status::invalid_source_fact);++cases;}
    {Fixture f;f.info.character_660=0;assert(f.run()==r::Status::complete && f.result.decision==r::Decision::player_not_found);++cases;}
    {Fixture f;f.mutation=11;assert(f.run()==r::Status::complete && f.result.unmatched_pops==0);++cases;}
    {Fixture f;f.e[1].object=&f.nonchar;assert(f.run()==r::Status::complete && f.result.search_count==1 && !f.result.found_player);++cases;}
    {Fixture f;f.p.zoned=1;assert(f.run()==r::Status::complete && f.result.search_count==1);++cases;}
    {Fixture f;f.p.visible=0;assert(f.run()==r::Status::complete && f.result.search_count==1);++cases;}
    {Fixture f;f.mutation=12;assert(f.run()==r::Status::complete);bool fresh=false;for(auto c:f.search_calls)if(c.op==9 && c.subject==B)fresh=true;assert(fresh);++cases;}
    {Fixture f;f.mutation=13;assert(f.run()==r::Status::complete);bool fresh=false;for(auto c:f.search_calls)if(c.op==10 && c.subject==B)fresh=true;assert(fresh);++cases;}
    {Fixture f;f.ownerPlayer=1;assert(f.run()==r::Status::complete && f.result.search_count==1);++cases;}
    {Fixture f;f.dead=1;assert(f.run()==r::Status::complete && f.result.search_count==0);++cases;}
    {Fixture f;f.rows[8].words[16]=0;assert(f.run()==r::Status::complete && f.result.search_count==0);++cases;}
    {Fixture f;f.pointP.coordinates[1]=NAN;assert(f.run()==r::Status::complete && f.result.search_count==2);++cases;}
    {Fixture f;f.pointP.coordinates[1]=-10;assert(f.run()==r::Status::complete && f.result.search_count==2);++cases;}
    {Fixture f;f.entries[0]={P,0,0};f.entries[1]={UINT64_C(0xffffffffffffffff),0,0};f.map.count=2;assert(f.run()==r::Status::complete && f.result.known_player);++cases;}
    {Fixture f;assert(r::init(&f.list,f.heap,1,&f.a.object,&f.services.search)==r::Status::complete);assert(r::search(&f.list,&f.registry,10,&f.services.search)==r::Status::capacity_exhausted && f.list.count==1);++cases;}
    {Fixture f;f.mutation=14;assert(f.run()==r::Status::invalid_source_fact && f.result.radius_word==bits(10));++cases;}
    {Fixture f;f.map.entries=reinterpret_cast<dh2::data::AggroEntry*>(&f.result);f.map.count=1;assert(f.run()==r::Status::invalid_source_fact);++cases;}
    {Fixture f;assert(r::init(&f.list,f.heap,8,&f.a.object,&f.services.search)==r::Status::complete);f.list.reference_character=nullptr;f.dead=1;unsigned accepted=0;
        assert(r::character_valid(&f.list,&f.p,&f.services.search,&accepted)==r::Status::complete && accepted==1);++cases;}
    {Fixture f;auto* bad=reinterpret_cast<r::State*>(reinterpret_cast<unsigned char*>(&f.state)+1);
        assert(r::update(bad,Q,f.heap,8,&f.services,&f.result)==r::Status::invalid_argument && f.calls.empty());++cases;}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"fresh_character_gates\":true,\"borrowed_point_and_table_reads\":true,\"player_character_identity\":true,\"failures_preserve_effects\":true}\n";
}
