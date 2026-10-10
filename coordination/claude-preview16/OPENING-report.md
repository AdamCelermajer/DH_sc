# OPENING report (Preview 16): actor clips, the Swamp opening and the LizardMan ambush through the production script host

Branch `p16/opening` (worktree `DH_wt/p16opening`), base `p16/integrate` (e6877ba6). Build `DH_wt/build-p16opening/dh-foundation.exe`. Not pushed.
Commits: ab3e6fa9 (lifecycle/spawn glue), 726dce79 (actor verbs and clip playback), 7168df40 (selectors, look targets, culling, caption name), c569eafe (FX), 82d6b7e4 (clip manifest test).

## Status (one paragraph)
Done and verified in the real EXE (quiet, hidden, silent): the whole authored `Swamp_Intro` (164 commands, 19 camera clips, 48 actor clips, 14 dialogue lines, 8 end commands) runs from the troll spawn (command 1) to `EndScriptedCutScene` with no abort, and the `LizardMan_Intro` ambush (trigger, two lizard spawns, unlock, `CombatTuto` with 6 caption lines) runs to completion. Both go through the production script host (`--campaign-triggers`), with no level-specific code. Not done (explicit gaps, section 5): scene-object animation (the cage `shake`/`open`), camera up/FOV from clips, equipment hand switching, the follower offsets of SetActorPosition, and the caption hold model, which is a placeholder that makes some captions early or late against the reference (section 4).

## 1. Investigation (evidence)

### 1a. Visual (reference, Part 1 v1.0.3, ffmpeg)
- Swamp intro, 2 s steps from 78 s: `ref/sheetA_78_108.png` (`C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview16/opening/ref/`). Troll and bars at 78-82 s, cage and prisoner at 84-88 s, Rene (priest, name plate "Rene") at 92-96 s, bright light bursts (PlayEffect) near the priest at 92-100 s, captions "...Crisr82?" 98-101 s, "Lord Crisr82!" 102-103 s, "Is he... already dead?" 104-106 s. SKIP visible throughout.
- Ambush, 1 s steps from 222 s: `ref/sheetB_222_238.png`. HUD visible 222-223 s, HUD hidden and SKIP visible at 224 s, lizards visible at 225-226 s, title "Combat Tutorial" at 227 s, "Red circles..." 228-231 s, "When wielding a melee weapon..." 232-235 s, "You will continue..." 236-237 s.
- Survey B3/B5 (`coordination/claude-preview15/CINE-survey.md`) gives the caption list up to 258 s; the timings used in section 4 come from it (+/-0.5 s to 2 s sampling).

