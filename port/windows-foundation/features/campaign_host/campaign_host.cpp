#include "campaign_host.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

namespace dh::foundation::campaign_host {
namespace {

// Source actor states: Limbus0, AwaitingToSpawn17 and Idle3 (original_actor_lifecycle.hpp).
constexpr std::int32_t kStateLimbus = 0;
constexpr std::int32_t kStateIdle = 3;
constexpr std::int32_t kStateAwaitingToSpawn = 17;

bool is_all_selector(const std::string& s) {
    if (s.size() != 3) return false;
    for (std::size_t i = 0; i < 3; ++i) if (std::tolower(static_cast<unsigned char>(s[i])) != "all"[i]) return false;
    return true;
}

bool is_adapter_unsupported(const std::string& error) {
    return error.rfind("Unsupported original campaign command kind", 0) == 0;
}

} // namespace

bool UnsupportedLog::note(const std::string& name) {
    const bool first = counts_.find(name) == counts_.end();
    ++counts_[name];
    if (first) std::cout << "[campaign] unsupported or stubbed (first occurrence): " << name << '\n';
    return first;
}

void UnsupportedLog::print_summary(std::ostream& out) const {
    if (counts_.empty()) { out << "[campaign] unsupported summary: none\n"; return; }
    out << "[campaign] unsupported summary:\n";
    for (const auto& entry : counts_) out << "  " << entry.first << " count=" << entry.second << '\n';
}

CampaignHost::CampaignHost(CampaignHostServices services) : services_(std::move(services)) {}

void CampaignHost::bind_world_providers(OriginalCampaignWorldProviders& p) {
    // Controller locks: main's lambdas keep writing globalControllerBlocked and characterControllerBlocked.
    previous_global_ = p.global_controller_blocked;
    p.global_controller_blocked = [this](bool blocked, std::string& e) {
        if (previous_global_ && !previous_global_(blocked, e)) return false;
        global_blocked_ = blocked;
        return true;
    };
    previous_character_ = p.character_controller_blocked;
    p.character_controller_blocked = [this](ActorId id, bool blocked, std::string& e) {
        if (previous_character_ && !previous_character_(id, blocked, e)) return false;
        if (blocked) character_blocked_.insert(id); else character_blocked_.erase(id);
        return true;
    };

    // Each provider below is filled only when no other owner bound it.
    if (!p.flash) p.flash = [this](bool show, const std::string& menu, std::uint32_t, bool,
                                   CampaignCommandPhase phase, bool& blocking, std::string& e) {
        blocking = false;
        if (phase != CampaignCommandPhase::execute) return true;
        if (menu == "HUD") { hud_visible_ = show; return true; }
        if (menu == "menu_skipcutscene") { skip_visible_ = show; cinematic_.set_skip_visible(show); if (!show) skip_pressed_ = false; return true; } // P16 CINE: drawn by the runner
        unsupported_.note("flash menu " + menu + " (no owner bound)");
        e = "Unsupported flash menu: " + menu;
        return false;
    };
    // P16 CINE: StartDialog (kind 10) queues one caption line: scalar 16 = StrID, 12 = style, 8 = actor (IDA
    // Script_StartDialog::Execute). WaitDialog (kind 12) blocks while any line is queued or shown
    // (IDA Level::hasActiveDialog). Lines are drawn by cinematic_runner; the box art is a placeholder.
    if (!p.dialog) p.dialog = [this](const OriginalCampaignCommand& c, CampaignCommandPhase phase, bool& blocking, std::string&) {
        if (c.kind == 12) { blocking = cinematic_.waiting(); return true; }
        blocking = false;
        if (phase != CampaignCommandPhase::execute) return true;
        const auto field = [&c](unsigned offset) -> std::int32_t {
            const auto i = c.scalars.find(offset);
            return i == c.scalars.end() ? -1 : static_cast<std::int32_t>(i->second);
        };
        cinematic_runner::CaptionLine line;
        line.text_id = field(16);
        line.style = field(12);
        line.actor = field(8);
        std::string resolved, error;
        if (line.text_id >= 0 && caption_text_ && caption_text_(line.text_id, resolved, error)) {
            line.text = resolved;
        } else {
            line.text = "[StrID " + std::to_string(line.text_id) + " unresolved]";
            unsupported_.note("dialogue StrID unresolved");
        }
        cinematic_.enqueue(std::move(line));
        return true;
    };
    // P16 CINE2: PlayCamera (kind 5). Scalar 8 = animations_dictionary id, scalar 12 = blocking flag.
    // IDA Script_PlayCamera::Execute (0x460110) starts CameraLevel::PlayAnim; IsBlocking (0x459370) waits
    // while the flag is set and the camera is still playing. The clip advances in frame(), the same clock.
    if (!p.camera_clip) p.camera_clip = [this](const OriginalCampaignCommand& c, CampaignCommandPhase phase, bool& blocking, std::string& e) {
        const auto field = [&c](unsigned offset) -> std::uint32_t {
            const auto i = c.scalars.find(offset);
            return i == c.scalars.end() ? 0u : i->second;
        };
        if (phase == CampaignCommandPhase::is_blocking) {
            blocking = clip_active_ && field(12) != 0 && static_cast<std::int32_t>(field(8)) == clip_id_;
            return true;
        }
        blocking = false;
        if (phase == CampaignCommandPhase::update) return true;
        // A new PlayAnim replaces the playing clip (CameraLevel owns one animator).
        clip_active_ = false;
        clip_id_ = -1;
        if (skip_active()) {
            // Sampled SKIP: the source plays the level set's own idle clip instead; here the follow camera stays.
            std::cout << "[campaign] PlayCamera skipped (SKIP sampled); follow camera kept\n";
            return true;
        }
        const auto id = static_cast<std::int32_t>(field(8));
        if (!services_.read_camera_clip) {
            e = "Unbound original campaign provider: camera clip bytes";
            return false;
        }
        std::vector<std::uint8_t> bytes, scene;
        std::string path;
        if (!services_.read_camera_clip(id, bytes, scene, path, e)) {
            if (e.empty()) e = "Camera clip read failed";
            return false;
        }
        if (!clip_.load(std::move(scene), std::move(bytes), e)) return false;
        if (!clip_.sample(0, clip_eye_, clip_target_, e)) return false;
        clip_id_ = id;
        clip_elapsed_ms_ = 0;
        clip_active_ = true;
        std::cout << "[campaign] PlayCamera id=" << id << " " << path << " duration_ms=" << clip_.duration_ms()
                  << " blocking=" << (field(12) != 0) << '\n';
        return true;
    };
    if (!p.flush_messages) p.flush_messages = [this](std::string&) {
        cinematic_.flush(); // P16 CINE: FlushMessages drops the queued and shown caption lines
        return true;
    };
    if (!p.request_save) p.request_save = [this](std::string&) {
        unsupported_.note("stub SaveGame (schema v4 writer not bound; nothing written)");
        return true;
    };
    if (!p.block_save) p.block_save = [this](std::string&) { save_blocked_ = true; return true; };
    if (!p.cutscene_mode) p.cutscene_mode = [this](bool entering, std::string&) {
        cutscene_mode_ = entering;
        cinematic_.set_active(entering); // P16 CINE: exit clears lines and the SKIP control
        if (!entering) { skip_pressed_ = false; save_blocked_ = false; } // [inf] the cutscene's own SaveGame ends the block
        return true;
    };
    if (!p.tutorial_gate) p.tutorial_gate = [this](int id, OriginalTutorialGate& gate, std::string&) {
        gate.player_available = true;
        gate.online = false;
        gate.enabled = services_.tutorials_enabled && consumed_tutorials_.count(id) == 0;
        gate.difficulty = services_.difficulty;
        gate.settings_menu_open = false;
        return true;
    };
    if (!p.consume_tutorial) p.consume_tutorial = [this](int id, std::string&) {
        consumed_tutorials_.insert(id);
        unsupported_.note("stub tutorial flag persistence (session only; save schema v4 pending)");
        return true;
    };
    if (!p.save_tutorial_settings) p.save_tutorial_settings = [this](bool, std::string&) {
        unsupported_.note("stub tutorial settings save (nothing written)");
        return true;
    };
    if (!p.character_selector) p.character_selector = [this](const std::string& selector, int, std::vector<ActorId>& out, std::string& e) {
        if (!is_all_selector(selector)) {
            unsupported_.note("character selector " + selector);
            e = "Unsupported source character selector: " + selector;
            return false;
        }
        if (!services_.all_actors) { e = "Character selector All requires the live combat registry"; return false; }
        return services_.all_actors(out, e);
    };
    if (!p.set_scripted) p.set_scripted = [this](ActorId id, bool scripted, std::string&) {
        if (scripted) scripted_.insert(id); else scripted_.erase(id);
        unsupported_.note("scripted flag stored only (no AI consumer bound yet)");
        return true;
    };
    if (!p.stop_actor) p.stop_actor = [this](ActorId, std::string&) {
        unsupported_.note("stub StopActor (movement ends through the controller lock)");
        return true;
    };
    if (!p.idle_gate) p.idle_gate = [this](ActorId id, bool& allowed, std::string& e) {
        if (!services_.actor_state) { e = "PutCharacterInIdle requires the live actor state owner"; return false; }
        bool alive = false;
        std::int32_t state = -1;
        if (!services_.actor_state(id, alive, state, e)) return false;
        allowed = alive && state != kStateLimbus && state != kStateAwaitingToSpawn;
        return true;
    };
    if (!p.set_idle) p.set_idle = [this](ActorId id, bool wait, std::string& e) {
        if (!services_.actor_state || !services_.set_actor_state) { e = "PutCharacterInIdle requires the live actor state owner"; return false; }
        bool alive = false;
        std::int32_t state = -1;
        if (!services_.actor_state(id, alive, state, e)) return false;
        if (state == kStateIdle) return true;
        if (wait) unsupported_.note("approximation: PutCharacterInIdle waitForAnim applied as immediate _SetState(3)");
        return services_.set_actor_state(id, kStateIdle, e);
    };
}

bool CampaignHost::bind_executor(OriginalCampaignRuntime& runtime, OriginalCampaignWorldAdapter& world, std::string& error) {
    if (runtime_) { error = "Campaign host executor is already bound"; return false; }
    SourceCampaignDispatchServicesV1 routes;
    routes.skip = [this](const OriginalCampaignCommand&, bool& skip, std::string&) {
        skip = skip_pressed_ && skip_visible_;
        return true;
    };
    // No SourceCinematicCommands, door or audio owner is bound in this milestone: pass-through to the router.
    routes.cinematics = [](CampaignCommandPhase, const OriginalCampaignCommand&, int, bool, bool& handled, bool& blocking, std::string&) {
        handled = false; blocking = false; return true;
    };
    routes.doors = [](CampaignCommandPhase, const OriginalCampaignCommand&, int, bool, bool& handled, bool& blocking, std::string&) {
        handled = false; blocking = false; return true;
    };
    routes.audio = [](CampaignCommandPhase, const OriginalCampaignCommand&, bool, std::int64_t, bool& handled, bool& blocking, std::string&) {
        handled = false; blocking = false; return true;
    };
    routes.context = [](const OriginalCampaignCommand&, SourceCampaignDispatchContextV1& out, std::string&) {
        out = {}; return true;
    };
    routes.remaining = [this](CampaignCommandPhase phase, const OriginalCampaignCommand& c, int module, bool skip, bool& blocking, std::string& e) {
        return execute_router(phase, c, module, skip, blocking, e);
    };
    owned_dispatch_ = std::make_unique<SourceCampaignDispatchV1>(std::move(routes));
    OriginalCampaignServices existing;
    // Single-player offline: no PM or network admission exists, so a start is always admitted.
    existing.admit_start = [](int, int, bool, bool& admitted, std::string&) { admitted = true; return true; };
    // No RNG owner is bound: ExecScript random arrays fail with an explicit error.
    if (!owned_dispatch_->bind(runtime, std::move(existing), error)) { owned_dispatch_.reset(); return false; }
    runtime_ = &runtime;
    world_ = &world;
    world.bind_runtime(runtime); // P16 CINE: DoTutorial (kind 78) starts its named script through the runtime
    error.clear();
    return true;
}

bool CampaignHost::build_zones(const std::vector<ActorDefinition>& declarations, std::string& error) {
    if (!runtime_) { error = "Campaign host zones require the bound executor"; return false; }
    if (!zones_.build(declarations, *runtime_, error)) return false;
    for (const auto& zone : zones_.zones()) {
        std::cout << "[campaign] trigger zone fed: " << zone.name << " script=" << zone.script << " min="
                  << zone.box.min[0] << ',' << zone.box.min[1] << ',' << zone.box.min[2] << " max="
                  << zone.box.max[0] << ',' << zone.box.max[1] << ',' << zone.box.max[2] << '\n';
    }
    for (const auto& line : zones_.skipped()) std::cout << "[campaign] trigger zone not fed: " << line << '\n';
    std::cout << "[campaign] trigger zones fed=" << zones_.zones().size() << " not-fed=" << zones_.skipped().size() << '\n';
    return true;
}

void CampaignHost::press_skip() {
    if (!skip_visible_) return;
    skip_pressed_ = true;
    std::cout << "[campaign] SKIP pressed (applies to commands sampled after this press)\n";
}

bool CampaignHost::execute_router(CampaignCommandPhase phase, const OriginalCampaignCommand& c, int module, bool skip, bool& blocking, std::string& e) {
    if (!world_ || !runtime_) { e = "Campaign host executor is not bound"; return false; }
    if (phase == CampaignCommandPhase::execute) {
        int index = -1;
        const std::string script = script_name_of(c, index);
        std::cout << "[campaign] command script=" << script << " index=" << index << " kind=" << c.kind
                  << " class=" << c.class_name << (skip ? " skip=1" : "") << '\n';
    }
    const bool ok = world_->command(phase, c, module, skip, blocking, e);
    if (!ok && phase == CampaignCommandPhase::execute && is_adapter_unsupported(e))
        unsupported_.note("kind " + std::to_string(c.kind) + " " + c.class_name + " (no owner bound)");
    return ok;
}

std::string CampaignHost::script_name_of(const OriginalCampaignCommand& c, int& index) const {
    index = -1;
    for (const auto& script : runtime_->scripts()) {
        for (std::size_t i = 0; i < script.commands.size(); ++i) {
            if (&script.commands[i] == &c) { index = static_cast<int>(i); return script.name; }
        }
    }
    return "<unknown>";
}

CameraPose CampaignHost::source_camera_pose(const CameraPose& follow) const {
    if (!clip_active_) return follow;
    CameraPose pose = follow; // up and FOV stay with the follow camera (see header)
    pose.position = clip_eye_;
    pose.target = clip_target_;
    return pose;
}

bool CampaignHost::advance_camera_clip(std::int32_t dt_ms, std::string& error) {
    error.clear();
    if (!clip_active_) return true;
    clip_elapsed_ms_ += std::max<std::int32_t>(0, dt_ms);
    const auto duration = clip_.duration_ms();
    if (clip_elapsed_ms_ > duration) clip_elapsed_ms_ = duration;
    if (!clip_.sample(clip_elapsed_ms_, clip_eye_, clip_target_, error)) return false;
    if (clip_elapsed_ms_ >= duration) {
        // CameraLevel::__Callback: completion clears the playing flag; the follow camera resumes.
        clip_active_ = false;
        std::cout << "[campaign] PlayCamera finished id=" << clip_id_ << " frame=" << frames_ << '\n';
    }
    return true;
}

bool CampaignHost::abort_cutscene(std::string& error) {
    clip_active_ = false; // P16 CINE2: abort ends a playing camera clip (follow camera resumes)
    cinematic_.set_active(false); // P16 CINE: abort drops caption lines and the SKIP control
    hud_visible_ = true;
    skip_visible_ = false;
    skip_pressed_ = false;
    cutscene_mode_ = false;
    save_blocked_ = false;
    bool ok = true;
    if (global_blocked_ && previous_global_ && !previous_global_(false, error)) ok = false;
    global_blocked_ = false;
    if (previous_character_) {
        for (const auto id : character_blocked_) if (!previous_character_(id, false, error)) ok = false;
    }
    character_blocked_.clear();
    if (!ok) return false;
    error.clear();
    return true;
}

void CampaignHost::frame(std::int32_t dt_ms, const std::array<float,3>& player, bool qualified) {
    if (!runtime_) return;
    // Diagnostic trace (trigger mode only): first frame, then every 30 frames.
    if (!reported_player_ || frames_ % 30 == 0) {
        reported_player_ = true;
        std::cout << "[campaign] frame=" << frames_ << " player position=" << player[0] << ',' << player[1] << ',' << player[2]
                  << " qualified=" << qualified << '\n';
    }
    ++frames_;
    cinematic_.update(dt_ms > 0 ? static_cast<std::uint32_t>(dt_ms) : 0u); // P16 CINE: caption hold, same clock as the executor
    std::string error;
    // P16 CINE2: the camera clip advances on the same clock before the executor reads is_blocking.
    if (!advance_camera_clip(dt_ms, error) || !runtime_->tick(dt_ms, error)) {
        // Policy: the failing cutscene is aborted, the session keeps its other triggers.
        // The runtime failure is cleared BEFORE contacts are fed, so one bad script cannot disable zones.
        ++aborts_;
        last_error_ = error;
        std::cout << "[campaign] cutscene aborted: " << error << '\n';
        std::string restore_error;
        const auto abandoned = runtime_->abandon_running_scripts();
        if (abort_cutscene(restore_error))
            std::cout << "[campaign] HUD, SKIP, cutscene mode, save block and controller locks restored; " << abandoned << " running script(s) abandoned\n";
        else
            std::cout << "[campaign] restore incomplete: " << restore_error << '\n';
    }
    zones_.update(*runtime_, player, qualified, -1);
}

void CampaignHost::print_summary(std::ostream& out) const {
    if (aborts_) out << "[campaign] cutscenes aborted=" << aborts_ << " last error: " << last_error_ << '\n';
    unsupported_.print_summary(out);
}

} // namespace dh::foundation::campaign_host
