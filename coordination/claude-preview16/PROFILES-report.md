# PROFILES report (p16/profiles, from p16/lifecycle2 3f553674)

Status: item 4 (general profile derivation, admission of `Swamp_Moth_Minions`) DONE and verified in the EXE for
spawn, idle, engagement, hit and kill. Item 2 (automatic despawn after death) NOT implemented: investigation only,
with one unresolved transition (section 5).

Commits: `e23fbb4a` (derivation module + validation), `f20d1c54` (spawn-test admission).

## 1. Investigation

### 1a. Where the authored data comes from (tables, not XML)
Derived fields were matched against the authored XML, so the mapping below is verified by the validation in section 3, not
assumed. Table files are read through the runtime content path `data/pydata/`.
- CharacterTable (`character_properties_*`): row index = `propertyRow`; field `AnimTable` = `animationTable`; `AnimTable` name
  = `animationTableName` (CharAnim character names, e.g. 42 `Moth`, 44 `MothMinion`); `ModelFile` = index into
  `character_models_dictionary` (value = model URI); `AI` = AI row (`aiId`, name = `ai` names[AI]); `AIFaction` = `factionId`.
  All 224 raw words are the profile's `property` entries, same order and values as authored (encoding: `Scale_X/Y/Z` =
  `original-character-scale`, all others `original-table-raw-integer-uninterpreted`, as authored).
- CharAnim (`animations_*`): for an AnimTable row, the state's sequence ids. Profile clip list = leaf clip URIs of those
  sequences, flattened through redirect steps, unique in first-occurrence order (`animations_dictionary` gives the URI).
- Melee: the same sequence ids nested as authored (`sequence` id/name/loop/type; `step` index/animationId/redirect/speed/
  blendOut/moveGO; redirect steps nest the referenced sequence; leaf steps carry the URI).
- AI row: `AttackDelay`..`ViewRadiusNoAggro` fields from `load_ai` (`AiProps`).
- Minion: CharacterTable row 370 `Swamp_Moth_Minions`: AnimTable 44 `MothMinion`, AI 41 (`Moth`), faction 7, model moth.bdae,
  the same as the authored Moth except Spawn: sequence 454 `Moth_Urn_Spawn` (clip `moth_urn_spawn.bdae`) vs 451 `Moth_Spawn`.

### 1b. Consumer state sets (contract, not invented values)
- Profile states: Template, Idle, Walk, Run, Attack, Died, Limbus (exactly the authored profile state set).
- Melee states: Attack, AttackStatic, Died, Idle, Injured, PreSpawn, Run, Spawn, Walk (exactly the authored melee set).
- Other CharAnim states (Blocking, Dodging, Stunned, Scared, KnockedBack, Despawn, ...) are NOT published. Authored files
  omit them; they stay in the tables. Despawn is the one that matters for item 2 (section 5).

### 1c. Combat policy (what the CLI supplied per enemy)
The authored enemies get their policy from CLI flags (`--combat-sequence X:Attack:0:0`, `--combat-idle`, `--combat-react`,
`--combat-death`, `--combat-marker X=attack_mainhand`, `--combat-retained-phase`, `--combat-motion-root X=auto`). The
derivation sets the same values: Idle/0/{0}, Injured/0/{0}, Died/0/{0}, Attack/0 group {0}, `attack_mainhand`,
retained phase clock, motion root auto, allow missing animation targets, refill vitals. The Attack group {0} is the authored
choice (the `0:0` leaf), not an IDA selection rule. Unverified: which Attack group the original picks for enemies.

### 1d. Original behaviour checked
- Spawn: summon from the urn/barrel container (`CONTAINERS-survey.md`: `moth_spawn_container` = 25% Summon(Swamp_Moth_Minions)).
  The Spawn clip is the urn spawn (`moth_urn_spawn.bdae`), which is what the derived Spawn sequence plays.
- Visual reference: NOT established. The 30 s contact sheet of `.local-inputs/reference-video/dh2-act1/...mp4` (duration
  1331.6 s, `profiles/ref-sheet-30s.png`) shows swamp combat but I could not identify a moth/urn frame at that resolution.
  Minion visuals are therefore compared only to the authored moth data, not to footage. Open.

## 2. Implementation
- `features/spawn/actor_profile_derivation_v1.{hpp,cpp}`: `load_profile_derivation_tables_v1`, `derive_actor_profile_v1`,
  `derive_melee_actor_v1`, `derive_enemy_combat_policy_v1`, state-set contract.
- `actor_profiles.{hpp,cpp}` `add_derived`, `original_melee_bindings.{hpp,cpp}` `add_derived_actor`: publish derived entries;
  duplicates rejected, authored entries always win.
