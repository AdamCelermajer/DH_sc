#include "android/asset_manager.h"
#include "gameplay_audio_sdl.hpp"
#include "model_renderer.hpp"
#include "../android-native/app/src/main/cpp/player_skill_effect_audit_v1.hpp"
#include "original_ui_session.hpp"
#include "../android-native/app/src/main/cpp/menu_start_commit_gate_v1.hpp"
#include "../game-data/campaign_profile_files_v1.hpp"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <SDL.h>
#include <SDL_syswm.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <cstdint>

// Defined in model_renderer.cpp; declared here to keep the scenario change within desktop_main.cpp.
namespace model_renderer {
std::int32_t gameplay_player_state() noexcept;
std::uint32_t tutorial_script_start_count(const char* script_name) noexcept;
bool tutorial_script_running(const char* script_name) noexcept;
bool script_controllers_blocked() noexcept;
}

namespace {
struct Options {
    std::filesystem::path assets;
    std::filesystem::path runtime;
    std::filesystem::path capture;
    std::filesystem::path initial_capture;
    std::uint64_t frames = 0;
    bool show_window = false;
    std::int32_t level_row = -1;
    // PlayerBase row used when the direct-row path has to create the fresh
    // source profile (the same class ids the authored class-selection menu
    // passes to NativeCreateSaveSlot). An existing campaign slot is reused as is.
    std::string player_class = "KnightPlayerBase";
    std::string player_name = "Desktop Knight";
    bool movement_attack_scenario = false;
    bool movement_kill_scenario = false;
    bool movement_defend_scenario = false;
    std::string transition_to;
    double transition_entry = 0.0;
    // Queue --transition-to 120 frames after the scenario's Continue revive
    // instead of at frame 300 (die, revive, then leave the level).
    bool transition_after_revive = false;
    std::uint64_t transition_frame = 300;
    // A -> B -> A: --return-to-origin ENTRY queues a second NativeGoToZone back to
    // the level this run started in (entry ENTRY) --return-delay frames after the
    // --transition-to destination finished loading, so the LevelSavegame the first
    // visit wrote is read back on the second.
    std::int32_t return_entry = -1;
    std::uint64_t return_delay = 240;
    // --enemy-census-at N (repeatable): log the Monster census at rendered frame N.
    std::vector<std::uint64_t> census_frames;
    // --start-script NAME[@N] (repeatable): start the named Level pyscript on the
    // Level's ScriptManager at rendered frame N (default 200).
    struct ScriptStart {std::string name; std::uint64_t frame;};
    std::vector<ScriptStart> script_starts;
    // --flush-dialogs: while a level script holds the controllers (LockCharacter
    // "All"), flush its dialogs every 20 frames like the player's taps would, so
    // an unattended run is not parked behind a cutscene such as Swamp_Intro.
    bool flush_dialogs = false;
    // --play-anim TARGET:DICT[:DICT2][:wait][@N] (repeatable): Character::
    // _RegisterAnim/_PlayAnim on the Player ("player") or a named Monster.
    struct PlayAnim {std::string target, dict, second; bool wait; std::uint64_t frame;};
    std::vector<PlayAnim> play_anims;
    bool fixed_cultist_scenario = false;
    bool fixed_cultist_kill_scenario = false;
    bool skill_kill_scenario = false;
    // touch-kill: move-kill, but the stick and the attack button are two fingers
    // on the authored HUD (the Android MotionEvent entry), held together.
    bool hud_touch_scenario = false;
    // Verification probes (faery-follow / faery-skill / skill-matrix). They use
    // only public renderer/UI entries and own no gameplay state.
    bool faery_follow_scenario = false;
    bool faery_skill_scenario = false;
    bool skill_matrix_scenario = false;
    // boss-fight: spawn --boss NAME (default per level row), put the Player at the
    // boss's authored arena entry spot (the boss's position plus --boss-offset
    // X,Y,Z, defaulting to the offset of the level's _prim_Waypoint_PrinceSpot), then
    // attack the boss (or just watch with --boss-passive) while logging the boss
    // AIS counters and both sides' health.
    // --boss-weaken: leave the boss 3 HP through the development debug_character_hit
    // fixture (the existing nonlethal source health kernel) so one Player blow kills it.
    bool boss_fight_scenario = false, boss_passive = false, boss_offset_given = false,
         boss_weaken = false;
    // boss-fight keeps running this many frames after the boss dies (the boss
    // outro scripts, quest completion and rewards run in them).
    std::uint64_t post_death_frames = 180;
    // --boss-natural: do not spawn the boss by hand. The Player is placed at the
    // arena approach (--boss-entry X,Y,Z, default per level row) and walks into the
    // authored intro TriggerZone (SWAMP _prim_TriggerZone_17 -> Swamp_Serpent_Intro);
    // on DARKWOOD the Dark Queen's intro is a Quest::SetState(5) script
    // (HG2_boss_intro), started the way Quest::ExecScript does. The scenario then
    // follows the running Level scripts until the boss leaves Limbus.
    bool boss_natural = false, boss_entry_given = false;
    // --boss-summon-trap NAME: once the fight is running, the boss owns a TimerTraps
    // member NAME (for example SwampkingSteamCloud) summoned at the Player's feet.
    std::string boss_summon_trap;
    // --boss-summon-ally NAME: once the fight is running, the Player summons the
    // CharacterTable member NAME (for example RisenZombie) beside itself.
    std::string boss_summon_ally;
    // --boss-refill: development fixture that tops the Player's HP back up to Max_HP
    // once per second so hazards with long lifetimes (TimerTrap clouds) can be observed.
    bool boss_refill = false;
    int boss_refill_hp = 0;  // --boss-refill-hp N: also raises Max_HP to N
    std::array<float, 3> boss_entry{};
    // Distance (planar) from the Player to the intro trigger centre / intro script name.
    std::string boss_intro_script;
    std::array<float, 3> boss_trigger{};
    bool boss_trigger_given = false;
    std::string boss_name;
    std::array<float, 3> boss_offset{};
    // Planar distance from the boss at which the Player is placed (the direction
    // is the offset's); 0 keeps the offset as is.
    float boss_distance = 220.0f;
    // faery-skill: press the authored HUD faery button with a touch instead of
    // calling the NativeHUDSpell entry directly.
    bool faery_hud_tap = false;
    // 1280x720 reference-layout touch point used by --faery-hud-tap (default: faery button).
    int hud_tap_x = 1176, hud_tap_y = 281;
    // skill-matrix: test only this SkillTree row (a failed train poisons later rows).
    std::int32_t matrix_only = -1;
    // faery-follow: select this Faery Save slot (0..4) through the same entry as
    // the HUD (ChangeFaery + Level::PlaceFaeryAndFollowers) at frame 30.
    std::int32_t follow_faery_select = -1;
    std::filesystem::path capture_series;
    std::uint64_t capture_every = 300;
    // faery-follow ends by leaving the level so the source LevelSavegame (which
    // serializes every Character position, the Faery's included) is published.
    std::string follow_exit_destination;
    double follow_exit_entry = 0.0;
    bool probe_scenario() const {
        return faery_follow_scenario || faery_skill_scenario || skill_matrix_scenario ||
               boss_fight_scenario || talk_npc_scenario || enter_zone_scenario;
    }
    // talk-npc: stand the Player next to a named quest-giver Character and press the
    // authored HUD attack button (Character::Interact on the selected NPC); the run
    // follows quest `watch_quest` until it has left Available. enter-zone: stand the
    // Player inside a named QuestMoveInZone volume instead.
    bool talk_npc_scenario = false, enter_zone_scenario = false;
    std::string npc_name = "_prim_NPC_LizBlood", zone_name = "_prim_WitchQuestStart";
    float npc_distance = 150.0f;
    std::int32_t watch_quest = -1;
    bool tutorial_potion_scenario = false;
    // first-kill-ui: move-kill on a fresh SWAMP profile where every dialog a script
    // starts (tutorials, quest updates) is dismissed by a real touch on the
    // authored DialogBox, never flushed from the queue.
    bool first_kill_ui_scenario = false;
};

Options parse(int argc, char** argv) {
    Options options;
#ifdef DH2_DEFAULT_ASSET_DIR
    options.assets = std::filesystem::u8path(DH2_DEFAULT_ASSET_DIR);
#else
    options.assets = std::filesystem::current_path() / "port/android-native/app/src/main/assets";
#endif
    bool frames_supplied = false;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        auto value = [&](const char* name) -> std::string {
            if (++i >= argc) throw std::runtime_error(std::string("Missing value for ") + name);
            return argv[i];
        };
        if (arg == "--assets") options.assets = value("--assets");
        else if (arg == "--boss") options.boss_name = value("--boss");
        else if (arg == "--boss-offset") {
            const auto text = value("--boss-offset");
            if (std::sscanf(text.c_str(), "%f,%f,%f", &options.boss_offset[0], &options.boss_offset[1],
                            &options.boss_offset[2]) != 3)
                throw std::runtime_error("--boss-offset expects X,Y,Z");
            options.boss_offset_given = true;
        }
        else if (arg == "--boss-passive") options.boss_passive = true;
        else if (arg == "--boss-natural") options.boss_natural = true;
        else if (arg == "--boss-refill") options.boss_refill = true;
        else if (arg == "--boss-refill-hp") {
            options.boss_refill = true;
            options.boss_refill_hp = std::stoi(value("--boss-refill-hp"));
        }
        else if (arg == "--boss-summon-trap") options.boss_summon_trap = value("--boss-summon-trap");
        else if (arg == "--boss-summon-ally") options.boss_summon_ally = value("--boss-summon-ally");
        else if (arg == "--boss-intro") options.boss_intro_script = value("--boss-intro");
        else if (arg == "--boss-trigger") {
            const auto text = value("--boss-trigger");
            if (std::sscanf(text.c_str(), "%f,%f,%f", &options.boss_trigger[0], &options.boss_trigger[1],
                            &options.boss_trigger[2]) != 3)
                throw std::runtime_error("--boss-trigger expects X,Y,Z");
            options.boss_trigger_given = true;
        }
        else if (arg == "--boss-entry") {
            const auto text = value("--boss-entry");
            if (std::sscanf(text.c_str(), "%f,%f,%f", &options.boss_entry[0], &options.boss_entry[1],
                            &options.boss_entry[2]) != 3)
                throw std::runtime_error("--boss-entry expects X,Y,Z");
            options.boss_entry_given = true;
        }
        else if (arg == "--boss-weaken") options.boss_weaken = true;
        else if (arg == "--npc") options.npc_name = value("--npc");
        else if (arg == "--npc-distance") options.npc_distance = std::stof(value("--npc-distance"));
        else if (arg == "--zone") options.zone_name = value("--zone");
        else if (arg == "--watch-quest") options.watch_quest = std::stoi(value("--watch-quest"));
        else if (arg == "--post-death-frames") options.post_death_frames = std::stoull(value("--post-death-frames"));
        else if (arg == "--boss-distance") options.boss_distance = std::stof(value("--boss-distance"));
        else if (arg == "--runtime-dir") options.runtime = value("--runtime-dir");
        else if (arg == "--level-row") {
            const auto row = value("--level-row");
            std::size_t parsed = 0;
            const auto number = std::stoll(row, &parsed);
            if (parsed != row.size() || number < 0 ||
                number > std::numeric_limits<std::int32_t>::max())
                throw std::runtime_error("--level-row must be a nonnegative 32-bit integer");
            options.level_row = static_cast<std::int32_t>(number);
            if (options.level_row != 23 && !model_renderer::static_level_name_for_row(options.level_row))
                throw std::runtime_error("--level-row supports the authored static MLX rows and generated rule rows");
        }
        else if (arg == "--player-class") {
            const auto name = value("--player-class");
            if (name == "knight" || name == "warrior") {
                options.player_class = "KnightPlayerBase"; options.player_name = "Desktop Knight";
            } else if (name == "mage") {
                options.player_class = "MagePlayerBase"; options.player_name = "Desktop Mage";
            } else if (name == "rogue") {
                options.player_class = "RoguePlayerBase"; options.player_name = "Desktop Rogue";
            } else throw std::runtime_error("--player-class must be knight, mage or rogue");
        }
        else if (arg == "--transition-to") {
            // Development check: queue NativeGoToZone/ChangeLevel to LEVEL[:entry].
            const std::string spec = value("--transition-to");
            const auto colon = spec.find(':');
            options.transition_to = spec.substr(0, colon);
            if (colon != std::string::npos) options.transition_entry = std::stod(spec.substr(colon + 1));
        }
        else if (arg == "--transition-after-revive") options.transition_after_revive = true;
        else if (arg == "--transition-at") options.transition_frame = std::stoull(value("--transition-at"));
        else if (arg == "--return-to-origin") options.return_entry = std::stoi(value("--return-to-origin"));
        else if (arg == "--return-delay") options.return_delay = std::stoull(value("--return-delay"));
        else if (arg == "--enemy-census-at") options.census_frames.push_back(std::stoull(value("--enemy-census-at")));
        else if (arg == "--start-script") {
            std::string spec = value("--start-script");
            std::uint64_t frame = 200;
            if (const auto at = spec.rfind('@'); at != std::string::npos) {
                frame = std::stoull(spec.substr(at + 1));
                spec.resize(at);
            }
            options.script_starts.push_back({spec, frame});
        }
        else if (arg == "--play-anim") {
            std::string spec = value("--play-anim");
            std::uint64_t frame = 200;
            if (const auto at = spec.rfind('@'); at != std::string::npos) {
                frame = std::stoull(spec.substr(at + 1));
                spec.resize(at);
            }
            std::vector<std::string> parts;
            for (std::size_t begin = 0;;) {
                const auto colon = spec.find(':', begin);
                parts.push_back(spec.substr(begin, colon == std::string::npos ? colon : colon - begin));
                if (colon == std::string::npos) break;
                begin = colon + 1;
            }
            bool wait = false;
            if (!parts.empty() && parts.back() == "wait") {wait = true; parts.pop_back();}
            if (parts.size() < 2 || parts.size() > 3)
                throw std::runtime_error("--play-anim expects TARGET:DICT[:DICT2][:wait][@FRAME]");
            options.play_anims.push_back({parts[0], parts[1], parts.size() == 3 ? parts[2] : std::string(), wait, frame});
        }
        else if (arg == "--capture") options.capture = value("--capture");
        else if (arg == "--capture-initial") options.initial_capture = value("--capture-initial");
        else if (arg == "--faery-hud-tap") options.faery_hud_tap = true;
        else if (arg == "--flush-dialogs") options.flush_dialogs = true;
        else if (arg == "--hud-tap-xy") {
            const auto xy = value("--hud-tap-xy");
            const auto comma = xy.find(',');
            if (comma == std::string::npos) throw std::runtime_error("--hud-tap-xy expects X,Y");
            options.hud_tap_x = std::stoi(xy.substr(0, comma));
            options.hud_tap_y = std::stoi(xy.substr(comma + 1));
        }
        else if (arg == "--matrix-only") options.matrix_only = std::stoi(value("--matrix-only"));
        else if (arg == "--follow-faery") options.follow_faery_select = std::stoi(value("--follow-faery"));
        else if (arg == "--capture-series") options.capture_series = value("--capture-series");
        else if (arg == "--capture-every") options.capture_every = std::stoull(value("--capture-every"));
        else if (arg == "--scenario") {
            const auto scenario = value("--scenario");
            if (scenario == "move-attack") options.movement_attack_scenario = true;
            else if (scenario == "move-kill")
                options.movement_attack_scenario = options.movement_kill_scenario = true;
            else if (scenario == "move-defend") options.movement_defend_scenario = true;
            else if (scenario == "tutorial-potion")
                options.movement_defend_scenario = options.tutorial_potion_scenario = true;
            else if (scenario == "first-kill-ui")
                options.movement_attack_scenario = options.movement_kill_scenario =
                    options.first_kill_ui_scenario = true;
            else if (scenario == "skill-kill")
                options.movement_attack_scenario = options.movement_kill_scenario = options.skill_kill_scenario = true;
            else if (scenario == "touch-kill")
                options.movement_attack_scenario = options.movement_kill_scenario = options.hud_touch_scenario = true;
            else if (scenario == "faery-follow") options.faery_follow_scenario = true;
            else if (scenario == "faery-skill") options.faery_skill_scenario = true;
            else if (scenario == "skill-matrix") options.skill_matrix_scenario = true;
            else if (scenario == "boss-fight") options.boss_fight_scenario = true;
            else if (scenario == "talk-npc") options.talk_npc_scenario = true;
            else if (scenario == "enter-zone") options.enter_zone_scenario = true;
            else if (scenario == "fixed-cultist07") options.fixed_cultist_scenario = true;
            else if (scenario == "fixed-cultist07-kill") options.fixed_cultist_kill_scenario = true;
            else throw std::runtime_error("Unknown scenario: " + scenario);
        }
        else if (arg == "--show") options.show_window = true;
        else if (arg == "--frames") {
            const auto count = value("--frames");
            std::size_t parsed = 0;
            options.frames = std::stoull(count, &parsed);
            if (parsed != count.size() || options.frames == 0)
                throw std::runtime_error("--frames must be a positive integer");
            frames_supplied = true;
        }
        else if (arg == "--help") {
            std::cout << "DH2 desktop runtime\n"
                         "  --assets DIR       Android app asset tree\n"
                         "  --runtime-dir DIR  save/config output (default per-user AppData)\n"
                         "  --level-row N      skip the menu and start source row 4 (DARKWOOD), 23 (Crypt), 41 (SWAMP) or any generated rule row\n"
                         "  --player-class C   knight (default), mage or rogue: class of the fresh profile created for --level-row runs\n"
                         "  --frames N         exit after N rendered UI/gameplay frames (hidden window, no audio)\n"
                         "  --show             show the window and play audio for --frames runs\n"
                         "  --transition-to L[:E] queue NativeGoToZone to level L (entry E) at frame 300 (see --transition-at)\n"
                         "  --transition-at N     queue --transition-to at rendered frame N instead of 300\n"
                         "  --transition-after-revive  queue --transition-to 120 frames after the first Continue revive (move-defend/move-kill)\n"
                         "  --return-to-origin E  after --transition-to arrives, go back to the --level-row level at entry E (A -> B -> A) and log enemy censuses\n"
                         "  --return-delay N      frames to wait in the --transition-to destination before returning (default 240)\n"
                         "  --enemy-census-at N   log every materialized Monster and the dead ones at rendered frame N (repeatable)\n"
                         "  --start-script NAME[@N] start the named Level pyscript (e.g. Swamp_Intro) on the Level ScriptManager at frame N (default 200)\n"
                         "  --flush-dialogs     accepted for compatibility: gameplay scenarios flush authored cutscene dialogs every 20 frames by default (first-kill-ui excepted)\n"
                         "  --play-anim TARGET:DICT[:DICT2][:wait][@N] run Character::_RegisterAnim/_PlayAnim (AnimDict names) on 'player' or a Monster name at frame N\n"
                         "  --capture FILE.ppm save the last framebuffer as a PPM image\n"
                         "  --capture-initial FILE.ppm save the initial frame (for gameplay scenarios)\n"
                         "  --scenario move-attack steer with normal movement toward a source enemy, then attack\n"
                         "  --scenario move-kill   steer to enemies, attack until kills, then walk onto dropped loot\n"
                         "  --scenario move-defend approach enemies without attacking; verify damage, death and Continue revive\n"
                         "  --scenario tutorial-potion move-defend on SWAMP; verify HP <= half starts cinematic_Tuto_potionUse once\n"
                         "  --scenario touch-kill  move-kill driven through the HUD: a held joystick finger steers, a second finger holds the attack button while the stick stays down\n"
                         "  --scenario first-kill-ui move-kill on a fresh SWAMP profile; script dialogs are dismissed by touching the DialogBox (no flush); fails if the Player stays locked\n"
                         "  --scenario skill-kill  move-kill, but in melee range uses the HUD skill slot 0 instead of basic attacks\n"
                         "  --scenario faery-follow march the Player along far selected-floor routes; log Player/faery state every 60 frames\n"
                         "  --scenario faery-skill  near enemies, trigger the HUD faery ability (NativeHUDSpell) repeatedly and log its effects\n"
                         "  --scenario skill-matrix train/equip every class skill, use it in melee range of an enemy, log state/HP/MP effects\n"
                         "  --scenario boss-fight [--boss NAME] [--boss-offset X,Y,Z] [--boss-passive] [--boss-weaken] [--post-death-frames N] spawn the level boss, place the Player at its arena entry spot, fight it and log its AIS counters\n"
                         "  --scenario boss-fight [--boss NAME] [--boss-offset X,Y,Z] [--boss-passive] [--boss-weaken] spawn the level boss, place the Player at its arena entry spot, fight it and log its AIS counters\n"
                         "  --scenario boss-fight --boss-natural [--boss-entry X,Y,Z] do not spawn: walk into the authored intro trigger (SWAMP) or start the quest-state intro (DARKWOOD), follow the intro script until the boss is active, then fight\n"
                         "  --matrix-only N       skill-matrix: test only SkillTree row N\n"
                         "  --follow-faery N      faery-follow: select Faery Save slot N (0..4) at frame 30\n"
                         "  --hud-tap-xy X,Y      touch point for --faery-hud-tap on the 1280x720 layout (default 1176,281; skill slot 0 is 980,445)\n"
                         "  --faery-hud-tap       faery-skill: touch the authored HUD faery button instead of calling the spell entry\n"
                         "  --capture-series PREFIX [--capture-every N] save PREFIX_<frame>.ppm every N frames (any bounded gameplay run)\n"
                         "  --scenario fixed-cultist07 route to Crypt enemy 4; attack when nearest and in melee range\n"
                         "  --scenario fixed-cultist07-kill route to Crypt enemy 4; repeat normal attacks until death or frame bound\n"
                         "Controls: click/touch authored menus; type for character names; WASD move, Space attack, mouse drag orbit, wheel zoom, Esc quit.\n";
            std::exit(0);
        } else throw std::runtime_error("Unknown argument: " + arg);
    }
    if (options.tutorial_potion_scenario) {
        if (options.level_row < 0) options.level_row = 41;
        if (!frames_supplied) options.frames = 3000;
    }
    if (options.first_kill_ui_scenario) {
        if (options.level_row < 0) options.level_row = 41;
        if (!frames_supplied) options.frames = 4500;
    }
    const auto scenario_count = unsigned(options.movement_attack_scenario) +
        unsigned(options.fixed_cultist_scenario) + unsigned(options.fixed_cultist_kill_scenario) +
        unsigned(options.faery_follow_scenario) + unsigned(options.faery_skill_scenario) +
        unsigned(options.skill_matrix_scenario) + unsigned(options.boss_fight_scenario) +
        unsigned(options.talk_npc_scenario) + unsigned(options.enter_zone_scenario);
    if (scenario_count > 1) throw std::runtime_error("Select only one gameplay scenario per run");
    if (options.return_entry >= 0 &&
        (options.transition_to.empty() || options.level_row < 0 ||
         !model_renderer::static_level_name_for_row(options.level_row)))
        throw std::runtime_error("--return-to-origin needs --transition-to and a static --level-row");
    if (options.probe_scenario()) {
        if (options.level_row < 0) options.level_row = 41;
        if (options.boss_fight_scenario) {
            // The level's authored boss intro scripts (Swamp_Serpent_Intro, HG2_boss_intro)
            // put the Player on _prim_Waypoint_PrinceSpot. The port's Level ObjectManager does
            // not register Waypoint objects, so the scenario places the Player at that waypoint's
            // authored offset from the boss (MGP local positions: bossroom_ruins_ns_ boss
            // (-1535.8,-320.7) / PrinceSpot (-985.9,-930.5); darkwood_14_01 DarkQueen
            // (585.6,-212.7) / PrinceSpot (3.0,-711.2)).
            if (options.boss_name.empty()) {
                if (options.level_row == 41) options.boss_name = "_prim_Boss_SwampKing_01";
                else if (options.level_row == 4) options.boss_name = "_prim_DarkQueen";
                else if (options.level_row == 8) options.boss_name = "Madruk_A";
                else throw std::runtime_error("boss-fight needs --boss NAME for this level row");
            }
            if (options.boss_natural) {
                if (options.level_row == 41) {
                    // _prim_TriggerZone_17 (module 7 record 0): centre (-15437.4, 12000, -337.8),
                    // dimensions 200 x scale (1.91, 8.55, 6.11). The Player starts at the
                    // arena's PrinceSpot and walks east into it.
                    if (options.boss_intro_script.empty()) options.boss_intro_script = "Swamp_Serpent_Intro";
                    if (!options.boss_trigger_given) {
                        options.boss_trigger = {-15437.4f, 12000.0f, -114.0f};
                        options.boss_trigger_given = true;
                    }
                    if (!options.boss_entry_given) {
                        options.boss_entry = {-18985.0f, 11070.0f, -94.0f};
                        options.boss_entry_given = true;
                    }
                } else if (options.level_row == 8) {
                    // DARK_DUNGEON_TEMPLE_BOSSROOM _prim_TriggerZone -> Intro_boss, half
                    // extents (1162, 556, 100). The entry SpawnPoint's script
                    // (move_TriggerZone) puts the zone on _prim_Waypoint_Fix
                    // (0, -1120, 1003.8), 730 units north of the Player's start.
                    if (options.boss_intro_script.empty()) options.boss_intro_script = "Intro_boss";
                    if (!options.boss_trigger_given) {
                        options.boss_trigger = {0.0f, -1120.0f, 1004.0f};
                        options.boss_trigger_given = true;
                    }
                } else if (options.level_row == 4) {
                    // The Dark Queen's HG2_boss_intro is the Hell Gate quest's slot-10 script.
                    if (options.boss_intro_script.empty()) options.boss_intro_script = "HG2_boss_intro";
                }
            }
            if (!options.boss_offset_given) {
                if (options.level_row == 8) options.boss_offset = {-500.0f, 0.0f, 20.0f};
                else if (options.level_row == 41) options.boss_offset = {549.9f, -609.7f, 20.0f};
                else if (options.level_row == 4) options.boss_offset = {-582.6f, -498.5f, 20.0f};
                else throw std::runtime_error("boss-fight needs --boss-offset X,Y,Z for this level row");
            }
        }
        if (options.faery_follow_scenario && options.follow_exit_destination.empty()) {
            // Valid SpawnPoint entry ids from the authored .spwn files (SWAMP 0/3/13, DARKWOOD 10/3/11).
            if (options.level_row == 41) { options.follow_exit_destination = "DARKWOOD"; options.follow_exit_entry = 3.0; }
            else if (options.level_row == 4) { options.follow_exit_destination = "SWAMP"; options.follow_exit_entry = 0.0; }
        }
        if (!frames_supplied)
            options.frames = options.faery_follow_scenario ? 1800 :
                             options.faery_skill_scenario ? 2400 :
                             options.boss_fight_scenario ? 3000 : 12000;
        if (options.frames < 4) throw std::runtime_error("probe scenarios require at least four gameplay frames");
    } else if (options.movement_attack_scenario) {
        if (options.level_row < 0) options.level_row = 23;
        if (!frames_supplied) options.frames = 300;
        if (options.frames < 4) throw std::runtime_error("move-attack requires at least four gameplay frames");
    } else if (options.fixed_cultist_scenario) {
        if (options.level_row < 0) options.level_row = 23;
        if (options.level_row != 23) throw std::runtime_error("fixed-cultist07 requires Crypt row 23");
        if (!frames_supplied) options.frames = 2400;
        if (options.frames < 4) throw std::runtime_error("fixed-cultist07 requires at least four gameplay frames");
    } else if (options.fixed_cultist_kill_scenario) {
        if (options.level_row < 0) options.level_row = 23;
        if (options.level_row != 23) throw std::runtime_error("fixed-cultist07-kill requires Crypt row 23");
        if (!frames_supplied) options.frames = 6000;
        if (options.frames < 4) throw std::runtime_error("fixed-cultist07-kill requires at least four gameplay frames");
    } else if (!options.initial_capture.empty()) {
        throw std::runtime_error("--capture-initial requires a gameplay scenario");
    }
    return options;
}

