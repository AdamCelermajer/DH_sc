#include "source_combo_consumers.hpp"
#include <utility>

namespace dh::foundation::combo {
bool refresh_owner_fields(dh2::character::AttackState64& ai,const OwnerFieldBindings& b,std::string& error){
    error.clear();
    if(!b.lease||!b.owner||ai.owner!=b.owner||!b.flags528||!b.headingActive1b5||!b.objectOfInterest14a4||!b.objectOfInterestType14a8){
        error="Reached genuine same-owner flags528/heading1b5/OOI14a4/type14a8 backing is missing";return false;
    }
    ai.owner_flags528=*b.flags528;ai.heading_active=*b.headingActive1b5;
    ai.object_of_interest=*b.objectOfInterest14a4;ai.object_of_interest_type=*b.objectOfInterestType14a8;
    return true;
}
bool SourceComboConsumers::coherent(std::string& error)const{
    const auto& b=bindings_;
    if(!b.lease||!b.attack||!b.targets||!b.targets->state||!b.targets->state->owner||
       !b.controller||!b.attack->owner||b.controller->owner!=b.attack->owner||
       b.targets->state->owner->identity!=b.attack->owner){error="Canonical combo AI/target/controller ownership lease is missing or mismatched";return false;}
    return true;
}
bool SourceComboConsumers::bind(ConsumerBindings bindings,std::string& error){
    auto previous=std::move(bindings_);bindings_=std::move(bindings);
    if(!coherent(error)){bindings_=std::move(previous);return false;}
    error.clear();return true;
}
bool SourceComboConsumers::publish(std::string& error){
    auto& b=bindings_;const auto& target=*b.targets->state;
    b.attack->target=target.target;b.attack->last_target=target.last_target;
    if(!b.publishTargets){error="Reached source target publication provider is missing";return false;}
    return b.publishTargets(target,error);
}
bool SourceComboConsumers::execute(const Operation& operation,bool& handled,std::string& error){
    using namespace dh2::character;
    handled=operation.service==attack_anim_controller_look_v1||operation.service==attack_anim_pre_attack_v1||operation.service==attack_anim_clear_nonsticky_v1;
    error.clear();if(!handled)return true;if(!coherent(error))return false;
    auto& b=bindings_;
    if(operation.service==attack_anim_clear_nonsticky_v1){
        // _ClearNonStickyTarget3d8d70 checks actual AI+4b before any services.
        if(b.targets->state->changed)return true;
        const auto status=dh2_character_clear_target(b.targets->state,&b.targets->services);
        // A failed deep setter may already have written candidate/owner fields.
        // Preserve that exact prefix in host views, without invented rollback.
        std::string publicationError;const bool published=publish(publicationError);
        if(status){error="Original nonsticky target clear failed after source prefix";return false;}
        if(!published){error=std::move(publicationError);return false;}return true;
    }
    if(operation.service==attack_anim_pre_attack_v1){
        bool allowed=false;
        if(!world_ai_can_attack_v1(b.attack->owner,0,b.targets->state->target,b.canAttack,allowed,error))return false;
        if(!allowed)return true;
        if(!b.activeAIS){error="Reached actual active AIS+1c storage is missing";return false;}
        const auto active=*b.activeAIS; // source reload after eligibility callbacks
        if(!active)return true;
        if(!b.aisPreAttack){error="Reached original active AIS OnPreAttack virtual is missing";return false;}
        return b.aisPreAttack(active,static_cast<std::int32_t>(operation.value),error);
    }
    struct Bridge {
        ConsumerBindings* bindings;std::string failure;
        static int invoke(void* context,const CharacterControlRequest32* request,CharacterControlResponse16* response){
            auto& bridge=*static_cast<Bridge*>(context);auto& b=*bridge.bindings;*response={};
            if(request->service==control_target_position){
                const float* actual=nullptr;
                if(!b.targetPosition||!b.targetPosition(request->subject,actual,bridge.failure)||!actual){if(bridge.failure.empty())bridge.failure="Reached genuine GetTargetPosition backing is missing";return -1;}
                for(unsigned i=0;i<3;++i)response->position[i]=actual[i];return 1;
            }
            if(request->service==control_look_at_point){
                if(!b.gamePosition||!b.headingAngle){bridge.failure="Reached same GameObject position/heading-angle backing is missing";return -1;}
                LookAtState16 look{{b.gamePosition[0],b.gamePosition[1],b.gamePosition[2]},*b.headingAngle};
                if(dh2_character_look_at_point(&look,request->position)){bridge.failure="Original GameObject LookAt(Point) rejected backing";return -1;}
                *b.headingAngle=look.heading_angle;return 1;
            }
            bridge.failure="Unexpected original LookAt controllable service";return -1;
        }
    } bridge{&b,{}};
    const CharacterControlServices16 services{&bridge,Bridge::invoke};
    if(dh2_character_controller_character(b.controller,controller_look_object,operation.subject,&services)!=1){
        error=bridge.failure.empty()?"Original controller/Character LookAt failed":std::move(bridge.failure);return false;
    }
    return true;
}
}
