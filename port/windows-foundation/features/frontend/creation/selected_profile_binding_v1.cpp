#include "selected_profile_binding_v1.hpp"
#include "../../../../level-world/campaign_save_filename_v45.hpp"

#include <algorithm>

namespace dh::foundation::frontend::creation {
namespace {
template <class A, class B>
bool same_owner(const std::shared_ptr<A>& a, const std::shared_ptr<B>& b) noexcept {
    // Jobs expose their storage as shared_ptr<void>, while the application
    // FileManager keeps the concrete transport type. Compare both the shared
    // control block and the underlying address without requiring matching
    // shared_ptr element types.
    return a && b && static_cast<const void*>(a.get()) == static_cast<const void*>(b.get()) &&
           !a.owner_before(b) && !b.owner_before(a);
}
}

bool SelectedProfileBindingV1::prepare(
    const std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61>& files,
    std::int32_t selected_slot,
    const dh2::character::CharacterProfileSlotStoreReceiptV59& actual_set_slot,
    dh2::character::CharacterProfileBootstrapInputsV59 source,
    std::string& error) {
    if (attempted_) {
        error = "Selected Character profile binding cannot replay its retained source prefix";
        return false;
    }
    attempted_ = true;
    files_ = files;

    if (!files_ || selected_slot < 0 || selected_slot >= 4 || !source.save || !source.load ||
        &source.load->save() != source.save.get() || !source.character ||
        source.save->character() != source.character || !source.actual_source_cells_lease ||
        !source.source_save14e8 || *source.source_save14e8 != reinterpret_cast<std::uintptr_t>(source.save.get()) ||
        !source.source_direct_save_slot_v122 || source.source_player_info_slot664 ||
        source.source_direct_save_slot_v122 != source.save->source_slot_field_v122() ||
        !source.source_set_slot || source.profile ||
        (source.selected_slot != -1 && source.selected_slot != selected_slot) ||
        ((!source.selected_file_bytes.data) != (source.selected_file_bytes.size == 0)) ||
        !actual_set_slot.save || actual_set_slot.save != source.save.get() ||
        actual_set_slot.value != selected_slot || source.save->slot() != selected_slot ||
        *source.source_direct_save_slot_v122 != selected_slot) {
        error = "Required actual selected slot and same-Character Save/SetSlot receipt";
        return false;
    }
    const auto jobs = files_->jobs();
    const auto private_files = files_->files();
    if (!jobs || !private_files || !same_owner(jobs->file_storage_v59(), private_files)) {
        error = "Required SAME Application FileManager and SavegameJobs owner";
        return false;
    }

    // Match source SG_GetFilename for an indexed player profile. read_save is
    // the retained Application wrapper around openSavefile's matching-job
    // flush and returns the exact file bytes that CampaignProfile C1 must see.
    const auto filename = dh2::level::campaign_save_filename_v45(
        static_cast<std::uint32_t>(selected_slot), false, false);
    auto receipt = std::make_shared<SelectedProfileFileReceiptV1>();
    receipt->files = files_;
    receipt->filename = filename;
    receipt->slot = selected_slot;
    bool found{};
    if (!files_->read_save(filename, found, receipt->bytes, error)) return false;
    if (!found) {
        error = "Selected Character profile file is absent; source CreateSaveSlot must publish it first";
        return false;
    }
    if (source.selected_file_bytes.size &&
        (source.selected_file_bytes.size != receipt->bytes.size() ||
         !std::equal(receipt->bytes.begin(), receipt->bytes.end(), source.selected_file_bytes.data))) {
        error = "Supplied selected profile bytes differ from the same Application read_save receipt";
        return false;
    }
    file_receipt_ = receipt; // Keep the actual read prefix if profile C1 fails.

    profile_ = std::make_shared<dh2::level::CampaignSaveProfileV45>(
        filename, private_files->services(), jobs);
    if (!profile_->construct(error)) return false;
    const auto cache = profile_->cache();
    if (!profile_->ready() || !cache || cache.bytes().size() != receipt->bytes.size() ||
        !std::equal(cache.bytes().begin(), cache.bytes().end(), receipt->bytes.begin())) {
        error = "CampaignProfile C1 cache differs from the selected Application file receipt";
        return false;
    }

    source.profile = profile_;
    source.selected_file_lease = file_receipt_;
    source.selected_file_bytes = {receipt->bytes.data(), receipt->bytes.size()};
    source.selected_slot = selected_slot;
    source.prior_slot_store = actual_set_slot;
    bootstrap_inputs_ = std::move(source);
    ready_ = true;
    error.clear();
    return true;
}

} // namespace dh::foundation::frontend::creation
