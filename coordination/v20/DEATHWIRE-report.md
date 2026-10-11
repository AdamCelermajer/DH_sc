# I024 death wiring report (v20deathwire)

Branch `v20/deathwire` (worktree `DH_wt/v20deathwire`), base `107f2a86`. Commits: `41596db5` (host + tests), `19748430` (FX gate, validator/latched message), `182e48dc` (main wiring), `f1d517f4` (checkpoint vitals scope, fixture, restore fixes).

Status in one line: the offline death → quote screen → Continue → checkpoint restore → revive → fade chain runs end to end in the real EXE, started by a real enemy hit at 10 HP. Several checks remain open or only partly demonstrated (listed in "Remaining gaps"). The brief's single-run order (checkpoint contact, then death, in one run) was NOT achieved; see "Integrated runtime".

## 1. Investigation (evidence and kind)

Sources: `coordination/v20/DEATH-FLOW-report.md` (source facts), IDA export `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c` (APK 1.0.2), SWF facts in the same report. No original death footage exists in the supplied videos (report: sparse grids and blackdetect found no death UI), so no video-based match is claimed for any death visual.

Facts checked directly in this session (IDA = logic evidence):
- `ReviveLocalPlayers` (0x37565c region): order in the pseudocode is `Level::LoadCheckpoint` FIRST, then `VoxSoundManager::SetSoundVolume(1, VolumeFX)`, then buffs removal, revive position, `Character::Revive(...,1)`, `SM_SetIdleState(0)`, `MinimumPotionsOnDeath` top-up. The brief and DEATH-FLOW-report say FX restore comes before LoadCheckpoint. I followed the pseudocode and the implementation restores FX right after LoadCheckpoint. Discrepancy stated explicitly.
- `Character::Revive` → `_InitHpMp` → `RegenHP/RegenMP(-1)` fills to the max properties; `revive_actor_state` (existing helper) matches that.
- Checkpoint eligibility, IDA-cited (new mappings in `main.cpp` `object_checkpoint_facts`):
  - `GetNewInstance<AnimatedDecor>` (0x342600) → `GameObject(…,20)` → `ObjectBase` (0x33f15c) zeroes byte+40 → ineligible (object_base).
  - `OpenableContainer` (0x3a1cb0) and `DestructibleContainer` (0x3a14bc) call `Container::Container` (0x3a06dc, sets byte+40=1) → container family.
  - `QuestMoveInZone` (0x396330) and `CheckpointZone` (0x39594c) call `Zone::Zone` (0x397c2c) which leaves byte+40 at 0 → area_zone (ineligible).
  - `TriggerZone` (0x39b890), `TriggerZoneExitLevel` (0x39c9a4), `TriggerObject` call `Trigger::Trigger` (0x398f44) which sets byte+40=1 → native_trigger (eligible unless skip_checkpoint). NOTE: DEATH-FLOW-report calls TriggerZone "area zone"; the IDA constructor shows otherwise. My mapping follows IDA; the report text should be corrected.
  - `Dummy`, `Waypoint` → object_base (DEATH-FLOW-report).
  - Lever, AnimatedObject, other unmapped gametypes stay fail-closed.
  - `campaign.*` world records are host-owned script state, not GameObjects: excluded (object_base), never restored.
- Actors: every GameSave actor is a Character; runtime-spawned Characters (respawns) have no authored declaration and take the constructor default (eligible, no skip).
- PreSpawn/Limbus: the restore plan's "already" facts now come from the live Session state (`original_actor_state` 1/17 = PreSpawn lifecycle); Limbus has no port owner and stays "not already"; CanRespawn stays unknown and fails closed.

Uncertainties (not resolved):
- `LevelSavegame::LoadCheckPoint` with NO checkpoint file: the source behaviour was not established. The implementation fails closed with an explicit message (the death host then releases input and reports the failure).
- Gameplay pause under the death menu: `_HandleLocalDeaths` is called from `PlayerManager::Update` while the menu is up (IDA), so I treat the Level as NOT paused during the modal (inference from that call path, not a direct observation). An earlier pause implementation made the restore refuse (lizard attack timer frozen at 800 ms), which is the evidence for this choice.
- Death screen visuals: the skull-shaped aperture and the absence of the red fill are the rendering of the authored geometry; they are NOT verified against original footage (no death screen in the videos).

## 2. Implementation

New module `features/persistence/death_host_v1.{hpp,cpp}` (+ `death_host_v1_tests.cpp`, CMake `death_host_v1` test):
- Source-gated admission (`AdmissionV1`): local Character assigned and IsDead; Level READY and PlayerManager phase are proxies (`!pendingTransition`, `drawn>0`); online route rejected. One flow per death.
- FX mute at death; wall-clock 30 Hz accumulator for the death screen (`DeathScreenRuntimeV1`) and the fade (`OriginalFadeFromBlackRuntimeV1`).
- Modal capture: `modal()` absorbs keys and pointer; pointer release in the stage → Continue (`original_death_continue_hit_test_v1`), Space = PC adaptation at the stage centre. Duplicate Continue cannot replay (runtime rejects it).
- Revive chain inside frame 88 (`run_authored_revive_v1`), ordered: pop death screen → push fade → LoadCheckpoint (+FX restore) → buffs → revive position → vitals → idle → minimum potions → level music restart → (separate) frame-88 previous-music callback. Frame 32 pops the fade.
- Failure latches: a failed revive releases the modal and reports once (no hang, no re-wrapping).

