#pragma once
#include "original_attack_sequence.hpp"
#include "retained_animation_owner.hpp"
namespace dh::foundation {
struct RetainedSequenceBoundary {
    bool beginning=false;
    std::size_t phase=0;
    std::uint32_t depth=0,step=0,count=0;
    std::int64_t sequenceId=-1,type=0,loops=0;
    std::vector<std::size_t> containerPath,leafPath;
};
struct RetainedSequenceCursorDecision {
    std::optional<std::uint32_t> setStep;
    bool skipNext=false;
    bool stop=false;
};
struct RetainedSequenceServices {
    // Synchronous source frame delivery BEFORE completion/phase selection.
    // Includes outgoing slot events and authored motion; caller owns event
    // meaning, actor identity, collision and MoveGO application.
    std::function<bool(std::size_t,const RetainedAnimationFrame&,std::string&)> frame;
    // Called once after final genuine timeline completion. State is closed
    // before invocation, allowing the caller's explicit next-state policy.
    std::function<bool(const dh2::timeline::Completion&,std::string&)> closed;
    // Same source hierarchy owner, called before selecting a newly entered
    // leaf and after outgoing scene events/motion before cursor completion.
    // Source SetStep/SkipNext apply BEFORE original sequential increment.
    std::function<bool(const RetainedSequenceBoundary&,RetainedSequenceCursorDecision&,std::string&)> boundary;
    // Optional final completion of a source seed_sequence (not raw seed or
    // a repeat boundary). The timeline latch is consumed before invocation;
    // this does not close or finish the previously prepared finite action.
    std::function<bool(const dh2::timeline::Completion&,std::string&)> seeded_closed;
};
// Finite selected source type1/type0/explicit type2 hierarchy. Owns retained
// two-slot animation, not a virtual aggregate duration or wall-time hit schedule.
// Visual must already contain the plan's selected clip resources and outlive us.
class RetainedSequencePlayback {
public:
    RetainedSequencePlayback(CharacterVisual& visual,RetainedAnimationOwner::RootSampler root)
        : visual_(visual),root_(std::move(root)) {}
    bool prepare(const AssetCatalog&,const OriginalCombatVisualPlan&,
                 const OriginalSequencePolicies&,const OriginalAttackSelection&,
                 RetainedSequenceServices,const std::string& action_name,std::string& error);
    bool prepare_preserving(const AssetCatalog&,const OriginalCombatVisualPlan&,
                 const OriginalSequencePolicies&,const OriginalAttackSelection&,
                 RetainedSequenceServices,const std::string& action_name,std::string& error);
    // Prepare only the exact source event/root owner needed by a seeded
    // authored state. This does not compile an attack sequence or invent a
    // finite action timeline; the caller must seed a source phase separately.
    bool prepare_seeded(const AssetCatalog&,const OriginalCombatVisualPlan&,
                 const OriginalCombatPhase&,RetainedSequenceServices,std::string& error);
    bool set_source_rate(float rate,std::string& error){return animation_?animation_->set_source_rate(rate,error):(error="Retained animation is not prepared",false);}
    bool begin(std::string& error);
    bool seed(const AssetCatalog&,const std::string& clip,const std::string& event_asset,
              bool loop,float rate,std::int32_t blend_out_ms,bool move_go,
              RetainedAnimationFrame& output,std::string& error);
    // Source single-leaf sequence Loop policy, distinct from raw timeline loop.
    // -1 repeats forever; positive values count additional selections; zero
    // completes once. Every actual completion selects NewAnim at this frame,
    // with genuine same-physical-resource extra and original root reset.
    bool seed_sequence(const AssetCatalog&,const std::string& clip,const std::string& event_asset,
                       float rate,std::int32_t blend_out_ms,bool move_go,
                       std::int32_t repeats,RetainedAnimationFrame& output,std::string& error);
    // Inactive explicit Idle/Injured/Died policies retain the same two slots.
    // frame callback phase==SIZE_MAX; finite seed completion is current_ended()
    // on animation()->pose(), not the prior action's finished() flag.
    bool advance_seeded(double wall_seconds,std::string& error);
    bool seeded()const noexcept{return seeded_;}
    // Aggregate range/markers serve existing validation ONLY. Live selection and
    // updates route genuine retained playback. Other clips require explicit seed.
    CombatVisualBinding binding();
    bool advance(double wall_seconds,std::string& error);
    void cancel() noexcept;
    bool active()const noexcept{return active_;}
    bool finished()const noexcept{return finished_;}
    std::size_t phase_index()const noexcept{return phase_;}
    const std::vector<OriginalAttackPhase>& phases()const{return phases_;}
    const RetainedAnimationOwner* animation()const noexcept{return animation_.get();}
    std::uint64_t completed_phases()const noexcept{return completed_;}
private:
    bool prepare_internal(bool,const AssetCatalog&,const OriginalCombatVisualPlan&,
                 const OriginalSequencePolicies&,const OriginalAttackSelection&,
                 RetainedSequenceServices,const std::string&,std::string&);
    bool enter(std::size_t,std::int32_t extra,std::string&);
    bool deliver(std::size_t,const RetainedAnimationFrame&,std::string&);
    bool begin_hierarchy(std::size_t,std::size_t,std::string&);
    bool complete_hierarchy(std::optional<std::size_t>&,std::size_t&,std::string&);
    struct Container {std::int64_t id=-1,type=0,loops=0;std::uint32_t count=0;};
    std::map<std::vector<std::size_t>,Container> containers_;
    std::vector<std::size_t> scope_path_;
    CharacterVisual& visual_;
    RetainedAnimationOwner::RootSampler root_;
    std::unique_ptr<RetainedAnimationOwner> animation_;
    std::unique_ptr<OriginalAttackSequence> validation_;
    std::map<std::string,std::string> event_paths_;
    std::string action_;
    std::string seed_clip_;
    float seed_rate_=1;
    std::int32_t seed_blend_{},seed_repeats_{};
    bool seed_move_{},seed_sequence_{},seed_completion_delivered_{};
    std::vector<OriginalAttackPhase> phases_;
    RetainedSequenceServices services_;
    std::size_t phase_{};
    std::uint64_t generation_{},completed_{};
    bool active_{},finished_{},seeded_{};
};
}
