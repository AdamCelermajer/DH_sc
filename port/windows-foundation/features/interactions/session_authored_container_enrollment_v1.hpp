#pragma once

#include "session_container_admitted_openable_v1.hpp"
#include "session_admitted_destructible_v1.hpp"

namespace dh::foundation::interactions {

// Result of admitting one decoded current-Level declaration. A normal source
// spawn rejection is a successful call with admission.admitted=false and no
// published object, retained visual, or interaction owner.
struct SessionAuthoredContainerEnrollmentResultV1 {
    SessionSourceObjectAdmissionReceiptV1 admission;
    std::shared_ptr<SessionContainerModernOpenableV1> openable;
    std::shared_ptr<SessionAdmittedDestructibleV1> destructible;
};

// Runs the exact source admission gate for one caller-built source candidate,
// then binds the matching source interaction owner. The caller supplies the
// decoded ActorDefinition, exact current-Level WorldObject (including source
// model, transform, and OBJS component), source admission leaves, and the
// original row/policy providers. This owner creates no Level, object graph,
// model resolver, online/quest/audio/physics/script provider, RNG, or store.
//
// Openables use the existing all-or-nothing row-backed composition. For
// destructibles, the exact admission receipt is forwarded to the existing
// admitted binder. Any post-admission binder failure removes only the same
// newly published WorldObject and retained visual while the original Session
// binding remains current.
bool enroll_session_authored_container_v1(
    CombatSession&, SessionSourceObjectAdmissionRequestV1,
    const AssetCatalog&, std::shared_ptr<SessionContainerRetainedVisualV1>,
    std::shared_ptr<SessionContainerModernDropV1>,
    SessionContainerModernOpenablePolicyV1,
    SessionDestructibleInteractionServicesV1,
    SessionAdmittedDestructibleProvidersV1,
    SessionAuthoredContainerEnrollmentResultV1&, std::string& error);

} // namespace dh::foundation::interactions