void update_movement(const std::array<bool, 4>& keys) {
    const float x = (keys[3] ? 1.0f : 0.0f) - (keys[2] ? 1.0f : 0.0f);
    const float y = (keys[0] ? 1.0f : 0.0f) - (keys[1] ? 1.0f : 0.0f);
    const float scale = x != 0 && y != 0 ? 0.70710678f : 1.0f;
    model_renderer::move_axis(x * scale, y * scale);
}

void print_vitals() {
    const auto values = model_renderer::player_vitals();
    std::cout << "HP " << values[0] / 256.0f << '/' << values[1] / 256.0f
              << "  MP " << values[2] / 256.0f << '/' << values[3] / 256.0f
              << (values[4] ? "  defeated" : "") << '\n';
}

void print_vitals_snapshot(const char* label, const std::array<int, 7>& values) {
    std::cout << label << " HP " << values[0] / 256.0f << '/' << values[1] / 256.0f
              << " (" << (values[0] - values[1]) / 256.0f << " from max)"
              << " MP " << values[2] / 256.0f << '/' << values[3] / 256.0f
              << " state " << values[6] << (values[4] ? " defeated" : " alive") << '\n';
}

struct CombatReceiptSnapshot {
    std::array<int, 7> vitals{};
    model_renderer::UiPlayerStatsReadV1 stats;
    std::int32_t gold{};
    std::map<std::int32_t, std::int64_t> inventory_by_item;
};

bool read_combat_receipt(CombatReceiptSnapshot& output, std::string& error) {
    std::uintptr_t player = 0;
    if (!model_renderer::ui_player_identity(player, error)) return false;
    CombatReceiptSnapshot candidate;
    candidate.vitals = model_renderer::player_vitals();
    if (!model_renderer::ui_player_stats(player, candidate.stats, error) ||
        !model_renderer::ui_player_inventory_gold(player, candidate.gold, error)) return false;
    for (std::int32_t slot = 0; slot <= 9; ++slot) {
        std::vector<model_renderer::UiInventoryItemReadV1> items;
        if (!model_renderer::ui_player_inventory_slot(player, slot, items, error)) return false;
        for (const auto& item : items) candidate.inventory_by_item[item.id] += item.quantity;
    }
    output = std::move(candidate);
    error.clear();
    return true;
}

void print_combat_receipt(const char* label, const CombatReceiptSnapshot& value) {
    std::cout << label << " HP " << value.vitals[0] / 256.0f
              << " | XP " << value.stats.xp << " | level " << value.stats.level
              << " | gold " << value.gold << " | inventory item IDs "
              << value.inventory_by_item.size() << " (UI slots 0..9)\n";
}

void print_combat_receipt_delta(const CombatReceiptSnapshot& before,
                                const CombatReceiptSnapshot& after) {
    auto items = after.inventory_by_item;
    for (const auto& [id, quantity] : before.inventory_by_item) items[id] -= quantity;
    for (auto it = items.begin(); it != items.end();) {
        if (it->second == 0) it = items.erase(it);
        else ++it;
    }
    std::cout << "Combat receipt delta | HP "
              << (after.vitals[0] - before.vitals[0]) / 256.0f
              << " | XP " << (after.stats.xp - before.stats.xp)
              << " | level " << before.stats.level << " -> " << after.stats.level
              << " | gold " << (after.gold - before.gold) << " | item deltas";
    if (items.empty()) std::cout << " none";
    for (const auto& [id, quantity] : items) std::cout << ' ' << id << ':' << quantity;
    std::cout << " (UI slots 0..9)\n";
}

// --scenario touch-kill: steering drives the authored HUD joystick finger through
// this hook instead of calling move_axis. A (0, 0) request keeps the finger held.
std::function<void(float, float)> g_scenario_axis;
void scenario_move_axis(float x, float y) {
    if (g_scenario_axis) g_scenario_axis(x, y);
    else model_renderer::move_axis(x, y);
}

// The Android MotionEvent bridge hands each pointer to OriginalUiSession::pointer
// with a source finger index. This drives the same entry: finger 0 holds the
// authored joystick, finger 1 holds the authored attack button, both at once.
struct HudTouchDriver {
    static constexpr unsigned kAttackControl = 7;    // ui::AuthoredHudControlV1::attack
    static constexpr unsigned kJoystickControl = 8;  // ui::AuthoredHudControlV1::joystick
    static constexpr unsigned kStickFinger = 0, kAttackFinger = 1;
    dh2::android_ui::OriginalUiSession* session = nullptr;
    SDL_Window* window = nullptr;
    bool stick_down = false, attack_down = false, camera_down = false;
    float stick_x = 0, stick_y = 0, attack_x = 0, attack_y = 0, camera_x = 0, camera_y = 0;
    unsigned camera_drags = 0;
    static constexpr unsigned kCameraFinger = 2;
    // A free finger on the world while fingers 0 and 1 hold the HUD: it alone pans the camera.
    bool drag_camera_finger(std::string& error) {
        int width = 1, height = 1;
        size(width, height);
        if (!camera_down) {
            camera_x = 0.55f * float(width);
            camera_y = 0.3f * float(height);
            if (!session->pointer(camera_x, camera_y, 0, kCameraFinger, error)) return false;
            camera_down = true;
            return true;
        }
        camera_y += (camera_drags++ % 2 ? -1.0f : 1.0f) * 0.04f * float(height);
        return session->pointer(camera_x, camera_y, 2, kCameraFinger, error);
    }

    void size(int& width, int& height) const { SDL_GetWindowSizeInPixels(window, &width, &height); }
    bool drive_stick(float axis_x, float axis_y, std::string& error) {
        if (axis_x == 0.0f && axis_y == 0.0f) return true;
        int width = 1, height = 1;
        size(width, height);
        if (!stick_down) {
            if (!session->hud_control_screen_point(kJoystickControl, width, height, stick_x, stick_y, error))
                return false;
            if (!session->pointer(stick_x, stick_y, 0, kStickFinger, error)) return false;
            stick_down = true;
            std::cout << "Scenario touch | finger 0 pressed the HUD joystick at " << stick_x << ',' << stick_y << '\n';
        }
        // MovementControl axes are x right, y up; screen y points down. 0.2 of the
        // height is past the authored stick radius, so the drag is at full magnitude.
        const float reach = 0.2f * float(height);
        return session->pointer(stick_x + axis_x * reach, stick_y - axis_y * reach, 2, kStickFinger, error);
    }
    bool press_attack(std::string& error) {
        int width = 1, height = 1;
        size(width, height);
        if (!session->hud_control_screen_point(kAttackControl, width, height, attack_x, attack_y, error))
            return false;
        if (!session->pointer(attack_x, attack_y, 0, kAttackFinger, error)) return false;
        attack_down = true;
        return true;
    }
    bool release_all(std::string& error) {
        bool ok = true;
        if (camera_down) ok = session->pointer(camera_x, camera_y, 1, kCameraFinger, error) && ok;
        camera_down = false;
        if (attack_down) ok = session->pointer(attack_x, attack_y, 1, kAttackFinger, error) && ok;
        if (stick_down) ok = session->pointer(stick_x, stick_y, 1, kStickFinger, error) && ok;
        attack_down = stick_down = false;
        return ok;
    }
};

bool steer_toward_nearest_enemy() {
    model_renderer::GameplayPoseSnapshotV1 pose;
    std::string error;
    if (!model_renderer::gameplay_pose_snapshot(pose, error))
        throw std::runtime_error("Shared gameplay pose snapshot failed: " + error);
    model_renderer::GameplayRouteSnapshotV1 route;
    if (!model_renderer::gameplay_route_snapshot(pose.enemy_position, route, error))
        throw std::runtime_error("Selected-floor route query unavailable: " + error);
    std::cout << "Scenario selected-floor route | sewn " << route.graph_sewn
              << " | nodes " << route.graph_nodes << " | edges " << route.graph_edges
              << " | status " << route.query_status << " | found " << route.path_found
              << " | node pair " << route.source_node << " -> " << route.target_node
              << " | waypoints " << route.waypoints.size()
              << " | start " << pose.player_position[0] << ',' << pose.player_position[1]
              << ',' << pose.player_position[2] << " | enemy " << pose.enemy_position[0]
              << ',' << pose.enemy_position[1] << ',' << pose.enemy_position[2] << '\n';
    if (route.query_status != 0 || !route.path_found || route.waypoints.empty())
        throw std::runtime_error("Selected-floor route is unavailable; refusing direct steering fallback");
    // Skip waypoints the player already stands on; otherwise steering stalls
    // at the route's first node.
    std::size_t next = 0;
    while (next + 1 < route.waypoints.size() &&
           std::hypot(route.waypoints[next][0] - pose.player_position[0],
                      route.waypoints[next][1] - pose.player_position[1]) <= 48.0f)
        ++next;
    const auto& waypoint = route.waypoints[next];
    const float dx = waypoint[0] - pose.player_position[0];
    const float dy = waypoint[1] - pose.player_position[1];
    const float distance = std::hypot(dx, dy);
    if (!std::isfinite(distance))
        throw std::runtime_error("Selected-floor route produced a non-finite waypoint");
    if (distance <= 48.0f) {
        scenario_move_axis(0.0f, 0.0f);
        return true;
    }
    std::cout << "Scenario steering toward source enemy " << pose.enemy_index
              << " at " << pose.enemy_position[0] << ',' << pose.enemy_position[1]
              << " HP " << pose.enemy_hp << '/' << pose.enemy_max_hp
              << " via waypoint " << waypoint[0] << ',' << waypoint[1] << '\n';
    // move_axis accepts Android MovementControl screen axes. Invert its
    // recovered fixed +45-degree basis for this selected-floor world vector.
    constexpr float inverse_sqrt_two = 0.7071067811865475f;
    scenario_move_axis((dx + dy) * inverse_sqrt_two / distance,
                              (dy - dx) * inverse_sqrt_two / distance);
    return true;
}

bool steer_toward_fixed_position(const std::array<float, 3>& target,
                                 std::uint32_t& graph_nodes,
                                 std::uint32_t& graph_edges,
                                 std::uint32_t& query_status,
                                 bool& path_found,
                                 std::uint32_t& waypoint_count) {
    model_renderer::GameplayPoseSnapshotV1 pose;
    std::string error;
    if (!model_renderer::gameplay_pose_snapshot(pose, error))
        throw std::runtime_error("Shared fixed-target pose snapshot failed: " + error);
    model_renderer::GameplayRouteSnapshotV1 route;
    if (!model_renderer::gameplay_route_snapshot(target, route, error))
        throw std::runtime_error("Fixed-target selected-floor route query unavailable: " + error);
    graph_nodes = route.graph_nodes;
    graph_edges = route.graph_edges;
    query_status = route.query_status;
    path_found = route.path_found;
    waypoint_count = static_cast<std::uint32_t>(route.waypoints.size());
    std::cout << "Fixed target route | sewn " << route.graph_sewn
              << " | nodes " << route.graph_nodes << " | edges " << route.graph_edges
              << " | status " << route.query_status << " | found " << route.path_found
              << " | node pair " << route.source_node << " -> " << route.target_node
              << " | waypoints " << route.waypoints.size()
              << " | start " << pose.player_position[0] << ',' << pose.player_position[1]
              << ',' << pose.player_position[2] << " | target " << target[0] << ','
              << target[1] << ',' << target[2] << '\n';
    if (route.query_status != 0 || !route.path_found || route.waypoints.empty())
        return false;
    const auto& waypoint = route.waypoints.front();
    const float dx = waypoint[0] - pose.player_position[0];
    const float dy = waypoint[1] - pose.player_position[1];
    const float distance = std::hypot(dx, dy);
    if (!std::isfinite(distance))
        throw std::runtime_error("Fixed-target route produced a non-finite waypoint");
    if (distance <= 48.0f) {
        model_renderer::move_axis(0.0f, 0.0f);
        return true;
    }
    constexpr float inverse_sqrt_two = 0.7071067811865475f;
    model_renderer::move_axis((dx + dy) * inverse_sqrt_two / distance,
                              (dy - dx) * inverse_sqrt_two / distance);
    return true;
}

// ---------------------------------------------------------------------------
// Verification probes: faery-follow, faery-skill, skill-matrix.
// They drive the same public entries the HUD uses (move_axis, player_attack,
// NativeHUDSkill/NativeHUDSpell equivalents, NativeSkillsTrainSkill, ...) and
// observe through the public read-only snapshots. They own no gameplay state.
// ---------------------------------------------------------------------------
using Vec3 = std::array<float, 3>;

std::string fmt(const char* format, ...) {
    char text[1024];
    va_list args;
    va_start(args, format);
    std::vsnprintf(text, sizeof text, format, args);
    va_end(args);
    return text;
}

void plog(const std::string& line) { std::cout << "PROBE " << line << std::endl; }

float planar_distance(const Vec3& a, const Vec3& b) { return std::hypot(a[0] - b[0], a[1] - b[1]); }

std::string vec_text(const Vec3& v) { return fmt("%.0f,%.0f,%.0f", v[0], v[1], v[2]); }

bool probe_route(const Vec3& target, model_renderer::GameplayRouteSnapshotV1& route) {
    std::string error;
    if (!model_renderer::gameplay_route_snapshot(target, route, error)) return false;
    return route.query_status == 0 && route.path_found && !route.waypoints.empty();
}

// World-space direction -> move_axis (inverts its recovered +45 degree basis).
void steer_world(float dx, float dy) {
    const float distance = std::hypot(dx, dy);
    if (!(distance > 0.0f) || !std::isfinite(distance)) {
        model_renderer::move_axis(0.0f, 0.0f);
        return;
    }
    constexpr float inverse_sqrt_two = 0.7071067811865475f;
    model_renderer::move_axis((dx + dy) * inverse_sqrt_two / distance,
                              (dy - dx) * inverse_sqrt_two / distance);
}

void halt_player() {
    model_renderer::move_axis(0.0f, 0.0f);
    model_renderer::stop_movement();
}

enum class March { moving, arrived, no_route };

March march_to(const Vec3& player, const Vec3& target, float arrive_radius) {
    model_renderer::GameplayRouteSnapshotV1 route;
    if (!probe_route(target, route)) return March::no_route;
    if (planar_distance(route.waypoints.back(), player) <= arrive_radius) {
        model_renderer::move_axis(0.0f, 0.0f);
        return March::arrived;
    }
    std::size_t next = 0;
    while (next + 1 < route.waypoints.size() &&
           planar_distance(route.waypoints[next], player) <= 48.0f)
        ++next;
    steer_world(route.waypoints[next][0] - player[0], route.waypoints[next][1] - player[1]);
    return March::moving;
}

struct EnemySum {
    int count = 0, dead = 0;
    double hp = 0.0;
};

// Every live-world Monster record that the public enemy snapshot exposes.
EnemySum all_enemy_hp() {
    EnemySum sum;
    for (std::int32_t index = 0; index < 4096; ++index) {
        model_renderer::GameplayEnemyStateSnapshotV1 enemy;
        std::string error;
        if (!model_renderer::gameplay_enemy_state_snapshot(index, enemy, error)) continue;
        ++sum.count;
        if (enemy.dead || enemy.hp <= 0) ++sum.dead;
        else sum.hp += enemy.hp / 256.0;
    }
    return sum;
}

// Names every Character-sheet value the public stats read exposes that differs.
std::string stats_diff(const model_renderer::UiPlayerStatsReadV1& a, const model_renderer::UiPlayerStatsReadV1& b) {
    std::string out;
#define DIFF(field) if (a.field != b.field) out += fmt(" " #field " %d->%d;", int(a.field), int(b.field));
    DIFF(max_hp) DIFF(max_mp) DIFF(strength) DIFF(dexterity) DIFF(endurance) DIFF(energy) DIFF(points)
    DIFF(rating_attack) DIFF(rating_critical) DIFF(rating_defense) DIFF(rating_dodge) DIFF(rating_block)
    DIFF(resistance_fire) DIFF(resistance_earth) DIFF(resistance_water) DIFF(resistance_air) DIFF(resistance_lightning)
    DIFF(damage_min_main_hand) DIFF(damage_max_main_hand) DIFF(damage_elemental_min_main_hand)
    DIFF(damage_elemental_max_main_hand) DIFF(damage_elemental_type_main_hand)
    DIFF(damage_fire_min_main_hand) DIFF(damage_fire_max_main_hand) DIFF(damage_water_min_main_hand)
    DIFF(damage_lightning_min_main_hand) DIFF(damage_air_min_main_hand) DIFF(damage_earth_min_main_hand)
    DIFF(menu_average_melee_to_hit) DIFF(physical_armor) DIFF(spell_rating_dodge) DIFF(menu_melee_damage_reduction)
    DIFF(spell_rating_critical) DIFF(menu_average_spell_to_hit)
    DIFF(spell_damage_bonus_fire) DIFF(spell_damage_bonus_earth) DIFF(spell_damage_bonus_water)
    DIFF(spell_damage_bonus_air) DIFF(spell_damage_bonus_lightning)
    DIFF(regen_hp) DIFF(regen_mp) DIFF(leech_hp) DIFF(leech_mp) DIFF(special_loot_gold_multiplier)
    DIFF(special_loot_magical_chance) DIFF(stun_resist_chance)
#undef DIFF
    return out.empty() ? " none" : out;
}

class ProbeRunner {
public:
    ProbeRunner(const Options& options, bool& died, bool& revived)
        : options_(options), died_(died), revived_(revived) {}

    void tick(std::uint64_t frame, std::uint64_t milliseconds) {
        frame_ = frame;
        milliseconds_ = milliseconds;
        if (!ready_ && !initialize()) return;
        if (!refresh_player()) return;
        if (options_.faery_follow_scenario) follow_tick();
        else if (options_.faery_skill_scenario) faery_skill_tick();
        else if (options_.skill_matrix_scenario) matrix_tick();
        else if (options_.boss_fight_scenario) boss_tick();
        else if (options_.talk_npc_scenario || options_.enter_zone_scenario) quest_tick();
    }

    void finish() {
        if (!ready_) {
            plog("finish | scenario never initialised (no player identity)");
            return;
        }
        if (options_.faery_follow_scenario) follow_finish();
        else if (options_.faery_skill_scenario) faery_skill_finish();
        else if (options_.skill_matrix_scenario) matrix_finish();
        else if (options_.boss_fight_scenario) boss_finish();
        else if (options_.talk_npc_scenario || options_.enter_zone_scenario) quest_finish();
    }

    bool failed() const { return !failure_.empty(); }
    // The matrix / faery-skill scenarios stop the run themselves once their
    // fixed work list is exhausted (they have no use for the remaining frames).
    bool finished() const {
        return (options_.boss_fight_scenario && death_frame_ && frame_ >= death_frame_ + 15) ||
               (options_.skill_matrix_scenario && mx_phase_ == MxPhase::done) ||
               (options_.faery_skill_scenario && fs_phase_ == FsPhase::done) ||
               ((options_.talk_npc_scenario || options_.enter_zone_scenario) &&
                qs_phase_ == QuestScenarioPhase::done && frame_ >= qs_done_frame_ + 300) ||
               (options_.boss_fight_scenario && bf_phase_ == BossPhase::done &&
                frame_ >= bf_done_frame_ + options_.post_death_frames);
    }
    // HUD touch hand-off for --faery-hud-tap: action 0 = press, 1 = release.
    bool take_tap(int& action) {
        if (!tap_queue_.empty() && frame_ >= tap_queue_.front().first) {
            action = tap_queue_.front().second;
            tap_queue_.erase(tap_queue_.begin());
            return true;
        }
        return false;
    }
    // HUD attack-button hand-off for talk-npc: 0 = press, 1 = release.
    bool take_attack_touch(int& action) {
        if (qs_touch_ < 0) return false;
        action = qs_touch_;
        qs_touch_ = -1;
        return true;
    }
    const std::string& failure() const { return failure_; }
    // An authored cutscene hides the HUD and locks the controllers: probe HUD casts
    // wait for it to end, and their timers pause instead of counting toward a verdict.
    void set_hud_blocked(bool blocked) { hud_blocked_ = blocked; }

private:
    // ----- shared -----------------------------------------------------------
    const Options& options_;
    bool& died_;
    bool& revived_;
    std::uint64_t frame_ = 0, milliseconds_ = 0;
    bool hud_blocked_ = false;
    bool ready_ = false;
    std::uintptr_t player_ = 0;
    std::string error_, failure_;
    model_renderer::GameplayPoseSnapshotV1 pose_;
    bool have_pose_ = false, pose_valid_once_ = false;
    model_renderer::UiPlayerHudProjectionV1 hud_{};
    unsigned deaths_ = 0;
    std::uint64_t death_frame_ = 0;
    std::uint64_t first_ms_ = 0;
    std::vector<std::pair<std::uint64_t, int>> tap_queue_;  // (frame, 2 move / 0 press / 1 release)

    bool initialize() {
        if (!model_renderer::ui_player_identity(player_, error_) || !player_) return false;
        ready_ = true;
        first_ms_ = milliseconds_;
        plog(fmt("init | frame %llu | player identity %llu | level row %d | scenario %s",
                 (unsigned long long)frame_, (unsigned long long)player_, options_.level_row,
                 options_.faery_follow_scenario ? "faery-follow" :
                 options_.faery_skill_scenario ? "faery-skill" :
                 options_.boss_fight_scenario ? "boss-fight" :
                 options_.talk_npc_scenario ? "talk-npc" :
                 options_.enter_zone_scenario ? "enter-zone" : "skill-matrix"));
        return true;
    }

    // Player death/revive bookkeeping shared with the Continue-tap block in main().
    bool refresh_player() {
        const auto vitals = model_renderer::player_vitals();
        if (vitals[4] != 0) {
            if (!died_ || revived_) {
                ++deaths_;
                if (!death_frame_) death_frame_ = frame_;
                plog(fmt("player died | frame %llu | deaths %u", (unsigned long long)frame_, deaths_));
            }
            died_ = true;
            revived_ = false;
            have_pose_ = false;
            return false;
        }
        if (died_ && !revived_) {
            revived_ = true;
            plog(fmt("player revived | frame %llu | HP %.1f", (unsigned long long)frame_,
                     vitals[0] / 256.0));
        }
        // The pose scan walks every world object; refresh it on the 15-frame
        // cadence the other scenarios use and keep the last value in between.
        if (frame_ % 15 == 0 || !pose_valid_once_) {
            have_pose_ = model_renderer::gameplay_pose_snapshot(pose_, error_);
            pose_valid_once_ = true;
        }
        return true;
    }

    float seconds() const { return float(milliseconds_ - first_ms_) / 1000.0f; }

    bool read_hud(bool refresh = true) {
        std::string error;
        return model_renderer::ui_player_hud_projection(player_, refresh, hud_, error);
    }

    // ----- faery-follow -----------------------------------------------------
    bool leg_active_ = false;
    Vec3 leg_start_{}, leg_end_{}, leg_progress_pos_{}, start_pos_{}, last_sample_pos_{};
    std::uint64_t leg_progress_frame_ = 0;
    unsigned legs_ = 0;
    double total_path_ = 0.0, max_from_start_ = 0.0;
    bool have_start_ = false;
    unsigned fight_attacks_ = 0, samples_ = 0;
    // Faery companion observations (model_renderer::gameplay_faery_snapshot).
    // The follower AI keeps the Faery near its master: it walks (MoveTo) when
    // the master is out of interaction range and warps behind the master once
    // the master leaves its 2300 unit view radius, so the distance stays
    // bounded however far the Player marches.
    static constexpr double faery_distance_bound_ = 2500.0;
    static constexpr std::uint64_t faery_recovery_frames_ = 120;
    bool follow_was_locked_ = false;
    std::uint64_t follow_unlock_frame_ = 0;
    unsigned faery_locked_samples_ = 0;
    unsigned faery_samples_ = 0, faery_moving_samples_ = 0, faery_unbounded_ = 0;
    unsigned faery_no_master_ = 0;
    double faery_max_distance_ = 0.0, faery_total_move_ = 0.0;
    Vec3 faery_last_{};
    bool have_faery_last_ = false;
    std::array<std::uint32_t, 8> faery_events_{};
    std::uint32_t faery_ai_frames_ = 0;

    bool pick_leg(const Vec3& player) {
        constexpr int directions = 16;
        constexpr float radii[] = {900.0f, 1800.0f, 3000.0f, 4500.0f};
        double best_score = 0.0;
        Vec3 best_end{};
        bool found = false;
        const Vec3 avoid = legs_ ? leg_start_ : player;
        for (const float radius : radii) {
            for (int d = 0; d < directions; ++d) {
                const float angle = 6.2831853f * float(d) / float(directions);
                const Vec3 candidate{player[0] + radius * std::cos(angle),
                                     player[1] + radius * std::sin(angle), player[2]};
                model_renderer::GameplayRouteSnapshotV1 route;
                if (!probe_route(candidate, route)) continue;
                const Vec3& end = route.waypoints.back();
                double score = planar_distance(end, player);
                if (score < 600.0) continue;
                if (planar_distance(end, avoid) < 1000.0) score *= 0.25;
                if (score > best_score) {
                    best_score = score;
                    best_end = end;
                    found = true;
                }
            }
        }
        if (!found) return false;
        leg_start_ = player;
        leg_end_ = best_end;
        leg_progress_pos_ = player;
        leg_progress_frame_ = frame_;
        leg_active_ = true;
        ++legs_;
        plog(fmt("follow leg %u | frame %llu | from %s | to %s | straight %.0f",
                 legs_, (unsigned long long)frame_, vec_text(player).c_str(),
                 vec_text(best_end).c_str(), planar_distance(best_end, player)));
        return true;
    }

