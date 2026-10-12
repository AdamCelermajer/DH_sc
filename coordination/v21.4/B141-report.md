# B141 - Bogwomp Chief intro scene aborts and leaves the camera locked on the scene

Owner: B141 implementation session (worktree `.claude/worktrees/agent-ab3d6eaf71a8e2234`). Base `windows-foundation` 45a0c602 (accepted v21.3, shipped EXE SHA256 `276C83146351F41E879F296143F8F59F74A0D3552F224D9015AEF87C3037779A`). Reviewer and integration are owned by root. `main.cpp` is shared with B137: the main.cpp change is small and localized (one helper lambda, two restore call sites, one host service) so it can be merged by hand.

Status in one line: reproduced on the shipped v21.3 EXE through the production Continue path, root cause found and fixed, fix verified on a frozen EXE on the same production path plus F9 and death-revive variants. The exact user session was not observable; that the user's session passed through a restore is inferred (see Uncertainties).

## 1. Investigation

### Which scene
Scene is `LizCaptain_Intro` (compiled bank `001_swamp_pyscripts.bin` id 14, byte 1811), trigger `_prim_TriggerZone_BigLizIntro` in `corner_ruin_ws_00.mgp`, zone x -6987..-5802, y 9428..9775, centre (-6395.03, 9602.12, 243.661). Actors: the brown beast is `_prim_Monster_05_325` (Swamp_LizadMan_Type3, CharacterTable row 363, `lizardman_big.bdae`, nameplate "Bogwomp Chief", HP 260); the green lizard is the captive `_prim_Monster_08_329`. This agrees with the coordinator clarification (first appearance of the brown champion, which knocks out the green lizard) and with B091/B094/B068. It is distinct from B127's `_prim_Monster_3` boardwalk Type2 champion and from the Troll.

Authored command sequence as executed (log of the shipped EXE): Begin (BlockSaveGame, FlushMessages, MarkCharacterAsScripted All, Hide/ShowFlash, LockCharacter All, StopActor, EnterCutSceneMode); #1 LockCharacter All; #2 SetCameraTarget captain (1 s transition); #4 SpawnCharacter captain; #6 LookActor; #9 PlayActorAnim `big_lizardman_pre_attack_01` (wait); #10/#13/#15 PlaySound 474/120/476; #11 PlayActorAnim `big_lizardman_attack_01`; #16 KillActor captive (Script kill reward/quest events, dead prefix); #17 `post_attack_01` (wait); #18 `idle_01`; #20 SetCameraTarget("") (camera back to the local player); #21 UnlockCharacter All; End contract (PutIdle, ..., ExitCutSceneMode, SaveGame).

### Visual evidence
- Supplied screenshot `.t3/userdata/attachments/35094ffd-...-1ccd114d-....png` (file time 10:42:19): tiled floor with skull piles, brown beast above a standing bright-green lizard, hero absent, HP/mana, skill buttons, "5 Potion: 4", pause button, no SKIP button, i.e. the HUD is back but the camera is still on the scene. Direct observation.
- Original v1.0.3 Part 1 (`z_Zky7qQdYs`), B091 sheet `.local-inputs/claude-v20.1/fix091/audit/captain462-478.png` (2 fps, 462-477.5 s) and B068 `moth-captain2.jpg`, viewed this session: SKIP is shown while the camera sits on the tiled chamber; the green lizard walks in, the brown beast strikes it, the green lizard falls and crawls away (about 468-470.5 s); HUD returns and the camera is back on the hero; at 474-478 s the beast has the "Bogwomp Chief" plate and ring and fights the hero. A 2 fps sheet cannot prove sub-second timing; no timing claim is made from it.
- Reproduced shipped-EXE frames (section 4) show the exact screenshot composition, and the fixed EXE frames show the full sequence (green lizard knocked away, camera back on the hero, Bogwomp Chief plate/ring fight, death, gold).

