#include "../playable_actor_world.hpp"
#include "../asset_catalog.hpp"
#include "../animation_markers.hpp"
#include "../actor_combat_runtime.hpp"
#include "../../game-data/faery_tables.hpp"
#include "../../game-data/data.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
std::vector<std::uint8_t> read_file(const std::string& path){
    std::ifstream f(path,std::ios::binary);check(bool(f),"Missing actual Faery source table input: "+path);
    return {std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>()};
}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& v){return {v.data(),v.size()};}
ActorState make_actor(ActorId id,const char* name,const OriginalCombatProperties& p){
    ActorState a;a.id=id;a.definition_id=name;a.faction_id=p.sheets.resolved[0];
    a.health=original_signed256(p.sheets.resolved[36]);a.max_health=original_signed256(p.sheets.resolved[38]);
    a.resource=original_signed256(p.sheets.resolved[41]);a.max_resource=original_signed256(p.sheets.resolved[43]);
    a.attack_ids={"test-attack"};return a;
}
struct Visual {
    std::map<std::string,AnimationMarkers> clips;
    std::string selected;double elapsed=0;
    void add(const std::string& name,std::int32_t end,std::int32_t marker){
        std::string error;const char* events[]={"attack_mainhand"};dh2::animation::EventGroup group[]={{1,events}};
        dh2::animation::EventView view{4,1,reinterpret_cast<const std::uint8_t*>(&marker),group};
        check(clips[name].load(view,0,end,error),error);
    }
    CombatVisualBinding binding(){return {
        [this](const std::string& name,bool loop,std::string& error){
            if(loop||!clips.count(name)){error="Test source clip is missing";return false;}
            selected=name;elapsed=0;return true;},
        [this](const std::string& name,std::int32_t& start,std::int32_t& end,std::string&){
            const auto i=clips.find(name);if(i==clips.end())return false;
            start=i->second.start_ms();end=i->second.end_ms();return true;},
        [this](const std::string& name,std::string&)->const AnimationMarkers*{
            const auto i=clips.find(name);return i==clips.end()?nullptr:&i->second;},
        [this](double dt,std::string&){elapsed+=dt;return true;}};}
};
}