    void follow_tick() {
        if (hud_blocked_) {
            follow_was_locked_ = true;
        } else if (follow_was_locked_) {
            follow_was_locked_ = false;
            follow_unlock_frame_ = frame_;
        }
        if (have_pose_ && !have_start_) {
            have_start_ = true;
            start_pos_ = last_sample_pos_ = pose_.player_position;
            plog(fmt("follow start | player %s", vec_text(start_pos_).c_str()));
        }
        if (options_.follow_faery_select >= 0 && frame_ == 30) {
            std::string select_error;
            const bool selected = model_renderer::ui_player_set_active_faery(
                player_, std::uint32_t(options_.follow_faery_select), select_error);
            plog(fmt("follow select Faery %d (ChangeFaery + PlaceFaeryAndFollowers): %s%s",
                     options_.follow_faery_select, selected ? "ok" : "ERROR: ", select_error.c_str()));
            if (!selected && failure_.empty())
                failure_ = "faery-follow: selecting the Faery failed: " + select_error;
        }
        if (frame_ % 15 == 0) follow_decide();
        if (frame_ % 60 == 0) follow_sample();
    }

    void follow_decide() {
        if (!have_pose_) return;
        if (hud_blocked_) {
            // Locked behind an authored cutscene: the walk and stuck timers pause.
            leg_progress_frame_ = frame_;
            return;
        }
        const Vec3& player = pose_.player_position;
        if (pose_.enemy_index >= 0 && pose_.enemy_hp > 0 && pose_.enemy_in_melee_range) {
            halt_player();
            model_renderer::player_attack();
            ++fight_attacks_;
            return;
        }
        bool need_leg = !leg_active_;
        if (leg_active_ && frame_ - leg_progress_frame_ >= 240) {
            if (planar_distance(player, leg_progress_pos_) < 100.0f) {
                plog(fmt("follow leg %u stuck | frame %llu | at %s", legs_,
                         (unsigned long long)frame_, vec_text(player).c_str()));
                need_leg = true;
            } else {
                leg_progress_frame_ = frame_;
                leg_progress_pos_ = player;
            }
        }
        if (need_leg && !pick_leg(player)) {
            plog(fmt("follow no reachable far leg from %s", vec_text(player).c_str()));
            halt_player();
            leg_active_ = false;
            return;
        }
        const auto result = march_to(player, leg_end_, 150.0f);
        if (result == March::arrived) {
            plog(fmt("follow leg %u arrived | frame %llu", legs_, (unsigned long long)frame_));
            leg_active_ = false;
        } else if (result == March::no_route) {
            leg_active_ = false;
        }
    }

    void follow_sample() {
        if (!have_pose_) return;
        const Vec3& player = pose_.player_position;
        const double moved = planar_distance(player, last_sample_pos_);
        total_path_ += moved;
        const double from_start = planar_distance(player, start_pos_);
        max_from_start_ = std::max(max_from_start_, from_start);
        last_sample_pos_ = player;
        std::int32_t faery_id = -1, faery_level = -1;
        std::string error;
        model_renderer::ui_player_active_faery(player_, faery_id, faery_level, error);
        const bool hud_ok = read_hud();
        const auto vitals = model_renderer::player_vitals();
        ++samples_;
        plog(fmt("follow sample | frame %llu | t %.1fs | player %s | moved %.0f | total %.0f | "
                 "from-start %.0f | leg %u -> %.0f | state %d | HP %.1f/%.1f MP %.1f | "
                 "faery id %d level %d | HUD faery cd %d%% usable %d%s | nearest enemy %d hp %.1f melee %d | "
                 "attacks %u | deaths %u",
                 (unsigned long long)frame_, seconds(), vec_text(player).c_str(), moved, total_path_,
                 from_start, legs_, leg_active_ ? planar_distance(player, leg_end_) : 0.0f,
                 model_renderer::gameplay_player_state(), vitals[0] / 256.0, vitals[1] / 256.0,
                 vitals[2] / 256.0, faery_id, faery_level, hud_.infos[6], hud_.infos[7],
                 hud_ok ? "" : " (hud read failed)", pose_.enemy_index, pose_.enemy_hp / 256.0,
                 int(pose_.enemy_in_melee_range), fight_attacks_, deaths_));
        follow_faery_sample();
    }

    void follow_faery_sample() {
        model_renderer::GameplayFaerySnapshotV1 faery;
        std::string error;
        if (!model_renderer::gameplay_faery_snapshot(faery, error)) {
            if (failure_.empty()) failure_ = "faery-follow: gameplay_faery_snapshot failed: " + error;
            return;
        }
        if (!faery.present) {
            plog(fmt("follow faery | frame %llu | no authored Faery in this Level", (unsigned long long)frame_));
            return;
        }
        ++faery_samples_;
        faery_events_ = faery.master_events;
        faery_ai_frames_ = faery.ai_frames;
        faery_max_distance_ = std::max<double>(faery_max_distance_, faery.player_distance);
        if (have_faery_last_) faery_total_move_ += planar_distance(faery.position, faery_last_);
        faery_last_ = faery.position;
        have_faery_last_ = true;
        if (faery.state == 4) ++faery_moving_samples_;
        if (!faery.master_is_player && frame_ > 120) ++faery_no_master_;
        // The first seconds allow the Faery's first AI ticks (master acquisition).
        // While LockCharacter("All") holds the controllers, CharAI::Update returns
        // before _UpdateMaster (0x3cfbf4), so the Faery cannot see or warp to a
        // Player that a cutscene moved. The bound applies again once the lock has
        // been released for faery_recovery_frames_.
        const bool recovering = follow_was_locked_ ||
                                frame_ - follow_unlock_frame_ < faery_recovery_frames_;
        if (recovering) ++faery_locked_samples_;
        if (frame_ > 180 && !recovering && faery.player_distance > faery_distance_bound_) {
            ++faery_unbounded_;
            if (failure_.empty())
                failure_ = fmt("faery-follow: Faery is %.0f units from the Player at frame %llu "
                               "(bound %.0f)", faery.player_distance, (unsigned long long)frame_,
                               faery_distance_bound_);
        }
        plog(fmt("follow faery | frame %llu | faery %s | dist %.0f | state %d | master %s%s | path %u | "
                 "moving %d | master events died %u revived %u out-of-sight %u in-sight %u out-of-range %u "
                 "ranged %u close %u melee %u | AI frames %u | visual faery %d",
                 (unsigned long long)frame_, vec_text(faery.position).c_str(), faery.player_distance,
                 faery.state, faery.master ? "set" : "none", faery.master_is_player ? " (Player)" : "",
                 faery.path_segments, int(faery.moving), faery.master_events[0], faery.master_events[1],
                 faery.master_events[2], faery.master_events[3], faery.master_events[4],
                 faery.master_events[5], faery.master_events[6], faery.master_events[7],
                 faery.ai_frames, faery.visual_faery_id));
    }

    void follow_finish() {
        plog(fmt("follow faery summary | samples %u | moving samples %u | max distance %.0f | total travel %.0f | "
                 "samples beyond %.0f: %u | samples without Player master %u | WarpBehind (out-of-sight) %u | "
                 "MoveTo ticks (out-of-range) %u | Stop ticks (melee range) %u | locked/recovering samples %u",
                 faery_samples_, faery_moving_samples_, faery_max_distance_, faery_total_move_,
                 faery_distance_bound_, faery_unbounded_, faery_no_master_, faery_events_[2], faery_events_[4],
                 faery_events_[7], faery_locked_samples_));
        if (failure_.empty()) {
            if (!faery_samples_)
                failure_ = "faery-follow: this Level has no authored Faery to observe";
            else if (faery_no_master_)
                failure_ = "faery-follow: the Faery never acquired the Player as its master";
            else if (total_path_ > 3000.0 && faery_total_move_ < 500.0)
                failure_ = fmt("faery-follow: the Player marched %.0f units but the Faery travelled only %.0f",
                               total_path_, faery_total_move_);
        }
        plog(fmt("follow summary | samples %u | legs %u | path %.0f | max distance from start %.0f | "
                 "melee attacks %u | deaths %u",
                 samples_, legs_, total_path_, max_from_start_, fight_attacks_, deaths_));
    }

    // ----- boss-fight -------------------------------------------------------
    // Spawns the named boss through the development SpawnCharacter request (the
    // original lets the Level's intro scripts do it), puts the Player on the
    // arena waypoint with the Level ScriptManager's SetActorPosition provider,
    // then closes in and attacks while logging the boss's AIS VM counters, FSM
    // state, target and both sides' health.
    enum class BossPhase { spawn, wait_init, place, fight, done, nat_place, nat_walk, nat_intro };
    BossPhase bf_phase_ = BossPhase::spawn;
    std::uint64_t bf_phase_frame_ = 0, bf_done_frame_ = 0;
    bool bf_ever_initialized_ = false, bf_ever_targeted_ = false, bf_boss_died_ = false,
         bf_trap_summoned_ = false, bf_ally_summoned_ = false;
    unsigned bf_refills_ = 0;
    double bf_player_hp_start_ = -1.0, bf_player_hp_min_ = 1e9, bf_boss_hp_start_ = -1.0,
           bf_boss_hp_min_ = 1e9;
    std::uint32_t bf_max_callbacks_ = 0, bf_max_failed_ = 0, bf_max_commands_ = 0,
                  bf_max_events_ = 0, bf_attacks_ = 0;
    std::string bf_last_state_;
    int bf_last_fsm_ = -2, bf_index_ = -1;
    std::string bf_last_attack_;
    // --boss-natural bookkeeping.
    bool bf_intro_started_ = false, bf_intro_completed_ = false;
    std::uint64_t bf_intro_start_frame_ = 0, bf_intro_end_frame_ = 0;
    std::string bf_last_running_;
    std::uint32_t bf_max_running_ = 0;

    void boss_tick() {
        const std::string& boss = options_.boss_name;
        model_renderer::BossProbeSnapshotV1 snap;
        std::string error;
        const bool have = model_renderer::boss_probe_snapshot(boss, snap, error);
        const auto vitals = model_renderer::player_vitals();
        const double player_hp = vitals[0] / 256.0;
        if (bf_player_hp_start_ < 0.0) bf_player_hp_start_ = player_hp;
        bf_player_hp_min_ = std::min(bf_player_hp_min_, player_hp);
        if (have) {
            bf_ever_initialized_ = bf_ever_initialized_ || snap.initialized;
            bf_ever_targeted_ = bf_ever_targeted_ || snap.target != 0;
            bf_max_callbacks_ = std::max(bf_max_callbacks_, snap.completed_callbacks);
            bf_max_failed_ = std::max(bf_max_failed_, snap.failed_callbacks);
            bf_max_commands_ = std::max(bf_max_commands_, snap.skill_commands);
            bf_max_events_ = std::max(bf_max_events_, snap.skill_events);
            const double boss_hp = snap.hp / 256.0;
            if (bf_boss_hp_start_ < 0.0 && snap.fsm_state > 0) bf_boss_hp_start_ = boss_hp;
            if (snap.fsm_state > 0) bf_boss_hp_min_ = std::min(bf_boss_hp_min_, boss_hp);
            if (snap.dead) bf_boss_died_ = true;
            if (snap.animation_state != bf_last_state_ || snap.fsm_state != bf_last_fsm_) {
                plog(fmt("boss transition | frame %llu | fsm %d -> %d | state '%s' -> '%s' | HP %.1f/%.1f",
                         (unsigned long long)frame_, bf_last_fsm_, snap.fsm_state,
                         bf_last_state_.c_str(), snap.animation_state.c_str(), snap.hp / 256.0,
                         snap.max_hp / 256.0));
                bf_last_state_ = snap.animation_state;
                bf_last_fsm_ = snap.fsm_state;
            }
        }
        const auto since = frame_ - bf_phase_frame_;
        switch (bf_phase_) {
        case BossPhase::nat_place: {
            if (options_.boss_trigger_given) {
                if (!options_.boss_entry_given) {
                    plog(fmt("boss natural | frame %llu | Player walks from the Level entry to the intro trigger",
                             (unsigned long long)frame_));
                    bf_phase_ = BossPhase::nat_walk;
                    bf_phase_frame_ = frame_;
                    return;
                }
                std::string place_error;
                const Vec3 spot{options_.boss_entry[0], options_.boss_entry[1], options_.boss_entry[2]};
                const bool placed = model_renderer::boss_probe_place_player(spot, place_error);
                plog(fmt("boss natural | frame %llu | Player placed at the arena approach %s: %s%s",
                         (unsigned long long)frame_, vec_text(spot).c_str(), placed ? "ok" : "ERROR ",
                         place_error.c_str()));
                if (!placed) {
                    failure_ = "boss-fight: placing the Player failed: " + place_error;
                    return;
                }
                bf_phase_ = BossPhase::nat_walk;
            } else {
                // DARKWOOD: HG2_boss_intro is the Dark Queen quest's slot-10 script, run by
                // Quest::SetState(5) -> Quest::ExecScript(10) -> ScriptManager::StartScript.
                std::string start_error;
                const bool started = model_renderer::boss_probe_start_level_script(
                    options_.boss_intro_script, start_error);
                plog(fmt("boss natural | frame %llu | quest-state stand-in starts %s: %s%s",
                         (unsigned long long)frame_, options_.boss_intro_script.c_str(),
                         started ? "ok" : "ERROR ", start_error.c_str()));
                if (!started) {
                    failure_ = "boss-fight: starting the intro script failed: " + start_error;
                    return;
                }
                bf_phase_ = BossPhase::nat_intro;
            }
            bf_phase_frame_ = frame_;
            return;
        }
        case BossPhase::nat_walk: {
            if (frame_ % 15 != 0) return;
            model_renderer::LevelScriptProbeV1 probe;
            std::string probe_error;
            if (model_renderer::level_script_probe(probe, probe_error) &&
                std::find(probe.started.begin(), probe.started.end(), options_.boss_intro_script) !=
                    probe.started.end()) {
                bf_intro_started_ = true;
                bf_intro_start_frame_ = frame_;
                halt_player();
                plog(fmt("boss natural | frame %llu | %s started by its TriggerZone | Player at %s",
                         (unsigned long long)frame_, options_.boss_intro_script.c_str(),
                         have_pose_ ? vec_text(pose_.player_position).c_str() : "?"));
                bf_phase_ = BossPhase::nat_intro;
                bf_phase_frame_ = frame_;
                return;
            }
            if (have_pose_) {
                const Vec3 target{options_.boss_trigger[0], options_.boss_trigger[1],
                                  options_.boss_trigger[2]};
                if (march_to(pose_.player_position, target, 40.0f) != March::moving)
                    steer_world(target[0] - pose_.player_position[0],
                                target[1] - pose_.player_position[1]);
            }
            if (frame_ % 120 == 0 && have_pose_) {
                model_renderer::GameplayRouteSnapshotV1 route;
                const bool probed = probe_route({options_.boss_trigger[0], options_.boss_trigger[1],
                                                 options_.boss_trigger[2]}, route);
                plog(fmt("boss natural | route probe %d | found %d graph sewn %d nodes %u src %u dst %u status %u waypoints %zu",
                         int(probed), int(route.path_found), int(route.graph_sewn), route.graph_nodes,
                         route.source_node, route.target_node, route.query_status, route.waypoints.size()));
            }
            if (frame_ % 120 == 0)
                plog(fmt("boss natural | frame %llu | walking to the intro trigger | Player at %s | distance %.0f",
                         (unsigned long long)frame_,
                         have_pose_ ? vec_text(pose_.player_position).c_str() : "?",
                         have_pose_ ? planar_distance(pose_.player_position,
                                          {options_.boss_trigger[0], options_.boss_trigger[1],
                                           options_.boss_trigger[2]}) : -1.0f));
            if (since > 1500) failure_ = "boss-fight: the Player never triggered the boss intro script";
            return;
        }
        case BossPhase::nat_intro: {
            if (frame_ % 15 != 0) return;
            model_renderer::LevelScriptProbeV1 probe;
            std::string probe_error;
            if (!model_renderer::level_script_probe(probe, probe_error)) return;
            const bool started = std::find(probe.started.begin(), probe.started.end(),
                                           options_.boss_intro_script) != probe.started.end();
            const bool completed = std::find(probe.completed.begin(), probe.completed.end(),
                                             options_.boss_intro_script) != probe.completed.end();
            if (started && !bf_intro_started_) {
                bf_intro_started_ = true;
                bf_intro_start_frame_ = frame_;
            }
            std::string running;
            for (const auto& name : probe.running) running += (running.empty() ? "" : ",") + name;
            bf_max_running_ = std::max<std::uint32_t>(bf_max_running_, probe.tasks_active);
            if (running != bf_last_running_) {
                plog(fmt("boss intro | frame %llu | running [%s] | scripted %d | cutscene %d | blocked %d | boss fsm %d '%s'",
                         (unsigned long long)frame_, running.c_str(), int(probe.player_scripted),
                         int(probe.cutscene_mode), int(probe.controller_blocked),
                         have ? snap.fsm_state : -1, have ? snap.animation_state.c_str() : "?"));
                bf_last_running_ = running;
            }
            if (completed && !bf_intro_completed_) {
                bf_intro_completed_ = true;
                bf_intro_end_frame_ = frame_;
                plog(fmt("boss intro | frame %llu | %s completed after %llu frames",
                         (unsigned long long)frame_, options_.boss_intro_script.c_str(),
                         (unsigned long long)(frame_ - bf_intro_start_frame_)));
            }
            if (bf_intro_completed_ && have && snap.fsm_state > 0) {
                plog(fmt("boss active | frame %llu | script %s | fsm %d '%s' | HP %.1f/%.1f | at %s | source visible %d renderer visible %d",
                         (unsigned long long)frame_, snap.script.c_str(), snap.fsm_state,
                         snap.animation_state.c_str(), snap.hp / 256.0, snap.max_hp / 256.0,
                         vec_text({snap.position[0], snap.position[1], snap.position[2]}).c_str(),
                         int(snap.source_visible), int(snap.renderer_visible)));
                if (options_.boss_weaken && snap.hp > 3 * 256) {
                    const std::string hit = model_renderer::debug_character_hit(
                        boss, std::uint32_t(snap.hp - 3 * 256));
                    plog(fmt("boss weaken | HP %.1f -> 3 | %s", snap.hp / 256.0, hit.c_str()));
                }
                bf_phase_ = BossPhase::fight;
                bf_phase_frame_ = frame_;
            } else if (since > 9000) {
                failure_ = fmt("boss-fight: the boss intro did not finish (started %d completed %d boss fsm %d)",
                               int(bf_intro_started_), int(bf_intro_completed_), have ? snap.fsm_state : -1);
            }
            return;
        }
        case BossPhase::spawn: {
            if (frame_ < 30) return;
            if (options_.boss_natural) {
                bf_phase_ = BossPhase::nat_place;
                bf_phase_frame_ = frame_;
                return;
            }
            const std::string result = model_renderer::spawn_character(boss);
            plog(fmt("boss spawn | frame %llu | %s | %s", (unsigned long long)frame_, boss.c_str(),
                     result.c_str()));
            bf_phase_ = BossPhase::wait_init;
            bf_phase_frame_ = frame_;
            return;
        }
        case BossPhase::wait_init:
            if (have && snap.initialized && snap.fsm_state > 0) {
                // The authored intro scripts end with ShowActor on the boss (Swamp_Serpent_Intro,
                // HG2_boss_intro); a development SpawnCharacter alone leaves it hidden.
                std::string show_error;
                const bool shown = model_renderer::boss_probe_script_command(42, boss, show_error);
                plog(fmt("boss ShowActor: %s%s", shown ? "ok" : "ERROR ", show_error.c_str()));
                if (options_.boss_weaken && snap.hp > 3 * 256) {
                    const std::string hit = model_renderer::debug_character_hit(
                        boss, std::uint32_t(snap.hp - 3 * 256));
                    plog(fmt("boss weaken | HP %.1f -> 3 | %s", snap.hp / 256.0, hit.c_str()));
                }
                plog(fmt("boss ready | frame %llu | script %s | fsm %d | HP %.1f | ais flags 0x%x | at %s",
                         (unsigned long long)frame_, snap.script.c_str(), snap.fsm_state,
                         snap.hp / 256.0, snap.ais_flags,
                         vec_text({snap.position[0], snap.position[1], snap.position[2]}).c_str()));
                bf_phase_ = BossPhase::place;
                bf_phase_frame_ = frame_;
            } else if (since > 600) {
                failure_ = fmt("boss-fight: %s never became ready (present %d, initialized %d, fsm %d, %s)",
                               boss.c_str(), int(have), int(have && snap.initialized),
                               have ? snap.fsm_state : -1, error.c_str());
            }
            return;
        case BossPhase::place: {
            std::string place_error;
            std::array<float, 3> offset = options_.boss_offset;
            const float offset_length = std::hypot(offset[0], offset[1]);
            if (options_.boss_distance > 0.0f && offset_length > 1.0f) {
                offset[0] *= options_.boss_distance / offset_length;
                offset[1] *= options_.boss_distance / offset_length;
            }
            const Vec3 spot{snap.position[0] + offset[0], snap.position[1] + offset[1],
                            snap.position[2] + offset[2]};
            const bool placed = have && model_renderer::boss_probe_place_player(spot, place_error);
            plog(fmt("boss place Player at %s: %s%s", vec_text(spot).c_str(),
                     placed ? "ok" : "ERROR ", place_error.c_str()));
            if (!placed) {
                failure_ = "boss-fight: placing the Player failed: " + place_error;
                return;
            }
            bf_phase_ = BossPhase::fight;
            bf_phase_frame_ = frame_;
            return;
        }
        case BossPhase::fight:
            if (options_.boss_refill && !snap.dead &&
                ((options_.boss_refill_hp > 0 && vitals[1] != options_.boss_refill_hp * 256) ||
                 (frame_ % 60 == 0 && vitals[0] < vitals[1] / 2))) {
                const auto result = model_renderer::debug_player_refill_hp(options_.boss_refill_hp);
                if (frame_ % 600 == 0 || bf_refills_++ == 0)
                    plog(fmt("boss refill | frame %llu | %s | HP %d/%d", (unsigned long long)frame_,
                             result.c_str(), vitals[0] / 256, vitals[1] / 256));
            }
            if (!options_.boss_summon_ally.empty() && !bf_ally_summoned_ && have_pose_ &&
                frame_ - bf_phase_frame_ >= 120) {
                bf_ally_summoned_ = true;
                std::string message;
                const bool ok = model_renderer::boss_probe_summon_character(
                    options_.boss_summon_ally,
                    {pose_.player_position[0] + 220.0f, pose_.player_position[1],
                     pose_.player_position[2]},
                    message);
                plog(fmt("boss ally | frame %llu | Player summons %s near %s | %s%s",
                         (unsigned long long)frame_, options_.boss_summon_ally.c_str(),
                         vec_text(pose_.player_position).c_str(), ok ? "" : "ERROR ", message.c_str()));
                if (!ok) failure_ = "boss-fight: summoning the ally failed: " + message;
            }
            if (!options_.boss_summon_trap.empty() && !bf_trap_summoned_ && have_pose_ &&
                frame_ - bf_phase_frame_ >= 120) {
                bf_trap_summoned_ = true;
                std::string message;
                const bool ok = model_renderer::boss_probe_summon_timer_trap(
                    options_.boss_name, options_.boss_summon_trap,
                    {pose_.player_position[0], pose_.player_position[1], pose_.player_position[2]},
                    message);
                plog(fmt("boss trap | frame %llu | %s from %s at %s | %s%s", (unsigned long long)frame_,
                         options_.boss_summon_trap.c_str(), options_.boss_name.c_str(),
                         vec_text(pose_.player_position).c_str(), ok ? "" : "ERROR ", message.c_str()));
                if (!ok) failure_ = "boss-fight: summoning the TimerTrap failed: " + message;
            }
            if (frame_ % 15 == 0 && have && have_pose_ && !snap.dead) {
                const Vec3 player = pose_.player_position;
                const Vec3 target{snap.position[0], snap.position[1], snap.position[2]};
                const float distance = planar_distance(player, target);
                bf_index_ = snap.world_index;
                if (options_.boss_passive) {
                    halt_player();
                } else if (distance > 260.0f) {
                    // The route to a Character-occupied cell ends short of it; once the
                    // route says "arrived" close the remaining gap straight at the boss.
                    if (march_to(player, target, 120.0f) != March::moving)
                        steer_world(target[0] - player[0], target[1] - player[1]);
                } else {
                    halt_player();
                    const std::string attack = model_renderer::player_attack(bf_index_);
                    if (attack != bf_last_attack_) {
                        plog(fmt("boss attack | frame %llu | boss index %d | distance %.0f | %s",
                                 (unsigned long long)frame_, bf_index_, distance, attack.c_str()));
                        bf_last_attack_ = attack;
                    }
                    if (attack == "Attacking") ++bf_attacks_;
                }
            }
            if (frame_ % 60 == 0 && have)
                plog(fmt("boss status | frame %llu | fsm %d '%s' | HP %.1f/%.1f | target %llu | dist %.0f | "
                         "callbacks %u failed %u | skill cmds %u events %u | player HP %.1f | attacks %u | "
                         "player at %s | nearest enemy %d melee %d | visible src %d rend %d",
                         (unsigned long long)frame_, snap.fsm_state, snap.animation_state.c_str(),
                         snap.hp / 256.0, snap.max_hp / 256.0, (unsigned long long)snap.target,
                         have_pose_ ? planar_distance(pose_.player_position,
                             {snap.position[0], snap.position[1], snap.position[2]}) : -1.0f,
                         snap.completed_callbacks, snap.failed_callbacks, snap.skill_commands,
                         snap.skill_events, player_hp, bf_attacks_,
                         vec_text(pose_.player_position).c_str(), pose_.enemy_index,
                         int(pose_.enemy_in_melee_range), int(snap.source_visible),
                         int(snap.renderer_visible)));
            if (bf_boss_died_) {
                plog(fmt("boss died | frame %llu", (unsigned long long)frame_));
                bf_phase_ = BossPhase::done;
                bf_done_frame_ = frame_;
            }
            return;
        case BossPhase::done:
            return;
        }
    }

