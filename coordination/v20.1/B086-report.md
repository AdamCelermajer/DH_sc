# B086 report: quit and relaunch (Continue) puts the hero at the wrong place and he cannot move

Worktree `DH_wt/fix086`, branch `fix/b086` from `windows-foundation` (2a8faf0f). Build: `build-fix086`, exit 0, `ctest` 165/166 (only `session_skill_binding` fails, allowed). Runs: baseline = accepted package EXE `windows-source-clock-v20` (SHA256 8BA1E537...8CCE); after = `build-fix086/dh-foundation.exe` (not packaged, so no package hash). All runs through `quiet_run.ps1` (hidden, silent, isolated dirs). Scratch: `.local-inputs/claude-v20.1/fix086/`.

## Verdict on "does our behaviour match the video and the IDA logic"
- Video: the reference video (P1/P2) never shows quitting or relaunching, so there is no video evidence for save/resume. This is internal infrastructure; the visible invariant it supports (hero and world are where they were) was checked against the user screenshot shot-7 and against our own pre-quit frames. Not a video match, stated explicitly.
- IDA: the new behaviour matches the source functions listed below. Two points are inferences (marked).

## Observed (reproduction in the real EXE)
1. shot-7 reproduced exactly (`r3/f450.png`, baseline EXE). The shot-7 HUD (zoom `shot7-hud.png`) shows an EMPTY HP bar (black) with full MP: the hero was loaded with HP 0. The hero stands at the default start spot (1090.75,-212.2) beside the cages, Rene left, moth top right, sword up, and cannot move: `playerControllerBlocked` includes `!alive()`, so movement is locked (also why skills are dead in B083 after a death).
2. How HP 0 reaches the disk (observed): dying with no checkpoint record logs `Death host failed: No source checkpoint record exists for this level yet` (`d2`), so Continue on the death screen does nothing and the hero lies dead with the HUD live. Any later profile write stores health 0: `F5` printed `Saved live checkpoint ... HP=0` (`d3`); the pause "main menu" profile-only path (main.cpp 5642-5654) copies live HP 0 as well. Relaunch (`r3`): `Selected profile source projection HP=0/165.098`.
3. Position/world: the menu "Continue" (same actions as a new game) never restores anything. `--resume-game`/F9 restore exists and was verified in P17 but is not reachable from the packaged args. `gameplay.save` (written by pause > main menu) is never read by the menu route; the level-world `.gsave` is read only on level-transition arrival. Nothing is written when the window is closed. So a relaunch always starts at `--position 1090.75,-212.202` with the profile (`r1`, `r2`).
4. Relaunch of a normal save after the opening (`r1/f200.png`) shows the same composition as shot-7 but with full HP, and the hero can move (scripted move works). So the "cannot move" part of shot-7 comes from the HP 0 profile, not from a persisted cutscene/tutorial/input lock (`globalControllerBlocked=0` in the final log of every run; tutorial flag file unchanged).

## IDA evidence (APK 1.0.2, pseudocode-all.c)
- `MenuBase::FS_ContinueGame` 0x420e64: Continue calls `Application::LoadLevel(... load-savegame flag = saved byte ..., 1 ...)`; `FS_StartGame` 0x4220a0 deletes the old save and passes flag 0.
- `Level::Level` 0x3f34c0 stores the flag at +241; level state 0x21 (pseudocode ~179467) runs `LevelSavegame::Load`, which deserializes `PlayerCharacter_0` (position, state, dead) and all saved objects. The profile is not reloaded by it (existing DEATH-FLOW report).
- `Application::Quit` 0x323bc4 and `GoToMainMenu` 0x32c1f4 both call `Level::QuickSave` (0x3f059c), guarded by Level state 38 and `!IsDead()`, then save the profile (`Character::SG_Save`/`SG_SaveLocalPlayer`). QuickSave writes the hero position into Character+1306..1308 and `LevelSavegame::Save`.
- `Character::InitPost` 0x3b4d60 (~132187): `Revive(this,nullptr,1)` then `_InitHpMp` (0x3b3a70 = `RegenHP(-1)`, 0x3bdca4 fills HP to the maximum). Every level load therefore starts a Character alive at full HP. Inference: a profile written dead should not start a session dead.
- Level state 0x23 (~179590): if `LevelSavegame::ValidateCheckpoint` fails or the arrival flag is set, `Level::CheckpointSave` runs at the spawn point. The original always has a checkpoint after a level load. NOT implemented here (see gaps); it is why the death Continue above fails in our port.

## Root cause
(a) no save at quit/main menu and no restore on Continue (menu route never uses the verified restore); (b) a profile written while dead (HP 0) starts a dead, immobile hero; (c) a Continue restore in a fresh process was rejected by two overly strict compatibility checks (see below), which is why the restore had never worked outside the same process.

