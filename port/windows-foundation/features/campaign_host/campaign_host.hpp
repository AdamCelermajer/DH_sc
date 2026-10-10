#pragma once
// P16 HOST: live script-host providers for the original campaign executor (HUD, SKIP, cutscene
// mode, controller locks, save block, dialogue/message stubs, tutorial gate, idle/scripted state,
// wait on the caller's clock) plus the trigger-zone set. Off unless main enables --campaign-triggers.
//
// Contract:
// - An unbound or unsupported provider returns an explicit error that names it. Nothing succeeds silently.
// - Stubs that only log (dialogue, message flush, save request, tutorial persistence) succeed and are
//   listed in the unsupported log with their occurrence counts. They never block.
// - The first occurrence of each unsupported name is printed at once; print_summary() lists name and count.
// - Controller locks are not owned here: the existing main flags (globalControllerBlocked and
//   characterControllerBlocked) stay the single truth; this host wraps the lambdas to track and restore them.
#include "trigger_zones.hpp"
#include "../cinematics/source_campaign_dispatch_v1.hpp"
#include "../cinematic_runner/cinematic_runner.hpp" // P16 CINE
#include "../../original_campaign_world_adapter.hpp"
#include "../../original_camera_clip.hpp" // P16 CINE2: PlayCamera clips
#include "../../camera.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <iosfwd>
#include <memory>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace dh::foundation::campaign_host {

class UnsupportedLog {
public:
    // Returns true only for the first occurrence of name (caller prints the one-time line).
    bool note(const std::string& name);
    const std::map<std::string,std::uint64_t>& counts() const noexcept { return counts_; }
    void print_summary(std::ostream& out) const;
private:
    std::map<std::string,std::uint64_t> counts_;
};

// P16 OPENING: owners of the actor script verbs (IDA Script_SetActorPosition::Execute, Script_LookActor::Execute
// (0x45ec50), Script_ShowActor::Execute (0x45eb14), Script_HideActor::Execute, Script_PlayActorAnim::Execute (0x45e890),
// Script_PutCharacterInLimbus::Execute; see OPENING-report.md for the decoded fields). main binds them to
// the live session, the population, the lifecycle and the source object registry. Unbound services fail explicitly.
struct ActorVerbServices {
    // found=false (with true) means the name is not in the loaded level: the source does nothing for that command.
    std::function<bool(const std::string& name,int module,ActorId& id,bool& found,std::string& e)> resolve_actor;
    std::function<bool(const std::string& name,int module,std::array<float,3>& position,bool& found,std::string& e)> waypoint_position;
    std::function<bool(ActorId,std::array<float,3>& position,std::string& e)> position_of;
    std::function<bool(ActorId,std::array<float,3> position,std::string& e)> teleport;
    std::function<bool(ActorId,std::array<float,3> target,std::string& e)> face;
    std::function<bool(ActorId,bool visible,std::string& e)> set_visible;
    std::function<bool(ActorId,std::string& e)> put_limbus;
    // Plays the animations_dictionary clip on the actor (no loop). duration_ms = authored clip range.
    std::function<bool(ActorId,std::int32_t dictionary_id,std::int32_t& duration_ms,std::string& e)> play_clip;
};

// Facts the host cannot own. main supplies them from the live combat session.
struct CampaignHostServices {
    std::function<bool(std::vector<ActorId>&,std::string&)> all_actors;
    std::function<bool(ActorId,bool& alive,std::int32_t& state,std::string&)> actor_state;
    std::function<bool(ActorId,std::int32_t state,std::string&)> set_actor_state;
    std::int32_t difficulty=0;           // source Normal
    bool tutorials_enabled=true;         // offline normal-difficulty tutorial policy
    // P16 CINE2: resolves a PlayCamera dictionary id to clip bytes, the level camera scene bytes and the path
    // (main binds CameraClipLibrary).
    std::function<bool(std::int32_t,std::vector<std::uint8_t>&,std::vector<std::uint8_t>&,std::string&,std::string&)> read_camera_clip;
    ActorVerbServices actor_verbs; // P16 OPENING: actor show/hide/look/move and PlayActorAnim owners (main binds)
};

class CampaignHost {
public:
    explicit CampaignHost(CampaignHostServices services);

    // Fills every provider that is still empty; wraps the controller-lock providers already bound by main.
    void bind_world_providers(OriginalCampaignWorldProviders& providers);

    // Installs the executor services (admission, command router, no RNG) through SourceCampaignDispatchV1.
    bool bind_executor(OriginalCampaignRuntime& runtime,OriginalCampaignWorldAdapter& world,std::string& error);

    // Builds trigger zones from the loaded level declarations (any map). Call after bind_executor.
    bool build_zones(const std::vector<ActorDefinition>& declarations,std::string& error);

