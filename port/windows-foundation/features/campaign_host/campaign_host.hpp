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
#include "../cinematics/script_source_services_v1.hpp" // v20 SCRIPTSVC: PlaySound/StopSound/SaveGame orchestration
#include "../cinematic_runner/cinematic_runner.hpp" // P16 CINE
#include "../cinematic_runner/dialog_tables.hpp" // B068
#include "../../original_campaign_world_adapter.hpp"
#include "../../original_camera_clip.hpp" // P16 CINE2: PlayCamera clips
#include "../../camera.hpp"
#include "../../playable_actor_world.hpp" // D3 (OPENING3): campaign lifecycle component
#include <array>
#include <cstdint>
#include <filesystem>
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
    // Script_PlayEffect keeps the authored local offset separate and passes the
    // exact named GameObject identity to the FX anchor provider.
    std::function<bool(const std::string& name,int module,std::uintptr_t& identity,bool& found,std::string& e)> resolve_fx_anchor;
    std::function<bool(const std::string& name,int module,std::array<float,3>& position,bool& found,std::string& e)> waypoint_position;
    std::function<bool(ActorId,std::array<float,3>& position,std::string& e)> position_of;
    std::function<bool(ActorId,std::array<float,3> position,std::string& e)> teleport;
    std::function<bool(ActorId,std::array<float,3> target,std::string& e)> face;
    std::function<bool(ActorId,bool visible,std::string& e)> set_visible;
    std::function<bool(ActorId,std::array<float,3>,bool warp,bool ignore_collisions,std::string&)> move_to;
    std::function<bool(ActorId,bool& blocking,std::string&)> move_blocking;
    std::function<bool(ActorId,std::string&)> kill;
    std::function<bool(ActorId,std::string& e)> put_limbus;
    // Plays the animations_dictionary clip on the actor (no loop). duration_ms = authored clip range.
    std::function<bool(ActorId,std::int32_t dictionary_id,std::int32_t& duration_ms,std::string& e)> play_clip;
    // Script_PlayEffect / Script_StopEffect: VisualFXManager::PlayAnimFXSet(set, waypoint + offsets) and StopAnimFXSet (the set
    // pool, not a stored handle). Failures are logged by the host and do not stop the cutscene (the source ignores the result).
    std::function<bool(std::int32_t set,const std::array<float,3>& position,std::uintptr_t anchor,std::string& e)> play_effect;
    std::function<bool(std::int32_t set,std::string& e)> stop_effect;
    // P16 OPENING4: Script_PlayAnimByName (kind 19): the named scene object's visual plays the clip (not blocking).
    // found=false when no scene object of that name is instantiated (logged once, the cutscene continues).
    std::function<bool(const std::string& object,const std::string& clip,bool& found,std::string& e)> play_object_clip;
    // P16 OPENING4: UnEquipHands (kind 51, equipped=false) and ReEquipHands (kind 52, equipped=true) on the actor
    // (IDA Script_UnEquipHands / Script_ReEquipHands: the hand slots 1 and 2 of the actor's inventory).
    std::function<bool(ActorId id,bool equipped,std::string& e)> hands;
};

