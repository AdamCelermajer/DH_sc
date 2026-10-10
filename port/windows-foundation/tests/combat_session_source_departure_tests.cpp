#include "../combat_session.hpp"
#include "../game_save.hpp"
#include "../features/skills_animation/skill_animation_program.hpp"
#include "../retained_animation_owner.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value){return {value.data(),value.size()};}
struct Fixture {
    AssetCatalog assets;OriginalPropertyDatabase db;OriginalMeleeBindings bindings;
    ActorCustomization customization;CombatSessionConfig config;ActorPopulation population;
    CharacterVisual visual;CombatSession session;skills_animation::SkillAnimationPrograms bank;
    std::string error;unsigned uses=0,finishes=0,posts=0,steps=0;bool cooldown=true,throwPost=false;
    std::vector<std::string> order;
    dh2::data::PropertySheet formula{};
    std::function<void()> onUse;
    explicit Fixture(const std::filesystem::path& repo):assets(repo/".local-inputs/windows-shared-assets"){
        check(load_original_property_tables(assets,"original-cache/data/pydata",db,error),error);
        check(bindings.load(assets,"original-melee-bindings.xml",error),error);
        const auto rows=assets.read("original-cache/data/pydata/animations_pyarray.bin");
        const auto names=assets.read("original-cache/data/pydata/animations_pyarraynames.bin");
        const auto fields=assets.read("original-cache/data/pydata/animations_pystructnames.bin");
        const auto values=assets.read("original-cache/data/pydata/animations_dictionary_pyarray.bin");
        std::ifstream file(repo/".local-inputs/actors/animations_dictionary_pyarraynames.bin",std::ios::binary);
        check(bool(file),"Source animation dictionary absent");
        const std::vector<std::uint8_t> keys{std::istreambuf_iterator<char>(file),{}};
        dh2::data::Dictionary clips;dh2::data::AnimationTables tables;
        check(dh2::data::load_dictionary(bytes(keys),bytes(values),clips,error)&&
              dh2::data::load_animation_tables(bytes(rows),bytes(names),bytes(fields),clips,tables,error),error);
        customization.allow_missing_animation_targets=true;
        OriginalCombatVisualPlan plan;
        check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"departure-player",plan,error),error);
        auto visualConfig=plan.config;visualConfig.motion_node_id="auto";visualConfig.consume_root_motion=true;
        check(skills_animation::build_skill_animation_programs(assets,tables,clips,visualConfig,{347},"departure-player",bank,error),error);
        config.playerId=1;config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
        config.playerVisualConfig=visualConfig;config.mainItemId="Longsword01";config.equippedItemIds={"Longsword01"};
        CombatSessionProfile knight;knight.initialIdle={"Idle",0,{0}};
        OriginalAttackSelection attack;attack.state="AttackStatic";attack.group_path={0};knight.sequenceAction=attack;
        knight.retainedPhaseClock=true;knight.damageMarkerNames={"attack_mainhand"};knight.propertyOptions={256,true};
        knight.reaction=CombatSessionChoice{"Injured",0,{0}};knight.death=CombatSessionChoice{"Died",0,{0}};
        knight.sourceAnimationClips=bank.plan.config.clips;knight.customization=customization;
        config.profiles.emplace("KnightPlayerBase",knight);
        CombatSessionProfile enemy;enemy.initialIdle={"Idle",0,{0}};enemy.action={"Attack",0,{0,1}};
        enemy.damageMarkerNames={"attack_mainhand"};enemy.customization=customization;enemy.propertyOptions={std::nullopt,true};
        config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
        PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";placed.definition.stableId=2;
        placed.definition.sourceId="departure-enemy";placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
        placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
        initialize(17);
    }
    void initialize(std::uint32_t seed){
        config.diagnosticRngSeed=seed;
        check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
        uses=finishes=posts=steps=0;order.clear();cooldown=true;throwPost=false;onUse={};
        session.set_step_entry_observer([&](const auto& event){if(event.actor==1){++steps;order.push_back("step");}});
    }
    std::uint32_t seed(std::int32_t state,std::uint32_t mask,bool injury){
        OriginalMeleeDamageProvider provider;auto target=*session.world()->combat_properties(1);
        target.facts.original_state=state;
        formula=session.world()->combat_properties(2)->sheets.resolved;
        // Controlled status thresholds exercise source FSM outcome branches.
        // Lizard's authored ordinary rating is zero; a seed cannot turn that
        // into an injury. No fabricated DamageEvent/outcome enters the runtime.
        formula[135]=formula[182]=injury?100*256:0;
        check(provider.bind_actor(1,target,error)&&provider.bind_actor(2,*session.world()->combat_properties(2),error),error);
        const auto category=session.world()->combat_properties(2)->facts.main_damage_class;
        for(std::uint32_t i=1;i<50000;++i){dh2::data::CombatRandom rng{i,0};OriginalMeleeResolution result;
            check(provider.resolve_result(2,1,mask,category,-1,0,rng,result,error,&formula),error);
            if(bool(result.original.outcomes&0x10u)==injury&&result.damage>0&&result.damage<session.actor(1)->max_health)return i;
        }throw std::runtime_error("No original formula seed for test branch");
    }
    CombatSessionStateAnimationServices services(){
        CombatSessionStateAnimationServices result;
        result.event=[&](ActorId id,const RetainedAnimationEvent& event,std::string&){check(id==1,"Foreign event");if(event.name=="do_skill"){if(onUse)onUse();++uses;}return true;};
        result.finished=[&](ActorId id,std::string&){check(id==1,"Foreign finish");++finishes;session.actor(1)->action=CharacterAction::idle;return true;};
        result.departed=[&](ActorId id,std::int32_t from,std::int32_t to,std::string&){
            check(id==1&&(from==6||from==7)&&(to==3||to==6||to==7||to==11||to==12),"Wrong source departure");
            ++posts;order.push_back("post");if(throwPost)throw std::runtime_error("fixture Post failed");return true;
        };
        const auto lease=session.actor_binding_lease();
        result.checkpoint=[&,lease](std::string& e){
            if(lease.expired()){e="fixture source checkpoint owner expired";return false;}
            if(cooldown){e="fixture source cooldown remains";return false;}return true;
        };
        return result;
    }
    void begin(std::int32_t state,std::uint32_t flags=0x6341u,std::uint64_t generation=1){
        OriginalAttackSelection selection;selection.state=bank.plan.sequences.front().state;
        session.actor(1)->action=CharacterAction::casting;
        check(session.play_actor_source_sequence(1,bank.plan,bank.policies,selection,services(),{state,flags,generation},error),error);
        check(session.original_actor_state(1)==state&&session.world()->combat_properties(1)->facts.original_state==state,
              "Source Skill/Cast state not published to same actor");
    }
    DamageEvent hit(std::uint32_t mask=0x22aab5u,bool lethal=false){
        CombatSessionSourceHit request;request.attacker=2;request.target=1;request.binding_lease=session.actor_binding_lease();
        request.generation=1;request.source_id="departure-source-hit";request.marker_name="attack_mainhand";
        request.mask=lethal?0x20080000u:mask;request.category=lethal?-1:session.world()->combat_properties(2)->facts.main_damage_class;
        if(!lethal)request.attacker_formula_sheet=&formula;
        if(lethal)request.direct_amount=static_cast<std::int32_t>(std::ceil(session.actor(1)->health*256));
        DamageEvent receipt;check(session.apply_source_result(request,receipt,error)&&receipt.applied,error);return receipt;
    }
    void frames(unsigned count){for(unsigned i=0;i<count;++i)check(session.update(1.0/60,{},Vec3{0,0,0},0,error),error);}
};
}
int main(int argc,char** argv){try{
    check(argc==2,"Repository root required");Fixture f(argv[1]);
    // Real formula outcomes, not fabricated DamageEvent bits.
    for(const auto state:{6,7}){
        const auto sourceSeed=f.seed(state,0x22aab5u,true);f.initialize(sourceSeed);f.begin(state,state==6?0x6341u:0x6301u);
        const auto* owner=f.session.retained_actor_pose(1);const auto receipt=f.hit();
        check(receipt.source_outcomes&&(*receipt.source_outcomes&0x10u)&&f.posts==0&&f.session.actor(1)->action==CharacterAction::casting,
              "Ordinary Injury50010 interrupted player Skill/Cast");
        InputActions held;held.attack=true;check(f.session.update(.01,held,{0,0,0},0,f.error),f.error);
        check(f.session.original_actor_state(1)==state,"Held basic attack silently replaced source cast");
        f.frames(240);check(f.uses==1&&f.finishes==1&&f.posts==0&&f.session.retained_actor_pose(1)==owner,
              "Non-admitted injury reset cursor, lost Use or replaced retained owner");
        check(!f.session.validate_lifecycle_checkpoint(f.error),"Unpersisted cooldown allowed save");
        f.cooldown=false;check(f.session.validate_lifecycle_checkpoint(f.error),f.error);
    }
    f.initialize(f.seed(6,0x22aab5u,false));f.begin(6);const auto ordinary=f.hit();
    check(ordinary.source_outcomes&&!(*ordinary.source_outcomes&0x10u)&&f.posts==0,"HP loss inferred departure");
    f.frames(240);check(f.uses==1&&f.finishes==1,"Ordinary damage lost source Use");
    // Direct source-mask injury, flagged Skill predicate and death all exit.
    for(unsigned branch=0;branch<3;++branch){
        const auto state=branch==1?6:7;const auto mask=branch==0?0x1022aab5u:0x22aab5u;
        const auto sourceSeed=f.seed(state,mask,true);f.initialize(sourceSeed);
        f.begin(state,branch==1?0x16341u:0x6301u);const auto* owner=f.session.retained_actor_pose(1);
        check(!f.session.validate_lifecycle_checkpoint(f.error),"Active source program allowed save");
        f.order.clear();const auto receipt=f.hit(mask,branch==2);
        check((branch==2?receipt.target_died:receipt.source_outcomes&&(*receipt.source_outcomes&0x10u))&&
              f.posts==1&&f.uses==0&&f.finishes==0&&f.order.size()>=2&&f.order[0]=="post"&&f.order[1]=="step",
              "Accepted injury/death did not run Post before incoming focus");
        const auto rng=f.session.world()->random_state();f.frames(240);
        check(f.posts==1&&f.uses==0&&f.finishes==0&&f.session.retained_actor_pose(1)==owner&&
              f.session.world()->random_state().seed==rng.seed&&f.session.world()->random_state().calls==rng.calls,
              "Cancelled source event resumed after reaction or consumed RNG");
    }
    f.initialize(17);f.begin(6);bool departed=false;const auto lease=f.session.actor_binding_lease();
    const auto foreign=std::make_shared<const int>(1);
    check(!f.session.cancel_actor_source_sequence(1,foreign,1,3,departed,f.error)&&f.posts==0,"Foreign departure admitted");
    check(f.session.cancel_actor_source_sequence(1,lease,2,3,departed,f.error)&&!departed,"Stale occurrence cancelled current cast");
    f.throwPost=true;
    check(!f.session.cancel_actor_source_sequence(1,lease,1,3,departed,f.error)&&departed&&f.posts==1,
          "Failed Post did not commit the departure prefix");
    check(f.session.cancel_actor_source_sequence(1,lease,1,3,departed,f.error)&&!departed&&f.posts==1,
          "Failed Post replayed on retry");f.frames(240);check(f.uses==0&&f.finishes==0,"Failed Post resumed cast");
    f.cooldown=false;check(f.session.validate_lifecycle_checkpoint(f.error),f.error);
    f.session.detach_for_restore();check(!f.session.cancel_actor_source_sequence(1,lease,1,3,departed,f.error),"Detached departure admitted");
    check(f.session.rebind_after_restore(f.error),f.error);
    check(!f.session.cancel_actor_source_sequence(1,lease,1,3,departed,f.error),"Old restore lease admitted departure");
    check(f.session.validate_lifecycle_checkpoint(f.error),"Old expired checkpoint callback survived successful rebind");
    // After a reached Use, the same accepted exit still has one Post and no
    // synthetic completion. No second event may be delivered from recovery.
    f.initialize(17);f.begin(6,0x6341u,3);
    for(unsigned i=0;i<240&&!f.uses;++i)f.frames(1);
    check(f.uses==1&&f.finishes==0,"Source fixture had no pre-completion Use boundary");
    check(f.session.cancel_actor_source_sequence(1,f.session.actor_binding_lease(),3,3,departed,f.error)&&departed&&f.posts==1,f.error);
    f.frames(240);check(f.uses==1&&f.posts==1&&f.finishes==0,"Post-Use departure replayed Use/Post/completion");
    f.cooldown=false;check(f.session.validate_lifecycle_checkpoint(f.error),f.error);
    // _SetState runs Blur/Focus even when next state equals the current ID.
    f.initialize(17);f.begin(6,0x6341u,4);
    check(f.session.cancel_actor_source_sequence(1,f.session.actor_binding_lease(),4,6,departed,f.error)&&departed&&f.posts==1,f.error);
    f.frames(240);check(f.uses==0&&f.posts==1&&f.finishes==0,"Accepted same-state restart resumed old occurrence");
    check(!f.session.clear_lifecycle_services(f.error)&&!f.session.validate_lifecycle_checkpoint(f.error),
          "Legacy cleanup erased an outstanding generic cooldown");
    // Detaching while a cast is active never forges a quiescent witness.
    f.initialize(17);f.begin(7,0x6301u);f.session.detach_for_restore();
    check(!f.session.clear_lifecycle_services(f.error)&&!f.session.rebind_after_restore(f.error),
          "Active cast detach/legacy cleanup admitted unpersisted restore");
    f.initialize(17);f.begin(6);
    check(!f.session.clear_lifecycle_services(f.error)&&f.posts==0&&f.session.original_actor_state(1)==6,
          "Legacy cleanup silently cancelled active generic cast/Post");
    f.frames(240);check(f.uses==1&&f.finishes==1,"Rejected cleanup damaged retained cast");
    // Timer frame phase uses the real current serial, independently of AI.
    f.initialize(17);unsigned ticks=0,decisions=0;std::uint64_t timerSerial=0;double wall=0;
    f.session.set_frame_begin_provider([&](CombatSession& current,double dt,std::string&){
        check(&current==&f.session&&current.update_serial()==timerSerial+1,"Timer tick guessed/skipped current frame");
        timerSerial=current.update_serial();++ticks;wall+=dt;return true;
    });
    f.session.set_actor_decision_provider([&](CombatSession& current,double,std::string&){
        check(current.update_serial()==timerSerial,"AI decision preceded actual frame timer tick");++decisions;return true;
    });
    f.onUse=[&](){check(f.session.update_serial()==timerSerial,"Source Use reached stale frame timer");};
    f.begin(6);f.frames(240);check(ticks==240&&decisions==240&&f.uses==1&&f.finishes==1&&std::abs(wall-4)<1e-8,
          "Timer provider duplicated/disabled AI or lost source markers");
    const auto elapsedBeforePause=wall;
    check(f.session.update(0,{},Vec3{0,0,0},0,f.error)&&ticks==241&&wall==elapsedBeforePause,
          "Zero dt changed elapsed time or lost actual frame phase");
    check(!f.session.update(-1,{},Vec3{0,0,0},0,f.error)&&ticks==241&&f.session.update_serial()==241,
          "Invalid frame advanced timer/serial");
    f.session.clear_frame_begin_provider();f.session.clear_actor_decision_provider();f.frames(1);
    check(ticks==241&&decisions==241,"Cleared frame provider still ran");
    // Animation-only class hosts still publish completed generic state back
    // to Idle; their normal vital/receiver policy must not retain stale state6.
    const auto originalProfile=f.config.profiles.at("KnightPlayerBase");
    auto& animationProfile=f.config.profiles.at("KnightPlayerBase");animationProfile.animationOnly=true;
    animationProfile.sequenceAction.reset();animationProfile.reaction.reset();animationProfile.death.reset();
    animationProfile.damageMarkerNames.clear();animationProfile.propertyOptions.refill_vitals=false;
    f.initialize(17);f.begin(6);f.frames(240);
    check(f.uses==1&&f.finishes==1&&f.session.original_actor_state(1)==3&&
          f.session.world()->combat_properties(1)->facts.original_state==3&&!f.session.world()->traits(1)->targetable,
          "Animation-only generic completion retained stale source Skill state");
    f.cooldown=false;check(f.session.validate_lifecycle_checkpoint(f.error),f.error);
    auto roleCharacter=make_default_character("pure-role-test","Warrior","KnightPlayerBase");
    f.session.actor(1)->persistent_character_id=roleCharacter.id;GameSave roleSave;
    check(capture_game_save("pure-role",1,roleCharacter,*f.session.world(),roleSave,f.error),f.error);
    f.initialize(17);f.session.actor(1)->persistent_character_id=roleCharacter.id;f.session.detach_for_restore();
    check(restore_game_save(roleSave,"pure-role",*f.session.world(),roleCharacter,f.error)&&f.session.rebind_after_restore(f.error),f.error);
    check(!f.session.world()->traits(1)->targetable,"Pure display actor changed saved targetable role across source cast/reinit/restore");
    f.config.profiles.at("KnightPlayerBase")=originalProfile;
    f.initialize(17);
    // Genuine legacy state ownership retains its old explicit save guard.
    check(f.session.set_actor_original_state(1,17,f.error)&&!f.session.validate_lifecycle_checkpoint(f.error),
          "Generic completion weakened campaign lifecycle guard");
    std::cout<<"PASS actual Session Skill/Cast injury admission, direct/death Post-before-focus, cancellation no stale Use/RNG, occurrence/lease/prefix guards, active/cooldown/quiescent checkpoint and legacy rejection\n";
    return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
