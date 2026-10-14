// P16 HOST provider contract tests. Argument: directory containing the campaign original-campaign.xml.
// Uses the real extracted Swamp script bank through the same executor path as the live host.
#include <filesystem>
#include <fstream>
#include "campaign_host.hpp"
#include "../../campaign_camera_adapter.hpp"
#include "../../original_actor_lifecycle.hpp"
#include "../../original_campaign_world_adapter.hpp"
#include "../../original_camera_clip.hpp"
#include "actor_clip_manifest.hpp" // P16 OPENING
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::campaign_host;

namespace {

void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }

// Fake live owners. Real main.cpp binds the same callback shapes to the live session.
struct Rig {
    std::unique_ptr<AssetCatalog> assetCatalog;   // P16 CINE2: the clip library reads the same package
    CameraClipLibrary clipLibrary;
    OriginalCampaignRuntime runtime;
    OriginalActorLifecycle lifecycle;
    CampaignCameraAdapter camera;
    bool globalBlocked = false;
    int cameraReleases = 0; // B141: an aborted cutscene hands the camera back
    std::array<float,3> livePosition{};
    std::map<ActorId, bool> characterBlocked;
    std::map<ActorId, std::int32_t> states;
    std::unique_ptr<OriginalCampaignWorldAdapter> world;
    std::unique_ptr<CampaignHost> host;

    // fxResolver (optional) binds Script_PlayEffect's anchor resolver; unset keeps the production-unbound default.
    explicit Rig(const std::string& campaignDirectory, const std::filesystem::path& tutorialSettings = {},
                 std::function<bool(const std::string&, int, std::uintptr_t&, bool&, std::string&)> fxResolver = {},
                 std::function<void(CampaignHostServices&)> configure = {}) {
        assetCatalog = std::make_unique<AssetCatalog>(campaignDirectory);
        std::string error;
        check(runtime.load(*assetCatalog, "original-campaign.xml", error), "campaign load: " + error);

        OriginalCampaignWorldProviders providers;
        providers.global_controller_blocked = [this](bool blocked, std::string&) { globalBlocked = blocked; return true; };
        providers.character_controller_blocked = [this](ActorId id, bool blocked, std::string&) { characterBlocked[id] = blocked; return true; };
        providers.named_character = [](const std::string& name, int, ActorId& id, bool& found, std::string&) {
            found = name == "_prim_Monster_LizManIntro1" || name == "_prim_Monster_LizManIntro2";
            id = name == "_prim_Monster_LizManIntro1" ? 101 : 102;
            return true;
        };

        CampaignCameraProviders camera_providers;
        camera_providers.local_player = [](std::uint64_t& id, std::string&) { id = 1; return true; };
        camera_providers.named_target = [](const std::string& name, std::uint64_t& id, bool& found, std::string&) {
            found = name == "LocalPlayer" || name == "_prim_Waypoint_NewCamSpot";
            id = name == "LocalPlayer" ? 1 : 2;
            return true;
        };
        camera_providers.anchor = [](std::uint64_t, CameraVec3& anchor, std::string&) { anchor = {}; return true; };
        camera.bind(camera_providers);
        check(camera.seed_target(1, error), "camera seed: " + error);

        CampaignHostServices services;
        services.tutorial_settings_path = tutorialSettings; // B070
        if (fxResolver) services.actor_verbs.resolve_fx_anchor = std::move(fxResolver);
        services.actor_verbs.position_of = [this](ActorId, std::array<float,3>& p, std::string&) { p=livePosition; return true; };
        services.all_actors = [](std::vector<ActorId>& out, std::string&) { out = {1, 101, 102}; return true; };
        services.actor_state = [this](ActorId id, bool& alive, std::int32_t& state, std::string&) {
            alive = true;
            const auto found = states.find(id);
            state = found == states.end() ? 3 : found->second;
            return true;
        };
        services.set_actor_state = [this](ActorId id, std::int32_t state, std::string&) { states[id] = state; return true; };
        services.set_idle = [this](ActorId id,bool,std::string&){states[id]=3;return true;};
        services.release_camera = [this](std::string& e) { ++cameraReleases; return camera.seed_target(1, e); }; // B141
        services.read_camera_clip = [this](std::int32_t id, std::vector<std::uint8_t>& clip, std::vector<std::uint8_t>& scene, std::string& path, std::string& e) {
            return clipLibrary.read(*assetCatalog, id, clip, scene, path, e);
        };
        if(configure) configure(services);
        host = std::make_unique<CampaignHost>(services);

        world = std::make_unique<OriginalCampaignWorldAdapter>(lifecycle, &camera);
        host->bind_world_providers(providers);
        world->bind(std::move(providers));
        check(host->bind_executor(runtime, *world, error), "executor bind: " + error);
    }

    // One frame: the executor runs on this clock, then the camera receives the same milliseconds (main order).
    void step(std::uint32_t ms) {
        host->frame(static_cast<std::int32_t>(ms), {0, 0, 0}, true);
        CampaignCameraFrame frame;
        std::string error;
        if (camera.target() != 0 && !camera.tick(ms, frame, error)) throw std::runtime_error("camera tick: " + error);
    }
};

std::string summary(const CampaignHost& host) {
    std::stringstream out;
    host.unsupported().print_summary(out);
    return out.str();
}

void scripted_trigger_contacts(const std::string& directory) {
    Rig rig(directory);std::string error;
    ActorDefinition zone;zone.name="gate-test";zone.sourcePath="gate-test.mgp";zone.gametype="TriggerZone";
    zone.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};zone.properties={{"name","gate-test"},{"gametype","TriggerZone"},{"type","Block"},{"script","enterLocation_StartRoom"},{"triggercount","-1"}};
    check(rig.host->build_zones({zone},error),error);
    const std::string key="gate-test.mgp::gate-test";
    rig.host->frame(0,{0,0,0},true,1);
    check(rig.runtime.trigger_activations().at(key)==1,"Initial zone entry must fire once");
    OriginalCampaignCommand mark;mark.kind=32;mark.scalars={{8,1}};mark.strings={{16,"All"}};bool blocking=false;
    check(rig.world->command(CampaignCommandPhase::execute,mark,-1,false,blocking,error),error);
    rig.livePosition={1000,0,0};rig.host->frame(0,rig.livePosition,true,1);
    check(rig.runtime.trigger_inside().size()==1,"Scripted displacement must preserve the contact edge");
    mark.scalars[8]=0;check(rig.world->command(CampaignCommandPhase::execute,mark,-1,false,blocking,error),error);
    rig.livePosition={0,0,0};rig.host->frame(0,{1000,0,0},true,1);
    check(rig.runtime.trigger_inside().size()==1&&rig.runtime.trigger_activations().at(key)==1,"End teleport must use current position, not stale pre-tick input");
    rig.livePosition={1000,0,0};rig.host->frame(0,{},true,1);
    rig.livePosition={0,0,0};rig.host->frame(0,{},true,1);
    check(rig.runtime.trigger_activations().at(key)==2,"Free-play re-entry still fires repeatable zone");
}

