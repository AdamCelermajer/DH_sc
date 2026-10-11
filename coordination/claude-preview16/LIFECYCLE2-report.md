# LIFECYCLE2 report (p16/lifecycle2, from p16/lifecycle 0fb95dc0)

Status: PARTIAL. Done and verified: engagement evidence (no registration gap found), declared authored spawn (LizardMan_Intro
stub trigger), despawn with physical release and pool slot reuse, the motion-phase core change reviewed with a test, and the
20-job regression A/B. Not done: general profile derivation for `Swamp_Moth_Minions` (item 4), automatic death-to-despawn,
and a producer for the Limbus (state 0) transition receipt.

## 1. Investigation

### 1a. Engagement of spawned actors (item 1)
- Method: a temporary, env-free trace in `RuntimeEnemyControllerV1::update` printed, every 30 frames, each monster's
  position, distance to the player, aggro/target, view radii, eligibility, sight and route. Removed before this commit.
- Base spawn run (`base-spawn.args`, `--enemy-ai`, spawn at frame 40, 240 frames), pool lizard `...768`:
  - frames 61-121: `state=1` (Spawn clip running), not aggroed, `inside=1` (within radius), so no acquisition yet;
  - frame 151: `aggro=1 target=player route=2 (attack) action=2`; the player takes hits from `...768` at frames 138 and 168.
  - Pool moth `...770` aggros at about frame 121 and hits at frame 139.
- Frame `lifecycle2/c130.png` shows the player fighting with the spawned lizards around, and `c100.png` shows Bogwomp plus
  a spawned lizard.
- Finding: spawned actors use the same path as authored ones (admitted to the population and session before the session is
  built, so they appear in `sourceBodyPlans`, `bindEnemyAI`, native bodies and the enemy controller id list). The eligibility
  gates (targetable, `lifecycleOriginalState`) clear at Idle3. The "stays idle" reading in wave 1 came from sampling frames up
  to 80 while the Spawn clip lasts about 80 frames (Spawn 40 to Idle3 about 124). Not reproduced in these runs: an idle
  spawned lizard. Possible other cause if the user still sees it: spawn farther than the view radius (1500), which is correct
  behaviour, or a different level/position. Not established.
- Note: the spawned lizard attacks from about 176 units (melee reach is 200 by `original_melee_in_range`). The authored lizard
  does the same from about 175 units, so this is not spawn-specific.

### 1b. Spawn arc vs reference (item 3, visual)
- Reference `spawn/video/intro-sheet-224-228.png` (4 fps): lizards enter over the top wall edge; one arcs onto the floor.
- Ours (`lifecycle2/sheet-intro.png`, frames 62/70/80/92/110/130, player at -3600,300,250): the first authored intro lizard
  appears mid-air near the wall at frame 110; the second is on the floor at frame 130. Our arc is the Spawn clip's own motion;
  no arc is added by the port. A 4 fps sheet cannot confirm timing, so this is observation only, not a timing match.

### 1c. LizardMan_Intro declarations (item 3)
- `original-campaign.xml` script 17 `LizardMan_Intro`: Script_SpawnCharacter(`_prim_Monster_LizManIntro1`) at t=0.5 s, wait
  1.5 s, Script_SpawnCharacter(`_prim_Monster_LizManIntro2`), wait 2 s, unlock, camera back, DoTutorial CombatTuto.
- Declarations in `original-cache/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp`: `charpropsname=Swamp_LizadMan_Type1`,
  `ai_state=Limbus`, `auto_spawn=0`, `ai_state_visible=0`. Without `--retain-hidden-actors` they are "Condition policy returned
  unknown; actor not instantiated" (wave-1 log). The lifecycle preset Limbus starts them at PreSpawn17 (the lizard's PreSpawn
  state is empty, so no PreSpawn animation; `freeze_animation_speed` applies).

### 1d. Limbus (state 0) and death (item 2)
- IDA `CSLimbus::OnBlur` (0x3c2be4): vtable+64(actor,1), SetPosition/SetRotation to the initial anchor, Revive, and limbus group
  bookkeeping. `CSLimbus::OnFocus` (0x3c2e58): flags328=0, vtable+64(actor), respawn timer when `Character+1328` and a delay
  exist, AI_ClearAllAggro. CSLimbus does NOT call SetPhysicalObject or DisableCollisions (CSPreSpawn does, lines 143157-143200).
