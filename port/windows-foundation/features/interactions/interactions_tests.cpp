#include "original_interactions.hpp"
#include "../../../level-world/tests/openable_container_real_cache_fixture_v1.hpp"
#include <cassert>
#include <iostream>
#include <limits>
using namespace dh::foundation::interactions;
int main(){
    std::string e;std::uintptr_t ooi=42;std::uintptr_t current_target=0;std::uint8_t use=0;
    bool remote=false,idle=true,moving=false;int calls=0;
    CharacterUseBorrow b;b.owner=1;b.object_of_interest=&ooi;b.ai_current_target=&current_target;b.use_requested=&use;
    b.remotely_updated=[&](bool& v,std::string&){v=remote;return true;};
    b.idle=[&](bool& v,std::string&){v=idle;return true;};
    b.moving=[&](bool& v,std::string&){v=moving;return true;};
    b.set_ai_target=[&](std::uintptr_t t,bool flag,std::string&){assert(t==42&&!flag&&use==0);++calls;return true;};
    assert(character_use(b,0,e)==Outcome::completed&&use==1&&calls==1);
    use=0;remote=true;assert(character_use(b,42,e)==Outcome::blocked&&calls==1);remote=false;
    current_target=1;assert(character_use(b,42,e)==Outcome::blocked);current_target=0;
    idle=false;assert(character_use(b,42,e)==Outcome::blocked);moving=true;
    assert(character_use(b,42,e)==Outcome::completed&&calls==2);
    std::uint8_t forced=0,locked=1;bool global=true;int prefix=0,dispatch=0;
    ControllerUseBorrow c;c.forced=&forced;c.locked=&locked;c.globally_blocked=&global;
    c.online=[](bool& v,std::string&){v=true;return true;};
    c.online_prefix=[&](std::uintptr_t t,std::string&){assert(t==42&&dispatch==0);++prefix;return true;};
    c.controllable=[&](std::uintptr_t,std::string&){assert(prefix==1);++dispatch;return Outcome::completed;};
    assert(controller_use(c,42,e)==Outcome::blocked);forced=255;
    assert(controller_use(c,42,e)==Outcome::completed&&dispatch==1);
    c.online_prefix={};assert(controller_use(c,42,e)==Outcome::failed&&!e.empty());
    RangeBorrow r;r.owner=1;r.current_target=42;float x=23;bool node=false;int type=0,melee_queries=0;
    r.target_position=[](auto,std::array<float,3>& v,std::string&){v={0,0,0};return true;};
    r.interaction_spot=[&](auto,std::array<float,3>& v,std::string&){v={x,0,0};return true;};
    r.has_interaction_node=[&](auto,bool& v,std::string&){v=node;return true;};
    r.melee_radius=[&](float& v,std::string&){++melee_queries;v=10;return true;};
    r.target_radius=[](auto,float& v,std::string&){v=3;return true;};
    r.interaction_padding=[](float& v,std::string&){v=10;return true;};
    r.interaction_type=[&](auto,auto,int& v,std::string&){v=type;return true;};bool in=false;
    assert(in_interaction_range(r,0,in,e)&&in);x=23.01f;assert(in_interaction_range(r,0,in,e)&&!in);
    type=8;x=13;assert(in_interaction_range(r,0,in,e)&&in);x=13.01f;assert(in_interaction_range(r,0,in,e)&&!in);
    node=true;type=0;x=80;const auto saved_queries=melee_queries;
    assert(in_interaction_range(r,0,in,e)&&in&&melee_queries==saved_queries);x=80.01f;assert(in_interaction_range(r,0,in,e)&&!in);
    type=8;x=0;assert(in_interaction_range(r,0,in,e)&&in);x=0.01f;assert(in_interaction_range(r,0,in,e)&&!in);
    x=std::numeric_limits<float>::quiet_NaN();assert(in_interaction_range(r,0,in,e)&&!in);
    dh2::world::OpenableContainerTableV1 table;
    assert(table.load(source_openable_records,sizeof source_openable_records,source_openable_names,sizeof source_openable_names,e)&&table.size()==68);
    dh2::world::OpenableContainerRowV1 row;std::int32_t id=-1;
    assert(table.resolve("AbbeyRuins_Normal_Chest",id,row,e)&&id==0&&row.sound==33&&row.keep_physics&&row.loot==0);
    dh2::world::OpenableContainerFieldsV1 fields;fields.key_id710=9;
    dh2::world::OpenableContainerServicesV1 svc;svc.is_character=[](auto,bool& v,std::string&){v=true;return true;};
    svc.find_key=[](auto,auto,bool& found,std::int16_t& qty,std::string&){found=true;qty=1;return true;};
    int consumed=0;svc.consume_key=[&](auto,auto,auto,bool& v,std::string&){++consumed;v=true;return true;};
    dh2::world::OpenableContainerOwnerV1 chest(fields,svc);bool unlocked=false;
    fields.key_consume=false;assert(chest.try_unlock(1,unlocked,e)&&!unlocked&&consumed==0);
    fields.key_consume=true;assert(chest.try_unlock(1,unlocked,e)&&unlocked&&consumed==1);
    fields.key_qty=2;assert(chest.try_unlock(1,unlocked,e)&&!unlocked&&consumed==1);
    fields.state394=4;assert(!chest.is_interactive(false)&&!chest.is_locked());
    Router router;std::uint32_t live_type=7;dh2::world::CanonicalObjectBorrowV1 object;
    object.identity=42;object.lease=std::make_shared<int>(0);object.type_f4=&live_type;
    assert(router.dispatch(object,1,e)==Outcome::unsupported);
    assert(router.bind(7,[](const auto& obj,auto actor,std::string&){return obj.identity==42&&actor==1;},e));
    assert(router.dispatch(object,1,e)==Outcome::completed);live_type=9;
    assert(router.dispatch(object,1,e)==Outcome::unsupported);
    std::cout<<"Interactions source gates, range boundaries, live dispatch and 68 real cache rows passed\n";
}

