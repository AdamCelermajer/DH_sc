#include "frontend_menu_audio_v1.hpp"
#include "feature_audio.hpp"
#include "winmm_output.hpp"
#include "windows_source_session_control_v1.hpp"
#include "../../../engine-audio/integration-v40/runtime/default-source-fields/default_source_fields_v40.hpp"
#include <cstring>
#include <limits>
#include <vector>

namespace dh::foundation::audio {

struct FrontendMenuAudioSessionV1::Context {
    AudioFilesystem files;
    dh2::audio::AudioLifecycleGateV40 lifecycle;
    bool focused{},minimized{};
};

namespace {
constexpr std::uint32_t menu_activity_owner=0x4d454e55u; // "MENU"

bool output_ready(void* raw,std::string& error) {
    const auto* context=static_cast<const FrontendMenuAudioSessionV1::Context*>(raw);
    if(!context||!context->focused||context->minimized) {
        error="Frontend window is not focused for authored menu Play";
        return false;
    }
    error.clear();return true;
}

bool submit_menu_emitter(const dh2::audio::AudioSoundV34& sound,
    const dh2::audio::AudioGroupV34& group,dh2::audio::AudioCommandV34& command,
    std::string& error) {
    if(sound.format==2) {
        error="Native menu Play has no source music-state provider";
        return false;
    }
    command.left=command.right=dh2::audio::original_fresh_emitter_gain_v40();
    command.pitch=dh2::audio::original_fresh_emitter_pitch_v40();
    command.volume_group=group.volume_group;
    error.clear();return true;
}
}

bool resolve_frontend_authored_menu_sound_v1(const char* menu,const char* button,
    const char* action,FrontendAuthoredMenuSoundV1& sound) {
    sound={};
    if(!menu||!button||!action||std::strcmp(menu,"menu_MainMenu")!=0||
        std::strcmp(button,"btn_MENU_SINGLE_PLAYER")!=0||
        std::strcmp(action,"onRelease")!=0)return false;
    sound={"menu_MainMenu","btn_MENU_SINGLE_PLAYER","onRelease","MenuConfirm",141,2};
    return true;
}

bool frontend_menu_event_qpc_ns_v1(std::int64_t& actual_event_qpc_ns,
    std::string& error) {
    return winmm_monotonic_ns(actual_event_qpc_ns,error);
}

FrontendMenuAudioSessionV1::FrontendMenuAudioSessionV1()=default;
FrontendMenuAudioSessionV1::~FrontendMenuAudioSessionV1() {
    if(!session_)return;
    std::string ignored;
    if(!shutdown(ignored)) {
        // Failed output closure must retain the provider lease and worker.
        static auto* retained=new std::vector<std::unique_ptr<dh2::audio::AudioNativeSessionV42>>;
        retained->push_back(std::move(session_));
    }
}

bool FrontendMenuAudioSessionV1::publish_activity(bool focused,bool minimized,
    std::string& error) {
    if(activity_sequence_&&focused_==focused&&minimized_==minimized) {
        error.clear();return true;
    }
    if(activity_sequence_==std::numeric_limits<std::uint32_t>::max()) {
        error="Frontend menu audio activity sequence exhausted";return false;
    }
    ++activity_sequence_;
    focused_=focused;minimized_=minimized;
    if(context_) {context_->focused=focused;context_->minimized=minimized;}
    const bool ok=context_&&context_->lifecycle.publish_activity(menu_activity_owner,
        activity_sequence_,!minimized,focused,focused,false);
    if(!ok){error="Frontend menu audio lifecycle owner rejected actual window activity";return false;}
    error.clear();return true;
}

bool FrontendMenuAudioSessionV1::start(const std::string& exact_asset_root,
    bool focused,bool minimized,std::string& error) {
    if(started_||session_) {error="Frontend menu audio session is already owned";return false;}
    if(exact_asset_root.empty()) {error="Required exact original audio asset root";return false;}
    context_=std::make_shared<Context>();context_->files.root=exact_asset_root;
    context_->focused=focused;context_->minimized=minimized;
    dh2::audio::AudioGameplaySourcesV40 sources;
    sources.context=context_.get();
    sources.exact_assets={&context_->files,AudioFilesystem::read,{}};
    sources.output_ready=output_ready;
    const auto manager_identity=reinterpret_cast<std::uintptr_t>(context_.get());
    session_=std::make_unique<dh2::audio::AudioNativeSessionV42>(manager_identity,
        sources,context_,context_->lifecycle,
        dh2::audio::windows_source_session_control_v1());
    if(!publish_activity(focused,minimized,error))return false;
    if(!session_->initialize(error))return false;
    started_=true;error.clear();return true;
}

bool FrontendMenuAudioSessionV1::window_activity(bool focused,bool minimized,
    std::string& error) {
    if(!session_||!started_) {error="Frontend menu audio session is not started";return false;}
    return publish_activity(focused,minimized,error);
}

bool FrontendMenuAudioSessionV1::ready() const {
    return session_&&session_->ready_for_current_source();
}

bool FrontendMenuAudioSessionV1::play_menu_sound(const char* name,
    std::int64_t event_ns,FrontendMenuAudioReceiptV1& receipt,std::string& error) {
    receipt={};error.clear();
    if(!session_||!started_) {error="Frontend menu audio session is not started";return false;}
    auto* runtime=session_->runtime_on_producer();
    if(!runtime||!runtime->source_data_initialized()) {
        error="Frontend menu audio source runtime is unavailable";return false;
    }
    if(!name||!*name) {
        receipt.status=FrontendMenuAudioStatusV1::unknown_source_name;
        receipt.detail="NativePlaySoundFX name is empty";return true;
    }
    receipt.source_ordinal=runtime->bindings().source_id(name);
    if(receipt.source_ordinal<0) {
        receipt.status=FrontendMenuAudioStatusV1::unknown_source_name;
        receipt.detail="Exact generated sound name is absent; original wrapper is a no-op";
        return true;
    }
    const auto* row=runtime->bindings().row(receipt.source_ordinal);
    if(!row||row->event) {
        receipt.status=FrontendMenuAudioStatusV1::rejected;
        receipt.detail="NativePlaySoundFX requires a generated plain Play row";
        return true;
    }
    receipt.xml_sound_uid=row->uid;
    if(!focused_||minimized_||!ready()) {
        receipt.status=FrontendMenuAudioStatusV1::skipped_without_focus;
        receipt.detail="Source Play did not reach a focused frontend output";
        return true;
    }
    const bool submitted=runtime->submit_plain_source(receipt.source_ordinal,event_ns,
        submit_menu_emitter,error);
    if(submitted) {
        receipt.status=FrontendMenuAudioStatusV1::submitted;
        receipt.token=runtime->last_token();return true;
    }
    receipt.detail=error;error.clear();
    if(receipt.detail.rfind("Unavailable original audio asset: ",0)==0)
        receipt.status=FrontendMenuAudioStatusV1::missing_original_asset;
    else if(!focused_||minimized_||!ready())
        receipt.status=FrontendMenuAudioStatusV1::skipped_without_focus;
    else receipt.status=FrontendMenuAudioStatusV1::rejected;
    return true;
}

bool FrontendMenuAudioSessionV1::play_authored_menu_action(const char* menu,
    const char* button,const char* action,std::int64_t event_ns,
    FrontendMenuAudioReceiptV1& receipt,std::string& error) {
    FrontendAuthoredMenuSoundV1 source;
    if(!resolve_frontend_authored_menu_sound_v1(menu,button,action,source)) {
        receipt={};receipt.status=FrontendMenuAudioStatusV1::unknown_source_name;
        receipt.detail="No recovered sound is mapped to this authored frontend action";
        error.clear();return true;
    }
    if(!session_||!started_) {error="Frontend menu audio session is not started";return false;}
    auto* runtime=session_->runtime_on_producer();
    if(!runtime||!runtime->source_data_initialized()) {
        error="Frontend menu audio source runtime is unavailable";return false;
    }
    const auto ordinal=runtime->bindings().source_id(source.sound_name);
    const auto* row=runtime->bindings().row(ordinal);
    if(ordinal!=source.source_ordinal||!row||row->event||row->uid!=source.xml_sound_uid) {
        receipt={};receipt.status=FrontendMenuAudioStatusV1::rejected;
        receipt.detail="Recovered authored action mapping disagrees with the generated source catalog";
        error.clear();return true;
    }
    if(!play_menu_sound(source.sound_name,event_ns,receipt,error))return false;
    if(receipt.source_ordinal!=source.source_ordinal||receipt.xml_sound_uid!=source.xml_sound_uid) {
        receipt.status=FrontendMenuAudioStatusV1::rejected;
        receipt.token=0;
        receipt.detail="Generated menu source identity changed after strict action lookup";
        error.clear();
    }
    return true;
}

bool FrontendMenuAudioSessionV1::play_music(const char* name,int fade_ms,
    FrontendMusicReceiptV1& receipt,std::string& error) {
    receipt={};error.clear();
    if(!session_||!started_) {error="Frontend menu audio session is not started";return false;}
    auto* runtime=session_->runtime_on_producer();
    if(!runtime||!runtime->source_data_initialized()) {
        error="Frontend menu audio source runtime is unavailable";return false;
    }
    if(!name||!*name) {
        receipt.status=FrontendMusicStatusV1::unknown_source_name;
        receipt.detail="NativePlayMusic name is empty";return true;
    }
    receipt.source_ordinal=runtime->bindings().source_id(name);
    if(receipt.source_ordinal<0) {
        receipt.status=FrontendMusicStatusV1::unknown_source_name;
        receipt.detail="Exact generated sound name is absent; original wrapper is a no-op";
        return true;
    }
    const auto* row=runtime->bindings().row(receipt.source_ordinal);
    receipt.xml_sound_uid=row?row->uid:-1;
    if(!focused_||minimized_||!ready()) {
        receipt.status=FrontendMusicStatusV1::skipped_without_focus;
        receipt.detail="Source PlayMusic did not reach a focused frontend output";
        return true;
    }
    MusicVoiceActionV1 action{};
    std::string playError;
    if(music_.play(*runtime,receipt.source_ordinal,fade_ms,action,playError)) {
        receipt.status=action==MusicVoiceActionV1::resumed?FrontendMusicStatusV1::resumed:
            action==MusicVoiceActionV1::switched?FrontendMusicStatusV1::switched:FrontendMusicStatusV1::started;
        return true;
    }
    receipt.detail=playError;
    if(playError.rfind("Unavailable original audio asset: ",0)==0)
        receipt.status=FrontendMusicStatusV1::missing_original_asset;
    else if(!focused_||minimized_||!ready())
        receipt.status=FrontendMusicStatusV1::skipped_without_focus;
    else receipt.status=FrontendMusicStatusV1::rejected;
    return true;
}

bool FrontendMenuAudioSessionV1::stop_music(int fade_ms,std::string& error) {
    if(music_.ordinal<0) {error.clear();return true;}
    auto* runtime=session_?session_->runtime_on_producer():nullptr;
    if(!runtime) {error="Frontend menu audio source runtime is unavailable";return false;}
    return music_.stop(*runtime,fade_ms,error);
}

bool FrontendMenuAudioSessionV1::music_playing(std::string& error) {
    auto* runtime=session_?session_->runtime_on_producer():nullptr;
    if(!runtime||music_.ordinal<0) {error.clear();return false;}
    bool playing{};
    if(!runtime->source_ordinal_playing(music_.ordinal,playing,error))return false;
    return playing;
}

void FrontendMenuAudioSessionV1::pump_receipts() {
    if(auto* runtime=session_?session_->runtime_on_producer():nullptr)runtime->pump_receipts();
}

bool FrontendMenuAudioSessionV1::take_receipt(dh2::audio::AudioReceiptV34& receipt) {
    auto* runtime=session_?session_->runtime_on_producer():nullptr;
    return runtime&&runtime->take_receipt(receipt);
}

bool FrontendMenuAudioSessionV1::shutdown(std::string& error) {
    if(!session_) {started_=false;context_.reset();error.clear();return true;}
    if(!session_->shutdown(error))return false;
    session_.reset();context_.reset();started_=false;error.clear();return true;
}

} // namespace dh::foundation::audio
