# Preview 14 survey: Stats + Skills point attribution, level-up grants

Stream: Stats/Skills pages (user report: skill and stat point attribution after a level-up does not work and has no effect on the character).
Scope: read-only survey of `port/`; scratch under `.local-inputs/claude-preview14/skills/`. No file under `port/`, `docs/`, or `tools/` was edited.

## Summary (read this first)

1. **Stat +/- buttons are unreachable in the live EXE.** `SourceCompositionV1::register_stat_training` (`port/windows-foundation/features/character_menu/source_composition.hpp:178`) has no caller anywhere in `port/windows-foundation`. Without it, every click on the Stats tab hits `source_composition.hpp:306-307` and fails with "Stats training buttons have no original source hit resolver". **Reproduced in a quiet run** (section F). The stat assignment owner (`CharacterMenuActionsOwnerV1::assign_stat`, `character_menu_stats_owner_v1.cpp`) is compiled into `level-world` but is never constructed by `main.cpp`. So a stat point can never be spent in the EXE. This is the main cause of the "stat points have no effect" report.
2. **Skill training is wired** (`main.cpp:1935-1956` services.train, `:1960-1990` details bindings, `runtime_skills_menu_v1.cpp:251-263` Upgrade gate). It debits property 157 and the CharacterState points and increments the saved rank. The rank reaches casts through CharacterState. Whether the Upgrade click works end to end in the EXE was **not** verified in this survey (no profile with points>0 and a stable slot map was built; see section F).
3. **The "1 point" symptom is a load-time starter grant.** When a save has `source_skill_slots_known == false`, `main.cpp:1913-1926` initializes the source rows and debits one point for saved row 0 (the native initial-slot grant). A 1-point profile therefore shows **0 points and no Upgrade button**. Reproduced in the existing `knight-p1` log and again in two quiet runs (`Direct player source points ... skill=1` followed by `Direct source Skills rows=16 firstRank=1 points=0`). Whether this is correct for legacy saves is an open question (section G).
4. **Level-up grants exist, but the live level-up has no presentation and no verified point grant.** The live path is `features/loot/runtime_death_rewards_v1.cpp:133-190` `level_up()`. It recalculates the class sheet (which, per `coordination/luna-act1/lane-12-handoff.md:15`, is where stat/skill points are awarded) and then `sync_character` copies properties 148/157 into CharacterState. No MENU_LEVEL_UP status, no `anim_levelup`, no FX, no tutorial flag is called from `windows-foundation` (grep: `enqueue_level_up`, `authored_progression_hud`, `level_presentation` have zero call sites there). No live level-up has been observed in the EXE (B038 report).
5. **Per-level point amounts, per-difficulty tables, and the Stats page's own HP/damage effect are not yet extracted or verified.** Difficulty is hard-coded to Normal in `main.cpp` (`unlocked_difficulty=0`).

## A. EXISTING

