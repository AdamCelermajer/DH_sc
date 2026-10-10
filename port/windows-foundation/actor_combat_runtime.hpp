#pragma once

#include "animation_markers.hpp"
#include "combat_system.hpp"
#include "original_character.hpp"
#include <functional>

namespace dh::foundation {
struct SourceCombatMarker { std::uint32_t stream_index=0;std::string name; };
using SourceCombatMarkerSink = std::function<bool(const SourceCombatMarker&, std::string&)>;

// Caller-owned visual callbacks also permit headless verification. Bound actor,
// visual and marker storage must outlive the binding; do not reload while owned.
struct CombatVisualBinding {
    std::function<bool(const std::string&, bool, std::string&)> select;
    std::function<bool(const std::string&, std::int32_t&, std::int32_t&, std::string&)> range;
    std::function<const AnimationMarkers*(const std::string&, std::string&)> markers;
    std::function<bool(double, std::string&)> update;
    // Optional source-owned clock. Range/markers then validate content only;
    // actual events and completion come from retained source timelines.
    std::function<bool()> source_clock;
    std::function<bool(std::vector<SourceCombatMarker>&,std::string&)> take_source_events;
    std::function<bool()> source_finished;
    std::function<bool(const std::string&,std::string&)> hold_terminal;
    // Direct reaction/death samples consume exactly one visual root delta and
    // pass it to the owning session's existing motion policy.
    std::function<Vec3()> take_root_motion;
    std::function<bool(Vec3,bool,std::string&)> apply_motion;
    // Optional synchronous source delivery. The sink is valid only during this
    // call and must run at the named source helper, before state forwarding.
    // This replaces take_source_events for that sample; never deliver both.
    std::function<bool(double, const SourceCombatMarkerSink&, std::string&)> update_source;
};

inline CombatVisualBinding combat_visual_binding(CharacterVisual& visual) {
    return {
        [&visual](const std::string& name, bool loop, std::string& error) { return loop?visual.select(name,true,error):visual.restart(name,false,error); },
        [&visual](const std::string& name, std::int32_t& start, std::int32_t& end, std::string& error) {
            return visual.animation_range(name, start, end, error);
        },
        [&visual](const std::string& name, std::string& error) { return visual.markers(name, error); },
        [&visual](double seconds, std::string& error) { return visual.update(seconds, error); }
    };
}

enum class CombatRuntimeTransitionCause { attack, injury, death, completion, interruption, locomotion, source_program };
enum class CombatRuntimeTransitionStage { before_change, after_change };
enum class CombatRuntimeHitEffectStage { before_effects, after_effects };
using CombatRuntimeHitEffectObserver = std::function<bool(const DamageEvent&,
    std::uint64_t occurrence,CombatRuntimeHitEffectStage,std::string&)>;
struct CombatRuntimeTransition {
    ActorId actor=invalid_actor_id;
    std::int32_t from_state=-1,to_state=-1;
    std::uint64_t generation=0;
    CombatRuntimeTransitionCause cause=CombatRuntimeTransitionCause::attack;
    CombatRuntimeTransitionStage stage=CombatRuntimeTransitionStage::before_change;
};

struct CombatPoseBindings {
    // Empty IDs disable the generic visual Injure reaction. A configured clip
    // starts only for an explicit source outcome bit 0x10; HP loss alone is not
    // evidence that the original result requested Injure.
    std::string react_clip_id;
    std::string death_clip_id;
    // Authored playback multipliers by named clip. Unlisted clips use the visual
    // clock's identity rate. Source elapsed scales; action elapsed is wall time.
    std::map<std::string, double> clip_rates;
    // Version1 save policy canonicalizes transients. A saved corpse keeps its
    // transform and terminal pose instead of replaying displacement on rebind.
    bool restore_dead_terminal=false;
    // Appended for aggregate-source compatibility with the prior pose binding.
    bool react_move_go=false;
    bool death_move_go=false;
    // Enable the original shared Character+0x14fc Injure admission gate for a
    // source-owned actor. Diagnostic fixtures opt in explicitly.
    bool source_injury_gate_enabled=false;
    // After the source 3000ms gate and clip lookup, before reaction focus.
    // Optional exact FSM admission; absent retains the prior diagnostic path.
    std::function<bool(const DamageEvent&,bool& accepted,std::string&)> injury_transition;
    // Admitted injury Focus after outgoing release. Source Type2 may choose a
    // leaf here, before incoming clip inspection/selection.
    std::function<bool(const DamageEvent&,std::string& clip,bool& move_go,std::string&)> injury_choice;
    // Optional accepted-transition consumer. The read-only state provider
    // captures source state BEFORE action/release mutations; -1 disables this
    // consumer for the current call. Callbacks run only after actual admission,
    // before and after action/pose publication. Held leaves emit no transition.
    // They may affect external body/navigation resources, not replace/reenter
    // Runtime/actors. Failure retains the reached prefix and blocks this binding
    // until fresh bind, rather than replaying that transition on a later frame.
    std::function<bool(std::int32_t&,std::string&)> transition_state;
    std::function<bool(const CombatRuntimeTransition&,std::string&)> transition;
};

class ActorCombatRuntime {
public:
    // AttackDefinition explicitly selects start-based compatibility or recovered
    // departure-based AttackDelay. Timers advance in wall time, not clip time.
    explicit ActorCombatRuntime(CombatSystem& combat) : combat_(combat) {}
    // Same applied receipt around Injury/Dead publication. The before phase
    // precedes visual/state effects; the core HP prefix is already committed.
    void set_hit_effect_observer(CombatRuntimeHitEffectObserver observer){hit_effect_observer_=std::move(observer);}
    // Actor must be the same stable record returned by CombatWorld::find_actor.
    bool bind(ActorState& actor, CombatVisualBinding visual,
              CombatPoseBindings poses, std::string& error);
    bool unbind(ActorId actor);
    bool begin(ActorId attacker, ActorId target, const AttackDefinition&, std::string& error);
    // Applies a result whose formula, RNG, source query and target admission
    // already ran. Shares marker-hit reaction/death handling without a reroll.
    bool apply_calculated_hit(ActorId attacker, ActorId target,
                              const std::string& source_id,
                              const std::string& occurrence_id, float amount,
                              std::optional<std::uint32_t> outcomes,
                              std::optional<std::uint32_t> source_mask,
                              DamageEvent& result, std::string& error);
    bool interrupt(ActorId actor);
    bool interrupt(ActorId actor,std::string& error);
    bool validate_transition_checkpoint(std::string& error)const;
    // This is the sole combat cooldown updater. Effects/errors are incremental:
    // already applied damage is retained; a failed action is safely interrupted.
    // Legacy queued markers sort by frame time, actor ID and author order.
    // update_source markers apply inside their actual callback/sample order;
    // mixed clocks do not establish original global Level traversal chronology.
    bool update(double seconds, std::vector<DamageEvent>& damage, std::string& error);
    // Locomotion owns animation sampling whenever this returns false. Dead poses
    // retain their last frame and remain owned until the actor revives/unbinds.
    bool owns_pose(ActorId actor) const noexcept;
    void clear();

private:
    enum class Pose { none, attack, react, death };
    struct Binding {
        ActorState* actor = nullptr;
        CombatVisualBinding visual;
        CombatPoseBindings poses;
        MarkerCursor cursor;
        const AnimationMarkers* markers = nullptr;
        Pose pose = Pose::none;
        double elapsed = 0;
        double playback_rate = 1;
        std::uint64_t duration_ms = 0;
        float source_injury_gate_ms=-1.0f;
        bool terminal_hold=false;
        bool delivering_transition=false;
        std::string transition_failure;
    };
    bool inspect_clip(Binding&, const std::string&, const AnimationMarkers*&,
                      std::uint64_t& duration, std::string& error);
    bool start_pose(Binding&, Pose pose, std::string& error,
                    const CombatRuntimeTransition* accepted=nullptr,bool silent=false,
                    bool prefix_delivered=false);
    bool capture_transition(Binding&,std::int32_t to,CombatRuntimeTransitionCause,
                            std::uint64_t generation,CombatRuntimeTransition&,std::string&);
    bool transition(Binding&,CombatRuntimeTransition,CombatRuntimeTransitionStage,std::string&);
    bool depart(Binding&,CombatRuntimeTransitionCause,std::string&);
    bool deferred_death(ActorId,bool&,std::string&);
    bool start_injure_reaction(Binding&,const DamageEvent&,bool& started,std::string& error);
    bool handle_applied_hit(const DamageEvent&, std::vector<DamageEvent>* receipts,
                            double remaining_wall_seconds, bool sample_remainder,
                            std::string& error);
    bool deliver_pose_motion(Binding&,Pose,std::string& error);
    void release(Binding&);
    bool synchronize(Binding&, std::string& error);
    bool consume_source_marker(ActorId, std::uint64_t, const SourceCombatMarker&,
                               std::uint64_t, std::vector<DamageEvent>&, std::string&);
    CombatSystem& combat_;
    CombatRuntimeHitEffectObserver hit_effect_observer_;
    std::string hit_effect_failure_;
    bool observe_hit_effect(const DamageEvent&,std::uint64_t,CombatRuntimeHitEffectStage,std::string&);
    std::uint64_t hit_effect_occurrence_=0;
    std::map<ActorId, Binding> bindings_;
};

} // namespace dh::foundation
