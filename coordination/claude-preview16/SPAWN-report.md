# SPAWN report (p16/spawn): generic spawn character from template

Status: owner, pool, focused tests and the --spawn-test debug wiring are DONE. The live spawn is BLOCKED by an existing
physics/lifecycle gap (section 5). No spawned actor has been seen spawning, animating or fighting in the live EXE.

## 1. Investigation (evidence)

### Visual (reference video v1.0.3, `.local-inputs/reference-video/dh2-act1/`)
- Contact sheet: `.local-inputs/claude-preview16/spawn/video/intro-sheet-224-228.png` (16 frames, 4 fps from 224.5 s).
- Observed: two green lizard figures come in over the top wall edge of the floor (frames 1-3); one arcs down onto the
  floor (about frames 2-5); both stand and shift near the centre (frames 6-9). HUD and the "Combat Tutorial" overlay
  appear around frames 10-11 (about 226-227 s). SKIP is visible throughout.
- Inference, not observed: the arc is the `lizardman_spawn_jump` clip. 4 fps cannot give exact timing or phase. The
  video is v1.0.3; the recovered source is v1.0.2.

### Logic (IDA pseudocode `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
- `GameObject::_Summon` (line ~108187, 0x39193c). Caller state word must be 3; CharacterTable index is bounds-checked.
  Arg 1 (bool) = spawn. Position: `summon_spot` node if present, else the caller position. When arg 5 is true the
  offsets are applied in the caller's look frame (`GetLookAtVec`); otherwise they are absolute. A floor query falls
  back to the caller position.
- Creation: `Character::CreateNPC`, then `SetInitialPosition`, `SetPosition(pos,1)`, `SetRotation(caller)`; summoned
  flag at npc+5348; `RoomZone::AddInitialObject`, else `ObjectManager::AddNoRoomObject` + `ZoneEntered`.
- Spawn: if arg 1 is true, `CharStateMachine::SM_SetSpawnState(sm,0,0)`, the same SetSpawnState(false,false) that
  Script_SpawnCharacter uses (PreSpawn17 -> Spawn1, Spawn clip, Idle3 after the sequence).
- Not verified: the meaning of the caller state word, the `row+116==1` check, and CreateNPC's own initial state. The pool
  models a created NPC as Limbus/PreSpawn17, which is what the authored intro actors carry.

### Data (shared CharacterTable, decoded with a scratch probe)
- `Swamp_Moth_Minions` (row 370): AnimTable 44, ModelFile 66, LevelMin 256, LevelMax 512. No entry in
  `actor-profiles-v2.xml`, so it has no actor profile. Deriving one from AnimTable 44 is NOT implemented.
- `Swamp_Moth_Type1` (row 371): AnimTable 42, ModelFile 66, Level 256, LevelMin 256, LevelMax 768. Profile present.
- `Swamp_LizadMan_Type1`: profile present. Melee bindings have a `Spawn` state (sequence `LizardMan_Spawn`, step
  `lizardman_spawn_jump.bdae`) and an empty `PreSpawn`. The profile XML has no Spawn state.
- Level fields: absent = -1 (the source tests `monster_LevelMax > -1`); absent LevelOffset = 0.

### Existing owners (port)
- Population: `ActorPopulation::load`, `select_template_profile` (weighted CharacterTemplate draw, caller RNG).
- Session: `CombatSession::initialize` builds ONE session from the population. There is no runtime add API.
- Lifecycle: `OriginalActorLifecycle` (add / spawn / put_idle / put_limbus / animation_finished / combat_enabled).
- Enemy AI: `bindEnemyAI` decision provider over session actors. The combat permission provider gates on the lifecycle.

## 2. Implementation (branch p16/spawn)

