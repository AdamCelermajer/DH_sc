#include "source_indexed_slot_service_v1.hpp"
#include "../../../../level-world/savegame_stream_v2.hpp"
#include <cstdio>

namespace dh::foundation::frontend::creation {
namespace {
template<class T> bool same_owner(const std::shared_ptr<T>& a,const std::shared_ptr<T>& b) {
    return a && b && a.get()==b.get() && !a.owner_before(b) && !b.owner_before(a);
}
std::string filename_for(std::int32_t slot) {
    char name[32]{};
    std::snprintf(name,sizeof(name),"dh2_%03d.savegame",slot);
    return name;
}
}

bool SourceIndexedSlotServiceV1::validate_completion(std::int32_t slot,
    const std::vector<std::uint8_t>& persisted,
    const SourceIndexedSlotCompletionV1& done,std::string& e) const {
    const auto& r=done.candidate;
    const auto& binding=done.selected_profile;
    if(!r||!binding||!binding->ready()||!done.source_publication) {
        e="Native indexed creation did not return its published candidate and prepared selected-profile binding";return false;
    }
    const auto receipt=binding->file_receipt();
    if(!receipt||!same_owner(receipt->files,services_.files)||receipt->slot!=slot||receipt->bytes!=persisted) {
        e="Selected profile file receipt differs from the exact same-Application indexed slot bytes";return false;
    }
    if(!r->init_complete||r->failed||!r->actor||!r->save||!r->load||!r->profile_bootstrap||
       !r->equipment||!r->equipment->ready()||!r->prepared_equipment_v60||
       r->prepared_equipment_v60!=r->equipment.get()) {
        e="Native Character creation is incomplete; require the same candidate InitPost, Save, Load and ready Gear";return false;
    }
    if(r->save->slot()!=slot||!same_owner(r->profile_bootstrap->save(),r->save)||
       !same_owner(r->profile_bootstrap->load_owner(),r->load)||
       !same_owner(r->profile_bootstrap->profile(),binding->profile())||
       !r->profile_bootstrap->finished()||!r->profile_bootstrap->gear_delivered()) {
        e="Candidate Save/Gear/profile bootstrap does not match the exact selected slot and profile binding";return false;
    }
    const auto& input=binding->bootstrap_inputs();
    if(input.selected_slot!=slot||!same_owner(input.save,r->save)||!same_owner(input.load,r->load)||
       !same_owner(input.profile,binding->profile())||!input.source_direct_save_slot_v122||
       *input.source_direct_save_slot_v122!=slot) {
        e="Selected profile bootstrap inputs are not the same candidate Save/Load/direct source slot";return false;
    }
    e.clear();return true;
}

bool SourceWorldManagerPublicationReceiptV1::valid_for(
    const std::shared_ptr<void>& expected_world,
    const std::shared_ptr<dh2::world::CanonicalObjectManagerV1>& expected_manager,
    std::uintptr_t expected_actor,std::string& e) const {
    if(!same_owner(world,expected_world)||!same_owner(manager,expected_manager)||
       object_key<=0||!actor||actor!=expected_actor||!source_add_operation){
        e="Native World/ObjectManager Add publication receipt has foreign or incomplete identity";return false;
    }
    const auto* published=manager->object(object_key);
    if(!published||published->identity!=actor||!published->lease){
        e="Same canonical ObjectManager no longer publishes the selected Character actor";return false;
    }
    e.clear();return true;
}

bool SourceIndexedSlotServiceV1::create_metadata(const SourceIndexedSlotRequestV1& request,
    SourceFreshSlotReceiptV1& output,std::string& e) {
    if(metadata_attempted_){e=error_.empty()?"NativeCreateSaveSlot metadata prefix cannot replay":error_;return false;}
    metadata_attempted_=true;
    auto fail=[&](const std::string& why){error_=why;e=why;return false;};
    if(!services_.application||!services_.files||!services_.characters)
        return fail("Required same-Application files, CharacterTable and native source metadata inputs");
    if(!services_.files->belongs_to_application(services_.application))
        return fail("Indexed metadata creation requires the existing same-Application file/job owner");
    if(!services_.files->files()||!services_.files->jobs()||
       services_.files->jobs()->file_storage_v59().get()!=services_.files->files().get())
        return fail("Required same-Application FileManager and SavegameJobs owner pair");

    std::int32_t slot=-1;
    for(std::int32_t candidate=0;candidate<4;++candidate){
        bool occupied{};std::string probe;
        if(!dh2::data::campaign_profile_exists_v1(services_.files->files()->directory_v59(),
             static_cast<std::uint32_t>(candidate),occupied,probe))return fail(probe);
        if(!occupied){slot=candidate;break;}
    }
    if(slot<0)return fail("No free source campaign profile slot");
    if(request.name.empty()||request.playable_class.empty())return fail("Required source player name and playable CharacterTable class");

    dh2::data::FreshPlayerProfileV1 metadata;
    std::string source_error;
    if(!dh2::data::fresh_player_profile_v1(*services_.characters,request.playable_class.c_str(),
        request.name.c_str(),request.source_timer,request.saved_date,metadata,source_error))return fail(source_error);
    const auto filename=filename_for(slot);
    // This is the original framed Savegame payload produced by the fresh
    // metadata serializer. Queue it on the SAME Application-global jobs owner.
    auto stream=std::make_unique<dh2::level::SavegameStreamV2>(
        dh2::data::Bytes{metadata.bytes.data(),metadata.bytes.size()});
    if(!services_.files->jobs()->add_write(filename,std::move(stream),source_error))return fail(source_error);
    if(!services_.files->flush(filename.c_str(),source_error))return fail(source_error);

    bool found{};std::vector<std::uint8_t> persisted;
    if(!services_.files->read_save(filename,found,persisted,source_error)||!found)return fail(
        source_error.empty()?"Fresh source profile was not readable after completed same-owner write":source_error);
    if(persisted!=metadata.bytes)return fail("Persisted indexed profile bytes differ from the original fresh metadata stream");
    dh2::data::PlayerProfileIndexV1 index;
    if(!index.load({persisted.data(),persisted.size()},source_error))return fail(source_error);
    dh2::data::MenuProfileMetadataV1 display;
    if(!dh2::data::load_menu_profile_metadata_v1({persisted.data(),persisted.size()},*services_.characters,
        slot,0,{},display,source_error))return fail(source_error);
    if(display.slot!=slot||display.name!=metadata.name||display.character_row!=metadata.character_row||display.level!=1)
        return fail("Fresh indexed profile failed actual source metadata readback");

    SourceFreshSlotReceiptV1 next;next.slot=slot;next.filename=filename;
    next.metadata=std::move(metadata);next.persisted_bytes=std::move(persisted);
    metadata_receipt_=next;output=std::move(next);error_.clear();e.clear();return true;
}

bool SourceIndexedSlotServiceV1::assign_selected_slot(std::int32_t local_index,
    std::int32_t slot,std::string& e) {
    if(assignment_attempted_){e=error_.empty()?"NativeAssignSaveSlotToPlayer prefix cannot replay":error_;return false;}
    assignment_attempted_=true;
    if(!metadata_receipt_||metadata_receipt_->slot!=slot){error_="Assignment slot does not match the exact fresh metadata receipt";e=error_;return false;}
    if(!services_.application||!services_.players||
       !services_.players->belongs_to_application(services_.application)){
        error_="Native assignment requires the existing same-Application PlayerManager";e=error_;return false;
    }
    if(!SourceIndexedSlotAssignmentReceiptV1::assign_source_slot(
        services_.players,local_index,slot,services_.first_local,assignment_receipt_,e)){
        error_=e;return false;
    }
    assigned_slot_=slot;error_.clear();e.clear();return true;
}

bool SourceIndexedSlotServiceV1::complete_native_creation(std::int32_t slot,
    SourceIndexedSlotResultV1& output,std::string& e) {
    if(completion_attempted_){e=error_.empty()?"Native Character creation continuation cannot replay":error_;return false;}
    completion_attempted_=true;
    auto fail=[&](const std::string& why){error_=why;e=why;return false;};
    if(!metadata_receipt_||metadata_receipt_->slot!=slot||assigned_slot_!=slot)
        return fail("Full Character creation requires the same metadata and selected-PlayerInfo slot prefixes");
    if(!services_.application||!services_.files||!services_.players||
       !services_.files->belongs_to_application(services_.application)||
       !services_.players->belongs_to_application(services_.application)||
       !services_.complete_native_creation)
        return fail("Required same-Application source owners and full Character creation continuation");
    SourceIndexedSlotCompletionV1 completion;
    std::string source_error;
    if(!services_.complete_native_creation(slot,metadata_receipt_->filename,
        metadata_receipt_->metadata,services_.files,completion,source_error))
        return fail(source_error.empty()?"Source Character C1/profile/InitPost continuation failed":source_error);
    if(!validate_completion(slot,metadata_receipt_->persisted_bytes,completion,source_error))return fail(source_error);
    completed_candidate_=completion.candidate;
    SourceIndexedSlotResultV1 next;next.slot=slot;next.filename=metadata_receipt_->filename;
    next.metadata=metadata_receipt_->metadata;next.selected_profile.record=completion.candidate;
    output=std::move(next);error_.clear();e.clear();return true;
}

bool SourceIndexedSlotServiceV1::validate_native_start(const SourceNativeStartEvidenceV1& evidence,
    dh::foundation::frontend::FrontendSourceStartReceiptV1& output,std::string& e) const {
    const auto& candidate=evidence.candidate;
    if(!SourceNativeStartEvidenceV1::same_candidate_slot(completed_candidate_,candidate,
       metadata_receipt_?metadata_receipt_->slot:-1,evidence.slot)||!same_owner(candidate,completed_candidate_)||!candidate||
       !candidate->init_complete||candidate->failed||!candidate->save||
       !same_owner(evidence.save,candidate->save)||evidence.slot<0||candidate->save->slot()!=evidence.slot){
        e="Native StartGame evidence does not name the exact completed canonical candidate and Save slot";return false;
    }
    if(!assignment_receipt_||!same_owner(evidence.assignment,assignment_receipt_)||
       !evidence.assignment->valid_current(services_.players,evidence.slot,e)){
        if(e.empty())e="Native StartGame evidence lacks the exact live same-PlayerManager assignment receipt";
        return false;
    }
    if(!evidence.native_start_operation||!evidence.world_manager_publication){
        e="Required actual NativeStartGame and typed same-World/ObjectManager Add publication receipts";return false;
    }
    if(!candidate->services.current_level||!evidence.current_level.receiver||!evidence.current_level.identity||
       !evidence.world||!same_owner(evidence.world,candidate->services.world)||
       !evidence.object_manager||!same_owner(evidence.object_manager,candidate->services.canonical_objects)){
        e="Native StartGame evidence lacks the same current Level, World and canonical ObjectManager owners";return false;
    }
    dh2::world::CharacterCurrentLevelBorrowV61 current;
    if(!candidate->services.current_level(current,e))return false;
    if(current.identity!=evidence.current_level.identity||
       !same_owner(current.receiver,evidence.current_level.receiver)||
       !current.mode118||!evidence.current_level.mode118){
        e="Native StartGame current-Level lease is stale or differs from the candidate's actual Application Level";return false;
    }
    if(!candidate->actor||!evidence.published_character||
       candidate->actor->shared_handle().cached!=evidence.published_character){
        e="Native StartGame publication does not name the same canonical Character actor";return false;
    }
    if(!evidence.world_manager_publication->valid_for(evidence.world,evidence.object_manager,
       evidence.published_character,e))return false;
    auto native_receipt=std::make_shared<SourceNativeStartEvidenceV1>(evidence);
    dh::foundation::frontend::FrontendSourceStartReceiptV1 next;
    next.selected_profile.record=candidate;next.selected_slot=evidence.slot;
    next.source_assign_receipt=std::static_pointer_cast<const void>(assignment_receipt_);
    next.source_start_receipt=std::static_pointer_cast<const void>(native_receipt);
    output=std::move(next);e.clear();return true;
}

bool SourceIndexedSlotServiceV1::adapt_native_start(std::int32_t slot,
    dh::foundation::frontend::FrontendSourceStartReceiptV1& output,std::string& e)const {
    if(!services_.source_native_start_success){
        e="Actual NativeStartGame success receipt unavailable: current front_ui_session only reports launch_pending/readback";return false;
    }
    if(!completed_candidate_||!metadata_receipt_||metadata_receipt_->slot!=slot||assigned_slot_!=slot){
        e="Native StartGame receipt requires the same completed candidate, metadata slot and live assignment receipt";return false;
    }
    SourceNativeStartEvidenceV1 evidence;
    if(!services_.source_native_start_success(slot,completed_candidate_,evidence,e)){
        if(e.empty())e="Actual native StartGame owner did not provide a successful typed receipt";
        return false;
    }
    return validate_native_start(evidence,output,e);
}

} // namespace dh::foundation::frontend::creation