| Item | File | Purpose | Status |
|---|---|---|---|
| Level-up (live) | `port/windows-foundation/features/loot/runtime_death_rewards_v1.cpp` `level_up()` (~133-190), `award_player_xp()` (~195-240), `sync_character()` (~50-70) | Kill XP to property 33/34, one-level carry, class recalc, vital refill, publish to CharacterState | integrated in `main.cpp:2823-2825`; verified only in isolated tests; live level-up not observed |
| Level-up (level-world copy) | `port/level-world/player_progression_v1.cpp` `level_up()` (35-62) | Same algorithm with a `level_presentation` callback (required, MENU_LEVEL_UP/FX/tutorial/achievement) | compiled into `windows-foundation/CMakeLists.txt:599`; presentation provider **not bound** in live path |
| Level-up HUD | `port/engine-ui/authored_progression_hud_v23.cpp` (sprite 162, 30 frames, `anim_levelup`) | Portrait level-up animation | source-only (no live caller) |
| Level-up status text | `port/engine-ui/menu_status_messages_v26.cpp` `enqueue_level_up` | MENU_LEVEL_UP queue | component-tested; no live caller |
| Stat assignment (portable) | `port/engine-ui/character_menu_stats_owner_v1.cpp` `character_menu_assign_stat_v1` | Spend property 148 to 149+stat, reset base to actor row, class recalc | component-tested (`engine-ui/tests/character_menu_stats_owner_v1.cpp`); **not constructed by main.cpp** |
| Stat actions dispatch | `port/engine-ui/character_menu_actions_owner_v1.cpp:84-89` `assign_stat`; `character_menu_queries_owner_v1.cpp:277` `NativeStatsAssignPoint` | Query/dispatch for authored callbacks | component-tested; not in EXE |
| Stat seam | `port/windows-foundation/features/character_menu/source_composition.hpp:178-209, 300-321`; `SOURCE_COMPOSITION.md` "Stat training route" | Stat hit resolver + same-owner training route | header-only; `register_stat_training` **no callers** (live: missing) |
| Stats page projection | `features/character_menu/character_menu.cpp:50,83-88` (Stat_Points 148 chooses art), `menu_stats.cpp` | Draw stats, +/- art visibility | integrated (display only) |
| Skills page | `features/generic_skills/runtime_skills_menu_v1.cpp` (points gate 251-263, Lock/Grey, Upgrade), `generic_skills_page_v1.cpp:103-104` (points from `state_->source_skill_points`) | Skills page view | integrated in `main.cpp:1994-1996`; B020 open (see its doc) |
| Skill training (same-Session) | `features/generic_skills/runtime_skill_session_training_v1.cpp` (commit ~290-295), `runtime_skill_progression_v1.cpp` (can_train 116-155) | Probe + commit: debit 157, saved row +1, recalc, staged publish | isolated tests (Knight, Rogue, Mage) PASS; integrated via `main.cpp:1935-1956` |
| Starter/initial grant | `port/level-world/player_initial_grants_v2.cpp`; `features/frontend/creation/runtime_creation_persistence_v1.cpp:311-360` | Fresh character: row0 grant, subtracted from property 157 at creation | component-tested |
| Direct bootstrap (test path) | `main.cpp:732-740` (fresh-player rows), `:1258-1262` (points read from Session), `:1913-1926` (first-grant debit) | Init rows + points for direct runs | integrated; source of the 1-point effect |
| Save fields | `save_store.cpp:117-118, 171-172` (stat/skill points + known flag); `game_save.cpp:72`; `player_profile_properties.cpp:68-70, 81-82` | Persist points, publish to Session on load | integrated, tested by roundtrip |
| Skill caps | `main.cpp:1906-1907` (`unlocked_difficulty=0`, "current gameplay start accepts Normal only"); `CharacterDesign` `MaxSkillLevelB/C/DNormal/Hard/VeryHard` via `engine-ui/character_menu_actions_owner_v1.cpp:142` | Per-difficulty max rank | Normal only |
| Fixture helper | `.local-inputs/claude-preview13/c-skills/tools/set_points.cpp` / `.exe` | Clone a save and set `source_skill_points` | **does not launch here** (exit -1073741515, missing `api-ms-win-crt-environment-l1-1-0.dll`); not fixed |

Status vocabulary: "integrated" means the code is reachable from `main.cpp`; "verified in EXE" means a log/capture from a run shows it.

## B. ORIGINAL BEHAVIOUR

### B1. Stat buttons (IDA-recovered SWF, `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`)
- Strength/Dexterity/Endurance/Energy buttons are `btn_train_strength` / `btn_train_dexterity` / `btn_train_endurance` / `btn_train_energy` (decl around `0x130..`, text lines 3859-3914). Each button's `onRelease` is at `0x149de` for Dexterity (the `function2` at `000149de`); the other three follow the same shape.
- Sequence in the Dexterity `onRelease` (lines 5600-5700):
  1. Guard: only if `_root.AddedStatsThisTurn == false` (`000149e9-000149f9`).
  2. If `menu_CharacterMenu.useSkillPoint == false`, call `NativeSaveGame` (`00014a1f`, trace "SAVING THE GODDAMN GAME!!!"). Skill-spend earlier in the same visit skips the save.
  3. Increment the local `StartingDex` display value, set `AddedStatsThisTurn = true` (`00014a45`).
  4. `NativeStatsAssignPoint(1)` (`00014a4f`; 0..3 = Str/Dex/End/Nrg per `SOURCE_COMPOSITION.md`).
  5. `MenuSpending` sound (`NativePlaySoundFX`, `00014a69`).
  6. Re-read `NativeGetPlayerStats` into a fresh object, set `StatsPointsLeft` and the text from `Stat_Points` (property 148), then show the glow while Stat_Points > 0, else `gotoAndStop("Idle")` (`00014af..` onward).
- Observation: the SWF gate is per visit (`AddedStatsThisTurn`), and the save happens only on the first spend in a visit. The port mirrors this with `menuUsedSkillPoint` (`main.cpp:1947`), but the stat path never reaches the code.
- Inference: `NativeStatsAssignPoint` is `Character::IncStat*` (per `SOURCE_COMPOSITION.md`), which decrements property 148 and increments 149+stat. The portable owner (`character_menu_stats_owner_v1.cpp:9-22`) does exactly that, then resets base and recalcs.

