#pragma once

#include "selected_profile_binding_v1.hpp"
#include "../frontend_runtime_v1.hpp"
#include "../../../../game-data/fresh_player_profile_v1.hpp"
#include "../../../../game-data/menu_profile_metadata_v1.hpp"
#include "../../../../game-data/campaign_profile_files_v1.hpp"
#include "../../../../level-world/application_player_manager_bootstrap_v59.hpp"
#include "../../../../level-world/canonical_character_candidate_v60.hpp"
#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace dh::foundation::frontend::creation {

struct SourceIndexedSlotRequestV1 {
    std::string name;
    std::string playable_class;
    // Source NativeAssignSaveSlotToPlayer player argument (normally first
    // offline local index 0); caller supplies the actual native selection.
    std::int32_t local_player_index{};
    // These are obtained from the real source timer/time providers by the
    // caller. The service never substitutes a demo seed or wall-clock value.
    std::uint32_t source_timer{};
    std::uint32_t saved_date{};
};

struct SourceIndexedSlotCompletionV1 {
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> candidate;
    std::shared_ptr<SelectedProfileBindingV1> selected_profile;
    // Pins the source publication/continuation which made this candidate live.
    std::shared_ptr<void> source_publication;
};
struct SourceNativeStartEvidenceV1;

struct SourceIndexedSlotServicesV1 {
    std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application;
    std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61> files;
    std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> players;
    const dh2::data::CharacterTable* characters{};
    dh2::player::FirstLocalControllerServicesV59 first_local;
    // Must execute the existing Character C1/Save::SetSlot/SelectedProfileBinding,
    // whole InitPost and native publication path. Returning a profile cache or
    // constructor prefix is insufficient; the receiver is checked below.
    std::function<bool(std::int32_t slot, const std::string& filename,
        const dh2::data::FreshPlayerProfileV1& metadata,
        const std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61>& files,
        SourceIndexedSlotCompletionV1&, std::string&)> complete_native_creation;
    // Must be backed by the actual source StartGame owner and return only after
    // its operation succeeds. Current front_ui_session NativeStartGame exposes
    // launch_pending/readback only, so this provider is intentionally absent.
    std::function<bool(std::int32_t slot,
        const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& candidate,
        SourceNativeStartEvidenceV1&,std::string&)> source_native_start_success;
};

struct SourceIndexedSlotResultV1 {
    std::int32_t slot{-1};
    std::string filename;
    dh2::data::FreshPlayerProfileV1 metadata;
    FrontendProfileLoanV1 selected_profile;
};

struct SourceFreshSlotReceiptV1 {
    std::int32_t slot{-1};
    std::string filename;
    dh2::data::FreshPlayerProfileV1 metadata;
    std::vector<std::uint8_t> persisted_bytes;
};

class SourceIndexedSlotAssignmentReceiptV1 final {
    friend class SourceIndexedSlotServiceV1;
    std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> players_;
    dh2::player::PlayerInfoFieldsV1* player_{};
    std::int32_t local_index_{-1},slot_{-1};
    SourceIndexedSlotAssignmentReceiptV1(
        std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> players,
        dh2::player::PlayerInfoFieldsV1* player,std::int32_t local_index,std::int32_t slot)
        :players_(std::move(players)),player_(player),local_index_(local_index),slot_(slot){}
    static bool same_players(const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& a,
        const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& b)noexcept {
        return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);
    }
public:
    static bool assign_source_slot(
        const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& players,
        std::int32_t local_index,std::int32_t slot,
        const dh2::player::FirstLocalControllerServicesV59& controllers,
        std::shared_ptr<SourceIndexedSlotAssignmentReceiptV1>& output,std::string& error) {
        const auto& players_arg=players;
        if(output||!players_arg||local_index<0||slot<0||slot>=4){
            error="Invalid or already-published NativeAssignSaveSlotToPlayer receipt request";return false;
        }
        if(!players_arg->assign_selected_save_slot_v70(local_index,slot,controllers,error))return false;
        dh2::player::PlayerInfoFieldsV1* selected{};
        if(!players_arg->get_local_player(local_index,false,selected,error)||!selected||selected->save_slot664!=slot){
            if(error.empty())error="Native assignment returned without same PlayerInfo+0x664 selected-slot readback";
            return false;
        }
        auto receipt=std::shared_ptr<SourceIndexedSlotAssignmentReceiptV1>(
            new SourceIndexedSlotAssignmentReceiptV1(players_arg,selected,local_index,slot));
        if(!receipt->valid_current(players_arg,slot,error))return false;
        output=std::move(receipt);error.clear();return true;
    }
    static bool matches_snapshot(
        const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& expected_players,
        dh2::player::PlayerInfoFieldsV1* expected_player,std::int32_t expected_local,
        std::int32_t expected_slot,
        const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& observed_players,
        dh2::player::PlayerInfoFieldsV1* observed_player,std::int32_t observed_local,
        std::int32_t observed_slot) noexcept {
        return expected_players&&observed_players&&expected_players.get()==observed_players.get()&&
            !expected_players.owner_before(observed_players)&&!observed_players.owner_before(expected_players)&&
            expected_player&&expected_player==observed_player&&expected_local>=0&&
            expected_local==observed_local&&expected_slot>=0&&expected_slot==observed_slot;
    }
    bool valid_current(const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& players,
                       std::int32_t slot,std::string& error) const {
        const auto& expected=players;
        if(!same_players(players_,expected)||slot_!=slot||local_index_<0||!player_){
            error="Assignment receipt belongs to another PlayerManager, local slot, or selected profile";return false;
        }
        dh2::player::PlayerInfoFieldsV1* current{};
        if(!expected->get_local_player(local_index_,false,current,error))return false;
        if(!current||current->save_slot664!=slot_||
           !matches_snapshot(players_,player_,local_index_,slot_,expected,current,local_index_,current->save_slot664)){
            error="Assignment receipt is stale: same PlayerInfo+0x664 no longer names the selected slot";return false;
        }
        error.clear();return true;
    }
    std::int32_t local_index()const noexcept{return local_index_;}
    std::int32_t slot()const noexcept{return slot_;}
    const auto& player_manager()const noexcept{return players_;}
};

