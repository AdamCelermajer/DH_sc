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

// P16 OPENING: source command fields (offsets from the decoded script bodies; -1 / empty when absent).
std::int32_t signed_field(const OriginalCampaignCommand& c, unsigned offset) {
    const auto i = c.scalars.find(offset);
    return i == c.scalars.end() ? -1 : static_cast<std::int32_t>(i->second);
}

std::string string_field(const OriginalCampaignCommand& c, unsigned offset) {
    const auto i = c.strings.find(offset);
    return i == c.strings.end() ? std::string() : i->second;
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
    // (IDA Level::hasActiveDialog). Lines are drawn by cinematic_runner (original dialog box art and timing).
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
        std::cout << "[campaign] caption frame=" << frames_ << " strid=" << line.text_id << " actor=" << line.actor << " text=" << line.text << '\n';
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
    // P16 OPENING: actor show/hide/look/move, PlayActorAnim and PutCharacterInLimbus (+ logged stubs for the
    // verbs without an owner, see actor_verb).
    if (!p.actor_verb) p.actor_verb = [this](const OriginalCampaignCommand& c, CampaignCommandPhase phase, int module, bool& blocking, std::string& e) {
        return actor_verb(c, phase, module, blocking, e);
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
        // OPENING2: cutscene mode nests. A tutorial that ends inside the opening must not restore the HUD and the
        // controller while the opening is still running, so the mode ends when the last enter has been exited.
        if (entering) ++cutscene_depth_;
        else if (cutscene_depth_ > 0) --cutscene_depth_;
        const bool active = cutscene_depth_ > 0;
        cutscene_mode_ = active;
        cinematic_.set_active(active); // P16 CINE: exit clears lines and the SKIP control
        if (!active) { skip_pressed_ = false; save_blocked_ = false; } // [inf] the cutscene's own SaveGame ends the block
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
    // P16 OPENING: any other selector is a source character NAME (GetObjectByName, as Script_CharacterState_Callback
    // resolves it): one actor when the level declares it, none (a no-op) when it does not.
    if (!p.character_selector) p.character_selector = [this](const std::string& selector, int module, std::vector<ActorId>& out, std::string& e) {
        if (is_all_selector(selector)) {
            if (!services_.all_actors) { e = "Character selector All requires the live combat registry"; return false; }
            return services_.all_actors(out, e);
        }
        out.clear();
        if (!services_.actor_verbs.resolve_actor) { e = "Unbound original campaign provider: actor lookup"; return false; }
        ActorId id = invalid_actor_id;
        bool found = false;
        if (!services_.actor_verbs.resolve_actor(selector, module, id, found, e)) return false;
        if (found) out.push_back(id);
        return true;
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
                  << " class=" << c.class_name << (skip ? " skip=1" : "") << " frame=" << frames_ << '\n';
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
    actor_clips_.clear(); // P16 OPENING: abort ends scripted actor clip timers (the pose stays as the clip left it)
    cinematic_.set_active(false); // P16 CINE: abort drops caption lines and the SKIP control
    hud_visible_ = true;
    skip_visible_ = false;
    skip_pressed_ = false;
    cutscene_mode_ = false;
    cutscene_depth_ = 0;
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

bool CampaignHost::actor_verb(const OriginalCampaignCommand& c, CampaignCommandPhase phase, int module, bool& blocking, std::string& e) {
    blocking = false;
    const auto& v = services_.actor_verbs;
    const auto note_unresolved = [this](const std::string& name) {
        if (actor_verb_unresolved_.insert(name).second)
            std::cout << "[campaign] actor name not in the loaded level (command is a no-op): " << name << '\n';
    };
    // Source GetObjectByName: found=false is a no-op for that command, an error is explicit.
    const auto resolve = [&](const std::string& name, ActorId& id, bool& found, std::string& error) {
        found = false;
        if (!v.resolve_actor) { error = "Unbound original campaign provider: actor lookup"; return false; }
        if (!v.resolve_actor(name, module, id, found, error)) return false;
        if (!found) note_unresolved(name);
        return true;
    };
    const auto unbound = [&e](const char* what) { e = std::string("Unbound original campaign provider: ") + what; return false; };
    ActorId id = invalid_actor_id;
    bool found = false;
    switch (c.kind) {
    case 45: { // Script_PlayActorAnim: actor @24, clip @8 (dictionary), chained clip @12 (none = -1), wait flag @28
        if (phase == CampaignCommandPhase::is_blocking) {
            if (signed_field(c, 28) <= 0) return true;
            if (!resolve(string_field(c, 24), id, found, e)) return false;
            const auto clip = found ? actor_clips_.find(id) : actor_clips_.end();
            blocking = clip != actor_clips_.end() && clip->second.remaining_ms > 0;
            return true;
        }
        if (phase != CampaignCommandPhase::execute) return true;
        if (!resolve(string_field(c, 24), id, found, e)) return false;
        if (!found) return true;
        if (!v.play_clip) return unbound("actor clip");
        std::int32_t duration = 0;
        if (!v.play_clip(id, signed_field(c, 8), duration, e)) return false;
        ActorClipState state;
        state.remaining_ms = duration;
        state.follow_dictionary = signed_field(c, 12);
        actor_clips_[id] = state;
        std::cout << "[campaign] PlayActorAnim actor=" << id << " clip=" << signed_field(c, 8) << " duration_ms=" << duration
                  << " wait=" << signed_field(c, 28) << '\n';
        return true;
    }
    case 46: { // Script_SetActorPosition: actor @20 moves to waypoint @12 (IDA: SetPosition(waypoint, true))
        if (phase != CampaignCommandPhase::execute) return true;
        if (!resolve(string_field(c, 20), id, found, e)) return false;
        if (!found) return true;
        std::array<float,3> position{};
        bool placed = false;
        const auto waypoint = string_field(c, 12);
        if (!v.waypoint_position) return unbound("waypoint position");
        if (!v.waypoint_position(waypoint, module, position, placed, e)) return false;
        if (!placed) { note_unresolved(waypoint); return true; }
        if (!v.teleport) return unbound("actor position");
        return v.teleport(id, position, e);
    }
    case 41: { // Script_LookActor: actor @20 turns to target @12 (HighestThreatPlayer = the local player)
        if (phase != CampaignCommandPhase::execute) return true;
        if (!resolve(string_field(c, 20), id, found, e)) return false;
        if (!found) return true;
        auto target = string_field(c, 12);
        if (target == "HighestThreatPlayer") target = "LocalPlayer";
        std::array<float,3> position{};
        // A named actor (the player or a declared character) is the target first; otherwise a placed waypoint.
        ActorId other = invalid_actor_id;
        bool targetIsActor = false;
        if (!v.resolve_actor) return unbound("actor lookup");
        if (!v.resolve_actor(target, module, other, targetIsActor, e)) return false;
        if (targetIsActor) {
            if (!v.position_of) return unbound("actor position");
            if (!v.position_of(other, position, e)) return false;
        } else {
            bool placed = false;
            if (!v.waypoint_position) return unbound("waypoint position");
            if (!v.waypoint_position(target, module, position, placed, e)) return false;
            if (!placed) { note_unresolved(target); return true; }
        }
        if (!v.face) return unbound("actor facing");
        return v.face(id, position, e);
    }
    case 42:   // Script_ShowActor: actor @12, GameObject vtable+64(true)
    case 43: { // Script_HideActor: actor @12, GameObject vtable+64(false)
        if (phase != CampaignCommandPhase::execute) return true;
        if (!resolve(string_field(c, 12), id, found, e)) return false;
        if (!found) return true;
        if (!v.set_visible) return unbound("actor visibility");
        return v.set_visible(id, c.kind == 42, e);
    }
    case 29: { // Script_PutCharacterInLimbus: actor @16 (the lifecycle Limbus record)
        if (phase != CampaignCommandPhase::execute) return true;
        if (!resolve(string_field(c, 16), id, found, e)) return false;
        if (!found) return true;
        if (!v.put_limbus) return unbound("put character in limbus");
        actor_clips_.erase(id);
        return v.put_limbus(id, e);
    }
    // Verbs without an owner in this host: explicit, counted, non-blocking (the cutscene keeps running).
    // OPENING2: Script_CONSOLE (kind 3) shows its text only when the DisplayScriptConsoleAsDialog debug switch is on;
    // the release default is off (IDA Script_CONSOLE::Execute), so the command does nothing and never blocks.
    case 3:  if (phase == CampaignCommandPhase::execute) unsupported_.note("Script_CONSOLE is a debug command (no-op: DisplayScriptConsoleAsDialog is off)"); return true;
    case 6:  if (phase == CampaignCommandPhase::execute) unsupported_.note("stub SetCameraClip (camera transition tuning not decoded)"); return true;
    case 19: if (phase == CampaignCommandPhase::execute) unsupported_.note("stub PlayAnimByName (object clips for scene objects are not bound)"); return true;
    case 20: { // Script_PlayEffect: set @8 at the position of the object @32 plus the authored offsets @16/@20/@24 (IDA Script_PlayEffect::Execute)
        if (phase != CampaignCommandPhase::execute) return true;
        const auto waypoint = string_field(c, 32);
        std::array<float,3> position{};
        bool placed = false;
        if (!v.waypoint_position) return unbound("waypoint position");
        if (!v.waypoint_position(waypoint, module, position, placed, e)) return false;
        if (!placed) { note_unresolved(waypoint); return true; }
        for (std::size_t axis = 0; axis < 3; ++axis) position[axis] += static_cast<float>(signed_field(c, 16 + 4 * static_cast<unsigned>(axis)));
        if (!v.play_effect) { unsupported_.note("stub PlayEffect (no FX owner bound)"); return true; }
        if (!v.play_effect(signed_field(c, 8), position, e)) { unsupported_.note("PlayEffect set failed: " + e); e.clear(); }
        return true;
    }
    case 21: { // Script_StopEffect: set @8
        if (phase != CampaignCommandPhase::execute) return true;
        if (!v.stop_effect) { unsupported_.note("stub StopEffect (no FX owner bound)"); return true; }
        if (!v.stop_effect(signed_field(c, 8), e)) { unsupported_.note("StopEffect set failed: " + e); e.clear(); }
        return true;
    }
    case 14: if (phase == CampaignCommandPhase::execute) unsupported_.note("stub StopSound (scripted sound stop not bound)"); return true;
    case 51: if (phase == CampaignCommandPhase::execute) unsupported_.note("stub UnEquipHands (equipment visuals not switched)"); return true;
    case 52: if (phase == CampaignCommandPhase::execute) unsupported_.note("stub ReEquipHands (equipment visuals not switched)"); return true;
    default: e = "Unsupported original campaign actor verb kind " + std::to_string(c.kind); return false;
    }
}

bool CampaignHost::advance_actor_clips(std::int32_t dt_ms, std::string& error) {
    error.clear();
    const auto step = std::max<std::int32_t>(0, dt_ms);
    for (auto it = actor_clips_.begin(); it != actor_clips_.end();) {
        if (it->second.remaining_ms > 0) it->second.remaining_ms -= step;
        if (it->second.remaining_ms > 0) { ++it; continue; }
        const auto id = it->first;
        const auto chained = it->second.follow_dictionary;
        it = actor_clips_.erase(it);
        if (chained < 0) continue;
        // The chained clip (scalar 12) follows the first one; its own wait is not set, so it does not block.
        if (!services_.actor_verbs.play_clip) { error = "Unbound original campaign provider: actor clip"; return false; }
        ActorClipState next;
        if (!services_.actor_verbs.play_clip(id, chained, next.remaining_ms, error)) return false;
        actor_clips_[id] = next;
    }
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
    if (!advance_camera_clip(dt_ms, error) || !advance_actor_clips(dt_ms, error) || !runtime_->tick(dt_ms, error)) {
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