- `main.cpp` (spawn-test block, small hunks): a candidate CharacterTable row with no authored profile or policy is derived
  (profile into `profiles`, policy into `options.combat.profiles` with `diagnosticAIEnabled` as the CLI path does). In
  `initializeCombat`, after the melee XML loads, the derived melee actor is published. Log lines: `SPAWN profile derived
  from tables`, `SPAWN melee bindings derived from tables`, `SPAWN profile not derivable`.
- Inert for runs without `--spawn-test`: verified byte-identical (section 4).
- Not changed: spawn owner, lifecycle, combat session, population (the spawn owner's own unit test still builds a fixture
  without derivation, so its `Minions profile not admitted` expectation stays valid for that fixture).

## 3. Isolated tests
- `actor_profile_derivation_v1_tests` (ctest `actor_profile_derivation_v1`, passes): regenerates all 25 authored profiles and
  all 25 authored melee actors from the tables and diffs every field: `VALIDATION profiles=25 melee=25 compared=17196
  differences=0`. Output: `.local-inputs/claude-preview16/profiles/validate1.txt`.
  - Profile: id/character, model, template, propertyRow, animationTable(+Name), each state's clip list and count and URI, all 224
    raw properties and encodings.
  - Melee: propertyRow, factionId, aiId, aiName, model, template, all `ai` attributes, the state set, every sequence
    (id/name/loop/type) and every step recursively (index/animationId/redirect/speed at float precision/blendOut/moveGO/uri).
  - Not compared (no derivation yet): `<clip>` timing and markers (`startMs/endMs`, marker lists), factions, relations.
  - Zero DIFF lines, so there is no per-field IDA reason to record. A negative control was not run (the diff is a plain
    equality check that prints on mismatch).
- `ctest`: 115 of 116 pass; only `session_skill_binding` fails (the exempted junction issue).

## 4. Integrated runtime verification (quiet, hidden, silent; EXE `build-p16profiles`)
Assets: `profiles/assets-run` = copy of `windows-source-clock-v19-preview-15-rc3/assets` plus `moth_urn_spawn.bdae` and
`moth_despawn_01.bdae` (from `.local-inputs/assets-extra/android`), see section 6.
- `m1` (240 frames, `minion-base.args`, spawn test Minions at -6750,1100,250 frame 40): `SPAWN profile derived from tables:
  Swamp_Moth_Minions animationTable=44 states=7`; `SPAWN melee bindings derived from tables`; `SPAWN ok template=Swamp_Moth_Minions
  ... actor=...772 ... busy=3/6`; `Lifecycle state ... previous=-1 state=17` -> `1` (Spawn) -> `3` (Idle); `Rendered frames=240;
  clean shutdown`. Lizard and authored Moth still spawn in the same run.
- `a-batch` (`atk-base.args` = m1 + `--attack-start-frame 60 --attack-frames 300`; captures at 120, 200, 400):
  - Engagement: the minion attacks the player (`Damage ... attacker=...772 target=18446744073709551615` = player id `max`,
    `removed=5.67` at frames 97 and 214). The same pattern appears for the authored Moth and lizards.
  - Hit and kill: player hits on the minion at frames 190, 223, 255, 287 (`removed` 15.0, 0, 14.8, 6.2); `Source death reward
    frame=287 victim=...772 xp=9`; `dead=1`; `Actor final ... HP=0/36`.
  - Frames: `a120.png` (minion visible with wings beside the player, the player fighting), `a200.png` (minion labelled
    "Muck Fly" with red damage numbers; the potion is from an earlier death at frame 173, another actor), `a400.png` (minion dead, lying on the floor,
    gold and potion). `s80.png` (frame 80): the minion visible with wings spread near the player.
- Observed visually: spawn, visible flying/idle moth, attacks, damage numbers, death. Not established: the flight motion is
  the authored Idle/Walk clip playback (not compared to footage), and the minion does not despawn (section 5).
- Non-spawn A/B (`ra-new` vs `ra-old`, 120 frames, same assets, EXE `build-p16lifecycle2` vs `build-p16profiles`):
  identical capture md5 `8c121824...` and identical log length (480 lines).
- Without the two package files the run fails: `Foundation error: Combat initialization: Original resource not found:
  data/3D/characters/moth/animations/moth_urn_spawn.bdae` (`pk.log`).

