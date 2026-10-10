#pragma once
#include "actor_state.hpp"
#include <functional>
#include <memory>
#include <string>
namespace dh::foundation {
// Exact bounded GameObject.Update motion phases38cc90..38ccdc. Entry is AFTER
// profiling/Debug/count/pending-action prefix and BEFORE online/idle-sound tail.
struct OriginalActorMotionFrameBorrow {
    std::shared_ptr<void> actor_lease;
    ActorState* actor{};
    ActorId identity{};
    // Existing source GameObject previous-position190/Euler19c cells. They
    // differ from PFObject.cachedPosition1e0 and are never a second authority.
    std::array<float,3>* previous_position190{};
    std::array<float,3>* previous_rotation19c{};
};
enum class OriginalActorMotionFramePhase {
    not_started, snapshot, path, rotation, subobjects, target_position, complete_prefix
};
struct OriginalActorMotionFrameServices {
    std::shared_ptr<void> runtime_lease;
    using Callback=std::function<bool(const OriginalActorMotionFrameBorrow&,std::string&)>;
    Callback update_path,update_rotation,update_subobjects,update_target_position;
};
struct OriginalActorMotionFrameResult {
    ActorId actor{};
    OriginalActorMotionFramePhase phase=OriginalActorMotionFramePhase::not_started;
    // Bits0..4: snapshots/path/rotation/subobjects/target completed.
    std::uint32_t completed_mask=0;
};
// Source phases run for every admitted actor; policy belongs inside the actual
// services, never an adapter-owned Move/Idle/Attack filter. Reached missing or
// failing callback stops there, preserving snapshots/prior callback effects.
bool update_original_actor_motion_frame(const OriginalActorMotionFrameBorrow&,
    const OriginalActorMotionFrameServices&,OriginalActorMotionFrameResult&,std::string& error);
}
