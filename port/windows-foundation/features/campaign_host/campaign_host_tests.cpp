// P16 HOST provider contract tests. Argument: directory containing the campaign original-campaign.xml.
// Uses the real extracted Swamp script bank through the same executor path as the live host.
#include "campaign_host.hpp"
#include "../../campaign_camera_adapter.hpp"
#include "../../original_actor_lifecycle.hpp"
#include "../../original_campaign_world_adapter.hpp"
#include "../../original_camera_clip.hpp"
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
    std::map<ActorId, bool> characterBlocked;
    std::map<ActorId, std::int32_t> states;
    std::unique_ptr<OriginalCampaignWorldAdapter> world;
    std::unique_ptr<CampaignHost> host;

    explicit Rig(const std::string& campaignDirectory) {
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
        services.all_actors = [](std::vector<ActorId>& out, std::string&) { out = {1, 101, 102}; return true; };
        services.actor_state = [this](ActorId id, bool& alive, std::int32_t& state, std::string&) {
            alive = true;
            const auto found = states.find(id);
            state = found == states.end() ? 3 : found->second;
            return true;
        };
        services.set_actor_state = [this](ActorId id, std::int32_t state, std::string&) { states[id] = state; return true; };
        services.read_camera_clip = [this](std::int32_t id, std::vector<std::uint8_t>& clip, std::vector<std::uint8_t>& scene, std::string& path, std::string& e) {
            return clipLibrary.read(*assetCatalog, id, clip, scene, path, e);
        };
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
    for (int i = 0; i < 4000 && rig.runtime.running(tuto); ++i) rig.step(10); // captions hold the script (WaitDialog)
    check(!rig.runtime.running(tuto) && rig.host->aborts() == 1 && !rig.globalBlocked, "session continues after an abort");
}

// SKIP is an abstract press: ignored while hidden, sampled by later commands while visible.
void skip_press_contract(const std::string& directory) {
    Rig rig(directory);
    std::string error;
    rig.host->press_skip();
    check(!rig.host->skip_active(), "SKIP press while hidden must be ignored");
    check(rig.runtime.start(rig.runtime.script_id("LizardMan_Intro", false), -1, false, error), error);
    for (int i = 0; i < 4; ++i) rig.step(0);
    check(rig.host->skip_visible(), "SKIP visible during the cutscene");
    rig.host->press_skip();
    check(rig.host->skip_active(), "SKIP press while visible must be sampled by later commands");
    for (int i = 0; i < 40 && rig.host->aborts() == 0; ++i) rig.step(100);
    check(rig.host->aborts() == 1 && !rig.host->skip_active(), "cutscene end or failure must clear the SKIP press");
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

// Without a resolver the line is an explicit marker, listed as unresolved, and the script still finishes.
void unresolved_caption_is_explicit(const std::string& directory) {
    Rig rig(directory);
    std::string error;
    const int script = rig.runtime.script_id("CombatTuto", false);
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
    const std::set<int> unsupported{5, 6, 7, 40, 41, 42, 43, 44, 45, 46};
    const std::set<int> safe{1, 2, 4, 8, 10, 12, 22, 23, 24, 25, 26, 31, 32, 39, 69, 70, 77, 78, 79};
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
    check(chosen >= 0, "fixture level script with an unsupported command after safe commands");
    const auto name = rig.runtime.scripts().at(static_cast<std::size_t>(chosen)).name;
    check(rig.runtime.start(chosen, -1, false, error), error);
    for (int i = 0; i < 40 && rig.host->aborts() == 0; ++i) rig.step(100);
    check(rig.host->aborts() == 1, "unsupported command must abort the cutscene explicitly: " + name);
    const auto text = summary(*rig.host);
    check(text.find("(no owner bound) count=1") != std::string::npos, "unsupported command must be listed by name with a count");
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

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply the campaign asset directory");
        lizard_intro_contract(argv[1]);
        skip_press_contract(argv[1]);
        captions_block_then_release(argv[1]);
        unresolved_caption_is_explicit(argv[1]);
        do_tutorial_starts_named_script(argv[1]);
        unsupported_commands_named(argv[1]);
        camera_clip_blocks_then_releases(argv[1]);
        std::cout << "campaign_host tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "campaign_host test failed: " << e.what() << '\n';
        return 1;
    }
}