    void boss_finish() {
        plog(fmt("boss-fight summary | boss %s | initialized %d | boss died %d | boss HP %.1f -> min %.1f | "
                 "Player HP %.1f -> min %.1f | AIS callbacks %u failed %u | skill commands %u | OnSkill events %u | "
                 "Player attacks %u | targeted %d | deaths %u",
                 options_.boss_name.c_str(), int(bf_ever_initialized_), int(bf_boss_died_),
                 bf_boss_hp_start_, bf_boss_hp_min_ > 1e8 ? -1.0 : bf_boss_hp_min_, bf_player_hp_start_,
                 bf_player_hp_min_ > 1e8 ? -1.0 : bf_player_hp_min_, bf_max_callbacks_, bf_max_failed_,
                 bf_max_commands_, bf_max_events_, bf_attacks_, int(bf_ever_targeted_), deaths_));
        if (options_.boss_natural)
            plog(fmt("boss-fight natural | intro %s started %d completed %d (frames %llu..%llu) | max concurrent scripts %u",
                     options_.boss_intro_script.c_str(), int(bf_intro_started_), int(bf_intro_completed_),
                     (unsigned long long)bf_intro_start_frame_, (unsigned long long)bf_intro_end_frame_,
                     bf_max_running_));
        if (options_.boss_natural && failure_.empty() && !bf_intro_completed_)
            failure_ = "boss-fight: the boss intro script never completed";
        if (!bf_ever_initialized_ && failure_.empty())
            failure_ = "boss-fight: the boss AIS VM never initialised";
    }

    // ----- talk-npc / enter-zone --------------------------------------------
    // Quest-giver acceptance through the authored controls. talk-npc stands the
    // Player at --npc-distance from the named Character and presses the HUD attack
    // button once the NPC is the selected interaction target; enter-zone stands the
    // Player in the named QuestMoveInZone. Both then follow the quest row.
    enum class QuestScenarioPhase { place, settle, press, watch, done };
    QuestScenarioPhase qs_phase_ = QuestScenarioPhase::place;
    std::uint64_t qs_phase_frame_ = 0, qs_done_frame_ = 0;
    int qs_touch_ = -1;
    std::int32_t qs_first_state_ = -1, qs_last_state_ = -1, qs_presses_ = 0;
    Vec3 qs_zone_center_{};

    std::int32_t quest_state_of(std::int32_t row) {
        std::vector<model_renderer::UiQuestReadV1> rows;
        std::string error;
        if (!model_renderer::ui_player_quest_rows(player_, rows, error, nullptr) || row < 0 ||
            std::size_t(row) >= rows.size())
            return -1;
        return rows[std::size_t(row)].state;
    }
    void quest_tick() {
        const bool talk = options_.talk_npc_scenario;
        const std::int32_t watch = options_.watch_quest >= 0 ? options_.watch_quest : (talk ? 51 : 52);
        const std::int32_t state = quest_state_of(watch);
        if (state != qs_last_state_) {
            plog(fmt("quest %d state %d -> %d | frame %llu", watch, qs_last_state_, state,
                     (unsigned long long)frame_));
            qs_last_state_ = state;
            if (qs_first_state_ < 0) qs_first_state_ = state;
        }
        if (frame_ % 60 == 0) {
            std::string ooi_name, ooi_error;
            std::int32_t ooi_type = -1;
            if (model_renderer::ooi_probe_snapshot(ooi_name, ooi_type, ooi_error))
                plog(fmt("quest scenario | frame %llu | selected object '%s' interaction type %d",
                         (unsigned long long)frame_, ooi_name.c_str(), ooi_type));
        }
        if (!talk && frame_ % 60 == 0) {
            std::string zone_status;
            if (model_renderer::quest_zone_probe_status(options_.zone_name, zone_status))
                plog(fmt("enter-zone | frame %llu | Player at %s | sensor %s", (unsigned long long)frame_,
                         have_pose_ ? vec_text(pose_.player_position).c_str() : "?", zone_status.c_str()));
        }
        const auto since = frame_ - qs_phase_frame_;
        switch (qs_phase_) {
        case QuestScenarioPhase::place: {
            if (frame_ < 30) return;
            std::array<float, 3> spot{};
            std::string place_error;
            if (talk) {
                model_renderer::BossProbeSnapshotV1 snap;
                if (!model_renderer::boss_probe_snapshot(options_.npc_name, snap, place_error)) {
                    failure_ = "talk-npc: " + options_.npc_name + " not found: " + place_error;
                    return;
                }
                spot = {snap.position[0] + options_.npc_distance, snap.position[1], snap.position[2]};
                plog(fmt("talk-npc | %s (enabled %d) at %s | Player to %s", options_.npc_name.c_str(),
                         int(snap.enabled),
                         vec_text({snap.position[0], snap.position[1], snap.position[2]}).c_str(),
                         vec_text(spot).c_str()));
                for (const char* sibling : {"_prim_NPC_LizBloodPOST", "_prim_NPC_LizBlood", "_prim_NPC_Mothgiver"}) {
                    model_renderer::BossProbeSnapshotV1 other;
                    std::string other_error;
                    if (model_renderer::boss_probe_snapshot(sibling, other, other_error))
                        plog(fmt("talk-npc | %s enabled %d", sibling, int(other.enabled)));
                }
            } else if (!model_renderer::quest_zone_probe_position(options_.zone_name, spot, place_error)) {
                failure_ = "enter-zone: zone " + options_.zone_name + " not found: " + place_error;
                return;
            } else {
                qs_zone_center_ = spot;
                plog(fmt("enter-zone | %s centre %s", options_.zone_name.c_str(), vec_text(spot).c_str()));
                // The Player walks in from where it stands; no teleport.
                qs_phase_ = QuestScenarioPhase::watch;
                qs_phase_frame_ = frame_;
                return;
            }
            if (!model_renderer::boss_probe_place_player(spot, place_error)) {
                failure_ = "quest scenario: placing the Player failed: " + place_error;
                return;
            }
            qs_phase_ = QuestScenarioPhase::settle;
            qs_phase_frame_ = frame_;
            return;
        }
        case QuestScenarioPhase::settle:
            if (since >= 45) {
                qs_phase_ = talk ? QuestScenarioPhase::press : QuestScenarioPhase::watch;
                qs_phase_frame_ = frame_;
            }
            return;
        case QuestScenarioPhase::press:
            // One press per 90 frames until the NPC answers; the release follows 6 frames later.
            if (since % 90 == 0) {
                qs_touch_ = 0;
                ++qs_presses_;
                plog(fmt("talk-npc | HUD attack press %d | frame %llu", qs_presses_, (unsigned long long)frame_));
            } else if (since % 90 == 6) {
                qs_touch_ = 1;
            }
            if (state != qs_first_state_ || qs_presses_ >= 6) {
                qs_phase_ = QuestScenarioPhase::watch;
                qs_phase_frame_ = frame_;
            }
            return;
        case QuestScenarioPhase::watch:
            // Steer every frame, like a held stick: the Player is pinned while idle
            // (CSMove::OnBlur), so a 15-frame gap leaves it static at the zone edge.
            if (!talk && have_pose_ && state == qs_first_state_) {
                // Walk through the normal movement controller toward the zone centre.
                if (march_to(pose_.player_position, qs_zone_center_, 60.0f) == March::no_route)
                    steer_world(qs_zone_center_[0] - pose_.player_position[0],
                                qs_zone_center_[1] - pose_.player_position[1]);
            }
            // talk-npc settles in a few hundred frames; the enter-zone walk to a far
            // QuestMoveInZone needs thousands of frames plus authored cutscenes.
            if (since > (talk ? 1800u : 9000u) || (state > qs_first_state_ && state >= 6)) {
                qs_phase_ = QuestScenarioPhase::done;
                qs_done_frame_ = frame_;
            }
            return;
        case QuestScenarioPhase::done:
            return;
        }
    }
    void quest_finish() {
        const bool talk = options_.talk_npc_scenario;
        const std::int32_t watch = options_.watch_quest >= 0 ? options_.watch_quest : (talk ? 51 : 52);
        const std::int32_t state = quest_state_of(watch);
        plog(fmt("%s summary | quest %d | state %d -> %d | presses %d", talk ? "talk-npc" : "enter-zone",
                 watch, qs_first_state_, state, qs_presses_));
        if (state <= qs_first_state_ && failure_.empty())
            failure_ = fmt("%s: quest %d never left state %d", talk ? "talk-npc" : "enter-zone", watch,
                           qs_first_state_);
    }

    // ----- faery-skill ------------------------------------------------------
    enum class FsPhase { setup, switch_wait, approach, watch, cooldown_wait, done };
    FsPhase fs_phase_ = FsPhase::setup;
    std::vector<std::int32_t> fs_queue_;
    std::size_t fs_pos_ = 0;
    unsigned fs_cast_for_faery_ = 0, fs_total_casts_ = 0, fs_effective_ = 0;
    std::uint64_t fs_wait_until_ = 0, fs_cast_frame_ = 0;
    int fs_last_state_ = -1, fs_state_frames_ = 0;
    std::string fs_transitions_;
    bool fs_use_ok_ = false;
    std::string fs_use_error_;
    std::array<int, 7> fs_pre_vitals_{};
    EnemySum fs_pre_enemies_;
    int fs_pre_cd_ = 0, fs_pre_usable_ = 0, fs_peak_cd_ = 0, fs_min_usable_ = 1;
    int fs_pre_state_ = -1;
    bool fs_saw_spell_state_ = false, fs_stuck_before_ = false;
    dh2::native::player_skill_effect_audit_v1::Counters fs_ec_pre_;
    int fs_target_ = -1;
    double fs_target_hp_before_ = 0.0, fs_target_hp_min_ = 0.0;
    double fs_player_hp_peak_ = 0.0;
    struct FsRecord {
        std::int32_t faery, level;
        unsigned cast;
        std::string verdict;
    };
    std::vector<FsRecord> fs_records_;

    void faery_skill_tick() {
        if (hud_blocked_ && fs_phase_ != FsPhase::setup && fs_phase_ != FsPhase::done) {
            // The HUD is hidden behind an authored cutscene: no cast goes out, and the
            // watch / cooldown / switch clocks pause until the controls are back.
            if (fs_phase_ == FsPhase::watch) ++fs_cast_frame_;
            if (fs_phase_ == FsPhase::cooldown_wait || fs_phase_ == FsPhase::switch_wait) ++fs_wait_until_;
            return;
        }
        switch (fs_phase_) {
        case FsPhase::setup: fs_setup(); break;
        case FsPhase::switch_wait:
            if (frame_ >= fs_wait_until_) fs_phase_ = FsPhase::approach;
            break;
        case FsPhase::approach:
            if (frame_ % 15 == 0) fs_approach();
            break;
        case FsPhase::watch: fs_watch(); break;
        case FsPhase::cooldown_wait:
            if (frame_ % 30 == 0) fs_cooldown_wait();
            break;
        case FsPhase::done: break;
        }
    }

    void fs_setup() {
        std::int32_t active = -1, level = -1;
        std::string error;
        if (!model_renderer::ui_player_active_faery(player_, active, level, error)) {
            failure_ = "faery-skill: ui_player_active_faery failed: " + error;
            fs_phase_ = FsPhase::done;
            return;
        }
        plog(fmt("faery-skill setup | active faery id %d level %d", active, level));
        std::vector<std::int32_t> unlocked;
        for (std::uint32_t id = 0; id < 5; ++id) {
            bool is_unlocked = false;
            if (!model_renderer::ui_player_faery_unlocked(player_, id, is_unlocked, error))
                plog(fmt("faery-skill setup | faery %u unlocked query failed: %s", id, error.c_str()));
            plog(fmt("faery-skill setup | faery %u unlocked %d%s", id, int(is_unlocked),
                     std::int32_t(id) == active ? " (active)" : ""));
            if (is_unlocked) unlocked.push_back(std::int32_t(id));
        }
        // Active faery first, then the other unlocked rows.
        fs_queue_.clear();
        if (active >= 0) fs_queue_.push_back(active);
        for (const auto id : unlocked)
            if (id != active) fs_queue_.push_back(id);
        if (active >= 0) {
            // NativeHUDSetActiveFaery: ChangeFaery + Level::PlaceFaeryAndFollowers.
            std::string place_error;
            const bool placed = model_renderer::ui_player_set_active_faery(player_, std::uint32_t(active), place_error);
            plog(fmt("faery-skill setup | set-active-faery %d (ChangeFaery + PlaceFaeryAndFollowers): %s%s", active,
                     placed ? "ok" : "ERROR: ", place_error.c_str()));
        }
        read_hud();
        plog(fmt("faery-skill setup | HUD faery id %d cd %d%% usable %d | skill slots %d %d %d | queue size %zu",
                 hud_.faery_id, hud_.infos[6], hud_.infos[7], hud_.skill_slots[0], hud_.skill_slots[1],
                 hud_.skill_slots[2], fs_queue_.size()));
        if (fs_queue_.empty()) {
            failure_ = "faery-skill: no unlocked faery";
            fs_phase_ = FsPhase::done;
            return;
        }
        fs_pos_ = 0;
        fs_cast_for_faery_ = 0;
        fs_phase_ = FsPhase::approach;
    }

    void fs_approach() {
        if (!have_pose_) return;
        if (pose_.enemy_index >= 0 && pose_.enemy_hp > 0 && pose_.enemy_in_melee_range) {
            halt_player();
            fs_cast();
            return;
        }
        if (pose_.enemy_index >= 0) {
            if (march_to(pose_.player_position, pose_.enemy_position, 40.0f) == March::no_route)
                plog("faery-skill approach | no selected-floor route to nearest enemy");
        }
    }

    void fs_cast() {
        fs_target_ = pose_.enemy_index;
        fs_target_hp_before_ = fs_target_hp_min_ = pose_.enemy_hp / 256.0;
        fs_pre_vitals_ = model_renderer::player_vitals();
        fs_pre_enemies_ = all_enemy_hp();
        read_hud();
        fs_pre_cd_ = hud_.infos[6];
        fs_pre_usable_ = hud_.infos[7];
        fs_peak_cd_ = fs_pre_cd_;
        fs_min_usable_ = fs_pre_usable_;
        fs_pre_state_ = model_renderer::gameplay_player_state();
        fs_last_state_ = fs_pre_state_;
        fs_state_frames_ = 0;
        fs_saw_spell_state_ = false;
        // A Player already in state 6/7 makes the source guard (AI_IsSpellUsable) a no-op.
        fs_stuck_before_ = fs_pre_state_ == 6 || fs_pre_state_ == 7;
        fs_player_hp_peak_ = fs_pre_vitals_[0] / 256.0;
        fs_transitions_.clear();
        fs_ec_pre_ = dh2::native::player_skill_effect_audit_v1::counters();
        std::string error;
        if (options_.faery_hud_tap) {
            // Authored HUD faery button: press now, release 6 frames later (the
            // GameSWF onRelease is what reaches NativeHUDSpell).
            // Pointer move first (GameSWF hit-tests the cursor), press, hold, release.
            tap_queue_ = {{frame_, 2}, {frame_ + 2, 0}, {frame_ + 8, 1}};
            fs_use_ok_ = true;
            fs_use_error_.clear();
        } else {
            fs_use_ok_ = model_renderer::ui_player_use_spell(player_, error);
            fs_use_error_ = error;
        }
        fs_cast_frame_ = frame_;
        ++fs_cast_for_faery_;
        ++fs_total_casts_;
        std::int32_t faery_id = -1, faery_level = -1;
        model_renderer::ui_player_active_faery(player_, faery_id, faery_level, error);
        const int state_after = model_renderer::gameplay_player_state();
        if (!fs_stuck_before_ && (state_after == 6 || state_after == 7)) fs_saw_spell_state_ = true;
        plog(fmt("faery-skill CAST begin | frame %llu | player at %s | faery %d level %d cast #%u | %s %s%s%s | state %d -> %d | "
                 "HUD cd %d%% usable %d | player HP %.1f MP %.1f | nearest enemy %d HP %.1f | all enemies live %d hp-sum %.1f dead %d",
                 (unsigned long long)frame_, vec_text(pose_.player_position).c_str(), faery_id, faery_level, fs_cast_for_faery_,
                 options_.faery_hud_tap ? "HUD tap" : "ui_player_use_spell",
                 fs_use_ok_ ? "ok" : "ERROR", fs_use_ok_ ? "" : ": ", fs_use_ok_ ? "" : fs_use_error_.c_str(),
                 fs_pre_state_, state_after, fs_pre_cd_, fs_pre_usable_, fs_pre_vitals_[0] / 256.0,
                 fs_pre_vitals_[2] / 256.0, fs_target_, fs_target_hp_before_,
                 fs_pre_enemies_.count - fs_pre_enemies_.dead, fs_pre_enemies_.hp, fs_pre_enemies_.dead));
        fs_phase_ = FsPhase::watch;
    }

    void fs_watch() {
        const std::uint64_t elapsed = frame_ - fs_cast_frame_;
        const int state = model_renderer::gameplay_player_state();
        if (state != fs_last_state_) {
            fs_transitions_ += fmt("%d@+%llu ", state, (unsigned long long)elapsed);
            fs_last_state_ = state;
        }
        if (!fs_stuck_before_ && (state == 6 || state == 7)) {
            fs_saw_spell_state_ = true;
            ++fs_state_frames_;
        }
        if (elapsed % 10 == 0) {
            read_hud();
            fs_peak_cd_ = std::max(fs_peak_cd_, hud_.infos[6]);
            fs_min_usable_ = std::min(fs_min_usable_, hud_.infos[7]);
            const auto vitals = model_renderer::player_vitals();
            fs_player_hp_peak_ = std::max(fs_player_hp_peak_, vitals[0] / 256.0);
            model_renderer::GameplayEnemyStateSnapshotV1 target;
            std::string error;
            if (fs_target_ >= 0 && model_renderer::gameplay_enemy_state_snapshot(fs_target_, target, error))
                fs_target_hp_min_ = std::min(fs_target_hp_min_, target.dead ? 0.0 : target.hp / 256.0);
        }
        const bool settled = elapsed >= 150 && state != 6 && state != 7;
        if (!settled && elapsed < 240) return;
        const auto vitals = model_renderer::player_vitals();
        const auto enemies = all_enemy_hp();
        read_hud();
        std::int32_t faery_id = -1, faery_level = -1;
        std::string error;
        model_renderer::ui_player_active_faery(player_, faery_id, faery_level, error);
        const double enemy_delta = enemies.hp - fs_pre_enemies_.hp;
        const bool mp_spent = vitals[2] < fs_pre_vitals_[2];
        const bool cooldown = fs_peak_cd_ > 0 || fs_min_usable_ == 0;
        std::string verdict;
        if (fs_stuck_before_ && fs_use_ok_)
            verdict = fmt("BLOCKED: Player already in state %d before the call (source AI_IsSpellUsable guard = silent no-op)",
                          fs_pre_state_);
        else if (!fs_use_ok_) verdict = "ERROR use call failed: " + fs_use_error_;
        else if (!fs_saw_spell_state_ && !cooldown && !mp_spent && enemy_delta >= -0.01 &&
                 fs_player_hp_peak_ <= fs_pre_vitals_[0] / 256.0 + 0.01)
            verdict = "NO-EFFECT (no state 6/7, no cooldown, no MP spent, no HP change)";
        else {
            verdict = "EFFECT:";
            if (fs_saw_spell_state_) verdict += fmt(" state6/7 %d frames;", fs_state_frames_);
            if (cooldown) verdict += fmt(" cooldown peak %d%%;", fs_peak_cd_);
            if (mp_spent) verdict += fmt(" MP %.1f->%.1f;", fs_pre_vitals_[2] / 256.0, vitals[2] / 256.0);
            if (enemy_delta < -0.01) verdict += fmt(" enemy HP-sum %.1f->%.1f;", fs_pre_enemies_.hp, enemies.hp);
            if (fs_player_hp_peak_ > fs_pre_vitals_[0] / 256.0 + 0.01)
                verdict += fmt(" player HP rose to %.1f (from %.1f);", fs_player_hp_peak_, fs_pre_vitals_[0] / 256.0);
        }
        verdict += fmt(" | Player state at end %d", state);
        // Effect ledger from the owners: the spell counts as effective only when
        // the faery OnSkill ran and something it asked for was delivered.
        {
            const auto ec = dh2::native::player_skill_effect_audit_v1::counters();
            const auto& pre = fs_ec_pre_;
            const std::uint32_t on_skill = ec.on_skill_dispatched - pre.on_skill_dispatched;
            const std::uint32_t failed = (ec.on_skill_failed - pre.on_skill_failed) + (ec.native_failed - pre.native_failed);
            const std::uint32_t hits = ec.attack_hits - pre.attack_hits;
            const std::uint32_t projectiles = ec.projectiles - pre.projectiles;
            const std::uint32_t buffs = ec.buffs - pre.buffs;
            const std::uint32_t fx_matched = ec.fx_matched - pre.fx_matched;
            const std::uint32_t fx_launched = ec.fx_launched - pre.fx_launched;
            const std::uint32_t sounds = ec.sound_playable - pre.sound_playable;
            // Positive spell/skill rolls count only when an enemy HP actually fell.
            const bool hp_fell = enemy_delta < -0.01 || fs_target_hp_min_ < fs_target_hp_before_ - 0.01;
            const bool ledger_effect = (hits && hp_fell) || projectiles || buffs || fx_launched || sounds;
            verdict += fmt(" | ledger OnSkill x%u (failed %u) attack-hits %u projectile %u buff %u FX matched %u launched %u sound %u%s%s",
                           on_skill, failed, hits, projectiles, buffs, fx_matched, fx_launched, sounds,
                           failed ? " BROKEN: " : "", failed ? ec.last_error.c_str() : "");
            if (failed == 0 && fs_use_ok_ && !fs_stuck_before_ && ledger_effect) ++fs_effective_;
        }
        plog(fmt("faery-skill CAST end | frame %llu (+%llu) | player at %s | faery %d level %d cast #%u | transitions [%s] | "
                 "player HP %.1f -> %.1f (peak %.1f) MP %.1f -> %.1f | target %d HP %.1f -> min %.1f | "
                 "all enemies hp-sum %.1f -> %.1f (live %d -> %d) | HUD cd peak %d%% usable min %d, now cd %d%% usable %d | %s",
                 (unsigned long long)frame_, (unsigned long long)elapsed, vec_text(pose_.player_position).c_str(),
                 faery_id, faery_level, fs_cast_for_faery_, fs_transitions_.c_str(), fs_pre_vitals_[0] / 256.0,
                 vitals[0] / 256.0, fs_player_hp_peak_, fs_pre_vitals_[2] / 256.0, vitals[2] / 256.0,
                 fs_target_, fs_target_hp_before_, fs_target_hp_min_, fs_pre_enemies_.hp, enemies.hp,
                 fs_pre_enemies_.count - fs_pre_enemies_.dead, enemies.count - enemies.dead, fs_peak_cd_,
                 fs_min_usable_, hud_.infos[6], hud_.infos[7], verdict.c_str()));
        fs_records_.push_back({faery_id, faery_level, fs_cast_for_faery_, verdict});
        if (state == 6 || state == 7) {
            // Is the cast lock permanent? Try the ordinary ways out: Stop, then a basic attack.
            halt_player();
            const auto attack = model_renderer::player_attack();
            plog(fmt("faery-skill recovery attempt from state %d | stop + player_attack -> \"%s\" | state now %d", state,
                     attack.c_str(), model_renderer::gameplay_player_state()));
        }
        if (fs_cast_for_faery_ >= 2) {
            // Next faery (re-placed through the HUD's NativeHUDSetActiveFaery path).
            ++fs_pos_;
            fs_cast_for_faery_ = 0;
            if (fs_pos_ >= fs_queue_.size()) {
                fs_phase_ = FsPhase::done;
                return;
            }
            const auto next = fs_queue_[fs_pos_];
            std::string switch_error;
            const bool ok = model_renderer::ui_player_set_active_faery(player_, std::uint32_t(next), switch_error);
            plog(fmt("faery-skill switch to faery %d | %s%s", next, ok ? "ok" : "ERROR: ", switch_error.c_str()));
            fs_wait_until_ = frame_ + 180;
            fs_phase_ = FsPhase::switch_wait;
            return;
        }
        fs_wait_until_ = frame_ + 300;
        fs_phase_ = FsPhase::cooldown_wait;
    }

    void fs_cooldown_wait() {
        read_hud();
        if (hud_.infos[7] != 0 || frame_ >= fs_wait_until_) {
            plog(fmt("faery-skill cooldown wait over | frame %llu | HUD cd %d%% usable %d%s",
                     (unsigned long long)frame_, hud_.infos[6], hud_.infos[7],
                     hud_.infos[7] ? "" : " (timeout; casting anyway)"));
            // A spell needs no melee target: later casts go out where the Player stands.
            if (have_pose_) fs_cast();
            else fs_phase_ = FsPhase::approach;
        }
    }

    void faery_skill_finish() {
        plog(fmt("faery-skill summary | casts %u | casts with an observable effect %u | deaths %u | final Player state %d",
                 fs_total_casts_, fs_effective_, deaths_, model_renderer::gameplay_player_state()));
        for (const auto& record : fs_records_)
            plog(fmt("faery-skill result | faery %d level %d cast #%u | %s", record.faery, record.level,
                     record.cast, record.verdict.c_str()));
    }

    // ----- skill-matrix -----------------------------------------------------
    enum class MxPhase { setup, prepare, approach, use, watch, wait, done };
    MxPhase mx_phase_ = MxPhase::setup;
    std::vector<model_renderer::UiSkillReadV1> mx_rows_;
    std::size_t mx_k_ = 0;
    unsigned mx_attempt_ = 0, mx_level_ups_ = 0;
    std::uint64_t mx_wait_until_ = 0, mx_use_frame_ = 0;
    int mx_last_state_ = -1, mx_state6_frames_ = 0;
    bool mx_saw6_ = false, mx_use_ok_ = false;
    std::string mx_use_error_, mx_transitions_;
    std::array<int, 7> mx_pre_vitals_{};
    EnemySum mx_pre_enemies_;
    int mx_pre_cd_ = 0, mx_pre_usable_ = 0, mx_peak_cd_ = 0, mx_min_usable_ = 1, mx_target_ = -1;
    double mx_target_before_ = 0.0, mx_target_min_ = 0.0;
    unsigned mx_deaths_at_use_ = 0;
    model_renderer::UiPlayerStatsReadV1 mx_stats_pre_, mx_stats_mid_;
    bool mx_stats_mid_taken_ = false;
    // Effect counters sampled just before each USE; the deltas at settle time
    // are what the verdict is allowed to count as delivered.
    dh2::native::player_skill_effect_audit_v1::Counters mx_ec_pre_;
    struct MxRecord {
        std::size_t index;
        std::int32_t id;
        std::string verdict;
    };
    std::vector<MxRecord> mx_records_;

    void matrix_tick() {
        if (frame_ + 3 >= options_.frames && mx_phase_ != MxPhase::done) {
            mx_record_unfinished("frame bound reached");
            mx_phase_ = MxPhase::done;
        }
        switch (mx_phase_) {
        case MxPhase::setup: mx_setup(); break;
        case MxPhase::prepare: mx_prepare(); break;
        case MxPhase::approach:
            if (frame_ % 15 == 0) mx_approach();
            break;
        case MxPhase::use: mx_use(); break;
        case MxPhase::watch: mx_watch(); break;
        case MxPhase::wait: mx_wait(); break;
        case MxPhase::done: break;
        }
    }

