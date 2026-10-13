# v21.8 mage report (items 4 and 6, Mage part)

Branch `v21.8-mage`, worktree `.claude/worktrees/mage`, build dir `C:/dh218/mage-build`, subst `M:`.
Base: `windows-foundation` @ 93f4823f.

## Summary per item

- **Item 1 (Mage basic staff attack + Tab target, I008): PARTIAL.**
  The staff attack now starts, plays the wand clip and reaches its state-5 projectile marker. Tab target
  selection worked once in a real run and was not reproduced in the final-exe runs.
  - Unit: `combat_system` gained `projectile_delivery` tests (validation, no melee damage at a marker); passes.
  - Real run (baseline, `mage-before1`): zero attack begins, zero markers, zero Tab selections.
  - Real run (`mage-step1c`/`step1d`, `mage-final-src`): attack begins at frames 40-164, marker `do_skill` at state 5 each time.
  - Real run (`mage-step1d`): `Source target command ... selected=<lizard>` once at frame 404. Not reproduced in `mage-final-tab`/`tab2` (timing-sensitive hidden-window runs).
- **Item 2 (shared ranged/projectile executor): WORKED for launch, flight and visual; contact/damage NOT bound.**
  - Unit: `runtime_session_projectile_v1` and `runtime_session_projectile_render_v1` now compile, link, are registered in CMake and pass (they were not in CMake before). Both now use the promoted `load_runtime_session_projectile_tables_v1`.
  - Real run (`mage-final-src`): `launch accepted id=1 row=20 model=elemental_bolt_fire.bdae`, then expiry impact (kind=1), no crash.
  - Real run (`mage-step2i`, captured frames 44-104): the fire bolt is visible in flight at frame 72, separate from the caster.
  - Not done: physical contacts. The three providers (enemy query, owner hit, hit semantics) return explicit errors, so a bolt cannot damage anything. Damage results trigger a loud throw if they ever appear.
- **Item 3 (ColdRay, ChainLightning, StoneEscape, Cyclone via the executor): DID NOT WORK / NOT STARTED.**
  No skill was admitted, no skill was routed through the executor, and no skill was verified. Reasons are listed below.

## Root cause of the item 1 gap (original vs port)

- Original: Staff01 (`equipment_type` 4/5) is a ranged source weapon. Its attack clip `skill_dh2_prince_mage_wand_attack.bdae` has a `do_skill` marker. The state-5 ranged event spawns ProjectileTable row 20 (FireWandProjectile, `elemental_bolt_fire.bdae`, speed 20, timer 1000 ms). `CharAI::melee` redirects ranged owners through `attack_range_redirect` (`level-world/character_ai_attack.cpp:39`).
- Port (before): the frontend "Selected class" block gave every non-Knight class an animation-only profile (`main.cpp` ~1119-1130). `main.cpp` ~7938 then forced attack and target-select off for animation-only profiles. The Rogue-only attack-bank block did not run for the Mage.
- Port gaps found once the Mage joined the attack-bank block: (a) `validate_attack_definition` requires melee damage markers; (b) the `attack_range_redirect` service was unbound (rejected); (c) the bridge classified the state-5 projectile marker and dropped it.

## What changed

- `combat_session.hpp/.cpp`: `CombatSessionProfile::rangedProjectileMarkers` (opt-in). Such an actor binds no melee damage marker and no melee source. Its state-5 `projectile`-kind markers are queued per actor; `CombatSession::take_projectile_markers(id)` drains them after the update. `attack_range_redirect` begins the same source attack only for ranged profiles; other owners keep the rejection.
- `combat_system.hpp/.cpp`: `AttackDefinition::projectile_delivery`. Validation accepts zero markers for it, and `consume_marker` never applies melee damage for it. Covered by `combat_system_tests` (`projectile_delivery_has_no_melee_damage`).
- `main.cpp`:
  - MagePlayerBase joins the existing Rogue attack-bank block with `rangedProjectileMarkers=true`. Rogue behaviour is unchanged.
  - Per frame, queued markers go to `launchPlayerRangedProjectile`: it builds the source inventory (main hand from `options.combat.mainItemId`), runs `runtime_source_range_selection_v1`, and calls `launch_state5`. The executor is then updated each frame. Visuals join the existing projectile-FX path under key domain 0xB156.
  - New option `--projectile-source-assets <dir>` (see below).
  - A refused launch is logged (`Mage projectile launch refused: ...`) and the game keeps running.
- `runtime_session_projectile_v1.hpp/.cpp`: promoted `load_runtime_session_projectile_tables_v1(AssetCatalog, error)`. The test's private copy was replaced by it.
- `CMakeLists.txt`: new `foundation_runtime_session_projectile` (the four `runtime_session_projectile_*` sources plus `runtime_source_projectile_v1.cpp`, `android-native/.../source_process_arrays_v101.cpp` and `game-data/combat_events.cpp`), linked into `dh-foundation`. Two new test targets: `runtime_session_projectile_v1` and `runtime_session_projectile_render_v1`, both pointed at `.local-inputs/runtime-session-projectile-v1/assets`.

## Packaging gap (needs the lead)