## Change (small, generic, no map/actor names)
- `main.cpp`: (1) startup Continue check after the frontend/profile is known: the profile's level-world save (`level_world_path(save, level)`) is used when it loads, is the same level and has the same profile id (`other_profile`/`other_level`/`unreadable`/`none` are logged and start fresh). It sets `resumeLevelWorld` + the existing frame-0 restore (`loadFrame=0`, `resumeGame` = no bind banners) and the F9 branch reads that file at the start frame (later F9 still reads `--game-save`). (2) `saveResumePoint()`: `Application::Quit`/`GoToMainMenu` equivalent: skipped for a dead hero or a cutscene that blocks saving (keeps the previous save), otherwise lifecycle persist + level-world save + profile rewrite from the same snapshot. Used by pause > main menu and by closing the window; a transient rejection (attack window) retries for up to 90 frames. (3) `--quit-frame N` (test aid) takes the window-close path. (4) the Continue restore flags are cleared when a level transition starts the next session.
- `game_save.cpp`: `checkpoint_binding_compatible` no longer rejects (a) a visual-only actor whose lifecycle revive filled a cell that a fresh load still has as the source `-1` sentinel (cave troll 110 HP vs -1), (b) a saved-untargetable actor against a fresh targetable binding (`targetable` is derived from the lifecycle permission; the prisoner). It still rejects saved-targetable against an untargetable binding, a different player role, and a saved sentinel that the session defines differently. Error text now names cell/values.
- `player_profile_properties.cpp`: a profile with health <= 0 projects to full health (InitPost Revive + `_InitHpMp`); MP and partial HP stay as saved.
- Tests: `player_profile_properties_tests` (dead profile starts at full health, MP kept), `game_save_tests` (saved-untargetable restore into a targetable load accepted and applied, reverse refused). Full ctest 165/166.

## Before / after (real EXE)
| Case | Before (package EXE) | After (build-fix086) |
|---|---|---|
| Quit in free play after the opening, moved right 260 frames, relaunch Continue (`n1`,`n1r`) | start spot 1090,-212, world at its opening state | log `Continue restore frame=0`, `Restored live checkpoint HP=165.098`, hero at the quit position 1579.04,-201.361 (same as `Level world snapshot ... reason=quit`), Rene/moth/cages as before quit, hero moves afterward (`n1/f4790.png` vs `n1r/f20.png`) |
| Dead profile (HP 0) + older level save (shot-7 repro, `d3` -> `r3`/`r3a`) | HP 0 bar, cannot move (`r3/f450.png`) | restored from the last level save, HP 165/165, moves (`r3a/f450.png`) |
| Dead profile, no level save (`r3b`) | HP 0, dead | HP 165/165 at the spawn, moves |
| After a chest (`c1`,`c1r`) | n/a | chest stays opened across relaunch: Space gives `status=rejected_state state=4->4` (no second loot) |
| Quit while an enemy attack blocks the checkpoint (`x1`) | n/a | `Resume point retry` frames 300..319, saved at 320 |

## Not verified / gaps (stated plainly)
- Quit after death + Continue (revive) was not run end to end: the death Continue fails in this port when the level has no checkpoint record (observed, `d2`); IDA shows the original creates a spawn checkpoint at level state 0x23. That is the B082/death owner's fix; I did not implement it to avoid a conflicting change in the death flow. With this change a dead hero is recovered by the next relaunch (last level save, or revive when none).
- Quit mid-opening cutscene: saving is skipped (`cutscene_blocks_saving`), so the opening replays; original behaviour here is not established.
- Dead-quit rolls the world and profile back to the last level save together (consistent state; original profile/world interplay for this case is not established).
- Partial HP across level loads keeps the port's P14 behaviour (the source refills HP in InitPost; I only revive a dead profile). Not changed, unverified against the original.
- Direct launches (`swamp.args`, `--fresh-player`) do not Continue: they regenerate starter gear and the restore rejects the changed equipment instances. The menu route (`Play.cmd`) is the Continue path. For the dead-profile bug the swamp route benefits from the HP fix only.
- Quit during transitions, real mouse/keyboard window-close (WM_CLOSE) not driven: the real close shares `window.should_close()` with the tested `--quit-frame` path, not exercised with a real window.
- Drops on the ground never survive a relaunch (existing P14 rule); not changed.
- Stale `.gsave` from a different profile in the same slot is ignored by the id check; it is not deleted when a new game starts (the original `FS_StartGame` deletes the save).

## Package files required
None (code only). Packaging must not add `--resume-game`; Continue works from the existing menu route.
