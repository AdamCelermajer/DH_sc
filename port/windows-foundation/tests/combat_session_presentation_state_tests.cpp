#include "../combat_session.hpp"
#include "../game_save.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
void run(const char* root,std::int32_t explicit_state){
    AssetCatalog assets(root);OriginalPropertyDatabase database;OriginalMeleeBindings bindings;
    ActorCustomization customization;customization.allow_missing_animation_targets=true;std::string error;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    OriginalCombatVisualPlan plan;check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",
        customization,"presentation-state-player",plan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;config.playerProfileId="KnightPlayerBase";
    config.tableRoot="original-cache/data/pydata";config.playerVisualConfig=plan.config;
    config.mainItemId="Longsword01";config.equippedItemIds={"Longsword01"};
    CombatSessionProfile player;player.action={"AttackStatic",0,{0,1}};player.initialIdle={"Idle",0,{0}};
    player.damageMarkerNames={"attack_mainhand"};player.propertyOptions={256,true};player.customization=customization;
    config.profiles.emplace("KnightPlayerBase",player);
    CombatSessionProfile presentation;presentation.animationOnly=true;presentation.initialIdle={"Idle",0,{0}};
    presentation.propertyOptions={std::nullopt,false};presentation.customization=customization;
    presentation.originalCombatState=explicit_state;
    config.profiles.emplace("Swamp_LizadMan_Type1",presentation);
    ActorPopulation population;PopulationActor npc;npc.profileId="Swamp_LizadMan_Type1";
    npc.definition.stableId=2;npc.definition.sourceId="presentation-lizard";
    npc.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};npc.transform=npc.definition.placement;
    population.actors().push_back(std::move(npc));CharacterVisual visual;CombatSession session;
    check(session.initialize(assets,database,bindings,config,visual,population,{0,0,0},customization,error),error);
    const auto expected=explicit_state==-1?3:explicit_state;
    const auto* source=bindings.find_actor("Swamp_LizadMan_Type1");check(source,"Original presentation source row missing");
    OriginalCombatFacts facts;facts.original_state=expected;OriginalCombatProperties reference;
    check(build_original_combat_properties(database,database.characters.names[source->propertyRow],
        presentation.propertyOptions,{},facts,reference,error),error);
    check(session.world()->combat_properties(2)->sheets.resolved==reference.sheets.resolved,
          "Initial state publication changed authored property/vital values");
    check(!session.world()->traits(2)->targetable&&session.actor(2)->attack_ids.empty(),
          "Presentation state publication added gameplay capability");
    check(session.original_actor_state(2)==expected&&session.world()->combat_properties(2)->facts.original_state==expected,
          "Prepared presentation state disagreed with canonical World state");
    const auto random=session.world()->random_state();
    session.actor(2)->health=0;session.actor(2)->action=CharacterAction::dead;
    check(session.update(0,{},Vec3{0,0,0},0,error),error);
    check(session.original_actor_state(2)==expected&&session.world()->combat_properties(2)->facts.original_state==expected,
          "Presentation zero-HP/action projection inferred a source death");
    if(explicit_state==-1){
        unsigned transitions=0;
        check(session.bind_reconstructible_actor_transition_handler(
            [&](const auto&,std::string& e){++transitions;e.clear();return true;},
            [](std::string& e){e.clear();return true;},error),error);
        session.actor(1)->persistent_character_id="presentation-state";
        auto character=make_default_character("presentation-state","Test","warrior");
        character.stats.health=session.actor(1)->health;character.stats.max_health=session.actor(1)->max_health;
        character.stats.resource=session.actor(1)->resource;character.stats.max_resource=session.actor(1)->max_resource;
        GameSave saved;check(capture_game_save("presentation-state",1,character,*session.world(),saved,error),error);
        session.detach_for_restore();check(restore_game_save(saved,"presentation-state",*session.world(),character,error),error);
        check(session.rebind_after_restore(error),error);
        check(session.bind_reconstructible_actor_transition_handler(
            [&](const auto&,std::string& e){++transitions;e.clear();return true;},
            [](std::string& e){e.clear();return true;},error),error);
        check(session.update(0,{},Vec3{0,0,0},0,error),error);
        check(session.original_actor_state(2)==3&&session.world()->combat_properties(2)->facts.original_state==3&&transitions==0,
              "Presentation restore lost canonical Idle or replayed transition effects");
    }
    check(session.world()->random_state().seed==random.seed&&session.world()->random_state().calls==random.calls,
          "Presentation initialization/restore invented a gameplay RNG draw");
}
}
int main(int argc,char** argv){try{
    check(argc==2,"Supply original assets");run(argv[1],-1);run(argv[1],17);
    std::cout<<"PASS actual presentation Idle canonical publication, authored vitals/capabilities, zero-HP/state independence, explicit-state preservation and silent restore\n";
    return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