    // P16 CINE: StrID resolver for caption lines (main binds the original MenuLocalization after its load).
    // Without it a caption line shows an explicit "[StrID n unresolved]" marker.
    void set_caption_text(std::function<bool(std::int32_t,std::string&,std::string&)> resolver) { caption_text_=std::move(resolver); }
    // P16 CINE: cinematic presentation state and draw description (authored 480x320 space).
    const cinematic_runner::CinematicRunner& cinematic() const noexcept { return cinematic_; }
    bool cinematic_skip_hit(float x,float y,float window_w,float window_h) const noexcept { return cinematic_.skip_hit(x,y,window_w,window_h); }

    // Abstract SKIP press. Ignored unless the SKIP control is currently visible.
    void press_skip();
    bool hud_visible() const noexcept { return hud_visible_; }
    bool skip_visible() const noexcept { return skip_visible_; }
    bool skip_active() const noexcept { return skip_pressed_ && skip_visible_; } // value sampled by the next command
    bool cutscene_mode() const noexcept { return cutscene_mode_; }
    bool save_blocked() const noexcept { return save_blocked_; }
    bool global_controller_blocked() const noexcept { return global_blocked_; }

    // P16 CINE2: scripted camera clip. While a PlayCamera clip plays, the source camera uses its eye and
    // target (up and FOV from the follow camera); otherwise the follow pose is returned unchanged.
    bool camera_clip_active() const noexcept { return clip_active_; }
    CameraPose source_camera_pose(const CameraPose& follow) const;

    // One frame: advances the executor on the caller clock, then feeds trigger contacts.
    // An executor error aborts only that cutscene: flags are restored and running scripts abandoned.
    void frame(std::int32_t dt_ms,const std::array<float,3>& player,bool qualified);
    bool enabled() const noexcept { return runtime_ != nullptr; }
    // Number of cutscenes aborted by an executor error (the session itself keeps running).
    std::uint64_t aborts() const noexcept { return aborts_; }
    const TriggerZoneSet& zones() const noexcept { return zones_; }
    const UnsupportedLog& unsupported() const noexcept { return unsupported_; }
    void print_summary(std::ostream& out) const;

private:
    CampaignHostServices services_;
    OriginalCampaignRuntime* runtime_=nullptr;
    OriginalCampaignWorldAdapter* world_=nullptr;
    std::unique_ptr<SourceCampaignDispatchV1> owned_dispatch_;
    TriggerZoneSet zones_;
    UnsupportedLog unsupported_;
    std::function<bool(bool,std::string&)> previous_global_;
    std::function<bool(ActorId,bool,std::string&)> previous_character_;
    std::set<ActorId> character_blocked_;
    std::set<ActorId> scripted_;
    std::set<int> consumed_tutorials_;
    cinematic_runner::CinematicRunner cinematic_; // P16 CINE
    std::function<bool(std::int32_t,std::string&,std::string&)> caption_text_; // P16 CINE
    OriginalCameraClip clip_;                     // P16 CINE2: PlayCamera owner (one clip at a time, as CameraLevel)
    bool clip_active_=false;
    std::int32_t clip_id_=-1;
    std::int32_t clip_elapsed_ms_=0;
    CameraVec3 clip_eye_{}, clip_target_{};
    bool advance_camera_clip(std::int32_t dt_ms, std::string& error); // P16 CINE2
    // P16 OPENING: actor verbs. A PlayActorAnim clip blocks only when its wait flag (scalar 28) is set (IDA
    // Script_PlayActorAnim::IsBlocking); its chained clip (scalar 12) starts when the first one ends.
    struct ActorClipState { std::int32_t remaining_ms=0; std::int32_t follow_dictionary=-1; };
    std::map<ActorId,ActorClipState> actor_clips_;
    bool actor_verb(const OriginalCampaignCommand& c, CampaignCommandPhase phase, int module, bool& blocking, std::string& e);
    bool advance_actor_clips(std::int32_t dt_ms, std::string& error);
    std::set<std::string> actor_verb_unresolved_;
    bool hud_visible_=true;
    bool skip_visible_=false;
    bool skip_pressed_=false;
    bool cutscene_mode_=false;
    bool save_blocked_=false;
    bool global_blocked_=false;
    bool reported_player_=false;
    std::uint64_t frames_=0;
    std::uint64_t aborts_=0;
    std::string last_error_;

    bool execute_router(CampaignCommandPhase,const OriginalCampaignCommand&,int module,bool skip,bool& blocking,std::string&);
    bool abort_cutscene(std::string& error);
    std::string script_name_of(const OriginalCampaignCommand&,int& index) const;
};

} // namespace dh::foundation::campaign_host
