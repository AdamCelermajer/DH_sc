#pragma once

#include "../../../game-data/player_save_write_owner_v1.hpp"
#include <memory>
#include <string>

namespace dh2::world { struct CanonicalCharacterCandidateRecordV60; }

namespace dh::foundation::campaign_save {

// Whole-profile SG_Save delivery on one already initialized canonical player.
// This adapter owns no Save, profile, Gear, skill, faery, quest, or wire state;
// it retains the existing candidate and delegates to its bound native writer.
class SourceSaveWriterBindingV1 {
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> candidate_;
    std::unique_ptr<dh2::data::PlayerSaveWriteOwnerV1> writer_;

    explicit SourceSaveWriterBindingV1(
        std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> candidate);
    bool coherent(std::string& error) const;

public:
    SourceSaveWriterBindingV1(const SourceSaveWriterBindingV1&) = delete;
    SourceSaveWriterBindingV1& operator=(const SourceSaveWriterBindingV1&) = delete;

    // Requires the original InitPost/profile completion path and all SAME
    // native owners. A constructed prefix never receives a save service.
    static bool bind(
        std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> candidate,
        std::shared_ptr<SourceSaveWriterBindingV1>& out,
        std::string& error);

    // Runs the existing PlayerSaveWriteOwnerV1::save() choreography, including
    // the same profile's save_all and real remaining online/sync providers.
    bool save(std::string& error);
    const dh2::data::PlayerSavegameV1* source_save() const noexcept;
};

} // namespace dh::foundation::campaign_save