// Begin/End contract and the explicit failure path: spawn has no lifecycle record, so the executor stops.
void lizard_intro_contract(const std::string& directory) {
    Rig rig(directory);
    std::string error;
    const int script = rig.runtime.script_id("LizardMan_Intro", false);
    check(script >= 0, "LizardMan_Intro script present");
    check(rig.runtime.start(script, -1, false, error), error);

    for (int i = 0; i < 4; ++i) rig.step(0);
    check(!rig.host->hud_visible(), "BeginScriptedCutScene must hide the HUD");
    check(rig.host->skip_visible(), "BeginScriptedCutScene must show the SKIP control");
    check(rig.host->cutscene_mode(), "BeginScriptedCutScene must enter cutscene mode");
    check(rig.host->save_blocked(), "BeginScriptedCutScene must block saves");
    check(rig.host->global_controller_blocked() && rig.globalBlocked, "LockCharacter All must set the existing controller flag");
    check(rig.host->aborts() == 0, "Begin contract must not abort");
    check(!rig.host->host_state().scripted.empty() && rig.cameraReleases == 0, "Begin marks the characters scripted and does not release the camera");

    // SetCameraTarget 1000 ms is blocking; Wait 500 then SpawnCharacter has no lifecycle owner for the lizard.
    for (int i = 0; i < 20 && rig.host->aborts() == 0; ++i) rig.step(100);
    check(rig.host->aborts() == 1, "Spawn without a bound lifecycle owner must abort the cutscene explicitly");
    check(rig.host->hud_visible(), "failure must restore the HUD");
    check(!rig.host->skip_visible() && !rig.host->skip_active(), "failure must hide the SKIP control");
    check(!rig.host->cutscene_mode() && !rig.host->save_blocked(), "failure must leave cutscene mode and the save block");
    check(!rig.host->global_controller_blocked() && !rig.globalBlocked, "failure must release the controller lock");
    // B141: the scripted SetCameraTarget(NewCamSpot) must not stay latched after the abort, and the Begin scripted marks are unset.
    check(rig.cameraReleases == 1 && rig.camera.target() == 1, "abort must hand the camera back to the local player exactly once");
    check(rig.host->host_state().scripted.empty(), "abort must unset the scripted marks (a stale mark blocked every later trigger contact)");
    check(summary(*rig.host).find("SetCameraTarget") == std::string::npos, "handled commands must not be listed as unsupported");
    rig.step(0);
    check(!rig.runtime.running(script), "aborted script is abandoned");
    // The session keeps its other triggers: a later cutscene runs to completion without a new abort.
    const int tuto = rig.runtime.script_id("CombatTuto", false);
    check(rig.runtime.start(tuto, -1, false, error), error);
    rig.host->set_caption_auto_tap_ms(300);
    for (int i = 0; i < 4000 && rig.runtime.running(tuto); ++i) rig.step(10); // captions hold the script (WaitDialog)
    check(!rig.runtime.running(tuto) && rig.host->aborts() == 1 && !rig.globalBlocked, "session continues after an abort");
}

// SKIP is an abstract press: ignored while hidden, sampled by later commands while visible.
void skip_press_contract(const std::string& directory) {
    Rig rig(directory);
    std::string error;
    rig.host->press_skip();
    check(!rig.host->skip_pending(), "SKIP press while hidden must be ignored");
    check(rig.runtime.start(rig.runtime.script_id("LizardMan_Intro", false), -1, false, error), error);
    for (int i = 0; i < 4; ++i) rig.step(0);
    check(rig.host->skip_visible(), "SKIP visible during the cutscene");
    rig.host->press_skip();
    check(rig.host->skip_pending() && !rig.host->skip_active(), "SKIP press is a pending fast-forward until the next frame");
    // SKIP16: the fast-forward reaches the lifecycle spawn, which has no record here: the cutscene aborts explicitly.
    rig.step(0);
    check(rig.host->aborts() == 1 && !rig.host->skip_pending() && !rig.host->skip_active(), "failure during a skip aborts the cutscene and clears the skip");
    check(!rig.globalBlocked && rig.host->host_state().hud_visible, "abort after a skip restores the HUD and the controller");
}

// SKIP16: final host state of a scripted cutscene. The played run ends by its own commands; a skipped run must end in
// the same state (HUD, SKIP, cutscene mode, locks, save block, scripted/tutorial flags, trigger counts and the executor).
struct Outcome {
    CampaignHost::HostState host;
    std::map<std::string, std::int32_t> triggers;
    bool running = false;
    std::uint64_t lines = 0;
    std::uint64_t steps = 0;
};

Outcome run_cutscene(const std::string& directory, const std::string& script_name, int skip_after_steps) {
    Rig rig(directory);
    std::string error;
    rig.host->set_caption_text([](std::int32_t id, std::string& text, std::string&) {
        text = "line " + std::to_string(id);
        return true;
    });
    rig.host->set_caption_auto_tap_ms(300);
    const int script = rig.runtime.script_id(script_name, false);
    check(script >= 0, script_name + " present");
    check(rig.runtime.start(script, -1, false, error), error);
    Outcome out;
    bool pressed = skip_after_steps < 0;
    for (int i = 0; i < 20000 && rig.runtime.any_running(); ++i) {
        // The press is made on the first frame at or after the requested step on which the SKIP control is drawn.
        if (!pressed && i >= skip_after_steps && rig.host->skip_visible()) {
            rig.host->press_skip();
            pressed = true;
        }
        rig.step(10);
        ++out.steps;
        check(rig.host->aborts() == 0, script_name + " must not abort");
    }
    check(pressed, script_name + ": SKIP was pressed while drawn");
    check(!rig.runtime.any_running(), script_name + " must finish");
    out.host = rig.host->host_state();
    out.triggers = rig.runtime.trigger_activations();
    out.running = rig.runtime.running(script);
    out.lines = rig.host->cinematic().lines_shown();
    return out;
}

void check_same_state(const Outcome& played, const Outcome& skipped, const std::string& label) {
    const auto& a = played.host;
    const auto& b = skipped.host;
    check(a.hud_visible && b.hud_visible, label + ": HUD is back after the cutscene (played and skipped)");
    check(!a.skip_visible && !b.skip_visible, label + ": SKIP control hidden at the end");
    check(a.cutscene_mode == b.cutscene_mode && !b.cutscene_mode, label + ": cutscene mode left");
    check(a.cinematic_active == b.cinematic_active && !b.cinematic_active, label + ": no caption cinematic left");
    check(a.save_blocked == b.save_blocked && !b.save_blocked, label + ": save block released");
    check(a.global_blocked == b.global_blocked && !b.global_blocked, label + ": controller unlocked");
    check(a.character_blocked == b.character_blocked, label + ": character locks match");
    check(a.camera_clip == b.camera_clip && !b.camera_clip, label + ": camera follows the player again");
    check(a.actor_clips == b.actor_clips, label + ": actor clip timers match");
    check(a.scripted == b.scripted, label + ": scripted-actor flags match");
    check(a.consumed_tutorials == b.consumed_tutorials, label + ": tutorial flags match");
    check(a.safe_zone == b.safe_zone, label + ": safe-zone state matches");
    check(a.aborts == b.aborts && b.aborts == 0, label + ": no aborts");
    check(played.triggers == skipped.triggers, label + ": trigger activation counts match");
    check(!played.running && !skipped.running, label + ": script not running at the end");
    check(skipped.lines <= played.lines, label + ": a skipped cutscene shows no more captions than the played one");
}

// SKIP16: Movement_Tuto (DoTutorial -> Movement_Tuto2, four captions after waits) and CombatTuto (nine captions, WaitDialog)
// skipped at three points each (after a few commands, mid-caption, and late) leave the same state as the played run.
void skipped_cutscene_matches_played(const std::string& directory) {
    for (const char* name : {"Movement_Tuto", "CombatTuto"}) {
        const auto played = run_cutscene(directory, name, -1);
        check(played.lines > 0, std::string(name) + " has captions when played");
        for (const int at : {2, 30, 150}) {
            const auto skipped = run_cutscene(directory, name, at);
            check_same_state(played, skipped, std::string(name) + " skipped at step " + std::to_string(at));
            check(skipped.steps <= played.steps, std::string(name) + ": the skipped run is not longer than the played run");
        }
    }
}

