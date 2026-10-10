#include "frontend_menu_audio_v1.hpp"
#include "winmm_output.hpp"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace dh::foundation::audio;
using namespace dh2::audio;

namespace {
void check(bool condition,const std::string& message) {
    if(!condition)throw std::runtime_error(message);
}
bool wait_ready(FrontendMenuAudioSessionV1& audio,bool value) {
    const auto until=std::chrono::steady_clock::now()+std::chrono::seconds(4);
    while(std::chrono::steady_clock::now()<until) {
        if(audio.ready()==value)return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return audio.ready()==value;
}
bool wait_started(FrontendMenuAudioSessionV1& audio,std::uint64_t token) {
    const auto until=std::chrono::steady_clock::now()+std::chrono::seconds(3);
    while(std::chrono::steady_clock::now()<until) {
        audio.pump_receipts();AudioReceiptV34 receipt;
        while(audio.take_receipt(receipt))
            if(receipt.token==token&&receipt.kind==AudioReceiptKindV34::started)return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return false;
}
}

int main(int argc,char** argv) {
    try {
        check(argc==2,"Supply isolated exact original soundpack asset root");
        FrontendMenuAudioSessionV1 audio;std::string error;
        check(audio.start(argv[1],true,false,error),"Real frontend menu audio start failed: "+error);
        check(wait_ready(audio,true),"WinMM output did not become ready for the focused frontend window");

        std::int64_t event_ns{};
        check(frontend_menu_event_qpc_ns_v1(event_ns,error),"Actual QPC sample unavailable: "+error);
        FrontendMenuAudioReceiptV1 receipt;
        check(audio.play_authored_menu_action("menu_MainMenu","btn_MENU_SINGLE_PLAYER",
            "onRelease",event_ns,receipt,error),
            "Main-menu Single Player MenuConfirm escaped the sound diagnostic boundary: "+error);
        check(receipt.status==FrontendMenuAudioStatusV1::submitted&&
            receipt.source_ordinal==141&&receipt.xml_sound_uid==2&&receipt.token,
            "Actual main-menu MenuConfirm did not preserve ordinal141 -> UID2 -> token");
        check(wait_started(audio,receipt.token),
            "WinMM did not acknowledge the exact original MenuConfirm sample voice");

        FrontendAuthoredMenuSoundV1 unknown_action;
        check(!resolve_frontend_authored_menu_sound_v1("menu_MainMenu",
            "btn_MENU_SINGLE_PLAYER","onPress",unknown_action),
            "Unrecovered onPress action was incorrectly mapped to a menu sound");
        check(!resolve_frontend_authored_menu_sound_v1("menu_MainMenu",
            "btn_MENU_OPTIONS","onRelease",unknown_action),
            "Another button was incorrectly mapped to the Single Player source action");
        check(audio.play_authored_menu_action("menu_MainMenu","btn_MENU_OPTIONS",
            "onRelease",event_ns,receipt,error)&&
            receipt.status==FrontendMenuAudioStatusV1::unknown_source_name&&receipt.token==0,
            "Unmapped menu action created a blanket click sound");

        check(frontend_menu_event_qpc_ns_v1(event_ns,error),"Actual QPC sample for MenuSelect unavailable: "+error);
        check(audio.play_menu_sound("MenuSelect",event_ns,receipt,error)&&
            receipt.status==FrontendMenuAudioStatusV1::submitted&&receipt.source_ordinal==143&&
            receipt.xml_sound_uid==3&&receipt.token,
            "Recovered menu MenuSelect did not preserve ordinal143 -> UID3 -> token");
        check(wait_started(audio,receipt.token),"WinMM did not acknowledge the exact original MenuSelect sample voice");

        check(audio.play_menu_sound("NoSuchAuthoredSound",event_ns,receipt,error)&&
            receipt.status==FrontendMenuAudioStatusV1::unknown_source_name&&
            receipt.token==0,"Unknown NativePlaySoundFX name did not remain an exact no-op");

        check(audio.window_activity(false,false,error),"Could not publish actual frontend focus loss: "+error);
        check(wait_ready(audio,false),"Retained WinMM output did not pause on actual focus loss");
        check(frontend_menu_event_qpc_ns_v1(event_ns,error),"Actual QPC sample after focus loss unavailable: "+error);
        check(audio.play_menu_sound("MenuConfirm",event_ns,receipt,error)&&
            receipt.status==FrontendMenuAudioStatusV1::skipped_without_focus&&receipt.token==0,
            "Unfocused menu action allocated or played a voice");

        check(audio.window_activity(true,false,error),"Could not publish actual frontend focus regain: "+error);
        check(wait_ready(audio,true),"Same retained WinMM output did not regain focus readiness");
        check(frontend_menu_event_qpc_ns_v1(event_ns,error),"Actual QPC sample after focus regain unavailable: "+error);
        check(audio.play_authored_menu_action("menu_MainMenu","btn_MENU_SINGLE_PLAYER",
            "onRelease",event_ns,receipt,error)&&
            receipt.status==FrontendMenuAudioStatusV1::submitted&&receipt.source_ordinal==141&&
            receipt.xml_sound_uid==2,"Focused main-menu MenuConfirm did not resume on retained output");
        check(wait_started(audio,receipt.token),"Resumed main-menu voice did not reach the WinMM mixer");

        check(audio.play_menu_sound("MenuSelect",0,receipt,error)&&
            receipt.status==FrontendMenuAudioStatusV1::rejected&&receipt.token==0,
            "Invalid authored timestamp created a source voice or vetoed the menu action");
        check(audio.shutdown(error),"Checked menu output close/join/drain failed: "+error);

        const auto missing_path=std::filesystem::path(argv[1])/"data/sounds/sfx_menu_select.wav";
        check(std::filesystem::remove(missing_path),"Could not prepare isolated missing-sample branch");
        FrontendMenuAudioSessionV1 missing_audio;
        check(missing_audio.start(argv[1],true,false,error),"Missing-sample branch output startup failed: "+error);
        check(wait_ready(missing_audio,true),"Missing-sample branch output did not focus");
        check(frontend_menu_event_qpc_ns_v1(event_ns,error),"Actual QPC sample for missing asset branch unavailable: "+error);
        check(missing_audio.play_menu_sound("MenuSelect",event_ns,receipt,error)&&
            receipt.status==FrontendMenuAudioStatusV1::missing_original_asset&&
            receipt.source_ordinal==143&&receipt.xml_sound_uid==3&&receipt.token==0&&
            receipt.detail.find("data/sounds/sfx_menu_select.wav")!=std::string::npos,
            "Missing original MenuSelect asset was substituted or vetoed its caller");
        check(missing_audio.shutdown(error),"Missing-sample branch close/join/drain failed: "+error);
        std::cout<<"PASS FrontendMenuAudioSessionV1: main-menu MenuConfirm ordinal141->UID2 and MenuSelect143->UID3 reached exact original WAVs and real WinMM started receipts; QPC/device-clock scheduling, unknown-name no-op, actual window focus pause/resume, invalid-time no voice, missing exact WAV diagnostic/no substitution, checked close/join/drain; audible parity not measured\n";
        return 0;
    } catch(const std::exception& ex) {std::cerr<<ex.what()<<'\n';return 1;}
}
