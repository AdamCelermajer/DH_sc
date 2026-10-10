#include "../original_actor_collision_filter.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh::foundation;
using dh2::navigation::ContactFilter;
static void check(bool ok,const char* error){if(!ok)throw std::runtime_error(error);}
int main(){try{std::string error;OriginalActorCollisionFilter filter;
    std::map<ActorId,std::int32_t> states{{1,3},{2,3}};std::map<ActorId,std::uint8_t> enabled{{1,1},{2,1}};
    std::vector<std::string> calls;
    OriginalCollisionOwnerServices source;
    source.character_handle=[&](ActorId id,bool& result,std::string&){calls.push_back("handle"+std::to_string(id));result=true;return true;};
    source.original_state=[&](ActorId id,std::int32_t& result,std::string&){calls.push_back("state"+std::to_string(id));result=states.at(id);return true;};
    source.enabled80=[&](ActorId id,std::uint8_t& result,std::string&){calls.push_back("enabled"+std::to_string(id));result=enabled.at(id);return true;};
    check(filter.bind(1,OriginalCollisionReceiverKind::po_character,source,error)&&
          filter.bind(2,OriginalCollisionReceiverKind::po_character,source,error),error.c_str());
    // Exact original player/monster physical categories/masks from body kernel.
    ContactFilter player{-1,4,0xd7f,1},monster{2,0x10,0xd3f,1};bool allowed=false;
    check(filter.test_pair(1,2,player,monster,allowed,error)&&allowed,"source Idle player/monster pair rejected");
    check(calls==std::vector<std::string>{"handle1","state1","enabled1","enabled2","handle2","state2","enabled2","enabled1"},
          "source prefix/enabled/two receiver ordering differs");
    states[1]=0;calls.clear();allowed=true;
    check(filter.test_pair(1,2,player,monster,allowed,error)&&!allowed,"source limbus owner admitted");
    check(calls==std::vector<std::string>{"handle1","state1","handle2","state2","enabled2","enabled1"},
          "source second receiver skipped after first denied or prefix read enabled too early");
    states[1]=10;
    check(filter.permits_category(1,1,allowed,error)&&allowed,"knockback must allow category1");
    check(filter.permits_category(1,2,allowed,error)&&allowed,"knockback must allow category2");
    check(filter.permits_category(1,3,allowed,error)&&allowed,"knockback must allow category3");
    check(filter.permits_category(1,4,allowed,error)&&!allowed,"knockback wrongly collided player category4");
    check(filter.permits_category(1,0x10,allowed,error)&&!allowed,"knockback wrongly collided monster category16");
    for(const std::int32_t source_state:{-1,1,3,4,5,6,7,12,17,19}){
        states[1]=source_state;
        check(filter.permits_category(1,0x10,allowed,error)&&allowed,"other original state wrongly inherited limbus/knockback rejection");
    }
    states[1]=3;enabled[1]=0;calls.clear();
    check(filter.test_one(1,2,player,monster,allowed,error)&&!allowed&&
          calls==std::vector<std::string>{"handle1","state1","enabled1"},"disabled source owner read peer/masks prematurely");
    enabled[1]=1;enabled[2]=0;check(filter.test_one(1,2,player,monster,allowed,error)&&!allowed,"disabled peer admitted");enabled[2]=1;
    enabled[1]=255;enabled[2]=128;check(filter.test_one(1,2,player,monster,allowed,error)&&allowed,"raw nonzero enabled byte not honored");
    enabled[1]=enabled[2]=1;
    auto same_positive=player;same_positive.group=2;same_positive.mask=0;
    check(filter.test_one(1,2,same_positive,monster,allowed,error)&&allowed,"positive group did not override masks");
    auto same_negative=monster;same_negative.group=-1;
    check(filter.test_one(1,2,player,same_negative,allowed,error)&&!allowed,"negative group did not reject");
    auto zero_mask=player;zero_mask.mask=0;check(filter.test_one(1,2,zero_mask,monster,allowed,error)&&!allowed,"zero shape mask admitted");
    auto missing=source;missing.original_state={};check(filter.bind(3,OriginalCollisionReceiverKind::po_character,missing,error),error.c_str());
    allowed=true;check(!filter.permits_category(3,0x10,allowed,error)&&allowed,"missing FSM silently became successful true/false");
    auto noncharacter=missing;noncharacter.character_handle=[](ActorId,bool& result,std::string&){result=false;return true;};
    check(filter.bind(3,OriginalCollisionReceiverKind::po_character,noncharacter,error)&&filter.permits_category(3,0x10,allowed,error)&&allowed,
          "actual non-Character handle incorrectly required FSM");
    filter.remove(3);allowed=true;check(!filter.test_one(1,3,player,monster,allowed,error)&&allowed,"unbound peer mutated output");
    check(!filter.bind(4,OriginalCollisionReceiverKind::po_character,{},error),"ungrounded empty collision services accepted");
    std::cout<<"original_actor_collision_filter PASS: source state/category prefix, enabled bytes, signed groups/masks, both receiver order, required provider failures\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