    void mx_record_unfinished(const char* why) {
        if (mx_k_ < mx_rows_.size() && mx_records_.size() <= mx_k_) {
            mx_records_.push_back({mx_k_, mx_rows_[mx_k_].id, std::string("NOT-TESTED: ") + why});
            plog(fmt("matrix skill %zu (id %d) NOT-TESTED: %s", mx_k_, mx_rows_[mx_k_].id, why));
        }
    }

    void mx_setup() {
        std::string error;
        for (std::uint32_t k = 0; k < 256; ++k) {
            model_renderer::UiSkillReadV1 row;
            if (!model_renderer::ui_player_skill(player_, k, row, error)) {
                if (k == 0) {
                    failure_ = "skill-matrix: ui_player_skill(0) failed: " + error;
                    mx_phase_ = MxPhase::done;
                    return;
                }
                break;
            }
            mx_rows_.push_back(row);
        }
        std::array<std::int32_t, 3> slots{};
        model_renderer::ui_player_skill_slots(player_, slots, error);
        std::int32_t points = -1;
        model_renderer::ui_player_skill_points(player_, points, error);
        model_renderer::UiPlayerStatsReadV1 stats;
        model_renderer::ui_player_stats(player_, stats, error);
        plog(fmt("matrix setup | class '%s' | character level %d | skill points %d | max HP %d max MP %d (raw stats) | HUD slots %d %d %d | skills in tree %zu",
                 stats.class_name.c_str(), stats.level, points, stats.max_hp, stats.max_mp, slots[0],
                 slots[1], slots[2], mx_rows_.size()));
        for (std::size_t k = 0; k < mx_rows_.size(); ++k) {
            const auto& r = mx_rows_[k];
            plog(fmt("matrix row %zu | skill id %d | saved level %d | slot %d | required char level %d | assignable %d | "
                     "can-increment %d | name text %d | icon %s",
                     k, r.id, r.level, r.slot, r.required_level, int(r.assignable), int(r.can_increment),
                     r.name_text, r.icon.c_str()));
        }
        mx_k_ = 0;
        mx_phase_ = MxPhase::prepare;
    }

    bool mx_reload(std::size_t k) {
        std::string error;
        return model_renderer::ui_player_skill(player_, std::uint32_t(k), mx_rows_[k], error);
    }

    void mx_prepare() {
        if (mx_k_ >= mx_rows_.size()) {
            mx_phase_ = MxPhase::done;
            return;
        }
        const std::size_t k = mx_k_;
        mx_attempt_ = 0;
        mx_reload(k);
        auto& row = mx_rows_[k];
        std::string error;
        if (options_.matrix_only >= 0 && std::int32_t(k) != options_.matrix_only) {
            mx_finish_skill("SKIPPED (--matrix-only)");
            return;
        }
        if (!row.assignable && row.icon == "blank") {
            mx_finish_skill(fmt("NOT-A-SKILL: placeholder row (skill id %d, icon 'blank', required char level %d, not assignable)",
                                row.id, row.required_level));
            return;
        }
        // Train once through the same NativeSkillsTrainSkill/IncSkill entry the
        // skills menu uses. Level-ups use the real Character::LevelUp path to
        // satisfy the character-level / skill-point gates.
        if (row.level <= 0) {
            unsigned ups = 0;
            for (; ups < 80 && !row.can_increment; ++ups) {
                std::int32_t points = 0;
                model_renderer::ui_player_skill_points(player_, points, error);
                if (row.character_level >= row.required_level && points > 0) break;  // another gate
                if (!model_renderer::ui_player_apply_level_up_state(player_, error)) {
                    plog(fmt("matrix skill %zu | LevelUp failed: %s", k, error.c_str()));
                    break;
                }
                ++mx_level_ups_;
                mx_reload(k);
            }
            mx_reload(k);
            std::int32_t points = 0;
            model_renderer::ui_player_skill_points(player_, points, error);
            plog(fmt("matrix skill %zu (id %d) | prepare | level-ups used %u | character level %d (required %d) | skill points %d | can-increment %d",
                     k, row.id, ups, row.character_level, row.required_level, points, int(row.can_increment)));
            if (row.can_increment) {
                std::uint32_t source_return = 0;
                std::int32_t points_after = 0;
                model_renderer::UiPlayerStatsReadV1 stats_before, stats_after;
                std::string stats_error;
                model_renderer::ui_player_stats(player_, stats_before, stats_error);
                std::int32_t potions_before_a = 0, potions_before_b = 0, potions_after_a = 0, potions_after_b = 0;
                model_renderer::ui_player_potions(player_, potions_before_a, potions_before_b, stats_error);
                const bool trained = model_renderer::ui_player_train_skill(
                    player_, std::uint32_t(k), false, source_return, points_after, error);
                plog(fmt("matrix skill %zu (id %d) | train | %s%s | source return %u | points left %d", k, row.id,
                         trained ? "ok" : "ERROR: ", error.c_str(), source_return, points_after));
                mx_reload(k);
                model_renderer::ui_player_stats(player_, stats_after, stats_error);
                model_renderer::ui_player_potions(player_, potions_after_a, potions_after_b, stats_error);
                if (!row.assignable) {
                    // Passive row: nothing to press; verify the trained level reaches the Character sheet.
                    std::string delta = stats_diff(stats_before, stats_after);
                    if (potions_before_a != potions_after_a || potions_before_b != potions_after_b)
                        delta += fmt(" potions %d/%d->%d/%d;", potions_before_a, potions_before_b, potions_after_a,
                                     potions_after_b);
                    const bool changed = delta != " none";
                    mx_finish_skill(fmt("PASSIVE %s: trained %s (saved level %d); Character-sheet delta right after training:%s",
                                        (trained && row.level > 0 && changed) ? "OK" :
                                        (trained && row.level > 0) ? "NO-SHEET-CHANGE" : "TRAIN-FAILED",
                                        trained ? "ok" : "ERROR", row.level, delta.c_str()));
                    return;
                }
            }
        }
        if (!row.assignable) {
            mx_finish_skill(fmt("PASSIVE NOT-TESTABLE: could not train (level %d, char level %d, required %d, can-increment %d)",
                                row.level, row.character_level, row.required_level, int(row.can_increment)));
            return;
        }
        if (row.level <= 0) {
            mx_finish_skill(fmt("NOT-TESTABLE: could not train (level %d, char level %d, required %d, can-increment %d)",
                                row.level, row.character_level, row.required_level, int(row.can_increment)));
            return;
        }
        if (!model_renderer::ui_player_equip_skill(player_, 0, std::int32_t(k), error)) {
            mx_finish_skill("EQUIP-REJECTED: " + error);
            return;
        }
        std::array<std::int32_t, 3> slots{};
        model_renderer::ui_player_skill_slots(player_, slots, error);
        plog(fmt("matrix skill %zu (id %d) | equipped in HUD slot 0 | level %d | HUD slots now %d %d %d", k, row.id,
                 row.level, slots[0], slots[1], slots[2]));
        mx_phase_ = MxPhase::approach;
    }

    void mx_approach() {
        if (!have_pose_) return;
        if (pose_.enemy_index >= 0 && pose_.enemy_hp > 0 && pose_.enemy_in_melee_range) {
            halt_player();
            mx_phase_ = MxPhase::use;
            return;
        }
        if (pose_.enemy_index >= 0 &&
            march_to(pose_.player_position, pose_.enemy_position, 40.0f) == March::no_route)
            plog("matrix approach | no selected-floor route to nearest enemy");
    }

    void mx_use() {
        halt_player();
        if (!have_pose_ || pose_.enemy_index < 0 || pose_.enemy_hp <= 0 || !pose_.enemy_in_melee_range) {
            mx_phase_ = MxPhase::approach;
            return;
        }
        ++mx_attempt_;
        mx_target_ = pose_.enemy_index;
        mx_target_before_ = mx_target_min_ = pose_.enemy_hp / 256.0;
        mx_pre_vitals_ = model_renderer::player_vitals();
        mx_pre_enemies_ = all_enemy_hp();
        read_hud();
        mx_pre_cd_ = hud_.infos[8];
        mx_pre_usable_ = hud_.infos[9];
        mx_peak_cd_ = mx_pre_cd_;
        mx_min_usable_ = mx_pre_usable_;
        mx_last_state_ = model_renderer::gameplay_player_state();
        mx_state6_frames_ = 0;
        mx_saw6_ = false;
        mx_transitions_.clear();
        mx_deaths_at_use_ = deaths_;
        std::string error;
        mx_stats_mid_taken_ = false;
        model_renderer::ui_player_stats(player_, mx_stats_pre_, error);
        mx_ec_pre_ = dh2::native::player_skill_effect_audit_v1::counters();
        mx_use_ok_ = model_renderer::ui_player_use_skill_slot(player_, 0, error);
        mx_use_error_ = error;
        mx_use_frame_ = frame_;
        const int after = model_renderer::gameplay_player_state();
        if (after == 6) mx_saw6_ = true;
        plog(fmt("matrix skill %zu (id %d) USE attempt %u | frame %llu | use %s%s%s | state %d -> %d | HUD slot0 cd %d%% usable %d | "
                 "player HP %.1f MP %.1f | target enemy %d HP %.1f | all enemies live %d hp-sum %.1f",
                 mx_k_, mx_rows_[mx_k_].id, mx_attempt_, (unsigned long long)frame_, mx_use_ok_ ? "ok" : "ERROR",
                 mx_use_ok_ ? "" : ": ", mx_use_ok_ ? "" : mx_use_error_.c_str(), mx_last_state_, after,
                 mx_pre_cd_, mx_pre_usable_, mx_pre_vitals_[0] / 256.0, mx_pre_vitals_[2] / 256.0, mx_target_,
                 mx_target_before_, mx_pre_enemies_.count - mx_pre_enemies_.dead, mx_pre_enemies_.hp));
        mx_phase_ = MxPhase::watch;
    }

    void mx_watch() {
        const std::uint64_t elapsed = frame_ - mx_use_frame_;
        const int state = model_renderer::gameplay_player_state();
        if (state != mx_last_state_) {
            mx_transitions_ += fmt("%d@+%llu ", state, (unsigned long long)elapsed);
            mx_last_state_ = state;
        }
        if (state == 6) {
            mx_saw6_ = true;
            ++mx_state6_frames_;
        }
        if (elapsed >= 20 && !mx_stats_mid_taken_) {
            // Buff-type skills (no direct damage) show up on the Character sheet while active.
            std::string stats_error;
            mx_stats_mid_taken_ = model_renderer::ui_player_stats(player_, mx_stats_mid_, stats_error);
        }
        if (elapsed % 10 == 0) {
            read_hud();
            mx_peak_cd_ = std::max(mx_peak_cd_, hud_.infos[8]);
            mx_min_usable_ = std::min(mx_min_usable_, hud_.infos[9]);
            model_renderer::GameplayEnemyStateSnapshotV1 target;
            std::string error;
            if (mx_target_ >= 0 && model_renderer::gameplay_enemy_state_snapshot(mx_target_, target, error))
                mx_target_min_ = std::min(mx_target_min_, target.dead ? 0.0 : target.hp / 256.0);
        }
        const bool settled = elapsed >= 90 && state != 6;
        if (!settled && elapsed < 360) return;
        const auto vitals = model_renderer::player_vitals();
        const auto enemies = all_enemy_hp();
        read_hud();
        const double enemy_delta = enemies.hp - mx_pre_enemies_.hp;
        const bool mp_spent = vitals[2] < mx_pre_vitals_[2];
        const bool cooldown = mx_peak_cd_ > 0 || mx_min_usable_ == 0;
        const bool died = deaths_ != mx_deaths_at_use_;
        plog(fmt("matrix skill %zu (id %d) USE end | attempt %u | frame +%llu | transitions [%s] | state-6 frames %d | "
                 "player HP %.1f -> %.1f, MP %.1f -> %.1f | target %d HP %.1f -> min %.1f | all enemies hp-sum %.1f -> %.1f "
                 "(live %d -> %d) | HUD slot0 cd peak %d%% usable min %d | player died %d",
                 mx_k_, mx_rows_[mx_k_].id, mx_attempt_, (unsigned long long)elapsed, mx_transitions_.c_str(),
                 mx_state6_frames_, mx_pre_vitals_[0] / 256.0, vitals[0] / 256.0, mx_pre_vitals_[2] / 256.0,
                 vitals[2] / 256.0, mx_target_, mx_target_before_, mx_target_min_, mx_pre_enemies_.hp, enemies.hp,
                 mx_pre_enemies_.count - mx_pre_enemies_.dead, enemies.count - enemies.dead, mx_peak_cd_,
                 mx_min_usable_, int(died)));
        std::string verdict;
        if (!mx_use_ok_) {
            verdict = "ERROR: use call failed: " + mx_use_error_;
        } else if (mx_saw6_) {
            // State 6 alone is not evidence. Count an effect only when the
            // source requested it and the owner delivered it: OnSkill dispatched,
            // a native damage/projectile/buff/sound/FX call succeeded, or the
            // target's HP fell.
            const auto ec = dh2::native::player_skill_effect_audit_v1::counters();
            const auto& pre = mx_ec_pre_;
            const std::uint32_t on_skill = ec.on_skill_dispatched - pre.on_skill_dispatched;
            const std::uint32_t failed = (ec.on_skill_failed - pre.on_skill_failed) +
                                         (ec.native_failed - pre.native_failed);
            const std::uint32_t attacks = ec.attacks - pre.attacks;
            const std::uint32_t attack_hits = ec.attack_hits - pre.attack_hits;
            const std::uint32_t projectiles = ec.projectiles - pre.projectiles;
            const std::uint32_t buffs = ec.buffs - pre.buffs;
            const std::uint32_t fx_matched = ec.fx_matched - pre.fx_matched;
            const std::uint32_t fx_launched = ec.fx_launched - pre.fx_launched;
            const std::uint32_t fx_missing = fx_matched > fx_launched ? fx_matched - fx_launched : 0;
            const std::uint32_t sounds = ec.sound_playable - pre.sound_playable;
            const std::uint32_t sound_requests = ec.sound_requests - pre.sound_requests;
            const bool target_hit = mx_target_min_ < mx_target_before_ - 0.01;
            const bool sheet_changed = mx_stats_mid_taken_ &&
                stats_diff(mx_stats_pre_, mx_stats_mid_) != " none";
            std::string effects;
            if (target_hit) effects += fmt("damage(target %.1f->%.1f) ", mx_target_before_, mx_target_min_);
            // A positive roll only counts when an enemy's HP actually fell.
            const bool enemy_hp_fell = enemy_delta < -0.01;
            const bool unapplied_roll = attack_hits && !target_hit && !enemy_hp_fell;
            if (attack_hits && !unapplied_roll) effects += fmt("attack-hit x%u ", attack_hits);
            if (projectiles) effects += fmt("projectile x%u ", projectiles);
            if (buffs || sheet_changed) effects += fmt("buff(x%u%s) ", buffs, sheet_changed ? ", sheet" : "");
            if (fx_launched) effects += fmt("fx-launched x%u ", fx_launched);
            if (sounds) effects += fmt("sound x%u ", sounds);
            const std::string detail = fmt("OnSkill x%u (failed %u) | attack rolls %u (positive damage %u) | "
                                           "FX matched %u launched %u | sound requests %u playable %u%s",
                                           on_skill, failed, attacks, attack_hits, fx_matched, fx_launched,
                                           sound_requests, sounds,
                                           ec.last_error.empty() ? "" : (" | last error: " + ec.last_error).c_str());
            plog(fmt("matrix skill %zu (id %d) effect ledger | %s", mx_k_, mx_rows_[mx_k_].id, detail.c_str()));
            if (failed) {
                verdict = "BROKEN: " + ec.last_error;
            } else if (on_skill == 0) {
                verdict = "NO-ONSKILL: entered state 6 but the authored do_skill never dispatched OnSkill";
            } else if (effects.empty() && unapplied_roll) {
                verdict = fmt("PARTIAL: OnSkill ran and %u positive attack roll(s), but no enemy HP fell",
                              attack_hits);
            } else if (effects.empty()) {
                verdict = fmt("NO-EFFECT: OnSkill ran, no damage/projectile/buff/FX/sound observed%s",
                              fx_missing ? fmt(" (FX matched but not launched x%u: asset or pool)", fx_missing).c_str() : "");
            } else {
                verdict = "WORKS: " + effects;
                if (fx_missing) verdict += fmt("; FX matched but not launched x%u", fx_missing);
            }
            if (!effects.empty() || on_skill) {
                if (mp_spent) verdict += fmt("; MP %.1f -> %.1f", mx_pre_vitals_[2] / 256.0, vitals[2] / 256.0);
                if (cooldown) verdict += fmt("; cooldown peak %d%%", mx_peak_cd_);
            }
        } else if (cooldown || mp_spent || enemy_delta < -0.01) {
            verdict = fmt("PARTIAL: use ok, never entered state 6, but side effects: %s%s%s",
                          cooldown ? "cooldown " : "", mp_spent ? "MP-spent " : "",
                          enemy_delta < -0.01 ? "enemy-damage" : "");
        } else {
            verdict = fmt("REFUSED/NO-EFFECT: use ok but no state 6, no MP spent, no cooldown (MP %.1f, HUD usable %d)",
                          vitals[2] / 256.0, mx_pre_usable_);
        }
        // A state-6 use whose attack rolls all missed (or that delivered nothing
        // observable) is retried too, so one unlucky roll is not reported as a
        // skill that does nothing.
        const bool no_effect_retry = mx_saw6_ && verdict.rfind("NO-EFFECT", 0) == 0;
        const bool needs_retry = mx_use_ok_ && ((!mx_saw6_ && !cooldown && !mp_spent) || no_effect_retry) &&
                                 mx_attempt_ < 4 && !died;
        if (needs_retry) {
            plog(fmt("matrix skill %zu (id %d) | attempt %u gave no effect; waiting for mana/cooldown, then retrying",
                     mx_k_, mx_rows_[mx_k_].id, mx_attempt_));
            mx_wait_until_ = frame_ + 240;
            mx_phase_ = MxPhase::wait;
            return;
        }
        mx_finish_skill(verdict);
    }

    void mx_wait() {
        // Keep the Player alive and the enemy engaged while mana/cooldown recover.
        if (have_pose_ && pose_.enemy_in_melee_range && pose_.enemy_hp > 0 && frame_ % 45 == 0) {
            halt_player();
            model_renderer::player_attack();
        }
        if (frame_ >= mx_wait_until_) mx_phase_ = MxPhase::approach;
    }

    void mx_finish_skill(const std::string& verdict) {
        const auto id = mx_rows_[mx_k_].id;
        plog(fmt("matrix RESULT skill %zu (id %d) | %s", mx_k_, id, verdict.c_str()));
        mx_records_.push_back({mx_k_, id, verdict});
        ++mx_k_;
        mx_phase_ = MxPhase::prepare;
    }

    void matrix_finish() {
        unsigned works = 0, partial = 0, passive_ok = 0, placeholder = 0, other = 0;
        unsigned no_effect = 0, broken = 0, no_onskill = 0;
        for (const auto& r : mx_records_) {
            if (r.verdict.rfind("WORKS", 0) == 0) ++works;
            else if (r.verdict.rfind("PARTIAL", 0) == 0) ++partial;
            else if (r.verdict.rfind("PASSIVE OK", 0) == 0) ++passive_ok;
            else if (r.verdict.rfind("NOT-A-SKILL", 0) == 0) ++placeholder;
            else if (r.verdict.rfind("NO-EFFECT", 0) == 0) ++no_effect;
            else if (r.verdict.rfind("BROKEN", 0) == 0) ++broken;
            else if (r.verdict.rfind("NO-ONSKILL", 0) == 0) ++no_onskill;
            else ++other;
        }
        plog(fmt("matrix summary | skill rows %zu | recorded %zu | active WORKS %u | active NO-EFFECT %u | "
                 "active NO-ONSKILL %u | active BROKEN %u | active PARTIAL %u | passive OK %u | "
                 "placeholder rows %u | other (failures/not tested) %u | level-ups applied %u | deaths %u",
                 mx_rows_.size(), mx_records_.size(), works, no_effect, no_onskill, broken, partial, passive_ok,
                 placeholder, other, mx_level_ups_, deaths_));
        for (const auto& r : mx_records_)
            plog(fmt("matrix table | skill %zu | id %d | %s", r.index, r.id, r.verdict.c_str()));
    }
};

// Copies every published source LevelSavegame in the runtime directory next to
// the capture series (PREFIX_<tag>_<file>) so the Character rows can be diffed.
void copy_level_saves(const std::filesystem::path& runtime, const std::filesystem::path& prefix,
                      const std::string& tag) {
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(runtime, ec)) {
        const auto name = entry.path().filename().string();
        const std::string suffix = "_level.savegame";
        if (name.size() < suffix.size() || name.compare(name.size() - suffix.size(), suffix.size(), suffix) != 0)
            continue;
        const auto target = std::filesystem::path(prefix.string() + "_" + tag + "_" + name);
        std::filesystem::copy_file(entry.path(), target, std::filesystem::copy_options::overwrite_existing, ec);
        plog("levelsave copy | " + tag + " | " + name + (ec ? " | FAILED " + ec.message() : " | ok"));
    }
}

struct PlayerUiFrame {
    dh2::android_ui::OriginalUiSession* session{};
    int width = 1;
    int height = 1;
};

bool draw_player_ui(void* context, const std::int32_t* sheet, std::size_t count,
                    std::uintptr_t character, std::string& error) {
    auto& frame = *static_cast<PlayerUiFrame*>(context);
    return frame.session->render_player(frame.width, frame.height, sheet, count, character, error);
}

bool dispatch_touch(dh2::android_ui::OriginalUiSession& session, SDL_Window* window,
                   int x, int y, int action, std::string& error) {
    int logical_width = 1, logical_height = 1, drawable_width = 1, drawable_height = 1;
    SDL_GetWindowSize(window, &logical_width, &logical_height);
    SDL_GetWindowSizeInPixels(window, &drawable_width, &drawable_height);
    const float sx = float(drawable_width) / std::max(1, logical_width);
    const float sy = float(drawable_height) / std::max(1, logical_height);
    return session.touch(float(x) * sx, float(y) * sy, action, error);
}

// One SDL finger as a source finger index (slot 0..3): the HUD sees it first, then the camera,
// exactly like a MotionEvent pointer on Android.
struct DesktopFinger {
    bool down = false;
    SDL_TouchID touch = 0;
    SDL_FingerID finger = 0;
    float x = 0.0f, y = 0.0f;
};

bool dispatch_finger_touch(dh2::android_ui::OriginalUiSession& session, SDL_Window* window,
                           float normalized_x, float normalized_y, int action, unsigned slot,
                           std::string& error) {
    int width = 1, height = 1;
    SDL_GetWindowSizeInPixels(window, &width, &height);
    const auto x = std::clamp(normalized_x, 0.0f, 1.0f) * float(width);
    const auto y = std::clamp(normalized_y, 0.0f, 1.0f) * float(height);
    return session.pointer(x, y, action, slot, error);
}

bool desktop_difficulty_count(void* context, std::uint32_t* count, std::string& error) {
    if (!context || !count) {
        error = "Desktop NativeCreateSaveSlot difficulty service is unavailable";
        return false;
    }
    return static_cast<dh2::android_ui::OriginalUiSession*>(context)
        ->game_difficulty_count(*count, error);
}

struct FrontRuntimeContext {
    AAssetManager* assets{};
    dh2::android_ui::OriginalUiSession* ui{};
};

void bind_desktop_front_runtime(dh2::android_ui::OriginalUiSession& ui,
                                FrontRuntimeContext& context) {
    dh2::android_ui::FrontRuntimeServices services{};
    services.context = &context;
    services.create_save_slot = [](void* raw, const std::string& name,
            const std::string& character, std::int32_t& slot,
            bool& publish_result, std::string& error) {
        auto& owner = *static_cast<FrontRuntimeContext*>(raw);
        if (!owner.assets || !owner.ui) {
            error = "Desktop NativeCreateSaveSlot owners are unavailable";
            return false;
        }
        return model_renderer::create_menu_save_slot(owner.assets, name, character,
            slot, publish_result, error, owner.ui, desktop_difficulty_count);
    };
    services.assign_save_slot = [](void*, std::int32_t slot, std::int32_t ordinal,
                                   std::string& error) {
        return model_renderer::assign_menu_save_slot(slot, ordinal, error);
    };
    services.eabi_integer = [](void*, double number, std::int32_t& value,
                               std::string& error) {
        if (!std::isfinite(number) || number < double(INT32_MIN) ||
            number > double(INT32_MAX)) {
            error = "Menu integer outside native conversion boundary";
            return false;
        }
        value = static_cast<std::int32_t>(number);
        error.clear();
        return true;
    };
    services.request_start_game = [](void*, bool numeric, std::int32_t requested,
            std::int32_t& slot, std::string& error) {
        return model_renderer::request_menu_start(numeric, requested, slot, error);
    };
    services.request_continue_game = [](void*, std::int32_t& slot, std::string& error) {
        return model_renderer::request_menu_continue(slot, error);
    };
    services.change_preview_slot = [](void*, std::int32_t slot, bool force,
                                       std::string& error) {
        return model_renderer::select_menu_preview_slot(slot, force, error);
    };
    services.debug_load = [](void* raw, std::string& error) {
        auto* owner = static_cast<FrontRuntimeContext*>(raw);
        return owner && owner->assets && model_renderer::menu_debug_load(owner->assets, error);
    };
    services.debug_query = [](void* raw, const char* key, std::string& error) {
        auto* owner = static_cast<FrontRuntimeContext*>(raw);
        return owner && owner->assets && model_renderer::menu_debug_query(owner->assets, key, error);
    };
    services.save_game = [](void*, std::uintptr_t character, std::string& error) {
        return model_renderer::ui_player_save_game(character, error);
    };
    services.revive_local_players = [](void*, std::string& error) {
        return model_renderer::ui_revive_local_players(error);
    };
    services.camera_zoom_lock = [](void*, bool locked) {
        model_renderer::camera_zoom_lock(locked);
    };
    ui.bind_front_runtime(services);
}

int game_swf_ascii_code(unsigned char value) noexcept {
    if (value >= 'a' && value <= 'z') return 65 + value - 'a';
    if (value >= 'A' && value <= 'Z') return 65 + value - 'A';
    if (value >= '0' && value <= '9') return value;
    switch (value) {
        case ' ': return 32;
        case ',': case '<': return 188;
        case '.': case '>': return 190;
        case ';': case ':': return 186;
        case '\'': case '"': return 222;
        case '/': case '?': return 191;
        case '`': case '~': return 192;
        case '-': case '_': return 189;
        case '=': case '+': return 187;
        case '[': case '{': return 219;
        case ']': case '}': return 221;
        case '\\': case '|': return 220;
        case '!': return 49;
        case '@': return 50;
        case '#': return 51;
        case '$': return 52;
        case '%': return 53;
        case '^': return 54;
        case '&': return 55;
        case '*': return 56;
        case '(': return 57;
        case ')': return 48;
        default: return -1;
    }
}