`main.cpp` hook points (localized, separate blocks):
- Trampolines and quote drawer before the label helpers (`DeathWireV1`, `deathWireStep<N>`, `deathQuoteDraw`: word wrap, Fontin SmallCaps 18 px, `$player` through `menuLocalization.symbol`).
- Death host block before `FramePacer`: the restore sequence mirrors the F9 live restore (`detach_for_restore` → `restore_death_checkpoint_snapshot_v1` → `rebind_after_restore` → container restore → native bodies → navigation → bind audio/equipment/death rewards/container loot/quests/presentations → skill coordinator and faery clock reset).
- Admission and clock hook before the pause decision; death does NOT pause the Level (see uncertainty).
- Input: death modal swallows key input and the pointer; release → `pointer_release`; Space press edge → `continue_key`; absorbed attempts are logged.
- Overlay draw: sprite742 / sprite743 after the HUD inside the HUD overlay.

Audio: `RuntimeAudioHostV1::set_fx_muted` gates `submit_actual_play` (accepted and dropped while muted, approximating source volume 0); `RuntimeSessionAudioV1::set_fx_muted`.

Checkpoint production (already in WIP, now runnable): genuine contact at CheckpointZone_02 saved `key=(-9166.11,11896.8,234.447) revive=(-9262.63,12441.3,255)` (distinct). Changed-key rule and zone gate unchanged.

Validator scope (`game_save.{hpp,cpp}`): `validate_game_save(…, checkpointUnboundVitals)`, `capture_game_save(…, checkpointCapture)`, `save_game(…, checkpointWrite)`. Only checkpoint paths pass `true`: an unbound source property (-1) with live 0 is accepted for targetable NPCs. Profile and F5 saves keep the strict rule; the combat_session test that forges targetable vitals still passes. Without this, the camp NPC `_prim_NPC` (targetable, no MP property) blocked every checkpoint.

Test-only, inert unless passed: `--test-player-hp N[@FRAME]` (sets live HP and sheet entry 36 at FRAME), a nearest-enemy trace every 50 frames and a fixture actor listing. Packaged args must not contain them.

## 3. Isolated tests

- `death_host_v1_tests` PASS (8 groups: gate and admission, timeline frame 64, Continue + revive order, early Continue skips frame 64, Space continue, revive failure fails closed, fade pop at frame 32 and second death, wall-clock 30 Hz, non-READY at frame 88 fails closed).
- `death_checkpoint_snapshot_v1` fixture corrected (it predated the current validator): save version 4, valid CQPG envelope with matching character id, character identity fields, mirrored character stats and original-sheet vitals. Its assertions are unchanged except the expected QEST bytes.
- Full ctest, final build (after all commits): `99% tests passed, 1 tests failed out of 162` — only `session_skill_binding` (known worktree asset-root case). Build exit 0.

## 4. Integrated runtime (frozen EXE)

