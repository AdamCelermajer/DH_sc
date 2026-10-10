#include "source_canonical_properties.hpp"
#include <utility>

namespace dh::foundation {

std::function<bool(const ActorState&, dh2::data::PropertyState&, std::string&)>
source_canonical_properties_preparer(const features::SourceCharacterOwnerFactory& factory,
                                     std::uintptr_t character_identity, std::string& error) {
    features::SourceCharacterOwnerAliases aliases;
    if (!factory.borrow_completed_character(character_identity, aliases, error)) return {};
    return [aliases = std::move(aliases)](const ActorState&, dh2::data::PropertyState& candidate,
                                           std::string& error) mutable {
        if (!aliases.validate(error)) return false;
        if (!aliases.properties || !aliases.property_view) {
            error = "Required validated canonical source PropertyState loan";
            return false;
        }
        // Copy the source cells as a whole: raw base, saved contributions,
        // current gear, and the resolved 224-property view. Adapter mutations
        // remain staged until the root's canonical owner publication runs.
        candidate = *aliases.properties;
        error.clear();
        return true;
    };
}

} // namespace dh::foundation
