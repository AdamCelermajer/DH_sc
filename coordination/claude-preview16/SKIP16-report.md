# SKIP16 report: SKIP ends scripted cutscenes; Preview 16 RC3

Branch `p16/integrate` (worktree `DH_wt/p16int`). Code commit `0838af0d` (SKIP16). Build `DH_wt/build-p16int/dh-foundation.exe`, SHA256 `3ACF50D2A249962156583C3F13CE010443F6AB65A94A7E0DBE423D71CF4D3D1D` (17928192 bytes). Nothing pushed, nothing merged, no other worktree touched. RC1 and RC2 are not changed. Package `DH_sc/.local-inputs/windows-source-clock-v19-preview-16-rc3` (copy of RC2, new EXE, RC3 documents).

## 1. Investigation (IDA, `libDungeonHunter2.so/pseudocode-all.c`)

The reject (verify-p16b) said SKIP advanced commands but the cutscene stayed on screen. Root cause in the port: the host sampled skip per command, but blocking was still honoured (WaitDialog, PlayCamera clip, Wait, ExecScript), so a skipped script still waited.

Decoded original behaviour:
- Button: the HUD SWF sprite `btn_MENU_SKIP` (in `dqhud_droid.swf`, string table) calls the native `NativeSkipScript` (registered at line 217873), which calls `ScriptManager::SkipScript(s_inst, -2, 0)` (line 225305). No confirmation string exists in the SKIP chain (the `MenuConfirm`/`confirm_msg` strings belong to other menus). The button's exact bytecode `onRelease` was not decoded; the evidence is the string table and the native registration.
- `ScriptManager::SkipScript` (0x4604c0, line 248556): flushes the DialogMsg queue (`MenuMessageManager<DialogMsg>::FlushEnqueuedMessages`), sends a network message only when online, and sets the global skip target (`*this = a2` when it was -1). It is global: every running script is affected.
- `ScriptManager::ExecuteScript` (line 246479): for each command it calls `Execute(skip, idx)` first. It calls `IsBlocking` only when not skipping, except for ExecScript (kind 0), which waits for its child. So while skipping, nothing waits (Wait, WaitDialog, WaitCamera, PlayCamera, PlayActorAnim, DoTutorial's child). Commands run in order within the same frame.
- `ExecuteAllScripts` (line 246555) clears the skip state (`StopSkipping`) at the end of the frame when no script is blocked. So a skip is a one-frame fast-forward.
- Per-command gates (`if (!a2)` / `if (a2)` in the Execute bodies):
  - StartDialog (248929) and StartDialogID (248971): nothing is enqueued while skipping.
  - PlayCamera (248410): plays the level set's idle clip instead of the cutscene clip.
  - SetCameraTarget (248361): target set with zero blend.
  - PlayEffect (248211): no FX.
  - PlaySound (248330): only the music flag still plays (`*(v3+12)`).
  - PlayAnimByName (247398), PlayAnimById (247439), PlayActorAnim (247632): no animation.
  - LockCharacter (247303): no controller lock (the adapter gates kind 24 the same way).
  - Ungated (run normally, so they are the state changes): SetActorPosition, MoveActor (its gate is only for the physical object), Show/HideActor, PutCharacterInIdle, ShowFlash/HideFlash, UnlockCharacter, ExitCutSceneMode, EnterCutSceneMode, SetLevelState, SetWorldMapLocationState, Spawn/PutInLimbus, AI state verbs, FlushMessages, StopEffect, StopSound, PlayLevelMusic, SaveGame, ExecScript (the child runs in the same pass).
- Confirmation: none. Presentation of the End contract: the authored `EndScriptedCutScene` (PutCharacterInIdle, ShowFlash/HideFlash, UnlockCharacter, ExitCutSceneMode, SaveGame) runs as ordinary commands.

## 2. Implementation

- `original_campaign_runtime.{hpp,cpp}`: `set_skipping(bool)` / `skipping()` / `any_running()`. While skipping no blocking query is made, except ExecScript (keeps `running(child)`); Wait does not wait.
- `features/campaign_host/campaign_host.{hpp,cpp}`:
  - `press_skip()` (visible only) is a one-shot request: it drops the queued captions at once (IDA SkipScript flush) and the fast-forward runs at the next `frame()` before the normal clock.
  - `fast_forward()`: clears the camera clip and the actor clip timers (the source replaces the clip; no clip starts), sets the runtime skipping flag, and ticks until no script runs (pass limit 1000, explicit error otherwise). Every command samples `skip=1` through the dispatcher route (`skip_active()`).
  - Gates in the host, mirroring the IDA Execute bodies: StartDialog (no caption), PlayCamera (no clip; follow kept), PlayActorAnim (none), PlayAnimByName (none), PlayEffect (none). LockCharacter is gated in the existing adapter (kind 24).
  - `restore_presentation()`: the host side of the End contract (HUD back, global and character locks released, cutscene mode left, captions, camera clip, actor clips, save block). It runs only when a skipped run leaves cutscene mode entered with nothing running, and it logs `SKIP applied the End contract`.
  - `host_state()` (snapshot of host-owned state for the played-vs-skipped test).
  - Abort path clears the skip state and the runtime flag.
- `features/cinematic_runner/cinematic_runner.{hpp,cpp}`: `skip_art_layout()` = the min/max of every triangle of `hud_panels::skip_batches_v1()` (x 3.35..182.35, y 0..54.45). Was a hand-set rectangle (3.4..196.4 x 0..54.5). `skip_hit` uses the same `viewport_for` mapping as the draw in main.cpp (`(x + offset) * scale`).
- `main.cpp`:
  - Enter (`VK_RETURN`) edge: press SKIP when the control is up, otherwise a caption tap. Enter is the port's existing press/tap key (`platform_key_codes.hpp`, Preview 15 boot). Space is not used: it is the context and attack key.
  - Verification inputs: `--campaign-click FRAME:X:Y` (authored 480x320 coordinates, mapped like the draw) becomes a pointer down on FRAME and an up on FRAME+1 through the same cursor branch as a real release. `--campaign-enter-frame N`.

Commit message and report: see git log. The earlier `--campaign-skip-frame N` is unchanged (it calls `press_skip()`).

## 3. Isolated tests (ctest, llvm-mingw, all pass except `session_skill_binding`)

- `cinematic_runner`: SKIP hit = the exported art bounds; the centre hits and two authored px outside misses at 480x320, 960x640, 1280x720, 1920x1080, 800x600, 1000x320, 320x480, 1280x1024.
- `campaign_host`:
  - `skip_press_contract`: a press is pending until the next frame; a failing spawn during a skip aborts explicitly and restores the HUD and controls.
  - `skipped_cutscene_matches_played`: Movement_Tuto (DoTutorial to Movement_Tuto2) and CombatTuto, played and skipped at three points each. Final host state (HUD, SKIP, cutscene mode, caption cinematic, save block, controller and character locks, camera clip, actor clip timers, scripted and tutorial flags, safe zone, aborts), the trigger activation counts, and no running script are all equal. A skipped run shows no more captions than the played one.
  - `skip_ends_captions_in_one_frame`: CombatTuto with a caption up; one press ends the script and the caption in one frame; HUD back; unlocked.
- Full ctest: 139 of 140 pass; `session_skill_binding` fails (known junction issue).

## 4. Integrated runtime verification (quiet hidden runs; logs and frames in `DH_sc/.local-inputs/claude-preview16/skip16/`)

Base: the preview production opening args (`skip16/base.args` = the opening2 production args, RC2 assets, `--campaign-commands original-campaign.xml --campaign-triggers --caption-auto-tap-ms 300`).

| Run | What | Result |
|---|---|---|
| playedA (4700 frames) | opening played to its end | EndScriptedCutScene at 3947; Movement tutorial ends 4632; 19 captions |
| clickA300 | click at SKIP art centre, frame 300 | `SKIP pressed`, `SKIP fast-forward passes=2 frame=302`, Swamp_Intro EndScriptedCutScene skip=1 at 302, no abort |
| clickA1000 / clickA2000 / clickA3000 | same, frames 1000, 2000, 3000 | same markers (passes=2, skip=1 End in the press frame) |
| enterA1500 | Enter key at frame 1500 | `SKIP pressed`, fast-forward in the press frame |
| missA600 | click at authored (300,200) | no press (no SKIP line); cutscene still running at 900 |
| clickA300..3000, enterA1500 vs playedA | state lines at 4700 (quest banners, lifecycle, quest runtime, every Actor final line) | 0 differing lines each |
| (frame 4700, played vs clickA300 viewed) | HUD, attack control, player position | HUD back; same position; the only visible difference is the priest's idle pose (below) |
| cap_w960/w1920/w800 + clk_w960/w1920/w800 | click at the art centre at three window sizes; captures at frame 299 | `SKIP pressed` in all three; drawn SKIP viewed at 800x600 at the click position |
| lizPlay / lizSkip (2600 frames) | LizardMan_Intro started by the harness; click at frame 200 in lizSkip | lizSkip: LizardMan_Intro and CombatTuto skipped in the press frame (no abort); lizPlay runs CombatTuto to ~1013 |
| chestPlay / chestSkip (700 frames) | chest_tuto started by the harness; click at 150 in chestSkip | chestSkip: fast-forward of chest_tuto and the running opening in one pass; End contract fallback applied (logged), no abort |
| ab-old vs ab-new (2400 and 6000 frames) | Preview 15.2 EXE and the new EXE, identical args (`swamp.args` of 15.2 + frames + space-key interval) | both exit 0, clean shutdown; XP 82 in both (same save); see section 5 |

Known differences in the results (not hidden):
- Priest pose: in the played opening the priest ends his cutscene choreography (PlayActorAnim, scenes 05 to 17) in a pose that is absent at the spot where the skipped run shows him idle. His position (794.067,-618.903,258) and HP match in every run (the Actor final lines are equal). A skipped PlayActorAnim plays no clip (IDA gate), so the end pose is not produced. Applying the end pose needs a visual seek on the actor visual that this build does not have.
- Ambush (lizPlay vs lizSkip): the troll's final position (played 0,0,0 target none; skipped 2546.58,-2723.95,258 target 0) and the priest's and faery's final positions differ. The played run reads 0,0,0 for several actors; the cause is not isolated. The ambush's state is therefore NOT claimed as identical.
- Chest run: the skip fast-forwards the running opening as well (as the source does); a chest run cannot be compared on its own in the harness, so the tutorial equality rests on the host test (section 3).
- The start-room script (`enterLocation_StartRoom`) enters cutscene mode and does not exit it within the played harness run (pre-existing; FIX16 noted the start-room dialogue). A skip ends it, and the End fallback then restores the HUD.

## 5. Regression A/B vs Preview 15.2 (identical args)

- Args: the 15.2 package `swamp.args` (no campaign flags; the 15.2 EXE accepts it), plus `--frames 2400 --space-key-interval 120:1500` (and a 6000-frame pair with `120:5000`). The same args file in both job folders; each job folder has junctions to its own package assets.
- Results: both exit 0, `Rendered frames=N; clean shutdown`, no `aborted` line, no Foundation error.
- Combat: neither run logs a combat hit, kill or drop; the XP line is 82 in both (from the same save). So the drop and XP comparison is empty, not a pass. The RC2 verifier also found no combat hits at this position. A combat regression needs an approach or a position that reaches the lizardmen.
- The new EXE logs two extra lines (`Quest banner ... xp=... gold=... (bind)` and `Container loot bound`) that the 15.2 EXE does not, from the post-15.2 quest runtime; they are not SKIP effects.

## 6. Gaps (explicit)

1. Cutscene actor poses after a skip (above): no end pose for skipped PlayActorAnim.
2. Ambush final state differs (above); cause not isolated.
3. Camera: a skipped PlayCamera keeps the follow camera (the source plays the level idle clip, which is not decoded); SetCameraTarget blend is not modelled.
4. Audio: scripted sound is a logged stub; PlaySound music is not played (no owner).
5. Combat A/B has no hits (above).
6. SKIP at 4:3 (800x600) is partly off-window (the X icon is cut) because the PC HUD mapping crops the authored width at that aspect; the visible part is clickable.
7. Keyboard: Enter only (no gamepad). Enter also taps captions.
8. The button's bytecode `onRelease` was not decoded (string table and native registration are the evidence).
9. Tutorial flags: session-only (save schema pending, unchanged).

## 7. Placeholders (ART/HUD rule)

No new placeholder art. The SKIP control and caption are the original `dqhud_droid` batches (HUDART). The SKIP hit area is the exported art bounds.

## 8. Package files required

- RC3 folder: copy of RC2 (3314 files) with `dh-foundation.exe` replaced and `README.txt`, `package-receipt.json`, `release-notes.json`, `manifest.json` updated. Manifest: 3311 entries, all verified against the files (0 mismatches). RC1 and RC2 are not modified.
- Harness folder: `DH_sc/.local-inputs/claude-preview16/skip16/` (`base.args`, `mk16.py`, `merge16.py`, `pack16.py`, job folders, `ab/`).

## 9. Verifier script

1. Build and tests: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16int -Jobs 6` (the wrapper hides ctest output; run ctest directly with the Android SDK `cmake/3.22.1/bin/ctest.exe` and llvm-mingw on PATH: 139 of 140, only `session_skill_binding` fails).
2. Opening skip (quiet): `cd DH_sc/.local-inputs/claude-preview16/skip16 && py -3 mk16.py clickA300 4700 --campaign-click 300:92.85:27.2`, then `port/windows-foundation/tools/quiet_run.ps1 -JobsFile jobs-clickA300.json`. Expect `SKIP pressed`, `SKIP fast-forward passes=2 frame=302`, `EndScriptedCutScene ... skip=1 frame=302`, and `Rendered frames=4700; clean shutdown`.
3. Host test: `campaign_host_tests.exe <rc2 assets>` (`skipped_cutscene_matches_played`).
4. Hit area: `cinematic_runner_tests.exe` (`skip_hit_art_at_window_sizes`).