- Wave-1 `put_limbus` ran none of the physical release, so a despawned pool body stayed in the native world. Port decision
  (labelled in code and here, not a source fact): the Limbus branch releases the physical receiver and disables collisions.
- Death (state 12) is not removed automatically. The lizard/moth bindings have no Despawn state (only `BigLizardMan_Despawn` in
  AnimTable), so the minion removal after death is not verified. Gap.

### 1e. Profile derivation for Swamp_Moth_Minions (item 4): NOT DONE
- Admission needs three things that are authored, not derived: (a) an `ActorProfile` (`actor-profiles-v2.xml`: model,
  propertyRow, animationTable, template, clip lists per state), (b) `OriginalMeleeBindings` sequences (`original-melee-bindings.xml`:
  per-state sequences with clip timing and markers), and (c) the combat policy. The offline exporter
  (`tools/export_actor_profiles.py`) takes audited JSON. I found no runtime producer that derives (a) or (b) from CharacterTable
  row 370 and AnimTable 44 for monsters on the Windows path. Minions still prints "profile not admitted".
- Suggested next step: derive (a) and (b) from the tables and validate by regenerating `Swamp_Moth_Type1` (row 371, AnimTable
  42 "Moth") and `Swamp_LizadMan_Type1` (row 360, AnimTable 34) and diffing against the authored XML before admitting Minions.

### 1f. Core change review: set_motion_phase_handler (item 5)
- Change (wave 1): the `!pendingMotion.empty()` guard is removed from `set_motion_phase_handler`.
- Why it is safe: queued samples are keyed by ActorId and carry only delta and `enabled`. `deliver_motion_phase` looks the actor
  up at delivery and moves the batch out before the callbacks. The handler is passed by value. The per-frame handler in main.cpp
  captures the same owners every frame. `updating`, `transitionDelivering`, `restoreTeardown`, `checkingAnimationCheckpoint`
  still refuse. `clear_motion_phase_handler` and `source_checkpoint` still refuse while samples are queued.
- Residual risk: a handler that swaps owners would deliver old samples to the new owner. No caller does that today. Other
  rebinds (animation notifications, actor transitions) still refuse with pending samples, so a checkpoint or rebind in the same
  frame as a spawn is refused. Documented; not changed here.

## 2. Implementation (this commit)
- `features/spawn/spawn_character_v1.{hpp,cpp}`: `parse_spawn_named_v1` (NAME@FRAME), `spawn_declared_v1` (wakes an
  authored declaration only from PreSpawn17, one `begin` with source Spawn clip, no pool slot).
- `main.cpp`: `--spawn-declared NAME@FRAME` (implies `--retain-hidden-actors`; the named declaration only is admitted to the
  lifecycle; a missing name or non-lifecycle declaration logs a rejection); `--despawn-test NAME@FRAME` (pool slot by population
  name, through `despawn_character_v1`). Both default off.
- `original_actor_lifecycle.cpp`: state 0 emits `remove_physical` and `set_collisions_enabled(false)` after set_flags/set_enabled.
- `features/physics/session_actor_transition_v1.cpp`: Limbus cases 0 for Blur (validate), Focus prefix (flags 0), Focus suffix
  (validate). Inert until a receipt producer exists (see gaps).
- `tests/combat_session_motion_phase_tests.cpp`: new block. A non-looping source sequence (player `Injured`, group {0}) is played
  between updates through the same `play_actor_state_sequence` path the lifecycle spawn uses. It queues samples: `clear` refuses
  ("without pending samples"), the per-frame rebind succeeds, and the next update delivers the queued samples to the rebound
  handler.
- `features/spawn/spawn_character_v1_tests.cpp`: named-request parser (valid, malformed), declared spawn refused from Idle3 with
  zero owner mutation, accepted from PreSpawn17 with one begin, begin failure reported, pool reuse (the despawned slot is handed
  out again with the same actor ID, busy count restored, both released at the end).
- Temporary engagement trace: added and removed (no trace in the committed diff).

## 3. Isolated tests
- `p14_build.ps1 -Name p16lifecycle2 -Test` build exit 0; ctest 114 of 115 pass. The only failure is `session_skill_binding`
  (the known junction issue in worktrees). Relevant tests pass: `spawn_character_v1`, `combat_session_motion_phase` (new block),
  `combat_session_actor_transition`, `session_actor_transition_physics`, `original_actor_lifecycle`.

