# CONDITIONS report (Preview 17)

Branch `p17/conditions`, worktree `DH_wt/p17conditions`, started at `261f2e49` (p16/integrate at start). Commits: `39de2b53` (evaluator + flip watch), `b3a722bb` (population, watch, save/reload wiring), `5f74795c` (gated declarations as world objects), `671aafb7` (cleanup), plus this report. Not pushed.

## Result

Delivered and verified in the real EXE on Swamp:
- One table-driven evaluator for authored v2Conditions (IsAfter_/IsBefore_/IsBetween_/quest-state names), with the IDA evaluation rules. Unknown names are logged once and fail with an error.
- Population uses it for `activate_cond`/`deactivate_cond`. On the 40-args levels batch, 143 of 232 unique declarations that returned "Condition policy returned unknown" are resolved; the remaining 89 are not conditions (80 `auto_spawn="0"`, 6 `spawn_prob`) or are authored names missing from the table (3).
- Event-driven flips: a quest state transition marks dependent conditions dirty, and only dirty conditions are evaluated. `IsAfter_Swamp_KillWitch2` flips false to true when the Witch quest closes; `IsBefore_Swamp_KillWitch` flips true to false in the same event.
- World state: each gated declaration (Door, AnimatedDecor, TriggerObject, QuestMoveInZone, zones) is a neutral world object with its authored stable ID. Component `dh2.world-gate.v1.<key>` (1 byte) stores the value its owner last applied. It rides the existing GameSave object roster; no schema bump. A fresh process loaded from the save restores these values with no replay.

Not delivered (see Gaps): door and AnimatedDecor visuals and physical state, `Script_OpenDoor`/`CloseDoor` execution, any waterfall (none exists in the Swamp data), and the reference-video comparison for doors. The gate owner is a logging stub (`owner=none (not wired)`).

## Investigation (evidence)

- Table format: `original-cache/data/pydata/v2conditions_pyarray.bin` = u32 row count, then per row `u32 count, count x (u32 op, u32 argument8, u32 argument_c), u32 type`. Names are u32 count, then u32 length + bytes, 118 entries. Decoded by `NativeConditionTableV69` and cross-checked by an independent parse of the bytes; both match row for row.
- Evaluation rules, IDA `libDungeonHunter2.so/pseudocode-all.c` `00478fe8 Condition_IsQuestInStateGeneric::Eval`: no local character or no quest returns false; `op 0: argument_c == state`, `op 1: argument_c > state`, `op 2: argument_c < state`. `ConditionList::Eval` (478754) short-circuits on the first false stub; an empty list is true. Factories 3 (IsPlayerInLevel), 4/5 (literal true), 6 (event state) follow `native_conditions_runtime_v69.cpp`; the Swamp table uses only ops 0-2.
- `IsAfter_Swamp_KillWitch2` = op 0 on quest 52 (`Swamp_KillWitch`), value 13 (`post_closed`). `IsBefore_Swamp_KillWitch` = op 1 on quest 52, value 13. The quest runtime reaches `post_closed` on its own after the witch kill (Completed to PostCompleted to PreClosed to Closed to PostClosed), so the flip happens at the kill.
- Authored names not in the table (logged once, actors stay unknown): `IsLowerThanDW_Phase_3`, `IsDuring_Darkwood_Ring`, `IsBefore_FireTemp_GoToHellgate`. The table has `IsDW_Phase_3` and `IsBefore_Firetemp_to_Hellgate`, which are different names.
- Waterfall: no waterfall-named object or data exists in the original cache, the Android staging, or the coordination notes. The only Swamp AnimatedDecors with `activate_cond` are `_anim_swamp_dustmotes_swamp2` (`IsAfter_Gothicus2Survivors`, merchantcamp) and `_anim_destructible_wall_001` / CS_wallbreak (`IsBefore_Swamp_Escape`, bossroom). Neither is witch-gated. The waterfall recollection stays unverified, as the progression-barriers report already recorded.
- Door: `_prim_SwampDoor_FIRE` (Door, `data="Swamp_Door_Ruins_FIRE"`, `opened=0`, `activate_cond="Invalid"`) and `_prim_SwampDoor`. Their game-object rows are in Android `game_objects_pyarray.bin`, which is not in the worktree cache. `TriggerObject _prim_TriggerObject` has `unlock_cond="IsAfter_Swamp_KillWitch2"` and runs `open_SwampKingDoor` (Script_OpenDoor on both doors).
- Visual evidence (AGENTS.md item 1): the reference video was NOT inspected for doors or the waterfall in this session. No visual claim is made here.

