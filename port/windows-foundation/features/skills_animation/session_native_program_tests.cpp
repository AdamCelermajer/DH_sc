#include "session_skill_animation.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_melee_bindings.hpp"
#include "../../asset_catalog.hpp"
#include "../../../game-data/skill_tables.hpp"
#include "../../../game-data/properties.hpp"
#include "../../../game-data/data.hpp"
#include "../../../game-data/animation_tables.hpp"
#include "../../../level-world/character_skills.hpp"
#include "../../../level-world/character_skill_callbacks_v3.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::skills_animation;
using namespace dh2::character::skills;

static void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
static dh2::data::Bytes view(const std::vector<std::uint8_t>& v){return {v.data(),v.size()};}

struct NativeBoundaryFixture {
    dh2::character::State fsm;
    unsigned skill_uses=0;
    unsigned plays=0;
    unsigned completions=0;
    unsigned frames=0;
    bool wrong_actor=false;
    ActorState foreign_actor;
    SkillActorBorrow borrowed;
    static int ai(void* raw,SkillAIContextV3*,const SkillAIRequest32V3* q,SkillAIResponse32V3* r){
        auto& self=*static_cast<NativeBoundaryFixture*>(raw);
        if(q->operation==skill_ai_callback_v3&&q->value==skill_use_v3){++self.skill_uses;r->word=1;return 0;}
        if(q->operation==skill_ai_using_v3||q->operation==skill_ai_casting_v3||q->operation==skill_ai_player_v3){r->word=0;return 0;}
        return -1;
    }
    static int state(void* raw,SkillStateV4*,const SkillStateRequest32V4* q,SkillStateResponse16V4* r){
        auto& self=*static_cast<NativeBoundaryFixture*>(raw);
        switch(q->operation){
        case skill_state_debug_load_v4:case skill_state_debug_get_v4:case skill_state_debug_destroy_v4:
        case skill_state_raise_v4:case skill_state_speed_v4:case skill_state_cancel_sneaking_v4:return 0;
        case skill_state_debug_construct_v4:r->identity=1;return 0;
        case skill_state_monster_v4:r->word=0;return 0;
        case skill_state_step_index_v4:r->word=0;return 0;
        case skill_state_step_count_v4:return 0;
        case skill_state_current_v4:r->word=6;return 0;
        default:return -1;
        }
    }
};