## 4. Integrated runtime verification (quiet, hidden, silent; EXE `build-p16lifecycle2`)
Scratch folder: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview16/lifecycle2/`.
- `scen` (`scen-base.args`, 240 frames): `SPAWN ok` pool lizard `...768` (frame 40) and moth `...770`; `SPAWN declared ok
  name=_prim_Monster_LizManIntro1 actor=...923` (frame 60) and `..._LizManIntro2` (frame 120) -> Spawn -> Idle3; at the end both
  intros `state=3 enabled=1 physical=1 collisions=1`. `SPAWN despawn actor=...768 busy=1/4` at frame 150 (state 3 -> 0, then
  `physical=0 collisions=0`). `SPAWN ok ... actor=...768 ... busy=2/4` at frame 170 (same actor: slot reused; state 0 -> 1).
  `Rendered frames=240; clean shutdown`, no `Foundation error`. Minions remains `profile not admitted` (item 4).
- `near` (`near-scen.base`, player at -3600,300,250, 200 frames): declared spawns ok; frames 62-130 captured (`sheet-intro.png`).
- Regression A/B (`regress/`): 20 jobs from `claude-preview13/verify-final` (smoke-R, b037-hold50/36/rel8/rel20, b004, b041 x3,
  kxp x3, hit-13/17, rogue-potion, b044-rogue-swamp, eq-torso/weap/bag, skills), run with the wave-1 EXE (`regress/wave1`) and the
  new EXE (`regress/wave2`):
  - Byte-identical captures (md5) for 15 jobs, including all B037 jobs, b004, b041, kxp, hit, b044 and skills.
  - Different captures for smoke-R, eq-torso, eq-weap and eq-bag. These run without `--fixed-step`, and the wave-1 EXE run
    against itself also differs (`regress/wave1-rerun`: smoke md5 4d2e883f vs 231a3a8d, eq-bag 182078d1 vs 627d9a51), so they are
    wall-clock nondeterministic, not a regression signal. Their log differences are audio outputFrame, frame-rate and animation
    lag jitter.
  - `rogue-potion-R` exits 1 in both builds ("Source button is not active on the current menu"): a pre-existing harness/menu
    issue, unchanged by this work.
  - Smoke batch: this job has no PASS lines; its capture is nondeterministic (above).

## 5. Gaps (honest)
1. Item 4 not done: `Swamp_Moth_Minions` still "profile not admitted" (section 1e).
2. Automatic despawn after death not done; the only despawn is `--despawn-test` (a stub trigger).
3. Limbus receipt producer not done: `put_limbus` runs through the lifecycle without a session receipt, so the new consumer cases
   for state 0 are reachable only once a producer exists; they have no receipt-level test. The physical release on Limbus is a
   port decision (1d), not verified against the source's physical behaviour.
4. Campaign `Script_SpawnCharacter` is not bound to these owners; `--spawn-declared` is the stub trigger the cinematic runner will
   replace.
5. Spawn arc timing and the top-wall entry are not verified frame-by-frame (the reference is 4 fps).
6. Engagement: the wave-1 "idle" was not reproduced; the reason is recorded in 1a, but the user's configuration is unknown.

## Placeholders
- None. No art or HUD was added. `--spawn-declared` and `--despawn-test` are debug triggers, default off.

## Package files required
- None new. The runs use the existing preview-12 and preview-13-rc2 assets.

## Verifier script
- `./mkjob.sh scen scen-base.args 240 && ./run.sh scen`: expect the `SPAWN declared ok`, `SPAWN despawn actor=...768`, the
  reused `SPAWN ok ... actor=...768` after frame 170, and the `Lifecycle final` lines above.
- `./mkjob.sh near near-scen.base 200 && ./run.sh near`: expect the declared-ok lines; frame captures with `./mkjob.sh n110
  near-scen.base 110`.
- Regression A/B: `regress/all.json` (wave1 + wave2, 40 jobs) through `quiet_run.ps1 -Parallel 12`; compare `regress/wave1/*` and
  `regress/wave2/*` capture md5s as in section 4.
- The debug build for wave 1 is `DH_wt/build-p16lifecycle`; this build is `DH_wt/build-p16lifecycle2`.
