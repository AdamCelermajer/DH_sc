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

    // SetCameraTarget 1000 ms is blocking; Wait 500 then SpawnCharacter has no lifecycle owner for the lizard.
    for (int i = 0; i < 20 && rig.host->aborts() == 0; ++i) rig.step(100);
    check(rig.host->aborts() == 1, "Spawn without a bound lifecycle owner must abort the cutscene explicitly");
    check(rig.host->hud_visible(), "failure must restore the HUD");
    check(!rig.host->skip_visible() && !rig.host->skip_active(), "failure must hide the SKIP control");
    check(!rig.host->cutscene_mode() && !rig.host->save_blocked(), "failure must leave cutscene mode and the save block");
    check(!rig.host->global_controller_blocked() && !rig.globalBlocked, "failure must release the controller lock");
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
        skipped_cutscene_matches_played(argv[1]);
        skip_ends_captions_in_one_frame(argv[1]);
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
        std::cout << "campaign_host tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "campaign_host test failed: " << e.what() << '\n';
        return 1;
    }
}