## Implementation

1. `features/conditions/authored_conditions_v1` (+ tests): decode (atomic; duplicate names rejected), `evaluate` (IDA rules; "" and "Invalid" are true; unknown name = error + logged once; unbound service = error), `dependencies`.
2. `features/conditions/condition_watch_v1`: watch per declared name; `quest_changed`/`event_changed` mark dirty; `flush` evaluates dirty names only and returns transitions; `prime` sets baselines with no transitions (bind, reload); `snapshot` for the probe aid. `world_from_quest_runtime` adapts the live runtime.
3. `quest_runtime`: `QuestRuntimeServicesV1::state_changed`, called once per applied transition in `set_state`. No polling.
4. `main.cpp` (anchors `P17 CONDITIONS`): population policy uses the evaluator (`activate` false = deferred when `--retain-hidden-actors`, otherwise excluded; `deactivate` true = excluded). `--condition-active`/`--condition-inactive` remain as debug overrides. The watch is primed at quest bind and reload, and flips are flushed per frame only when dirty.
5. `features/world_gates/world_gate_v1` (+ tests): bind (idempotent by stable ID), reconcile on bind (a persisted value that differs from the current condition gets one owner call), live flip apply (owner call plus persist). Owner is `WorldGateOwnerV1`; the door/decor runtime plugs in there.
6. Test aids (not gameplay): `--quest-debug-kill-property FRAME:PROPERTY:COUNT`, `--condition-probe FRAME`.

## Isolated tests

- `conditions_v1` (real tables from `levels-root/original-cache/data/pydata`): table size and names; truth table for IsAfter/IsBefore/IsBetween over fixed states; missing quest is false for both; Invalid/"" true; unknown name errors; unbound service errors; truncated input rejected atomically; with the real quest runtime, the Witch zone accept and property-435 kill flip exactly `IsAfter false->true` and `IsBefore true->false`; a repeated kill does not re-evaluate; save and reload do not replay the flip.
- `world_gate_v1` (AI tables from `windows-shared-assets`): first bind creates objects with no owner call; live flip reaches only the gate reading that condition and is persisted; equal restore replays nothing; differing persisted value gets one apply; object without the component gets its component; unavailable condition and stable ID 0 are reported and never bound.
- ctest (133 tests, build `-Jobs 6`): 131 pass. Failing: `session_skill_binding` (allowed by the brief) and `character_menu` (see Pre-existing failure).

## Integrated runtime verification (quiet hidden runs, Swamp `001_swamp`, real EXE)

Assets: `.local-inputs/claude-preview16/quests-run/assets` (rc4 plus the quest tables). Logs: `.local-inputs/claude-preview17/conditions-run/out/`.

- Clean run (`conditions-witch.log`): frame 30 probe `IsAfter_Swamp_KillWitch2=false`; Witch row 52 accepted at frame 60; property 435 kill at frame 120. Output: `Condition flip IsAfter_Swamp_KillWitch2 false->true cause=quest-event` and `Condition flip IsBefore_Swamp_KillWitch true->false`, once each. Gates: `TriggerObject unlock_cond=IsAfter_Swamp_KillWitch2 value=true`; `QuestMoveInZone _prim_WitchQuestStart activate_cond=IsBefore_Swamp_KillWitch value=false`. Save at frame 200; in-process reload at frame 220: `World gates bound=12 notices=0`, no owner call, no replay; probes at 225 and 280 are true.
- Restart (`conditions-restart.log`, `--fresh-player` loads the existing save): `Character ... xp=50`, `Quest runtime bound ... current=-1 cqpg=1676`, probe at frame 30 `IsAfter_Swamp_KillWitch2=true`, gate state restored `_prim_TriggerObject unlock_cond applied=true` and `_prim_WitchQuestStart activate_cond applied=false`, no owner call, no flip.
- Levels batch (the 39 map args of the P16 batch, same args including its `RENE_FOLLOW` override, rerun with this build; logs in `.local-inputs/claude-preview17/levels-batch/out/`): raw "Condition policy returned unknown" lines 255 (P16) to 93 (P17); unique declarations 232 to 89. Classification of the 89 by reading each declaration block: `auto_spawn="0"` 80, `spawn_prob` 6, authored name absent from the table 3.