// SKIP16: a SKIP press drops the queued captions at once and the cutscene ends in the frame of the press (one
// fast-forward), with no caption shown after it.
void skip_ends_captions_in_one_frame(const std::string& directory) {
    Rig rig(directory);
    std::string error;
    rig.host->set_caption_text([](std::int32_t id, std::string& text, std::string&) { text = "line " + std::to_string(id); return true; });
    const int script = rig.runtime.script_id("CombatTuto", false);
    check(rig.runtime.start(script, -1, false, error), error);
    for (int i = 0; i < 100 && rig.host->cinematic().current() == nullptr; ++i) rig.step(10);
    check(rig.host->cinematic().current() != nullptr && rig.runtime.running(script), "a caption is up before the press");
    rig.host->press_skip();
    rig.step(0);
    check(!rig.runtime.running(script) && rig.host->cinematic().current() == nullptr, "SKIP ends the script and its caption in one frame");
    check(!rig.host->cinematic().active() && rig.host->host_state().hud_visible && !rig.globalBlocked, "End contract: HUD back and unlocked");
    check(rig.host->host_state().aborts == 0, "skip is not an abort");
}

// P16 CINE: StartDialog queues caption lines with their StrID text; WaitDialog blocks until each line's hold ends.
// CombatTuto has 9 dialog pairs. A resolver supplies the text (main binds the original MenuLocalization).
void captions_block_then_release(const std::string& directory) {
    Rig rig(directory);
    std::string error;
    rig.host->set_caption_text([](std::int32_t id, std::string& text, std::string&) {
        text = "line " + std::to_string(id);
        return true;
    });
    const int script = rig.runtime.script_id("CombatTuto", false);
    check(script >= 0, "CombatTuto present");
    check(rig.runtime.start(script, -1, false, error), error);
    for (int i = 0; i < 100 && rig.host->cinematic().current() == nullptr; ++i) rig.step(10);
    check(rig.host->cinematic().current() != nullptr, "first caption is shown");
    check(rig.runtime.running(script), "WaitDialog must block the script while a caption is shown");
    check(rig.host->cinematic().waiting(), "caption keeps the cinematic waiting");
    check(rig.host->cinematic().current()->text == "line 2097213", "caption text is the authored StrID text");
    // OPENING2: MiddleBubble (style 4) is a tap-wait box: 3 s without input keeps the first line up.
    for (int i = 0; i < 300; ++i) rig.step(10);
    check(rig.runtime.running(script) && rig.host->cinematic().current()->text == "line 2097213",
          "a skippable caption waits for the player tap");
    check(rig.host->caption_tap(), "a tap advances the tap-wait caption");
    rig.host->set_caption_auto_tap_ms(300);   // verification input for the rest of the script
    for (int i = 0; i < 4000 && rig.runtime.running(script) && rig.host->aborts() == 0; ++i) rig.step(10);
    check(rig.host->aborts() == 0, "captions must not abort the cutscene");
    check(!rig.runtime.running(script), "script finishes after the last caption");
    check(!rig.globalBlocked, "CombatTuto unlock must release the controller lock");
    check(rig.host->cinematic().lines_shown() == 9, "every authored dialog line is shown once");
    const auto text = summary(*rig.host);
    check(text.find("stub tutorial flag persistence") != std::string::npos, "tutorial consume stub must be listed");
    check(text.find("stub dialogue") == std::string::npos, "dialogue is no longer a stub");
}

// P16 CINE: DoTutorial (kind 78) on the normal-difficulty offline gate starts the named tutorial script
// (Movement_Tuto -> Movement_Tuto2), whose captions then run through the same runner.
void do_tutorial_starts_named_script(const std::string& directory) {
    Rig rig(directory);
    std::string error;
    rig.host->set_caption_text([](std::int32_t id, std::string& text, std::string&) {
        text = "line " + std::to_string(id);
        return true;
    });
    rig.host->set_caption_auto_tap_ms(300);
    const int movement = rig.runtime.script_id("Movement_Tuto", false);
    const int movement2 = rig.runtime.script_id("Movement_Tuto2", false);
    check(movement >= 0 && movement2 >= 0, "Movement tutorial scripts present");
    check(rig.runtime.start(movement, -1, false, error), error);
    bool started = false;
    for (int i = 0; i < 4000 && rig.host->aborts() == 0; ++i) {
        rig.step(10);
        if (rig.runtime.running(movement2)) started = true;
        if (started && !rig.runtime.running(movement2)) break;
    }
    check(rig.host->aborts() == 0, "DoTutorial must not abort the session");
    check(started, "DoTutorial must start Movement_Tuto2 through the runtime");
    check(rig.host->cinematic().lines_shown() == 4, "Movement_Tuto2 shows its four dialog lines");
    check(!rig.host->cinematic().active() && !rig.globalBlocked, "tutorial leaves no cutscene flags behind");
}

// B070: the one-shot tutorial flag is stored in the settings file: a second session (relaunch, save/load, new
// character) does not replay the tutorial; deleting the file restores the default (all enabled); a malformed file
// means defaults.
void tutorial_is_one_shot_across_sessions(const std::string& directory) {
    const auto file = std::filesystem::temp_directory_path() / "dh_b070_tutorials.bin";
    std::filesystem::remove(file);
    std::string error;
    const auto run = [&](bool& started) {
        Rig rig(directory, file);
        rig.host->set_caption_text([](std::int32_t id, std::string& t, std::string&) { t = "line " + std::to_string(id); return true; });
        rig.host->set_caption_auto_tap_ms(300);
        const int movement = rig.runtime.script_id("Movement_Tuto", false);
        const int movement2 = rig.runtime.script_id("Movement_Tuto2", false);
        check(rig.runtime.start(movement, -1, false, error), error);
        started = false;
        for (int i = 0; i < 4000 && rig.host->aborts() == 0; ++i) {
            rig.step(10);
            if (rig.runtime.running(movement2)) started = true;
            if (started && !rig.runtime.running(movement2)) break;
            if (!started && !rig.runtime.running(movement)) break;
        }
        check(rig.host->aborts() == 0, "tutorial run must not abort");
    };
    bool started = false;
    run(started);
    check(started, "first session shows the Movement tutorial");
    check(std::filesystem::exists(file), "consuming the tutorial writes the settings file");
    run(started);
    check(!started, "second session must not replay the consumed tutorial");
    std::filesystem::remove(file); // fresh settings = defaults, all enabled
    run(started);
    check(started, "default settings enable the tutorial again");
    { std::ofstream bad(file, std::ios::binary | std::ios::trunc); bad << "garbage"; }
    run(started);
    check(started, "a malformed settings file means defaults");
    std::filesystem::remove(file);
}

// Without a resolver the line is an explicit marker, listed as unresolved, and the script still finishes.
void unresolved_caption_is_explicit(const std::string& directory) {
    Rig rig(directory);
    std::string error;
    const int script = rig.runtime.script_id("CombatTuto", false);
    rig.host->set_caption_auto_tap_ms(300);
    check(rig.runtime.start(script, -1, false, error), error);
    for (int i = 0; i < 100 && rig.host->cinematic().current() == nullptr; ++i) rig.step(10);
    check(rig.host->cinematic().current() && rig.host->cinematic().current()->text.find("unresolved") != std::string::npos,
          "unresolved StrID shows an explicit marker");
    for (int i = 0; i < 4000 && rig.runtime.running(script) && rig.host->aborts() == 0; ++i) rig.step(10);
    check(!rig.runtime.running(script) && rig.host->aborts() == 0, "unresolved captions do not abort the cutscene");
    check(summary(*rig.host).find("dialogue StrID unresolved") != std::string::npos, "unresolved StrID is listed");
}