bool game_swf_ascii_shift(unsigned char value) noexcept {
    return (value >= 'A' && value <= 'Z') ||
        std::string_view("!@#$%^&*()_+{}|:\"<>?~").find(char(value)) != std::string_view::npos;
}

int game_swf_special_key(SDL_Keycode key) noexcept {
    switch (key) {
        case SDLK_BACKSPACE: return 8;
        case SDLK_TAB: return 9;
        case SDLK_RETURN: case SDLK_KP_ENTER: return 13;
        case SDLK_ESCAPE: return 27;
        case SDLK_PAGEUP: return 34;
        case SDLK_PAGEDOWN: return 33;
        case SDLK_HOME: return 36;
        case SDLK_END: return 35;
        case SDLK_LEFT: return 37;
        case SDLK_UP: return 38;
        case SDLK_RIGHT: return 39;
        case SDLK_DOWN: return 40;
        case SDLK_DELETE: return 46;
        case SDLK_LSHIFT: case SDLK_RSHIFT: return 16;
        default: return -1;
    }
}

bool send_menu_text(dh2::android_ui::OriginalUiSession& ui,
                    const char* text, std::string& error) {
    if (!text) return true;
    for (const auto* cursor = reinterpret_cast<const unsigned char*>(text); *cursor; ++cursor) {
        const int code = game_swf_ascii_code(*cursor);
        if (code < 0) continue;
        const bool shift = game_swf_ascii_shift(*cursor);
        if (shift && !ui.menu_key_event(16, true, error)) return false;
        if (!ui.menu_key_event(static_cast<std::uint32_t>(code), true, error) ||
            !ui.menu_key_event(static_cast<std::uint32_t>(code), false, error)) return false;
        if (shift && !ui.menu_key_event(16, false, error)) return false;
    }
    return true;
}

bool start_from_authored_menu(const dh2::android_ui::NativeStartGameIntentV1& request,
        AAssetManager* assets, const std::filesystem::path& runtime,
        dh2::android_ui::OriginalUiSession& ui, bool& gameplay, std::string& error) {
    if (request.slot < 0 || !assets) {
        error = "Authored NativeStartGame returned an invalid slot or asset owner";
        return false;
    }
    ui.deactivate();
    const auto startup = model_renderer::start_menu_game(request.slot, assets, -1,
        request.has_numeric_difficulty, request.requested_difficulty);
    const bool world_loaded = model_renderer::gameplay_startup_loaded(startup);
    std::string attach_error;
    const bool hud_attached = world_loaded && ui.attach_player(runtime.string(), attach_error);
    model_renderer::bind_gameplay_ui_session(hud_attached ? &ui : nullptr);
    if (dh2::android_ui::menu_start_commit_gate_v1(world_loaded, hud_attached)) {
        gameplay = true;
        std::cout << "Authored NativeStartGame | slot " << request.slot << " | " << startup << '\n';
        error.clear();
        return true;
    }
    if (world_loaded) model_renderer::deactivate();
    std::string recovery_error;
    if (!ui.resume_front_after_start_failure(recovery_error)) {
        error = "NativeStartGame failed: " +
            (attach_error.empty() ? startup : attach_error) +
            "; authored menu recovery failed: " + recovery_error;
    } else {
        error = "NativeStartGame failed: " +
            (attach_error.empty() ? startup : attach_error);
    }
    return false;
}

std::int32_t select_or_create_source_profile(
        AAssetManager* assets, const std::filesystem::path& runtime,
        dh2::android_ui::OriginalUiSession& original_ui,
        const std::string& player_name, const std::string& player_class) {
    std::string error;
    for (std::uint32_t candidate = 0; candidate < 4; ++candidate) {
        bool exists = false;
        if (!dh2::data::campaign_profile_exists_v1(runtime.string(), candidate, exists, error))
            throw std::runtime_error("Source campaign profile lookup failed: " + error);
        if (!exists) continue;
        if (!model_renderer::assign_menu_save_slot(static_cast<std::int32_t>(candidate), 0, error))
            throw std::runtime_error("Source campaign profile assignment failed: " + error);
        std::cout << "Using existing source campaign slot " << candidate << '\n';
        return static_cast<std::int32_t>(candidate);
    }

    std::int32_t slot = -1;
    bool published = false;
    if (!model_renderer::create_menu_save_slot(
            assets, player_name, player_class, slot, published, error,
            &original_ui, desktop_difficulty_count))
        throw std::runtime_error("Source profile creation failed: " + error);
    if (!published || slot < 0)
        throw std::runtime_error("NativeCreateSaveSlot did not publish a canonical campaign profile");
    if (!model_renderer::assign_menu_save_slot(slot, 0, error))
        throw std::runtime_error("Source campaign profile assignment failed: " + error);
    std::cout << "Created source campaign profile in slot " << slot << '\n';
    return slot;
}

void save_capture(const std::filesystem::path& path, int width, int height) {
    GLint viewport[4]{};
    glGetIntegerv(GL_VIEWPORT, viewport);
    if (viewport[0] != 0 || viewport[1] != 0 || viewport[2] != width || viewport[3] != height)
        throw std::runtime_error("GLES capture viewport does not match the framebuffer dimensions");
    const auto prior_error = glGetError();
    if (prior_error != GL_NO_ERROR)
        throw std::runtime_error("GLES reported an error before framebuffer readback: " + std::to_string(prior_error));
    std::vector<std::uint8_t> pixels(static_cast<std::size_t>(width) * height * 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glFinish();
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    const auto read_error = glGetError();
    if (read_error != GL_NO_ERROR)
        throw std::runtime_error("GLES framebuffer readback failed with error " + std::to_string(read_error));
    if (!path.parent_path().empty()) std::filesystem::create_directories(path.parent_path());
    std::ofstream output(path, std::ios::binary);
    if (!output) throw std::runtime_error("Could not open capture path: " + path.string());
    output << "P6\n" << width << ' ' << height << "\n255\n";
    std::vector<std::uint8_t> rgb(static_cast<std::size_t>(width) * 3);
    for (int y = height - 1; y >= 0; --y) {
        const auto* row = pixels.data() + static_cast<std::size_t>(y) * width * 4;
        for (int x = 0; x < width; ++x) {
            rgb[static_cast<std::size_t>(x) * 3 + 0] = row[static_cast<std::size_t>(x) * 4 + 0];
            rgb[static_cast<std::size_t>(x) * 3 + 1] = row[static_cast<std::size_t>(x) * 4 + 1];
            rgb[static_cast<std::size_t>(x) * 3 + 2] = row[static_cast<std::size_t>(x) * 4 + 2];
        }
        output.write(reinterpret_cast<const char*>(rgb.data()), static_cast<std::streamsize>(rgb.size()));
    }
    if (!output) throw std::runtime_error("Framebuffer capture write failed: " + path.string());
}
}

