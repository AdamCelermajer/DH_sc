#include "../character_faery_placement_v1.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using namespace dh2::character_faery_placement_v1;

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    struct Node { Identity id; bool faery; bool follower; } nodes[2]{{11,true,false},{12,false,true}};
    std::vector<Operation> calls;
    std::vector<Identity> masters;
    std::vector<Identity> linked_players;
    Vector3 placed[2]{};
    unsigned placed_count=0, disable_count=0, change_count=0;

    static int invoke(void* raw, const Request& q, Reply* r) {
        auto& f=*static_cast<Fixture*>(raw);
        f.calls.push_back(q.operation);
        switch(q.operation) {
        case Operation::list_begin: r->identity=1000; return 0;
        case Operation::list_end: r->identity=1002; return 0;
        case Operation::list_value: r->identity=q.subject==1000?11:12; return 0;
        case Operation::list_next: r->identity=q.subject+1; return 0;
        case Operation::is_faery:
            r->word=(q.subject==11); return 0;
        case Operation::is_follower:
            r->word=(q.subject==12); return 0;
        case Operation::online_state: r->word=0; return 0;
        case Operation::local_player: r->identity=20; require(q.value==1,"GetLocalPlayer flag changed"); return 0;
        case Operation::player_character: r->identity=q.subject==20?30:40; return 0;
        case Operation::look_at_vector: r->vector={1,2,3}; return 0;
        case Operation::target_position: r->vector={10,20,30}; return 0;
        case Operation::set_position:
            require(q.vector&&f.placed_count<2,"placement vector missing");
            f.placed[f.placed_count++]=*q.vector; return 0;
        case Operation::force_update_position: return 0;
        case Operation::disable_zoning: ++f.disable_count; return 0;
        case Operation::player_count: r->word=2; return 0;
        case Operation::player_at: r->identity=q.index?21:20; return 0;
        case Operation::set_player_faery: f.linked_players.push_back(q.subject); require(q.argument==11,"wrong linked Faery"); return 0;
        case Operation::previous_ai_master: r->identity=55; return 0;
        case Operation::set_ai_master: f.masters.push_back(q.argument); return 0;
        case Operation::current_faery_id: require(q.subject==30&&q.value==-1,"current Faery source args changed"); r->word=2; return 0;
        case Operation::change_faery: require(q.subject==30&&q.value==2,"ChangeFaery args changed"); ++f.change_count; return 0;
        case Operation::hosting_player:
        case Operation::hosting_player_for_follower_state:
        case Operation::follower_host_state:
        case Operation::is_local_player_hosting:
            throw std::runtime_error("unexpected online-only operation");
        }
        return 1;
    }
};

void placement_and_owner_order() {
    Fixture fixture;
    Runtime runtime({&fixture,Fixture::invoke});
    Result result{};
    require(runtime.place(0,&result)==Status::complete,"offline placement failed");
    require(result.visited==2&&result.placed_faeries==1&&result.placed_followers==1,
            "Faery/follower list classification changed");
    require(fixture.placed_count==2&&fixture.placed[0].x==11&&
            fixture.placed[0].y==22&&fixture.placed[0].z==33,
            "Faery did not use source look + target placement");
    require(fixture.disable_count==1,"followers must disable zoning after placement");
    require(fixture.linked_players==std::vector<Identity>{30,40},
            "Faery was not linked to every existing Player Character");
    require(fixture.masters==std::vector<Identity>{30,55},
            "Faery AI master was not restored after ChangeFaery");
    require(fixture.change_count==1,"saved Faery selection was not applied once");
    std::cout<<"FAERY_PLACEMENT_HOST_PASS visited="<<result.visited
             <<" faeries="<<result.placed_faeries<<" followers="<<result.placed_followers
             <<" callbacks="<<result.callbacks<<"\n";
}
}

int main() {
    try { placement_and_owner_order(); }
    catch(const std::exception& e) { std::cerr<<"FAIL: "<<e.what()<<"\n"; return 1; }
}