// Typed evidence returned only by an actual successful native StartGame owner.
// Each lease denotes the reached source operation/identity, not a frontend
// publication marker or candidate-construction prefix.
struct SourceWorldManagerPublicationReceiptV1 {
    std::shared_ptr<void> world;
    std::shared_ptr<dh2::world::CanonicalObjectManagerV1> manager;
    std::int32_t object_key{-1};
    std::uintptr_t actor{};
    std::shared_ptr<const void> source_add_operation;
    bool valid_for(const std::shared_ptr<void>& expected_world,
        const std::shared_ptr<dh2::world::CanonicalObjectManagerV1>& expected_manager,
        std::uintptr_t expected_actor,std::string& error)const;
};

struct SourceNativeStartEvidenceV1 {
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> candidate;
    std::shared_ptr<SourceIndexedSlotAssignmentReceiptV1> assignment;
    std::shared_ptr<dh2::data::PlayerSavegameV1> save;
    std::int32_t slot{-1};
    dh2::world::CharacterCurrentLevelBorrowV61 current_level;
    std::shared_ptr<void> world;
    std::shared_ptr<dh2::world::CanonicalObjectManagerV1> object_manager;
    std::uintptr_t published_character{};
    std::shared_ptr<const void> native_start_operation;
    std::shared_ptr<const SourceWorldManagerPublicationReceiptV1> world_manager_publication;
    static bool same_candidate_slot(
        const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& expected_record,
        const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& observed_record,
        std::int32_t expected_slot,std::int32_t observed_slot)noexcept {
        return expected_record&&observed_record&&expected_record.get()==observed_record.get()&&
            !expected_record.owner_before(observed_record)&&!observed_record.owner_before(expected_record)&&
            expected_slot>=0&&expected_slot==observed_slot;
    }
};

// One-shot source slot creation. It owns the source metadata/file prefix and
// accepts gameplay success only after the caller's native path returns the
// same complete canonical Character/Save/Gear/profile graph.
class SourceIndexedSlotServiceV1 final {
    SourceIndexedSlotServicesV1 services_;
    bool metadata_attempted_{},assignment_attempted_{},completion_attempted_{};
    std::optional<SourceFreshSlotReceiptV1> metadata_receipt_;
    std::shared_ptr<SourceIndexedSlotAssignmentReceiptV1> assignment_receipt_;
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> completed_candidate_;
    std::int32_t assigned_slot_{-1};
    std::string error_;

    bool validate_completion(std::int32_t slot,
        const std::vector<std::uint8_t>& persisted,
        const SourceIndexedSlotCompletionV1&, std::string&) const;
    bool validate_native_start(const SourceNativeStartEvidenceV1&,
        dh::foundation::frontend::FrontendSourceStartReceiptV1&,std::string&) const;
public:
    explicit SourceIndexedSlotServiceV1(SourceIndexedSlotServicesV1 services)
        : services_(std::move(services)) {}

    // NativeCreateSaveSlot phase: writes and verifies only the genuine fresh
    // metadata profile. This receipt is not gameplay creation success.
    bool create_metadata(const SourceIndexedSlotRequestV1&, SourceFreshSlotReceiptV1&,
                         std::string& error);
    // NativeAssignSaveSlotToPlayer phase: exact same Application PlayerManager.
    bool assign_selected_slot(std::int32_t local_player_index, std::int32_t slot,
                              std::string& error);
    // Native start-game creation continuation: success requires complete
    // canonical candidate InitPost + same Save/Gear/profile + publication.
    bool complete_native_creation(std::int32_t slot, SourceIndexedSlotResultV1&,
                                  std::string& error);
    const auto& assignment_receipt()const noexcept{return assignment_receipt_;}
    // Returns the frontend receipt only for complete, same-owner typed evidence
    // from an actual successful native StartGame producer. No provider in the
    // current front-end path can satisfy this yet (it reports launch_pending).
    bool adapt_native_start(std::int32_t slot,
        dh::foundation::frontend::FrontendSourceStartReceiptV1&,std::string& error)const;
    bool attempted() const noexcept { return metadata_attempted_||assignment_attempted_||completion_attempted_; }
    const std::string& retained_error() const noexcept { return error_; }
};

} // namespace dh::foundation::frontend::creation
