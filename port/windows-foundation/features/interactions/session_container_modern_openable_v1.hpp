#pragma once

#include "session_container_modern_drop_v1.hpp"
#include "session_container_retained_visual_v1.hpp"

namespace dh::foundation::interactions {

// Caller-owned receipt from the modern object-enrollment owner. This records
// that this exact object was admitted before its container behavior is bound;
// it is deliberately not inferred from mere WorldObject presence here.
struct SessionContainerPriorAdmissionV1 {
    const void* session_identity{};
    std::shared_ptr<const void> session_lease;
    std::shared_ptr<const void> admission_owner;
    ActorId object_id{invalid_actor_id};
    std::string definition_name;
    std::string data_desc;
};

// Caller-owned leaves from the same modern Session. These are deliberately
// the original Openable source services: the binder does not invent spawn
// probability, conditions, sounds, scripts, inventory, or quest policy.
struct SessionContainerModernOpenablePolicyV1 {
    // When present, create() derives canonical Openable fields from this
    // original table and requires the explicit prior-admission receipt.
    std::shared_ptr<const dh2::world::OpenableContainerTableV1> authored_table;
    SessionContainerPriorAdmissionV1 prior_admission;
    void* other_actor_context{};
    SessionContainerActorResolverV1 resolve_other_actor{};
    // Legacy full-prefix path input. In row-backed admitted mode, only the
    // authored data_desc is checked; remaining values derive from the exact
    // source row, original constructor defaults, and saved state component.
    dh2::world::OpenableContainerFieldsV1 source_fields;
    dh2::world::OpenableContainerServicesV1 source;
    dh2::world::OpenableContainerInteractionServicesV2 interaction;
};

// One authored Openable source object composed over the current same-session
// WorldObject, retained visual callbacks, scoped Session RNG, and existing
// world-item store. This is one object binding, not an object registry.
class SessionContainerModernOpenableV1 final
    : public std::enable_shared_from_this<SessionContainerModernOpenableV1> {
public:
    static bool create(CombatSession&, const ActorDefinition&,
        const AssetCatalog&,
        SessionContainerRetainedVisualV1&,
        std::shared_ptr<SessionContainerModernDropV1>,
        SessionContainerModernOpenablePolicyV1,
        std::shared_ptr<SessionContainerModernOpenableV1>&,
        std::string& error);

    bool init_post(std::string& error);
    // Initialize an already-admitted current WorldObject without rerunning
    // GameObject::InitPost/CheckSpawnProbability or emitting spawn events.
    bool initialize_admitted(std::string& error);
    bool restore_silently(std::string& error);
    bool interact(ActorId opener, std::string& error);
    bool animation_event(const RetainedAnimationEvent&, std::string& error);
    bool animation_finished(std::uint64_t generation, bool timeline_active,
                            std::string& error);
    ActorId source_id() const noexcept { return source_id_; }
    const dh2::world::OpenableContainerFieldsV1& source_fields() const noexcept {
        return fields_;
    }

private:
    SessionContainerModernOpenableV1(CombatSession&, const ActorDefinition&,
        SessionContainerRetainedVisualV1&,
        std::shared_ptr<SessionContainerModernDropV1>,
        SessionContainerModernOpenablePolicyV1);
    bool current(std::string& error) const;
    static bool resolve_actor(void*, const void*, ActorId,
        SessionContainerActorBorrowV1&, std::string&);

    CombatSession* session_{};
    const ActorDefinition* definition_{};
    SessionContainerRetainedVisualV1* visual_{};
    std::shared_ptr<SessionContainerModernDropV1> drop_;
    SessionContainerModernOpenablePolicyV1 policy_;
    std::shared_ptr<const void> lease_;
    ActorId source_id_{invalid_actor_id};
    dh2::world::OpenableContainerFieldsV1 fields_;
    std::shared_ptr<SessionOpenableInteractionV1> interaction_;
};

} // namespace dh::foundation::interactions
