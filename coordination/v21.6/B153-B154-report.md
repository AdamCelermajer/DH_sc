# B153 / B154: witch encounter (cutscene lag, garbled meshes, ASDAS caption; untargetable witch)

Branch `v21.6-witch` (worktree `.claude/worktrees/v216-witch`), base `488d9e82`. Not pushed. Tracker (`docs/BUGS-AND-IMPLEMENTATION.md`) and `docs/RESOLVED-BUGS.md` not edited: B153 is only partly fixed and B154 is not fixed.

Status in one line: B153 has one confirmed root cause fixed (per-frame checkpoint retry in direct launch) and one partly fixed cost (first-use cutscene clip binding); the garbled meshes were not reproduced. B154 is root-caused, but the admissible fix does not exist yet: the witch's own combat data is not ported.

Evidence was taken from the original reference video `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1`, the IDA-era recovered scripts (`scripts/ai/witch.luac`, `pydata/scripts/002_swamp_cave_witch_pyscripts.bin` decoded through `act1-script-inventory.json`), the frozen v21.5 EXE, and hidden-window runs of the worktree build.

## 1. Investigation

### B153 visual evidence

- User screenshot `...b1c088fe...png` (17:51): caption "And a natural execution chamber. We can't let her live and continue this...", speaker plate "ASDAS", witch-intro framing (priest left, hero right, white glow centre). Garbled/torn geometry is visible on the priest's robe and the left edge.
- Original reference frames extracted with ffmpeg from the v1.0.3 video (`C:/dh216/ref/w952..w970.png`): the same line appears at about 964 s, with the speaker plate "CRISR82" (the uploader's profile name) and the subtitle "crisr82! She must be the one who sealed the" at about 952 s. So the plate shows the player's profile name in the original too.
- Our frame 480 of the same intro (`new-big/f480.png`) shows the same line with plate "Player" (the test profile name). Composition matches the reference (large sword in foreground, priest left, hero right, SKIP top-left).
- "ASDAS" is therefore the player's profile name (the user's character name), not an original placeholder and not a fallback. Logic: `main.cpp` caption resolver replaces `$player` with the profile name for both caption text and speaker plate (`campaign_host.cpp` 151-155; `main.cpp` ~3885). No "ASDAS" string exists in any original asset (checked ASCII and UTF-16 across the reference package and `assets-extra`).
- Torn meshes: not reproduced. Our captures at 1201x720 and 1800x1100, direct start and route start, show no torn geometry on the priest or the hero. The user's torn shot is not explained; I have no evidence for a cause and make no claim.

### B153 logic and measurements

Timeline of the intro (`Intro_Witch`, frames from script start at frame 3): captions at frames 298, 376, 456; script finished near frame 740.

Frame measurements (hidden window, RTX 4070, `DH_PERF=1`):

| run | frames | avg per second | notable spikes |
|---|---|---|---|
| frozen v21.5 reference, direct intro | 2400 | 16.7 ms | 100-160 ms at startup; 25-60 ms sporadic |
| new build before fixes (`new-tap`) | 2400 | 16.7 ms | 325 ms at frame 437 (input), 146 ms at startup |
| checkpoint fix only (`fix2-direct`) | 1000 | 16.7 ms | 68 ms at frame 298, 84 ms at 535 |
| checkpoint + clip prebind (`fix3-direct`) | 1000 | 16.7 ms | 45 ms at 297, 36 ms at 536 |

The "input" phase (poll to the second input mark) includes the campaign frame, so it carries script commands and the level-start checkpoint.

**Root cause 1 (fixed): level-start checkpoint retry every frame in direct launch.**
- `Play-swamp.cmd` runs `dh-foundation.exe --startup-config swamp.args`. `swamp.args` has no `--save`, so the default `character.save` is relative (`main.cpp:427`). Its parent path is empty.
- The B108 level-start checkpoint keeps retrying while incomplete (`main.cpp` ~7070-7090). The writer (`services.save_level_checkpoint`, ~2446-2454) first calls `save_game` on the level world file, then `fs::create_directories(checkpoint_path.parent_path())`. For an empty parent that call throws `filesystem_error` ("create_directories [""]"), so the checkpoint never completes.
- Measured in `new-tap`: 2399 "Level start checkpoint status=not_saved ... create_directories" lines in 2400 frames, and the level world file is rewritten on each attempt. Average `input` phase 5.5 ms per frame (40 seconds sampled).
- The user's screenshot shows a console window behind the game with diagnostic text at its left edge (trailing "frame=40x"-style numbers and a "sequence=171 step=1 occurrence=17 dispatched=1 sets=250" line). That step-FX line is a different diagnostic (`main.cpp:4011`). I did not verify it is per frame.
- Route launch through the menu (`Continue`, absolute slot save) produced zero such lines.

**Root cause 2 (partly fixed): first use of each scripted cutscene clip binds the BDAE in the frame that plays it.**
- `PlayActorAnim` calls `CombatSession::play_actor_clip`, which calls `RetainedSequencePlayback::seed`, which reads and `bind_events` the BDAE on first use (`retained_sequence_playback.cpp:96-111`).
- Measured with a temporary timer (reverted): 7-15 ms per first-use clip (14.8 ms for `cs:563`, 7-10 ms for most later clips), 0.1-0.5 ms for already-bound clips. Frame 298 plays four clips plus a camera in one frame (68 ms stall); frame 536 plays one clip after several others (84-98 ms stall).

### B154 logic