## Gaps (not done)

1. Door and AnimatedDecor visual and physical state. The gate owner is a stub. Needs: the door's game-object row (`Swamp_Door_Ruins_FIRE`, Android `game_objects_pyarray.bin`) decoded into the worktree data; the model and open/close clips played through the container game-object visual path (`features/containers`); door collision through `canonical_door_v27` providers. Owner: P17 doors stream.
2. `Script_OpenDoor`/`CloseDoor` execution. `features/progression_barriers/source_door_commands` is not wired into the campaign runtime, and the TriggerObject unlock/use path is not implemented, so `open_SwampKingDoor` does not run. The lever's `unlock_cond` now evaluates and persists.
3. Waterfall. No witch-gated waterfall exists in the Swamp data (see Investigation). Nothing was implemented, and the reference video was not checked for it.
4. Live character activation. Population is evaluated at level load. A flip does not spawn or despawn characters; that needs the spawn owner (P16 SPAWN).
5. Live combat kill of the Bogwitch was not exercised. The witch kill used the property-435 test aid. The QUESTS report's open kill-producer mapping still applies.
6. The 3 authored names missing from the table are unresolved (data or source question).
7. The `auto_spawn="0"` and `spawn_prob` notices still use the generic text "Condition policy returned unknown" (ActorPopulation message). Only the text is misleading.
8. The levels comparison is by notice counts over the 39 map args (`base.args`, the 40th file, has no P16 job and produces no unknown notices; excluding it changes nothing), not a per-map verifier.

## Pre-existing failure (not caused by this work)

`character_menu` (ctest #88) fails on this branch and on base `261f2e49`: assertion "Tab overlap left part not answered by the nearer Quest Log" (P16 MAPFIX rule). `character_menu_tests` links only `foundation_data` and its own sources, and `git diff 261f2e49 HEAD -- port/windows-foundation/features/character_menu` is empty. The rule was superseded on `p16/integrate` by `b7862d6f` ("QUESTUI2 rule supersedes MAPFIX overlap rule"), which is not in base `261f2e49`. Merging current `p16/integrate` clears it. I did not merge other streams' commits here.

## Package files required

- Condition evaluation needs `original-cache/data/pydata/v2conditions_pyarray.bin`, `v2conditions_pyarraynames.bin`, `v2conditions_pystructnames.bin` (present in the worktree cache).
- It also needs the quest table `v2quests_pyarray.bin` and `v2quests_pyarraynames.bin` for the quest state. These are absent from the rc4 package and present only in `.local-inputs/claude-preview16/levels-root/original-cache/data/pydata/`. Without them the population policy has no quest state and every `activate_cond` returns an error (logged, actor unknown). The package must carry the quest tables.

## Placeholders

- Gate owner (`world_gates` owner callback in `main.cpp`, `worldGateOwner`): logs `owner=none (not wired)`; no visual or physical change.
- Test aids, not gameplay: `--quest-debug-kill-property`, `--condition-probe`, existing `--quest-debug-accept`/`--quest-debug-kill`.
- Debug overrides: `--condition-active`, `--condition-inactive`.

## Verifier script

1. Build and test: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p17conditions -Test -Jobs 6`. Expect exactly `character_menu` and `session_skill_binding` to fail.
2. Unit: `conditions_v1_tests <levels-root/original-cache/data/pydata>` and `world_gate_v1_tests <.local-inputs/windows-shared-assets>`.
3. Clean Witch run: delete `conditions-run/out/witch.save` and `witch-restart.save`, then `quiet_run.ps1 -JobsFile conditions-run/jobs.json -Parallel 1`. Expect the flip lines at frame 120 and `World gates bound=12 notices=0` after the reload at 220.
4. Restart: copy `out/witch.save` to `out/witch-restart.save`, then `quiet_run.ps1 -JobsFile conditions-run/jobs-restart.json -Parallel 1`. Expect `IsAfter_Swamp_KillWitch2=true` at frame 30, no flip, and `applied=true` for `_prim_TriggerObject`.
5. Levels: `quiet_run.ps1 -JobsFile levels-batch/jobs.json -Parallel 8`, then count `Condition policy returned unknown` lines in `levels-batch/out/*.log`.
