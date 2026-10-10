#pragma once
#include "original_actor_lifecycle.hpp"
#include "campaign_camera_adapter.hpp"
namespace dh::foundation {
struct OriginalTutorialGate {
    bool player_available=false,online=false,enabled=false,settings_menu_open=false;
    int difficulty=-1;
};
struct OriginalCampaignWorldProviders {
    // Source GetObjectByName(name,contextModule,nullptr,create=false) then
    // Character handle conversion. found=false includes non-character handles.
    std::function<bool(const std::string&,int module,ActorId&,bool& found,std::string&)> named_character;
    // Original Script_CharacterState_Callback selector semantics + list order.
    // Reserved source selectors belong here; no actor/map strings are inferred.
    std::function<bool(const std::string&,int module,std::vector<ActorId>&,std::string&)> character_selector;
    std::function<bool(bool blocked,std::string&)> global_controller_blocked;
    std::function<bool(ActorId,bool blocked,std::string&)> character_controller_blocked;
    std::function<bool(ActorId,bool scripted,std::string&)> set_scripted;
    std::function<bool(ActorId,std::string&)> stop_actor;
    // Source callback excludes dead, actual Limbus0 and AwaitingToSpawn17.
    std::function<bool(ActorId,bool& allowed,std::string&)> idle_gate;
    std::function<bool(ActorId,bool wait_for_animation,std::string&)> set_idle;
    std::function<bool(bool entering,std::string&)> cutscene_mode;
    std::function<bool(bool show,const std::string& menu,std::uint32_t duration,bool wait,
                       CampaignCommandPhase,bool& blocking,std::string&)> flash;
    std::function<bool(const OriginalCampaignCommand&,CampaignCommandPhase,bool& blocking,std::string&)> dialog;
    // P14 FAERY (T3): same-owner source Faery script effects (commands 27/28).
    std::function<bool(std::uint32_t slot,std::uint32_t state,std::string&)> set_faery_state;
    std::function<bool(std::uint32_t slot,std::string&)> inc_faery_level;
    std::function<bool(std::string&)> flush_messages,request_save,block_save;
    std::function<bool(int tutorial_id,OriginalTutorialGate&,std::string&)> tutorial_gate;
    std::function<bool(int tutorial_id,std::string&)> consume_tutorial;
    std::function<bool(bool immediately,std::string&)> save_tutorial_settings;
};
class OriginalCampaignWorldAdapter {
public:
    OriginalCampaignWorldAdapter(OriginalActorLifecycle&,CampaignCameraAdapter* camera=nullptr);
    void bind(OriginalCampaignWorldProviders providers) { providers_=std::move(providers); }
    void bind_runtime(OriginalCampaignRuntime& runtime) { runtime_=&runtime; }
    bool command(CampaignCommandPhase,const OriginalCampaignCommand&,int module,
                 bool skip,bool& blocking,std::string& error);
private:
    OriginalActorLifecycle* lifecycle_;
    CampaignCameraAdapter* camera_;
    OriginalCampaignRuntime* runtime_=nullptr;
    OriginalCampaignWorldProviders providers_;
};
}
