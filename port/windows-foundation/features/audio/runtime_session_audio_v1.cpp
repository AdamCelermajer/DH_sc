#include "runtime_session_audio_v1.hpp"
#include "audio_source_target_position_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_camera_anchor.hpp"
#include "winmm_output.hpp"
#include <limits>
#include <initializer_list>

namespace dh::foundation::audio {
struct RuntimeSessionAudioV1::Context {
    RuntimeAudioHostFactsV1 facts;
    std::weak_ptr<CombatSession> session;
    bool minimal_randoms{};
    std::uint64_t frame{};
};

namespace {
std::vector<std::uint8_t> read_first(const AssetCatalog& assets,
    std::initializer_list<const char*> paths) {
    for(const auto* path:paths)try {return assets.read(path);} catch(const std::exception&) {}
    return {};
}

bool load_attack_metadata(const std::string& root,dh2::data::AnimationTables& animations,
    dh2::data::ItemTable& items,std::string& error) {
    try {
        AssetCatalog assets(root);
        const auto clip_names=read_first(assets,{
            "data/animations_dictionary_pyarraynames.bin",
            "data/pydata/animations_dictionary_pyarraynames.bin"});
        const auto clip_values=read_first(assets,{
            "data/animations_dictionary_pyarray.bin",
            "data/pydata/animations_dictionary_pyarray.bin"});
        const auto animation_records=read_first(assets,{
            "original-cache/data/pydata/animations_pyarray.bin",
            "data/pydata/animations_pyarray.bin","data/animations_pyarray.bin"});
        const auto animation_names=read_first(assets,{
            "original-cache/data/pydata/animations_pyarraynames.bin",
            "data/pydata/animations_pyarraynames.bin","data/animations_pyarraynames.bin"});
        const auto animation_fields=read_first(assets,{
            "original-cache/data/pydata/animations_pystructnames.bin",
            "data/pydata/animations_pystructnames.bin","data/animations_pystructnames.bin"});
        const auto item_records=read_first(assets,{
            "original-cache/data/pydata/loot_table_pyarray.bin",
            "data/pydata/loot_table_pyarray.bin"});
        const auto item_names=read_first(assets,{
            "original-cache/data/pydata/loot_table_pyarraynames.bin",
            "data/pydata/loot_table_pyarraynames.bin"});
        const auto item_fields=read_first(assets,{
            "original-cache/data/pydata/loot_table_pystructnames.bin",
            "data/pydata/loot_table_pystructnames.bin"});
        if(clip_names.empty()||clip_values.empty()||animation_records.empty()||
           animation_names.empty()||animation_fields.empty()||item_records.empty()||
           item_names.empty()||item_fields.empty()) {
            error="Actual source AnimTable/ItemTable metadata is unavailable under the audio asset root";
            return false;
        }
        dh2::data::Dictionary clips;
        if(!dh2::data::load_dictionary({clip_names.data(),clip_names.size()},
              {clip_values.data(),clip_values.size()},clips,error))return false;
        if(!dh2::data::load_animation_tables({animation_records.data(),animation_records.size()},
              {animation_names.data(),animation_names.size()},
              {animation_fields.data(),animation_fields.size()},clips,animations,error))return false;
        if(!dh2::data::load_items({item_records.data(),item_records.size()},
              {item_names.data(),item_names.size()},
              {item_fields.data(),item_fields.size()},items,error))return false;
        error.clear();return true;
    } catch(const std::exception& ex) {error=ex.what();return false;}
}
}

RuntimeSessionAudioV1::RuntimeSessionAudioV1(std::ostream& log):log_(log) {}
RuntimeSessionAudioV1::~RuntimeSessionAudioV1() {
    std::string ignored;shutdown(ignored);
}

bool RuntimeSessionAudioV1::start(const std::string& assets,
    const std::vector<std::uint8_t>& bytes,int listener_index,
    const std::shared_ptr<CombatSession>& session,bool focused,bool minimized,
    std::string& error,const std::string& gameplay_metadata_root) {
    if(!session||!session->world()) {error="Audio requires the current gameplay Session";return false;}
    std::vector<dh2::audio::AudioListenerRowV38> rows;
    if(!tables_.load(bytes,error)||
       !dh2::audio::audio_listener_rows_v38(bytes.data(),bytes.size(),rows,error))return false;
    if(listener_index<0||std::size_t(listener_index)>=rows.size()) {
        error="Original audio listener index is outside the loaded table";return false;
    }
    listener_=rows[std::size_t(listener_index)];
    // Level C1 defaults to row1: player target, camera XY look, camera up.
    // Alternative screen-unproject/body-up policies need actual providers.
    if(listener_.anchor!=2||listener_.orientation!=2||listener_.up_vector!=0) {
        error="Audio listener row requires an unbound screen/body projection policy";return false;
    }
    const auto& metadata_root=gameplay_metadata_root.empty()?assets:gameplay_metadata_root;
    attack_tables_ready_=load_attack_metadata(metadata_root,animations_,items_,error);
    if(!attack_tables_ready_) {
        log_<<"Audio step sound unavailable from gameplay metadata root "<<metadata_root
            <<": "<<error<<"; remaining source audio remains active\n";
        error.clear();
    }
    context_=std::make_shared<Context>();context_->session=session;
    RuntimeAudioHostConfigV1 config;
    // These are identities of this retained in-house host/gameplay context,
    // never claims of original Vox/GS object addresses.
    config.actual_manager_identity=reinterpret_cast<std::uintptr_t>(this);
    config.gameplay_context_identity=reinterpret_cast<std::uintptr_t>(context_.get());
    config.exact_asset_root=assets;config.provider_lease=context_;
    config.providers.context=context_.get();
    config.providers.snapshot=[](void* raw,RuntimeAudioHostFactsV1& facts,std::string& e) {
        auto& context=*static_cast<Context*>(raw);facts=context.facts;
        const auto current=context.session.lock();
        facts.gameplay_active=current&&current->world()&&!current->actor_binding_lease().expired();
        e.clear();return true;
    };
    config.providers.source_event_random={context_.get(),[](void* raw,int& value) {
        const auto current=static_cast<Context*>(raw)->session.lock();
        if(!current||!current->world())return false;
        std::uint32_t result;std::string error;
        if(!current->world()->random_uniform(INT32_MAX,result,error))return false;
        value=static_cast<int>(result);return true;
    }};
    host_=std::make_unique<RuntimeAudioHostV1>(std::move(config));
    activity_sequence_=1;focused_=focused;minimized_=minimized;activity_known_=true;
    if(!host_->publish_window_activity(1,activity_sequence_,!minimized,focused,focused,false)||
       !host_->start(error))return false;
    if(!bind(session,error))return false;
    log_<<"Audio initialized listener="<<listener_index<<" anchor="<<listener_.anchor
        <<" reference="<<listener_.reference_distance<<" maximum="<<listener_.maximum_distance
        <<"; one V42 WinMM output, source metadata\n";
    return true;
}

void RuntimeSessionAudioV1::unbind() noexcept {
    if(const auto session=bound_.lock()) {
        session->clear_retained_frame_audio_observer();
        session->set_step_entry_observer({});
    }
    attack_step_observer_={};attack_.reset();combat_.reset();bound_.reset();clock_={};
    if(context_) {context_->session.reset();context_->facts.gameplay_active=false;
        context_->facts.listener_initialized=false;}
}

bool RuntimeSessionAudioV1::add_step_entry_presentation_observer(
    CombatSessionStepObserver observer,std::string& error) {
    return step_entry_presenters_.add(std::move(observer),error);
}

void RuntimeSessionAudioV1::clear_step_entry_presentation_observers() noexcept {
    step_entry_presenters_.clear();
}

CombatSessionStepObserver RuntimeSessionAudioV1::compose_step_entry_observer(
    CombatSessionStepObserver audio_first) {
    return step_entry_presenters_.compose(std::move(audio_first),[this](const std::string& message) {
        ++diagnostics_;try {log_<<message<<'\n';} catch(...) {}
    });
}

bool RuntimeSessionAudioV1::bind(const std::shared_ptr<CombatSession>& session,
                               std::string& error) {
    unbind();
    if(!host_||!context_||!session||!session->world()||session->actor_binding_lease().expired()) {
        error="Audio bind requires the current initialized output and Session lease";return false;
    }
    if(context_->facts.world_epoch==UINT64_MAX) {error="Audio gameplay epoch exhausted";return false;}
    ++context_->facts.world_epoch;context_->session=session;
    RuntimeCombatAudioServicesV1 services;
    services.minimal_randoms=[context=context_](std::uint32_t& value,std::string& e) {
        value=context->minimal_randoms?1:0;e.clear();return true;
    };
    services.random=[context=context_](std::uint32_t bound,std::uint32_t& value,std::string& e) {
        const auto current=context->session.lock();
        if(!current||!current->world()) {e="Audio gameplay RNG Session is unavailable";return false;}
        return current->world()->random_uniform(bound,value,e);
    };
    services.diagnostic=[this](const RuntimeCombatAudioDiagnosticV1& diagnostic) {
        if(diagnostic.status==RuntimeCombatAudioStatusV1::not_applicable)return;
        if(diagnostic.status==RuntimeCombatAudioStatusV1::dispatched)++dispatched_;
        else ++diagnostics_;
        log_<<"Audio source frame="<<context_->frame<<" actor="<<diagnostic.actor
            <<" target="<<diagnostic.target<<" event="<<diagnostic.event_name
            <<" ordinal="<<diagnostic.event_index<<" sound="<<diagnostic.source_id
            <<" status="<<int(diagnostic.status)<<" detail="<<diagnostic.detail<<'\n';
    };
    combat_=host_->make_combat_audio(*session,tables_,std::move(services));
    if(!combat_) {error="Audio combat observer could not borrow the existing output";return false;}
    if(attack_tables_ready_) {
        attack_=std::make_unique<RuntimeAttackSoundV1>(
            [this](ActorId actor,std::int32_t sound,const std::array<float,3>& position,
                   std::int64_t qpc,std::string& detail) {
                return host_&&host_->submit_source_sound(actor,sound,position,qpc,detail);
            },[this]{return host_&&host_->ready();},*session,animations_,items_,
            [this](const RuntimeAttackSoundDiagnosticV1& item) {
                if(item.status==RuntimeAttackSoundStatusV1::not_applicable)return;
                log_<<"Audio step source update="<<item.update_serial<<" occurrence="<<item.occurrence
                    <<" actor="<<item.actor<<" sequence="<<item.sequence_id
                    <<" step="<<item.step<<" sound="<<item.sound_id
                    <<" status="<<int(item.status)<<" detail="<<item.detail<<'\n';
            });
        attack_step_observer_=attack_->step_entry_observer();
    }
    session->set_step_entry_observer(compose_step_entry_observer(attack_step_observer_));
    session->set_retained_frame_audio_observer(combat_->retained_event_observer());
    bound_=session;error.clear();return true;
}

bool RuntimeSessionAudioV1::window_activity(bool focused,bool minimized,std::string& error) {
    if(!host_) {error="Audio host is unavailable";return false;}
    if(!activity_known_||focused_!=focused||minimized_!=minimized) {
        if(activity_sequence_==UINT32_MAX) {error="Audio window activity sequence exhausted";return false;}
        focused_=focused;minimized_=minimized;activity_known_=true;
        if(!host_->publish_window_activity(1,++activity_sequence_,!minimized,focused,focused,false)) {
            error="Actual audio window activity publication failed";return false;
        }
        log_<<"Audio window activity focused="<<focused<<" minimized="<<minimized<<'\n';
        // Original Application::Pause -> PauseAllSounds; Resume -> ResumeAllSounds.
        if(!host_->set_output_paused(!focused||minimized,error))return false;
    }
    error.clear();return true;
}

bool RuntimeSessionAudioV1::set_level_music(const std::string& name,std::string& error) {
    if(!host_) {error="Audio host is unavailable";return false;}
    level_music_name_=name;level_music_error_.clear();error.clear();return true;
}

const RetainedFrameAudioClock* RuntimeSessionAudioV1::before_update(const Camera& camera,
    bool focused,bool minimized,bool minimal_randoms,std::uint64_t frame,std::string& error) {
    clock_={};
    if(!host_||!context_) {error="Audio host is unavailable";return nullptr;}
    if(!window_activity(focused,minimized,error))return nullptr;
    context_->frame=frame;context_->minimal_randoms=minimal_randoms;
    const auto session=context_->session.lock();
    const auto* player=session?session->actor(session->player_id()):nullptr;
    if(!player) {error="Audio local player position is unavailable";return nullptr;}
    const auto position=audio_source_target_position_v1(*player);
    const float front[3]{camera.target.x-camera.eye.x,camera.target.y-camera.eye.y,0};
    const float up[3]{camera.up.x,camera.up.y,camera.up.z};
    auto& facts=context_->facts;
    if(!dh2::audio::audio_listener_update_v38(listener_,position.data(),front,up,facts.listener,error))return nullptr;
    facts.listener_initialized=true;facts.listener_world_epoch=facts.world_epoch;
    // Native music state remains unknown until its selected VXN is initialized.
    // The ordinary WAV swing/hit sources do not consume this field.
    facts.native_initial_state=-1;
    if(!host_->device_clock(clock_,error))return nullptr;
    error.clear();return &clock_;
}

bool RuntimeSessionAudioV1::after_update(std::string& error) {
    if(!host_) {error="Audio host is unavailable";return false;}
    if(!host_->update(error))return false;
    // Level::Update-equivalent start: only with focused, non-minimised output.
    // Same-ordinal requests are not repeated once the voice is owned.
    if(!level_music_name_.empty()&&activity_known_&&focused_&&!minimized_) {
        const auto ordinal=host_->source_ordinal(level_music_name_.c_str());
        std::string musicError;
        if(ordinal<0) musicError="Level music is absent from the source sound table: "+level_music_name_;
        else if(ordinal!=host_->level_music_ordinal())host_->play_level_music(ordinal,2000,musicError);
        if(!musicError.empty()&&musicError!=level_music_error_) {
            level_music_error_=musicError;
            log_<<"Level music diagnostic: "<<musicError<<" (retrying)\n";
        }
        if(musicError.empty())level_music_error_.clear();
    }
    dh2::audio::AudioReceiptV34 receipt;
    while(host_->take_receipt(receipt)) {
        if(receipt.kind==dh2::audio::AudioReceiptKindV34::started)++started_;
        log_<<"Audio voice kind="<<int(receipt.kind)<<" token="<<receipt.token<<" outputFrame="<<receipt.frame<<'\n';
    }
    return true;
}

bool RuntimeSessionAudioV1::shutdown(std::string& error) {
    unbind();
    clear_step_entry_presentation_observers();
    if(host_&&!host_->shutdown(error))return false;
    host_.reset();context_.reset();error.clear();return true;
}
void RuntimeSessionAudioV1::summary() const {
    log_<<"Audio final dispatched="<<dispatched_<<" startedVoices="<<started_
        <<" diagnostics="<<diagnostics_<<"; original assets, no substituted samples\n";
#ifdef _WIN32
    // B039: wall time is first-to-latest pump update; rendered time is mixer frames at 48 kHz.
    const auto pump=winmm_pump_stats_v1();
    if(pump.updates&&host_)
        log_<<"WinMM pump: underruns="<<pump.underruns<<" refills="<<pump.refills
            <<" renderedSeconds="<<double(host_->rendered_frames())/48000.0
            <<" wallSeconds="<<double(pump.last_ns-pump.first_ns)/1e9
            <<" maxGapMs="<<double(pump.max_update_gap_ns)/1e6
            <<" gapsOver40ms="<<pump.gaps_over_40ms<<'\n';
#endif
}
}
