#include "../playable_actor_bodies.hpp"
#include "../original_actor_properties.hpp"
#include "../playable_actor_world.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
struct Fields{ActorState actor;std::array<float,3> destination{};std::uintptr_t attached=0,visual=0;};
}
int main(int argc,char** argv){try{
    check(argc==2,"Supply original assets");AssetCatalog assets(argv[1]);std::string error;
    auto database=std::make_shared<OriginalPropertyDatabase>();
    auto ai=std::make_shared<dh2::data::AiTables>();
    check(load_original_property_tables(assets,"original-cache/data/pydata",*database,error),error);
    check(load_original_ai_tables(assets,"original-cache/data/pydata",*ai,error),error);
    auto world=std::make_shared<dh2::physical::NativeWorld>();const float bounds[]{-1000,-1000,1000,1000};world->load(bounds);
    PlayableActorBodies pool;
    std::vector<std::shared_ptr<Fields>> fields;
    std::vector<std::shared_ptr<OriginalCombatProperties>> properties;
    unsigned filters=0,pending_peers=0,adds=0,removes=0;
    void* rolled_back_context=nullptr;
    const auto bind=[&](ActorId id,const std::string& row,bool wrong_plan){
        auto actor=std::make_shared<Fields>();actor->actor.id=id;actor->actor.transform.position={100,100,0};
        actor->destination=actor->actor.transform.position;actor->actor.health=73;actor->actor.target_id=999;
        fields.push_back(actor);
        auto resolved=std::make_shared<OriginalCombatProperties>();OriginalActorProperties source;
        check(resolve_original_actor_properties(database->characters,database->classes,row,{256,true},source,error),error);
        resolved->sheets=std::move(source.sheets);properties.push_back(resolved);
        OriginalActorBodyPlanInput input;input.properties=&resolved->sheets;input.ai=ai.get();input.owner_identity=id;
        input.position={100,100,0};input.source_name=row;
        input.visual.model_path=row=="KnightPlayerBase"?"models/prince_modular.bdae":"original-cache/data/3d/characters/lizardman/lizardman.bdae";
        input.visual.use_authored_modular_defaults=row=="KnightPlayerBase";
        OriginalActorBodyPlan plan;check(make_original_actor_body_plan(assets,input,plan,error),error);
        if(wrong_plan)plan.group_index^=1;
        OriginalActorPhysicalBindings services;services.actor_lease=actor;services.world_lease=world;services.data_lease=resolved;
        services.world=world.get();services.ai=ai.get();services.destination1a8=actor->destination.data();
        services.attached2e0=&actor->attached;services.visual2d8=&actor->visual;
        services.static84=[](auto& out,auto&){out=0;return true;};
        services.is_player=[row](auto type,auto& out,auto&){out=original_actor_source_is_player(type,row);return true;};
        services.debug_switch=[](const char*,bool& out,std::string&){out=false;return true;};
        services.filter=[&,id](void* context,const auto&,const auto&,bool& allowed,std::string& e){
            ++filters;ActorId peer=0;
            if(!pool.resolve_physical_actor(context,peer,e))return false;
            check(peer&&pool.physical(id)&&pool.physical(peer),"Synchronous filter lost an actual actor owner");
            if(!pool.physical(peer)->native().body)++pending_peers;
            if(id==102)rolled_back_context=const_cast<OriginalActorPhysical*>(pool.physical(id));
            if(peer==102)rolled_back_context=context;
            allowed=true;e.clear();return true;
        };
        services.contact=[&](auto event,void* context,unsigned,std::string& e){
            ActorId peer=0;if(!pool.resolve_physical_actor(context,peer,e))return false;
            check(peer!=0,"Real contact lost its peer during registration/release");
            if(event==dh2::physical::ContactEvent::add)++adds;
            if(event==dh2::physical::ContactEvent::remove)++removes;
            e.clear();return true;
        };
        return pool.bind(actor->actor,*resolved,plan,std::move(services),error);
    };
    const auto empty_count=world->backend()->GetBodyCount();
    check(bind(100,"Swamp_LizadMan_Type1",false),error);
    check(bind(101,"KnightPlayerBase",false),error);
    check(pool.size()==2&&world->backend()->GetBodyCount()==empty_count+2,
          "Real overlapping body allocation lost or duplicated the actor registry");
    check(filters>0&&pending_peers>0,"Fixture did not resolve a real pending peer inside native allocation");
    const auto accepted_count=world->backend()->GetBodyCount();
    check(!bind(102,"KnightPlayerBase",true),"Invalid source plan was accepted");
    check(pool.size()==2&&!pool.physical(102)&&world->backend()->GetBodyCount()==accepted_count,
          "Failed plan left a candidate registry/body or removed an existing owner");
    if(rolled_back_context){ActorId output=999;check(!pool.resolve_physical_actor(rolled_back_context,output,error),
        "Rolled-back physical context remained resolvable");}
    check(pool.set_pinned(100,false,error)&&pool.set_pinned(101,false,error),error);
    world->update(16);check(adds>0,"Real overlapping bodies did not exercise contact delivery");
    const auto prior_removes=removes;check(pool.release(101,error),error);
    check(removes>prior_removes&&pool.size()==1&&pool.physical(100),
          "Release failed to retain peer identity through actual contact removal");
    for(const auto& field:fields)check(field->actor.health==73&&field->actor.target_id==999,
          "Physical construction/release changed actor gameplay authority");
    check(pool.clear(error)&&world->backend()->GetBodyCount()==empty_count,error);
    std::cout<<"PASS real overlapping Lizard/Knight construction: filters="<<filters
             <<" pending_peers="<<pending_peers<<" adds="<<adds<<" removes="<<removes
             <<"; candidate rollback and release identity; no native or registry leak\n";
    return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
