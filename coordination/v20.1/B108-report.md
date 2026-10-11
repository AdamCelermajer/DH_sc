# B108 report: Continue after death fails with "No source checkpoint record exists" (no CheckpointZone touched yet)

Worktree `DH_wt/fix108`, branch `fix/b108` from `v20.1/integrate` (dfc67d8d). Build `build-fix108` exit 0 (`-Jobs 2`), ctest all pass except `session_skill_binding` (allowed); `frame_pacer` failed once under `-j2` load and passes alone. Scratch: `.local-inputs/claude-v20.1/fix108/{a,b,c}`.

## Evidence
- Video: no death or level-start footage exists; not a video match (stated). Internal invariant: the original Continue always has a checkpoint.
- IDA (APK 1.0.2, pseudocode-all.c): Level state 0x23 (~179586-179612): when `LevelSavegame::ValidateCheckpoint(...)` returns 0 (no record for this level/row), or the Level arrival flag (Level+245) is set, or online non-host rules apply, it takes `Level::GetSpawnPoint` (else the local Character, else origin), builds key = that position and calls `Level::CheckpointSave(level, key, arrivalFlag)`, then clears Level+245.
- `Level::CheckpointSave` 0x3f04b4 gates: LevelSavegame (+236) present, Level state field (+304) > 33, local Character present, `!IsDead` (vtable+52). It writes key to Character+1306..1308; revive = Character position (+352) unless arg3 (arrival), then revive = key; calls `Character::SG_SaveCheckpoint` and `LevelSavegame::SaveCheckPoint` 0x463130. Same function CheckpointZone uses (arg3=false) but the zone adds its own same-key suppression (Character+5224); state 0x23 has none. Other producers: only CheckpointZone::OnCollisionBegins 0x395f04 (and the 111122 caller, a debug/cheat path).
- Consequence: a freshly loaded level always has a checkpoint; v20 port only created one on a CheckpointZone contact, so `Death host failed: No source checkpoint record exists` stranded the hero on the death screen (also noted in B086 report gaps).

## Change (main.cpp only, +18 lines)
Once per session after frame 1 (state > 33 analogue, player alive, a save path, no pending transition): if no checkpoint snapshot exists for the level, or the level was entered by transition, run the existing selective-snapshot machinery (`saveSourceCheckpointContact`) with a synthetic zone named `level_start` at the player's spawn position (key = revive = spawn), with the zone same-key suppression bypassed (state 0x23 has none). An existing checkpoint from a previous session is kept (ValidateCheckpoint succeeded). Logs: `Level start checkpoint frame=1`, or `status=exists`.

## Verification (real EXE, quiet_run, isolated save, fixture runF args, NO checkpoint zone touched)
Observed:
- Run a: `Source checkpoint saved zone=level_start key=-6752.64,938.285,255 revive=same actors=26 objects=25 qest=1`, hero killed by the lizard at frame 57 (`Death flow admitted`), f300 shows the quote plus the PC hint, Continue (Space) at 400 -> `Death checkpoint restored zone=level_start actorsRestored=26 actorsMissing=0 objectsRestored=25`, `Character::Revive HP=165.098/165.098 MP=27.25/27.25`; f500 shows the hero alive at full HP/MP bars with HUD; Space at 440/470 absorbed (`Death input absorbed`), no second Continue/duplicate restore. The hero was later killed again (test aid keeps HP 10 context/enemy adjacent) and the flow re-admitted at frame 864.
- Run b+c (relaunch with the same save dir after death on the death screen): `Source checkpoint loaded ... zone=level_start`, `Level start checkpoint status=exists`, hero alive at full HP (`HP=165.098/165.098`).
Inferred / not verified:
- Skills usable after revive: controller not blocked after revive (`controllerBlocked=0` in later frames, HUD slot 2 shown ready in f500) but I did not drive a skill cast after the revive in this fixture (B083 owns skill gating).
- Real quit-after-death + relaunch through the menu route: `--quit-frame`/resume-point live in the unmerged branch `fix/b086`, so the real window-close path was NOT tested here; only the relaunch-with-existing-checkpoint path above. With both merged, B086's dead-profile-revive plus this checkpoint cover it; re-check after merge.
- Arrival-by-transition variant (revive=key) uses the same call, not run separately.
- No unit test: logic lives in the main.cpp lambda wiring around the already-unit-tested death-checkpoint zone contact.