### 1b. Logic (IDA `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
- `Script_SetActorPosition::Execute` (line 247541): actor is the object named @20, the waypoint @12. `SM_SetIdleState(sm,0)`, then the player's followers (PlayerManager players 1-3) are placed at +/-200 around the waypoint (only when the actor is the local player), then `GameObject::SetPosition(actor, waypoint position, 1)` and `ForceUpdatePosition`. Port: teleport through the native body (when bound) or the transform; population-only actors update their placed transform. Followers are NOT moved (gap G3).
- `Script_LookActor::Execute` (0x45ec50, line 247745): actor @20 and target @12 (`HighestThreatPlayer` maps to the player). `Cmd_LookAt` on the player actors and the actor. Port: the heading is set at once (no turn animation; gap G5).
- `Script_ShowActor::Execute` (0x45eb14, line 247711): `DisableZoning()` then GameObject vtable+64(true). `Script_HideActor::Execute` (line 247374): vtable+64(false). Port: population activation (`set_enabled`), which the draw and update gates honour; the zoning side effect is not modelled.
- `Script_PlayActorAnim::Execute` (0x45e890, line 247632): actor = name @24; writes clip @8 (and @12) into AnimTable rows and calls `SM_SetAnimState`. `Script_PlayActorAnim::IsBlocking` (line 244583): blocks only when the wait byte @28 is set AND the actor is still in that state. 45 of 48 commands have wait=0, so they are non-blocking; command 139 (the troll's last clip) has wait=1.
- `Script_PutCharacterInLimbus::Execute` (line 248040): actor @16, `SM_SetLimbusState(sm, @8)`. Port: the lifecycle's `put_limbus` (Limbus 0).
- `Script_PlayAnimByName::Execute` (line 247398): object @24 (for example `_anim_cage_001`), clip name @12, `GameObject`'s visual animator. Port: not bound (gap G1).
- `Script_PlayEffect::Execute` (line 248211): `VisualFXManager::PlayAnimFXSet(set @8, position = (@16,@20,@24), anchor = object @32)`. `Script_StopEffect` drops the set (`StopAnimFXSet`). Port: `sourceEffectsFactory->manager().play_set` / `drop_set_by_id_v117` (the same owner as the level-up set).
- `Script_SetCameraClip::Execute` (line 246643): camera tuning (values 10000/5000/900, scalar 12 = 10 or 900): calls the camera's vtable+304/+308 with the two scalars. Not decoded; logged stub (G6).
- `Script_PlayCamera` / `CameraLevel::PlayAnim`: as CINE2 (unchanged).
- Lifecycle: `Script_SpawnCharacter` (kind 30) on `_prim_ActorTroll` and the two lizards goes PreSpawn17 -> Spawn1 -> Idle3 through the existing lifecycle (SPAWN/LIFECYCLE2 owners); the timing below uses that path unchanged.

### 1c. Data
- `Swamp_Intro` (script 30): kinds 45 x48, 5 x19, 46 x15, 12/10 x14/14, 26 x7, 41 x6, 20 x5, 43 x4, 42 x4, 19 x4, 31 x3, 25 x3, 24 x3, 8 x2, 6 x2, 21 x2, 14 x2, 0 x2, 52, 51, 30, 29, 27 (decoded dump: `.local-inputs/claude-preview16/opening/swamp_intro.txt`).
- `_prim_ActorTroll` (obj_4of4): `ai_state=Limbus`, `auto_spawn=0`, `activate_cond=GameStartOnly`, profile `Swamp_ActorTroll` (authored, no melee entry in the XML: derived bindings used).
- Lizard declarations (obj_3of4): `ai_state=Limbus`, `auto_spawn=0`, no activation condition.

## 2. Implementation

### 2a. Lifecycle, spawn and population glue (general, no level names)
- `main.cpp` `parse`: `--campaign-triggers` implies `--retain-hidden-actors`.
- Population policy: under campaign triggers, a declaration with `auto_spawn=0` is deferred only when its source preset is `Limbus` or `PreSpawn`; other `auto_spawn=0` objects stay unknown (as without the host).
- Lifecycle admission: under campaign triggers every declaration with `auto_spawn=0` and preset `Limbus`/`PreSpawn` joins the lifecycle (its record is what `SpawnCharacter` needs). Template-inherited presets on auto-spawning monsters are not admitted (an earlier version admitted about 16 LizTemplate actors and broke their combat state; corrected).
- Missing shared melee/profile for such a declaration: explicit notice and skip (no throw), except for explicit `--spawn-declared` names.
- Derived combat policy and melee bindings for those declarations (`actor_profile_derivation_v1`) when the authored profile has no policy/melee.
- `combat_session.cpp`: the source resource sentinel (-1, no MP pool) projects to zero for every actor (previously an invalid state for scripted actors such as the troll).
- Population-only actors (not combat actors) are not alive for the source Idle gate (a no-op for `PutCharacterInIdle`).

### 2b. Actor clips (PlayActorAnim, kind 45)
- `features/campaign_host/actor_clip_manifest.{hpp,cpp}`: collects every PlayActorAnim clip (scalar 8, and 12 when set) from the loaded scripts, keyed by actor name. Clip names are `cs:<dictionary id>`.
- Before the population loads: each clip is added to its declared actor's profile state bank (population-only actors load it), to the profile's combat source bank (`CombatSessionProfile::sourceAnimationClips`, which the session uses when it builds the actor visual), or, for the local player, to the player's visual config (with `allow_missing_animation_targets` for the cutscene camera channels, the same policy the population uses).
- `CombatSession::play_actor_clip` / `actor_clip_duration_ms` (session actors: retained `seed_sequence` with no lifecycle callbacks; the completion does not change the source state). Population-only actors: `CharacterVisual::select` on the population visual (the frame loop advances it).
- Blocking: `CampaignHost::actor_verb` for kind 45 blocks only when the wait byte is set and the clip is still playing (IDA IsBlocking). The chained clip (scalar 12) starts when the first ends.

### 2c. Actor verbs (`features/campaign_host/campaign_host.{hpp,cpp}`, `original_campaign_world_adapter.{hpp,cpp}`)
- Kinds routed to `actor_verb`: 41 LookActor, 42/43 Show/Hide, 45 PlayActorAnim, 46 SetActorPosition, 29 PutCharacterInLimbus, 20 PlayEffect, 21 StopEffect; logged, non-blocking stubs for 6 SetCameraClip, 14 StopSound, 19 PlayAnimByName, 51/52 UnEquipHands/ReEquipHands.
- Named character selectors (kinds 24/25/31/32/39, for example `_prim_NPC_PriestGood`) resolve through the same name lookup; only `All` uses the registry.
- Name lookups: an actor name not in the loaded level is a logged no-op (once per name), as the source's null check does; `LocalPlayer`/`Player` are the local player.
- `main.cpp` binds `ActorVerbServices` (resolve, waypoint anchor, position, teleport, face, visibility, Limbus, clip play) and `bind_fx` (the effects manager). `$player` in caption text is replaced by the character name.
- Combat text: a label whose point is behind the current camera is culled (it was a fatal error when a scripted camera looked away from a fight).

### 2d. Tests
- `campaign_host_tests` (updated): unsupported kinds are now {3, 7, 40, 44} (CONSOLE and the kinds with no owner); new `actor_clip_manifest_collects_script_clips` (48 PlayActorAnim clips and chained clips collected from the real Swamp script; `cs:413` naming).
- Existing contracts unchanged (lizard intro, SKIP, captions, DoTutorial, camera clip).

## 3. Isolated and integrated verification

### 3a. Isolated
- `p14_build.ps1 -Name p16opening -Test`: ctest 128 of 129 pass. Only `session_skill_binding` fails (allowed, the junction). `campaign_host`, `campaign_trigger_zones`, `original_camera_clip`, `cinematic_runner`, `spawn_character_v1` pass.
- Note: `container_open_script_v1`, `winmm_pump_priority_v1` and `schema_v4` need the toolchain on PATH when run by hand (they passed under the build script's ctest).

### 3b. Integrated (quiet hidden runs, `port/windows-foundation/tools/quiet_run.ps1`, silent)
| Job (folder under `.local-inputs/claude-preview16/opening/`) | Shows | Result |
|---|---|---|
| `run26` (9000 frames, `--campaign-start Swamp_Intro --campaign-triggers --condition-active GameStartOnly`) | full opening | exit 0, `Rendered frames=9000; clean shutdown`, no `Foundation error`, no `cutscene aborted`, 19 PlayCamera (359-376, 55), 48 PlayActorAnim, 15 captions, 8 end commands (`EndScriptedCutScene` at frame 5715) |
| `lizard1` (1500 frames, walk west from -2750 with `--move-axis -1,0,0`) | ambush | trigger frame 11, spawn 1 frame 43, spawn 2 frame 137, unlock 262, CombatTuto captions 265-1343, exit 0 |
| `dflt` (300 frames, no campaign flags) | default path | exit 0, no campaign lines |
| `f120`..`f3900`, `e1100`, `e1300`, `e1500`, `lz60`, `lz150`, `lz300` | frames viewed | see 3c |

### 3c. Frames viewed (LOOK)
- `f600`: cage with prisoner, the player with the sword, SKIP placeholder, camera on the cage (scene 01 clip).
- `f1800`: caption "Is he... already dead?" over the cage (reference 104-106 s).
- `f2400`: caption "Oh! It's a miracle! ..." with Rene at the cage.
- `f3600`: priest in the robe, caption "Now get out of here..." (reference 138 s).
- `lz150`: a lizard spawned on the floor, HUD hidden, SKIP visible (reference 224-226 s: lizards come over the wall; our spawn point is in the room, gap T3).
- `e1500`: caption "...YU!" (`$player` replaced with the save's name; the reference shows the user's name "Crisr82").
- Effects: PlayEffect sets 116-120 now play (no FX error after the FX resources were staged); the flash was not in the captured frames (the camera faced elsewhere). Visual confirmation of the flash is an open item.

## 4. Observed (reference) vs implemented timing

Our times are seconds of script time (frame / 60) from the start of `Swamp_Intro`; reference times are seconds of the video. Best fit for the start: t0 = 76 s (the "Is he... already dead?" line, ours 28.6 s, reference 104 s, gives 75.5; "You fool!" gives 76.7). Uncertainty +/-2 s.

| Caption (line) | Ours (s) | Ours + 76 | Reference (s) | Difference (ours - ref) |
|---|---|---|---|---|
| "...$player!" (2031617) | 24.0 | 100.0 | 98-101 | about 0 to +2 |
| "Lord $player!" (2031618) | 26.3 | 102.3 | 102-103 | +0.3 |
| "Is he... already dead?" (2031619) | 28.6 | 104.6 | 104-106 | +0.6 |
| "He's dead alright..." (2031620) | 31.2 | 107.2 | 108-112 | -0.8 |
| "Oh! It's a miracle!..." (2031621) | 38.2 | 114.2 | 116-124 | -1.8 |
| "W... what happened?" (2031622) | 42.3 | 118.3 | 122-124 | -3.7 |
| "Friends, $player..." (2031623) | 46.0 | 122.0 | 126-132 | -4.0 |
| "Sorry to disappoint..." (2031624) | 53.5 | 129.5 | 134-136 | -4.5 |
| "Now get out of here..." (2031625) | 58.6 | 134.6 | 138 | -3.4 |
| "You fool! ..." (2031626) | 71.4 | 147.4 | 148-150 | -0.6 |
| "A noise like that..." (2031627) | 75.1 | 151.1 | 152-154 | -0.9 |
| "Then we haven't much time..." (2031628) | 78.9 | 154.9 | 156-160 | -1.1 |
| "Alright, you win..." (2031629) | 83.5 | 159.5 | 162-164 | -2.5 |
| "Now let's get going..." (2031630) | 87.2 | 163.2 | 166-168 | -2.8 |

Ambush (`lizard1`), trigger inferred at 223.8 s (the reference shows HUD hidden at 223.5-224; survey B5):

| Event | Ours (script s from trigger) | Ours (s, video) | Reference (s) | Difference |
|---|---|---|---|---|
| Lizard 1 spawn (command 4) | 0.53 | 224.3 | visible 224.5 | about 0 |
| Lizard 2 spawn (command 6) | 2.07 | 225.9 | visible 226 | about 0 |
| Unlock + DoTutorial (commands 8-11) | 4.2 | 228.0 | "Combat Tutorial" title 227 | +1.0 |
| "Red circles..." | 6.7 | 230.5 | 228-231 | +2.5 |
| "When wielding a melee weapon..." | 10.4 | 234.2 | 232-235 | +2.2 |
| "You will continue..." | 14.4 | 238.1 | 236-237 | +2.1 |
| "When wielding a ranged weapon..." | 18.6 | 242.3 | 240 | +2.3 |
| "To stand still..." | 22.2 | 246.0 | 244 | +2.0 |

Observed differences that matter:
- T1 (caption hold, placeholder P4): our hold is 2000 ms + 25 ms per byte, capped at 6000 ms. Title cards are held too long ("Combat Tutorial": ours 2.4 s, reference about 1 s), which shifts the CombatTuto captions by about 2 s. Longer lines in the middle of the intro ("Oh! It's a miracle", reference 8 s, ours 4.1 s) are short. The hold needs the original dialogue timing (audio length or Flash advance), not decoded here.
- T2 (intro start): the reference starts the cutscene at about 76-80 s after the loading screen; ours starts at script time 0 with `--campaign-start` (the starter of `Swamp_Intro` is not decoded; survey G4).
- T3 (spawn position): the reference lizards come over the wall top; ours spawn on the floor (Spawn clip from the spawn point; the source spawn point placement is not checked frame by frame).
- T4 (camera cuts): camera clip durations are the authored ones (4000 ms, 6000 ms, ...); the reference cut points were not measured frame by frame (the survey gives only 2 s samples).

## 5. Gaps (explicit)
- G1 Scene objects: `Script_PlayAnimByName` (cage `idle`, `shake`, `open`, `idle_opened` on `_anim_cage_001`; commands 14, 100, 106, 112) is a logged stub. The cage is in the module `.mvp` (not instantiated as a scene object). The staged `cs_swamp_intro_cage.bdae` is not used. Not drawn (see Placeholders P6).
- G2 PlayActorAnim semantics: the chained clip plays after clip 8 (the source uses AnimTable rows 8+2 and 8+8); after the clip the actor holds its last frame (the state machine return is not modelled). Blocking uses the authored clip range.
- G3 SetActorPosition followers (the player's party at +/-200) are not moved.
- G4 LookActor turns at once (no turn animation); the heading convention (atan2 of the target) is not verified visually on the actors.
- G5 Show/Hide of the local player is not applied (logged). Show/Hide of other actors uses population activation; the zoning side effect is not modelled.
- G6 SetCameraClip (10000/5000/900; camera transition tuning) is a logged stub.
- G7 UnEquipHands/ReEquipHands are logged stubs (weapons stay in hands). StopSound is a logged stub (sound stop not bound).
- G8 PlayEffect sets 116-120 play through the source effects manager; the flash is not visually confirmed in the captured frames.
- G9 Caption hold (T1) and the name plate of named speakers (not in the traced chain).
- G10 `GameStartOnly` is a launch condition (`--condition-active GameStartOnly`) in these runs. The production startup configuration (`startup.args`, `swamp.args`) does not set it, so a new game in production would not spawn the troll/prisoner declarations. Root decision needed: which game-start state sets this condition. The starter of `Swamp_Intro` in production is still not identified (survey G4).
- G11 PlayCamera up and FOV stay with the follow camera (CINE2 G3). SKIP semantics unverified (CINE2 G5).
- G12 `activate_cond` is not evaluated for 5 zones (not fed); unchanged.
- G13 SaveGame and tutorial persistence remain stubs (save schema v4 writer not bound).
- G14 Campaign triggers also enable deferred declarations and derived policies for script-spawnable declarations only; other maps may have declarations that need the same treatment (generality check on the Android darkwood level is not part of this task).

## Placeholders (ART/HUD rule)
| # | Placeholder | Where | What it stands for | Reference to replicate from |
|---|---|---|---|---|
| P1 | SKIP: red 20x20 block + "SKIP" label (existing, CINE2 P1) | `cinematic_runner.cpp` | `menu_skipcutscene` (dqhud_droid.swf sprite 737/736, bitmap-filled) | SKIP visible 78-172 s, 180-204, 224-260 s (sheets A/B) |
| P2 | Caption band: translucent full-width box (existing, CINE2 P2) | `cinematic_runner.cpp` | `tutorials_dialog` (dialogBox 730, text 728); no name plate | captions above (section 4), sheets A/B |
| P3 | Caption typeface: font id 5 (existing) | `main.cpp` | EditText font 7 (htmlText face) | same captions |
| P4 | Caption hold: 2000 ms + 25 ms/byte, cap 6000 ms (existing) | `cinematic_runner.hpp` | the Flash dialog advance (not decoded) | section 4 table (T1) |
| P5 | `[StrID n unresolved]` marker (existing) | `campaign_host.cpp` | error marker, not art | none |
| P6 | Cage animation (cage idle/shake/open, G1): NOT drawn (stub, no placeholder art) | `campaign_host.cpp` (kind 19) | the cage on `_anim_cage_001` | sheet A, 84-96 s (cage and prisoner); the open/shake times not measured |
| P7 | Flash/glow FX at the priest (PlayEffect 116-120) is played by the source FX owner; no placeholder art was added | `main.cpp` `bind_fx` | `VisualFXManager::PlayAnimFXSet` sets 116-120 | sheet A, 92-104 s (light bursts); visual check pending (G8) |

## Package files required
These files were copied (non-destructively, `cp -n`) into the rc3 input package `.local-inputs/windows-source-clock-v19-preview-15-rc3/assets` from the Android staging `.local-inputs/assets-extra/android/...` (the originals are unchanged). The rc3 package must ship them for the intro to run:
- `original-cache/data/pydata/v2quests_pyarray.bin` and `..._pyarraynames.bin` (from `claude-preview16/quests-run/assets`).
- `original-cache/data/3d/characters/moth/animations/moth_urn_spawn.bdae` (Swamp_Moth_Minions derived profile).
- `data/3d/characters/<dir>/animations/*.bdae` for dark_queen, faeries, lizardman, madruk, moth, npcs, prince, root_troll, swampking, troll, witch (all character animation folders of the Android staging; the cutscene `cs_*` clips for the Swamp intro actors, and the npc/prince/troll clips they need).
- `data/3d/camera/**/*.bdae` (120 files: `animations/cs_swamp_intro/*` scenes 01-18 and the other camera animation sets).
- `data/3d/animateddecors/cs_swamp_intro_cage.bdae` (cage clip, not yet used, G1).
- `data/3d/interface/cs_swamp_intro_0{1,2}_bloodsplat_prisoner.bdae`, `cs_swamp_intro_09_rezeffect.bdae`, and the other `cs_swamp_intro_*.bdae` (5 files; the effect sets 116-120 need them).
- Also copied earlier to `original-cache/data/3d/characters/*/animations` (401 cs_ clips; harmless duplicates of the same files).
- Scripts and data unchanged: `original-campaign.xml`, `actor-profiles-v2.xml`, `original-melee-bindings.xml` (the Swamp_ActorTroll profile has no melee entry; derived bindings are used).

## Verifier script (exact)
1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16opening -Test` (only `session_skill_binding` may fail; run the ctest with the toolchain on PATH if `container_open_script_v1`, `winmm_pump_priority_v1` or `schema_v4` show exit code 0xc0000135).
2. Full opening (quiet): `cd .local-inputs/claude-preview16/opening && ./run.sh run26 9000 --condition-active GameStartOnly` (expect `Rendered frames=9000; clean shutdown`, no `cutscene aborted`, `EndScriptedCutScene` 8 command lines, 48 PlayActorAnim, 19 PlayCamera, 15 captions, no `Foundation error`).
3. Ambush (quiet): `./run.sh lizard1 1500 --base=base-lizard.args --position -2750,-3.4,255 --move-axis -1,0,0 --move-frames 300 --move-run` (expect trigger at frame 11, spawn lines at frames 43 and 137, CombatTuto captions 265-1343, no abort).
4. Default path (quiet): `jobs-dflt.json` (no campaign lines, `Rendered frames=300`).
5. Frames: `py -3 tools/ppm2png.py <job>/frame.ppm` and view (PPM header parsing is exact in `tools/ppm2png.py`).
6. Reference sheets: `ffmpeg -ss 78 -i "<video>" -t 30 -vf "fps=1/2,scale=320:-1,tile=5x3" -frames:v 1 ref/sheetA_78_108.png` (no drawtext: the local ffmpeg build segfaults with it).