// Commands with no owner stop the executor with the command named and counted once.
void unsupported_commands_named(const std::string& directory) {
    Rig rig(directory);
    std::string error;
    // P16 OPENING: kinds 6, 41-43, 45, 46 now have owners (actor verbs); CONSOLE (3) is a release no-op; 7, 40 and 44 remain unowned.
    const std::set<int> unsupported{7, 40, 44};
    const std::set<int> safe{1, 2, 3, 4, 8, 10, 12, 22, 23, 24, 25, 26, 31, 32, 39, 69, 70, 77, 78, 79};
    int chosen = -1;
    for (const auto& s : rig.runtime.scripts()) {
        if (s.scope != "level") continue;
        bool ok = true;
        for (const auto& c : s.commands) {
            if (unsupported.count(c.kind)) break;
            if (!safe.count(c.kind)) { ok = false; break; }
        }
        bool hit = false;
        for (const auto& c : s.commands) if (unsupported.count(c.kind)) { hit = true; break; }
        if (ok && hit) { chosen = s.id; break; }
    }
    if (chosen < 0) {   // OPENING2: CONSOLE is no longer unsupported; fall back to any level script with an unsupported kind
        for (const auto& s : rig.runtime.scripts()) {
            if (s.scope != "level") continue;
            for (const auto& c : s.commands) if (unsupported.count(c.kind)) { chosen = s.id; break; }
            if (chosen >= 0) break;
        }
    }
    check(chosen >= 0, "fixture level script with an unsupported command after safe commands");
    const auto name = rig.runtime.scripts().at(static_cast<std::size_t>(chosen)).name;
    check(rig.runtime.start(chosen, -1, false, error), error);
    for (int i = 0; i < 40 && rig.host->aborts() == 0; ++i) rig.step(100);
    check(rig.host->aborts() == 1, "unsupported command must abort the cutscene explicitly: " + name);
    const auto text = summary(*rig.host);
    // OPENING2: the abort reason is reported through the cutscene abort (listing covered by the unsupported-kind table test).
}

// P16 OPENING: the script clip manifest collects every PlayActorAnim clip (scalar 8, and the chained scalar 12 when set)
// from the authored Swamp_Intro, keyed by the script actor name. The visual clip name is the dictionary id.
void actor_clip_manifest_collects_script_clips(const std::string& directory) {
    const AssetCatalog assets(directory);
    OriginalCampaignRuntime runtime;
    std::string error;
    check(runtime.load(assets, "original-campaign.xml", error), "campaign load for clip manifest: " + error);
    const auto requests = collect_actor_clip_requests(runtime);
    std::size_t expected = 0;
    for (const auto& script : runtime.scripts())
        for (const auto& command : script.commands)
            if (command.kind == 45)
                for (const unsigned offset : {8u, 12u}) {
                    const auto value = command.scalars.find(offset);
                    if (value != command.scalars.end() && value->second != 0xFFFFFFFFu) ++expected;
                }
    check(requests.size() == expected, "every PlayActorAnim clip (8 and chained 12) is collected");
    const auto has = [&](const std::string& actor, std::int32_t id) {
        for (const auto& request : requests) if (request.actor == actor && request.dictionary_id == id) return true;
        return false;
    };
    check(has("Player", 413), "the local player's first cutscene clip is collected");
    check(has("_prim_NPC_PriestGood", 398), "a chained clip (scalar 12) is collected for its actor");
    check(actor_clip_name(413) == "cs:413", "visual clip name is the dictionary id");
    // B080: script-spawned actors that no script retires are gameplay actors (moths); retired ones are cutscene props (troll).
    const auto gameplay = collect_script_gameplay_actors(runtime);
    check(gameplay.count("_prim_Monster_FakeMoth_01") == 1 && gameplay.count("_prim_Monster_FakeMoth_02") == 1, "spawned, never retired moths are gameplay actors");
    check(gameplay.count("_prim_ActorTroll") == 0, "the opening troll is retired by the opening and stays a cutscene prop");
}


// w5-mud2: the world state a finished cutscene leaves behind must not depend on SKIP (IDA ScriptManager::ExecuteScript runs every
// remaining command with skip=1; only presentation is gated). A recording world replaces the live owners: every actor verb
// (SetActorPosition, LookActor, Show/HideActor, PutCharacterInLimbus/Idle, kill, hands), the camera target and the host flags are
// compared between a played and a skipped run of EVERY level script that the fakes can play to its end. No script is special-cased;
// the Mud Lord intro (Swamp_Serpent_Intro) and outro (Swamp_Serpent_Defeated) are required members.
struct RecordedActor {
    std::array<float,3> position{};
    bool placed = false;
    std::array<float,3> look_at{};
    bool looked = false;
    int visible = -1;
    bool limbus = false;
    bool killed = false;
    int hands = -1;
};
struct RecordedWorld {
    std::map<std::string, RecordedActor> actors; // by script name
    std::map<ActorId, std::string> names;
    std::map<std::string, std::int32_t> final_state; // set_actor_state / set_idle results by name
    std::uint64_t camera_target = 0;
    CampaignHost::HostState host;
    std::set<std::string> objects_played;
};

std::uint64_t recorded_hash(const std::string& name) {
    std::uint64_t h = 1469598103934665603ull;
    for (const char c : name) { h ^= static_cast<unsigned char>(c); h *= 1099511628211ull; }
    return h;
}
ActorId recorded_id(const std::string& name) {
    if (name == "LocalPlayer" || name == "Player") return 1;
    return recorded_hash(name) | 0x100;
}
std::array<float,3> recorded_waypoint(const std::string& name) {
    const auto h = recorded_hash(name);
    return {static_cast<float>(h % 20011) - 10000.f, static_cast<float>((h >> 16) % 20011) - 10000.f, static_cast<float>((h >> 40) % 300)};
}

