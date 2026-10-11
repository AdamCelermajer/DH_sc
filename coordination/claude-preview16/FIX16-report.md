# FIX16 report: fixes for the rejected Preview 16 RC1 verification, packaged as RC2

Branch `p16/integrate` (worktree `DH_wt/p16int`). Build source commit for the shipped EXE: `e3649d59`. Nothing merged, nothing pushed, no other worktree touched. Package: `DH_sc/.local-inputs/windows-source-clock-v19-preview-16-rc2` (copy of RC1; RC1 not modified, verified by file count and mtime). Harness folders: `DH_sc/.local-inputs/claude-preview16/fix16/` (sweeps, quiet-run jobs) and `claude-preview16/verify-p16/recipes/` (verifier recipes, `RECIPES.md`).

## 1. Status of the RC1 verification findings

| Finding (verify-p16) | Status in RC2 | Evidence |
|---|---|---|
| Quest path: `Source camera target: Live Character camera anchor provider is required` exits 1 during the witch dialogue | FIXED | quest-log2 (exit 0): `Swamp_Witch_Activate` runs SetCameraTarget (frame 56), StartDialog, caption "The Bogwitch sealed my people here long ago.", WaitDialog; `Rendered frames=170; clean shutdown`. Frame 150 viewed. |
| Witch zone aborts on kind 16 (Script_EnterSafeZone) | FIXED | Cherry-pick `435d1e4e` (P17FIX) applied with one conflict (main.cpp: kept P16 `drawn=0`, added the safe-zone bind). Log: `safe zone entered frame=2`, `Safe zone music: enter track=SwampMerchantCampMusic`, run exit 0. |
| Missing `CS_wallbreak` asset | FIXED | Added `data/3d/animateddecors/cs_wallbreak_death_cutscene.bdae` from `assets-extra/android` (the device folder copy is identical in name and location). The scene object logs `model=data/3D/AnimatedDecors/CS_wallbreak_death_cutscene.bdae meshes=2`. |
| Other aborts in opening/witch/quest/ambush scripts | FIXED (stubs with one log line) | Sweep of all 55 level scripts (section 3). |
| Harness: opening lock blocks Space, pickup, map, level-up, F5/F9, continue | RECIPES PRODUCED | Section 4. Each item reached in the real EXE; frames viewed. |
| Rebuild, ctest, package rc2 | DONE | Section 5. |

## 2. Code changes (commits on p16/integrate)

- `91c0d9f5` P17FIX cherry-pick (`435d1e4e`), adapted.
- `f504ea26` camera anchor provider bound for every script camera target. Investigation: IDA `GameObject::GetCameraAnchorPosition` (0x003943b8) returns the object's own position unless an auxiliary anchor node is attached (CINE report; the aux node is not decoded). Before: `SourceWorldObjects::anchor` required a provider for Characters and none was bound. Now bound in main.cpp next to the camera providers.
- `721cc557` the anchor falls back to the authored placement when a Character has no live instance yet (the witch before its zone starts). Found by quest-log run `Actor position unavailable`.
- `d09a8ac2` host verb stubs: PlayLevelMusic (15), KillActor (44), RestartLevel (63), ShowTrophies (68), each logged once and non-blocking; routed through the host verb table.
- `02bc8264` PlaySound (13), MoveActor (40), OpenDoor (54), CloseDoor (55) logged once and skipped (no owner in this build). Show/HideActor and SetActorPosition on an actor with no live instance: logged no-op (only for the "unavailable" error; other errors still fail).
- `11702623` LookActor and PlayActorAnim on an actor with no live instance: logged no-op.
- `e3649d59` LockCharacter/UnlockCharacter on a named NPC without a session body: flag recorded, logged once (only the player has a controller).

No new placeholder art. Logged stubs are listed in section 3 and in `release-notes.json` ("stubs").

## 3. Unsupported commands in the Swamp scripts (item 4)

Method: 55 level-scope scripts from the RC2 `original-campaign.xml`, each started by name with `--campaign-start` at a position away from the start-room trigger; 400 frames each; quiet hidden silent runs. Plus the witch zone and quest-log jobs and an opening profile. Earlier batches found the aborts in order; the final sweep (`fix16/jobs/h-*`, 57 jobs, exe `e3649d59`) has exit 0 and clean shutdown in all 57 and no `cutscene aborted` and no `Unsupported original campaign command` line.

