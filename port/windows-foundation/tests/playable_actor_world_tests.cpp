#include "../playable_actor_world.hpp"
#include "../asset_catalog.hpp"
#include "../animation_markers.hpp"
#include "../actor_combat_runtime.hpp"
#include <iostream>
#include <stdexcept>
#include <cmath>
using namespace dh::foundation;
static void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
static ActorState actor(ActorId id,const std::string& name,const OriginalCombatProperties& props) {
    ActorState result;result.id=id;result.definition_id=name;result.faction_id=props.sheets.resolved[0];
    result.health=original_signed256(props.sheets.resolved[36]);
    result.max_health=original_signed256(props.sheets.resolved[38]);
    result.resource=original_signed256(props.sheets.resolved[41]);
    result.max_resource=original_signed256(props.sheets.resolved[43]);return result;
}
struct RuntimeVisual {
 std::map<std::string,AnimationMarkers> clips;std::string selected;double elapsed=0;
 void add(const std::string& name,std::int32_t start,std::int32_t end,std::int32_t marker){
  std::string error;const char* event[]={"attack_mainhand"};dh2::animation::EventGroup group[]={{1,event}};
  dh2::animation::EventView view{4,1,reinterpret_cast<const std::uint8_t*>(&marker),group};
  if(!clips[name].load(view,start,end,error))throw std::runtime_error(error);
 }
 CombatVisualBinding binding(){return {
  [this](const std::string& name,bool loop,std::string& error){if(loop||!clips.count(name)){error="Unknown runtime test clip";return false;}selected=name;elapsed=0;return true;},
  [this](const std::string& name,std::int32_t& start,std::int32_t& end,std::string&){auto i=clips.find(name);if(i==clips.end())return false;start=i->second.start_ms();end=i->second.end_ms();return true;},
  [this](const std::string& name,std::string&)->const AnimationMarkers*{auto i=clips.find(name);return i==clips.end()?nullptr:&i->second;},
  [this](double dt,std::string&){elapsed+=dt;return true;}};}
};
int main(int argc,char** argv){try {
    if(argc!=3)throw std::runtime_error("Supply original equipment and population asset roots");
    AssetCatalog equipment_assets(argv[1]),population_assets(argv[2]);
    std::string error;const std::string root="original-cache/data/pydata/";
    OriginalPropertyDatabase db;dh2::data::AiTables tables;
    check(load_original_property_tables(equipment_assets,root,db,error),error);
    check(load_original_ai_tables(population_assets,root,tables,error),error);
    const auto data=equipment_assets.read(root+"loot_table_pyarray.bin"),
               names=equipment_assets.read(root+"loot_table_pyarraynames.bin"),
               fields=equipment_assets.read(root+"loot_table_pystructnames.bin");
    dh2::data::ItemTable items;
    check(dh2::data::load_items({data.data(),data.size()},{names.data(),names.size()},
        {fields.data(),fields.size()},items,error),error);
    const auto sword=items.rows.at(664).record;OriginalCombatFacts facts;
    check(original_combat_equipment_facts(&sword,nullptr,facts,error),error);
    OriginalCombatProperties knight,lizard;
    check(build_original_combat_properties(db,"KnightPlayerBase",{256,true},{{sword,false,{}}},facts,knight,error),error);
    check(build_original_combat_properties(db,"Swamp_LizadMan_Type1",{std::nullopt,true},{},{},lizard,error),error);
    dh2::data::CombatRandom rng{1234,0}; // Deliberate test stream, runtime has no default.
    PlayableActorWorld world(tables,rng);
    {
        const auto before=world.random_state();auto expected=before;
        const auto native=dh2_combat_random(&expected,37);std::uint32_t draw=999;
        check(world.random_uniform(37,draw,error)&&draw==static_cast<std::uint32_t>(native),"Shared source draw differs");
        check(rng.seed==expected.seed&&rng.calls==expected.calls,"Draw did not update actual host stream");
        const auto after=rng;draw=999;
        check(!world.random_uniform(UINT32_MAX,draw,error)&&draw==999&&rng.seed==after.seed&&rng.calls==after.calls,"Invalid range mutated stream/output");
        check(world.random_uniform(0,draw,error)&&draw==0&&rng.seed==after.seed&&rng.calls==after.calls+1,"Source Random(0) semantics differ");
        rng=before;std::uint32_t restored=999;
        check(world.random_uniform(37,restored,error)&&restored==static_cast<std::uint32_t>(native),"Restored shared stream did not replay draw");
        rng=before; // Existing combat fixture retains its original starting stream.
    }
    check(world.bind_actor(actor(1,"KnightPlayerBase",knight),knight,{true,true,sword},error),error);
    ActorState* stable=world.find_actor(1);
    check(world.bind_actor(actor(2,"Swamp_LizadMan_Type1",lizard),lizard,{false,true,std::nullopt},error),error);
    check(world.find_actor(1)==stable,"binding actor invalidated stable pointer");
    check(!world.bind_actor(actor(1,"duplicate",knight),knight,{true,true,sword},error),"duplicate ID accepted");
    std::cout<<"original factions Knight="<<stable->faction_id<<" Lizard="<<world.find_actor(2)->faction_id
             <<" enemy="<<world.eligible_target(*stable,*world.find_actor(2))
             <<" reaches="<<world.melee_reach(1)<<"+"<<world.melee_reach(2)<<'\n';
    check(world.eligible_target(*stable,*world.find_actor(2)),"original warrior/lizard relation not hostile");
    check(world.bind_source("original-main",{},error),error);
    {
        const auto saved=rng;auto expected=saved;
        auto expect=[&](std::int32_t bound){return dh2_combat_random(&expected,bound);};
        check(world.with_loot_random([&](dh2::data::LootRandom8V2& loan,std::string& e){
            std::int32_t first{},last{};std::uint32_t audio{};
            check(dh2_loot_v2_random(&loan,13,&first)==0&&first==expect(13),"Loot loan changed source random result");
            check(world.random_state().seed==loan.seed&&world.random_state().calls==loan.calls,"World snapshot missed native loot prefix");
            check(world.random_uniform(17,audio,e)&&audio==std::uint32_t(expect(17)),"Reentrant audio draw lost loot prefix/order");
            check(dh2_loot_v2_random(&loan,23,&last)==0&&last==expect(23),"Native loot draw lost reentrant audio prefix");
            std::string nested;
            check(!world.with_loot_random([](auto&,auto&){return true;},nested),"Nested loan replaced shared stream");
            check(!world.replace_actors({},saved,nested)&&world.find_actor(1)==stable,"Loan allowed actor/RNG replacement");
            return true;
        },error),error);
        check(rng.seed==expected.seed&&rng.calls==expected.calls,"Scoped loot/audio draws did not commit to actual host RNG");
        check(!world.with_loot_random([&](auto& loan,std::string& e){std::int32_t value{};check(dh2_loot_v2_random(&loan,7,&value)==0&&value==expect(7),"Failed prefix draw");e="expected loot failure";return false;},error)&&error=="expected loot failure","Failed loot callback lost its error");
        check(rng.seed==expected.seed&&rng.calls==expected.calls,"Failed loot callback rolled back consumed RNG prefix");
        check(!world.with_loot_random([&](auto& loan,std::string&)->bool{std::int32_t value{};check(dh2_loot_v2_random(&loan,9,&value)==0&&value==expect(9),"Throwing prefix draw");throw std::runtime_error("expected loot exception");},error)&&error=="expected loot exception","Thrown callback was not reported");
        check(rng.seed==expected.seed&&rng.calls==expected.calls,"Throwing loot callback lost consumed RNG prefix");
        check(!world.with_loot_random([&](auto&,auto&){world.clear();return true;},error)&&world.find_actor(1)==stable,
              "Loan allowed registry clear while actor receivers are borrowed");
        std::uint32_t next{};check(world.random_uniform(0,next,error)&&next==std::uint32_t(expect(0)),"Loan failure left a dangling active loan");
        check(rng.seed==expected.seed&&rng.calls==expected.calls,"Random(0) after loan failure differs");
        rng=saved;

        // Compare an interleaved native loot / genuine combat / synchronous
        // presentation draw / native loot path with an independent SAME-data
        // world executing those operations in their actual source order.
        auto reference_rng=saved;PlayableActorWorld reference(tables,reference_rng);
        check(reference.bind_actor(actor(1,"KnightPlayerBase",knight),knight,{true,true,sword},error),error);
        check(reference.bind_actor(actor(2,"Swamp_LizadMan_Type1",lizard),lizard,{false,true,std::nullopt},error),error);
        check(reference.bind_source("original-main",{},error),error);
        std::uint32_t reference_first{},reference_audio{},reference_last{};float reference_damage{};
        check(reference.random_uniform(13,reference_first,error),error);
        check(reference.resolve_damage("original-main",*reference.find_actor(1),*reference.find_actor(2),"loan-combat",reference_damage,error),error);
        check(reference.random_uniform(19,reference_audio,error)&&reference.random_uniform(23,reference_last,error),error);
        unsigned notifications=0;std::uint32_t actual_audio{};float actual_damage{};
        world.set_resolution_observer([&](const auto&,std::string& e){++notifications;return world.random_uniform(19,actual_audio,e);});
        check(world.with_loot_random([&](auto& loan,std::string& e){
            std::int32_t first{},last{};
            check(dh2_loot_v2_random(&loan,13,&first)==0&&std::uint32_t(first)==reference_first,"Combat-prefix loot mismatch");
            if(!world.resolve_damage("original-main",*stable,*world.find_actor(2),"loan-combat",actual_damage,e))return false;
            check(dh2_loot_v2_random(&loan,23,&last)==0&&std::uint32_t(last)==reference_last,"Combat/presentation prefix lost before next loot draw");
            return true;
        },error),error);
        check(actual_damage==reference_damage&&actual_audio==reference_audio&&notifications==1&&
              rng.seed==reference_rng.seed&&rng.calls==reference_rng.calls,"Loan changed genuine combat/presentation results or stream order");
        world.set_resolution_observer({});check(world.take_resolutions().size()==1,"Loan duplicated genuine combat result");rng=saved;
        std::cout<<"Shared loot/combat/audio stream PASS named loan, consumed failure prefixes, nested/replace guards\n";
    }
    unsigned observed=0;PlayableCombatResolution snapshot;std::array<float,3> sourcePosition{};
    world.set_resolution_observer([&](const PlayableCombatResolution& resolution,std::string&){
        ++observed;snapshot=resolution;sourcePosition=world.find_actor(resolution.victim)->transform.position;
        check(world.take_resolutions().empty(),"Presentation observer ran after the queued result boundary");
        return true;
    });
    float amount=-1;
    check(world.resolve_damage("original-main",*stable,*world.find_actor(2),"attack",amount,error),error);
    check(amount>=0&&rng.calls>0,"source damage/RNG not consumed");
    auto events=world.take_resolutions();
    check(events.size()==1&&events[0].marker_name=="attack"&&events[0].melee.damage==amount,
        "full original event not retained");
    check(observed==1&&snapshot.attacker==1&&snapshot.victim==2&&snapshot.melee.damage==amount,
        "Synchronous presentation snapshot duplicated or replaced the original result");
    stable->attack_ids={"source-carrier"};
    AttackDefinition sourceAttack;sourceAttack.id="source-carrier";sourceAttack.animation_clip_id="source-carrier-clip";
    sourceAttack.maximum_range=2;sourceAttack.damage_markers={{"attack_mainhand","original-main"}};
    CombatSystem sourceCombat(world);
    check(sourceCombat.begin(1,2,sourceAttack,77,error),error);
    MarkerOccurrence sourceMarker;sourceMarker.generation=77;sourceMarker.marker.name="attack_mainhand";
    DamageEvent sourceEvent;
    check(sourceCombat.consume_marker(1,sourceMarker,sourceEvent,error),error);
    const auto carried=world.take_resolutions();
    check(sourceEvent.applied&&sourceEvent.source_outcomes&&carried.size()==1
          &&*sourceEvent.source_outcomes==carried[0].melee.original.outcomes,
          "Genuine original kernel outcomes did not reach the canonical DamageEvent");
    // Find a deterministic seed which produces a real source Injure bit for
    // these exact property sheets, then exercise the complete marker/runtime path.
    dh2::data::CombatRandom sourceRng{1,0};PlayableActorWorld sourceWorld(tables,sourceRng);
    auto sourceAttacker=actor(11,"KnightPlayerBase",knight),sourceVictim=actor(12,"Swamp_LizadMan_Type1",lizard);
    sourceAttacker.attack_ids={"source-runtime"};sourceVictim.attack_ids={"source-runtime"};
    sourceVictim.transform.position={1,0,0};
    check(sourceWorld.bind_actor(sourceAttacker,knight,{true,true,sword},error),error);
    check(sourceWorld.bind_actor(sourceVictim,lizard,{false,true,std::nullopt},error),error);
    check(sourceWorld.bind_source("original-main",{},error),error);
    std::uint32_t injureSeed=0;
    for(std::uint32_t seed=1;seed<=50000&&!injureSeed;++seed){
      sourceRng={seed,0};float resolvedDamage=-1;std::optional<std::uint32_t> outcomes,sourceMask;
      check(sourceWorld.resolve_damage_with_outcomes("original-main",*sourceWorld.find_actor(11),
        *sourceWorld.find_actor(12),"seed-probe",resolvedDamage,outcomes,sourceMask,error),error);
      check(sourceMask&&*sourceMask==0x22aab5u,"Source melee mask was not preserved");
      if(outcomes&&(*outcomes&0x10u))injureSeed=seed;
      sourceWorld.take_resolutions();
    }
    check(injureSeed!=0,"Original melee fixture produced no Injure outcome in seed search");
    sourceRng={injureSeed,0};
    CombatSystem runtimeCombat(sourceWorld);ActorCombatRuntime sourceRuntime(runtimeCombat);
    RuntimeVisual sourceFirst,sourceSecond;
    for(auto* visual:{&sourceFirst,&sourceSecond}){visual->add("attack",0,200,50);visual->add("react",0,100,1000);}
    check(sourceRuntime.bind(*sourceWorld.find_actor(11),sourceFirst.binding(),{},error),error);
    CombatPoseBindings sourceReact;sourceReact.react_clip_id="react";
    check(sourceRuntime.bind(*sourceWorld.find_actor(12),sourceSecond.binding(),sourceReact,error),error);
    AttackDefinition sourceDefinition;sourceDefinition.id="source-runtime";sourceDefinition.animation_clip_id="attack";
    sourceDefinition.maximum_range=2;sourceDefinition.damage_markers={{"attack_mainhand","original-main"}};
    check(sourceRuntime.begin(11,12,sourceDefinition,error)&&sourceRuntime.begin(12,11,sourceDefinition,error),error);
    std::vector<DamageEvent> sourceEvents;check(sourceRuntime.update(.06,sourceEvents,error),error);
    check(sourceEvents.size()==1&&sourceEvents[0].source_outcomes
          &&(*sourceEvents[0].source_outcomes&0x10u)&&runtimeCombat.attacking(11)
          &&!runtimeCombat.attacking(12)&&sourceWorld.find_actor(12)->target_id==invalid_actor_id
          &&sourceSecond.selected=="react",
          "Actual original kernel Injure bit did not reach same-session reaction admission");
    const auto capturedPosition=sourcePosition;world.find_actor(2)->transform.position[0]+=1;
    check(sourcePosition==capturedPosition,"Later movement changed the captured source result position");
    world.find_actor(2)->transform.position=capturedPosition;
    world.set_resolution_observer({});
    check(world.take_resolutions().empty(),"events drained twice");
    world.set_resolution_observer([](const PlayableCombatResolution&,std::string& e){e="Unavailable presentation fixture";return false;});
    const auto callsBeforePresentationFailure=rng.calls;amount=-1;
    check(world.resolve_damage("original-main",*stable,*world.find_actor(2),"attack",amount,error)&&amount>=0&&error.empty(),
        "Optional presentation failure rejected genuine damage");
    events=world.take_resolutions();const auto diagnostics=world.take_resolution_observer_errors();
    check(events.size()==1&&events[0].melee.damage==amount&&rng.calls>callsBeforePresentationFailure,
        "Presentation failure dropped the real damage/RNG result");
    check(diagnostics==std::vector<std::string>{"Unavailable presentation fixture"}&&world.take_resolution_observer_errors().empty(),
        "Presentation diagnostics duplicated or disappeared");
    world.set_resolution_observer({});
    const float reach=world.melee_reach(1)+world.melee_reach(2);
    auto* target=world.find_actor(2);target->transform.position={reach,0,0};
    check(!world.original_melee_in_range(1,2),"strict original reach boundary accepted");
    const auto before=rng;float retained=91;
    check(!world.resolve_damage("original-main",*stable,*target,"attack",retained,error)&&retained==91&&
          rng.seed==before.seed&&rng.calls==before.calls,"out-of-range changed output/RNG");
    target->transform.position={0,0,reach};
    check(!world.original_melee_in_range(1,2),"vertical distance ignored");
    target->transform.position={0,0,0};
    check(world.update_combat_properties(2,lizard,{true,true,std::nullopt},error),error);
    check(!world.eligible_target(*stable,*target),"original both-player hostility exclusion omitted");
    check(world.update_combat_properties(2,lizard,{false,false,std::nullopt},error),error);
    check(!world.eligible_target(*stable,*target),"untargetable actor accepted");
    check(world.update_combat_properties(2,lizard,{false,true,std::nullopt},error),error);
    apply_actor_damage(*target,target->health);
    check(!world.eligible_target(*stable,*target),"dead actor accepted");
    stable->target_id=2;check(world.remove_actor(2)&&stable->target_id==0,"removed target reference retained");
    // Exercise directed signed relationship semantics using genuine faction rows.
    bool directed=false;
    for(std::size_t owner=0;owner<tables.factions.size()&&!directed;++owner)
      for(std::size_t victim=0;victim<tables.factions.size()&&!directed;++victim)
        if(dh2::data::ai_enemy(tables,int(owner),int(victim),false,false)&&
           !dh2::data::ai_enemy(tables,int(victim),int(owner),false,false)){
          auto a=knight,b=lizard;a.sheets.resolved[0]=int(owner);b.sheets.resolved[0]=int(victim);
          check(world.bind_actor(actor(3,"relation-fixture-owner",a),a,{false,true,sword},error),error);
          check(world.bind_actor(actor(4,"relation-fixture-victim",b),b,{false,true,std::nullopt},error),error);
          check(world.eligible_target(*world.find_actor(3),*world.find_actor(4))&&
                !world.eligible_target(*world.find_actor(4),*world.find_actor(3)),"faction relationship lost direction");
          directed=true;
        }
    check(directed,"original directed relationship scenario unavailable");
    auto saved=tables;check(!load_original_ai_tables(population_assets,"missing",tables,error)&&
        tables.factions.size()==saved.factions.size(),"failed table load replaced old data");
    const auto rng_before_clear=rng;world.clear();
    check(world.actors().empty()&&world.take_resolutions().empty()&&rng.seed==rng_before_clear.seed&&
        rng.calls==rng_before_clear.calls,"world clear replaced host RNG or retained actors/events");
    std::cout<<"playable_actor_world PASS\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
