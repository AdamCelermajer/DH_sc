# HOST report (Preview 16): campaign script-host providers and generic trigger zones

Branch `p16/host` (worktree `DH_wt/p16host`), base `p15/integrate` (b05ae173). Not pushed. Option `--campaign-triggers` is OFF by default; production enable is a root decision.

## Status
- Investigation: done (CINE-survey A/B/D; code: `original_campaign_runtime`, `original_campaign_world_adapter`, `source_campaign_dispatch_v1`, `actor_definitions`, `canonical_trigger_zone_v22` box rule, `zone_collision_runtime_v83` inside test).
- Implementation: done for the milestone scope (providers, router, trigger zones, EXE wiring behind the option).
- Isolated tests: 5/5 campaign CTest entries pass (2 new suites, 3 existing campaign suites unchanged in result).
- Integrated runtime verification (quiet EXE batches, hidden desktop): done for the LizardMan_Intro trigger path, the abort/continue policy, default-off, and HUD hide (visual).
- Not verified / not implemented: see Remaining gaps.

## Evidence notes (AGENTS.md)
- Visual: reference timings in CINE-survey B5/B6 (HUD hidden at 224-226 s, SKIP visible in cutscene, no SKIP press seen). This session compared live frames only: baseline frame 25 shows HUD; trigger run frame 25 (cutscene active) shows HUD hidden. Images: `.local-inputs/claude-preview16/host-runs/cap-base/baseline25.png` and `cap-cut/cutscene25.png`.
- Logic: BeginScriptedCutScene = common 1 (70 BlockSave, 79 Flush, 32 MarkScripted All, 23 HideFlash HUD, 22 ShowFlash menu_skipcutscene, 24 Lock All, 39 StopActor All, 1 CutSceneMode). EndScriptedCutScene = common 3 (31 Idle All, 12 WaitDialog, 22/23 flashes, 32, 25 Unlock All, 2, 69 SaveGame). LizardMan_Intro = script 17 (common 1, 24 All, 8 1000 ms, 26 500, 30 Lizard1, 26 1500, 30 Lizard2, 26 2000, 25 All, 8 LocalPlayer, common 3, 78 CombatTuto(10,7)). Source: `reports/encounter-source-scripts.json`.
- Geometry (verified in EXE): obj_3of4 module is placed at offset (-6000,0,0). LizardMan zone is world-centred at (-2897.05,-3.4,256.7), half extent 100*scale = (100,555,100). Player start in Swamp is (1090.75,-212.2,258). The zone is WEST of the start. The builder uses the placement translation (world), so no module offset logic is duplicated.
- Skip semantics: recovered source only (sampled per command; Lock with skip returns early; Wait ignores skip). Not observed in reference.
- Uncertain: PutCharacterInIdle with wait is applied as an immediate _SetState(3) (approximation, logged once). StopActor is a logged stub (controller lock ends player motion). Script_CONSOLE (kind 3, used by Hide_Troll) has no owner; its original behaviour is not verified here, so it stays an explicit unsupported command.

## Design
- `features/campaign_host/campaign_host.{hpp,cpp}`
  - Providers filled only where empty (HUD and SKIP flash, cutscene mode, block save, dialogue stub, message flush stub, save-request stub, tutorial gate/consume/settings stubs, character selector "All", scripted flag, StopActor stub, idle gate, set idle). Controller locks wrap main's existing lambdas, so `globalControllerBlocked` and `characterControllerBlocked` remain the only flags.
  - Executor binding through `SourceCampaignDispatchV1` (reused). Cinematic, door and audio owners are pass-through, so their kinds reach the router. Admission: single player, always admitted. No RNG owner (array ExecScript fails explicitly).
  - Router logs every executed command (`[campaign] command script=.. index=.. kind=.. class=..`). Wait and ExecScript are runtime-internal and not logged.
  - Unsupported log: name and count; first occurrence printed at once; summary at exit (`print_summary`).
  - SKIP: `press_skip()` is an abstract press. Ignored unless SKIP is visible; the sampled skip value is applied to later commands; cleared at cutscene end and on abort.
  - Frame: the executor ticks on the app dt (own clock), then trigger contacts are fed.
  - Failure policy (changed during verification): an executor error ABORTS THE CUTSCENE, not the session. Flags are restored, running scripts are abandoned (`OriginalCampaignRuntime::abandon_running_scripts`), and the latched failure is cleared before contacts are fed. A failed contact disables only that zone. A first run without this fix disabled all zones after one spawn failure (seen in the logs and fixed; unit test `failure_keeps_zones`).
- `features/campaign_host/trigger_zones.{hpp,cpp}`: builds zones from `load_actor_definitions` output for ANY loaded level (`population.definitions()`). Trigger record: the campaign XML record when its file basename and name match, otherwise the declaration is registered (`OriginalCampaignRuntime::register_trigger`). Not fed (logged with reason once at build): non-Block shapes, rotated shapes (convention unverified), `activate_cond` (not evaluated), provider fields (`script_all_player*`, `effect_one_player`, `is_door_closed`), scripts or move-out scripts not in the loaded bank, ambiguous or duplicate keys. Edge and count are the runtime's (`trigger_contact`).
- `main.cpp` (11 hunks, `P16 HOST` anchors): option `--campaign-triggers` (requires `--campaign-commands`); host providers before `campaignWorld.bind`; sourceObjects load and camera admission also when triggers are on; executor bind and zone build after the campaign load; per-frame tick; HUD gate on both HUD batch loops; summary at exit.