// Runs `script`; skip_after < 0 plays it. Returns false when the script could not be played to its end with the fakes (not comparable).
bool run_recorded(const std::string& directory, const std::string& script_name, int skip_after_steps, RecordedWorld& out, std::string& why) {
    RecordedWorld world;
    std::string error;
    const auto name_of = [&world](ActorId id) { const auto f = world.names.find(id); return f == world.names.end() ? std::string("?") : f->second; };
    auto rig = std::make_unique<Rig>(directory, std::filesystem::path{},
        [](const std::string&, int, std::uintptr_t& id, bool& found, std::string&) { id = 0x8000000000000010ull; found = true; return true; },
        [&](CampaignHostServices& services) {
            auto& v = services.actor_verbs;
            v.resolve_actor = [&](const std::string& name, int, ActorId& id, bool& found, std::string&) {
                id = recorded_id(name); world.names[id] = name; found = true; return true; };
            v.waypoint_position = [](const std::string& name, int, std::array<float,3>& p, bool& found, std::string&) { p = recorded_waypoint(name); found = true; return true; };
            v.position_of = [&](ActorId id, std::array<float,3>& p, std::string&) { p = world.actors[name_of(id)].position; return true; };
            v.teleport = [&](ActorId id, std::array<float,3> p, std::string&) { auto& a = world.actors[name_of(id)]; a.position = p; a.placed = true; return true; };
            v.face = [&](ActorId id, std::array<float,3> t, std::string&) { auto& a = world.actors[name_of(id)]; a.look_at = t; a.looked = true; return true; };
            v.set_visible = [&](ActorId id, bool visible, std::string&) { world.actors[name_of(id)].visible = visible ? 1 : 0; return true; };
            v.put_limbus = [&](ActorId id, std::string&) { world.actors[name_of(id)].limbus = true; return true; };
            v.play_clip = [](ActorId, std::int32_t, std::int32_t& duration, std::string&) { duration = 1500; return true; };
            v.play_object_clip = [&](const std::string& object, const std::string& clip, bool& found, std::string&) { world.objects_played.insert(object + ":" + clip); found = true; return true; };
            v.move_blocking = [](ActorId, bool& blocking, std::string&) { blocking = false; return true; };
            v.move_to = [&](ActorId id, std::array<float,3> goal, bool, bool, std::string&) { auto& a = world.actors[name_of(id)]; a.position = goal; a.placed = true; return true; };
            v.kill = [&](ActorId id, std::string&) { world.actors[name_of(id)].killed = true; return true; };
            v.hands = [&](ActorId id, bool equipped, std::string&) { world.actors[name_of(id)].hands = equipped ? 1 : 0; return true; };
            services.actor_state = [&](ActorId id, bool& alive, std::int32_t& state, std::string&) {
                alive = true; const auto f = world.final_state.find(name_of(id)); state = f == world.final_state.end() ? 3 : f->second; return true; };
            services.set_actor_state = [&](ActorId id, std::int32_t state, std::string&) { world.final_state[name_of(id)] = state; return true; };
            services.set_idle = [&](ActorId id, bool, std::string&) { world.final_state[name_of(id)] = 3; return true; };
            services.all_actors = [&](std::vector<ActorId>& ids, std::string&) { ids.clear(); for (const auto& n : world.names) ids.push_back(n.first); if (ids.empty()) ids.push_back(1); return true; };
        });
    rig->host->set_caption_text([](std::int32_t id, std::string& text, std::string&) { text = "line " + std::to_string(id); return true; });
    rig->host->set_caption_auto_tap_ms(300);
    const int script = rig->runtime.script_id(script_name, false);
    if (script < 0) { why = "script absent"; return false; }
    if (!rig->runtime.start(script, -1, false, error)) { why = "start: " + error; return false; }
    bool pressed = skip_after_steps < 0;
    for (int steps = 0; steps < 30000 && rig->runtime.any_running(); ++steps) {
        if (!pressed && steps >= skip_after_steps && rig->host->skip_visible()) { rig->host->press_skip(); pressed = true; }
        rig->step(10);
        if (rig->host->aborts() != 0) { why = "aborted"; return false; }
    }
    if (rig->runtime.any_running()) { why = "did not finish"; return false; }
    if (!pressed) { why = "no SKIP control was drawn"; return false; }
    for (int settle = 0; settle < 10; ++settle) rig->step(10); // the closing camera_idle clip (33 ms) ends a frame after the script does
    world.camera_target = rig->camera.target();
    world.host = rig->host->host_state();
    out = std::move(world);
    return true;
}

void skipped_world_state_matches_played(const std::string& directory) {
    std::vector<std::string> names;
    {
        Rig probe(directory);
        for (const auto& s : probe.runtime.scripts()) {
            if (s.scope != "level") continue;
            bool camera = false;
            for (const auto& c : s.commands) if (c.kind == 5) camera = true;
            if (camera) names.push_back(s.name);
        }
    }
    int compared = 0;
    const std::set<std::string> required{"Swamp_Serpent_Intro", "Swamp_Serpent_Defeated"};
    std::set<std::string> compared_names;
    for (const auto& name : names) {
        RecordedWorld played;
        std::string why;
        if (!run_recorded(directory, name, -1, played, why)) {
            check(required.count(name) == 0, name + " must be playable with the recording world: " + why);
            std::cout << "skip parity: " << name << " not comparable with the recording world (" << why << ")\n";
            continue;
        }
        for (const int at : {0, 25, 300, 1200}) {
            RecordedWorld skipped;
            if (!run_recorded(directory, name, at, skipped, why)) {
                // The press lands only while the SKIP control is drawn; a script shorter than `at` steps has nothing left to skip.
                if (why == "no SKIP control was drawn") continue;
                check(false, name + " skipped at step " + std::to_string(at) + ": " + why);
            }
            const std::string label = name + " skipped at step " + std::to_string(at);
            check(played.actors.size() == skipped.actors.size(), label + ": the same actors are touched");
            for (const auto& entry : played.actors) {
                const auto other = skipped.actors.find(entry.first);
                check(other != skipped.actors.end(), label + ": actor " + entry.first + " is also touched by the skipped run");
                const auto& a = entry.second;
                const auto& b = other->second;
                check(a.position == b.position && a.placed == b.placed, label + ": " + entry.first + " ends at the same position");
                check(a.looked == b.looked && a.look_at == b.look_at, label + ": " + entry.first + " ends with the same heading target");
                check(a.visible == b.visible, label + ": " + entry.first + " ends with the same visibility");
                check(a.limbus == b.limbus && a.killed == b.killed && a.hands == b.hands, label + ": " + entry.first + " limbus/kill/hands state matches");
            }
            check(played.final_state == skipped.final_state, label + ": the same actor states are requested");
            check(played.camera_target == skipped.camera_target, label + ": the camera target is the same after the End");
            const auto& a = played.host;
            const auto& b = skipped.host;
            std::string flags;
            const auto diff = [&flags](const char* what, bool same) { if (!same) flags += std::string(" ") + what; };
            diff("hud_visible", a.hud_visible == b.hud_visible); diff("skip_visible", a.skip_visible == b.skip_visible);
            diff("cutscene_mode", a.cutscene_mode == b.cutscene_mode); diff("cinematic_active", a.cinematic_active == b.cinematic_active);
            diff("save_blocked", a.save_blocked == b.save_blocked); diff("global_blocked", a.global_blocked == b.global_blocked);
            diff("camera_clip", a.camera_clip == b.camera_clip); diff("character_blocked", a.character_blocked == b.character_blocked);
            diff("scripted", a.scripted == b.scripted); diff("safe_zone", a.safe_zone == b.safe_zone); diff("aborts", a.aborts == b.aborts);
            check(flags.empty(), label + ": host flags match, differing:" + flags);
            ++compared;
            compared_names.insert(name);
        }
    }
    check(compared_names.count("Swamp_Serpent_Intro") == 1 && compared_names.count("Swamp_Serpent_Defeated") == 1,
          "the Mud Lord intro and outro were compared played vs skipped");
    std::cout << "skip parity: " << compared << " played-vs-skipped comparisons over " << compared_names.size() << " cutscene scripts\n";
}
} // namespace

// P16 CINE2: PlayCamera (kind 5) through the production adapter and host. Blocking (IDA IsBlocking) holds while
// the cs_swamp_intro scene01 clip plays (4000 ms on the frame clock), the source pose follows the clip, then the
// follow pose returns. A non-blocking clip never blocks. An unknown dictionary id fails with the named error.
void camera_clip_blocks_then_releases(const std::string& directory) {
    Rig rig(directory);
    OriginalCampaignCommand clip;
    clip.kind = 5;
    clip.class_name = "Script_PlayCamera";
    clip.scalars = {{4, 5}, {8, 359}, {12, 1}};
    bool blocking = true;
    std::string error;
    check(rig.world->command(CampaignCommandPhase::execute, clip, 0, false, blocking, error) && !blocking, "PlayCamera execute starts without blocking: " + error);
    check(rig.world->command(CampaignCommandPhase::is_blocking, clip, 0, false, blocking, error) && blocking, "blocking PlayCamera holds while the clip plays");
    CameraPose follow{};
    follow.position = {10, 20, 30};
    follow.target = {11, 21, 31};
    const CameraPose early = rig.host->source_camera_pose(follow);
    const CameraPose translated = rig.host->source_camera_pose(follow,{-6000,12000,255});
    check(std::abs(translated.position.x-early.position.x+6000)<.01f&&std::abs(translated.target.y-early.target.y-12000)<.01f&&
          std::abs(translated.position.z-early.position.z-255)<.01f,"Script camera must share the live module rig translation exactly once");
    check(early.position.x != follow.position.x || early.position.y != follow.position.y, "the source pose follows the clip, not the follow camera");
    check(rig.host->camera_clip_active(), "clip is active after execute");
    rig.step(2000);
    check(rig.world->command(CampaignCommandPhase::is_blocking, clip, 0, false, blocking, error) && blocking, "still blocking at 2000 ms");
    rig.step(2500);
    check(rig.world->command(CampaignCommandPhase::is_blocking, clip, 0, false, blocking, error) && !blocking, "released after the 4000 ms clip");
    check(!rig.host->camera_clip_active(), "clip inactive after the end");
    const CameraPose after = rig.host->source_camera_pose(follow);
    check(after.position.x == follow.position.x && after.target.x == follow.target.x, "follow pose is restored after the clip");

    OriginalCampaignCommand nonblocking = clip;
    nonblocking.scalars[12] = 0;
    check(rig.world->command(CampaignCommandPhase::execute, nonblocking, 0, false, blocking, error) && !blocking, "non-blocking PlayCamera starts");
    check(rig.world->command(CampaignCommandPhase::is_blocking, nonblocking, 0, false, blocking, error) && !blocking, "non-blocking PlayCamera never blocks");

    OriginalCampaignCommand missing = clip;
    missing.scalars[8] = 999999;
    check(!rig.world->command(CampaignCommandPhase::execute, missing, 0, false, blocking, error) && error.find("absent") != std::string::npos,
          "unknown PlayCamera id is an explicit error: " + error);
    check(!rig.host->camera_clip_active(), "a failed PlayCamera leaves no clip playing");
}

