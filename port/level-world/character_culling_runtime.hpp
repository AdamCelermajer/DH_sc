#pragma once

#include "character_update_eligibility.hpp"
#include "object_update_culling.hpp"

namespace dh2::character_culling_runtime {

struct Bindings {
    character_update_eligibility::Character* character;
    object_update_culling::Object* object;
    const object_update_culling::Aabb* aabb;
    const object_update_culling::Globals* globals;
    // Both source callers use these same online and remote virtual providers.
    const character_update_eligibility::Services* character_services;
    // Only reached GetCurrentLevel/getViewFrustum calls use this table.
    const object_update_culling::Services* camera_services;
};

enum class Status : int {
    complete, invalid_argument, character_failed, culling_failed
};
struct Result {
    character_update_eligibility::Status character_status;
    object_update_culling::Status culling_status;
    unsigned culling_calls;
    character_update_eligibility::Result character;
    object_update_culling::Result culling;
};

// Compose the recovered CanUpdate and TestCullingBeforeUpdate bodies. Source
// projections are borrowed, live, and retained by the caller through return.
// The culling Object must describe the same Character; no phase is reseeded.
// Camera/AABB/object facts are required only if their source branch is taken.
// Provider tables are captured on entry. Reached providers may mutate fields;
// their completed effects remain after later failure. No native wiring or
// additional original function implementation is claimed by this composition.
// Logical projections/control/tables must not alias the full Result or each
// other's incompatible storage. Callback-returned camera facts and both captured
// and current Visual roots are checked before nested kernels reach them. Nested
// calls cannot reuse an active Result or place their projections inside one.
// Independent nested calls require independent result storage; source
// same-Character reentry is rejected.
Status evaluate(const Bindings*, Result*);

} // namespace dh2::character_culling_runtime
