#include "../combat_session.hpp"
#include <iostream>
#include <stdexcept>
#include <cmath>
using namespace dh::foundation;
static void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
int main(int argc,char** argv){try{
    check(argc==2,"Supply original shared asset root");AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase db;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",db,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan playerPlan;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"source-hit-player",playerPlan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=17;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=playerPlan.config;
    CombatSessionProfile player;player.action={"AttackStatic",0,{0,1}};player.initialIdle={"Idle",0,{0}};
    player.damageMarkerNames={"attack_mainhand"};player.customization=customization;player.propertyOptions={256,true};
    config.profiles.emplace("KnightPlayerBase",player);
    CombatSessionProfile enemy;enemy.action={"Attack",0,{0,1}};enemy.initialIdle={"Idle",0,{0}};
    enemy.death=CombatSessionChoice{"Died",0,{0}};enemy.damageMarkerNames={"attack_mainhand"};
    enemy.customization=customization;enemy.motionRoot="auto";enemy.propertyOptions={std::nullopt,true};
    config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;placed.definition.sourceId="source-hit-enemy";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,17,1};placed.transform=placed.definition.placement;
    population.actors().push_back(std::move(placed));CharacterVisual visual;CombatSession session;
    check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
    WorldObject chest;chest.id=4308955945491066525ull;chest.name="_prim_OpenableContainer_03";
    chest.transform.position={-7013.42f,12982.703f,250};
    chest.visual.model="data/3D/GameObjects/go_chest_swamp.bdae";
    check(session.world()->bind_object(chest,error),error);
    CombatSessionSourceHit hit;hit.attacker=1;hit.target=2;hit.binding_lease=session.actor_binding_lease();
    hit.source_id="source-spell-event";hit.marker_name="cast-hit";hit.generation=1;hit.mask=0x1005554au;
    hit.element=0;DamageEvent first;
    const auto before=session.world()->random_state();const float health=session.actor(2)->health;
    check(session.apply_source_result(hit,first,error)&&first.applied,error);
    check(first.source_mask==hit.mask&&first.source_outcomes.has_value()&&
        session.actor(2)->health==health-first.health_removed,"Source event did not apply exact result");
    const auto after=session.world()->random_state();check(after.calls>before.calls,"Source calculation did not use shared RNG");
    DamageEvent duplicate;
    check(session.apply_source_result(hit,duplicate,error)&&!duplicate.applied,error);
    check(session.world()->random_state().seed==after.seed&&session.world()->random_state().calls==after.calls&&
        session.actor(2)->health==health-first.health_removed,"Duplicate event rerolled or damaged again");
    InputActions input;check(session.update(0,input,{0,0,0},0,error),error);
    check(session.events().size()==1&&session.resolutions().size()==1&&
        session.events().front().requested_damage==first.requested_damage,"Pre-frame source receipt was lost at update");
    check(session.update(0,input,{0,0,0},0,error)&&session.events().empty()&&session.resolutions().empty(),
        "Source receipt duplicated on next frame");
    // Source F_DotAttack request mask carries an explicit fixed amount. This
    // boundary test supplies the current HP to exercise a lethal source event;
    // it does not define a production damage value or a timer-based hit.
    hit.source_id="source-dot-event";hit.generation=2;hit.mask=0x20080000u;
    hit.direct_amount=static_cast<std::int32_t>(std::ceil(session.actor(2)->health*256.0f));
    DamageEvent lethal;check(session.apply_source_result(hit,lethal,error)&&lethal.target_died,error);
    check(session.owns_pose(2),"Lethal source event did not enter same death runtime");
    const auto deadRng=session.world()->random_state();
    check(session.apply_source_result(hit,duplicate,error)&&!duplicate.applied,error);
    check(session.world()->random_state().seed==deadRng.seed&&session.world()->random_state().calls==deadRng.calls,
        "Dead target replay rerolled");
    // Source Celest's Lua calls SpellCombatRoll a second time even when its
    // preceding call killed this retained target. Compare the calculation-only
    // continuation with the actual original provider; health/death stay held.
    auto second=hit;second.event_index=1;second.mask=0x1005554au;second.direct_amount=0;
    OriginalMeleeDamageProvider original;
    check(original.bind_actor(1,*session.world()->combat_properties(1),error)&&
          original.bind_actor(2,*session.world()->combat_properties(2),error),error);
    auto expectedRng=deadRng;OriginalMeleeResolution expected;
    check(original.resolve_result(1,2,second.mask,second.category,second.element,0,expectedRng,expected,error),error);
    unsigned presentations=0;
    session.world()->set_resolution_observer([&](const auto&,std::string&)->bool{++presentations;throw std::runtime_error("fixture source text unavailable");});
    const auto corpse=*session.actor(2);CombatSessionSourceCalculation calculation;
    check(session.resolve_source_result_only(second,calculation,error)&&calculation.calculated,error);
    check(calculation.result.original.amount==expected.original.amount&&
          calculation.result.original.outcomes==expected.original.outcomes&&
          calculation.result.original.mask==expected.original.mask&&
          session.world()->random_state().seed==expectedRng.seed&&session.world()->random_state().calls==expectedRng.calls,
          "Dead target source calculation changed formula/payload/RNG");
    check(session.actor(2)->health==corpse.health&&session.actor(2)->action==corpse.action&&
          session.actor(2)->action_elapsed_seconds==corpse.action_elapsed_seconds&&session.owns_pose(2),
          "Calculation-only continuation changed dead target/pose");
    check(presentations==1&&session.world()->take_resolution_observer_errors()==std::vector<std::string>{"fixture source text unavailable"},
          "Source presentation failure vetoed or repeated calculation");
    check(session.resolve_source_result_only(second,calculation,error)&&!calculation.calculated&&
          session.world()->random_state().seed==expectedRng.seed&&session.world()->random_state().calls==expectedRng.calls,
          "Calculation-only duplicate rerolled");
    check(session.apply_source_result(second,duplicate,error)&&!duplicate.applied,
          "Calculated occurrence was admitted again as health damage");
    check(session.update(0,input,{0,0,0},0,error)&&session.events().size()==1&&
          session.events().front().target_died&&session.resolutions().size()==2,
          "Calculation-only result published damage or lost reached resolution");
    session.world()->set_resolution_observer({});
    const auto foreignOwner=std::make_shared<const int>(1);
    auto foreign=hit;foreign.binding_lease=foreignOwner;
    check(!session.apply_source_result(foreign,duplicate,error),"Live foreign binding accepted");
    auto foreignCalculation=second;foreignCalculation.binding_lease=foreignOwner;
    check(!session.resolve_source_result_only(foreignCalculation,calculation,error),"Foreign source calculation accepted");
    const auto oldLease=hit.binding_lease;session.detach_for_restore();
    check(!session.apply_source_result(hit,duplicate,error)&&oldLease.expired(),"Detached source owner accepted work");
    check(!session.resolve_source_result_only(second,calculation,error),"Detached source calculation accepted");
    check(session.rebind_after_restore(error),error);
    check(session.world()->find_object(chest.id)&&!session.actor(chest.id)&&
          !session.world()->combat_properties(chest.id),"Neutral subset broke Session roster/rebind");
    check(!session.apply_source_result(hit,duplicate,error),"Pre-restore source owner accepted work after rebind");
    check(!session.resolve_source_result_only(second,calculation,error),"Pre-restore source calculation accepted");
    std::cout<<"PASS actual Session source calculation/application, full result, once-only RNG/damage, next-frame receipts, death, foreign/detached/restored lease rejection\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
