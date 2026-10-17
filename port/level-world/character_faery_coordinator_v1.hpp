#pragma once

#include "character_runtime_factory_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_faery_coordinator_v1 {

namespace factory = character_runtime_factory_v1;
using Bindings = character::CoordinatorBindings;

enum class Status : std::uint8_t {
    complete,
    invalid_record,
    owner_mismatch,
    service_unavailable,
    already_bound,
    bind_failed,
};

// Bind the existing factory Record's one Coordinator to caller-owned live
// Character facts, state services, and retained CharAI timer routing. The
// callback graph remains borrowed from the per-Faery owner; this helper does
// not allocate a second FSM, timer store, or Character.
Status bind(factory::Record&, const Bindings&, std::string& error);

} // namespace dh2::character_faery_coordinator_v1