int main(int argc,char** argv){try{
    check(argc==2,"Supply repository root");
    const std::filesystem::path repo(argv[1]);
    AssetCatalog assets(repo/".local-inputs/windows-shared-assets"),bindingAssets(repo/".local-inputs/windows-melee-bindings");
    OriginalPropertyDatabase database;OriginalMeleeBindings bindings;std::string error;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(bindings.load(bindingAssets,"original-melee-bindings.xml",error),error);

    const auto skillBytes=assets.read("original-cache/data/pydata/skills_pyarray.bin");
    const auto skillNames=assets.read("original-cache/data/pydata/skills_pyarraynames.bin");
    const auto skillSchema=assets.read("original-cache/data/pydata/skills_pystructnames.bin");
    dh2::data::SkillTables skillTables;
    check(skillTables.load(view(skillBytes),view(skillNames),view(skillSchema),error),error);
    const auto skillBorrow=skillTables.borrow();
    const auto knight=skillBorrow.list_index("Knight");
    check(knight>=0&&!skillBorrow.lists()[knight].empty(),"Original Knight skill list unavailable");
    const auto skillRow=skillBorrow.lists()[knight][0];
    check(skillRow==7&&skillBorrow.skill_names()[skillRow]=="BashDown"&&
          skillBorrow.skills()[skillRow].scalar.words[1]==347,"Original Knight position0 does not resolve to BashDown root347");

    const std::string root="original-cache/data/pydata/";
    const auto ar=assets.read(root+"animations_pyarray.bin"),an=assets.read(root+"animations_pyarraynames.bin"),af=assets.read(root+"animations_pystructnames.bin");
    const auto dictionaryNames=AssetCatalog(repo).read(".local-inputs/actors/animations_dictionary_pyarraynames.bin");
    const auto dictionaryData=assets.read(root+"animations_dictionary_pyarray.bin");
    dh2::data::Dictionary dictionary;dh2::data::AnimationTables animationTables;
    check(dh2::data::load_dictionary(view(dictionaryNames),view(dictionaryData),dictionary,error),error);
    check(dh2::data::load_animation_tables(view(ar),view(an),view(af),dictionary,animationTables,error),error);

    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";
    customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan base;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"host-bank",base,error),error);
    auto compileVisual=base.config;compileVisual.motion_node_id="auto";compileVisual.consume_root_motion=true;
    auto playerVisual=compileVisual;playerVisual.clips.clear();
    SkillAnimationPrograms programs;
    check(build_skill_animation_programs(assets,animationTables,dictionary,compileVisual,{347},"host-bank",programs,error),error);
    check(programs.plan.sequences.size()==1&&programs.plan.sequences.front().id==347&&
          programs.plan.sequences.front().name=="Knight_BashDown","Compiled source program differs from loaded Skill row");
    check(programs.plan.sequences.front().phases.size()==1,"BashDown source plan is not the expected actual one-leaf program");

    CombatSessionConfig config;config.diagnosticRngSeed=347;config.playerId=1;config.playerProfileId="KnightPlayerBase";
    config.tableRoot="original-cache/data/pydata";config.playerVisualConfig=playerVisual;
    CombatSessionProfile profile;profile.initialIdle={"Idle",0,{0}};profile.damageMarkerNames={"attack_mainhand"};
    profile.propertyOptions={256,true};profile.sequenceAction=OriginalAttackSelection{};
    profile.sequenceAction->state="AttackStatic";profile.retainedPhaseClock=true;
    for(const auto& clip:programs.plan.config.clips){
        const auto prior=std::find_if(compileVisual.clips.begin(),compileVisual.clips.end(),[&](const auto& c){return c.first==clip.first;});
        if(prior==compileVisual.clips.end())profile.sourceAnimationClips.push_back(clip);
        else check(prior->second==clip.second,"Skill bank conflicts with preloaded actor alias");
    }
    config.profiles.emplace(config.playerProfileId,profile);
    CharacterVisual visual;ActorPopulation population;CombatSession session;
    check(session.initialize(assets,database,bindings,config,visual,population,{0,0,0},customization,error),error);
    const auto* poseOwner=session.retained_actor_pose(1);
    check(poseOwner,"CombatSession did not install its retained actor pose owner");
    const auto* slotStorage=poseOwner->slots().data();

    dh2::data::PropertyRules propertyRules;
    check(dh2::data::load_property_rules(database.characters,propertyRules,error),error);
    dh2::data::PropertyState propertyState;dh2::data::reset_properties(propertyRules,propertyState);
    auto propertyView=dh2::data::property_view(propertyRules,propertyState);
    NativeBoundaryFixture native;
    native.fsm.current=6;
    auto* actor=session.actor(1);check(actor,"CombatSession player actor unavailable");
    const auto character=reinterpret_cast<std::uintptr_t>(actor);
    SkillAIOwnerV3 aiOwner{character,0,0};SkillAIStateV3 aiFields{};
    Instance32 instance{character,skillBorrow.skills()[skillRow].script.c_str(),0,0,0,0};
    const Instance32* instanceRows[]={&instance};
    State40 slots{character,{instanceRows,1,0},{}};
    aiFields.current=0;
    SkillAIContextV3 ai{&aiOwner,&slots,&aiFields,7,0};
    SkillStateV4 state{&native.fsm,character,0,0,0,0,0,0,{}};
    native.borrowed={actor,&propertyView,&ai,&state};

    SessionSkillServices services;
    services.borrow=[&](CombatSession& current,ActorId id,SkillActorBorrow& out,std::string&){
        if(&current!=&session||id!=1)return false;
        out=native.borrowed;if(native.wrong_actor)out.actor=&native.foreign_actor;return true;
    };
    services.row=[&](ActorId id,std::uint32_t index,std::int32_t& row,std::string&){if(id!=1||index!=0)return false;row=skillRow;return true;};
    services.selection=[&](ActorId id,std::int32_t rootId,OriginalAttackSelection& selected,std::string&){
        if(id!=1)return false;selected={};selected.state=skill_sequence_state(rootId);selected.variant=0;return true;
    };
    services.play=[&](CombatSession& current,ActorId id,const OriginalCombatVisualPlan& plan,
        const OriginalSequencePolicies& policies,const OriginalAttackSelection& selected,
        CombatSessionStateAnimationServices callbacks,std::string& failure){
        ++native.plays;return current.play_actor_source_sequence(id,plan,policies,selected,std::move(callbacks),failure);
    };
    services.finished=[&](CombatSession& current,ActorId id,std::string&){
        ++native.completions;return &current==&session&&id==1&&current.retained_actor_pose(id)==poseOwner;
    };
    services.ai={&native,NativeBoundaryFixture::ai};services.state={&native,NativeBoundaryFixture::state};

    SessionSkillAnimation animation(session,1,native.borrowed,skillBorrow,programs,services);
    check(animation.state_operation(dh2::character::skills::skill_state_event_v4,1,0,0,0,error),error);
    native.wrong_actor=true;
    check(!animation.state_operation(dh2::character::skills::skill_state_event_v4,1,0,0,0,error)&&
          error.find("owners changed")!=std::string::npos,"Changed same-session actor loan was not rejected");
    native.wrong_actor=false;

    native.fsm.animation_override=347;
    check(animation.state_operation(dh2::character::skills::skill_state_focus_v4,0,0,0,0,error),error);
    check(native.plays==1&&session.retained_actor_pose(1)==poseOwner&&poseOwner->slots().data()==slotStorage,
          "SessionSkillServices.play did not reuse the existing actor pose slots");
    const auto alias=programs.plan.sequences.front().phases.front().clipName;
    check(std::any_of(poseOwner->slots().begin(),poseOwner->slots().end(),[&](const auto& slot){return slot.clip_id==alias;}),
          "Loaded original BashDown alias was not selected in the existing Session slots");

    // Mutating the caller's plan after play proves that the Session retained
    // its own compiled metadata. The preloaded visual resources remain owned
    // by CombatSession and the source callbacks remain alive in this fixture.
    programs.plan.sequences.front().phases.front().clipName="caller-bank-mutated-after-play";
    InputActions input;
    for(unsigned frame=0;frame<240&&!native.completions;++frame){
        check(session.update(1.0/60,input,{0,0,0},0,error),error);
        ++native.frames;
    }
    check(native.completions==1,"Session did not close the whole retained source program exactly once");
    check(native.skill_uses==1,"Actual authored do_skill marker did not reach the explicit callback boundary exactly once");
    check(session.retained_actor_pose(1)==poseOwner&&poseOwner->slots().data()==slotStorage,
          "CombatSession replaced the original pose owner while completing the skill plan");

    const auto playsBefore= native.plays;
    native.fsm.animation_override=521;
    check(!animation.state_operation(dh2::character::skills::skill_state_focus_v4,0,0,0,0,error)&&
          error.find("does not match explicitly loaded original program")!=std::string::npos,
          "Root absent from the actor's compiled bank was accepted");
    check(native.plays==playsBefore,"Rejected program root reached SessionSkillServices.play");

    auto missingServices=services;missingServices.play={};
    SessionSkillAnimation missingPlay(session,1,native.borrowed,skillBorrow,programs,std::move(missingServices));
    native.fsm.animation_override=347;
    check(!missingPlay.state_operation(dh2::character::skills::skill_state_focus_v4,0,0,0,0,error)&&
          error.find("Session pose/whole-finished provider unbound")!=std::string::npos,
          "Missing Session play/finish provider was treated as successful");

    session.detach_for_restore();
    check(!animation.state_operation(dh2::character::skills::skill_state_focus_v4,0,0,0,0,error)&&
          error.find("owners changed")!=std::string::npos,
          "Detached Session accepted a provider returning its old actor/native loans");
    check(session.clear_lifecycle_services(error)&&session.rebind_after_restore(error),error);
    check(!animation.state_operation(dh2::character::skills::skill_state_focus_v4,0,0,0,0,error)&&
          error.find("owners changed")!=std::string::npos,
          "Restore rebind revived stale native skill loans with unchanged actor addresses");

    std::cout<<"PASS real CombatSession SessionSkillServices.play binder, original Skill row7/root347, same-owner/detached loan rejection, loaded-bank admission and metadata copy lifetime\n";
    std::cout<<"NOTE explicit native callback fixtures exercise the borrowed boundary only; no complete native FSM transition, Lua VM cast, mana/target/effect path is claimed\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
