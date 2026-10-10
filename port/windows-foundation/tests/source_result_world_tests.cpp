#include "../playable_actor_world.hpp"
#include "../asset_catalog.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
using namespace dh::foundation;
static void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
static ActorState actor(ActorId id,const char* name,const OriginalCombatProperties& p){
    ActorState a;a.id=id;a.definition_id=name;a.faction_id=p.sheets.resolved[0];
    a.health=original_signed256(p.sheets.resolved[36]);a.max_health=original_signed256(p.sheets.resolved[38]);
    a.resource=original_signed256(p.sheets.resolved[41]);a.max_resource=original_signed256(p.sheets.resolved[43]);return a;
}
int main(int argc,char** argv){try{
    check(argc==3,"Supply property/AI asset roots");AssetCatalog properties(argv[1]),population(argv[2]);
    const std::string root="original-cache/data/pydata/";std::string error;
    OriginalPropertyDatabase db;dh2::data::AiTables tables;
    check(load_original_property_tables(properties,root,db,error),error);
    check(load_original_ai_tables(population,root,tables,error),error);
    OriginalCombatProperties knight,lizard;
    check(build_original_combat_properties(db,"KnightPlayerBase",{256,true},{},{},knight,error),error);
    check(build_original_combat_properties(db,"Swamp_LizadMan_Type1",{std::nullopt,true},{},{},lizard,error),error);
    dh2::data::CombatRandom random{17,0},expected=random;PlayableActorWorld world(tables,random);
    check(world.bind_actor(actor(1,"KnightPlayerBase",knight),knight,{true,true,std::nullopt},error),error);
    check(world.bind_actor(actor(2,"Swamp_LizadMan_Type1",lizard),lizard,{false,true,std::nullopt},error),error);
    world.find_actor(2)->transform.position={1000,0,0};
    check(!world.original_melee_in_range(1,2),"Fixture unexpectedly in melee range");
    const auto view=[](const OriginalCombatProperties& p){const auto& f=p.facts;
        return dh2::data::CombatantView{p.sheets.resolved.data(),f.main_damage_class,f.off_damage_class,
            unsigned(f.two_hander),unsigned(f.dual_wield),unsigned(f.shield),f.original_state,f.combo_hits};};
    const auto a=view(knight),d=view(lizard);
    const auto prefix=dh2_combat_random(&expected,13);dh2::data::CombatResult source;
    const dh2::data::CombatResultRequest request{&a,&d,&expected,0x1005554au,-1,0,0};
    check(dh2_combat_result(&source,&request)==0,"Source result failed");
    const auto sound=dh2_combat_random(&expected,17),suffix=dh2_combat_random(&expected,23);
    unsigned notifications=0;std::uint32_t actualSound{};OriginalMeleeResolution actual;
    const float oldHealth=world.find_actor(2)->health;
    world.set_resolution_observer([&](const PlayableCombatResolution& event,std::string& e){
        ++notifications;check(event.source_id=="source-spell-query"&&event.marker_name=="authored-cast","Source result identity lost");
        check(world.find_actor(2)->health==oldHealth,"Calculation applied health before lifecycle");
        return world.random_uniform(17,actualSound,e);
    });
    check(world.with_loot_random([&](auto& loan,std::string& e){
        std::int32_t first{},last{};
        check(dh2_loot_v2_random(&loan,13,&first)==0&&first==prefix,"Prefix mismatch");
        if(!world.resolve_source_result("source-spell-query",1,2,"authored-cast",request.mask,
            request.weapon_category,request.element,request.direct_amount,actual,e))return false;
        check(dh2_loot_v2_random(&loan,23,&last)==0&&last==suffix,"Suffix mismatch");return true;
    },error),error);
    check(random.seed==expected.seed&&random.calls==expected.calls&&actualSound==std::uint32_t(sound),"Shared result/RNG/audio order differs");
    check(notifications==1&&actual.original.amount==source.amount&&actual.original.outcomes==source.outcomes&&
        actual.original.mask==source.mask&&actual.original.element==source.element&&
        actual.original.dot_amount==source.dot_amount&&actual.original.dot_duration==source.dot_duration&&
        actual.original.hp_leech==source.hp_leech&&actual.original.mp_leech==source.mp_leech,"Full source payload differs");
    const auto events=world.take_resolutions();check(events.size()==1&&events[0].attacker==1&&events[0].victim==2,"Resolution stream mismatch");
    check(world.find_actor(2)->health==oldHealth,"Result calculation independently applied damage");
    // A source spell applies its class to the same caster's scratch formula
    // sheet. Ensure calculation borrows that sheet without publishing it as
    // permanent player properties or silently using the base class instead.
    auto formula=knight.sheets.resolved;formula[172]=256;
    const auto classIt=std::find(db.classes.names.begin(),db.classes.names.end(),"Spell_Fire_2");
    check(classIt!=db.classes.names.end(),"Original spell class absent");
    check(dh2::data::apply_class(db.classes,int(classIt-db.classes.names.begin()),formula,error,
        &knight.sheets.resolved),error);
    std::cout<<"Source Spell_Fire_2 class="<<int(classIt-db.classes.names.begin())<<" changed properties:";
    for(std::size_t i=0;i<formula.size();++i)if(formula[i]!=knight.sheets.resolved[i])
        std::cout<<' '<<i<<'='<<knight.sheets.resolved[i]<<"->"<<formula[i];
    std::cout<<'\n';
    auto formulaView=a;formulaView.properties=formula.data();
    world.set_resolution_observer({});unsigned changedByClass=0;
    for(unsigned seed=1;seed<=32;++seed){
        random={seed,0};auto projectedExpected=random,baseExpected=random;
        dh2::data::CombatResult spell,baseSpell;
        const dh2::data::CombatResultRequest spellRequest{&formulaView,&d,&projectedExpected,0x1005554au,-1,0,0};
        const dh2::data::CombatResultRequest baseRequest{&a,&d,&baseExpected,0x1005554au,-1,0,0};
        check(dh2_combat_result(&spell,&spellRequest)==0&&dh2_combat_result(&baseSpell,&baseRequest)==0,
            "Source formula-sheet request failed");
        if(seed==1)std::cout<<"Source scratch/base amount="<<spell.amount<<'/'<<baseSpell.amount
            <<" outcomes="<<spell.outcomes<<'/'<<baseSpell.outcomes<<'\n';
        OriginalMeleeResolution spellResult;
        check(world.resolve_source_result("source-spell-class",1,2,"authored-cast",spellRequest.mask,-1,0,0,
            spellResult,error,&formula),error);
        const auto projectedActual=world.random_state();
        check(projectedActual.seed==projectedExpected.seed&&projectedActual.calls==projectedExpected.calls&&
            spellResult.original.amount==spell.amount&&spellResult.original.outcomes==spell.outcomes,
            "Spell calculation ignored source class formula sheet");
        if(spell.amount!=baseSpell.amount||spell.outcomes!=baseSpell.outcomes)++changedByClass;
    }
    // The original F_SpellAttack mask does not set CF_SetCombatants' skill bit
    // 0x08000000. This exact request therefore reads normal 79/80 damage,
    // rather than the Fire class's 174/175 fields. Equality is source-consistent.
    check(changedByClass==0,"Original spell mask unexpectedly read the skill-only damage fields");
    check(world.combat_properties(1)->sheets.resolved==knight.sheets.resolved&&
        world.find_actor(2)->health==oldHealth,"Scratch formula changed permanent actor state");
    const auto before=random;actual.damage=123;
    check(!world.resolve_source_result("source-spell-query",1,999,"authored-cast",request.mask,-1,0,0,actual,error)&&
        actual.damage==123&&random.seed==before.seed&&random.calls==before.calls,"Rejected target mutated output/RNG");
    std::cout<<"PASS source skill/spell result on original same-world sheets, full outcomes, loan/audio RNG order, no melee substitution, lifecycle-only health application, rejected target preservation\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
