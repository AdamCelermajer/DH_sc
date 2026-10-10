#include "../combat_session.hpp"
#include "../game_save.hpp"
#include "../content_paths.hpp"
#include "../features/combat/runtime_player_combo_chain_v1.hpp"
#include "../features/combat/runtime_player_profile_attack_bank_v1.hpp"
#include "../../script-runtime/script_constants.hpp"
#include <iostream>
#include <set>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
bool same(const dh2::data::CombatResult& a,const dh2::data::CombatResult& b){
    return a.amount==b.amount&&a.dot_element==b.dot_element&&a.dot_duration==b.dot_duration&&
        a.dot_amount==b.dot_amount&&a.hp_leech==b.hp_leech&&a.mp_leech==b.mp_leech&&
        a.outcomes==b.outcomes&&a.mask==b.mask&&a.weapon_category==b.weapon_category&&a.element==b.element;
}
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
    OriginalCombatVisualPlan base;check(build_original_combat_visual_plan(assets,bindings,"RoguePlayerBase",customization,"bank-base",base,error),error);
    equipment_menu::RuntimePlayerLocomotionV1 equipped;
    const auto dagger=dh2::data::item_id(items,"Dagger01");check(dagger>=0,"Original dagger missing");
    check(equipment_menu::resolve_runtime_player_locomotion_v1(50,items,dagger,dagger,0,stance,animations,clips,equipped,error)&&equipped.stance==2,error);
    combat::RuntimePlayerComboChainPlanV1 attack;
    check(combat::plan_runtime_player_combo_chain_v1(assets,equipped,stance.stanced_list_mask,3,true,animations,clips,
        "RoguePlayerBase",base.config,"source-dual-bank",attack,error),error);
    check(attack.selected_sequence_id==245&&attack.visual.sequences[0].phases.size()==12,"Wrong source dual static graph");
    CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;config.playerProfileId="RoguePlayerBase";
    config.tableRoot="original-cache/data/pydata";config.playerVisualConfig=base.config;
    config.playerVisualConfig.motion_node_id="auto";config.playerVisualConfig.consume_root_motion=true;
    config.mainItemId=config.offItemId="Dagger01";
    config.equippedItemIds={"StartingSuitRogue","StartingBootsRogue","StartingGlovesRogue","Dagger01","Dagger01"};
    CombatSessionProfile player;player.initialIdle={"Idle",0,{0}};player.propertyOptions={256,true};
    player.reaction=CombatSessionChoice{"Injured",0,{0}};player.death=CombatSessionChoice{"Died",0,{0}};
    player.sequenceAction=attack.selection;player.retainedPhaseClock=true;player.sourceCombo=true;
    player.sourceAttackBank=attack.visual;player.sourceAttackPolicies=attack.sequence_policies;player.sourceMeleeHandMarkers=true;
    player.damageMarkerNames={"attack_mainhand","attack_offhand"};config.profiles.emplace(config.playerProfileId,player);
    CombatSessionProfile enemy;enemy.action={"Attack",0,{0,1}};enemy.initialIdle={"Idle",0,{0}};
    // Source level20 defender keeps the four-group continuity fixture alive;
    // source stats/formulas are unchanged. This is not Act1 balance evidence.
    enemy.propertyOptions={20*256,true};enemy.damageMarkerNames={"attack_mainhand"};enemy.customization=customization;
    config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";placed.definition.stableId=2;
    placed.definition.sourceId="source-attack-bank-lizard";placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
    placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
    CharacterVisual visual;CombatSession session;
    check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
    const auto* sourceSequence=session.attack_sequence(1);check(sourceSequence&&sourceSequence->phases().size()==12,"Bank was not installed as outgoing action");
    for(std::size_t i=0;i<12;++i)check(sourceSequence->phases()[i].source.sourceUri==attack.visual.sequences[0].phases[i].sourceUri,
        "Session substituted baseline 1H phase");
    const auto* actor=session.actor(1);const auto* samePose=session.retained_actor_pose(1);
    const auto lease=session.actor_binding_lease().lock();const auto gear=actor->equipment;
    auto expectedRng=session.world()->random_state();std::set<std::string> hands;unsigned hits=0;
    session.world()->set_resolution_observer([&](const PlayableCombatResolution& receipt,std::string& e){
        if(receipt.attacker!=1)return true;
        const bool left=receipt.marker_name=="attack_offhand";
        check(left||receipt.marker_name=="attack_mainhand","Unexpected source melee event");
        OriginalMeleeDamageProvider reference;OriginalMeleeResolution result;
        check(reference.bind_actor(1,*session.world()->combat_properties(1),e)&&reference.bind_actor(2,*session.world()->combat_properties(2),e)&&
            reference.bind_source("exact-source-hand",{left,false},e)&&reference.resolve("exact-source-hand",1,2,expectedRng,result,e),e);
        check(same(result.original,receipt.melee.original)&&result.damage==receipt.melee.damage&&
            expectedRng.seed==session.world()->random_state().seed&&expectedRng.calls==session.world()->random_state().calls,
            "Named hand event resolved wrong original formula or RNG");
        const auto suffix=left?"/offhand":"/mainhand";
        check(receipt.source_id==std::string("actor-1/melee")+suffix,"Named event retained profile-wide hand source");
        hands.insert(receipt.marker_name);++hits;return true;
    });
    InputActions input;input.targetSelect=true;check(session.update(0,input,{0,0,0},0,error),error);
    input.targetSelect=false;input.attack=true;check(session.update(0,input,{0,0,0},0,error),error);
    check(session.owns_pose(1),"Source Rogue attack did not begin");
    std::set<std::uint32_t> groups{0};const auto generation=session.combo_boundaries().front().generation;
    for(unsigned i=0;i<900&&session.owns_pose(1);++i){
        const auto* state=session.source_attack_state(1);check(state,"Source combo owner missing");input.attack=state->index<3;
        check(session.update(1.0/60,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
        for(const auto& boundary:session.combo_boundaries())if(boundary.depth==0&&boundary.beginning){
            groups.insert(boundary.step);check(boundary.generation==generation,"Held attack replaced source combo generation");
        }
        check(session.actor(1)==actor&&session.retained_actor_pose(1)==samePose&&session.actor_binding_lease().lock()==lease,
            "Attack replaced actor/pose/binding owner");
    }
    check(!session.owns_pose(1)&&hands==std::set<std::string>{"attack_mainhand","attack_offhand"}&&hits>1,
        "Authored dual chain did not finish or deliver both source hands");
    if(groups!=std::set<std::uint32_t>{0,1,2,3}){
        std::cerr<<"Observed groups:";for(auto group:groups)std::cerr<<' '<<group;
        std::cerr<<" hits="<<hits<<" targetHP="<<session.actor(2)->health<<'\n';
        throw std::runtime_error("Held attack did not advance four dual root groups");
    }
    check(session.actor(1)->equipment.size()==gear.size()&&session.actor(1)->target_id==2,"Attack lost gear or living target");
    auto character=make_default_character("source-bank-test","Rogue",config.playerProfileId);session.actor(1)->persistent_character_id=character.id;
    GameSave saved;check(capture_game_save("source-bank",1,character,*session.world(),saved,error),error);
    session.detach_for_restore();check(restore_game_save(saved,"source-bank",*session.world(),character,error)&&session.rebind_after_restore(error),error);
    const auto rng=session.world()->random_state();
    check(session.update(0,{},Vec3{0,0,0},0,error)&&session.world()->random_state().calls==rng.calls,"Rebind replayed a source hit");
    const auto* still=session.actor(1);const auto currentLease=session.actor_binding_lease().lock();
    const auto reject=[&](CombatSessionConfig invalid){
        check(!session.initialize(assets,db,bindings,invalid,visual,population,{0,0,0},customization,error),"Invalid source bank admitted");
        check(session.actor(1)==still&&session.actor_binding_lease().lock()==currentLease&&session.world()->random_state().calls==rng.calls,
            "Rejected source bank replaced live actor/lease/RNG");
    };
    auto invalid=config;invalid.profiles.at(config.playerProfileId).sourceAttackBank->profileId="KnightPlayerBase";reject(invalid);
    invalid=config;invalid.profiles.at(config.playerProfileId).sourceAttackBank->config.model_path="other.bdae";reject(invalid);
    invalid=config;invalid.profiles.at(config.playerProfileId).sourceAttackPolicies.at(245).type=2;reject(invalid);
    invalid=config;invalid.profiles.at(config.playerProfileId).sourceAttackBank->sequences[0].state="Idle";reject(invalid);
    invalid=config;invalid.profiles.at(config.playerProfileId).sourceAttackBank->config.clips.front().second="other.bdae";reject(invalid);
    invalid=config;invalid.profiles.at(config.playerProfileId).sourceAttackPolicies.clear();reject(invalid);
    invalid=config;invalid.profiles.at(config.playerProfileId).damageMarkerNames={"do_skill"};reject(invalid);
    invalid=config;invalid.profiles.at(config.playerProfileId).sourceAttackStateSelection=true;reject(invalid);

    // Real matching class/gear bank contains both source predecessor branches.
    ActorProfileLibrary profiles;check(profiles.load(assets,"actor-profiles-v2.xml",error),error);
    const auto* rogue=profiles.find(config.playerProfileId);check(rogue,"Actual Rogue profile missing");
    combat::RuntimePlayerProfileAttackBankPlanV1 both;
    check(combat::plan_runtime_player_profile_attack_bank_v1(assets,*rogue,equipped,stance.stanced_list_mask,
        animations,clips,base,"source-dual-dynamic",both,error),error);
    auto dynamic=config;auto& dynamicPolicy=dynamic.profiles.at(config.playerProfileId);
    dynamicPolicy.sourceAttackBank=both.bank;dynamicPolicy.sourceAttackPolicies=both.sequence_policies;
    dynamicPolicy.sequenceAction=both.static_selection;dynamicPolicy.sourceAttackStateSelection=true;
    for(const bool moving:{false,true}){
        check(session.initialize(assets,db,bindings,dynamic,visual,population,{0,0,0},customization,error),error);
        const auto* stableActor=session.actor(1);const auto* stablePose=session.retained_actor_pose(1);
        const auto stableLease=session.actor_binding_lease().lock();auto dynamicRng=session.world()->random_state();
        const auto& expectedRoot=*both.bank.sequence(moving?"Attack":"AttackStatic",0);
        const auto* initialAction=session.attack_sequence(1);
        check(initialAction->phases()[0].source.sourceUri==both.bank.sequence("AttackStatic",0)->phases[0].sourceUri,
            "Initial explicit static root was not preserved before commands");
        session.world()->set_resolution_observer([&](const PlayableCombatResolution& receipt,std::string& e){
            if(receipt.attacker!=1)return true;OriginalMeleeDamageProvider reference;OriginalMeleeResolution result;
            check(reference.bind_actor(1,*session.world()->combat_properties(1),e)&&reference.bind_actor(2,*session.world()->combat_properties(2),e)&&
                reference.bind_source("hand",{receipt.marker_name=="attack_offhand",false},e)&&
                reference.resolve("hand",1,2,dynamicRng,result,e),e);
            check(same(result.original,receipt.melee.original)&&dynamicRng.seed==session.world()->random_state().seed&&
                dynamicRng.calls==session.world()->random_state().calls,"Dynamic root changed original hand formula/RNG");return true;
        });
        InputActions command;command.targetSelect=true;check(session.update(0,command,{0,0,0},0,error),error);
        command.targetSelect=false;command.attack=true;if(moving)command.move2D.x=1;
        session.set_diagnostic_controller_admission_provider([](ActorId id,DiagnosticControllerAdmissionFacts& facts,std::string&){
            facts={id,id,0,1,0,0};return true;
        },[](ActorId,bool& online,std::string&){online=false;return true;});
        check(session.update(0,command,{0,0,0},0,error),error);
        check(!session.owns_pose(1)&&session.attack_sequence(1)==initialAction&&
            initialAction->phases()[0].source.sourceUri==both.bank.sequence("AttackStatic",0)->phases[0].sourceUri&&
            session.world()->random_state().calls==dynamicRng.calls,"Blocked command selected source attack root or consumed RNG");
        session.clear_diagnostic_controller_admission_provider();
        // Out-of-range command still must not perform source focus selection.
        const auto targetPosition=session.actor(2)->transform.position;session.actor(2)->transform.position[0]=999999;
        check(session.update(0,command,{0,0,0},0,error),error);
        check(!session.owns_pose(1)&&initialAction->phases()[0].source.sourceUri==both.bank.sequence("AttackStatic",0)->phases[0].sourceUri&&
            session.world()->random_state().calls==dynamicRng.calls,"Out-of-range command selected source root");
        session.actor(2)->transform.position=targetPosition;
        command.targetSelect=true;command.attack=false;check(session.update(0,command,{0,0,0},0,error),error);
        command.targetSelect=false;command.attack=true;
        check(session.update(0,command,{0,0,0},0,error)&&session.owns_pose(1),error);
        check(session.attack_sequence(1)==initialAction&&initialAction->phases()[0].source.sourceUri==expectedRoot.phases[0].sourceUri,
            "Source predecessor did not select exact moving/static root on stable action object");
        const auto actionGeneration=session.combo_boundaries().front().generation;
        // Change the input while held: a continuation must retain its root.
        command.move2D.x=moving?0.f:1.f;
        for(unsigned frame=0;frame<900&&session.owns_pose(1);++frame){
            command.attack=session.source_attack_state(1)->index<3;
            check(session.update(1.0/60,command,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
            check(session.actor(1)==stableActor&&session.retained_actor_pose(1)==stablePose&&session.actor_binding_lease().lock()==stableLease&&
                initialAction->phases()[0].source.sourceUri==expectedRoot.phases[0].sourceUri,
                "Held active input reselected source root or replaced pose/actor");
            for(const auto& boundary:session.combo_boundaries())check(boundary.generation==actionGeneration,"Dynamic held command restarted action");
        }
        check(!session.owns_pose(1),"Dynamic source action did not finish");
        std::cout<<"PASS admitted predecessor "<<(moving?4:3)<<" selects original root"<<expectedRoot.id<<
            "; blocked/range gates, held root and same validation/pose owner retained\n";
    }
    // Real selected save properties must precede full-combat vital validation.
    OriginalActorProperties fresh;check(resolve_original_fresh_player(db,config.playerProfileId,fresh,error),error);
    auto selected=make_default_character("selected-rogue-startup","Selected",config.playerProfileId);
    selected.stats.level=1;
    selected.stats.health=fresh.health-7.25f;selected.stats.max_health=fresh.max_health;
    selected.stats.resource=fresh.resource-2.5f;selected.stats.max_resource=fresh.max_resource;
    selected.stats.strength=original_signed256(fresh.sheets.resolved[149]);
    selected.stats.dexterity=original_signed256(fresh.sheets.resolved[150]);
    selected.stats.endurance=original_signed256(fresh.sheets.resolved[151]);
    selected.stats.energy=original_signed256(fresh.sheets.resolved[152]);
    selected.source_endurance_energy_known=selected.source_points_known=true;
    selected.experience=8;selected.source_stat_points=2;selected.source_skill_points=3;
    auto startup=dynamic;startup.selectedPlayerProfile=&selected;
    startup.profiles.at(config.playerProfileId).propertyOptions.refill_vitals=false;
    check(session.initialize(assets,db,bindings,startup,visual,population,{0,0,0},customization,error),error);
    const auto* selectedActor=session.actor(1);const auto* selectedProps=session.world()->combat_properties(1);
    check(selectedActor->health==selected.stats.health&&selectedActor->resource==selected.stats.resource&&
        selectedProps->sheets.resolved[36]==fresh.sheets.resolved[36]-7*256-64&&
        selectedProps->sheets.resolved[41]==fresh.sheets.resolved[41]-2*256-128,
        "Pre-bind selected vitals were refilled or source sentinels escaped");
    check(selectedProps->sheets.resolved[33]==8*256&&selectedProps->sheets.resolved[148]==2*256&&
        selectedProps->sheets.resolved[157]==3*256&&selectedActor->persistent_character_id==selected.id&&
        selectedActor->attack_ids.size()==1&&selectedActor->equipment.size()==5,
        "Pre-bind selected progression/identity or duplicate equipped occurrences lost");
    const auto stableSelectedLease=session.actor_binding_lease().lock();const auto selectedRandom=session.world()->random_state();
    const auto rejectProfile=[&](CharacterState bad){
        auto rejected=startup;rejected.selectedPlayerProfile=&bad;
        check(!session.initialize(assets,db,bindings,rejected,visual,population,{0,0,0},customization,error),
            "Invalid selected profile passed startup");
        check(session.actor(1)==selectedActor&&session.actor_binding_lease().lock()==stableSelectedLease&&
            session.actor(1)->health==selected.stats.health&&session.world()->random_state().seed==selectedRandom.seed&&
            session.world()->random_state().calls==selectedRandom.calls,"Invalid startup changed live Session or RNG");
    };
    auto bad=selected;bad.class_id="KnightPlayerBase";rejectProfile(bad);
    bad=selected;bad.stats.level=2;rejectProfile(bad);
    bad=selected;bad.stats.health=bad.stats.max_health+1;rejectProfile(bad);
    // initialize does not retain the borrowed profile. Later mutation cannot
    // alter the already-owned source sheets or currently bound actor.
    selected.stats.health=1;selected.experience=999;
    check(session.actor(1)->health==fresh.health-7.25f&&session.world()->combat_properties(1)->sheets.resolved[33]==8*256,
        "Session retained mutable selected-profile authority after startup");
    std::cout<<"PASS selected Rogue pre-bind partial vitals/progression/identity, real dual gear, no refill, no retained profile borrow, atomic rejected class/level/vitals\n";
    std::cout<<"PASS actual Rogue dual root245/12 leaves/four ordered groups, both named hands exact source result/RNG, stable same owner, strict save/rebind and invalid-bank atomic rejection; hits="<<hits<<'\n';
    return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
