# P17INT report (Preview 17 integration)

Branch `p17/integrate` (worktree `DH_wt/p17int`, build `DH_wt/build-p17int/dh-foundation.exe`), built from `p16/integrate` `f4ea322d`. Not pushed; no other worktree touched.

## Commits (in order)

| Commit | What |
|---|---|
| `ad260eef` | Merge `p17/firewall` (conflict resolution, see below) |
| `13fad447` | Merge `p17/transitions2` |
| `698e4a48` | Merge `p17/procgen2` |
| `a3046e2c` | Test: one character save with every component present reloads exactly (schema 4) |
| `9083417b` | Exit zone gate honours `--condition-active` / `--condition-inactive` |
| `078dc5e8` | Trigger zones gated by `activate_cond` through the CONDITIONS evaluator; procedural exits show the prompt; schema_v4 temp-dir race fixed |

## Merge decisions

- **main.cpp (firewall) line endings.** `p17/firewall` committed `main.cpp` with CRLF; base and HEAD are LF. Git reported one 5,000-line conflict. I re-ran the 3-way merge against an LF-normalised copy: 15 real hunks, all resolved in the merge commit.
- **Condition evaluator.** OPENING2's `conditionActive` (named-condition lambda) is replaced by the CONDITIONS `AuthoredConditionsV1` evaluator for population, the flip watch and the exit gate (one evaluator). `quest_conditions_v1` stays as a module with its own tests, no longer used by main.
- **Checkpoint rule (combat_session).** Both rules kept: OPENING3 `set_lifecycle_serialized_by_host` admits the lifecycle; DOORS `lifecycleActivity` replaces the blanket permission-provider refusal (idle lifecycle admitted); `mark_lifecycle_baseline` kept.
- **Save format.** Schema stays 4. The component tail is always written (procgen2 tests require it); the reader accepts a missing tail (older v4 saves).
- **Level exits: Space, not E.** The exit prompt is confirmed by the Space context dispatcher (press edge, no object of interest present; NPCs, chests and levers keep priority; the attack is suppressed on that frame). The E key is no longer read. `--transition-key` and `--transition-confirm` remain as test hooks.
- **Exit gate.** `LoggedConditionStub` replaced by `FunctionConditionSource` over the evaluator and the live quest runtime (debug overrides honoured).
- **Trigger zones.** Zones with `activate_cond` are built; each frame a gated zone is in contact only while its condition is true. Before this, the host rejected them ("not evaluated by the host"), so `_prim_TriggerZone_001` (Door_Inspection_South) never fired.
- **Procedural exits.** The prompt now shows for procedural targets (witch cave, Swamp 2); the level is generated from the profile's saved seed.
- **Effects.** Embedded (scene-owned) FX goes through the one FX frame (`set_embedded_sources`). The level-up billboard was not re-checked for a duplicate path (gap).

## Verification (real EXE, quiet hidden runs, rc3 assets; procgen2 `asset-root3` for transition runs)

Run folder: `DH_sc/.local-inputs/claude-preview17/p17int-run/` (logs in each folder, frames in `png/`, `cap/`, `ref/`).

| Item | Result | Evidence |
|---|---|---|
| Build | pass | `p14_build.ps1 -Name p17int -Jobs 6` exit 0 |
| ctest | 146 of 147 pass; only `session_skill_binding` fails (allowed), three consecutive runs | `ctest -j 6` |
| Swamp opening plays | verified (frames) | `open/open.log` captions "The Boglands - Ancient Prison", "...Player!"; `png/open-520.png`, `png/open-1500.png` viewed |
| Fire door flip, forced `IsAfter_Swamp_KillWitch2` | verified (logs) | `[door] command ... _prim_SwampDoor_FIRE state=0->3`, `_prim_SwampDoor state=0->3`; FX `_prim_SwampDoor_FIRE attached`, `Source FX final packetFrames=240 presentationFailed=0` |
| Red door FX attached | verified (log) | `Embedded FX _prim_Swamp2Door_FIRE attached model=go_door_fire_red.bdae` |
| Swamp -> Darkwood -> Swamp via Space | verified (logs) | `round/round.log`: prompt shown frame 41, `Transition confirmed key=Space ... frame=60`, return confirmed (2 confirms, 2 session ends) |
| Swamp -> witch cave via Space (procedural) | verified (logs) | `witch/witch.log`: prompt "Bogwitch Cavern", `Transition confirmed key=Space`, seed 3241881308 generated and saved |
| Saved seed reused after reload | verified (log) | `witchreload/witchreload.log`: `Procedural level generated ... seed=3241881308` (same seed from the save) |
| Entry record | verified (log + unit test) | `Transition character loaded ... level=002_swamp_witchcave`; `save_all_components_v4` |
| All-components save | verified (unit test) | `save_all_components_v4_tests`: seed, entry, `dh2.world-gate.*`, `campaign_lifecycle_v1`, `dh2.container.hits.v1`, `dh2.source-container.objs.v1` reload byte-exact, re-save byte-exact, schema 4 |
| Cutscene from zone with condition | verified (log) | `door2/door2.log`: `_prim_TriggerZone_001 -> script Door_Inspection_South started` ... `finished`; `World gate state _prim_TriggerZone_001 activate_cond applied=true` (`IsBefore_Gothicus2Survivors`, present in the v2conditions table) |
| Space chest / barrel | verified (log) | `space-barrel/space-barrel.log`: `Context container ... status=accepted`, `Container opened ... loot=9` |
| Map tab | verified (log) | `map/map.log`: `Character menu Map selected frame=150`, `Map room visited` |
| Quest tab | exit 0 only | `quest/quest.log` not inspected for the tab |