int main(int argc,char** argv){try{
    check(argc==4,"Supply original property, population, and Faery table roots");
    AssetCatalog property_assets(argv[1]),population_assets(argv[2]);
    const std::string root="original-cache/data/pydata/";std::string error;
    OriginalPropertyDatabase db;dh2::data::AiTables tables;
    check(load_original_property_tables(property_assets,root,db,error),error);
    check(load_original_ai_tables(population_assets,root,tables,error),error);
    const auto item_records=property_assets.read(root+"loot_table_pyarray.bin");
    const auto item_names=property_assets.read(root+"loot_table_pyarraynames.bin");
    const auto item_schema=property_assets.read(root+"loot_table_pystructnames.bin");
    dh2::data::ItemTable items;
    check(dh2::data::load_items({item_records.data(),item_records.size()},
        {item_names.data(),item_names.size()},{item_schema.data(),item_schema.size()},items,error),error);
    check(items.rows.size()>664,"Original Knight melee fixture item row is absent");
    const auto sword=items.rows.at(664).record;OriginalCombatFacts player_facts;
    check(original_combat_equipment_facts(&sword,nullptr,player_facts,error),error);
    const auto faery_raw=read_file(std::string(argv[3])+"/faeries_pyarray.bin");
    const auto faery_names=read_file(std::string(argv[3])+"/faeries_pyarraynames.bin");
    const auto faery_schema=read_file(std::string(argv[3])+"/faeries_pystructnames.bin");
    dh2::data::FaeryTables faeries;
    check(faeries.load(bytes(faery_raw),bytes(faery_names),bytes(faery_schema),error),error);
    const auto faery_source=faeries.borrow();
    const auto hotty=faery_source.faery_index("Fake_Hotty");
    check(hotty>=0,"Original Faery Fire spell source row Fake_Hotty is absent");
    const auto actual_faery_spell_type=static_cast<std::int32_t>(
        faery_source.faeries()[std::size_t(hotty)].scalar.words[7]);
    OriginalCombatProperties player,enemy;
    check(build_original_combat_properties(db,"KnightPlayerBase",{256,true},{{sword,false,{}}},player_facts,player,error),error);
    check(build_original_combat_properties(db,"Swamp_LizadMan_Type1",{std::nullopt,true},{},{},enemy,error),error);
    const auto fire=std::find(db.classes.names.begin(),db.classes.names.end(),"Spell_Fire_2");
    check(fire!=db.classes.names.end(),"Authored Spell_Fire_2 class is absent");
    auto formula=player.sheets.resolved;formula[172]=256;
    check(dh2::data::apply_class(db.classes,int(fire-db.classes.names.begin()),formula,error,
        &player.sheets.resolved),error);

    dh2::data::CombatRandom random{1,0};PlayableActorWorld world(tables,random);
    auto player_actor=make_actor(31,"KnightPlayerBase",player);
    auto enemy_actor=make_actor(32,"Swamp_LizadMan_Type1",enemy);
    enemy_actor.transform.position={1,0,0};
    check(world.bind_actor(player_actor,player,{true,true,std::nullopt},error),error);
    check(world.bind_actor(enemy_actor,enemy,{false,true,std::nullopt},error),error);
    const std::uint32_t authored_mask=0x1005554au;
    random={17,0};OriginalMeleeResolution spell_result;
    check(world.resolve_source_result("source-spell:Spell_Fire_2",31,32,"cast-hit",
        authored_mask,-1,actual_faery_spell_type,0,spell_result,error,&formula),error);
    const auto spell_receipts=world.take_resolutions();
    check(spell_receipts.size()==1&&spell_receipts[0].source_id=="source-spell:Spell_Fire_2"
        &&spell_result.original.mask==authored_mask&&spell_result.damage>0
        &&!(spell_result.original.outcomes&0x10u),
        "Actual Fire_2 calculation did not produce its retained non-Injure source result");
    const auto rng_after_spell=world.random_state();

    Visual attacker_visual,victim_visual;
    attacker_visual.add("attack",10000,9000);victim_visual.add("attack",10000,9000);
    victim_visual.add("react",100,50);victim_visual.add("death",120,60);
    CombatSystem combat(world);ActorCombatRuntime runtime(combat);
    check(runtime.bind(*world.find_actor(31),attacker_visual.binding(),{},error),error);
    CombatPoseBindings victim_poses;victim_poses.react_clip_id="react";victim_poses.death_clip_id="death";
    victim_poses.source_injury_gate_enabled=true;
    check(runtime.bind(*world.find_actor(32),victim_visual.binding(),victim_poses,error),error);
    AttackDefinition attack;attack.id="test-attack";attack.animation_clip_id="attack";
    attack.maximum_range=2;attack.damage_markers={{"attack_mainhand","unfired-test-marker"}};
    attack.cooldown_seconds=0;attack.cooldown_timing=CooldownTiming::attack_start;
    check(runtime.begin(31,32,attack,error)&&runtime.begin(32,31,attack,error),error);
    std::vector<DamageEvent> ordinary_events;check(runtime.update(.02,ordinary_events,error),error);
    const auto attacker_cursor=attacker_visual.elapsed;
    const auto attacker_elapsed=world.find_actor(31)->action_elapsed_seconds;
    const auto rng_before_spell_apply=world.random_state();
    check(rng_before_spell_apply.seed==rng_after_spell.seed&&rng_before_spell_apply.calls==rng_after_spell.calls,
        "Runtime fixture changed RNG after calculating the source spell result");
    DamageEvent receipt;
    check(runtime.apply_calculated_hit(31,32,"source-spell:Spell_Fire_2","cast-hit#0",
        spell_result.damage,spell_result.original.outcomes,spell_result.original.mask,receipt,error),error);
    const float initial_enemy_health=enemy_actor.health;
    const float spell_removed=std::min(initial_enemy_health,spell_result.damage);
    check(receipt.applied&&receipt.source_id=="source-spell:Spell_Fire_2"
        &&receipt.marker_name=="cast-hit#0"&&receipt.source_outcomes==spell_result.original.outcomes
        &&receipt.source_mask==spell_result.original.mask&&receipt.requested_damage==spell_result.damage
        &&receipt.health_removed==spell_removed&&world.find_actor(32)->health==initial_enemy_health-spell_removed,
        "Calculated source spell result did not apply its exact basic damage/full payload once");
    check(combat.attacking(31)&&combat.attacking(32)&&world.find_actor(31)->target_id==32
        &&world.find_actor(32)->target_id==31&&victim_visual.selected=="attack"
        &&attacker_visual.selected=="attack"&&attacker_visual.elapsed==attacker_cursor
        &&world.find_actor(31)->action_elapsed_seconds==attacker_elapsed,
        "Non-Injure calculated spell changed either attack or the attacker clock/cursor");
    check(world.random_state().seed==rng_before_spell_apply.seed&&world.random_state().calls==rng_before_spell_apply.calls,
        "Calculated spell application consumed the source RNG");

    check(world.bind_source("original-main",{},error),error);
    std::uint32_t injury_seed=0;float melee_damage=0;
    std::optional<std::uint32_t> melee_outcomes,melee_mask;
    for(std::uint32_t seed=1;seed<=50000&&!injury_seed;++seed){
        random={seed,0};float candidate_damage=0;
        std::optional<std::uint32_t> candidate_outcomes,candidate_mask;
        check(world.resolve_damage_with_outcomes("original-main",*world.find_actor(31),*world.find_actor(32),
            "melee-probe",candidate_damage,candidate_outcomes,candidate_mask,error),error);
        if(candidate_damage>0&&candidate_damage<world.find_actor(32)->health
           &&candidate_outcomes&&(*candidate_outcomes&0x10u)){
            injury_seed=seed;melee_damage=candidate_damage;melee_outcomes=candidate_outcomes;melee_mask=candidate_mask;
        }
        world.take_resolutions();
    }
    check(injury_seed!=0,"Original equipped source melee fixture produced no bounded Injure result");
    random={injury_seed,0};
    check(world.resolve_damage_with_outcomes("original-main",*world.find_actor(31),*world.find_actor(32),
        "attack_mainhand",melee_damage,melee_outcomes,melee_mask,error),error);
    world.take_resolutions();
    const auto rng_after_melee=world.random_state();
    check(melee_outcomes&&(*melee_outcomes&0x10u)&&melee_mask,
        "Selected original melee result lost its source Injure/mask fields");
    check(runtime.apply_calculated_hit(31,32,"original-main","melee-hit#0",melee_damage,
        melee_outcomes,melee_mask,receipt,error),error);
    check(receipt.applied&&receipt.source_outcomes==melee_outcomes&&receipt.source_mask==melee_mask
        &&receipt.health_removed==std::min(world.find_actor(32)->health+receipt.health_removed,melee_damage)
        &&!combat.attacking(32)&&victim_visual.selected=="react"
        &&world.find_actor(32)->action==CharacterAction::hurt,
        "Actual original source Injure result did not reach shared runtime admission/reaction");
    check(combat.attacking(31)&&world.find_actor(31)->target_id==32
        &&world.random_state().seed==rng_after_melee.seed&&world.random_state().calls==rng_after_melee.calls,
        "Actual calculated melee application changed attacker state or consumed RNG");

    // The source gate suppresses repeat Injury for 3000 ms. After the reaction
    // ends, an active victim attack survives a second source Injure occurrence.
    check(runtime.update(.11,ordinary_events,error),error);
    check(!runtime.owns_pose(32)&&world.find_actor(32)->action==CharacterAction::idle,
        "Actual source reaction clip did not finish");
    world.find_actor(32)->health=world.find_actor(32)->max_health;
    check(runtime.begin(32,31,attack,error),error);
    const auto victim_attack_cursor=victim_visual.elapsed;
    check(runtime.apply_calculated_hit(31,32,"original-main","melee-hit#1",melee_damage,
        melee_outcomes,melee_mask,receipt,error),error);
    check(combat.attacking(32)&&runtime.owns_pose(32)&&victim_visual.selected=="attack"
        &&world.find_actor(32)->target_id==31&&victim_visual.elapsed==victim_attack_cursor,
        "Actual source Injury gate failed to suppress repeat reaction during attack");
    check(runtime.update(2.9,ordinary_events,error),error);
    check(combat.attacking(32),"Long source gate tick unexpectedly replaced the victim attack");
    world.find_actor(32)->health=world.find_actor(32)->max_health;
    check(runtime.apply_calculated_hit(31,32,"original-main","melee-hit#2",melee_damage,
        melee_outcomes,melee_mask,receipt,error),error);
    check(!combat.attacking(32)&&victim_visual.selected=="react"
        &&world.find_actor(32)->action==CharacterAction::hurt,
        "Actual source Injury was not admitted after the shared 3000 ms gate expired: attacking="
        +std::to_string(combat.attacking(32))+" selected="+victim_visual.selected+" action="
        +std::to_string(int(world.find_actor(32)->action))+" health="
        +std::to_string(world.find_actor(32)->health)+" damage="+std::to_string(melee_damage));

    // Reuse an actual calculated spell payload as a distinct lethal occurrence
    // after setting only the test actor's current HP; source stats/result remain
    // the authored values above.
    world.find_actor(32)->health=std::max(0.01f,spell_result.damage*.5f);
    const float lethal_before=world.find_actor(32)->health;
    const auto rng_before_death=world.random_state();
    check(runtime.apply_calculated_hit(31,32,"source-spell:Spell_Fire_2","cast-hit#1",
        spell_result.damage,spell_result.original.outcomes,spell_result.original.mask,receipt,error),error);
    check(receipt.target_died&&receipt.health_removed==lethal_before&&!world.find_actor(32)->alive()
        &&runtime.owns_pose(32)&&victim_visual.selected=="death"
        &&world.find_actor(32)->action==CharacterAction::dead,
        "Actual calculated spell payload did not enter the canonical death path");
    check(world.random_state().seed==rng_before_death.seed&&world.random_state().calls==rng_before_death.calls,
        "Lethal calculated-hit application rerolled the source result");
    std::cout<<"PASS actual Spell_Fire_2 and original melee calculations -> same live PlayableActorWorld -> ActorCombatRuntime application; RNG stable, full payload retained, source Injury gate/reaction and death path\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
