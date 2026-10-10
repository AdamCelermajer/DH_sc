#pragma once

#include "session_source_object_admission_v1.hpp"

namespace dh::foundation::interactions {

struct SessionContainerAdmittedOpenableResultV1 {
    SessionSourceObjectAdmissionReceiptV1 admission;
    std::shared_ptr<SessionContainerModernOpenableV1> openable;
};

// Candidate admission + row-backed Openable binding/initialization as one
// current-Session operation. A normal source probability rejection returns
// true with admission.admitted=false and no visual/openable binding. Missing
// required container providers fail before the source spawn RNG is consumed.
// The caller-owned ActorDefinition must remain immutable and alive for the
// returned Openable binding lifetime, matching the level definitions lease.
bool admit_and_bind_session_openable_v1(
    CombatSession&,SessionSourceObjectAdmissionRequestV1,
    const AssetCatalog&,SessionContainerRetainedVisualV1&,
    std::shared_ptr<SessionContainerModernDropV1>,
    SessionContainerModernOpenablePolicyV1,
    SessionContainerAdmittedOpenableResultV1&,std::string& error);

} // namespace dh::foundation::interactions