Frozen EXE: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/v20/deathwire/frozen/dh-foundation-deathwire.exe`
SHA-256: `ee315f4b430c77144c1ebf2832195c39ddf0109493ea286e80ecea67b17450fc`
Run: `--startup-config .../v20/deathwire/runF/run.args` (quiet, hidden: `tools/quiet_run.ps1`, job `runF/jobs.json`), log `runF/run.log`, captures `runF/f*.ppm` and `.png`.

Sequence (frames in the log; death flow timing uses the 30 Hz wall-clock UI clock):
- Setup: start at (-6752.641, 938.285, 250), 100 units from AI-enabled lizard `7118915781085668844` (AI gate `Swamp_LizadMan_Type1=Limbus`), `--test-player-hp 10` at frame 0 (HP 10 of 165.098). Checkpoint restored from the separate contact run (below).
- Lethal hit: `Damage frame=131 attacker=7118… removed=1.11719 dead=1`. The kill is the lizard's normal attack through the Session damage path; no set-dead call.
- 132: `Death FX volume muted`, `Death flow admitted frame=132 dead=1 level=READY phase=1`.
- 259: `Death NativeScreenIsBlack` (timeline frame 64).
- 400: `Death Continue (Space)` (PC adaptation).
- 423: `Death profile after restore: equipment=4 inventory=4 gold=0 xp=0` (before = after), `Death checkpoint restored … actorsRestored=28 actorsMissing=16 objectsRestored=25 objectsMissing=0 qest=1`, `Death FX volume restored`, `buffs cleared records=0`, `revive position pos=-9262.63,12441.3,255` (= checkpoint revive, not the death place and not the zone key), `Character::Revive HP=165.098/165.098 MP=27.25/27.25` (source-derived max), `SM_SetIdleState … restored-idle`, `MinimumPotionsOnDeath=3 potionsNow=3`, `Level music transition: kind=revive-stop … fadeMs=2`, `NativePlayPreviousMusic frame=423`, `Source checkpoint contact zone=_prim_CheckpointZone_02 result=2` (the re-entry at the revive point is the same key: skipped, as the source rule requires).
- 440, 470: `Death input absorbed (Space) … Continue key is absorbed by the death modal` (duplicate presses during the fade).
- Later: `Level music transition: kind=revive-restart … fadeMs=1000` (after the update, as the audio host defers it).
- 483: `Death fade-from-black PopMenu menu_FadeFromBlackScreen frame=483` (33-frame fade, pops at its frame 32).
- After revive the camp fight continues: the same lizard hits the revived knight (~8 per hit), HP 165.1 → 59.3 by frame 1350 (real post-revive combat).

Contact run (separate, same EXE): `.local-inputs/v20/deathwire/runA2/run.args`, log `runA2/run.log`: `Source checkpoint saved zone=_prim_CheckpointZone_02 key=-9166.11,11896.8,234.447 revive=-9262.63,12441.3,255 actors=44 objects=25 qest=1` — the contact came from walking into the box (the zone aid `--player-to` does not move the Session actor in this build).

Why two runs: no single run reached CheckpointZone_02 and an AI-enabled enemy: the three AI-enabled monsters (7118…, 1125…, 4660…) are 2300+ units from every checkpoint zone, and the camp enemies do not engage without it. The fight run therefore loads the checkpoint file written by the genuine contact run (copied, same level, same save naming). This is a deliberate departure from "checkpoint then die in one run"; the checkpoint itself is genuine.

## 5. Visual verification (frames inspected, `runF`, PNG via `tools/ppm2png.py`)

- f200 (fade mid-way, before the quote): black surround with a skull-shaped aperture through which the scene shows at full brightness (the dead lizard and the knight lying down). No red fill visible. Not verified against original footage.
- f260: fully black (quote fade-in begins around timeline 65).
- f300: "Return from the void, Player! Gothicus is in her time of need!" — Fontin SmallCaps, white, centred in two lines, `$player` replaced. Upright after fixing my own converter (first render was flipped).
- f430 (earlier run, same behaviour): post-revive fade over the camp scene; HUD shows "5 Potion: 3".
- f800 (earlier run): settled camp fight with Bogwomp nameplate and combat text.
Visual uncertainties: the exact authored position of the quote block and the ornament geometry could not be compared with original footage.

## 6. Remaining gaps (exact)

1. Single-run contact-then-death not achieved (two runs, see section 4).
2. Within-run "changed object reverted by restore" not demonstrated: the restore counts are real (28 actors, 25 objects) but no post-checkpoint change happened inside the run that was reverted.
3. Gear/XP/gold preservation is weak evidence: the fixture's profile has xp 0 and gold 0; equipment and inventory counts are unchanged (4/4).
4. Pointer release and pointer absorption are unit-tested and wired, but not exercised in the live run.
5. Duplicate Continue during the hide timeline (frames 76-88) is unit-tested only.
6. Frame-88 previous music: the port has no previous-track owner, so the callback is logged and a no-op; the level track restart is the revive's `on_local_players_revived`.
7. Buffs: no port buff owner; the step logs `records=0`. Source `PROPS_RemoveAllBuffs` behaviour is not reproduced beyond that.
8. SM_SetIdleState: `script_put_idle` rejects the player's sentinel id; the restored action is idle (from the checkpoint plan) and that is what is reported.
9. `LoadCheckPoint` with no checkpoint file: fails closed; source behaviour not established.
10. GameEventManager::ReInit, Character SG_LoadCheckpoint(16)/QEST-only receiver, SG_Update: no separate owners; the checkpoint restore carries QEST and rebinds quests. Not a literal mapping.
11. Campaign lifecycle is not restored by a checkpoint (not a GameObject); `load_game` remains strict, so a level-world file written with the checkpoint vitals rule cannot be re-read by the normal transition loader (`load_game` has no checkpoint flag). Needs a follow-up before transitions can return to a checkpointed level.
12. Lever and AnimatedObject: unmapped, fail closed (not present in this level's checkpoint path so far).
13. Gameplay not paused under the death modal (inference, see section 1).
14. FX mute is an accept-and-drop gate, an approximation of source volume 0.
15. Online and global-death routes: not wired (offline only, as required).
16. Death-screen ornament and quote placement are not verified against original footage.
17. Two fixture facts for the test harness, not packaged: `--test-player-hp`, `--position` at the lizard, and the nearest-enemy trace are test-only; the run args are kept in `.local-inputs/v20/deathwire/runF`.
18. `session_skill_binding` still fails in the worktree (known).

Files: `port/windows-foundation/features/persistence/death_host_v1.{hpp,cpp}`, `death_host_v1_tests.cpp`, `main.cpp`, `game_save.{hpp,cpp}`, `features/persistence/death_checkpoint_snapshot_v1.cpp`, `features/persistence/death_checkpoint_snapshot_codec_tests.cpp`, `features/audio/runtime_audio_host_v1.{hpp,cpp}`, `features/audio/runtime_session_audio_v1.{hpp,cpp}`, `CMakeLists.txt` (death_host_v1 only).
