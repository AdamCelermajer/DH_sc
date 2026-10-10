#pragma once

#include "session_openable_interaction_v1.hpp"

namespace dh::foundation::interactions {

// Adapts a container source owner to the current CombatSession's retained
// WorldObject visual. This owns no visual, callback queue, or object registry;
// the caller retains it for the bound session lifecycle and refreshes after
// detach/restore.
class SessionContainerRetainedVisualV1 final {
public:
    explicit SessionContainerRetainedVisualV1(CombatSession&);
    SessionContainerRetainedVisualV1(const SessionContainerRetainedVisualV1&)=delete;

    bool refresh_after_restore(std::string& error);
    bool bind_object(ActorId,const AssetCatalog&,std::string& error);
    // Root-callable enrollment seam: validates the current same-world object
    // against the authored definition before loading its retained BRES visual.
    bool bind_authored_object(const ActorDefinition&,const AssetCatalog&,std::string& error);
    // Rebinds after Session restore and silently selects the source state pose.
    // State 2 maps to idle, state 4 to idleactive; other byte states play none.
    bool restore_authored_object(const ActorDefinition&,const AssetCatalog&,std::string& error);
    bool restore_pose(ActorId,const std::string&,bool loop,std::string& error);
    std::shared_ptr<const void> session_lease()const noexcept{return lease_;}
    const void* session_identity()const noexcept{return session_;}
    std::uint64_t binding_lifecycle()const noexcept{return lifecycle_;}

    static bool bind_callbacks(void*,const void*,ActorId,
        SessionContainerEventCallbackV1,SessionContainerCompletionCallbackV1,
        std::string&);
    static bool play_clip(void*,const void*,ActorId,const char*,bool&,
                          std::string&);
    static bool scene_flags(void*,const void*,ActorId,std::uint32_t,
                            std::uint32_t,std::string&);

private:
    bool current(const void*,ActorId,std::string&,bool require_visual=true)const;
    CombatSession* session_{};
    std::shared_ptr<const void> lease_;
    std::uint64_t lifecycle_{};
};

} // namespace dh::foundation::interactions