- `features/spawn/spawn_character_v1.{hpp,cpp}` (new):
  - Resolve: a CharacterTable row name (direct), or a Charater_Templates row (one weighted draw from the caller's shared
    RNG). The row name is the profile id. Level comes from the row's LevelMin/LevelMax/LevelOffset through the existing
    `resolve_runtime_monster_level_v1` (monster.luac policy). Absent LevelMax (-1) means unscaled.
  - `SpawnPoolV1`: slots admitted before the session, with stable IDs from `pool_stable_id_base`. `acquire` never reuses a
    busy slot; also `release`, `mark_failed`. Transient: pool state is never persisted, and a reload re-declares every
    slot free.
  - `spawn_character_v1`: validate (finite position and heading, positive host level) -> resolve -> profile admitted? ->
    acquire slot -> `place` -> `begin` (clip policy: source_spawn_state = lifecycle spawn; none = put_idle). Exactly one
    log line per attempt (`SPAWN ok|rejected|failed-state ...`). A failure after `begin` keeps the slot busy and failed.
  - `despawn_character_v1`: lifecycle put_limbus, then release. Refused for failed slots.
  - `parse_spawn_test_v1`: `TEMPLATE@X,Y,Z@FRAME`.
- `actor_population.{hpp,cpp}`: `ActorPopulation::admit_declared` admits one declared actor through the same profile
  transform and visual loader as `load()`. It rejects duplicate stable IDs and counts as authored.
- `main.cpp` (small hunks, each marked `P16 SPAWN`):
  - `--spawn-test TEMPLATE@X,Y,Z@FRAME`: repeatable, default off.
  - Before the session is built: each candidate profile that has an actor profile AND a combat policy gets 2 pool slots,
    admitted into the same population (deferred). The level is written to that profile's propertyOptions.
  - `lifecycleSpawnChoice` (function scope): the explicit `--lifecycle-spawn` if given, else the source Spawn state's
    first leaf.
  - Pool actors join the lifecycle as Limbus/PreSpawn17 (`ai_state=Limbus`, `ai_state_visible=0`).
  - Frame hook at the `--spawn-test` frame. Owners: session actor transform, native bodies, the combat RNG
    (`PlayableActorWorld::random_uniform`), and the actor lifecycle.
  - `bindSourcePhysicalTransitions`: PreSpawn17 and Spawn1 bodies take the lifecycle's flags (0x1300 / 0x241) instead of
    throwing "supports normalized Idle/Dead only". Evidence: `OriginalActorLifecycle::change` publishes those flags.
- `CMakeLists.txt`: `foundation_runtime_spawn` (spawn owner + monster level policy) and `spawn_character_v1_tests`.

Bug found and fixed during integration: a lambda local to the lifecycle block was captured by reference by the lifecycle
services and called later (a dangling reference). It is now at function scope. This also fixed the authored intro
replay, which crashed in the same build before the fix.

## 3. Tests (real output)

- `spawn_character_v1_tests <repo>`, using the real CharacterTable, templates, actor-profiles-v2.xml and real visual load:
  `PASS parse=7 candidates=direct+template level=row-scaled pool=unique+capacity admit=deferred+visual spawn=place+clip+one-line rejections=unadmitted,nofree,nonfinite failed-state=kept-busy template=AbbeyCommonType1_Cutlists members=6 despawn=freed` (exit 0)
- `actor_population_template_selection_tests`: `PASS symbolicTemplateRow=93 selectedCharacterRow=361 weightedBound=5 externalDraws=1 unknownConditionGated=true missingRngGated=true` (exit 0, unchanged)
- Build: `p14_build.ps1 -Name p16spawn` exit 0. EXE: `DH_wt/build-p16spawn/dh-foundation.exe`.

## 4. Quiet-batch verification (EXE through quiet_run.ps1, hidden and silent)

Scratch folder: `.local-inputs/claude-preview16/spawn/`. Args come from the Preview 13 b004-P Swamp baseline (v12 assets).

| job | args | result | shows |
|---|---|---|---|
| a240 | `--spawn-test` lizard@40, moth_type1@40, minions@40; 240 frames | exit 0, "Rendered frames=240; clean shutdown" | pool admitted; minions rejected with an explicit reason; lizard and moth spawns refused at begin (section 5) |
| f50 | same; 50 frames; capture `video/f50.png` | exit 0 | scene and HUD render; no spawned actor visible (expected, the spawns were refused) |
| b0 | baseline, no `--spawn-test`; 60 frames | exit 0 | default-off path runs clean |
| c7 | known intro args (`--campaign-command LizardMan_Intro:4:33`, which runs the authored lifecycle Spawn) | exit 0; "Lifecycle state ... previous=17 state=1" | authored lifecycle Spawn path works after the dangling-capture fix (no AI) |