Unsupported kinds found and their handling:
- 13 Script_PlaySound: aborted MothIntro, LizCaptain_Intro, Swamp_Serpent_Intro. Now a logged stub. No scripted sound owner exists on the production path (the canonical source audio adapter is used only by a smoke test). Real sound is open.
- 40 Script_MoveActor (TrollReturn): logged stub; the actor stays in place. Open.
- 54 Script_OpenDoor (open_SwampKingDoor), 55 Script_CloseDoor (Swamp_Serpent_Intro): logged stubs; doors are P17.
- 15 PlayLevelMusic (ResumeLevelMusic), 44 KillActor (LizCaptain_Intro), 63 RestartLevel (PlayerDeathScript), 68 ShowTrophies (ShowUnlockedTrophies): logged stubs (not reached in the opening; the death restart is owned by the death flow, not by this script).
- Actor-availability aborts in standalone starts (not unsupported kinds): LookActor, SetActorPosition, PlayActorAnim, Show/HideActor, LockCharacter on an actor with no live instance. Now logged no-ops.
- Other stubs already logged in RC1 (unchanged): StopActor, SetCameraClip tuning, scripted flags without AI consumer, SaveGame (schema v4 writer not bound; nothing written), tutorial flags/settings save, StopSound, PutCharacterInIdle approximation.
- Missing optional FX (`PlayEffect set failed: Exact authored FX resource not found`) for Swamp King scenes: `cs_sk_intro_scene01_running_in`, `cs_sk_outro_scene01_swampkingdeath`, `cs_sk_outro_scene03_smokinhead` added from `assets-extra/android`; the Serpent scripts rerun with no PlayEffect failure (`fix16/jobs/i-*`).

The opening (new game, auto-tap 300 ms, 6000 frames; `fix16/jobs/opening`) reaches its end (EndScriptedCutScene at frame 3943) with stubs only.

## 4. Verifier recipes (item 5)