Frame comparison, orange fire (coordinator item): `ref/sheet-red-door.png`. Top: local Part 1 video at 610, 616, 622 s (Celeste dialogue, orange fire wall). Bottom: our door2 run at frames 30, 120, 200 (camera at the gate, player at -12137,10444). The red door's flames are **not visible** in our frames; the reference orange wall is not reproduced.

## Not verified / open

1. **Walk-over pickup and level-up placeholder on the merged EXE: not verified.** The Preview 16 recipes (`walk`, `levelup150`) run on `build-p16int` show the target, pickup (`reason=walkover`) and `Level up presentation frame=143`. On the merged EXE the same args produce no kill, no drop, no level-up. Cause seen so far: population differs (population RNG calls 21 vs 20; visuals 35 vs 34), consistent with the CONDITIONS evaluator resolving declarations that were unknown before. Not established as a bug; it needs a recipe update or a check that the new population is intended.
2. **Save/reload with doors in one run: not verified.** Save and restore are refused while a cutscene runs. With the campaign triggers on, `enterLocation_MerchantCamp` (fed at the gate position) aborts on unsupported `Script_EnterSafeZone` (kind 16) and leaves cutscene mode on. The Preview 16 EXE aborts the same script at frame 2 (pre-existing, not from this merge). Door, container and entry records are covered only piecemeal: the unit test, the transition logs, and the door logs.
3. **Red door flames in the gameplay camera.** Not captured; the flames drawn at the door are not visible at the camera positions tried (door2, door3; door3 fell into a scripted troll sequence). `Door_Inspection_South` has no door command, so the red door does not flip in that cutscene.
4. **Orange wall of fire (7:59 reference).** Not reproduced. The identification from `FIREWALL-report` (`_prim_SwampDoor_FIRE` is violet) is unchanged; FIRE2's red-door candidate (`_prim_Swamp2Door_FIRE`) is consistent with the 610-622 s frames but not shown on screen.
5. **Level-up single path.** Not checked for a duplicate FX path.
6. **Quest tab frame** not inspected (exit 0 only).
7. **Unsupported command.** `Script_EnterSafeZone` (kind 16) has no owner: cutscenes that use it abort and leave cutscene mode on (see 2).

## Placeholders

- `--transition-key` / `--transition-confirm`: test hooks (not gameplay).
- Condition overrides `--condition-active/--condition-inactive`: debug aids; they now also apply to the exit gate and trigger zones.

## Package files required

rc3 package (`windows-source-clock-v19-preview-15-rc3`) for the opening, door, Space, Quest and Map runs. The transition and witch runs use `DH_sc/.local-inputs/claude-preview17/procgen2/asset-root3` (overlay with the Swamp module declarations, including `deadend_brdwalk_w_00.mgp` with the witch exit).

## Verifier script

1. `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p17int -Jobs 6`, then ctest with the llvm-mingw bin on PATH: only `session_skill_binding` fails.
2. Quiet runs: `port/windows-foundation/tools/quiet_run.ps1 -JobsFile <run>/jobs-b.json -Parallel 5` (opening, door, save, round, witch); `jobs-c.json` (round, witch, door2); `jobs-d.json` (witchreload, after witch).
3. Grep: `Transition confirmed key=Space`, `Procedural level generated ... seed=3241881308`, `_prim_TriggerZone_001 -> script Door_Inspection_South started`, `state=0->3`.
