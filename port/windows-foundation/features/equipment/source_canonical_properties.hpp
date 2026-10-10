#pragma once

#include "../../actor_state.hpp"
#include "../../../game-data/properties.hpp"
#include "../actor_frame/source_character_owner_factory.hpp"
#include <functional>

namespace dh::foundation {

// Produces the existing equipment preparation callback only after the factory
// grants a completed-character loan. Constructor-prefix aliases are rejected.
// The captured aliases pin the real native candidate graph; each call
// revalidates all owner/property identities before copying its four source
// sheets into the adapter's transactional candidate.
std::function<bool(const ActorState&, dh2::data::PropertyState&, std::string&)>
source_canonical_properties_preparer(
    const features::SourceCharacterOwnerFactory& factory,
    std::uintptr_t character_identity, std::string& error);

} // namespace dh::foundation
