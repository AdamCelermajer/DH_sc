#include "actor_combat_runtime.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <set>

namespace dh::foundation {

bool ActorCombatRuntime::inspect_clip(Binding& binding, const std::string& clip,
                                     const AnimationMarkers*& markers, std::uint64_t& duration,
                                     std::string& error) {
    std::int32_t start = 0, end = 0;
    if (clip.empty() || !binding.visual.range(clip, start, end, error)) {
        if (error.empty()) error = "Combat clip is not bound";
        return false;
    }
    markers = binding.visual.markers(clip, error);
    if (!markers || start < 0 || end < start || markers->start_ms() != start || markers->end_ms() != end) {
        if (error.empty()) error = "Combat animation range and marker track disagree";
        return false;
    }
    duration = static_cast<std::uint64_t>(std::int64_t(end) - start);
    return true;
}

bool ActorCombatRuntime::bind(ActorState& actor, CombatVisualBinding visual,
                              CombatPoseBindings poses, std::string& error) {
    error.clear();
    if(!validate_transition_checkpoint(error))return false;
    if (!validate_actor_state(actor, error)) return false;
    if (!visual.select || !visual.range || !visual.markers || !visual.update) {
        error = "Combat visual binding is incomplete"; return false;
    }
    if(bool(visual.source_clock)!=bool(visual.take_source_events)||bool(visual.source_clock)!=bool(visual.source_finished)) {
        error="Source combat clock, events and completion services must be complete";return false;
    }
    if(visual.update_source&&!visual.source_clock) {error="Synchronous combat events require a source clock";return false;}
    if(bool(poses.transition_state)!=bool(poses.transition)){
        error="Accepted transition state and consumer must be supplied together";return false;
    }
    if (bindings_.count(actor.id)) { error = "Combat actor is already bound"; return false; }
    Binding binding; binding.actor = &actor; binding.visual = std::move(visual); binding.poses = std::move(poses);
    for (const auto& rate : binding.poses.clip_rates) {
        if (rate.first.empty() || !std::isfinite(rate.second) || rate.second <= 0) {
            error = "Combat clip playback rate is invalid"; return false;
        }
    }
    for (const auto* clip : {&binding.poses.react_clip_id, &binding.poses.death_clip_id}) {
        if (!clip->empty()) {
            const AnimationMarkers* track = nullptr; std::uint64_t duration = 0;
            if (!inspect_clip(binding, *clip, track, duration, error)) return false;
        }
    }
    if(binding.poses.restore_dead_terminal&&!actor.alive()&&!binding.poses.death_clip_id.empty()) {
        if(binding.visual.source_clock&&!binding.visual.hold_terminal) {error="Source saved corpse requires terminal pose publication without clock replay";return false;}
        if(!start_pose(binding,Pose::death,error,nullptr,true))return false;
        if(binding.visual.hold_terminal) {
            if(!binding.visual.hold_terminal(binding.poses.death_clip_id,error))return false;
        }else if(!binding.visual.update(double(binding.duration_ms)/1000,error))return false;
        binding.elapsed=double(binding.duration_ms)/1000;binding.cursor.elapsed_ms=binding.duration_ms;binding.terminal_hold=true;
    }
    bindings_.emplace(actor.id, std::move(binding));
    return true;
}

bool ActorCombatRuntime::capture_transition(Binding& binding,std::int32_t to,
    CombatRuntimeTransitionCause cause,std::uint64_t generation,
    CombatRuntimeTransition& receipt,std::string& error){
    receipt={};receipt.actor=binding.actor->id;receipt.to_state=to;
    receipt.generation=generation;receipt.cause=cause;
    if(!binding.transition_failure.empty()){error=binding.transition_failure;return false;}
    if(binding.delivering_transition){error="Accepted actor transition cannot reenter";return false;}
    if(!binding.poses.transition_state)return true;
    struct Scope{bool& flag;~Scope(){flag=false;}} scope{binding.delivering_transition};binding.delivering_transition=true;
    try{
        if(!binding.poses.transition_state(receipt.from_state,error))return false;
        if(receipt.from_state< -1||receipt.from_state>18){error="Accepted transition source state is invalid";return false;}
        return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
      catch(...){error="Accepted transition state query threw";return false;}
}
bool ActorCombatRuntime::transition(Binding& binding,CombatRuntimeTransition receipt,
    CombatRuntimeTransitionStage stage,std::string& error){
    if(receipt.from_state==-1)return true;
    if(!binding.transition_failure.empty()){error=binding.transition_failure;return false;}
    if(binding.delivering_transition){error="Accepted actor transition cannot reenter";return false;}
    receipt.stage=stage;
    struct Scope{bool& flag;~Scope(){flag=false;}} scope{binding.delivering_transition};binding.delivering_transition=true;
    try{
        if(binding.poses.transition(receipt,error))return true;
        if(error.empty())error="Accepted actor transition consumer rejected after its reached prefix";
    }catch(const std::exception& ex){error=ex.what();}
     catch(...){error="Accepted actor transition consumer threw after its reached prefix";}
    binding.transition_failure=error;return false;
}
bool ActorCombatRuntime::depart(Binding& binding,CombatRuntimeTransitionCause cause,std::string& error){
    if(binding.pose==Pose::none)return true;
    CombatRuntimeTransition receipt;
    if(!capture_transition(binding,binding.actor->alive()?3:12,cause,binding.cursor.generation,receipt,error)||
       !transition(binding,receipt,CombatRuntimeTransitionStage::before_change,error))return false;
    release(binding);
    return transition(binding,receipt,CombatRuntimeTransitionStage::after_change,error);
}
bool ActorCombatRuntime::validate_transition_checkpoint(std::string& error)const{
    for(const auto& pair:bindings_){
        if(pair.second.delivering_transition){error="Accepted actor transition is in flight";return false;}
        if(!pair.second.transition_failure.empty()){error=pair.second.transition_failure;return false;}
    }
    error.clear();return true;
}
bool ActorCombatRuntime::deferred_death(ActorId id,bool& defer,std::string& error){
    defer=false;const auto target=bindings_.find(id);
    if(target==bindings_.end()||target->second.poses.death_clip_id.empty())return true;
    CombatRuntimeTransition receipt;
    if(!capture_transition(target->second,12,CombatRuntimeTransitionCause::death,0,receipt,error))return false;
    defer=receipt.from_state!=-1;return true;
}

void ActorCombatRuntime::release(Binding& binding) {
    if (binding.pose == Pose::attack) combat_.interrupt(binding.actor->id);
    if (binding.pose == Pose::react && binding.actor->action == CharacterAction::hurt)
        reset_actor_action(*binding.actor, CharacterAction::idle);
    binding.pose = Pose::none; binding.markers = nullptr; binding.elapsed = 0; binding.duration_ms = 0;
    binding.terminal_hold=false;
}

bool ActorCombatRuntime::unbind(ActorId actor) {
    std::string error;if(!validate_transition_checkpoint(error))return false;
    const auto entry = bindings_.find(actor);
    if (entry == bindings_.end()) return false;
    release(entry->second); bindings_.erase(entry); return true;
}

bool ActorCombatRuntime::begin(ActorId attacker, ActorId target, const AttackDefinition& definition,
                               std::string& error) {
    error.clear();
    if(!validate_transition_checkpoint(error))return false;
    const auto entry = bindings_.find(attacker);
    if (entry == bindings_.end()) { error = "Attacking actor has no combat visual binding"; return false; }
    auto& binding = entry->second;
    if(binding.delivering_transition||!binding.transition_failure.empty()){
        error=binding.transition_failure.empty()?"Accepted actor transition cannot reenter":binding.transition_failure;return false;
    }
    if (binding.pose != Pose::none) { error = "Actor already owns a combat pose"; return false; }
    if (!validate_attack_definition(definition, error)) return false;
    const AnimationMarkers* track = nullptr; std::uint64_t duration = 0;
    if (!inspect_clip(binding, definition.animation_clip_id, track, duration, error)) return false;
    for (const auto& marker : definition.damage_markers) {
        if (std::none_of(track->markers().begin(), track->markers().end(),
            [&](const AnimationMarker& authored) { return authored.name == marker.marker_name; })) {
            error = "Attack damage binding has no authored clip marker: " + marker.marker_name; return false;
        }
    }
    auto cursor = binding.cursor;
    if (!AnimationMarkers::restart(cursor, error)) return false;
    if(!combat_.validate_begin(attacker,target,definition,cursor.generation,error))return false;
    CombatRuntimeTransition accepted;
    if(!capture_transition(binding,5,CombatRuntimeTransitionCause::attack,cursor.generation,accepted,error))return false;
    // Retire the admitted occurrence before delivery so a rejected consumer
    // cannot reuse the same generation after reaching an external effect.
    binding.cursor=cursor;
    if(!transition(binding,accepted,CombatRuntimeTransitionStage::before_change,error))return false;
    if (!combat_.begin(attacker, target, definition, cursor.generation, error)) {
        if(accepted.from_state!=-1)binding.transition_failure=error;return false;
    }
    if (!binding.visual.select(definition.animation_clip_id, false, error)) {
        combat_.interrupt(attacker);if(accepted.from_state!=-1)binding.transition_failure=error.empty()?"Accepted attack selection failed after transition prefix":error;return false;
    }
    binding.cursor = cursor; binding.markers = track; binding.duration_ms = duration;
    const auto rate = binding.poses.clip_rates.find(definition.animation_clip_id);
    binding.playback_rate = rate == binding.poses.clip_rates.end() ? 1 : rate->second;
    binding.elapsed = 0; binding.pose = Pose::attack;
    return transition(binding,accepted,CombatRuntimeTransitionStage::after_change,error);
}

bool ActorCombatRuntime::apply_calculated_hit(ActorId attacker, ActorId target,
    const std::string& source_id, const std::string& occurrence_id, float amount,
    std::optional<std::uint32_t> outcomes, std::optional<std::uint32_t> source_mask,
    DamageEvent& result, std::string& error) {
    if(!validate_transition_checkpoint(error))return false;
    bool defer=false;if(!deferred_death(target,defer,error))return false;
    if (!combat_.apply_calculated_hit(attacker, target, source_id, occurrence_id,
                                     amount, outcomes, source_mask, result, error,defer))
        return false;
    // The receipt remains available even if post-hit visual/death publication
    // fails; damage is incremental and must never be rolled back or repeated.
    return handle_applied_hit(result, nullptr, 0.0, false, error);
}

bool ActorCombatRuntime::start_pose(Binding& binding, Pose pose, std::string& error,
    const CombatRuntimeTransition* admitted,bool silent,bool prefix_delivered) {
    const auto& clip = pose == Pose::death ? binding.poses.death_clip_id : binding.poses.react_clip_id;
    if (clip.empty()) return true;
    const AnimationMarkers* track = nullptr; std::uint64_t duration = 0;
    if (!inspect_clip(binding, clip, track, duration, error)) return false;
    auto cursor = binding.cursor;
    if (!AnimationMarkers::restart(cursor, error)) return false;
    CombatRuntimeTransition accepted;
    if(admitted){accepted=*admitted;accepted.generation=cursor.generation;}
    else if(!silent&&!capture_transition(binding,pose==Pose::death?12:11,
                pose==Pose::death?CombatRuntimeTransitionCause::death:CombatRuntimeTransitionCause::injury,
                cursor.generation,accepted,error))return false;
    if(silent)accepted.from_state=-1;
    binding.cursor=cursor;
    if(!prefix_delivered&&!transition(binding,accepted,CombatRuntimeTransitionStage::before_change,error))return false;
    release(binding);
    if (!binding.visual.select(clip, false, error)) {
        if(accepted.from_state!=-1)binding.transition_failure=error.empty()?"Accepted pose selection failed after transition prefix":error;return false;
    }
    binding.cursor = cursor; binding.markers = track; binding.duration_ms = duration;
    const auto rate = binding.poses.clip_rates.find(clip);
    binding.playback_rate = rate == binding.poses.clip_rates.end() ? 1 : rate->second;
    binding.elapsed = 0; binding.pose = pose;
    reset_actor_action(*binding.actor, pose == Pose::death ? CharacterAction::dead : CharacterAction::hurt);
    return transition(binding,accepted,CombatRuntimeTransitionStage::after_change,error);
}

bool ActorCombatRuntime::deliver_pose_motion(Binding& binding,Pose pose,std::string& error) {
    if(pose!=Pose::react&&pose!=Pose::death)return true;
    if(!binding.visual.take_root_motion)return true;
    const Vec3 delta=binding.visual.take_root_motion();
    if(!binding.visual.apply_motion)return true;
    const bool moveGo=pose==Pose::death?binding.poses.death_move_go:binding.poses.react_move_go;
    if(!binding.visual.apply_motion(delta,moveGo,error)) {
        if(error.empty())error="Reaction/death root motion delivery failed";
        return false;
    }
    return true;
}

bool ActorCombatRuntime::start_injure_reaction(Binding& binding,const DamageEvent& result,
                                               bool& started,std::string& error) {
    started=false;
    if(!result.source_outcomes||!(*result.source_outcomes&0x10u))return true;
    if(binding.poses.source_injury_gate_enabled) {
        // NPC and player source setters share Character+0x14fc. Positive is a
        // successful no-op; a fresh request writes 3000 before clip lookup.
        if(binding.source_injury_gate_ms>0.0f)return true;
        binding.source_injury_gate_ms=3000.0f;
    }
    if(binding.poses.react_clip_id.empty())return true;
    CombatRuntimeTransition accepted;
    if(!capture_transition(binding,11,CombatRuntimeTransitionCause::injury,0,accepted,error))return false;
    if(binding.poses.injury_transition){
        bool accepted=false;
        if(!binding.poses.injury_transition(result,accepted,error))return false;
        if(!accepted)return true;
    }
    auto next_cursor=binding.cursor;
    if(!AnimationMarkers::restart(next_cursor,error))return false;
    accepted.generation=next_cursor.generation;
    if(!transition(binding,accepted,CombatRuntimeTransitionStage::before_change,error))return false;
    if(binding.poses.injury_choice){
        release(binding);
        std::string clip;bool move_go=false;
        if(!binding.poses.injury_choice(result,clip,move_go,error)){
            if(accepted.from_state!=-1)binding.transition_failure=error.empty()?"Injury choice failed after transition prefix":error;return false;
        }
        if(clip.empty()){error="Admitted injury choice returned no original clip";if(accepted.from_state!=-1)binding.transition_failure=error;return false;}
        binding.poses.react_clip_id=std::move(clip);binding.poses.react_move_go=move_go;
    }
    if(!start_pose(binding,Pose::react,error,&accepted,false,true))return false;
    started=binding.pose==Pose::react;
    return true;
}

bool ActorCombatRuntime::synchronize(Binding& binding, std::string& error) {
    auto& actor = *binding.actor;
    if (!actor.alive()) {
        if (binding.pose != Pose::death) {
            if (!start_pose(binding, Pose::death, error)) return false;
        }
        return true;
    }
    if(binding.pose==Pose::attack){
        CombatRuntimeTransition current;
        if(!capture_transition(binding,3,CombatRuntimeTransitionCause::interruption,binding.cursor.generation,current,error))return false;
        if(current.from_state!=-1&&!combat_.active_attack_valid(actor.id))
            return depart(binding,CombatRuntimeTransitionCause::interruption,error);
    }
    if (binding.pose == Pose::death&&!depart(binding,CombatRuntimeTransitionCause::interruption,error))return false;
    if (binding.pose == Pose::attack
        && (!combat_.attacking(actor.id) || actor.action != CharacterAction::attacking)&&
        !depart(binding,CombatRuntimeTransitionCause::interruption,error))return false;
    if (binding.pose == Pose::react && actor.action != CharacterAction::hurt&&
        !depart(binding,CombatRuntimeTransitionCause::interruption,error))return false;
    if (binding.pose == Pose::none && actor.action == CharacterAction::hurt)
        return start_pose(binding, Pose::react, error);
    return true;
}

bool ActorCombatRuntime::interrupt(ActorId actor) {
    std::string error;return interrupt(actor,error);
}
bool ActorCombatRuntime::interrupt(ActorId actor,std::string& error) {
    error.clear();if(!validate_transition_checkpoint(error))return false;
    const auto entry = bindings_.find(actor);
    if (entry == bindings_.end() || entry->second.pose == Pose::none) return false;
    return depart(entry->second,CombatRuntimeTransitionCause::interruption,error);
}

bool ActorCombatRuntime::consume_source_marker(ActorId actor, std::uint64_t generation,
    const SourceCombatMarker& event, std::uint64_t elapsed_ms,
    std::vector<DamageEvent>& damage, std::string& error) {
    const auto entry=bindings_.find(actor);
    if(entry==bindings_.end())return true;
    auto& binding=entry->second;
    if(binding.pose!=Pose::attack||binding.cursor.generation!=generation)return true;
    if(!synchronize(binding,error))return false;
    if(binding.pose!=Pose::attack||binding.cursor.generation!=generation)return true;
    MarkerOccurrence marker;marker.generation=generation;marker.elapsed_ms=elapsed_ms;
    marker.marker.index=event.stream_index;marker.marker.name=event.name;
    marker.marker.time_ms=static_cast<std::int32_t>(std::min<std::uint64_t>(elapsed_ms,INT32_MAX));
    DamageEvent result;
    bool defer=false;if(!deferred_death(binding.actor->target_id,defer,error))return false;
    if(!combat_.consume_marker(actor,marker,result,error,defer)){release(binding);return false;}
    if(!result.applied)return true;
    return handle_applied_hit(result,&damage,0.0,false,error);
}

bool ActorCombatRuntime::handle_applied_hit(const DamageEvent& result,
    std::vector<DamageEvent>* receipts, double remaining_wall_seconds, bool sample_remainder,
    std::string& error) {
    if (!std::isfinite(remaining_wall_seconds) || remaining_wall_seconds < 0) {
        error = "Applied-hit remainder must be finite and nonnegative";
        return false;
    }
    if (receipts) receipts->push_back(result);
    const auto victim = bindings_.find(result.target);
    if (victim == bindings_.end()) return true;
    bool new_pose = false;
    if (result.target_died) {
        if (!synchronize(victim->second, error)) return false;
        new_pose = victim->second.pose == Pose::death;
    } else if (!start_injure_reaction(victim->second, result, new_pose, error)) {
        return false;
    }
    if (!new_pose || !sample_remainder) return true;

    auto& target_binding = victim->second;
    const double duration = double(target_binding.duration_ms) / 1000;
    const double remaining_wall = std::min(duration / target_binding.playback_rate,
                                           remaining_wall_seconds);
    const double remaining = std::min(duration, remaining_wall * target_binding.playback_rate);
    auto cursor = target_binding.cursor;
    std::vector<MarkerOccurrence> unused;
    const auto elapsed_ms = remaining >= duration ? target_binding.duration_ms
        : static_cast<std::uint64_t>(std::floor(remaining * 1000));
    if (!target_binding.markers->advance(cursor, elapsed_ms, false, unused, error)
        || !target_binding.visual.update(remaining, error)) {
        release(target_binding); return false;
    }
    if (!deliver_pose_motion(target_binding, target_binding.pose, error)) {
        release(target_binding); return false;
    }
    target_binding.cursor = cursor; target_binding.elapsed = remaining;
    target_binding.actor->action_elapsed_seconds = static_cast<float>(remaining / target_binding.playback_rate);
    return true;
}

bool ActorCombatRuntime::update(double seconds, std::vector<DamageEvent>& damage, std::string& error) {
    error.clear();
    if(!validate_transition_checkpoint(error))return false;
    if (!std::isfinite(seconds) || seconds < 0 || seconds > double(std::numeric_limits<std::uint64_t>::max()) / 1000) {
        error = "Combat runtime elapsed time is invalid"; return false;
    }
    const double frame_ms_value=std::floor(seconds*1000.0);
    const bool source_gate_enabled=std::any_of(bindings_.begin(),bindings_.end(),[](const auto& item){
        return item.second.poses.source_injury_gate_enabled;
    });
    if(source_gate_enabled&&frame_ms_value>double(std::numeric_limits<std::uint32_t>::max())) {
        error="Source Injure gate frame exceeds unsigned millisecond clock";return false;
    }
    damage.clear();
    const auto frame_ms=source_gate_enabled?static_cast<std::uint32_t>(frame_ms_value):0u;
    for(auto& item:bindings_)if(item.second.poses.source_injury_gate_enabled
       &&item.second.source_injury_gate_ms>0.0f)
        item.second.source_injury_gate_ms-=static_cast<float>(frame_ms);
    // Retire enabled source actors before Combat's compatibility invalid-target
    // cleanup clears their outgoing target/action/cooldown cells.
    for(auto& item:bindings_)if(item.second.pose==Pose::attack&&!synchronize(item.second,error))return false;
    if (!combat_.update(0, error)) return false;
    struct Pending { ActorId actor; MarkerOccurrence marker; double frame_seconds; bool finish; };
    std::vector<Pending> pending;
    std::set<ActorId> source_reentered;
    double clock = 0;
    for (auto& item : bindings_) {
        auto& binding = item.second;
        if (!synchronize(binding, error)) return false;
        if (binding.pose == Pose::none) continue;
        if(binding.pose==Pose::death&&binding.terminal_hold)continue;
        if(binding.visual.source_clock&&binding.visual.source_clock()) {
            if(binding.visual.update_source) {
                // Retained callbacks occur at this source sample, not at an
                // invented marker subframe. Advance timers once to that sample.
                if(!combat_.update(std::max(0.0,seconds-clock),error))return false;
                clock=seconds;
                const auto generation=binding.cursor.generation;
                const auto pose=binding.pose;
                const auto next_elapsed=binding.elapsed+seconds;
                const auto elapsed_ms=static_cast<std::uint64_t>(std::floor(next_elapsed*1000));
                binding.elapsed=next_elapsed;binding.cursor.elapsed_ms=elapsed_ms;
                binding.actor->action_elapsed_seconds=static_cast<float>(next_elapsed);
                std::string callback_error;
                bool callback_failed=false;
                const SourceCombatMarkerSink sink=[&,id=item.first,generation,elapsed_ms](const SourceCombatMarker& event,std::string& e){
                    if(callback_failed){e=callback_error;return false;}
                    if(!consume_source_marker(id,generation,event,elapsed_ms,damage,e)){
                        callback_failed=true;callback_error=e.empty()?"Synchronous source combat marker failed":e;e=callback_error;return false;
                    }
                    return true;
                };
                const bool updated=binding.visual.update_source(seconds,sink,error);
                if(!updated||callback_failed) {
                    if(callback_failed)error=callback_error;
                    else if(error.empty())error="Synchronous source combat visual update failed";
                    if(binding.cursor.generation==generation&&binding.pose==pose)release(binding);
                    return false;
                }
                if(binding.cursor.generation==generation&&binding.pose==pose
                   &&(pose==Pose::react||pose==Pose::death)
                   &&!deliver_pose_motion(binding,pose,error))return false;
                // Source callbacks can replace/interrupt the pose. Never close
                // the replacement using the old sample's completion latch.
                if(binding.cursor.generation!=generation||binding.pose!=pose){source_reentered.insert(item.first);continue;}
                if(binding.pose==Pose::attack&&binding.visual.source_finished()) {
                    if(!depart(binding,CombatRuntimeTransitionCause::completion,error))return false;
                }
                continue;
            }
            std::vector<SourceCombatMarker> events;
            if(!binding.visual.update(seconds,error)||!binding.visual.take_source_events(events,error)) {release(binding);return false;}
            if((binding.pose==Pose::react||binding.pose==Pose::death)
               &&!deliver_pose_motion(binding,binding.pose,error)){release(binding);return false;}
            binding.elapsed+=seconds;
            binding.actor->action_elapsed_seconds=static_cast<float>(binding.elapsed);
            const auto elapsedMs=static_cast<std::uint64_t>(std::floor(binding.elapsed*1000));
            binding.cursor.elapsed_ms=elapsedMs;
            if(binding.pose==Pose::attack) {
                for(const auto& event:events) {
                    MarkerOccurrence marker;marker.generation=binding.cursor.generation;marker.elapsed_ms=elapsedMs;
                    marker.marker.index=event.stream_index;marker.marker.name=event.name;
                    marker.marker.time_ms=static_cast<std::int32_t>(std::min<std::uint64_t>(elapsedMs,INT32_MAX));
                    // Original callbacks execute at the actual source sample,
                    // with lag retained by the playback owner for its consumers.
                    pending.push_back({item.first,std::move(marker),seconds,false});
                }
                if(binding.visual.source_finished()) {MarkerOccurrence end;end.generation=binding.cursor.generation;pending.push_back({item.first,std::move(end),seconds,true});}
            }
            continue;
        }
        const double duration = double(binding.duration_ms) / 1000;
        const double old_elapsed = binding.elapsed;
        const double remaining_source = std::max(0.0, duration - binding.elapsed);
        const double sample_wall_seconds = std::min(seconds, remaining_source / binding.playback_rate);
        const double sample_seconds = std::min(remaining_source, sample_wall_seconds * binding.playback_rate);
        const double next_elapsed = std::min(duration, binding.elapsed + sample_seconds);
        // Keep integer marker sampling and visual sampling on the same total clock;
        // fractional milliseconds accumulate instead of rounding every frame.
        const auto next_ms = next_elapsed >= duration ? binding.duration_ms
            : static_cast<std::uint64_t>(std::floor(next_elapsed * 1000));
        auto cursor = binding.cursor;
        std::vector<MarkerOccurrence> markers;
        const auto old_ms = cursor.elapsed_ms;
        if (!binding.markers->advance(cursor, next_ms - old_ms, false, markers, error)
            || !binding.visual.update(sample_seconds, error)) {
            release(binding); return false;
        }
        if((binding.pose==Pose::react||binding.pose==Pose::death)
           &&!deliver_pose_motion(binding,binding.pose,error)){release(binding);return false;}
        binding.cursor = cursor; binding.elapsed = next_elapsed;
        binding.actor->action_elapsed_seconds = static_cast<float>(next_elapsed / binding.playback_rate);
        if (binding.pose == Pose::attack) for (auto& marker : markers) {
            const double offset = std::max(0.0, (double(marker.elapsed_ms) / 1000 - old_elapsed) / binding.playback_rate);
            pending.push_back({item.first, std::move(marker), std::min(seconds, offset), false});
        }
        if (binding.pose == Pose::attack && next_ms >= binding.duration_ms) {
            MarkerOccurrence end; end.generation = binding.cursor.generation;
            pending.push_back({item.first, std::move(end), sample_wall_seconds, true});
        }
    }
    std::stable_sort(pending.begin(), pending.end(), [](const Pending& a, const Pending& b) {
        if (a.frame_seconds != b.frame_seconds) return a.frame_seconds < b.frame_seconds;
        if (a.finish != b.finish) return !a.finish; // Authored end markers precede clip departure.
        return a.actor < b.actor;
    });
    for (const auto& event : pending) {
        if (!combat_.update(std::max(0.0,event.frame_seconds - clock), error)) return false;
        clock = std::max(clock,event.frame_seconds);
        auto& binding = bindings_.at(event.actor);
        if (!synchronize(binding, error)) return false;
        if (binding.pose != Pose::attack || binding.cursor.generation != event.marker.generation) continue;
        if (event.finish) {
            if(!depart(binding,CombatRuntimeTransitionCause::completion,error))return false;
            continue;
        }
        DamageEvent result;
        bool defer=false;if(!deferred_death(binding.actor->target_id,defer,error))return false;
        if (!combat_.consume_marker(event.actor, event.marker, result, error,defer)) { release(binding); return false; }
        if (result.applied && !handle_applied_hit(result,&damage,
                std::max(0.0,seconds-event.frame_seconds),true,error)) return false;
    }
    if (!combat_.update(std::max(0.0, seconds - clock), error)) return false;
    for (auto& item : bindings_) {
        auto& binding = item.second;
        if (!synchronize(binding, error)) return false;
        if(source_reentered.count(item.first))continue;
        const bool sourceClock=binding.visual.source_clock&&binding.visual.source_clock();
        const bool finished=sourceClock?binding.visual.source_finished():binding.cursor.elapsed_ms>=binding.duration_ms;
        if (binding.pose != Pose::none && binding.pose != Pose::death && finished) {
            if(!depart(binding,CombatRuntimeTransitionCause::completion,error))return false;
        }
    }
    return true;
}

bool ActorCombatRuntime::owns_pose(ActorId actor) const noexcept {
    const auto entry = bindings_.find(actor);
    return entry != bindings_.end() && entry->second.pose != Pose::none;
}
void ActorCombatRuntime::clear() {
    for(const auto& item:bindings_)if(item.second.delivering_transition)
        throw std::logic_error("Cannot clear Runtime during accepted transition delivery");
    for (auto& item : bindings_) release(item.second);
    bindings_.clear();
}

} // namespace dh::foundation
