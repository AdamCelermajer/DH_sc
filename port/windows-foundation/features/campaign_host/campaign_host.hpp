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
#include "../../original_campaign_world_adapter.hpp"
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

// Facts the host cannot own. main supplies them from the live combat session.
struct CampaignHostServices {
    std::function<bool(std::vector<ActorId>&,std::string&)> all_actors;
    std::function<bool(ActorId,bool& alive,std::int32_t& state,std::string&)> actor_state;
    std::function<bool(ActorId,std::int32_t state,std::string&)> set_actor_state;
    std::int32_t difficulty=0;           // source Normal
    bool tutorials_enabled=true;         // offline normal-difficulty tutorial policy
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

    // Abstract SKIP press. Ignored unless the SKIP control is currently visible.
    void press_skip();
    bool hud_visible() const noexcept { return hud_visible_; }
    bool skip_visible() const noexcept { return skip_visible_; }
    bool skip_active() const noexcept { return skip_pressed_ && skip_visible_; } // value sampled by the next command
    bool cutscene_mode() const noexcept { return cutscene_mode_; }
    bool save_blocked() const noexcept { return save_blocked_; }
    bool global_controller_blocked() const noexcept { return global_blocked_; }

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
