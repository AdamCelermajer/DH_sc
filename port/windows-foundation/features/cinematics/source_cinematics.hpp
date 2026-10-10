#pragma once
#include "../../campaign_camera_adapter.hpp"
#include "../../../level-world/canonical_object_manager_v1.hpp"
#include "../../../level-world/gameplay_camera_runtime_v11.hpp"
#include <memory>

namespace dh::foundation {
struct SourceCinematicProviders {
    // All callbacks weakly borrow their world/application. A successful NULL
    // camera is the original absent current-Level/camera branch, not a fallback.
    std::weak_ptr<dh2::world::CanonicalObjectManagerV1> objects;
    std::shared_ptr<CampaignCameraAdapter> targetCamera;
    std::function<bool(std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>&,std::string&)> camera;
    std::function<bool(std::string&)> trace; // actual Debug.load/GetSwitch
    std::function<bool(std::shared_ptr<void>&,std::uint8_t*&,std::string&)> globalControl;
    std::function<bool(std::uintptr_t,std::shared_ptr<void>&,std::uint8_t*&,std::string&)> actorControl;
    std::function<bool(std::uintptr_t,std::int32_t,std::int32_t,std::int32_t,std::string&)> playActor;
    std::function<bool(std::uintptr_t,std::int32_t,bool&,std::string&)> actorBlocking;
    // Scoped SAME receiver fields/methods. Position reads return the live
    // Position160 backing, never a snapshot. Peers are PM GetPlayer(i,false).
    std::function<bool(std::uintptr_t,bool&,std::string&)> isPlayer;
    std::function<bool(int,std::uintptr_t&,std::string&)> peerCharacter;
    std::function<bool(bool&,std::string&)> currentLevel;
    std::function<bool(std::uintptr_t,bool,std::string&)> setIdle;
    std::function<bool(std::uintptr_t,std::shared_ptr<void>&,const float*&,std::string&)> position;
    std::function<bool(std::uintptr_t,const float*,bool,std::string&)> setPosition;
    std::function<bool(std::uintptr_t,std::string&)> forceUpdatePosition;
    std::function<bool(std::uintptr_t,std::shared_ptr<void>&,std::uint8_t*&,std::string&)> controllerForced;
    std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> controllerLook;
    std::function<bool(std::uintptr_t,std::shared_ptr<void>&,const std::uint8_t*&,std::string&)> actorStatic;
    // Whole original GoTo/physical/path body; receives source skip/collision/
    // wait and captured84, returns source retained wait. No linear tween.
    std::function<bool(std::uintptr_t,std::uintptr_t,bool,bool,bool,std::uint8_t&,bool&,std::string&)> moveActor;
    std::function<bool(std::uintptr_t,std::uint8_t,bool&,std::string&)> moveBlocking;
    // Complete original PM/UI/network cutscene effects remain sole-owner services.
    std::function<bool(bool enter,bool skip,int module,std::string&)> cutsceneMode;
};
class SourceCinematicCommands {
public:
    explicit SourceCinematicCommands(SourceCinematicProviders);
    bool command(CampaignCommandPhase,const OriginalCampaignCommand&,int module,
                 bool skip,bool& handled,bool& blocking,std::string&);
    // Source timestamp, not invented elapsed time; root schedules this once.
    bool scene_phase(std::uint32_t sourceTimestamp,std::string&);
private:
    SourceCinematicProviders providers_;
    std::map<const OriginalCampaignCommand*,dh2::target_providers::Handle16> actors_;
    struct MoveState { std::uintptr_t actor=0;std::shared_ptr<void> lease;std::uint8_t originalStatic=0;bool wait=false; };
    std::map<const OriginalCampaignCommand*,MoveState> moves_;
    bool actor_command(CampaignCommandPhase,const OriginalCampaignCommand&,int,bool,bool&,std::string&);
};
enum class CinematicState { ready,running,finished,cancelled,failed };
enum class CinematicInput { skip,cancel };
struct CinematicEvent { CinematicState state; int script=-1,module=-1; };
struct SourceCinematicSessionProviders {
    OriginalCampaignServices campaign; // actual admission/random/remaining effects
    std::shared_ptr<SourceCinematicCommands> commands;
    // Deliver input to actual source skip state and return that same state.
    std::function<bool(CinematicInput,bool& skipActive,std::string&)> input;
    // Host abort/restoration service, required before schedule quarantine.
    // No source ScriptManager stop/reset or automatic unlock is invented here.
    std::function<bool(int script,int module,std::string&)> abortWorld;
    std::function<void(const CinematicEvent&)> event; // observation only
};
// Exclusive, single-use session over a caller-loaded original campaign bank.
// OriginalCampaignRuntime owns command/wait/child state; this adds host lifecycle
// and typed source effects, never a second World/camera/control state copy.
class SourceCinematicSession {
public:
    SourceCinematicSession(std::shared_ptr<OriginalCampaignRuntime>,SourceCinematicSessionProviders);
    bool start(int originalScript,int module,bool received,std::string&);
    bool tick_scripts(std::int32_t sourceDeltaMs,std::string&);
    bool input(CinematicInput,std::string&);
    CinematicState state() const noexcept;
    bool skip_active() const noexcept;
private:
    struct Impl;std::shared_ptr<Impl> impl_;
};
}