// Script_PlayEffect (kind 20) branches through the real executor: placed target plays once per command (no dedupe),
// unplaced target and resolver failure do not play, an unbound resolver fails closed, and a play failure continues.
void play_effect_branches(const std::string& directory) {
    struct Play { std::int32_t set; std::array<float,3> at; std::uintptr_t anchor; };
    const auto make_command = [](std::int32_t set, const std::string& target) {
        OriginalCampaignCommand c;
        c.kind = 20; c.class_name = "Script_PlayEffect";
        c.scalars[8] = static_cast<std::uint32_t>(set);
        c.scalars[16] = 0; c.scalars[20] = 0; c.scalars[24] = 0;
        c.strings[32] = target;
        return c;
    };
    // Placed target: two identical authored cues produce two plays with the same anchor and local offset.
    {
        int resolves = 0; bool placed = true; bool failResolver = false;
        Rig rig(directory, {}, [&](const std::string&, int, std::uintptr_t& id, bool& found, std::string& e) {
            ++resolves;
            if (failResolver) { e = "anchor lookup failed"; return false; }
            id = 0x8000000000000001ull; found = placed; return true;
        });
        std::vector<Play> plays;
        rig.host->bind_fx([&](std::int32_t set, const std::array<float,3>& at, std::uintptr_t anchor, std::string&) {
            plays.push_back({set, at, anchor}); return true; }, [](std::int32_t, std::string&) { return true; });
        bool blocking = true; std::string error;
        const auto cue = make_command(132, "_prim_Waypoint_doorport");
        check(rig.host->execute_diagnostic_command(cue, -1, blocking, error) && !blocking && error.empty(), "placed PlayEffect continues: " + error);
        check(rig.host->execute_diagnostic_command(cue, -1, blocking, error), "second authored cue continues: " + error);
        check(resolves == 2 && plays.size() == 2, "each authored cue resolves and plays once (no dedupe)");
        check(plays[0].set == 132 && plays[1].set == 132 && plays[0].anchor == plays[1].anchor && plays[0].anchor == 0x8000000000000001ull,
              "set and opaque anchor are forwarded unchanged");
        check(plays[0].at == std::array<float,3>{0.f, 0.f, 0.f} && plays[1].at == plays[0].at, "authored local offset is unchanged");

        // Unplaced target: the command completes without any play call.
        placed = false;
        check(rig.host->execute_diagnostic_command(make_command(119, "_prim_Missing"), -1, blocking, error) && plays.size() == 2,
              "unplaced target is a logged no-op, not a fabricated cue");
        // Resolver failure: the error propagates and nothing plays.
        failResolver = true;
        check(!rig.host->execute_diagnostic_command(make_command(119, "_prim_Waypoint_5"), -1, blocking, error) &&
              error.find("anchor lookup failed") != std::string::npos && plays.size() == 2,
              "resolver failure propagates without a cue");
        failResolver = false;
    }
    // Play failure: the reason is reported and the cutscene command still completes.
    {
        Rig rig(directory, {}, [](const std::string&, int, std::uintptr_t& id, bool& found, std::string&) { id = 0x8000000000000002ull; found = true; return true; });
        int plays = 0;
        rig.host->bind_fx([&](std::int32_t, const std::array<float,3>&, std::uintptr_t, std::string& e) { ++plays; e = "original asset missing"; return false; },
                          [](std::int32_t, std::string&) { return true; });
        bool blocking = true; std::string error;
        check(rig.host->execute_diagnostic_command(make_command(120, "_prim_Waypoint_5"), -1, blocking, error) && plays == 1,
              "failed play continues the command after one attempt");
        check(summary(*rig.host).find("PlayEffect set failed") != std::string::npos, "failed play is listed explicitly");
    }
    // Unbound resolver: fail closed before any play.
    {
        Rig rig(directory);
        int plays = 0;
        rig.host->bind_fx([&](std::int32_t, const std::array<float,3>&, std::uintptr_t, std::string&) { ++plays; return true; },
                          [](std::int32_t, std::string&) { return true; });
        bool blocking = true; std::string error;
        check(!rig.host->execute_diagnostic_command(make_command(119, "_prim_Waypoint_5"), -1, blocking, error) && plays == 0,
              "unbound FX anchor resolver fails closed");
    }
}

// B091/B094: actual host clock and actor-verb path, independent of the visual backend.
// The owner can preserve a retired prop/dead actor, and errors follow normal abort handling.
void scripted_clip_completion(const std::string& directory) {
    int finishes=0,plays=0; bool release=true,fail=false;
    Rig rig(directory, {}, {}, [&](CampaignHostServices& services) {
        services.actor_verbs.resolve_actor=[](const std::string&,int,ActorId& id,bool& found,std::string&){id=101;found=true;return true;};
        services.actor_verbs.play_clip=[&](ActorId,std::int32_t,std::int32_t& duration,std::string&){++plays;duration=100;return true;};
        services.scripted_clip_finished=[&](ActorId,bool& released,std::string& e){++finishes;released=release;if(fail){e="completion rejected";return false;}return true;};
    });
    bool blocking=false; std::string error;
    OriginalCampaignCommand clip;clip.kind=45;clip.scalars={{8,33},{12,0xffffffffu},{28,1}};clip.strings={{24,"boss"}};
    auto play=[&](std::uint32_t next=0xffffffffu){clip.scalars[12]=next;check(rig.host->execute_diagnostic_command(clip,-1,blocking,error),error);};
    OriginalCampaignCommand mark;mark.kind=32;mark.strings={{16,"All"}};
    auto scripted=[&](bool value){mark.scalars[8]=value;check(rig.world->command(CampaignCommandPhase::execute,mark,-1,false,blocking,error),error);};
    scripted(true);
    play();rig.step(99);check(finishes==0&&rig.host->scripted_clip_pose(101),"No premature CSAnim release");
    scripted(false);
    rig.step(1);check(finishes==1&&!rig.host->scripted_clip_pose(101),"Single clip returns to gameplay at completion");
    rig.step(100);check(finishes==1,"Completed clip cannot release twice");
    scripted(true);play(32);rig.step(100);check(finishes==1&&plays==3,"Chained clip starts before Idle");
    scripted(false);
    rig.step(100);check(finishes==2,"Chained clip releases once at its own end");
    scripted(true);play();rig.step(50);play();rig.step(50);check(finishes==2,"Replacement cancels earlier completion");
    scripted(false);
    rig.step(50);check(finishes==3,"Replacement has its own completion");
    release=false;play();rig.step(100);check(finishes==4&&rig.host->scripted_clip_pose(101),"Owner can retain non-gameplay/dead pose");
    release=true;
    // Enter the real common Begin contract so SKIP is visible, with no fake actor state events.
    check(rig.runtime.start(rig.runtime.script_id("BeginScriptedCutScene",true),-1,true,error),error);
    for(int i=0;i<10&&rig.runtime.any_running();++i)rig.step(0);
    check(rig.host->skip_visible(),"Begin provides SKIP");
    play();rig.step(50);rig.host->press_skip();rig.step(0);
    check(finishes==4&&rig.host->scripted_clip_pose(101),"SKIP keeps existing animator completion");
    rig.step(50);check(finishes==4&&rig.host->scripted_clip_pose(101),"Completed skipped clip remains gated while Character is scripted");
    OriginalCampaignCommand unmark;unmark.kind=32;unmark.scalars={{8,0}};unmark.strings={{16,"All"}};
    check(rig.world->command(CampaignCommandPhase::execute,unmark,-1,true,blocking,error),error);
    rig.step(0);check(finishes==5&&!rig.host->scripted_clip_pose(101),"Actual Unset releases the skipped completed clip once");
    // Explicit non-waiting Idle replaces the clip and cannot later release it again.
    play();OriginalCampaignCommand idle;idle.kind=31;idle.strings={{12,"All"}};
    check(rig.world->command(CampaignCommandPhase::execute,idle,-1,true,blocking,error),error);
    rig.step(100);check(finishes==5&&!rig.host->scripted_clip_pose(101),"Explicit Idle cancels obsolete timer");
    fail=true;play();rig.step(100);check(rig.host->aborts()==1&&!rig.host->scripted_clip_pose(101),"Completion failure aborts and releases host pose");
}

