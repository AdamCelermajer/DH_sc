#pragma once
#include "actor_blended_playback.hpp"

namespace dh2::actor_scene_retention_v1 {
// Read-only CPU pose copy for a development world/GL recreation boundary.
// The existing playback and immutable bank remain their sole owners. This
// snapshot owns neither GPU handles nor Scene views, resources, clocks or events.
class Snapshot {
    struct NodePose {
        std::string id;std::int32_t parent=-1;
        float translation[3]{},quaternion[4]{},scale[3]{};
    };
    const actor::BlendedPlayback* playback_=nullptr;
    const actor::ClipBank* bank_=nullptr;
    std::vector<NodePose> nodes_;
    visual::Root root_{};
    std::int32_t animated_=-1;
public:
    bool capture(const actor::BlendedPlayback&,const actor::ClipBank&,
                 const visual::SceneBinding&,const scene::Scene&,std::string& error);
    bool restore(const actor::BlendedPlayback&,const actor::ClipBank&,
                 visual::SceneBinding&,scene::Scene&,std::string& error)const;
    void clear()noexcept;
    bool empty()const noexcept{return !playback_;}
};
// One owning thread captures after the complete current source frame and keeps
// playback/bank alive, at the same addresses and unchanged, until restoration.
// The bank is immutable; no erase/replacement/recompile, active observer or
// ongoing scene/frame update during either call. A snapshot can outlive the old
// Scene and binding. Rebind only a graph with matching IDs, parent order and
// original animation root; fresh materials/instances stay owned by that graph.
// Capture failures preserve the prior snapshot. Restore stages a fresh binding,
// exact actual TRS/Root and existing update_world, committing only on success;
// failures preserve both scene and binding. No sampler, fade update, event,
// timeline jump/replay, RNG, root integration or synthetic completion is used.
// Retains untouched/disabled channels and time-only culled pose accurately.
// Caller clears the snapshot after successful restore or terminal teardown.
}