- The v21.7 package carries only 55 of the PyData files the ordered source loader needs, and no `data/3D/projectiles/elemental_bolt_fire.bdae`. The complete set is in `.local-inputs/runtime-session-projectile-v1/assets/original-cache` and `.../data/3d/projectiles`.
- Until the v21.8 staging adds them, a Mage run needs `--projectile-source-assets <that assets dir>`. Without it the attack still plays but no bolt is launched (logged as refused).
- Recommended follow-up for the staging script: add the 79 missing pydata files and `elemental_bolt_fire.bdae` to the package.

## Mage test profile (how it was made)

- No Mage seed exists in `C:/dh217/seeds` (all are Knight). I did NOT run the character-creation flow.
- Used the profile file from the existing creation output: `.local-inputs/claude-preview13/verify-play/p12/h-mage.save` (944 bytes, MagePlayerBase), copied to `C:/dh218/seeds/mage/profile.save`.
- Launch: the B026 Mage candidate args (`.local-inputs/b026-mage-current-candidate/run.args`, copied to `C:/dh218/seeds/mage/template-run.args`). They start the menu with `--menu-actions menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER` and `--save <case>/profile.save`. Paths are rebound to the v21.7 package.
- Runner: `C:/dh218/tools/run-mage.ps1` (clicks, frames, captures, extra args; the same `quiet_click_sync.ps1` injector as `run-case.ps1`).
- Test-only: `--test-player-hp,1000` keeps the Mage alive against lizards.

## Real-run evidence (hidden window, real keys)

| Case | exe | begin | marker | launch | Tab | note |
|---|---|---|---|---|---|---|
| mage-before1 | before (pre-change) | 0 | 0 | 0 | 0 | baseline: "outgoing attacks await" |
| mage-step1c | step1 | 1 | 1 | - | 0 | attack and marker reached |
| mage-step1d | step1 | 2 | 2 | - | 1 | Tab selected lizard |
| mage-step2c | step2 (no model) | 2 | 3 | 3 | 0 | launches accepted, model not found |
| mage-step2i | step2d | 2 | 2 | 2 | 0 | frames 44-104 captured, bolt visible at f72 |
| mage-final-src | final | 1 | 1 | 1 | 0 | launch, expiry impact |
| mage-final-nosrc2 | final | 3 | 3 | 0 | - | refused x3, no crash |
| mage-final-tab / tab2 | final | 1 / 0 | - | - | 0 | Tab not reproduced |

## Tests

- Full CTest on the final branch state (before the last launch-refusal change, which is `main.cpp` only): **189/189 passed**.
  Registered tests total 189 (HEAD had 187 by subtraction of my two new projectile registrations). The coordination matrix's "184" is stale.
- `combat_system` (new test case) passes. `runtime_session_projectile_v1` and `runtime_session_projectile_render_v1` pass.
- Gap: no unit test covers the session-level queue and `attack_range_redirect` path. It is verified only by the real runs above.

## Item 3: why not done

- The skill cast whitelist (`generic_skills/runtime_skill_cast_coordinator_v1.cpp:115-145`) is owned by skills-a. I added no admission.
- The skill casts are state-6 `do_skill` markers. Those are classified as `skill` kind, not `projectile`, so they do not reach the new drain.
- Per the matrix, ColdRay is rejected by the coordinator, and ChainLightning and StoneEscape have no linked cue or FX.
- **Merge point for skills-a:** the drain in `main.cpp` after `combatSession->update` (the `take_projectile_markers` loop calling `launchPlayerRangedProjectile`). Skill projectiles would need a state-6 sink plus the skill row (Cyclone: `projectile_dh2_prince_mage_cyclone.bdae`; ChainLightning: `projectile_dh2_chain_lightning.bdae`, both staged in v21.8).

## Audit notes (for the later audit; not touched here)

- Witch: `enemy_ai/runtime_ranged_skill_monster_v1` keeps its own projectile list and draws under 0xB154. It is not routed through the shared executor, as instructed.
- Mud Lord: `bosses/runtime_swampking_core_ai_v1` (branch-only) keeps its own projectiles under 0xB155. Not routed.
- The shared executor still has no contact owner; any physical-contact work will need the three providers bound.

## Known gaps / inferences (marked)

- Inference: the `attack_range_redirect` implementation begins the same source attack (`request(requested)`). I inferred this from the control flow of `CharAI::melee`; I did not verify the body of the original redirect callee in IDA.
- Inference: projectile origin is the actor position with source heading (`{sin h, -cos h, 0}`, the same convention as `enemy_ai::source_look_vector_v1`). The original uses the live projectile node when present; not done.
- Gap: the staff attack keeps the melee reach as `maximum_range`. The ranged band is not used for approach or range gating.
- Gap: the Tab selection is not reproduced in the final runs.
- Gap: no `RuntimeSessionProjectileLaunchV1` tests with the actual Mage fixture in the session test; the existing fixture uses an animation-only player.

## Cleanup

- Junctions: 929 junctions to primary `.local-inputs` directories, plus about 4,040 hardlinks of primary root files, all created in `.claude/worktrees/mage/.local-inputs` (list: `C:/dh218/mage-junctions.txt`). Junctions removed with `cmd /c rmdir` (no recursive delete, all verified as reparse points); hardlinks unlinked (primary files unaffected).
- `subst M:` removed.