### B2. Skill training (Skills page)
- `presetAllSkills` (`0x20b2d`) and the selected-row `onUp` (`0x20e9a-0x210a1`): `NativeSkillsTrainSkill(true, position, 0)` decides the Add Skill / Upgrade Skill visibility (`README-runtime_skills_text_v1.md`, `B020-evidence-20261010.md`).
- Recovered `IncSkill` gate order (README-runtime_skill_session_training_v1): saved rows present, property 157 > 0, `SkillTable.RequiredLevel <= level`, rank < CharacterDesign cap, `CanIncrementSkill`; on accept, debit 157 by 1, increment the saved row, recalc, store property 194 potion capacity.
- Visible state: "Skill points left" (property 157), Lock at `level < RequiredLevel`, Grey at rank 0 (B020). Reference frames: `.local-inputs/fidelity-video-v18/reference-396.png` (Headsplitter rank 1) and `reference-399.png` (Inner Strength, "Unlocked at level 3"). These are stills; they do not show the Upgrade animation.

### B3. Level-up grants and presentation
- XP, level, carry, and vitals: property 33/34 (fixed-point x256), 19 level, 36/38 HP and 41/43 MP. `LevelUp` adds one level (property 19 +256), clears XP, reloads the CharacterTable base row, runs class recalc, refills HP/MP to the new maxima, carries bounded XP, then calls `SG_Save` and the level presentation (`player_progression_v1.cpp:35-62`; `coordination/luna-act1/lane-12-handoff.md:15-16`).
- Stat/skill points per level come from the **class formulas during base recalc** (lane-12 handoff). Exact per-level amounts for Knight/Rogue/Mage have **not** been extracted in this survey.
- Presentation (IDA `LevelUp` `3beb88`, 62 direct calls per `port/level-world/reference/ranged-attack-v41/progression-bindings-v44.md`): localized MENU_LEVEL_UP status, `PlayerLevelUp`, VisualFX event 87, level-2 tutorial flag, level-12 `IsSpecTime`, level-up portrait animation (sprite 162, 30 frames), and XP text.
- Video: `coordination/claude-preview13/F-report.md` (previous worker) observed a level-up tutorial overlay ("When you gain a level..." / "To assign the points...") at 376-384 s. `B038-report.md` observed no level-up in 355-376 s. I did not re-open the video in this survey, so these are the prior observations, not new ones.

### B4. Difficulty
- Skill rank cap per difficulty: CharacterDesign `MaxSkillLevelBNormal`, `CHard`, `DVeryHard`, indexed by the unlocked difficulty (`engine-ui/character_menu_actions_owner_v1.cpp:142`). Unlocked difficulty is a campaign/save value; the port hard-codes 0.

## C. GAPS (with evidence)

| # | Gap | Evidence |
|---|---|---|
| G1 | Stat +/- route is unreachable: no `register_stat_training` call, no `CharacterMenuActionsOwnerV1` in main.cpp | grep: `register_stat_training` only defined at `source_composition.hpp:178`; live run `t1` log: `Character menu action diagnostic: Stats training buttons have no original source hit resolver` (x2 for down/up) |
| G2 | Even if wired, `character_menu_assign_stat_v1` does not write `CharacterState.source_stat_points` or save | `character_menu_stats_owner_v1.cpp:9-22` only touches the property graph; `save_store` persists `source_stat_points` only from CharacterState |
| G3 | Stat assignment does not refresh the Stats page or live actor HP/damage through the same path as level-up (no `sync_character` analog) | Only `class_recalc` + `debug_load/query` in the owner; no `update_combat_properties` / sync |
| G4 | Live level-up has no presentation (no status message, portrait anim, FX, tutorial flag) | grep in `windows-foundation`: zero calls to `enqueue_level_up`, `authored_progression_hud`, `level_presentation` |
| G5 | Two level-up implementations exist (loot `runtime_death_rewards_v1.cpp` live; `level-world/player_progression_v1.cpp` with presentation contract) | Both compiled; they differ on the presentation branch; risk of drift |
| G6 | 1-point profile: starter grant debits the only point on unknown-slot saves; page then shows 0 points and no Upgrade | `knight-p1/out/skills-p1.log` and runs `s1`/`t1` (`skill=1` then `points=0`); `main.cpp:1913-1926` |
| G7 | Skill spends after the first in a menu visit are not saved until a later save point; app exit in that window could lose them | `main.cpp:1947` `if(!menuUsedSkillPoint&&save)`; no evidence the close path saves points (not verified) |
| G8 | Difficulty fixed to Normal; the B/C/D rank caps are not selected from save/campaign | `main.cpp:1906-1907` `unlocked_difficulty=0` |
| G9 | Skills Upgrade in the EXE (click to rank+1, points-1, cast uses new rank) not verified | No EXE click run yet (section F); B020 matrix still pending (`B020-evidence-20261010.md`) |
| G10 | Stat hit contours for the four btn_train_* buttons are not in the port (no bounds) | `source_layout.json` lists the btn_train_* paths only as excluded art; no hit bounds |
| G11 | Per-level stat/skill point amounts by class not extracted | Only the mechanism is documented (lane-12 handoff) |
| G12 | HP/MP and damage effects of stat points and skill ranks on the live actor not verified | No test run; Stat path not wired (G1) |

