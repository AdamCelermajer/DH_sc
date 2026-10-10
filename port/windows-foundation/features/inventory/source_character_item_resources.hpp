#pragma once
#include "source_character_inventory_binding.hpp"
#include <map>
#include <mutex>

namespace dh::foundation::inventory {

// Pins the exact pre-created campaign resource object and every existing host
// service context that Gear's callbacks borrow. It owns no source tables.
struct SourceCharacterGearResourcePinsV1 {
    std::shared_ptr<const dh2::character::SourceItemResourcesV88> items;
    std::vector<std::shared_ptr<void>> host_services;
};

// Typed association for the one SourceItemResourcesV88 already loaded by the
// same campaign/cache owner. Publish it from equipment_inputs before
// configure_equipment/Load4; Gear and the inventory resolver then borrow this
// same object. The association is external to the V60 record to avoid a core
// layout/owner change and never decodes SourceItemResources itself.
class SourceCharacterItemResourcesV1 final
    : public std::enable_shared_from_this<SourceCharacterItemResourcesV1> {
    struct Entry {
        std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;
        std::shared_ptr<const dh2::character::SourceItemResourcesV88> items;
    };
    mutable std::mutex mutex_;
    std::map<const dh2::world::CanonicalCharacterCandidateRecordV60*,Entry> by_record_;
public:
    bool publish(
        const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,
        std::shared_ptr<const dh2::character::SourceItemResourcesV88>,
        std::string& error);
    bool borrow(
        const dh2::world::CanonicalCharacterCandidateRecordV60&,
        std::shared_ptr<const dh2::character::SourceItemResourcesV88>&,
        std::string& error) const;
    SourceItemResourcesBorrow borrower();
    bool configure_gear_inputs(
        const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,
        const std::shared_ptr<const dh2::character::SourceItemResourcesV88>&,
        dh2::player::PlayerEquipmentRenderInputsV1&,
        const dh2::ui::HudTextEnvironmentV1& actual_text_environment,
        std::vector<std::shared_ptr<void>> host_services,
        std::string& error);
    bool release_after_character_unpublication(
        const dh2::world::CanonicalCharacterCandidateRecordV60&,
        std::string& error);
};

} // namespace dh::foundation::inventory