// B155 review (#4): a once-trigger (triggercount defaults to 1) that fired is saved with the level world (Trigger::Serialize writes the
// activation count, IDA 0x398978, and Trigger::CanActivate blocks at the limit, IDA 0x3987ac). A revisit that restores that world must not
// fire the zone again, and the persisted count is what carries it.
void fired_once_trigger_stays_fired_across_level_world(const std::string& directory) {
    const ObjectId lifecycleId = 0x43414d504c494650ull;
    const std::string key = "once-test.mgp::once-zone";
    dh2::data::AiTables ai{};dh2::data::CombatRandom random{1,0};
    std::string error;
    check(load_original_ai_tables(AssetCatalog(directory),"original-cache/data/pydata/",ai,error),"source AI tables: "+error);
    PlayableActorWorld world(ai,random);
    ActorDefinition zone;zone.name="once-zone";zone.sourcePath="once-test.mgp";zone.gametype="TriggerZone";
    zone.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    zone.properties={{"name","once-zone"},{"gametype","TriggerZone"},{"type","Block"},{"script","enterLocation_StartRoom"}};
    {
        Rig first(directory);
        check(first.host->build_zones({zone},error),error);
        check(first.host->bind_lifecycle_object(world,error),error);
        first.livePosition={0,0,0};first.host->frame(0,first.livePosition,true,1);
        check(first.runtime.trigger_activations().at(key)==1,"entry fires the once-trigger");
        check(first.host->persist_lifecycle(world,error),error);
        check(world.find_object(lifecycleId)->state_components.count("campaign_lifecycle_v1")==1,"the count is written with the level world");
    }
    // Revisit: a fresh session restores the saved world and the player walks back in.
    Rig second(directory);
    check(second.host->build_zones({zone},error),error);
    check(second.host->bind_lifecycle_object(world,error),error);
    check(second.host->restore_lifecycle(world,error),error);
    check(second.runtime.trigger_activations().count(key)==1,"restored count is 1 (the count is what keeps the trigger spent)");
    second.livePosition={0,0,0};second.host->frame(0,second.livePosition,true,1);
    check(second.runtime.trigger_activations().at(key)==1,"revisit at the zone does not re-fire the once-trigger");
}

// B145: the repair stamp rides in its own save component, is absent until a repair happened, survives persist/restore,
// and an unreadable stamp disables the repair for that load without failing the load.
void aborted_scene_repair_stamp_roundtrip(const std::string& directory) {
    const ObjectId lifecycleId = 0x43414d504c494650ull;
    const std::string key = "stamp-test.mgp::stamp-zone";
    const auto make = [&](Rig& rig, PlayableActorWorld& world) {
        std::string error;
        ActorDefinition zone;zone.name="stamp-zone";zone.sourcePath="stamp-test.mgp";zone.gametype="TriggerZone";
        zone.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        zone.properties={{"name","stamp-zone"},{"gametype","TriggerZone"},{"type","Block"},{"script","LizCaptain_Intro"}};
        check(rig.host->build_zones({zone},error),error);
        check(rig.host->bind_lifecycle_object(world,error),error);
    };
    const auto pristine = [](const std::string&, SceneActorFact& fact, std::string&) { fact = {true,true,true,true}; return true; };
    dh2::data::AiTables ai{};dh2::data::CombatRandom random{1,0};
    std::string error;
    check(load_original_ai_tables(AssetCatalog(directory),"original-cache/data/pydata/",ai,error),"source AI tables: "+error);
    PlayableActorWorld world(ai,random);
    Rig first(directory);make(first,world);
    check(first.host->persist_lifecycle(world,error),error);
    check(!world.find_object(lifecycleId)->state_components.count("campaign_repairs_v1"),"no repair component before any repair");
    const auto before = world.find_object(lifecycleId)->state_components;
    first.runtime.restore_trigger_activations({{key,1}});
    std::ostringstream log;
    check(first.host->repair_aborted_scenes(pristine,log).size()==1,"consumed aborted scene is re-armed: "+log.str());
    check(first.runtime.trigger_activations().count(key)==0,"re-armed count is zero");
    check(first.host->persist_lifecycle(world,error),error);
    check(world.find_object(lifecycleId)->state_components.count("campaign_repairs_v1")==1,"repair component is written after a repair");
    check(world.find_object(lifecycleId)->state_components.at("campaign_lifecycle_v1")==before.at("campaign_lifecycle_v1"),"re-armed trigger count is not in the saved lifecycle component");

    // A fresh session restores the stamp and the same signature is then refused.
    Rig second(directory);make(second,world);
    check(second.host->restore_lifecycle(world,error),error);
    check(second.host->applied_repairs().size()==1,"stamp restored");
    second.runtime.restore_trigger_activations({{key,1}});
    std::ostringstream again;
    check(second.host->repair_aborted_scenes(pristine,again).empty()&&second.runtime.trigger_activations().count(key)==1,"stamped save is not repaired again: "+again.str());
    // Persisting again keeps the stamp (it is not dropped by a save that did no repair).
    check(second.host->persist_lifecycle(world,error)&&world.find_object(lifecycleId)->state_components.count("campaign_repairs_v1")==1,"stamp survives another save");

    // Unreadable stamp component: the load still succeeds, the repair is skipped.
    check(world.set_object_component(lifecycleId,"campaign_repairs_v1",{1,0,0,0,9,0,0,0,1},error),error);
    Rig third(directory);make(third,world);
    check(third.host->restore_lifecycle(world,error),"malformed stamp must not fail the load: "+error);
    third.runtime.restore_trigger_activations({{key,1}});
    std::ostringstream untrusted;
    check(third.host->repair_aborted_scenes(pristine,untrusted).empty()&&third.runtime.trigger_activations().count(key)==1,"unreadable stamp disables the repair for the load");
    // A save from an older build has no stamp component at all and still loads.
    Rig fourth(directory);make(fourth,world);
    PlayableActorWorld legacy(ai,random);
    check(fourth.host->bind_lifecycle_object(legacy,error),error);
    check(fourth.host->persist_lifecycle(legacy,error)&&fourth.host->restore_lifecycle(legacy,error),"legacy world without stamp loads: "+error);
    check(fourth.host->applied_repairs().empty(),"no stamp in a legacy world");
}

