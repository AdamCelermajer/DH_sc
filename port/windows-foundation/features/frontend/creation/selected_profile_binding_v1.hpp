#pragma once

#include "../../../../level-world/application_save_files_owner_v61.hpp"
#include "../../../../level-world/character_profile_bootstrap_v59.hpp"
#include "../../../../level-world/campaign_save_profile_v45.hpp"
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::frontend::creation {

// Immutable result of ApplicationSaveFilesOwnerV61::read_save for the
// selected source filename. Keeping the Application owner pins the same
// FileManager and Application-global SavegameJobs used by CampaignProfile C1.
struct SelectedProfileFileReceiptV1 {
    std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61> files;
    std::string filename;
    std::int32_t slot{-1};
    std::vector<std::uint8_t> bytes;
};

// Owns one selected-file read, one CampaignSaveProfile C1, and the existing
// CharacterProfileBootstrap V59 ordering. It adopts the caller's already
// constructed Character Save/Load and never creates another Save or file/job
// transport. A failed prefix is retained and cannot be replayed.
class SelectedProfileBindingV1 {
    bool attempted_{};
    bool ready_{};
    std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61> files_;
    std::shared_ptr<SelectedProfileFileReceiptV1> file_receipt_;
    std::shared_ptr<dh2::level::CampaignSaveProfileV45> profile_;
    dh2::character::CharacterProfileBootstrapInputsV59 bootstrap_inputs_;
public:
    bool prepare(
        const std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61>&,
        std::int32_t selected_slot,
        const dh2::character::CharacterProfileSlotStoreReceiptV59& actual_set_slot,
        dh2::character::CharacterProfileBootstrapInputsV59 source,
        std::string& error);

    bool ready() const noexcept { return ready_; }
    const auto& file_receipt() const noexcept { return file_receipt_; }
    const auto& profile() const noexcept { return profile_; }
    const auto& bootstrap_inputs() const noexcept { return bootstrap_inputs_; }
};

} // namespace dh::foundation::frontend::creation
