# Preview 13 Group I report (wave 2): B043 key-5 potion after MP spend, B044 Rogue Swamp attack marker

Status: implementer report. Both root causes are found with evidence; both fixes are applied in source with focused tests. NEITHER is verified in an integrated EXE (the root must build it). Not closed.

## Setup

- WIP EXE `.local-inputs/windows-source-clock-v19-preview-13-candidate/dh-foundation.exe` (SHA256 AA4442AD...) copied into my own folder `.local-inputs/claude-preview13/i/pkg/` (assets hard-linked, not modified). Saves are copies: `h-rogue.save`, `h-mage.save` (from H's copies of the templates), `k-knight.save` (copy of `.local-inputs/b008-production-profile-matrix/knight-equipped/profile.save`, the only Knight save found with Potion0).
- Scratch: `.local-inputs/claude-preview13/i/{pkg,scripts,negctl,link}`. Shared `windows-foundation-build` was only read (`ninja -t commands`), never built.

## B043: key-5 potion fails after a Celest/Hotty MP spend

### Reproduction (WIP EXE, observed in logs)
| run | sequence | result |
|---|---|---|
| Rogue `r1.args` | key 2 JumpKick (MP 40.25 -> 30.25), key 5 at f60 | `result=1 consumed=1 quantity=5->4 MPraw=7744->10304` (works) |
| Knight `k1.args` | key 2 BashDown (MP -> 21.25), key 5 at f60 | `result=1 consumed=1 quantity=5->4 MPraw=5440->6976` (works) |
| Rogue `r2.args` | key 4 Faery/Celest (MP 30.25 -> 20.25), key 5 at f60 | `result=0 consumed=0 quantity=0->0 ... Source PropertyAdd could not synchronize live actor vital` (FAIL) |
| Mage `m2.args` | key 4 Celest (MP 68.5 -> 58.5), key 5 at f150 | same FAIL |

The HUD shows `Potion: 5` in the failing frames. Mage ColdRay (key 2) did not spend MP in this run (log `phase=0`, MP unchanged), so it is not a usable MP-spend case here.

### Root cause (source + IDA)
- Original `Character::UseMana` (IDA `pseudocode-all.c:138204`, `_ZN9Character7UseManaEi` at 0x3bdef4) calls `CharProperties::PROPS_Add(props, 41, -cost)` after `HasMana`. That is a PropertyAdd, not a direct write.
- The generic skill debit (`features/generic_skills/runtime_skill_cast_prepare_v1.cpp` ~line 249) does the same: `dh2_property_add(&view, 41, -cost)`. Consistent with the Knight/Rogue runs above.
- **Celest** (`features/faery_menu/celest_cast_v1.cpp` ~line 224) and **Hotty** (`features/faery_menu/hotty_cast_v1.cpp` ~line 413) instead write `next_properties.sheets.resolved[41] = old_mp - cost` directly.
- Property 41 (MP) is type 32 (`property-rules.json`: `effective_type 32`, saved-based). `dh2_property_add` for type 32 writes `saved` and then `resolve()` recomputes `resolved` from base+saved (`game-data/properties.cpp:27,34`). A direct `resolved` write is therefore silently reverted by the next resolve. The source sheet keeps the pre-debit MP while the actor/CharacterState hold the debited MP.
- The potion path (`features/inventory/runtime_session_potion_use_v1.cpp:186-190`) then calls `synchronize_live_vital(41,43)`. Its diff from the stale sheet is 0, `dh2_property_add(…,0)` re-resolves to the stale value, and the check `state.resolved[41] != current_raw` fails. That is exactly the logged error. HP (36) is type 32 too but was never written directly, so the HP pass succeeds (this matches the HP_then_MP order seen in the log). Knight and Rogue generic casts are not affected.
- Brief hypothesis "Potion0 ID925 Regen kernel / property sync" is NOT the defect: the potion sync logic is correct for a consistent sheet. The sheet is what is wrong.

### Fix (applied)
- `features/faery_menu/celest_cast_v1.cpp` and `features/faery_menu/hotty_cast_v1.cpp`: replace the direct `resolved[41]` write with `dh2_property_add(&view, 41, -cost)` on the candidate sheet, plus a post-check that `resolved[41] == old_mp - cost` (fails before publication otherwise). Comments cite the IDA function.
- No change to `runtime_session_potion_use_v1.cpp` or `properties.cpp`.

### Tests
- Hotty (`features/faery_menu/tests/hotty_cast_v1_tests.cpp`): new regression assertion after the UseMana debit: a full `dh2::data::recalc_properties` must keep `resolved[41]`.
  - Fixed source: `run_hotty_cast_v1_tests.ps1` -> PASS (output: `Hotty source preparation/query: ... mana debit ...passed`).
  - Negative control: the pre-fix `hotty_cast_v1.cpp` (`git show HEAD:...`, compiled as `.local-inputs/claude-preview13/i/negctl/hotty_old.exe` with the new test) -> exit 1, `Hotty UseMana MP debit did not survive a source property re-resolve`. So the new assertion catches the bug.
- Potion (`features/inventory/runtime_session_potion_use_v1_tests.cpp`): new case after a UseMana-style property-41 debit (generic PropertyAdd, live actor resource re-synced) -> the next potion press must `used`, consume one, and refill MP to max. Runner: `run_runtime_session_potion_use_v1_tests.ps1` -> PASS (`{"validation":"PASS",...,"source_regen_order":"HP_then_MP",...}`).
  - Note: this case passes on the pre-fix code too (it exercises the consistent path); the regression for the bug itself is the Hotty assertion above and the EXE repro.
- Runner housekeeping: `run_runtime_session_potion_use_v1_tests.ps1` contained a UTF-8 arrow (`HP→MP`) without BOM; Windows PowerShell 5.1 mis-parsed it and the runner could not start (pre-existing, not caused by this change). Replaced the arrow with `HP-then-MP` (ASCII only) so the runner runs.

### Knight check (verified, not assumed)
- Knight `k1` run (WIP EXE): BashDown MP spend then potion -> `used`, quantity 5->4, MP 21.25->27.25. Knight is NOT affected by a generic MP spend. Knight has no Celest/Hotty path, so the defect is Faery-specific (Rogue and Mage).

## B044: Rogue Swamp `Selected source phase has no damage marker: RoguePlayerBase/attack_offhand`

### Reproduction
- C-worker args (`.local-inputs/claude-preview13/c-skills/rogue/keys.args`, copied to my folder with relative asset paths) on the WIP EXE: `Source player melee bank class=RoguePlayerBase ... main=664 off=-1` then `Combat initialization: Selected source phase has no damage marker: RoguePlayerBase/attack_offhand` (exit 1, before any HUD). Reproduced.
- Rogue with a saved dual-dagger profile (`r1`, `r2`, startup.args route with `--save h-rogue.save`): no combat-init error; JumpKick and Celest run.
- Swamp route (`swamp.args`, `--fresh-player`): starts the Knight (`KnightPlayerBase`, BashDown), the saved Rogue is not loaded. So "Swamp args + Rogue save" is not a valid Rogue route.

### Root cause
- `main.cpp` ~line 889 (Rogue source melee block, `if(state.class_id=="RoguePlayerBase"&&locomotionLibrary)`) set `policy.damageMarkerNames={"attack_mainhand","attack_offhand"}` UNCONDITIONALLY.
- `combat_session.cpp:1336` requires every listed marker to exist in the selected phase clip. The selected static phase (243) for `main=664 off=-1` is a single-hand clip with no `attack_offhand` marker, so combat init throws.
- The `off=-1` state comes from the args: `--fresh-player` with a single `--combat-main-item Longsword01` and no offhand (`currentLocomotionItems`, main.cpp ~784-800: equipment is read from args when `equipmentStateInitialized` is false). With the saved Rogue equipment (dual daggers), `off>=0` and the failure does not occur.
- Verdict: the trigger is the Knight-style args (Rogue given one hand), but the code defect is real and reusable: the offhand marker requirement ignores whether an offhand is equipped. A Rogue with no offhand item would hit the same failure. Whether a player can reach an empty offhand in-game is NOT verified.

### Fix (applied, main.cpp, one hunk)
- Anchor: `policy.damageMarkerNames={"attack_mainhand","attack_offhand"};` (line ~889, inside the Rogue source-melee block where `hands` is defined at ~867).
- New: `policy.damageMarkerNames={"attack_mainhand"};if(hands.second>=0)policy.damageMarkerNames.push_back("attack_offhand");` with a `// B044:` comment.
- Dual-wield behaviour is unchanged (both markers as before). Only the single-offhand case drops the offhand marker.
- Syntax: `main.cpp` compiled with `-fsyntax-only` using the saved build compile line (`.local-inputs/claude-preview13/h/main-compile-line.txt`, `-o` and `-MF` replaced, output in my folder): exit 0.

### Not verified for B044
- No EXE run of the fix. A private link (objects from the shared build dir, output in my folder) segfaulted for both the current and HEAD main.cpp, and also for the unchanged build-dir object set's current-tree compile, so the shared prebuilt libs do not match current sources. The base link from the shared object set (unchanged main.cpp.obj) runs. This is an integrated-build issue the root must resolve. So "a Rogue swing works end to end after the fix" is NOT shown.
- No focused unit test: the marker list is built in `main.cpp`, not in a testable unit. The change is a one-line rule; its effect was reasoned from the failing branch and the reproduced log.

## Changes (all files)
1. `port/windows-foundation/features/faery_menu/celest_cast_v1.cpp`: UseMana MP debit via `dh2_property_add` + post-check (B043).
2. `port/windows-foundation/features/faery_menu/hotty_cast_v1.cpp`: same (B043).
3. `port/windows-foundation/features/faery_menu/tests/hotty_cast_v1_tests.cpp`: re-resolve regression assertion (B043).
4. `port/windows-foundation/features/inventory/runtime_session_potion_use_v1_tests.cpp`: post-MP-spend potion case (B043).
5. `port/windows-foundation/features/inventory/run_runtime_session_potion_use_v1_tests.ps1`: arrow -> ASCII (runner parse fix, pre-existing).
6. `port/windows-foundation/main.cpp`: ONE hunk at the Rogue source-melee `damageMarkerNames` line (B044).
No saves, candidate folders, tracker, shared build dir, or commits touched.

## Package files required
- No new asset is required by either fix (code-only). Checked in the WIP package: `assets/original-cache/data/...` used by the runs above are present (the runs loaded them).

## Runner results (standalone runners, run in my wave-2 scratch, PowerShell 5.1)

PASS (exit 0):
- `faery_menu/run_hotty_cast_v1_tests.ps1` (includes the new B043 re-resolve assertion)
- `inventory/run_runtime_session_potion_use_v1_tests.ps1` (includes the new post-MP-spend case)
- `inventory/run_inventory_character_pane_tests.ps1`
- `loot/run_runtime_death_rewards_v1_tests.ps1`, `loot/run_runtime_death_drop_pickup_session_v1_tests.ps1`, `loot/run_runtime_loot_source_owner_v1_tests.ps1`, `loot/run_runtime_world_item_adapter_v1_tests.ps1`
- `generic_skills/run_runtime_skill_mana_v1_tests.ps1`, `run_runtime_skill_activation_v1_tests.ps1`, `run_runtime_skill_progression_v1_tests.ps1`, `run_runtime_skill_animation_bank_rank0_v1_tests.ps1`, `run_runtime_tests.ps1`, `run_runtime_skills_text_v1_tests.ps1`, `run_pc_gameplay_hud_v1_tests.ps1`, `run_pc_skill_hud_projection_v1_tests.ps1`, `run_pc_skill_input_binding_v1_tests.ps1`
- `combat/run_target_retention_regression_tests.ps1`, `run_runtime_player_profile_attack_bank_v1_tests.ps1`, `run_runtime_player_incoming_animation_bank_v1_tests.ps1`, `run_b013_attack_continuity_v1_tests.ps1`

FAIL (not attributed to my edits; NOT baseline-checked):
- `faery_menu/run_hotty_session_cast_v1_tests.ps1`: link error, `undefined symbol dh::foundation::effects::is_runtime_source_fx_uri_v1 / read_runtime_source_fx_asset_v1` (the runner's source list does not include the file that defines them). Nothing in `features/effects` is modified in git, so this looks like a pre-existing runner gap. Root should confirm.
- `generic_skills/run_runtime_skill_activation_session_v1_tests.ps1` (exit 1, runner throws "tests failed"), `generic_skills/run_post_skill_attack_regression_v1.ps1` (exit 1), `combat/run_runtime_player_combo_chain_v1_tests.ps1`, `combat/run_b012_death_mid_swing_v1_tests.ps1`, `combat/run_target_facing_regression_tests.ps1` (access violation, exit -1073741819).
  These compile `combat_session`, `actor_state`, `character_state` and related files, which other wave-2 workers have modified but not committed (`git status`: combat_session.cpp/hpp, actor_state.cpp, character_state.hpp, actor_combat_runtime, physics/session_actor_transition, b035 files, runtime_skills_menu_v1.cpp). None of my edited files is in their dependency set. I did not run them against HEAD, so the cause is unproven.
- Missing (names in the list do not exist): `combat/run_tests.ps1`, `combat/run_b037_attack_skill_admission_v1.ps1`.

Side effect: running the runners rewrote tracked JSON reports under `port/windows-foundation/reports/` (`runtime-session-potion-use-v1.json`, `runtime-death-rewards-v1.json`, `runtime-death-drop-pickup-session-v1.json`, `runtime-loot-source-owner-v1.json`, `runtime-world-item-adapter-v1.json`). These are generated outputs; I did not commit or revert them. The root should decide whether to keep or restore them.

## Verifier script (for the integrated EXE)
- B043 (potion after Celest MP spend), run from a copy of the package with a fresh copy of the Rogue profile (`.local-inputs/claude-preview13/i/pkg/h-rogue.save` is a valid template):
  `dh-foundation.exe --startup-config <args>`, where args = `startup.args` + `--save <copy>.save --menu-actions menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER --fixed-step .016 --skill-key-frame 30:4 --skill-key-frame 60:5 --frames 80 --capture pot.ppm`.
  Expected after fix: `Source Faery key=4 frame=30 ... MP=30.25`, then `Source potion key=5 frame=60 result=1 consumed=1 quantity=5->4 ...` (no `PropertyAdd could not synchronize`). Current WIP: `result=0 ... could not synchronize`. Mage: `m2`-style with a Mage save, expected the same.
- B044: args = the C-worker Rogue keys args (`keys.args`) with the asset-root prefix removed. Expected after fix: `Source player melee bank class=RoguePlayerBase ... off=-1`, NO `no damage marker` error, and the run reaches the HUD (`--skill-key-frame` lines execute). Current WIP: `Foundation error: Combat initialization: Selected source phase has no damage marker: RoguePlayerBase/attack_offhand`.
- Also a dual Rogue run (`r1`-style, saved dual-dagger profile) must still pass: JumpKick and potion work.

## Uncertainties / not verified
1. No integrated EXE with either fix. Both need the root's build; the shared build dir objects are not consistent with current sources (see B044).
2. Celest/Hotty path after the fix is verified by unit tests only (Hotty runner + negative control); the EXE run is pending.
3. B044 single-hand Rogue: whether a player can reach an empty offhand in-game is not verified.
4. Mage generic MP-spend in the EXE was not observed (ColdRay did not spend MP in my run). Mage Celest is the observed failing case.
5. The potion test case I added passes on the pre-fix code; it is a guard, not the bug regression.
6. The loot file `features/loot/runtime_death_rewards_v1.cpp` has vital-sync logic but no direct write to resolved[41] (grep); I did not change it. The Celest/Hotty fix removes the stale sheet it would otherwise sync from.
