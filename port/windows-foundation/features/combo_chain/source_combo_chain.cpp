#include "source_combo_chain.hpp"

namespace dh::foundation::combo { namespace {
struct Invocation {
    const Boundary& boundary;
    BoundaryEffects& effects;
    std::string& error;
    static int invoke(void* context,dh2::character::AttackState64& ai,
                      const dh2::character::AttackAnimationRequestV1& request,std::uint32_t& result){
        using namespace dh2::character;
        auto& self=*static_cast<Invocation*>(context);result=0;
        switch(request.service){
        case attack_anim_step_index_v1:result=self.boundary.step;return 0;
        case attack_anim_step_count_v1:result=self.boundary.count;return 0;
        case attack_anim_has_combo_v1:result=self.boundary.hasCombo;return 0;
        case attack_anim_can_range_v1:
            if(!self.boundary.canRange){self.error="Reached original ranged-capability provider is missing";return -1;}
            result=self.boundary.canRange();return 0;
        case attack_anim_target_dead_v1:
            if(!self.boundary.targetDead){self.error="Reached original target-death provider is missing";return -1;}
            result=self.boundary.targetDead(request.subject);return 0;
        default:
            const Operation operation{static_cast<AttackAnimationServiceV1>(request.service),request.value,request.subject};
            self.effects.operations.push_back(operation);
            if(self.boundary.onOperation&&!self.boundary.onOperation(ai,operation)){
                self.error="Reached original combo owner operation failed";return -1;
            }
            return 0;
        }
    }
};
bool run(bool beginning,dh2::character::AttackState64& ai,const Boundary& boundary,
         BoundaryEffects& effects,std::string& error){
    error.clear();effects.operations.clear();
    if(!ai.owner||boundary.depth>2||!boundary.count||boundary.step>=boundary.count){error="Invalid source combo boundary identity/depth/step domain";return false;}
    Invocation invocation{boundary,effects,error};
    const dh2::character::AttackAnimationBorrowV1 borrow{&ai,&boundary.depth,&boundary.lookAt};
    const dh2::character::AttackAnimationServicesV1 services{&invocation,Invocation::invoke};
    const int result=beginning?dh2_character_attack_animation_begin_v1(&borrow,&services):dh2_character_attack_animation_end_v1(&borrow,&services);
    if(result!=1){if(error.empty())error="Original attack animation callback rejected boundary";return false;}
    return true;
}
}
bool begin(dh2::character::AttackState64& ai,const Boundary& boundary,BoundaryEffects& effects,std::string& error){return run(true,ai,boundary,effects,error);}
bool end(dh2::character::AttackState64& ai,const Boundary& boundary,BoundaryEffects& effects,std::string& error){return run(false,ai,boundary,effects,error);}
bool command(dh2::character::AttackState64& ai,bool delivered,std::uintptr_t target,
             const dh2::character::AttackServices16& services,std::string& error){
    error.clear();if(!delivered)return true;
    const auto result=dh2_character_ai_melee_attack(&ai,target,0,&services);
    if(result){error="Original melee input command failed source owner/provider validation";return false;}
    return true;
}
bool controller_command(dh2::character::ControllerAttackState32& controller,dh2::character::AttackState64& ai,
                        bool delivered,std::uintptr_t target,const dh2::character::AttackServices16& services,std::string& error){
    error.clear();if(!delivered)return true;
    if(dh2_character_cmd_attack(&controller,&ai,target,&services)){error="Original controller attack dispatch rejected live owner/providers";return false;}
    return true;
}
bool retained_boundary(dh2::character::AttackState64& ai,const RetainedSequenceBoundary& source,
                       Boundary providers,BoundaryEffects& effects,RetainedSequenceCursorDecision& decision,std::string& error){
    providers.depth=source.depth;providers.step=source.step;providers.count=source.count;
    if(!(source.beginning?begin(ai,providers,effects,error):end(ai,providers,effects,error)))return false;
    for(const auto& operation:effects.operations){
        if(operation.service==dh2::character::attack_anim_set_step_v1)decision.setStep=operation.value;
        else if(operation.service==dh2::character::attack_anim_skip_next_v1)decision.skipNext=true;
    }
    return true;
}
bool apply_cursor_operations(const BoundaryEffects& effects,dh2::data::AnimationFrame& cursor,std::string& error){
    error.clear();
    for(const auto& operation:effects.operations){
        if(operation.service==dh2::character::attack_anim_set_step_v1)cursor.step=operation.value;
        else if(operation.service==dh2::character::attack_anim_skip_next_v1)++cursor.step;
    }
    return true;
}
bool advance_group(std::int32_t type,std::uint32_t count,std::uint32_t step,std::int32_t loops,
                   const BoundaryEffects& effects,GroupAdvance& output,std::string& error){
    error.clear();if(type<0||type>2||!count){error="Invalid original source group Type/count";return false;}
    dh2::data::AnimationFrame cursor{-1,loops,step};
    if(!apply_cursor_operations(effects,cursor,error))return false;
    // AnimationScheduler::complete_with_services permits synchronous End
    // callbacks to set the live cursor to count. Its older body-only kernel
    // rejects such indices, so preserve the actual live branch here.
    std::uint32_t completion=2;
    if(cursor.step<count)completion=dh2_animation_complete(type,count,&cursor.step,&cursor.loops);
    else {
        if(type==1)++cursor.step;
        if(cursor.loops==0)completion=2;
        else {if(cursor.loops>0)--cursor.loops;completion=0;}
    }
    output={completion,cursor.step,cursor.loops};return true;
}
bool select_start(const dh2::data::AnimationTables& tables,std::int32_t id,
                  dh2::data::AnimationRandom& rng,dh2::data::AnimationStart& output,
                  std::string& error,bool enabled){
    return dh2::data::choose_animation_start(tables,id,rng,output,error,enabled);
}
}