int main(int argc, char** argv) {
    try {
        auto options = parse(argc, argv);
        const bool any_fixed_cultist_scenario =
            options.fixed_cultist_scenario || options.fixed_cultist_kill_scenario;
        SDL_SetMainReady();
        // Bounded (--frames) runs are automated tests: render into a hidden
        // window and use SDL's dummy audio driver unless --show is given.
        const bool headless = options.frames > 0 && !options.show_window;
        if (headless) SDL_setenv("SDL_AUDIODRIVER", "dummy", 1);
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
            throw std::runtime_error(std::string("SDL_Init: ") + SDL_GetError());
        if (options.runtime.empty()) {
            char* preference = SDL_GetPrefPath("Gameloft", "Dungeon Hunter II");
            if (!preference)
                throw std::runtime_error(std::string("SDL_GetPrefPath: ") + SDL_GetError());
            options.runtime = std::filesystem::u8path(preference);
            SDL_free(preference);
        } else options.runtime = std::filesystem::absolute(options.runtime);
        std::filesystem::create_directories(options.runtime);
        SDL_Window* window = SDL_CreateWindow("Dungeon Hunter II | shared native runtime",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI |
            (headless ? SDL_WINDOW_HIDDEN : 0));
        if (!window) throw std::runtime_error(std::string("SDL_CreateWindow: ") + SDL_GetError());
        SDL_SysWMinfo window_info{};
        SDL_VERSION(&window_info.version);
        if (!SDL_GetWindowWMInfo(window, &window_info) || window_info.subsystem != SDL_SYSWM_WINDOWS)
            throw std::runtime_error(std::string("SDL Windows window handle: ") + SDL_GetError());
        EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        EGLint egl_major = 0, egl_minor = 0;
        if (display == EGL_NO_DISPLAY || !eglInitialize(display, &egl_major, &egl_minor))
            throw std::runtime_error("ANGLE eglInitialize failed");
        if (!eglBindAPI(EGL_OPENGL_ES_API)) throw std::runtime_error("ANGLE eglBindAPI failed");
        const EGLint config_attributes[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
        };
        EGLConfig egl_config{};
        EGLint config_count = 0;
        if (!eglChooseConfig(display, config_attributes, &egl_config, 1, &config_count) || config_count != 1)
            throw std::runtime_error("ANGLE has no compatible RGBA8/GLES2 window config");
        const EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        EGLContext context = eglCreateContext(display, egl_config, EGL_NO_CONTEXT, context_attributes);
        if (context == EGL_NO_CONTEXT) throw std::runtime_error("ANGLE GLES2 context creation failed");
        EGLSurface surface = eglCreateWindowSurface(display, egl_config,
            window_info.info.win.window, nullptr);
        if (surface == EGL_NO_SURFACE) throw std::runtime_error("ANGLE SDL window surface creation failed");
        if (!eglMakeCurrent(display, surface, surface, context))
            throw std::runtime_error("ANGLE eglMakeCurrent failed");
        eglSwapInterval(display, 1);

        std::unique_ptr<AAssetManager, decltype(&AAssetManager_delete)> assets(
            AAssetManager_createFromFilesystem(options.assets.string().c_str()),
            &AAssetManager_delete);
        if (!assets) throw std::runtime_error("Could not open asset directory: " + options.assets.string());
        dh2::desktop_native::GameplayAudioSdl gameplay_audio;
        std::string audio_error;
        if (!gameplay_audio.open(audio_error))
            SDL_Log("DH2 gameplay audio unavailable; continuing without output: %s", audio_error.c_str());

        model_renderer::runtime_directory(options.runtime.string());
        model_renderer::reset_context();
        dh2::android_ui::OriginalUiSession original_ui;
        std::string ui_error;
        if (!original_ui.initialize(assets.get(), ui_error))
            throw std::runtime_error("Shared UI asset services failed: " + ui_error);
        const auto design_settings = original_ui.design_settings_table();
        model_renderer::bind_xp_design_settings(design_settings.data, design_settings.size);
        model_renderer::bind_item_text_services(&original_ui,
            [](void* context, const dh2::data::ItemTable& items,
               const dh2::data::CharacterTable& characters) {
                return static_cast<dh2::android_ui::OriginalUiSession*>(context)
                    ->item_text_services(items, characters);
            });
        model_renderer::bind_source_dialog_services({&original_ui,
            [](void* context, std::int32_t actor, std::int32_t style, std::int32_t text) {
                std::string error;
                const bool ok = static_cast<dh2::android_ui::OriginalUiSession*>(context)
                    ->enqueue_source_dialog(actor, style, text, error);
                if (!ok) SDL_Log("Source StartDialog dispatch failed: %s", error.c_str());
                return ok ? 1 : -1;
            },
            [](void* context) {
                bool active = false; std::string error;
                const bool ok = static_cast<dh2::android_ui::OriginalUiSession*>(context)
                    ->source_dialog_active(active, error);
                if (!ok) SDL_Log("Source WaitDialog predicate failed: %s", error.c_str());
                return ok ? (active ? 1 : 0) : -1;
            },
            [](void* context, std::int32_t dialog_id) {
                std::string error;
                const bool ok = static_cast<dh2::android_ui::OriginalUiSession*>(context)
                    ->enqueue_source_dialog_id(dialog_id, error);
                if (!ok) SDL_Log("Source StartDialogID dispatch failed: %s", error.c_str());
                return ok ? 1 : -1;
            }});
        FrontRuntimeContext front_runtime{assets.get(), &original_ui};
        bind_desktop_front_runtime(original_ui, front_runtime);
        // Every exit path (including a failed scenario that unwinds through the
        // catch below) must tear the gameplay world down while the UI session,
        // audio and asset owners it borrows are still alive, exactly as the normal
        // exit does; otherwise process-exit statics outlive the Level they unlock.
        struct GameplayShutdownGuard {
            dh2::android_ui::OriginalUiSession& ui;
            bool armed = true;
            ~GameplayShutdownGuard() {
                if (!armed) return;
                try {
                    model_renderer::deactivate();
                    ui.deactivate();
                } catch (const std::exception& failure) {
                    std::cerr << "Gameplay shutdown after failure also failed: " << failure.what() << '\n';
                } catch (...) {
                    std::cerr << "Gameplay shutdown after failure also failed\n";
                }
            }
        } shutdown_guard{original_ui};
        int initial_width = 1, initial_height = 1;
        bool gameplay = false;
        if (options.level_row >= 0) {
            // Keep the explicit direct row path for bounded development runs.
            // The default launch below enters the actual authored main menu.
            if (!original_ui.load_front_screen(options.runtime.string(), "loading", ui_error))
                throw std::runtime_error("Shared loading UI setup failed: " + ui_error);
            SDL_GetWindowSizeInPixels(window, &initial_width, &initial_height);
            if (!original_ui.render(initial_width, initial_height, ui_error))
                throw std::runtime_error("Shared loading UI frame failed: " + ui_error);
            const auto slot = select_or_create_source_profile(
                assets.get(), options.runtime, original_ui, options.player_name, options.player_class);
            const auto debug_level_row = options.level_row == 41 ? -1 : options.level_row;
            const auto loaded = model_renderer::start_menu_game(
                slot, assets.get(), debug_level_row, false, 0);
            const std::string expected_prefix = options.level_row == 23 ? std::string("Crypt |") :
                std::string(model_renderer::static_level_name_for_row(options.level_row)) + " |";
            if (loaded.rfind(expected_prefix, 0) != 0)
                throw std::runtime_error("Canonical source-menu row " + std::to_string(options.level_row) +
                                         " resolved a different level: " + loaded);
            std::cout << loaded << '\n';
            if (!original_ui.attach_player(options.runtime.string(), ui_error))
                throw std::runtime_error("Shared player HUD attach failed: " + ui_error);
            model_renderer::bind_gameplay_ui_session(&original_ui);
            gameplay = true;
        } else {
            if (!original_ui.load_front_screen(options.runtime.string(), "main", ui_error))
                throw std::runtime_error("Authored main menu setup failed: " + ui_error);
            std::cout << "Original main menu | click/touch authored controls | Esc quits\n";
        }
        SDL_GetWindowSizeInPixels(window, &initial_width, &initial_height);
        glViewport(0, 0, initial_width, initial_height);
        if (gameplay) model_renderer::drawable_target_changed(initial_width, initial_height);
        const auto* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const auto* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        std::cout << "Renderer: " << (renderer ? renderer : "unknown")
                  << " | GLES: " << (version ? version : "unknown") << '\n';
        std::cout << "Authored menu: mouse/touch and name typing | Gameplay: WASD move, Space attack nearest reachable enemy, drag orbit, wheel zoom | Esc quit\n";

        std::array<bool, 4> keys{};
        bool running = true, dragging = false;
        std::array<DesktopFinger, 4> fingers{};
        bool text_input_started = false;
        std::uint64_t rendered_frames = 0;
        int mouse_x = 0, mouse_y = 0;
        auto last_vitals = SDL_GetTicks64();
        std::array<int, 7> scenario_initial_vitals{};
        std::int32_t scenario_target_index = -1;
        // move-kill: ray/area skills (Mage Cold Ray, Rogue Roundhouse...) can kill
        // an enemy other than the nearest one the driver targeted, so every source
        // Monster is scanned. Monsters already dead (or at 0 HP) when the scan
        // first runs are the baseline and never count as kills.
        std::set<std::int32_t> scenario_dead_seen;
        bool scenario_dead_baseline = false;
        std::int32_t scenario_target_initial_hp = 0;
        std::string scenario_attack_result;
        bool scenario_started = false, scenario_attack_sent = false;
        // A fresh SWAMP profile starts Swamp_Escape's lifecycle (Swamp_Intro, then Movement_Tuto):
        // scenarios begin once those cutscenes are over, the budget does not count them.
        bool intro_released = false, intro_holding = false;
        std::uint64_t intro_held_frames = 0;
        // Authored cutscenes (BeginScriptedCutScene hides the HUD, locks the controllers
        // behind dialogs) met mid-scenario: the scenarios wait for the controls to return
        // and the frames spent waiting extend the bound, capped at 9000.
        std::uint64_t cutscene_wait_frames = 0;
        bool cutscene_was_active = false;
        unsigned scenario_kills = 0;
        unsigned scenario_pickups = 0, scenario_loot_frames = 0;
        std::int32_t scenario_last_item_count = -1;
        bool scenario_damage_verified = false;
        bool scenario_player_died = false, scenario_revived = false;
        std::uint64_t transition_revive_frame = 0;
        unsigned scenario_death_taps = 0;
        std::int32_t scenario_last_hp = 0;
        std::int32_t scenario_skill_state = -1;
        unsigned scenario_skill_uses = 0;
        ProbeRunner probe_runner(options, scenario_player_died, scenario_revived);
        // touch-kill: the stick (finger 0) and attack button (finger 1) are held together.
        HudTouchDriver touch_driver;
        touch_driver.session = &original_ui;
        touch_driver.window = window;
        bool touch_walk_attack_verified = false;
        unsigned touch_hits = 0, touch_pans_before = 0;
        // touch_pans_before is the HUD fingers' pan count, taken once at the first free-finger press
        // (a cutscene may release and later re-press the free finger; its pans are not HUD pans).
        bool touch_camera_started = false;
        std::int32_t touch_prev_state = -1, touch_state_before_press = -1;
        std::int32_t touch_hit_enemy = -1, touch_hit_hp = 0;
        std::uint64_t tutorial_verified_frame = 0, tutorial_finished_frame = 0;
        std::int32_t tutorial_hp_start = 0, tutorial_hp_end = 0, tutorial_hp_last = 0, tutorial_hp_lowest = 0;
        std::uint64_t tutorial_lock_frame = 0, tutorial_unlock_frame = 0;
        // first-kill-ui: touches that landed on a visible dialog, consecutive frames the
        // controllers stayed blocked, and the longest such stretch.
        unsigned dialog_taps = 0;
        std::uint64_t blocked_streak = 0, blocked_longest = 0;
        unsigned tutorial_hp_drops = 0;
        std::array<float, 3> tutorial_position{};
        float tutorial_moved = 0.0f;
        std::uint64_t fixed_attack_frame = 0;
        std::uint64_t fixed_next_attack_frame = 0;
        std::uint64_t fixed_target_dead_frame = 0;
        std::uintptr_t fixed_target_identity = 0;
        std::uint32_t fixed_attack_count = 0;
        bool fixed_kill_complete = false;
        std::string fixed_kill_failure;
        CombatReceiptSnapshot fixed_receipt_before;
        bool fixed_receipt_before_ready = false;
        std::uint32_t fixed_route_nodes = 0, fixed_route_edges = 0,
                      fixed_route_status = 0, fixed_route_waypoints = 0;
        bool fixed_route_found = false;
        constexpr std::int32_t fixed_target_index = 4;
        constexpr std::array<float, 3> fixed_target_position{-1234.43f, -369.638f, 198.437f};
        auto sync_text_input = [&] {
            const bool should_start = !gameplay && original_ui.menu_text_input_active();
            if (should_start == text_input_started) return;
            if (should_start) SDL_StartTextInput();
            else SDL_StopTextInput();
            text_input_started = should_start;
        };
        auto return_to_authored_menu = [&] {
            if (!gameplay || !original_ui.consume_gameplay_main_menu_request()) return;
            std::string transition_error;
            if (!model_renderer::unload_game_to_menu(transition_error)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not return to authored menu: %s",
                             transition_error.c_str());
                return;
            }
            if (!original_ui.load_front_screen(options.runtime.string(), "main", transition_error))
                throw std::runtime_error("Authored main menu reload failed: " + transition_error);
            gameplay = false;
            keys.fill(false);
            model_renderer::stop_movement();
            std::cout << "Returned to authored main menu\n";
        };
        // --return-to-origin: A -> B -> A. The census lists every materialized
        // ungated source Monster and the dead ones (HP <= 0) by name, so the
        // departure and the return can be compared line for line.
        const std::string origin_level = options.return_entry >= 0
            ? std::string(model_renderer::static_level_name_for_row(options.level_row)) : std::string();
        std::uint64_t return_due_frame = 0, return_census_frame = 0;
        bool return_queued = false;
        auto enemy_census = [&](const char* label) {
            std::vector<std::string> dead;
            unsigned monsters = 0;
            std::string census_error;
            for (std::int32_t index = 0; index < 1024; ++index) {
                model_renderer::GameplayEnemyStateSnapshotV1 enemy;
                if (!model_renderer::gameplay_enemy_state_snapshot(index, enemy, census_error)) continue;
                ++monsters;
                if (enemy.dead || enemy.hp <= 0) dead.push_back(enemy.name);
            }
            std::sort(dead.begin(), dead.end());
            std::string names;
            for (const auto& name : dead) names += (names.empty() ? "" : ",") + name;
            std::cout << "Enemy census | " << label << " | frame " << rendered_frames << " | monsters "
                      << monsters << " | dead " << dead.size() << " | " << names << '\n';
        };
        auto pump_front_audio = [&] {
            for (auto cue = original_ui.consume_menu_sound(); !cue.empty();
                 cue = original_ui.consume_menu_sound()) {
                std::string audio_error;
                if (!gameplay_audio.play_menu_sound(assets.get(), cue, audio_error))
                    SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Authored menu sound unavailable (%s): %s",
                                cue.c_str(), audio_error.c_str());
            }
            for (auto command = original_ui.consume_menu_audio(); !command.empty();
                 command = original_ui.consume_menu_audio()) {
                std::string audio_error;
                if (!gameplay_audio.apply_menu_audio_command(command, audio_error))
                    SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Authored menu audio command rejected (%s): %s",
                                command.c_str(), audio_error.c_str());
            }
        };
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
                else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int width = 1, height = 1;
                    SDL_GetWindowSizeInPixels(window, &width, &height);
                    glViewport(0, 0, width, height);
                    if (gameplay) model_renderer::drawable_target_changed(width, height);
                } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    if (dragging && !dispatch_touch(original_ui, window, mouse_x, mouse_y, 3, ui_error))
                        std::cerr << "HUD touch release on focus loss failed: " << ui_error << '\n';
                    for (unsigned slot = 0; slot < fingers.size(); ++slot) {
                        if (!fingers[slot].down) continue;
                        if (!dispatch_finger_touch(original_ui, window, fingers[slot].x, fingers[slot].y, 3, slot, ui_error))
                            std::cerr << "Authored touch cancel on focus loss failed: " << ui_error << '\n';
                        fingers[slot].down = false;
                    }
                    dragging = false;
                    keys.fill(false);
                    if (gameplay) model_renderer::stop_movement();
                } else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE &&
                        (!original_ui.menu_text_input_active() || gameplay)) {
                        running = false;
                    } else if (!gameplay && original_ui.menu_text_input_active()) {
                        const int code = game_swf_special_key(event.key.keysym.sym);
                        if (code >= 0 && !original_ui.menu_key_event(static_cast<std::uint32_t>(code), true, ui_error))
                            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Menu key-down failed: %s", ui_error.c_str());
                    } else if (gameplay) {
                        if (event.key.keysym.scancode == SDL_SCANCODE_W) keys[0] = true;
                        if (event.key.keysym.scancode == SDL_SCANCODE_S) keys[1] = true;
                        if (event.key.keysym.scancode == SDL_SCANCODE_A) keys[2] = true;
                        if (event.key.keysym.scancode == SDL_SCANCODE_D) keys[3] = true;
                        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                            std::cout << model_renderer::player_attack() << '\n';
                        update_movement(keys);
                    }
                } else if (event.type == SDL_KEYUP) {
                    if (!gameplay && original_ui.menu_text_input_active()) {
                        const int code = game_swf_special_key(event.key.keysym.sym);
                        if (code >= 0 && !original_ui.menu_key_event(static_cast<std::uint32_t>(code), false, ui_error))
                            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Menu key-up failed: %s", ui_error.c_str());
                    } else if (gameplay) {
                        if (event.key.keysym.scancode == SDL_SCANCODE_W) keys[0] = false;
                        if (event.key.keysym.scancode == SDL_SCANCODE_S) keys[1] = false;
                        if (event.key.keysym.scancode == SDL_SCANCODE_A) keys[2] = false;
                        if (event.key.keysym.scancode == SDL_SCANCODE_D) keys[3] = false;
                        update_movement(keys);
                        if (!keys[0] && !keys[1] && !keys[2] && !keys[3]) model_renderer::stop_movement();
                    }
                } else if (event.type == SDL_TEXTINPUT) {
                    if (!gameplay && original_ui.menu_text_input_active() &&
                        !send_menu_text(original_ui, event.text.text, ui_error))
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Menu text input failed: %s", ui_error.c_str());
                } else if (event.type == SDL_FINGERDOWN || event.type == SDL_FINGERMOTION || event.type == SDL_FINGERUP) {
                    // Up to four simultaneous fingers, each on its own source finger index.
                    unsigned slot = unsigned(fingers.size());
                    for (unsigned i = 0; i < fingers.size(); ++i)
                        if (fingers[i].down && fingers[i].touch == event.tfinger.touchId &&
                            fingers[i].finger == event.tfinger.fingerId) slot = i;
                    if (event.type == SDL_FINGERDOWN && slot == fingers.size())
                        for (unsigned i = 0; i < fingers.size() && slot == fingers.size(); ++i)
                            if (!fingers[i].down) slot = i;
                    if (slot < fingers.size()) {
                        auto& finger = fingers[slot];
                        const int action = event.type == SDL_FINGERDOWN ? 0 : event.type == SDL_FINGERMOTION ? 2 : 1;
                        if (event.type == SDL_FINGERDOWN) {
                            finger.down = true;
                            finger.touch = event.tfinger.touchId;
                            finger.finger = event.tfinger.fingerId;
                        }
                        finger.x = event.tfinger.x;
                        finger.y = event.tfinger.y;
                        if (!dispatch_finger_touch(original_ui, window, finger.x, finger.y, action, slot, ui_error))
                            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Authored touch %s failed: %s",
                                         action == 0 ? "down" : action == 2 ? "move" : "up", ui_error.c_str());
                        if (event.type == SDL_FINGERUP) finger.down = false;
                    }
                } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.which != SDL_TOUCH_MOUSEID &&
                           event.button.button == SDL_BUTTON_LEFT) {
                    dragging = true; mouse_x = event.button.x; mouse_y = event.button.y;
                    if (!dispatch_touch(original_ui, window, event.button.x, event.button.y, 0, ui_error))
                        std::cerr << "Authored UI touch down failed: " << ui_error << '\n';
                } else if (event.type == SDL_MOUSEBUTTONUP && event.button.which != SDL_TOUCH_MOUSEID &&
                           event.button.button == SDL_BUTTON_LEFT) {
                    if (!dispatch_touch(original_ui, window, event.button.x, event.button.y, 1, ui_error))
                        std::cerr << "Authored UI touch up failed: " << ui_error << '\n';
                    dragging = false;
                } else if (event.type == SDL_MOUSEMOTION && event.motion.which != SDL_TOUCH_MOUSEID &&
                           (!gameplay || dragging)) {
                    if (!dispatch_touch(original_ui, window, event.motion.x, event.motion.y, 2, ui_error))
                        std::cerr << "Authored UI pointer move failed: " << ui_error << '\n';
                    if (gameplay && dragging && original_ui.camera_pan_allowed())
                        model_renderer::orbit(float(event.motion.x - mouse_x), float(event.motion.y - mouse_y), 1.0f);
                    mouse_x = event.motion.x; mouse_y = event.motion.y;
                } else if (event.type == SDL_MOUSEWHEEL) {
                    if (gameplay) model_renderer::orbit(0.0f, 0.0f, event.wheel.y > 0 ? 0.92f : 1.08f);
                }
            }
            return_to_authored_menu();
            sync_text_input();
            intro_holding = false;
            const bool cutscene_now = gameplay && (model_renderer::script_controllers_blocked() ||
                                                   original_ui.source_cutscene_active());
            if (cutscene_now != cutscene_was_active) {
                cutscene_was_active = cutscene_now;
                std::cout << "Scenario cutscene " << (cutscene_now ? "started" : "over")
                          << " | frame " << rendered_frames << " | HUD "
                          << (cutscene_now ? "hidden, scenario waits for the controls" : "back") << std::endl;
            }
            if (gameplay && options.level_row == 41 && !intro_released &&
                (options.movement_attack_scenario || options.movement_defend_scenario ||
                 options.movement_kill_scenario || options.skill_kill_scenario ||
                 options.tutorial_potion_scenario || options.probe_scenario())) {
                bool finished = false;
                std::uintptr_t player_identity = 0;
                std::vector<model_renderer::UiQuestReadV1> quest_rows;
                if (rendered_frames >= 4 &&
                    model_renderer::ui_player_identity(player_identity, ui_error) &&
                    model_renderer::ui_player_quest_rows(player_identity, quest_rows, ui_error, nullptr) &&
                    quest_rows.size() > 50 && quest_rows[50].state >= 6 &&
                    !model_renderer::script_controllers_blocked() &&
                    !model_renderer::tutorial_script_running("Swamp_Intro") &&
                    !model_renderer::tutorial_script_running("Movement_Tuto") &&
                    !model_renderer::tutorial_script_running("Movement_Tuto2"))
                    finished = true;
                if (finished || intro_held_frames >= 9000) {
                    intro_released = true;
                    std::cout << "Scenario intro released | frame " << rendered_frames << " | held "
                              << intro_held_frames << " frames | "
                              << (finished ? "Swamp_Escape active, cutscenes over" : "hold timed out") << std::endl;
                } else {
                    intro_holding = true;
                    ++intro_held_frames;
                    ++options.frames;
                    if (rendered_frames % 20 == 0 && model_renderer::script_controllers_blocked() &&
                        !original_ui.flush_source_messages(ui_error))
                        throw std::runtime_error("Intro dialog flush failed: " + ui_error);
                }
            }
            // Cutscene time is not scenario time: a frame spent waiting for the controls
            // extends the bound by one frame, as the intro hold does below.
            if (cutscene_now && !intro_holding && !options.first_kill_ui_scenario &&
                cutscene_wait_frames < 9000) {
                ++cutscene_wait_frames;
                ++options.frames;
            }
            if (gameplay && options.faery_follow_scenario && !options.capture_series.empty() && rendered_frames == 20)
                copy_level_saves(options.runtime, options.capture_series, "entry");
            if (gameplay && options.probe_scenario() && rendered_frames >= 2 && !intro_holding) {
                probe_runner.set_hud_blocked(cutscene_now);
                probe_runner.tick(rendered_frames - intro_held_frames, SDL_GetTicks64());
                int tap_action = -1;
                if (probe_runner.take_tap(tap_action)) {
                    // Centre of the authored HUD faery button on the 1280x720 reference layout.
                    int window_width = 1, window_height = 1;
                    SDL_GetWindowSize(window, &window_width, &window_height);
                    if (!dispatch_touch(original_ui, window, window_width * options.hud_tap_x / 1280,
                                        window_height * options.hud_tap_y / 720,
                                        tap_action, ui_error))
                        plog("HUD faery button tap failed: " + ui_error);
                    else plog(tap_action == 2 ? "HUD pointer moved onto the tap point" :
                              tap_action == 0 ? "HUD tap point pressed" : "HUD tap point released");
                }
                int attack_action = -1;
                if (probe_runner.take_attack_touch(attack_action)) {
                    const bool touched = attack_action == 0 ? touch_driver.press_attack(ui_error)
                                                             : touch_driver.release_all(ui_error);
                    if (!touched) plog("HUD attack touch failed: " + ui_error);
                }
                if (probe_runner.failed()) {
                    plog("scenario failure: " + probe_runner.failure());
                    running = false;
                }
                if (probe_runner.finished()) running = false;
            }
            // touch-kill: the attack must begin while the movement stick finger is still down.
            // Sampled every frame so the one-frame walk -> attack edge cannot be missed.
            if (options.hud_touch_scenario && scenario_started && touch_driver.attack_down &&
                touch_driver.stick_down) {
                const auto state = model_renderer::gameplay_player_state();
                if (!touch_walk_attack_verified && touch_prev_state == 4 && state == 5) {
                    touch_walk_attack_verified = true;
                    std::cout << "Scenario walk-attack verified | Player state 4 -> 5 with the stick finger and the attack"
                                 " finger both held | frame " << rendered_frames << '\n';
                }
                touch_prev_state = state;
            }
            // move-kill: steer to the nearest enemy and keep attacking it through the
            // normal player_attack() route until its source HP reaches zero.
            if (options.movement_kill_scenario && scenario_started &&
                rendered_frames && rendered_frames % 15 == 0) {
                [&] {  // returns early while the Player is dead
                model_renderer::GameplayPoseSnapshotV1 pose;
                if (!model_renderer::gameplay_pose_snapshot(pose, ui_error)) {
                    // Monsters fight back: on death the Continue taps below
                    // revive the Player and the kill loop resumes.
                    if (!scenario_player_died)
                        std::cout << "Scenario move-kill | Player died | " << ui_error << '\n';
                    scenario_player_died = true;
                    scenario_revived = false;
                    return;
                }
                if (scenario_player_died && !scenario_revived) {
                    scenario_revived = true;
                    std::cout << "Scenario move-kill | Player revived | HP " << pose.player_hp << '\n';
                }
                for (std::int32_t index = 0; index < 1024; ++index) {
                    if (scenario_dead_seen.count(index)) continue;
                    model_renderer::GameplayEnemyStateSnapshotV1 target;
                    if (!model_renderer::gameplay_enemy_state_snapshot(index, target, ui_error) ||
                        !(target.dead || target.hp <= 0)) continue;
                    scenario_dead_seen.insert(index);
                    if (!scenario_dead_baseline) continue;
                    std::cout << "Scenario kill verified | " << target.name << " HP " << target.hp
                              << '/' << target.max_hp << " dead " << target.dead << '\n';
                    ++scenario_kills;
                    if (options.hud_touch_scenario && touch_driver.attack_down && touch_driver.stick_down) {
                        ++touch_hits;
                        std::cout << "Scenario touch hit verified | killing blow on " << target.name
                                  << " with the stick finger and the attack finger both held\n";
                    }
                    if (index == scenario_target_index) scenario_target_index = -1;
                }
                scenario_dead_baseline = true;
                // After a kill, walk onto the nearest dropped item and verify
                // the normal pickup removes it from the world.
                model_renderer::GameplayWorldItemSnapshotV1 items;
                const bool have_items = scenario_kills > 0 && scenario_pickups < 3 && !options.hud_touch_scenario &&
                    model_renderer::gameplay_world_item_snapshot(items, ui_error) && items.count > 0;
                if (options.skill_kill_scenario) {
                    const auto state = model_renderer::gameplay_player_state();
                    if (scenario_skill_state == 6 && state != 6)
                        std::cout << "Scenario skill used | state " << state << " | uses " << ++scenario_skill_uses << '\n';
                    scenario_skill_state = state;
                }
                if (have_items && scenario_last_item_count >= 0 && items.count < scenario_last_item_count) {
                    ++scenario_pickups;
                    std::cout << "Scenario pickup verified | world items " << scenario_last_item_count
                              << " -> " << items.count << '\n';
                }
                if (have_items) scenario_last_item_count = items.count;
                if (cutscene_now) {
                    // The HUD is hidden and the Player locked behind an authored cutscene:
                    // let go of any touch fingers and hold still until the controls return.
                    if (touch_driver.attack_down || touch_driver.stick_down || touch_driver.camera_down) {
                        std::string release_error;
                        touch_driver.release_all(release_error);
                    }
                    keys.fill(false);update_movement(keys);model_renderer::stop_movement();
                } else if (have_items && items.distance > 20.0f && scenario_loot_frames < 600) {
                    scenario_loot_frames += 15;
                    const float dx = items.nearest[0] - pose.player_position[0];
                    const float dy = items.nearest[1] - pose.player_position[1];
                    const float distance = std::hypot(dx, dy);
                    constexpr float inverse_sqrt_two = 0.7071067811865475f;
                    model_renderer::move_axis((dx + dy) * inverse_sqrt_two / distance,
                                              (dy - dx) * inverse_sqrt_two / distance);
                } else if (options.hud_touch_scenario && pose.enemy_index >= 0 && pose.enemy_hp > 0 &&
                           pose.enemy_in_melee_range) {
                    // The walking half of the device report: the stick finger stays down and keeps
                    // pushing at the enemy while a second finger presses and holds the attack button.
                    scenario_target_index = pose.enemy_index;
                    {
                        const float dx = pose.enemy_position[0] - pose.player_position[0];
                        const float dy = pose.enemy_position[1] - pose.player_position[1];
                        const float distance = std::max(std::hypot(dx, dy), 1.0f);
                        constexpr float inverse_sqrt_two = 0.7071067811865475f;
                        if (!touch_driver.drive_stick((dx + dy) * inverse_sqrt_two / distance,
                                                      (dy - dx) * inverse_sqrt_two / distance, ui_error)) {
                            // A HUD that is not on screen yet (a cutscene just ended) is waited for.
                            if (ui_error.find("no pressable area") != std::string::npos) return;
                            throw std::runtime_error("touch-kill stick drag failed: " + ui_error);
                        }
                    }
                    if (!touch_driver.attack_down) {
                        touch_state_before_press = model_renderer::gameplay_player_state();
                        if (!touch_driver.press_attack(ui_error)) {
                            if (ui_error.find("no pressable area") != std::string::npos) return;
                            throw std::runtime_error("touch-kill attack button press failed: " + ui_error);
                        }
                        // The HUD attack press starts the attack inside the touch entry itself.
                        const auto state_after_press = model_renderer::gameplay_player_state();
                        std::cout << "Scenario touch | finger 1 pressed the HUD attack button at "
                                  << touch_driver.attack_x << ',' << touch_driver.attack_y
                                  << " | stick finger held " << touch_driver.stick_down
                                  << " | Player state " << touch_state_before_press << " -> "
                                  << state_after_press << '\n';
                        if (!touch_walk_attack_verified && touch_driver.stick_down &&
                            touch_state_before_press == 4 && state_after_press == 5) {
                            touch_walk_attack_verified = true;
                            std::cout << "Scenario walk-attack verified | the attack press took the Player from walking"
                                         " (state 4) to attacking (state 5) while the stick finger stayed down\n";
                        }
                    }
                    {
                        // A third, free finger on the world pans the camera; the two HUD fingers never do.
                        unsigned pans = 0, applied = 0, pinches = 0;
                        if (!touch_camera_started) {
                            original_ui.camera_gesture_counts(pans, applied, pinches);
                            touch_pans_before = pans;
                            touch_camera_started = true;
                        }
                        if (!touch_driver.drag_camera_finger(ui_error))
                            throw std::runtime_error("touch-kill free camera finger failed: " + ui_error);
                    }
                    if (touch_hit_enemy != pose.enemy_index) {
                        touch_hit_enemy = pose.enemy_index;
                        touch_hit_hp = pose.enemy_hp;
                    } else if (pose.enemy_hp < touch_hit_hp) {
                        ++touch_hits;
                        std::cout << "Scenario touch hit verified | enemy " << pose.enemy_index << " HP "
                                  << touch_hit_hp << " -> " << pose.enemy_hp << " | stick held "
                                  << touch_driver.stick_down << " | attack held " << touch_driver.attack_down
                                  << " | Player state " << model_renderer::gameplay_player_state() << '\n';
                        touch_hit_hp = pose.enemy_hp;
                    }
                } else if (pose.enemy_index >= 0 && pose.enemy_hp > 0 && pose.enemy_in_melee_range) {
                    keys.fill(false);update_movement(keys);model_renderer::stop_movement();
                    scenario_target_index = pose.enemy_index;
                    if (options.skill_kill_scenario) {
                        // Same entry the authored HUD calls on release (NativeHUDSkill).
                        if (scenario_skill_state != 6) {
                            std::uintptr_t player = 0;std::array<std::int32_t, 3> slots{};
                            if (!model_renderer::ui_player_identity(player, ui_error) || !player ||
                                !model_renderer::ui_player_skill_slots(player, slots, ui_error) || slots[0] < 0)
                                throw std::runtime_error("skill-kill requires a skill equipped in HUD slot 0: " + ui_error);
                            const bool used = model_renderer::ui_player_use_skill_slot(player, 0, ui_error);
                            std::cout << "Scenario skill | enemy " << pose.enemy_index << " HP " << pose.enemy_hp
                                      << '/' << pose.enemy_max_hp << " | slot 0 skill " << slots[0]
                                      << " | state " << model_renderer::gameplay_player_state()
                                      << " | use " << (used ? "ok" : "error: " + ui_error) << '\n';
                            // A skill refused for mana/cooldown leaves the Player in its prior
                            // state; after repeated refusals fall back to a basic attack.
                            static unsigned refused = 0;
                            refused = model_renderer::gameplay_player_state() == 6 ? 0 : refused + 1;
                            if (refused > 6) {
                                const auto attack = model_renderer::player_attack();
                                std::cout << "Scenario skill fallback attack | " << attack << '\n';
                            }
                        }
                    } else {
                        const auto attack = model_renderer::player_attack();
                        std::cout << "Scenario attack | enemy " << pose.enemy_index << " HP " << pose.enemy_hp
                                  << '/' << pose.enemy_max_hp << " | " << attack << '\n';
                    }
                } else if (pose.enemy_index >= 0) {
                    steer_toward_nearest_enemy();
                }
                }();
            }
            // move-defend: approach the nearest enemy without attacking and verify that
            // its source melee attack lowers the Player's HP.
            if (options.movement_defend_scenario && scenario_started &&
                rendered_frames && rendered_frames % 15 == 0 && !scenario_player_died) {
                model_renderer::GameplayPoseSnapshotV1 pose;
                if (!model_renderer::gameplay_pose_snapshot(pose, ui_error)) {
                    if (!scenario_player_died)
                        std::cout << "Scenario defend | Player died | " << ui_error << '\n';
                    scenario_damage_verified = scenario_player_died = true;
                } else {
                    if (!scenario_damage_verified && scenario_last_hp > 0 && pose.player_hp < scenario_last_hp) {
                        std::cout << "Scenario damage verified | Player HP " << scenario_last_hp << " -> "
                                  << pose.player_hp << '/' << pose.player_max_hp << '\n';
                        scenario_damage_verified = true;
                    }
                    scenario_last_hp = pose.player_hp;
                    if (pose.enemy_index >= 0 && pose.enemy_in_melee_range) {
                        keys.fill(false);update_movement(keys);model_renderer::stop_movement();
                    } else if (pose.enemy_index >= 0) {
                        steer_toward_nearest_enemy();
                    }
                }
            }
            // tutorial-potion (move-defend driver): once the Player's HP is at or below
            // half of Max_HP, Character::HitFor 0x3a8bc4 must have started the SWAMP
            // script cinematic_Tuto_potionUse through the Level ScriptManager session,
            // exactly once (settings tutorial byte 4 is then cleared and saved). While
            // LockCharacter("All") holds every controller the Player must neither lose
            // HP nor move: the dialogs are held for 240 frames to prove it, then flushed
            // (Script_FlushMessages stands in for the player's taps) until the whole
            // script, including EndScriptedCutScene, has finished. Animation events of a
            // swing already under way are not lock-gated (CharAI::RaiseAIEvent case 40),
            // so a drop in the first 60 locked frames is that swing finishing and only
            // logged; later drops fail the scenario.
            if (options.tutorial_potion_scenario && scenario_started && !tutorial_finished_frame) {
                model_renderer::GameplayPoseSnapshotV1 pose;
                if (!tutorial_verified_frame) {
                    const auto started = model_renderer::tutorial_script_start_count("cinematic_Tuto_potionUse");
                    if (started) {
                        tutorial_verified_frame = rendered_frames + 1;
                        if (model_renderer::gameplay_pose_snapshot(pose, ui_error)) {
                            tutorial_hp_start = pose.player_hp;
                            std::cout << "Scenario tutorial verified | cinematic_Tuto_potionUse started "
                                      << started << " time(s) | Player HP " << pose.player_hp << '/'
                                      << pose.player_max_hp << " (half " << pose.player_max_hp / 2 << ")" << std::endl;
                        } else {
                            std::cout << "Scenario tutorial verified | cinematic_Tuto_potionUse started "
                                      << started << " time(s) | Player died on the same hit" << std::endl;
                        }
                    }
                } else {
                    const bool alive = model_renderer::gameplay_pose_snapshot(pose, ui_error);
                    const bool blocked = model_renderer::script_controllers_blocked();
                    if (alive) tutorial_hp_end = pose.player_hp;
                    if (alive && blocked && !tutorial_lock_frame) {
                        tutorial_lock_frame = rendered_frames;
                        tutorial_hp_last = tutorial_hp_lowest = pose.player_hp;
                        tutorial_position = pose.player_position;
                        std::cout << "Scenario tutorial locked | frame +" << rendered_frames - tutorial_verified_frame
                                  << " | Player HP " << pose.player_hp << std::endl;
                    } else if (alive && tutorial_lock_frame && !tutorial_unlock_frame) {
                        if (pose.player_hp < tutorial_hp_last) {
                            const bool in_flight = rendered_frames < tutorial_lock_frame + 60;
                            if (!in_flight) ++tutorial_hp_drops;
                            std::cout << "Scenario tutorial HP drop while locked | frame +"
                                      << rendered_frames - tutorial_verified_frame << " | "
                                      << tutorial_hp_last << " -> " << pose.player_hp
                                      << (in_flight ? " | swing already under way" : "") << std::endl;
                        }
                        tutorial_hp_last = pose.player_hp;
                        tutorial_hp_lowest = std::min(tutorial_hp_lowest, pose.player_hp);
                        tutorial_moved = std::max(tutorial_moved, std::hypot(
                            pose.player_position[0] - tutorial_position[0],
                            pose.player_position[1] - tutorial_position[1]));
                    }
                    if (tutorial_lock_frame && !tutorial_unlock_frame && !blocked) {
                        tutorial_unlock_frame = rendered_frames;
                        std::cout << "Scenario tutorial unlocked | locked for " << rendered_frames - tutorial_lock_frame
                                  << " frames | Player HP " << tutorial_hp_last << " lowest " << tutorial_hp_lowest
                                  << " | HP drops " << tutorial_hp_drops << " | Player moved " << tutorial_moved << std::endl;
                    }
                    if (rendered_frames >= tutorial_verified_frame + 240) {
                        if (!model_renderer::tutorial_script_running("cinematic_Tuto_potionUse")) {
                            std::cout << "Scenario tutorial completed | cinematic_Tuto_potionUse finished after "
                                      << rendered_frames - tutorial_verified_frame << " frames | Player HP start "
                                      << tutorial_hp_start << " end " << tutorial_hp_end << std::endl;
                            tutorial_finished_frame = rendered_frames;
                        } else if (rendered_frames % 20 == 0 && !original_ui.flush_source_messages(ui_error)) {
                            throw std::runtime_error("Tutorial dialog flush failed: " + ui_error);
                        }
                    }
                }
            }
            // The 60-frame tail after the cinematic ends only once a death is revived: the
            // move-defend driver still owes its Continue revive if the Player fell in the tail.
            if (options.tutorial_potion_scenario && tutorial_finished_frame &&
                rendered_frames >= tutorial_finished_frame + 60 &&
                !(scenario_player_died && !scenario_revived)) running = false;
            // first-kill-ui: a player taps the dialog box to read on. Press and release
            // are separate frames so GameSWF sees both edges; the dialog queue is never
            // flushed, so a dialog the UI cannot show or dismiss keeps the Player locked
            // and fails the scenario below.
            if (options.first_kill_ui_scenario && gameplay) {
                const bool blocked_now = model_renderer::script_controllers_blocked();
                blocked_streak = blocked_now ? blocked_streak + 1 : 0;
                blocked_longest = std::max(blocked_longest, blocked_streak);
                bool dialog_active = false;
                if (!original_ui.source_dialog_active(dialog_active, ui_error))
                    throw std::runtime_error("Dialog activity query failed: " + ui_error);
                const auto phase = rendered_frames % 24;
                if (dialog_active && (phase == 0 || phase == 6)) {
                    int window_width = 1, window_height = 1;
                    SDL_GetWindowSize(window, &window_width, &window_height);
                    if (!dispatch_touch(original_ui, window, window_width / 2, window_height / 2,
                                        phase == 0 ? 0 : 1, ui_error))
                        throw std::runtime_error("Dialog tap failed: " + ui_error);
                    if (phase == 6) {
                        ++dialog_taps;
                        if (dialog_taps == 1 || dialog_taps % 10 == 0)
                            std::cout << "Scenario dialog tap " << dialog_taps << " | frame " << rendered_frames
                                      << " | controllers blocked " << blocked_now << std::endl;
                    }
                }
            }
            // skill-matrix applies real Character::LevelUp steps, which start the
            // authored Level Up tutorial script (LockCharacter "All", BlockSaveGame and
            // dialogs that wait for the player's taps). Flush its dialogs like those
            // taps would so the probe is not parked behind the script until the bound.
            // boss-natural: the boss intro cutscene waits for the player's dialog taps.
            // Every Level TriggerZone is a live ObjectManager object now, so the walking
            // scenarios cross the authored LizardMan_Intro / CombatTuto scripts too; they
            // lock the controllers behind dialogs the player taps through. The
            // tutorial-potion scenario keeps its own hold-then-flush proof above once
            // cinematic_Tuto_potionUse has started; before that it taps these too.
            // Default for every gameplay scenario: while an authored cutscene holds the HUD
            // or the controllers, its dialogs are flushed every 20 frames (what a player's
            // taps amount to). first-kill-ui taps the DialogBox itself instead, unless
            // --flush-dialogs asks for the flush; tutorial-potion holds its dialogs for 240
            // frames once cinematic_Tuto_potionUse has started, before its own flush below.
            if ((!options.first_kill_ui_scenario || options.flush_dialogs) &&
                gameplay && rendered_frames % 20 == 0 && cutscene_now &&
                !(options.tutorial_potion_scenario && tutorial_verified_frame) &&
                !original_ui.flush_source_messages(ui_error) &&
                model_renderer::script_controllers_blocked())
                throw std::runtime_error("Cutscene dialog flush failed: " + ui_error);
            // After death, tap the authored death screen's btn_Continue (full-screen
            // hit area) so its timeline reaches NativeReviveAllPlayers.
            if ((options.movement_defend_scenario || options.movement_kill_scenario ||
                 options.probe_scenario()) &&
                scenario_player_died && !scenario_revived &&
                (rendered_frames % 120 == 60 || rendered_frames % 120 == 66)) {
                // Press and release on separate frames so GameSWF sees the edge.
                int window_width = 1, window_height = 1;
                SDL_GetWindowSize(window, &window_width, &window_height);
                const int phase = rendered_frames % 120 == 60 ? 0 : 1;
                if (!dispatch_touch(original_ui, window, window_width / 2, window_height / 2, phase, ui_error))
                    throw std::runtime_error("Death screen Continue tap failed: " + ui_error);
                if (phase == 1) ++scenario_death_taps;
                model_renderer::GameplayPoseSnapshotV1 pose;
                if (model_renderer::gameplay_pose_snapshot(pose, ui_error) && pose.player_hp > 0) {
                    std::cout << "Scenario revive verified | Player HP " << pose.player_hp << '/'
                              << pose.player_max_hp << " | taps " << scenario_death_taps << '\n';
                    scenario_revived = true;
                }
            }
            if (options.movement_attack_scenario && !options.movement_kill_scenario &&
                scenario_started && !scenario_attack_sent) {
                if (rendered_frames >= options.frames / 2 && !cutscene_now) {
                    keys.fill(false);
                    update_movement(keys);
                    model_renderer::stop_movement();
                    model_renderer::GameplayPoseSnapshotV1 pose;
                    if (!model_renderer::gameplay_pose_snapshot(pose, ui_error))
                        throw std::runtime_error("Shared attack pose snapshot failed: " + ui_error);
                    std::cout << "Scenario attack candidate " << pose.enemy_index << " at "
                              << pose.enemy_position[0] << ',' << pose.enemy_position[1]
                              << " HP " << pose.enemy_hp << '/' << pose.enemy_max_hp << '\n';
                    scenario_target_index = pose.enemy_index;
                    scenario_target_initial_hp = pose.enemy_hp;
                    scenario_attack_result = model_renderer::player_attack();
                    std::cout << "Scenario nearest-enemy attack result: " << scenario_attack_result << '\n';
                    scenario_attack_sent = true;
                } else if (rendered_frames && rendered_frames % 30 == 0) {
                    steer_toward_nearest_enemy();
                }
            }
            if (options.fixed_cultist_scenario && scenario_started && !scenario_attack_sent &&
                rendered_frames && rendered_frames % 30 == 0) {
                model_renderer::GameplayEnemyStateSnapshotV1 target_state;
                if (!model_renderer::gameplay_enemy_state_snapshot(fixed_target_index, target_state, ui_error))
                    throw std::runtime_error("Fixed Crypt target snapshot failed: " + ui_error);
                if (target_state.name != "_prim_tmp_cultist07")
                    throw std::runtime_error("Fixed Crypt target identity changed: " + target_state.name);
                model_renderer::GameplayPoseSnapshotV1 pose;
                if (!model_renderer::gameplay_pose_snapshot(pose, ui_error))
                    throw std::runtime_error("Fixed Crypt target pose failed: " + ui_error);
                const float target_distance = std::hypot(
                    pose.player_position[0] - fixed_target_position[0],
                    pose.player_position[1] - fixed_target_position[1]);
                if (pose.enemy_index == fixed_target_index &&
                    pose.enemy_in_melee_range) {
                    keys.fill(false);
                    update_movement(keys);
                    model_renderer::stop_movement();
                    scenario_target_index = fixed_target_index;
                    scenario_target_initial_hp = target_state.hp;
                    scenario_attack_result = model_renderer::player_attack();
                    scenario_attack_sent = true;
                    fixed_attack_frame = rendered_frames;
                    std::cout << "Fixed Crypt attack | index " << fixed_target_index
                              << " | name " << target_state.name << " | player-target XY "
                              << target_distance << " | HP " << target_state.hp << '/'
                              << target_state.max_hp << " | result " << scenario_attack_result << '\n';
                } else {
                    if (rendered_frames % 300 == 0)
                        std::cout << "Fixed Crypt approach progress | nearest " << pose.enemy_index
                                  << " | target XY " << target_distance << " | target HP "
                                  << target_state.hp << '/' << target_state.max_hp
                                  << " | live melee reach " << pose.enemy_in_melee_range << '\n';
                    if (!steer_toward_fixed_position(fixed_target_position, fixed_route_nodes,
                            fixed_route_edges, fixed_route_status, fixed_route_found,
                            fixed_route_waypoints)) {
                        std::cout << "Fixed Crypt route blocked | nearest " << pose.enemy_index
                                  << " | target XY " << target_distance << " | graph nodes "
                                  << fixed_route_nodes << " | edges " << fixed_route_edges
                                  << " | status " << fixed_route_status << " | found "
                                  << fixed_route_found << " | waypoints " << fixed_route_waypoints << '\n';
                        throw std::runtime_error("No selected-floor path to fixed Crypt target");
                    }
                }
            }
            if (options.fixed_cultist_kill_scenario && scenario_started &&
                !fixed_kill_complete && fixed_kill_failure.empty() &&
                rendered_frames && rendered_frames % 15 == 0) {
                model_renderer::GameplayEnemyStateSnapshotV1 target_state;
                if (!model_renderer::gameplay_enemy_state_snapshot(
                        fixed_target_index, target_state, ui_error)) {
                    fixed_kill_failure = "Fixed Crypt target snapshot failed: " + ui_error;
                } else if (target_state.name != "_prim_tmp_cultist07") {
                    fixed_kill_failure = "Fixed Crypt target identity changed: " + target_state.name;
                } else if (target_state.dead || target_state.hp <= 0) {
                    if (!fixed_target_dead_frame) {
                        fixed_target_dead_frame = rendered_frames + 1;
                        keys.fill(false);
                        update_movement(keys);
                        model_renderer::stop_movement();
                        std::cout << "Fixed Crypt target death observed | " << target_state.name
                                  << " | HP " << target_state.hp << '/' << target_state.max_hp
                                  << " | attack attempts " << fixed_attack_count << '\n';
                    }
                } else {
                    model_renderer::GameplayPoseSnapshotV1 pose;
                    if (!model_renderer::gameplay_pose_snapshot(pose, ui_error)) {
                        fixed_kill_failure = "Fixed Crypt target pose failed: " + ui_error;
                    } else if (pose.enemy_index != fixed_target_index) {
                        if (!steer_toward_fixed_position(fixed_target_position, fixed_route_nodes,
                                fixed_route_edges, fixed_route_status, fixed_route_found,
                                fixed_route_waypoints))
                            fixed_kill_failure = "No selected-floor path to fixed Crypt target";
                    } else {
                        const float target_distance = std::hypot(
                            pose.player_position[0] - fixed_target_position[0],
                            pose.player_position[1] - fixed_target_position[1]);
                        if (!pose.enemy_in_melee_range) {
                            if (!steer_toward_fixed_position(fixed_target_position, fixed_route_nodes,
                                    fixed_route_edges, fixed_route_status, fixed_route_found,
                                    fixed_route_waypoints))
                                fixed_kill_failure = "No selected-floor path to fixed Crypt target";
                        } else if (!fixed_target_dead_frame &&
                                   rendered_frames >= fixed_next_attack_frame) {
                            keys.fill(false);
                            update_movement(keys);
                            model_renderer::stop_movement();
                            // The SDL Space handler calls this same source-owned
                            // entry; cooldown and animation acceptance stay live.
                            const auto attack_result = model_renderer::player_attack();
                            std::cout << "Fixed Crypt normal attack | attempt "
                                      << (fixed_attack_count + 1) << " | candidate " << pose.enemy_index
                                      << " | target XY " << target_distance << " | HP "
                                      << target_state.hp << '/' << target_state.max_hp
                                      << " | result " << attack_result << '\n';
                            if (attack_result == "Attacking") {
                                std::uintptr_t player = 0;
                                model_renderer::UiEnemyHudProjectionV1 hud_target;
                                if (!model_renderer::ui_player_identity(player, ui_error) ||
                                    !model_renderer::ui_player_enemy_hud_projection(
                                        player, hud_target, ui_error)) {
                                    fixed_kill_failure = "Player attack target projection failed: " + ui_error;
                                } else if (!hud_target.character_target.present ||
                                    !hud_target.character_target.debug_name ||
                                    std::string_view(hud_target.character_target.debug_name) !=
                                        "_prim_tmp_cultist07") {
                                    fixed_kill_failure = "Default Player attack selected a different target";
                                } else if (fixed_target_identity &&
                                    fixed_target_identity != hud_target.character_target.identity) {
                                    fixed_kill_failure = "Fixed Crypt target Character identity changed";
                                } else {
                                    fixed_target_identity = hud_target.character_target.identity;
                                    scenario_target_index = fixed_target_index;
                                    if (fixed_attack_count == 0) {
                                        scenario_target_initial_hp = target_state.hp;
                                        fixed_attack_frame = rendered_frames;
                                        scenario_attack_result = attack_result;
                                    }
                                    ++fixed_attack_count;
                                    scenario_attack_sent = true;
                                    fixed_next_attack_frame = rendered_frames + 60;
                                    std::cout << "Fixed Crypt selected target identity "
                                              << fixed_target_identity << " | Player AI target "
                                              << (hud_target.ai_target.debug_name ?
                                                  hud_target.ai_target.debug_name : "<none>") << '\n';
                                }
                            } else if (attack_result == "Attack is cooling down" ||
                                       attack_result == "Attack is already in progress") {
                                fixed_next_attack_frame = rendered_frames + 15;
                            } else {
                                fixed_kill_failure = "Normal Player attack failed: " + attack_result;
                            }
                        }
                        if (rendered_frames % 300 == 0)
                            std::cout << "Fixed Crypt kill progress | nearest " << pose.enemy_index
                                      << " | target XY " << target_distance << " | target HP "
                                      << target_state.hp << '/' << target_state.max_hp
                                      << " | live melee reach " << pose.enemy_in_melee_range
                                      << " | attacks " << fixed_attack_count << '\n';
                    }
                }
            }
            int width = 1, height = 1;
            SDL_GetWindowSizeInPixels(window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0.08f, 0.09f, 0.11f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            if (gameplay) {
                model_renderer::draw(width, height);
                if (!model_renderer::active())
                    throw std::runtime_error("Shared renderer deactivated during gameplay frame; frame rejected");
                PlayerUiFrame player_ui{&original_ui, width, height};
                if (!model_renderer::with_player_status_sheet(&player_ui, draw_player_ui, ui_error))
                    throw std::runtime_error("Shared player HUD frame failed: " + ui_error);
            } else if (!original_ui.render(width, height, ui_error)) {
                throw std::runtime_error("Authored main menu frame failed: " + ui_error);
            }
            dh2::android_ui::NativeStartGameIntentV1 launch_request;
            if (gameplay && !options.transition_to.empty()) {
                bool transition_due = rendered_frames == options.transition_frame;
                if (options.transition_after_revive) {
                    if (scenario_revived && !transition_revive_frame)
                        transition_revive_frame = rendered_frames;
                    transition_due = transition_revive_frame &&
                                     rendered_frames == transition_revive_frame + 120;
                }
                if (transition_due) {
                    if (options.return_entry >= 0) enemy_census("departure");
                    original_ui.queue_level_transition(options.transition_to, options.transition_entry);
                }
            }
            if (gameplay && std::find(options.census_frames.begin(), options.census_frames.end(),
                                      rendered_frames) != options.census_frames.end())
                enemy_census("requested");
            if (gameplay) {
                for (const auto& start : options.script_starts)
                    if (rendered_frames == start.frame)
                        plog("PROBE start-script " + start.name + " | frame " + std::to_string(rendered_frames) +
                             " | " + model_renderer::debug_start_level_script(start.name));
                for (const auto& anim : options.play_anims)
                    if (rendered_frames == anim.frame)
                        plog("PROBE play-anim " + anim.target + " " + anim.dict +
                             (anim.second.empty() ? "" : "," + anim.second) +
                             (anim.wait ? " wait" : "") + " | frame " + std::to_string(rendered_frames) + " | " +
                             model_renderer::debug_character_play_anim(anim.target, anim.dict, anim.second, anim.wait));
            }
            if (gameplay && options.return_entry >= 0) {
                if (return_due_frame && !return_queued && rendered_frames == return_due_frame) {
                    return_queued = true;
                    plog("returning to the origin level | " + origin_level);
                    original_ui.queue_level_transition(origin_level, double(options.return_entry));
                }
                if (return_census_frame && rendered_frames == return_census_frame)
                    enemy_census("return");
            }
            if (gameplay && options.faery_follow_scenario && !options.follow_exit_destination.empty() &&
                options.frames > 8 && rendered_frames + 3 == options.frames) {
                probe_runner.finish();
                plog("leaving the level to publish its LevelSavegame | " + options.follow_exit_destination);
                original_ui.queue_level_transition(options.follow_exit_destination, options.follow_exit_entry);
            }
            if (gameplay && original_ui.consume_launch_request(launch_request)) {
                // NativeGoToZone / Script_ChangeLevel: the shared transition owner.
                if (launch_request.kind ==
                    dh2::android_ui::NativeStartGameIntentV1::Kind::level_transition) {
                    const auto transition = model_renderer::transition_level(
                        launch_request.destination_name, launch_request.raw_entry_point,
                        assets.get(), &original_ui,
                        [](void* raw, bool source_flag, const std::string& first,
                           const std::string& second, std::int32_t entry, std::string& error) {
                            auto* ui = static_cast<dh2::android_ui::OriginalUiSession*>(raw);
                            return ui && ui->display_fast_travel(source_flag, first, second, entry, error);
                        });
                    std::cout << "Level transition | " << launch_request.destination_name
                              << " | " << transition << '\n';
                    if (options.return_entry >= 0) {
                        if (!return_queued && !return_due_frame)
                            return_due_frame = rendered_frames + options.return_delay;
                        else if (return_queued && !return_census_frame)
                            return_census_frame = rendered_frames + 10;
                    }
                }
            } else if (!gameplay && original_ui.consume_launch_request(launch_request)) {
                if (!start_from_authored_menu(launch_request, assets.get(), options.runtime,
                                              original_ui, gameplay, ui_error))
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", ui_error.c_str());
                keys.fill(false);
                model_renderer::stop_movement();
                sync_text_input();
            }
            pump_front_audio();
            gameplay_audio.pump(assets.get());
            if ((options.movement_attack_scenario || options.fixed_cultist_scenario ||
                 options.fixed_cultist_kill_scenario) && scenario_attack_sent &&
                (rendered_frames + 1) % 30 == 0) {
                model_renderer::GameplayEnemyStateSnapshotV1 target_state;
                if (!model_renderer::gameplay_enemy_state_snapshot(
                        scenario_target_index, target_state, ui_error)) {
                    if (options.fixed_cultist_kill_scenario)
                        fixed_kill_failure = "Shared exact-target HP snapshot failed: " + ui_error;
                    else
                        throw std::runtime_error("Shared exact-target HP snapshot failed: " + ui_error);
                } else {
                    std::cout << "Scenario target HP | " << target_state.name << ' '
                              << target_state.hp << '/' << target_state.max_hp
                              << " | delta " << target_state.hp - scenario_target_initial_hp
                              << " | dead " << target_state.dead << '\n';
                    if (options.fixed_cultist_kill_scenario && target_state.dead &&
                        !fixed_target_dead_frame)
                        fixed_target_dead_frame = rendered_frames + 1;
                }
            }
            if (gameplay && (options.movement_attack_scenario || options.movement_defend_scenario || options.fixed_cultist_scenario ||
                             options.fixed_cultist_kill_scenario) && !scenario_started && !intro_holding &&
                !cutscene_now) {
                scenario_initial_vitals = model_renderer::player_vitals();
                print_vitals_snapshot("Scenario initial", scenario_initial_vitals);
                model_renderer::move_axis(0.0f, 0.0f);
                if (options.hud_touch_scenario)
                    g_scenario_axis = [&](float x, float y) {
                        // No stick press while an authored cutscene hides the HUD: the finger
                        // would reach the world and pan the camera instead of steering.
                        if (model_renderer::script_controllers_blocked() || original_ui.source_cutscene_active())
                            return;
                        std::string stick_error;
                        if (!touch_driver.drive_stick(x, y, stick_error))
                            std::cout << "Scenario touch | stick not ready yet: " << stick_error << '\n';
                    };
                if (any_fixed_cultist_scenario) {
                    model_renderer::GameplayEnemyStateSnapshotV1 target_state;
                    if (!model_renderer::gameplay_enemy_state_snapshot(fixed_target_index, target_state, ui_error))
                        throw std::runtime_error("Fixed Crypt target snapshot failed: " + ui_error);
                    std::cout << "Fixed Crypt target baseline | index " << fixed_target_index
                              << " | name " << target_state.name << " | HP " << target_state.hp
                              << '/' << target_state.max_hp << " | position "
                              << fixed_target_position[0] << ',' << fixed_target_position[1]
                              << ',' << fixed_target_position[2] << '\n';
                    if (target_state.name != "_prim_tmp_cultist07")
                        throw std::runtime_error("Fixed Crypt target identity differs from expected candidate 4");
                    if (options.fixed_cultist_kill_scenario) {
                        if (!read_combat_receipt(fixed_receipt_before, ui_error))
                            throw std::runtime_error("Initial Player XP/inventory snapshot failed: " + ui_error);
                        fixed_receipt_before_ready = true;
                        print_combat_receipt("Fixed Crypt player receipt before", fixed_receipt_before);
                    }
                    if (!steer_toward_fixed_position(fixed_target_position, fixed_route_nodes,
                            fixed_route_edges, fixed_route_status, fixed_route_found,
                            fixed_route_waypoints))
                        throw std::runtime_error("No selected-floor path to fixed Crypt target");
                } else if (!steer_toward_nearest_enemy()) {
                    throw std::runtime_error("No nearest source enemy is available for move-attack");
                }
                scenario_started = true;
                if (!options.initial_capture.empty()) save_capture(options.initial_capture, width, height);
            }
            if (gameplay && !options.capture_series.empty() && options.capture_every &&
                rendered_frames && rendered_frames % options.capture_every == 0) {
                char suffix[32];
                std::snprintf(suffix, sizeof suffix, "_%05llu.ppm", (unsigned long long)rendered_frames);
                save_capture(std::filesystem::path(options.capture_series.string() + suffix), width, height);
            }
            bool capture_saved_this_frame = false;
            if (options.frames && rendered_frames + 1 == options.frames && !options.capture.empty()) {
                save_capture(options.capture, width, height);
                capture_saved_this_frame = true;
            }
            const auto completed_frame = rendered_frames + 1;
            const bool fixed_attack_settled = options.fixed_cultist_scenario &&
                scenario_attack_sent && completed_frame >= fixed_attack_frame + 120 &&
                (!options.frames || completed_frame < options.frames);
            const bool fixed_kill_dead_settled = options.fixed_cultist_kill_scenario &&
                fixed_target_dead_frame && completed_frame >= fixed_target_dead_frame + 120;
            if (!capture_saved_this_frame && !options.capture.empty() &&
                (fixed_attack_settled || fixed_kill_dead_settled ||
                 (options.fixed_cultist_kill_scenario && !fixed_kill_failure.empty()))) {
                save_capture(options.capture, width, height);
                capture_saved_this_frame = true;
            }
            if (fixed_kill_dead_settled) {
                fixed_kill_complete = true;
                std::cout << "Fixed Crypt kill scenario completed its 120-frame post-death observation\n";
                running = false;
            }
            if (options.fixed_cultist_kill_scenario && !fixed_kill_failure.empty()) {
                std::cout << "Fixed Crypt kill scenario stopped on source failure: "
                          << fixed_kill_failure << '\n';
                running = false;
            }
            eglSwapBuffers(display, surface);
            ++rendered_frames;
            if (options.frames && rendered_frames >= options.frames) running = false;
            if (options.fixed_cultist_scenario && scenario_attack_sent &&
                rendered_frames >= fixed_attack_frame + 120) {
                running = false;
            }
            const auto now = SDL_GetTicks64();
            if (gameplay && now - last_vitals >= 1000) { print_vitals(); last_vitals = now; }
        }

        model_renderer::stop_movement();
        if (options.return_entry >= 0 && gameplay) {
            enemy_census("final");
            if (!return_census_frame)
                throw std::runtime_error("--return-to-origin never completed the A -> B -> A transitions inside the frame bound");
        }
        if (options.faery_follow_scenario && gameplay && !options.capture_series.empty())
            copy_level_saves(options.runtime, options.capture_series, "exit");
        if (options.probe_scenario() && gameplay) {
            if (!(options.faery_follow_scenario && !options.follow_exit_destination.empty() && options.frames > 8))
                probe_runner.finish();
            if (probe_runner.failed())
                throw std::runtime_error("probe scenario failed: " + probe_runner.failure());
        }
        if (options.fixed_cultist_kill_scenario && gameplay) {
            if (!fixed_receipt_before_ready)
                throw std::runtime_error("Fixed Crypt kill scenario did not capture its initial Player receipt");
            CombatReceiptSnapshot receipt_after;
            if (!read_combat_receipt(receipt_after, ui_error))
                throw std::runtime_error("Final Player XP/inventory snapshot failed: " + ui_error);
            print_combat_receipt("Fixed Crypt player receipt after", receipt_after);
            print_combat_receipt_delta(fixed_receipt_before, receipt_after);
            model_renderer::GameplayEnemyStateSnapshotV1 target_state;
            if (!model_renderer::gameplay_enemy_state_snapshot(
                    fixed_target_index, target_state, ui_error))
                throw std::runtime_error("Final fixed Crypt target snapshot failed: " + ui_error);
            std::cout << "Fixed Crypt kill result | " << target_state.name << ' '
                      << target_state.hp << '/' << target_state.max_hp
                      << " | dead " << target_state.dead
                      << " | attacks " << fixed_attack_count
                      << " | selected identity " << fixed_target_identity << '\n';
            if (!fixed_kill_failure.empty())
                throw std::runtime_error(fixed_kill_failure);
            if (!fixed_kill_complete || !target_state.dead || target_state.hp > 0)
                throw std::runtime_error("fixed-cultist07-kill reached its frame bound before verified death");
            if (fixed_attack_count == 0 || !fixed_target_identity)
                throw std::runtime_error("fixed-cultist07-kill completed without a verified normal attack");
        }
        if (options.first_kill_ui_scenario && gameplay) {
            bool dialog_active = false;
            if (!original_ui.source_dialog_active(dialog_active, ui_error))
                throw std::runtime_error("Final dialog activity query failed: " + ui_error);
            const bool blocked_at_end = model_renderer::script_controllers_blocked();
            std::cout << "Scenario first-kill-ui | kills " << scenario_kills << " | dialog taps " << dialog_taps
                      << " | longest controller lock " << blocked_longest << " frames | locked at end "
                      << blocked_at_end << " | dialog active at end " << dialog_active
                      << " | potionUse starts "
                      << model_renderer::tutorial_script_start_count("cinematic_Tuto_potionUse")
                      << " | levelUp starts "
                      << model_renderer::tutorial_script_start_count("cinematic_Tuto_levelUp") << std::endl;
            if (!scenario_kills)
                throw std::runtime_error("first-kill-ui reached its frame bound without a verified kill");
            if (blocked_at_end || dialog_active)
                throw std::runtime_error("first-kill-ui ended with the Player still locked behind a dialog (freeze)");
            if (model_renderer::tutorial_script_running("cinematic_Tuto_potionUse") ||
                model_renderer::tutorial_script_running("cinematic_Tuto_levelUp"))
                throw std::runtime_error("first-kill-ui ended with a tutorial cinematic still running (freeze)");
            if ((model_renderer::tutorial_script_start_count("cinematic_Tuto_potionUse") ||
                 model_renderer::tutorial_script_start_count("cinematic_Tuto_levelUp")) && !dialog_taps)
                throw std::runtime_error("first-kill-ui finished a tutorial without any dialog touch");
        }
        if (options.tutorial_potion_scenario && gameplay) {
            if (!tutorial_verified_frame)
                throw std::runtime_error("tutorial-potion reached its frame bound without cinematic_Tuto_potionUse starting");
            if (!tutorial_finished_frame)
                throw std::runtime_error("tutorial-potion started cinematic_Tuto_potionUse but it never finished");
            if (!tutorial_lock_frame || !tutorial_unlock_frame)
                throw std::runtime_error("tutorial-potion never saw LockCharacter(All) held and released");
            if (tutorial_hp_drops || tutorial_moved > 1.0f)
                throw std::runtime_error("tutorial-potion Player lost HP or moved while LockCharacter(All) was held");
            if (model_renderer::tutorial_script_start_count("cinematic_Tuto_potionUse") != 1)
                throw std::runtime_error("tutorial-potion expected cinematic_Tuto_potionUse to start exactly once");
        }
        if (options.movement_defend_scenario && gameplay) {
            if (!scenario_damage_verified)
                throw std::runtime_error("move-defend reached its frame bound without verified Player damage");
            if (scenario_player_died && !scenario_revived)
                throw std::runtime_error("move-defend Player died but the death screen never revived the Player");
            std::cout << "Scenario move-defend complete\n";
        }
        if (options.hud_touch_scenario && gameplay) {
            if (!touch_driver.release_all(ui_error))
                throw std::runtime_error("touch-kill finger release failed: " + ui_error);
            std::cout << "Scenario touch-kill | walk attack verified " << touch_walk_attack_verified
                      << " | hits with both fingers held " << touch_hits << '\n';
            unsigned pans = 0, applied = 0, pinches = 0;
            original_ui.camera_gesture_counts(pans, applied, pinches);
            std::cout << "Scenario touch-kill | camera pans before the free finger " << touch_pans_before
                      << " | pans from the free finger: delivered " << pans - touch_pans_before
                      << " applied " << applied << " | pinch steps " << pinches << '\n';
            if (touch_pans_before)
                throw std::runtime_error("touch-kill: a HUD finger (stick or attack) panned the camera");
            if (pans == touch_pans_before)
                throw std::runtime_error("touch-kill: the free finger never panned the camera");
            if (!touch_walk_attack_verified)
                throw std::runtime_error("touch-kill never started an attack while the movement stick finger was held");
            if (!touch_hits)
                throw std::runtime_error("touch-kill landed no hit while the movement stick finger was held");
        }
        if (options.movement_kill_scenario && gameplay) {
            if (!scenario_kills)
                throw std::runtime_error("move-kill reached its frame bound without a verified enemy death");
            std::cout << "Scenario move-kill complete | verified kills " << scenario_kills << " | verified pickups " << scenario_pickups << '\n';
        } else if ((options.movement_attack_scenario || options.fixed_cultist_scenario) && gameplay) {
            if (!scenario_attack_sent) {
                if (options.fixed_cultist_scenario) {
                    model_renderer::GameplayPoseSnapshotV1 pose;
                    if (model_renderer::gameplay_pose_snapshot(pose, ui_error))
                        std::cout << "Fixed Crypt scenario ended before attack | nearest " << pose.enemy_index
                                  << " | target XY " << std::hypot(pose.player_position[0] - fixed_target_position[0],
                                                                      pose.player_position[1] - fixed_target_position[1])
                                  << " | graph nodes " << fixed_route_nodes << " | edges " << fixed_route_edges
                                  << " | status " << fixed_route_status << " | found " << fixed_route_found
                                  << " | waypoints " << fixed_route_waypoints << '\n';
                    throw std::runtime_error("fixed-cultist07 reached frame bound without candidate 4 becoming nearest");
                }
                throw std::runtime_error("move-attack scenario ended before its nearest-enemy attack");
            }
            const auto final_vitals = model_renderer::player_vitals();
            print_vitals_snapshot("Scenario final", final_vitals);
            std::cout << "Scenario HP delta " << (final_vitals[0] - scenario_initial_vitals[0]) / 256.0f
                      << " | MP delta " << (final_vitals[2] - scenario_initial_vitals[2]) / 256.0f << '\n';
            model_renderer::GameplayEnemyStateSnapshotV1 target_state;
            if (!model_renderer::gameplay_enemy_state_snapshot(
                    scenario_target_index, target_state, ui_error))
                throw std::runtime_error("Final shared exact-target HP snapshot failed: " + ui_error);
            std::cout << "Scenario target final | " << target_state.name << ' '
                      << target_state.hp << '/' << target_state.max_hp
                      << " | delta " << target_state.hp - scenario_target_initial_hp
                      << " | dead " << target_state.dead
                      << " | attack " << scenario_attack_result << '\n';
        }
        shutdown_guard.armed = false;
        model_renderer::deactivate();
        original_ui.deactivate();
        pump_front_audio();
        gameplay_audio.pump(assets.get());
        gameplay_audio.close();
        SDL_StopTextInput();
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(display, surface);
        eglDestroyContext(display, context);
        eglTerminate(display);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "DH2 desktop runtime failed: " << error.what() << '\n';
        SDL_Quit();
        return 1;
    }
}