- The witch row: `_prim_Monster_04_56`, `charpropsname=Witch`, `auto_spawn=0`, `ai_state=Limbus` (`bossroom_witch_.mgp`, `002_swamp_witchcave.rule.xml`).
- Script commands (decoded): `Intro_Witch` spawns it (kind 30) and later puts it in Idle at `WitchTeleport` (kind 31). `SkipCutscene_Bugfix` (trigger zone `_prim_TriggerZone_BugFix`) retires it (kind 29). `Return_Cinematic` kills it (kind 44).
- `witch.luac`: `OnTargetInCloseRange` calls `DoSkill(0)`; a teleport timer (10 s) runs on enemy spotted; death music on `OnDied`. The ranged skill is `witch_ranged_00.luac` with `projectile_dh2_bogwitch`.
- Our code: `collect_script_gameplay_actors` (`features/campaign_host/actor_clip_manifest.cpp`) removes any spawned actor that appears in a kind-29 retire anywhere. The witch is therefore not a gameplay actor, and `main.cpp` (~2080) admits it as "script visual actor ... without combat policy ... source combat remains separate" (logged in every reference run). `PlayableActorWorld::eligible_target` (`playable_actor_world.cpp:167`) rejects actors without a combat binding, so Tab cannot select the witch and it cannot be killed.
- Original reference data supports "gameplay": PutCharacterInIdle after spawn (kind 31 in WitchTeleport). The retire comes from the skip-recovery bugfix that does not run in the normal flow (not present in our runs).

## 2. Changes

### Committed (B153)

- `game_save.hpp/.cpp`: `ensure_parent_directory(path, error)`. An empty parent (bare relative save name) needs no creation; otherwise `create_directories` with an error code and a clear message.
- `main.cpp` checkpoint writer: `fs::create_directories(parent)` replaced by `f::ensure_parent_directory(checkpoint_path, e)` (one line; the world save is unchanged).
- `tests/game_save_parent_directory_tests.cpp` (new) and its CTest entry: bare name accepted, nested parents created, retry on an existing parent accepted, blocked parent refused with message.
- `retained_sequence_playback.hpp/.cpp`: `prebind_clip(assets, clip, path, error)`: the first half of `seed` (read and `bind_events`), idempotent, refuses a different path for a bound clip name.
- `combat_session.hpp/.cpp`: `prebind_actor_clip(id, clip, path, error)` for retained actors.
- `main.cpp` after the combat session is created: for each population actor whose profile has script clips, prebind each clip (failures printed as `[campaign] script clip prebind skipped`, not fatal).
- `tests/retained_seeded_completion_tests.cpp`: new case (prebind idempotent, conflicting path refused, seed after prebind succeeds).

### Not committed (B154)

- Attempted: classification change (spawned and not idled after retire is a gameplay actor). The witch then enters the full combat policy and combat initialization throws: `Combat initialization: Selected source phase has no damage marker: Witch/attack_mainhand` (run `fix-direct`, before the B153 fix). Patch kept outside the repo at `C:/dh216/b154-classification-attempt.patch`. Reverted.
- Why the attempt is not admissible: the witch's melee clips contain only `attack_01`/`attack_02` sequence names, while lizard clips contain `attack_mainhand`. The witch's close-range attack is a ranged skill (`DoSkill(0)`), and combat backends reject ranged work. The derived enemy policy needs `Injured` for an incoming-only actor, and the witch's AnimTable 76 has no `Injured` state (see `.local-inputs/claude-preview161/fix068/v20audit/witch-profile.txt`: Idle, Walk, Died, Despawn, Attack pre/attack/post x2, Limbus, Run, Template only).

## 3. Tests

- `game_save_parent_directory`: pass (new, 1 test binary).
- `retained_seeded_completion`: pass (includes the new prebind case).
- Full CTest (worktree build, Release, Ninja): 100% passed, 180 of 180 (`ctest.log`, 494 s).

Note on the CTest run: the worktree has a `.local-inputs` directory junction to the primary's `.local-inputs` (needed by the asset-dependent tests; gitignored; removed at the end).

## 4. Runtime verification

- Hidden-window direct intro, new build with checkpoint fix: 0 "not_saved" lines, checkpoint saved once, three captions, script finished, no combat-init error. Input phase average 0.35 ms per second line (was 5.5).
- Same run with clip prebind: stall at the clip-start frames 297 and 536 reduced from 68/84-98 ms to 45/36 ms. Not eliminated; the remaining first-use cost (camera clip, first sample, actor visual) is not measured.
- B154: runtime investigation only. The E-travel route (`route-ref`, M2 recipe with the slot1 seed) reaches the cave and the hero reaches the witch zone, but the intro trigger never fired. That seed's quest state appears to be past the witch (the c684 seed, with witch states 8/50/52 = 6, is the one that plays the intro).

Runtime-verified: checkpoint retry removal (direct launch), reduced clip-binding stall (partial), B154 classification and its combat-init failure (reference and new build logs).
Unit-only: `ensure_parent_directory` behaviour; prebind idempotence and conflict refusal.

## 5. Remaining gaps (honest)

- B153 torn meshes: not reproduced in any run; the user's screenshot (`...b1c088fe...png`) is the only evidence.
- B153 residual first-use clip cost (35-45 ms at clip-start frames): not located further. Prebinding does not cover camera clips or first-sample costs.
- B153 extreme lag: our hidden-window runs average 16.7 ms per frame in the intro. The user's magnitude is not reproduced; the console (if visible) cost is not measured here.
- B154: the witch cannot be made targetable or killable without porting its combat data: the ranged skill (`witch_ranged_00` + `projectile_dh2_bogwitch`), the witch script (`witch.luac`: timer teleport, `DoSkill(0)`, injury response), and its hurt state (the witch's AnimTable has no Injured). The B154 screenshot named in the brief (`...fd81c5cf...png`) is not present in the attachments folder; the analysis above used the logs and code only.
- The route (E-travel) intro-trigger case needs a seed with the witch quest still open; not produced here.