## D. DESIGN (minimal reusable implementation)

### D1. Wire the Stats page (fixes G1, G2, G3, G10)
- Data model: reuse `CharacterState.source_stat_points` / `source_skill_points` and the Session property 148/157. No new save field.
- New small owner: `features/character_menu/stat_training_v1.{hpp,cpp}` (or extend `character_menu_stats_owner_v1`) that, for stat index 0..3:
  1. checks `CharacterState.source_stat_points > 0` and that the Session property 148 agrees (same owner check as `character_menu_assign_stat_v1`);
  2. calls `character_menu_assign_stat_v1` (existing) on the live equipment property graph;
  3. copies resolved 148 into `CharacterState.source_stat_points`, and resolved 149..152 into `CharacterState.stats` if those are mirrored (check `sync_character`);
  4. calls `world.update_combat_properties` for the player (same as level-up) so HP/MP/damage recompute;
  5. saves on the first spend per visit (mirror `menuUsedSkillPoint`), per SWF `NativeSaveGame` order.
- Hook 1: `main.cpp` near the composition setup (`~1802-1804`): construct the stats graph with the same owner token as `characterMenuComposition` (`SourceCompositionV1(sharedCharacter)`). Hook 2: `main.cpp` near the skills registration (`~1994-1996`): call `characterMenuComposition->register_stat_training(owner, actions, stat_button_resolver, pre, post, error)`. `stat_button_resolver` needs the four btn_train_* hit contours (G10). Hook 3: nothing else; the release at `main.cpp:~2404-2407` already forwards to `release()`.
- CMake: `level-world/CMakeLists.txt:230-231, 250` already compiles the two engine-ui owners. Add the new owner beside them (`windows-foundation/CMakeLists.txt` if built there).

### D2. Level-up feedback (fixes G4, G5)
- One implementation: make `runtime_death_rewards_v1.cpp level_up()` the live owner and add a `level_presentation` hook that calls `menu_status_messages_v26::enqueue_level_up` (MENU_LEVEL_UP via `NativeGetStringFromSymbol`), `authored_progression_hud_v23` (sprite 162 `anim_levelup`), VisualFX event 87, and the level-2 tutorial flag. Keep `player_progression_v1` as the shared library function and pass the presentation as a callback (its contract already exists).
- Hook: `runtime_death_rewards_v1.cpp` after the successful `level_up()` (~`award_player_xp`), and `main.cpp:2823-2825` (already calls `deathRewards.after_update`).

### D3. Level-up grant table and difficulty (fixes G8, G11)
- Measure, do not guess: call the existing `dh2_class_recalc_base` for Knight/Rogue/Mage at levels 1..N (same as `level_up`), log the property 148/157 deltas per level, and store the table as data with its source row reference. Per-difficulty: read `MaxSkillLevelB/C/DNormal/Hard/VeryHard` through the existing `constant()` query and select by the save's unlocked difficulty (new save field only if one does not already exist; otherwise default 0, never break old saves).

### D4. Starter grant on load (fixes G6)
- Do not debit a point on load for a save that already has any saved skill row (the source grant is at creation only). Only run the native row-0 grant on fresh creation (`runtime_creation_persistence_v1.cpp:357-360` already subtracts it). Legacy saves: publish their saved points unchanged; mark slots known without debit. Open question G-Q1 below.

### D5. Persistence (fixes G7)
- Save at each successful spend, or when the Stats/Skills page closes, using the existing `save_character` (no format change; fields already exist in `save_store.cpp:117-118`). Keep the one-save-per-visit prefix only as the SWF's `NativeSaveGame` order, not as the only save.

Save-format impact: none for D1, D2, D4, D5. D3 adds at most one optional difficulty field; it must default to 0 when absent so existing saves load unchanged.