// Facts the host cannot own. main supplies them from the live combat session.
struct CampaignHostServices {
    // P17 CONDITIONS: answers trigger zone activate_cond names (main binds the authored evaluator). Unset: gated zones get no contact.
    std::function<bool(const std::string&)> condition_active;
    // Source TriggerZone::Update door predicate. `suppressed=true` means the named Door's
    // IsDoorClosedActivated returned true and contact/inside state must not advance.
    std::function<bool(const std::string&,bool& suppressed,std::string&)> door_gate_suppressed;
    std::function<bool(std::vector<ActorId>&,std::string&)> all_actors;
    std::function<bool(ActorId,bool& alive,std::int32_t& state,std::string&)> actor_state;
    std::function<bool(ActorId,std::int32_t state,std::string&)> set_actor_state;
    std::function<bool(ActorId,bool wait_for_animation,std::string&)> set_idle;
    std::function<bool(ActorId)> scripted_actor_pose;
    // B091: source CSAnim Idle admission. Script_PlayActorAnim sets the un-scripted OnUpdate flag (+1345).
    // CSAnim::OnUpdate (0x3c1a3c) can then call Idle when Character's scripted flag (+5248) clears, even before a clip ends.
    // Completed clips remain pending while scripted; actual Unset also admits a current unfinished clip and cancels its follow.
    // The owner decides
    // which actors it applies to (gameplay actors); released=true drops the held cutscene pose.
    std::function<bool(ActorId,bool& released,std::string&)> scripted_clip_finished;
    std::int32_t difficulty=0;           // source Normal
    bool tutorials_enabled=true;         // offline normal-difficulty tutorial policy
    // B070: the original keeps the 14 tutorial-enabled bytes in the GLOBAL settings file (SavegameManager +0x29,
    // all 1 by _initSettings; DoTutorial/LockTutorial clear one and save the settings), not in a per-character save.
    // Empty path = session only (tests, no settings location).
    std::filesystem::path tutorial_settings_path;
    // P16 CINE2: resolves a PlayCamera dictionary id to clip bytes, the level camera scene bytes and the path
    // (main binds CameraClipLibrary).
    std::function<bool(std::int32_t,std::vector<std::uint8_t>&,std::vector<std::uint8_t>&,std::string&,std::string&)> read_camera_clip;
    ActorVerbServices actor_verbs; // P16 OPENING: actor show/hide/look/move and PlayActorAnim owners (main binds)
    // P17 DOORS: Script_OpenDoor / Script_CloseDoor (kinds 54/55) through the existing routes.doors slot.
    // Unset = pass-through (no door owner bound), as before.
    std::function<bool(CampaignCommandPhase,const OriginalCampaignCommand&,int module,bool skip,bool& handled,bool& blocking,std::string&)> doors;
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
    // B068: DialogActors rows (speaker name plate) and DialogStyles names (box variant) of the original dialogs tables.
    void set_dialog_tables(std::vector<cinematic_runner::DialogActorRow> actors, std::map<std::int32_t, std::string> styles) {
        dialog_actors_ = std::move(actors);
        dialog_styles_ = std::move(styles);
    }
    // P17 SAFEZONE: the music owner for Script_EnterSafeZone (true) and Script_LeaveSafeZone (false). Without it the
    // command still updates the host state (safe_zone()), and the log records the transition.
    void bind_safe_zone(std::function<bool(bool entering,std::string&)> music) { safe_zone_music_=std::move(music); }
    bool safe_zone() const noexcept { return safe_zone_; }
    // P16 OPENING: scripted FX owners (PlayEffect/StopEffect). main binds them once the effects factory exists; the host
    // calls them at command time, so the bound functions must read the current owner.
    void bind_fx(std::function<bool(std::int32_t,const std::array<float,3>&,std::uintptr_t,std::string&)> play,
                 std::function<bool(std::int32_t,std::string&)> stop) {
        services_.actor_verbs.play_effect = std::move(play);
        services_.actor_verbs.stop_effect = std::move(stop);
    }
    // P16 CINE: cinematic presentation state and draw description (authored 480x320 space).
    const cinematic_runner::CinematicRunner& cinematic() const noexcept { return cinematic_; }
    bool cinematic_skip_hit(float x,float y,float window_w,float window_h) const noexcept { return cinematic_.skip_hit(x,y,window_w,window_h); }
    // OPENING2: a player tap advances a tap-wait caption (btn_next). Returns true when a caption used it.
    bool caption_tap() { return cinematic_.tap(); }
    // Verification input (--caption-auto-tap-ms): taps tap-wait captions after this many ms. 0 = player only.
    void set_caption_auto_tap_ms(std::uint32_t ms) { cinematic_.set_auto_tap_ms(ms); }