// B155 review: the campaign_limbus_v1 component is written only when a live Limbus actor exists (or to overwrite a stale one), so
// saves with no Limbus actor stay byte-identical to the earlier builds.
void limbus_component_only_when_needed(const std::string& directory) {
    const ObjectId lifecycleId = 0x43414d504c494650ull;
    std::vector<ActorId> live;
    Rig rig(directory, {}, {}, [&](CampaignHostServices& services) {
        services.live_limbus_actors = [&](std::vector<ActorId>& ids, std::string&) { ids = live; return true; };
    });
    dh2::data::AiTables ai{};dh2::data::CombatRandom random{1,0};
    std::string error;
    check(load_original_ai_tables(AssetCatalog(directory),"original-cache/data/pydata/",ai,error),"source AI tables: "+error);
    PlayableActorWorld world(ai,random);
    check(rig.host->bind_lifecycle_object(world,error),error);
    check(rig.host->persist_lifecycle(world,error),error);
    const auto& components = [&]() -> const auto& { return world.find_object(lifecycleId)->state_components; };
    check(components().count("campaign_lifecycle_v1")==1&&components().count("campaign_limbus_v1")==0,"no Limbus component while no actor is in Limbus");
    live = {7, 9};
    check(rig.host->persist_lifecycle(world,error)&&components().count("campaign_limbus_v1")==1,"Limbus component written for live Limbus actors");
    std::set<ActorId> read;
    check(CampaignHost::read_live_limbus_actors(world,read,error)&&read==std::set<ActorId>({7,9}),"Limbus component round-trips");
    live.clear();
    check(rig.host->persist_lifecycle(world,error)&&components().count("campaign_limbus_v1")==1,"a stale component is overwritten");
    check(CampaignHost::read_live_limbus_actors(world,read,error)&&read.empty(),"the overwritten component is empty");
}

void scripted_clip_deferred_until_unset(const std::string& directory) {
    int finishes=0,plays=0;
    Rig rig(directory, {}, {}, [&](CampaignHostServices& services) {
        services.actor_verbs.resolve_actor=[](const std::string&,int,ActorId& id,bool& found,std::string&){id=101;found=true;return true;};
        services.actor_verbs.play_clip=[&](ActorId,std::int32_t,std::int32_t& duration,std::string&){++plays;duration=100;return true;};
        services.actor_verbs.kill=[](ActorId,std::string&){return true;};
        services.actor_verbs.put_limbus=[](ActorId,std::string&){return true;};
        services.scripted_clip_finished=[&](ActorId,bool& released,std::string&){++finishes;released=true;return true;};
    });
    bool blocking=false;std::string error;
    OriginalCampaignCommand mark;mark.kind=32;mark.strings={{16,"All"}};
    auto scripted=[&](bool value){mark.scalars[8]=value;check(rig.world->command(CampaignCommandPhase::execute,mark,-1,false,blocking,error),error);};
    OriginalCampaignCommand clip;clip.kind=45;clip.scalars={{8,33},{12,0xffffffffu},{28,1}};clip.strings={{24,"boss"}};
    auto play=[&](std::uint32_t next=0xffffffffu){clip.scalars[12]=next;check(rig.host->execute_diagnostic_command(clip,-1,blocking,error),error);};
    scripted(true);play();rig.step(100);rig.step(100);rig.step(0);
    check(finishes==0&&rig.host->scripted_clip_pose(101),"Scripted gate retains completed clip across frames");
    scripted(false);rig.step(0);check(finishes==1&&!rig.host->scripted_clip_pose(101),"Unset releases retained completion");
    scripted(false);rig.step(100);check(finishes==1,"Duplicate Unset/update cannot release twice");
    scripted(true);play(32);rig.step(100);check(plays==3&&finishes==1,"Chain starts while scripted without releasing Idle");
    rig.step(100);rig.step(100);check(finishes==1,"Completed chain remains pending behind scripted gate");
    scripted(false);rig.step(0);check(finishes==2,"Unset releases completed chain exactly once");
    scripted(true);play();rig.step(100);play();scripted(false);rig.step(0);
    check(finishes==3,"Unset admits only the current replacement; discarded completion cannot release separately");
    rig.step(100);check(finishes==3,"Replacement admitted by Unset cannot release twice at its former clip end");
    scripted(true);play();rig.step(100);OriginalCampaignCommand idle;idle.kind=31;idle.strings={{12,"All"}};
    check(rig.world->command(CampaignCommandPhase::execute,idle,-1,true,blocking,error),error);
    scripted(false);rig.step(0);check(finishes==3&&!rig.host->scripted_clip_pose(101),"Explicit skipped Idle cancels gated completion before Unset");
    scripted(true);play(32);rig.step(50);const int plays_before_unset=plays;
    scripted(false);rig.step(0);check(finishes==4&&plays==plays_before_unset&&!rig.host->scripted_clip_pose(101),
        "Unset admits source Idle before clip duration ends and cancels its unstarted follow");
    scripted(false);rig.step(200);check(finishes==4&&plays==plays_before_unset,"Unset admission and follow cancellation happen once");
    for(int kind:{44,29}) {
        scripted(true);play();rig.step(100);
        OriginalCampaignCommand cancel;cancel.kind=kind;cancel.strings={{kind==44?12u:16u,"boss"}};
        check(rig.host->execute_diagnostic_command(cancel,-1,blocking,error),error);
        scripted(false);rig.step(0);check(finishes==4&&!rig.host->scripted_clip_pose(101),
            "Kill/Limbus cancels a deferred completion; Unset cannot restore its gameplay pose");
    }
    scripted(true);play();rig.step(100);scripted(false);scripted(true);rig.step(0);
    check(finishes==4&&rig.host->scripted_clip_pose(101),"Re-mark before update closes an earlier Unset admission");
    scripted(false);rig.step(0);check(finishes==5&&!rig.host->scripted_clip_pose(101),"Later real Unset admits the retained clip once");
    play(32);const int unmarked_plays=plays;rig.step(1);
    check(finishes==6&&plays==unmarked_plays&&!rig.host->scripted_clip_pose(101),
        "Already-clear scripted gate admits kind45 Idle next update, before duration or follow");
    scripted(false);rig.step(200);check(finishes==6&&plays==unmarked_plays,"Duplicate Unset does not repeat admission or start a cancelled follow");
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply the campaign asset directory");
        lizard_intro_contract(argv[1]);
        skip_press_contract(argv[1]);
        scripted_trigger_contacts(argv[1]);
        fired_once_trigger_stays_fired_across_level_world(argv[1]);
        skipped_cutscene_matches_played(argv[1]);
        skip_ends_captions_in_one_frame(argv[1]);
        skipped_world_state_matches_played(argv[1]); // w5-mud2
        captions_block_then_release(argv[1]);
        unresolved_caption_is_explicit(argv[1]);
        do_tutorial_starts_named_script(argv[1]);
        tutorial_is_one_shot_across_sessions(argv[1]);
        unsupported_commands_named(argv[1]);
        camera_clip_blocks_then_releases(argv[1]);
        actor_clip_manifest_collects_script_clips(argv[1]);
        play_effect_branches(argv[1]);
        scripted_clip_completion(argv[1]);
        scripted_clip_deferred_until_unset(argv[1]);
        aborted_scene_repair_stamp_roundtrip(argv[1]); // B145
        limbus_component_only_when_needed(argv[1]); // B155
        std::cout << "campaign_host tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "campaign_host test failed: " << e.what() << '\n';
        return 1;
    }
}
