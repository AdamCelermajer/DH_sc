// v21.8 mage: session-level coverage of the ranged (Staff01) player attack.
//
// Original: CharAI::AI_DoRangeAttack (0x3d076c) does LookAt(target) and SM_SetAttackState with no melee-range gate
// (docs/ida-fidelity-audit-2026-10-11/weapon-range.md); the state-5 projectile marker is not a melee hit.
// Covered here (the earlier Mage work verified these only in real runs):
//   - a ranged owner (rangedProjectileMarkers) begins its staff attack on a target far outside melee reach (the melee owners keep\r\n//     their range gate; that is covered by combat_session_source_attack_bank and combat_system);
//   - the player turns toward that far target (LookAt), a melee owner would not;
//   - the state-5 marker is queued per actor (take_projectile_markers) and no melee damage is applied;
//   - the queue is drained once and is empty for other actors.
#include "../combat_session.hpp"
#include "../content_paths.hpp"
#include "../features/combat/runtime_player_combo_chain_v1.hpp"
#include "../../script-runtime/script_constants.hpp"
#include "../../level-world/navigation_heading.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
}
int main(int argc,char** argv){try{
    check(argc==2,"Original shared asset root required");AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase db;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",db,error)&&bindings.load(assets,"original-melee-bindings.xml",error),error);
    const auto read=[&](const char* name){return read_content(assets,std::string("data/pydata/")+name);};
    const auto bytes=[](const auto& buffer){return dh2::data::Bytes{buffer.data(),buffer.size()};};
    const auto clipNames=read("animations_dictionary_pyarraynames.bin"),clipValues=read("animations_dictionary_pyarray.bin");
    dh2::data::Dictionary clips;check(dh2::data::load_dictionary(bytes(clipNames),bytes(clipValues),clips,error),error);
    const auto records=read("animations_pyarray.bin"),names=read("animations_pyarraynames.bin"),fields=read("animations_pystructnames.bin");
    dh2::data::AnimationTables animations;check(dh2::data::load_animation_tables(bytes(records),bytes(names),bytes(fields),clips,animations,error),error);
    const auto itemRecords=read("loot_table_pyarray.bin"),itemNames=read("loot_table_pyarraynames.bin"),itemFields=read("loot_table_pystructnames.bin");
    dh2::data::ItemTable items;check(dh2::data::load_items(bytes(itemRecords),bytes(itemNames),bytes(itemFields),items,error),error);
    const auto constantsBytes=read_content(assets,"data/animations_pycst.bin");
    auto* constants=dh2_script_constants_create();check(constants,"Source constants unavailable");dh2_script_constants_reload reload{};
    check(dh2_script_constants_load(constants,constantsBytes.data(),static_cast<std::uint32_t>(constantsBytes.size()),&reload)==0,"Source constants failed");
    equipment_menu::RuntimePlayerLocomotionConstantsV1 stance;
    check(equipment_menu::load_runtime_player_locomotion_constants_v1([&](const char* group,const char* key,std::int32_t& value,std::string& e){
        if(dh2_script_constants_get(constants,group,key,&value)){e="Missing source constant";return false;}return true;
    },stance,error),error);dh2_script_constants_destroy(constants);
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan base;check(build_original_combat_visual_plan(assets,bindings,"MagePlayerBase",customization,"ranged-base",base,error),error);
    equipment_menu::RuntimePlayerLocomotionV1 equipped;
    const auto staff=dh2::data::item_id(items,"Staff01");check(staff>=0,"Original Staff01 missing");
    check(equipment_menu::resolve_runtime_player_locomotion_v1(49,items,staff,-1,0,stance,animations,clips,equipped,error),error);
    combat::RuntimePlayerComboChainPlanV1 attack;
    check(combat::plan_runtime_player_combo_chain_v1(assets,equipped,stance.stanced_list_mask,3,true,animations,clips,
        "MagePlayerBase",base.config,"source-staff-bank",attack,error),error);
    std::cout<<"Staff stance="<<equipped.stance<<" sequence="<<attack.selected_sequence_id<<'\n';

    const auto run=[&](float targetX,float targetY,unsigned& markers,float& heading,float& expectedHeading,
                       float& targetHealthBefore,float& targetHealthAfter,bool& attackBegan,bool tab=true,ActorId* targetAfterAttack=nullptr){
        CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;config.playerProfileId="MagePlayerBase";
        config.tableRoot="original-cache/data/pydata";config.playerVisualConfig=base.config;
        config.playerVisualConfig.motion_node_id="auto";config.playerVisualConfig.consume_root_motion=true;
        config.mainItemId="Staff01";
        config.equippedItemIds={"StartingSuitMage","StartingBootsMage","StartingGlovesMage","Staff01"};
        CombatSessionProfile player;player.initialIdle={"Idle",0,{0}};player.propertyOptions={256,true};
        player.reaction=CombatSessionChoice{"Injured",0,{0}};player.death=CombatSessionChoice{"Died",0,{0}};
        player.sequenceAction=attack.selection;player.retainedPhaseClock=true;player.sourceCombo=true;
        player.sourceAttackBank=attack.visual;player.sourceAttackPolicies=attack.sequence_policies;
        player.rangedProjectileMarkers=true;
        config.profiles.emplace(config.playerProfileId,player);
        CombatSessionProfile enemy;enemy.action={"Attack",0,{0,1}};enemy.initialIdle={"Idle",0,{0}};
        enemy.propertyOptions={20*256,true};enemy.damageMarkerNames={"attack_mainhand"};enemy.customization=customization;
        config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
        ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";placed.definition.stableId=2;
        placed.definition.sourceId="ranged-attack-lizard";placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,targetX,targetY,0,1};
        placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
        CharacterVisual visual;CombatSession session;
        check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
        InputActions input;
        if(tab){
            input.targetSelect=true;check(session.update(0,input,{0,0,0},0,error),error);
            check(session.actor(1)->target_id==2,"Target select did not pick the far lizard");
        }
        targetHealthBefore=session.actor(2)->health;
        input.targetSelect=false;input.attack=true;check(session.update(1.0/60,input,{0,0,0},0,error),error);
        attackBegan=session.owns_pose(1);
        if(targetAfterAttack)*targetAfterAttack=session.actor(1)->target_id;
        markers=0;
        for(unsigned i=0;i<240&&session.owns_pose(1);++i){
            input.attack=false;
            check(session.update(1.0/60,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
            for(const auto& marker:session.take_projectile_markers(1)){
                check(marker.name=="do_skill"||marker.name=="attack_ranged","Unexpected projectile marker "+marker.name);++markers;
            }
            check(session.take_projectile_markers(2).empty(),"Another actor's queue was not empty");
        }
        heading=session.actor(1)->transform.rotation[2];
        float direction[3]{targetX,targetY,0};float expected=0;
        check(dh2_nav_look_towards(&expected,direction)==0,"Reference LookAt heading failed");expectedHeading=expected;
        targetHealthAfter=session.actor(2)->health;
        check(session.take_projectile_markers(1).empty(),"Marker queue was not drained");
    };
    unsigned markers=0;float heading=0,expected=0,before=0,after=0;bool began=false;
    // Far target (600 units, well outside melee reach) off the initial heading.
    run(600.f,300.f,markers,heading,expected,before,after,began);
    std::cout<<"ranged: began="<<began<<" markers="<<markers<<" heading="<<heading<<" expected="<<expected<<" health "<<before<<"->"<<after<<'\n';
    check(began,"Ranged owner did not begin its staff attack on a far target");
    check(markers>=1,"Ranged owner queued no state-5 projectile marker");
    check(std::fabs(heading-expected)<0.05f,"Ranged owner did not LookAt its far target");
    check(after==before,"Ranged attack applied a melee hit at a marker");
    // v21.8 magefix2: AI_DoRangeAttack with NO explicit target searches the weapon range (Staff01 ItemTable max range 1000):
    // an enemy at 600 is auto-targeted and faced (AI_SetTarget + LookAt); one at 1500 is out of range, the staff attack
    // still starts and fires along the look vector (SM_SetAttackState has no target gate).
    {
        ActorId autoTarget=invalid_actor_id;
        run(600.f,300.f,markers,heading,expected,before,after,began,false,&autoTarget);
        std::cout<<"ranged no-Tab in range: began="<<began<<" markers="<<markers<<" target="<<autoTarget<<" heading="<<heading<<" expected="<<expected<<'\n';
        check(began&&markers>=1,"Ranged owner without a target did not attack an enemy within its weapon range");
        check(autoTarget==2,"Ranged owner did not auto-target the enemy within its weapon range");
        check(std::fabs(heading-expected)<0.05f,"Ranged owner did not face its auto-selected target");
        ActorId farTarget=2;
        run(1500.f,900.f,markers,heading,expected,before,after,began,false,&farTarget);
        std::cout<<"ranged no-Tab out of range: began="<<began<<" markers="<<markers<<" target="<<farTarget<<'\n';
        check(began&&markers>=1,"Ranged owner without a target must still attack (SM_SetAttackState has no target gate)");
        check(farTarget==invalid_actor_id,"Ranged owner auto-targeted an enemy beyond its weapon range");
    }
    std::cout<<"combat_session_ranged_attack passed\n";
    return 0;
}catch(const std::exception& e){std::cerr<<"combat_session_ranged_attack failed: "<<e.what()<<'\n';return 1;}}
