#include "source_combo_chain.hpp"
#include "../../original_attack_sequence.hpp"
#include "../../../level-world/character_animation_ai.hpp"
#include <iostream>
#include <set>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh2::character;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
bool operation(const combo::BoundaryEffects& effects,AttackAnimationServiceV1 service){for(const auto& op:effects.operations)if(op.service==service)return true;return false;}
struct InputBackend {
    bool attacking=true,dead=false;
    AttackTargetList24 list{17,nullptr,0,0};
    static void invoke(void* context,AttackState64* ai,ControllerAttackState32*,const AttackRequest32* q,AttackResponse16* out){
        auto& b=*static_cast<InputBackend*>(context);*out={};
        switch(q->service){
        case attack_owner_dead:out->word=b.dead;break;
        case attack_owner_ranged:break;
        case attack_is_attacking:out->word=b.attacking;break;
        case attack_list_create:out->identity=reinterpret_cast<std::uintptr_t>(&b.list);break;
        case attack_can_attack_current:out->word=1;break;
        case attack_target_dead:break;
        case attack_current_in_melee:out->word=1;break;
        case attack_owner_player:out->word=1;break;
        case attack_set_target:ai->target=q->payload;break;
        case attack_set_attack_state:b.attacking=true;break;
        case attack_controllable_dispatch:{AttackServices16 services{context,invoke};dh2_character_ai_melee_attack(ai,q->payload,0,&services);break;}
        default:break;
        }
    }
};
void sequence_node(const OriginalMeleeStep& step,dh2::data::AnimationTables& tables,const OriginalSequencePolicies& policies){
    if(step.redirect!=1)return;
    const auto policy=policies.find(step.animationId);check(policy!=policies.end(),"Original redirect policy absent");
    auto& sequence=tables.sequences.at(step.animationId);sequence.type=static_cast<int>(policy->second.type);sequence.loop=static_cast<int>(policy->second.loop);
    sequence.steps.clear();
    for(const auto& child:step.children){dh2::data::AnimationStep record;record.anim=static_cast<int>(child.animationId);record.redir=static_cast<int>(child.redirect);record.speed=static_cast<float>(child.speed);sequence.steps.push_back(record);}
    for(const auto& child:step.children)sequence_node(child,tables,policies);
}
void source_table(const OriginalMeleeSequence& source,dh2::data::AnimationTables& tables,const OriginalSequencePolicies& policies){
    tables.sequences.resize(4096);auto& root=tables.sequences.at(source.id);root.type=static_cast<int>(source.type);root.loop=static_cast<int>(source.loop);
    for(const auto& step:source.steps){dh2::data::AnimationStep record;record.anim=static_cast<int>(step.animationId);record.redir=static_cast<int>(step.redirect);record.speed=static_cast<float>(step.speed);root.steps.push_back(record);sequence_node(step,tables,policies);}
}
}
int main(int argc,char** argv){try{
    std::string error;AttackState64 ai{};ai.owner=1;ai.target=2;ai.object_of_interest_type=-1;
    InputBackend input;AttackServices16 services{&input,InputBackend::invoke};combo::BoundaryEffects effects;
    combo::Boundary inner;inner.depth=1;inner.count=3;inner.hasCombo=true;inner.targetDead=[](auto){return false;};
    check(combo::begin(ai,inner,effects,error),error);check(ai.last==1,"Source pre phase did not close continuation window");
    check(combo::command(ai,true,2,services,error),error);check(ai.continued==0,"Command during pre phase was buffered");
    inner.step=1;check(combo::begin(ai,inner,effects,error),error);check(ai.last==0,"Source strike did not open continuation window");
    check(combo::command(ai,false,2,services,error),error);check(ai.continued==0,"Undelivered input created continuation");
    check(combo::command(ai,true,2,services,error),error);check(ai.continued==1,"Source strike press did not continue");
    check(combo::command(ai,true,2,services,error),error);check(ai.continued==1,"Repeated held command changed boolean continuation into a counter");
    ControllerAttackState32 controller{};controller.controllable=controller.character=ai.owner;controller.blocked=1;ai.continued=0;
    check(combo::controller_command(controller,ai,true,2,services,error),error);check(ai.continued==0,"Blocked original controller accepted held continuation");
    controller.blocked=0;check(combo::controller_command(controller,ai,true,2,services,error),error);check(ai.continued==1,"Unlocked original controller did not dispatch continuation");
    check(combo::end(ai,inner,effects,error),error);check(operation(effects,attack_anim_skip_next_v1)&&ai.continued==1,"Live-target strike end did not preserve continuation and skip recovery");
    combo::Boundary root;root.count=3;root.hasCombo=true;root.canRange=[](){return false;};
    check(combo::end(ai,root,effects,error),error);combo::GroupAdvance next;
    check(combo::advance_group(1,3,0,0,effects,next,error),error);check(next.completion==1&&next.nextStep==1&&ai.continued==0,"Authored ordered root did not advance to second attack");
    root.step=1;check(combo::begin(ai,root,effects,error),error);check(ai.index==1,"Source AI attack index did not follow root cursor");
    check(combo::end(ai,root,effects,error),error);check(combo::advance_group(1,3,1,0,effects,next,error),error);
    check(next.completion==2&&operation(effects,attack_anim_clear_nonsticky_v1),"Released nonranged combo did not stop at source root end");
    inner.step=2;check(combo::begin(ai,inner,effects,error),error);check(ai.last==1&&ai.finisher==1,"Source recovery/finisher flags differ");
    check(combo::command(ai,true,2,services,error),error);check(ai.continued==0,"Recovery input was improperly buffered");
    // Clearing combo ownership on interruption prevents end callbacks progressing.
    root.hasCombo=false;ai.continued=1;check(combo::end(ai,root,effects,error),error);check(effects.operations.empty()&&ai.continued==1,"Interrupted callback invented a continuation reset or next step");
    input.attacking=false;check(combo::command(ai,true,2,services,error),error);
    check(ai.continued==0&&input.attacking,"Fresh source attack after interruption did not reset continuation through actual input owner");
    inner.step=1;inner.hasCombo=true;inner.targetDead=[](auto){return true;};check(combo::end(ai,inner,effects,error),error);check(!operation(effects,attack_anim_skip_next_v1)&&ai.continued==0,"Dead target skipped recovery or retained continuation");
    // Source callbacks can reenter and clear target during death lookup.
    ai.target=2;ai.continued=1;inner.targetDead=[&](auto){ai.target=0;return true;};check(combo::end(ai,inner,effects,error),error);check(operation(effects,attack_anim_skip_next_v1),"Source target reload after reentry was lost");
    check(argc==2,"Supply repository root for actual source data tests");const auto repository=std::filesystem::path(argv[1]);
    AssetCatalog metadata(repository/".local-inputs/windows-melee-bindings"),assets(repository/".local-inputs/windows-shared-assets");OriginalMeleeBindings bindings;OriginalSequencePolicies policies;
    check(bindings.load(metadata,"original-melee-bindings.xml",error),error);check(original_sequence_policies(bindings,policies,error),error);
    OriginalCombatVisualPlan knight;check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",{},"source-knight",knight,error),error);
    const auto* authored=bindings.sequence("KnightPlayerBase","AttackStatic",0);check(authored&&authored->type==1&&authored->steps.size()==3,"Original knight root type/count differs");
    std::set<std::string> strikes;for(std::size_t group=0;group<authored->steps.size();++group){const auto* strike=knight.phase("AttackStatic",0,{group,1});check(strike,"Original authored strike phase absent");strikes.insert(strike->sourceUri);}
    check(strikes.size()==3,"Actual authored consecutive Knight attacks do not differ");
    dh2::data::AnimationTables knightTables;source_table(*authored,knightTables,policies);dh2::data::AnimationRandom random{1234,0};dh2::data::AnimationStart start;
    check(combo::select_start(knightTables,static_cast<int>(authored->id),random,start,error),error);check(start.layers.front().second==0&&random.calls==0,"Ordered source player root consumed randomness or skipped first attack");
    const auto* npc=bindings.sequence("Swamp_LizadMan_Type1","Attack",0);check(npc&&npc->type==2,"Original enemy root is not type2");dh2::data::AnimationTables npcTables;source_table(*npc,npcTables,policies);
    std::set<unsigned> selected;const auto before=random.calls;for(unsigned i=0;i<16;++i){check(combo::select_start(npcTables,static_cast<int>(npc->id),random,start,error),error);selected.insert(start.layers.front().second);}
    check(random.calls==before+16&&selected.size()>1,"Enemy initial source choices did not use original RNG stream");
    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"retained-combo",knight,error),error);
    const auto runRetained=[&](bool hold,bool blocked,bool interrupt){
        auto config=knight.config;config.consume_root_motion=true;config.motion_node_id="auto";
        CharacterVisual visual;check(visual.load(assets,config,error),error);
        RetainedSequencePlayback playback(visual,complete_pose_root_sampler(visual));
        AttackState64 owner{};owner.owner=1;owner.target=2;owner.object_of_interest_type=-1;
        InputBackend backend;backend.attacking=false;AttackServices16 sourceServices{&backend,InputBackend::invoke};
        ControllerAttackState32 control{};control.controllable=control.character=1;control.blocked=blocked;
        combo::Boundary providers;providers.hasCombo=dh2_character_animation_has_combo(static_cast<int>(authored->id),4096,static_cast<int>(authored->type));
        providers.canRange=[](){return false;};providers.targetDead=[](auto){return false;};
        providers.onOperation=[](AttackState64& ai,const combo::Operation& op){if(op.service==attack_anim_clear_nonsticky_v1)ai.target=0;return true;};
        std::vector<std::size_t> roots;std::vector<std::pair<bool,std::vector<std::size_t>>> trace;std::size_t markers=0;unsigned closes=0;
        RetainedSequenceServices playbackServices;
        playbackServices.frame=[&](std::size_t,const RetainedAnimationFrame& frame,std::string&){for(const auto& event:frame.events)if(event.name=="attack_mainhand")++markers;return true;};
        playbackServices.closed=[&](const auto&,std::string&){++closes;return true;};
        playbackServices.boundary=[&](const RetainedSequenceBoundary& source,RetainedSequenceCursorDecision& decision,std::string& e){
            trace.push_back({source.beginning,source.containerPath});
            if(source.beginning&&source.depth==0)roots.push_back(source.step);
            combo::BoundaryEffects boundaryEffects;return combo::retained_boundary(owner,source,providers,boundaryEffects,decision,e);
        };
        OriginalAttackSelection selection;selection.state="AttackStatic";
        check(playback.prepare(assets,knight,policies,selection,playbackServices,"retained-combo-action",error),error);
        check(playback.phases().size()==9,"Actual full-root program did not retain all three authored groups");
        check(playback.begin(error),error);bool interrupted=false;
        for(unsigned frame=0;frame<600&&playback.active();++frame){
            backend.attacking=playback.active();
            const bool delivered=hold&&(roots.empty()||roots.back()<2);
            check(combo::controller_command(control,owner,delivered,2,sourceServices,error),error);
            if(interrupt&&owner.last==0){
                const auto previousTrace=trace.size();playback.cancel();check(!playback.active()&&trace.size()==previousTrace,"Interruption delivered a stale source End callback");
                backend.attacking=false;control.blocked=0;
                check(combo::controller_command(control,owner,true,2,sourceServices,error),error);check(owner.continued==0,"Fresh retained action retained interrupted continuation");
                check(playback.begin(error),error);interrupted=true;break;
            }
            check(playback.advance(0.016,error),error);
        }
        if(interrupt){check(interrupted&&roots.back()==0,"Interrupted restart did not begin authored root0");playback.cancel();return;}
        check(playback.finished()&&closes==1,"Retained combo did not close exactly once after release");
        if(hold&&!blocked){check(roots==std::vector<std::size_t>{0,1,2},"Held source input did not progress through actual ordered three groups");check(markers==3,"Held source chain lost/duplicated authored hit markers");}
        else {check(roots==std::vector<std::size_t>{0},"Released/blocked source input progressed to another attack");check(markers==1,"Released/blocked first group emitted another attack hit");}
        check(trace.size()>=3&&trace[0].first&&trace[0].second.empty()&&trace[1].first&&trace[1].second==std::vector<std::size_t>{0},"Retained Begin hierarchy order differs");
    };
    runRetained(true,false,false);runRetained(false,false,false);runRetained(true,true,false);runRetained(true,false,true);
    std::cout<<"PASS actual retained full-root hold=three varied swings, release=one, controllerblock=one, interruption=fresh root0; same slots/clock and authored hit markers\n";
    std::cout<<"PASS source pre/strike/recovery input windows, held-vs-press delivery, interruption/death/reentry, ordered next root, three authored Knight strikes, NPC type2 original RNG\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
