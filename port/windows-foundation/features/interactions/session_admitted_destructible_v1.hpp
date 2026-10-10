#pragma once

#include "session_source_object_admission_v1.hpp"
#include "session_destructible_interaction_v1.hpp"

namespace dh::foundation::interactions {

struct SessionAdmittedDestructibleProvidersV1 {
    using VisualRowValidator = bool(*)(void*,const ActorDefinition&,
        const dh2::world::DestructibleContainerRowV16&,const WorldObject&,
        std::string&);
    // Retains the exact source definition set and all raw callback contexts
    // supplied in SessionDestructibleInteractionServicesV1.
    std::shared_ptr<const void> owner;
    void* context{};
    VisualRowValidator validate_visual_row{};
};

// One already-admitted DestructibleContainer over the current Session's exact
// WorldObject, GameSave component, retained visual, and existing drop/store.
// This adapter never creates admission evidence or source policy providers.
class SessionAdmittedDestructibleV1 final {
public:
    static bool bind(CombatSession&,const ActorDefinition&,
        const SessionSourceObjectAdmissionReceiptV1&,const AssetCatalog&,
        std::shared_ptr<SessionContainerRetainedVisualV1>,
        SessionDestructibleInteractionServicesV1,
        SessionAdmittedDestructibleProvidersV1,
        std::shared_ptr<SessionAdmittedDestructibleV1>&,std::string& error);

    bool interact(ActorId opener,std::string& error);
    bool animation_event(const RetainedAnimationEvent&,std::string& error);
    bool animation_finished(std::uint64_t generation,bool active,std::string& error);
    ActorId source_id()const noexcept{return source_id_;}

private:
    SessionAdmittedDestructibleV1(CombatSession&,ActorId,
        std::shared_ptr<const void>,std::shared_ptr<const void>,
        std::shared_ptr<SessionContainerRetainedVisualV1>,
        std::shared_ptr<const void>,std::shared_ptr<SessionDestructibleInteractionV1>);
    bool current(std::string&)const;

    CombatSession* session_{};
    ActorId source_id_{invalid_actor_id};
    std::shared_ptr<const void> session_lease_,session_lifetime_lease_;
    std::shared_ptr<SessionContainerRetainedVisualV1> visual_;
    std::shared_ptr<const void> providers_owner_;
    std::shared_ptr<SessionDestructibleInteractionV1> interaction_;
};

} // namespace dh::foundation::interactions
