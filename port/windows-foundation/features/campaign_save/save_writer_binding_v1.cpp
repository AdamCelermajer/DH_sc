#include "save_writer_binding_v1.hpp"
#include "../../../level-world/canonical_character_candidate_v60.hpp"

namespace dh::foundation::campaign_save {

SourceSaveWriterBindingV1::SourceSaveWriterBindingV1(
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> candidate)
    : candidate_(std::move(candidate)) {}

bool SourceSaveWriterBindingV1::coherent(std::string& error) const {
    error.clear();
    const auto candidate = candidate_;
    if (!candidate || !candidate->init_complete || candidate->failed ||
        !candidate->actor || !candidate->actor->object || !candidate->save ||
        !candidate->load || !candidate->profile_bootstrap ||
        !candidate->equipment || !candidate->equipment->ready() ||
        !candidate->player_script_owner_v62 || !candidate->player_script_owner_v62->ready() ||
        !candidate->faery_association_v68 || !candidate->quest_sync_owner) {
        error = "Required completed SAME player InitPost/Save/Gear/PlayerSkills/Faery/Quest owners";
        return false;
    }

    const auto& bootstrap = candidate->profile_bootstrap;
    const auto campaign_writer = bootstrap->campaign_writer();
    const auto quests = bootstrap->quest_owner_v70();
    const auto* inventory = candidate->equipment->inventory();
    const auto identity = candidate->actor->object->identity;
    if (!bootstrap->finished() || bootstrap->save() != candidate->save ||
        bootstrap->load_owner() != candidate->load ||
        !campaign_writer || !campaign_writer->ready() ||
        !candidate->save->character() || candidate->save->character() != identity ||
        &candidate->load->save() != candidate->save.get() ||
        !candidate->load->profile().identity ||
        candidate->load->profile().identity != reinterpret_cast<std::uintptr_t>(bootstrap->profile().get()) ||
        candidate->load->profile().owner.get() != bootstrap->profile().get() ||
        candidate->player_script_owner_v62->native_saved_owner() != candidate->save ||
        !inventory || inventory->character() != identity ||
        candidate->faery_association_v68->character != identity ||
        !quests || quests->save() != candidate->save ||
        !candidate->save->skills_initialized() ||
        !candidate->save->faeries_initialized()[0] ||
        !candidate->save->faeries_initialized()[1] ||
        !candidate->save->faeries_initialized()[2]) {
        error = "Required identical initialized Character/Save/Profile/Gear/Skills/Faery/Quest graph";
        return false;
    }
    if (!writer_ || &writer_->authority() != candidate->load.get() ||
        &writer_->authority().save() != candidate->save.get()) {
        error = "Required SAME existing PlayerSaveWriteOwnerV1 authority";
        return false;
    }
    return true;
}

bool SourceSaveWriterBindingV1::bind(
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> candidate,
    std::shared_ptr<SourceSaveWriterBindingV1>& out,
    std::string& error) {
    out.reset();
    if (!candidate) {
        error = "Required SAME canonical player candidate for whole-profile save";
        return false;
    }
    auto binding = std::shared_ptr<SourceSaveWriterBindingV1>(
        new SourceSaveWriterBindingV1(std::move(candidate)));
    if (!binding->candidate_->profile_bootstrap ||
        !binding->candidate_->profile_bootstrap->finished()) {
        error = "Required completed SAME CharacterProfileBootstrap before Save binding";
        return false;
    }
    const auto campaign_writer = binding->candidate_->profile_bootstrap->campaign_writer();
    if (!campaign_writer || !campaign_writer->ready() || !binding->candidate_->load) {
        error = "Required existing whole-profile CharacterMenuCampaignSaveV50 writer";
        return false;
    }
    binding->writer_ = std::make_unique<dh2::data::PlayerSaveWriteOwnerV1>(
        binding->candidate_->load, campaign_writer->write_services());
    if (!binding->coherent(error)) return false;
    out = std::move(binding);
    error.clear();
    return true;
}

bool SourceSaveWriterBindingV1::save(std::string& error) {
    if (!coherent(error)) return false;
    // Missing original online, network, synchronization, or checkpoint
    // providers remain explicit failures from the retained campaign writer.
    return writer_->save(error);
}

const dh2::data::PlayerSavegameV1* SourceSaveWriterBindingV1::source_save() const noexcept {
    return candidate_ && candidate_->save ? candidate_->save.get() : nullptr;
}

} // namespace dh::foundation::campaign_save
