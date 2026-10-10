#include "source_character_owner_factory_visual_binding.hpp"

#include "../../../level-world/canonical_character_pf_v62.hpp"
#include <stdexcept>

namespace dh::foundation::features {

SourceCharacterOwnerFactoryVisualWorldV1::SourceCharacterOwnerFactoryVisualWorldV1(
    std::shared_ptr<SourceNavigationWorldStorage> navigation,
    std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> roots,
    std::shared_ptr<dh2::character::CharacterCandidateCacheV62> cache,
    std::shared_ptr<dh2::world::CanonicalObjectManagerV1> canonical_manager,
    std::shared_ptr<const dh2::character::CharacterGameDesign::Borrow> target_design)
    : navigation_(std::move(navigation)), roots_(std::move(roots)),
      cache_(std::move(cache)), canonical_manager_(std::move(canonical_manager)),
      target_design_(std::move(target_design)) {
    if (target_design_) {
        if (!*target_design_ || !target_design_->ai())
            throw std::invalid_argument("Required original GameDesign AI tables for native world target directory");
        target_directory_=std::make_unique<dh2::character::skills::CharacterWorldRuntimeV1>(*target_design_->ai());
    }
}

std::shared_ptr<void> SourceCharacterOwnerFactoryVisualWorldV1::lifetime() {
    return shared_from_this();
}

std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1>
SourceCharacterOwnerFactoryVisualWorldV1::target_directory() {
    if (!target_directory_) return {};
    return {shared_from_this(),target_directory_.get()};
}

bool SourceCharacterOwnerFactoryVisualWorldV1::ready(std::string& error) const {
    if (!navigation_ || !navigation_->floors || !navigation_->floors->sewn ||
        !navigation_->registry.entries || !navigation_->registry.floors ||
        !navigation_->registry.capacity || !navigation_->registry.floor_capacity) {
        error = "Required same sewn SourceNavigationWorldStorage and obstacle backing";
        return false;
    }
    if (!roots_ || !cache_ || !cache_->ready() || canonical_manager_.expired()) {
        error = "Required same SceneManager, loaded Character cache, and canonical ObjectManager";
        return false;
    }
    if (!cache_->files().read) {
        error = "Required actual CharacterCandidateCacheV62 asset reader";
        return false;
    }
    error.clear();
    return true;
}

bool SourceCharacterOwnerFactoryVisualWorldV1::bind(
    dh2::world::CanonicalCharacterCandidateRecordV60& record,
    dh2::character::CharacterFamilyVisualServicesV6& output,
    std::string& error) const {
    if (!ready(error)) return false;
    const auto canonical_manager = canonical_manager_.lock();
    if (!canonical_manager) {
        error = "Expired same Application-owned canonical ObjectManager";
        return false;
    }
    if (record.services.world.get() != this ||
        record.services.canonical_objects.get() != canonical_manager.get()) {
        error = "Character visual context must be the same candidate World/ObjectManager";
        return false;
    }
    if (!record.services.models || record.services.models != &cache_->models() ||
        !record.services.animation_tables ||
        record.services.animation_tables != &cache_->animation_tables() ||
        !record.services.loot_tables || record.services.loot_tables != &cache_->loot()) {
        error = "Character visual context requires the candidate's same loaded resource cache";
        return false;
    }
    if (!record.actor || !record.actor->object || !record.actor->object->identity ||
        record.actor->object->properties != record.properties) {
        error = "Required same canonical Character constructor/property owner";
        return false;
    }
    if ((output.world && output.world.get() != record.services.world.get()) ||
        (output.scene_manager && output.scene_manager.get() != roots_.get()) ||
        (output.visual.owner && output.visual.owner.get() != record.services.world.get())) {
        error = "Character visual provider received a foreign World/SceneManager owner";
        return false;
    }

    // This provider is called at the real V60 constructor boundary, before
    // InitPost writes model290. Do not inspect or prefill either source string.
    output.world = record.services.world;
    output.scene_manager = roots_;
    output.visual.owner = record.services.world;
    const auto cache = cache_;
    output.visual.read_asset = [cache](const std::string& path,
                                       std::vector<std::uint8_t>& bytes,
                                       bool& found, std::string& e) {
        const auto& files = cache->files();
        if (!files.read) {
            e = "Required same CharacterCandidateCacheV62 file reader";
            return false;
        }
        return files.read(path, found, bytes, e);
    };

    const auto nav = navigation_;
    const auto roots = roots_;
    const auto weak_manager = canonical_manager_;
    const auto weak_record =
        std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(
            record.shared_from_this());
    const auto weak_world = weak_from_this();
    output.update_pf = [weak_record, weak_world, weak_manager, nav, roots](std::string& e) {
        const auto same = weak_record.lock();
        const auto context = weak_world.lock();
        const auto manager = weak_manager.lock();
        if (!same || !context || !same->actor || !same->actor->object ||
            !manager ||
            same->services.world.get() != context.get() ||
            same->services.canonical_objects.get() != manager.get() ||
            context->navigation_.get() != nav.get() ||
            context->roots_.get() != roots.get() || !nav->floors ||
            !nav->floors->sewn) {
            e = "Expired or foreign same Character/World/PF owner";
            return false;
        }
        return dh2::world::canonical_character_update_pf_v62(
            *same, nav->floors->collision_world, nav->registry, e);
    };

    // Character::IsAnimated (0x3a2ee0) is the verified source virtual that
    // returns literal true. Preserve any root-supplied equivalent producer.
    if (!output.visual.parent_is_animated) {
        output.visual.parent_is_animated = [](bool& value, std::string& e) {
            value = true;
            e.clear();
            return true;
        };
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::features