Folder `claude-preview16/verify-p16/recipes/`, documented in `RECIPES.md` (commands, expected markers, results). Base: production `swamp` config (campaign lines kept, `--fresh-player` loads the profile) or `swamp-nocamp` (campaign lines removed). Profile P0: a post-opening save (`Saved live checkpoint frame=4800`, after the opening's end at 3946) produced by `recipes/p0-profile`.

| Item | Recipe | Result (frame viewed) |
|---|---|---|
| Space opens a chest | r-space (swamp + P0, `--space-key-interval 20:200`) | PASS: `Context button frame=20`, `Container opened ... loot=227 frame=34`; f60 viewed (chest open, loot label) |
| Walk-over pickup, no key | r-pickup (`--interact-at ...@20 --move-segment 50:110:0,1`) | PASS: `World item pickup frame=50 ... Bow02 reason=walkover picked=1` |
| Quest Log with the Assigned row | r-quest (swamp-nocamp + P0, `--quest-page-frame 150`) | PASS: "Prison Break" in the Assigned list (selected), COMPLETED header; f235 viewed. Detail pane empty (open). |
| Map tab | r-map (swamp-nocamp + P0, `--map-page-frame 150`) | PASS: The Boglands, visited area, player marker, legend and zoom buttons; f235 viewed |
| Level-up | r-levelup3 (swamp-nocamp + levelup profile, `--space-key-interval 30:400`) | PASS for the presentation: `Level up presentation frame=143 level=2 ... drawn=0`; f151 viewed (placeholder column). NOT reached from normal XP: the profile is the XP 7000000 save from the packager's run (hand-set). The kill-test path gives no death reward. |
| F5 / F9 | r-f5f9 (`--save-frame 120 --load-frame 200`) | PASS: `Saved live checkpoint frame=120`, `Restored live checkpoint frame=200`, no rejection. `--save-frame/--load-frame` run the same branch as the F5/F9 keys. |
| Fresh-process continue | r-continue-a (startup + menu slot → Start, profile in folder) | PASS: `current=50`, zero `Swamp_Intro` commands, frame 600 viewed (HUD, play, no cutscene). Caveat: the saved position is not restored (startup position inside the start room, so the start-room dialogue runs). |

Facts found on the way:
- Without campaign triggers F5 is refused: `Campaign lifecycle/controller providers are not persisted` (the host sets `set_lifecycle_serialized_by_host` only with `--campaign-triggers`). Production has triggers, so this is by design; the recipes use the production path.
- `btn_MENU_CONTINUE` is not an enabled button on `menu_MainMenu` (`screen_interaction.cpp`: only `btn_MENU_SINGLE_PLAYER`, the slot arrows and `btnDelete` are enabled there). The verifier's `continue-fresh` used a button that cannot be pressed. The continue is the slot and Start.
- The start position `1090.75,-212.202,258` and the chest position `-2198.67,835.19,300` are inside `enterLocation_StartRoom` (zone x -8489..2965, y -5390..3386), so campaign runs at those positions show the start-room dialogue. Recipes that do not need the campaign use `swamp-nocamp`.
- The SKIP press (`--campaign-skip-frame 60`, `fix16/jobs/skip-open`) did not end the opening: the cutscene still blocked saving at frame 500. Not fixed; open.

## 5. Build, tests, package (item 6)

- Build: `p14_build.ps1 -Name p16int -Jobs 6` exit 0 on each change. Final EXE `DH_wt/build-p16int/dh-foundation.exe` = `e3649d59`, SHA256 `1283C3D36D38FE659443652020E698D9F841A28DF59B7B1904FB376C53E46912`.
- ctest (run directly with the llvm-mingw PATH; the PowerShell wrapper hid the output in the build script): 139 of 140 pass; only `session_skill_binding` fails (known junction issue). Log `claude-preview16/fix16/ctest-final.log` (final build).
- Package `windows-source-clock-v19-preview-16-rc2`: copy of RC1 (3310 files) with the new EXE, four added assets (`cs_wallbreak_death_cutscene.bdae`, three `cs_sk_*.bdae` interface clips), new README, release-notes.json, package-receipt.json, and manifest.json (3311 SHA256 entries, all files except the three documents, the RC1 rule). No saves packaged. RC1 unchanged. Receipt and manifest parse as JSON; the manifest EXE hash matches the file.
- Smoke in the package: the recipes and the sweep above ran from per-job junctions into the RC2 asset folders (the package itself is not written by the runs; saves and captures are in job folders).

## 6. Not verified, open (for the root)

- Audio: all runs silent; scripted PlaySound is a logged stub (the cutscene sound is not played).
- SKIP in the opening (touch path and the harness press) did not end the cutscene; the opening was played to its natural end with auto-tap.
- Level-up from normal XP: not reached in the harness (no XP source; the test used a hand-set XP save). LEVEL UP text, bloom and flash are placeholders.
- Quest Log detail pane empty; MAKE ACTIVE and initial selection as in RC1.
- The continue restores the profile at the startup position; `gameplay.save` is not loaded at startup.
- Doors (P17), moths (summon/despawn on the production path), rotated/activate-condition zones, second-level maps, and the Darkwood map are not verified.
- Play-swamp.cmd starts the opening (the start position is inside the start-room zone); a decision for the root.
- Not run: RC1's verification list items 1b (SKIP action) and 12 (15.2 A/B combat).
- Stopped by PID: one diagnostic recipe (`r-continue-b`, menu only) had no frame limit and was stopped by its own process ID; its result is not used.

## 7. Package files required

Added to RC2 from `DH_sc/.local-inputs/assets-extra/android` (same files as the Android device folder `files/data`):
- `data/3d/animateddecors/cs_wallbreak_death_cutscene.bdae`
- `data/3d/interface/cs_sk_intro_scene01_running_in.bdae`
- `data/3d/interface/cs_sk_outro_scene01_swampkingdeath.bdae`
- `data/3d/interface/cs_sk_outro_scene03_smokinhead.bdae`

## 8. Verifier script

Recipes: `claude-preview16/verify-p16/recipes/RECIPES.md`; generators `recipes/gen/mkrecipe.sh` (`swamp`, `swamp-nocamp`, `startup` bases; profile copy) and `recipes/gen/mkrecipes.sh` (quiet-run JSON). Batches: `recipes/batchE.json`, `batchF.json`, `batchG.json`; sweep `fix16/batchH.json`. Run with `port/windows-foundation/tools/quiet_run.ps1 -JobsFile <json> -Parallel N`. Feature markers are listed per recipe in `RECIPES.md`.
