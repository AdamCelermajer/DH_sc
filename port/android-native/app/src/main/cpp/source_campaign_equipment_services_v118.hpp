#pragma once
#include <fresh_inventory_owned_v4.hpp>
#include <memory>
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace dh2::world {struct CanonicalCharacterCandidateRecordV60;}
namespace dh2::character {class SourceItemResourcesV88;}
namespace model_renderer {
// Retained continuation provider for the SAME constructor37c/Gear inventory.
// Actor/Application are weak; immutable item/text resources remain pinned.
bool make_source_campaign_equipment_services_v118(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,
 const std::shared_ptr<dh2::character::SourceItemResourcesV88>&,
 dh2::data::OwnedInventoryServicesV4&,std::shared_ptr<void>&,std::string&);
//Called before the existing Gear/inventory release frees its actual Items.
//Drops only their auxiliary power-description aliases, never Items/Gear.
bool release_source_campaign_equipment_presentation_v118(const std::shared_ptr<void>&,
 dh2::world::CanonicalCharacterCandidateRecordV60&,std::string&);
}