Verifier: `quiet_run.ps1 -JobsFile jobs.json -Parallel 2`, with jobs built by `jobs/mkjobs.ps1` from `jobs/*.args`
(`mkargs.sh <frames> <capture-abs> <out> [spawn lines]`). Expected for a240: `SPAWN pool profile=Swamp_LizadMan_Type1 slots=2`,
`SPAWN pool profile=Swamp_Moth_Type1 slots=2`, `SPAWN profile not admitted ... Swamp_Moth_Minions`, `SPAWN failed-state ...
Legacy lifecycle source program has no admitted physical transition recipe`, then a clean exit.

## 5. Blocker (not fixed; needs an owner for physical transitions)

With AI on (`--enemy-ai`), the physical transition handler is bound. CombatSession then refuses every lifecycle state change
that is not a generic Skill6/Cast7/KnockedBack10 source program:
- `play_actor_source_sequence`: "Legacy lifecycle source program has no admitted physical transition recipe" (combat_session.cpp ~2122).
- `select_actor_state_leaf`: "Legacy lifecycle leaf selection has no admitted physical transition recipe" (~2063).

AI needs floor motion and native bodies, and those bind the handler. So no lifecycle spawn (PreSpawn17 -> Spawn1 -> Idle3) can
run while AI is active. This also blocks the authored LizardMan_Intro under AI. It is already recorded as a production gap in
`runtime-enemy-source-spawn-admission-handoff.json`. Without AI, the lifecycle spawn runs (the c7 path), but nothing fights.

Consequence: spawned lizards and moths do not spawn, animate or fight in the live EXE. Verified only up to the refusal.
The missing piece: admit the PreSpawn17 -> Spawn1 -> Idle3 source transitions (blur, focus, after change) in
`SessionActorTransitionConsumerV1`, with the same expected-state checks the generic states use.

## 6. Limits (plain)

- Not a runtime create. Pool slots are declared before the session is built, because CombatSession has no runtime add.
  Spawns are limited to the 2 slots reserved per test profile.
- Level scaling writes the profile-level `propertyOptions.level_raw`, so authored actors with the same profile get the same
  level. This matches the source monster script, but it was not verified in play.
- Faction and summoner are recorded in the request and plan. They are not wired into combat relations or the actor's faction.
- Despawn is implemented in the owner, but no automatic despawn on death is wired. Save/reload was not exercised.
- `Swamp_Moth_Minions` (the barrel summon) has no actor profile. Its AnimTable 44 clips are not derived, so it is rejected
  explicitly. The barrel `moth_spawn_container` / `GameObject::_Summon` call is NOT wired to this owner.
- No spawn animation frames were captured from the live game, because the spawn never plays. The reference sheet is above.
- Generality is proven at the data level for any CharacterTable row or Charater_Templates row in the shared assets (unit
  test). It is NOT proven visually or live on another map. The Android device folder was not used.
- Unverified: the source caller gate (state == 3), the `row+116` check, and CreateNPC's initial state.
- The B013 lizard level (clamp to 1..3) is applied only when a spawn test is given.

## Package files required

None new. Uses the existing Swamp package (`original-cache/data/pydata`, `actor-profiles-v2.xml`, animation and model
assets). No new assets were added.

## Next steps (for the owner)

1. Physical transition recipe for PreSpawn17 -> Spawn1 -> Idle3 (section 5). Then re-run `--spawn-test` with `--enemy-ai`
   and capture frames 40 to 70 to look at the spawn arc and the first attack.
2. Wire `GameObject::_Summon` to `spawn_character_v1` from the container loader (Lua OnOpen `Summon`).
3. Derive profiles from CharacterTable + AnimTable for rows missing from `actor-profiles-v2.xml` (Swamp_Moth_Minions).
