#pragma once
#include "original_combat_visual_plan.hpp"
#include "actor_combat_runtime.hpp"

namespace dh::foundation {
struct OriginalSequencePolicy { std::int64_t id=-1,type=0,loop=0;std::string name; };
using OriginalSequencePolicies=std::map<std::int64_t,OriginalSequencePolicy>;
bool original_sequence_policies(const OriginalMeleeBindings&,OriginalSequencePolicies&,std::string& error);
struct OriginalAttackSelection {
    std::string state;
    std::size_t variant=0;
    // Optional explicit redirected group boundary. Empty executes the entire
    // selected root sequence; nonempty executes that subtree only. Chaining
    // subsequent player combo groups then remains the caller's decision.
    std::vector<std::size_t> group_path;
    // Container source path -> explicit child choice for source type2 sequences.
    // Empty path selects the root group's step, not every flattened combo group.
    std::map<std::vector<std::size_t>,std::size_t> choices;
    double actor_rate=1;
};
struct OriginalAttackPhase {
    OriginalCombatPhase source;
    std::int32_t start_ms=0,end_ms=0;
    double rate=1,wall_start_seconds=0,wall_duration_seconds=0;
};
struct OriginalAttackScheduledMarker {
    AnimationMarker source;
    std::size_t phase=0;
    double wall_seconds=0;
    std::int32_t virtual_ms=0;
};
struct OriginalAttackSequenceServices {
    CombatVisualBinding visual;
    // Must restart this clip at its authored start even when already selected.
    // Explicit service avoids assuming ordinary visual.select restarts a clip.
    std::function<bool(const std::string&,std::string&)> restart_clip;
    // Consume each sampled segment before restarting the next phase. The host
    // receives the authored MoveGO policy and owns displacement/collision.
    std::function<Vec3()> take_motion;
    std::function<bool(Vec3,bool,std::string&)> apply_motion;
};

// Clean chronological playback over original finite phase groups and markers.
// Source frame-boundary overshoot discard and two-slot pose blending are separate
// fidelity gaps; blendOut metadata is preserved and NEVER shortens clip duration.
class OriginalAttackSequence {
public:
    bool prepare(const OriginalCombatVisualPlan&,const OriginalSequencePolicies&,
                 const OriginalAttackSelection&,OriginalAttackSequenceServices,
                 const std::string& virtual_action_name,std::string& error);
    CombatVisualBinding binding(); // This owner must outlive the returned binding.
    bool begin(std::string& error);
    bool advance(double wall_seconds,std::string& error);
    const std::vector<OriginalAttackPhase>& phases() const {return phases_;}
    const std::vector<OriginalAttackScheduledMarker>& scheduled_markers() const {return schedule_;}
    const OriginalAttackPhase* current_phase() const {return phase_<phases_.size()?&phases_[phase_]:nullptr;}
    bool finished() const {return active_ && phase_>=phases_.size();}
    double elapsed_seconds() const {return elapsed_;}
private:
    bool enter(std::size_t,std::string&);
    std::string alias_;
    OriginalAttackSequenceServices services_;
    std::vector<OriginalAttackPhase> phases_;
    std::vector<OriginalAttackScheduledMarker> schedule_;
    AnimationMarkers virtual_markers_;
    std::int32_t virtual_end_ms_=0;
    std::size_t phase_=0;
    double elapsed_=0,phase_elapsed_=0;
    bool active_=false;
};
}