## 5. Item 2: automatic despawn after death (NOT DONE; investigation only)
IDA (`pseudocode-all.c`):
- `CSDead::OnEvent` (0x3c4c3c), event 34 (end of the death animation): `SetPhysicalObject(nullptr)` (body released); if
  `!(vtbl+40)(character)`: `TMR_Start(Despawn_Delay, event 46)`, flags328 = 64.
- `CSDead::OnFocus` (0x3c4d50): flags328 = 577; when the anim is not pending (`+314 == -1`) and `!(vtbl+40)`: the same timer.
- `Despawn_Delay` = 2000 (`design_pycst.bin`, CharacterDesign). `Despawn_Speed` = 2000 (not read by any function I traced).
- `CharTimers::Update` (0x3db640) raises the timer's event on the character; event 46 has no case in `CharAI::RaiseAIEvent`
  (0x3cbb34), so it reaches `CharStateMachine::RaiseStateEvent` and the current state (CSDead), which does not handle 46.
  **Unresolved:** the transition from Died to the Despawn state on event 46 is not in these functions. It is probably table-
  driven (state chart) or in a base-class path; I did not recover it.
- `CSDespawn::OnFocus` (0x3c32fc): flags328 = 512, `SM_SetAnim(-1)`, `CancelSneaking`. The Despawn clip is AnimTable
  `Despawn` = sequence 443 `Moth_Despawn` (`moth_despawn_01.bdae`, Android-only, see section 6).
- `CSDespawn::OnBlur` (0x3c3794): `if (!CanRespawn && ...)`: clears the local player's target; `if (a3+5348 || IsSummoned(a3))
  ObjectBase::Delete(a3)`. So summoned minions are removed when the Despawn state ends; non-summoned actors are not deleted
  by this path.
- `VisualObject::StartFadeOut/UpdateFadeOut/StopFadeOut` (0x470cd8..) are empty in this build. `Visual_AlphaFadeOutDespawn`
  (= 3000 for the moth, a raw table value) has no reader I could find. No fade-out is applied by the original code I read.
- Port state: the only despawn is the owner `despawn_character_v1` (hide via put_limbus, release slot). Nothing triggers it
  after death, so the minion stays lying on the floor (`a400.png`).
What is needed to finish: (1) recover the event-46 transition owner; (2) start Despawn_Delay at the death-anim end (anim-end
event 34 is already logged for actors); (3) play the Despawn sequence; (4) delete summoned actors (`despawn_character_v1`) and
keep non-summoned ones per the recovered rule; (5) verify in a quiet run with frames.

## 6. Package files required
- `moth_urn_spawn.bdae` (Minions Spawn clip; required, the run fails without it) and `moth_despawn_01.bdae` (Despawn clip; needed by
  item 2). Both are staged non-destructively in `.local-inputs/assets-extra/android/data/3d/characters/moth/animations/`
  (copied from the Android device folder; the staged copies were already present). Package root to receive them:
  `original-cache/data/3d/characters/moth/animations/` (the lowercase device path). Not copied into any accepted package here.
- Runs use `profiles/assets-run` (a copy of the rc3 assets with those two files added).

## 7. Gaps (honest)
1. Item 2 (automatic despawn) not implemented (section 5); the event-46 transition owner is unresolved.
2. Clip timing and markers (`startMs/endMs`, markers) are not derived; the attack path reads visual markers at runtime, and only
   the troll-return and companion code reads melee clip metadata. Minions do not need it. Logged, not derived.
3. The Attack group is the authored `0` default (`group_path {0}`), not an IDA selection rule.
4. Minion visuals not compared with footage (reference timestamps not located); flight style unverified.
5. Derived profile needs every clip of its state set in the package. A missing clip stops the run ("Original resource not found"),
   by design, not silently skipped.
6. Derivation runs only in the `--spawn-test` path (the population and the campaign `Script_SpawnCharacter` do not use it yet).
   The spawn owner's own unit test still models the unadmitted case with its fixture.
7. `session_skill_binding` ctest failure: known junction issue (exempted).

## Placeholders
- None. No art or HUD added. The attack-group default and the state sets are documented contracts, not placeholders.

## Verifier script
- Validation: `build-p16profiles/actor_profile_derivation_v1_tests.exe <worktree> <assets-root>`; expect `VALIDATION ... differences=0`.
- Minion run: `profiles/mkjob.sh m1 minion-base.args 240 && profiles/run.sh m1 1`; expect `SPAWN ok template=Swamp_Moth_Minions`, `Rendered frames=240`.
- Engagement/kill: `profiles/mkjob.sh a400 atk-base.args 400`; expect `Source death reward ... victim=...772` and `Actor final ... HP=0/36`.
- Regression: `ra-new` vs `ra-old` capture md5 equal.
