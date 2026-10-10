#include "b035_session_push_effect_v1.hpp"
#include "../skills_animation/skill_animation_program.hpp"
#include "../../content_paths.hpp"

namespace dh::foundation::combat {
namespace {
bool same_owner(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) noexcept {
    return !a.owner_before(b) && !b.owner_before(a);
}
}

bool capture_session_push_admission_v1(CombatSession& session,
    const CombatSessionSourceHit& hit, SessionPushAdmissionV1& output,
    std::string& error) {
    error.clear();
    if (hit.binding_lease.expired() || !hit.generation || hit.source_id.empty() ||
        hit.marker_name.empty() || hit.attacker == invalid_actor_id ||
        hit.target == invalid_actor_id ||
        !same_owner(hit.binding_lease, session.actor_binding_lease())) {
        error = "Push admission requires a current same-Session source occurrence";
        return false;
    }
    const auto* attacker = session.actor(hit.attacker);
    const auto* target = session.actor(hit.target);
    const auto* traits = session.world() ? session.world()->traits(hit.target) : nullptr;
    if (!attacker || !target || !traits) {
        error = "Push admission requires both live Session actors and target source role";
        return false;
    }
    const auto source_state = session.original_actor_state(hit.target);
    if (source_state < 0 || source_state > 18) {
        error = "Push admission requires a known pre-hit original target state";
        return false;
    }

    SessionPushAdmissionV1 next;
    next.attacker = hit.attacker;
    next.target = hit.target;
    next.binding_lease = hit.binding_lease;
    next.generation = hit.generation;
    next.event_index = hit.event_index;
    next.source_id = hit.source_id;
    next.marker_name = hit.marker_name;
    next.target_state_before_hit = source_state;
    next.target_was_player = traits->is_player;
    next.target_was_alive = target->alive();
    // F_ApplyResult's status block requires defender IsPlayer and
    // SM_IsIdle(machine,0); state3 is the explicit Session Idle projection.
    next.source_push_gate_admitted = next.target_was_player &&
        next.target_was_alive && source_state == 3;
    output = std::move(next);
    return true;
}

bool consume_session_push_result_v1(CombatSession& session,
    SessionPushAdmissionV1& admission, const DamageEvent& receipt,
    const SessionPushEffectSinkV1& sink, bool& consumed, std::string& error) {
    consumed = false;
    error.clear();
    if (admission.binding_lease.expired() ||
        !same_owner(admission.binding_lease, session.actor_binding_lease()) ||
        admission.attacker == invalid_actor_id || admission.target == invalid_actor_id ||
        !admission.generation || admission.source_id.empty() || admission.marker_name.empty()) {
        error = "Push result requires the live Session and admission that captured this source hit";
        return false;
    }
    if (receipt.attacker != admission.attacker || receipt.target != admission.target ||
        receipt.source_id != admission.source_id || receipt.marker_name != admission.marker_name) {
        error = "Push result does not match its captured source occurrence";
        return false;
    }
    if (admission.consumed || !receipt.applied || !admission.source_push_gate_admitted ||
        receipt.target_died) return true;
    if (!receipt.source_outcomes || !receipt.source_mask) {
        error = "Applied source result lacks original outcome or result-mask bits";
        return false;
    }
    if ((*receipt.source_outcomes & 0x80u) == 0) {
        admission.consumed = true;
        return true;
    }
    if (!sink) {
        error = "Push result reached an admitted target but no same-Session effect provider is bound";
        return false;
    }
    const auto* attacker = session.actor(admission.attacker);
    const auto* target = session.actor(admission.target);
    if (!attacker || !target || !target->alive()) {
        error = "Push result actors changed before the same-Session effect was consumed";
        return false;
    }

    SessionPushRequestV1 request;
    request.attacker = admission.attacker;
    request.target = admission.target;
    request.generation = admission.generation;
    request.event_index = admission.event_index;
    request.source_id = admission.source_id;
    request.marker_name = admission.marker_name;
    request.source_outcomes = *receipt.source_outcomes;
    request.source_mask = *receipt.source_mask;
    request.great = (request.source_mask & 0x00100000u) != 0;
    request.direct = (request.source_mask & 0x18000000u) != 0;
    request.target_state_before_hit = admission.target_state_before_hit;

    // Mark before entering the host callback: if it fails after a physical/FSM
    // prefix, a retry must not replay that state transition.
    admission.consumed = true;
    if (!sink(session, request, error)) {
        if (error.empty()) error = "Same-Session Push effect provider failed";
        return false;
    }
    consumed = true;
    return true;
}

bool build_session_push_animation_bank_v1(const AssetCatalog& assets,
    const dh2::data::AnimationTables& animations,const dh2::data::Dictionary& clips,
    std::int32_t target_animation_table,std::int32_t stance,
    std::uint32_t stanced_animation_mask,const CharacterVisualConfig& visual,
    const std::string& role,
    SessionPushAnimationBankV1& output,std::string& error) {
    error.clear();
    if(target_animation_table<0||
       std::size_t(target_animation_table)>=animations.characters.size()){
        error="Push animation bank requires a valid actor CharAnimTable row";return false;
    }
    if(stance<0){error="Push animation bank requires an explicit nonnegative stance index";return false;}
    const auto& fields=animations.characters[static_cast<std::size_t>(target_animation_table)].fields;
    const auto source_root=[&](std::size_t field,std::uint32_t stanced_bit,
                               std::int32_t& resolved)->bool{
        if(fields[field].size()!=1){error="Push animation table scalar field is malformed";return false;}
        const auto base=fields[field][0];
        if(base<0){resolved=-1;return true;} // Original SM_SetKnockBackState no transition.
        const std::int64_t value=std::int64_t(base)+
            ((stanced_animation_mask&stanced_bit)?stance:0);
        if(value<0||std::size_t(value)>=animations.sequences.size()){
            error="Stanced Push animation root is outside the source sequence table";return false;
        }
        resolved=static_cast<std::int32_t>(value);return true;
    };
    std::int32_t great=-1,normal=-1;
    if(!source_root(8,0x800u,great)||!source_root(16,0x400u,normal))return false;
    if(great<0&&normal<0){error="Push animation bank has no authored source root";return false;}
    std::vector<std::int32_t> roots;
    if(normal>=0)roots.push_back(normal);
    if(great>=0&&great!=normal)roots.push_back(great);
    if(roots.empty())roots.push_back(great);
    skills_animation::SkillAnimationPrograms source_bank;
    if(!skills_animation::build_skill_animation_programs(assets,animations,clips,visual,
        roots,role,source_bank,error))return false;
    const auto rename_root=[&](std::int32_t sequence_id,const std::string& state)->bool{
        if(sequence_id<0)return true;
        auto found=std::find_if(source_bank.plan.sequences.begin(),source_bank.plan.sequences.end(),
            [&](const auto& sequence){return sequence.id==sequence_id;});
        if(found==source_bank.plan.sequences.end())return false;
        auto copy=*found;copy.state=state;
        auto existing=std::find_if(source_bank.plan.sequences.begin(),source_bank.plan.sequences.end(),
            [&](const auto& sequence){return sequence.state==state;});
        if(existing==source_bank.plan.sequences.end())source_bank.plan.sequences.push_back(std::move(copy));
        else *existing=std::move(copy);
        if(std::find(source_bank.plan.stateNames.begin(),source_bank.plan.stateNames.end(),state)==
           source_bank.plan.stateNames.end())source_bank.plan.stateNames.push_back(state);
        return true;
    };
    if(!rename_root(normal,"KnockedBack")||!rename_root(great,"GreatKnockedBack")){
        error="Push animation bank lost an authored root while naming the two variants";return false;
    }
    SessionPushAnimationBankV1 next;
    next.plan=std::move(source_bank.plan);
    next.policies=std::move(source_bank.policies);
    next.normal_sequence=normal;next.great_sequence=great;
    next.stance=stance;next.stanced_animation_mask=stanced_animation_mask;
    output=std::move(next);return true;
}

bool play_session_push_animation_v1(CombatSession& session,
    const SessionPushRequestV1& request,const SessionPushAnimationBankV1& bank,
    CombatSessionStateAnimationServices services,std::string& error) {
    error.clear();
    if(request.attacker==invalid_actor_id||request.target==invalid_actor_id||
       request.target_state_before_hit!=3||(request.source_outcomes&0x80u)==0){
        error="Push animation requires an admitted incoming Player/Idle Push request";return false;
    }
    const auto* attacker=session.actor(request.attacker);
    auto* target=session.actor(request.target);
    if(!attacker||!target||!target->alive()){
        error="Push animation actors are no longer live in the same Session";return false;
    }
    const auto state=request.great?bank.great_state:bank.normal_state;
    const auto sequence=request.great?bank.great_sequence:bank.normal_sequence;
    if(sequence<0)return true; // The original state setter returns without a pose.
    if(!bank.plan.sequence(state,0)){
        error="Push animation bank lacks the source-selected authored sequence";return false;
    }
    OriginalAttackSelection selection;
    selection.state=state;selection.variant=0;selection.actor_rate=1.0;
    // The source clip's own authored MoveGO is retained in the bank. Session
    // root-motion extraction owns actor/body movement and the caller's current
    // MotionHandler/PhaseHandler remains the collision authority.
    const auto prior_finished=services.finished;
    services.finished=[&session,target_id=request.target,prior_finished](ActorId actor_id,
        std::string& finish_error){
        if(actor_id!=target_id){finish_error="Push sequence completed for another actor";return false;}
        if(prior_finished&&!prior_finished(actor_id,finish_error))return false;
        auto* current=session.actor(target_id);
        if(!current){finish_error="Push target left the same Session before pose completion";return false;}
        return true;
    };
    if(!session.play_actor_source_sequence(request.target,bank.plan,bank.policies,
        selection,std::move(services),error))return false;
    // CharacterAction has no KnockedBack value. Retained source-sequence
    // ownership is the active pose here; leave the coarse action out of Hurt
    // so ActorCombatRuntime does not restart Injury and reclaim the same pose.
    // The integrating state owner must publish/gate original state 10.
    target=session.actor(request.target);
    if(!target||!target->alive()){
        error="Push target changed before the retained pose was published";return false;
    }
    target->action=CharacterAction::idle;
    target->action_elapsed_seconds=0;
    return true;
}

} // namespace dh::foundation::combat