### Logic evidence
- `Script_SpawnCharacter::Execute` 0x45f400 (pseudocode line 248020 of `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`): `ObjectManager::GetObjectByName(name)`; if a Character, `CharStateMachine::SM_SetSpawnState`. There is no lifecycle registry; any live Character can be spawned.
- `Character::Serialize` 0x3a65c4 writes `SM_GetState`; `Character::Deserialize` 0x3a6210 (line 122398) maps a saved state 1 (Spawn) or 17 (PreSpawn) to `SM_SetPreSpawnState`, states 0/2/12 to Limbus, others to Idle (Limbus if dead). So an unspawned or mid-Spawn Character is restored as PreSpawn and a later SpawnCharacter works on it.
- Port: `OriginalActorLifecycle` records are created only once at level start (`main.cpp` lifecycle loop, `actorLifecycle.add`). Both restore sites, the F9/Continue frame restore and the death-checkpoint restore, call `actorLifecycle.clear()` (records borrow `ActorState*` of the replaced world) and never rebuild them. `OriginalCampaignWorldAdapter` kind 30 calls `lifecycle_->spawn(id)`, which returns `Unknown original lifecycle actor`.
- `CampaignHost::frame` aborts the failing cutscene (`abort_cutscene`): HUD, SKIP, cutscene mode, save block and controller locks are restored, but it never touched the camera target (the scripted `SetCameraTarget` #2 stays latched until the authored #20, which never runs) and it did not unset the scripted marks (`scripted_` kept the local player unqualified for every later trigger contact).
- This exact failure signature (`cutscene aborted: Unknown original lifecycle actor`) was historically fixed at level start in Preview 16 (HOST/CINE2 reports) by registering lifecycle records; the restore paths regressed it.

### Evidence from the user's save
The package's `character.world-001_swamp.gsave` (written 10:43:29, about 70 s after the screenshot, by the quit resume point, which is skipped while a cutscene blocks saving) was copied and inspected read-only with a private tool: `_prim_TriggerZone_BigLizIntro` activation count 1; hero at (-6295.39, 9515.83), the contact spot at the south edge of the zone; captive alive (HP 47.5) at its authored position; captain at its authored spawn with physical presence 0; two Bogwomp lizards just south of the zone dead; Rene present near the hero; profile shows 4 potions (a restored Continue of that profile shows "5 Potion: 4", matching the screenshot). A save written while save-blocking was off, with the trigger consumed but the captive alive and the captain unspawned, is the state left by an aborted scene. It does not show the cause of the abort.

### Uncertainties (not turned into rules)
- The user's session was not observed. The fix addresses the one abort cause reproduced; the user's route through a restore (Continue from the main menu, F9, or a death Continue earlier in the same session) is inferred from the abort signature plus the save contents. No abort was reproduced on a session that never restored.
- Not found: any abort in these no-restore variations on the shipped EXE (inside-zone start, walk-in with the south Bogwomps alive, SKIP at host frames 10/60/130/190/215/240, pause/resume mid-scene, F5 and F9 mid-scene (rejected as blocked), quit mid-scene (save skipped), 12 RNG seeds including loot drops).
- Original behaviour for an aborted cutscene does not exist (the source has no abort). The camera/scripted-mark release on abort is a defensive port policy that mirrors the End contract (`EndScriptedCutScene` Unset + the authored closing `SetCameraTarget("")`).

## 2. Implementation (local commit on branch `b141-camera-lock`)
- `original_actor_lifecycle.{hpp,cpp}`: `add()` keeps a baseline of the preset PreSpawn/Limbus actors' authored facts; new `detach_for_restore()` (drops borrowed records, keeps the baseline; `clear()` stays the full reset), `pre_spawn_baseline_ids()`, and `restore_pre_spawn(id, actor, enabled)`, which registers the restored actor at PreSpawn17 (flags 0x1300, collisions off) without replaying any transition effect, so the later Spawn transition runs the same code as on a fresh level.
- `main.cpp`: helper `restoreLifecyclePreSpawn` re-registers exactly the baseline actors that are alive, not source-Limbus and have a known absent physical body (Deserialize: Spawn1/PreSpawn17 -> PreSpawn). It sets the same maps as level start (`lifecyclePhysical/Collisions/Flags/IdleSuppressed`) and publishes session state 17 only when it is not already 17. Called after rebind in the Continue/F9 path and after the captured population admission in the death-checkpoint path. Both sites now call `detach_for_restore()` instead of `clear()`. New host service `release_camera` (zero-duration `seed_target(local player)`, the call the restore paths already use).
- `campaign_host.{hpp,cpp}`: `abort_cutscene` unsets the scripted marks and calls `release_camera`.
- No actor, map or scene name list; no guard weakened (`CampaignCameraAdapter::tick` and the lifecycle `failed`/unknown-actor errors are unchanged; unspawnable non-baseline actors still fail explicitly).

## 3. Isolated tests
- `original_actor_lifecycle_tests`: baseline survives `detach_for_restore`; only preset PreSpawn/Limbus actors keep one; restore registers at 17 with no effect calls; duplicate restore refused; non-baseline actor refused; Spawn transition then runs on the restored actor; second restore after a spawn works; `clear()` drops the baseline.
- `campaign_host_tests` (real compiled bank, real `OriginalCampaignWorldAdapter`/`CampaignCameraAdapter`): the abort at SpawnCharacter now hands the camera back exactly once (target NewCamSpot -> local player) and clears the scripted marks; Begin does not release the camera.
- Full CTest of the private build: 177/177 passed (`.local-inputs/v21.4/b141/ctest.log`; the worktree asset root was supplied by a temporary junction to the primary `.local-inputs`, removed non-recursively afterwards).

## 4. Integrated runtime and visual verification
Private build `.local-inputs/v21.4/b141/build` (clang++ 23.1.3 Release, fresh configure; baseline build of HEAD 45a0c602 `base-build.exe` SHA256 `1273CDA6B95D20E9034237F6BACEC9BD68D65B2384601A8D969D7CB11ACC06EE`). Quiet hidden isolated desktops (`quiet_run.ps1`, `quiet_click.ps1` variant), copies of the package saves, normal compiled banks, `--campaign-triggers` as in the package args, no named-start/condition injection.

Frozen fixed EXE: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/v21.4/b141/frozen/dh-foundation-06D428C47D73EAE43257C540935BD2BB83FD1226E358869FA8F74990D4982D52.exe`, SHA256 `06D428C47D73EAE43257C540935BD2BB83FD1226E358869FA8F74990D4982D52`.

| Run (`.local-inputs/v21.4/b141/runs/`) | EXE | Result |
|---|---|---|
| `base-play` (direct swamp start, hero in zone) | shipped 276C | scene completes: kill at frame 222, SetCameraTarget("") at 300. Control: a fresh session without a restore does not fail |
| `cont-play` Continue from a normal pre-trigger resume save (`pre-save5`), walk north into the zone | shipped 276C | `cutscene aborted: Unknown original lifecycle actor` after SpawnCharacter (frame 111), HUD back, camera final target = captain; frames reproduce the user's screenshot (`evidence/continue-before-after.png`, top row) |
| `fix-cont-play` same input | fixed | `Lifecycle restored PreSpawn actor=..._prim_Monster_05_325`; scene runs 63-360, `Script kill complete` of the captive, camera target back to the player; hero visible with HUD (bottom row of the same sheet) |
| `f9twice-base` / `f9twice-fix`: direct start, F5 at frame 5, scene, F9 at 352, walk back in (second scene) | shipped / fixed | shipped: first scene ok, second aborts and locks the camera; fixed: both scenes finish. Shows F9 alone (no menu Continue) triggers it |
| `death-base` / `death-fix`: hero killed at frame 15, Space continue (checkpoint restore), then walk into the zone | shipped / fixed | shipped aborts the first captain scene after the revive; fixed completes it |
| `fix-enter`: Continue + real Win32 Enter key mid-scene (frame 118) | fixed | `SKIP fast-forward ok`, script finishes, camera target = player |
| `fix-c-skip120/250`, `fix-c-play` | fixed | scene ok played and skipped; no abort |
| `fix-f-play`, `fix-f-skip` (walk to the captain after Continue, test player HP 1000 as a duration fixture) | fixed | Bogwomp Chief plate/ring, damage both ways (24 and 28 lines), End34 death, body released, gold; played and skipped. Contact sheet viewed (`fix-f-play/sheet.png`) |
| `f5-base` / `f5-fix`: F5 after Continue | shipped / fixed | identical result (checkpoint saved, same HP/RNG): re-registered records do not block checkpoints |
| `fix-d-play` | fixed | artefact of the HP fixture (`health exceeds max_health`) aborts the scene at End (after its own SetCameraTarget); the camera is on the player. Not a mid-scene abort proof |

Not run: a real mouse click on the SKIP button during the Continue path (SKIP is exercised by `--campaign-skip-frame` and a real Enter key), pause during the Continue path, audible audio.

## 5. Remaining gaps
- The user's own session path is inferred (see Uncertainties). If the user can say whether the same session began with Continue/F9/death-Continue before this scene, it would confirm it; if the problem recurs on the fixed build, the log lines `[campaign] cutscene aborted: ...` and `Lifecycle restored PreSpawn ...` identify the next cause (the shipped package writes no log; a run through `Play-swamp.cmd` with stdout redirected would capture it).
- Existing saves already written after the abort are not repaired: the trigger count is consumed, so the scene will not replay; on the fixed build their captain restores as PreSpawn (consistent with Character::Deserialize), i.e. unspawned. Not fixed, not verified beyond a restore of the user's copy (`fix-usersave`, no error).
- Only preset PreSpawn/Limbus actors are re-registered. Other lifecycle-declared actors (Idle `auto_spawn=0` actors, spawn-pool slots in other states, actors despawned to Limbus) are still record-less after a restore, so a later script that spawns/hides them would still abort, now with a restored camera. Not exercised in this session.
- The death-checkpoint restore does not call `campaignHost.restore_lifecycle` (trigger counts persist across a death revive); observed in code, outside this fix.
- The camera/scripted-mark release on abort has unit coverage and a real abort observation only on the shipped build (lock) and an abort-at-End observation on the fixed build; no real mid-scene abort on the fixed build exists because the known cause is removed.
- Sub-second timing and exact captive pose against the original video were not compared; only action order and framing were.
- Android native port untouched.