    // Abstract SKIP press. Ignored unless the SKIP control is currently visible. SKIP16: the press is a one-shot
    // fast-forward of every running script at the next frame (IDA ScriptManager::SkipScript): queued captions are
    // dropped at once, the scripts run their state changes without waiting, and they end at their own End commands.
    void press_skip();
    bool skip_pending() const noexcept { return skip_requested_; }
    bool hud_visible() const noexcept { return hud_visible_; }
    bool skip_visible() const noexcept { return skip_visible_; }
    bool skip_active() const noexcept { return skipping_; } // value sampled by every command during the fast-forward
    // SKIP16: the host-owned state that a finished cutscene leaves behind (played and skipped runs must match).
    struct HostState {
        bool hud_visible = true;
        bool skip_visible = false;
        bool cutscene_mode = false;
        bool cinematic_active = false;
        bool save_blocked = false;
        bool global_blocked = false;
        bool camera_clip = false;
        std::size_t actor_clips = 0;
        std::set<ActorId> character_blocked;
        std::set<ActorId> scripted;
        std::set<int> consumed_tutorials;
        bool safe_zone = false;
        std::uint64_t aborts = 0;
    };
    HostState host_state() const;
    bool cutscene_mode() const noexcept { return cutscene_mode_; }
    // B068: SetCameraClip planes; 0 = use the level config (camera_znear / camera_zfar).
    float camera_near_override() const noexcept { return camera_near_override_; }
    float camera_far_override() const noexcept { return camera_far_override_; }
    // B068: a scripted PlayActorAnim clip owns the actor's pose (playing, or finished and held) until PutCharacterInIdle: the per-frame
    // locomotion selection of the local player must not replace it (the original CharStateMachine stays in the scripted anim state).
    bool scripted_clip_pose(ActorId id) const noexcept { return held_clip_actors_.count(id)!=0&&(!services_.scripted_actor_pose||services_.scripted_actor_pose(id)); }
    // TriggerZone::Update 0x39b458 ignores contacts while the local character is marked scripted (+5248).
    bool character_scripted(ActorId id) const noexcept { return scripted_.count(id) != 0; }
    bool save_blocked() const noexcept { return save_blocked_; }
    void set_save_blocked(bool blocked) noexcept { save_blocked_ = blocked; } // SG_BlockLevel(false) clears the cutscene block
    // v20 SCRIPTSVC: Script_PlaySound/StopSound (kinds 13/14) and Script_SaveGame (kind 69) owners. Unbound = logged stub.
    void bind_script_sound(cinematics::ScriptSoundServicesV1 s) { script_sound_ = std::move(s); }
    void bind_script_save(cinematics::ScriptSaveServicesV1 s) { script_save_ = std::move(s); }
    bool global_controller_blocked() const noexcept { return global_blocked_; }

    // P16 CINE2: scripted camera clip. While a PlayCamera clip plays, the source camera uses its eye and
    // target (up and FOV from the follow camera); otherwise the follow pose is returned unchanged.
    bool camera_clip_active() const noexcept { return clip_active_; }
    CameraPose source_camera_pose(const CameraPose& follow,CameraVec3 rig_position={}) const;