## E. WORK BREAKDOWN

1. **S (<2 h) - Live stat route check and fixture.** Build a clean fixture with stat points>0 and a known slot map (fix or replace `set_points.exe`: run it from an environment with the CRT DLL on PATH, or patch the points fields at their offsets in `save_store.cpp:117-118`). Verifier: quiet batch with `--profile-click-frame` + `--menu-release` on the Stats tab; expect the diagnostic before the fix and the spend log after it. Depends on nothing.
2. **M (<half day) - Wire the stat spend (D1).** Hooks in `main.cpp` (2 sites), owner change, four hit contours. Test: `engine-ui` isolated test plus quiet batch: points 3 -> click Strength x2 -> log `Stat_Points 3->1`, `Strength +2`, `CharacterState.source_stat_points == 1`, save roundtrip, HP/MP recalc unchanged for non-vital stats. Visual: capture Stats page before/after (LOOK at PNG). Verify the first-spend save order (`NativeSaveGame` before increment).
3. **M - Skills Upgrade end to end (G9, G6, G7).** Fix the starter debit (D4); add save-on-spend (D5). Test: fresh isolated profiles at points 0, 1, 3 (B020 matrix L1-L4): verify the Upgrade button, the rank change, cast damage using the new rank, and save reload. Verifier: quiet batch with skill page frame + release on the row and Add Skill (coordinates from SWF art, to be measured).
4. **M - Level-up feedback (D2) and grant table (D3).** Bind presentation in the live death-reward path; table extraction by recalc. Test: force a level-up with the existing debug switch `OneKillLevelUp` (`runtime_death_rewards_v1.cpp:209-213`) in a quiet batch: expect MENU_LEVEL_UP status, `anim_levelup`, points +X per the table, HP/MP refill. Capture the portrait frame.
5. **S - Difficulty selection (D3 part).** Select caps from save; default Normal.

Order: 1 -> 2 -> 3 -> 4 -> 5. Tasks 2 and 3 touch the same `main.cpp` block (lines ~1800-2000): one owner.

## F. VERIFICATION DONE IN THIS SURVEY (quiet, hidden, parallel)

- Runner: `port/windows-foundation/tools/quiet_run.ps1 -JobsFile jobs.json -Parallel 2` with `dh-foundation.exe` from `.local-inputs/windows-source-clock-v19-preview-13-rc2`. Assets rewritten from the preview-12 candidate path (missing) to the preview-13-rc2 `assets`/`audio-assets`; nothing else changed. Jobs: `.local-inputs/claude-preview14/skills/jobs.json`, summary `summary.json`.
- Job `s1-skills` (knight-p1 profile, skills page frame 120): exit 0 in 6.9 s. Log: `Direct player source points ... stat=0 skill=1`, `Direct source Skills rows=16 firstRank=1 points=0`, `Character menu Skills selected frame=120`.
- Job `t1-stats-click` (same profile, profile click frame 60, stats-tab release at authored 240:160 twice down/up): exit 0. Log: `Character menu opened frame=60 via profile input`, then **`Character menu action diagnostic: Stats training buttons have no original source hit resolver`** (x2). This proves G1 at runtime, independent of the exact button coordinates (any non-nav click on the Stats tab takes that path).
- Not run (time): captures were not converted to PNG and inspected; no Skills Upgrade click; no stat-point profile (this fixture has stat=0, so the Stats page has nothing to spend); no level-up.

## G. OPEN QUESTIONS (cannot decide from evidence)

- G-Q1: For a legacy save (`source_skill_slots_known == false`) that already has skill rows spent, is the row-0 starter debit correct, or is it a double debit? Original source: `player_initial_grants_v2.cpp` grants row 0 only when its level is zero. Need an original-save fixture to decide.
- G-Q2: Exact per-level stat and skill point amounts per class and difficulty (class formulas). Needs the recalc table extraction (E4).
- G-Q3: Do the four btn_train_* hit contours and the Add Skill / Upgrade button bounds come from the SWF shapes (`dqcharmenu_droid.swf`, `menu_CharacterSheetNew/Strength/btn_train_strength`) and does the port need a new hit-table export? Need the export tool (`export_art.py`) output.
- G-Q4: Does the app-exit / menu-close path already save spent points (G7)? Needs a run that spends two points, then closes without the first-spend save path.
- G-Q5: Reference video Part 2 Stats/Skills pages (sheets2 + ffmpeg) were not re-extracted here. Needed for the Stats +/- glow/grey timing and the level-up presentation timing (only the F-report observation at 376-384 s exists).
