#pragma once

#include "../../source_navigation_world_storage.hpp"
#include "../../../level-world/canonical_character_candidate_v60.hpp"
#include "../../../level-world/character_candidate_cache_v62.hpp"
#include "../../../level-world/gameobject_scene_root_registry_v1.hpp"
#include "../../../level-world/canonical_object_manager_v1.hpp"
#include "../../../level-world/character_world_runtime_v1.hpp"

namespace dh::foundation::features {

// Typed in-house source-world lifetime for the native Character visual
// provider. It pins the same navigation, SceneManager, resource cache, and
// canonical ObjectManager that the candidate already uses; it is not a
// replacement source Level or a second scene/PF authority.
class SourceCharacterOwnerFactoryVisualWorldV1 final
    : public std::enable_shared_from_this<SourceCharacterOwnerFactoryVisualWorldV1> {
public:
    SourceCharacterOwnerFactoryVisualWorldV1(
        std::shared_ptr<SourceNavigationWorldStorage> navigation,
        std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> roots,
        std::shared_ptr<dh2::character::CharacterCandidateCacheV62> cache,
        std::shared_ptr<dh2::world::CanonicalObjectManagerV1> canonical_manager,
        std::shared_ptr<const dh2::character::CharacterGameDesign::Borrow> target_design = {});

    // Assign this result to CanonicalCharacterCandidateServicesV60::world.
    // bind() requires the same owner and pointer on the candidate record.
    std::shared_ptr<void> lifetime();

    // The sole target directory for this native world. Its aliasing lease has
    // the same control block as lifetime(), and pins the original AI tables.
    // An older visual-only caller without target_design receives no directory.
    std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> target_directory();

    // Add the real family visual/PF services without replacing the root's
    // animation, registration, selection, event, or FX providers.
    bool bind(dh2::world::CanonicalCharacterCandidateRecordV60&,
              dh2::character::CharacterFamilyVisualServicesV6&,
              std::string& error) const;

    bool ready(std::string& error) const;

private:
    std::shared_ptr<SourceNavigationWorldStorage> navigation_;
    std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> roots_;
    std::shared_ptr<dh2::character::CharacterCandidateCacheV62> cache_;
    // Application owns the process ObjectManager. Keeping it weak avoids the
    // manager -> published candidate -> services.world -> context cycle.
    std::weak_ptr<dh2::world::CanonicalObjectManagerV1> canonical_manager_;
    std::shared_ptr<const dh2::character::CharacterGameDesign::Borrow> target_design_;
    std::unique_ptr<dh2::character::skills::CharacterWorldRuntimeV1> target_directory_;
};

} // namespace dh::foundation::features