## Changes (commit on p16/host)
- `port/windows-foundation/features/campaign_host/` (6 files: host, zones, two test files)
- `port/windows-foundation/original_campaign_runtime.{hpp,cpp}`: `register_trigger`, `abandon_running_scripts` (additive)
- `port/windows-foundation/main.cpp`: P16 HOST hunks only
- `port/windows-foundation/CMakeLists.txt`: one P16 HOST block (sources + 2 tests)

## Tests
- `campaign_host` (real Swamp campaign XML): Begin/End contract (HUD off, SKIP on, cutscene mode, save block, controller lock); spawn without lifecycle record aborts explicitly and restores all flags; session continues after abort (CombatTuto runs); SKIP press ignored when hidden, sampled when visible, cleared on abort; dialogue and tutorial stubs do not block CombatTuto; unsupported kind 45 is named and counted once.
- `campaign_trigger_zones`: helper geometry; Swamp declarations: LizardMan zone edge (enter starts once, stay does not repeat, triggercount 1 blocks re-entry), MerchantCamp unlimited zone starts per entry, activate_cond zones not fed; failure does not disable zones; second level (`data/scene/003_darkwood.mlx`, Android): 5 zones fed by registered declarations (fixture bank built from the level's own script names), 11 reported not fed with reasons; per-zone edge tests.

## Quiet EXE verification (hidden desktop, `tools/quiet_run.ps1`, `DH_AUDIO_SILENT`)
Build: `DH_wt/build-p16host/dh-foundation.exe` (from `p14_build.ps1 -Name p16host`). Base args: rc3 `swamp.args` with absolute assets path, `--campaign-commands original-campaign.xml --campaign-triggers --fixed-step 0.0166667`. Job folders: `.local-inputs/claude-preview16/host-runs/`.
- v1 `walk-west` (start -2750,-3.4,255; `--move-axis -1,0,0`, 300 frames): `trigger _prim_TriggerZone_LizManIntro -> script LizardMan_Intro started`; commands 1 (lock), 2 (camera), 4 (spawn) logged; `cutscene aborted: Unknown original lifecycle actor` (no lifecycle record for the lizard: the spawn owner's branch provides it); HUD, SKIP, cutscene mode, save block and controller locks restored; `script LizardMan_Intro finished`; no zone disabled.
- v2 `walk-from-kill` (start -2700,-3.4,255, inside KillTroll): `Hide_Troll` starts, kind 3 Script_CONSOLE unsupported, cutscene aborted and restored; the walk then fires LizardMan later (session continues) and aborts at spawn as in v1.
- v3 `control-east` (no zone on the east route): no trigger lines; 29 campaign lines are the zone list and the summary only.
- v4 `default-off` (no `--campaign-triggers`): 0 campaign lines.
- Visual: frame 25 baseline (HUD visible) vs frame 25 trigger run (HUD hidden), as above.

Unsupported or stubbed names seen in the runs (count shown in the summary at exit): `stub FlushMessages`, `scripted flag stored only`, `stub StopActor`, `stub dialogue Script_StartDialog`, `stub dialogue Script_WaitDialog`, `stub tutorial flag persistence`, `stub SaveGame`, `kind 3 Script_CONSOLE`.

## Package files required
- `.local-inputs/assets-extra/android/data/scene/003_darkwood.mlx` (copied from the Android device folder; non-destructive `cp -n`)
- `.local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/*.mgp` (29 files, same copy)
- Existing package: `windows-source-clock-v19-preview-15-rc3` (`assets/original-campaign.xml`, `swamp.args`). Not modified.

## Verifier script (exact)
1. `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16host -Test`
2. Jobs: `host-runs/jobs-verify.json` (v1..v4) run with `port/windows-foundation/tools/quiet_run.ps1 -JobsFile ... -Parallel 4`.
3. Expected: v1 contains `trigger _prim_TriggerZone_LizManIntro -> script LizardMan_Intro started` and `cutscene aborted: Unknown original lifecycle actor`; v2 contains `Hide_Troll` abort then a LizardMan start; v3 has no `trigger _prim` start; v4 has no `[campaign]` line.

## Remaining gaps (explicit)
1. Spawn: the lizards are not spawned. `SpawnCharacter` fails with `Unknown original lifecycle actor` until the spawn branch registers lifecycle records. Expected; the cutscene then aborts and restores.
2. SKIP is state only: no SKIP control is drawn and no press is wired to input. The API `press_skip()` exists.
3. Dialogue text and the tutorial prompt box are stubs (logged, not drawn). `features/tutorials` is not connected; tutorial persistence is session-only (save schema v4 pending).
4. SaveGame and BlockSave: BlockSave sets a flag that no save writer reads yet; SaveGame is a logged stub (nothing written).
5. Camera playback: kinds 5 (PlayCamera) and the clip paths are unsupported (explicit). Only the SetCameraTarget idle path runs.
6. Movement: a full walk to LizardMan through navigation was not established from the spawn; the test walks from grounded points just west of the zone (x -2750). Moves east of x≈1580 from the spawn hit walls.
7. Rotated trigger shapes (5 Swamp zones, 4 darkwood zones) and zones with activate_cond (5 Swamp zones, 3 darkwood zones) are not fed until the source rotation convention and the condition evaluation are verified. Four darkwood zones require is_door_closed (provider not bound).
8. Unverified against the original: PutCharacterInIdle wait (immediate approximation), StopActor (stub), Script_CONSOLE (no owner).
9. The per-frame player position trace is printed in trigger mode only (every 30 frames) as a diagnostic.
