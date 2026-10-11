# PACK16 report: Preview 16 RC1 package

Branch `p16/integrate` (worktree `DH_wt/p16int`), source commit `f4ea322d` (tree clean at build). Nothing merged, nothing pushed. The package lives outside git at `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/windows-source-clock-v19-preview-16-rc1` (666 MB, 3310 files). The 15.2 folder, the rc3 package and all other packages were not modified.

## 1. Build and tests

- `p14_build.ps1 -Name p16int -Jobs 6` built `DH_wt/build-p16int/dh-foundation.exe` (233 targets, exit 0).
- The `-Test` step of the script printed nothing useful (PowerShell wrapped ctest's stderr). ctest was rerun directly with the llvm-mingw bin on PATH: 139 of 140 pass; only `session_skill_binding` fails (the known junction issue, allowed). Log: `DH_wt/p16int-ctest.log`.
- EXE SHA256 `05B69C44E7139C0F4502187738CC618B4E39B109C394ED37DD6250286A1472CE`.

## 2. What was added and where it came from

Base: copy of `windows-source-clock-v19-preview-15-2` (non-destructive `copytree`), then `dh-foundation.exe` replaced by the new build. Every other file was copied only if the RC folder lacked it (`pack16-copy-log.json` lists each file and its source). Sources, in priority order: rc3 package (`windows-source-clock-v19-preview-15-rc3/assets`), `claude-preview16/levels-root`, `assets-extra/android` and `assets-extra/ios`, and the Android device folder `files/data`. The device folder was never needed.

| Group (from report) | Files | Source | Reports |
|---|---|---|---|
| Quest table `original-cache/data/pydata/v2quests_pyarray.bin` and `...names.bin` | 2 | rc3 | QUESTS, QUESTUI, MERGEFIX, OPENING |
| Camera scenes `data/3d/camera/**` incl. `animations/cs_swamp_intro/*` | 125 new | rc3 (112), levels-root (13) | CINE2, OPENING |
| Actor clips for the opening: `data/3d/characters/{dark_queen,faeries,lizardman,madruk,moth,npcs,prince,root_troll,swampking,troll,witch}/animations/*.bdae` | 949 new | rc3 (mostly), levels-root (1 each folder) | OPENING, OPENING2 |
| Cage clip `data/3d/animateddecors/cs_swamp_intro_cage.bdae` | 1 | rc3 | OPENING, OPENING4 |
| Cutscene interface clips `data/3d/interface/cs_swamp_intro_*.bdae` | 5 | rc3 | OPENING |
| Moth clips `moth_urn_spawn.bdae`, `moth_despawn_01.bdae` (data and original-cache paths) | 2 (+2 already present) | rc3 | PROFILES, DESPAWN, OPENING |
| Minimap camera `data/3d/camera/minimapcameras.bdae` | 0 (already in 15.2; the iOS assets-extra copy was not compared) | 15.2 | MAPFIX |
| Game object tables `data/game_objects_{pyarray,pyarraynames,dictionary_pyarray,dictionary_pyarraynames}.bin` | 4 | rc3 | CONTAINERS |
| Gameobject models `data/3d/gameobjects/*` (incl. go_chest_swamp, go_swamp_urn_breakable) | 85 | rc3 | CONTAINERS |
| Level scenes `data/scene/{003_darkwood,005_infectedvillage,025_icy_hub,036_underworld_hub}.mlx` and original-cache copies | 8 | rc3 | CONTAINERS, MAP, HOST |
| Level modules for those four levels (data and original-cache) | 296 | rc3 | CONTAINERS, LEVELS |
| FX files for PlayEffect sets 116-120 `data/3d/fx/*`, `original-cache/data/3d/fx/*` | 14 | levels-root | OPENING |

Already present in 15.2 and therefore not copied: HUD SWF and art (`dqhud_droid.swf`, `MenusGraphics_droid.tga`), `dqcharmenu_droid.swf`, `map_bottom.tga`, `001_swamp.mlx`, the Swamp module, quest text (`sidequests`, `gameplaymenus`, `global`, `menu`), `common_text_pycst.bin`, loot tables (`loot_table_*`, `loot_audiovisual_*`), `actor-profiles-v2.xml`, `original-campaign.xml`, `original-melee-bindings.xml`, `development-controller.xml`, fonts.

Not shipped (not needed by a P16 code path, or deferred):
- Door and fire models: `Door` declarations are unsupported (P17); no P16 code path loads a door or fire model.
- Darkwood unit-test copies from MAP (`map/assets-darkwood`): covered by the level modules above.
- Android `cs_swamp_intro` copies in assets-extra: the Windows build reads the rc3-equivalent clips above.

Removed from the RC copy only: `gameplay.save` and `character.save`. The 15.2 folder contains saves written at 03:38 (after it was packaged, by an earlier run); its receipt says none are packaged. Shipping them would start the game in a saved state.

## 3. Startup and launchers

- `startup.args` and `swamp.args` were replaced by the production configs `claude-preview16/opening2/production/*` (adds `--campaign-commands original-campaign.xml --campaign-triggers`). Both use relative paths (`assets`, `data/scene/001_swamp.mlx`, `original-campaign.xml` resolved against the assets root). Checked: no absolute paths.
- `--condition-active GameStartOnly` is absent, so a new game starts the opening through the quest states (OPENING2). `--condition-active RENE_FOLLOW` is kept: it is the Preview 15 Priest/follow condition, not the opening flag, and removing it would drop the Priest from the Swamp. The brief said "no command line condition flag"; this is a deliberate exception to flag for the root.
- `Play.cmd` and `Play-swamp.cmd` are unchanged (they use `%~dp0`). They were not double-clicked (that would write saves into the shipped folder). The equivalent EXE command lines were run from job folders.

## 4. Smoke runs (quiet_run, hidden desktop, silent)

Method: each job is a folder `.local-inputs/claude-preview16/pack16/jobs/<name>/` with the RC assets hard-linked (so the shipped files are never written; only the job's own saves and captures are new), the exe, and a generated `startup.args` (production config plus overrides). Jobs file via `pack16/jobs-*.json`, run through `port/windows-foundation/tools/quiet_run.ps1`. Frames were converted with ffmpeg and viewed.

| Job | Setup | Result | Evidence |
|---|---|---|---|
| boot3 | startup.args, touches at 4/10/16 s, 150 menu frames | PASS | Intro movie skipped by touch; `menu_MainMenu` with Start game / Options / Info and an Empty profile slot (`boot3/menu150.png`) |
| newgame2 | startup.args; Start game > Hero > class confirm; 3000 frames | PASS (partial) | Opening runs from a new game; 0 "cutscene aborted"; captions "The Boglands - Ancient Prison" (frame 2) and "...Hero!" (1441); cutscene frames 600 and 1200 viewed (`newgame2/caps/f600.png`, `f1200.png`); frames 1800/2400 captured, not viewed; end of opening (EndScriptedCutScene) not reached |
| swamp | startup.args, `--start-mode swamp`, 900 frames | PASS | `Rendered frames=900; clean shutdown` |
| chest | swamp.args, `--interact-at _prim_OpenableContainer_2@20` | PASS | interact accepted at distance 100 (frame 20); opened frame 34; loot 227 delivered; frame 60 viewed |
| spacechest | swamp.args without campaign lines, `--space-key-interval 20:3` | PASS | `Context button frame=20`, chest opened frame 34 (Space opens the chest) |
| pickup2 | as chest, plus `--move-segment 50:110:0,1` | PASS | `World item pickup frame=50 item=1 id=Bow02 reason=walkover picked=1` |
| levelup4 | swamp.args without campaign lines, fixed-step 0.016, Space 30:400, XP-high save | PASS (placeholder) | `Level up presentation frame=143 level=2 fx=135:level_up:played`; frame 151 viewed: white placeholder column, New Quest banner (`levelup4/caps/f151.png`); `LEVEL UP!` text `drawn=0` |
| map | swamp.args, `--map-page-frame 150` | PASS | Map page: zone title "The Boglands", visited area, player marker, Show legend / Reset zoom (`map/caps/final.png`) |
| quest4 | swamp.args, debug-accept row 50 at frame 60, `--campaign-skip-frame 100`, page at 200 | FAIL (gap) | Accept succeeded; Quest Log page opens with headers only, no row listed at frame 230 (`quest4/caps/f230.png`). Cause not found |
| ctest | build-p16int | 139/140 | session_skill_binding only |

Not run in this smoke (reports only): F5/F9 save-load for containers and zones, lizard ambush end to end, the full opening end, moth summon, despawn, level transitions, Darkwood map in game, audio.

Harness notes (process hygiene): two early jobs (boot, newgame) stalled on the title splash because no touch was scripted (OPENING4 had the same stall). They were abandoned, not re-run, and their orphaned processes ended on their own. A stalled boot2 process was stopped by its job-folder PID only; no process was stopped by name.

## 5. What is missing or open (for the root and P17)

Full list with sources is in `release-notes.json` ("open"). Most important:
- Quest Log: the accepted opening quest is not listed even though the accept succeeds (quest4). The earlier QUESTUI2 run listed rows in a different save state. Needs a look before the RC is accepted.
- Level-up: placeholder column only; `LEVEL UP!` text not drawn (no HUD status owner); bloom and body-height flash missing.
- Opening: flash of PlayEffect 116-120 not seen on screen; caption speaker names not drawn; placeholder hold times; the end of the opening was not reached in the smoke; cage shake/open timing not compared.
- Doors not collidable (P17). Trigger zones with rotated shapes or `activate_cond` are not fed.
- Moths: summon refused on the live container path (no profile there); despawn clip not on by default.
- Map: entrance, quest giver, objective and room-exit markers not produced; second-level map not verified in game.
- Launchers and audio not verified by a human or by audio.

## 6. Files

- Package: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/windows-source-clock-v19-preview-16-rc1/` (`README.txt`, `release-notes.json`, `package-receipt.json`, `manifest.json` with SHA256 of all 3307 other files)
- Copy log: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview16/pack16-copy-log.json`
- Smoke jobs and frames: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview16/pack16/jobs/`
- Generators: `C:/Users/adamc/Desktop/workspace/DH_wt/p16pkg-tmp/` (`build_rc1.py`, `make_job.py`, `jobs_pack16.py`, `write_notes.py`)