    // One frame: advances the executor on the caller clock, then feeds trigger contacts.
    // An executor error aborts only that cutscene: flags are restored and running scripts abandoned.
    void frame(std::int32_t dt_ms,const std::array<float,3>& player,bool qualified,ActorId local_player=invalid_actor_id);
    bool enabled() const noexcept { return runtime_ != nullptr; }
    // Diagnostic-only one-shot execute through the same typed dispatch used
    // by the bound campaign runtime. This keeps command harnesses on the
    // production owner route (doors, FX, actor verbs) without starting a
    // second scheduler or changing trigger admission.
    bool execute_diagnostic_command(const OriginalCampaignCommand&, int module,
                                    bool& blocking, std::string& error);
    // D3 (OPENING3): campaign lifecycle through the save component mechanism. A neutral world object (fixed ID, bound
    // at level load like the containers) carries component campaign_lifecycle_v1 (trigger once-activation counts).
    // Persist before a GameSave capture; restore after restore_game_save. Quest states already live in CQPG.
    bool bind_lifecycle_object(PlayableActorWorld& world, std::string& error);
    bool persist_lifecycle(PlayableActorWorld& world, std::string& error) const;
    bool restore_lifecycle(const PlayableActorWorld& world, std::string& error);
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
    bool tutorials_loaded_=false;
    void load_tutorial_settings();           // B070: reads consumed_tutorials_ from services_.tutorial_settings_path
    bool save_tutorial_settings(std::string&); // B070: writes it back (atomic replace)
    cinematic_runner::CinematicRunner cinematic_; // P16 CINE
    std::function<bool(std::int32_t,std::string&,std::string&)> caption_text_; // P16 CINE
    std::vector<cinematic_runner::DialogActorRow> dialog_actors_; // B068
    std::map<std::int32_t, std::string> dialog_styles_;           // B068
    OriginalCameraClip clip_;                     // P16 CINE2: PlayCamera owner (one clip at a time, as CameraLevel)
    bool clip_active_=false;
    bool clip_hold_=false; // B068: a finished clip keeps its last pose (CameraLevel::__Callback only clears the playing flag) until the next clip / the End contract
    std::int32_t clip_id_=-1;
    std::int32_t clip_elapsed_ms_=0;
    CameraVec3 clip_eye_{}, clip_target_{};
    OriginalCameraClip::View clip_view_{}; // P16 OPENING4: up and FOV of the clip (the follow values when the clip has none)
    bool advance_camera_clip(std::int32_t dt_ms, std::string& error); // P16 CINE2
    // P16 OPENING: actor verbs. A PlayActorAnim clip blocks only when its wait flag (scalar 28) is set (IDA
    // Script_PlayActorAnim::IsBlocking); its chained clip (scalar 12) starts when the first one ends.
    struct ActorClipState { std::int32_t remaining_ms=0; std::int32_t follow_dictionary=-1; bool idle_admitted_by_unset=false; };
    std::map<ActorId,ActorClipState> actor_clips_;
    float camera_near_override_ = 0.f, camera_far_override_ = 0.f; // B068
    std::set<ActorId> held_clip_actors_; // B068: actors whose scripted clip pose is held until PutCharacterInIdle / the End contract
    bool actor_verb(const OriginalCampaignCommand& c, CampaignCommandPhase phase, int module, bool& blocking, std::string& e);
    bool advance_actor_clips(std::int32_t dt_ms, std::string& error);
    std::set<std::string> actor_verb_unresolved_;
    bool hud_visible_=true;
    bool skip_visible_=false;
    bool skip_requested_=false; // SKIP16: pressed, fast-forward pending for the next frame
    bool skipping_=false;       // SKIP16: the fast-forward pass is running (sampled by every command)
    bool cutscene_mode_=false;
    bool safe_zone_=false; // P17 SAFEZONE: Enter/Leave state (IDA VoxSoundManager in-safe-zone flag)
    std::function<bool(bool,std::string&)> safe_zone_music_;
    int cutscene_depth_=0; // OPENING2: nested enters of cutscene mode
    bool save_blocked_=false;
    cinematics::ScriptSoundServicesV1 script_sound_; // v20 SCRIPTSVC (PlaySound/StopSound owners)
    cinematics::ScriptSaveServicesV1 script_save_;   // v20 SCRIPTSVC (regular SaveGame owners)
    bool global_blocked_=false;
    bool reported_player_=false;
    std::uint64_t frames_=0;
    std::uint64_t aborts_=0;
    std::string last_error_;

    bool execute_router(CampaignCommandPhase,const OriginalCampaignCommand&,int module,bool skip,bool& blocking,std::string&);
    bool abort_cutscene(std::string& error);
    bool fast_forward(std::string& error); // SKIP16
    bool restore_presentation(std::string& error); // SKIP16: the End contract's host side (HUD, locks, captions, clips)
    std::string script_name_of(const OriginalCampaignCommand&,int& index) const;
};

} // namespace dh::foundation::campaign_host
